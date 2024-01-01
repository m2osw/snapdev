// Copyright (c) 2011-2024  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Convert a number to a string.
 *
 * This function allows you to convert a number (integer or double) to a
 * string. The function accepts a \em base (radix) parameter so it is
 * possible to convert the number to any base from 2 to 36.
 */

// C++
//
//#include    <cmath>
//#include    <stdexcept>
//#include    <type_traits>



namespace snapdev
{



/** \brief Convert integrals to a string.
 *
 * This template is used to convert integers to a string of characters.
 * It supports a base so you can convert integers to decimal,
 * octal, hexadecimal, binary, and any other base from 2 up to 36.
 *
 * \note
 * This class is overfully complete and can be used to traverse the resulting
 * string.
 *
 * \tparam T  The type of number (i.e. `int`).
 * \tparam base  Desired base.
 * \tparam uppercase  Whether to use uppercase (true) or lowercase (false).
 * \tparam CharT  Type of characters of the output string.
 */
template<
      typename T = int
    , typename CharT = char
    , std::enable_if_t<std::is_integral_v<T>, int> = 0>
std::basic_string<CharT> integer_to_string(T value, int base = 10, bool uppercase = false)
{
    if(base < 2 || base > 36)
    {
        throw std::range_error("base is out of range in integer_to_string()");
    }

    CharT buf[40];
    CharT * end(buf + sizeof(buf) / sizeof(CharT));
    CharT * ptr(end);
    if(value != 0)
    {
        bool const neg(value < 0);
        if(neg)
        {
            value = -value;
        }
        for(; value != 0; value /= base)
        {
            --ptr;
            int const digit(value % base);
            if(digit < 10)
            {
                *ptr = '0' + digit;
            }
            else if(uppercase)
            {
                *ptr = ('A' - 10) + digit;
            }
            else
            {
                *ptr = ('a' - 10) + digit;
            }
        }

        if(neg)
        {
            --ptr;
            *ptr = '-';
        }
    }
    else
    {
        --ptr;
        *ptr = '0';
    }

    return std::basic_string<CharT>(ptr, end - ptr);
}



} // namespace snapdev
// vim: ts=4 sw=4 et
