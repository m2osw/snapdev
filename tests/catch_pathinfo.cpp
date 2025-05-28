// Copyright (c) 2021-2025  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Verify that the path functions work.
 *
 * This file implements tests for the pathinfo function.
 */

// self
//
#include    <snapdev/pathinfo.h>

#include    "catch_main.h"


// C++ lib
//
#include    <set>


// last include
//
#include    <snapdev/poison.h>




CATCH_TEST_CASE("pathinfo_replace_suffix", "[filename][pathinfo]")
{
    CATCH_START_SECTION("pathinfo_replace_suffix: replace existing suffix")
    {
        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "/full/path/example/pathinfo.cpp"
                , ".cpp"
                , ".h") == "/full/path/example/pathinfo.h");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "/full/path/example/pathinfo.h"
                , ".h"
                , ".cpp") == "/full/path/example/pathinfo.cpp");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "relative/path/pathinfo.cpp"
                , ".cpp"
                , ".h") == "relative/path/pathinfo.h");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "relative/path/pathinfo.h"
                , ".h"
                , ".cpp") == "relative/path/pathinfo.cpp");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "no-path.cpp"
                , ".cpp"
                , ".h") == "no-path.h");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "no-path.h"
                , ".h"
                , ".cpp") == "no-path.cpp");
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("pathinfo_replace_suffix: replace non-existant suffix")
    {
        std::string full_path_c("/full/path/example/pathinfo.c");
        std::string full_path_hpp("/full/path/example/pathinfo.hpp");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix(
                  full_path_c
                , ".cpp"
                , ".h") == "/full/path/example/pathinfo.c.h");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix(
                  full_path_hpp
                , ".h"
                , ".cpp") == "/full/path/example/pathinfo.hpp.cpp");

        std::string relative_path_c("relative/path/pathinfo.c");
        std::string relative_path_hpp("relative/path/pathinfo.hpp");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix(
                  relative_path_c
                , ".cpp"
                , ".h") == "relative/path/pathinfo.c.h");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix(
                  relative_path_hpp
                , ".h"
                , ".cpp") == "relative/path/pathinfo.hpp.cpp");

        std::string no_path_c("no-path.c");
        std::string no_path_hpp("no-path.hpp");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix(
                  no_path_c
                , ".cpp"
                , ".h") == "no-path.c.h");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix(
                  no_path_hpp
                , ".h"
                , ".cpp") == "no-path.hpp.cpp");
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("pathinfo_replace_suffix: remove suffix when present")
    {
        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "/full/path/example/pathinfo.cpp"
                , ".cpp") == "/full/path/example/pathinfo");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "/full/path/example/pathinfo.h"
                , ".h") == "/full/path/example/pathinfo");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "relative/path/pathinfo.cpp"
                , ".cpp") == "relative/path/pathinfo");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "relative/path/pathinfo.h"
                , ".h") == "relative/path/pathinfo");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "no-path.cpp"
                , ".cpp") == "no-path");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "no-path.h"
                , ".h") == "no-path");
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("pathinfo_replace_suffix: remove suffix when absent")
    {
        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "/full/path/example/pathinfo.c"
                , ".cpp") == "/full/path/example/pathinfo.c");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "/full/path/example/pathinfo.hpp"
                , ".h") == "/full/path/example/pathinfo.hpp");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "relative/path/pathinfo.c"
                , ".cpp") == "relative/path/pathinfo.c");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "relative/path/pathinfo.hpp"
                , ".h") == "relative/path/pathinfo.hpp");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "no-path.c"
                , ".cpp") == "no-path.c");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "no-path.hpp"
                , ".h") == "no-path.hpp");
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("pathinfo_replace_suffix: do nothing if non-existant suffix")
    {
        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "/full/path/example/pathinfo.c"
                , ".cpp"
                , ".h"
                , true) == "/full/path/example/pathinfo.c");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "/full/path/example/pathinfo.c"
                , ".cpp"
                , ""
                , true) == "/full/path/example/pathinfo.c");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "/full/path/example/pathinfo.hpp"
                , ".h"
                , ".cpp"
                , true) == "/full/path/example/pathinfo.hpp");

        CATCH_REQUIRE(snapdev::pathinfo::replace_suffix<std::string>(
                  "/full/path/example/pathinfo.hpp"
                , ".h"
                , std::string()
                , true) == "/full/path/example/pathinfo.hpp");
    }
    CATCH_END_SECTION()
}


CATCH_TEST_CASE("pathinfo_canonicalize", "[filename][pathinfo]")
{
    CATCH_START_SECTION("pathinfo_canonicalize: canonicalize paths")
    {
        char const * to_canonicalize[] =
        {
            "", "", ".",

            "/", "/", "/",

            "/full/path", "", "/full/path",
            "///full//path/", "", "/full/path",

            "relative///path", "and.this", "relative/path/and.this",
            "relative///path/", "and.this", "relative/path/and.this",
            "relative///path", "/and.this", "relative/path/and.this",

            "relative///with///", "///more-path///", "relative/with/more-path",
        };

        for(std::size_t idx(0); idx < std::size(to_canonicalize) / 3; idx += 3)
        {
            CATCH_REQUIRE(snapdev::pathinfo::canonicalize(
                      to_canonicalize[idx + 0]
                    , to_canonicalize[idx + 1]) == to_canonicalize[idx + 2]);
        }
    }
    CATCH_END_SECTION()
}


