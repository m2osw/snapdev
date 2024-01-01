// Copyright (c) 2011-2024  Made to Order Software Corp.  All Rights Reserved
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
constexpr std::string_view  g_port_decimal = snapdev::integer_to_string_literal<LOCAL_PORT>.data();
constexpr std::string_view  g_port_decimal_explicit = snapdev::integer_to_string_literal<LOCAL_PORT, 10>.data();
constexpr std::string_view  g_port_hexadecimal = snapdev::integer_to_string_literal<LOCAL_PORT, 16>.data();
constexpr std::string_view  g_port_hexadecimal_uppercase = snapdev::integer_to_string_literal<LOCAL_PORT, 16, true>.data();
constexpr std::string_view  g_port_octal = snapdev::integer_to_string_literal<LOCAL_PORT, 8>.data();
constexpr std::string_view  g_port_binary = snapdev::integer_to_string_literal<LOCAL_PORT, 2>.data();

// these require C++20, becaue double_wrapper() is not otherwise accepted as a type to pass values as is
//constexpr float const       CONSTANT_ROOT_TWO = 1.414213562373095048801688724209698078;
//constexpr double const      CONSTANT_PI = 3.141592653589793238462643383279502884;
//constexpr long double const CONSTANT_E = 2.718281828459045235360287471352662497;
//constexpr std::string_view  g_root_two = snapdev::floating_point_to_string_literal<snapdev::detail::double_wrapper(CONSTANT_ROOT_TWO)>.data();
//constexpr std::string_view  g_pi = snapdev::floating_point_to_string_literal<CONSTANT_PI>.data();
//constexpr std::string_view  g_e = snapdev::floating_point_to_string_literal<CONSTANT_E>.data();

}


CATCH_TEST_CASE("to_string_literal", "[string]")
{
    CATCH_START_SECTION("integer_to_string_literal: verify integral literals")
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

// this requires C++20 to compile...
//
//    CATCH_START_SECTION("floating_point_to_string_literal: verify float literals")
//    {
////constexpr float const       CONSTANT_ROOT_TWO = 1.414213562373095048801688724209698078;
////constexpr double const      CONSTANT_PI = 3.141592653589793238462643383279502884;
////constexpr long double const CONSTANT_E = 2.718281828459045235360287471352662497;
////constexpr std::string_view  g_root_two = snapdev::floating_point_to_string_literal(CONSTANT_ROOT_TWO);
////constexpr std::string_view  g_pi = snapdev::floating_point_to_string_literal(CONSTANT_PI);
////constexpr std::string_view  g_e = snapdev::floating_point_to_string_literal(CONSTANT_E);
//
//        std::stringstream ds;
//        ds << std::setprecision(12) << CONSTANT_ROOT_TWO;
//        CATCH_REQUIRE(ds.str() == std::string(g_root_two));
//    }
//    CATCH_END_SECTION()
}


// vim: ts=4 sw=4 et
