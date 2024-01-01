// Copyright (c) 2019-2024  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Verify that the safe_object template works.
 *
 * This file implements tests for the safe_object template to make sure that
 * it works as expected.
 */

// self
//
#include    <snapdev/safe_object.h>

#include    "catch_main.h"


// snapdev
//
#include    <snapdev/not_reached.h>


// last include
//
#include    <snapdev/poison.h>


namespace
{


bool g_allocated = false;

bool object_is_allocated()
{
    return g_allocated;
}

class test_object
{
public:
    test_object()
    {
        g_allocated = true;
    }

    ~test_object()
    {
        g_allocated = false;
    }
};


typedef int     identifier_t;

typedef std::map<identifier_t, bool>        map_id_t;

map_id_t        g_map_ids = map_id_t();

identifier_t alloc_id()
{
    for(;;)
    {
        identifier_t id(rand());
        if(g_map_ids.find(id) == g_map_ids.end())
        {
            g_map_ids[id] = true;
            return id;
        }
    }
}

void delete_id(identifier_t id)
{
    auto it(g_map_ids.find(id));
    CATCH_REQUIRE(it != g_map_ids.end());
    g_map_ids.erase(it);
}

bool id_is_allocated(identifier_t id)
{
    return g_map_ids.find(id) != g_map_ids.end();
}


} // no name namespace


CATCH_TEST_CASE("safe_object", "[raii]")
{
    CATCH_START_SECTION("safe_object: expected usage")
    {
        snapdev::safe_object<test_object *> so;
        CATCH_REQUIRE_FALSE(object_is_allocated());
        test_object * obj(new test_object);
        CATCH_REQUIRE(object_is_allocated());
        so.make_safe(obj);
        CATCH_REQUIRE(obj != nullptr);  // this is always true because if new fails, it throws bad_alloc
        CATCH_REQUIRE(object_is_allocated());
        so.release();
        CATCH_REQUIRE(object_is_allocated());
        delete obj;
        CATCH_REQUIRE_FALSE(object_is_allocated());
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("safe_object: with exception")
    {
        try
        {
            snapdev::safe_object<test_object *> so;
            CATCH_REQUIRE_FALSE(object_is_allocated());
            test_object * obj(new test_object);
            CATCH_REQUIRE(object_is_allocated());
            so.make_safe(obj);
            CATCH_REQUIRE(obj != nullptr);  // this is always true because if new fails, it throws bad_alloc
            CATCH_REQUIRE(object_is_allocated());
            throw std::range_error("test exception");
            snapdev::NOT_REACHED();
        }
        catch(std::exception const & e)
        {
            CATCH_REQUIRE_FALSE(object_is_allocated());
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("safe_object: test both ways")
    {
        for(int idx(0); idx < 10; ++idx)
        {
            try
            {
                snapdev::safe_object<test_object *> so;
                CATCH_REQUIRE_FALSE(object_is_allocated());
                test_object * obj(new test_object);
                CATCH_REQUIRE(object_is_allocated());
                so.make_safe(obj);
                CATCH_REQUIRE(obj != nullptr);  // this is always true because if new fails, it throws bad_alloc
                CATCH_REQUIRE(object_is_allocated());
                if((idx & 1) == 0)
                {
                    throw std::range_error("test exception");
                    snapdev::NOT_REACHED();
                }
                so.release();
                CATCH_REQUIRE(object_is_allocated());
                delete obj;
                CATCH_REQUIRE_FALSE(object_is_allocated());
            }
            catch(std::exception const & e)
            {
                // prevent propagation
            }
            CATCH_REQUIRE_FALSE(object_is_allocated());
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("safe_object: test with resource identifiers")
    {
        for(int idx(0); idx < 10; ++idx)
        {
            identifier_t id(rand());
            try
            {
                snapdev::safe_object<identifier_t, delete_id> so;
                CATCH_REQUIRE_FALSE(id_is_allocated(id));
                id = alloc_id();
                CATCH_REQUIRE(id_is_allocated(id));
                so.make_safe(id);
                CATCH_REQUIRE(id_is_allocated(id));
                if((idx & 1) == 0)
                {
                    throw std::range_error("id exception");
                    snapdev::NOT_REACHED();
                }
                so.release();
                CATCH_REQUIRE(id_is_allocated(id));
                delete_id(id);
                CATCH_REQUIRE_FALSE(id_is_allocated(id));
            }
            catch(std::exception const & e)
            {
                // prevent propagation
            }
            CATCH_REQUIRE_FALSE(id_is_allocated(id));
        }
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et

