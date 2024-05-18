// Copyright (c) 2011-2024  Made to Order Software Corp.  All Rights Reserved
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


/** \brief Check whether code is unexpectendly reached in a test.
 *
 * Some tests verify that code is being reached by generating all the
 * possible errors. This way we can eventually get full coverage unit
 * tests.
 *
 * At times, there is just no way to generate certain errors. For
 * example, when getting the current clock, the function may return
 * an error. However, in all likelihood, it will work 100% of the
 * time. So code checking such errors is generally put between
 * macros so they don't participate in coverage tests. However,
 * if that error just prints a message, it is likely to be transparent
 * to the unit test, even if covered.
 *
 * So to make sure that it is indeed not covered, we call this function.
 *
 * Here is an example:
 *
 * \code
 *     if(!f_connection->send_message(lock_message))
 *     {
 *         // LCOV_EXCL_START
 *         f_state = state_t::CLUCK_STATE_FAILED;
 *         set_reason(reason_t::CLUCK_REASON_TRANSMISSION_ERROR);
 *         snapdev::NOT_REACHED_IN_TEST();
 *         return false;
 *         // LCOV_EXCL_STOP
 *     }
 * \endcode
 *
 * If somehow with time the send_message() ends up returning false, this
 * code will execute and end up calling this NOT_REACHED_IN_TEST() function
 * which calls abort() after writing an error message when running a coverage
 * test.
 *
 * \note
 * The function does absolutely nothing when not building using the sanitizer
 * (which we expect is used when testing only).
 *
 * \note
 * The function is not marked as "no return" since it does return when not
 * running the coverage test. Actually, it does absolutely nothing in all
 * other cases.
 *
 * \note
 * If your ignored code always ends by raising an exception, then there is
 * no need to call this function. Your test should not expect that exception
 * so you'll know if the test fails in that unexpected way.
 */
inline void NOT_REACHED_IN_TEST()
{
#ifdef __SANITIZE_ADDRESS__
    std::cerr << "NOT_REACHED_TEST called, process will abort." << std::endl;

    std::cerr << "Stack trace:" << std::endl;
    libexcept::stack_trace_t trace(libexcept::collect_stack_trace_with_line_numbers());
    for(auto l : trace)
    {
        std::cerr << "  " << l << std::endl;
    }

   abort();
#endif
}



} // namespace snapdev
// vim: ts=4 sw=4 et
