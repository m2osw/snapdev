// Copyright (c) 2016-2025  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Change the user and group ownership of a file.
 *
 * This implementation allows for changing the ownership of a file by name.
 * The default C system function requires the user identifier and the group
 * identifier. Our function accepts strings with the names and automatically
 * converts the names in a string.
 *
 * The chownnm() is defined four times to allow for the user and/or the
 * group to be passed either as a string or as an identifier.
 */

// C++
//
#include    <string>
#include    <stdexcept>


// C
//
#include    <grp.h>
#include    <pwd.h>
#include    <unistd.h>



namespace snapdev
{


constexpr uid_t     NO_UID = static_cast<uid_t>(-1);
constexpr uid_t     NO_GID = static_cast<gid_t>(-1);


/** \brief Convert a user name in its Unix uid_t.
 *
 * This function searches for a user identifier in /etc/passwd and return
 * the corresponding uid_t value.
 *
 * \param[in] user_name  The name of the user to search.
 *
 * \return The uid_t of the named user or NO_UID.
 */
inline uid_t getuid(std::string const & user_name)
{
    if(!user_name.empty())
    {
        struct passwd const * pwd(getpwnam(user_name.c_str()));
        if(pwd != nullptr)
        {
            return pwd->pw_uid;
        }
    }

    return NO_UID;
}


/** \brief Get the name of a Unix user.
 *
 * This function retrieves the name of a Unix user given its uid. By
 * default, it uses the uid of the current user.
 *
 * \note
 * The current maximum size of the getpwuid_r() buffer is 1Kb. It may
 * grow with time. See sysconf(_SC_GETGR_R_SIZE_MAX).
 *
 * \param[in] uid  The user identifier which name is being retrieved.
 *
 * \return The name of the user.
 */
inline std::string get_user_name(uid_t uid = ::getuid())
{
    passwd p, *ptr = nullptr;
    char buf[1024];
    int const r(getpwuid_r(uid, &p, buf, sizeof(buf), &ptr));
    if(r == -1 || ptr == nullptr)
    {
        return std::string();
    }
    return p.pw_name;
}


/** \brief Convert a group name in a gid_t.
 *
 * This function searches for the named group in /etc/group and return the
 * corresponding gid_t value.
 *
 * \param[in] group_name  The name of a group to convert to a gid_t.
 *
 * \return The gid_t of the named group or NO_GID.
 */
inline gid_t getgid(std::string const & group_name)
{
    if(!group_name.empty())
    {
        struct group const * grp(getgrnam(group_name.c_str()));
        if(grp != nullptr)
        {
            return grp->gr_gid;
        }
    }

    return NO_GID;
}


/** \brief Get the name of a Unix group.
 *
 * This function retrieves the name of a Unix group given its gid. By
 * default, it uses the main gid of the current user.
 *
 * \param[in] gid  The group identifier which name is being retrieved.
 *
 * \return The name of the group or an empty string if not available.
 */
inline std::string get_group_name(gid_t gid = ::getgid())
{
    group g, *ptr = nullptr;
    char buf[1024];
    int const r(getgrgid_r(gid, &g, buf, sizeof(buf), &ptr));
    if(r == -1 || ptr == nullptr)
    {
        return std::string();
    }
    return g.gr_name;
}


inline int chownnm(
      std::string const & path
    , uid_t uid
    , gid_t gid)
{
    if(path.empty())
    {
        throw std::invalid_argument("the path cannot be empty in snap::chownnm()");
    }

    if(uid != NO_UID
    || gid != NO_GID)
    {
        return chown(path.c_str(), uid, gid);
    }

    // in case both are undefined (it happens in the mkdir_p() function)
    //
    return 0;
}


/** \brief Set the owner and group of a file or directory.
 *
 * This function determines the user identifier and group identifier
 * from the specified names and use them to call chown().
 *
 * The function can fail if an identifier for the user or group names
 * cannot be determined.
 *
 * The function may fail if the path is invalid or permissions do not
 * allow for ownership modifications.
 *
 * \param[in] path  The path to the file or directory to change.
 * \param[in] user_name  The name of the user.
 * \param[in] group_name  The name of the group.
 *
 * \return 0 if chown succeeded,
 *         -1 if an error occurs (i.e. permissions denied, unknown user/group)
 *         and errno is set accordingly.
 */
inline int chownnm(
      std::string const & path
    , std::string const & user_name
    , std::string const & group_name)
{
    return chownnm(path, getuid(user_name), getgid(group_name));
}


inline int chownnm(
      std::string const & path
    , uid_t uid
    , std::string const & group_name)
{
    return chownnm(path, uid, getgid(group_name));
}


inline int chownnm(
      std::string const & path
    , std::string const & user_name
    , gid_t gid)
{
    return chownnm(path, getuid(user_name), gid);
}



} // namespace snapdev
// vim: ts=4 sw=4 et
