// Copyright (c) 2018-2026  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Verify that the tokenize_string() function works.
 *
 * This file implements tests for the tokenize_string() function.
 */

// self
//
#include    <snapdev/tokenize_string.h>

#include    "catch_main.h"


// last include
//
#include    <snapdev/poison.h>


namespace
{





}
// no name namespace



CATCH_TEST_CASE("tokenize_string", "[string]")
{
    CATCH_START_SECTION("tokenize_string: empty input non-empty output if keeping empty entries")
    {
        std::vector<std::string> tokens;
        CATCH_REQUIRE(snapdev::tokenize_string(
                  tokens
                , ""
                , " ") == 1);
        CATCH_REQUIRE(tokens.size() == 1);
        CATCH_REQUIRE(tokens[0] == "");
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("tokenize_string: empty input empty output when removing empty entries")
    {
        std::vector<std::string> tokens;
        CATCH_REQUIRE(snapdev::tokenize_string(
                  tokens
                , ""
                , " "
                , true) == 0);
        CATCH_REQUIRE(tokens.size() == 0);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("tokenize_string: keep empty entries")
    {
        std::vector<std::string> tokens;
        CATCH_REQUIRE(snapdev::tokenize_string(
                  tokens
                , "   foo    catch  bar   "
                , " ") == 13);
        CATCH_REQUIRE(tokens.size() == 13);
        CATCH_REQUIRE(tokens[ 0] == "");
        CATCH_REQUIRE(tokens[ 1] == "");
        CATCH_REQUIRE(tokens[ 2] == "");
        CATCH_REQUIRE(tokens[ 3] == "foo");
        CATCH_REQUIRE(tokens[ 4] == "");
        CATCH_REQUIRE(tokens[ 5] == "");
        CATCH_REQUIRE(tokens[ 6] == "");
        CATCH_REQUIRE(tokens[ 7] == "catch");
        CATCH_REQUIRE(tokens[ 8] == "");
        CATCH_REQUIRE(tokens[ 9] == "bar");
        CATCH_REQUIRE(tokens[10] == "");
        CATCH_REQUIRE(tokens[11] == "");
        CATCH_REQUIRE(tokens[12] == "");
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("tokenize_string: remove empty entries")
    {
        std::vector<std::string> tokens;
        CATCH_REQUIRE(snapdev::tokenize_string(
                  tokens
                , "   foo    catch  bar   "
                , " "
                , true) == 3);
        CATCH_REQUIRE(tokens.size() == 3);
        CATCH_REQUIRE(tokens[0] == "foo");
        CATCH_REQUIRE(tokens[1] == "catch");
        CATCH_REQUIRE(tokens[2] == "bar");
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("tokenize_string: comma separated")
    {
        std::vector<std::string> tokens;
        CATCH_REQUIRE(snapdev::tokenize_string(
                  tokens
                , ",,,foo,,,,catch,,bar,,,"
                , ",") == 13);
        CATCH_REQUIRE(tokens.size() == 13);
        CATCH_REQUIRE(tokens[ 0] == "");
        CATCH_REQUIRE(tokens[ 1] == "");
        CATCH_REQUIRE(tokens[ 2] == "");
        CATCH_REQUIRE(tokens[ 3] == "foo");
        CATCH_REQUIRE(tokens[ 4] == "");
        CATCH_REQUIRE(tokens[ 5] == "");
        CATCH_REQUIRE(tokens[ 6] == "");
        CATCH_REQUIRE(tokens[ 7] == "catch");
        CATCH_REQUIRE(tokens[ 8] == "");
        CATCH_REQUIRE(tokens[ 9] == "bar");
        CATCH_REQUIRE(tokens[10] == "");
        CATCH_REQUIRE(tokens[11] == "");
        CATCH_REQUIRE(tokens[12] == "");
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("tokenize_string: comma separated, remove empty")
    {
        std::vector<std::string> tokens;
        CATCH_REQUIRE(snapdev::tokenize_string(
                  tokens
                , ",,, foo ,,,,catch  ,,bar ,,,"
                , ","
                , true) == 3);
        CATCH_REQUIRE(tokens.size() == 3);
        CATCH_REQUIRE(tokens[0] == " foo ");
        CATCH_REQUIRE(tokens[1] == "catch  ");
        CATCH_REQUIRE(tokens[2] == "bar ");
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("tokenize_string: comma separated, remove empty and trim")
    {
        std::vector<std::string> tokens;
        CATCH_REQUIRE(snapdev::tokenize_string(
                  tokens
                , ", ,, foo ,,    ,,catch  ,,bar ,,,  "
                , ","
                , true
                , " ") == 3);
        CATCH_REQUIRE(tokens.size() == 3);
        CATCH_REQUIRE(tokens[0] == "foo");
        CATCH_REQUIRE(tokens[1] == "catch");
        CATCH_REQUIRE(tokens[2] == "bar");
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("tokenize_string: arrow (string) separated, remove empty and trim")
    {
        std::vector<std::string> tokens;
        CATCH_REQUIRE(snapdev::tokenize_string(
                  tokens
                , "a => b => c => d"
                , "=>"
                , true
                , " "
                , snapdev::string_predicate<decltype(tokens)>) == 4);
        CATCH_REQUIRE(tokens.size() == 4);
        CATCH_REQUIRE(tokens[0] == "a");
        CATCH_REQUIRE(tokens[1] == "b");
        CATCH_REQUIRE(tokens[2] == "c");
        CATCH_REQUIRE(tokens[3] == "d");
    }
    CATCH_END_SECTION()
}

//template<class ContainerT, typename PredicateT = decltype(character_predicate<ContainerT>)>
//size_t tokenize_string(ContainerT & tokens
//                     , typename ContainerT::value_type const & str
//                     , typename ContainerT::value_type const & delimiters
//                     , bool const trim_empty = false
//                     , typename ContainerT::value_type const & trim_string = typename ContainerT::value_type()
//                     , PredicateT compare_function = &character_predicate<ContainerT>)



// vim: ts=4 sw=4 et
