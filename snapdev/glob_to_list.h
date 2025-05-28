// Copyright (c) 2019-2025  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Create a list of files using glob().
 *
 * This template allows you to insert filenames from the output of a glob()
 * call to an STL container.
 */

// snapdev
//
#include    "snapdev/pathinfo.h"
#include    "snapdev/raii_generic_deleter.h"


// C++
//
#include    <list>
#include    <memory>
#include    <set>


// C
//
#include    <glob.h>
#include    <limits.h>
#include    <stdlib.h>
#include    <sys/stat.h>



namespace snapdev
{


/** \brief An object that holds the information about the file being loaded.
 *
 * In order to only read certain types of files (such as directories),
 * we have to get the lstat()'s. This object represents one file found
 * in the directory with it's lstat()'s.
 *
 * Note that the regular functions use `lstat()` to read the file
 * statistics. The `target_...()` functions read the target statistics.
 * In other words, if the file is a symbolic link, the functions return
 * different results.
 */
class file
{
public:
    file(std::string const & filename)
        : f_filename(filename)
    {
    }

    std::string const & filename() const
    {
        return f_filename;
    }

    bool exists() const
    {
        load_stats();
        return f_stat_loaded;
    }

    bool is_symbolic_link() const
    {
        if(exists())
        {
            return S_ISLNK(f_stat.st_mode);
        }
        return false;
    }

    bool is_regular_file() const
    {
        if(exists())
        {
            return S_ISREG(f_stat.st_mode);
        }
        return false;
    }

    bool is_directory() const
    {
        if(exists())
        {
            return S_ISDIR(f_stat.st_mode);
        }
        return false;
    }

    bool is_character() const
    {
        if(exists())
        {
            return S_ISCHR(f_stat.st_mode);
        }
        return false;
    }

    bool is_block() const
    {
        if(exists())
        {
            return S_ISBLK(f_stat.st_mode);
        }
        return false;
    }

    bool is_fifo() const
    {
        if(exists())
        {
            return S_ISFIFO(f_stat.st_mode);
        }
        return false;
    }

    bool is_socket() const
    {
        if(exists())
        {
            return S_ISSOCK(f_stat.st_mode);
        }
        return false;
    }

    int is_suid() const
    {
        if(exists())
        {
            return (f_stat.st_mode & S_ISUID) != 0;
        }
        return 0;
    }

    int is_sgid() const
    {
        if(exists())
        {
            return (f_stat.st_mode & S_ISGID) != 0;
        }
        return 0;
    }

    int is_vtx() const
    {
        if(exists())
        {
            return (f_stat.st_mode & S_ISVTX) != 0;
        }
        return 0;
    }

    int permissions() const
    {
        if(exists())
        {
            return f_stat.st_mode & 0777;
        }
        return 0;
    }

    uid_t uid() const
    {
        if(exists())
        {
            return f_stat.st_uid;
        }
        return -1;
    }

    gid_t gid() const
    {
        if(exists())
        {
            return f_stat.st_gid;
        }
        return -1;
    }

    gid_t size() const
    {
        if(exists())
        {
            return f_stat.st_size;
        }
        return -1;
    }

    bool target_exists() const
    {
        load_target_stats();
        return f_target_stat_loaded;
    }

    bool is_target_symbolic_link() const
    {
        if(target_exists())
        {
            return S_ISLNK(f_target_stat.st_mode);
        }
        return false;
    }

    bool is_target_regular_file() const
    {
        if(target_exists())
        {
            return S_ISREG(f_target_stat.st_mode);
        }
        return false;
    }

    bool is_target_directory() const
    {
        if(target_exists())
        {
            return S_ISDIR(f_target_stat.st_mode);
        }
        return false;
    }

    bool is_target_character() const
    {
        if(target_exists())
        {
            return S_ISCHR(f_target_stat.st_mode);
        }
        return false;
    }

    bool is_target_block() const
    {
        if(target_exists())
        {
            return S_ISBLK(f_target_stat.st_mode);
        }
        return false;
    }

    bool is_target_fifo() const
    {
        if(target_exists())
        {
            return S_ISFIFO(f_target_stat.st_mode);
        }
        return false;
    }

    bool is_target_socket() const
    {
        if(target_exists())
        {
            return S_ISSOCK(f_target_stat.st_mode);
        }
        return false;
    }

