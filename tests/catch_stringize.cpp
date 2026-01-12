// Copyright (c) 2023-2026  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Verify that SNAPDEV_STRINGIZE() works as expected.
 *
 * This file implements tests to verify the stringize macro.
 */

// self
//
#include    <snapdev/stringize.h>

#include    "catch_main.h"


// last include
//
#include    <snapdev/poison.h>



CATCH_TEST_CASE("stringize", "[string]")
{
    CATCH_START_SECTION("stringize: build year")
    {
        std::string copyright("Copyright (c) 2023-"
            SNAPDEV_STRINGIZE(UTC_BUILD_YEAR)
            " by Made to Order Software Corporation, All Rights Reserved");

        std::stringstream ss;
        ss << "Copyright (c) 2023-";
        ss << UTC_BUILD_YEAR;
        ss << " by Made to Order Software Corporation, All Rights Reserved";

        CATCH_REQUIRE(ss.str() == copyright);
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
