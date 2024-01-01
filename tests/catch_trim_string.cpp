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
 * \brief Verify that the trim_string() function works.
 *
 * This file implements tests for the trim_string() function.
 */

// self
//
#include    <snapdev/trim_string.h>

#include    "catch_main.h"


// last include
//
#include    <snapdev/poison.h>


namespace
{



struct strings_t
{
    char const *        f_original = nullptr;

    char const *        f_start = nullptr;
    char const *        f_end = nullptr;
    char const *        f_inside = nullptr;
    char const *        f_start_and_end = nullptr;
    char const *        f_start_and_inside = nullptr;
    char const *        f_inside_and_end = nullptr;
    char const *        f_all = nullptr;

    char const *        f_spaces_start = nullptr;
    char const *        f_spaces_end = nullptr;
    char const *        f_spaces_inside = nullptr;
    char const *        f_spaces_start_and_end = nullptr;
    char const *        f_spaces_start_and_inside = nullptr;
    char const *        f_spaces_inside_and_end = nullptr;
    char const *        f_spaces_all = nullptr;
};

constexpr strings_t const   g_strings[] =
{
    {
        /* orginal */ "   do \t  not  \f trim   \r anything    \n  here   ",

        /* start   */ "do \t  not  \f trim   \r anything    \n  here   ",
        /* end     */ "   do \t  not  \f trim   \r anything    \n  here",
        /* inside  */ " do not trim anything here ",
        /* s/e     */ "do \t  not  \f trim   \r anything    \n  here",
        /* s/i     */ "do not trim anything here ",
        /* i/e     */ " do not trim anything here",
        /* all     */ "do not trim anything here",

        /* start   */ "do \t  not  \f trim   \r anything    \n  here   ",
        /* end     */ "   do \t  not  \f trim   \r anything    \n  here",
        /* inside  */ " do \t not \f trim \r anything \n here ",
        /* s/e     */ "do \t  not  \f trim   \r anything    \n  here",
        /* s/i     */ "do \t not \f trim \r anything \n here ",
        /* i/e     */ " do \t not \f trim \r anything \n here",
        /* all     */ "do \t not \f trim \r anything \n here",
    },
    {
        /* orginal */ "  do  \t not    trim  \r  anything   \n here   again!\r\n",

        /* start   */ "do  \t not    trim  \r  anything   \n here   again!\r\n",
        /* end     */ "  do  \t not    trim  \r  anything   \n here   again!",
        /* inside  */ " do not trim anything here again! ",
        /* s/e     */ "do  \t not    trim  \r  anything   \n here   again!",
        /* s/i     */ "do not trim anything here again! ",
        /* i/e     */ " do not trim anything here again!",
        /* all     */ "do not trim anything here again!",

        /* start   */ "do  \t not    trim  \r  anything   \n here   again!\r\n",
        /* end     */ "  do  \t not    trim  \r  anything   \n here   again!\r\n",
        /* inside  */ " do \t not trim \r anything \n here again!\r\n",
        /* s/e     */ "do  \t not    trim  \r  anything   \n here   again!\r\n",
        /* s/i     */ "do \t not trim \r anything \n here again!\r\n",
        /* i/e     */ " do \t not trim \r anything \n here again!\r\n",
        /* all     */ "do \t not trim \r anything \n here again!\r\n",
    },
    {
        /* orginal */ "nothing-to-remove",

        /* start   */ "nothing-to-remove",
        /* end     */ "nothing-to-remove",
        /* inside  */ "nothing-to-remove",
        /* s/e     */ "nothing-to-remove",
        /* s/i     */ "nothing-to-remove",
        /* i/e     */ "nothing-to-remove",
        /* all     */ "nothing-to-remove",

        /* start   */ "nothing-to-remove",
        /* end     */ "nothing-to-remove",
        /* inside  */ "nothing-to-remove",
        /* s/e     */ "nothing-to-remove",
        /* s/i     */ "nothing-to-remove",
        /* i/e     */ "nothing-to-remove",
        /* all     */ "nothing-to-remove",
    },
};


}
// no name namespace



CATCH_TEST_CASE("trim_string", "[string]")
{
    CATCH_START_SECTION("trim_string many cases")
    {
        for(auto const & p : g_strings)
        {
            std::string const original(p.f_original);

            // no trimming
            //
            CATCH_REQUIRE(snapdev::trim_string(original, false, false) == p.f_original);
            CATCH_REQUIRE(snapdev::trim_string(original, false, false, false, std::string(" ")) == p.f_original);

            // start trimming
            //
            CATCH_REQUIRE(snapdev::trim_string(original, true, false) == p.f_start);
            CATCH_REQUIRE(snapdev::trim_string(original, true, false, false, std::string(" ")) == p.f_spaces_start);

            // end trimming
            //
            CATCH_REQUIRE(snapdev::trim_string(original, false, true) == p.f_end);
            CATCH_REQUIRE(snapdev::trim_string(original, false, true, false, std::string(" ")) == p.f_spaces_end);

            // inside trimming
            //
            CATCH_REQUIRE(snapdev::trim_string(original, false, false, true) == p.f_inside);
            CATCH_REQUIRE(snapdev::trim_string(original, false, false, true, std::string(" ")) == p.f_spaces_inside);

            // start/end trimming
            //
            CATCH_REQUIRE(snapdev::trim_string(original) == p.f_start_and_end);
            CATCH_REQUIRE(snapdev::trim_string(original, true, true, false, std::string(" ")) == p.f_spaces_start_and_end);

            // start/inside trimming
            //
            CATCH_REQUIRE(snapdev::trim_string(original, true, false, true) == p.f_start_and_inside);
            CATCH_REQUIRE(snapdev::trim_string(original, true, false, true, std::string(" ")) == p.f_spaces_start_and_inside);

            // inside/end trimming
            //
            CATCH_REQUIRE(snapdev::trim_string(original, false, true, true) == p.f_inside_and_end);
            CATCH_REQUIRE(snapdev::trim_string(original, false, true, true, std::string(" ")) == p.f_spaces_inside_and_end);

            // all trimming
            //
            CATCH_REQUIRE(snapdev::trim_string(original, true, true, true) == p.f_all);
            CATCH_REQUIRE(snapdev::trim_string(original, true, true, true, std::string(" ")) == p.f_spaces_all);
        }
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
