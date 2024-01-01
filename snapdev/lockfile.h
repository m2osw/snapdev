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
 * The lockfile class is used to have a shared or an exclusive lock.
 * This is particularly useful when you need to tweak resources between
 * multiple processes. In most cases, to have a similar lock between
 * threads, you want to use a mutex, which is much faster.
 */

// libexcept
//
#include    <libexcept/exception.h>

// C
//
#include    <sys/file.h>
#include    <unistd.h>
#include    <string.h>



namespace snapdev
{

DECLARE_MAIN_EXCEPTION(lockfile_error);

DECLARE_EXCEPTION(lockfile_error, lockfile_file_error);
DECLARE_EXCEPTION(lockfile_error, lockfile_lock_error);
DECLARE_EXCEPTION(lockfile_error, lockfile_not_locked_error);



// TODO: move implementation to a .cpp file?
class lockfile
{
public:
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
    enum class mode_t
    {
        LOCKFILE_EXCLUSIVE,
        LOCKFILE_SHARED
    };

    /** \brief Initialize the lock file with its filename.
     *
     * This function sets up a file for locking. If the file does not yet
     * exists, it creates it.
     *
     * \warning
     * The file is not locked once the lockfile is initializes by
     * the constructor. You must call the lock() function.
     *
     * \exception snap_process_exception_file_error
     * This exception is raised if the file at \p path can't be opened or
     * created.
     *
     * \param[in] path  The path to the lockfile.
     * \param[in] mode  The lock can be either shared or exclusive.
     *
     * \sa lock()
     * \sa try_lock()
     */
    lockfile(std::string const & path, mode_t mode)
        : f_path(path)
        , f_mode(mode == mode_t::LOCKFILE_EXCLUSIVE ? LOCK_EX : LOCK_SH)
        , f_fd(::open(f_path.c_str()
             , O_WRONLY | O_CREAT
             , S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH))
    {
        // if we cannot open/create the file we cannot handle the lock
        // at all so we throw
        //
        if(f_fd == -1)
        {
            int const e(errno);
            throw lockfile_file_error(
                      "Error creating lock file \""
                    + f_path
                    + "\" (errno: "
                    + std::to_string(e)
                    + ", "
                    + strerror(e)
                    + ")");
        }
    }

    /** \brief Copy a lockfile in another.
     *
     * This copy constructor copies the \p rhs lockfile in this new
     * lockfile.
     *
     * You may make as many copies as you need (although there is a
     * limit to the number of open file descriptor...) Note that
     * all the copies and \p rhs need to all get destroyed before
     * the lock gets released.
     *
     * \warning
     * On return, the file in \p rhs is locked by one more instance
     * and the file that was in \p this is one instance closer to
     * get unlocked.
     *
     * \exception snap_process_exception_not_locked_error
     * This exception gets raised in the event the lock is not active.
     *
     * \param[in] rhs  The right hand side.
     */
    lockfile(lockfile const & rhs)
        : f_path(rhs.f_path)
        , f_mode(rhs.f_mode)
        , f_locked(rhs.f_locked)
    {
        if(!f_locked)
        {
            throw lockfile_not_locked_error(
                      "Only locked files can be copied. \""
                    + f_path
                    + "\" is not currently locked.");
        }
        f_fd = dup(rhs.f_fd);
        if(f_fd == -1)
        {
            int const e(errno);
            throw lockfile_not_locked_error(
                      "Copying file \""
                    + f_path
                    + "\" failed (errno: "
                    + std::to_string(e)
                    + ", "
                    + strerror(e)
                    + ")");
        }
    }

    /** \brief Make sure that the lock file gets closed.
     *
     * This function closes the lock file and returns. Note that a lock
     * file can be duplicated (copied) and when that happens, the lock
     * is attached to the duplicate. In order for the lock to be
     * released all the copies must be destroyed.
     */
    ~lockfile()
    {
        ::close(f_fd);
    }

    /** \brief Copy the \p rhs to this lockfile.
     *
     * This function closes the existing lockfile handle and then copies
     * the \p rhs in this lockfile instead.
     *
     * Note that in effect this someone acts like an unlock feature
     * although if 'this' has copies, the lock will not be release
     * just yet.
     *
     * \warning
     * On return, the file in \p rhs is locked by one more instance
     * and the file that was in \p this is one instance closer to
     * get unlocked, unless (&rhs == this) is true.
     *
     * \exception snap_process_exception_not_locked_error
     * This exception gets raised in the event the lock is not active.
     *
     * \param[in] rhs  The source to copy in this lockfile.
     */
    lockfile & operator = (lockfile const & rhs)
    {
        if(this != &rhs)
        {
            if(!rhs.f_locked)
            {
                throw lockfile_not_locked_error(
                          "Only locked files can be assigned to other lockfile objects (file \""
                        + f_path
                        + "\" is not current locked).");
            }

            ::close(f_fd);

            f_path = rhs.f_path;
            f_mode = rhs.f_mode;
            f_fd = dup(rhs.f_fd);
            f_locked = true;

            if(f_fd == -1)
            {
                int const e(errno);
                throw lockfile_not_locked_error(
                          "Copying file \""
                        + f_path
                        + "\" failed (errno: "
                        + std::to_string(e)
                        + ", "
                        + strerror(e)
                        + ").");
            }
        }

        return *this;
    }

