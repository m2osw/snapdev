// Copyright (c) 2022-2023  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Search for a needle in a memory buffer.
 *
 * This function searches for a buffer (needle) inside another buffer
 * (haystack).
 */

namespace snapdev
{



/** \brief Search memory haystack for a needle.
 *
 * This function searches for \p needle within \p haystack. If found, it
 * returns the position at which \p needle is in \p haystack as a byte
 * position from the start of \p haystack.
 *
 * If \p needle is not found in \p haystack then the function returns -1.
 *
 * \param[in] haystack  The memory buffer to search.
 * \param[in] haystack_size  The number of bytes inside the haystack.
 * \param[in] needle  The memory buffer to search inside \p haystack.
 * \param[in] needle_size  The size of the needle buffer.
 *
 * \return
 * -1 if \p needle is not found, its position inside \p haystack
 * otherwise as a multiple of sizeof(T).
 *
 * \sa
 * https://stackoverflow.com/questions/8584644/strstr-for-a-string-that-is-not-null-terminated#answer-30053496
 */
template<typename T>
int memsearch(
      T const * haystack
    , int haystack_size
    , T const * needle
    , int needle_size)
{
    haystack_size -= needle_size;
    for(int haystack_pos(0); haystack_pos <= haystack_size; ++haystack_pos, ++haystack)
    {
        int needle_pos(0);
        for(; needle_pos < needle_size; ++needle_pos)
        {
            if(haystack[needle_pos] != needle[needle_pos])
            {
                break;
            }
        }
        if(needle_pos == needle_size)
        {
            return haystack_pos;
        }
    }
    return -1;
}



} // namespace snapdev
// vim: ts=4 sw=4 et
