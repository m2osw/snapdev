// Copyright (c) 2011-2024  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Join strings together.
 *
 * This template expects a container of strings and a string separator.
 * It accumulates the strings by concatenating them one after the
 * other and adding the separator between each string. The separator is
 * not added at the start or the end of the resulting string.
 *
 * You can also specified a start and end from your container if you
 * do not want to join all the strings defined in your container.
 *
 * If you want to do concatenation at compile time, use the string_view
 * templates instead of an std::string container.
 */

// C++
//
#include    <algorithm>
#include    <array>
#include    <numeric>
#include    <string>
#include    <string_view>



namespace snapdev
{

/** \brief Transform a set of strings in a string.
 *
 * This function concatenate all the strings from a container adding a
 * separator in between each. In effect, it does:
 *
 * \code
 *      s1 + sep + s2 + sep + s3...
 * \endcode
 *
 * If you do not need a separator, you can use the std::accumulate() function
 * although it is going to be slower (i.e. it will do a lot of realloc() since
 * it does not know how long the final string is going to be.)
 *
 * \note
 * The separator is added whether the token being added is empty or not.
 * If you do not want to add separators between empty strings, make sure
 * to remove them from your container first.
 *
 * \param[in] tokens  The container of strings.
 * \param[in] separator  The separator to add between each string.
 *
 * \return the number of items in the resulting container.
 */
template<class ContainerT>
typename ContainerT::value_type join_strings(
        ContainerT const & tokens
      , typename ContainerT::value_type const & separator)
{
    typename ContainerT::value_type result;

    // we have a special case because we want to access the first
    // item (see tokens[0] below) to make the for_each() simpler.
    //
    if(!tokens.empty())
    {
        // calculate the final size, which is way faster than reallocating
        // over and over again in the 'result += string' below
        //
        std::size_t const total_size(std::accumulate(
                  tokens.begin()
                , tokens.end()
                , static_cast<std::string>(separator).length() * (tokens.size() - 1)
                , [](std::size_t const & sum, typename ContainerT::value_type const & str)
                    {
                        return sum + static_cast<std::string>(str).length();
                    }));

        static_cast<std::string &>(result).reserve(total_size);

        // avoid special case in the loop
        // (i.e. no separator before the first token)
        //
        static_cast<std::string &>(result) += *tokens.begin();

        std::for_each(
                  std::next(tokens.begin())
                , tokens.end()
                , [separator, &result](auto const & s)
                        {
                            static_cast<std::string &>(result) += static_cast<std::string const &>(separator)
                                                                + static_cast<std::string const &>(s);
                        });
    }

    return result;
}


template<class InputIt>
typename std::iterator_traits<InputIt>::value_type join_strings(
        InputIt const & first
      , InputIt const & last
      , typename std::iterator_traits<InputIt>::value_type const & separator)
{
    typename std::iterator_traits<InputIt>::value_type result;

    // we have a special case because we want to access the first
    // item (see tokens[0] below) to make the for_each() simpler.
    //
    if(first != last)
    {
        // calculate the final size, which is way faster than reallocating
        // over and over again in the 'result += string' below
        //
        std::size_t const total_size(std::accumulate(
                              first
                            , last
                            , separator.length() * (std::distance(first, last) - 1)
                            , [](std::size_t const & sum, typename std::iterator_traits<InputIt>::value_type const & str)
                                {
                                    return sum + str.length();
                                }));

        result.reserve(total_size);

        // avoid special case in the loop
        // (i.e. no separator before the first or after the last token)
        //
        result += *first;

        std::for_each(
                  std::next(first)
                , last
                , [separator, &result](auto const & s)
                        {
                            result += separator + s;
                        });
    }

    return result;
}


namespace detail
{

/** \brief Join strings at compile time.
 *
 * Here is a template to concatate a set of `std::string_view`'s.
 *
 * If you have two or more string views that you want to join at compile
 * time, the join_string_views template uses this template and returns
 * an std::string_view.
 */
template<std::string_view const & ...strings>
struct join_string_views_impl
{
    // join all strings into a single std::array of chars
    //
    static constexpr auto concatenate() noexcept
    {
        constexpr std::size_t const len = (strings.size() + ... + 0);
        std::array<char, len + 1> arr{};
        auto append = [i = 0, &arr](auto const & s) mutable
        {
            for(auto c : s)
            {
                arr[i] = c;
                ++i;
            }
        };
        (append(strings), ...);
        arr[len] = 0;
        return arr;
    }

