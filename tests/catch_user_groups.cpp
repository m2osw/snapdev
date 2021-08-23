// Copyright (c) 2018-2021  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Verify that the user_groups() function works.
 *
 * This file implements tests for the user_groups() function.
 */

// self
//
#include    <snapdev/user_groups.h>

#include    <snapdev/file_contents.h>
#include    <snapdev/join_strings.h>
#include    <snapdev/ostream_to_buf.h>
#include    <snapdev/tokenize_string.h>

#include    "catch_main.h"


// last include
//
#include    <snapdev/poison.h>




CATCH_TEST_CASE("user_groups", "[os]")
{
    // Note: this test fails if you add/remove groups from your user
    //       and do not re-log-in; one way to test in that case:
    //
    //           $ id -ng
    //           alexis       <-- a.k.a. old group
    //           $ newgrp snapwebsites
    //           $ newgrp alexis
    //
    CATCH_START_SECTION("user_groups: groups")
    {
        std::string const filename(SNAP_CATCH2_NAMESPACE::g_tmp_dir + "/system-groups.txt");
        CATCH_REQUIRE(system(("groups >" + filename).c_str()) == 0);
        snap::file_contents system_groups(filename);
        system_groups.read_all();

        char const * user(getenv("USER"));
        CATCH_REQUIRE(user != nullptr);
        std::set<std::string> our_groups(snap::user_group_names<std::set<std::string>>(user));

        std::set<std::string> system_set;
        snap::tokenize_string(
                  system_set
                , system_groups.contents()
                , " \n"
                , true);

        CATCH_REQUIRE(system_set == our_groups);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("user_groups: invalid user")
    {
        std::set<std::string> our_groups(snap::user_group_names<std::set<std::string>>("invalid-user"));
        CATCH_REQUIRE(our_groups.empty());
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
