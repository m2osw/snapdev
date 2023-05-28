// Copyright (c) 2022-2023  Made to Order Software Corp.  All Rights Reserved
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
#pragma once

/** \file
 * \brief Tokenize a format string to a list of items.
 *
 * Various functions make use of the format strings a la `printf(3)`.
 * This implements a function that reads the format and generates a
 * list of items describing the format using three traits.
 *
 * The main function supports these traits:
 *
 * \li The trait defining the format introducer (often `%`) and whether it
 * can be doubled ("%%") and/or escaped ("\\\\").
 * \li The trait defining the format flags: characters appearing between
 * the `%` and the final format letter.
 * \li The number trait which defines whether a number can appear between
 * the introducer and the format letter.
 * \li The trait defining the format letters supported in the string.
 *
 * This is useful to tokenize the `printf(3)`, `strftime(3)` and other
 * similar format strings.
 *
 * \note
 * At the moment, format letters and flag characters are limited to ASCII.
 */

// self
//
#include    <snapdev/not_used.h>



// libexcept
//
//#include    <libexcept/exception.h>


// C++
//
//#include    <cmath>
#include    <cstdint>
#include    <limits>
#include    <list>
//#include    <iomanip>
#include    <iostream>
#include    <string>
#include    <set>


// C
//
//#include    <langinfo.h>
//#include    <stdlib.h>
//#include    <string.h>
//#include    <sys/time.h>
//#include    <time.h>




namespace snapdev
{

//DECLARE_MAIN_EXCEPTION(timespec_ex_exception);

//DECLARE_EXCEPTION(timespec_ex_exception, overflow);
//DECLARE_EXCEPTION(timespec_ex_exception, clock_error);


enum class format_error_t : std::uint8_t
{
    FORMAT_ERROR_DUPLICATE,         // something is defined more than once
    FORMAT_ERROR_EOS,               // end of format string found within a format definition
    FORMAT_ERROR_MISMATCH,          // flag not compatible with format or similar
    FORMAT_ERROR_OVERFLOW,          // number (width, precision, position) overflow
    FORMAT_ERROR_SYNTAX,            // something is wrong with the syntax (i.e. '$' without a number in printf format)
    FORMAT_ERROR_UNKNOWN,           // unknown format character
};

typedef std::set<format_error_t>    format_error_set_t;



typedef std::uint32_t           format_flag_t;

constexpr format_flag_t const   FORMAT_FLAG_NONE                = 0x000000;     // no flags / not a flag



template<typename _CharT>
class format_item
{
public:
    typedef _CharT                          char_t;
    typedef std::list<format_item>          list_t;

    static constexpr int                    NUMBER_UNDEFINED = std::numeric_limits<int>::min();

    // errors detected in this format item
    format_error_set_t const & errors() const
    {
        return f_errors;
    }

    bool has_errors() const
    {
        return !f_errors.empty();
    }

    bool has_error(format_error_t e) const
    {
        return f_errors.contains(e);
    }

    void add_error(format_error_t e)
    {
        f_errors.insert(e);
    }

    // string segment
    typename std::basic_string<char_t> string() const
    {
        return f_string;
    }

    void string(std::basic_string<_CharT> const & s)
    {
        f_string = s;
    }

    operator std::string () const
    {
        return f_string;
    }

    // flags
    format_flag_t flags() const
    {
        return f_flags;
    }

    bool has_flags(format_flag_t flags) const
    {
        return (f_flags & flags) != 0;
    }

    void flags(format_flag_t flags)
    {
        f_flags = flags;
    }

    void add_flags(format_flag_t flags)
    {
        f_flags |= flags;
    }

    void set_masked_flags(format_flag_t flags, format_flag_t mask)
    {
        f_flags = (f_flags & ~mask) | flags;
    }

    void remove_flags(format_flag_t flags)
    {
        f_flags &= ~flags;
    }

    // width, precision, position
    int width() const // if negative, we found '*m$'
    {
        return f_width;
    }

    bool has_width() const
    {
        return f_width != NUMBER_UNDEFINED;
    }

    void width(int w)
    {
        f_width = w;
    }

    int precision() const // if negative, we found '.*m$'
    {
        return f_precision;
    }

    bool has_precision() const
    {
        return f_precision != NUMBER_UNDEFINED;
    }

    void precision(int p)
    {
        f_precision = p;
    }

    int position() const // cannot be negative or 0 if defined
    {
        return f_position;
    }

    bool has_position() const
    {
        return f_position != NUMBER_UNDEFINED;
    }

