// Copyright (c) 2022-2025  Made to Order Software Corp.  All Rights Reserved
//
// https://snapwebsites.org/project/snapdev
// contact@m2osw.com
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
#pragma once

/** \file
 * \brief Convert a numeric or string literal to a 128 bit value.
 *
 * These functions transform string literals or numeric literals to 128 bit
 * values as supported by g++. We support signed and unsigned in decimal,
 * hexadecimal, octal, and binary.
 */

// C++
//
#include    <cstring>
#include    <limits>


namespace snapdev
{
namespace literals
{

/** \brief Convert a literal to a signed __int128.
 *
 * This function converts a literal string to a signed __int128 integer
 * number.
 *
 * \code
 * using namespace snapdev::literals;
 *
 * __int128 value = "0xabcd000000000000"_int128;
 * \endcode
 *
 * \todo
 * Make sure that the first '\0' represents the end of the string.
 *
 * \todo
 * Support the negative (and positive) sign at the start of the string
 * (with number literal, it is viewed as an operator so it works as is,
 * with with string literals, we have to handle those ourselves).
 *
 * \param[in] literal  A 128 bit literal number.
 *
 * \return The __int128 number.
 *
 * \sa operator ""_uint128()
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
constexpr __int128 operator ""_int128(char const * literal, std::size_t len)
{
    __int128 result(0);
    __int128 base(10);

    if(len == 0)
    {
        throw std::invalid_argument("A string literal of a int128 must be at least one cahracter.");
    }

    if(*literal == '0')
    {
        ++literal;
        if(*literal == '\0')
        {
            return result;
        }
        if(*literal == 'x'
        || *literal == 'X')
        {
            // parse a hexadecimal number
            //
            base = 16;
            ++literal;
            if(*literal == '\0')
            {
                throw std::invalid_argument("0x must be followed by at least one hexadecimal digit.");
            }
        }
        else if(*literal == 'b'
             || *literal == 'B')
        {
            // parse a binary number
            //
            base = 2;
            ++literal;
            if(*literal == '\0')
            {
                throw std::invalid_argument("0b must be followed by at least one binary digit.");
            }
        }
        else
        {
            // parse an octal number
            //
            base = 8;
        }
    }
    // else -- parse a decimal number

    for(;;)
    {
        char const c(*literal++);
        if(c == '\0')
        {
            // valid
            //
            return result;
        }
        int digit(std::numeric_limits<int>::max());
        if(c >= '0' && c <= '9')
        {
            digit = c - '0';
        }
        else if(c >= 'a' && c <= 'f')
        {
            digit = c - ('a' - 10);
        }
        else if(c >= 'A' && c <= 'F')
        {
            digit = c - ('A' - 10);
        }
        if(digit >= base)
        {
            throw std::domain_error(
                  std::string("digit '")
                + c
                + "' too large for the selected base");
        }

        __int128 const old(result);
        result = result * base + digit;
        if(result < old)
        {
            // overflow
            //
            // IMPORTANT NOTE: This test works because we do not deal with
            // the '-' sign which the C++ compiler handles for us
            //
            throw std::domain_error("signed __int128 literal too large.");
        }
    }

    return result;
}
#pragma GCC diagnostic pop


/** \brief Convert a literal number to an __int128.
 *
 * This function converts a literal number to an __int128. For example:
 *
 * \code
 * using namespace snapdev::literals;
 *
 * __int128 value = 123_int128;
 * \endcode
 *
 * \param[in] literal  A literal number.
 *
 * \return The literal number converted to an __int128 value.
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
constexpr __int128 operator ""_int128(char const * literal)
{
    return operator ""_int128(literal, strlen(literal));
}
#pragma GCC diagnostic pop


/** \brief Convert a literal to an unsigned __int128.
 *
 * This function converts a literal string to an unsigned __int128 integer
 * number.
 *
 * \todo
 * Make sure that the first '\0' represents the end of the string.
 *
 * \param[in] literal  A 128 bit literal number.
 *
 * \return The unsigned __int128 number.
 *
 * \sa operator ""_int128()
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
constexpr unsigned __int128 operator ""_uint128(char const * literal, std::size_t len)
{
    unsigned __int128 result(0);
    unsigned __int128 base(10);

    if(len == 0)
    {
        throw std::invalid_argument("A string literal of a uint128 must be at least one cahracter.");
    }

    if(*literal == '0')
    {
        ++literal;
        if(*literal == '\0')
        {
            return result;
        }
        if(*literal == 'x'
        || *literal == 'X')
        {
            // parse a hexadecimal number
            //
            base = 16;
            ++literal;
            if(*literal == '\0')
            {
                throw std::invalid_argument("0x must be followed by at least one hexadecimal digit.");
            }
        }
        else if(*literal == 'b'
             || *literal == 'B')
        {
            // parse a binary number
            //
            base = 2;
            ++literal;
            if(*literal == '\0')
            {
                throw std::invalid_argument("0b must be followed by at least one binary digit.");
            }
        }
        else
        {
            // parse an octal number
            //
            base = 8;
        }
    }
    // else -- parse a decimal number

    for(;;)
    {
        char const c(*literal++);
        if(c == '\0')
        {
            // valid
            //
            return result;
        }
        unsigned int digit(std::numeric_limits<unsigned int>::max());
        if(c >= '0' && c <= '9')
        {
            digit = c - '0';
        }
        else if(c >= 'a' && c <= 'f')
        {
            digit = c - ('a' - 10);
        }
        else if(c >= 'A' && c <= 'F')
        {
            digit = c - ('A' - 10);
        }
        if(digit >= base)
        {
            throw std::domain_error(
                  std::string("digit '")
                + c
                + "' too large for the selected base");
        }

        unsigned __int128 const old(result);
        result = result * base + digit;
        if(result < old)
        {
            // overflow
            //
            throw std::domain_error("unsigned __int128 literal too large.");
        }
    }

    return result;
}
#pragma GCC diagnostic pop


/** \brief Convert a literal number to an unsigned __int128 value.
 *
 * This function converts a literal number to an unsigned __int128.
 * For example:
 *
 * \code
 * using namespace snapdev::literals;
 *
 * unsigned __int128 value = 123_uint128;
 * \endcode
 *
 * \param[in] literal  A literal number.
 *
 * \return The literal number converted to an unsigned __int128 value.
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
constexpr unsigned __int128 operator ""_uint128(char const * literal)
{
    return operator ""_uint128(literal, strlen(literal));
}
#pragma GCC diagnostic pop



} // namespace literals
} // namespace snapdev
// vim: ts=4 sw=4 et
