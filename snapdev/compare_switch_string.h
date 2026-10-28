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
#pragma once

/** \file
 * \brief Utility function to compare against a static string in a switch.
 *
 * I often use a switch statement to match the first character of string
 * and then use a case for each first character. Then within that case
 * block, I check the rest of the string.
 *
 * The fastest way to compare a string in that case is to check each
 * character one at a time. For example, to compare the word TEST:
 *
 * \code
 *     switch(s[0])
 *     {
 *     ...
 *     case 'T':
 *     	   if(s[1] == 'E' && s[2] == 'S' && s[3] == 'T' && s[4] == '\0')
 *     	   {
 *     	       ... handle TEST case ...
 *     	   }
 *     ...
 *     }
 * \endcode
 *
 * The problem is that it's tedious and prone to mistake to type so many
 * tests in one if() statement. You may swap letters, use the wrong case,
 * forget to increment the index... many possibilities for mistakes.
 *
 * Here the template offers a way to enter the word as a string and the
 * compiler transforms it in a set of if() statement similar to the above.
 * The resulting assembly language is as optimized as possible.
 *
 * \note
 * If you compare really long strings, this is not as optimized as the
 * strcmp() function which has the ability to read 8 characters at a time
 * and compare them in one go.
 */

// self
//
#include    "snapdev/not_used.h"


// C++
//
#include    <cstdint>
#include    <sstream>
#include    <stdexcept>
#include    <utility>



namespace snapdev
{



namespace detail
{



/** \brief Comparison modes.
 *
 * This enumeration is used to defines how to compare the upper/lower case
 * characters and the end of the string.
 *
 * The "exact match" variant is usually what you want: i.e. `a == b`.
 *
 * The "starts with" variant means that this function always returns true.
 * The string must start with these characters and may also be exactly
 * equal.
 *
 * \code
 *     a[0] == b[0]
 *     a[1] == b[1]
 *     ...
 *     a[N] == b[N]
 *     a[N+1] == '\0' || a[N+1] != '\0'
 * \endcode
 *
 * The "perfect prefix" variant is similar to the "start with" except that
 * the string must go on. In other words, `a[N+1] != '\0'` must be true.
 *
 * The "case insensitive" flag is used to know whether to check letters
 * as is or in uppercase.
 *
 * The "switch" flag allows us to skip testing the very first character.
 * This is because if you already tested it in a switch statement, testing
 * it again is a waste of time and space:
 *
 * \code
 *     switch(s[0])
 *     {
 *     case 'A':
 *         if(compare_switch_string<"ABC">(s)) ...
 *     ...
 *     }
 * \endcode
 *
 * The `case 'A':` already checked `s[0]` so we can as well skip that.
 */
typedef std::uint32_t           compare_mode_t;

// how to check the last character
//
constexpr compare_mode_t        COMPARE_MODE_MASK             = 0x000F;
constexpr compare_mode_t        COMPARE_MODE_EXACT_MATCH      = 0x0001;
constexpr compare_mode_t        COMPARE_MODE_STARTS_WITH      = 0x0002;
constexpr compare_mode_t        COMPARE_MODE_PERFECT_PREFIX   = 0x0003;

// how to test letters
//
constexpr compare_mode_t        COMPARE_MODE_CASE_INSENSITIVE = 0x0010;

// whether to test the first letter
//
constexpr compare_mode_t        COMPARE_MODE_SWITCH           = 0x0020;



/** \brief Compare the last character of input.
 *
 * This function makes sure that the input string ends as expected according
 * to the specified \p mode.
 *
 * This function gets called automatically when the last character
 * gets checked (no more ARGS).
 *
 * \tparam mode  How to check the end of the string.
 * \param[in] s  The string to check.
 *
 * \return true if \p s ends as expected by \p mode.
 */
template<compare_mode_t mode>
constexpr bool compare_chars(char const * s)
{
    if constexpr ((mode & COMPARE_MODE_MASK) == COMPARE_MODE_EXACT_MATCH)
    {
        return *s == '\0';
    }

    if constexpr ((mode & COMPARE_MODE_MASK) == COMPARE_MODE_PERFECT_PREFIX)
    {
        return *s != '\0';
    }

    return true;
}


/** \brief Compare one character.
 *
 * This function checks one character (the first character in \p s against
 * \p c). If not equal, the function immediately returns false.
 *
 * If the characters are equal, then the function gets called again with
 * the next character.
 *
 * If there is no next character (\p args is empty), then the specialization
 * of the function gets called and verifies that the input string ends with
 * a null character.
 *
 * \param[in] s  The string to check.
 * \param[in] c  The character to check.
 * \param[in] args  The following characters.
 *
 * \return true if the strings are equal.
 */
template<compare_mode_t mode, class ...ARGS>
constexpr bool compare_chars(char const * s, char const c, ARGS ...args)
{
    if constexpr ((mode & COMPARE_MODE_CASE_INSENSITIVE) != 0)
    {
        if((*s & 0xDF) != c && *s != c)
        {
            return false;
        }
    }
    else
    {
        if(*s != c)
        {
            return false;
        }
    }
    return compare_chars<mode>(s + 1, args...);
}


/** \brief Skip one character.
 *
 * This makes the first character \em transparent. That is, it does not
 * get compared. Since we already know what the first character is (i.e.
 * the letter in the `case` of the switch we're in).
 *
 * As a result, the assembly just directly accesses s[1] instead of starting
 * the compare at s[0].
 *
 * \tparam ARGS  The type of the \p args parameter (a.k.a. char).
 *
 * \param[in] s  The input string to compare.
 * \param[in] c  The very first character.
 * \param[in] args  The list of characters to compare against, except the first.
 *
 * \return true if all the characters match the input string in sequence.
 */
template<compare_mode_t mode, class... ARGS>
constexpr bool compare_switch_skip_one(char const * s, char c, ARGS ...args)
{
    if constexpr ((mode & COMPARE_MODE_SWITCH) != 0)
    {
#ifdef _DEBUG
        if(*s != c)
        {
            std::stringstream ss;
            ss << "first letter invalid: "
               << c
               << " <-> "
               << s
               << '.';
            throw std::runtime_error(ss.str());
        }
#endif
        return compare_chars<mode>(s + 1, args...);
    }

    return compare_chars<mode>(s + 1, c, args...);
}


/** \brief Structure used to transform the string in a sequence of characters.
 *
 * Although the input string is an array of characters (char[]), using
 * this structure allows us to convert the data field in a sequence
 * of characters using `std::make_index_sequence<>`
 *
 * \tparam N  The number of characters in the static string including the
 *            null terminator.
 */
template<std::size_t N>
struct static_string
{
    char data[N - 1]{};

