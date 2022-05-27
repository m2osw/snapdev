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
 * \brief Various mathematical functions.
 *
 * These functions are extensions to the math functions offered by the C
 * and the C++ libraries.
 */

// self
//
#include    <snapdev/int128_literal.h>


namespace snapdev
{


/** \brief Computer a power of a 128 bit integer number.
 *
 * This function multiplies the specified \p value by itself \p power times.
 *
 * This is a function to computer an integer power so \p power must be an
 * integer.
 *
 * \todo
 * The function does not check for overflow.
 *
 * \param[in] value  A 128 bit number.
 * \param[in] power  The number of times to multiply value by itself.
 *
 * \return The resulting value ^ power.
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
constexpr __int128 power128(__int128 value, int power)
{
    using namespace snapdev::literals;

    if(power == 0)
    {
        return 1_int128;
    }
    __int128 result(value);
    for(--power; power > 0; --power)
    {
        result *= value;
    }
    return result;
}
#pragma GCC diagnostic pop


/** \brief Computer a power of a 128 bit unsigned integer number.
 *
 * This function multiplies the specified \p value by itself \p power times.
 *
 * This is a function to computer an unsigned integer power so \p power must
 * be an integer.
 *
 * \todo
 * The function does not check for overflow.
 *
 * \param[in] value  A 128 bit unsigned number.
 * \param[in] power  The number of times to multiply value by itself.
 *
 * \return The resulting value ^ power.
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
constexpr unsigned __int128 power128(unsigned __int128 value, int power)
{
    using namespace snapdev::literals;

    if(power == 0)
    {
        return 1_uint128;
    }
    unsigned __int128 result(value);
    for(--power; power > 0; --power)
    {
        result *= value;
    }
    return result;
}
#pragma GCC diagnostic pop


} // namespace snapdev
// vim: ts=4 sw=4 et
