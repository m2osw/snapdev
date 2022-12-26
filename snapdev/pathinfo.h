// Copyright (c) 2011-2022  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Utility functions to handle paths.
 *
 * Since we started before the C++ extension supporting various paths, we
 * have our own set of functions. Also the system path support does not match
 * our needs one to one.
 */

// self
//
#include    "snapdev/reverse_cstring.h"


// C++
//
#include    <string>



namespace snapdev
{
namespace pathinfo
{


/** \brief Retrieve the basename of a path.
 *
 * This function retrieves the basename of a path. You can also remove the
 * suffix (often called a file extension) and a prefix.
 *
 * \code
 *     // the following returns true
 *     snapdev::pathinfo::basename(
 *                "/usr/share/snapwebsites/in.basename.txt"
 *              , ".txt"
 *              , "in.") == "basename"
 * \endcode
 *
 * To remove the suffix, whatever it is, use the special pattern ".*".
 *
 * \tparam StringT  The type of string to parse.
 * \param[in] path  The path from which basename gets retrieved.
 * \param[in] suffix  If the path ends with that suffix, remove it.
 * \param[in] prefix  If the path starts with that prefix, remove it.
 *
 * \return The basename of \p path.
 */
template<class StringT>
StringT basename(StringT const & path
               , typename std::decay<StringT>::type const & suffix = ""
               , typename std::decay<StringT>::type const & prefix = "")
{
    // ignore path if present
    //
    typename StringT::size_type pos(path.rfind('/'));
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
    if(suffix.length() == 2
    && suffix[0] == '.'
    && suffix[1] == '*')
    {
        typename StringT::size_type end(path.rfind('.'));
        if(end != StringT::npos && end > pos)
        {
            // whatever the suffix is
            //
            return path.substr(pos, end - pos);
        }
    }
    else if(suffix.length() <= path.length() - pos
         && path.compare(path.length() - suffix.length(), suffix.length(), suffix) == 0)
    {
        // remove a specific suffix
        //
        return path.substr(pos, path.length() - pos - suffix.length());
    }

    // ignore possible suffix
    //
    return path.substr(pos);
}


/** \brief Replace the suffix with another.
 *
 * This function checks whether a file ends with a given suffix. If so then
 * the existing suffix gets removed. Then it happens the new suffix.
 *
 * The function is not checking whether a suffix starts with a period.
 * It can include any other character.
 *
 * \code
 *     // the following expressions return true
 *     snap::string_pathinfo_replace_suffix(
 *                "/usr/share/snapwebsites/replace.cpp"
 *              , ".cpp"
 *              , ".h") == "/usr/share/snapwebsites/replace.h"
 *
 *     snap::string_pathinfo_replace_suffix(
 *                "/usr/share/snapwebsites/replace"
 *              , ".cpp"
 *              , ".h") == "/usr/share/snapwebsites/replace.h"
 *
 *     snap::string_pathinfo_replace_suffix(
 *                "/usr/share/snapwebsites/replace.txt"
 *              , ".*"
 *              , ".h") == "/usr/share/snapwebsites/replace.h"
 * \endcode
 *
 * \note
 * By default, the \p new_suffix parameter is set to the empty string.
 * This means the function can be used to trim the string from
 * \p old_suffix.
 *
 * \todo
 * Add a function which supports an array of \p old_suffix.
 *
 * \tparam StringT  The type of string to parse.
 * \param[in] path  The path from which to replace a suffix.
 * \param[in] old_suffix  If the path ends with that suffix, remove it.
 * \param[in] new_suffix  Append this suffix.
 * \param[in] no_change_on_missing  If old_suffix is missing, do not change
 * the \t path.
 *
 * \return \p path with its suffix replaced as defined above.
 */
template<class StringT>
StringT replace_suffix(
          StringT const & path
        , typename std::decay<StringT>::type const & old_suffix
        , typename std::decay<StringT>::type const & new_suffix = ""
        , bool no_change_on_missing = false)
{
    if(old_suffix == ".*")
    {
        std::size_t const slash(path.rfind('/') + 1);
        std::size_t const pos(path.rfind('.'));
        if(pos <= slash)
        {
            return path + new_suffix;
        }
        return path.substr(0, pos) + new_suffix;
    }

    // TODO: with C++20 we could use:   path.ends_with(old_suffix)
    //
    if(path.length() >= old_suffix.length()
    && path.c_str() + path.length() - old_suffix.length() == old_suffix)
    {
        return path.substr(0, path.length() - old_suffix.length()) + new_suffix;
    }

    if(no_change_on_missing)
    {
        return path;
    }

    return path + new_suffix;
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
template<class StringT>
StringT dirname(StringT const & path)
{
    typename StringT::size_type pos(path.rfind('/'));
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


/** \brief Check whether filename represents "." or ".."
 *
 * The two special files named "." and ".." are in general not useful in
 * our applications. You can check whether a filename represents one
 * of those files and if so ignore the file altogether.
 *
 * Note that those two filenames do represent valid files. In most cases,
 * you want to ignore them when reading a directory recursively including
 * filenames starting with a period (hidden files).
 *
 * \param[in] filename  The filename to be checked.
 *
 * \return true if filename ends with "." or "..".
 */
inline bool is_dot_or_dot_dot(char const * filename)
{
    char const * end(filename);

    // go to the end of filename
    //
    while(*end != '\0')
    {
        ++end;
    }

    // just "."
    //
    if(end - filename == 1 && filename[0] == '.')
    {
        return true;
    }

    // just ".."
    //
    if(end - filename == 2 && filename[0] == '.' && filename[1] == '.')
    {
        return true;
    }

    // ends with "/."
    //
    if(end - filename >= 2 && end[-2] == '/' && end[-1] == '.')
    {
        return true;
    }

    // ends with "/.."
    //
    if(end - filename >= 3 && end[-3] == '/' && end[-2] == '.' && end[-1] == '.')
    {
        return true;
    }

    return false;
}


inline bool is_dot_or_dot_dot(std::string const & filename)
{
    return is_dot_or_dot_dot(filename.c_str());
}


} // namespace pathinfo
} // namespace snapdev
// vim: ts=4 sw=4 et
