// Copyright (c) 2011-2024  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Make a string regex proof by escaping all the special characters.
 *
 * The function below goes through a string and escape all the special
 * characters such as a period (.) using a backslash.
 */

// C++
//
#include    <cstring>
#include    <string>
#include    <stdexcept>



namespace snapdev
{

namespace
{



/** \brief Define the special regex characters.
 *
 * This macro is used so we can pick the largest character at compile time
 * since that gives us a valid sizeof().
 *
 * \note
 * For proper ECMA regex we need to have the `'/'`. Either way, it
 * is probably safer to have it (for someone may also want to create
 * regular expressions to use with a command such as `sed`). We may want
 * to change this parameter with a dynamic one so we can choose the type
 * of regex we are dealing with and whether such and such character is
 * special or not.
 *
 * \warning
 * The characters have to be sorted by their ASCII code.
 */
#define SNAPDEV_SPECIAL_REGEX_CHARACTERS    "$()*+./?[\\]^{|}"


/** \brief List of special regular expression characters.
 *
 * These metacharacters are used in regular expressions with some meaning.
 * If not escaped you may run in some problems.
 *
 * The list is sorted which allows us to break the search as soon as the
 * character `c` is smaller than the current special character.
 *
 * * '$' -- the end character (0x24)
 * * '(' -- the start group (0x28)
 * * ')' -- the end group (0x29)
 * * '*' -- the or operator (0x2A)
 * * '+' -- the one or more character (0x2B)
 * * '.' -- the any character (0x2E)
 * * '?' -- the or operator (0x3F)
 * * '[' -- the start character set (0x5B)
 * * '\\' -- the escape character (0x5C)
 * * ']' -- the end character set (0x5D)
 * * '^' -- the start character (0x5E)
 * * '{' -- the start repeat specification (0x7B)
 * * '|' -- the or operator (0x7C)
 * * '}' -- the end repeat specification (0x7D)
 */
char const * g_special_regex_characters = SNAPDEV_SPECIAL_REGEX_CHARACTERS;

char const g_min_special_regex_character = g_special_regex_characters[0];
char const g_max_special_regex_character = g_special_regex_characters[sizeof(SNAPDEV_SPECIAL_REGEX_CHARACTERS) - 2];



} // no name namespace


/** \brief Convert a string so it can be used as is in a regular expression.
 *
 * This function goes through the input string and escapes all the
 * characters that are considered special (also called metacharacters).
 * The output can be used as is in a regular expression.
 *
 * \note
 * The function is optimized for speed.
 *
 * Based on function found here:
 * https://stackoverflow.com/questions/39228912/stdregex-escape-special-characters-for-use-in-regex
 *
 * \tparam CharT  The type of character the string uses.
 * \tparam Traits  The traits of the characters.
 * \tparam Allocator  The functions used to allocate this type of string.
 *
 * \param[in] s  The string to properly escape for a regular expression.
 *
 * \return The properly escaped string for a regular expression.
 */
template<
    class CharT,
    class Traits = std::char_traits<CharT>,
    class Allocator = std::allocator<CharT>>
std::basic_string<CharT, Traits, Allocator> escape_special_regex_characters(std::basic_string<CharT, Traits, Allocator> const & s)
{
#ifdef _DEBUG
    for(char const * r(g_special_regex_characters + 1); *r != '\0'; ++r)
    {
        if(r[-1] >= r[0])
        {
            throw std::logic_error("g_special_regex_characters characters are not properly sorted."); // LCOV_EXCL_LINE
        }
    }
    if(g_special_regex_characters[0] != g_min_special_regex_character)
    {
        throw std::logic_error("g_min_special_regex_character is not the minimum."); // LCOV_EXCL_LINE
    }
    if(g_special_regex_characters[strlen(g_special_regex_characters) - 1] != g_max_special_regex_character)
    {
        throw std::logic_error( // LCOV_EXCL_LINE
              "g_max_special_regex_character ("
            + std::to_string(static_cast<int>(g_max_special_regex_character))
            + ") is not the maximum (sizeof = "
            + std::to_string(sizeof(SNAPDEV_SPECIAL_REGEX_CHARACTERS))
            + ", strlen = "
            + std::to_string(strlen(g_special_regex_characters))
            + ").");
    }
#endif

    std::string result;
    result.reserve(s.size() * 2);
    for(auto const c : s)
    {
        if(c >= g_min_special_regex_character
        && c <= g_max_special_regex_character)
        {
            for(char const * r(g_special_regex_characters); *r != '\0'; ++r)
            {
                if(*r == c)
                {
                    result += '\\';
                    break;
                }
                if(*r > c)
                {
                    break;
                }
            }
        }
        result += c;
    }

    return result;
} // LCOV_EXCL_LINE



} // namespace snapdev
// vim: ts=4 sw=4 et
