// Copyright (c) 2011-2026  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Determine whether a parameter represents a string literal.
 *
 * This template is used to determine whether the input is
 * a string literal.
 *
 * There are two main idea about a string literal. It is represented
 * by an array of characters which is constant. So if we detect such,
 * then the function returns true, otherwise, in all other cases the
 * function returns false.
 *
 * So if you have an array of bytes not representing an actual string,
 * you will still get "true" as long as the array is constant.
 *
 * In general, you use this function this way:
 *
 * \code
 *     if(snapdev::is_string_literal(str)) ...
 * \endcode
 *
 * The function is overloaded for `char`, `wchar_t`, `char8_t`,
 * `char16_t`, and `char32_t`.
 *
 * \note
 * The `char8_t` type is only available since C++20.
 */


// C++
//
#include    <cstdint>
#include    <functional>



namespace snapdev
{



/** \brief No a string literal.
 *
 * This template is used as the negative case when testing for a string
 * literal. It is the template used in the false case if the literal is
 * not even an array.
 *
 * \tparam T  A type which is not an array.
 */
template<typename T>
class is_string_literal
    : public std::false_type
{
};


/** \brief Check whether the array represents a string literal.
 *
 * This template is used in conditional templates to verify that
 * the input is a string literal. This means that the type of a value
 * is an array and that the type of each element is one of the character
 * types (char, wchar_t, char8_t, char16_t, or char32_t).
 *
 * \warning
 * Note that any character buffer is viewed as a "string literal".
 * So a definition as follow will return true even though you may
 * see it as a buffer rather than a string literal:
 * \warning
 * \code
 *     constexpr std::uint8_t const g_buffer[5] = { 1, 2, 3, 4, 5 };
 * \endcode
 *
 * \tparam T  The type of the array element.
 * \tparam N  The size of the array.
 */
template<typename T, std::size_t N>
class is_string_literal<T[N]>
    : public std::disjunction<
              std::is_same<T, char>
            , std::is_same<T, wchar_t>
            , std::is_same<T, char8_t>
            , std::is_same<T, char16_t>
            , std::is_same<T, char32_t>>
{
};





template<std::size_t N>
bool is_a_string_literal(char const (&)[N])
{
    return true;
}


template<std::size_t N>
bool is_a_string_literal(char (&)[N])
{
    return false;
}


template<std::size_t N>
bool is_a_string_literal(wchar_t const (&)[N])
{
    return true;
}


template<std::size_t N>
bool is_a_string_literal(wchar_t (&)[N])
{
    return false;
}


#ifdef __cpp_char8_t
template<std::size_t N>
bool is_a_string_literal(char8_t const (&)[N])
{
    return true;
}


template<std::size_t N>
bool is_a_string_literal(char8_t (&)[N])
{
    return false;
}
#endif


template<std::size_t N>
bool is_a_string_literal(char16_t const (&)[N])
{
    return true;
}


template<std::size_t N>
bool is_a_string_literal(char16_t (&)[N])
{
    return false;
}


template<std::size_t N>
bool is_a_string_literal(char32_t const (&)[N])
{
    return true;
}


template<std::size_t N>
bool is_a_string_literal(char32_t (&)[N])
{
    return false;
}


template<typename T>
bool is_a_string_literal(T &)
{
    return false;
}



} // namespace snapdev
// vim: ts=4 sw=4 et
