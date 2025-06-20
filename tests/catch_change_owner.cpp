// Copyright (c) 2018-2025  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Verify that the chownnm() function works.
 *
 * This file implements tests for the chownnm() function.
 */

// self
//
#include    <snapdev/chownnm.h>

#include    <snapdev/file_contents.h>
#include    <snapdev/user_groups.h>

#include    "catch_main.h"


// C++ lib
//
#include    <set>


// last include
//
#include    <snapdev/poison.h>




CATCH_TEST_CASE("chownnm", "[os]")
{
    CATCH_START_SECTION("chownnm: change group")
    {
        struct group const * grp(getgrnam("snapwebsites"));
        if(grp == nullptr)
        {
            // adding a test otherwise this test fails when the snapwebsites
            // group is missing
            //
            CATCH_REQUIRE(grp == nullptr);
            std::cerr << "warning: skipping change group test because \"snapwebsites\" group doesn't exist.\n";
        }
        else
        {
            char const * user(getenv("USER"));
            bool permitted(true);
            CATCH_REQUIRE(user != nullptr);
            if(strcmp(user, "root") != 0)
            {
                std::set<std::string> our_groups(snapdev::user_group_names<std::set<std::string>>(user));
                if(our_groups.find("snapwebsites") == our_groups.end())
                {
                    permitted = false;
                    std::cerr << "error: we expect the tester to be the \"root\" user or part of the \"snapwebsites\" group to run this test section.\n";
                }
            }

            if(permitted)
            {
                std::string const filename(SNAP_CATCH2_NAMESPACE::g_tmp_dir() + "/group-test.txt");
                snapdev::file_contents system_groups(filename);
                system_groups.contents("test file--testing changing group\n");
                system_groups.write_all();

                struct stat st;
                CATCH_REQUIRE(stat(filename.c_str(), &st) == 0);
                if(st.st_gid == grp->gr_gid)
                {
                    std::cerr << "warning: your default group is \"snapwebsites\" so the test is not going to change anything\n";
                }

                CATCH_REQUIRE(snapdev::chownnm(filename, std::string(), "snapwebsites") == 0);
                struct stat verify;
                CATCH_REQUIRE(stat(filename.c_str(), &verify) == 0);
                CATCH_REQUIRE(verify.st_gid == grp->gr_gid);

                // restore former group
                //
                struct group * org_group(getgrgid(st.st_gid));
                CATCH_REQUIRE(org_group != nullptr);
                CATCH_REQUIRE(snapdev::chownnm(filename, std::string(), org_group->gr_name) == 0);
                CATCH_REQUIRE(stat(filename.c_str(), &verify) == 0);
                CATCH_REQUIRE(verify.st_gid == org_group->gr_gid);
            }
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("chownnm: change owner")
    {
        // TODO: this is contradictory since we can't run this test as root...
        //
        char const * user(getenv("USER"));
        CATCH_REQUIRE(user != nullptr);
        struct passwd const * pwd(getpwnam("snapwebsites"));
        if(pwd == nullptr
        || strcmp(user, "root") != 0)
        {
            std::cerr << "warning: skipping change owner test because your are not root and/or the \"snapwebsites\" user doesn't exist.\n";
        }
        else
        {
            std::string const filename(SNAP_CATCH2_NAMESPACE::g_tmp_dir() + "/owner-test.txt");
            snapdev::file_contents system_groups(filename);
            system_groups.contents("test file--testing changing owner\n");
            system_groups.write_all();

            struct stat st;
            CATCH_REQUIRE(stat(filename.c_str(), &st) == 0);
            if(st.st_uid == pwd->pw_uid)
            {
                // this should not happen since we tested above that we are
                // root to properly run this test (otherwise we bail out)
                //
                std::cerr << "warning: your default owner is \"snapwebsites\" so the test is not going to change anything\n";
            }

            CATCH_REQUIRE(snapdev::chownnm(filename, "snapwebsites", std::string()) == 0);
            struct stat verify;
            CATCH_REQUIRE(stat(filename.c_str(), &verify) == 0);
            CATCH_REQUIRE(verify.st_uid == pwd->pw_uid);

            // restore former group
            //
            struct passwd * org_owner(getpwuid(st.st_uid));
            CATCH_REQUIRE(org_owner != nullptr);
            CATCH_REQUIRE(snapdev::chownnm(filename, std::string(), org_owner->pw_name) == 0);
            CATCH_REQUIRE(stat(filename.c_str(), &verify) == 0);
            CATCH_REQUIRE(verify.st_uid == org_owner->pw_uid);
        }
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
