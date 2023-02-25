// Copyright (c) 2013-2023  Made to Order Software Corp.  All Rights Reserved
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
 * \brief An RAII chdir() call.
 *
 * This class allows you to change your current directory and have it restored
 * once your stack unfolds. This works even when calling sub-functions which
 * themselves use chdir() through this class.
 */

// self
//
#include    <snapdev/not_used.h>



namespace snapdev
{

/** \brief Change the current working directory until the destructor executes.
 *
 * This class is used to change the current working directory to another
 * and safely come back once you are done with the work in that directory.
 *
 * \code
 *     {
 *         safe_chdir safe("new-path");
 *
 *         ...your code...
 *     }
 *     // here the path is reverted to what it was before this block
 * \endcode
 *
 * \note
 * Changing the working directory in a process using threads is not safe.
 * The current working directory is shared between all threads.
 */
class safe_chdir
{
public:
    /** \brief Save the current working directory and change to the new one.
     *
     * This function saves the current working directory in order to restore
     * it later.
     *
     * It then change the current directory to the specified \p path.
     *
     * \exception std::logic_error
     * If the chdir() fails with EFAULT, then this logic error is raised since
     * this means the input \p path parameter is invalid.
     *
     * \exception std::bad_alloc
     * If the chdir() command fails with ENOMEM, then this exception is
     * raised.
     *
     * \exception std::runtime_error
     * If the chdir() generates an error other than one recognized, then
     * this error is raised. The "what" message will include the error
     * information.
     *
     * \param[in] path  The path to the new working directory.
     */
    safe_chdir(std::string const & path)
        : f_original_path(get_current_dir_name())
    {
        if(f_original_path == nullptr)
        {
            int const e(errno);
            throw std::logic_error(
                  "the current working directory could not be queried (err: "
                + std::to_string(e)
                + ", "
                + strerror(e)
                + ").");
        }

        if(chdir(path.c_str()) != 0)
        {
            if(errno == EFAULT)
            {
                throw std::logic_error("the path variable is invalid.");
            }
            if(errno == ENOMEM)
            {
                throw std::bad_alloc();
            }
            int const e(errno);
            throw std::runtime_error(
                      "chdir() generated error: "
                    + std::to_string(e)
                    + ", "
                    + strerror(e));
        }
    }

    /** \brief Restore the current working directory.
     *
     * The destructor change the directory back to the original path.
     *
     * \note
     * Failure to restore the current path is ignored.
     */
    ~safe_chdir()
    {
        NOT_USED(chdir(f_original_path.c_str()));
    }

    safe_chdir(safe_chdir const & rhs) = delete;
    safe_chdir & operator = (safe_chdir const & rhs) = delete;

private:
    /** \brief The working directory before constructing this object.
     *
     * This variable member holds the original working directory. The one
     * defined before you call this function. This allows this class to
     * safely come back to the original working directory on destruction.
     */
    std::unique_ptr<char>   f_original_path = std::unique_ptr<char>();
};



} // namespace snapdev
// vim: ts=4 sw=4 et
