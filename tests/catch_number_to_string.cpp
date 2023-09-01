// Copyright (c) 2021-2023  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Verify the various \<number>_to_string() functions.
 *
 * This checks a few transformations to make sure they work as expected.
 *
 * The supported functions are:
 *
 * \li integer_to_string()
 */

// self
//
#include    "catch_main.h"



// snapdev
//
#include    <snapdev/number_to_string.h>


// last include
//
#include    <snapdev/poison.h>


// __int128 is not ISO C++ yet
#pragma GCC diagnostic ignored "-Wpedantic"


CATCH_TEST_CASE("number_to_string(int)", "[string]")
{
    CATCH_START_SECTION("number_to_string(int): convert integers (int) to a string")
    {
        for(int i(0); i < 1000; ++i)
        {
            int n(rand());
            { // default base
                std::string const result(snapdev::integer_to_string(n));
                std::string const expected(std::to_string(n));
                CATCH_REQUIRE(expected == result);
            }
            { // explicit base 10
                std::string const result(snapdev::integer_to_string(n, 10));
                std::string const expected(std::to_string(n));
                CATCH_REQUIRE(expected == result);
            }
            { // base 8
                std::string const result(snapdev::integer_to_string(n, 8));
                std::stringstream ss;
                ss << std::oct << n;
                std::string const expected(ss.str());
                CATCH_REQUIRE(expected == result);
            }
            { // base 16 -- lowercase (default)
                std::string const result(snapdev::integer_to_string(n, 16));
                std::stringstream ss;
                ss << std::hex << n;
                std::string const expected(ss.str());
                CATCH_REQUIRE(expected == result);
            }
            { // base 16 -- lowercase (explicit)
                std::string const result(snapdev::integer_to_string(n, 16, false));
                std::stringstream ss;
                ss << std::hex << n;
                std::string const expected(ss.str());
                CATCH_REQUIRE(expected == result);
            }
            { // base 16 -- uppercase
                std::string const result(snapdev::integer_to_string(n, 16, true));
                std::stringstream ss;
                ss << std::hex << std::uppercase << n;
                std::string const expected(ss.str());
                CATCH_REQUIRE(expected == result);
            }
        }
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
