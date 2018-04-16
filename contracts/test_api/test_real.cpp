#include <enumivolib/real.hpp>
#include <enumivolib/enumivo.hpp>

#include "test_api.hpp"

void test_real::create_instances() {
    enumivo::real lhs1(5);
    eosio_assert(lhs1.value() == 5, "real instance value is wrong");
}

void test_real::test_division() {
    enumivo::real lhs1(5);
    enumivo::real rhs1(10);
    enumivo::real result1 = lhs1 / rhs1;

    uint64_t a = double_div(i64_to_double(5), i64_to_double(10));
    eosio_assert(a == result1.value(), "real division result is wrong");
}

void test_real::test_division_by_0() {
    enumivo::real lhs1(5);
    enumivo::real rhs1(0);
    enumivo::real result1 = lhs1 / rhs1;
    // in order to get rid of unused parameter warning
    result1 = 0;

    eosio_assert(false, "should've thrown an error");
}

void test_real::test_multiplication() {
    enumivo::real lhs1(5);
    enumivo::real rhs1(10);
    enumivo::real result1 = lhs1 * rhs1;
    uint64_t res = double_mult( 5, 10 );
    eosio_assert(res == result1.value(), "real multiplication result is wrong");
}

void test_real::test_addition()
{
    enumivo::real lhs1(5);
    enumivo::real rhs1(10);
    enumivo::real result1 = lhs1 / rhs1;
    uint64_t a = double_div(i64_to_double(5), i64_to_double(10));

    enumivo::real lhs2(5);
    enumivo::real rhs2(2);
    enumivo::real result2 = lhs2 / rhs2;
    uint64_t b = double_div(i64_to_double(5), i64_to_double(2));


    enumivo::real sum = result1+result2;
    uint64_t c = double_add( a, b );
    eosio_assert(sum.value() == c, "real addition operation result is wrong");
}


