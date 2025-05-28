// Copyright (c) 2019-2025  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Check whether the intersection of two sets is empty.
 *
 * This function simulates the computation of an intersection between two
 * sets to determine whether the intersection is empty or not.
 *
 * In other words, if two elements are equal, the function returns false
 * otherwise it returns true.
 *
 * The function will iterate at most n times where n is the minimum size
 * between the two sets and the intersection is the empty set.
 */

// C++
//
#include    <algorithm>
#include    <iterator>
#include    <type_traits>



namespace snapdev
{


namespace detail
{

template<typename T, typename F>
constexpr auto has_member_impl(F && f) -> decltype(f(std::declval<T>()), true) { return true; }

template<typename>
constexpr bool has_member_impl(...) { return false; }

}


/** \brief This function retrieves the keys of a map.
 *
 * This function is used to retreive the keys of a map in a different
 * container. At this, it is useful to have a copy of the keys.
 *
 * \note
 * Since this function creates a copy, you should only use it when a
 * copy is required. For example, we have a cppprocess::refresh()
 * function which refreshes the list of processes. So it does not
 * delete all the existing process_info objects it knows about which
 * allows for the already cached data to be retrieved again without
 * any I/O.
 *
 * \tparam C  The type of the output container such as a vector or set.
 * \tparam M  The type of map from which to extract the keys.
 * \param[in,out] keys  The container were the keys get added.
 * \param[in] m  The map from which to extract the keys.
 *
 * \sa https://stackoverflow.com/questions/110157/#answer-65378776
 */
template<class C, class M>
void map_keyset(C & keys, M const & m)
{
    std::transform(
          m.cbegin()
        , m.cend()
        , std::inserter(keys, keys.begin())
        , [](auto const & p) { return p.first; });
}


} // namespace snapdev
// vim: ts=4 sw=4 et
