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
 * \brief Verify that the lockfile classes work as expected.
 *
 * This file implements tests to verify the lockfile and lockfd functionality.
 */

// file being tested
//
#include    <snapdev/lockfile.h>


// self
//
#include    "catch_main.h"


// snapdev
//
#include    <snapdev/timespec_ex.h>


// C++
//
#include    <thread>


// last include
//
#include    <snapdev/poison.h>



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



CATCH_TEST_CASE("lockfile", "[lock][file]")
{
    CATCH_START_SECTION("lockfile: simple lock/unlock test")
    {
        std::string const path(SNAP_CATCH2_NAMESPACE::g_tmp_dir());
        std::string const filename(path + "/test-1.lock");
        snapdev::lockfile lock(filename);
        CATCH_REQUIRE_FALSE(lock.is_locked());
        lock.lock();
        CATCH_REQUIRE(lock.is_locked());
        lock.lock();
        CATCH_REQUIRE(lock.is_locked());
        lock.unlock();
        CATCH_REQUIRE_FALSE(lock.is_locked());
        lock.unlock();
        CATCH_REQUIRE_FALSE(lock.is_locked());

        CATCH_REQUIRE(lock.get_path() == filename);
        CATCH_REQUIRE(lock.get_operation() == snapdev::operation_t::OPERATION_EXCLUSIVE);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("lockfile: test a lock with a thread")
    {
        std::string const path(SNAP_CATCH2_NAMESPACE::g_tmp_dir());
        std::string const filename(path + "/test-2.lock");

        snapdev::lockfile lock(filename);
        CATCH_REQUIRE_FALSE(lock.is_locked());
        lock.lock();
        CATCH_REQUIRE(lock.is_locked());

        lockfile_thread t(filename);
        t.start_thread();
        CATCH_REQUIRE(t.is_running());

        // still locked
        //
        CATCH_REQUIRE(lock.is_locked());
        CATCH_REQUIRE(t.is_running());

        // sleep a bit to make sure things stay locked
        //
        sleep(2);
        CATCH_REQUIRE(lock.is_locked());
        CATCH_REQUIRE(t.is_running());

        lock.unlock();
        CATCH_REQUIRE_FALSE(lock.is_locked());

        for(;;)
        {
            if(!t.is_running())
            {
                break;
            }
            snapdev::timespec_ex wait(0.001); // 1ms
            CATCH_REQUIRE(nanosleep(&wait, nullptr) == 0);
        }

        CATCH_REQUIRE_FALSE(lock.is_locked());
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("lockfile: verify the automatic unlock with a thread")
    {
        std::string const path(SNAP_CATCH2_NAMESPACE::g_tmp_dir());
        std::string const filename(path + "/test-3.lock");

        snapdev::lockfile * lock(new snapdev::lockfile(filename));
        CATCH_REQUIRE_FALSE(lock->is_locked());
        lock->lock();
        CATCH_REQUIRE(lock->is_locked());

        lockfile_thread t(filename);
        t.start_thread();
        CATCH_REQUIRE(t.is_running());

        // still locked
        //
        CATCH_REQUIRE(lock->is_locked());
        CATCH_REQUIRE(t.is_running());

        // sleep a bit to make sure things stay locked
        //
        sleep(2);
        CATCH_REQUIRE(lock->is_locked());
        CATCH_REQUIRE(t.is_running());

        // verify that the delete removes the lock
        //
        delete lock;

        for(;;)
        {
            if(!t.is_running())
            {
                break;
            }
            snapdev::timespec_ex wait(0.001); // 1ms
            CATCH_REQUIRE(nanosleep(&wait, nullptr) == 0);
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("lockfile: a shared lock does not prevent the thread from finishing")
    {
        std::string const path(SNAP_CATCH2_NAMESPACE::g_tmp_dir());
        std::string const filename(path + "/test-4.lock");

        snapdev::lockfile lock(filename, snapdev::operation_t::OPERATION_SHARED);
        CATCH_REQUIRE_FALSE(lock.is_locked());
        lock.lock();
        CATCH_REQUIRE(lock.is_locked());

        lockfile_thread t(filename, snapdev::operation_t::OPERATION_SHARED);
        t.start_thread();
        //CATCH_REQUIRE(t.is_running()); -- the thread may return at any time, so we cannot check whether it is running

        // still locked
        //
        CATCH_REQUIRE(lock.is_locked());

        // sleep a bit to make sure things stay locked
        //
        sleep(2);
        CATCH_REQUIRE(lock.is_locked());

        // the thread will die on its own without us having to unlock()
        //
        for(;;)
        {
            if(!t.is_running())
            {
                break;
            }
            //snapdev::timespec_ex wait(0.001); // 1ms
            //CATCH_REQUIRE(nanosleep(&wait, nullptr) == 0);
            sleep(1);
        }

        // still locked and the thread returned as expected
        //
        CATCH_REQUIRE(lock.is_locked());
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("lockfile: test a try_lock() with a thread")
    {
        std::string const path(SNAP_CATCH2_NAMESPACE::g_tmp_dir());
        std::string const filename(path + "/test-5.lock");

        snapdev::lockfile lock(filename);
        CATCH_REQUIRE_FALSE(lock.is_locked());
        lock.lock();
        CATCH_REQUIRE(lock.is_locked());

        lockfile_thread t(filename);
        t.start_thread(2);  // the try_lock will fail
        //CATCH_REQUIRE(t.is_running()); -- the thread may return at any time, so we cannot check whether it is running

        // still locked
        //
        CATCH_REQUIRE(lock.is_locked());

        // sleep a bit to make sure things stay locked
        //
        sleep(2);
        CATCH_REQUIRE(lock.is_locked());

        for(;;)
        {
            if(!t.is_running())
            {
                break;
            }
            snapdev::timespec_ex wait(0.001); // 1ms
            CATCH_REQUIRE(nanosleep(&wait, nullptr) == 0);
        }

        // still locked and the thread returned as expected
        //
        CATCH_REQUIRE(lock.is_locked());
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("lockfile: test a shared lock and try_lock() with a thread")
    {
        std::string const path(SNAP_CATCH2_NAMESPACE::g_tmp_dir());
        std::string const filename(path + "/test-6.lock");

        snapdev::lockfile lock(filename, snapdev::operation_t::OPERATION_SHARED);
        CATCH_REQUIRE_FALSE(lock.is_locked());
        lock.lock();
        CATCH_REQUIRE(lock.is_locked());

        lockfile_thread t(filename, snapdev::operation_t::OPERATION_SHARED);
        t.start_thread(1);  // the try_lock will succeed
        //CATCH_REQUIRE(t.is_running()); -- the thread may return at any time, so we cannot check whether it is running

        // still locked
        //
        CATCH_REQUIRE(lock.is_locked());

        // sleep a bit to make sure things stay locked
        //
        sleep(2);
        CATCH_REQUIRE(lock.is_locked());

        for(;;)
        {
            if(!t.is_running())
            {
                break;
            }
            snapdev::timespec_ex wait(0.001); // 1ms
            CATCH_REQUIRE(nanosleep(&wait, nullptr) == 0);
        }

        // still locked and the thread returned as expected
        //
        CATCH_REQUIRE(lock.is_locked());
        CATCH_REQUIRE(lock.get_operation() == snapdev::operation_t::OPERATION_SHARED);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("lockfile: test an exclusive lock and a shared try_lock() lock with a thread")
    {
        std::string const path(SNAP_CATCH2_NAMESPACE::g_tmp_dir());
        std::string const filename(path + "/test-7.lock");

        snapdev::lockfile lock(filename);
        CATCH_REQUIRE_FALSE(lock.is_locked());
        lock.lock();
        CATCH_REQUIRE(lock.is_locked());

        lockfile_thread t(filename, snapdev::operation_t::OPERATION_SHARED);
        t.start_thread(2);  // the try_lock will fail anyway
        //CATCH_REQUIRE(t.is_running()); -- the thread may return at any time, so we cannot check whether it is running

        // still locked
        //
        CATCH_REQUIRE(lock.is_locked());

        // sleep a bit to make sure things stay locked
        //
        sleep(2);
        CATCH_REQUIRE(lock.is_locked());

        for(;;)
        {
            if(!t.is_running())
            {
                break;
            }
            snapdev::timespec_ex wait(0.001); // 1ms
            CATCH_REQUIRE(nanosleep(&wait, nullptr) == 0);
        }

        // still locked and the thread returned as expected
        //
        CATCH_REQUIRE(lock.is_locked());
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("lockfile: test a shared lock and an exclusive try_lock() lock with a thread")
    {
        std::string const path(SNAP_CATCH2_NAMESPACE::g_tmp_dir());
        std::string const filename(path + "/test-8.lock");

        snapdev::lockfile lock(filename, snapdev::operation_t::OPERATION_SHARED);
        CATCH_REQUIRE_FALSE(lock.is_locked());
        lock.lock();
        CATCH_REQUIRE(lock.is_locked());

        lockfile_thread t(filename);
        t.start_thread(2);  // the try_lock will fail anyway
        //CATCH_REQUIRE(t.is_running()); -- the thread may return at any time, so we cannot check whether it is running

        // still locked
        //
        CATCH_REQUIRE(lock.is_locked());

        // sleep a bit to make sure things stay locked
        //
        sleep(2);
        CATCH_REQUIRE(lock.is_locked());

        for(;;)
        {
            if(!t.is_running())
            {
                break;
            }
            snapdev::timespec_ex wait(0.001); // 1ms
            CATCH_REQUIRE(nanosleep(&wait, nullptr) == 0);
        }

        // still locked and the thread returned as expected
        //
        CATCH_REQUIRE(lock.is_locked());
        CATCH_REQUIRE(lock.get_operation() == snapdev::operation_t::OPERATION_SHARED);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("lockfile: test copying a lockfile")
    {
        std::string const path(SNAP_CATCH2_NAMESPACE::g_tmp_dir());
        std::string const filename(path + "/test-9.lock");
        snapdev::lockfile lock(filename);
        CATCH_REQUIRE_FALSE(lock.is_locked());
        lock.lock();
        CATCH_REQUIRE(lock.is_locked());
        lock.lock();
        CATCH_REQUIRE(lock.is_locked());
        lock.unlock();
        CATCH_REQUIRE_FALSE(lock.is_locked());
        lock.unlock();
        CATCH_REQUIRE_FALSE(lock.is_locked());

        {
            snapdev::lockfile copy(lock);

            CATCH_REQUIRE_FALSE(lock.is_locked());
            CATCH_REQUIRE_FALSE(copy.is_locked());
            lock.lock();
            CATCH_REQUIRE(lock.is_locked());
            CATCH_REQUIRE(copy.is_locked());

            CATCH_REQUIRE(copy.get_path() == filename);
            CATCH_REQUIRE(copy.get_operation() == snapdev::operation_t::OPERATION_EXCLUSIVE);
        }

        {
            snapdev::lockfile copy(lock);

            CATCH_REQUIRE(lock.is_locked());
            CATCH_REQUIRE(copy.is_locked());
            lock.unlock();
            CATCH_REQUIRE_FALSE(lock.is_locked());
            CATCH_REQUIRE_FALSE(copy.is_locked());

            CATCH_REQUIRE(copy.get_path() == filename);
            CATCH_REQUIRE(copy.get_operation() == snapdev::operation_t::OPERATION_EXCLUSIVE);
        }

        CATCH_REQUIRE(lock.get_path() == filename);
        CATCH_REQUIRE(lock.get_operation() == snapdev::operation_t::OPERATION_EXCLUSIVE);
    }
    CATCH_END_SECTION()
}


CATCH_TEST_CASE("lockfile_error", "[lock][file]")
{
    CATCH_START_SECTION("lockfile_error: no path generates an error trying to create the lock file")
    {
        int const e(ENOENT);
        CATCH_REQUIRE_THROWS_MATCHES(
                  snapdev::lockfile(std::string())
                , snapdev::file_error
                , Catch::Matchers::ExceptionMessage(
                          "lockfile_error: Error creating lock file \"\" (errno: "
                        + std::to_string(e)
                        + ", "
                        + strerror(e)
                        + ")."));
    }
    CATCH_END_SECTION()
}


CATCH_TEST_CASE("lockfd", "[lock][file]")
{
    CATCH_START_SECTION("lockfd: test exclusive lock with -1, nothing happens")
    {
        snapdev::lockfd lock(-1);
        CATCH_REQUIRE_FALSE(lock.is_locked());
        CATCH_REQUIRE_FALSE(lock.lock());
        CATCH_REQUIRE_FALSE(lock.is_locked());
        lock.unlock();
        CATCH_REQUIRE_FALSE(lock.is_locked());
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("lockfd: test shared lock with -1, nothing happens")
    {
        snapdev::lockfd lock(-1, snapdev::operation_t::OPERATION_SHARED);
        CATCH_REQUIRE_FALSE(lock.is_locked());
        CATCH_REQUIRE_FALSE(lock.lock());
        CATCH_REQUIRE_FALSE(lock.is_locked());
        lock.unlock();
        CATCH_REQUIRE_FALSE(lock.is_locked());
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("lockfd: test shared lock with actual file")
    {
        std::string const path(SNAP_CATCH2_NAMESPACE::g_tmp_dir());
        std::string const filename(path + "/fd-test-1.lock");
        int const fd(::open(filename.c_str(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH));
        {
            snapdev::lockfd lock(fd, snapdev::operation_t::OPERATION_SHARED);
            CATCH_REQUIRE(lock.is_locked());
            lock.unlock();
            CATCH_REQUIRE_FALSE(lock.is_locked());
            CATCH_REQUIRE(lock.lock());
            CATCH_REQUIRE(lock.is_locked());
        }
        close(fd);
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
