// Copyright (c) 2016-2024  Made to Order Software Corp.  All Rights Reserved
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
#pragma once

/** \file
 * \brief Lock a file.
 *
 * The lockfile class is used to have a shared or an exclusive lock to a file.
 * This is particularly useful when you need to tweak resources between
 * multiple processes. In most cases, to have a similar lock between
 * threads, you want to use a mutex, which is much faster.
 *
 * \warning
 * The class itself is not thread safe. However, two instances of the class
 * can be used to create locks within each one of your thread. That works
 * as expected.
 */

// libexcept
//
#include    <libexcept/exception.h>


// snapdev
//
#include    <snapdev/not_reached.h>
#include    <snapdev/raii_generic_deleter.h>


// C
//
#include    <sys/file.h>
#include    <unistd.h>
#include    <string.h>



namespace snapdev
{

DECLARE_MAIN_EXCEPTION(lockfile_error);

DECLARE_LOGIC_ERROR(lockfile_logic_error); // LCOV_EXCL_LINE

DECLARE_EXCEPTION(lockfile_error, file_error);
DECLARE_EXCEPTION(lockfile_error, lock_error); // LCOV_EXCL_LINE
DECLARE_EXCEPTION(lockfile_error, not_locked_error); // LCOV_EXCL_LINE



/** \brief Define how to lock the file.
 *
 * An exclusive lock makes sure only this one process obtains that lock.
 *
 * A shared lock allows anyone who requested for the shared lock to
 * access the resource. Obviously, a shared lock means that you should
 * use the resource in read-only mode.
 *
 * Do not attempt to first create a shared locked and then an exclusive
 * lock. That is likely to get you stuck.
 */
enum class operation_t
{
    OPERATION_EXCLUSIVE,
    OPERATION_SHARED
};



/** \brief Manage a lockfile.
 *
 * This class is expected to be used to manage a lockfile. In most cases,
 * this is used for a lock between processes and the purpose of the file
 * is just and only the lock (i.e. the file remains empty).
 *
 * For a lock on a data file, you may want to consider using the lockfd
 * class instead.
 *
 * This class allows you to copy lockfile objects. Note, however, that
 * the resulting copies point to the same lock file and locking or
 * unlocking explicitly, by calling the lock(), try_lock(), and unlock()
 * functions, affects all the objects (i.e. the lock is inplace for all
 * after a call to lock() and released after calling unlock()).
 *
 * The class works in an RAII setup. So you can just create the lockfile
 * on your stack and once done with that current block of code, it
 * automatically unlocks the file.
 *
 * \code
 *     {
 *         lockfile interprocess_lock("/run/snapwebsites/interprocess.lock");
 *         interprocess_lock.lock();
 *
 *         ...do things that require the lock...
 *     } // auto-unlock happens here
 * \endcode
 *
 * There is an unlock() function since once in a while it is very useful
 * to unlock your file before the close of the current block of code
 * happens.
 */
class lockfile
{
private:
    class lock_t
    {
    public:
        typedef std::shared_ptr<lock_t>     pointer_t;

        lock_t(std::string const & path, operation_t operation, mode_t mode)
            : f_path(path)
            , f_operation(operation == operation_t::OPERATION_EXCLUSIVE ? LOCK_EX : LOCK_SH)
            , f_fd(::open(f_path.c_str(), O_WRONLY | O_CREAT, mode))
        {
            // if we cannot open/create the file we cannot handle the lock
            // at all so we throw
            //
            if(f_fd == nullptr)
            {
                int const e(errno);
                throw file_error(
                          "Error creating lock file \""
                        + f_path
                        + "\" (errno: "
                        + std::to_string(e)
                        + ", "
                        + strerror(e)
                        + ").");
            }
        }

        std::string const & get_path() const
        {
            return f_path;
        }

        operation_t get_operation() const
        {
            switch(f_operation)
            {
            case LOCK_SH:
                return operation_t::OPERATION_SHARED;

            case LOCK_EX:
                return operation_t::OPERATION_EXCLUSIVE;

            default: // LCOV_EXCL_LINE
                throw lockfile_logic_error("Unknown operation mode."); // LCOV_EXCL_LINE

            }
            NOT_REACHED(); // LCOV_EXCL_LINE
        }

        void lock()
        {
            if(!f_locked)
            {
                int const r(::flock(f_fd.get(), f_operation));
                if(r != 0)
                {
                    // the only way I can see to get this error in a unit
                    // test would be to emit a signal (kill(SIGINT)) so
                    // for now just ignore
                    //
                    // LCOV_EXCL_START
                    int const e(errno);
                    throw lock_error(
                              "Lock \""
                            + f_path
                            + "\" could not be obtained (errno: "
                            + std::to_string(e)
                            + ", "
                            + strerror(e)
                            + ").");
                    // LCOV_EXCL_STOP
                }
                f_locked = true;
            }
        }

        bool try_lock()
        {
            if(!f_locked)
            {
                int const r(::flock(f_fd.get(), f_operation | LOCK_NB));
                if(r != 0)
                {
                    if(errno == EWOULDBLOCK)
                    {
                        return false;
                    }

                    // LCOV_EXCL_START
                    int const e(errno);
                    throw lock_error(
                                      "Lock \""
                                    + f_path
                                    + "\" could not be obtained (errno: "
                                    + std::to_string(e)
                                    + ", "
                                    + strerror(e)
                                    + ").");
                    // LCOV_EXCL_STOP
                }
                f_locked = true;
            }

            return true;
        }

        void unlock()
        {
            if(f_locked)
            {
                ::flock(f_fd.get(), LOCK_UN);
                f_locked = false;
            }
        }

        bool is_locked() const
        {
            return f_locked;
        }

