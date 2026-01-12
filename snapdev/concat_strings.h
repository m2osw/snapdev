// Copyright (c) 2024-2026  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Concatenate strings at compile time.
 *
 * At the moment, this is not super useful because it can only concatenate
 * two strings (although you can use concat<> any number of times, you have
 * to repeat it if you want to concatenate many strings).
 *
 * We would need to tweak this version to support any number of strings.
 *
 * Also, it does not work with our names.an strings since those are compiled
 * and not statically accessible at compile time. (See as2js project for
 * details about names.an files.)
 *
 * Source: https://stackoverflow.com/questions/24783400/#answer-72510788
 */



namespace snapdev
{



/* \brief Concatenate S1 and S2 at compile time.
 *
 * The input must be static strings. For example:
 *
 * \code
 *     constexpr char const hello[] = "hello ";
 *     constexpr char const world[] = "world";
 *
 *     // concatenate hello and world at compile time
 *     //
 *     std::cout << snapdev::concat<hello, world>::value << std::endl;
 * \endcode
 *
 * This template calls itself for all the characters found in S1 except
 * the null terminator.
 *
 * \tparam S1  left hand side string
 * \tparam S2  right hand side string
 * \tparam I1  position within S1 (you could skip some characters by using
 *             a number larger than 0)
 * \tparam I2  position within S2 (you could skip some characters by using
 *             a number larger than 0)
 * \tparam C1  character from S1 at position I1
 * \tparam C2  character from S2 at position I2
 * \tparam Chars  "extracted" characters
 */
template<char const * S1, char const * S2,
        int I1 = 0, int I2 = 0,
        char C1 = S1[I1], char C2 = S2[I2],
        char... Chars>
struct concat
    : concat<S1, S2, I1 + 1, I2, S1[I1 + 1], C2, Chars..., C1>
{
};

/** \brief Loop through S2.
 *
 * This version of the concat template continues to loop through the
 * characters, but this time it goes through the characters found in
 * S2 instead of S1 up until the null is found.
 *
 * \tparam S1  left hand side string
 * \tparam S2  right hand side string
 * \tparam I1  position within S1
 * \tparam I2  position within S2
 * \tparam C2  character from S2 at position I2
 * \tparam Chars  "extracted" characters
 */
template<char const * S1, char const * S2,
        int I1, int I2,
        char C2,
        char... Chars>
struct concat<S1, S2, I1, I2, 0, C2, Chars...>
    : concat<S1, S2, I1, I2 + 1, 0, S2[I2 + 1], Chars..., C2>
{
};

/** \brief Generate the string literal.
 *
 * This last specialization of the concat template ends the concatenation
 * and generates a string literal in `value`.
 *
 * \tparam S1  left hand side string
 * \tparam S2  right hand side string
 * \tparam I1  position within S1
 * \tparam I2  position within S2
 * \tparam Chars  "extracted" characters
 */
template<char const * S1, char const * S2,
        int I1, int I2,
        char... Chars>
struct concat<S1, S2, I1, I2, 0, 0, Chars...>
{
    static constexpr const char value[] = { Chars... , 0 };
};



} // namespace snapdev
// vim: ts=4 sw=4 et
