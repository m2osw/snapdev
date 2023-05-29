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
                .f_string = "%",
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


printf_formats_t const g_strftime_formats[] =
{
    {
        .f_format_string = "Data Driven %% Tests",
        .f_results = {
            {
                .f_string = "Data Driven ",
            },
            {
                .f_string = "%",
            },
            {
                .f_string = " Tests",
            },
        },
    },
    {
        .f_format_string = "Simple %a Code",
        .f_results = {
            {
                .f_string = "Simple ",
            },
            {
                .f_string = "%a",
                .f_format = 'a',
            },
            {
                .f_string = " Code",
            },
        },
    },
    {
        .f_format_string = "Week Day %_A and Space Pad",
        .f_results = {
            {
                .f_string = "Week Day ",
            },
            {
                .f_string = "%_A",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_PAD_WITH_SPACES,
                .f_format = 'A',
            },
            {
                .f_string = " and Space Pad",
            },
        },
    },
    {
        .f_format_string = "Abbr Month %-b without pad",
        .f_results = {
            {
                .f_string = "Abbr Month ",
            },
            {
                .f_string = "%-b",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_NO_PAD,
                .f_format = 'b',
            },
            {
                .f_string = " without pad",
            },
        },
    },
    {
        .f_format_string = "Month %0B Zero Pad",
        .f_results = {
            {
                .f_string = "Month ",
            },
            {
                .f_string = "%0B",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_PAD_WITH_ZEROES,
                .f_format = 'B',
            },
            {
                .f_string = " Zero Pad",
            },
        },
    },
    {
        .f_format_string = "Default Date %^c Uppercase",
        .f_results = {
            {
                .f_string = "Default Date ",
            },
            {
                .f_string = "%^c",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_UPPERCASE,
                .f_format = 'c',
            },
            {
                .f_string = " Uppercase",
            },
        },
    },
    {
        .f_format_string = "Century %#C Swap Case",
        .f_results = {
            {
                .f_string = "Century ",
            },
            {
                .f_string = "%#C",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_SWAP_CASE,
                .f_format = 'C',
            },
            {
                .f_string = " Swap Case",
            },
        },
    },
    {
        .f_format_string = "Day %Od Alternate",
        .f_results = {
            {
                .f_string = "Day ",
            },
            {
                .f_string = "%Od",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_MODIFIER,
                .f_format = 'd',
            },
            {
                .f_string = " Alternate",
            },
        },
    },
    {
        .f_format_string = "Date %_^D Pad/Upper",
        .f_results = {
            {
                .f_string = "Date ",
            },
            {
                .f_string = "%_^D",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_PAD_WITH_SPACES
                         | snapdev::strftime_flag_traits<char>::FORMAT_FLAG_UPPERCASE,
                .f_format = 'D',
            },
            {
                .f_string = " Pad/Upper",
            },
        },
    },
    {
        .f_format_string = "Alternate Form %-#Oe Flag",
        .f_results = {
            {
                .f_string = "Alternate Form ",
            },
            {
                .f_string = "%-#Oe",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_NO_PAD
                         | snapdev::strftime_flag_traits<char>::FORMAT_FLAG_SWAP_CASE
                         | snapdev::strftime_flag_traits<char>::FORMAT_FLAG_MODIFIER,
                .f_format = 'e',
            },
            {
                .f_string = " Flag",
            },
        },
    },
    {
        .f_format_string = "ISO 8601 %^0F Date",
        .f_results = {
            {
                .f_string = "ISO 8601 ",
            },
            {
                .f_string = "%^0F",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_PAD_WITH_ZEROES
                         | snapdev::strftime_flag_traits<char>::FORMAT_FLAG_UPPERCASE,
                .f_format = 'F',
            },
            {
                .f_string = " Date",
            },
        },
    },
    {
        .f_format_string = "Year %G Based on Week",
        .f_results = {
            {
                .f_string = "Year ",
            },
            {
                .f_string = "%G",
                .f_format = 'G',
            },
            {
                .f_string = " Based on Week",
            },
        },
    },
    {
        .f_format_string = "Small %g Year",
        .f_results = {
            {
                .f_string = "Small ",
            },
            {
                .f_string = "%g",
                .f_format = 'g',
            },
            {
                .f_string = " Year",
            },
        },
    },
    {
        .f_format_string = "Switch to b %--h and Duplicate",
        .f_results = {
            {
                .f_string = "Switch to b ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_DUPLICATE,
                },
                .f_string = "%--h",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_NO_PAD,
                .f_format = 'b',
            },
            {
                .f_string = " and Duplicate",
            },
        },
    },
    {
        .f_format_string = "Hour %__H and Duplicate",
        .f_results = {
            {
                .f_string = "Hour ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_DUPLICATE,
                },
                .f_string = "%__H",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_PAD_WITH_SPACES,
                .f_format = 'H',
            },
            {
                .f_string = " and Duplicate",
            },
        },
    },
    {
        .f_format_string = "12h %##I and Duplicate",
        .f_results = {
            {
                .f_string = "12h ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_DUPLICATE,
                },
                .f_string = "%##I",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_SWAP_CASE,
                .f_format = 'I',
            },
            {
                .f_string = " and Duplicate",
            },
        },
    },
    {
        .f_format_string = "Year Day %00j and Duplicate",
        .f_results = {
            {
                .f_string = "Year Day ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_DUPLICATE,
                },
                .f_string = "%00j",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_PAD_WITH_ZEROES,
                .f_format = 'j',
            },
            {
                .f_string = " and Duplicate",
            },
        },
    },
    {
        .f_format_string = "24h %^^k and Duplicate",
        .f_results = {
            {
                .f_string = "24h ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_DUPLICATE,
                },
                .f_string = "%^^k",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_UPPERCASE,
                .f_format = 'k',
            },
            {
                .f_string = " and Duplicate",
            },
        },
    },
    {
        .f_format_string = "12h %_0l and Mismatch",
        .f_results = {
            {
                .f_string = "12h ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_MISMATCH,
                },
                .f_string = "%_0l",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_PAD_WITH_SPACES
                         | snapdev::strftime_flag_traits<char>::FORMAT_FLAG_PAD_WITH_ZEROES,
                .f_format = 'l',
            },
            {
                .f_string = " and Mismatch",
            },
        },
    },
    {
        .f_format_string = "Month %0_m Mismatch",
        .f_results = {
            {
                .f_string = "Month ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_MISMATCH,
                },
                .f_string = "%0_m",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_PAD_WITH_SPACES
                         | snapdev::strftime_flag_traits<char>::FORMAT_FLAG_PAD_WITH_ZEROES,
                .f_format = 'm',
            },
            {
                .f_string = " Mismatch",
            },
        },
    },
    {
        .f_format_string = "Minutes %_-M and Mismatch",
        .f_results = {
            {
                .f_string = "Minutes ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_MISMATCH,
                },
                .f_string = "%_-M",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_NO_PAD
                         | snapdev::strftime_flag_traits<char>::FORMAT_FLAG_PAD_WITH_SPACES,
                .f_format = 'M',
            },
            {
                .f_string = " and Mismatch",
            },
        },
    },
    {
        .f_format_string = "Newline %-_n and Mismatch",
        .f_results = {
            {
                .f_string = "Newline ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_MISMATCH,
                },
                .f_string = "%-_n",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_NO_PAD
                         | snapdev::strftime_flag_traits<char>::FORMAT_FLAG_PAD_WITH_SPACES,
                .f_format = 'n',
            },
            {
                .f_string = " and Mismatch",
            },
        },
    },
    {
        .f_format_string = "Nanoseconds %-N (for us)",
        .f_results = {
            {
                .f_string = "Nanoseconds ",
            },
            {
                .f_string = "%-N",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_NO_PAD,
                .f_format = 'N',
            },
            {
                .f_string = " (for us)",
            },
        },
    },
    {
        .f_format_string = "AM/PM %-0p Mismatch",
        .f_results = {
            {
                .f_string = "AM/PM ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_MISMATCH,
                },
                .f_string = "%-0p",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_NO_PAD
                         | snapdev::strftime_flag_traits<char>::FORMAT_FLAG_PAD_WITH_ZEROES,
                .f_format = 'p',
            },
            {
                .f_string = " Mismatch",
            },
        },
    },
    {
        .f_format_string = "am/pm %0-P and Mismatch",
        .f_results = {
            {
                .f_string = "am/pm ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_MISMATCH,
                },
                .f_string = "%0-P",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_NO_PAD
                         | snapdev::strftime_flag_traits<char>::FORMAT_FLAG_PAD_WITH_ZEROES,
                .f_format = 'P',
            },
            {
                .f_string = " and Mismatch",
            },
        },
    },
    {
        .f_format_string = "Time %^#r Upper/Flip",
        .f_results = {
            {
                .f_string = "Time ",
            },
            {
                .f_string = "%^#r",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_UPPERCASE
                         | snapdev::strftime_flag_traits<char>::FORMAT_FLAG_SWAP_CASE,
                .f_format = 'r',
            },
            {
                .f_string = " Upper/Flip",
            },
        },
    },
    {
        .f_format_string = "Time %#^r Flip/Upper",
        .f_results = {
            {
                .f_string = "Time ",
            },
            {
                .f_string = "%#^r",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_UPPERCASE
                         | snapdev::strftime_flag_traits<char>::FORMAT_FLAG_SWAP_CASE,
                .f_format = 'r',
            },
            {
                .f_string = " Flip/Upper",
            },
        },
    },
    {
        .f_format_string = "Basic Time %R",
        .f_results = {
            {
                .f_string = "Basic Time ",
            },
            {
                .f_string = "%R",
                .f_format = 'R',
            },
        },
    },
    {
        .f_format_string = "Seconds %s.%EN Nanoseconds",
        .f_results = {
            {
                .f_string = "Seconds ",
            },
            {
                .f_string = "%s",
                .f_format = 's',
            },
            {
                .f_string = ".",
            },
            {
                .f_string = "%EN",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_EXTENDED,
                .f_format = 'N',
            },
            {
                .f_string = " Nanoseconds",
            },
        },
    },
    {
        .f_format_string = "2-digit seconds %02S no error",
        .f_results = {
            {
                .f_string = "2-digit seconds ",
            },
            {
                .f_string = "%02S",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_PAD_WITH_ZEROES,
                .f_width = 2,
                .f_format = 'S',
            },
            {
                .f_string = " no error",
            },
        },
    },
    {
        .f_format_string = "Tab %t Char",
        .f_results = {
            {
                .f_string = "Tab ",
            },
            {
                .f_string = "%t",
                .f_format = 't',
            },
            {
                .f_string = " Char",
            },
        },
    },
    {
        .f_format_string = "End Of String %_#^",
        .f_results = {
            {
                .f_string = "End Of String ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_EOS,
                },
                .f_string = "%_#^",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_PAD_WITH_SPACES
                         | snapdev::strftime_flag_traits<char>::FORMAT_FLAG_UPPERCASE
                         | snapdev::strftime_flag_traits<char>::FORMAT_FLAG_SWAP_CASE,
            },
        },
    },
    {
        .f_format_string = "Time %^5T and EOS %",
        .f_results = {
            {
                .f_string = "Time ",
            },
            {
                .f_string = "%^5T",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_UPPERCASE,
                .f_width = 5,
                .f_format = 'T',
            },
            {
                .f_string = " and EOS ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_EOS,
                },
                .f_string = "%",
            },
        },
    },
    {
        .f_format_string = "%0u Start Of String",
        .f_results = {
            {
                .f_string = "%0u",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_PAD_WITH_ZEROES,
                .f_format = 'u',
            },
            {
                .f_string = " Start Of String",
            },
        },
    },
    {
        .f_format_string = "%U%10V%w",
        .f_results = {
            {
                .f_string = "%U",
                .f_format = 'U',
            },
            {
                .f_string = "%10V",
                .f_width = 10,
                .f_format = 'V',
            },
            {
                .f_string = "%w",
                .f_format = 'w',
            },
        },
    },
    {
        .f_format_string = "without%Wspaces",
        .f_results = {
            {
                .f_string = "without",
            },
            {
                .f_string = "%W",
                .f_format = 'W',
            },
            {
                .f_string = "spaces",
            },
        },
    },
    {
        .f_format_string = "Unknown %q Error",
        .f_results = {
            {
                .f_string = "Unknown ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_UNKNOWN,
                },
                .f_string = "%",
            },
            {
                .f_string = "q Error",
            },
        },
    },
    {
        .f_format_string = "No %3.4z Separator",
        .f_results = {
            {
                .f_string = "No ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_UNKNOWN,
                },
                .f_string = "%",
                .f_width = 3,
            },
            {
                .f_string = "3.4z Separator",
            },
        },
    },
    {
        .f_format_string = "Date %x Only",
        .f_results = {
            {
                .f_string = "Date ",
            },
            {
                .f_string = "%x",
                .f_format = 'x',
            },
            {
                .f_string = " Only",
            },
        },
    },
    {
        .f_format_string = "Time %X Only",
        .f_results = {
            {
                .f_string = "Time ",
            },
            {
                .f_string = "%X",
                .f_format = 'X',
            },
            {
                .f_string = " Only",
            },
        },
    },
    {
        .f_format_string = "Year %y without century",
        .f_results = {
            {
                .f_string = "Year ",
            },
            {
                .f_string = "%y",
                .f_format = 'y',
            },
            {
                .f_string = " without century",
            },
        },
    },
    {
        .f_format_string = "Full %Y year",
        .f_results = {
            {
                .f_string = "Full ",
            },
            {
                .f_string = "%Y",
                .f_format = 'Y',
            },
            {
                .f_string = " year",
            },
        },
    },
    {
        .f_format_string = "Zone %z (numeric)",
        .f_results = {
            {
                .f_string = "Zone ",
            },
            {
                .f_string = "%z",
                .f_format = 'z',
            },
            {
                .f_string = " (numeric)",
            },
        },
    },
    {
        .f_format_string = "Zone %Z (abbr)",
        .f_results = {
            {
                .f_string = "Zone ",
            },
            {
                .f_string = "%Z",
                .f_format = 'Z',
            },
            {
                .f_string = " (abbr)",
            },
        },
    },
    {
        .f_format_string = "date(1) %+ format",
        .f_results = {
            {
                .f_string = "date(1) ",
            },
            {
                .f_string = "%+",
                .f_format = '+',
            },
            {
                .f_string = " format",
            },
        },
    },
    {
        .f_format_string = "extended %Ec format",
        .f_results = {
            {
                .f_string = "extended ",
            },
            {
                .f_string = "%Ec",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_EXTENDED,
                .f_format = 'c',
            },
            {
                .f_string = " format",
            },
        },
    },
    {
        .f_format_string = "extended %EC format",
        .f_results = {
            {
                .f_string = "extended ",
            },
            {
                .f_string = "%EC",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_EXTENDED,
                .f_format = 'C',
            },
            {
                .f_string = " format",
            },
        },
    },
    {
        .f_format_string = "extended %Ex format",
        .f_results = {
            {
                .f_string = "extended ",
            },
            {
                .f_string = "%Ex",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_EXTENDED,
                .f_format = 'x',
            },
            {
                .f_string = " format",
            },
        },
    },
    {
        .f_format_string = "extended %EX format",
        .f_results = {
            {
                .f_string = "extended ",
            },
            {
                .f_string = "%EX",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_EXTENDED,
                .f_format = 'X',
            },
            {
                .f_string = " format",
            },
        },
    },
    {
        .f_format_string = "extended %Ey format",
        .f_results = {
            {
                .f_string = "extended ",
            },
            {
                .f_string = "%Ey",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_EXTENDED,
                .f_format = 'y',
            },
            {
                .f_string = " format",
            },
        },
    },
    {
        .f_format_string = "extended %EY format",
        .f_results = {
            {
                .f_string = "extended ",
            },
            {
                .f_string = "%EY",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_EXTENDED,
                .f_format = 'Y',
            },
            {
                .f_string = " format",
            },
        },
    },
    {
        .f_format_string = "modified %Od format",
        .f_results = {
            {
                .f_string = "modified ",
            },
            {
                .f_string = "%Od",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_MODIFIER,
                .f_format = 'd',
            },
            {
                .f_string = " format",
            },
        },
    },
    {
        .f_format_string = "modified %Oe format",
        .f_results = {
            {
                .f_string = "modified ",
            },
            {
                .f_string = "%Oe",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_MODIFIER,
                .f_format = 'e',
            },
            {
                .f_string = " format",
            },
        },
    },
    {
        .f_format_string = "modified %OH format",
        .f_results = {
            {
                .f_string = "modified ",
            },
            {
                .f_string = "%OH",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_MODIFIER,
                .f_format = 'H',
            },
            {
                .f_string = " format",
            },
        },
    },
    {
        .f_format_string = "modified %OI format",
        .f_results = {
            {
                .f_string = "modified ",
            },
            {
                .f_string = "%OI",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_MODIFIER,
                .f_format = 'I',
            },
            {
                .f_string = " format",
            },
        },
    },
    {
        .f_format_string = "modified %Om format",
        .f_results = {
            {
                .f_string = "modified ",
            },
            {
                .f_string = "%Om",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_MODIFIER,
                .f_format = 'm',
            },
            {
                .f_string = " format",
            },
        },
    },
    {
        .f_format_string = "modified %OM format",
        .f_results = {
            {
                .f_string = "modified ",
            },
            {
                .f_string = "%OM",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_MODIFIER,
                .f_format = 'M',
            },
            {
                .f_string = " format",
            },
        },
    },
    {
        .f_format_string = "modified %OS format",
        .f_results = {
            {
                .f_string = "modified ",
            },
            {
                .f_string = "%OS",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_MODIFIER,
                .f_format = 'S',
            },
            {
                .f_string = " format",
            },
        },
    },
    {
        .f_format_string = "modified %Ou format",
        .f_results = {
            {
                .f_string = "modified ",
            },
            {
                .f_string = "%Ou",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_MODIFIER,
                .f_format = 'u',
            },
            {
                .f_string = " format",
            },
        },
    },
    {
        .f_format_string = "modified %OU format",
        .f_results = {
            {
                .f_string = "modified ",
            },
            {
                .f_string = "%OU",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_MODIFIER,
                .f_format = 'U',
            },
            {
                .f_string = " format",
            },
        },
    },
    {
        .f_format_string = "modified %OV format",
        .f_results = {
            {
                .f_string = "modified ",
            },
            {
                .f_string = "%OV",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_MODIFIER,
                .f_format = 'V',
            },
            {
                .f_string = " format",
            },
        },
    },
    {
        .f_format_string = "modified %Ow format",
        .f_results = {
            {
                .f_string = "modified ",
            },
            {
                .f_string = "%Ow",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_MODIFIER,
                .f_format = 'w',
            },
            {
                .f_string = " format",
            },
        },
    },
    {
        .f_format_string = "modified %OW format",
        .f_results = {
            {
                .f_string = "modified ",
            },
            {
                .f_string = "%OW",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_MODIFIER,
                .f_format = 'W',
            },
            {
                .f_string = " format",
            },
        },
    },
    {
        .f_format_string = "modified %Oy format",
        .f_results = {
            {
                .f_string = "modified ",
            },
            {
                .f_string = "%Oy",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_MODIFIER,
                .f_format = 'y',
            },
            {
                .f_string = " format",
            },
        },
    },
    {
        .f_format_string = "extended %Ef unknown",
        .f_results = {
            {
                .f_string = "extended ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_UNKNOWN,
                },
                .f_string = "%",
            },
            {
                .f_string = "Ef unknown",
            },
        },
    },
    {
        .f_format_string = "modified %Of unknown",
        .f_results = {
            {
                .f_string = "modified ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_UNKNOWN,
                },
                .f_string = "%",
            },
            {
                .f_string = "Of unknown",
            },
        },
    },
    {
        .f_format_string = "Overflow at end %10001Ey",
        .f_results = {
            {
                .f_string = "Overflow at end ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_OVERFLOW,
                },
                .f_string = "%10001Ey",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_EXTENDED,
                .f_width = 10000,
                .f_format = 'y',
            },
        },
    },
    {
        .f_format_string = "Overflow at end %99999Ey",
        .f_results = {
            {
                .f_string = "Overflow at end ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_OVERFLOW,
                },
                .f_string = "%99999Ey",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_EXTENDED,
                .f_width = 10000,
                .f_format = 'y',
            },
        },
    },
    {
        .f_format_string = "Overflow at end %3472830Ey",
        .f_results = {
            {
                .f_string = "Overflow at end ",
            },
            {
                .f_errors = {
                    snapdev::format_error_t::FORMAT_ERROR_OVERFLOW,
                },
                .f_string = "%3472830Ey",
                .f_flags = snapdev::strftime_flag_traits<char>::FORMAT_FLAG_EXTENDED,
                .f_width = 10000,
                .f_format = 'y',
            },
        },
    },
};



