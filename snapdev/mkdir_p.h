// Copyright (c) 2013-2022  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Function to create directories from a full path.
 *
 * This function is similar to the `mkdir -p <path>` command line. Further,
 * our implementation supports the filename within the path. You can also
 * define the name of the user and group you want the child most directory
 * to be in the end.
 */

// self
//
#include    <snapdev/chownnm.h>
#include    <snapdev/tokenize_string.h>


// C++
//
#include    <list>


// C
//
#include    <sys/stat.h>
#include    <sys/types.h>





namespace snapdev
{


/** \brief Create directory as with `mkdir -p ...`.
 *
 * This function creates all the directories so one can create a file
 * under the deepest directory specified in \p path.
 *
 * If \p path includes the filename, then make sure to set
 * \p include_filename parameter to true. That way this function
 * ignores that name.
 *
 * You may also pass the mode and owner/group details for the new
 * or existing directories. The function ignores these parameters if
 * set to their default (0 for mode and an empty string for owner
 * and group.)
 *
 * The default mode of 0755 is used when creating a directory if you
 * used 0 as the mode. In most cases, acceptable modes are:
 *
 * \li 0700
 * \li 0770
 * \li 0775
 * \li 0750
 * \li 0755
 *
 * Other modes are likely to not be useful for a directory.
 *
 * The owner and group parameters can be set to specific user and group
 * names. These names must existing in /etc/passwd and /etc/group. When
 * both are empty strings, chown() is not called.
 *
 * The function accepts paths with double slashes as if there was
 * just one (i.e. "/etc//snapwebsites" is viewed as "/etc/snapwebsites".)
 * This is the standard Unix behavior.
 *
 * The function returns -1 if one or more of the directories cannot
 * be created or adjusted according to the parameters. It also logs
 * a message to your log file specifying which directory could not
 * be created.
 *
 * If the function returns -1, then errno is set to the error returned
 * by the last mkdir(), chmod(), or chown() that failed.
 *
 * \note
 * The two main errors when this function fails are: (1) the directory
 * cannot be created because you do not have enough permissions; and
 * (2) the named directory exists in the form of a file which is not
 * a directory.
 *
 * \bug
 * Many of the default directories that we need to have to run our
 * servers are to be created in directories that are owned by root.
 * This causes problems when attempting to run Snap! executables
 * as a developer.
 *
 * \param[in] path  The path to create.
 * \param[in] include_filename  If true, the \p path parameter also
 *            includes a filename.
 * \param[in] mode  The new directories mode if not zero.
 * \param[in] owner  The new directories owner.
 * \param[in] group  The new directories group.
 *
 * \return 0 if the directory exists at the time the function returns,
 *         -1 if an error occurs (i.e. permissions denied)
 */
inline int mkdir_p(
      std::string const & path
    , bool include_filename = false
    , int mode = 0
    , std::string const & owner = std::string()
    , std::string const & group = std::string())
{
    // we skip empty parts since "//" is the same as "/" in a Unix path.
    //
    std::list<std::string> segments;
    tokenize_string(segments, path, "/", true);
    if(segments.empty())
    {
        return 0;
    }

    if(include_filename)
    {
        segments.pop_back();
    }

    std::string p;
    bool add_slash(path[0] == '/');
    std::size_t const max_segments(segments.size());
    for(std::size_t idx(0); idx < max_segments; ++idx)
    {
        // compute path
        //
        if(add_slash)
        {
            p += "/";
        }
        else
        {
            add_slash = true;
        }

        p += segments.front();
        segments.pop_front();

        // already exists?
        //
        struct stat s;
        if(stat(p.c_str(), &s) == 0)
        {
            // the file exists, it is a directory?
            //
            if(S_ISDIR(s.st_mode))
            {
                // make sure the last segment (directory we are really
                // expected to create) has the correct mode and ownership
                //
                if(idx + 1 == max_segments)
                {
                    if(mode != 0)
                    {
                        if(chmod(p.c_str(), mode) != 0)
                        {
                            return -1;
                        }
                    }
                    if(chownnm(p, owner, group) != 0)
                    {
                        return -1;
                    }
                }
                continue;
            }

            // not a directory, that is an error
            //
            errno = EEXIST;
            return -1;
        }

        // attempt creating
        //
        if(mkdir(p.c_str(), mode == 0 ? 0755 : mode) != 0)
        {
            return -1;
        }

        // directories we create are also assigned owner/group
        //
        if(chownnm(p, owner, group) != 0)
        {
            return -1;
        }
    }

    return 0;
}



} // snap namespacedev
// vim: ts=4 sw=4 et
