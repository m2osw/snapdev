// Copyright (c) 2018-2026  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Verify that the mkdir_p() function works.
 *
 * This file implements tests for the mkdir_p() function.
 */

// self
//
#include    <snapdev/isatty.h>

#include    "catch_main.h"


// C
//
//#include    <unistd.h>


// last include
//
#include    <snapdev/poison.h>




CATCH_TEST_CASE("isatty", "[os]")
{
    CATCH_START_SECTION("isatty: stringstream")
    {
        std::stringstream ss;
        CATCH_REQUIRE_FALSE(snapdev::isatty(ss));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("isatty: cout/cerr/clog/cin")
    {
        // while running in a console, these work as expected (u.e. they
        // are TTYs) but when running the coverage test, we also pipe the
        // output to a file and it means stdout and stderr are not TTYs
        //
        if(isatty(fileno(stdin)))
        {
            CATCH_REQUIRE(snapdev::isatty(std::cin));
        }
        else
        {
            CATCH_REQUIRE_FALSE(snapdev::isatty(std::cin));
        }
        if(isatty(fileno(stdout)))
        {
            CATCH_REQUIRE(snapdev::isatty(std::cout));
        }
        else
        {
            CATCH_REQUIRE_FALSE(snapdev::isatty(std::cout));
        }
        if(isatty(fileno(stderr)))
        {
            CATCH_REQUIRE(snapdev::isatty(std::cerr));
            CATCH_REQUIRE(snapdev::isatty(std::clog));
        }
        else
        {
            CATCH_REQUIRE_FALSE(snapdev::isatty(std::cerr));
            CATCH_REQUIRE_FALSE(snapdev::isatty(std::clog));
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("isatty: /dev/tty")
    {
        // opening the /dev/tty works only when running the test from a
        // terminal; from a cron job, it fails
        //
        std::fstream out;
        out.open("/dev/tty");
        if(out.is_open())
        {
            CATCH_REQUIRE(snapdev::isatty(out));
        }
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
