// Copyright (c) 2011-2025  Made to Order Software Corp.  All Rights Reserved
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
#include    "snapdev/join_strings.h"
#include    "snapdev/not_reached.h"
#include    "snapdev/raii_generic_deleter.h"
#include    "snapdev/reverse_cstring.h"
#include    "snapdev/tokenize_string.h"


// C++
//
#include    <sstream>
#include    <string>
#include    <vector>


// C
//
#include    <limits.h>
#include    <stdlib.h>
#include    <string.h>
#include    <unistd.h>



namespace snapdev
{
namespace pathinfo
{



/** \brief Check whether the path is absolute or not.
 *
 * This function returns true if the path starts with a '/'.
 *
 * \warning
 * It is considered valid to call this function with an empty string.
 * In that case, the function returns false. However, it is not valid
 * to call the function with a nullptr and you are likely to crash
 * in that case.
 *
 * \param[in] path  The path to check.
 *
 * \return true if \p path start with a '/'.
 */
inline bool is_absolute(char const * path)
{
    return *path == '/';
}


inline bool is_absolute(std::string const & path)
{
    return is_absolute(path.c_str());
}


/** \brief Check whether \p path is relative or not.
 *
 * This function is the inverse of the is_absolute() function. It returns
 * true if \p path is relative and false otherwise.
 *
 * \param[in] path  The path to be checked.
 *
 * \return true if the path is considered relative.
 */
template<class T>
bool is_relative(T const & path)
{
    return !is_absolute(path);
}


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
 *     snap::pathinfo::replace_suffix(
 *                "/usr/share/snapwebsites/replace.cpp"
 *              , ".cpp"
 *              , ".h") == "/usr/share/snapwebsites/replace.h"
 *
 *     snap::pathinfo::replace_suffix(
 *                "/usr/share/snapwebsites/replace"
 *              , ".cpp"
 *              , ".h") == "/usr/share/snapwebsites/replace.h"
 *
 *     snap::pathinfo::replace_suffix(
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
 * the \p path.
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
 *     snap::pathinfo::dirname(
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
        if(is_absolute(path))
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


/** \brief Get the current working directory.
 *
 * This function retrieves the current working directory.
 *
 * If an error occurs, a message is saved in error_msg and the function
 * returns an empty string.
 *
 * \exception bad_alloc
 * If the command cannot allocation a buffer to save the current working
 * directory the function throws this exception.
 *
 * \param[out] error_msg  A variable where we save the error message.
 *
 * \return The current working directory or an empty string on error.
 */
inline std::string getcwd(std::string & error_msg)
{
    char * cwd(get_current_dir_name());
    if(cwd != nullptr)
    {
        raii_buffer_t auto_free(cwd);
        return std::string(cwd);
    }

    if(errno == ENOMEM)
    {
        throw std::bad_alloc();
    }

    int const e(errno);

    std::stringstream ss;
    ss << "getcwd() ";

    switch(e)
    {
    case EACCES:
        ss << "is missing permission to read or search a component of the current working directory.";
        break;

    case ENOENT:
        ss << "found an unlinked current working directory.";
        break;

    default:
        ss << "failed: "
           << strerror(e);
        break;

    }
    error_msg = ss.str();

    // trying to get errno returned as expected, assuming std::string does
    // not modify it we should be good
    //
    std::string result;
    errno = e;
    return result;
}


/** \brief Convert the input \p path in a canonicalized path.
 *
 * This function goes through the specified \p path and canonicalize
 * it. This means:
 *
 * * removing any "/./"
 * * removing any "/../"
 * * replacing softlinks with the target path
 *
 * The resulting path is likely going to be an absolute path.
 *
 * \note
 * If the input path is an empty string (equivalent to ".") then the
 * result may also be the empty string even though no errors happened.
 * You can distinguish both cases by checking `error_msg.empty()` first.
 * If true, then the function did not generate an error.
 *
 * \todo
 * Create a version which does not generate a full path. Instead, we could
 * do the work on realpath(3) and look at only returning a path which is
 * equivalent to what we had on input (i.e. keeping it relative if it
 * were relative on input). Similarly, we could then allow for softlink
 * to be taken or not and completely replace the input or not.
 *
 * \exception bad_alloc
 * If the command cannot allocate a buffer to save the current working
 * directory the function throws this exception.
 *
 * \param[in] path  The path to canonicalize.
 * \param[out] error_msg  A variable where the function saves an error message.
 *
 * \return The canonicalized version of \p path or an error and errno set
 * to whatever error realpath(3) returned.
 *
 * \sa file_exists()
 */
inline std::string realpath(std::string const & path, std::string & error_msg)
{
    char buf[PATH_MAX + 1];
    buf[PATH_MAX] = '\0';
    if(::realpath(path.c_str(), buf) == buf)
    {
        error_msg.clear();
        return buf;
    }

    if(errno == ENOMEM)
    {
        throw std::bad_alloc();
    }

    // it failed
    //
    int const e(errno);

    std::stringstream ss;
    ss << "realpath(\""
       << path
       << "\") ";
    switch(e)
    {
    case EACCES:
        ss << "is missing permission to read or search a component of the path.";
        break;

    case EIO:
        ss << "had I/O issues while searching.";
        break;

    case ELOOP:
        ss << "found too many symbolic links.";
        break;

    case ENAMETOOLONG:
        ss << "output buffer too small for path.";
        break;

    case ENOENT:
        ss << "could not find the specified file.";
        break;

    case ENOTDIR:
        ss << "found a file instead of a directory within the path.";
        break;

    default:
        ss << "failed: "
           << strerror(e);
        break;

    }
    error_msg = ss.str();

    // trying to get errno returned as expected, assuming std::string does
    // not modify it we should be good
    //
    std::string result;
    errno = e;
    return result;
}


/** \brief Check whether a file exists.
 *
 * This function checks whether a fileexists. You can tweak the flags
 * to test whether the file is executable and/or writable. See the \p mode
 * parameter as defined by the access() function:
 *
 * \li F_OK -- the file exists
 * \li R_OK -- the file can be opened for reading
 * \li W_OK -- the file can be opened for writing
 * \li X_OK -- the file can be executed
 *
 * The default this function uses if `F_OK`. In many cases, you may want
 * to change the default to `R_OK` or `X_OK` if you want to read that
 * file or execute that script.
 *
 * \param[in] filename  The name of the file to check.
 * \param[in] mode  The mode to check the file permissions with.
 *
 * \return true if the mode matches the file permissions.
 */
inline bool file_exists(std::string const & filename, int mode = F_OK)
{
    return access(filename.c_str(), mode) == 0;
}


/** \brief Change path to a relative path.
 *
 * This function make a path relative to another.
 *
 * Say you have:
 *
 * \code
 * base = "/a/b/c/d/e/f";
 * path = "/a/b/c/g/h/i";
 * \endcode
 *
 * the function removes the common path segments (`a/b/c`) and then adds
 * a number of parent (`..`) equal to the number of segments left in
 * the base, three in our example. The result is then:
 *
 * \code
 *     "../../../g/h/i"
 * \endcode
 *
 * \param[in] base  Make \p path relative to this base.
 * \param[in] path  The path to transform to relative.
 *
 * \return The relative path.
 */
inline std::string relative_path(std::string const & base, std::string const & path)
{
    if(is_relative(base) || is_relative(path))
    {
        return std::string();
    }

    std::vector<std::string> base_segments;
    tokenize_string(base_segments, base, "/", true);

    std::vector<std::string> path_segments;
    tokenize_string(path_segments, path, "/", true);

    std::size_t const max(std::min(base_segments.size(), path_segments.size()));
    std::size_t idx(0);
    for(; idx < max; ++idx)
    {
        if(base_segments[idx] != path_segments[idx])
        {
            break;
        }
    }

    if(idx > 0)
    {
        base_segments.erase(base_segments.begin(), base_segments.begin() + idx);
        path_segments.erase(path_segments.begin(), path_segments.begin() + idx);
    }

    for(idx = 0; idx < base_segments.size(); ++idx)
    {
        path_segments.insert(path_segments.begin(), "..");
    }

    return join_strings(path_segments, "/");
}


/** \brief Canonicalize a path and filename.
 *
 * This function concatenates \p path and \p filename with a "/" in between
 * and then it canonicalize the result.
 *
 * The canonicalization means that the resulting path will:
 *
 * \li not include more than one "/" between two names,
 * \li not include any "." unless the result would otherwise be the empty
 *     string then "." is returned instead,
 * \li not include a ".." preceded by a name other than ".."
 *
 * The \p filename parameter can be the empty string.
 *
 * \note
 * The removal of the ".." is not verifying whether the path is valid on
 * the current file system. If you want to do such a verification, use the
 * system realpath() function instead.
 *
 * \param[in] path  The introducer path.
 * \param[in] filename  A filename to happen to the path.
 *
 * \return The path and filename canonicalized.
 */
inline std::string canonicalize(
      std::string const & path
    , std::string const & filename)
{
    bool const is_root(
        path.empty()
            ? (filename.empty()
                ? false
                : is_absolute(filename))
            : is_absolute(path));

    // break up the path & filename as segments
    //
    std::vector<std::string> segments;
    snapdev::tokenize_string(segments, path, "/", true);
    snapdev::tokenize_string(segments, filename, "/", true);

    // remove ".", they are not useful
    //
    auto no_dot_end(std::remove(segments.begin(), segments.end(), "."));
    segments.erase(no_dot_end, segments.end());

    // remove "<path>" and ".." when "<path>" is not ".."
    //
    for(auto it(segments.begin()); it != segments.end(); )
    {
        if(*it == ".."
        && it == segments.begin()
        && is_root)
        {
            it = segments.erase(it);
        }
        else if(*it == ".." && it != segments.begin() && *(it - 1) != "..")
        {
            it = segments.erase(it - 1, it + 1);
        }
        else
        {
            ++it;
        }
    }

    // transform back to a path
    //
    std::string const new_path(join_strings(segments, "/"));
    if(is_root)
    {
        // the path is a root path
        //
        return "/" + new_path;
    }

    // this is a relative path
    //
    return new_path.empty() ? "." : new_path;
}


/** \brief Check a path to determine whether it includes a pattern or not.
 *
 * This function scans the specified \p path for a glob() like pattern.
 * We detect the `*`, `?`, and the start of `[` or `{`. Note that the
 * glob function generally views the `[` and `{` as regular characters
 * if these characters are not closed with a match `]` or `}` respectively.
 *
 * The flags below correspond to flags supported by the glob(3) and
 * fnmatch(3) functions.
 *
 * * `GLOB_NOESCAPE`, `FNM_NOESCAPE` -- \p escape
 * * `GLOB_BRACE` -- \p braces
 * * `FNM_EXTMATCH` -- \p extended
 *
 * \param[in] path  The path to check for a pattern.
 * \param[in] escape  Whether to allow the `\\` character to escape
 * metacharacters. The default is true since a path with a pattern is more
 * powerful if the `\\` is properly supported.
 * \param[in] braces  Whether the braces (`{`) are accepted. This is a
 * GNU extension so by default we do not accept braces.
 * \param[in] extended  Whether the extended patterns are supported. GNU
 * supports `#(pattern-list)` where `#` is one of `?*+@!`.
 *
 * \return true if \p path includes one of the glob() metacharacters.
 */
inline bool has_pattern(
      std::string const & path
    , bool escape = true
    , bool braces = false
    , bool extended = false)
{
    for(char const * s(path.c_str()); *s != '\0'; ++s)
    {
        switch(*s)
        {
        case '\\':
            if(escape)
            {
                // silently skip one character
                //
                ++s;
                if(*s == '\0')
                {
                    return false;
                }
            }
            break;

        case '*':
        case '?':
            return true;

        case '[': // a ']' must be present for '[' to represent a valid character class
            {
                // backslashes are viewed as themself within a class definition
                // so there is not particular anything if we find such here;
                //
                // also POSIX says that if a '/' is included, then the pattern
                // is invalid; we just ignore that at the moment
                //
                char const * c(s + 1);
                if(*c == '!' || *c == '^') // POSIX does not support '^', bash does, though
                {
                    ++c;
                }
                if(*c == ']')
                {
                    ++c;
                }
                for(; *c != '\0'; ++c)
                {
                    if(*c == ']')
                    {
                        return true;
                    }
                }
            }
            break;

        case '{': // a '}' must be present for '{' to represent a valid pattern
            if(braces)
            {
                for(char const * c(s + 1); *c != '\0'; ++c)
                {
                    if(*c == '\\' && escape)
                    {
                        ++c;
                        if(*c == '\0')
                        {
                            break;
                        }
                    }
                    else if(*c == '}')
                    {
                        return true;
                    }
                }
            }
            break;

        case '+': // an extended pattern must be between '(...)'
        case '@':
        case '!':
            if(extended)
            {
                char const * c(s + 1);
                if(*c == '(')
                {
                    for(++c; *c != '\0'; ++c)
                    {
                        if(*c == '\\' && escape)
                        {
                            ++c;
                            if(*c == '\0')
                            {
                                break;
                            }
                        }
                        else if(*c == ')')
                        {
                            return true;
                        }
                    }
                }
            }
            break;

        }
    }

    return false;
}


/** \brief Check whether a path is equal or a child of another path.
 *
 * This function compares \p child against \p parent to see whether
 * \p child is indeed a child of \p parent.
 *
 * For example, the following returns true:
 *
 * \code
 *     snapdev::is_child_path("/var", "/var/lib/");
 * \endcode
 *
 * All paths are considered to be a child of the root path `/`.
 *
 * We consider two types of paths: relative and full. Both paths must
 * be either relative or full, otherwise the function returns false.
 *
 * The function canonicalize the paths, so if multiple slashes separate
 * some of the names, these are viewed as one slash (i.e. `"/" == "//"`).
 * Also an empty string (`""`) is viewed as the current directory
 * which can also be expressed as `"."` and thus `"" == "."` is considered
 * true.
 *
 * \todo
 * Fully support the `"" == "."` case. We support that properly only at
 * the top at the moment (i.e. `"/var" == "/var/."` is currently viewed
 * as false).
 *
 * \param[in] parent  The parent path.
 * \param[in] child  The child to match against \p parent.
 * \param[in] equal  Return this boolean value if the \p parent equals
 * the \p child.
 *
 * \return true if \p child is a child of \p parent.
 */
inline bool is_child_path(
      std::string const & parent
    , std::string const & child
    , bool equal = true)
{
    // paths should not be empty, but if so, handle specially
    //
    if(parent.empty()
    || child.empty())
    {
        std::string const p(parent.empty() ? "." : parent);
        std::string const c(child.empty() ? "." : parent);
        return p == c ? equal : false;
    }

    // both paths must be full or relative
    //
    if(is_absolute(parent) ^ is_absolute(child))
    {
        return false;
    }

    char const * p(parent.c_str());
    char const * c(child.c_str());
    for(;;)
    {
        while(*p == '/')
        {
            ++p;
        }
        while(*c == '/')
        {
            ++c;
        }
        if(*p == '\0')
        {
            return *c == '\0' ? equal : true;
        }
        do
        {
            if(*p != *c)
            {
                if((*p == '/' || *p == '\0')
                && (*c == '/' || *c == '\0'))
                {
                    while(*c == '/')
                    {
                        ++c;
                    }
                    return *c == '\0' ? equal : true;
                }
                return false;
            }
            if(*p == '\0')
            {
                return equal;
            }
            ++p;
            ++c;
        }
        while(*p != '/' || *c != '/');
    }
    snapdev::NOT_REACHED();
}



} // namespace pathinfo
} // namespace snapdev
// vim: ts=4 sw=4 et
