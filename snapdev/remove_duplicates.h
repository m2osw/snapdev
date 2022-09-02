// Copyright (c) 2011-2022  Made to Order Software Corp.  All Rights Reserved
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
//
// Based on: http://stackoverflow.com/questions/236129/split-a-string-in-c#1493195
//
#pragma once

/** \file
 * \brief Template used to remove duplicates in a container.
 *
 * Some containers, such as a vector, allow for duplicates. This function
 * implements an algorithm to remove duplicates.
 *
 * \important
 * If your vector is already sorted, you may want to consider using the
 * erase/unique directly instead of calling this function which will
 * call sort() again.
 */

// C++
//
#include    <algorithm>



namespace snapdev
{


/** \brief Remove duplicates in the specified container.
 *
 * This function goes through the elements of your container after sorting
 * them and eliminates duplicates.
 *
 * This function works with vectors.
 *
 * \note
 * Maps and sets are already sorted and already do not include duplicates.
 * In other words, this function is useless for these containers.
 *
 * \warning
 * On return, the elements in the container are sorted.
 *
 * \tparam ContainerT  The type of string.
 * \param[in,out] container  The container where duplicates get removed.
 *
 * \return The reference to the input container (NOT A COPY).
 */
template<typename ContainerT>
ContainerT & remove_duplicates(ContainerT & container)
{
    std::sort(container.begin(), container.end());

    container.erase(std::unique(
                          container.begin()
                        , container.end())
                    , container.end());

    return container;
}



} // namespace snapdev
// vim: ts=4 sw=4 et
