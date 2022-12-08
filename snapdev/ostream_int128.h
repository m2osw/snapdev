// Copyright (c) 2021-2022  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Print large integers to any iostream.
 *
 * This function prints large integers (128 bits) to the specified iostream.
 */

// C++
//
#include    <ostream>
#include    <sstream>


namespace snapdev
{

/** \brief Convert an __int128 to a string.
 *
 * This function converts an __int128 to a string.
 *
 * \param[in] x  An __int128 number.
 *
 * \return A string representing that __int128 number.
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
inline std::string to_string(__int128 x, __int128 base = 10, bool uppercase = false)
{
    char buf[129];  // binary requires 128 + '-' = 129 chars

    char const adjust((uppercase ? 'A' : 'a') - ('9' + 1));

    int idx(sizeof(buf));
    unsigned __int128 y(x < 0
        ? -static_cast<unsigned __int128>(x)
        : static_cast<unsigned __int128>(x));
    while(y >= static_cast<unsigned __int128>(base))
    {
        --idx;
        buf[idx] = y % base + '0';
        if(buf[idx] > '9')
        {
            buf[idx] += adjust;
        }
        y /= base;
    }
    --idx;
    buf[idx] = y + '0';
    if(buf[idx] > '9')
    {
        buf[idx] += adjust;
    }
    if(x < 0)
    {
        --idx;
        buf[idx] = '-';
    }

    return std::string(buf + idx, sizeof(buf) - idx);
}
#pragma GCC diagnostic pop


/** \brief Convert an unsigned __int128 to a string.
 *
 * This function converts an unsigned __int128 to a string.
 *
 * \param[in] x  An unsigned __int128 number.
 *
 * \return A string representing that unsigned __int128 number.
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
inline std::string to_string(unsigned __int128 x, unsigned __int128 base = 10, bool uppercase = false)
{
    char buf[128];  // binary requires 128 chars

    char const adjust((uppercase ? 'A' : 'a') - ('9' + 1));

    int idx(sizeof(buf));
    while(x >= base)
    {
        --idx;
        buf[idx] = x % base + '0';
        if(buf[idx] > '9')
        {
            buf[idx] += adjust;
        }
        x /= base;
    }
    --idx;
    buf[idx] = x + '0';
    if(buf[idx] > '9')
    {
        buf[idx] += adjust;
    }

    return std::string(buf + idx, sizeof(buf) - idx);
}
#pragma GCC diagnostic pop

} //namespace snap


// no namespace for operators, it's easier that way


/** \brief Output an __int128 number.
 *
 * This function outputs the specified __int128 number to this output
 * stream. It respects the base and for hexadecimal, it also respects
 * the uppercase format.
 *
 * \note
 * The hexadecimal and octal formats do not understand negative numbers.
 * This function respects the C++ definition and prints out unsigned
 * numbers when one of the std::hex or std::oct format are set. If you
 * would prefer a negative number (i.e. -0x1 instead of 0xfff...fff)
 * then make sure to directly call the snapdev::to_string() function
 * instead.
 *
 * \warning
 * This implementation does not offer proper formatting.
 * It will first generate a string then output that string
 * which means the result is not what you'd expect if you
 * used formatting such as std::setfill() and std::setw().
 *
 * \param[in] os  The output stream.
 * \param[in] x  The value to be output in \p os.
 *
 * \return A reference to \p os.
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
inline std::ostream & operator << (std::ostream & os, __int128 x)
{
    std::ios_base::fmtflags const fmt(os.flags() & std::ios_base::basefield);
    if(fmt == std::ios_base::oct)
    {
        if(x != 0
        && os.flags() & std::ios_base::showbase)
        {
            os << '0';
        }
        return os << snapdev::to_string(static_cast<unsigned __int128>(x), 8);
    }
    else if(fmt == std::ios_base::hex)
    {
        if(x != 0
        && os.flags() & std::ios_base::showbase)
        {
            if(os.flags() & std::ios_base::uppercase)
            {
                os << "0X";
            }
            else
            {
                os << "0x";
            }
        }
        return os << snapdev::to_string(static_cast<unsigned __int128>(x), 16, (os.flags() & std::ios_base::uppercase) != 0);
    }

    // the '-' is added by the to_string(), but not the '+'
    //
    if(x >= 0
    && os.flags() & std::ios_base::showpos)
    {
        os << '+';
    }

    return os << snapdev::to_string(x);
}
#pragma GCC diagnostic pop


/** \brief Output an unsigned __int128 number.
 *
 * This function outputs the specified __int128 number to this output
 * stream. It respects the base and for hexadecimal, it also respects
 * the uppercase format.
 *
 * \warning
 * This implementation does not offer proper formatting.
 * It will first generate a string then output that string
 * which means the result is not what you'd expect if you
 * used formatting such as std::setfill() and std::setw().
 *
 * \param[in] os  The output stream.
 * \param[in] x  The value to be output in \p os.
 *
 * \return A reference to \p os.
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
inline std::ostream & operator << (std::ostream & os, unsigned __int128 x)
{
    std::ios_base::fmtflags const fmt(os.flags() & std::ios_base::basefield);
    int base(10);
    bool uppercase(false);
    if(fmt == std::ios_base::oct)
    {
        base = 8;
        if(os.flags() & std::ios_base::showbase)
        {
            os << '0';
        }
    }
    else if(fmt == std::ios_base::hex)
    {
        uppercase = (os.flags() & std::ios_base::uppercase) != 0;
        base = 16;
        if(os.flags() & std::ios_base::showbase)
        {
            if(uppercase)
            {
                os << "0X";
            }
            else
            {
                os << "0x";
            }
        }
    }
    else if(os.flags() & std::ios_base::showpos)
    {
        os << '+';
    }
    return os << snapdev::to_string(x, base, uppercase);
}
#pragma GCC diagnostic pop


// vim: ts=4 sw=4 et
