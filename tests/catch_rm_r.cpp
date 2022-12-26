// Copyright (c) 2018-2022  Made to Order Software Corp.  All Rights Reserved
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

/** \file
 * \brief Verify that the rm_r() function works.
 *
 * This file implements tests for the rm_r() function.
 */

// self
//
#include    <snapdev/rm_r.h>

#include    <snapdev/as_root.h>
#include    <snapdev/raii_generic_deleter.h>
#include    <snapdev/mkdir_p.h>
#include    <snapdev/pathinfo.h>

#include    "catch_main.h"


// C
//
#include    <sys/socket.h>
#include    <sys/sysmacros.h>
#include    <sys/un.h>
#include    <unistd.h>


// last include
//
#include    <snapdev/poison.h>


namespace
{



void create_files(std::string const & folder, int depth, int max_depth, bool special_files, std::vector<int> & sockets)
{
    int r(0);

    // first create this directory (unless it exists, then leave it alone)
    //
    struct stat st;
    if(stat(folder.c_str(), &st) == 0)
    {
        // if it already exists and it is a directory, go on
        //
        CATCH_REQUIRE(S_ISDIR(st.st_mode));
    }
    else
    {
        r = mkdir(folder.c_str(), 0700);
        CATCH_REQUIRE(r == 0);
    }

    // second create a few text files in this folder
    //
    int const max(rand() % 5 + 1);
    for(int i(1); i <= max; ++i)
    {
        std::string filename(folder);
        filename += '/';
        filename += "file";
        filename += std::to_string(i);
        filename += ".txt";

        std::ofstream out(filename);
        out << SNAP_CATCH2_NAMESPACE::random_string(10, 100);
        CATCH_REQUIRE(!!out);
    }

    if(special_files)
    {
        // also create special files
        //
        std::string fifo(folder);
        fifo += "/file.fifo";
        r = mkfifo(fifo.c_str(), 0700);
        CATCH_REQUIRE(r == 0);

        std::string socket_name(folder);
        socket_name += "/file.socket";
        struct sockaddr_un addr = {};
        addr.sun_family = AF_UNIX;
        CATCH_REQUIRE(socket_name.length() < sizeof(addr.sun_path));
        strncpy(addr.sun_path, socket_name.c_str(), sizeof(addr.sun_path));
        int s = socket(AF_UNIX, SOCK_STREAM, 0);
        CATCH_REQUIRE(s != -1);
        sockets.push_back(s);
        r = bind(s, reinterpret_cast<sockaddr const *>(&addr), sizeof(addr));
        CATCH_REQUIRE(r == 0);

        // creating device files is a root matter
        //
        snapdev::as_root root;
        if(root.is_switched())
        {
            std::string char_dev(folder);
            char_dev += "/char.dev";
            r = mknod(char_dev.c_str(), 0700 | S_IFCHR, makedev(1, 5));    // like /dev/zero
            CATCH_REQUIRE(r == 0);

            std::string block_dev(folder);
            block_dev += "/block.dev";
            r = mknod(block_dev.c_str(), 0700 | S_IFBLK, makedev(7, 1));   // like /dev/loop1
            CATCH_REQUIRE(r == 0);
        }
        else
        {
            static bool message_shown(false);
            if(!message_shown)
            {
                message_shown = true;

                std::cout << "--- note that the special file test cannot create block and character devices.\n"
                    << "--- if you want to test those, make sure to run as root:\n"
                    << "---    sudo unittest ... rm_r\n";
            }
        }
    }

    // third create sub-directories and recursively create files/dirs
    // unless we already reached the maximum depth
    //
    if(depth >= max_depth)
    {
        return;
    }
    int const max_foo(rand() % 3 + 2);
    for(int foo(1); foo <= max_foo; ++foo)
    {
        std::string sub_folder(folder);
        sub_folder += "/foo";
        sub_folder += std::to_string(foo);

        create_files(sub_folder, depth + 1, max_depth, special_files, sockets);
    }
}



}