    /** \brief Actually lock the file.
     *
     * This function is one that locks the file. At this point it
     * is not possible to change the mode or the path to the lock file.
     *
     * If the file is already locked, nothing happens.
     * Note that there is no counter. There is no way for this
     * class to know whether the file was locked more than once.
     * (The destructor would have a problem otherwise!) If you
     * want to lock a file \em more \em than \em once then you
     * want to make copies of the lock file.
     *
     * This function blocks until the lock is obtained or the function
     * fails with an exception.
     *
     * \exception snap_process_exception_lock_error
     * This exception is raised if the flock() function returns an
     * error.
     *
     * \sa try_lock()
     */
    void lock()
    {
        if(!f_locked)
        {
            int const r(::flock(f_fd, f_mode));
            if(r != 0)
            {
                int const e(errno);
                throw lockfile_lock_error(
                                  "Lock \""
                                + f_path
                                + "\" could not be obtained (errno: "
                                + std::to_string(e)
                                + ", "
                                + strerror(e)
                                + ").");
            }
            f_locked = true;
        }
    }

    /** \brief Try to lock the file.
     *
     * This function is one that locks the file. At this point it
     * is not possible to change the mode or the path to the lock file.
     *
     * If the file is already locked, nothing happens.
     * Note that there is no counter. There is no way for this
     * class to know whether the file was locked more than once.
     * (The destructor would have a problem otherwise!) If you
     * want to lock a file \em more \em than \em once then you
     * want to make copies of the lock file.
     *
     * This function tries the lock. If the lock cannot be obtained
     * at this time, it is expected to return immediately with false.
     *
     * \exception snap_process_exception_lock_error
     * This exception is raised if the flock() function returns an
     * error.
     *
     * \param[in] non_blocking  Whether to block until we gain access.
     *
     * \return true if the lock took, false otherwise.
     *
     * \sa lock()
     */
    bool try_lock()
    {
        if(!f_locked)
        {
            int const r(::flock(f_fd, f_mode | LOCK_NB));
            if(r != 0)
            {
                if(errno == EWOULDBLOCK)
                {
                    return false;
                }
                int const e(errno);
                throw lockfile_lock_error(
                                  "Lock \""
                                + f_path
                                + "\" could not be obtained (errno: "
                                + std::to_string(e)
                                + ", "
                                + strerror(e)
                                + ").");
            }
            f_locked = true;
        }
        return true;
    }

    /** \brief Release the lock.
     *
     * This function releases the lock and keeps the file open.
     *
     * If the file was not currently locked, nothing happens.
     */
    void unlock()
    {
        if(f_locked && f_fd != -1)
        {
            ::flock(f_fd, LOCK_UN);
            f_locked = false;
        }
    }

    /** \brief Check whether the lock is in effect.
     *
     * This function returns true if a previous call to lock() or
     * try_lock() actually locked the file.
     *
     * \note
     * When a lock function fails, the file is likely going to be closed
     * automatically. Once the file is closed, it always remains unlocked.
     *
     * \return true if the file is currently locked, false otherwise.
     */
    bool is_locked() const
    {
        return f_locked && f_fd != -1;
    }

private:
    std::string         f_path;
    int                 f_mode = LOCK_EX;
    int                 f_fd = -1;
    bool                f_locked = false;
};





class lockfd
{
public:
    enum class mode_t
    {
        LOCKFILE_EXCLUSIVE,
        LOCKFILE_SHARED
    };

    lockfd(int fd, mode_t mode)
        : f_fd(fd)
        , f_mode(mode == mode_t::LOCKFILE_EXCLUSIVE ? LOCK_EX : LOCK_SH)
    {
        if(f_fd != -1)
        {
            flock(f_fd, f_mode);
            f_locked = true;
        }
    }

    ~lockfd()
    {
        unlock();
    }

    bool lock()
    {
        if(!f_locked && f_fd != -1)
        {
            flock(f_fd, f_mode);
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
    int                 f_mode = LOCK_EX;
    bool                f_locked = false;
};




} // namespace snapdev
// vim: ts=4 sw=4 et
