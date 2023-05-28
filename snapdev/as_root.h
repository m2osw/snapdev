// Copyright (c) 2016-2023  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Class & Function dealing with changing user ownership.
 *
 * The class is used to gain a different user and group ownership for a
 * short period of time. It is expected to be used on the stack for when
 * you need to execute some code as a different user. It is called as_root
 * because in most cases you want to become root.
 *
 * The function drop_root_privileges() is used to completely drop root
 * privileges. This is often done just after a fork() and just before
 * the following exec() so child processes do not gain root privileges.
 * It is also used by servers that spawn child processes and do not want
 * those children to have root access.
 */

// self
//
#include    <snapdev/not_reached.h>
#include    <snapdev/not_used.h>


// libexcept
//
#include    "libexcept/exception.h"


// C++
//
#include    <algorithm>
#include    <string>


// C
//
#include    <grp.h>
#include    <pwd.h>
#include    <unistd.h>



namespace snapdev
{



DECLARE_MAIN_EXCEPTION(as_root_exception);

DECLARE_EXCEPTION(as_root_exception, still_root);
DECLARE_EXCEPTION(as_root_exception, unknown_user);


/** \brief A value representing an invalid group number.
 *
 * By default, the as_root class does not change the group ID. This value
 * represents that fact.
 *
 * \todo
 * Determine whether -1 is really not a valid group number. We may need to
 * handle this differently.
 */
constexpr gid_t NO_GROUP = static_cast<gid_t>(-1);


/** \brief Safely become root (or another user) and back.
 *
 * This class defines a way to become a different user (as defined by the
 * 's' bit of the executable) and then come back as the original user once
 * your work requiring that user is done.
 *
 * The class uses RAII so when an object of that type gets destroyed,
 * it automatically restore the user. You can also restore the user
 * sooner if safer in your code.
 *
 * This class is used only when a process is given a different user as
 * the owner of the executable file and when the set user identifier flag
 * is set on the executable (a.k.a. `chmod u+s /usr/bin/my-tool`).
 *
 * The class constructor attempts the change to the specified user (default
 * is root). If that fails, the is_switched() function returns false. It is
 * likely important that you verified whether the effective user ID was
 * changed or not to make sure that you can run the commands you expect to
 * run as that user.
 *
 * The class can be used recursively. So if function A uses as_root,
 * then calls function B which also uses as_root, the second time
 * the switch does nothing since you are already that user. Function A
 * will stil properly restore the user to the normal user instead of
 * the other user.
 *
 * The class is only expected to be used on the stack. If you use it
 * in an object, then the restore may not work as expected (i.e. it
 * could happen in the wrong order).
 *
 * \note
 * Make sure to check that the switch happened. A switch could fail because
 * the user cannot have yet another process under his belt or because the
 * kernel does not have enough memory to apply the change. It may not always
 * be because of unavailable permissions.
 *
 * \todo
 * I think that the f_switched boolean flag may need to be changed to a
 * 3 value type because the switch_on() may be able to change the group
 * and not the user and same with switch_off(). So we could be in a partial
 * state and right now that is not properly represented.
 */
class as_root
{
public:
    /** \brief Shared pointer.
     *
     * If you want to create the object within an if() block, then you
     * can use this pointer like so:
     *
     * \code
     *     ...
     *     snapdev::as_root::pointer_t other_user;
     *     if(<expr>)
     *     {
     *         other_user = std::make_shared<snapdev::as_root>(uid);
     *     }
     *     ...
     *     // once end of function is reached, user is restored
     * \endcode
     */
    typedef std::shared_ptr<as_root>    pointer_t;

    /** \brief Save the current user then switch to another user.
     *
     * This function switches us to the \p uid user. If that fails,
     * then the is_switched() function returns false. You can get
     * the exact error using the error_number() function. You can
     * also check the user identifier with getuid() and see whether
     * it matches what you otherwise expect.
     *
     * The destructor automatically reverts back to the original user.
     *
     * \param[in] uid  The user identifier you want to become.
     * \param[in] gid  The group identifier you want to become.
     */
    as_root(uid_t uid = 0, gid_t gid = NO_GROUP)
        : f_new_uid(uid)
        , f_new_gid(gid)
        , f_user_uid(getuid())
        , f_group_gid(getgid())
    {
        switch_on();
    }

