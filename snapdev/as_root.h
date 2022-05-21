// Copyright (c) 2016-2022  Made to Order Software Corp.  All Rights Reserved
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
// 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#pragma once

// self
//
#include    <snapdev/not_used.h>


// C++
//
#include    <algorithm>
#include    <string>
#include    <type_traits>



namespace snapdev
{



/** \brief Safely become root and back.
 *
 * This class defines a way to become root and then come back as the
 * original user one your work requiring root is done.
 *
 * The class uses RAII so when an object of that type gets destroyed,
 * it automatically restore the user. You can also restore the user
 * sooner if safer.
 *
 * This class is used only when a process is given root as the owner
 * of the file and when the set user identifier flag is set on the
 * executable (a.k.a. `chmod u+s /usr/bin/my-tool`).
 *
 * The class constructor attempts the change to root. If that fails,
 * the valid() function returns false. It is likely important that
 * you verified whether the user change worked or not to make sure
 * that you can run the following commands as root.
 *
 * The class can be used recursively. So if function A uses as_root,
 * then calls function B which also uses as_root, the second time
 * the switch will do nothing since you are already root. Function A
 * will stil properly restore the user to the normal user instead of
 * root.
 *
 * The class is only expected to be used on the stack. If you use it
 * in an object, then the restore may not work as expected (i.e. it
 * could happen in the wrong order).
 */
class as_root
{
public:
    /** \brief Save the current user then switch to root.
     *
     * This function switches us to the root user. If that fails,
     * then the valid() function will return false. You can get
     * the exact error using the error_number() function.
     *
     * The destructor will automatically revert back to your
     * original user.
     */
    as_root()
        : f_user_uid(getuid())
    {
        if(seteuid(0) != 0)
        {
            f_errno = errno;
        }
    }

    /** \brief Restore the user as it was on construction.
     *
     * This function restores the user as found in the construtor. If
     * the constructor could not switch to root, then the function fails.
     */
    ~as_root()
    {
        if(f_errno == 0)
        {
            NOT_USED(seteuid(f_user_uid));
        }
    }

    /** \brief Check whether the as_root object is considered valid.
     *
     * The constructor of the function attempts to switch to the root
     * user. If the function fails, then the errno value is saved.
     * This function checks that error, if not zero, then the switch
     * to the root user did not happen so the as_root object is not
     * considered valid.
     *
     * To get the actual error number, use the error_number() function.
     *
     * \return true if the constructor worked and thus we are root.
     *
     * \sa error_number()
     */
    bool valid() const
    {
        return f_errno == 0;
    }

    /** \brief Get the error number.
     *
     * The constructor saves the error number of the seteuid() function
     * fails. That error number can be retrieved using this function.
     * If you do not call any other system function, the errno variable
     * will still be set to the correct value, however, you must use
     * the valid() function to make sure that an error actually occurred.
     *
     * \return The errno as set by the seteuid() on an error, otherwise 0.
     */
    int error_number() const
    {
        return f_errno;
    }

private:
    /** \brief The user UID on entry.
     *
     * The constructor saves the current user identifier so we can restore
     * it on exit. This may be 0 if you use the object multiple times in
     * a recursive manner.
     */
    uid_t       f_user_uid = 0;

    /** \brief The error number.
     *
     * If the seteuid() found in the constructor fails, then this value is
     * set to the errno parameter as set by the seteuid() function.
     *
     * By default, the value is 0 which means that the seteuid() function
     * suceeded in changing the user identifier to the root user identifier.
     */
    int         f_errno = 0;
};



} // namespace snapdev
// vim: ts=4 sw=4 et
