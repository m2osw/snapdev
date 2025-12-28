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
#include    <set>


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
    return g_expected_a == 33;
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

            foo(int & cnt, int id)
                : f_id(id)
                , f_count(cnt)
            {
            }

            bool the_callback(int v, int q, int z)
            {
                CATCH_REQUIRE(v == f_expected_a);
                CATCH_REQUIRE(q == f_expected_b);
                CATCH_REQUIRE(z == f_expected_c);
                CATCH_REQUIRE(f_count == f_expected_count);
                ++f_count;
                return f_count < 9;
            }

            bool void_callback()
            {
                ++f_count;
                return true;
            }

            int f_id = 0;
            int & f_count;
            int f_expected_a = 0;
            int f_expected_b = 0;
            int f_expected_c = 0;
            int f_expected_count = 0;
        };
        snapdev::callback_manager<foo::pointer_t> m;

        foo::pointer_t f1(std::make_shared<foo>(count, 1));
        foo::pointer_t f2(std::make_shared<foo>(count, 2));
        foo::pointer_t f3(std::make_shared<foo>(count, 3));

        f1->f_expected_a = 5;
        f2->f_expected_a = 5;
        f3->f_expected_a = 5;
        f1->f_expected_b = 13;
        f2->f_expected_b = 13;
        f3->f_expected_b = 13;
        f1->f_expected_c = 7;
        f2->f_expected_c = 7;
        f3->f_expected_c = 7;
        f1->f_expected_count = 1;
        f2->f_expected_count = 0;
        f3->f_expected_count = 2;

        CATCH_REQUIRE(m.size() == 0);
        CATCH_REQUIRE(m.empty());
        auto const id1(m.add_callback(f1, 0));
        auto const id2(m.add_callback(f2, 1));
        auto const id3(m.add_callback(f3, -1));
        CATCH_REQUIRE_FALSE(m.empty());
        CATCH_REQUIRE(m.size() == 3);

        CATCH_REQUIRE(m.call(&foo::the_callback, 5, 13, 7));

        CATCH_REQUIRE(count == 3);
        CATCH_REQUIRE(f1->f_count == 3);
        CATCH_REQUIRE(f2->f_count == 3);
        CATCH_REQUIRE(f3->f_count == 3);

        CATCH_REQUIRE(m.call(&foo::void_callback));

        CATCH_REQUIRE(count == 6);
        CATCH_REQUIRE(f1->f_count == 6);
        CATCH_REQUIRE(f2->f_count == 6);
        CATCH_REQUIRE(f3->f_count == 6);

        CATCH_REQUIRE(m.remove_callback(id2));
        CATCH_REQUIRE_FALSE(m.empty());
        CATCH_REQUIRE(m.size() == 2);

        CATCH_REQUIRE_FALSE(m.remove_callback(id2));
        CATCH_REQUIRE_FALSE(m.empty());
        CATCH_REQUIRE(m.size() == 2);

        f1->f_expected_a = 12;
        f3->f_expected_a = 12;
        f1->f_expected_b = 37;
        f3->f_expected_b = 37;
        f1->f_expected_c = 17;
        f3->f_expected_c = 17;

        f1->f_expected_count = 6;
        f3->f_expected_count = 7;

        CATCH_REQUIRE(m.call(&foo::the_callback, 12, 37, 17));

        CATCH_REQUIRE(count == 8);
        CATCH_REQUIRE(f1->f_count == 8);
        CATCH_REQUIRE(f2->f_count == 8);
        CATCH_REQUIRE(f3->f_count == 8);

        f1->f_expected_a = 25;
        f1->f_expected_b = 31;
        f1->f_expected_c =  6;

        f1->f_expected_count = 8;

        CATCH_REQUIRE_FALSE(m.call(&foo::the_callback, 25, 31, 6));

        CATCH_REQUIRE(count == 9);
        CATCH_REQUIRE(f1->f_count == 9);
        CATCH_REQUIRE(f2->f_count == 9);
        CATCH_REQUIRE(f3->f_count == 9);

        CATCH_REQUIRE_FALSE(m.empty());
        CATCH_REQUIRE(m.clear());
        CATCH_REQUIRE(m.empty());
        CATCH_REQUIRE(m.size() == 0);
        CATCH_REQUIRE_FALSE(m.clear());
        CATCH_REQUIRE_FALSE(m.remove_callback(id1));
        CATCH_REQUIRE_FALSE(m.remove_callback(id3));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("callback manager: direct function callback with 3 parameters")
    {
        g_called = false;

        typedef bool (*callback_t)(int, int, std::string);

        snapdev::callback_manager<callback_t> m;

        CATCH_REQUIRE(m.add_callback(the_static_callback));
        CATCH_REQUIRE_FALSE(m.empty());
        CATCH_REQUIRE(m.size() == 1);

        CATCH_REQUIRE_FALSE(g_called);
        CATCH_REQUIRE(m.call(33, 51, "expected value"));
        CATCH_REQUIRE(g_called);

        g_called = false;
        g_expected_a = 44;
        g_expected_b = 50;
        g_expected_c = "return false";
        CATCH_REQUIRE_FALSE(g_called);
        CATCH_REQUIRE_FALSE(m.call(44, 50, "return false"));
        CATCH_REQUIRE(g_called);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("callback manager: direct function callback with no parameters")
    {
        g_called = false;

        typedef bool (*callback_t)();
        typedef std::list<callback_t> list_t;

        snapdev::callback_manager<callback_t> m;

        CATCH_REQUIRE(m.add_callback(void_static_callback));

        CATCH_REQUIRE_FALSE(g_called);
        CATCH_REQUIRE(m.call());
        CATCH_REQUIRE(g_called);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("callback manager: std::bind() function")
    {
        class bind
        {
        public:
            typedef std::set<bind> set_t;

            bool my_callback(int param)
            {
                g_expected = param;

                return true;
            }

            int g_expected = -1;
        };

        bind b;

        auto f(std::bind(&bind::my_callback, &b, 111));

        snapdev::callback_manager<decltype(f)> m;

        auto const id(m.add_callback(f));

        CATCH_REQUIRE(m.call());

        CATCH_REQUIRE_FALSE(m.empty());
        CATCH_REQUIRE(m.remove_callback(id));
        CATCH_REQUIRE(m.empty());
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("callback manager: verify priority and lambda functions")
    {
        std::vector<snapdev::callback_manager<std::function<void()>>::priority_t> order;

        auto f1 = [&order]()
        {
            order.push_back(1);
        };
        auto f2 = [&order]()
        {
            order.push_back(2);
        };
        auto f3 = [&order]()
        {
            order.push_back(3);
        };
        auto f4 = [&order]()
        {
            order.push_back(4);
        };
        auto f5 = [&order]()
        {
            order.push_back(5);
        };

        struct func_t
        {
            std::function<void()>   f_func = nullptr;
            int                     f_priority = 0;

            bool operator < (func_t b) const
            {
                return f_priority < b.f_priority;
            }
        };

        std::vector<func_t> functions;
        functions.push_back({f1, 1});
        functions.push_back({f2, 2});
        functions.push_back({f3, 3});
        functions.push_back({f4, 4});
        functions.push_back({f5, 5});
        std::sort(functions.begin(), functions.end());

        // we have 5! = 120 permutations to test
        //
        do
        {
            snapdev::callback_manager<std::function<void()>> m;
            for(auto const & f : functions)
            {
                // the order in which these get added changes on each loop
                // but the priority remains the same so the functions get
                // called in the expected order (largest priority first)
                //
                m.add_callback(f.f_func, f.f_priority);
            }
            order.clear();
            m.call();
            CATCH_REQUIRE(m.size() == 5);
            CATCH_REQUIRE_FALSE(m.empty());
            CATCH_REQUIRE(order.size() == 5);
            CATCH_REQUIRE(order[0] == 5);
            CATCH_REQUIRE(order[1] == 4);
            CATCH_REQUIRE(order[2] == 3);
            CATCH_REQUIRE(order[3] == 2);
            CATCH_REQUIRE(order[4] == 1);
            m.clear();
            CATCH_REQUIRE(m.size() == 0);
            CATCH_REQUIRE(m.empty());
        }
        while(std::next_permutation(functions.begin(), functions.end()));
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
