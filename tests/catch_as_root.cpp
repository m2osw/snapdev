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
 * \brief Verify the as_root class.
 *
 * This file implements tests to verify that the as_root class works.
 *
 * \note
 * Since our tests do not get the suid flag set and are not root, we
 * can't really test that we can become root.
 */

// self
//
#include    "catch_main.h"


// snapdev
//
#include    <snapdev/as_root.h>


// C++
//
#include    <fstream>




CATCH_TEST_CASE("as_root", "[raii][os]")
{
    CATCH_START_SECTION("as_root: failing switch to root user")
    {
        snapdev::as_root test;
        CATCH_REQUIRE_FALSE(test.is_switched());
        CATCH_REQUIRE(test.error_number() == EPERM);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("as_root: working to (not) switch to ourself")
    {
        snapdev::as_root test(getuid()); // pretty useless, but it works
        CATCH_REQUIRE(test.is_switched());
        CATCH_REQUIRE(test.error_number() == 0);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("drop root privileges")
    {
        // we do not expect tests to be run by the root user
        //
        snapdev::drop_root_privileges();

        // if reach here, it worked
        //
        CATCH_REQUIRE(true);
    }
    CATCH_END_SECTION()
}


// vim: ts=4 sw=4 et
