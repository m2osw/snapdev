// Copyright (c) 2021-2026  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Verify the saturated_add() functions.
 *
 * This file implements tests to verify that the saturated_add() functions
 * will indeed saturate the result on an overflow.
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


CATCH_TEST_CASE("saturated_add_uint", "[math]")
{
    CATCH_START_SECTION("saturated_add_uint8: test all possible cases with 8 bits")
    {
        for(std::uint32_t n(0); n < 256; ++n)
        {
            for(std::uint32_t m(0); m < 256; ++m)
            {
                std::uint32_t expected(n + m);
                if(expected > 255)      // easy to test in this case
                {
                    expected = 255;
                }
                std::uint32_t const result(snapdev::saturated_add(
                              static_cast<std::uint8_t>(n)
                            , static_cast<std::uint8_t>(m)));
                CATCH_REQUIRE(result == expected);
            }
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("saturated_add_uint16: test many possible cases with 16 bits")
    {
        for(std::uint32_t n(0); n < 65536; n += rand() % 100)
        {
            for(std::uint32_t m(0); m < 65536; m += rand() % 100)
            {
                std::uint32_t expected(n + m);
                if(expected > 65535)      // easy to test in this case
                {
                    expected = 65535;
                }
                std::uint32_t const result(snapdev::saturated_add(
                              static_cast<std::uint16_t>(n)
                            , static_cast<std::uint16_t>(m)));
                CATCH_REQUIRE(result == expected);
            }
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("saturated_add_uint32: test a few possible cases with 32 bits")
    {
        // no overflow at all
        {
            std::uint32_t const result(snapdev::saturated_add(
                          static_cast<std::uint32_t>(1'000)
                        , static_cast<std::uint32_t>(32'000)));
            CATCH_REQUIRE(result == 33'000);
        }

        // no overflow, but close
        {
            std::uint32_t const result(snapdev::saturated_add(
                          static_cast<std::uint32_t>(1'000)
                        , std::numeric_limits<std::uint32_t>::max() - 1'000));
            CATCH_REQUIRE(result == std::numeric_limits<std::uint32_t>::max());
        }

        // overflow by 1
        {
            std::uint32_t const result(snapdev::saturated_add(
                          static_cast<std::uint32_t>(1'001)
                        , std::numeric_limits<std::uint32_t>::max() - 1'000));
            CATCH_REQUIRE(result == std::numeric_limits<std::uint32_t>::max());
        }

        // definitive overflow
        {
            std::uint32_t const result(snapdev::saturated_add(
                          static_cast<std::uint32_t>(3'501)
                        , std::numeric_limits<std::uint32_t>::max() - 1'000));
            CATCH_REQUIRE(result == std::numeric_limits<std::uint32_t>::max());
        }

        // max. overflow
        {
            std::uint32_t const result(snapdev::saturated_add(
                          std::numeric_limits<std::uint32_t>::max()
                        , std::numeric_limits<std::uint32_t>::max()));
            CATCH_REQUIRE(result == std::numeric_limits<std::uint32_t>::max());
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("saturated_add_uint64: test a few possible cases with 64 bits")
    {
        // no overflow at all
        {
            std::uint64_t const result(snapdev::saturated_add(
                          static_cast<std::uint64_t>(1'000)
                        , static_cast<std::uint64_t>(32'000)));
            CATCH_REQUIRE(result == 33'000);
        }

        // no overflow, but close
        {
            std::uint64_t const result(snapdev::saturated_add(
                          static_cast<std::uint64_t>(1'000)
                        , std::numeric_limits<std::uint64_t>::max() - 1'000));
            CATCH_REQUIRE(result == std::numeric_limits<std::uint64_t>::max());
        }

        // overflow by 1
        {
            std::uint64_t const result(snapdev::saturated_add(
                          static_cast<std::uint64_t>(1'001)
                        , std::numeric_limits<std::uint64_t>::max() - 1'000));
            CATCH_REQUIRE(result == std::numeric_limits<std::uint64_t>::max());
        }

        // definitive overflow
        {
            std::uint64_t const result(snapdev::saturated_add(
                          static_cast<std::uint64_t>(3'501)
                        , std::numeric_limits<std::uint64_t>::max() - 1'000));
            CATCH_REQUIRE(result == std::numeric_limits<std::uint64_t>::max());
        }

        // max. overflow
        {
            std::uint64_t const result(snapdev::saturated_add(
                          std::numeric_limits<std::uint64_t>::max()
                        , std::numeric_limits<std::uint64_t>::max()));
            CATCH_REQUIRE(result == std::numeric_limits<std::uint64_t>::max());
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("saturated_add_uint128: test a few possible cases with 128 bits")
    {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
        // no overflow at all
        {
            unsigned __int128 const result(snapdev::saturated_add(
                          static_cast<unsigned __int128>(1'000)
                        , static_cast<unsigned __int128>(32'000)));
            CATCH_REQUIRE(result == 33'000);
        }

        // no overflow, but close
        {
            unsigned __int128 const result(snapdev::saturated_add(
                          static_cast<unsigned __int128>(1'000)
                        , std::numeric_limits<unsigned __int128>::max() - 1'000));
            CATCH_REQUIRE(result == std::numeric_limits<unsigned __int128>::max());
        }

        // overflow by 1
        {
            unsigned __int128 const result(snapdev::saturated_add(
                          static_cast<unsigned __int128>(1'001)
                        , std::numeric_limits<unsigned __int128>::max() - 1'000));
            CATCH_REQUIRE(result == std::numeric_limits<unsigned __int128>::max());
        }

        // definitive overflow
        {
            unsigned __int128 const result(snapdev::saturated_add(
                          static_cast<unsigned __int128>(3'501)
                        , std::numeric_limits<unsigned __int128>::max() - 1'000));
            CATCH_REQUIRE(result == std::numeric_limits<unsigned __int128>::max());
        }

        // max. overflow
        {
            unsigned __int128 const result(snapdev::saturated_add(
                          std::numeric_limits<unsigned __int128>::max()
                        , std::numeric_limits<unsigned __int128>::max()));
            CATCH_REQUIRE(result == std::numeric_limits<unsigned __int128>::max());
        }
#pragma GCC diagnostic pop
    }
    CATCH_END_SECTION()
}


CATCH_TEST_CASE("saturated_add_int", "[math]")
{
    CATCH_START_SECTION("saturated_add_int8: test all possible cases with 8 bits")
    {
        for(std::int32_t n(-128); n < 128; ++n)
        {
            for(std::int32_t m(-128); m < 128; ++m)
            {
                std::int32_t expected(n + m);
                if(expected < -128)
                {
                    expected = -128;
                }
                else if(expected > 127)
                {
                    expected = 127;
                }
                std::int32_t const result(snapdev::saturated_add(
                              static_cast<std::int8_t>(n)
                            , static_cast<std::int8_t>(m)));
                CATCH_REQUIRE(result == expected);
            }
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("saturated_add_int16: test many possible cases with 16 bits")
    {
        for(std::int32_t n(-32768); n < 32768; n += rand() % 100)
        {
            for(std::int32_t m(-32768); m < 32768; m += rand() % 100)
            {
                std::int32_t expected(n + m);
                if(expected < -32768)
                {
                    expected = -32768;
                }
                else if(expected > 32767)
                {
                    expected = 32767;
                }
                std::int32_t const result(snapdev::saturated_add(
                              static_cast<std::int16_t>(n)
                            , static_cast<std::int16_t>(m)));
                CATCH_REQUIRE(result == expected);
            }
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("saturated_add_int32: test a few possible cases with 32 bits")
    {
        // no overflow at all
        {
            std::int32_t const result(snapdev::saturated_add(
                          static_cast<std::int32_t>(1'000)
                        , static_cast<std::int32_t>(33'000)));
            CATCH_REQUIRE(result == 34'000);
        }

        // no overflow, but close
        {
            std::int32_t const result(snapdev::saturated_add(
                          static_cast<std::int32_t>(1'000)
                        , std::numeric_limits<std::int32_t>::max() - 1'000));
            CATCH_REQUIRE(result == std::numeric_limits<std::int32_t>::max());
        }

        // overflow by 1
        {
            std::int32_t const result(snapdev::saturated_add(
                          static_cast<std::int32_t>(1'001)
                        , std::numeric_limits<std::int32_t>::max() - 1'000));
            CATCH_REQUIRE(result == std::numeric_limits<std::int32_t>::max());
        }

        // definitive overflow
        {
            std::int32_t const result(snapdev::saturated_add(
                          static_cast<std::int32_t>(3'501)
                        , std::numeric_limits<std::int32_t>::max() - 1'000));
            CATCH_REQUIRE(result == std::numeric_limits<std::int32_t>::max());
        }

        // max. overflow
        {
            std::int32_t const result(snapdev::saturated_add(
                          std::numeric_limits<std::int32_t>::max()
                        , std::numeric_limits<std::int32_t>::max()));
            CATCH_REQUIRE(result == std::numeric_limits<std::int32_t>::max());
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("saturated_add_int64: test a few possible cases with 64 bits")
    {
        // no overflow at all
        {
            std::int64_t const result(snapdev::saturated_add(
                          static_cast<std::int64_t>(1'000)
                        , static_cast<std::int64_t>(32'000)));
            CATCH_REQUIRE(result == 33'000);
        }

        // no overflow, but close
        {
            std::int64_t const result(snapdev::saturated_add(
                          static_cast<std::int64_t>(1'000)
                        , std::numeric_limits<std::int64_t>::max() - 1'000));
            CATCH_REQUIRE(result == std::numeric_limits<std::int64_t>::max());
        }

        // overflow by 1
        {
            std::int64_t const result(snapdev::saturated_add(
                          static_cast<std::int64_t>(1'001)
                        , std::numeric_limits<std::int64_t>::max() - 1'000));
            CATCH_REQUIRE(result == std::numeric_limits<std::int64_t>::max());
        }

        // definitive overflow
        {
            std::int64_t const result(snapdev::saturated_add(
                          static_cast<std::int64_t>(3'501)
                        , std::numeric_limits<std::int64_t>::max() - 1'000));
            CATCH_REQUIRE(result == std::numeric_limits<std::int64_t>::max());
        }

        // max. overflow
        {
            std::int64_t const result(snapdev::saturated_add(
                          std::numeric_limits<std::int64_t>::max()
                        , std::numeric_limits<std::int64_t>::max()));
            CATCH_REQUIRE(result == std::numeric_limits<std::int64_t>::max());
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("saturated_add_int128: test a few possible cases with 128 bits")
    {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
        // no overflow at all
        {
            __int128 const result(snapdev::saturated_add(
                          static_cast<__int128>(1'000LL)
                        , static_cast<__int128>(32'000'000LL)));
            CATCH_REQUIRE(result == 32'001'000LL);
        }

        // no overflow, but close
        {
            __int128 const result(snapdev::saturated_add(
                          static_cast<__int128>(1'000)
                        , std::numeric_limits<__int128>::max() - 1'000));
            CATCH_REQUIRE(result == std::numeric_limits<__int128>::max());
        }

        // overflow by 1
        {
            __int128 const result(snapdev::saturated_add(
                          static_cast<__int128>(1'001)
                        , std::numeric_limits<__int128>::max() - 1'000));
            CATCH_REQUIRE(result == std::numeric_limits<__int128>::max());
        }

        // definitive overflow
        {
            __int128 const result(snapdev::saturated_add(
                          static_cast<__int128>(3'501)
                        , std::numeric_limits<__int128>::max() - 1'000));
            CATCH_REQUIRE(result == std::numeric_limits<__int128>::max());
        }

        // max. overflow
        {
            __int128 const result(snapdev::saturated_add(
                          std::numeric_limits<__int128>::max()
                        , std::numeric_limits<__int128>::max()));
            CATCH_REQUIRE(result == std::numeric_limits<__int128>::max());
        }
#pragma GCC diagnostic pop
    }
    CATCH_END_SECTION()
}


// vim: ts=4 sw=4 et