    /** \brief Save the current user then switch to another user.
     *
     * This function switches us to the named user. If that fails,
     * then the is_switched() function returns false. You can get
     * the exact error using the error_number() function.
     *
     * If you become a different user other than root, you may want to
     * define the \p groupname parameter in order to have full permissions
     * to do a snappdev::chownnm().
     *
     * The destructor automatically reverts back to the original user.
     *
     * \exception unknown_user
     * If the named user or group do not exist on this computer, then this
     * exception is raised. It is also used in case the \p username
     * parameter is the empty string.
     *
     * \param[in] username  The name of the user you want to become.
     * \param[in] groupname  The name of the group you want to become or an
     * empty string to not change the group.
     */
    as_root(
              std::string const & username
            , std::string const & groupname = std::string())
        : f_user_uid(getuid())
    {
        if(username.empty())
        {
            throw unknown_user("user name to switch to cannot be an empty string.");
        }
        passwd * user(getpwnam(username.c_str()));
        if(user == nullptr)
        {
            unknown_user e(
                  "user \""
                + username
                + "\" to switch to was not found on this computer.");
            e.set_parameter("username", username);
            throw e;
        }
        if(!groupname.empty())
        {
            group * grp(getgrnam(groupname.c_str()));
            if(grp == nullptr)
            {
                unknown_user e(
                      "group \""
                    + groupname
                    + "\" to switch to was not found on this computer.");
                e.set_parameter("groupname", groupname);
                throw e;
            }
            f_new_gid = grp->gr_gid;
        }
        f_new_uid = user->pw_uid;

        switch_on();
    }

    /** \brief Restore the user as it was just before construction.
     *
     * This function restores the user as found in the construtor. If
     * the constructor could not switch to the other user, then this
     * function does nothing.
     */
    ~as_root()
    {
        NOT_USED(switch_off());
    }

    /** \brief Set the effective user ID back to what it was on construction.
     *
     * This function reverse the constructor action and returns the effective
     * user ID back to the user it was set to on construction. The destructor
     * calls this function automatically, but it is publicly available in
     * case you wanted to change the effective user ID back sooner.
     *
     * If the switch fails, then the error_number() function returns the
     * errno code returned by the seteuid() function.
     *
     * \return true if the switch worked as expected.
     *
     * \sa switch_on()
     * \sa error_number()
     */
    bool switch_off()
    {
        if(f_switched)
        {
            f_errno = 0;
            if(f_group_gid != NO_GROUP
            && setegid(f_group_gid) != 0)
            {
                f_errno = errno;
            }
            if(seteuid(f_user_uid) != 0)
            {
                f_errno = errno;
            }
            if(f_errno != 0)
            {
                return false;
            }
            f_switched = false;
        }

        return true;
    }

    /** \brief Switch the new user back on.
     *
     * This function sets the effective user ID back to the one specified
     * on the constructor. If that is already the current user ID (as per
     * the f_switched internal flag), then nothing happens.
     *
     * If you have part of the code that you want to execute as the new
     * user, then part as the old user, and then back again with the
     * new user, you can do so using the switch_on() and switch_off()
     * functions.
     *
     * If an error occurs, the function returns false and you can get
     * the errno set by the seteuid() function by calling the error_number()
     * function.
     *
     * \note
     * You cannot change the new effective user ID to this function. This
     * is because it would be much more difficult to manage that way. If
     * you need to switch to yet another user, then consider using another
     * as_root object.
     *
     * \return true if the switch succeeded.
     *
     * \sa switch_off()
     * \sa error_number()
     */
    bool switch_on()
    {
        if(!f_switched)
        {
            if(seteuid(f_new_uid) != 0)
            {
                f_errno = errno;
                return false;
            }
            if(f_new_gid != NO_GROUP)
            {
                if(setegid(f_new_gid) != 0)
                {
                    // attempt to restore the previous user on error
                    //
                    NOT_USED(seteuid(f_user_uid));

                    f_errno = errno;
                    return false;
                }
            }
            f_switched = true;
            f_errno = 0;
        }
        return true;
    }

    /** \brief Check whether the as_root object is considered switched.
     *
     * The constructor of the function attempts to switch to a different
     * user. If the function fails, then the errno value is saved and
     * the switched flag is set to true. This function returns the value
     * of the switch flag. If true, then you are switched. If false, you
     * are not switched.
     *
     * If you are not switched, you may check the error number with
     * the error_number() function to know whether it's an invalid
     * state or just that you are not the new user at the moment.
     *
     * \return true if the constructor worked and thus we are root.
     *
     * \sa error_number()
     */
    bool is_switched() const
    {
        return f_switched;
    }

    /** \brief Get the error number.
     *
     * The switch_on() and switch_off() functions saves the error number
     * set by the seteuid() function when it fails. That error number can
     * be retrieved using this function.
     *
     * If you call the switch_on() and switch_off() functions and they
     * succeed, then this function returns 0.
     *
     * \return The errno as set by the seteuid() on an error, otherwise 0.
     *
     * \sa is_switched()
     * \sa switch_on()
     * \sa switch_off()
     */
    int error_number() const
    {
        return f_errno;
    }

private:
    /** \brief The UID to become.
     *
     * This value represents the UID to become. It can be set on the
     * constructor only. It cannot be changed later. If you need yet
     * another user, you will have to create a different as_root object
     * while you are that other user.
     */
    uid_t       f_new_uid = 0;

