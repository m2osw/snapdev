// Copyright (c) 2021  Made to Order Software Corp.  All Rights Reserved
//
// https://snapwebsites.org/project/snapdev
// contact@m2osw.com
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

/** \file
 * \brief Verify that the path functions work.
 *
 * This file implements tests for the pathinfo function.
 */

// self
//
#include    <snapdev/pathinfo.h>

#include    "catch_main.h"


// C++ lib
//
#include    <set>


// last include
//
#include    <snapdev/poison.h>




CATCH_TEST_CASE("pathinfo_replace_suffix", "[pathinfo]")
{
    CATCH_START_SECTION("pathinfo: replace existing suffix")
    {
        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "/full/path/example/pathinfo.cpp"
                , ".cpp"
                , ".h") == "/full/path/example/pathinfo.h");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "/full/path/example/pathinfo.h"
                , ".h"
                , ".cpp") == "/full/path/example/pathinfo.cpp");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "relative/path/pathinfo.cpp"
                , ".cpp"
                , ".h") == "relative/path/pathinfo.h");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "relative/path/pathinfo.h"
                , ".h"
                , ".cpp") == "relative/path/pathinfo.cpp");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "no-path.cpp"
                , ".cpp"
                , ".h") == "no-path.h");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "no-path.h"
                , ".h"
                , ".cpp") == "no-path.cpp");
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("pathinfo: replace non-existant suffix")
    {
        std::string full_path_c("/full/path/example/pathinfo.c");
        std::string full_path_hpp("/full/path/example/pathinfo.hpp");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix(
                  full_path_c
                , ".cpp"
                , ".h") == "/full/path/example/pathinfo.c.h");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix(
                  full_path_hpp
                , ".h"
                , ".cpp") == "/full/path/example/pathinfo.hpp.cpp");

        std::string relative_path_c("relative/path/pathinfo.c");
        std::string relative_path_hpp("relative/path/pathinfo.hpp");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix(
                  relative_path_c
                , ".cpp"
                , ".h") == "relative/path/pathinfo.c.h");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix(
                  relative_path_hpp
                , ".h"
                , ".cpp") == "relative/path/pathinfo.hpp.cpp");

        std::string no_path_c("no-path.c");
        std::string no_path_hpp("no-path.hpp");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix(
                  no_path_c
                , ".cpp"
                , ".h") == "no-path.c.h");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix(
                  no_path_hpp
                , ".h"
                , ".cpp") == "no-path.hpp.cpp");
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("pathinfo: remove suffix when present")
    {
        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "/full/path/example/pathinfo.cpp"
                , ".cpp") == "/full/path/example/pathinfo");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "/full/path/example/pathinfo.h"
                , ".h") == "/full/path/example/pathinfo");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "relative/path/pathinfo.cpp"
                , ".cpp") == "relative/path/pathinfo");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "relative/path/pathinfo.h"
                , ".h") == "relative/path/pathinfo");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "no-path.cpp"
                , ".cpp") == "no-path");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "no-path.h"
                , ".h") == "no-path");
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("pathinfo: remove suffix when absent")
    {
        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "/full/path/example/pathinfo.c"
                , ".cpp") == "/full/path/example/pathinfo.c");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "/full/path/example/pathinfo.hpp"
                , ".h") == "/full/path/example/pathinfo.hpp");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "relative/path/pathinfo.c"
                , ".cpp") == "relative/path/pathinfo.c");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "relative/path/pathinfo.hpp"
                , ".h") == "relative/path/pathinfo.hpp");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "no-path.c"
                , ".cpp") == "no-path.c");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "no-path.hpp"
                , ".h") == "no-path.hpp");
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("pathinfo: do nothing if non-existant suffix")
    {
        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "/full/path/example/pathinfo.c"
                , ".cpp"
                , ".h"
                , true) == "/full/path/example/pathinfo.c");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "/full/path/example/pathinfo.c"
                , ".cpp"
                , ""
                , true) == "/full/path/example/pathinfo.c");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "/full/path/example/pathinfo.hpp"
                , ".h"
                , ".cpp"
                , true) == "/full/path/example/pathinfo.hpp");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "/full/path/example/pathinfo.hpp"
                , ".h"
                , std::string()
                , true) == "/full/path/example/pathinfo.hpp");
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
