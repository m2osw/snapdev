// Copyright (c) 2021-2023  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Verify that the hexadecimal convertions work.
 *
 * This file implements tests for the hexadecimal to binary and vice
 * versa functions.
 */

// self
//
#include    <snapdev/hexadecimal_string.h>

#include    "catch_main.h"



// C++ lib
//
#include    <iomanip>
#include    <set>


// last include
//
#include    <snapdev/poison.h>



namespace
{



// function stolen from the libutf8 library
//   see https://snapwebsites.org/project/libutf8
int wctombs(char * mb, char32_t wc, size_t len)
{
    auto verify_length = [&len](size_t required_len)
    {
        if(len < required_len)
        {
            throw std::logic_error("wctombs() called with an output buffer which is too small.");
        }
    };

    if(wc < 0x80)
    {
        verify_length(2);

        /* this will also encode '\0'... */
        mb[0] = static_cast<char>(wc);
        mb[1] = '\0';
        return 1;
    }
    if(wc < 0x800)
    {
        verify_length(3);

        mb[0] = static_cast<char>((wc >> 6) | 0xC0);
        mb[1] = (wc & 0x3F) | 0x80;
        mb[2] = '\0';
        return 2;
    }

    // avoid encoding the UTF-16 surrogate because those code points do not
    // represent characters
    //
    if(wc < 0xD800 || wc > 0xDFFF)
    {
        if(wc < 0x10000)
        {
            verify_length(4);

            mb[0] = static_cast<char>((wc >> 12) | 0xE0);
            mb[1] = ((wc >> 6) & 0x3F) | 0x80;
            mb[2] = (wc & 0x3F) | 0x80;
            mb[3] = '\0';
            return 3;
        }
        if(wc < 0x110000)
        {
            verify_length(5);

            mb[0] = static_cast<char>((wc >> 18) | 0xF0);
            mb[1] = ((wc >> 12) & 0x3F) | 0x80;
            mb[2] = ((wc >> 6) & 0x3F) | 0x80;
            mb[3] = (wc & 0x3F) | 0x80;
            mb[4] = '\0';
            return 4;
        }
    }

    verify_length(1);

    /* an invalid wide character */
    mb[0] = '\0';
    return -1;
}


} // no name namespace



CATCH_TEST_CASE("hexadecimal_string_hex_digits", "[hexadecimal_string]")
{
    CATCH_START_SECTION("hexadecimal_string: verify hexadecimal digit detection")
    {
        for(int i(0); i < 0x110000; ++i) // all of Unicode
        {
            if((i >= '0' && i <= '9')
            || (i >= 'a' && i <= 'f')
            || (i >= 'A' && i <= 'F'))
            {
                CATCH_REQUIRE(snapdev::is_hexdigit(i));
            }
            else
            {
                CATCH_REQUIRE_FALSE(snapdev::is_hexdigit(i));
            }
        }
    }
    CATCH_END_SECTION()
}


