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
#pragma once

/** \file
 * \brief Convert an integer to a string at compile time.
 *
 * This is very similar to the snapdev::numeric_string template.
 *
 * These functions are useful to create a string from an integer at compile
 * time.
 *
 * Original written by Clyne Sullivan, released in the public domain.
 * https://github.com/tcsullivan/constexpr-to-string
 */

// C++
//
#include    <cmath>
#include    <stdexcept>
#include    <type_traits>



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
 * This class is overkill and can be used to traverse the resulting
 * string. I've kept it that way although we don't generally use it
 * for more than a simple conversion so at the moment it stays
 * as a detail.
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
class integer_to_string_literal_impl
{
public:
    /** \brief The type of the value.
     *
     * This typedef captures the exact type of the value being passed to
     * this template.
     */
    typedef decltype(value)     value_type;

    /** \brief The type of character.
     *
     * This typedef captures the type of character used in the output string.
     */
    typedef CharT               char_type;

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
     * We use a lambda function to compute the exact length of the output
     * at compile time (i.e. constexpr).
     */
    CharT   f_buf[BUFFER_SIZE] = {};

public:
    /** \brief Contruct the object which converts the value.
     *
     * The constructor fills f_buf with the string representation of \p value.
     */
    constexpr integer_to_string_literal_impl() noexcept
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

#ifdef _DEBUG
        if(ptr != begin())
        {
            std::terminate(); // wrong number of characters added.
        }
#endif
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




/** \brief Convert a floating pointer number to a string literal.
 *
 * This class provides the ability to convert a floating point number to a
 * string at compile-time.
 *
 * Here is an example of usage of the simplified template:
 *
 * \code
 * constexpr float const       CONSTANT_ROOT_TWO = 1.414213562373095048801688724209698078;
 * constexpr char const *      g_root_two_view = snapdev::floating_point_to_string_literal<CONSTANT_ROOT_TWO>.data();
 * ...
 * // example using that value as an advgetopt default value:
 * DefaultValue(g_root_two_view)
 * \endcode
 *
 * The type of floating point used determines the number of digits after
 * the decimal point. This is 6 for a float, 15 for a double, and
 * 18 for a long double.
 *
 * \warning
 * The input numbers are currently limited to values that can fit in a
 * `long long int`. The function does not add an exponent (`e+123` notation).
 * Very small numbers result in zero. Numbers that are too large generate
 * a compile time `range_error`.
 *
 * \todo
 * avoid the zeroes at the end of the number (after the decimal point,
 * except for one 3.0 but not 3.000000).
 *
 * \note
 * This class is overkill and can be used to traverse the resulting
 * string. I've kept it that way although we don't generally use it
 * for more than a simple conversion so at the moment it stays
 * as a detail.
 *
 * \tparam value  Number to convert
 * \tparam CharT  Type of characters in the returned string.
 */
template<
      auto value
    , typename CharT
    , std::enable_if_t<std::is_floating_point_v<decltype(value)>, int> = 0>
class floating_point_to_string_literal_impl
{
public:
    typedef decltype(value)     value_type;
    typedef CharT               char_type;

    static constexpr long long int whole()
    {
        long long int const r(static_cast<long long int>(value));
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
        if(r != std::trunc(r))
        {
            throw std::range_error("floating point is too large for compile time conversion to string.");
        }
#pragma GCC diagnostic pop
        return r;
    }

    static constexpr int fractional_length()
    {
        return std::numeric_limits<value_type>::digits10;
    }

    static constexpr long long int frac()
    {
        value_type const v(value - static_cast<value_type>(whole()));
        constexpr value_type const exponent(fractional_length() + 1);

        // the following takes the next digit in account to properly round the value up or down
        //
        return static_cast<long long int>(std::round(v * pow(10.0, exponent) / 10.0));
    }

