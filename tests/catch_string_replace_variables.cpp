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
 * \brief Verify the string_replace_variables() function.
 *
 * Make sure that it does replace variables as expected.
 */

// self
//
#include    <snapdev/string_replace_variables.h>

#include    "catch_main.h"


// C++
//
#include    <deque>
#include    <list>


// last include
//
#include    <snapdev/poison.h>



CATCH_TEST_CASE("string_replace_variables", "[string_replace_variables][string]")
{
    CATCH_START_SECTION("string_replace_variables: empty input string")
    {
        std::string const empty;
        std::string result(snapdev::string_replace_variables(empty));
        CATCH_REQUIRE(empty == result);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("string_replace_variables: replace $HOME or ${HOME}")
    {
        // we expect HOME to be set but if not use "/root"
        //
        char const * home(getenv("HOME"));
        if(home == nullptr)
        {
            setenv("HOME", "/root", 1);
            home = getenv("HOME");
            CATCH_REQUIRE(home != nullptr);
        }

        // try at the start of the string
        //
        std::string result(snapdev::string_replace_variables(std::string("$HOME/.cache/snapdev/memory.txt")));
        CATCH_REQUIRE(std::string(home) + "/.cache/snapdev/memory.txt" == result);

        result = snapdev::string_replace_variables(std::string("${HOME}/snapdev/intro"));
        CATCH_REQUIRE(std::string(home) + "/snapdev/intro" == result);

        // try in the middle of the string
        //
        result = snapdev::string_replace_variables(std::string("/magic:$HOME/snapdev"));
        CATCH_REQUIRE("/magic:" + std::string(home) + "/snapdev" == result);

        result = snapdev::string_replace_variables(std::string("/magic:${HOME}/snapdev"));
        CATCH_REQUIRE("/magic:" + std::string(home) + "/snapdev" == result);

        // try at the end of the string
        //
        result = snapdev::string_replace_variables(std::string("/var/lib/snapdev/$HOME"));
        CATCH_REQUIRE("/var/lib/snapdev/" + std::string(home) == result);

        result = snapdev::string_replace_variables(std::string("/run/${HOME}"));
        CATCH_REQUIRE("/run/" + std::string(home) == result);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("string_replace_variables: test _SNAPDEV and snapdev123 (underscore, lowercase, digits)")
    {
        // test with an underscore
        //
        setenv("_SNAPDEV", "special_name", 1);
        std::string result(snapdev::string_replace_variables(std::string("Name = $_SNAPDEV {working?}")));
        CATCH_REQUIRE("Name = special_name {working?}" == result);

        result = snapdev::string_replace_variables(std::string("Name between braces = ${_SNAPDEV} {working?}"));
        CATCH_REQUIRE("Name between braces = special_name {working?}" == result);

        // test with lowercase and digits
        //
        setenv("snapdev123", "Lowercase & Digits", 1);
        result = snapdev::string_replace_variables(std::string("Another variable {$snapdev123}"));
        CATCH_REQUIRE("Another variable {Lowercase & Digits}" == result);

        result = snapdev::string_replace_variables(std::string("Double braces: {${snapdev123}}"));
        CATCH_REQUIRE("Double braces: {Lowercase & Digits}" == result);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("string_replace_variables: replace $$ with pid")
    {
        std::string result(snapdev::string_replace_variables(std::string("/proc/$$/status")));
        CATCH_REQUIRE("/proc/" + std::to_string(getpid()) + "/status" == result);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("string_replace_variables: dollar at the end of the string")
    {
        std::string result(snapdev::string_replace_variables(std::string("/ignore/this$")));
        CATCH_REQUIRE("/ignore/this$" == result);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("string_replace_variables: no name")
    {
        std::string result(snapdev::string_replace_variables(std::string("/var/$/snapdev")));
        CATCH_REQUIRE("/var/$/snapdev" == result);

        result = snapdev::string_replace_variables(std::string("/var/${}/snapdev"));
        CATCH_REQUIRE("/var/${}/snapdev" == result);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("string_replace_variables: invalid name")
    {
        std::string result(snapdev::string_replace_variables(std::string("/var/${bad/snapdev")));
        CATCH_REQUIRE("/var/${bad/snapdev" == result);

        result = snapdev::string_replace_variables(std::string("/var/${not_closing"));
        CATCH_REQUIRE("/var/${not_closing" == result);
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
