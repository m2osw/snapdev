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
 * \brief Verify that the hexadecimal convertions work.
 *
 * This file implements tests for the hexadecimal to binary and vice
 * versa functions.
 */

// self
//
#include    <snapdev/compare_switch_string.h>

#include    "catch_main.h"



// C++
//
#include    <iomanip>
//#include    <set>


// last include
//
#include    <snapdev/poison.h>



CATCH_TEST_CASE("compare_switch_string", "[string]")
{
    CATCH_START_SECTION("compare_switch_string: verify compare works as expected")
    {
        char const * strings[] = {
            "CREATE",
            "CONTEXT",
            "FROM",
            "SELECT",
            "ORDER",
            "INDEX",
            "Connect",
        };
        for(auto const & s : strings)
        {
            switch(*s)
            {
            case 'C':
                if(snapdev::compare_switch_string<"CREATE">(s))
                {
                    CATCH_REQUIRE(strcmp(s, "CREATE") == 0);
                    CATCH_REQUIRE(snapdev::compare_upper_switch_string<"Create">(s));
                }
                if(snapdev::compare_switch_string<"CREATED">(s))
                {
                    CATCH_REQUIRE(strcmp(s, "CREATED") == 0);
                }
                if(snapdev::compare_switch_string<"CONTEXT">(s))
                {
                    CATCH_REQUIRE(strcmp(s, "CONTEXT") == 0);
                    CATCH_REQUIRE_FALSE(snapdev::compare_switch_string<"Context">(s));
                    CATCH_REQUIRE(snapdev::compare_upper_switch_string<"context">(s));
                }
                if(snapdev::compare_switch_string<"COMMENT">(s))
                {
                    CATCH_REQUIRE(strcmp(s, "COMMENT") == 0);
                }
                if(snapdev::compare_upper_switch_string<"CONNECT">(s))
                {
                    CATCH_REQUIRE(strcmp(s, "Connect") == 0);
                }
                break;

            case 'F':
                if(snapdev::compare_switch_string<"FIRST">(s))
                {
                    CATCH_REQUIRE(strcmp(s, "FIRST") == 0);
                }
                if(snapdev::compare_switch_string<"FROM">(s))
                {
                    CATCH_REQUIRE(strcmp(s, "FROM") == 0);
                }
                if(snapdev::compare_switch_string<"FORWARD">(s))
                {
                    CATCH_REQUIRE(strcmp(s, "FORWARD") == 0);
                }
                break;

            case 'I':
                if(snapdev::compare_switch_string<"IN">(s))
                {
                    CATCH_REQUIRE(strcmp(s, "IN") == 0);
                }
                if(snapdev::compare_switch_string<"INDEX">(s))
                {
                    CATCH_REQUIRE(strcmp(s, "INDEX") == 0);
                }
                if(snapdev::compare_switch_string<"IMPORT">(s))
                {
                    CATCH_REQUIRE(strcmp(s, "IMPORT") == 0);
                }
                break;

            case 'O':
                if(snapdev::compare_switch_string<"OR">(s))
                {
                    CATCH_REQUIRE(strcmp(s, "OR") == 0);
                }
                if(snapdev::compare_switch_string<"ORDER">(s))
                {
                    CATCH_REQUIRE(strcmp(s, "ORDER") == 0);
                }
                if(snapdev::compare_switch_string<"OLDER">(s))
                {
                    CATCH_REQUIRE(strcmp(s, "OLDER") == 0);
                }
                break;

            case 'S':
                if(snapdev::compare_switch_string<"SORT">(s))
                {
                    CATCH_REQUIRE(strcmp(s, "SORT") == 0);
                }
                if(snapdev::compare_switch_string<"SELECT">(s))
                {
                    CATCH_REQUIRE(strcmp(s, "SELECT") == 0);
                }
                if(snapdev::compare_switch_string<"SET">(s))
                {
                    CATCH_REQUIRE(strcmp(s, "SET") == 0);
                }
                if(snapdev::compare_switch_string<"SELL">(s))
                {
                    CATCH_REQUIRE(strcmp(s, "SELL") == 0);
                }
                break;

            }
        }
    }
    CATCH_END_SECTION()
}


CATCH_TEST_CASE("compare_switch_string_errors", "[string][error]")
{
    CATCH_START_SECTION("compare_switch_string_errors: check first letter in debug")
    {
#ifdef _DEBUG
        CATCH_REQUIRE_THROWS_MATCHES(
                  snapdev::compare_switch_string<"QUIT">("KUIT")
                , std::runtime_error
                , Catch::Matchers::ExceptionMessage("first letter invalid: Q <-> KUIT."));

        CATCH_REQUIRE_THROWS_MATCHES(
                  snapdev::compare_upper_switch_string<"QUIT">("KUIT")
                , std::runtime_error
                , Catch::Matchers::ExceptionMessage("first letter invalid: Q <-> KUIT."));
#else
      // in release, it thinks the strings are equal (not a bug)
      //
      CATCH_REQUIRE(snapdev::compare_switch_string<"QUIT">("KUIT"));
      CATCH_REQUIRE(snapdev::compare_upper_switch_string<"QUIT">("KUIT"));
#endif
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
