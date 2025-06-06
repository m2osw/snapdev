// Copyright (c) 2011-2025  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Template used to transform ASCII 'a' to 'z' to uppercase.
 *
 * In many places, I like to transform ASCII to upper case very quickly.
 * Here is a function to do that inline.
 */

// C++
//
#include    <algorithm>
#include    <string>



namespace snapdev
{



/** \brief Transform ASCII characters (a-z) to uppercase.
 *
 * This function transforms a string ASCII characters (a-z) to uppercase.
 *
 * \tparam StringT  The type of container used to output the tokens.
 * \param[in] str  The string to trim.
 *
 * \return The string with only uppercase characters.
 */
template<class StringT>
StringT to_upper(StringT const & str)
{
    StringT result;

    result.reserve(str.length());
    for(auto const & c : str)
    {
        if(c >= 'a' && c <= 'z')
        {
            result += c & ~0x20;
        }
        else
        {
            result += c;
        }
    }

    return result;
}



} // namespace snapdev
// vim: ts=4 sw=4 et
