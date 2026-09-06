// Copyright (c) 2023-2026  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Verify that encode()/decode() work as expected.
 *
 * This file implements tests to verify the encode() and decode() functions
 * used to transform an input to base64 and back.
 */

// self
//
#include    <snapdev/base64.h>

#include    "catch_main.h"


// last include
//
#include    <snapdev/poison.h>



CATCH_TEST_CASE("base64", "[base64][string]")
{
    CATCH_START_SECTION("base64: verify encoding & decoding")
    {
        for(int count(0); count < 1000; ++count)
        {
            // determine buffer size
            //
            std::size_t size;
            SNAP_CATCH2_NAMESPACE::random(size);
            size = size % 2000 + 1;

            // fill buffer with random data
            //
            std::string input;
            for(std::size_t idx(0); idx < size; ++idx)
            {
                std::uint8_t b;
                SNAP_CATCH2_NAMESPACE::random(b);
                input += static_cast<char>(b);
            }

            // encode the buffer
            //
            std::string output("ignore input in the output variable");
            snapdev::base64::encode(input, output);

            // encoded size is 4 characters for every 3 bytes of input rounded up
            //
            std::size_t const expected_size((size + 2) / 3 * 4);
            CATCH_REQUIRE(expected_size == output.length());

            // compare with OS base64 command
            //
            std::string const expected_output(SNAP_CATCH2_NAMESPACE::popen2("base64 --wrap=0", input));
//{
//std::ofstream f("input.bin");
//f.write(input.data(), input.length());
//}
//{
//std::ofstream f("output.b64");
//f.write(output.data(), output.length());
//}
//{
//std::ofstream f("out.b64");
//f.write(expected_output.data(), expected_output.length());
//}
            CATCH_REQUIRE_LONG_STRING(expected_output, output);

            std::string back("will this work?");
            CATCH_REQUIRE(snapdev::base64::decode(output, back));

            CATCH_REQUIRE(input.length() == back.length());
            CATCH_REQUIRE_LONG_STRING(input, back);
        }
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
