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

/** \file
 * \brief Verify that the timed_getch compiles & times out.
 *
 * Since we cannot run standalone tests if we need the user to hit a key
 * on his keyboard this test is limited to a timeout.
 *
 * At least we make sure that the file compiles that way.
 */

// self
//
#include    <snapdev/timed_getchar.h>

#include    "catch_main.h"


// snapdev
//
#include    <snapdev/not_reached.h>


// last include
//
#include    <snapdev/poison.h>


namespace
{




} // no name namespace


CATCH_TEST_CASE("timed_getchar_timeout", "[terminal]")
{
    CATCH_START_SECTION("timed_getchar_timeout: make sure that our getchar() times out")
    {
        int const r(snapdev::timed_getchar(0.3));
        CATCH_REQUIRE(r == 0); // 0 in case of a timeout (-1 error, more than 0 user hit a character)
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
