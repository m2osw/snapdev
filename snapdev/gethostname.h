// Copyright (c) 2011-2022  Made to Order Software Corp.  All Rights Reserved
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
//
#pragma once

/** \file
 * \brief Get the computer hostname.
 *
 * This function calls the ::gethostname() system function to retrieve this
 * computer current hostname. If that fails, it throws, otherwise it returns
 * the name in an std::string.
 */

// C++ lib
//
#include    <string>


// C lib
//
#include    <limits.h>
#include    <string.h>
#include    <unistd.h>


namespace snapdev
{


/** \brief Get the hostname.
 *
 * This function retrieves this server current hostname.
 *
 * \exception std::runtime_error
 * This exception is raised if the hostname is not available.
 *
 * \return The hostname in a string.
 */
std::string gethostname()
{
    std::string result;

    char host[HOST_NAME_MAX + 1];
    host[HOST_NAME_MAX] = '\0';
    host[0] = '\0';
    if(::gethostname(host, sizeof(host)) != 0
    || host[0] == '\0')
    {
        int const e(errno);
        throw std::runtime_error(
                  "::gethostname() failed with "
                + std::to_string(e)
                + ", "
                + strerror(e));
    }

    return std::string(host);
}

} // namespace snapdev
// vim: ts=4 sw=4 et
