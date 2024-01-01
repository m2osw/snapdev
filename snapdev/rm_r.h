// Copyright (c) 2013-2024  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Function to remove directories and their contents.
 *
 * This function is similar to the `rm -r <path>` command line.
 *
 * Note that if you just want to remove a directory if it is not empty,
 * then simply use the `rmdir()` function.
 */

// self
//
#include    <snapdev/glob_to_list.h>


// C++
//
#include    <algorithm>
#include    <vector>


// C
//
#include    <sys/stat.h>
#include    <sys/types.h>





namespace snapdev
{


/** \brief Mode for removal of special files.
 *
 * When using an `rm -rf ...` you end up deleting absolutely everything
 * and in some cases that may not quite be what you wanted to do. The
 * rm_r() function uses these values to determine how to handle
 * special files. It also uses a flag named `keep_going` to know whether
 * to stop on the very first error (default) or go on and delete as much
 * as possible before returning.
 */
enum class special_file_t
{
    /** \brief Generate an error if a special file exists.
     *
     * When the `special` parameter of the rm_r() function is set to this
     * value, the function runs a first sweep to see whether a special file
     * exists. If so, it fails early with EPERM and no file gets deleted.
     */
    SPECIAL_FILE_ERROR,

    /** \brief Ignore special files.
     *
     * By default, the rm_r() function generates an error if it files a
     * special file (a file other than a regular file or a directory).
     *
     * When using this option, the special files are kept in place.
     */
    SPECIAL_FILE_IGNORE,

    /** \brief Remove FIFOs and sockets, keep devices.
     *
     * This value can be used to request the deletion of regular files,
     * directories, FIFOs and sockets. However, device files (block and
     * character devices) are not deleted.
     */
    SPECIAL_FILE_KEEP_DEVICES,

