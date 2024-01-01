// Copyright (c) 2016-2024  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Enumerate a container.
 *
 * This template calls a function once per item in a container.
 *
 * The container has to support enumeration.
 */

// C++
//
#include    <functional>



namespace snapdev
{


/** \brief A function used to enumerate elements of a container.
 *
 * Once in a while it's practical to call a function for each element
 * of a container. This is a helper function which does just that
 * hiding the std::for_each call.
 *
 * The function definition has to look like this:
 *
 * \code
 *     func(int index, T::value_type const & item);
 * \endcode
 *
 * The index starts at 0 and increases by 1 for each item.
 *
 * \tparam T  The type of container to enumerate.
 * \param[in] func  The function to call with each item.
 */
template<class T>
void enumerate(T const & c, std::function<void (int, typename T::value_type)> func)
{
    int index(0);
    std::for_each(
          c.cbegin()
        , c.cend()
        , [&func, &index](auto const & item)
        {
            func(index, item);
            ++index;
        });
}



} // namespace snapdev
// vim: ts=4 sw=4 et
