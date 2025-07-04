// Copyright (c) 2025  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Get the current user's name.
 *
 * There is always a current user on a Unix system. If 0, it is the root
 * user. Otherwise, it is at least a UID (user identifier) and the UID
 * may be mapped to a user name through the /etc/passwd file.
 *
 * This function helps in retrieving that name which may or may not
 * be in the USER environment variable (i.e. in a CRON environment,
 * the USER variable needs to be explicitly set).
 */

// self
//
#include    <snapdev/static_to_dynamic_buffer.h>


// libexcept
//
//#include    "libexcept/exception.h"


// C++
//
//#include    <algorithm>
#include    <string>
//#include    <cstdint>
//#include    <iomanip>


// C
//
#include    <pwd.h>
#include    <unistd.h>



namespace snapdev
{



/** \brief Retrieve the name of the specified user.
 *
 * This function retrieve the name of the specified user. If the name cannot
 * be gathered, then the function returns an empty string.
 *
 * This function takes a user identifier and converts it to a user name.
 * By default, it uses the getuid() function. If you want to use a different
 * UID, make sure to pass the parameter as required.
 *
 * In effect, by default, the function is equivalent to:
 *
 * \code
 * char const * username(getenv("USER"));
 * \endcode
 *
 * assuming the USER variable is defined properly.
 *
 * On return, if the user was not found, errno should be set to ENOENT.
 * Other errors may occur (interrupt, I/O, permissions, too many files
 * opened, not enough memory).
 *
 * \param[in] uid  The user identifier.
 *
 * \return The name of the user if available, an empty string otherwise.
 */
inline std::string username(uid_t uid = ::getuid())
{
    static_to_dynamic_buffer<char> buf;
    for(;;)
    {
        struct passwd p;
        struct passwd * result(nullptr);
        int const r(getpwuid_r(uid, &p, buf.get(), buf.size(), &result));
        if(r == 0)
        {
            if(result != nullptr)
            {
                return p.pw_name;
            }
            errno = ENOENT;
            break;
        }
        if(r != ERANGE)
        {
            if(r == ENOMEM)
            {
                throw std::bad_alloc();
            }
            break;
        }
        buf.increase_size(1024);        // LCOV_EXCL_LINE
    }                                   // LCOV_EXCL_LINE

    return std::string();
}



} // namespace snapdev
// vim: ts=4 sw=4 et
