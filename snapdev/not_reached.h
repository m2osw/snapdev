// Copyright (c) 2011-2022  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Functions used for sanity checks.
 *
 * Often, we have points in our code that cannot be logically reached. By
 * adding the snapdev::NOT_REACHED() function call at those location, you
 * ensures that if somehow those are actually reached, then the program
 * terminates since it is not expected to go there.
 */

// libexcept
//
#include    <libexcept/exception.h>


// C++
//
#include    <iostream>


// C
//
#include    <stdlib.h>



namespace snapdev
{

[[noreturn]] [[deprecated]] inline void NOTREACHED()
{
    std::cerr << "NOT_REACHED called, process will abort." << std::endl;

    std::cerr << "Stack trace:" << std::endl;
    libexcept::stack_trace_t trace(libexcept::collect_stack_trace_with_line_numbers());
    for(auto l : trace)
    {
        std::cerr << "  " << l << std::endl;
    }

    abort();
}

[[noreturn]] inline void NOT_REACHED()
{
    std::cerr << "NOT_REACHED called, process will abort." << std::endl;

    std::cerr << "Stack trace:" << std::endl;
    libexcept::stack_trace_t trace(libexcept::collect_stack_trace_with_line_numbers());
    for(auto l : trace)
    {
        std::cerr << "  " << l << std::endl;
    }

    abort();
}

} // namespace snapdev
// vim: ts=4 sw=4 et