    // give the joined string static storage
    //
    static constexpr auto concatenated_strings = concatenate();

    // view as a std::string_view
    //
    static constexpr std::string_view value{
              concatenated_strings.data()
            , concatenated_strings.size() - 1
        };
};

/** \brief Join strings at compile time with a separator.
 *
 * This template is similar to the previous one, only it allows us to also
 * add a separator betweene each string.
 *
 * If you have two or more string views that you want to join with a
 * separator at compile time, the join_string_views_with_separator template
 * uses this template and returns an std::string_view.
 *
 * \tparam separator  The separator to add between each string
 * \tparam strings  The string views to concatanate
 */
template<
      std::string_view const & separator
    , std::string_view const & ...strings>
struct join_string_views_with_separator_impl
{
    // join all strings into a single std::array of chars
    //
    static constexpr auto concatenate() noexcept
    {
        constexpr std::size_t const count = sizeof...(strings); //std::tuple_size<std::tuple<strings...>>::value;
        constexpr std::size_t const len = (strings.size() + ... + 0) + separator.length() * (count - 1);
        std::array<char, len + 1> arr{};
        auto append = [i = 0, &arr](auto const & s) mutable
        {
            if(i != 0)
            {
                for(auto c : separator)
                {
                    arr[i] = c;
                    ++i;
                }
            }
            for(auto c : s)
            {
                arr[i] = c;
                ++i;
            }
        };
        (append(strings), ...);
        arr[len] = 0;
        return arr;
    }

    // give the joined string static storage
    //
    static constexpr auto concatenated_strings = concatenate();

    // view as a std::string_view
    //
    static constexpr std::string_view value{
              concatenated_strings.data()
            , concatenated_strings.size() - 1
        };
};

} // namespace detail


/** \brief Join string views at compile time.
 *
 * Whenever you create a string view (i.e. a string literal in C++17 and newer
 * compilers), you can concatenate them using the join_string_views template.
 *
 * Here is an example on how to use this template:
 *
 * \code
 *     // various strings
 *     //
 *     constexpr std::string_view hello = "Hello";
 *     constexpr std::string_view space = " ";
 *     constexpr std::string_view world = "world";
 *     constexpr std::string_view bang = "!";
 *
 *     // concatenated strings
 *     //
 *     constexpr std::string_view hello_world = join_string_views<hello, space, world, bang>;
 * \endcode
 *
 * \source
 * https://stackoverflow.com/questions/38955940
 */
template<std::string_view const & ...strings>
static constexpr auto join_string_views = detail::join_string_views_impl<strings...>::value;


/** \brief Join string views with a separator at compile time.
 *
 * Whenever you create a string view (i.e. a string literal in C++17 and newer
 * compilers), you can concatenate them using the
 * join_string_views_with_separator template.
 *
 * Here is an example on how to use this template:
 *
 * \code
 *     // various strings
 *     //
 *     constexpr std::string_view space = " ";
 *     constexpr std::string_view hello = "Hello";
 *     constexpr std::string_view every = "every";
 *     constexpr std::string_view body = "body";
 *
 *     // concatenated strings
 *     //
 *     constexpr std::string_view hello_everyone = join_string_views<space, hello, every, body>;
 * \endcode
 */
template<std::string_view const & separator, std::string_view const & ...strings>
static constexpr auto join_string_views_with_separator = detail::join_string_views_with_separator_impl<separator, strings...>::value;


} // namespace snapdev
// vim: ts=4 sw=4 et