    void position(int position)
    {
        f_position = position;
    }

    // format
    char_t format() const
    {
        return f_format;
    }

    bool is_format() const
    {
        return f_format != static_cast<char_t>('\0');
    }

    void format(char_t f)
    {
        f_format = f;
    }

private:
    format_error_set_t          f_errors = {};
    std::basic_string<char_t>   f_string = std::basic_string<char_t>();
    format_flag_t               f_flags = FORMAT_FLAG_NONE;
    int                         f_width = NUMBER_UNDEFINED;
    int                         f_precision = NUMBER_UNDEFINED;
    int                         f_position = NUMBER_UNDEFINED;
    char_t                      f_format = static_cast<char_t>('\0');
};


template<
      typename _CharT
    , _CharT introducer = '%'
    , _CharT start_enclose = '\0'
    , _CharT end_enclose = '\0'>
class percent_introducer_traits
{
public:
    typedef _CharT char_t;

    static bool is_introducer(char_t c)
    {
        return c == introducer;
    }

    static bool is_start_enclose(char_t c)
    {
        return start_enclose != '\0' && c == start_enclose;
    }

    static bool is_end_enclose(char_t c)
    {
        return end_enclose != '\0' && c == end_enclose;
    }

    static bool double_to_escape()
    {
        return true;
    }

    static bool escape_character(char_t c)
    {
        NOT_USED(c);
        return false;
    }
};



template<typename _CharT>
class no_flag_traits
{
public:
    typedef _CharT  char_t;

    static bool is_flag(char_t c, format_item<_CharT> & f)
    {
        return false;
    }
};



template<typename _CharT>
class printf_flag_traits
{
public:
    typedef _CharT  char_t;

    static constexpr format_flag_t const       FORMAT_FLAG_ALTERNATE_FORM     = 0x0001; // '#'
    static constexpr format_flag_t const       FORMAT_FLAG_LEFT_ADJUSTED      = 0x0002; // '-'
    static constexpr format_flag_t const       FORMAT_FLAG_SPACE_SIGN         = 0x0004; // ' '
    static constexpr format_flag_t const       FORMAT_FLAG_SHOW_SIGN          = 0x0008; // '+'
    static constexpr format_flag_t const       FORMAT_FLAG_GROUPING           = 0x0010; // '\''
    static constexpr format_flag_t const       FORMAT_FLAG_ALTERNATE_DIGITS   = 0x0020; // 'I'

    static constexpr format_flag_t const       FORMAT_FLAG_LENGTH_MASK        = 0x0F00;
    static constexpr format_flag_t const       FORMAT_FLAG_LENGTH_INT         = 0x0000; // no flag (default)
    static constexpr format_flag_t const       FORMAT_FLAG_LENGTH_CHAR        = 0x0100; // 'hh'
    static constexpr format_flag_t const       FORMAT_FLAG_LENGTH_SHORT       = 0x0200; // 'h'
    static constexpr format_flag_t const       FORMAT_FLAG_LENGTH_LONG        = 0x0300; // 'l'
    static constexpr format_flag_t const       FORMAT_FLAG_LENGTH_LONG_LONG   = 0x0400; // 'll' or 'q'
    static constexpr format_flag_t const       FORMAT_FLAG_LENGTH_LONG_DOUBLE = 0x0500; // 'L'
    static constexpr format_flag_t const       FORMAT_FLAG_LENGTH_INTMAX_T    = 0x0600; // 'j'
    static constexpr format_flag_t const       FORMAT_FLAG_LENGTH_SIZE_T      = 0x0700; // 'z' (or 'Z')
    static constexpr format_flag_t const       FORMAT_FLAG_LENGTH_PTRDFF_T    = 0x0800; // 't'

