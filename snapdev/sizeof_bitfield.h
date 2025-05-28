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
 * \brief Compute the size of a bitfield at compile time.
 *
 * This lambda functions and macro are used to compute the size of a bitfield
 * at compile time. This can be useful to generate masks, verify that we have
 * enough bits, etc.
 */



/** \brief Macro used to compute the width of a bit field at compile time.
 *
 * For a regular field, you can use sizeof() and if you want the result in
 * bits, multiply by 8 (or `CHAR_BIT`).
 *
 * If you need to know the width of a field at compile time and that field
 * is a bit field, then there is no language provided mechanism at the moment
 * that gives you that information.
 *
 * The following macro uses a constexpr lambda to compute the width of such
 * a field by shifting one bit inside that field until it gets cleared.
 */
#define SIZEOF_BITFIELD(T, f)       \
    []() constexpr -> std::size_t       \
    {                                   \
        T t{};                          \
        t.f = 1;                        \
        std::size_t width(0);           \
        while(t.f != 0)                 \
        {                               \
            t.f <<= 1;                  \
            ++width;                    \
        }                               \
        return width;                   \
    }()


// vim: ts=4 sw=4 et