    int is_target_uid() const
    {
        if(target_exists())
        {
            return (f_target_stat.st_mode & S_ISUID) != 0;
        }
        return 0;
    }

    int is_target_gid() const
    {
        if(target_exists())
        {
            return (f_target_stat.st_mode & S_ISGID) != 0;
        }
        return 0;
    }

    int is_target_vtx() const
    {
        if(target_exists())
        {
            return (f_target_stat.st_mode & S_ISVTX) != 0;
        }
        return 0;
    }

    int target_permissions() const
    {
        if(target_exists())
        {
            return f_target_stat.st_mode & 0777;
        }
        return 0;
    }

    uid_t target_uid() const
    {
        if(target_exists())
        {
            return f_target_stat.st_uid;
        }
        return -1;
    }

    gid_t target_gid() const
    {
        if(target_exists())
        {
            return f_target_stat.st_gid;
        }
        return -1;
    }

    gid_t target_size() const
    {
        if(target_exists())
        {
            return f_target_stat.st_size;
        }
        return -1;
    }

private:
    void load_stats() const
    {
        if(f_stat_loaded)
        {
            return;
        }

        if(lstat(f_filename.c_str(), &f_stat) != 0)
        {
            return;
        }

        f_stat_loaded = true;
    }

    void load_target_stats() const
    {
        if(f_target_stat_loaded)
        {
            return;
        }

        if(stat(f_filename.c_str(), &f_target_stat) != 0)
        {
            return;
        }

        f_target_stat_loaded = true;
    }

