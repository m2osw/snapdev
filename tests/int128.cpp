/*
 * Copyright (c) 2021  Made to Order Software Corp.  All Rights Reserved
 *
 * https://snapwebsites.org/
 * contact@m2osw.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/** \file
 * \brief Verify the various __int128 output support.
 *
 * This file implements tests to verify that the support to print out
 * large numbers works as expected.
 */

// self
//
#include "main.h"



// snapdev lib
//
#include "snapdev/ostream_int128.h"


#pragma GCC diagnostic ignored "-Wpedantic"


CATCH_TEST_CASE("ostream_int128", "[ostream][int128]")
{
    CATCH_START_SECTION("small numbers (-10 to +10)")
    {
        for(int i(-10); i <= 10; ++i)
        {
            __int128 l(i);
            std::stringstream ss;
            ss << l;
            CATCH_REQUIRE(ss.str() == std::to_string(i));
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("positive numbers")
    {
        for(int i(0); i < 1000; ++i)
        {
            std::int32_t v((rand() & 0x7FFFFFFF));
            __int128 l(v);
            std::stringstream ss;
            ss << l;
            CATCH_REQUIRE(ss.str() == std::to_string(v));
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("negative numbers")
    {
        for(int i(0); i < 1000; ++i)
        {
            std::int32_t v(-(rand() & 0x7FFFFFFF));
            __int128 l(v);
            std::stringstream ss;
            ss << l;
            CATCH_REQUIRE(ss.str() == std::to_string(v));
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("most positive number")
    {
        unsigned __int128 l(0);
        l = ~l;
        std::stringstream ss;
        ss << l;
        CATCH_REQUIRE(ss.str() == "340282366920938463463374607431768211455");
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("most negative number")
    {
        __int128 l(1);
        l <<= 127;
        std::stringstream ss;
        ss << l;
        CATCH_REQUIRE(ss.str() == "-170141183460469231731687303715884105728");
    }
    CATCH_END_SECTION()
}


// vim: ts=4 sw=4 et
