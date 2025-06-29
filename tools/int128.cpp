// Copyright (c) 2022-2025  Made to Order Software Corp.  All Rights Reserved
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

// self
//
#include    <snapdev/ostream_int128.h>


// C++
//
#include    <iostream>


// C
//
#include    <string.h>


// last include
//
#include    <snapdev/poison.h>



// __int128 is not ISO C++ yet
#pragma GCC diagnostic ignored "-Wpedantic"



void powers_of_two()
{
    __int128 p(1);
    for(int i(0); i < 128; ++i)
    {
        std::cout << p << "\n";
        p <<= 1;
    }
}


int main(int argc, char * argv[])
{
    for(int i(1); i < argc; ++i)
    {
        if(strcmp(argv[i], "--powers-of-two") == 0)
        {
            powers_of_two();
        }
        else
        {
            std::cerr << "error: unknown option \""
                          << argv[i]
                          << "\"."
                          << std::endl;
        }
    }

    return 0;
}

// vim: ts=4 sw=4 et
