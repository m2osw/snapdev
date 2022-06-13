// Copyright (c) 2011-2022  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Verify the to_string_literal templates.
 *
 * This file verifies that the to_string_literal template works as expected.
 */

// snapdev
//
#include    <snapdev/to_string_literal.h>


// self
//
#include    "catch_main.h"


// C++
//
#include    <iomanip>



namespace
{

// convert a port number to a string at compile time
//
constexpr int const         LOCAL_PORT = 9123;
constexpr std::string_view  g_port_decimal = snapdev::to_string_literal<LOCAL_PORT>.data();
constexpr std::string_view  g_port_decimal_explicit = snapdev::to_string_literal<LOCAL_PORT, 10>.data();
constexpr std::string_view  g_port_hexadecimal = snapdev::to_string_literal<LOCAL_PORT, 16>.data();
constexpr std::string_view  g_port_hexadecimal_uppercase = snapdev::to_string_literal<LOCAL_PORT, 16, true>.data();
constexpr std::string_view  g_port_octal = snapdev::to_string_literal<LOCAL_PORT, 8>.data();
constexpr std::string_view  g_port_binary = snapdev::to_string_literal<LOCAL_PORT, 2>.data();

}


CATCH_TEST_CASE("to_string_literal", "[basic][strings]")
{
    CATCH_START_SECTION("to_string_literal: verify literals")
    {
        std::stringstream ds;
        ds << std::setbase(10) << LOCAL_PORT;
        CATCH_REQUIRE(ds.str() == std::string(g_port_decimal));
        CATCH_REQUIRE(ds.str() == std::string(g_port_decimal_explicit));

        std::stringstream hs;
        hs << std::setbase(16) << LOCAL_PORT;
        CATCH_REQUIRE(hs.str() == std::string(g_port_hexadecimal));

        std::stringstream hsu;
        hsu << std::uppercase << std::setbase(16) << LOCAL_PORT;
        CATCH_REQUIRE(hsu.str() == std::string(g_port_hexadecimal_uppercase));

        std::stringstream os;
        os << std::setbase(8) << LOCAL_PORT;
        CATCH_REQUIRE(os.str() == std::string(g_port_octal));

        // unfortunately, for some really odd reasons, the std::setbase()
        // is limited to 8, 10, and 16...
        //
        char binary[64] = {};
        std::size_t q(std::size(binary) - 1);
        for(auto p(LOCAL_PORT); p != 0; p /= 2)
        {
            --q;
            binary[q] = (p & 1) + '0';
        }
        CATCH_REQUIRE(std::string(binary + q) == std::string(g_port_binary));
    }
    CATCH_END_SECTION()
}


// vim: ts=4 sw=4 et
