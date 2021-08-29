// Copyright (c) 2011-2021  Made to Order Software Corp.  All Rights Reserved
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


// libexcept lib
//
#include "libexcept/exception.h"


// C++ lib
//
#include <algorithm>



namespace snap
{


DECLARE_MAIN_EXCEPTION(string_exception);

DECLARE_EXCEPTION(string_exception, string_invalid_parameter);



/** \brief Transform a binary string to hexadecimal.
 *
 * This function transforms a string of binary bytes (any value from 0x00
 * to 0xFF) to a string of hexadecimal digits.
 *
 * The output string will be exactly 2x the size of the input string.
 *
 * \param[in] binary  The input binary string to convert.
 *
 * \return The hexademical representation of the input string.
 */
inline std::string bin_to_hex(std::string const & binary)
{
    if(binary.empty())
    {
        return std::string();
    }

    std::string result;

    result.reserve(binary.length() * 2);

    std::for_each(
              binary.begin()
            , binary.end()
            , [&result](char const & c)
            {
                auto to_hex([](char d)
                {
                    return d < 10 ? d + '0' : d + ('a' - 10);
                });

                result.push_back(to_hex((c >> 4) & 15));
                result.push_back(to_hex(c & 15));
            });

    return result;
}


/** \brief Convert an hexadecimal string to a binary string.
 *
 * This function is the inverse of the bin_to_hex() function. It
 * converts a text string of hexadecimal numbers (exactly 2 digits
 * each) into a binary string (a string of any bytes from 0x00 to
 * 0xFF.)
 *
 * The output will be exactly half the size of the input.
 *
 * \exception string_invalid_parameter
 * If the input string is not considered valid, then this exception is
 * raised. To be valid every single character must be an hexadecimal
 * digit (0-9, a-f, A-F) and the length of the string must be even.
 *
 * \param[in] hex  The salt as an hexadecimal string of characters.
 *
 * \return The converted value in a binary string.
 */
inline std::string hex_to_bin(std::string const & hex)
{
    std::string result;

    if((hex.length() & 1) != 0)
    {
        throw string_invalid_parameter("the hex parameter must have an even size");
    }

    for(char const * s(hex.c_str()); *s != '\0'; s += 2)
    {
        int value(0);

        // first digit
        //
        if(s[0] >= '0' && s[0] <= '9')
        {
            value = (s[0] - '0') * 16;
        }
        else if(s[0] >= 'a' && s[0] <= 'f')
        {
            value = (s[0] - 'a' + 10) * 16;
        }
        else if(s[0] >= 'A' && s[0] <= 'F')
        {
            value = (s[0] - 'A' + 10) * 16;
        }
        else
        {
            throw string_invalid_parameter("the hex parameter must only contain valid hexadecimal digits");
        }

        // second digit
        //
        if(s[1] >= '0' && s[1] <= '9')
        {
            value += s[1] - '0';
        }
        else if(s[1] >= 'a' && s[1] <= 'f')
        {
            value += s[1] - 'a' + 10;
        }
        else if(s[1] >= 'A' && s[1] <= 'F')
        {
            value += s[1] - 'A' + 10;
        }
        else
        {
            throw string_invalid_parameter("the hex parameter must only contain valid hexadecimal digits");
        }

        result.push_back(value);
    }

    return result;
}


/** \brief Transform an integer to a string of hexadecimal digits.
 *
 * This function transforms an integer to a string of hexadecimal digits.
 *
 * The output string is optimized to not include any unnecessary leading
 * zeroes.
 *
 * \note
 * The function does not add an introducer (so no "0x" at the start of
 * the resulting string).
 *
 * \tparam T  The type of integer to convert to a string.
 * \param[in] value  The input integer to convert.
 *
 * \return The hexademical representation of the input integer.
 */
template<class T>
inline std::string int_to_hex(T value)
{
    // special case, without this, we'd get an empty string for zero
    //
    if(value == 0)
    {
        return std::string("0");
    }

    char buf[sizeof(T) * 2 + 1];
    char * d(buf + sizeof(T) * 2);
    *d = '\0';

    for(; value != 0; value >>= 4)
    {
        --d;
        int const v(value & 15);
        *d = v < 10 ? v + '0' : v + ('a' - 10);
    }

    return std::string(d);
}


} // namespace snap
// vim: ts=4 sw=4 et