CATCH_TEST_CASE("pathinfo_has_pattern", "[filename][pathinfo]")
{
    CATCH_START_SECTION("pathinfo_has_pattern: has pattern function")
    {
        struct test_data
        {
            char const *        f_path = nullptr;
            int                 f_true = 0;
        };
        constexpr test_data const patterns[] =
        {
            // no pattern
            { ".",                              0x00 },
            { "without-pattern",                0x00 },
            { "regular/path/without/pattern",   0x00 },
            { "path/../without/../pattern",     0x00 },

            // *
            { "path/*/with/pattern",            0xFF },
            { "path/\\*/with/pattern",          0x55 },

            // ?
            { "path/?/with/pattern",            0xFF },
            { "path/\\?/with/pattern",          0x55 },

            // [...]
            { "path/[a-z]/with/pattern",        0xFF },
            { "path/[a-\\]/with/pattern",       0xFF },
            { "path/\\[a-z]/with/pattern",      0x55 },
            { "path/\\[a-z*]/with/pattern",     0xFF },
            { "path/\\[a-z?]/with/pattern",     0xFF },

            // {...}
            { "path/{abc,def}/with/pattern",    0xCC },
            { "path/\\{abc,dev}/with/pattern",  0x44 },
            { "path/\\{abc,*dev}/with/pattern", 0xFF },
            { "path/\\{abc?,dev}/with/pattern", 0xFF },

            // [*?+@!](...)
            { "path/*(abc,def)/with/pattern",   0xFF },
            { "path/?(abc,dev)/with/pattern",   0xFF },
            { "path/+(abc,dev)/with/pattern",   0xF0 },
            { "path/@(abc,dev)/with/pattern",   0xF0 },
            { "path/!(abc,dev)/with/pattern",   0xF0 },
            { "path/+(a*bc,dev)/with/pattern",  0xFF },
            { "path/@(ab*c,dev)/with/pattern",  0xFF },
            { "path/!(abc*,dev)/with/pattern",  0xFF },
            { "path/+(a?bc,dev)/with/pattern",  0xFF },
            { "path/@(ab?c,dev)/with/pattern",  0xFF },
            { "path/!(abc?,dev)/with/pattern",  0xFF },
            { "path/\\*(abc,def)/with/pattern", 0x55 },
            { "path/\\?(abc,dev)/with/pattern", 0x55 },
            { "path/\\+(abc,dev)/with/pattern", 0x50 },
            { "path/\\@(abc,dev)/with/pattern", 0x50 },
            { "path/\\!(abc,dev)/with/pattern", 0x50 },
            { "path/*\\(abc,def)/with/pattern", 0xFF },
            { "path/?\\(abc,dev)/with/pattern", 0xFF },
            { "path/+\\(abc,dev)/with/pattern", 0x00 },
            { "path/@\\(abc,dev)/with/pattern", 0x00 },
            { "path/!\\(abc,dev)/with/pattern", 0x00 },
        };

        for(std::size_t idx(0); idx < std::size(patterns); ++idx)
        {
            for(int mask(0); mask < 8; ++mask)
            {
                bool const result((patterns[idx].f_true & (1 << mask)) != 0);
//std::cerr << "--- idx: " << idx << " path: \"" << patterns[idx].f_path << "\" mask: " << mask << " result: " << std::boolalpha << result << "\n";
                CATCH_REQUIRE(snapdev::pathinfo::has_pattern(
                          patterns[idx].f_path
                        , (mask & 1) != 0
                        , (mask & 2) != 0
                        , (mask & 4) != 0) == result);
            }
        }
    }
    CATCH_END_SECTION()
}


