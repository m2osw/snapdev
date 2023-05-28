// Copyright (c) 2023  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Verify that the tokenize_format functions & traits work.
 *
 * This file implements tests for the tokenize_format functions and
 * traits for printf(3) and strftime(3).
 */

// self
//
#include    <snapdev/tokenize_format.h>

#include    "catch_main.h"


// last include
//
#include    <snapdev/poison.h>


namespace
{



constexpr char const * const g_printf_letters = "diouxXeEfFgGaAcsCSpnm";


struct printf_size_t
{
    char const * const              f_flag = nullptr;
    snapdev::format_flag_t const    f_length = snapdev::FORMAT_FLAG_NONE;
};

constexpr printf_size_t const g_printf_sizes[] = {
    {
        "",
        snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_INT,
    },
    {
        "hh",
        snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_CHAR,
    },
    {
        "h",
        snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_SHORT,
    },
    {
        "l",
        snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_LONG,
    },
    {
        "ll",
        snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_LONG_LONG,
    },
    {
        "q",
        snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_LONG_LONG,
    },
    {
        "L",
        snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_LONG_DOUBLE,
    },
    {
        "j",
        snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_INTMAX_T,
    },
    {
        "z",
        snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_SIZE_T,
    },
    {
        "Z",
        snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_SIZE_T,
    },
    {
        "t",
        snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_PTRDFF_T,
    },
};



struct printf_result_t
{
    snapdev::format_error_set_t     f_errors = {};
    char const * const              f_string = nullptr;
    snapdev::format_flag_t const    f_flags = snapdev::FORMAT_FLAG_NONE;
    int                             f_width = snapdev::format_item<char>::NUMBER_UNDEFINED;
    int                             f_precision = snapdev::format_item<char>::NUMBER_UNDEFINED;
    int                             f_position = snapdev::format_item<char>::NUMBER_UNDEFINED;
    char                            f_format = '\0';
};

struct printf_formats_t
{
    char const * const              f_format_string = nullptr;
    std::list<printf_result_t>      f_results = {};
};

printf_formats_t const g_printf_formats[] =
{
    {
        .f_format_string = "Data Driven %% Tests",
        .f_results = {
            {
                .f_string = "Data Driven ",
            },
            {
                .f_string = "%%",
            },
            {
                .f_string = " Tests",
            },
        },
    },
    {
        .f_format_string = "Position %32$i Only",
        .f_results = {
            {
                .f_string = "Position ",
            },
            {
                .f_string = "%32$i",  // for now this keeps the original...
                .f_position = 32,
                .f_format = 'd',
            },
            {
                .f_string = " Only",
            },
        },
    },
    {
        .f_format_string = "Width %72d Only",
        .f_results = {
            {
                .f_string = "Width ",
            },
            {
                .f_string = "%72d",
                .f_width = 72,
                .f_format = 'd',
            },
            {
                .f_string = " Only",
            },
        },
    },
    {
        .f_format_string = "Precision %l.l23x Only",
        .f_results = {
            {
                .f_string = "Precision ",
            },
            {
                .f_string = "%l.l23x",
                .f_flags = snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_LONG_LONG,
                .f_precision = 23,
                .f_format = 'x',
            },
            {
                .f_string = " Only",
            },
        },
    },
    {
        .f_format_string = "Width/Precision/Position %4$72.41lX Mix",
        .f_results = {
            {
                .f_string = "Width/Precision/Position ",
            },
            {
                .f_string = "%4$72.41lX",
                .f_flags = snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_LONG,
                .f_width = 72,
                .f_precision = 41,
                .f_position = 4,
                .f_format = 'X',
            },
            {
                .f_string = " Mix",
            },
        },
    },
    {
        .f_format_string = "Width/Precision %*.h*u Dynamic",
        .f_results = {
            {
                .f_string = "Width/Precision ",
            },
            {
                .f_string = "%*.h*u",
                .f_flags = snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_SHORT,
                .f_width = 0,
                .f_precision = 0,
                .f_format = 'u',
            },
            {
                .f_string = " Dynamic",
            },
        },
    },
    {
        .f_format_string = "Position/Width/Precision %h*3$.*5$h1$o Ultra Dynamic",
        .f_results = {
            {
                .f_string = "Position/Width/Precision ",
            },
            {
                .f_string = "%h*3$.*5$h1$o",
                .f_flags = snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_CHAR,
                .f_width = -3,
                .f_precision = -5,
                .f_position = 1,
                .f_format = 'o',
            },
            {
                .f_string = " Ultra Dynamic",
            },
        },
    },
    {
        .f_format_string = "Space Sign %7$ q*8$.*2$f Flag",
        .f_results = {
            {
                .f_string = "Space Sign ",
            },
            {
                .f_string = "%7$ q*8$.*2$f",
                .f_flags = snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_LONG_LONG
                         | snapdev::printf_flag_traits<char>::FORMAT_FLAG_SPACE_SIGN,
                .f_width = -8,
                .f_precision = -2,
                .f_position = 7,
                .f_format = 'f',
            },
            {
                .f_string = " Flag",
            },
        },
    },
    {
        .f_format_string = "Left Adjusted %10000$-q*8$.*2$f Flag",
        .f_results = {
            {
                .f_string = "Left Adjusted ",
            },
            {
                .f_string = "%10000$-q*8$.*2$f",
                .f_flags = snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_LONG_LONG
                         | snapdev::printf_flag_traits<char>::FORMAT_FLAG_LEFT_ADJUSTED,
                .f_width = -8,
                .f_precision = -2,
                .f_position = 10000,
                .f_format = 'f',
            },
            {
                .f_string = " Flag",
            },
        },
    },
    {
        .f_format_string = "Alternate Form %1$#*10000$.*5$C Flag",
        .f_results = {
            {
                .f_string = "Alternate Form ",
            },
            {
                .f_string = "%1$#*10000$.*5$C",
                .f_flags = snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_LONG
                         | snapdev::printf_flag_traits<char>::FORMAT_FLAG_ALTERNATE_FORM,
                .f_width = -10000,
                .f_precision = -5,
                .f_position = 1,
                .f_format = 'c',
            },
            {
                .f_string = " Flag",
            },
        },
    },
    {
        .f_format_string = "Show Sign %1$301.*10000$+S Flag",
        .f_results = {
            {
                .f_string = "Show Sign ",
            },
            {
                .f_string = "%1$301.*10000$+S",
                .f_flags = snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_LONG
                         | snapdev::printf_flag_traits<char>::FORMAT_FLAG_SHOW_SIGN,
                .f_width = 301,
                .f_precision = -10000,
                .f_position = 1,
                .f_format = 's',
            },
            {
                .f_string = " Flag",
            },
        },
    },
    {
        .f_format_string = "Grouping %'1$301.*31$S Flag",
        .f_results = {
            {
                .f_string = "Grouping ",
            },
            {
                .f_string = "%'1$301.*31$S",
                .f_flags = snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_LONG
                         | snapdev::printf_flag_traits<char>::FORMAT_FLAG_GROUPING,
                .f_width = 301,
                .f_precision = -31,
                .f_position = 1,
                .f_format = 's',
            },
            {
                .f_string = " Flag",
            },
        },
    },
    {
        .f_format_string = "Alternate Digits %2$307.I*41$zG Flag",
        .f_results = {
            {
                .f_string = "Alternate Digits ",
            },
            {
                .f_string = "%2$307.I*41$zG",
                .f_flags = snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_SIZE_T
                         | snapdev::printf_flag_traits<char>::FORMAT_FLAG_ALTERNATE_DIGITS,
                .f_width = 307,
                .f_precision = -41,
                .f_position = 2,
                .f_format = 'G',
            },
            {
                .f_string = " Flag",
            },
        },
    },
    {
        .f_format_string = "Error %--e Duplicate",
        .f_results = {
            {
                .f_string = "Error ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_DUPLICATE,
                },
                .f_string = "%--e",
                .f_flags = snapdev::printf_flag_traits<char>::FORMAT_FLAG_LEFT_ADJUSTED,
                .f_format = 'e',
            },
            {
                .f_string = " Duplicate",
            },
        },
    },
    {
        .f_format_string = "Error %  E Duplicate",
        .f_results = {
            {
                .f_string = "Error ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_DUPLICATE,
                },
                .f_string = "%  E",
                .f_flags = snapdev::printf_flag_traits<char>::FORMAT_FLAG_SPACE_SIGN,
                .f_format = 'E',
            },
            {
                .f_string = " Duplicate",
            },
        },
    },
    {
        .f_format_string = "Error %##G Duplicate",
        .f_results = {
            {
                .f_string = "Error ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_DUPLICATE,
                },
                .f_string = "%##G",
                .f_flags = snapdev::printf_flag_traits<char>::FORMAT_FLAG_ALTERNATE_FORM,
                .f_format = 'G',
            },
            {
                .f_string = " Duplicate",
            },
        },
    },
    {
        .f_format_string = "Error %++f Duplicate",
        .f_results = {
            {
                .f_string = "Error ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_DUPLICATE,
                },
                .f_string = "%++f",
                .f_flags = snapdev::printf_flag_traits<char>::FORMAT_FLAG_SHOW_SIGN,
                .f_format = 'f',
            },
            {
                .f_string = " Duplicate",
            },
        },
    },
    {
        .f_format_string = "Error %''F Duplicate",
        .f_results = {
            {
                .f_string = "Error ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_DUPLICATE,
                },
                .f_string = "%''F",
                .f_flags = snapdev::printf_flag_traits<char>::FORMAT_FLAG_GROUPING,
                .f_format = 'F',
            },
            {
                .f_string = " Duplicate",
            },
        },
    },
    {
        .f_format_string = "Error %IIA Duplicate",
        .f_results = {
            {
                .f_string = "Error ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_DUPLICATE,
                },
                .f_string = "%IIA",
                .f_flags = snapdev::printf_flag_traits<char>::FORMAT_FLAG_ALTERNATE_DIGITS,
                .f_format = 'A',
            },
            {
                .f_string = " Duplicate",
            },
        },
    },
    {
        .f_format_string = "Unknown Format %h*Y Error",
        .f_results = {
            {
                .f_string = "Unknown Format ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_UNKNOWN,
                },
                .f_string = "%",
                .f_flags = snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_SHORT,
                .f_width = 0,
            },
            {
                .f_string = "h*Y Error",
            },
        },
    },
    {
        .f_format_string = "Duplicated Width %h*h4p Error",
        .f_results = {
            {
                .f_string = "Duplicated Width ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_DUPLICATE,
                },
                .f_string = "%h*h4p",
                .f_flags = snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_CHAR,
                .f_width = 4,
                .f_format = 'p',
            },
            {
                .f_string = " Error",
            },
        },
    },
    {
        .f_format_string = "Duplicated Width %l4l*n Error",
        .f_results = {
            {
                .f_string = "Duplicated Width ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_DUPLICATE,
                },
                .f_string = "%l4l*n",
                .f_flags = snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_LONG_LONG,
                .f_width = 0,
                .f_format = 'n',
            },
            {
                .f_string = " Error",
            },
        },
    },
    {
        .f_format_string = "Mismatch Widths %*4hhp Error",
        .f_results = {
            {
                .f_string = "Mismatch Widths ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_MISMATCH,
                },
                .f_string = "%*4hhp",
                .f_flags = snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_CHAR,
                .f_width = 4,
                .f_format = 'p',
            },
            {
                .f_string = " Error",
            },
        },
    },
    {
        .f_format_string = "Lone $ %L$m Error",
        .f_results = {
            {
                .f_string = "Lone $ ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_SYNTAX,
                },
                .f_string = "%",
                .f_flags = snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_LONG_DOUBLE,
            },
            {
                .f_string = "L$m Error",
            },
        },
    },
    {
        .f_format_string = "Duplicated Width %.h*h4p Error",
        .f_results = {
            {
                .f_string = "Duplicated Width ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_DUPLICATE,
                },
                .f_string = "%.h*h4p",
                .f_flags = snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_CHAR,
                .f_precision = 4,
                .f_format = 'p',
            },
            {
                .f_string = " Error",
            },
        },
    },
    {
        .f_format_string = "Duplicated Width %.l4l*n Error",
        .f_results = {
            {
                .f_string = "Duplicated Width ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_DUPLICATE,
                },
                .f_string = "%.l4l*n",
                .f_flags = snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_LONG_LONG,
                .f_precision = 0,
                .f_format = 'n',
            },
            {
                .f_string = " Error",
            },
        },
    },
    {
        .f_format_string = "Mismatch Widths %.*4hhp Error",
        .f_results = {
            {
                .f_string = "Mismatch Widths ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_MISMATCH,
                },
                .f_string = "%.*4hhp",
                .f_flags = snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_CHAR,
                .f_precision = 4,
                .f_format = 'p',
            },
            {
                .f_string = " Error",
            },
        },
    },
    {
        .f_format_string = "Two Positions %4$1$Zp Error",
        .f_results = {
            {
                .f_string = "Two Positions ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_DUPLICATE,
                },
                .f_string = "%4$1$Zp",
                .f_flags = snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_SIZE_T,
                .f_position = 1,
                .f_format = 'p',
            },
            {
                .f_string = " Error",
            },
        },
    },
    {
        .f_format_string = "End Of String %z*",
        .f_results = {
            {
                .f_string = "End Of String ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_EOS,
                },
                .f_string = "%z*",
                .f_flags = snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_SIZE_T,
                .f_width = 0,
            },
        },
    },
    {
        .f_format_string = "End Of String %.z*",
        .f_results = {
            {
                .f_string = "End Of String ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_EOS,
                },
                .f_string = "%.z*",
                .f_flags = snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_SIZE_T,
                .f_precision = 0,
            },
        },
    },
    {
        .f_format_string = "End Of String %1..5zp",
        .f_results = {
            {
                .f_string = "End Of String ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_DUPLICATE,
                },
                .f_string = "%1..5zp",
                .f_flags = snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_SIZE_T,
                .f_width = 1,
                .f_precision = 5,
                .f_format = 'p',
            },
        },
    },
    {
        .f_format_string = "End Of String %10001Lp",
        .f_results = {
            {
                .f_string = "End Of String ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_OVERFLOW,
                },
                .f_string = "%10001Lp",
                .f_flags = snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_LONG_DOUBLE,
                .f_width = 10000,
                .f_format = 'p',
            },
        },
    },
    {
        .f_format_string = "End Of String %1j*a",
        .f_results = {
            {
                .f_string = "End Of String ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_DUPLICATE,
                },
                .f_string = "%1j*a",
                .f_flags = snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_INTMAX_T,
                .f_width = 0,
                .f_format = 'a',
            },
        },
    },
    {
        .f_format_string = "End Of String %.1j*a",
        .f_results = {
            {
                .f_string = "End Of String ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_DUPLICATE,
                },
                .f_string = "%.1j*a",
                .f_flags = snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_INTMAX_T,
                .f_precision = 0,
                .f_format = 'a',
            },
        },
    },
    {
        .f_format_string = "End Of String %1j*3$A",
        .f_results = {
            {
                .f_string = "End Of String ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_DUPLICATE,
                },
                .f_string = "%1j*3$A",
                .f_flags = snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_INTMAX_T,
                .f_width = -3,
                .f_format = 'A',
            },
        },
    },
    {
        .f_format_string = "End Of String %.1j*3$A",
        .f_results = {
            {
                .f_string = "End Of String ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_DUPLICATE,
                },
                .f_string = "%.1j*3$A",
                .f_flags = snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_INTMAX_T,
                .f_precision = -3,
                .f_format = 'A',
            },
        },
    },
};



}
// no name namespace



