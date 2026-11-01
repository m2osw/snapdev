// Copyright (c) 2026  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Verify that the snapdev::preset_atomic<> works as expected.
 *
 * This file implements tests to verify the snapdev::preset_atomic<>
 * at least computes the correct values. At some point, we would
 * need to create a thread and have interaction to make sure it
 * really works.
 */

// self
//
#include    <snapdev/preset_atomic.h>

#include    "catch_main.h"


// last include
//
#include    <snapdev/poison.h>



CATCH_TEST_CASE("preset_atomic", "[atomic]")
{
    CATCH_START_SECTION("preset_atomic: verify basic operations (non-volatile int)")
    {
        snapdev::preset_atomic<int, std::memory_order::relaxed, std::memory_order::relaxed> a{3};

        CATCH_REQUIRE(a.get_read_memory_order() == std::memory_order::relaxed);
        CATCH_REQUIRE(a.get_write_memory_order() == std::memory_order::relaxed);
        CATCH_REQUIRE(a.get_read_modify_write_memory_order() == std::memory_order::relaxed);

        CATCH_REQUIRE(a == 3);
        a++;
        CATCH_REQUIRE(a == 4);
        ++a;
        CATCH_REQUIRE(a == 5);
        a--;
        CATCH_REQUIRE(a == 4);
        --a;
        CATCH_REQUIRE(a == 3);

        a = 10;
        CATCH_REQUIRE(a == 10);
        a += 5;
        CATCH_REQUIRE(a == 15);
        a -= 5;
        CATCH_REQUIRE(a == 10);

        a = 0xFF;
        CATCH_REQUIRE(a == 0xFF);
        a &= 0x55;
        CATCH_REQUIRE(a == 0x55);

        a = 0xAA;
        CATCH_REQUIRE(a == 0xAA);
        a |= 0x55;
        CATCH_REQUIRE(a == 0xFF);

        a = 0xFF;
        CATCH_REQUIRE(a == 0xFF);
        a ^= 0x55;
        CATCH_REQUIRE(a == 0xAA);

        // without other threads, these return known results
        //
        CATCH_REQUIRE(a.store_if_larger(500));
        CATCH_REQUIRE(a == 500);
        CATCH_REQUIRE_FALSE(a.store_if_larger(100));
        CATCH_REQUIRE(a == 500);
        CATCH_REQUIRE(a.store_if_smaller(100));
        CATCH_REQUIRE(a == 100);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("preset_atomic: verify basic operations (volatile int)")
    {
        volatile snapdev::preset_atomic<int, std::memory_order::relaxed, std::memory_order::relaxed> a{3};

        CATCH_REQUIRE(a.get_read_memory_order() == std::memory_order::relaxed);
        CATCH_REQUIRE(a.get_write_memory_order() == std::memory_order::relaxed);
        CATCH_REQUIRE(a.get_read_modify_write_memory_order() == std::memory_order::relaxed);

        CATCH_REQUIRE(a == 3);
        a++;
        CATCH_REQUIRE(a == 4);
        ++a;
        CATCH_REQUIRE(a == 5);
        a--;
        CATCH_REQUIRE(a == 4);
        --a;
        CATCH_REQUIRE(a == 3);

        a = 10;
        CATCH_REQUIRE(a == 10);
        a += 5;
        CATCH_REQUIRE(a == 15);
        a -= 5;
        CATCH_REQUIRE(a == 10);

        a = 0xFF;
        CATCH_REQUIRE(a == 0xFF);
        a &= 0x55;
        CATCH_REQUIRE(a == 0x55);

        a = 0xAA;
        CATCH_REQUIRE(a == 0xAA);
        a |= 0x55;
        CATCH_REQUIRE(a == 0xFF);

        a = 0xFF;
        CATCH_REQUIRE(a == 0xFF);
        a ^= 0x55;
        CATCH_REQUIRE(a == 0xAA);

        // without other threads, these return known results
        //
        CATCH_REQUIRE(a.store_if_larger(500));
        CATCH_REQUIRE(a == 500);
        CATCH_REQUIRE_FALSE(a.store_if_larger(100));
        CATCH_REQUIRE(a == 500);
        CATCH_REQUIRE(a.store_if_smaller(100));
        CATCH_REQUIRE(a == 100);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("preset_atomic: verify basic operations (non-volatile float)")
    {
        snapdev::preset_atomic<float, std::memory_order::relaxed, std::memory_order::relaxed> a{3.14};

        CATCH_REQUIRE(a.get_read_memory_order() == std::memory_order::relaxed);
        CATCH_REQUIRE(a.get_write_memory_order() == std::memory_order::relaxed);
        CATCH_REQUIRE(a.get_read_modify_write_memory_order() == std::memory_order::relaxed);

        CATCH_REQUIRE(a == 3.14_a);
        a++;
        CATCH_REQUIRE(a == 4.14_a);
        ++a;
        CATCH_REQUIRE(a == 5.14_a);
        a--;
        CATCH_REQUIRE(a == 4.14_a);
        --a;
        CATCH_REQUIRE(a == 3.14_a);

        a = 10.0;
        CATCH_REQUIRE(a == 10.0_a);
        a += 5;
        CATCH_REQUIRE(a == 15.0_a);
        a -= 5;
        CATCH_REQUIRE(a == 10.0_a);

        // without other threads, these return known results
        //
        CATCH_REQUIRE(a.store_if_larger(50.0));
        CATCH_REQUIRE(a == 50.0_a);
        CATCH_REQUIRE_FALSE(a.store_if_larger(10.0));
        CATCH_REQUIRE(a == 50.0_a);
        CATCH_REQUIRE(a.store_if_smaller(10.0));
        CATCH_REQUIRE(a == 10.0_a);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("preset_atomic: verify basic operations (volatile float)")
    {
        volatile snapdev::preset_atomic<float, std::memory_order::relaxed, std::memory_order::relaxed> a{3.14};

        CATCH_REQUIRE(a.get_read_memory_order() == std::memory_order::relaxed);
        CATCH_REQUIRE(a.get_write_memory_order() == std::memory_order::relaxed);
        CATCH_REQUIRE(a.get_read_modify_write_memory_order() == std::memory_order::relaxed);

        CATCH_REQUIRE(a == 3.14_a);
        a++;
        CATCH_REQUIRE(a == 4.14_a);
        ++a;
        CATCH_REQUIRE(a == 5.14_a);
        a--;
        CATCH_REQUIRE(a == 4.14_a);
        --a;
        CATCH_REQUIRE(a == 3.14_a);

        a = 10.0;
        CATCH_REQUIRE(a == 10.0_a);
        a += 5.0;
        CATCH_REQUIRE(a == 15.0_a);
        a -= 5.0;
        CATCH_REQUIRE(a == 10.0_a);

        // without other threads, these return known results
        //
        CATCH_REQUIRE(a.store_if_larger(50.0));
        CATCH_REQUIRE(a == 50.0_a);
        CATCH_REQUIRE_FALSE(a.store_if_larger(10.0));
        CATCH_REQUIRE(a == 50.0_a);
        CATCH_REQUIRE(a.store_if_smaller(10.0));
        CATCH_REQUIRE(a == 10.0_a);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("preset_atomic: verify basic operations (non-volatile pointer)")
    {
        int var[10] = {3};
        snapdev::preset_atomic<int *, std::memory_order::relaxed, std::memory_order::relaxed> a; //(&var);

        CATCH_REQUIRE(a.get_read_memory_order() == std::memory_order::relaxed);
        CATCH_REQUIRE(a.get_write_memory_order() == std::memory_order::relaxed);
        CATCH_REQUIRE(a.get_read_modify_write_memory_order() == std::memory_order::relaxed);

        a = var;
        CATCH_REQUIRE(a == var);
        a++;
        CATCH_REQUIRE(a == var + 1);
        ++a;
        CATCH_REQUIRE(a == var + 2);
        a--;
        CATCH_REQUIRE(a == var + 1);
        --a;
        CATCH_REQUIRE(a == var);

        int ten[10] = {10};
        a = ten;
        CATCH_REQUIRE(a == ten);
        a += 5;
        CATCH_REQUIRE(a == ten + 5);
        a -= 5;
        CATCH_REQUIRE(a == ten);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("preset_atomic: verify basic operations (volatile pointer)")
    {
        int var[10] = {3};
        volatile snapdev::preset_atomic<int *, std::memory_order::relaxed, std::memory_order::relaxed> a; //(&var);

        CATCH_REQUIRE(a.get_read_memory_order() == std::memory_order::relaxed);
        CATCH_REQUIRE(a.get_write_memory_order() == std::memory_order::relaxed);
        CATCH_REQUIRE(a.get_read_modify_write_memory_order() == std::memory_order::relaxed);

        a = var;
        CATCH_REQUIRE(a == var);
        a++;
        CATCH_REQUIRE(a == var + 1);
        ++a;
        CATCH_REQUIRE(a == var + 2);
        a--;
        CATCH_REQUIRE(a == var + 1);
        --a;
        CATCH_REQUIRE(a == var);

        int ten[10] = {10};
        a = ten;
        CATCH_REQUIRE(a == ten);
        a += 5;
        CATCH_REQUIRE(a == ten + 5);
        a -= 5;
        CATCH_REQUIRE(a == ten);
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
