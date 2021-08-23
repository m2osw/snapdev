// Copyright (c) 2019-2021  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Calculate the log2() of an integer.
 *
 * This function calculates the number of zero bits before the first bit set
 * to 1. This is the floor of the log of a number in base 2 or:
 *
 *     floor(log(n) / log(2))
 *
 * The function makes use of an intrinsic so that way it goes as fast as
 * possible which should be one CPU cycle to find the bit and another couple
 * of cycles to transform the number in the correct `log2()` value.
 *
 * The result could be used to do:
 *
 *     1 << log2(v)
 *
 * to find v against assuming v was an exact power of 2 to start with.
 */

// C lib
//
#include <stdio.h>


namespace snap
{


/** \brief Calculate the log2() of an integer.
 *
 * This function uses an intrinsic instruction to calculate the log2()
 * of a 64 bit integer. The lower bits are ignored.
 *
 * This is equivalent to finding the position of the first bit set starting
 * at bit 63 and going down to bit 0.
 *
 * Note that `log2(0)` is undefined. The function returns -1 in that case.
 *
 * If your number is signed, make sure to pass the absolute value:
 * `log2(llabs(v))`.
 *
 * Source:
 * https://stackoverflow.com/questions/11376288/fast-computing-of-log2-for-64-bit-integers
 *
 * \param[in] v  The integer to calculate the log2() from.
 *
 * \return `floor(log(v) / log(2))` or -1 when v == 0.
 */
int log2(std::uint64_t v)
{
    if(v == 0)
    {
        return -1;
    }

    return 63 - __builtin_clzll(v);
}



} // namespace snap
// vim: ts=4 sw=4 et
