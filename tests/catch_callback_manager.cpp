// Copyright (c) 2018-2021  Made to Order Software Corp.  All Rights Reserved
//
// https://snapwebsites.org/project/snapdev
// contact@m2osw.com
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

/** \file
 * \brief Verify that the callback_manager class works.
 *
 * This file implements tests for the callback_manager class.
 */

// self
//
#include    <snapdev/callback_manager.h>

#include    "catch_main.h"


// snapdev include
//
#include    <snapdev/not_used.h>


// C++ include
//
#include    <list>


// last include
//
#include    <snapdev/poison.h>


namespace
{


bool g_called = false;
int g_expected_a = 33;
int g_expected_b = 51;
std::string g_expected_c = "expected value";

bool the_static_callback(int v, int q, std::string z)
{
    CATCH_REQUIRE(v == g_expected_a);
    CATCH_REQUIRE(q == g_expected_b);
    CATCH_REQUIRE(z == g_expected_c);
    g_called = true;
    return true;
}

bool void_static_callback()
{
    g_called = true;
    return true;
}




}
// no name namespace



CATCH_TEST_CASE("callback_manager", "[callback]")
{
    CATCH_START_SECTION("callback manager: member function callback")
    {
        int count(0);

        class foo
        {
        public:
            typedef std::shared_ptr<foo>    pointer_t;
            typedef std::vector<pointer_t>  vector_t;

            foo(int & cnt)
                : f_count(cnt)
            {
            }

            bool the_callback(int v, int q, int z)
            {
                CATCH_REQUIRE(v == f_expected_a);
                CATCH_REQUIRE(q == f_expected_b);
                CATCH_REQUIRE(z == f_expected_c);
                ++f_count;
                return true;
            }

            bool void_callback()
            {
                ++f_count;
                return true;
            }

            int & f_count;
            int f_expected_a = 0;
            int f_expected_b = 0;
            int f_expected_c = 0;
        };
        snap::callback_manager<foo::vector_t, true> m;

        foo::pointer_t f1(std::make_shared<foo>(count));
        foo::pointer_t f2(std::make_shared<foo>(count));
        foo::pointer_t f3(std::make_shared<foo>(count));

        f1->f_expected_a = 5;
        f2->f_expected_a = 5;
        f3->f_expected_a = 5;
        f1->f_expected_b = 13;
        f2->f_expected_b = 13;
        f3->f_expected_b = 13;
        f1->f_expected_c = 7;
        f2->f_expected_c = 7;
        f3->f_expected_c = 7;

        CATCH_REQUIRE(m.size() == 0);
        CATCH_REQUIRE(m.empty());
        CATCH_REQUIRE(m.add_callback(f1));
        CATCH_REQUIRE(m.add_callback(f2));
        CATCH_REQUIRE(m.add_callback(f3));
        CATCH_REQUIRE_FALSE(m.empty());
        CATCH_REQUIRE(m.size() == 3);

        m.call(&foo::the_callback, 5, 13, 7);

        CATCH_REQUIRE(count == 3);
        CATCH_REQUIRE(f1->f_count == 3);
        CATCH_REQUIRE(f2->f_count == 3);
        CATCH_REQUIRE(f3->f_count == 3);

        m.call(&foo::void_callback);

        CATCH_REQUIRE(count == 6);
        CATCH_REQUIRE(f1->f_count == 6);
        CATCH_REQUIRE(f2->f_count == 6);
        CATCH_REQUIRE(f3->f_count == 6);

        m.remove_callback(f2);
        CATCH_REQUIRE_FALSE(m.empty());
        CATCH_REQUIRE(m.size() == 2);

        f1->f_expected_a = 12;
        f3->f_expected_a = 12;
        f1->f_expected_b = 37;
        f3->f_expected_b = 37;
        f1->f_expected_c = 17;
        f3->f_expected_c = 17;

        m.call(&foo::the_callback, 12, 37, 17);

        CATCH_REQUIRE(count == 8);
        CATCH_REQUIRE(f1->f_count == 8);
        CATCH_REQUIRE(f2->f_count == 8);
        CATCH_REQUIRE(f3->f_count == 8);

        CATCH_REQUIRE_FALSE(m.empty());
        CATCH_REQUIRE(m.clear());
        CATCH_REQUIRE(m.empty());
        CATCH_REQUIRE(m.size() == 0);
        CATCH_REQUIRE_FALSE(m.clear());
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("callback manager: direct function callback with 3 parameters")
    {
        g_called = false;

        typedef bool (*callback_t)(int, int, std::string);
        typedef std::list<callback_t> list_t;

        snap::callback_manager<list_t, true> m;

        CATCH_REQUIRE(m.add_callback(the_static_callback));

        CATCH_REQUIRE_FALSE(g_called);
        m.call(33, 51, "expected value");
        CATCH_REQUIRE(g_called);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("callback manager: direct function callback with no parameters")
    {
        g_called = false;

        typedef bool (*callback_t)();
        typedef std::list<callback_t> list_t;

        snap::callback_manager<list_t, true> m;

        CATCH_REQUIRE(m.add_callback(void_static_callback));

        CATCH_REQUIRE_FALSE(g_called);
        m.call();
        CATCH_REQUIRE(g_called);
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
