// Copyright (c) 2006-2025  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Start the catch test for snapdev.
 *
 * This file implements the main test functions (initialization).
 */

// Tell catch we want it to add the runner code in this file.
#define CATCH_CONFIG_RUNNER

// self
//
#include    "catch_main.h"


// snapdev
//
#include    <snapdev/clocale.h>
#include    <snapdev/version.h>


// libexcept
//
#include    <libexcept/exception.h>


// C++
//
#include    <sstream>


// last include
//
#include    <snapdev/poison.h>



int main(int argc, char * argv[])
{
    snapdev::clocale();

    return SNAP_CATCH2_NAMESPACE::snap_catch2_main(
              "snapdev"
            , SNAPDEV_VERSION_STRING
            , argc
            , argv
            , []() { libexcept::set_collect_stack(libexcept::collect_stack_t::COLLECT_STACK_NO); }
            , nullptr
            , nullptr
        );
}

// vim: ts=4 sw=4 et
