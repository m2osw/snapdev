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

/** \file
 * \brief Verify the NOT_USED() functions.
 *
 * This file implements tests to verify that the unique_number implementation
 * works properly even when multiple processes use it simultaneously.
 */

// file being tested
//
#include    <snapdev/not_used.h>


// self
//
#include    "catch_main.h"


// last include
//
#include    <snapdev/poison.h>



namespace
{



int verify_void(int a)
{
    snapdev::NOT_USED();
    return a;
}


int verify(int a, long b, float c)
{
    snapdev::NOT_USED(b, c);
    return a;
}



} // no name namespace



CATCH_TEST_CASE("not_used", "[not_used]")
{
    CATCH_START_SECTION("not_used: verify NOT_USED() without parameters")
    {
        CATCH_REQUIRE(verify_void(1409) == 1409);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("not_used: verify NOT_USED() works as expected (2 parameters)")
    {
        CATCH_REQUIRE(verify(34, -1000, 3.1409) == 34);
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