// the full implementation is found in the advgetopt library
//
template<typename _CharT>
class usage_flag_traits
{
public:
    typedef _CharT  char_t;

    static constexpr snapdev::format_flag_t const       FORMAT_FLAG_EXTENDED = 0x0001; // '*'

    static bool is_flag(char_t c, snapdev::format_item<_CharT> & f)
    {
        if(c == '*')
        {
            if(f.has_flags(FORMAT_FLAG_EXTENDED))
            {
                f.add_error(snapdev::format_error_t::FORMAT_ERROR_DUPLICATE);
            }
            f.add_flags(FORMAT_FLAG_EXTENDED);
            return true;
        }
        return false;
    }
};

template<typename _CharT>
class usage_letter_traits
{
public:
    static std::basic_string<_CharT>::size_type is_format(char const * s, snapdev::format_item<_CharT> & f)
    {
        if(s[0] == 'a'
        || s[0] == 'd')
        {
            f.format(s[0]);
            return 1UL;
        }
        f.add_error(snapdev::format_error_t::FORMAT_ERROR_UNKNOWN);
        return 0;
    }
};


printf_formats_t const g_usage_formats[] =
{
    {
        .f_format_string = "Data Driven %% Tests",
        .f_results = {
            {
                .f_string = "Data Driven ",
            },
            {
                .f_string = "%",
            },
            {
                .f_string = " Tests",
            },
        },
    },
    {
        .f_format_string = "Simple %a Code",
        .f_results = {
            {
                .f_string = "Simple ",
            },
            {
                .f_string = "%a",
                .f_format = 'a',
            },
            {
                .f_string = " Code",
            },
        },
    },
    {
        .f_format_string = "Extended %*d Format",
        .f_results = {
            {
                .f_string = "Extended ",
            },
            {
                .f_string = "%*d",
                .f_flags = usage_flag_traits<char>::FORMAT_FLAG_EXTENDED,
                .f_format = 'd',
            },
            {
                .f_string = " Format",
            },
        },
    },
};



}
// no name namespace