    static constexpr std::size_t BUFFER_SIZE = []() constexpr noexcept
                {
                    std::size_t len(2);
                    if(value <= 0.0)
                    {
                        ++len; // for the '-' or prefix '0'
                    }

                    for(auto v(whole()); v != 0; ++len, v /= 10);

                    // at the moment the fractional length is hard coded
                    // but we should not count the ending zeroes (and not
                    // insert them later)
                    //
                    //for(auto v(frac()); v != 0; ++len, v /= 10);
                    len += fractional_length();

                    return len;
                }();

private:
    CharT   f_buf[BUFFER_SIZE] = {};

public:
    /**
     * Constructs the object, filling `f_buf` with the string
     * representation of `value`.
     */
    constexpr floating_point_to_string_literal_impl() noexcept
    {
        CharT * ptr = end();

        --ptr;
        *ptr = '\0';

        // adding the fraction requires a specific count
        // (which means at the moment we add ending zeroes...)
        //
        int count(fractional_length());
        for(long long int v(frac()); count > 0; v /= 10, --count)
        {
            --ptr;
            *ptr = '0' + (value < 0.0 ? -1 : 1) * (v % 10);
        }

        --ptr;
        *ptr = '.';

        long long int n(whole());
        if(n != 0)
        {
            for(long long int v(n); v != 0; v /= 10)
            {
                --ptr;
                *ptr = '0' + (n < 0 ? -1 : 1) * (v % 10);
            }
        }
        else
        {
            --ptr;
            *ptr = '0';
        }

        if(value < 0.0)
        {
            --ptr;
            *ptr = '-';
        }

#ifdef _DEBUG
        if(ptr != begin())
        {
            std::terminate(); // wrong number of characters added.
        }
#endif
    }

    constexpr operator CharT * () noexcept
    {
        return f_buf;
    }

    constexpr operator CharT const * () const noexcept
    {
        return f_buf;
    }

    constexpr std::size_t length() const noexcept
    {
        return BUFFER_SIZE - 1;
    }

    constexpr std::size_t size() const noexcept
    {
        return BUFFER_SIZE;
    }

    constexpr CharT * data() noexcept
    {
        return f_buf;
    }

    constexpr CharT const * data() const noexcept
    {
        return f_buf;
    }

    constexpr CharT & operator [] (unsigned int i)
    {
        if(static_cast<std::size_t>(i) >= size())
        {
            throw std::range_error("index out of range");
        }

        return f_buf[i];
    }

    constexpr CharT const & operator [] (unsigned int i) const
    {
        if(static_cast<std::size_t>(i) >= size())
        {
            throw std::range_error("index out of range");
        }

        return f_buf[i];
    }

    constexpr CharT & front() noexcept
    {
        return f_buf[0];
    }

    constexpr CharT const & front() const noexcept
    {
        return f_buf[0];
    }

    constexpr CharT & back() noexcept
    {
        return f_buf[length()];
    }

    constexpr CharT const & back() const noexcept
    {
        return f_buf[length()];
    }

    // Iterators
    constexpr CharT * begin() noexcept
    {
        return f_buf;
    }

    constexpr CharT const * begin() const noexcept
    {
        return f_buf;
    }

    constexpr CharT * end() noexcept
    {
        return f_buf + size();
    }

    constexpr CharT const * end() const noexcept
    {
        return f_buf + size();
    }
};


} // namespace detail

/** \brief Convert an integer to a string at compile time.
 *
 * This template simplifies the conversion of an integer to a string at
 * compile time. It supports four parameters: the value to convert, the
 * base, whether to use uppercase or lowercase, and the type of characters
 * you want in the output.
 *
 * Any basic integral type is accepted: `char`, `short`, `int`, `long`,
 * `long long`, `__int128`, and their unsigned counterparts and other
 * extensions offered by your compiler (a.k.a. `size_t`).
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
 * \tparam value  The integer to convert to a string.
 * \tparam base  The base used to convert the value (2 to 36).
 * \tparam uppercase  Whether to use uppercase (true) or lowercase (false).
 * \tparam CharT  The character type of the output string.
 */
template<
      auto value
    , int base = 10
    , bool uppercase = false
    , typename CharT = char
    , std::enable_if_t<std::is_integral_v<decltype(value)>, int> = 0>
constexpr detail::integer_to_string_literal_impl<value, base, uppercase, CharT> integer_to_string_literal;


/** \brief Convert a floating point to a string at compile time.
 *
 * Simplifies use of `floating_point_to_string_literal_impl` for floating
 * points from `floating_point_to_string_literal_impl<N>()` to
 * `floating_point_to_string_literal<N>`.
 *
 * \tparam value  The floating point to convert to a string.
 * \tparam CharT  The character type of the output string.
 */
template<
      auto value
    , typename CharT = char>
constexpr detail::floating_point_to_string_literal_impl<value, CharT> floating_point_to_string_literal;


} // namespace snapdev
// vim: ts=4 sw=4 et