    constexpr static_string(char const (&str)[N])
    {
        for(std::size_t i(0); i < N - 1; ++i)
        {
            data[i] = str[i];
        }
    }

    template<compare_mode_t mode>
    constexpr static_string<N> fix() const
    {
        if constexpr ((mode & COMPARE_MODE_SWITCH) != 0)
        {
            static_assert(N >= 3, "static_string() requires a string with at least 2 characters when used with a switch flavor.");
        }
        else
        {
            static_assert(N >= 2, "static_string() requires a string with at least 1 character.");
        }

        static_string<N> copy = *this;

        if constexpr ((mode & COMPARE_MODE_CASE_INSENSITIVE) != 0)
        {
            for(std::size_t i(0); i < N - 1; ++i)
            {
                if(copy.data[i] >= 'a' && copy.data[i] <= 'z')
                {
                    copy.data[i] &= 0xDF;
                }
            }
        }

        return copy;
    }
};


/** \brief Convert the list of indexes to a list of characters.
 *
 * The compare_switch_string() function generates a list of indexes (index
 * sequence). Here we use that list to extract the individual characters
 * from the static string \p S.
 *
 * \tparam mode  How to compare the strings.
 * \tparam S  The static string input to compare against.
 *
 * \param[in] s  The string to compare the static string against.
 * \param[in] I  A sequence of indexes.
 *
 * \return true if the two strings are equal.
 */
template<compare_mode_t mode, static_string S, std::size_t... I>
constexpr bool compare_string_impl(char const * s, std::index_sequence<I...>)
{
    return compare_switch_skip_one<mode>(s, S.data[I]...);
}



} // namespace detail



/** \brief Compare two strings against each other.
 *
 * \warning
 * The first character of the two input strings is expected to already have
 * been compared and known to be equal. This function does NOT check that
 * character again. This is verified in your debug version (using _DEBUG).
 *
 * This function compares the input string \p s against the static string
 * \p S. If equal, then the function returns true. Otherwise it returns
 * false.
 *
 * This is written so that the compiler can optimize the comparison to
 * just a set of `cmp` and `jump/branch` instructions. For example, it
 * could look like this in amd64:
 *
 * \code
 *     // 'U' already checked, skip
 *     mov    0x8(%rbp),%sdi
 *     xor    %eax,%eax
 *     cmpb   $0x4e,0x2(%sdi) // N
 *     jne    done
 *     cmpb   $0x44,0x2(%sdi) // D
 *     jne    done
 *     cmpb   $0x4f,0x3(%sdi) // O
 *     jne    done
 *     cmpb   $0x0,0x4(%sdi) // '\0'
 *     sete   %al
 * done:
 *     ret
 * \endcode
 *
 * \tparam S  The static string to compare against the dynamic string.
 *
 * \param[in] s  The dynamic string to compare against the static string.
 *
 * \return true if both strings are equal.
 */


#define COMPARE_SWITCH_STRING_IMPL(name, mode) \
    template<detail::static_string S> \
    constexpr bool compare_##name##_string(char const * s) \
    { \
        constexpr auto Q = S.template fix<mode>(); \
        return detail::compare_string_impl<mode, Q>(s, std::make_index_sequence<sizeof(Q.data)>{}); \
    } \
    template<detail::static_string S> \
    constexpr bool compare_##name##_string(std::string const & s) \
    { \
        constexpr auto Q = S.template fix<mode>(); \
        return detail::compare_string_impl<mode, Q>(s.c_str(), std::make_index_sequence<sizeof(Q.data)>{}); \
    }

#define COMPARE_SWITCH_STRING(name, mode) \
    COMPARE_SWITCH_STRING_IMPL(name, mode) \
    COMPARE_SWITCH_STRING_IMPL(name##_switch, mode | detail::COMPARE_MODE_SWITCH) \
    COMPARE_SWITCH_STRING_IMPL(upper_##name, mode | detail::COMPARE_MODE_CASE_INSENSITIVE) \
    COMPARE_SWITCH_STRING_IMPL(upper_##name##_switch, mode | detail::COMPARE_MODE_CASE_INSENSITIVE | detail::COMPARE_MODE_SWITCH) \

COMPARE_SWITCH_STRING(exact,          detail::COMPARE_MODE_EXACT_MATCH)
COMPARE_SWITCH_STRING(starts_with,    detail::COMPARE_MODE_STARTS_WITH)
COMPARE_SWITCH_STRING(perfect_prefix, detail::COMPARE_MODE_PERFECT_PREFIX)



} // namespace snapdev
// vim: ts=4 sw=4 et
