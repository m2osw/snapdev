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
//
#pragma once

/** \file
 * \brief Determine whether a parameter can represent a string literal.
 *
 * This template can be used to determine whether the input looks like
 * a string literal.
 *
 * There are two main idea about a string literal. It is represented
 * by an array of characters which is constant. So if we detect such,
 * then the function returns true, otherwise, in all other cases the
 * function returns false.
 *
 * So if you have an array of bytes not representing an actual string,
 * you will still get a "true" as long as the array is constant.
 *
 * In general, you use this function this way:
 *
 * \code
 *     if(is_string_literal(str)) ...
 * \endcode
 *
 * The function is overloaded for `char`, `wchar_t`, `char8_t`,
 * `char16_t`, and `char32_t`.
 *
 * \note
 * The `char8_t` type is only available in C++20 and over.
 */



namespace snap
{


template<std::size_t N>
bool is_string_literal(char const (&)[N])
{
    return true;
}


template<std::size_t N>
bool is_string_literal(char (&)[N])
{
    return false;
}


template<std::size_t N>
bool is_string_literal(wchar_t const (&)[N])
{
    return true;
}


template<std::size_t N>
bool is_string_literal(wchar_t (&)[N])
{
    return false;
}


#ifdef __cpp_char8_t
template<std::size_t N>
bool is_string_literal(char8_t const (&)[N])
{
    return true;
}


template<std::size_t N>
bool is_string_literal(char8_t (&)[N])
{
    return false;
}
#endif


template<std::size_t N>
bool is_string_literal(char16_t const (&)[N])
{
    return true;
}


template<std::size_t N>
bool is_string_literal(char16_t (&)[N])
{
    return false;
}


template<std::size_t N>
bool is_string_literal(char32_t const (&)[N])
{
    return true;
}


template<std::size_t N>
bool is_string_literal(char32_t (&)[N])
{
    return false;
}


template<typename T>
bool is_string_literal(T &)
{
    return false;
}




} // namespace snap
// vim: ts=4 sw=4 et
