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
 * \brief Verify the string_replace_many() function.
 *
 * Make sure that it does replace as expected.
 */

// self
//
#include    <snapdev/string_replace_many.h>

#include    "catch_main.h"


// C++
//
#include    <deque>
#include    <list>


// last include
//
#include    <snapdev/poison.h>



CATCH_TEST_CASE("string_replace_many", "[string_replace_many][string]")
{
    CATCH_START_SECTION("string_replace_many: empty input string")
    {
        std::string const empty;
        std::string result(snapdev::string_replace_many(empty, {{"this", "that"}}));
        CATCH_REQUIRE(empty == result);

        // is the following a bug?
        // (i.e. I am thinking it should return "once")
        //
        result = snapdev::string_replace_many(empty, {{"", "once"}});
        CATCH_REQUIRE(empty == result);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("string_replace_many: replace one string")
    {
        std::string result(snapdev::string_replace_many(std::string("this is replaced"), {{"this", "that"}}));
        CATCH_REQUIRE("that is replaced" == result);

        result = snapdev::string_replace_many(std::string("place this in the middle"), {{"this", "that"}});
        CATCH_REQUIRE("place that in the middle" == result);

        result = snapdev::string_replace_many(std::string("place at the end"), {{"end", "finish"}});
        CATCH_REQUIRE("place at the finish" == result);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("string_replace_many: replace many strings")
    {
        std::string result(snapdev::string_replace_many(std::string("this is replaced isn't it?"), {{"is", "XXX"}}));
        CATCH_REQUIRE("thXXX XXX replaced XXXn't it?" == result);

        result = snapdev::string_replace_many(std::string("XXstartXXmiddle endXX"), {{"XX", "*"}});
        CATCH_REQUIRE("*start*middle end*" == result);

        result = snapdev::string_replace_many(std::string("glueglueglueglue"), {{"glue", " WORD "}});
        CATCH_REQUIRE(" WORD  WORD  WORD  WORD " == result);

        result = snapdev::string_replace_many(std::string("this is something that is eaten"), {{"is", "at"}, {"th", "z"}, {"eat", ""}, {"en", "fr"}});
        CATCH_REQUIRE("zat at somezing zat at fr" == result);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("string_replace_many: dashing string")
    {
        std::string result(snapdev::string_replace_many(std::string("dash this string here"), {{"", "-"}}));
        CATCH_REQUIRE("-d-a-s-h- -t-h-i-s- -s-t-r-i-n-g- -h-e-r-e" == result);
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
