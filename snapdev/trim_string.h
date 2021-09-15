// Copyright (c) 2011-2021  Made to Order Software Corp.  All Rights Reserved
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
//
#pragma once

/** \file
 * \brief Template used to trim a string.
 *
 * This function will trim the start, end, and inside of a string.
 *
 * The function works with any standard string. The characters to trim
 * are themselves to be defined in a string.
 *
 * The function also takes three flags to determine where the trimming
 * is to happen.
 */

// self
//
#include    "snapdev/reverse_cstring.h"


// C++ lib
//
#include    <string>
#include    <algorithm>



namespace snap
{


/** \brief Trim a string.
 *
 * This function goes through the input string and remove
 * \p trim_characters from the \p trim_start, the \p trim_end, and the
 * \p trim_inside.
 *
 * \todo
 * Add support for UTF-8 (in which case we need to move this to our
 * libutf8 library?).
 *
 * \tparam StringT  The type of container used to output the tokens.
 * \param[in] str  The string to trim.
 * \param[in] trim_start  Trim the start of the input string.
 * \param[in] trim_end  Trim the end of the input string.
 * \param[in] trim_inside  Trim the inside of the input string.
 * \param[in] trim_characters  Trim those characters from the start/inside/end.
 *
 * \return The trimmed string.
 */
template<class StringT>
StringT trim_string(
          StringT const & str
        , bool const trim_start = true
        , bool const trim_end = true
        , bool const trim_inside = false
        , StringT const & trim_characters = StringT())
{
    typename StringT::value_type const * start(str.data());
    typename StringT::value_type const * end(str.data() + str.length());

    if(trim_start)
    {
        start = std::find_if_not(
              start
            , end
            , [&trim_characters](auto const c)
              {
                  if(trim_characters.empty())
                  {
                      return std::iswspace(c) != 0;
                  }
                  return trim_characters.find(c) != StringT::npos;
              });
    }

    if(start < end
    && trim_end)
    {
        reverse_cstring<typename StringT::value_type const> const rstr(start, end);
        auto const p(std::find_if_not(
              rstr.begin()
            , rstr.end()
            , [&trim_characters](auto const c)
              {
                  if(trim_characters.empty())
                  {
                      return std::iswspace(c) != 0;
                  }
                  return trim_characters.find(c) != StringT::npos;
              }));
        end = p.get();
    }

    if(!trim_inside)
    {
        return StringT(start, end);
    }

    // in this case, we want to copy the characters one by one
    //
    bool space(false);
    StringT result;
    result.reserve(end - start);

    if(!trim_start)
    {
        for(; start < end && !space; ++start)
        {
            result += *start;
            if(trim_characters.empty())
            {
                space = std::iswspace(*start);
            }
            else
            {
                space = trim_characters.find(*start) != StringT::npos;
            }
        }
    }

    if(trim_characters.empty())
    {
        for(; start < end; ++start)
        {
            if(space)
            {
                space = std::iswspace(*start);
                if(!space)
                {
                    result += *start;
                }
            }
            else
            {
                space = std::iswspace(*start);
                if(space)
                {
                    result += ' ';
                }
                else
                {
                    result += *start;
                }
            }
        }
    }
    else
    {
        for(; start < end; ++start)
        {
            if(space)
            {
                space = trim_characters.find(*start) != StringT::npos;
                if(!space)
                {
                    result += *start;
                }
            }
            else
            {
                space = trim_characters.find(*start) != StringT::npos;
                if(space)
                {
                    result += ' ';
                }
                else
                {
                    result += *start;
                }
            }
        }
    }

    return result;
}

} // namespace snap
// vim: ts=4 sw=4 et
