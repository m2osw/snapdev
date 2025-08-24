// Copyright (c) 2018-2025  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Verify that the vector_streambuf class works.
 *
 * This file implements tests for the vector_streambuf class to make
 * sure that the flags are indeed safe.
 */

// self
//
#include    <snapdev/vector_streambuf.h>

#include    "catch_main.h"


// C++
//
#include    <sstream>


// last include
//
#include    <snapdev/poison.h>




CATCH_TEST_CASE("vector_streambuf", "[stream]")
{
    CATCH_START_SECTION("vector_streambuf: verify read & seek capability")
    {
        std::vector<char> data{ 1, 2, 3, 4, 5, 6, 7 };

        snapdev::vector_streambuf buf(data);

        std::istream in(&buf);

        CATCH_REQUIRE(in.tellg() == 0);
        CATCH_REQUIRE(in.get() == 1);
        CATCH_REQUIRE(in.tellg() == 1);
        CATCH_REQUIRE(in.get() == 2);
        CATCH_REQUIRE(in.tellg() == 2);
        CATCH_REQUIRE(in.get() == 3);
        CATCH_REQUIRE(in.tellg() == 3);
        CATCH_REQUIRE(in.get() == 4);
        CATCH_REQUIRE(in.tellg() == 4);
        CATCH_REQUIRE(in.get() == 5);
        CATCH_REQUIRE(in.tellg() == 5);
        CATCH_REQUIRE(in.get() == 6);
        CATCH_REQUIRE(in.tellg() == 6);
        CATCH_REQUIRE(in.get() == 7);
        CATCH_REQUIRE(in.tellg() == 7);

        CATCH_REQUIRE(in.get() == EOF);
        CATCH_REQUIRE(in.tellg() == -1); // when eof() is true, the returned position is always -1
        CATCH_REQUIRE(in.get() == EOF);
        CATCH_REQUIRE(in.tellg() == -1);
        CATCH_REQUIRE(in.get() == EOF);
        CATCH_REQUIRE(in.tellg() == -1);

        in.clear(); // clear EOF
        in.seekg(4);
        CATCH_REQUIRE(in.get() == 5);

        in.seekg(-10);
        CATCH_REQUIRE(in.get() == 1);

        in.seekg(10);
        CATCH_REQUIRE(in.get() == EOF);

        in.clear(); // clear EOF
        in.seekg(-4, std::ios_base::end);
        CATCH_REQUIRE(in.get() == 4);
        CATCH_REQUIRE(in.get() == 5);

        // our exception gets swallowed by default, but the fail bit gets
        // set as expected
        //
        CATCH_REQUIRE_FALSE(in.fail());
        in.seekg(-4, static_cast<std::ios_base::seekdir>(100));
        CATCH_REQUIRE(in.fail());
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("vector_streambuf: verify read, write & seek capability")
    {
        std::vector<char> data{ 1, 2, 3, 4, 5, 6, 7 };

        snapdev::vector_streambuf buf(data);

        std::iostream s(&buf);

        // as above, we can read up to EOF
        //
        CATCH_REQUIRE(s.tellg() == 0);
        CATCH_REQUIRE(s.get() == 1);
        CATCH_REQUIRE(s.tellg() == 1);
        CATCH_REQUIRE(s.get() == 2);
        CATCH_REQUIRE(s.tellg() == 2);
        CATCH_REQUIRE(s.get() == 3);
        CATCH_REQUIRE(s.tellg() == 3);
        CATCH_REQUIRE(s.get() == 4);
        CATCH_REQUIRE(s.tellg() == 4);
        CATCH_REQUIRE(s.get() == 5);
        CATCH_REQUIRE(s.tellg() == 5);
        CATCH_REQUIRE(s.get() == 6);
        CATCH_REQUIRE(s.tellg() == 6);
        CATCH_REQUIRE(s.get() == 7);
        CATCH_REQUIRE(s.tellg() == 7);

        CATCH_REQUIRE(s.get() == EOF);
        s.clear(); // clear EOF

        s.seekp(4);
        CATCH_REQUIRE(s.tellp() == 4);
        s.put(10);
        CATCH_REQUIRE_FALSE(s.fail());
        CATCH_REQUIRE(s.tellp() == 5);
        s.put(11);
        CATCH_REQUIRE_FALSE(s.fail());
        CATCH_REQUIRE(s.tellp() == 6);
        s.put(12);
        CATCH_REQUIRE_FALSE(s.fail());
        CATCH_REQUIRE(s.tellp() == 7);

        s.seekg(4);
        CATCH_REQUIRE(s.get() == 10);
        CATCH_REQUIRE(s.get() == 11);
        CATCH_REQUIRE(s.get() == 12);

        CATCH_REQUIRE(s.get() == EOF);

        s.clear();
        s.seekp(-10);
        CATCH_REQUIRE(s.tellp() == 0);
        s.seekp(10);
        CATCH_REQUIRE(s.tellp() == 7);
        s.seekp(-4, std::ios_base::end);
        CATCH_REQUIRE(s.tellp() == 3);
        s.put(20);
        s.put(21);
        s.put(22);
        s.seekg(3);
        CATCH_REQUIRE(s.get() == 20);
        CATCH_REQUIRE(s.get() == 21);
        CATCH_REQUIRE(s.get() == 22);
        CATCH_REQUIRE(s.get() == 12);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("vector_streambuf: verify read, write & seek capability with a read-only vector")
    {
        std::vector<char> const data{ 1, 2, 3, 4, 5, 6, 7 };

        snapdev::vector_streambuf buf(data);

        std::iostream s(&buf);

        // as above, we can read up to EOF
        //
        CATCH_REQUIRE(s.tellg() == 0);
        CATCH_REQUIRE(s.get() == 1);
        CATCH_REQUIRE(s.tellg() == 1);
        CATCH_REQUIRE(s.get() == 2);
        CATCH_REQUIRE(s.tellg() == 2);
        CATCH_REQUIRE(s.get() == 3);
        CATCH_REQUIRE(s.tellg() == 3);
        CATCH_REQUIRE(s.get() == 4);
        CATCH_REQUIRE(s.tellg() == 4);
        CATCH_REQUIRE(s.get() == 5);
        CATCH_REQUIRE(s.tellg() == 5);
        CATCH_REQUIRE(s.get() == 6);
        CATCH_REQUIRE(s.tellg() == 6);
        CATCH_REQUIRE(s.get() == 7);
        CATCH_REQUIRE(s.tellg() == 7);

        CATCH_REQUIRE(s.get() == EOF);
        s.clear(); // clear EOF

        s.seekp(4);
        CATCH_REQUIRE_FALSE(s.fail());
        s.put(10);
        CATCH_REQUIRE(s.fail());
        s.clear();
        s.put(11);
        CATCH_REQUIRE(s.fail());
        s.clear();
        s.put(12);
        CATCH_REQUIRE(s.fail());
        s.clear();

        s.seekg(4);
        CATCH_REQUIRE(s.get() == 5);  // buffer not updated
        CATCH_REQUIRE(s.get() == 6);
        CATCH_REQUIRE(s.get() == 7);

        CATCH_REQUIRE(s.get() == EOF);
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et

