/**
* @file token.hpp
* @copyright defined in enumivo/LICENSE.txt
* @brief Defines types and ABI for standard token messages and database tables
*
*/
#pragma once
#include <enumivolib/math.hpp>
#include <enumivolib/print.hpp>
#include <enumivolib/reflect.hpp>
#include <enumivolib/asset.hpp>
#include <enumivolib/serialize.hpp>


namespace enumivo {
  /**
  *  @defgroup tokens Token API
  *  @brief Defines the ABI for interfacing with standard-compatible token messages and database tables.
  *  @ingroup contractdev
  *
  * @{
  */

  template<typename BaseToken, typename QuoteToken>
  struct price;

  template< uint64_t Code,
            uint64_t Symbol,
            typename NumberType = uint64_t 
              >
  struct token {
    /**
    * Type of the currency (e.g. enu) represented as an unsigned 64 bit integer
    * @brief  Type of the currency
    */
    static const uint64_t code   = Code;
    static const uint64_t symbol = Symbol;

    /**
    * Default constructor
    * @brief Default constructor
    */
    token(){}


    template<typename Base, typename Quote>
    friend price<Base,Quote> operator / ( const Base& b, const Quote& q );

    explicit operator asset()const { return asset( int64_t(quantity), Symbol ); }

    token( const asset& a ):quantity(NumberType(a.amount)) {
       eosio_assert( a.symbol == Symbol, "attempt to construct token from asset with different symbol" );
       eosio_assert( 0 <= a.amount, "attemt to convert asset with negative value to token" );
    }

    /**
    * Constructor for token given quantity of tokens available
    * @brief Constructor for token given quantity of tokens available
    * @param v - quantity of tokens available
    */
    explicit token( NumberType v ):quantity(v){}

    /**
    * Quantity of tokens available
    * @brief Quantity of tokens available
    */
    NumberType quantity = 0;

    /**
    * Subtracts quantity of token from this object
    * Throws an exception if underflow
    * @brief Subtracts quantity of token from this object
    * @param a token to be subtracted
    * @return this token after subtraction
    */
    token& operator-=( const token& a ) {
      eosio_assert( quantity >= a.quantity, "integer underflow subtracting token balance" );
      quantity -= a.quantity;
      return *this;
    }

    /**
    * Adds quantity of token to this object
    * Throws an exception if overflow
    * @brief Adds quantity of token to this object
    * @param a token to be added
    * @return this token after addition
    */
    token& operator+=( const token& a ) {
      eosio_assert( quantity + a.quantity >= a.quantity, "integer overflow adding token balance" );
      quantity += a.quantity;
      return *this;
    }

    /**
    * Adds quantity of two tokens and return a new token
    * Throws an exception if overflow
    * @brief Adds quantity of two tokens and return a new token
    * @param a token to be added
    * @param b token to be added
    * @return result of addition as a new token
    */
    inline friend token operator+( const token& a, const token& b ) {
      token result = a;
      result += b;
      return result;
    }

    /**
    * Subtracts quantity of two tokens and return a new token
    * Throws an exception if underflow
    * @brief Subtracts quantity of two tokens and return a new token
    * @param a token to be subtracted
    * @param b token to be subtracted
    * @return result of subtraction as a new token
    */
    inline friend token operator-( const token& a, const token& b ) {
      token result = a;
      result -= b;
      return result;
    }

    /**
    * Multiplies quantity of token by an integer
    * Throws an exception if overflow
    * @brief Multiplies quantity of token by an integer
    * @param a multipier
    * @return this token after addition
    */
    token& operator*=( uint64_t a ) {
      eosio_assert( a == 0 || (quantity * a) / a == quantity, "integer overflow multiplying token balance" );
      quantity *= a;
      return *this;
    }

    /**
    * Multiplies token and integer
    * Throws an exception if overflow
    * @brief Multiplies quantity of two tokens and return a new token
    * @param a token to be multiplied
    * @param b multipier
    * @return result of addition as a new token
    */
    inline friend token operator*( const token& a, uint64_t b ) {
      token result = a;
      result *= b;
      return result;
    }

    /**
    * Multiplies token and integer
    * Throws an exception if overflow
    * @brief Multiplies quantity of two tokens and return a new token
    * @param a token to be multiplied
    * @param b multipier
    * @return result of addition as a new token
    */
   inline friend token operator*( uint64_t b, const token& a ) {
      token result = a;
      result *= b;
      return result;
    }

    /**
    * Divides quantity of token by an integer
    * Throws an exception if overflow
    * @brief Divides quantity of token by an integer
    * @param a multipier
    * @return this token after addition
    */
    token& operator/=( uint64_t a ) {
      quantity /= a;
      return *this;
    }

