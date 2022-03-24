// Copyright (c) 2022  Made to Order Software Corp.  All Rights Reserved
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
#pragma once

// self
//
#include    <snapdev/not_used.h>


// C++
//
#include    <iostream>



namespace snapdev
{

/** \brief A debug/non-debug assert() which can include side effects.
 *
 * This function is safe to use without any special protection. The
 * expression to verify will be optimized out, except for any part which
 * may have side effects.
 *
 * \param[in] test_result  The value to be tested, if true, the assert passes.
 * \param[in] ... args  Arguments to pass to std::cerr for a user defined
 * message.
 */
template<typename ... Args>
void SAFE_ASSERT(bool test_result, Args && ... args)
{
#ifdef _DEBUG
    if(!test_result)
    {
        (std::cerr << ... << args);
        std::cerr << std::endl;
        std::terminate();
    }
#else
    NOT_USED(test_result, args...);
#endif
}



} // snap namespacedev
// vim: ts=4 sw=4 et
