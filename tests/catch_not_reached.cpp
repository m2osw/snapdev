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
 * \brief Verify the unique_number class.
 *
 * This file implements tests to verify that the unique_number implementation
 * works properly even when multiple processes use it simultaneously.
 */

// file being tested
//
#include    <snapdev/not_reached.h>


// self
//
#include    "catch_main.h"


// C
//
#include    <signal.h>
#include    <sys/wait.h>


// last include
//
#include    <snapdev/poison.h>



namespace
{



extern "C" {
void catch_abort(int sig)
{
    static_cast<void>(sig);
    exit(0);
}
}



} // no name namespace



CATCH_TEST_CASE("not_reached", "[not_reached]")
{
    CATCH_START_SECTION("not_reached: verify that snapdev::NOT_REACHED() generates a SIGABRT")
    {
        pid_t const child(fork());
        CATCH_REQUIRE(child != -1);

        if(child == 0)
        {
            // we are the child
            //
            signal(SIGABRT, catch_abort);
            snapdev::NOT_REACHED();
            exit(1);
            CATCH_REQUIRE(!"this statement should never be reached");
        }

        int status(0);
        pid_t const r(waitpid(child, &status, 0));
        CATCH_REQUIRE(r == child);
        CATCH_REQUIRE(WIFEXITED(status));
        CATCH_REQUIRE(WEXITSTATUS(status) == 0);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("not_reached: verify that snapdev::NOT_REACHED_IN_TEST() generates a SIGABRT when the sanitizer is active")
    {
        pid_t const child(fork());
        CATCH_REQUIRE(child != -1);

        if(child == 0)
        {
            // we are the child
            //
            signal(SIGABRT, catch_abort);
            snapdev::NOT_REACHED_IN_TEST();
            exit(1);
            CATCH_REQUIRE(!"this statement should never be reached");
        }

        int status(0);
        pid_t const r(waitpid(child, &status, 0));
        CATCH_REQUIRE(r == child);
        CATCH_REQUIRE(WIFEXITED(status));
#ifdef __SANITIZE_ADDRESS__
        CATCH_REQUIRE(WEXITSTATUS(status) == 0);
#else
        CATCH_REQUIRE(WEXITSTATUS(status) == 1);
#endif
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