CATCH_TEST_CASE("hexadecimal_string_16_bit_values", "[hexadecimal_string]")
{
    CATCH_START_SECTION("hexadecimal_string: all 16 bit values")
    {
        for(int i(0); i < 65'536; ++i)
        {
            std::stringstream ss;
            ss << std::hex << i;
            std::string value(ss.str());
            if((value.length() & 1) != 0)
            {
                value = "0" + value;
            }
            std::string bin(snapdev::hex_to_bin(value));
            std::string upper;
            if(bin.length() == 1)
            {
                CATCH_REQUIRE(i < 256);
                CATCH_REQUIRE(static_cast<unsigned char>(bin[0]) == static_cast<unsigned char>(i));

                CATCH_REQUIRE(snapdev::int_to_hex(i, false) == ss.str());
                CATCH_REQUIRE(snapdev::int_to_hex(i, false, 2) == value);

                CATCH_REQUIRE(snapdev::hex_to_int<int>(ss.str()) == i);
                CATCH_REQUIRE(snapdev::hex_to_int<int>(value) == i);

                upper = snapdev::int_to_hex(i, true, 2);
            }
            else
            {
                CATCH_REQUIRE(bin.length() == 2);
                CATCH_REQUIRE(static_cast<unsigned char>(bin[0]) == static_cast<unsigned char>(i >> 8));
                CATCH_REQUIRE(static_cast<unsigned char>(bin[1]) == static_cast<unsigned char>(i));

                CATCH_REQUIRE(snapdev::int_to_hex(i, false) == ss.str());
                CATCH_REQUIRE(snapdev::int_to_hex(i, false, 4) == value);
                CATCH_REQUIRE(snapdev::hex_to_int<int>(ss.str()) == i);
                CATCH_REQUIRE(snapdev::hex_to_int<int>(value) == i);

                upper = snapdev::int_to_hex(i, true, 4);
            }

            CATCH_REQUIRE(snapdev::bin_to_hex(bin) == value);

            std::string bin_from_upper(snapdev::hex_to_bin(upper));
            if(bin_from_upper.length() == 1)
            {
                CATCH_REQUIRE(i < 256);
                CATCH_REQUIRE(static_cast<unsigned char>(bin[0]) == static_cast<unsigned char>(i));
            }
            else
            {
                CATCH_REQUIRE(bin.length() == 2);
                CATCH_REQUIRE(static_cast<unsigned char>(bin[0]) == static_cast<unsigned char>(i >> 8));
                CATCH_REQUIRE(static_cast<unsigned char>(bin[1]) == static_cast<unsigned char>(i));
            }
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("hexadecimal_string: hex_to_bin & bin_to_hex, large (and small) random numbers")
    {
        for(int i(0); i < 1'000; ++i)
        {
            std::stringstream ss;
            std::string result;
            int const length((rand() % 16 + 2) & -2);
            for(int j(0); j < length; ++j)
            {
                int value(rand() % 256);
                ss << std::setw(2) << std::setfill('0') << std::hex << value;
                result.push_back(value);
            }
            CATCH_REQUIRE(snapdev::hex_to_bin(ss.str()) == result);
            CATCH_REQUIRE(snapdev::bin_to_hex(result) == ss.str());
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("hexadecimal_string: bin_to_hex with empty")
    {
        CATCH_REQUIRE(snapdev::bin_to_hex(std::string()) == std::string());
        CATCH_REQUIRE(snapdev::bin_to_hex("") == "");
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("hexadecimal_string: large width for int_to_hex()")
    {
        for(int i(0); i < 256; ++i)
        {
            std::uint8_t c(i & 255);
            std::string const value(snapdev::int_to_hex(c, true, 4));  // 4 when type is at most 2 digits, return 2 digits...
            std::stringstream ss;
            ss << std::setw(2) << std::setfill('0') << std::hex << std::uppercase << i;
            CATCH_REQUIRE(ss.str() == value);
        }
    }
    CATCH_END_SECTION()
}


CATCH_TEST_CASE("hexadecimal_string_invalid_input", "[hexadecimal_string][error]")
{
    CATCH_START_SECTION("hexadecimal_string: invalid length")
    {
        for(int i(1); i <= 31; i += 2)
        {
            std::stringstream ss;
            for(int j(0); j < i; ++j)
            {
                int const value(rand() % 16);
                ss << std::hex << value;
            }
            CATCH_REQUIRE_THROWS_MATCHES(
                      snapdev::hex_to_bin(ss.str())
                    , snapdev::hexadecimal_string_invalid_parameter
                    , Catch::Matchers::ExceptionMessage(
                              "hexadecimal_string_exception: the hex parameter must have an even size."));
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("hexadecimal_string: invalid digits")
    {
        int left(0);
        int right(0);
        bool small_char(false);
        while(left < 25 || right < 25 || !small_char)
        {
            std::stringstream ss;
            ss << std::hex << rand();
            if((ss.str().length() & 1) != 0)
            {
                ++right;
            }
            else
            {
                ++left;
            }

            // all of our std::string are considered UTF-8 so add a Unicode
            // character even though the hex_to_bin() function really
            // only expects ASCII
            //
            char32_t invalid(U'\0');
            for(;;)
            {
                invalid = rand() % (0x110000 - 1) + 1;
                if(!small_char && left > 10 && right > 10)
                {
                    invalid = (invalid - 1) % 0x7F + 1;
                }
                if((invalid < 0xD800 || invalid >= 0xE000)  // don't try with UTF-16 surrogates
                && !snapdev::is_hexdigit(invalid))
                {
                    if(invalid < 0x80)
                    {
                        small_char = true;
                    }
                    char buf[16];
                    wctombs(buf, invalid, sizeof(buf));
                    ss << buf;
                    break;
                }
            }

            // make sure the length is even
            //
            if((ss.str().length() & 1) != 0)
            {
                ss << std::hex << rand() % 16;
            }

            if(invalid >= 0x80)
            {
                // in this case the character is not available so we cannot
                // just add it cleanly to the exception message
                //
                CATCH_REQUIRE_THROWS_MATCHES(
                          snapdev::hex_to_bin(ss.str())
                        , snapdev::hexadecimal_string_invalid_parameter
                        , Catch::Matchers::ExceptionMessage(
                                    "hexadecimal_string_exception: input character is not an hexadecimal digit."));
            }
            else
            {
                CATCH_REQUIRE_THROWS_MATCHES(
                          snapdev::hex_to_bin(ss.str())
                        , snapdev::hexadecimal_string_invalid_parameter
                        , Catch::Matchers::ExceptionMessage(
                                    std::string("hexadecimal_string_exception: input character '")
                                  + static_cast<char>(invalid)
                                  + "' is not an hexadecimal digit."));
            }
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("hexadecimal_string: integer too small (overflow)")
    {
        for(int i(0); i <= 1'000; ++i)
        {
            std::uint32_t value(rand());
            while(value < 256)
            {
                value = rand();
            }
            std::stringstream ss;
            ss << value;
            CATCH_REQUIRE_THROWS_MATCHES(
                      snapdev::hex_to_int<std::uint8_t>(ss.str())
                    , snapdev::hexadecimal_string_out_of_range
                    , Catch::Matchers::ExceptionMessage(
                              "hexadecimal_string_out_of_range: input string has an hexadecimal number which is too large for the output integer type."));
        }
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
