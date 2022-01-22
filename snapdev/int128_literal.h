// Copyright (c) 2022  Made to Order Software Corp.  All Rights Reserved
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
#pragma once

/** \file
 * \brief Convert a string literal to a 128 bit value.
 *
 * These functions transform string literals to 128 bit values as supported
 * by g++. We support signed and unsigned in decimal, hexadecimal, octal,
 * and binary.
 */

// C++ lib
//
//#include    <sstream>


namespace snap
{

/** \brief Convert a literal to a signed __int128.
 *
 * This function converts a literal string to a signed __int128 integer
 * number.
 *
 * \param[in] literal  A 128 bit literal number.
 *
 * \return The __int128 number.
 *
 * \sa operator""_uint128()
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
constexpr __int128 operator""_int128(char const * literal)
{
    __int128 result(0);
    __int128 base(10);

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
            // parse an hexadecimal number
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
        int digit(0);
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

        __int128 old(result);
        result = result * base + digit;
        if(result < old)
        {
            // overflow
            //
            throw std::domain_error("signed __int128 literal too large.");
        }
    }

    return result;
}
#pragma GCC diagnostic pop


/** \brief Convert a literal to an unsigned __int128.
 *
 * This function converts a literal string to an unsigned __int128 integer
 * number.
 *
 * \param[in] literal  A 128 bit literal number.
 *
 * \return The unsigned __int128 number.
 *
 * \sa operator""_uint128()
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
constexpr unsigned __int128 operator""_uint128(char const * literal)
{
    unsigned __int128 result(0);
    unsigned __int128 base(10);

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
            // parse an hexadecimal number
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
        unsigned int digit(0);
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

        unsigned __int128 old(result);
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



}
// snap namespace
// vim: ts=4 sw=4 et