    static bool is_flag(char_t c, format_item<_CharT> & f)
    {
        switch(c)
        {
        case '#':
            if(f.has_flags(FORMAT_FLAG_ALTERNATE_FORM))
            {
                f.add_error(format_error_t::FORMAT_ERROR_DUPLICATE);
            }
            f.add_flags(FORMAT_FLAG_ALTERNATE_FORM);
            return true;

        case '-':
            if(f.has_flags(FORMAT_FLAG_LEFT_ADJUSTED))
            {
                f.add_error(format_error_t::FORMAT_ERROR_DUPLICATE);
            }
            f.add_flags(FORMAT_FLAG_LEFT_ADJUSTED);
            return true;

        case ' ':
            if(f.has_flags(FORMAT_FLAG_SPACE_SIGN))
            {
                f.add_error(format_error_t::FORMAT_ERROR_DUPLICATE);
            }
            f.add_flags(FORMAT_FLAG_SPACE_SIGN);
            return true;

        case '+':
            if(f.has_flags(FORMAT_FLAG_SHOW_SIGN))
            {
                f.add_error(format_error_t::FORMAT_ERROR_DUPLICATE);
            }
            f.add_flags(FORMAT_FLAG_SHOW_SIGN);
            return true;

        case '\'':
            if(f.has_flags(FORMAT_FLAG_GROUPING))
            {
                f.add_error(format_error_t::FORMAT_ERROR_DUPLICATE);
            }
            f.add_flags(FORMAT_FLAG_GROUPING);
            return true;

        case 'I':
            if(f.has_flags(FORMAT_FLAG_ALTERNATE_DIGITS))
            {
                f.add_error(format_error_t::FORMAT_ERROR_DUPLICATE);
            }
            f.add_flags(FORMAT_FLAG_ALTERNATE_DIGITS);
            return true;

        case 'h':
            switch(f.flags() & FORMAT_FLAG_LENGTH_MASK)
            {
            case FORMAT_FLAG_LENGTH_INT:
                f.add_flags(FORMAT_FLAG_LENGTH_SHORT);
                break;

            case FORMAT_FLAG_LENGTH_SHORT:
                f.set_masked_flags(FORMAT_FLAG_LENGTH_CHAR, FORMAT_FLAG_LENGTH_MASK);
                break;

            default:
                f.add_error(format_error_t::FORMAT_ERROR_DUPLICATE);
                break;

            }
            return true;

        case 'l':
            switch(f.flags() & FORMAT_FLAG_LENGTH_MASK)
            {
            case FORMAT_FLAG_LENGTH_INT:
                f.add_flags(FORMAT_FLAG_LENGTH_LONG);
                break;

            case FORMAT_FLAG_LENGTH_LONG:
                f.set_masked_flags(FORMAT_FLAG_LENGTH_LONG_LONG, FORMAT_FLAG_LENGTH_MASK);
                break;

            default:
                f.add_error(format_error_t::FORMAT_ERROR_DUPLICATE);
                break;

            }
            return true;

        case 'q':
            if((f.flags() & FORMAT_FLAG_LENGTH_MASK) == FORMAT_FLAG_LENGTH_INT)
            {
                f.add_flags(FORMAT_FLAG_LENGTH_LONG_LONG);
            }
            else
            {
                f.add_error(format_error_t::FORMAT_ERROR_DUPLICATE);
            }
            return true;

        case 'L':
            if((f.flags() & FORMAT_FLAG_LENGTH_MASK) == FORMAT_FLAG_LENGTH_INT)
            {
                f.add_flags(FORMAT_FLAG_LENGTH_LONG_DOUBLE);
            }
            else
            {
                f.add_error(format_error_t::FORMAT_ERROR_DUPLICATE);
            }
            return true;

        case 'j':
            if((f.flags() & FORMAT_FLAG_LENGTH_MASK) == FORMAT_FLAG_LENGTH_INT)
            {
                f.add_flags(FORMAT_FLAG_LENGTH_INTMAX_T);
            }
            else
            {
                f.add_error(format_error_t::FORMAT_ERROR_DUPLICATE);
            }
            return true;

        case 'z':
        case 'Z':
            if((f.flags() & FORMAT_FLAG_LENGTH_MASK) == FORMAT_FLAG_LENGTH_INT)
            {
                f.add_flags(FORMAT_FLAG_LENGTH_SIZE_T);
            }
            else
            {
                f.add_error(format_error_t::FORMAT_ERROR_DUPLICATE);
            }
            return true;

        case 't':
            if((f.flags() & FORMAT_FLAG_LENGTH_MASK) == FORMAT_FLAG_LENGTH_INT)
            {
                f.add_flags(FORMAT_FLAG_LENGTH_PTRDFF_T);
            }
            else
            {
                f.add_error(format_error_t::FORMAT_ERROR_DUPLICATE);
            }
            return true;

        default:
            return false;

        }
    }
};



template<typename _CharT>
class strftime_flag_traits
{
public:
    typedef _CharT  char_t;

