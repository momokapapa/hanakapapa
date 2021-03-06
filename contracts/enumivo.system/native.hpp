/**
 *  @file
 *  @copyright defined in enumivo/LICENSE.txt
 */
#pragma once

#include <enumivolib/action.hpp>
#include <enumivolib/public_key.hpp>
#include <enumivolib/types.hpp>

namespace enumivosystem {
   using enumivo::permission_level;
   using enumivo::public_key;

   typedef std::vector<char> bytes;

   struct permission_level_weight {
      permission_level  permission;
      weight_type       weight;

      // explicit serialization macro is not necessary, used here only to improve compilation time
      ENULIB_SERIALIZE( permission_level_weight, (permission)(weight) )
   };

   struct key_weight {
      public_key   key;
      weight_type  weight;

      // explicit serialization macro is not necessary, used here only to improve compilation time
      ENULIB_SERIALIZE( key_weight, (key)(weight) )
   };

   struct authority {
      uint32_t                              threshold;
      std::vector<key_weight>               keys;
      std::vector<permission_level_weight>  accounts;

      // explicit serialization macro is not necessary, used here only to improve compilation time
      ENULIB_SERIALIZE( authority, (threshold)(keys)(accounts) )
   };

   /*
    * Empty handlers for native messages.
    * Method parameters commented out to prevent generation of code that parses input data.
    */
   class native {
      public:

      void newaccount( /*account_name     creator,
                              account_name     name,
                              const authority& owner,
                              const authority& active,
                              const authority& recovery*/ ) {}

      void updateauth( /*account_name     account,
                              permission_name  permission,
                              permission_name  parent,
                              const authority& data*/ ) {}

      void deleteauth( /*account_name account, permission_name permission*/ ) {}

      void linkauth( /*account_name    account,
                            account_name    code,
                            action_name     type,
                            permission_name requirement*/ ) {}

      void unlinkauth( /*account_name account,
                              account_name code,
                              action_name  type*/ ) {}

      void postrecovery( /*account_name       account,
                                const authority&   data,
                                const std::string& memo*/ ) {}

      void passrecovery( /*account_name account*/ ) {}

      void vetorecovery( /*account_name account*/ ) {}

      void onerror( /*const bytes&*/ ) {}

      void canceldelay( /*permission_level canceling_auth, transaction_id_type trx_id*/ ) {}

   };
}
