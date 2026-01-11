// Copyright (c) 2026  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Determine the length of a string literal at compile time.
 *
 * This template is used to retrieve the length of a string literal at
 * compile time:
 *
 * \code
 *     if(snapdev::string_literal_length(str) > 3) ...
 * \endcode
 *
 * The function makes sure that the input string is a `char`, `wchar_t`,
 * `char8_t`, `char16_t`, and `char32_t`.
 *
 * \note
 * The `char8_t` type is only available since C++20.
 */

// self
//
#include    <snapdev/is_nonnull.h>
#include    <snapdev/is_string_literal.h>


// C++
//
#include    <type_traits>



namespace snapdev
{



/** \brief Get the length of a string literal at compile time.
 *
 * This template is used to get a string literal length at compile time.
 * It gives the size of the character array and also verifies that the
 * type is indeed a character type (char, wchar_t, char8_t, char16_t,
 * or char32_t).
 *
 * \warning
 * Note that any character buffer is viewed as a "string literal".
 * So a definition as follow will return true even though you may
 * see it as a buffer rather than a string literal:
 * \warning
 * \code
 *     constexpr std::uint8_t const g_buffer[5] = { 1, 2, 3, 4, 5 };
 * \endcode
 * \warning
 * Furher, a character buffer may have a size which is not the length
 * of the string as in:
 * \warning
 * \code
 *     constexpr char const g_buffer[256] = { 'O', 'o', 'p', 's' };
 * \endcode
 * \warning
 * as a string of 4 characters in a buffer of 256 chars so this function
 * returns 255. In that latter case, you may want to consider using the
 * string_length() template function instead.
 *
 * \tparam T  The type of the array element.
 * \tparam N  The size of the array.
 *
 * \sa string_length()
 * \sa is_string_literal
 * \sa is_a_string_literal
 */
template<typename T, std::size_t N>
std::enable_if_t<is_string_literal<T[N]>::value, std::size_t>
string_literal_length(T const (&)[N])
{
    return N - 1;
}


/** \brief Compute the length of a string at compile time.
 *
 * This function is similar to the string_literal_length() except that
 * it works with a buffer of any size and works like strlen() as in,
 * it stops at the first `'\0'` character.
 *
 * The template also verifies that the array type is of a character
 * type (char, wchar_t, char8_t, char16_t, char32_t).
 *
 * \exception found_nullptr
 * If the input pointer is a null pointer, then this exception is raised.
 *
 * \tparam T  The type of the string.
 */
template<typename T>
constexpr std::enable_if_t<std::disjunction_v<
              std::is_same<T, char>
            , std::is_same<T, wchar_t>
            , std::is_same<T, char8_t>
            , std::is_same<T, char16_t>
            , std::is_same<T, char32_t>>, std::size_t>
string_length(T const * s)
{
    return *is_nonnull(s) == '\0' ? 0 : 1 + string_length(s + 1);
}



} // namespace snapdev
// vim: ts=4 sw=4 et
