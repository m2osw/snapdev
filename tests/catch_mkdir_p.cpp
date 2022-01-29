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
 * \brief Verify that the mkdir_p() function works.
 *
 * This file implements tests for the mkdir_p() function.
 */

// self
//
#include    <snapdev/mkdir_p.h>

#include    <snapdev/file_contents.h>
#include    <snapdev/raii_generic_deleter.h>
#include    <snapdev/pathinfo.h>

#include    "catch_main.h"


// C lib
//
#include    <unistd.h>


// last include
//
#include    <snapdev/poison.h>




CATCH_TEST_CASE("mkdir_p", "[os]")
{
    CATCH_START_SECTION("mkdir_p: empty")
    {
        CATCH_REQUIRE(snapdev::mkdir_p(std::string(), true,  0700, "user",        "group")       == 0);
        CATCH_REQUIRE(snapdev::mkdir_p(std::string(), false, 0700, "user",        "group")       == 0);
        CATCH_REQUIRE(snapdev::mkdir_p(std::string(), true,  0775, "user",        "group")       == 0);
        CATCH_REQUIRE(snapdev::mkdir_p(std::string(), false, 0775, "user",        "group")       == 0);
        CATCH_REQUIRE(snapdev::mkdir_p(std::string(), true,  0700, std::string(), "group")       == 0);
        CATCH_REQUIRE(snapdev::mkdir_p(std::string(), false, 0700, std::string(), "group")       == 0);
        CATCH_REQUIRE(snapdev::mkdir_p(std::string(), true,  0775, std::string(), "group")       == 0);
        CATCH_REQUIRE(snapdev::mkdir_p(std::string(), false, 0775, std::string(), "group")       == 0);
        CATCH_REQUIRE(snapdev::mkdir_p(std::string(), true,  0700, "user",        std::string()) == 0);
        CATCH_REQUIRE(snapdev::mkdir_p(std::string(), false, 0700, "user",        std::string()) == 0);
        CATCH_REQUIRE(snapdev::mkdir_p(std::string(), true,  0775, "user",        std::string()) == 0);
        CATCH_REQUIRE(snapdev::mkdir_p(std::string(), false, 0775, "user",        std::string()) == 0);
        CATCH_REQUIRE(snapdev::mkdir_p(std::string(), true,  0700, std::string(), std::string()) == 0);
        CATCH_REQUIRE(snapdev::mkdir_p(std::string(), false, 0700, std::string(), std::string()) == 0);
        CATCH_REQUIRE(snapdev::mkdir_p(std::string(), true,  0775, std::string(), std::string()) == 0);
        CATCH_REQUIRE(snapdev::mkdir_p(std::string(), false, 0775, std::string(), std::string()) == 0);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("mkdir_p: create")
    {
        // TODO: for a complete test, we would need to transform the g_tmp_dir
        //       in a relative path, which is complicated at the moment...
        //
        std::string const filename(SNAP_CATCH2_NAMESPACE::g_tmp_dir() + "/with/extra/sub-dirs/and-a-file.txt");
        CATCH_REQUIRE(snapdev::mkdir_p(filename, true, 0700) == 0);

        // the file itself needs to not exist
        //
        struct stat st;
        CATCH_REQUIRE(stat(filename.c_str(), &st) != 0);

        // the last directory needs to be 0700
        //
        std::string const basename(snapdev::pathinfo::dirname(filename));
        CATCH_REQUIRE(stat(basename.c_str(), &st) == 0);
        CATCH_REQUIRE(S_ISDIR(st.st_mode));
        CATCH_REQUIRE((st.st_mode & 0777) == 0700);

        // re-create with a different mode changes it
        //
        CATCH_REQUIRE(snapdev::mkdir_p(filename, true, 0755) == 0);
        CATCH_REQUIRE(stat(basename.c_str(), &st) == 0);
        CATCH_REQUIRE(S_ISDIR(st.st_mode));
        CATCH_REQUIRE((st.st_mode & 0777) == 0755);

        // last component is a file, that creates an error
        //
        snapdev::file_contents actual_file(filename);
        actual_file.contents("test file--testing that create folder fails if a file exists\n");
        actual_file.write_all();

        int const r(snapdev::mkdir_p(filename, false, 0700));
        int const e(errno);
        CATCH_REQUIRE(r == -1);
        CATCH_REQUIRE(e == EEXIST);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("mkdir_p: create full path")
    {
        std::unique_ptr<char *, snapdev::raii_generic_deleter<char *, nullptr, decltype(&::free), &::free>> cwd(get_current_dir_name());
        std::string const filename(SNAP_CATCH2_NAMESPACE::g_tmp_dir() + "/test/with/root/path");
        CATCH_REQUIRE(snapdev::mkdir_p(filename, false, 0750) == 0);

        // the last directory needs to be 0700
        //
        std::string const basename(snapdev::pathinfo::dirname(filename));
        struct stat st;
        CATCH_REQUIRE(stat(basename.c_str(), &st) == 0);
        bool is_dir(S_ISDIR(st.st_mode));
        CATCH_REQUIRE(is_dir);
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
