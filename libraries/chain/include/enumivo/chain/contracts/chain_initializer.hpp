/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#pragma once

#include <enumivo/chain/contracts/genesis_state.hpp>
#include <enumivo/chain/contracts/types.hpp>
#include <enumivo/chain/chain_controller.hpp>

namespace enumivo { namespace chain {  namespace contracts {

   class chain_initializer 
   {

      public:
         chain_initializer(const genesis_state_type& genesis) : genesis(genesis) {}

         time_point              get_chain_start_time();
         chain::chain_config     get_chain_start_configuration();
         producer_schedule_type  get_chain_start_producers();

         void register_types(chain::chain_controller& chain, chainbase::database& db);

         void prepare_database(chain::chain_controller& chain, chainbase::database& db);

         static abi_def eos_contract_abi(const abi_def& enumivo_system_abi);

      private:
         genesis_state_type genesis;
   };

} } } // namespace enumivo::chain::contracts