    static constexpr format_flag_t const       FORMAT_FLAG_PAD_WITH_SPACES = 0x01; // '_'
    static constexpr format_flag_t const       FORMAT_FLAG_NO_PAD          = 0x02; // '-'
    static constexpr format_flag_t const       FORMAT_FLAG_PAD_WITH_ZEROES = 0x04; // '0'
    static constexpr format_flag_t const       FORMAT_FLAG_UPPERCASE       = 0x08; // '#'
    static constexpr format_flag_t const       FORMAT_FLAG_SWAP_CASE       = 0x10; // '^'
    static constexpr format_flag_t const       FORMAT_FLAG_EXTENDED        = 0x20; // 'E'
    static constexpr format_flag_t const       FORMAT_FLAG_MODIFIER        = 0x10; // 'O'

    static bool is_flag(char_t c, format_item<_CharT> & f)
    {
        switch(c)
        {
        case '_':
            if(f.has_flags(FORMAT_FLAG_PAD_WITH_SPACES))
            {
                f.add_error(format_error_t::FORMAT_ERROR_DUPLICATE);
            }
            if(f.has_flags(FORMAT_FLAG_NO_PAD | FORMAT_FLAG_PAD_WITH_ZEROES))
            {
                f.add_error(format_error_t::FORMAT_ERROR_MISMATCH);
            }
            f.add_flags(FORMAT_FLAG_PAD_WITH_SPACES);
            return true;

        case '-':
            if(f.has_flags(FORMAT_FLAG_NO_PAD))
            {
                f.add_error(format_error_t::FORMAT_ERROR_DUPLICATE);
            }
            if(f.has_flags(FORMAT_FLAG_PAD_WITH_SPACES | FORMAT_FLAG_PAD_WITH_ZEROES))
            {
                f.add_error(format_error_t::FORMAT_ERROR_MISMATCH);
            }
            f.add_flags(FORMAT_FLAG_NO_PAD);
            return true;

        case '0':
            if(f.has_flags(FORMAT_FLAG_PAD_WITH_ZEROES))
            {
                f.add_error(format_error_t::FORMAT_ERROR_DUPLICATE);
            }
            if(f.has_flags(FORMAT_FLAG_PAD_WITH_SPACES | FORMAT_FLAG_NO_PAD))
            {
                f.add_error(format_error_t::FORMAT_ERROR_MISMATCH);
            }
            f.add_flags(FORMAT_FLAG_PAD_WITH_ZEROES);
            return true;

        case '^':
            if(f.has_flags(FORMAT_FLAG_UPPERCASE))
            {
                f.add_error(format_error_t::FORMAT_ERROR_DUPLICATE);
            }
            f.add_flags(FORMAT_FLAG_UPPERCASE);
            return true;

        case '#':
            if(f.has_flags(FORMAT_FLAG_SWAP_CASE))
            {
                f.add_error(format_error_t::FORMAT_ERROR_DUPLICATE);
            }
            f.add_flags(FORMAT_FLAG_SWAP_CASE);
            return true;

        default:
            return false;

        }
    }
};



template<typename _CharT>
class no_number_traits
{
public:
    typedef _CharT  char_t;

    static bool support_numbers()
    {
        return false;
    }

    static bool is_number_separator(char_t c)
    {
        NOT_USED(c);
        return false;
    }

    static bool is_number_position(char_t c)
    {
        NOT_USED(c);
        return false;
    }

    static bool is_dynamic_position(char_t c)
    {
        NOT_USED(c);
        return false;
    }

    static bool parse_number(char_t c, int & number, format_item<_CharT> & f)
    {
        NOT_USED(c, number, f);
        return false;
    }
};



template<typename _CharT>
class printf_number_traits
{
public:
    typedef _CharT  char_t;

    static bool support_numbers()
    {
        return true;
    }

    static bool is_number_separator(char_t c)
    {
        return c == '.';
    }

    static bool is_number_position(char_t c)
    {
        return c == '$';
    }

    static bool is_dynamic_position(char_t c)
    {
        return c == '*';
    }

