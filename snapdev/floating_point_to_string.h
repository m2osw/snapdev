// Copyright (c) 2019-2025  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Create a list of files using glob().
 *
 * This template allows you to insert filenames from the output of a glob()
 * call to an STL container.
 */

// snapdev
//
//#include    "snapdev/pathinfo.h"
//#include    "snapdev/raii_generic_deleter.h"


// C++
//
#include    <cmath>
//#include    <memory>
#include    <string>


// C
//
//#include    <limits.h>
//#include    <stdlib.h>



namespace snapdev
{



template<
    typename F,
    class CharT,
    class Traits = std::char_traits<CharT>,
    class Allocator = std::allocator<CharT>>
std::basic_string<CharT, Traits, Allocator> floating_point_to_string(F value, bool keep_period = true)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
    if(std::isnan(value))
    {
        return std::basic_string<CharT, Traits, Allocator>("NaN");
    }
    else if(value == 0.0)
    {
        if(keep_period)
        {
            return std::basic_string<CharT, Traits, Allocator>("0.0");
        }
        return std::basic_string<CharT, Traits, Allocator>("0");
    }
    else if(std::isinf(value))
    {
        if(std::signbit(value))
        {
            return std::basic_string<CharT, Traits, Allocator>("-Infinity");
        }
        else
        {
            return std::basic_string<CharT, Traits, Allocator>("Infinity");
        }
    }
    else
    {
        std::basic_string<CharT, Traits, Allocator> str(std::to_string(value));
        if(str.find('.') != str.npos)
        {
            while(str.back() == '0')
            {
                str.pop_back();
            }
            if(str.back() == '.')
            {
                if(keep_period)
                {
                    str += '0';
                }
                else
                {
                    str.pop_back();
                }
            }
        }
        return str;
    }
#pragma GCC diagnostic pop
}



} // namespace snapdev
// vim: ts=4 sw=4 et
