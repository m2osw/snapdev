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
 * \brief Verify that the file_contents class works.
 *
 * This file implements tests for the file_content class.
 */

// self
//
#include    <snapdev/file_contents.h>

#include    "catch_main.h"


// last include
//
#include    <snapdev/poison.h>




CATCH_TEST_CASE("file_contents", "[os]")
{
    CATCH_START_SECTION("file_contents: write -> read contents")
    {
        // try to create file, but directory is missing
        {
            std::string const content("Open fails in this case\n");
            std::string const filename(SNAP_CATCH2_NAMESPACE::g_tmp_dir() + "/inexcistant-directory/test-file.txt");

            {
                snap::file_contents content_test_output(filename, false);
                content_test_output.contents(content);
                CATCH_REQUIRE_FALSE(content_test_output.write_all());
                CATCH_REQUIRE(content_test_output.last_error() == "could not open file \""
                                                                      + filename
                                                                      + "\" for writing.");
            }

            {
                snap::file_contents content_test_input(filename);
                CATCH_REQUIRE_FALSE(content_test_input.read_all());
                CATCH_REQUIRE_FALSE(content_test_input.contents() == content);
            }
        }

        // create directory & file
        {
            std::string const content("We're testing a write...\nand then a read\n");
            std::string const filename(SNAP_CATCH2_NAMESPACE::g_tmp_dir() + "/contents/test-file.txt");

            {
                snap::file_contents content_test_output(filename, true);
                content_test_output.contents(content);
                CATCH_REQUIRE(content_test_output.write_all());
            }

            {
                snap::file_contents content_test_input(filename);
                CATCH_REQUIRE(content_test_input.read_all());
                CATCH_REQUIRE(static_cast<snap::file_contents const &>(content_test_input).contents() == content);
            }
        }

        // check that if the directory exists, it works as expected
        {
            std::string const content("Try again without the create directory and it still works\n");
            std::string const filename(SNAP_CATCH2_NAMESPACE::g_tmp_dir() + "/contents/dir-exists.txt");

            {
                snap::file_contents content_test_output(filename, false);
                content_test_output.contents(content);
                CATCH_REQUIRE(content_test_output.write_all());
            }

            {
                snap::file_contents content_test_input(filename);
                CATCH_REQUIRE(content_test_input.read_all());
                CATCH_REQUIRE(content_test_input.contents() == content);
            }
        }

        {
            std::string const filename(SNAP_CATCH2_NAMESPACE::g_tmp_dir() + "/contents/dir-exists.txt/object.java");

            CATCH_REQUIRE_THROWS_MATCHES(
                      snap::file_contents(filename, true, true)
                    , std::ios::failure
                    , Catch::Matchers::ExceptionMessage(
                              "snapdev::file_contents: the full path to filename for a file_contents object could not be created: iostream error"));
        }

        // this one tries to delete the "contents" directory and that fails
        // since it's a directory (and it's not even empty)
        {
            std::string const filename(SNAP_CATCH2_NAMESPACE::g_tmp_dir() + "/contents");
            snap::file_contents content_test_output(filename, false, true);
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("file_contents: write temporary -> read fails")
    {
        std::string const content("We're testing a write...\nand then a read but the file is gone!\n");
        std::string const filename(SNAP_CATCH2_NAMESPACE::g_tmp_dir() + "/contents/temporary-file.txt");

        {
            snap::file_contents content_test_output(filename, true, true);
            content_test_output.contents(content);
            CATCH_REQUIRE(content_test_output.write_all());
        }

        {
            snap::file_contents content_test_input(filename);
            CATCH_REQUIRE_FALSE(content_test_input.read_all());
            CATCH_REQUIRE(content_test_input.contents() == std::string());
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("file_contents: empty filename is not accepted")
    {
        CATCH_REQUIRE_THROWS_MATCHES(
                  snap::file_contents(std::string(), true, true)
                , std::invalid_argument
                , Catch::Matchers::ExceptionMessage(
                          "snapdev::file_contents: the filename of a file_contents object cannot be the empty string."));
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
