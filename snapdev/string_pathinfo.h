// Copyright (c) 2011-2021  Made to Order Software Corp.  All Rights Reserved
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

#include "snapdev/reverse_cstring.h"

#include <string>

namespace snap
{


/** \brief Retrieve the basename of a path.
 *
 * This function retrieves the basename of a path. You can also remove the
 * suffix (often called a file extension) and a prefix.
 *
 * \code
 *     // the following returns true
 *     snap::string_pathinfo_basename(
 *                "/usr/share/snapwebsites/in.basename.txt"
 *              , ".txt"
 *              , "in.") == "basename"
 * \endcode
 *
 * \tparam StringT  The type of string to parse.
 * \param[in] path  The path from which basename gets retrieved.
 * \param[in] suffix  If the path ends with that suffix, remove it.
 * \param[in] prefix  If the path starts with that prefix, remove it.
 *
 * \return The basename of \p path.
 */
template<class StringT>
StringT string_pathinfo_basename(StringT const & path
                               , typename std::decay<StringT>::type const & suffix = ""
                               , typename std::decay<StringT>::type const & prefix = "")
{
    // ignore path if present
    //
    typename StringT::size_type pos(path.find_last_of('/'));
    if(pos == StringT::npos)
    {
        // if no '/' in string, the entire name is a basename
        //
        pos = 0;
    }
    else
    {
        ++pos;      // skip the actual '/'
    }

    // ignore prefix if present
    //
    if(prefix.length() <= path.length() - pos
    && path.compare(pos, prefix.length(), prefix) == 0)
    {
        pos += prefix.length();
    }

    // if the path ends with suffix, then return the path without it
    //
    if(suffix.length() <= path.length() - pos
    && path.compare(path.length() - suffix.length(), suffix.length(), suffix) == 0)
    {
        return path.substr(pos, path.length() - pos - suffix.length());
    }

    // no suffix in this case
    //
    return path.substr(pos);
}


/** \brief Retrieve the directory name of a path.
 *
 * This function retrieves the directory name of a path. The returned path
 * is the empty string if the input does not include any '/'.
 *
 * \code
 *     // the following returns true
 *     snap::string_pathinfo_dirname(
 *         "/usr/share/snapwebsites/in.filename.txt");
 *             == "/usr/share/snapwebsites";
 * \endcode
 *
 * \param[in] path  The path from which basename gets retrieved.
 *
 * \return The directory name of \p path.
 */
template < class StringT >
StringT string_pathinfo_dirname(StringT const & path)
{
    typename StringT::size_type pos(path.find_last_of('/'));
    if(pos == StringT::npos)
    {
        return StringT();
    }
    else if(pos == 0)
    {
        if(path[0] == '/')
        {
            return StringT("/");
        }
        return StringT(".");
    }
    else
    {
        return path.substr(0, pos);
    }
}

} // namespace snap
// vim: ts=4 sw=4 et
