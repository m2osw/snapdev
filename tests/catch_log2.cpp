// Copyright (c) 2018-2023  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Verify that the log2() function.
 *
 * This file implements test for the log2() function.
 */

// self
//
#include    "catch_main.h"



// snapdev lib
//
#include    <snapdev/log2.h>


// last include
//
#include    <snapdev/poison.h>






CATCH_TEST_CASE("log2", "[math]")
{
    CATCH_START_SECTION("log2: zero")
    {
        CATCH_REQUIRE(snapdev::log2(0) == -1);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("log2: powers of 2")
    {
        for(int i = 0; i < 64; ++i)
        {
            std::uint64_t const v = 1ULL << i;
            CATCH_REQUIRE(snapdev::log2(v) == i);
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("log2: random numbers")
    {
        for(int i = 0; i < 64; ++i)
        {
            std::uint64_t r = (static_cast<std::uint64_t>(rand()) << 32) ^ static_cast<std::uint64_t>(rand());
            r &= (1ULL << i) - 1ULL;
            std::uint64_t const v = (1ULL << i) | r;

            // same difference, all the lower bits are ignored in the
            // computation
            //
            CATCH_REQUIRE(snapdev::log2(v) == i);
        }
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
