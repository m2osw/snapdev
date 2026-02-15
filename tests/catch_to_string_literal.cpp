// Copyright (c) 2011-2026  Made to Order Software Corp.  All Rights Reserved
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
constexpr std::string_view  g_decimal_zero = snapdev::integer_to_string_literal<0>.data(); // a special case
constexpr std::string_view  g_decimal_minus_one = snapdev::integer_to_string_literal<-1>.data();
constexpr int const         LOCAL_PORT = 9123;
constexpr std::string_view  g_port_decimal = snapdev::integer_to_string_literal<LOCAL_PORT>.data();
constexpr std::string_view  g_port_decimal_explicit = snapdev::integer_to_string_literal<LOCAL_PORT, 10>.data();
constexpr std::string_view  g_port_hexadecimal = snapdev::integer_to_string_literal<LOCAL_PORT, 16>.data();
constexpr std::string_view  g_port_hexadecimal_uppercase = snapdev::integer_to_string_literal<LOCAL_PORT, 16, true>.data();
constexpr std::string_view  g_port_octal = snapdev::integer_to_string_literal<LOCAL_PORT, 8>.data();
constexpr std::string_view  g_port_binary = snapdev::integer_to_string_literal<LOCAL_PORT, 2>.data();

// TODO: we need to check all the integer types (char, short, int, long, long long and unsigned counter parts and special types supported by g++ a.k.a. __int128)

constexpr std::string_view  g_float_zero = snapdev::floating_point_to_string_literal<0.0f>.data();
constexpr std::string_view  g_double_zero = snapdev::floating_point_to_string_literal<0.0>.data();
constexpr std::string_view  g_double_double_zero = snapdev::floating_point_to_string_literal<0.0l>.data();
constexpr std::string_view  g_float_minus_one = snapdev::floating_point_to_string_literal<-1.0f>.data();
constexpr std::string_view  g_double_minus_one = snapdev::floating_point_to_string_literal<-1.0>.data();
constexpr std::string_view  g_double_double_minus_one = snapdev::floating_point_to_string_literal<-1.0l>.data();
constexpr std::string_view  g_double_value = snapdev::floating_point_to_string_literal<-1008410.000456>.data();
constexpr float const       CONSTANT_ROOT_TWO = 1.414213562373095048801688724209698078;
constexpr double const      CONSTANT_PI = 3.141592653589793238462643383279502884;
constexpr long double const CONSTANT_E = 2.718281828459045235360287471352662497;
constexpr std::string_view  g_root_two = snapdev::floating_point_to_string_literal<CONSTANT_ROOT_TWO>.data();
constexpr std::string_view  g_pi = snapdev::floating_point_to_string_literal<CONSTANT_PI>.data();
constexpr std::string_view  g_e = snapdev::floating_point_to_string_literal<CONSTANT_E>.data();

}


CATCH_TEST_CASE("to_string_literal", "[string]")
{
    CATCH_START_SECTION("integer_to_string_literal: verify integral literals")
    {
        CATCH_REQUIRE("0" == std::string(g_decimal_zero));
        CATCH_REQUIRE("-1" == std::string(g_decimal_minus_one));

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

    CATCH_START_SECTION("floating_point_to_string_literal: verify float literals")
    {
        CATCH_REQUIRE("0.000000" == g_float_zero);
        CATCH_REQUIRE("0.000000000000000" == g_double_zero);
        CATCH_REQUIRE("0.000000000000000000" == g_double_double_zero);
        CATCH_REQUIRE("-1.000000" == g_float_minus_one);
        CATCH_REQUIRE("-1.000000000000000" == g_double_minus_one);
        CATCH_REQUIRE("-1.000000000000000000" == g_double_double_minus_one);

        std::stringstream value;
        value << std::setprecision(22) << -1008410.000456;
        CATCH_REQUIRE(value.str() == g_double_value);

        std::stringstream root_two;
        root_two << std::setprecision(7) << CONSTANT_ROOT_TWO;
        CATCH_REQUIRE(root_two.str() == std::string(g_root_two));

        std::stringstream pi;
        pi << std::setprecision(16) << CONSTANT_PI;
        CATCH_REQUIRE(pi.str() == std::string(g_pi));

        std::stringstream e;
        e << std::setprecision(19) << CONSTANT_E;
        CATCH_REQUIRE(e.str() == std::string(g_e));

// we also want to check every single function validity; probably in a separate test
//
//snapdev::detail::floating_point_to_string_literal_impl<CONSTANT_ROOT_TWO, char> r;
//std::cout << "r whole() = " << r.whole() << "\n";
//std::cout << "r fractional_length() = " << r.fractional_length() << "\n";
//std::cout << "r frac() = " << r.frac() << "\n";
//std::cout << "r BUFFER_SIZE = " << r.BUFFER_SIZE << "\n";
//std::cout << "r length() = " << r.length() << "\n";
//std::cout << "r size() = " << r.size() << "\n";
//char const * ptr = r;
//for(std::size_t i(0); i < r.size(); ++i)
//{
//std::cout << "r char[" << i << "] = " << static_cast<int>(ptr[i]) << "\n";
//}

    }
    CATCH_END_SECTION()
}


// vim: ts=4 sw=4 et
