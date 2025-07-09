// Copyright (c) 2016-2025  Made to Order Software Corp.  All Rights Reserved
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

// C++
//
#include    <algorithm>
#include    <string>
#include    <type_traits>
#include    <vector>



namespace snapdev
{

/** \brief Search needles in input string and replace with replacement strings.
 *
 * This function takes two parameters: a string and a vector of string pairs
 * representing a needle (first) and a replacement (second).
 *
 * The algorithm checks each needle at the current position, starting at
 * position 0. The first match gets replaced. What was replaced is not
 * checked any further (it is part of the output.)
 *
 * Therefore, if you have two needles one after another such as: "car" and
 * then "carpool", the second one will never match since whatever get
 * replaced does not participate in the next match and whenever a word
 * starts with "car" it matches the first pair and never has a chance
 * to hit the second pair. In other words, make sure you needles are in
 * the correct order (i.e. probably longest first.)
 *
 * \todo
 * Look into whether we can find a way to find all the possible replacements
 * in order to compute the output string length without having to do many
 * reallocations.
 *
 * \todo
 * Add another version which compares case insensitively.
 *
 * \todo
 * Add another version which uses regex to do the compares.
 *
 * \param[in] input  The input string where replacements will occur.
 * \param[in] search_and_replace  The search parameters, the vector of needles/replacement
 *                    strings.
 *
 * \return A new string with the search_and_replace applied to the input string.
 */
template<class StringT>
StringT string_replace_many(StringT const & input
                   , std::vector<std::pair<typename std::decay<StringT>::type,
                                           typename std::decay<StringT>::type>> search_and_replace)
{
    typename StringT::size_type pos(0);
    typename StringT::size_type const len(input.length());
    StringT result;

    while(pos < len)
    {
        auto const & match(std::find_if(
                search_and_replace.begin(),
                search_and_replace.end(),
                [input, len, pos](auto const & nr)
                {
                    // check whether we still have enough characters first
                    // and if so, compare against the input string for equality
                    //
                    if(len - pos >= nr.first.length()
                    && input.compare(pos, nr.first.length(), nr.first) == 0)
                    {
                        // we found a match so return true
                        //
                        return true;
                    }
                    return false;
                }));

        if(match == search_and_replace.end())
        {
            // no match found, copy the character as is
            //
            result += input[pos];
            ++pos;
        }
        else
        {
            // got a replacement, use it and then skip the matched
            // characters in the input string
            //
            result += match->second;
            if(match->first.empty())
            {
                // empty does not match anything, so keep the character
                //
                result += input[pos];
                ++pos;
            }
            else
            {
                pos += match->first.length();
            }
        }
    }

    return result;
} // LCOV_EXCL_LINE



} // namespace snapdev
// vim: ts=4 sw=4 et
