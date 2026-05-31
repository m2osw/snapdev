// Copyright (c) 2026  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Verify that the safe_tcattr object works.
 *
 * This file implements tests for the safe_tcattr template to make sure that
 * it works as expected.
 */

// self
//
#include    <snapdev/safe_tcattr.h>

#include    "catch_main.h"


// snapdev
//
//#include    <snapdev/not_reached.h>


// last include
//
#include    <snapdev/poison.h>


namespace
{




} // no name namespace


CATCH_TEST_CASE("safe_tcattr", "[raii][terminal]")
{
    CATCH_START_SECTION("safe_tcattr: remove/restore ECHO")
    {
        snapdev::safe_tcattr safe;
        termios attr = {};
        safe.get_original_attributes(attr);
        termios const original_attr(attr);
        if((original_attr.c_lflag & ECHO) != 0)
        {
            attr.c_lflag &= ~ECHO;
            safe.set_attributes(TCSANOW, attr);

            termios new_attr = {};
            safe.get_attributes(new_attr);
            CATCH_REQUIRE((new_attr.c_lflag & ECHO) == 0);

            safe.restore_attributes();

            termios restored_attr = {};
            safe.get_attributes(restored_attr);
            CATCH_REQUIRE((restored_attr.c_lflag & ECHO) != 0);
        }
        else
        {
            // you can test that case as a programmer using ./mk -l -t safe_tcattr
            //
            attr.c_lflag |= ECHO;
            safe.set_attributes(TCSANOW, attr);

            termios new_attr = {};
            safe.get_attributes(new_attr);
            CATCH_REQUIRE((new_attr.c_lflag & ECHO) != 0);

            safe.restore_attributes();

            termios restored_attr = {};
            safe.get_attributes(restored_attr);
            CATCH_REQUIRE((restored_attr.c_lflag & ECHO) == 0);
        }
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
