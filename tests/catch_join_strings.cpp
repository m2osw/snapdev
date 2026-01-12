// Copyright (c) 2011-2026  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Verify the join_strings function & templates.
 *
 * This file implements tests to verify that the join_strings function works
 * as expected.
 *
 * Further it verifies that string concatenation happens as expected.
 */

// snapdev
//
#include    <snapdev/join_strings.h>

#include    <snapdev/to_string_literal.h>


// self
//
#include    "catch_main.h"



namespace
{

// example taken from communicatord project
//
constexpr int const         LOCAL_PORT = 4040;
constexpr std::string_view  g_communicatord_default_ip = "127.0.0.1";
constexpr std::string_view  g_communicatord_default_port = snapdev::integer_to_string_literal<LOCAL_PORT>.data();
constexpr std::string_view  g_communicatord_colon = ":";
constexpr std::string_view  g_communicatord_default_ip_port = snapdev::join_string_views<g_communicatord_default_ip, g_communicatord_colon, g_communicatord_default_port>;

constexpr std::string_view  g_communicatord_default_ip_port_v2 = snapdev::join_string_views_with_separator<g_communicatord_colon, g_communicatord_default_ip, g_communicatord_default_port>;


}


CATCH_TEST_CASE("join_string_view", "[string]")
{
    CATCH_START_SECTION("join_string_view: join strings at compile time")
    {
        std::string concat(g_communicatord_default_ip);
        concat += ':';
        concat += std::to_string(LOCAL_PORT);

        // the cast here is because catch2 doesn't handle the
        //
        //     string == string_view
        //
        CATCH_REQUIRE(concat == std::string(g_communicatord_default_ip_port));
        CATCH_REQUIRE(concat == std::string(g_communicatord_default_ip_port_v2));
    }
    CATCH_END_SECTION()
}


CATCH_TEST_CASE("join_strings", "[string]")
{
    CATCH_START_SECTION("join_strings: join strings at runtime with empty string in between")
    {
        std::string manual_concat(g_communicatord_default_ip);
        manual_concat += ':';
        manual_concat += std::to_string(LOCAL_PORT);

        std::vector<std::string> const list = {
                  std::string(g_communicatord_default_ip)
                , std::string(":")
                , std::string(g_communicatord_default_port)
            };
        std::string const join_concat(snapdev::join_strings(list, ""));

        CATCH_REQUIRE(manual_concat == join_concat);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("join_strings: join strings at runtime with commas")
    {
        std::vector<std::string> const list = {
                  "Item 1"
                , "Item 2"
                , "Item 3"
            };
        std::string const join_concat(snapdev::join_strings(list, ", "));

        CATCH_REQUIRE(join_concat == "Item 1, Item 2, Item 3");
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("join_strings: \"join\" one string with commas")
    {
        std::vector<std::string> const list = { "Item 1" };
        std::string const join_concat(snapdev::join_strings(list, ", "));

        CATCH_REQUIRE(join_concat == "Item 1");
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("join_strings: nothing to join with commas")
    {
        std::vector<std::string> const list;
        std::string const join_concat(snapdev::join_strings(list, ", "));

        CATCH_REQUIRE(join_concat == std::string());
    }
    CATCH_END_SECTION()
}


// vim: ts=4 sw=4 et