CATCH_TEST_CASE("tokenize_format", "[string]")
{
    CATCH_START_SECTION("tokenize_format: printf escape %")
    {
        snapdev::format_item<char>::list_t items(snapdev::tokenize_format<
                  char
                , snapdev::printf_letter_traits<char>
                , snapdev::printf_flag_traits<char>
                , snapdev::printf_number_traits<char>>("Test %% by itself"));

        CATCH_REQUIRE(items.size() == 3);

        auto it(items.begin());

        CATCH_REQUIRE(it->errors().empty());
        CATCH_REQUIRE_FALSE(it->has_errors());
        CATCH_REQUIRE_FALSE(it->has_error(snapdev::format_error_t::FORMAT_ERROR_DUPLICATE));
        CATCH_REQUIRE_FALSE(it->has_error(snapdev::format_error_t::FORMAT_ERROR_EOS));
        CATCH_REQUIRE_FALSE(it->has_error(snapdev::format_error_t::FORMAT_ERROR_MISMATCH));
        CATCH_REQUIRE_FALSE(it->has_error(snapdev::format_error_t::FORMAT_ERROR_OVERFLOW));
        CATCH_REQUIRE_FALSE(it->has_error(snapdev::format_error_t::FORMAT_ERROR_SYNTAX));
        CATCH_REQUIRE_FALSE(it->has_error(snapdev::format_error_t::FORMAT_ERROR_UNKNOWN));
        CATCH_REQUIRE(it->string() == "Test ");
        CATCH_REQUIRE(it->flags() == snapdev::FORMAT_FLAG_NONE);
        for(std::size_t flag(0); flag < sizeof(snapdev::format_flag_t) * CHAR_BIT; ++flag)
        {
            CATCH_REQUIRE_FALSE(it->has_flags(static_cast<snapdev::format_flag_t>(1LL << flag)));
        }
        CATCH_REQUIRE_FALSE(it->has_width());
        CATCH_REQUIRE_FALSE(it->has_precision());
        CATCH_REQUIRE_FALSE(it->has_position());
        CATCH_REQUIRE_FALSE(it->is_format());

        ++it;
        CATCH_REQUIRE_FALSE(it->has_errors());
        CATCH_REQUIRE(it->string() == "%%");
        CATCH_REQUIRE(it->flags() == snapdev::FORMAT_FLAG_NONE);
        CATCH_REQUIRE_FALSE(it->has_width());
        CATCH_REQUIRE_FALSE(it->has_precision());
        CATCH_REQUIRE_FALSE(it->has_position());
        CATCH_REQUIRE_FALSE(it->is_format());

        ++it;
        CATCH_REQUIRE_FALSE(it->has_errors());
        CATCH_REQUIRE(it->string() == " by itself");
        CATCH_REQUIRE(it->flags() == snapdev::FORMAT_FLAG_NONE);
        CATCH_REQUIRE_FALSE(it->has_width());
        CATCH_REQUIRE_FALSE(it->has_precision());
        CATCH_REQUIRE_FALSE(it->has_position());
        CATCH_REQUIRE_FALSE(it->is_format());
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("tokenize_format: printf letters only")
    {
        for(char const * s(g_printf_letters); *s != '\0'; ++s)
        {
            snapdev::format_item<char>::list_t items(snapdev::tokenize_format<
                      char
                    , snapdev::printf_letter_traits<char>
                    , snapdev::printf_flag_traits<char>
                    , snapdev::printf_number_traits<char>>(std::string("Letter %") + *s + " by itself"));

            CATCH_REQUIRE(items.size() == 3);

            auto it(items.begin());

            CATCH_REQUIRE(it->errors().empty());
            CATCH_REQUIRE_FALSE(it->has_errors());
            CATCH_REQUIRE_FALSE(it->has_error(snapdev::format_error_t::FORMAT_ERROR_DUPLICATE));
            CATCH_REQUIRE_FALSE(it->has_error(snapdev::format_error_t::FORMAT_ERROR_EOS));
            CATCH_REQUIRE_FALSE(it->has_error(snapdev::format_error_t::FORMAT_ERROR_MISMATCH));
            CATCH_REQUIRE_FALSE(it->has_error(snapdev::format_error_t::FORMAT_ERROR_OVERFLOW));
            CATCH_REQUIRE_FALSE(it->has_error(snapdev::format_error_t::FORMAT_ERROR_SYNTAX));
            CATCH_REQUIRE_FALSE(it->has_error(snapdev::format_error_t::FORMAT_ERROR_UNKNOWN));
            CATCH_REQUIRE(it->string() == "Letter ");
            CATCH_REQUIRE(it->flags() == snapdev::FORMAT_FLAG_NONE);
            for(std::size_t flag(0); flag < sizeof(snapdev::format_flag_t) * CHAR_BIT; ++flag)
            {
                CATCH_REQUIRE_FALSE(it->has_flags(static_cast<snapdev::format_flag_t>(1LL << flag)));
            }
            CATCH_REQUIRE_FALSE(it->has_width());
            CATCH_REQUIRE_FALSE(it->has_precision());
            CATCH_REQUIRE_FALSE(it->has_position());
            CATCH_REQUIRE_FALSE(it->is_format());

            ++it;
            CATCH_REQUIRE_FALSE(it->has_errors());
            CATCH_REQUIRE(it->string() == std::string("%") + *s);
            char letter(*s);
            switch(letter)
            {
            case 'i':
                letter = 'd';
                break;

            case 'S':
            case 'C':
                CATCH_REQUIRE(it->flags() == snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_LONG);
                letter = letter | 0x20;
                break;

            default:
                CATCH_REQUIRE(it->flags() == snapdev::FORMAT_FLAG_NONE);
                break;

            }
            CATCH_REQUIRE_FALSE(it->has_width());
            CATCH_REQUIRE_FALSE(it->has_precision());
            CATCH_REQUIRE_FALSE(it->has_position());
            CATCH_REQUIRE(it->is_format());
            CATCH_REQUIRE(it->format() == letter);

            ++it;
            CATCH_REQUIRE_FALSE(it->has_errors());
            CATCH_REQUIRE(it->string() == " by itself");
            CATCH_REQUIRE(it->flags() == snapdev::FORMAT_FLAG_NONE);
            CATCH_REQUIRE_FALSE(it->has_width());
            CATCH_REQUIRE_FALSE(it->has_precision());
            CATCH_REQUIRE_FALSE(it->has_position());
            CATCH_REQUIRE_FALSE(it->is_format());
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("tokenize_format: printf size + format")
    {
        for(std::size_t idx(0); idx < std::size(g_printf_sizes); ++idx)
        {
            snapdev::format_item<char>::list_t items(snapdev::tokenize_format<
                      char
                    , snapdev::printf_letter_traits<char>
                    , snapdev::printf_flag_traits<char>
                    , snapdev::printf_number_traits<char>>(std::string("Sized %") + g_printf_sizes[idx].f_flag + "d format"));

            CATCH_REQUIRE(items.size() == 3);

            auto it(items.begin());

            CATCH_REQUIRE(it->errors().empty());
            CATCH_REQUIRE_FALSE(it->has_errors());
            CATCH_REQUIRE(it->string() == "Sized ");
            CATCH_REQUIRE(it->flags() == snapdev::FORMAT_FLAG_NONE);
            CATCH_REQUIRE_FALSE(it->has_width());
            CATCH_REQUIRE_FALSE(it->has_precision());
            CATCH_REQUIRE_FALSE(it->has_position());
            CATCH_REQUIRE_FALSE(it->is_format());

            ++it;
            CATCH_REQUIRE_FALSE(it->has_errors());
            CATCH_REQUIRE(it->string() == std::string("%") + g_printf_sizes[idx].f_flag + "d");
            CATCH_REQUIRE(it->flags() == g_printf_sizes[idx].f_length);
            CATCH_REQUIRE_FALSE(it->has_width());
            CATCH_REQUIRE_FALSE(it->has_precision());
            CATCH_REQUIRE_FALSE(it->has_position());
            CATCH_REQUIRE(it->is_format());
            CATCH_REQUIRE(it->format() == 'd');

            ++it;
            CATCH_REQUIRE_FALSE(it->has_errors());
            CATCH_REQUIRE(it->string() == " format");
            CATCH_REQUIRE(it->flags() == snapdev::FORMAT_FLAG_NONE);
            CATCH_REQUIRE_FALSE(it->has_width());
            CATCH_REQUIRE_FALSE(it->has_precision());
            CATCH_REQUIRE_FALSE(it->has_position());
            CATCH_REQUIRE_FALSE(it->is_format());
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("tokenize_format: printf size defined twice")
    {
        snapdev::format_error_set_t duplicate{ snapdev::format_error_t::FORMAT_ERROR_DUPLICATE };
        int format(rand() % (sizeof(g_printf_letters) - 1));
        for(std::size_t i(0); i < std::size(g_printf_sizes); ++i)
        {
            for(std::size_t j(0); j < std::size(g_printf_sizes); ++j)
            {
                if(strcmp(g_printf_sizes[i].f_flag, "") == 0
                || strcmp(g_printf_sizes[j].f_flag, "") == 0
                || (strcmp(g_printf_sizes[i].f_flag, "h") == 0 && strcmp(g_printf_sizes[j].f_flag, "h") == 0)
                || (strcmp(g_printf_sizes[i].f_flag, "l") == 0 && strcmp(g_printf_sizes[j].f_flag, "l") == 0))
                {
                    // doubling these won't generate an error
                    // and here we are verifying duplicates
                    //
                    continue;
                }

                std::string pattern("%");
                pattern += g_printf_sizes[i].f_flag;
                pattern += g_printf_sizes[j].f_flag;
                pattern += g_printf_letters[format];
//std::cerr << "  --- pattern [" << pattern << "]\n";

                snapdev::format_item<char>::list_t items(snapdev::tokenize_format<
                          char
                        , snapdev::printf_letter_traits<char>
                        , snapdev::printf_flag_traits<char>
                        , snapdev::printf_number_traits<char>>(std::string("Double sized ") + pattern + " format"));

                CATCH_REQUIRE(items.size() == 3);

                auto it(items.begin());

                CATCH_REQUIRE_FALSE(it->has_errors());
                CATCH_REQUIRE(it->string() == "Double sized ");
                CATCH_REQUIRE(it->flags() == snapdev::FORMAT_FLAG_NONE);
                CATCH_REQUIRE_FALSE(it->has_width());
                CATCH_REQUIRE_FALSE(it->has_precision());
                CATCH_REQUIRE_FALSE(it->has_position());
                CATCH_REQUIRE_FALSE(it->is_format());

                ++it;
                CATCH_REQUIRE(it->has_errors());
                CATCH_REQUIRE(it->errors() == duplicate);
                CATCH_REQUIRE(it->string() == pattern);
                switch((g_printf_sizes[i].f_length << 16) | g_printf_sizes[j].f_length)
                {
                case (snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_CHAR << 16) | snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_CHAR:
                case (snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_SHORT << 16) | snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_CHAR:
                case (snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_CHAR << 16) | snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_SHORT:
                case (snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_SHORT << 16) | snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_SHORT:
                    CATCH_REQUIRE(it->flags() == snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_CHAR);
                    break;

                case (snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_LONG << 16) | snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_LONG:
                case (snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_LONG_LONG << 16) | snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_LONG:
                case (snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_LONG_LONG << 16) | snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_LONG_LONG:
                    CATCH_REQUIRE(it->flags() == snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_LONG_LONG);
                    break;

                case (snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_LONG << 16) | snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_LONG_LONG:
                    // "q" does not overwrite by "l"
                    //
                    if(strcmp(g_printf_sizes[j].f_flag, "q") == 0)
                    {
                        CATCH_REQUIRE(it->flags() == snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_LONG);
                    }
                    else
                    {
                        CATCH_REQUIRE(it->flags() == snapdev::printf_flag_traits<char>::FORMAT_FLAG_LENGTH_LONG_LONG);
                    }
                    break;

                default:
                    CATCH_REQUIRE(it->flags() == g_printf_sizes[i].f_length);
                    break;

                }
                CATCH_REQUIRE_FALSE(it->has_width());
                CATCH_REQUIRE_FALSE(it->has_precision());
                CATCH_REQUIRE_FALSE(it->has_position());
                CATCH_REQUIRE(it->is_format());
                switch(g_printf_letters[format])
                {
                case 'i':
                    CATCH_REQUIRE(it->format() == 'd');
                    break;

                case 'S':
                    CATCH_REQUIRE(it->format() == 's');
                    break;

                case 'C':
                    CATCH_REQUIRE(it->format() == 'c');
                    break;

                default:
                    CATCH_REQUIRE(it->format() == g_printf_letters[format]);

                }
                ++it;
                CATCH_REQUIRE_FALSE(it->has_errors());
                CATCH_REQUIRE(it->string() == " format");
                CATCH_REQUIRE(it->flags() == snapdev::FORMAT_FLAG_NONE);
                CATCH_REQUIRE_FALSE(it->has_width());
                CATCH_REQUIRE_FALSE(it->has_precision());
                CATCH_REQUIRE_FALSE(it->has_position());
                CATCH_REQUIRE_FALSE(it->is_format());

                format = (format + 1) % (sizeof(g_printf_letters) - 1);
            }
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("tokenize_format: printf data driven tests")
    {
        for(std::size_t idx(0); idx < std::size(g_printf_formats); ++idx)
        {
            snapdev::format_item<char>::list_t items(snapdev::tokenize_format<
                      char
                    , snapdev::printf_letter_traits<char>
                    , snapdev::printf_flag_traits<char>
                    , snapdev::printf_number_traits<char>>(g_printf_formats[idx].f_format_string));

            CATCH_REQUIRE(items.size() == g_printf_formats[idx].f_results.size());

            auto r(g_printf_formats[idx].f_results.begin());
            for(auto it(items.begin()); it != items.end(); ++it, ++r)
            {
std::cerr << "   +++ parsed [" << g_printf_formats[idx].f_format_string << "] compare item [" << it->string() << "]\n";
if(it->has_errors())
{
for(auto const & e : it->errors())
{
std::cerr << "     >>> error: " << static_cast<int>(e) << "\n";
}
}
                // verify that test data match in size (we already verified
                // the size, but this is better than a SEGV)
                //
                CATCH_REQUIRE(r != g_printf_formats[idx].f_results.end());

                CATCH_REQUIRE(it->errors() == r->f_errors);
                CATCH_REQUIRE(it->string() == r->f_string);
                CATCH_REQUIRE(it->flags() == r->f_flags);
                CATCH_REQUIRE(it->width() == r->f_width);
                CATCH_REQUIRE(it->precision() == r->f_precision);
                CATCH_REQUIRE(it->position() == r->f_position);
                CATCH_REQUIRE(it->format() == r->f_format);
            }
        }
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
