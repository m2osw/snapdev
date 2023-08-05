// Copyright (c) 2018-2023  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Verify that the different remove duplicate algorithms work.
 *
 * This file implements tests for the different remove duplicate functions.
 */

// self
//
#include    <snapdev/remove_duplicates.h>

#include    "catch_main.h"


// C++
//
#include    <deque>
#include    <list>


// last include
//
#include    <snapdev/poison.h>


namespace
{





}
// no name namespace



CATCH_TEST_CASE("sort_and_remove_duplicates", "[remove_duplicates][container]")
{
    CATCH_START_SECTION("sort_and_remove_duplicates: empty container")
    {
        std::vector<std::string> empty;
        std::vector<std::string> *output = &snapdev::sort_and_remove_duplicates(empty);
        CATCH_REQUIRE(&empty == output);
        CATCH_REQUIRE(empty.empty());
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("sort_and_remove_duplicates: container without duplicates")
    {
        for(int count(0); count < 10; ++count)
        {
            std::vector<std::string> container;
            std::size_t size(rand() % 100 + 50);
            while(container.size() < size)
            {
                std::string const s(SNAP_CATCH2_NAMESPACE::random_string(0, rand() % 25));
                if(std::find(container.begin(), container.end(), s) == container.end())
                {
                    container.push_back(s);
                }
            }
            std::vector<std::string> copy(container);
            std::vector<std::string> *output = &snapdev::sort_and_remove_duplicates(container);
            CATCH_REQUIRE(&container == output);
            CATCH_REQUIRE(container.size() == copy.size());

            std::sort(copy.begin(), copy.end());
            CATCH_REQUIRE(container == copy);
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("sort_and_remove_duplicates: container with duplicates")
    {
        for(int count(0); count < 10; ++count)
        {
            std::vector<std::string> container;

            // we want to be in control of the duplication and know exactly what
            // we are going to get back out so we need to create a container
            // without duplicates first then add duplicates in a second loop
            //
            std::size_t size(rand() % 100 + 50);
            while(container.size() < size)
            {
                std::string const s(SNAP_CATCH2_NAMESPACE::random_string(0, rand() % 25));
                if(std::find(container.begin(), container.end(), s) == container.end())
                {
                    container.push_back(s);
                }
            }
            std::vector<std::string> copy(container);
            std::size_t dups(rand() % 50 + 25);
            for(std::size_t i(0); i < dups; ++i)
            {
                int const d(rand() % container.size());
                int const p(rand() % (container.size() + 1));
                container.insert(container.begin() + p, container[d]);
            }
            std::vector<std::string> *output = &snapdev::sort_and_remove_duplicates(container);
            CATCH_REQUIRE(&container == output);
            CATCH_REQUIRE(container.size() == copy.size());

            std::sort(copy.begin(), copy.end());
            CATCH_REQUIRE(container == copy);
        }
    }
    CATCH_END_SECTION()
}


CATCH_TEST_CASE("sorted_remove_duplicates", "[remove_duplicates][container]")
{
    CATCH_START_SECTION("sorted_remove_duplicates: empty container")
    {
        std::vector<std::string> empty;
        std::vector<std::string> *output = &snapdev::sorted_remove_duplicates(empty);
        CATCH_REQUIRE(&empty == output);
        CATCH_REQUIRE(empty.empty());
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("sorted_remove_duplicates: container without duplicates")
    {
        for(int count(0); count < 10; ++count)
        {
            std::vector<std::string> container;
            std::size_t size(rand() % 100 + 50);
            while(container.size() < size)
            {
                std::string const s(SNAP_CATCH2_NAMESPACE::random_string(0, rand() % 25));
                if(std::find(container.begin(), container.end(), s) == container.end())
                {
                    container.push_back(s);
                }
            }
            std::sort(container.begin(), container.end());
            std::vector<std::string> copy(container);
            std::vector<std::string> *output = &snapdev::sorted_remove_duplicates(container);
            CATCH_REQUIRE(&container == output);
            CATCH_REQUIRE(container.size() == copy.size());
            CATCH_REQUIRE(container == copy);
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("sorted_remove_duplicates: container with duplicates")
    {
        for(int count(0); count < 10; ++count)
        {
            std::vector<std::string> container;

            // we want to be in control of the duplication and know exactly what
            // we are going to get back out so we need to create a container
            // without duplicates first then add duplicates in a second loop
            //
            std::size_t size(rand() % 100 + 50);
            while(container.size() < size)
            {
                std::string const s(SNAP_CATCH2_NAMESPACE::random_string(0, rand() % 25));
                if(std::find(container.begin(), container.end(), s) == container.end())
                {
                    container.push_back(s);
                }
            }
            std::vector<std::string> copy(container);
            std::size_t dups(rand() % 50 + 25);
            for(std::size_t i(0); i < dups; ++i)
            {
                int const d(rand() % container.size());
                int const p(rand() % (container.size() + 1));
                container.insert(container.begin() + p, container[d]);
            }
            std::sort(container.begin(), container.end());
            std::vector<std::string> *output = &snapdev::sorted_remove_duplicates(container);
            CATCH_REQUIRE(&container == output);
            CATCH_REQUIRE(container.size() == copy.size());

            std::sort(copy.begin(), copy.end());
            CATCH_REQUIRE(container == copy);
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("sorted_remove_duplicates: container with one item duplicated")
    {
        for(int count(0); count < 10; ++count)
        {
            std::list<std::string> container;

            // we want to be in control of the duplication and know exactly what
            // we are going to get back out so we need to create a container
            // without duplicates first then add duplicates in a second loop
            //
            std::string const s(SNAP_CATCH2_NAMESPACE::random_string(0, rand() % 25));
            std::size_t size(rand() % 100 + 50);
            while(container.size() < size)
            {
                container.push_back(s);
            }
            std::list<std::string> *output = &snapdev::sorted_remove_duplicates(container);
            CATCH_REQUIRE(&container == output);
            CATCH_REQUIRE(container.size() == 1);
            CATCH_REQUIRE(container.front() == s);
        }
    }
    CATCH_END_SECTION()
}


CATCH_TEST_CASE("unsorted_remove_duplicates", "[remove_duplicates][container]")
{
    CATCH_START_SECTION("unsorted_remove_duplicates: empty container")
    {
        std::vector<std::string> empty;
        std::vector<std::string> *output = &snapdev::unsorted_remove_duplicates(empty);
        CATCH_REQUIRE(&empty == output);
        CATCH_REQUIRE(empty.empty());
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("unsorted_remove_duplicates: container without duplicates")
    {
        std::vector<std::string> container;
        std::size_t size(rand() % 100 + 50);
        while(container.size() < size)
        {
            std::string const s(SNAP_CATCH2_NAMESPACE::random_string(0, rand() % 25));
            if(std::find(container.begin(), container.end(), s) == container.end())
            {
                container.push_back(s);
            }
        }
        std::vector<std::string> copy(container);
        std::vector<std::string> *output = &snapdev::unsorted_remove_duplicates(container);
        CATCH_REQUIRE(&container == output);
        CATCH_REQUIRE(container.size() == copy.size());
        CATCH_REQUIRE(container == copy);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("unsorted_remove_duplicates: container with duplicates (vector)")
    {
        std::vector<std::string> container;

        // we want to be in control of the duplication and know exactly what
        // we are going to get back out so we need to create a container
        // without duplicates first then add duplicates in a second loop
        //
        std::size_t size(rand() % 100 + 50);
        while(container.size() < size)
        {
            std::string const s(SNAP_CATCH2_NAMESPACE::random_string(0, rand() % 25));
            if(std::find(container.begin(), container.end(), s) == container.end())
            {
                container.push_back(s);
            }
        }
        std::vector<std::string> copy(container);
        std::size_t dups(rand() % 50 + 25);
        for(std::size_t i(0); i < dups; ++i)
        {
            int const d(rand() % container.size());

            // since items do not get sorted, we need p > d for this test
            // to work properly
            //
            int const p(d + rand() % (container.size() - d) + 1);

            container.insert(container.begin() + p, container[d]);
        }
        std::vector<std::string> *output = &snapdev::unsorted_remove_duplicates(container);
        CATCH_REQUIRE(&container == output);
        CATCH_REQUIRE(container.size() == copy.size());
        CATCH_REQUIRE(container == copy);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("unsorted_remove_duplicates: container with duplicates (list)")
    {
        std::list<std::string> container;

        // we want to be in control of the duplication and know exactly what
        // we are going to get back out so we need to create a container
        // without duplicates first then add duplicates in a second loop
        //
        std::size_t size(rand() % 100 + 50);
        while(container.size() < size)
        {
            std::string const s(SNAP_CATCH2_NAMESPACE::random_string(0, rand() % 25));
            if(std::find(container.begin(), container.end(), s) == container.end())
            {
                container.push_back(s);
            }
        }
        std::list<std::string> copy(container);
        std::size_t dups(rand() % 50 + 25);
        for(std::size_t i(0); i < dups; ++i)
        {
            int const d(rand() % container.size());

            // since items do not get sorted, we need p > d for this test
            // to work properly
            //
            int const p(d + rand() % (container.size() - d) + 1);

            auto d_it(container.begin());
            std::advance(d_it, d);
            auto p_it(container.begin());
            std::advance(p_it, p);
            container.insert(p_it, *d_it);
        }
        std::list<std::string> *output = &snapdev::unsorted_remove_duplicates(container);
        CATCH_REQUIRE(&container == output);
        CATCH_REQUIRE(container.size() == copy.size());
        CATCH_REQUIRE(container == copy);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("unsorted_remove_duplicates: container with one item duplicated")
    {
        for(int count(0); count < 10; ++count)
        {
            std::deque<std::string> container;

            // we want to be in control of the duplication and know exactly what
            // we are going to get back out so we need to create a container
            // without duplicates first then add duplicates in a second loop
            //
            std::string const s(SNAP_CATCH2_NAMESPACE::random_string(0, rand() % 25));
            std::size_t size(rand() % 100 + 50);
            while(container.size() < size)
            {
                container.push_back(s);
            }
            std::deque<std::string> *output = &snapdev::sorted_remove_duplicates(container);
            CATCH_REQUIRE(&container == output);
            CATCH_REQUIRE(container.size() == 1);
            CATCH_REQUIRE(container.front() == s);
        }
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
