// Snap Websites Server -- read mount points in Linux
// Copyright (c) 2013-2022  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Gather the list of mount points on a system.
 *
 * This function reads the list of mount points and save those in a vector.
 */

// self
//
#include    "snapdev/raii_generic_deleter.h"


// C++
//
#include    <vector>


// C
//
#include    <mntent.h>



namespace snapdev
{


typedef std::unique_ptr<FILE, raii_pointer_deleter<FILE, decltype(&::endmntent), &::endmntent>> endmntent_pointer_t;


class mount_entry
{
public:
    typedef std::vector<mount_entry>    vector_t;

    mount_entry(struct mntent * e)
        : f_fsname(e->mnt_fsname)
        , f_dir(e->mnt_dir)
        , f_type(e->mnt_type)
        , f_options(e->mnt_opts)
        , f_freq(e->mnt_freq)
        , f_passno(e->mnt_passno)
    {
    }

    std::string const & get_fsname()  const { return f_fsname;  }
    std::string const & get_dir()     const { return f_dir;     }
    std::string const & get_type()    const { return f_type;    }
    std::string const & get_options() const { return f_options; }
    int                 get_freq()    const { return f_freq;    }
    int                 get_passno()  const { return f_passno;  }

private:
    std::string         f_fsname;
    std::string         f_dir;
    std::string         f_type;
    std::string         f_options;
    int                 f_freq;
    int                 f_passno;
};



class mounts
    : public mount_entry::vector_t
{
public:
    /** \brief Read all the mount points defined in file defined by \p path.
     *
     * This function reads all the mount points found in the file pointed by
     * \p path and adds them to this mounts object which is a vector.
     *
     * You have full access to the vector once the mounts constructor returns
     * so you can add or delete entries if you want to.
     *
     * \warning
     * This funciton is not thread safe.
     *
     * \param[in] path  The path to an fstab file such as "/etc/fstab" or
     *                  "/proc/mounts".
     */
    mounts(std::string const & path = std::string("/proc/mounts"))
        : f_path(path)
    {
        struct mntent * m;
        FILE * in;

        in = setmntent(path.c_str(), "r");
        if(in == nullptr)
        {
            throw std::runtime_error("mounts() cannot open \"" + path + "\"");
        }
        endmntent_pointer_t safe(in);

        while(m = getmntent(in))
        {
            push_back(mount_entry(m));
        }
    }

    /** \brief Retrieve the path of these mounts.
     *
     * This function returns the path and filename of the file used
     * to read the mount points.
     */
    std::string const & get_path() const
    {
        return f_path;
    }

private:
    std::string             f_path;
};



} // namespace snapdev
// vim: ts=4 sw=4 et
