// Copyright (c) 2018-2024  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Verify that the memsearch() function works.
 *
 * This file implements tests for the memsearch() function.
 */

// self
//
#include    <snapdev/memsearch.h>

#include    "catch_main.h"


// last include
//
#include    <snapdev/poison.h>




CATCH_TEST_CASE("memsearch", "[memory]")
{
    CATCH_START_SECTION("memsearch: verify memory search")
    {
        char const haystack[] = {
            0x00, 0x30, 0x18, 0x29, static_cast<char>(0xFF), 0x45, 0x71, static_cast<char>(0xAC),
            0x10, 0x41, 0x58, 0x33, static_cast<char>(0xEF), static_cast<char>(0xD5), 0x6F, 0x39,
        };

        char const needle1[] = { 0x18, 0x29, static_cast<char>(0xFF) };
        int pos(snapdev::memsearch(haystack, sizeof(haystack), needle1, sizeof(needle1)));
        CATCH_REQUIRE(pos == 2);
        pos = snapdev::memsearch(needle1, sizeof(needle1), haystack, sizeof(haystack));
        CATCH_REQUIRE(pos == -1);

        char const needle2[] = { 0x11, 0x29, static_cast<char>(0xFF) };
        pos = snapdev::memsearch(haystack, sizeof(haystack), needle2, sizeof(needle2));
        CATCH_REQUIRE(pos == -1);
        pos = snapdev::memsearch(needle2, sizeof(needle2), haystack, sizeof(haystack));
        CATCH_REQUIRE(pos == -1);

        char const needle3[] = { static_cast<char>(0xD5), 0x6F, 0x39 };
        pos = snapdev::memsearch(haystack, sizeof(haystack), needle3, sizeof(needle3));
        CATCH_REQUIRE(pos == 13);
        pos = snapdev::memsearch(needle3, sizeof(needle3), haystack, sizeof(haystack));
        CATCH_REQUIRE(pos == -1);

        pos = snapdev::memsearch(haystack, sizeof(haystack), haystack, sizeof(haystack));
        CATCH_REQUIRE(pos == 0);
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
