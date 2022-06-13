// Copyright (c) 2020-2022  Made to Order Software Corp.  All Rights Reserved
//
// https://snapwebsites.org/project/snapdev
// contact@m2osw.com
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
// Based on: https://stackoverflow.com/questions/6713420/c-convert-integer-to-string-at-compile-time
//
#pragma once


namespace snapdev
{


// calculate absolute value
constexpr std::int64_t abs_val(std::int64_t x)
{
    return x < 0 ? -x : x;
}


// calculate number of digits needed, including minus sign
constexpr std::int64_t num_digits(std::int64_t x)
{
    return x < 0 ? 1 + num_digits(-x) : x < 10 ? 1 : 1 + num_digits(x / 10);
}


// meta-programming string type: each different string is a unique type
template<char... args>
struct metastring
{
    const char data[sizeof... (args)] = { args... };
};


// recursive number-printing template, general case (for three or more digits)
template<std::int64_t size, std::int64_t x, char... args>
struct numeric_builder
{
    typedef typename numeric_builder<size - 1, x / 10, '0' + abs_val(x) % 10, args...>::type type;
};


// special case for two digits; minus sign is handled here */
template<std::int64_t x, char... args>
struct numeric_builder<2, x, args...>
{
    typedef metastring<x < 0 ? '-' : '0' + x / 10, '0' + abs_val(x) % 10, args...> type;
};


// special case for one digit (positive numbers only)
template<std::int64_t x, char... args>
struct numeric_builder<1, x, args...>
{
    typedef metastring<'0' + x, args...> type;
};


// convenience wrapper for numeric_builder
template<std::int64_t x>
class numeric_string
{
private:
    // generate a unique string type representing this number
    typedef typename numeric_builder<num_digits(x), x, '\0'>::type type;

    // declare a static string of that type (instantiated later at file scope)
    static constexpr type value {};

public:
    // returns a pointer to the instantiated string
    static constexpr const char * get()
    {
        return value.data;
    }
};



/** \brief Instantiate numeric_string::value as needed for different numbers
 *
 * Here are a few examples on how the numeric_string<> template gets used:
 *
 * \code
 *     static constexpr const char * five = numeric_string<5>::get();
 *     static constexpr const char * one_ten = numeric_string<110>::get();
 *     static constexpr const char * minus_thirty = numeric_string<-30>::get();
 * \endcode
 *
 * \note
 * At this time I defined the type as `std::int64_t`. That means you can't
 * use this template with an `std::uint64_t` or numbers larger than 64 bits.
 * I'd like to look at whether it would be possible to auto define the type
 * instead.
 *
 * \note
 * If you can deal with std::string_view, the snapdev::to_string_literal
 * is much more useful as it converts strings with any base and the
 * class offers many options (begin()/end(), cast back to char *, different
 * type of characters, etc.)
 */
template<std::int64_t x>
constexpr typename numeric_string<x>::type numeric_string<x>::value;


} // namespace snapdev
// vim: ts=4 sw=4 et
