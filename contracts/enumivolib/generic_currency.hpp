#pragma once
#include <enumivolib/multi_index.hpp>
#include <enumivolib/token.hpp>
#include <enumivolib/asset.hpp>
#include <enumivolib/dispatcher.hpp>
#include <enumivolib/serialize.hpp>
#include <enumivolib/action.hpp>
#include <string>

#define N(X) ::enumivo::string_to_name(#X)

namespace enumivo {
   using std::string;

   template<typename Token>
   class generic_currency {
      public:
          typedef Token token_type;
          static const uint64_t code                = token_type::code;
          static const uint64_t symbol              = token_type::symbol;
          static const uint64_t accounts_table_name = N(account);
          static const uint64_t stats_table_name    = N(stat);

          ACTION( code, issue ) {
             typedef action_meta<code,N(issue)> meta;
             
             issue() { }
             issue(account_name a, asset q): to(a), quantity(q) { } 
             account_name to;
             asset        quantity;

             ENULIB_SERIALIZE( issue, (to)(quantity) )
          };

          ACTION( code, transfer ) {
             transfer(){}
             transfer( account_name f, account_name t, token_type q ):from(f),to(t),quantity(q){}

             account_name from;
             account_name to;
             asset        quantity;

             template<typename DataStream>
             friend DataStream& operator << ( DataStream& ds, const transfer& t ){
                return ds << t.from << t.to << t.quantity;
             }
             template<typename DataStream>
             friend DataStream& operator >> ( DataStream& ds, transfer& t ){
                ds >> t.from >> t.to >> t.quantity;
                eosio_assert( t.quantity.symbol== token_type::symbol, "unexpected asset type" );
                return ds;
             }
          };

          struct transfer_memo : public transfer {
             transfer_memo(){}
             transfer_memo( account_name f, account_name t, token_type q, string m )
             :transfer( f, t, q ), memo( std::move(m) ){}

             string       memo;

             ENULIB_SERIALIZE_DERIVED( transfer_memo, transfer, (memo) )
          };

          struct account {
             token_type balance;
             uint64_t   symbol = token_type::symbol;

             auto primary_key() const { return symbol; }

             ENULIB_SERIALIZE( account, (balance)(symbol) )
          };

          struct currency_stats {
             token_type supply;
             uint64_t   symbol = token_type::symbol;

             auto primary_key() const { return symbol; }

             ENULIB_SERIALIZE( currency_stats, (supply)(symbol) )
          };

          /**
           *  Each user stores their balance in the singleton table under the
           *  scope of their account name.
           */
          typedef enumivo::multi_index<accounts_table_name, account> accounts;
          typedef enumivo::multi_index<stats_table_name, currency_stats> stats;

          static token_type get_balance( account_name owner ) {
             accounts t( code, owner );
             auto itr = t.find( symbol );
             return itr != t.end() ? itr->balance : token_type( asset(0, symbol) );
          }

         static void set_balance( account_name owner, token_type balance, account_name create_bill_to, account_name update_bill_to ) {
             accounts t( code, owner );
             auto f = [&](account& acc) {
                acc.symbol = symbol;
                acc.balance = balance;
             };
             auto itr = t.find( symbol );
             if( itr != t.end() ) {
                t.modify( itr, update_bill_to, f);
             } else {
                t.emplace( create_bill_to, f);
             }
          }

          static void on( const issue& act ) {
             require_auth( code );

             stats t( code, code );
             auto itr = t.find( symbol );
             if( itr != t.end() ) {
                t.modify(itr, 0, [&](currency_stats& s) { s.supply += act.quantity; });
             } else {
                t.emplace(code, [&](currency_stats& s) { s.supply = act.quantity; });
             }

             set_balance( code, get_balance( code ) + token_type(act.quantity), code, 0 );

             inline_transfer( code, act.to, act.quantity );
          }


          static void on( const transfer& act ) {
             require_auth( act.from );
             require_recipient(act.to,act.from);

             set_balance( act.from, get_balance( act.from ) - token_type(act.quantity), act.from, act.from );
             set_balance( act.to, get_balance( act.to ) + token_type(act.quantity), act.from, 0 );
          }

          static void inline_transfer( account_name from, account_name to, token_type quantity,
                                       string memo = string() )
          {
             action act( permission_level(from,N(active)), transfer_memo( from, to, asset(quantity), move(memo) ));
             act.send();
          }

         static void inline_issue(account_name to, token_type quantity)
         {
            action act(permission_level(code, N(active)), issue(to, asset(quantity)));
            act.send();
         }

          static token_type get_total_supply() {
             stats t( code, code );
             auto ptr = t.find( symbol );
             return ptr != t.end() ? ptr->supply : token_type(0);
          }

         static void apply( account_name c, action_name act) {
            enumivo::dispatch<generic_currency, transfer, issue>(c,act);
         }
   };

} /// namespace enumivo
