// Copyright (c) 2011-2022  Made to Order Software Corp.  All Rights Reserved
//
// https://snapwebsites.org/project/snapdev
// contact@m2osw.com
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#pragma once

/**
 * Original written by Clyne Sullivan, released in the public domain.
 * https://github.com/tcsullivan/constexpr-to-string
 */

// C++
//
#include    <type_traits>
#include    <stdexcept>



namespace snapdev
{

namespace detail
{


/** \brief Convert integrals to a string at compile time.
 *
 * This template is used to convert integers to a string of characters at
 * compile time. It supports a base so you can convert integers to decimal,
 * octal, hexadecimal, binary, and any other base from 2 up to 36.
 *
 * \note
 * This class is overfully complete and can be used to traverse the resulting
 * string. I've kept it that way although we don't generally use it for more
 * than a simple conversion so at the moment it will stay as a detail.
 *
 * \tparam value  The number to convert.
 * \tparam base  Desired base.
 * \tparam uppercase  Whether to use uppercase (true) or lowercase (false).
 * \tparam CharT  Type of characters of the output string.
 */
template<
      auto value
    , int base
    , bool uppercase
    , typename CharT
    , std::enable_if_t<std::is_integral_v<decltype(value)>, int> = 0
    , std::enable_if_t<(base >= 2 && base <= 36), int> = 0>
class to_string_literal_impl
{
public:
    /** \brief Computed size of the string array.
     *
     * To hold the string, we need an array of CharT characters. This is
     * the size of that array, including the '-' for negative numbers
     * and the '\0' for the null terminator.
     *
     * This parameter is public, the size() function returns the same
     * number.
     */
    static constexpr std::size_t BUFFER_SIZE = []() constexpr noexcept
                {
                    // when value is negative, need extra CharT for sign
                    //
                    std::size_t len(value > 0 ? 1 : 2);
                    for(auto v(value); v != 0; ++len, v /= base);
                    return len;
                }();

private:
    /** \brief Initialize the output buffer.
     *
     * The following defines a buffer which will hold the final string.
     * We use a lambda function to compute the exact lengrh of the output
     * at compile time (i.e. constexpr).
     */
    CharT   f_buf[BUFFER_SIZE] = {};

 public:
    /** \brief Contruct the object which converts the value.
     *
     * The constructor fills f_buf with the string representation of \p value.
     */
    constexpr to_string_literal_impl() noexcept
    {
        CharT * ptr(end());
        --ptr;
        *ptr = '\0';
        if(value != 0)
        {
            for(auto v(value); v != 0; v /= base)
            {
                --ptr;
                int const digit((value < 0 ? -1 : 1) * (v % base));
                if(digit < 10)
                {
                    *ptr = '0' + digit;
                }
                else if(uppercase)
                {
                    *ptr = ('A' - 10) + digit;
                }
                else
                {
                    *ptr = ('a' - 10) + digit;
                }
            }

            if(value < 0)
            {
                --ptr;
                *ptr = '-';
            }
        }
        else
        {
            --ptr;
            *ptr = '0';
        }
    }

    /** \brief Cast to CharT *
     *
     * This function returns a direct pointer to the internal buffer.
     *
     * \return The pointer to the internal buffer.
     */
    constexpr operator CharT * () noexcept
    {
        return f_buf;
    }

    /** \brief Cast to CharT const *
     *
     * This function returns a direct pointer to the internal buffer.
     *
     * \return The const pointer to the internal buffer.
     */
    constexpr operator CharT const * () const noexcept
    {
        return f_buf;
    }

    /** \brief Length of the final string.
     *
     * This function returns the length of the final string as strlen() would
     * return.
     *
     * \return Length of the final string in f_buf.
     */
    constexpr std::size_t length() const noexcept
    {
        return BUFFER_SIZE - 1;
    }

    /** \brief Size of f_buf.
     *
     * This function returns the total size of the f_buf array of CharT.
     * This includes the final '\0' character.
     *
     * \return Exact size of the allocated buffer.
     */
    constexpr std::size_t size() const noexcept
    {
        return BUFFER_SIZE;
    }

    /** \brief Access to the buffer.
     *
     * This function gives you read/write access to the buffer. You can
     * retrieve the exact size of the buffer using the size() function.
     *
     * \return A pointer to the internal buffer.
     */
    constexpr CharT * data() noexcept
    {
        return f_buf;
    }

