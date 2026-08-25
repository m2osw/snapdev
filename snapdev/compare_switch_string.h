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
#include    <sstream>
#include    <stdexcept>
#include    <utility>



namespace snapdev
{



namespace detail
{



/** \brief Compare the last character of input.
 *
 * This function makes sure that the input string ends with '\0'.
 *
 * This function gets called automatically when the last character
 * gets checked (no more ARGS).
 *
 * \paramm[in] s  The string to check.
 *
 * \return true if *s is the null character.
 */
constexpr bool compare_switch_chars(char const * s)
{
    return *s == '\0';
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
template<class ...ARGS>
constexpr bool compare_switch_chars(char const * s, char c, ARGS ...args)
{
    if(*s != c)
    {
        return false;
    }
    return compare_switch_chars(s + 1, args...);
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
template<class... ARGS>
constexpr bool compare_switch_skip_one(char const * s, char c, ARGS ...args)
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
#else
    NOT_USED(c);
#endif
    return compare_switch_chars(s + 1, args...);
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
struct static_switch_string
{
    char data[N]{};

    constexpr static_switch_string(char const (&str)[N])
    {
        static_assert(N >= 3, "static_string() requires a string with at least 2 characters (plus the null terminator).");
        for(std::size_t i = 0; i < N; ++i)
        {
            data[i] = str[i];
        }
    }
};


/** \brief Convert the list of indexes to a list of characters.
 *
 * The compare_switch_string() function generates a list of indexes (index
 * sequence). Here we use that list to extract the individual characters
 * from the static string \p S.
 *
 * \tparam S  The static string input to compare against.
 *
 * \param[in] s  The string to compare the static string against.
 * \param[in] I  A sequence of indexes.
 *
 * \return true if the two strings are equal.
 */
template<static_switch_string S, std::size_t... I>
constexpr bool compare_switch_string_impl(char const * s, std::index_sequence<I...>)
{
    return compare_switch_skip_one(s, S.data[I]...);
}


template<static_switch_string S, std::size_t... I>
constexpr bool compare_full_string_impl(char const * s, std::index_sequence<I...>)
{
    return compare_switch_chars(s, S.data[I]...);
}



/** \brief Compare the last character of input.
 *
 * This function makes sure that the input string ends with '\0'.
 *
 * This function gets called automatically when the last character
 * gets checked (no more ARGS).
 *
 * \paramm[in] s  The string to check.
 *
 * \return true if *s is the null character.
 */
constexpr bool compare_upper_switch_chars(char const * s)
{
    return *s == '\0';
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
template<class ...ARGS>
constexpr bool compare_upper_switch_chars(char const * s, char c, ARGS ...args)
{
    char u(*s);
    if(u >= 'a' && u <= 'z')
    {
        u &= 0xDF;
    }
    if(u != c)
    {
        return false;
    }
    return compare_upper_switch_chars(s + 1, args...);
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
template<class... ARGS>
constexpr bool compare_upper_switch_skip_one(char const * s, char c, ARGS ...args)
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
#else
    NOT_USED(c);
#endif
    return compare_upper_switch_chars(s + 1, args...);
}


/** \brief Structure used to transform the string in a sequence of characters.
 *
 * Although the input string is an array of characters (char[]), using
 * this structure allows us to convert the data field in a sequence
 * of characters using `std::make_index_sequence<>`
 *
 * If the static string is not all uppercase, the constructor ensures that
 * those characters do get saved (and later tested) using uppercase only.
 *
 * \tparam N  The number of characters in the static string including the
 *            null terminator.
 */
template<std::size_t N>
struct static_upper_switch_string
{
    char data[N]{};

    constexpr static_upper_switch_string(char const (&str)[N])
    {
        static_assert(N >= 3, "static_string() requires a string with at least 2 characters (plus the null terminator).");
        for(std::size_t i = 0; i < N; ++i)
        {
            if(str[i] >= 'a' && str[i] <= 'z')
            {
                // force uppercase
                //
                data[i] = str[i] & 0xDF;
            }
            else
            {
                data[i] = str[i];
            }
        }
    }
};


/** \brief Convert the list of indexes to a list of characters.
 *
 * The compare_upper_switch_string() function generates a list of indexes (index
 * sequence). Here we use that list to extract the individual characters
 * from the static string \p S.
 *
 * \tparam S  The static string input to compare against.
 *
 * \param[in] s  The string to compare the static string against.
 * \param[in] I  A sequence of indexes.
 *
 * \return true if the two strings are equal.
 */
template<static_upper_switch_string S, std::size_t... I>
constexpr bool compare_upper_switch_string_impl(char const * s, std::index_sequence<I...>)
{
    return compare_upper_switch_skip_one(s, S.data[I]...);
}


template<static_upper_switch_string S, std::size_t... I>
constexpr bool compare_upper_full_string_impl(char const * s, std::index_sequence<I...>)
{
    return compare_upper_switch_chars(s, S.data[I]...);
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
 * \return true if both strings are equal.
 */
template<detail::static_switch_string S>
constexpr bool compare_switch_string(char const * s)
{
    // Deduct size minus 1 to avoid the '\0'
    return detail::compare_switch_string_impl<S>(s, std::make_index_sequence<sizeof(S.data) - 1>{});
}


template<detail::static_switch_string S>
constexpr bool compare_switch_string(std::string const & s)
{
    // Deduct size minus 1 to avoid the '\0'
    return detail::compare_switch_string_impl<S>(s.c_str(), std::make_index_sequence<sizeof(S.data) - 1>{});
}


template<detail::static_switch_string S>
constexpr bool compare_full_string(char const * s)
{
    // Deduct size minus 1 to avoid the '\0'
    return detail::compare_full_string_impl<S>(s, std::make_index_sequence<sizeof(S.data) - 1>{});
}


template<detail::static_switch_string S>
constexpr bool compare_full_string(std::string const & s)
{
    // Deduct size minus 1 to avoid the '\0'
    return detail::compare_full_string_impl<S>(s.c_str(), std::make_index_sequence<sizeof(S.data) - 1>{});
}


/** \brief This is the same as the compare_switch_string() function in uppercase.
 *
 * Assuming your input string may be upper or lower case, this function
 * compares using upper case only by converting the input string 'a' to
 * 'z' to the corresponding 'A' to 'Z'.
 *
 * This assumes that the input string is already all in upper case or the
 * compare will always fail.
 *
 * \tparam S  A static string to compare against.
 *
 * \param[in] s  The input string to compare against the static string \p S.
 *
 * \return true if both strings are equal.
 */
template<detail::static_upper_switch_string S>
constexpr bool compare_upper_switch_string(char const * s)
{
    // Deduct size minus 1 to avoid the '\0'
    return detail::compare_upper_switch_string_impl<S>(s, std::make_index_sequence<sizeof(S.data) - 1>{});
}


template<detail::static_upper_switch_string S>
constexpr bool compare_upper_switch_string(std::string const & s)
{
    return detail::compare_upper_switch_string_impl<S>(s.c_str(), std::make_index_sequence<sizeof(S.data) - 1>{});
}


template<detail::static_upper_switch_string S>
constexpr bool compare_upper_full_string(char const * s)
{
    // Deduct size minus 1 to avoid the '\0'
    return detail::compare_upper_full_string_impl<S>(s, std::make_index_sequence<sizeof(S.data) - 1>{});
}


template<detail::static_upper_switch_string S>
constexpr bool compare_upper_full_string(std::string const & s)
{
    return detail::compare_upper_full_string_impl<S>(s.c_str(), std::make_index_sequence<sizeof(S.data) - 1>{});
}



} // namespace snapdev
// vim: ts=4 sw=4 et
