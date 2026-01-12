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
 * \brief Verify that the username() function works.
 *
 * This file implements tests for the username() function.
 */

// self
//
#include    <snapdev/username.h>

//#include    <snapdev/file_contents.h>
//#include    <snapdev/user_groups.h>

#include    "catch_main.h"


// C++
//
//#include    <set>


// last include
//
#include    <snapdev/poison.h>




CATCH_TEST_CASE("username", "[os]")
{
    CATCH_START_SECTION("username: convert a UID to a user name")
    {
        {
            std::string const root(snapdev::username(0));
            CATCH_REQUIRE(root == "root");
        }

        {
            char const * user(getenv("USER"));
            if(user != nullptr)
            {
                std::string const name(snapdev::username());
                CATCH_REQUIRE(name == user);
            }
        }
    }
    CATCH_END_SECTION()
}


CATCH_TEST_CASE("username_invalid", "[os][error]")
{
    CATCH_START_SECTION("username: convert a UID to a user name")
    {
        std::string const non_existant(snapdev::username(999999999));
        CATCH_REQUIRE(errno == ENOENT);
        CATCH_REQUIRE(non_existant.empty());
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
