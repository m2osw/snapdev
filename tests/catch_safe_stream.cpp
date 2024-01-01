// Copyright (c) 2018-2024  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Verify that the safe_stream class works.
 *
 * This file implements tests for the safe_stream class to make sure that
 * the flags are indeed safe.
 */

// self
//
#include    <snapdev/safe_stream.h>

#include    "catch_main.h"


// C++
//
#include    <sstream>


// last include
//
#include    <snapdev/poison.h>




CATCH_TEST_CASE("safe_stream", "[stream][raii][os]")
{
    CATCH_START_SECTION("safe_stream: hex/oct")
    {
        {
            std::stringstream ss;

            std::ios_base::fmtflags const flags(ss.flags());

            {
                snapdev::safe_stream safe(ss);
                ss << std::hex << 123 << '\n';
                CATCH_REQUIRE("7b\n" == ss.str());
            }
            CATCH_REQUIRE(flags == ss.flags());

            {
                snapdev::safe_stream safe(ss);
                ss << std::oct << 123 << '\n';
                CATCH_REQUIRE("7b\n173\n" == ss.str());
            }
            CATCH_REQUIRE(flags == ss.flags());
        }
    }
    CATCH_END_SECTION()

    // TODO: test precision, width, user defined flags...
}



// vim: ts=4 sw=4 et

