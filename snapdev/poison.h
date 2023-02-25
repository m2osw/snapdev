// Copyright (c) 2013-2023  Made to Order Software Corp.  All Rights Reserved
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
 * \brief List of functions we 100% forbid in the Snap! C++ environment.
 *
 * The function marked as poisoned are difficult to use without taking
 * the risk of generating bugs. For example, the strcat() function does
 * not verify that you do not overwrite data past the end of your
 * buffers. The strncat() is better since you specify a boundary, only
 * the size (n) is not the total size of the buffer, meaning that it
 * varies as the string length varies (i.e. buffer size - strlen(s)).
 *
 * In most cases, C++ offers much better alternatives for all of these
 * functions.
 */


// strcat() is really bad, use QString or std::string
// also forget about strcpy()
#pragma GCC poison strcat strncat wcscat wcsncat strcpy

// printf() is generally okay, but std::cerr/cout, sstream are safer
#pragma GCC poison printf   fprintf   sprintf   snprintf \
                   vprintf  vfprintf  vsprintf  vsnprintf \
                   wprintf  fwprintf  swprintf  snwprintf \
                   vwprintf vfwprintf vswprintf vswnprintf

// vim: ts=4 sw=4 et