CATCH_TEST_CASE("pathinfo_is_child_path", "[filename][pathinfo]")
{
    CATCH_START_SECTION("pathinfo_is_child_path: is child path function")
    {
        struct test_data
        {
            char const *        f_parent = nullptr;
            char const *        f_child = nullptr;
            bool                f_equal = true;
            bool                f_result = false;
        };
        constexpr test_data const parent_child[] =
        {
            // not a child / mismatch
            {
                ".",
                "..",
                true,
                false,
            },
            {
                ".",
                "..",
                false,
                false,
            },
            {
                "/var",
                "/usr/share/snapdev",
                true,
                false,
            },
            {
                "/var",
                "/usr/share/snapdev",
                false,
                false,
            },
            {
                "/var",
                "Desktop",
                true,
                false,
            },
            {
                "/var",
                "Desktop",
                false,
                false,
            },
            {
                "",
                "/usr/share/snapdev",
                true,
                false,
            },
            {
                "/usr/share",
                "",
                true,
                false,
            },

            // child, not equal
            {
                "/var",
                "/var/lib/snapdev",
                true,
                true,
            },
            {
                "/var",
                "/var//lib/snapdev",
                true,
                true,
            },
            {
                "//var",
                "/var/lib/snapdev/",
                true,
                true,
            },
            {
                "//var//",
                "/var/lib//snapdev/",
                true,
                true,
            },
            {
                "/var",
                "/var/lib/snapdev",
                false,
                true,
            },
            {
                "/var",
                "/var//lib/snapdev",
                false,
                true,
            },
            {
                "//var",
                "/var/lib/snapdev/",
                false,
                true,
            },
            {
                "//var//",
                "/var/lib//snapdev/",
                false,
                true,
            },

            // child, equal
            {
                "/var",
                "/var",
                true,
                true,
            },
            {
                "/var",
                "/var",
                false,
                false,
            },
            {
                "/var/",
                "/var",
                true,
                true,
            },
            {
                "/var/",
                "/var",
                false,
                false,
            },
            {
                "/var",
                "/var/",
                true,
                true,
            },
            {
                "/var",
                "/var/",
                false,
                false,
            },
            {
                "",
                "",
                true,
                true,
            },
            {
                "",
                "",
                false,
                false,
            },
        };

        for(std::size_t idx(0); idx < std::size(parent_child); ++idx)
        {
            bool const result(snapdev::pathinfo::is_child_path(
                      parent_child[idx].f_parent
                    , parent_child[idx].f_child
                    , parent_child[idx].f_equal
                ));

//std::cout << "--- idx: " << idx
//          << " path: \"" << parent_child[idx].f_parent
//          << "\" child: " << parent_child[idx].f_child
//          << " equal: " << std::boolalpha << parent_child[idx].f_equal
//          << " expected result: " << std::boolalpha << parent_child[idx].f_result
//          << " result: " << std::boolalpha << result
//          << "\n";
            CATCH_REQUIRE(result == parent_child[idx].f_result);
        }
    }
    CATCH_END_SECTION()
}


CATCH_TEST_CASE("pathinfo_is_absolute", "[filename][pathinfo]")
{
    CATCH_START_SECTION("pathinfo_is_absolute: check absolute and relative paths")
    {
        struct path_expected_t
        {
            char const * const  f_path = nullptr;
            bool                f_absolute = false;
        };
        path_expected_t paths[] = {
            { "/absolute",                  true  },
            { "/the/length/has/no/effect",  true  },
            { "/",                          true  },
            { "relative",                   false },
            { ".",                          false },
            { "",                           false },
        };
        for(auto const & p : paths)
        {
            CATCH_REQUIRE(snapdev::pathinfo::is_absolute(p.f_path) == p.f_absolute);
            CATCH_REQUIRE(snapdev::pathinfo::is_relative(p.f_path) == !p.f_absolute);
        }
    }
    CATCH_END_SECTION()
}


CATCH_TEST_CASE("pathinfo_relative", "[filename][pathinfo]")
{
    CATCH_START_SECTION("pathinfo_relative: verify that path gets properly changed")
    {
        struct path_expected_t
        {
            char const * const  f_base = nullptr;
            char const * const  f_path = nullptr;
            char const * const  f_result = nullptr;
        };
        path_expected_t paths[] = {
            {
                "/a/b/c/d/e",
                "/a/b/c/g/h/i",
                "../../g/h/i",
            },
            {
                "/a/b/c/d/e/f",
                "/a/b/c/g/h/i",
                "../../../g/h/i",
            },
            {
                "/a/b/c/d/e/f",
                "/x/y/z/g/h/i",
                "../../../../../../x/y/z/g/h/i",
            },
            {
                "a/b/c/d/e/f",
                "/x/y/z/g/h/i",
                "",
            },
            {
                "/a/b/c/d/e/f",
                "x/y/z/g/h/i",
                "",
            },
        };
        for(auto const & p : paths)
        {
            CATCH_REQUIRE(snapdev::pathinfo::relative_path(p.f_base, p.f_path) == p.f_result);
        }
    }
    CATCH_END_SECTION()
}


CATCH_TEST_CASE("pathinfo_realpath", "[filename][pathinfo]")
{
    CATCH_START_SECTION("pathinfo_realpath: make sure the realpath works")
    {
        std::string error_msg;
        std::string const cwd(snapdev::pathinfo::getcwd(error_msg));
        CATCH_REQUIRE(!cwd.empty());
        CATCH_REQUIRE(error_msg.empty());
        CATCH_REQUIRE(snapdev::pathinfo::realpath(".", error_msg) == cwd);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("pathinfo_realpath: realpath fails for non-existant files")
    {
        std::string error_msg;
        CATCH_REQUIRE(snapdev::pathinfo::realpath("/this/is/not/valid", error_msg) == std::string());
        CATCH_REQUIRE(error_msg == "realpath(\"/this/is/not/valid\") could not find the specified file.");
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("pathinfo_realpath: realpath fails for file instead of directory")
    {
        std::string error_msg;
        CATCH_REQUIRE(snapdev::pathinfo::realpath("/bin/ls/invalid", error_msg) == std::string());
        CATCH_REQUIRE(error_msg == "realpath(\"/bin/ls/invalid\") found a file instead of a directory within the path.");
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
