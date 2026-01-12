// Copyright (c) 2006-2026  Made to Order Software Corp.  All Rights Reserved
//
// https://snapwebsites.org/
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

// snapcatch2
//
#include    <catch2/snapcatch2.hpp>


// C++
//
#include    <string>
#include    <cstring>
#include    <cstdlib>
#include    <iostream>


// The message() can be used to verify that version
//
//#include <snapdev/stringize.h>
//#pragma message("Building unit tests with Catch2 version " SNAPDEV_STRINGIZE(CATCH_VERSION_MAJOR) "." SNAPDEV_STRINGIZE(CATCH_VERSION_MINOR) "." SNAPDEV_STRINGIZE(CATCH_VERSION_PATCH))

//#pragma GCC diagnostic pop


namespace SNAP_CATCH2_NAMESPACE
{



class obj_setenv
{
public:
    obj_setenv(const std::string& var)
        : f_copy(strdup(var.c_str()))
    {
        putenv(f_copy);
        std::string::size_type p(var.find_first_of('='));
        f_name = var.substr(0, p);
    }
    obj_setenv(obj_setenv const & rhs) = delete;
    obj_setenv & operator = (obj_setenv const & rhs) = delete;
    ~obj_setenv()
    {
        putenv(strdup((f_name + "=").c_str()));
        free(f_copy);
    }

private:
    char *          f_copy = nullptr;
    std::string     f_name = std::string();
};


inline char32_t rand_char(bool full_range = false)
{
    char32_t const max((full_range ? 0x0110000 : 0x0010000) - (0xE000 - 0xD800));

    char32_t wc;
    do
    {
        wc = ((rand() << 16) ^ rand()) % max;
    }
    while(wc == 0);
    if(wc >= 0xD800)
    {
        // skip the surrogates
        //
        wc += 0xE000 - 0xD800;
    }

    return wc;
}


inline std::int64_t rand_int64()
{
    return (static_cast<std::int64_t>(rand()) << 48)
         | (static_cast<std::int64_t>(rand()) << 32)
         | (static_cast<std::int64_t>(rand()) << 16)
         | (static_cast<std::int64_t>(rand()) <<  0);
}



}
// unittest namespace
// vim: ts=4 sw=4 et
