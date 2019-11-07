// Check whether a set intersection is empty or not
// Copyright (c) 2019  Made to Order Software Corp.  All Rights Reserved
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
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#pragma once

/** \file
 * \brief Create a list of files using glob().
 *
 * This template allows you to insert filenames from the output of a glob()
 * call to an STL container.
 */

// snapdev lib
//
#include    <snapdev/raii_generic_deleter.h>


// C++ lib
//
#include    <memory>


// C lib
//
#include    <glob.h>


namespace snap
{


/** \brief A smart pointer to auto-delete glob results.
 *
 * This type defines a smart pointer which automatically frees all
 * the data allocated by glob() and this pointer too.
 *
 * Usage:
 *
 * \code
 *     snap::glob_to_list<std::vector<string>> glob;
 *     if(glob.read_path<
 *              snap::glob_to_list_flag_t::GLOB_FLAG_IGNORE_ERRORS,
 *              snap::glob_to_list_flag_t::GLOB_FLAG_PERIOD>(pattern))
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
 *     else {
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
    GLOB_FLAG_BRACE,              // allow {a,b,c}...
    GLOB_FLAG_IGNORE_ERRORS,      // read as much as possible
    GLOB_FLAG_MARK_DIRECTORY,     // add "/" to dirctory names
    GLOB_FLAG_NO_ESCAPE,          // ignore '\'
    GLOB_FLAG_ONLY_DIRECTORIES,   // only return directories
    GLOB_FLAG_PERIOD,             // allow period at the start (i.e. pattern ".*")
    GLOB_FLAG_TILDE               // transform "~/..." with "$HOME/..."
};


/** \brief Manage the results of glob() calls.
 *
 * This template is able to call glob() and insert the results to your
 * class.
 *
 * The supported flags allow for selecting which files to ignore. By
 * default, files that start with a period (.) are ignored.
 *
 * Here is an example of usage:
 *
 * \code
 *     glob_to_list<std::vector<std::string>> dir;
 *     if(!dir.read_path<snap::glob_to_list_flag_t::GLOB_FLAG_ONLY_DIRECTORIES>("/proc/[0-9]*"))
 *     {
 *         ...handle error...
 *         return;
 *     }
 *     for(auto f : dir)
 *     {
 *         ...f is std::string with filename...
 *     }
 * \endcode
 *
 * \tparam C  The type of the container where to add the filenames.
 * \tparam T  The type used for the filenames (C<T>).
 */
template<typename C>
class glob_to_list
    : public C
{
public:
    typedef C                       container_t;

    /** \brief Read files at given path.
     *
     * This function runs the glob() function with the given path.
     *
     * The \p path variable is expected to include a path with glob() like
     * patterns (i.e. `*.txt`, `0?.mp3`, etc.)
     *
     * \param[in] path  The path with glob patterns.
     */
	template<glob_to_list_flag_t ...args>
	bool read_path(std::string const & path)
    {
        int const flags = GLOB_NOSORT;// | flags_merge<args...>();

        glob_pointer_t dir(new glob_t);
        *dir = glob_t();
        g_self = this;
        int const r(glob(path.c_str(), flags, glob_err_callback, dir.get()));
        g_self = nullptr;       // to detect if glob_err_callback gets called improperly
        if(r == 0)
        {
            for(size_t idx(0); idx < dir->gl_pathc; ++idx)
            {
                C::insert(C::end(), typename glob_to_list::value_type(std::string(dir->gl_pathv[idx])));
            }
        }
        else
        {
            // do nothing when errors occur
            //
            std::string err_msg;
            switch(r)
            {
            case GLOB_NOSPACE:
                f_last_error_message =
                          "glob(\""
                        + path
                        + "\") did not have enough memory to alllocate its buffers.";
                break;

            case GLOB_ABORTED:
                f_last_error_message =
                          "glob(\""
                        + path
                        + "\") was aborted after a read error.";
                break;

            case GLOB_NOMATCH:
                f_last_error_message = "glob(\""
                        + path
                        + "\") could not find any files matching the pattern.";
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

    std::string get_last_error_message() const
    {
        return f_last_error_message;
    }

    std::string get_last_error_path() const
    {
        return f_last_error_path;
    }

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

        case glob_to_list_flag_t::GLOB_FLAG_TILDE:
            return GLOB_TILDE_CHECK | flags_merge<args...>();

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

    static thread_local glob_to_list *
                                g_self;

    std::string                 f_last_error_message = std::string();
    std::string                 f_last_error_path = std::string();
    int                         f_last_error_errno = 0;
};


template <typename C>
thread_local glob_to_list<C> * glob_to_list<C>::g_self = nullptr;


} // namespace snap
// vim: ts=4 sw=4 et
