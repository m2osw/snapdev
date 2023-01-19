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
#pragma once

/** \file
 * \brief Convert binary to a string in hexadecimal.
 *
 * C++ offers the std::to_string() function which converts numbers to
 * decimal. However, we often want to convert strings to hexadecimal and
 * using std::stringstream is not as efficient as using these functions.
 *
 * Here we can convert a buffer of any length to hexadecimal and vice
 * versa. You can also use this implementation to covert just one byte.
 */

// libexcept
//
#include    "libexcept/exception.h"


// C++
//
#include    <algorithm>
#include    <climits>
#include    <iomanip>



namespace snapdev
{



DECLARE_MAIN_EXCEPTION(hexadecimal_string_exception);

DECLARE_OUT_OF_RANGE(hexadecimal_string_out_of_range);

DECLARE_EXCEPTION(hexadecimal_string_exception, hexadecimal_string_invalid_parameter);



/** \brief Check whether character is an hexadecimal digit.
 *
 * This function checks whether the given \p c character represents
 * an hexadecimal digit.
 *
 * The function accepts upper and lower case characters (A-F or a-f)
 * as the same set of digits.
 *
 * \rparam charT  The type of character to be tested.
 * \param[in] c  The character to be tested.
 *
 * \return true if the the character represents an hexadecimal digit.
 */
template<class charT>
bool is_hexdigit(charT c)
{
    return (c >= '0' && c <= '9')
        || (c >= 'a' && c <= 'f')
        || (c >= 'A' && c <= 'F');
}


/** \brief Convert one hexadecimal digit to a character.
 *
 * This function converts an hexadecimal number from 0 to 15 into
 * an hexadecimal character: '0' to '9' or 'a' to 'f'.
 *
 * To get uppercase, simple set the \p uppercase parameter to true.
 *
 * \param[in] d  The hexadecimal digit convert.
 *
 * \return An hexadecimal character.
 */
template<class intT>
char to_hex(intT d, bool uppercase = false)
{
    if(static_cast<typename std::make_unsigned<intT>::type>(d) >= 16)
    {
        throw hexadecimal_string_out_of_range(
                  "input number ("
                + std::to_string(d)
                + ") is negative or too large to represent one hexadecimal digit.");
    }
    return d < 10 ? d + '0' : d + ((uppercase ? 'A' : 'a') - 10);
}


/** \brief Convert an hexadecimal character in a number.
 *
 * This function converts an hexadecimal character that represents a valid
 * digit in an integer.
 *
 * \exception hexadecimal_string_invalid_parameter
 * If the input is not a valid hexadecimal character, then this error is
 * raised.
 *
 * \tparam charT  The type of the character (char, char32_t, etc.)
 * \param[in] c  The character to convert to a number.
 *
 * \return The converted character as an integer.
 */
template<class charT>
int hexdigit_to_number(charT c)
{
    if(c >= '0' && c <= '9')
    {
        return c - '0';
    }
    if(c >= 'a' && c <= 'f')
    {
        return c - ('a' - 10);
    }
    if(c >= 'A' && c <= 'F')
    {
        return c - ('A' - 10);
    }
    if(static_cast<typename std::make_unsigned<charT>::type>(c) < 0x80)
    {
        throw hexadecimal_string_invalid_parameter(
                  std::string("input character '")
                + static_cast<char>(c)
                + "' is not an hexadecimal digit.");
    }
    // if character represents a UTF-8 charater, we do not have all the
    // bytes to convert it so just use a plan error message
    //
    throw hexadecimal_string_invalid_parameter(
              "input character is not an hexadecimal digit.");
}


/** \brief Transform a binary string to hexadecimal.
 *
 * This function transforms a string of binary bytes (any value from 0x00
 * to 0xFF) to a string of hexadecimal digits.
 *
 * The output string will be exactly 2x the size of the input string.
 *
 * You can request uppercase instead of lowercase for the letters a to f.
 * The default is to use lowercase characters.
 *
 * \param[in] binary  The input binary string to convert.
 * \param[in] uppercase  If true, use uppercase letters for a-f.
 *
 * \return The hexademical representation of the input string.
 */
inline std::string bin_to_hex(std::string const & binary, bool uppercase = false)
{
    if(binary.empty())
    {
        return std::string();
    }

    std::string result;

    result.reserve(binary.length() * 2);

    std::for_each(
              binary.begin()
            , binary.end()
            , [&result,uppercase](char const & c)
            {
                result.push_back(to_hex((c >> 4) & 15, uppercase));
                result.push_back(to_hex(c & 15, uppercase));
            });

    return result;
}


/** \brief Convert an hexadecimal string to a binary string.
 *
 * This function is the inverse of the bin_to_hex() function. It
 * converts a text string of hexadecimal numbers (exactly 2 digits
 * each) into a binary string (a string of any bytes from 0x00 to
 * 0xFF.)
 *
 * The output will be exactly half the size of the input.
 *
 * \note
 * The output is saved in a string in big endian format.
 *
 * \exception hexadecimal_string_invalid_parameter
 * If the input string is not considered valid, then this exception is
 * raised. To be valid every single character must be an hexadecimal
 * digit (0-9, a-f, A-F) and the length of the string must be even.
 *
 * \param[in] hex  The hexadecimal string of characters.
 *
 * \return The converted value in a binary string.
 */
inline std::string hex_to_bin(std::string const & hex)
{
    if((hex.length() & 1) != 0)
    {
        throw hexadecimal_string_invalid_parameter("the hex parameter must have an even size.");
    }

    std::string result;

    for(char const * s(hex.c_str()); *s != '\0'; s += 2)
    {
        char value = static_cast<char>(hexdigit_to_number(s[0]) * 16
                                     + hexdigit_to_number(s[1]));
        result.push_back(value);
    }

    return result;
}


/** \brief Transform an integer to a string of hexadecimal digits.
 *
 * This function transforms an integer to a string of hexadecimal digits.
 *
 * The output string is optimized to not include any unnecessary leading
 * zeroes.
 *
 * The input value is considered to be positive or zero. Negative numbers
 * are viewed as their unsigned corresponding number (i.e. if the input
 * is an int32_t is viewed as uint32_t).
 *
 * \note
 * The function does not add an introducer (so no "0x" at the start of
 * the resulting string).
 *
 * \tparam T  The type of integer to convert to a string.
 * \param[in] value  The input integer to convert.
 * \param[in] uppercase  Whether to use upper (true) or lower (false) case
 * letters for the hexadecimal digits A to F.
 * \param[in] width  The minimum width (prepend '0' to reach this width).
 *
 * \return The hexademical representation of the input integer.
 */
template<class T>
inline std::string int_to_hex(
      T value
    , bool uppercase = false
    , std::uint32_t width = 1)
{
    typedef typename std::make_unsigned<T>::type unsigned_value_t;

    char buf[sizeof(T) * 2 + 1];
    char * d(buf + sizeof(T) * 2);
    char const * const e(d);
    *d = '\0';

    char const letter_digit(uppercase ? 'A' - 10 : 'a' - 10);

    for(unsigned_value_t unsigned_value(value);
        unsigned_value != 0;
        unsigned_value >>= 4)
    {
        --d;
        int const v(unsigned_value & 15);
        *d = v < 10 ? v + '0' : v + letter_digit;
    }

    if(*d == '\0')
    {
        --d;
        *d = '0';
    }

    if(width > sizeof(buf) - 1)
    {
        width = sizeof(buf) - 1;
    }
    while(e - d < width)
    {
        --d;
        *d = '0';
    }

    return std::string(d, e - d);
}


/** \brief Convert a string to an integer.
 *
 * This function converts a string that represents an hexadecimal number
 * in an integer.
 *
 * \exception out_of_range
 * If the number is too large for the integer, then this exception is
 * raised.
 *
 * \exception hexadecimal_string_invalid_parameter
 * If the input string is not considered valid, then this exception is
 * raised. To be valid every single character must be an hexadecimal
 * digit (0-9, a-f, A-F) and the length of the string must be even.
 *
 * \tparam T  The type of integer to return.
 * \param[in] hex  The hexadecimal string to be converted.
 *
 * \return The converted \p hex parameter as an integer.
 */
template<class T>
T hex_to_int(std::string const & hex)
{
    T value(0);
    for(auto const c : hex)
    {
        if((value >> (sizeof(T) * CHAR_BIT - 4)) != 0)
        {
            throw hexadecimal_string_out_of_range("input string has an hexadecimal number which is too large for the output integer type.");
        }
        value *= 16;
        value += hexdigit_to_number(c);
    }
    return value;
}


} // namespace snapdev
// vim: ts=4 sw=4 et
