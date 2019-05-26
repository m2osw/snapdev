/*
 * File:
 *    tests/catch2_main.cpp
 *
 * License:
 *    Copyright (c) 2006-2019  Made to Order Software Corp.  All Rights Reserved
 *
 *    https://snapwebsites.org/
 *    contact@m2osw.com
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License along
 *    with this program; if not, write to the Free Software Foundation, Inc.,
 *    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Authors:
 *    Alexis Wilke   alexis@m2osw.com
 */

// Tell catch we want it to add the runner code in this file.
#define CATCH_CONFIG_RUNNER

// self
//
#include "catch2_main.h"

// snapdev lib
//
#include "snapdev/version.h"

// libexcept lib
//
#include "libexcept/exception.h"

// C++ lib
//
#include <sstream>


namespace unittest
{
    bool            g_verbose;
}


int unittest_main(int argc, char * argv[])
{
    int         set_seed = 0;
    bool        progress = false;
    bool        version = false;

    Catch::Session session;

    auto cli = session.cli()
             | Catch::clara::Opt(set_seed, "seed")
                ["-S"]["--seed"]
                ("value to seed the randomizer, if not specified, randomize")
             | Catch::clara::Opt(progress)
                ["-p"]["--progress"]
                ("print name of test section being run")
             | Catch::clara::Opt(version)
                ["-V"]["--version"]
                ("print out the libutf8 library version these unit tests pertain to");

    session.cli(cli);

    auto result(session.applyCommandLine(argc, argv));
    if(result != 0)
    {
        std::cerr << "Error in command line." << std::endl;
        exit(1);
    }

    if(version)
    {
        std::cout << SNAPDEV_VERSION_STRING << std::endl;
        exit(0);
    }

    unittest::g_verbose = progress;

    // by default we get a different seed each time; that really helps
    // in detecting errors! (I know, I wrote loads of tests before)
    //
    unsigned int const seed(set_seed != 0
                            ? set_seed
                            : static_cast<unsigned int>(time(NULL)));
    srand(seed);
    std::cout << "libutf8 v" << SNAPDEV_VERSION_STRING << " [" << getpid() << "]:unittest: seed is " << seed << std::endl;

    return session.run();
}


int main(int argc, char * argv[])
{
    int r(1);

    try
    {
        libexcept::set_collect_stack(false);
        r = unittest_main(argc, argv);
    }
    catch(std::logic_error const & e)
    {
        std::cerr << "fatal error: caught a logic error in snapdev unit tests: " << e.what() << "\n";
    }

    return r;
}

// vim: ts=4 sw=4 et