    /**
    * Divides token and integer
    * Throws an exception if overflow
    * @brief Divides quantity of two tokens and return a new token
    * @param a token to be multiplied
    * @param b multipier
    * @return result of addition as a new token
    */
    inline friend token operator/( const token& a, uint64_t b ) {
      token result = a;
      result /= b;
      return result;
    }

     /**
    * Divides two tokens
    * Throws an exception if overflow
    * @brief Divides quantity of two tokens and return a new token
    * @param a token
    * @param b token
    * @return result of addition as a new token
    */
    inline friend NumberType operator/( const token& a, const token& b ) {
      return a.quantity / b.quantity;
    }

    /**
    * Less than or equal to comparison operator
    * @brief Less than or equal to comparison operator
    * @param a token to be compared
    * @param b token to be compared
    * @return true if quantity of a is less than or equal to quantity of b
    */
    friend bool operator <= ( const token& a, const token& b ) { return a.quantity <= b.quantity; }

    /**
    * Less than comparison operator
    * @brief Less than comparison operator
    * @param a token to be compared
    * @param b token to be compared
    * @return true if quantity of a is less than quantity of b
    */
    friend bool operator <  ( const token& a, const token& b ) { return a.quantity <  b.quantity; }

    /**
    * Greater than or equal to comparison operator
    * @brief Greater than or equal to comparison operator
    * @param a token to be compared
    * @param b token to be compared
    * @return true if quantity of a is greater than or equal to quantity of b
    */
    friend bool operator >= ( const token& a, const token& b ) { return a.quantity >= b.quantity; }

    /**
    * Greater than comparison operator
    * @brief Greater than comparison operator
    * @param a token to be compared
    * @param b token to be compared
    * @return true if quantity of a is greater than quantity of b
    */
    friend bool operator >  ( const token& a, const token& b ) { return a.quantity >  b.quantity; }

    /**
    * Equality comparison operator
    * @brief Equality comparison operator
    * @param a token to be compared
    * @param b token to be compared
    * @return true if quantity of a is equal to quantity of b
    */
    friend bool operator == ( const token& a, const token& b ) { return a.quantity == b.quantity; }
    /**
    * Inequality comparison operator
    * @brief Inequality comparison operator
    * @param a token to be compared
    * @param b token to be compared
    * @return true if quantity of a is not equal to quantity of b
    */
    friend bool operator != ( const token& a, const token& b ) { return a.quantity != b.quantity; }

    /**
    * Boolean conversion operator
    * @brief Boolean conversion operator
    * @return true if quantity is not zero
    */
    explicit operator bool()const { return quantity != 0; }

    template<typename DataStream>
    friend DataStream& operator << ( DataStream& ds, const token& t ){
       return ds << t.quantity;
    }
    template<typename DataStream>
    friend DataStream& operator >> ( DataStream& ds, token& t ){
       return ds >> t.quantity;
    }

  };
  /// @}


  template<typename BaseToken, typename QuoteToken>
  struct price_ratio {
     BaseToken   base;
     QuoteToken  quote;
  };

  template<typename Base, typename Quote>
  price<Base,Quote> operator / ( const Base& b, const Quote& q ) {
     return price<Base,Quote>(b,q);
  }
  

  /**
  *
  *  @brief Defines a fixed precision price between two tokens.
  *  @tparam BaseToken - represents the type of the base token
  *  @tparam QuoteToken -  represents the type of the quote token
  *  @ingroup tokens
  *
  *  @details Defines a fixed precision price between two tokens.
  *  A price is written as  X  Base/Quote. Where X is a power of 10 which makes it simpler to just shift the decimal.
  *  It supports the following operator: /, \, <=, <, ==, !=, >=, > and also print functionality
  *
  *  Example:
  *  @code
  *  typedef enumivo::token<uint64_t, N(MyBaseToken)> MyBaseToken;
  *  typedef enumivo::token<uint64_t, N(MyQuoteToken)> MyQuoteToken;
  *  typedef price<MyBaseToken, MyQuoteToken> MyBaseToQuotePrice;
  *  MyBaseToken zeroBaseToken;
  *  MyQuoteToken zeroQuoteToken;
  *  MyBaseToQuotePrice zeroBaseToQuote(zeroBaseToken, zeroQuoteToken); // throws invalid price exception
  *  MyBaseToken baseToken(128);
  *  MyQuoteToken quoteToken(128);
  *  MyBaseToQuotePrice aPrice(baseToken, quoteToken);
  *  aPrice.print(); // Output: 1e+15. MyBaseToken / MyQuoteToken
  *  MyQuoteToken anotherQuote = baseToken / price;
  *  std::cout << (anotherQuote == quoteToken); // Output: true
  *  MyBaseToken anotherBase = quoteToken * price;
  *  std::cout << (anotherBase == baseToken); // Output: true
  *  MyBaseToQuotePrice anotherPrice(baseToken, quoteToken);
  *  std::cout << (aPrice == anotherPrice); // Output: true
  *  std::cout << (aPrice != anotherPrice); // Output: false
  *  MyBaseToken base256(256);
  *  MyBaseToQuotePrice price2(base256, quoteToken);
  *  std::cout << (price2 > aPrice); // Output: true
  *  std::cout << (aPrice < price2); // Output: true
  *  @endcode
  *
  *  @{
  */
  template<typename BaseToken, typename QuoteToken>
  struct price
  {
    typedef BaseToken  base_token_type;
    typedef QuoteToken quote_token_type;
    /**
    *  The largest base 10 integer that can be represented with 53 bits of
    *  a double. This number keeps the math in range of JavaScript. By
    *  being a power of 10 it makes it easier for developers to read and
    *  interpret the integer by simply shifting the decimal.
    *  @brief Precision of the price.
    */
    static const uint64_t precision = 1000ll*1000ll*1000ll*1000ll*1000ll;

