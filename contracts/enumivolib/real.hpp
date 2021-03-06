#pragma once
#include <enumivolib/math.hpp>
#include <enumivolib/print.hpp>

namespace enumivo {
    /**
    *  @defgroup real Real number
    *  @ingroup mathcppapi
    *  @brief Real number data type with basic operators. Wrap double class of Math C API.
    *
    * Example:
    * @code
    * real a(100);
    * real b(10);
    * real c = a+b
    * real d = a / b
    * real e = a*b
    * if(a == b) {}
    * if(a > b) {}
    * if(a < b) {}
    * auto val = d.value();
    * @endcode
    * @{
    */
    class real {
    private:
        uint64_t val;
    public:
        /**
        * @brief Constructor to double object from uint64 value
        *
        * @details Constructor to double object from uint64 value
        * @param _val    data
        */
        real(const uint64_t &_val) : val(_val) {}

        uint64_t value() const { return val; }

        // Arithmetic operations
        real operator+(const real &rhs) const;

        real operator*(const real &rhs) const;

        real operator/(const real &rhs) const;

        // Comparison operators
        friend bool operator==(const real &c1, const real &c2);

        friend bool operator>(const real &c1, const real &c2);

        friend bool operator<(const real &c1, const real &c2);

    };

    /**
     * @brief Add two real variables
     * 
     * @details Add two real variables
     * @param rhs    double variable to be added with this
     * @return the sum of this and rhs
     */
    real real::operator+(const real &rhs) const {
        auto _result = double_add(value(), rhs.value());
        return real(_result);
    }

    /**
     * @brief Multiply two real variables
     * 
     * @details Multiply two real variables
     * @param rhs    double variable to be multiplied with this
     * @return the result after multiplication
     */
    real real::operator*(const real &rhs) const {
        auto _result = double_mult(value(), rhs.value());
        return real(_result);
    }

    /**
     * @brief Division between two real variables
     * 
     * @details Division between two real variables
     * @param rhs    double variable to be multiplied with this
     * @return the result after division
     */
    real real::operator/(const real &rhs) const {
        auto _result = double_div(i64_to_double(value()), i64_to_double(rhs.value()));
        return real(_result);
    }

    /**
     * @brief Compares two double variables c1 and c2
     * 
     * @details Compares two double variables c1 and c2
     * @return if c1 == c2, return true, otherwise false
     */
    bool operator==(const real &c1, const real &c2) {
        auto res = double_eq(c1.value(), c2.value());
        return (res == 1);
    }

    /**
     * @brief Compares two double variables c1 and c2
     * 
     * @details Compares two double variables c1 and c2
     * @return if c1 > c2, return true, otherwise false
     */
    bool operator>(const real &c1, const real &c2) {
        auto res = double_gt(c1.value(), c2.value());
        return (res == 1);
    }

    /**
     * @brief Compares two double variables c1 and c2
     * 
     * @details Compares two double variables c1 and c2
     * @return if c1 < c2, return true, otherwise false
     */
    bool operator<(const real &c1, const real &c2) {
        auto res = double_lt(c1.value(), c2.value());
        return (res == 1);
    }
   /// @} real
}
