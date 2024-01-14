// Copyright (c) 2011-2024  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Verify the unique_number class.
 *
 * This file implements tests to verify that the unique_number implementation
 * works properly even when multiple processes use it simultaneously.
 */

// file being tested
//
#include    <snapdev/unique_number.h>


// self
//
#include    "catch_main.h"


// snapdev
//
#include    <snapdev/not_used.h>



// C++
//
#include    <random>
#include    <thread>


// last include
//
#include    <snapdev/poison.h>



// turn off pedantic for __int128
//
#pragma GCC diagnostic ignored "-Wpedantic"


namespace
{


class lockfile_thread
{
public:
    lockfile_thread(
              std::string const & filename
            , snapdev::operation_t operation = snapdev::operation_t::OPERATION_EXCLUSIVE)
        : f_filename(filename)
        , f_operation(operation)
    {
    }

    lockfile_thread(lockfile_thread const &) = delete;
    lockfile_thread & operator = (lockfile_thread const &) = delete;

    ~lockfile_thread()
    {
        if(f_thread != nullptr)
        {
            f_thread->join();
            delete f_thread;
        }
    }

    void start_thread(int try_lock = 0)
    {
        f_running = true;
        if(try_lock)
        {
            f_thread = new std::thread(&lockfile_thread::run_try_lock, this, try_lock);
        }
        else
        {
            f_thread = new std::thread(&lockfile_thread::run, this);
        }
    }

    void run()
    {
        snapdev::lockfile lock(f_filename, f_operation);
        CATCH_REQUIRE_FALSE(lock.is_locked());
        lock.lock();
        CATCH_REQUIRE(lock.is_locked());

        std::lock_guard<std::mutex> guard(f_mutex);
        f_running = false;
    }

    void run_try_lock(int try_lock)
    {
        snapdev::lockfile lock(f_filename, f_operation);
        CATCH_REQUIRE_FALSE(lock.is_locked());
        lock.try_lock();
        if(try_lock == 1)
        {
            CATCH_REQUIRE(lock.is_locked());
        }
        else
        {
            CATCH_REQUIRE_FALSE(lock.is_locked());
        }

        std::lock_guard<std::mutex> guard(f_mutex);
        f_running = false;
    }

    bool is_running() const
    {
        std::lock_guard<std::mutex> guard(f_mutex);
        return f_running;
    }

private:
    mutable std::mutex      f_mutex = std::mutex();
    std::string             f_filename = std::string();
    snapdev::operation_t    f_operation = snapdev::operation_t::OPERATION_EXCLUSIVE;
    std::thread *           f_thread = nullptr;
    bool                    f_running = false;
};


} // no name namespace



CATCH_TEST_CASE("unique_number", "[file]")
{
    CATCH_START_SECTION("unique_number: verify unique number basic counter")
    {
        std::string const path(SNAP_CATCH2_NAMESPACE::g_tmp_dir());
        std::string const filename(path + "/test-1.number");

        snapdev::NOT_USED(unlink(filename.c_str()));

        for(unsigned int n(1); n <= 100; ++n)
        {
            unsigned __int128 const number(snapdev::unique_number(filename));
            CATCH_REQUIRE(n == number);
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("unique_number: verify unique number indexes")
    {
        std::random_device rd;
        std::mt19937 g(rd());

        std::string const path(SNAP_CATCH2_NAMESPACE::g_tmp_dir());
        std::string const filename(path + "/test-2.number");
std::cerr << "--- file: " << filename << "\n";

        snapdev::NOT_USED(unlink(filename.c_str()));

        // 100 indexes repeated 100 times
        //
        // that way we can shuffle all the indexes in order to count them
        // in different order
        //
        std::vector<unsigned int> indexes;
        for(unsigned int count(1); count <= 100; ++count)
        {
            for(unsigned int idx(0); idx < 100; ++idx)
            {
                indexes.push_back(idx);
            }
        }
        std::shuffle(indexes.begin(), indexes.end(), g);

        std::vector<unsigned int> counters(100);
        for(auto idx : indexes)
        {
            unsigned __int128 const number(snapdev::unique_number(filename, idx));
            ++counters[idx];
            CATCH_REQUIRE(counters[idx] == number);
        }
    }
    CATCH_END_SECTION()
}


CATCH_TEST_CASE("unique_number_error", "[file][error]")
{
    CATCH_START_SECTION("unique_number_error: non-empty filename is required")
    {
        CATCH_REQUIRE_THROWS_MATCHES(
                  snapdev::unique_number(std::string())
                , snapdev::path_missing
                , Catch::Matchers::ExceptionMessage(
                          "unique_number_error: a counter filename must be specified when calling snapdev::unique_number."));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("unique_number_error: index out of range")
    {
        std::string const path(SNAP_CATCH2_NAMESPACE::g_tmp_dir());
        std::string const filename(path + "/test-3.number");

        for(int index(-100); index < 0; ++index)
        {
            CATCH_REQUIRE_THROWS_MATCHES(
                      snapdev::unique_number(filename, index)
                    , snapdev::out_of_range
                    , Catch::Matchers::ExceptionMessage(
                              "unique_number_error: counter index in unique_number must be defined between 0 and "
                            + std::to_string(snapdev::COUNTER_MAXIMUM_INDEX - 1)
                            + " inclusive."));
        }

        for(int index(snapdev::COUNTER_MAXIMUM_INDEX); index < snapdev::COUNTER_MAXIMUM_INDEX + 100; ++index)
        {
            CATCH_REQUIRE_THROWS_MATCHES(
                      snapdev::unique_number(filename, index)
                    , snapdev::out_of_range
                    , Catch::Matchers::ExceptionMessage(
                              "unique_number_error: counter index in unique_number must be defined between 0 and "
                            + std::to_string(snapdev::COUNTER_MAXIMUM_INDEX - 1)
                            + " inclusive."));
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("unique_number_error: file cannot be opened")
    {
        CATCH_REQUIRE_THROWS_MATCHES(
                  snapdev::unique_number("/this/wont/open/since/it/does/not/exist")
                , snapdev::io_error
                , Catch::Matchers::ExceptionMessage(
                          "unique_number_error: could not open unique_number file \"/this/wont/open/since/it/does/not/exist\"."));
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
