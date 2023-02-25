// Copyright (c) 2022-2023  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Template used to create case insensitive strings.
 *
 * This is an extension of the std::basic_string that allows one to use
 * case insensitive strings containers such as maps and sets.
 *
 * It does so by using the toupper() function when comparing strings against
 * each other. Note that the toupper() function is run on one character at
 * a time, which means it is not compatible with certain languages where
 * multiple characters may be affected when computer uppercase or lowercase
 * characters.
 */

// C++
//
#include    <algorithm>
#include    <cwctype>
#include    <string>


// C
//
#include    <string.h>



namespace snapdev
{


/** \brief Trait to compare characters in lowercase (Case insensitively).
 *
 * This function is used to compare two characters together to allow for
 * case insensitive compare between standard strings.
 *
 * By default we define four types of case insensitive strings for
 * `char`, `wchar_t`, `char32_t` and `char16_t`.
 *
 * \warning
 * The compare doesn't work 100% correctly for a locale. It's still a
 * binary compare, but after we transform standalone characters to
 * lowercase (i.e. lowercase requires context to work in all locales).
 */
template<typename _CharT>
class case_insensitive_traits
    : public std::char_traits<_CharT>
{
public:
    typedef _CharT char_t;

    static bool eq(char_t c1, char_t c2)
    {
        return toupper(c1) == toupper(c2);
    }

    static bool ne(char_t c1, char_t c2)
    {
        return toupper(c1) != toupper(c2);
    }

    static bool lt(char_t c1, char_t c2)
    {
        return toupper(c1) <  toupper(c2);
    }

    static int compare(char_t const * s1, char_t const * s2, size_t n)
    {
        for(; n != 0; --n)
        {
            if(lt(*s1, *s2))
            {
                return -1;
            }
            if(lt(*s2, *s1))
            {
                return 1;
            }
            ++s1;
            ++s2;
        }

        return 0;
    }

    static char_t const * find(char_t const * s, int n, char_t a)
    {
        for(; n > 0 && ne(*s, a); --n, ++s);
        return s;
    }
};

typedef std::basic_string<char,     case_insensitive_traits<char>>      case_insensitive_string;
typedef std::basic_string<wchar_t,  case_insensitive_traits<wchar_t>>   case_insensitive_wstring;
typedef std::basic_string<char16_t, case_insensitive_traits<char16_t>>  case_insensitive_string16;
typedef std::basic_string<char32_t, case_insensitive_traits<char32_t>>  case_insensitive_string32;

template<typename _CharT>
std::basic_string<_CharT, case_insensitive_traits<_CharT>> to_case_insensitive_string(std::basic_string<_CharT> const & input)
{
    return std::basic_string<_CharT, case_insensitive_traits<_CharT>>(input.c_str(), input.length());
}

template<typename _CharT>
std::basic_string<_CharT> to_string(std::basic_string<_CharT, case_insensitive_traits<_CharT>> const & input)
{
    return std::basic_string<_CharT>(input.c_str(), input.length());
}

} // namespace snapdev
// vim: ts=4 sw=4 et
