/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#include <algorithm>
#include <vector>
#include <iterator>
#include <boost/test/unit_test.hpp>

#include <enumivo/chain/chain_controller.hpp>
#include <enumivo/chain/exceptions.hpp>
#include <enumivo/chain/permission_object.hpp>

#include <enumivo/testing/tester.hpp>

#include <enumivo/utilities/tempdir.hpp>

#include <fc/crypto/digest.hpp>

#include <boost/test/unit_test.hpp>
#include <boost/range/algorithm/find.hpp>
#include <boost/range/algorithm/find_if.hpp>
#include <boost/range/algorithm/permutation.hpp>

using namespace enumivo;
using namespace chain;
using tester = enumivo::testing::tester;

BOOST_AUTO_TEST_SUITE(special_account_tests)

//Check special accounts exits in genesis
BOOST_FIXTURE_TEST_CASE(accounts_exists, tester)
{ try {

      tester test;
      chain::chain_controller *control = test.control.get();
      chain::database &chain1_db = control->get_mutable_database();

      auto nobody = chain1_db.find<account_object, by_name>(config::nobody_account_name);
      BOOST_CHECK(nobody != nullptr);
      const auto& nobody_active_authority = chain1_db.get<permission_object, by_owner>(boost::make_tuple(config::nobody_account_name, config::active_name));
      BOOST_CHECK_EQUAL(nobody_active_authority.auth.threshold, 0);
      BOOST_CHECK_EQUAL(nobody_active_authority.auth.accounts.size(), 0);
      BOOST_CHECK_EQUAL(nobody_active_authority.auth.keys.size(), 0);

      const auto& nobody_owner_authority = chain1_db.get<permission_object, by_owner>(boost::make_tuple(config::nobody_account_name, config::owner_name));
      BOOST_CHECK_EQUAL(nobody_owner_authority.auth.threshold, 0);
      BOOST_CHECK_EQUAL(nobody_owner_authority.auth.accounts.size(), 0);
      BOOST_CHECK_EQUAL(nobody_owner_authority.auth.keys.size(), 0);

      auto producers = chain1_db.find<account_object, by_name>(config::producers_account_name);
      BOOST_CHECK(producers != nullptr);

      auto& gpo = chain1_db.get<global_property_object>();

      const auto& producers_active_authority = chain1_db.get<permission_object, by_owner>(boost::make_tuple(config::producers_account_name, config::active_name));
      auto expected_threshold = ENU_PERCENT_CEIL(gpo.active_producers.producers.size(), config::producers_authority_threshold_pct);
      BOOST_CHECK_EQUAL(producers_active_authority.auth.threshold, expected_threshold);
      BOOST_CHECK_EQUAL(producers_active_authority.auth.accounts.size(), gpo.active_producers.producers.size());
      BOOST_CHECK_EQUAL(producers_active_authority.auth.keys.size(), 0);

      std::vector<account_name> active_auth;
      for(auto& apw : producers_active_authority.auth.accounts) {
         active_auth.emplace_back(apw.permission.actor);
      }

      std::vector<account_name> diff;
      for (int i = 0; i < std::max(active_auth.size(), gpo.active_producers.producers.size()); ++i) {
         account_name n1 = i < active_auth.size() ? active_auth[i] : (account_name)0;
         account_name n2 = i < gpo.active_producers.producers.size() ? gpo.active_producers.producers[i].producer_name : (account_name)0;
         if (n1 != n2) diff.push_back((uint64_t)n2 - (uint64_t)n1);
      }

      BOOST_CHECK_EQUAL(diff.size(), 0);

      const auto& producers_owner_authority = chain1_db.get<permission_object, by_owner>(boost::make_tuple(config::producers_account_name, config::owner_name));
      BOOST_CHECK_EQUAL(producers_owner_authority.auth.threshold, 0);
      BOOST_CHECK_EQUAL(producers_owner_authority.auth.accounts.size(), 0);
      BOOST_CHECK_EQUAL(producers_owner_authority.auth.keys.size(), 0);

} FC_LOG_AND_RETHROW() }

BOOST_AUTO_TEST_SUITE_END()
