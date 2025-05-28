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
 * \brief Verify the saturated_subtract() function.
 *
 * This file implements tests to verify that the saturated_subtract() function
 * will indeed saturate the result on an underflow.
 */

// self
//
#include    "catch_main.h"



// snapdev
//
#include    <snapdev/math.h>
#include    <snapdev/ostream_int128.h>


// last include
//
#include    <snapdev/poison.h>


// __int128 is not ISO C++ yet
#pragma GCC diagnostic ignored "-Wpedantic"


CATCH_TEST_CASE("saturated_subtract_uint", "[math]")
{
    CATCH_START_SECTION("saturated_subtract_uint8: test all possible cases with 8 bits")
    {
        for(std::uint32_t n(0); n < 256; ++n)
        {
            for(std::uint32_t m(0); m < 256; ++m)
            {
                std::int32_t expected(n - m);
                if(expected < 0)      // easy to test in this case
                {
                    expected = 0;
                }
                std::int32_t const result(snapdev::saturated_subtract(
                              static_cast<std::uint8_t>(n)
                            , static_cast<std::uint8_t>(m)));
                CATCH_REQUIRE(result == expected);
            }
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("saturated_subtract_uint16: test many possible cases with 16 bits")
    {
        for(std::uint32_t n(0); n < 65536; n += rand() % 100)
        {
            for(std::uint32_t m(0); m < 65536; m += rand() % 100)
            {
                std::int32_t expected(n - m);
                if(expected < 0)      // easy to test in this case
                {
                    expected = 0;
                }
                std::int32_t const result(snapdev::saturated_subtract(
                              static_cast<std::uint16_t>(n)
                            , static_cast<std::uint16_t>(m)));
                CATCH_REQUIRE(result == expected);
            }
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("saturated_subtract: test a few possible cases with 32 bits")
    {
        // no underflow at all
        {
            std::uint32_t const result(snapdev::saturated_subtract(
                          static_cast<std::uint32_t>(32'000)
                        , static_cast<std::uint32_t>(1'000)));
            CATCH_REQUIRE(result == 31'000UL);
        }

        // no underflow, but close
        {
            std::uint32_t const result(snapdev::saturated_subtract(
                          static_cast<std::uint32_t>(1'000)
                        , static_cast<std::uint32_t>(1'000)));
            CATCH_REQUIRE(result == std::numeric_limits<std::uint32_t>::min());
        }

        // underflow by 1
        {
            std::uint32_t const result(snapdev::saturated_subtract(
                          static_cast<std::uint32_t>(1'000)
                        , static_cast<std::uint32_t>(1'001)));
            CATCH_REQUIRE(result == std::numeric_limits<std::uint32_t>::min());
        }

        // definitive underflow
        {
            std::uint32_t const result(snapdev::saturated_subtract(
                          static_cast<std::uint32_t>(1'000)
                        , static_cast<std::uint32_t>(3'501)));
            CATCH_REQUIRE(result == std::numeric_limits<std::uint32_t>::min());
        }

        // max. underflow
        {
            std::uint32_t const result(snapdev::saturated_subtract(
                          std::numeric_limits<std::uint32_t>::min()
                        , std::numeric_limits<std::uint32_t>::max()));
            CATCH_REQUIRE(result == std::numeric_limits<std::uint32_t>::min());
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("saturated_subtract_uint64: test a few possible cases with 64 bits")
    {
        // no underflow at all
        {
            std::uint64_t const result(snapdev::saturated_subtract(
                          static_cast<std::uint64_t>(32'000)
                        , static_cast<std::uint64_t>(1'000)));
            CATCH_REQUIRE(result == 31'000UL);
        }

        // no underflow, but close
        {
            std::uint64_t const result(snapdev::saturated_subtract(
                          static_cast<std::uint64_t>(1'000)
                        , static_cast<std::uint64_t>(1'000)));
            CATCH_REQUIRE(result == std::numeric_limits<std::uint64_t>::min());
        }

        // underflow by 1
        {
            std::uint64_t const result(snapdev::saturated_subtract(
                          static_cast<std::uint64_t>(1'000)
                        , static_cast<std::uint64_t>(1'001)));
            CATCH_REQUIRE(result == std::numeric_limits<std::uint64_t>::min());
        }

        // definitive underflow
        {
            std::uint64_t const result(snapdev::saturated_subtract(
                          static_cast<std::uint64_t>(1'000)
                        , static_cast<std::uint64_t>(3'501)));
            CATCH_REQUIRE(result == std::numeric_limits<std::uint64_t>::min());
        }

        // max. underflow
        {
            std::uint64_t const result(snapdev::saturated_subtract(
                          std::numeric_limits<std::uint64_t>::min()
                        , std::numeric_limits<std::uint64_t>::max()));
            CATCH_REQUIRE(result == std::numeric_limits<std::uint64_t>::min());
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("saturated_subtract_uint128: test a few possible cases with 128 bits")
    {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
        // no underflow at all
        {
            unsigned __int128 const result(snapdev::saturated_subtract(
                          static_cast<unsigned __int128>(32'000)
                        , static_cast<unsigned __int128>(1'000)));
            CATCH_REQUIRE(result == 31'000UL);
        }

        // no underflow, but close
        {
            unsigned __int128 const result(snapdev::saturated_subtract(
                          static_cast<unsigned __int128>(1'000)
                        , static_cast<unsigned __int128>(1'000)));
            CATCH_REQUIRE(result == std::numeric_limits<unsigned __int128>::min());
        }

        // underflow by 1
        {
            unsigned __int128 const result(snapdev::saturated_subtract(
                          static_cast<unsigned __int128>(1'000)
                        , static_cast<unsigned __int128>(1'001)));
            CATCH_REQUIRE(result == std::numeric_limits<unsigned __int128>::min());
        }

        // definitive underflow
        {
            unsigned __int128 const result(snapdev::saturated_subtract(
                          static_cast<unsigned __int128>(3'501)
                        , std::numeric_limits<unsigned __int128>::max() - 1'000));
            CATCH_REQUIRE(result == std::numeric_limits<unsigned __int128>::min());
        }

        // max. underflow
        {
            unsigned __int128 const result(snapdev::saturated_subtract(
                          std::numeric_limits<unsigned __int128>::min()
                        , std::numeric_limits<unsigned __int128>::max()));
            CATCH_REQUIRE(result == std::numeric_limits<unsigned __int128>::min());
        }
#pragma GCC diagnostic pop
    }
    CATCH_END_SECTION()
}


// vim: ts=4 sw=4 et
