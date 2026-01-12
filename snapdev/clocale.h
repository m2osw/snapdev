// Copyright (c) 2025-2026  Made to Order Software Corp.  All Rights Reserved
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
 * \brief For your application to use the "C" locale.
 *
 * In most cases, we want backend servers to be running with "no locale".
 * The "C" locale is generally the best representation of such.
 *
 * We offer locale support through our libutf8 library. That uses the
 * Unicode library which supports everything one can possibly need and
 * is thread safe. Anything else should use this function in main()
 * to cancel all sorts of issues (i.e. date conversions is one notorious
 * one).
 */

// self
//
//#include    <snapdev/tokenize_format.h>



// libexcept
//
#include    <libexcept/exception.h>


// C++
//
//#include    <cmath>
//#include    <cstdint>
//#include    <iomanip>
//#include    <iostream>
//#include    <sstream>
//#include    <thread>
//#include    <type_traits>


// C
//
//#include    <langinfo.h>
//#include    <stdlib.h>
//#include    <string.h>
//#include    <sys/time.h>
//#include    <time.h>



namespace snapdev
{


DECLARE_MAIN_EXCEPTION(clocale_exception);

DECLARE_EXCEPTION(clocale_exception, setup_error); // LCOV_EXCL_LINE



/** \brief Setup the locale to "C" and timezone to ":UTC".
 *
 * This function sets the locale() to "C" and the timezone to ":UTC".
 * This means the system will behave as if we had no locale and
 * where running with no timezone. This is quite practical for
 * services and also tests to avoid time conversions which should
 * be avoided since servers are accessed by people anywhere in the
 * world.
 *
 * \exception setup_error
 * If the setlocale() function call fails, then this error is raised.
 * This should pretty much never happen since the "C" locale is always
 * valid on all Unices.
 */
inline void clocale()
{
    if(setlocale(LC_ALL, "C") == nullptr)
    {
        throw setup_error("could not set locale to \"C\".");
    }

    char const * tz(getenv("TZ"));
    if(tz == nullptr
    || strcmp(tz, ":UTC") != 0)
    {
        setenv("TZ", ":UTC", 1);
        tzset();
    }
}



} // namespace snapdev
// vim: ts=4 sw=4 et
