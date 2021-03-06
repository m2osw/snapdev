// Snap Websites Server -- check that code could not be reached
// Copyright (c) 2011-2019  Made to Order Software Corp.  All Rights Reserved
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
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#pragma once

// libexcept
//
#include <libexcept/exception.h>

// C++ lib
//
#include <iostream>

// C lib
//
#include <stdlib.h>



namespace snap
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

} // namespace snap
// vim: ts=4 sw=4 et
