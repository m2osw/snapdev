// Copyright (c) 2022-2026  Made to Order Software Corp.  All Rights Reserved.
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
 * \brief Classes to serialize and deserialize data to base64 encoding.
 *
 * This file implements a couple of functions used to encode a buffer
 * to base64.
 *
 * \todo
 * We should look at having a version that can be used with streams so
 * we do not need to have the input and output buffers entirely in
 * memory as now.
 */

// C++
//
#include    <string>
#include    <iostream>



namespace snapdev
{
namespace base64
{


namespace
{



constexpr char const g_to_base64[] =
{
    // 8x8 characters
    //
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/',
};


constexpr signed char const g_from_base64[] =
{
    62, -1, -1, -1, 63, 52, 53, 54,
    55, 56, 57, 58, 59, 60, 61, -1,
    -1, -1, -1, -1, -1, -1,  0,  1,
     2,  3,  4,  5,  6,  7,  8,  9,
    10, 11, 12, 13, 14, 15, 16, 17,
    18, 19, 20, 21, 22, 23, 24, 25,
    -1, -1, -1, -1, -1, -1, 26, 27,
    28, 29, 30, 31, 32, 33, 34, 35,
    36, 37, 38, 39, 40, 41, 42, 43,
    44, 45, 46, 47, 48, 49, 50, 51,
};


} // no name namespace



void encode(std::string const & in, std::string & out)
{
    // reset output (just in case)
    //
    out.clear();
    out.reserve((in.length() + 2) / 3 * 4);

    // WARNING: following algorithm does NOT take any line length
    //          in account; and it is deadly well optimized
    //
    unsigned char const *s(reinterpret_cast<unsigned char const *>(in.data()));
    for(std::size_t idx(0); idx < in.length(); )
    {
        // get 1 to 3 characters of input
        //
        out += g_to_base64[s[idx] >> 2]; // & 0x3F not required
        ++idx;
        if(idx < in.length())
        {
            out += g_to_base64[((s[idx - 1] << 4) & 0x30) | ((s[idx] >> 4) & 0x0F)];
            ++idx;
            if(idx < in.length())
            {
                // 24 bits of input uses 4 base64 characters
                //
                out += g_to_base64[((s[idx - 1] << 2) & 0x3C) | ((s[idx] >> 6) & 0x03)];
                out += g_to_base64[s[idx] & 0x3F];
                ++idx;
            }
            else
            {
                // 16 bits of input uses 3 base64 characters + 1 pad
                //
                out += g_to_base64[(s[idx - 1] << 2) & 0x3C];
                out += '=';
                break;
            }
        }
        else
        {
            // 8 bits of input uses 2 base64 characters + 2 pads
            out += g_to_base64[(s[idx - 1] << 4) & 0x30];
            out += "==";
            break;
        }
    }
}


/** \brief Decode a base64 input buffer.
 *
 * \todo
 * Add support for newlines.
 *
 * \param[in] in  The input base64 string to convert back to binary.
 * \param[out] out  The decoded output buffer.
 *
 * \return true if the entire input was valid base64, false otherwise
 */
bool decode(std::string const & in, std::string & out)
{
    // reset output (just in case)
    //
    out.clear();
    out.reserve(in.length() / 4 * 3); // may be 1 or 2 bytes smaller

    // WARNING: following algorithm does NOT take any line length
    //          in account; and it is deadly well optimized
    //
    int state(0);
    char value(0);
    unsigned char const *s(reinterpret_cast<unsigned char const *>(in.data()));
    for(std::size_t idx(0); idx < in.length(); ++idx, ++s)
    {
        char c(*s);
        if(c == '=')
        {
            break;
        }
        if(c < '+'
        || c > 'z')
        {
            return false;
        }
        signed char const fragment(g_from_base64[static_cast<unsigned char>(c - '+')]);
        if(fragment < 0)
        {
            return false;
        }
        switch(state)
        {
        case 0:
            value = fragment << 2; // get 6 bits
            break;

        case 1:
            value |= fragment >> 4; // get 2 bits
            out += value;
            value = fragment << 4; // keep 4 bits
            break;

        case 2:
            value |= fragment >> 2; // get 4 bits
            out += value;
            value = fragment << 6; // keep 2 bits
            break;

        case 3:
            value |= fragment; // get 6 bits
            out += value;
            break;

        }
        state = (state + 1) % 4;
    }

    return true;
}



} // namespace base64
} // namespace snapdev
// vim: ts=4 sw=4 et
