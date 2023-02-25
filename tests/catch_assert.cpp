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

/** \file
 * \brief Verify that the SAFE_ASSERT function works.
 *
 * This file verifies we can use the SAFE_ASSERT function as expected.
 */

// self
//
#include    <snapdev/safe_assert.h>

#include    "catch_main.h"


// last include
//
#include    <snapdev/poison.h>




CATCH_TEST_CASE("safe_assert", "[foo]")
{
    CATCH_START_SECTION("safe_assert: make sure assert can pass")
    {
        constexpr bool valid(true);
        snapdev::SAFE_ASSERT(valid, "this works");
        CATCH_REQUIRE(valid);
    }
    CATCH_END_SECTION()

#ifdef NDEBUG
    CATCH_START_SECTION("safe_assert: test invalid assert in Release mode")
    {
        constexpr bool invalid(false);
        snapdev::SAFE_ASSERT(invalid, "this \"passes\"", ", but really it's not compiled in in Release mode");
        CATCH_REQUIRE_FALSE(invalid);
    }
    CATCH_END_SECTION()
#endif
}



// vim: ts=4 sw=4 et