CATCH_TEST_CASE("rm_r", "[os]")
{
    int r(0);

    CATCH_START_SECTION("rm_r: missing")
    {
        std::string path(SNAP_CATCH2_NAMESPACE::g_tmp_dir());
        path += "/missing";

        r = snapdev::rm_r(path);
        CATCH_REQUIRE(r == 0);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("rm_r: empty directory")
    {
        std::string path(SNAP_CATCH2_NAMESPACE::g_tmp_dir());
        path += "/empty";

        struct stat st;
        if(stat(path.c_str(), &st) == 0)
        {
            // if it already exists and it is a directory, go on
            //
            CATCH_REQUIRE(S_ISDIR(st.st_mode));
        }
        else
        {
            r = mkdir(path.c_str(), 0700);
            CATCH_REQUIRE(r == 0);
        }

        r = snapdev::rm_r(path);
        CATCH_REQUIRE(r == 0);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("rm_r: directory with a few files")
    {
        std::string path(SNAP_CATCH2_NAMESPACE::g_tmp_dir());
        path += "/directory";

        struct stat st;
        if(stat(path.c_str(), &st) == 0)
        {
            // if it already exists and it is a directory, go on
            //
            CATCH_REQUIRE(S_ISDIR(st.st_mode));
        }
        else
        {
            r = mkdir(path.c_str(), 0700);
            CATCH_REQUIRE(r == 0);
        }

        int const max(rand() % 5 + 1);
        for(int i(1); i <= max; ++i)
        {
            std::string filename(path);
            filename += '/';
            filename += "file";
            filename += std::to_string(i);
            filename += ".txt";

            std::ofstream out(filename);
            out << SNAP_CATCH2_NAMESPACE::random_string(10, 100);
        }

        r = snapdev::rm_r(path);
        CATCH_REQUIRE(r == 0);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("rm_r: directory tree")
    {
        std::string path(SNAP_CATCH2_NAMESPACE::g_tmp_dir());
        path += "/tree";

        std::vector<int> sockets;
        int const max_depth(rand() % 3 + 1);
        create_files(path, 1, max_depth, false, sockets);
        // ignore the sockets since we do not create special files

        r = snapdev::rm_r(path);
        CATCH_REQUIRE(r == 0);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("rm_r: directory tree with special files")
    {
        std::string path(SNAP_CATCH2_NAMESPACE::g_tmp_dir());
        path += "/special";

        std::vector<int> sockets;
        int const max_depth(rand() % 3 + 1);
        create_files(path, 1, max_depth, true, sockets);

        // deleting as is must fail
        // (i.e. we detect special files early and don't delete anything)
        //
        r = snapdev::rm_r(path);
        if(r == 0)
        {
            CATCH_REQUIRE(r != 0);
        }
        CATCH_REQUIRE(errno == EPERM);

        // try with SPECIAL_FILE_IGNORE
        // (i.e. delete everything exception special files)
        //
        r = snapdev::rm_r(path, snapdev::special_file_t::SPECIAL_FILE_IGNORE);
        CATCH_REQUIRE(r == 0);

        // try with SPECIAL_FILE_KEEP_DEVICES
        // (i.e. delete everything exception devices)
        //
        r = snapdev::rm_r(path, snapdev::special_file_t::SPECIAL_FILE_KEEP_DEVICES);
        CATCH_REQUIRE(r == 0);

        // try with SPECIAL_FILE_REMOVE
        // (i.e. delete absolutely everything)
        //
        // note: this requires root permissions (we do not create if we
        //       can't become root)
        {
            snapdev::as_root root;
            r = snapdev::rm_r(path, snapdev::special_file_t::SPECIAL_FILE_REMOVE);
            CATCH_REQUIRE(r == 0);
        }

        // now close all the sockets (ignore errors)
        //
        for(auto const & s : sockets)
        {
            close(s);
        }

        // finally make sure the directory is gone
        //
        struct stat st;
        CATCH_REQUIRE(stat(path.c_str(), &st) != 0);
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