    private:
        std::string         f_path = std::string();
        int                 f_operation = LOCK_EX;
        raii_fd_t           f_fd = raii_fd_t();
        bool                f_locked = false;
    };

public:
    /** \brief Initialize the lock file with its filename.
     *
     * This function sets up a file for interprocess locking. If the
     * file does not yet exists, it gets created.
     *
     * By deleting all instances of a lockfile and its copies, the file
     * automatically gets unlocked.
     *
     * \warning
     * The file is **not** locked by the constructor. You must call the
     * lock() function. This also gives you the ability to call
     * try_lock() instead and avoid being stuck waiting for the lock.
     *
     * \note
     * A lockfile can be copied. There is still only one single lock.
     * If you call the lock() and unlock() functions, it acts on all
     * the your lockfile instances.
     *
     * \exception file_error
     * This exception is raised if the file at \p path cannot be opened or
     * created.
     *
     * \param[in] path  The path to the lockfile.
     * \param[in] operation  The lock can be either shared or exclusive.
     * \param[in] mode  The mode used when creating the file.
     *
     * \sa lock()
     * \sa try_lock()
     */
    lockfile(
              std::string const & path
            , operation_t operation = operation_t::OPERATION_EXCLUSIVE
            , mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH)
        : f_lock(std::make_shared<lock_t>(path, operation, mode))
    {
    }

    /** \brief Get the path.
     *
     * This function returns the path used to open/create the lock file
     * used here.
     *
     * \return The path to the lockfile.
     */
    std::string const & get_path() const
    {
        return f_lock->get_path();
    }

    /** \brief Get the lock operation.
     *
     * This function returns the operation used to create the lock.
     * This is one of the operation_t values.
     *
     * \return The lock operation mode.
     */
    operation_t get_operation() const
    {
        return f_lock->get_operation();
    }

    /** \brief Lock the file.
     *
     * Calling this function locks the file. At this point it is not
     * possible to change the mode or the path to the lock file.
     *
     * If the file is already locked, nothing happens.
     * Note that there is no counter. There is no way for this
     * class to know whether the file was locked more than once.
     * (The destructor would have a problem otherwise!) If you
     * want to lock a file \em more \em than \em once then you
     * want to make copies of the lock file and not explicitly call
     * the unlock() function. This is viewed as a recursive lock
     * (just like you have recursive mutexes between threads).
     *
     * This function blocks until the lock is obtained or the function
     * fails with an exception.
     *
     * \warning
     * If your code makes copies of a lockfile, then calling this function
     * locks the file for all the copies. Once a file is locked and then
     * you make copies, you probably should not explicitly call this
     * function anymore.
     *
     * \exception lock_error
     * This exception is raised if the flock() system function returns
     * an error.
     *
     * \sa try_lock()
     */
    void lock()
    {
        f_lock->lock();
    }

    /** \brief Try to lock the file.
     *
     * This function tries to lock the file. If the lock cannot be
     * obtained at this time, it is expected to return immediately
     * with false. Otherwise, the function works like the lock()
     * function.
     *
     * \exception lock_error
     * This exception is raised if the flock() system function returns
     * an error.
     *
     * \return true if the file is locked on return, false otherwise.
     *
     * \sa lock()
     */
    bool try_lock()
    {
        return f_lock->try_lock();
    }

    /** \brief Release the lock.
     *
     * This function releases the lock and keeps the file open so you
     * can later call lock() or try_lock() to re-obtain the lock.
     *
     * If the file was not currently locked, nothing happens.
     *
     * \warning
     * If your code makes copies of a lockfile, then calling this function
     * unlocks the file for all the copies. You probably should not
     * explicitly call this function if you make copies.
     */
    void unlock()
    {
        f_lock->unlock();
    }

    /** \brief Check whether the lock is in effect.
     *
     * This function returns true if a previous call to lock() or
     * try_lock() actually locked the file and no calls to unlock()
     * took place so far.
     *
     * \return true if the file is currently locked, false otherwise.
     */
    bool is_locked() const
    {
        return f_lock->is_locked();
    }

private:
    lock_t::pointer_t   f_lock = lock_t::pointer_t();
};




/** \brief Lock a file descriptor.
 *
 * This class is used to lock a file descriptor on your stack. This
 * can be used in exclusive mode to prevent any other process from
 * using the same file, or in shared mode, in which case it
 * prevents other process from modifying the file.
 *
 * This class is really only expected to be used on your stack or as
 * a field of a short lived object.
 *
 * The lock() and unlock() functions offer you to lock and unlock the
 * file any number of times. However, the lock is not cumulative. So
 * calling lock() more than once does not lock the file more than once.
 * So a single unlock() will unlock that file at that point.
 */
class lockfd
{
public:
    lockfd(int fd, operation_t operation = operation_t::OPERATION_EXCLUSIVE)
        : f_fd(fd)
        , f_operation(operation == operation_t::OPERATION_EXCLUSIVE ? LOCK_EX : LOCK_SH)
    {
        lock();
    }

    lockfd(lockfd const &) = delete;

    ~lockfd()
    {
        unlock();
    }

    lockfd & operator = (lockfd const &) = delete;

    bool lock()
    {
        if(!f_locked && f_fd != -1)
        {
            flock(f_fd, f_operation);
            f_locked = true;
        }
        return f_locked;
    }

    void unlock()
    {
        if(f_locked)
        {
            flock(f_fd, LOCK_UN);
            f_locked = false;
        }
    }

    bool is_locked() const
    {
        return f_locked;
    }

private:
    int                 f_fd = -1;
    int                 f_operation = LOCK_EX;
    bool                f_locked = false;
};



} // namespace snapdev
// vim: ts=4 sw=4 et
