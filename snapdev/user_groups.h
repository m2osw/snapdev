// Copyright (c) 2021-2022  Made to Order Software Corp.  All Rights Reserved
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

// C++ lib
//
#include    <set>
#include    <string>
#include    <vector>


// C lib
//
#include    <grp.h>
#include    <pwd.h>



namespace snapdev
{


/** \brief Get the set of user group names.
 *
 * This function creates a set of group names a user is a part of.
 *
 * \tparam ContainterT  The type of container used to output the tokens.
 * \param[in] user  The name of the user.
 *
 * \return A set of strings with the group names.
 */
template<class ContainerT>
ContainerT user_group_names(std::string const & user)
{
    struct passwd * pw(getpwnam(user.c_str()));
    if(pw == nullptr)
    {
        return ContainerT();
    }

    gid_t group(-1);
    int count(1);
    int r(getgrouplist(
              user.c_str()
            , pw->pw_gid
            , &group
            , &count));

    if(count <= 0)
    {
        return ContainerT();        // LCOV_EXCL_LINE
    }

    std::vector<gid_t> group_list(count);
    r = getgrouplist(
              user.c_str()
            , pw->pw_gid
            , group_list.data()
            , &count);
    if(r != count)
    {
        return ContainerT();        // LCOV_EXCL_LINE
    }

    ContainerT result;
    for(auto const gid : group_list)
    {
        struct group * grp(getgrgid(gid));
        if(grp == nullptr)  // I don't think this can happen... although it could be deleted under our feet
        {
            // Note: only the root user could really do this
            //       so that's why I had to exclude this line
            //
            result.insert(std::to_string(gid));        // LCOV_EXCL_LINE
        }
        else
        {
            result.insert(grp->gr_name);
        }
    }

    return result;
}



} // namespace snapdev
// vim: ts=4 sw=4 et