    /** \brief Remove special files.
     *
     * The rm_r() can be asked to remove absolutely everything, including
     * all special files.
     */
    SPECIAL_FILE_REMOVE,
};


/** \brief Remove a directory as with `rm -r ...`.
 *
 * This function goes through the specified directory and remove all the
 * files including the directory itself. If `path` includes a pattern,
 * then all the corresponding files are deleted.
 *
 * When the function returns -1, the errno is set to what the command used
 * set it at (i.e EPERM, ENOTDIR, etc.)
 *
 * The \p special parameter is used to define the behavior of the function
 * when a special file is found in the directory tree. Here are the
 * different types and how they are handled depending on the \p special
 * parameter:
 *
 * * Regular Files and Symbolic Links
 *
 *   These are always removed unless an error occurs before that specific
 *   file is reached.
 *
 * * Directories
 *
 *   These are removed if empty after the removal of other files (or they
 *   started empty).
 *
 *   If a special file is found inside a directory and that special file does
 *   not get removed, then the directory is silently left behind.
 *
 * * FIFO and Sockets
 *
 *   When the \p special parameter is set to
 *   special_file_t::SPECIAL_FILE_ERROR, any FIFO or Socket found in the
 *   directory tree prevents the deletion of any file. The function returns
 *   with -1 and errno set to EPERM.
 *
 *   When the \p special parameter is set to
 *   special_file_t::SPECIAL_FILE_IGNORE, FIFO and Socket files and their
 *   parent directories are all kept.
 *
 *   When the \p special parameter is set to
 *   special_file_t::SPECIAL_FILE_KEEP_DEVICES or
 *   special_file_t::SPECIAL_FILE_REMOVE, FIFO and Socket files get removed.
 *
 * * Block and Character Device Files
 *
 *   When the \p special parameter is set to
 *   special_file_t::SPECIAL_FILE_ERROR, any FIFO or Socket found in the
 *   directory tree prevents the deletion of any file. The function returns
 *   with -1 and errno set to EPERM.
 *
 *   When the \p special parameter is set to
 *   special_file_t::SPECIAL_FILE_IGNORE or
 *   special_file_t::SPECIAL_FILE_KEEP_DEVICES, Block and Character Device
 *   files and their parent directories are all kept.
 *
 *   When the \p special parameter is set to
 *   special_file_t::SPECIAL_FILE_REMOVE, Block and Character Device files get
 *   removed.
 *
 * The \p keep_going flag can be used to request the function to delete as
 * many files as possible even if errors are encountered.
 *
 * \warning
 * As mentioned above, the function may return 0 even though the resulting
 * path does not get deleted. This happens when special files are found and
 * kept behind.
 *
 * \param[in] pattern  The path to remove. It can include a glob-like pattern.
 * \param[in] special  How to handle special files.
 * \param[in] keep_going  Try to delete as much as possible.
 *
 * \return 0 if the directory gets removed;
 *         -1 if an error occurs (i.e. permissions denied).
 */
inline int rm_r(
      std::string const & pattern
    , special_file_t special = special_file_t::SPECIAL_FILE_ERROR
    , bool keep_going = false)
{
    snapdev::glob_to_list<std::vector<std::string>> glob;
    int e(0);
    std::string::size_type pos(0);
    struct stat st;

    std::string root;
    std::string extra;
    if(stat(pattern.c_str(), &st) == 0)
    {
        if(S_ISDIR(st.st_mode))
        {
            extra = "/*";
            root = glob.get_real_path(pattern);
        }
    }

    // get a list of the existing files
    //
    if(glob.read_path<
             snapdev::glob_to_list_flag_t::GLOB_FLAG_IGNORE_ERRORS,
             snapdev::glob_to_list_flag_t::GLOB_FLAG_BRACE,
             snapdev::glob_to_list_flag_t::GLOB_FLAG_PERIOD,
             snapdev::glob_to_list_flag_t::GLOB_FLAG_TILDE,
             snapdev::glob_to_list_flag_t::GLOB_FLAG_RECURSIVE,
             snapdev::glob_to_list_flag_t::GLOB_FLAG_EMPTY>(pattern + extra))
    {
        // also delete the root if it was not a pattern
        //
        if(!root.empty())
        {
            glob.push_back(root);
        }

        // sort the filenames, longest first since that represents the
        // deepest file in a given directory and that is the order in
        // which we need to delete all the files/directories
        //
        std::sort(
              glob.begin()
            , glob.end()
            , [](std::string const & lhs, std::string const & rhs)
              {
                  return lhs.size() > rhs.size();
              });

        // do a first sweep to determine whether any special files exist
        // and if so return with -1 and errno = EPERM
        //
        if(special == special_file_t::SPECIAL_FILE_ERROR
        && !keep_going)
        {
            for(auto const & filename : glob)
            {
                if(stat(filename.c_str(), &st) == 0)
                {
                    switch(st.st_mode & S_IFMT)
                    {
                    case S_IFDIR:
                    case S_IFREG:
                    case S_IFLNK:
                        break;

                    default: // devices, fifos, sockets, and unknown
                        errno = EPERM;
                        return -1;

                    }
                }
                else if(errno != ENOENT)
                {
                    // except for missing files stop on errors
                    //
                    return -1;
                }
            }
        }

        // now process the removal
        //
        // WARNING: I use an index because I may remove parent folders
        //          and thus would mess up an `auto`
        //
        for(std::size_t i(0); i < glob.size(); ++i)
        {
            std::string filename(glob[i]);
            if(stat(filename.c_str(), &st) == 0)
            {
                int r(0);
                bool keep_parents(false);
                switch(st.st_mode & S_IFMT)
                {
                case S_IFDIR:
                    r = rmdir(filename.c_str());
                    break;

                case S_IFREG:
                case S_IFLNK:
                    r = unlink(filename.c_str());
                    break;

                case S_IFIFO:
                case S_IFSOCK:
                    if(special == special_file_t::SPECIAL_FILE_KEEP_DEVICES
                    || special == special_file_t::SPECIAL_FILE_REMOVE)
                    {
                        r = unlink(filename.c_str());
                    }
                    else
                    {
                        keep_parents = true;

                        if(special == special_file_t::SPECIAL_FILE_ERROR
                        && e == 0)
                        {
                            e = EPERM;
                        }
                    }
                    break;

                case S_IFBLK:
                case S_IFCHR:
                    if(special == special_file_t::SPECIAL_FILE_REMOVE)
                    {
                        r = unlink(filename.c_str());
                    }
                    else
                    {
                        keep_parents = true;

                        if(special == special_file_t::SPECIAL_FILE_ERROR
                        && e == 0)
                        {
                            e = EPERM;
                        }
                    }
                    break;

                default:
                    // unknown file type, do not mess with it
                    //
                    errno = EPERM;
                    return -1;

                }
                if(r != 0)
                {
                    if(!keep_going)
                    {
                        return -1;
                    }
                    if(e != 0)
                    {
                        e = errno;
                    }
                }
                if(keep_parents)
                {
                    // remove the parent directories since we won't be able
                    // to delete them since we're "legally" keeping some
                    // files in the child most directory
                    //
                    for(;;)
                    {
                        pos = filename.rfind('/');
                        if(pos == std::string::npos)
                        {
                            break;
                        }
                        filename = filename.substr(0, pos);
                        for(std::size_t j(i + 1); j < glob.size(); ++j)
                        {
                            if(glob[j] == filename)
                            {
                                glob.erase(glob.begin() + j);
                                break;
                            }
                        }
                    }
                }
            }
            else if(errno != ENOENT)
            {
                // the stat() failed with an error other than:
                //    "file does not exist"
                //
                if(!keep_going)
                {
                    return -1;
                }
                if(e == 0)
                {
                    e = errno;
                }
            }
        }
    }
    // else -- the directories/files do not exist, success

    if(e == 0)
    {
        return 0;
    }

    // case where keep_going is true and an error occurred
    //
    errno = e;

    return -1;
}



} // namespace snapdev
// vim: ts=4 sw=4 et
