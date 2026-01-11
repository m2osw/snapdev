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
 * \brief Verify that the is_string_literal works.
 *
 * This file makes sure we can compile with the is_string_literal and
 * all the different type of strings.
 */

// self
//
#include    <snapdev/is_string_literal.h>

#include    "catch_main.h"


// last include
//
#include    <snapdev/poison.h>



namespace
{


constexpr char const            g_char_literal[] = "it is!";
constexpr wchar_t const         g_wchar_literal[] = L"it is!";
constexpr char8_t const         g_char8_literal[] = u8"it is!";
constexpr char16_t const        g_char16_literal[] = u"it is!";
constexpr char32_t const        g_char32_literal[] = U"it is!";


} // no name namespace



CATCH_TEST_CASE("is_string_literal", "[string]")
{
    CATCH_START_SECTION("is_string_literal: check all string literals inline")
    {
        CATCH_REQUIRE(snapdev::is_a_string_literal("it is!"));        // char
        CATCH_REQUIRE(snapdev::is_a_string_literal(L"it is!"));       // wchar_t
        CATCH_REQUIRE(snapdev::is_a_string_literal(u8"it is!"));      // char8_t
        CATCH_REQUIRE(snapdev::is_a_string_literal(u"it is!"));       // char16_t
        CATCH_REQUIRE(snapdev::is_a_string_literal(U"it is!"));       // char32_t

        // a way to test other types to make sure it actually works
        //char const * const s = "blah";
        //CATCH_REQUIRE(snapdev::is_a_string_literal(s));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("is_string_literal: check all constexpr string literals")
    {
        CATCH_REQUIRE(snapdev::is_a_string_literal(g_char_literal));    // char
        CATCH_REQUIRE(snapdev::is_a_string_literal(g_wchar_literal));   // wchar_t
        CATCH_REQUIRE(snapdev::is_a_string_literal(g_char8_literal));   // char8_t
        CATCH_REQUIRE(snapdev::is_a_string_literal(g_char16_literal));  // char16_t
        CATCH_REQUIRE(snapdev::is_a_string_literal(g_char32_literal));  // char32_t
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