    std::string         f_filename = std::string();
    mutable struct stat f_stat = {};
    mutable struct stat f_target_stat = {};
    mutable bool        f_stat_loaded = false;
    mutable bool        f_target_stat_loaded = false;
};


/** \brief A smart pointer to auto-delete glob results.
 *
 * This type defines a smart pointer which automatically frees all
 * the data allocated by glob() and this pointer too.
 *
 * Usage:
 *
 * \code
 *     snapdev::glob_to_list<std::vector<std::string>> glob;
 *     if(glob.read_path<
 *              snapdev::glob_to_list_flag_t::GLOB_FLAG_IGNORE_ERRORS,
 *              snapdev::glob_to_list_flag_t::GLOB_FLAG_PERIOD>(pattern))
 *     {
 *         if(!glob.empty())
 *         {
 *             // handle file names
 *
 *         }
 *         else
 *         {
 *             // no files case
 *         }
 *     }
 *     else
 *     {
 *         // handle error
 *     }
 * \endcode
 *
 * Note that the glob() function always gets called with the GLOB_NOSORT
 * flag set. If you want sorted results, use a container which returns
 * the data sorted such as the `std::set<>`.
 *
 * \warning
 * glob() is not thread safe and we currently do not add any additional
 * support to make it thread safe. Especially, the glob() function
 * makes use of a global function for errors and that uses global
 * pointers.
 * \warning
 * If you use our cppthread project, you can use a guard to lock a global
 * mutex. Remember that if you may get called before main() you must first
 * lock the `g_system_mutex`. Otherwise the initialization process may
 * not work correctly and your mutex may get initialized after you hit
 * your `cppthread::guard` statement (i.e. your g_mutex must be a pointer
 * that you allocate the first time you use it and to make that thread
 * safe you need to first lock the `g_system_mutex`).
 *
 * \code
 *     cppthread::mutex g_mutex;
 *     {
 *         cppthread::guard lock(g_mutex);
 *
 *         ...glob.read_path<...>(pattern);...
 *     }
 * \endcode
 */
typedef std::unique_ptr<glob_t, raii_pointer_deleter<glob_t, decltype(&::globfree), &::globfree>> glob_pointer_t;


enum class glob_to_list_flag_t
{
    GLOB_FLAG_NONE,               // not a flag, useful in case you need a value for ?:
    GLOB_FLAG_BRACE,              // allow {a,b,c}...
    GLOB_FLAG_IGNORE_ERRORS,      // read as much as possible
    GLOB_FLAG_MARK_DIRECTORY,     // add "/" to directory names
    GLOB_FLAG_NO_ESCAPE,          // ignore '\'
    GLOB_FLAG_ONLY_DIRECTORIES,   // only return directories
    GLOB_FLAG_PERIOD,             // allow period at the start (i.e. pattern ".*")
    GLOB_FLAG_DOT_AND_DOT_DOT,    // allow "." and ".." to be included
    GLOB_FLAG_TILDE,              // transform "~/..." with "$HOME/..."
    GLOB_FLAG_RECURSIVE,          // when a directory is found, read inside it too
    GLOB_FLAG_FOLLOW_SYMLINK,     // in recursive mode, do or do not follow symlinks
    GLOB_FLAG_EMPTY,              // on a GLOB_NOMATCH error, still return true
};


/** \brief Manage the results of glob() calls.
 *
 * This template is able to call glob() and insert the results to your
 * container object. If the type of the container is std::string, then
 * only the filenames are returned. If the type is set to a snapdev::file,
 * then the function returns a set of snapdev::file objects.
 *
 * The supported flags allow for selecting which files to ignore. By
 * default, files that start with a period (.) are ignored.
 *
 * Here is an example of usage:
 *
 * \code
 *     glob_to_list<std::vector<std::string>> dir;
 *     if(!dir.read_path<snapdev::glob_to_list_flag_t::GLOB_FLAG_ONLY_DIRECTORIES>("/proc/[0-9]*"))
 *     {
 *         ...handle error...
 *         return;
 *     }
 *     for(auto const & f : dir)
 *     {
 *         ...f is std::string with filename...
 *     }
 * \endcode
 *
 * The Go-like pattern "..." is understood by this class. What happens
 * when the filename is set to "..." is that the pattern is replaced with
 * "*" and the GLOB_FLAG_RECURSIVE flag is set. This means all the files
 * from the directory specified before the "..." patterns are returned.
 * This means you will be responsible for checking the filenames if you
 * need to have a more constraining patterns than "*". Otherwise, you
 * may want to consider using a usual glob pattern and set the recursive
 * flag _manually_. Note that the "./" introducer is not required. It
 * is assumed if not specified.
 *
 * \warning
 * The class is not multithread safe. The glob() function makes use of a
 * global variable to report errors so there is no way at this point to
 * make it safe (without a \em service like implementation).
 *
 * \todo
 * Consider using wordexp() instead of glob() to get additional substitution
 * support (environment variables, ~user, arithmetic, splitting, unquoting).
 *
 * \tparam C  The type of the container where to add the filenames.
 * \tparam T  The type used for the filenames (C<T>).
 */
template<typename C>
class glob_to_list
    : public C
{
private:
    typedef std::set<std::string>   directories_t;

public:
    typedef C                       container_t;

    /** \brief Read files at given path.
     *
     * This function runs the glob() function with the given path.
     *
     * The \p path variable is expected to include a path with glob() like
     * patterns (i.e. `*.txt`, `sound/0?.mp3`, etc.)
     *
     * Note that the glob()-ing is done on the entire path. However, only
     * the last part (after the last slash) is used in case you use the
     * GLOB_FLAG_RECURSIVE. Note that in recursive mode, the directories
     * will always be read since we have to recurse through them.
     *
     * \remarks
     * This implementation is not multithread safe. Make sure to use this
     * function in a part of your code which is locked.
     *
     * \todo
     * Add a read_path() which better supports dynamic flags.
     *
     * \tparam args  A list of one or more glob to list flags.
     * \param[in] path  The path with glob patterns.
     *
     * \return true if no error occurred.
     */
    template<glob_to_list_flag_t ...args>
    bool read_path(std::string const & path)
    {
        f_dot_and_dot_dot = false;
        f_recursive = false;
        f_follow_symlinks = false;
        int const flags = GLOB_NOSORT | flags_merge<args...>();

        // we want to get the pattern in case it is recursive, only the
        // pattern itself may imply recursivity (i.e. the "..." Go-like
        // pattern) so we extract it no matter what
        //
        std::string pattern;
        std::string directory;
        std::string::size_type const pos(path.rfind('/'));
        if(pos == std::string::npos)
        {
            directory = ".";
            pattern = path;
        }
        else
        {
            directory = path.substr(0, pos);
            pattern = path.substr(pos + 1);
        }

        if(pattern == "...")
        {
            pattern = "*";
            f_recursive = true;
        }

        if(!f_recursive)
        {
            return read_directory(path, flags);
        }

        std::string const real_dir(get_real_path(directory));
        if(real_dir.empty())
        {
            return false;
        }

        // in recursive mode we want to collect the list of directories
        // along whatever the user wants to collect and then process
        // those directories as well
        //
        directories_t visited;
        return recursive_read_path(
                      real_dir
                    , pattern
                    , flags
                    , visited);
    }

    /** \brief Convert the input \p path to a canonicalized path.
     *
     * This function goes through the specified \p path and canonicalize
     * it. This means:
     *
     * * removing any "/./"
     * * removing any "/../"
     * * replacing softlinks with the target path
     *
     * The resulting path is likely going to be a full path.
     *
     * \param[in] path  The path to canonicalize.
     *
     * \return The canonicalized version of \p path or an empty string on error.
     *
     * \sa snapdev::pathinfo::realpath()
     */
    std::string get_real_path(std::string const & path)
    {
        std::string const result(pathinfo::realpath(path, f_last_error_message));
        if(!f_last_error_message.empty())
        {
            f_last_error_errno = errno;
            f_last_error_path = path;
        }
        return result;
    }

    /** \brief The last error message.
     *
     * Whenever a call fails, it saves an error message here.
     *
     * The error message is whatever represents the error best from our
     * point of view.
     *
     * \note
     * Error messages get overwritten so you must call this function
     * before calling another function to not lose intermediate messages.
     *
     * \return The last error message.
     */
    std::string get_last_error_message() const
    {
        return f_last_error_message;
    }

    /** \brief The last error path.
     *
     * The path that generated the error. In most cases, this is the input
     * path you specified, with the pattern still intact. When using the
     * recursive feature, the path will be the path of the directory that
     * is currently being handled.
     *
     * \return The path that generated the error.
     */
    std::string get_last_error_path() const
    {
        return f_last_error_path;
    }

    /** \brief Retrieve the last error number.
     *
     * Whenever an error occurs with a system function, the errno value
     * gets saved in the "last error errno" variable which can then be
     * retrieved using this function. This should be used instead of
     * trying to understand the error message which is expected to only
     * be used for human consumption.
     *
     * \return The last error errno value.
     */
    int get_last_error_errno() const
    {
        return f_last_error_errno;
    }

private:
    template<class none = void>
    constexpr int flags_merge()
    {
        return 0;
    }

    template<glob_to_list_flag_t flag, glob_to_list_flag_t ...args>
    constexpr int flags_merge()
    {
        switch(flag)
        {
        case glob_to_list_flag_t::GLOB_FLAG_NONE:
            return flags_merge<args...>();

        case glob_to_list_flag_t::GLOB_FLAG_BRACE:
            return GLOB_BRACE | flags_merge<args...>();

        case glob_to_list_flag_t::GLOB_FLAG_IGNORE_ERRORS:
            return GLOB_ERR | flags_merge<args...>();

        case glob_to_list_flag_t::GLOB_FLAG_MARK_DIRECTORY:
            return GLOB_MARK | flags_merge<args...>();

        case glob_to_list_flag_t::GLOB_FLAG_NO_ESCAPE:
            return GLOB_NOESCAPE | flags_merge<args...>();

        case glob_to_list_flag_t::GLOB_FLAG_ONLY_DIRECTORIES:
            return GLOB_ONLYDIR | flags_merge<args...>();

        case glob_to_list_flag_t::GLOB_FLAG_PERIOD:
            return GLOB_PERIOD | flags_merge<args...>();

        case glob_to_list_flag_t::GLOB_FLAG_DOT_AND_DOT_DOT:
            f_dot_and_dot_dot = true;
            return flags_merge<args...>();

        case glob_to_list_flag_t::GLOB_FLAG_TILDE:
            return GLOB_TILDE_CHECK | flags_merge<args...>();

        case glob_to_list_flag_t::GLOB_FLAG_RECURSIVE:
            f_recursive = true;
            return flags_merge<args...>();

        case glob_to_list_flag_t::GLOB_FLAG_FOLLOW_SYMLINK:
            f_follow_symlinks = true;
            return flags_merge<args...>();

        case glob_to_list_flag_t::GLOB_FLAG_EMPTY:
            f_empty = true;
            return flags_merge<args...>();

        }

        throw std::logic_error("unimplemented GLOB_FLAG_... in flags_merge()");
    }

    static int glob_err_callback(char const * p, int e)
    {
        g_self->f_last_error_message = "caught an error while reading a directory.";
        g_self->f_last_error_path = p;
        g_self->f_last_error_errno = e;

        return 0;
    }

    bool read_directory(std::string const & path, int const flags)
    {
        glob_t dir = glob_t();
        g_self = this;
        int const r(glob(path.c_str(), flags, glob_err_callback, &dir));
        g_self = nullptr;       // to detect if glob_err_callback gets called improperly
        if(r == 0)
        {
            glob_pointer_t auto_release_dir(&dir);
            for(std::size_t idx(0); idx < dir.gl_pathc; ++idx)
            {
                if(!f_dot_and_dot_dot
                && pathinfo::is_dot_or_dot_dot(dir.gl_pathv[idx]))
                {
                    continue;
                }
                C::insert(C::end(), typename glob_to_list::value_type(std::string(dir.gl_pathv[idx])));
            }
        }
        else
        {
            // handle errors
            //
            switch(r)
            {
            case GLOB_NOSPACE:
                f_last_error_message =
                          "glob(\""
                        + path
                        + "\") did not have enough memory to allocate its buffers.";
                break;

            case GLOB_ABORTED:
                f_last_error_message =
                          "glob(\""
                        + path
                        + "\") was aborted after a read error.";
                break;

            case GLOB_NOMATCH:
                if(f_empty)
                {
                    return true;
                }
                f_last_error_message = "glob(\""
                        + path
                        + "\") could not find any files matching the pattern.";
                f_last_error_errno = ENOENT;
                break;

            default:
                f_last_error_message = "unknown glob(\""
                        + path
                        + "\") error code: "
                        + std::to_string(r)
                        + ".";
                break;

            }

            return false;
        }

        return true;
    }

    bool recursive_read_path(
          std::string const & path
        , std::string const & pattern
        , int flags
        , directories_t & visited)
    {
        // (1) read client's files in path
        //
        if(!read_directory(path + '/' + pattern, flags))
        {
            if(f_last_error_errno != ENOENT)
            {
                return false;
            }
        }

        // (2) find child directories
        //
        typedef std::list<file> dir_list_t;
        glob_to_list<dir_list_t> sub_dirs;
        bool success(false);
        if((flags & GLOB_ERR) != 0)
        {
            success = sub_dirs.read_path<
                  glob_to_list_flag_t::GLOB_FLAG_IGNORE_ERRORS
                , glob_to_list_flag_t::GLOB_FLAG_ONLY_DIRECTORIES>(path + "/*");
        }
        else
        {
            success = sub_dirs.read_path<
                  glob_to_list_flag_t::GLOB_FLAG_ONLY_DIRECTORIES>(path + "/*");
        }
        if(!success
        && sub_dirs.get_last_error_errno() != ENOENT)
        {
            f_last_error_message = sub_dirs.get_last_error_message();
            f_last_error_path = sub_dirs.get_last_error_path();
            f_last_error_errno = sub_dirs.get_last_error_errno();
            return f_last_error_errno != ENOENT;
        }

        // (3) read the sub-directories
        //
        for(auto const & d : sub_dirs)
        {
            if(!d.exists())
            {
                continue;
            }
            if(d.is_symbolic_link())
            {
                if(!f_follow_symlinks
                || !d.is_target_directory())
                {
                    continue;
                }
            }
            else
            {
                if(!d.is_directory())
                {
                    continue;
                }
            }

            // because we use a real-path, we may find that some paths are
            // duplicates (most certainly because of a softlink)
            //
            std::string const p(get_real_path(d.filename()));
            if(visited.insert(p).second)
            {
                if(!recursive_read_path(
                      p
                    , pattern
                    , flags
                    , visited))
                {
                    return false;
                }
            }
        }

        return true;
    }

    static thread_local glob_to_list *
                                g_self;

    std::string                 f_last_error_message = std::string();
    std::string                 f_last_error_path = std::string();
    int                         f_last_error_errno = 0;
    bool                        f_dot_and_dot_dot = false;
    bool                        f_recursive = false;
    bool                        f_follow_symlinks = false;
    bool                        f_empty = false;
};


template <typename C>
thread_local glob_to_list<C> * glob_to_list<C>::g_self = nullptr;


} // namespace snapdev
// vim: ts=4 sw=4 et