    static bool parse_number(char_t c, int & number, format_item<_CharT> & f)
    {
        if(c >= '0' && c <= '9')
        {
            number *= 10;
            number += c - '0';
            if(number > 10'000)  // let's not exagerate, position or width of more than 10,000?!
            {
                number = 10'000;    // prevent further growth
                f.add_error(format_error_t::FORMAT_ERROR_OVERFLOW);
            }
            return true;
        }

        return false;
    }
};



template<typename _CharT>
class printf_letter_traits
{
public:
    static std::basic_string<_CharT>::size_type is_format(char const * s, format_item<_CharT> & f)
    {
        switch(s[0])
        {
        case 'i': // simplify to 'd'
        case 'd':
            f.format('d');
            return 1UL;

        case 'C': // simplify to 'lc'
            f.add_flags(printf_flag_traits<_CharT>::FORMAT_FLAG_LENGTH_LONG);
            f.format('c');
            return 1UL;

        case 'S': // simplify to 'ls'
            f.add_flags(printf_flag_traits<_CharT>::FORMAT_FLAG_LENGTH_LONG);
            f.format('s');
            return 1UL;

        case 'o':
        case 'u':
        case 'x':
        case 'X':
        case 'e':
        case 'E':
        case 'f':
        case 'F':
        case 'g':
        case 'G':
        case 'a':
        case 'A':
        case 'c':
        case 's':
        case 'p':
        case 'n':
        case 'm':
            f.format(s[0]);
            return 1UL;

        case '$':
            // character is known, but it appears in the wrong place
            //
            f.add_error(format_error_t::FORMAT_ERROR_SYNTAX);
            break;

        default:
std::cerr << "got character [" << s[0] << "]\n";
            f.add_error(format_error_t::FORMAT_ERROR_UNKNOWN);
            break;

        }

        return 0;
    }
};



template<typename _CharT>
class strftime_letter_traits
{
public:
    static std::basic_string<_CharT>::size_type is_format(char const * s, format_item<_CharT> & f)
    {
        switch(s[0])
        {
        case 'a':
        case 'A':
        case 'b':
        case 'B':
        case 'c':
        case 'C':
        case 'd':
        case 'D':
        case 'e':
        case 'F':
        case 'g':
        case 'G':
        case 'h':
        case 'H':
        case 'I':
        case 'j':
        case 'k':
        case 'l':
        case 'm':
        case 'M':
        case 'n':
        case 'p':
        case 'P':
        case 'r':
        case 'R':
        case 's':
        case 'S':
        case 't':
        case 'T':
        case 'u':
        case 'U':
        case 'V':
        case 'w':
        case 'W':
        case 'x':
        case 'X':
        case 'y':
        case 'Y':
        case 'z':
        case 'Z':
        case '+':
            f.format(s[0]);
            return 1UL;

        case 'E':
            switch(s[1])
            {
            case 'c':
            case 'C':
            case 'x':
            case 'X':
            case 'y':
            case 'Y':
                f.add_flags(strftime_flag_traits<_CharT>::FORMAT_FLAG_EXTENDED);
                f.format(s[1]);
                return 2UL;

            }
            break;

        case 'O':
            switch(s[1])
            {
            case 'd':
            case 'e':
            case 'H':
            case 'I':
            case 'm':
            case 'M':
            case 'S':
            case 'u':
            case 'U':
            case 'V':
            case 'w':
            case 'W':
            case 'y':
                // mark that we found an 'O' modifier followed by a valid letter
                //
                f.add_flags(strftime_flag_traits<_CharT>::FORMAT_FLAG_MODIFIER);
                f.format(s[1]);
                return 2UL;

            }
            break;

        }

        f.add_error(format_error_t::FORMAT_ERROR_UNKNOWN);

        return 0;
    }
};



template<
      typename _CharT
    , typename LetterTraits
    , typename FlagTraits = no_flag_traits<_CharT>
    , typename NumberTraits = no_number_traits<_CharT>
    , typename IntroducerTraits = percent_introducer_traits<_CharT>>
format_item<_CharT>::list_t tokenize_format(std::basic_string<_CharT> const & format_string)
{
    typename format_item<_CharT>::list_t result;

    typename std::basic_string<_CharT>::size_type end(format_string.length());
    for(typename std::basic_string<_CharT>::size_type pos(0); pos < end; )
    {
        typename std::basic_string<_CharT>::size_type const begin(pos);
        if(IntroducerTraits::is_introducer(format_string[pos]))
        {
            format_item<_CharT> item;
            ++pos;
            if(IntroducerTraits::double_to_escape()
            && IntroducerTraits::is_introducer(format_string[pos]))
            {
                // "%%" case
                //
                ++pos;
                item.string(format_string.substr(begin, pos - begin));
                result.push_back(item);
            }
            else
            {
                // parse a whole format item
                //
                bool found_width(false);
                bool found_number_separator(false);
                bool found_precision(false);
                bool found_position(false);
                for(; pos < end; ++pos)
                {
                    // handle flags
                    //
                    if(FlagTraits::is_flag(format_string[pos], item))
                    {
                        continue;
                    }

                    // handle numbers
                    //
                    if(NumberTraits::support_numbers())
                    {
                        if(!found_number_separator)
                        {
                            if(NumberTraits::is_number_separator(format_string[pos]))
                            {
                                found_number_separator = true;
std::cerr << "   <<< got separator!\n";
                                continue;
                            }
                        }
                        else
                        {
                            if(NumberTraits::is_number_separator(format_string[pos]))
                            {
                                // we cannot have more than one '.' in a valid format
                                //
                                item.add_error(format_error_t::FORMAT_ERROR_DUPLICATE);
                                continue;
                            }
                        }
                        bool dynamic_position(NumberTraits::is_dynamic_position(format_string[pos]));
                        if(dynamic_position)
                        {
                            if(pos + 1 >= end)
                            {
                                item.add_error(format_error_t::FORMAT_ERROR_EOS);
                                item.string(format_string.substr(begin, end - begin));
                                if(found_number_separator)
                                {
                                    item.precision(0);
                                }
                                else
                                {
                                    item.width(0);
                                }
                                result.push_back(item);
                                continue;
                            }
                            ++pos;
                        }
                        bool has_digits(false);
                        int number(0);
                        for(; pos < end; ++pos)
                        {
                            if(!NumberTraits::parse_number(format_string[pos], number, item))
                            {
                                break;
                            }
                            has_digits = true;
                        }
                        bool const has_number(has_digits || dynamic_position);
                        if(has_number)
                        {
                            if(NumberTraits::is_number_position(format_string[pos]))
                            {
                                ++pos;
                                if(dynamic_position)
                                {
                                    if(found_number_separator)
                                    {
                                        if(found_precision)
                                        {
                                            item.add_error(format_error_t::FORMAT_ERROR_DUPLICATE);
                                        }
                                        found_precision = true;
                                        item.precision(-number);
                                    }
                                    else
                                    {
                                        if(found_width)
                                        {
                                            item.add_error(format_error_t::FORMAT_ERROR_DUPLICATE);
                                        }
                                        found_width = true;
                                        item.width(-number);
                                    }
                                }
                                else
                                {
                                    if(found_position)
                                    {
                                        item.add_error(format_error_t::FORMAT_ERROR_DUPLICATE);
                                    }
                                    found_position = true;
                                    item.position(number);
                                }
                            }
                            else
                            {
                                // you cannot at the same time have *
                                // and a number not followed by a '$'
                                //
                                if(dynamic_position && has_digits)
                                {
                                    item.add_error(format_error_t::FORMAT_ERROR_MISMATCH);
                                }
                                if(found_number_separator)
                                {
                                    if(found_precision)
                                    {
                                        item.add_error(format_error_t::FORMAT_ERROR_DUPLICATE);
                                    }
                                    found_precision = true;
                                    item.precision(number);
                                }
                                else
                                {
                                    if(found_width)
                                    {
                                        item.add_error(format_error_t::FORMAT_ERROR_DUPLICATE);
                                    }
                                    found_width = true;
                                    item.width(number);
                                }
                            }
std::cerr << "--- got number " << number << " followed by " << format_string[pos] << "?\n";
                            --pos;
                            continue;
                        }
                    }

                    // handle the letter (must match or that format is not valid)
                    //
                    typename std::basic_string<_CharT>::size_type const len(LetterTraits::is_format(format_string.c_str() + pos, item));
                    if(len != 0)
                    {
                        pos += len;
                        item.string(format_string.substr(begin, pos - begin));
                        result.push_back(item);
                    }
                    else
                    {
                        // error(s) occurred:
                        //
                        // 1. save a one character (introducer) item with the error(s)
                        // 2. restart just after the introducer
                        //
                        pos = begin + 1;
                        item.string(format_string.substr(begin, pos - begin));
                        result.push_back(item);
                    }
                    break;
                }
            }
        }
        else
        {
            format_item<_CharT> item;
            for(++pos; pos < end; ++pos)
            {
                if(IntroducerTraits::is_introducer(format_string[pos]))
                {
                    break;
                }
                if(pos + 1 < end
                && IntroducerTraits::escape_character(format_string[pos]))
                {
                    ++pos;
                }
            }
            item.string(format_string.substr(begin, pos - begin));
            result.push_back(item);
        }
    }

    return result;
}


//template<typename _CharT>
//std::string join_format_items(
//      std::basic_string<_CharT> const & format_string
//    , typename format_item<_CharT>::list_t items)
//{
//}



} // namespace snapdev
// vim: ts=4 sw=4 et
