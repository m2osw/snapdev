// Copyright (c) 2021-2025  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Verify the floating point to string convertions.
 *
 * This file implements tests for the floating point to string.
 */

// self
//
#include    <snapdev/floating_point_to_string.h>

#include    "catch_main.h"



// C++
//
//#include    <iomanip>
//#include    <set>


// last include
//
#include    <snapdev/poison.h>



namespace
{





} // no name namespace



CATCH_TEST_CASE("floating_point_to_string", "[float][string]")
{
    CATCH_START_SECTION("floating_point_to_string: verify conversion of floating points to string")
    {
        struct conversion_t
        {
            double      f_floating_point = 0.0;
            bool        f_keep_period = true;
            std::string f_string = std::string();
        };

        conversion_t const conversions[] =
        {
            {
                .f_floating_point = 0.0,
                .f_keep_period = true,
                .f_string = "0.0",
            },
            {
                .f_floating_point = 0.0,
                .f_keep_period = false,
                .f_string = "0",
            },
            {
                .f_floating_point = 1.0,
                .f_keep_period = true,
                .f_string = "1.0",
            },
            {
                .f_floating_point = 1.0,
                .f_keep_period = false,
                .f_string = "1",
            },
            {
                .f_floating_point = -1.0,
                .f_keep_period = true,
                .f_string = "-1.0",
            },
            {
                .f_floating_point = -1.0,
                .f_keep_period = false,
                .f_string = "-1",
            },
            {
                .f_floating_point = std::numeric_limits<double>::infinity(),
                .f_keep_period = true,
                .f_string = "Infinity",
            },
            {
                .f_floating_point = std::numeric_limits<double>::infinity(),
                .f_keep_period = false,
                .f_string = "Infinity",
            },
            {
                .f_floating_point = -std::numeric_limits<double>::infinity(),
                .f_keep_period = true,
                .f_string = "-Infinity",
            },
            {
                .f_floating_point = -std::numeric_limits<double>::infinity(),
                .f_keep_period = false,
                .f_string = "-Infinity",
            },
            {
                .f_floating_point = std::numeric_limits<double>::quiet_NaN(),
                .f_keep_period = true,
                .f_string = "NaN",
            },
            {
                .f_floating_point = std::numeric_limits<double>::quiet_NaN(),
                .f_keep_period = false,
                .f_string = "NaN",
            },
        };

        for(auto const & c : conversions)
        {
            if(c.f_keep_period)
            {
                // try with the default
                //
                std::string const default_keep_period(snapdev::floating_point_to_string<double, char>(c.f_floating_point));
                CATCH_REQUIRE(c.f_string == default_keep_period);
            }

            std::string const result(snapdev::floating_point_to_string<double, char>(c.f_floating_point, c.f_keep_period));
            CATCH_REQUIRE(c.f_string == result);
        }
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
