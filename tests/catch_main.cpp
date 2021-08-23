// Copyright (c) 2006-2021  Made to Order Software Corp.  All Rights Reserved
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
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.


/** \file
 * \brief Start the catch test for snapdev.
 *
 * This file implements the main test functions (initialization).
 */

// Tell catch we want it to add the runner code in this file.
#define CATCH_CONFIG_RUNNER

// self
//
#include    "catch_main.h"


// snapdev lib
//
#include    <snapdev/not_used.h>
#include    <snapdev/version.h>


// libexcept lib
//
#include    <libexcept/exception.h>


// C++ lib
//
#include    <sstream>


// last include
//
#include    <snapdev/poison.h>




namespace SNAP_CATCH2_NAMESPACE
{


std::string                 g_tmp_dir;



}
// SNAP_CATCH2_NAMESPACE namespace


namespace
{


Catch::clara::Parser add_command_line_options(Catch::clara::Parser const & cli)
{
    return cli
         | Catch::clara::Opt(SNAP_CATCH2_NAMESPACE::g_tmp_dir, "tmp")
              ["-T"]["--tmp"]
              ("a path to a temporary directory used by the tests.");
}



int finish_init(Catch::Session & session)
{
    snap::NOT_USED(session);

    if(!SNAP_CATCH2_NAMESPACE::g_tmp_dir.empty())
    {
        if(SNAP_CATCH2_NAMESPACE::g_tmp_dir == "/tmp")
        {
            std::cerr << "fatal error: you must specify a sub-directory for your temporary directory such as /tmp/snapdev";
            exit(1);
        }
    }
    else
    {
        SNAP_CATCH2_NAMESPACE::g_tmp_dir = "/tmp/snapdev";
    }

    // delete the existing tmp directory
    {
        std::stringstream ss;
        ss << "rm -rf \"" << SNAP_CATCH2_NAMESPACE::g_tmp_dir << "\"";
        if(system(ss.str().c_str()) != 0)
        {
            std::cerr << "fatal error: could not delete temporary directory \"" << SNAP_CATCH2_NAMESPACE::g_tmp_dir << "\".";
            exit(1);
        }
    }

    // then re-create the directory
    {
        std::stringstream ss;
        ss << "mkdir -p \"" << SNAP_CATCH2_NAMESPACE::g_tmp_dir << "\"";
        if(system(ss.str().c_str()) != 0)
        {
            std::cerr << "fatal error: could not create temporary directory \"" << SNAP_CATCH2_NAMESPACE::g_tmp_dir << "\".";
            exit(1);
        }
    }

    return 0;
}



}


int main(int argc, char * argv[])
{
    return SNAP_CATCH2_NAMESPACE::snap_catch2_main(
              "snapdev"
            , SNAPDEV_VERSION_STRING
            , argc
            , argv
            , []() { libexcept::set_collect_stack(false); }
            , &add_command_line_options
            , &finish_init
        );
}

// vim: ts=4 sw=4 et
