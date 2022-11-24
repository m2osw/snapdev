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
//
// Based on: http://stackoverflow.com/questions/236129/split-a-string-in-c#1493195
//
#pragma once

/** \file
 * \brief Template used to transform a string in tokens.
 *
 * This file includes a template used to search for delimiters used to
 * break a string in tokens. Each token is further trimmed and optionally
 * empty tokens are dropped.
 */

// self
//
#include    <snapdev/reverse_cstring.h>


// C++
//
#include    <string>
#include    <algorithm>



namespace snapdev
{


/** \brief Search for characters.
 *
 * This function is the default predicate which sees the delimiters
 * as an array of characters so the tokenization happens on any one
 * character that matches the characters in the list of delimiters.
 *
 * When none of the delimiters are found in the rest of the string,
 * the function returns ContainerT::value_type::npos which means
 * that the rest of the string is a token on its own.
 *
 * \tparam ContainerT  The type of string.
 * \param[in] str  The string being tokenized.
 * \param[in] delimiters  The string of delimiters.
 * \param[in] last_pos  Last position with a match.
 *
 * \return The position of another match or ContainerT::value_type::npos.
 */
template<typename ContainerT>
typename ContainerT::value_type::size_type character_predicate(
          typename ContainerT::value_type const & str
        , typename ContainerT::value_type const & delimiters
        , typename ContainerT::value_type::size_type & last_pos)
{
    typename ContainerT::value_type::size_type const pos(str.find_first_of(delimiters, last_pos));
    if(pos == ContainerT::value_type::npos)
    {
        last_pos = pos;
    }
    else
    {
        last_pos = pos + 1;
    }
    return pos;
}


/** \brief Search for the delimiter string.
 *
 * This function is the another predicate you can use with the
 * tokenize_string() function.
 *
 * This one views the \p delimiters as a one string delimiter. In
 * other words, there is only one delimiter in this case.
 *
 * When the string delimiter is not found in the rest of the string,
 * the function returns ContainerT::value_type::npos which means
 * that the rest of the string is a token on its own.
 *
 * \note
 * This is not the default, you must pass this function explicitly if
 * you want to use it.
 *
 * \param[in] str  The string being tokenized.
 * \param[in] delimiter  The string delimiter.
 * \param[in] last_pos  Last position with a match.
 *
 * \return The position of another match or ContainerT::value_type::npos.
 */
template<typename ContainerT>
typename ContainerT::value_type::size_type string_predicate(
          typename ContainerT::value_type const & str
        , typename ContainerT::value_type const & delimiter
        , typename ContainerT::value_type::size_type & last_pos)
{
    typename ContainerT::value_type::size_type const pos(str.find(delimiter, last_pos));
    if(pos == ContainerT::value_type::npos)
    {
        last_pos = pos;
    }
    else
    {
        last_pos = pos + delimiter.length();
    }
    return pos;
}


/** \brief Transform a string in a vector of strings.
 *
 * This function transforms a string to a vector a strings
 * as separated by the specified delimiters.
 *
 * The trim_empty parameter can be used to avoid empty entries,
 * either at the start, middle, or end.
 *
 * The default predicate, character_predicate(), searches the
 * input string for characters as found in the delimiters string.
 * If you need a more robust predicate, you can declare your own
 * function and pass it as the last parameter of the
 * tokenize_string() function.
 *
 * \note
 * If the tokens vector is not empty, the items of the string
 * being tokenized will be appended to the existing vector.
 *
 * \todo
 * Add support for quotation. Quoted sections may include delimiters.
 *
 * \tparam ContainterT  The type of container used to output the tokens.
 * \tparam PredicateT  The type of the predicate function.
 * \param[in,out] tokens  The container receiving the resulting strings.
 * \param[in] str  The string to tokenize.
 * \param[in] delimiters  The list of character delimiters.
 * \param[in] trim_empty  Whether to keep empty entries or not.
 * \param[in] trim_string  Trim those characters from the start/end before saving.
 * \param[in] compare_function  The function used to search for tokens.
 *
 * \return the number of items in the resulting container.
 */
template<class ContainerT, typename PredicateT = decltype(character_predicate<ContainerT>)>
size_t tokenize_string(ContainerT & tokens
                     , typename ContainerT::value_type const & str
                     , typename ContainerT::value_type const & delimiters
                     , bool const trim_empty = false
                     , typename ContainerT::value_type const & trim_string = typename ContainerT::value_type()
                     , PredicateT compare_function = &character_predicate<ContainerT>)
{
    for(typename ContainerT::value_type::size_type pos(0),
                                                   last_pos(0);
        pos != ContainerT::value_type::npos;
        )
    {
        typename ContainerT::value_type::size_type const start_pos(last_pos);
        pos = (*compare_function)(str, delimiters, last_pos);

        typename ContainerT::value_type::value_type const * start(str.data() + start_pos);
        typename ContainerT::value_type::value_type const * end(str.data() + (pos == ContainerT::value_type::npos ? str.length() : pos));

        if(start != end                 // if not (already) empty
        && !trim_string.empty())        // and there are characters to trim
        {
            // find first character not in trim_string
            //
            start = std::find_if_not(
                  start
                , end
                , [&trim_string](auto const c)
                  {
                      return trim_string.find(c) != ContainerT::value_type::npos;
                  });

            // find last character not in trim_string
            //
            if(start < end)
            {
                reverse_cstring<typename ContainerT::value_type::value_type const> const rstr(start, end);
                auto const p(std::find_if_not(
                      rstr.begin()
                    , rstr.end()
                    , [&trim_string](auto const c)
                      {
                          return trim_string.find(c) != ContainerT::value_type::npos;
                      }));
                end = p.get();
            }
        }

        if(start != end     // if not empty
        || !trim_empty)     // or user accepts empty
        {
            tokens.insert(tokens.end(), typename ContainerT::value_type(start, end - start));
        }
    }

    return tokens.size();
}

} // namespace snapdev
// vim: ts=4 sw=4 et
