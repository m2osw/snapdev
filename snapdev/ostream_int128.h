// Copyright (c) 2021  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Print large integers to any iostream.
 *
 * This function prints large integers (128 bits) to the specified iostream.
 */

// C++ lib
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
std::string to_string(__int128 x)
{
    char buf[42];

    int idx(sizeof(buf));
    unsigned __int128 y(x < 0
        ? -static_cast<unsigned __int128>(x)
        : static_cast<unsigned __int128>(x));
    constexpr unsigned __int128 const limit(9);
    while(y > limit)
    {
        --idx;
        buf[idx] = y % 10 + '0';
        y /= 10;
    }
    --idx;
    buf[idx] = y + '0';
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
std::string to_string(unsigned __int128 x)
{
    char buf[42];

    int idx(sizeof(buf));
    while(x > 9)
    {
        --idx;
        buf[idx] = x % 10 + '0';
        x /= 10;
    }
    --idx;
    buf[idx] = x + '0';

    return std::string(buf + idx, sizeof(buf) - idx);
}
#pragma GCC diagnostic pop

} //namespace snap


// no namespace for operators, it's easier that way


/** \brief Output an __int128 number.
 *
 * \warning
 * This implementation does not offer proper formatting.
 * It will first generate a string then output that string
 * which means the result is not what you'd expect if you
 * used formatting such as std::hex and std::setw().
 *
 * \param[in] os  The output stream.
 * \param[in] x  The value to be output in \p os.
 *
 * \return A reference to \p os.
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
std::ostream & operator << (std::ostream & os, __int128 x)
{
    return os << snapdev::to_string(x);
}
#pragma GCC diagnostic pop


/** \brief Output an unsigned __int128 number.
 *
 * \warning
 * This implementation does not offer proper formatting.
 * It will first generate a string then output that string
 * which means the result is not what you'd expect if you
 * used formatting such as std::hex and std::setw().
 *
 * \param[in] os  The output stream.
 * \param[in] x  The value to be output in \p os.
 *
 * \return A reference to \p os.
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
std::ostream & operator << (std::ostream & os, unsigned __int128 x)
{
    return os << snapdev::to_string(x);
}
#pragma GCC diagnostic pop


// vim: ts=4 sw=4 et