    /** \brief The GID to become.
     *
     * This value represents the GID to become or NO_GROUP. It can be set on
     * the constructor only. It cannot be changed later. If you need yet
     * another group, you will have to create a different as_root object
     * while you are that other user.
     */
    gid_t       f_new_gid = NO_GROUP;

    /** \brief The user UID on entry.
     *
     * The constructor saves the current user identifier so we can restore
     * it on destruction.
     */
    uid_t       f_user_uid = 0;

    /** \brief The group GID on entry.
     *
     * The constructor saves the current group identifier so we can restore
     * it on destruction.
     */
    gid_t       f_group_gid = NO_GROUP;

    /** \brief The error number.
     *
     * If the seteuid() found in the constructor fails, then this value is
     * set to the errno parameter as set by the seteuid() function.
     *
     * By default, the value is 0 which means that the seteuid() function
     * succeeded in changing the user identifier to the root user identifier.
     */
    int         f_errno = 0;

    /** \brief Whether the switch is still in effect.
     *
     * This flag is set to true when the constructor succeed in switching
     * the user to the specified UID.
     *
     * You can later switch back and forth and thus the flag may be turned
     * back on or off.
     */
    bool        f_switched = false;
};


/** \brief Modes available to the drop_root_privileges() function.
 *
 * These modes define how to act in case the caller cannot be switched to
 * a user other than root.
 */
enum class drop_privilege_mode_t
{
    /** \brief The default mode is to fail.
     *
     * This mode is the default. In this case, a drop failure means that the
     * function throws a snapdev::still_root exception.
     */
    DROP_PRIVILEGE_MODE_FAIL,

    /** \brief Try swithing to another user.
     *
     * This mode allows the process to attempt to switch to the specified
     * user. By default, that user is "nobody". You can change that to a
     * different user by changing the nobody parameter of the
     * drop_root_privileges() function.
     */
    DROP_PRIVILEGE_MODE_TRY_NOBODY,

    /** \brief Allow the root user.
     *
     * If this mode is used and the drop of privileges fails to go to a
     * user other than root, then the function simply returns. That means
     * you will be running your code as root. If that is not acceptable,
     * then you do not want to use this option.
     */
    DROP_PRIVILEGE_MODE_ALLOW_ROOT,
};



/** \brief This functionmakes sure your process cannot become root.
 *
 * To make sure your application is not given root privileges, you can call
 * this function when your main() function starts. This is useful for
 * services which are not expected to run as root.
 *
 * By default, the function attempts to switch to the user that started
 * the process (`getuid(2)`). If that is still root, then it uses the
 * \p mode parameter to know how to react next.
 *
 * \li drop_privilege_mode_t::DROP_PRIVILEGE_MODE_ALLOW_ROOT
 *
 * If it is acceptable to run your software as root anyway, then use this
 * mode instead of the default.
 *
 * \li drop_privilege_mode_t::DROP_PRIVILEGE_MODE_TRY_NOBODY
 *
 * Try dropping privileges to the specified \p user_name user. By default,
 * \p user_name is set to "nobody", hence the name of the mode.
 *
 * \li drop_privilege_mode_t::DROP_PRIVILEGE_MODE_FAIL
 *
 * When \p mode is set to this value, failure to drop privileges result
 * in the `still_root` exception.
 *
 * \exception still_root
 * It is important to note that, by default, if the user is still root after
 * the attempt to drop privileges, the function raises this exception.
 * You can let your users continue to run as root by changing the mode
 * parameter.
 *
 * \param[in] mode  Defines what to do in case the privileges are not dropped.
 */
inline void drop_root_privileges(
      drop_privilege_mode_t const mode = drop_privilege_mode_t::DROP_PRIVILEGE_MODE_FAIL
    , std::string const & user_name = "nobody")
{
    // try dropping privileges
    //
    int r(setuid(getuid()));
    if(r == 0)
    {
        // still root?
        //
        if(getuid() != 0)
        {
            return;
        }
    }

    switch(mode)
    {
    case drop_privilege_mode_t::DROP_PRIVILEGE_MODE_ALLOW_ROOT:
        // user allows root anyway
        //
        return;

    case drop_privilege_mode_t::DROP_PRIVILEGE_MODE_TRY_NOBODY:
        {
            passwd * user(getpwnam(user_name.c_str()));
            if(user == nullptr)
            {
                unknown_user e(
                      "user \""
                    + user_name
                    + "\" to drop privileges to was not found on this computer.");
                e.set_parameter("username", user_name);
                throw e;
            }

            if(setuid(user->pw_uid) == 0)
            {
                if(getuid() == user->pw_uid)
                {
                    // we are a nobody process now, we're safe
                    //
                    return;
                }
            }
        }
        [[fallthrough]];
    case drop_privilege_mode_t::DROP_PRIVILEGE_MODE_FAIL:
        throw still_root("this process could not drop root privileges.");

    }
    snapdev::NOT_REACHED();
}



} // namespace snapdev
// vim: ts=4 sw=4 et
