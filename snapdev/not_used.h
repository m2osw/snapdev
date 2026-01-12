// Copyright (c) 2011-2026  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Functions used to mark variables as unused.
 *
 * One drawback of C++ is that at times we have overloaded functions that
 * include parameters that end up not being used. By default, this generates
 * a warning in our environment. This is actually very useful to either
 * remove the unused parameter or notice the fact that we used the wrong
 * parameter somewhere in our function.
 *
 * There are times, though, when the parameter is expected to not be used
 * In those cases, this function is useful to \em hide the parameter:
 *
 * \code
 *     snapdev::NOT_USED(param);
 *     // or with multiple parameters
 *     snapdev::NOT_USED(param1, param2, ..., paramN);
 * \endcode
 */

namespace snapdev
{



inline constexpr void NOT_USED()
{
}


template <class T, class ...ARGS>
inline constexpr void NOT_USED(T && first, ARGS && ...args)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
    static_cast<void>(first);
#pragma GCC diagnostic pop
    NOT_USED(args...);
}



} // namespace snapdev
// vim: ts=4 sw=4 et