    /**
    * Default constructor.
    * Initialize base per quote to be 1.
    * @brief Default constructor.
    */
    price():base_per_quote(1ul){}
    explicit price( uint128_t b ):base_per_quote(b){}
    price& operator=( uint128_t b ) {
       base_per_quote = b;
       return *this;
    }

    /**
    * Construction for price given the base token and quote token.
    * @brief Construction for price given the base token and quote token.
    * @param base - base token
    * @param quote - quote token
    */
    price( BaseToken base, QuoteToken quote ) {
      eosio_assert( base  >= BaseToken(1ul), "invalid price" );
      eosio_assert( quote >= QuoteToken(1ul), "invalid price" );

      base_per_quote = base.quantity;
      base_per_quote *= precision;
      base_per_quote /= quote.quantity;
    }

    /**
    * Operator returns a quote token given a base token and the conversion price.
    * @brief Operator returns a quote token given a base token and the conversion price.
    * @param b - base token
    * @param q - price
    * @return quote token
    */
    friend QuoteToken operator / ( BaseToken b, const price& q ) {
      return QuoteToken( uint64_t((uint128(b.quantity) * uint128(precision)   / q.base_per_quote)) );
    }

    /**
    * Operator returns a base token given a quote token and the conversion price.
    * @brief Operator returns a base token given a quote token and the conversion price.
    * @param b - quote token
    * @param q - price
    * @return base token
    */
    friend BaseToken operator * ( const QuoteToken& b, const price& q ) {
      //return QuoteToken( uint64_t( mult_div_i128( b.quantity, q.base_per_quote, precision ) ) );
      return BaseToken( uint64_t((b.quantity * q.base_per_quote) / precision) );
    }

    /**
    * Less than or equal to comparison operator.
    * @brief Less than or equal to comparison operator.
    * @param a price to be compared
    * @param b price to be compared
    * @return true if base per quote of a is less than  or equal to base per quote of b
    */
    friend bool operator <= ( const price& a, const price& b ) { return a.base_per_quote <= b.base_per_quote; }

    /**
    * Less than comparison operator.
    * @brief Less than comparison operator.
    * @param a price to be compared
    * @param b price to be compared
    * @return true if base per quote of a is less than base per quote of b
    */
    friend bool operator <  ( const price& a, const price& b ) { return a.base_per_quote <  b.base_per_quote; }

    /**
    * Greater than or equal to comparison operator.
    * @brief Greater than or equal to comparison operator.
    * @param a price to be compared
    * @param b price to be compared
    * @return true if base per quote of a is greater than or equal to base per quote of b
    */
    friend bool operator >= ( const price& a, const price& b ) { return a.base_per_quote >= b.base_per_quote; }

    /**
    * Greater than comparison operator.
    * @brief Greater than comparison operator.
    * @param a price to be compared
    * @param b price to be compared
    * @return true if base per quote of a is greater than base per quote of b
    */
    friend bool operator >  ( const price& a, const price& b ) { return a.base_per_quote >  b.base_per_quote; }

    /**
    * Equality comparison operator.
    * @brief Equality comparison operator.
    * @param a price to be compared
    * @param b price to be compared
    * @return true if base per quote of a is equal to base per quote of b
    */
    friend bool operator == ( const price& a, const price& b ) { return a.base_per_quote == b.base_per_quote; }

    /**
    * Inequality comparison operator.
    * @brief Inequality comparison operator.
    * @param a price to be compared
    * @param b price to be compared
    * @return true if base per quote of a is not equal to base per quote of b
    */
    friend bool operator != ( const price& a, const price& b ) { return a.base_per_quote != b.base_per_quote; }


    operator uint128_t()const { return base_per_quote; }

    ENULIB_SERIALIZE( price, (base_per_quote) )
  private:
    /**
    * Represents as number of base tokens to purchase 1 quote token.
    * @brief Represents number of base tokens to purchase 1 quote token.
    */
    uint128_t base_per_quote;
  };


  /// @}


  /// @} tokenhppapi




}

