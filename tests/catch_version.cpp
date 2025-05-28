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
 * \brief Verify the version macros.
 *
 * This file implements tests for the version macros.
 */

// self
//
#include    <snapdev/version.h>

#include    "catch_main.h"


// last include
//
#include    <snapdev/poison.h>




CATCH_TEST_CASE("version", "[version]")
{
    CATCH_START_SECTION("version: snapdev version")
    {
        std::string v;
        v += std::to_string(SNAPDEV_VERSION_MAJOR);
        v += '.';
        v += std::to_string(SNAPDEV_VERSION_MINOR);
        v += '.';
        v += std::to_string(SNAPDEV_VERSION_PATCH);

        CATCH_REQUIRE(SNAPDEV_VERSION_STRING == v);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("version: check wanted/current version")
    {
        // we probably will never reach version 1 billion...
        //
        bool const result(SNAPDEV_CHECK_VERSION(1'000'000'000, 555, 123, SNAPDEV_VERSION_MAJOR, SNAPDEV_VERSION_MINOR, SNAPDEV_VERSION_PATCH));
        CATCH_REQUIRE_FALSE(result);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("version: check explicit versions")
    {
        // now compare explicit versions on both sides to make sure we got that right
        //
        struct wanted_t
        {
            int     f_wanted_major = 0;
            int     f_wanted_minor = 0;
            int     f_wanted_patch = 0;
            int     f_current_major = 0;
            int     f_current_minor = 0;
            int     f_current_patch = 0;
            bool    f_result = false;
        };

        constexpr wanted_t const wanted[] =
        {
            // true if left version <= to right version
            { 7, 3,  9, 7, 3,  9, true  },
            { 7, 3,  9, 7, 3, 10, true  },
            { 7, 3, 11, 7, 3, 10, false },
            { 7, 4,  9, 7, 3, 10, false },
            { 7, 2, 11, 7, 3, 10, true  },
            { 8, 3,  9, 7, 3,  9, false },
            { 7, 3,  9, 8, 3,  9, true  },
            { 8, 2,  9, 7, 3,  9, false },
            { 7, 9,  9, 8, 3,  9, true  },
            { 7, 9,  9, 8, 3,  1, true  },
            { 7, 0,  0, 8, 0,  0, true  },
            { 8, 0,  0, 8, 0,  0, true  },
            { 9, 0,  0, 8, 0,  0, false },
        };

        for(std::size_t idx(0); idx < std::size(wanted); ++idx)
        {
            bool const result(SNAPDEV_CHECK_VERSION(
                            wanted[idx].f_wanted_major,
                            wanted[idx].f_wanted_minor,
                            wanted[idx].f_wanted_patch,
                            wanted[idx].f_current_major,
                            wanted[idx].f_current_minor,
                            wanted[idx].f_current_patch));

//std::cout << "--- compare versions "
//<< wanted[idx].f_wanted_major << '.' << wanted[idx].f_wanted_minor << '.' << wanted[idx].f_wanted_patch
//<< " <=> "
//<< wanted[idx].f_current_major << '.' << wanted[idx].f_current_minor << '.' << wanted[idx].f_current_patch
//<< " -> " << result << " (expected: " << wanted[idx].f_result << ")\n";

            CATCH_REQUIRE(wanted[idx].f_result == result);
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("version: we need at least g++ 7.x")
    {
        bool const wanted(SNAPDEV_CHECK_GCC_VERSION(7, 5, 0));
        CATCH_REQUIRE(wanted);
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