    /** \brief Read access to the buffer.
     *
     * This function gives you direct read access to the buffer.
     *
     * \return A const pointer to the internal buffer.
     */
    constexpr CharT const * data() const noexcept
    {
        return f_buf;
    }

    /** \brief Access a character in the buffer.
     *
     * This function allows you to access a character by index. This version
     * of the function allows you to replace the character with another.
     *
     * \param[in] i  The index at which you want to retrieve a character.
     *
     * \return The read/write CharT reference at the specified index.
     */
    constexpr CharT & operator[] (int i)
    {
        if(static_cast<std::size_t>(i) >= size())
        {
            throw std::range_error("index out of range");
        }

        return f_buf[i];
    }

    /** \brief Access a character in the buffer.
     *
     * This function allows you to read a character by index.
     *
     * \return The CharT at the specified index.
     */
    constexpr CharT const operator[] (int i) const
    {
        if(static_cast<std::size_t>(i) >= size())
        {
            throw std::range_error("index out of range");
        }

        return f_buf[i];
    }

    /** \brief Read/write access the front of the string.
     *
     * This function returns a reference to the front, a.k.a. the first
     * character in the string buffer.
     *
     * \return A reference to the front of the string.
     */
    constexpr CharT & front() noexcept
    {
        return f_buf[0];
    }

    /** \brief Read access the front of the string.
     *
     * This function returns a reference to the front, a.k.a. the first
     * character in the string buffer.
     *
     * \return A const reference to the front of the string.
     */
    constexpr CharT const & front() const noexcept
    {
        return f_buf[0];
    }

    /** \brief Read/write access the back of the string.
     *
     * This function returns a reference to the back, a.k.a. the last
     * character in the string buffer.
     *
     * \return A reference to the back of the string.
     */
    constexpr CharT & back() noexcept
    {
        return f_buf[length()];
    }

    /** \brief Read access the back of the string.
     *
     * This function returns a reference to the back, a.k.a. the last
     * character in the string buffer.
     *
     * \return A const reference to the back of the string.
     */
    constexpr CharT const & back() const noexcept
    {
        return f_buf[length()];
    }

    /** \brief Access to the start of the string.
     *
     * This function returns a pointer to the start of the string.
     *
     * \return A pointer to the start of the string.
     */
    constexpr CharT * begin() noexcept
    {
        return f_buf;
    }

    /** \brief Access to the start of the string.
     *
     * This function returns a pointer to the start of the string.
     *
     * \return A pointer to the start of the string.
     */
    constexpr CharT const * begin() const noexcept
    {
        return f_buf;
    }

    /** \brief Access to the end of the string.
     *
     * This function returns a pointer to the end of the string.
     *
     * \return A pointer to the end of the string.
     */
    constexpr CharT * end() noexcept
    {
        return f_buf + size();
    }

    /** \brief Access to the end of the string.
     *
     * This function returns a pointer to the end of the string.
     *
     * \return A pointer to the end of the string.
     */
    constexpr CharT const * end() const noexcept
    {
        return f_buf + size();
    }
};

} // namespace detail

/** \brief Convert an integer to a string at compile time.
 *
 * This template simplifies the conversion of an integer to a string at
 * compile time. It supports three parameters: the value to convert, the
 * base, and the type of characters you want in the output.
 *
 * By default the base is set to 10. Any base from 2 to 36 is supported.
 * No introducer is added. If you would like an introducer, you may be
 * able to join it with the snapdev::join_strings template.
 *
 * The output is an array of char by default. You can also change that
 * by changing the third parameter. Any character type will work as
 * expected.
 *
 * \note
 * If you want to avoid the std::string_view use, you can check out the
 * numeric_string.h instead. That other one only deals with `const char`.
 *
 * \tparam value  The value to convert to a string.
 * \tparam base  The base used to convert the value (2 to 36).
 * \tparam uppercase  Whether to use uppercase (true) or lowercase (false).
 * \tparam CharT  The character type of the output string.
 */
template<
      auto value
    , int base = 10
    , bool uppercase = false
    , typename CharT = char>
constexpr detail::to_string_literal_impl<value, base, uppercase, CharT> to_string_literal;


} // namespace snapdev
// vim: ts=4 sw=4 et
