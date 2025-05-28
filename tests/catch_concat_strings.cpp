// Copyright (c) 2024-2025  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Verify the concat_string templates.
 *
 * This file implements tests for the concat_string templates.
 */

// self
//
#include    <snapdev/concat_strings.h>

#include    <snapdev/is_string_literal.h>

#include    "catch_main.h"


// last include
//
#include    <snapdev/poison.h>


namespace
{


constexpr char const hello[] = "hello ";
constexpr char const world[] = "world!";


} // no name namespace

CATCH_TEST_CASE("concat_strings", "[string]")
{
    CATCH_START_SECTION("concat_strings: concatenate two strings at compile time")
    {
        char const * const r = snapdev::concat<hello, world>::value;
        CATCH_REQUIRE(strcmp(r, "hello world!") == 0);
        CATCH_REQUIRE(snapdev::is_string_literal(snapdev::concat<hello, world>::value));
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
