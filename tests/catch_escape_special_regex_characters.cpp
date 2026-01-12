// Copyright (c) 2021-2026  Made to Order Software Corp.  All Rights Reserved
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

/** \file
 * \brief Verify the escaping of regex special characters.
 *
 * At times you want to build a regular expression dynamically with parts
 * of the expression including user entered characters. Those character
 * usually need to be escaped to work properly in a regular expression.
 * For example, the period (.) character has a special meaning in a
 * regular expression (usually, all character except '\\n' or \\r').
 * For that character to represent a period in the source string of
 * the regular expression, it needs to be escaped (preceded by a
 * backslash character). This function does that on all the regular
 * expression characters.
 *
 * This file implements tests to verify that all the special characters
 * are indeed escaped.
 */

// self
//
#include    <snapdev/escape_special_regex_characters.h>

#include    "catch_main.h"



// last include
//
#include    <snapdev/poison.h>



namespace
{





} // no name namespace



CATCH_TEST_CASE("escape_special_regex_characters", "[regex][string]")
{
    CATCH_START_SECTION("escape_special_regex_characters: verify that all special characters are indeed escaped")
    {
        char const special_characters[] = {
            '$',
            '(',
            ')',
            '*',
            '+',
            '.',
            '/',
            '?',
            '[',
            '\\',
            ']',
            '^',
            '{',
            '|',
            '}',
            '\0',
        };

        int pos(0);
        for(int c(1); c < 128; ++c)
        {
            std::string const in(1, static_cast<char>(c));
            std::string const out(snapdev::escape_special_regex_characters(in));
            if(c == special_characters[pos])
            {
                std::string const expected('\\' + in);
                CATCH_REQUIRE(expected == out);
                ++pos;
            }
            else
            {
                CATCH_REQUIRE(in == out);
            }
        }
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