CATCH_TEST_CASE("tokenize_format_printf", "[string]")
{
    CATCH_START_SECTION("tokenize_format_printf: escape %")
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
        CATCH_REQUIRE(it->string() == "%");
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

    CATCH_START_SECTION("tokenize_format_printf: letters only")
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

    CATCH_START_SECTION("tokenize_format_printf: size + format")
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

    CATCH_START_SECTION("tokenize_format_printf: size defined twice")
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

    CATCH_START_SECTION("tokenize_format_printf: data driven tests")
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
//std::cerr << "   +++ parsed [" << g_printf_formats[idx].f_format_string << "] compare item [" << it->string() << "]\n";
//if(it->has_errors())
//{
//for(auto const & e : it->errors())
//{
//std::cerr << "     >>> error: " << static_cast<int>(e) << "\n";
//}
//}
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


CATCH_TEST_CASE("tokenize_format_strftime", "[string]")
{
    CATCH_START_SECTION("tokenize_format_strftime: no nanoseconds support")
    {
        snapdev::format_item<char>::list_t items(snapdev::tokenize_format<
                  char
                , snapdev::strftime_letter_traits<char>
                , snapdev::strftime_flag_traits<char>
                , snapdev::strftime_number_traits<char>>("%s.%N"));

        CATCH_REQUIRE(items.size() == 4);

        auto it(items.begin());

        CATCH_REQUIRE(it->string() == "%s");

        ++it;
        CATCH_REQUIRE(it->string() == ".");

        ++it;
        CATCH_REQUIRE(it->string() == "%");
        CATCH_REQUIRE(it->has_errors());
        CATCH_REQUIRE(it->errors() == snapdev::format_error_set_t{ snapdev::format_error_t::FORMAT_ERROR_UNKNOWN });
        CATCH_REQUIRE(it->format() == '\0');

        ++it;
        CATCH_REQUIRE(it->string() == "N");
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("tokenize_format_strftime: no nanoseconds support (extended)")
    {
        snapdev::format_item<char>::list_t items(snapdev::tokenize_format<
                  char
                , snapdev::strftime_letter_traits<char>
                , snapdev::strftime_flag_traits<char>
                , snapdev::strftime_number_traits<char>>("%s.%EN"));

        CATCH_REQUIRE(items.size() == 4);

        auto it(items.begin());

        CATCH_REQUIRE(it->string() == "%s");

        ++it;
        CATCH_REQUIRE(it->string() == ".");

        ++it;
        CATCH_REQUIRE(it->string() == "%");
        CATCH_REQUIRE(it->has_errors());
        CATCH_REQUIRE(it->errors() == snapdev::format_error_set_t{ snapdev::format_error_t::FORMAT_ERROR_UNKNOWN });
        CATCH_REQUIRE(it->format() == '\0');

        ++it;
        CATCH_REQUIRE(it->string() == "EN");
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("tokenize_format_strftime: data driven tests")
    {
        for(std::size_t idx(0); idx < std::size(g_strftime_formats); ++idx)
        {
            snapdev::format_item<char>::list_t items(snapdev::tokenize_format<
                      char
                    , snapdev::strftime_letter_traits<char, true>
                    , snapdev::strftime_flag_traits<char>
                    , snapdev::strftime_number_traits<char>>(g_strftime_formats[idx].f_format_string));

            CATCH_REQUIRE(items.size() == g_strftime_formats[idx].f_results.size());

            auto r(g_strftime_formats[idx].f_results.begin());
            for(auto it(items.begin()); it != items.end(); ++it, ++r)
            {
//std::cerr << "   +++ parsed [" << g_strftime_formats[idx].f_format_string << "] compare item [" << it->string() << "]\n";
//if(it->has_errors())
//{
//for(auto const & e : it->errors())
//{
//std::cerr << "     >>> error: " << static_cast<int>(e) << "\n";
//}
//}
                // verify that test data match in size (we already verified
                // the size, but this is better than a SEGV)
                //
                CATCH_REQUIRE(r != g_strftime_formats[idx].f_results.end());

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


// the following tests a small subset of the advgetopt Usage: ... format
// support to verify the "no number" implementation
//
CATCH_TEST_CASE("tokenize_format_advgetopt", "[string]")
{
    CATCH_START_SECTION("tokenize_format_advgetopt: data driven tests")
    {
        for(std::size_t idx(0); idx < std::size(g_usage_formats); ++idx)
        {
            snapdev::format_item<char>::list_t items(snapdev::tokenize_format<
                      char
                    , usage_letter_traits<char>
                    , usage_flag_traits<char>>(g_usage_formats[idx].f_format_string));

            CATCH_REQUIRE(items.size() == g_usage_formats[idx].f_results.size());

            auto r(g_usage_formats[idx].f_results.begin());
            for(auto it(items.begin()); it != items.end(); ++it, ++r)
            {
//std::cerr << "   +++ parsed [" << g_usage_formats[idx].f_format_string << "] compare item [" << it->string() << "]\n";
//if(it->has_errors())
//{
//for(auto const & e : it->errors())
//{
//std::cerr << "     >>> error: " << static_cast<int>(e) << "\n";
//}
//}
                // verify that test data match in size (we already verified
                // the size, but this is better than a SEGV)
                //
                CATCH_REQUIRE(r != g_usage_formats[idx].f_results.end());

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
