// Check whether a set intersection is empty or not
// Copyright (c) 2019  Made to Order Software Corp.  All Rights Reserved
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

/** \file
 * \brief Check whether the intersetion of two sets is empty.
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

// C++ lib
//
#include <set>



namespace snap
{


/** \brief Check whether two sets have elements in common or not.
 *
 * This function search for elements in common between the two sets
 * passed as input. If none of the elements are equal, then the
 * function returns true (i.e. the intersection is empty.)
 *
 * \tparam K  The type of the sets key. Both sets must have the same key.
 * \tparam C  The comparator, std::less<K> by default.
 * \tparam A  The set allocator, std::allocator<K> by default.
 * \param[in] lhs  Left hand side set.
 * \param[in] rhs  Right hand side set.
 *
 * \return true if the intersection is empty, false otherwise.
 */
template<typename K, typename C = std::less<K>, typename A = std::allocator<K>>
bool empty_set_intersection(std::set<K, C, A> const & lhs, std::set<K, C, A> const & rhs)
{
    auto a(lhs.begin());
    auto b(rhs.begin());

    C c;
    while(a != lhs.end() && b != rhs.end())
    {
        if(*a == *b)
        {
            return false;
        }

        if(c(*a, *b))
        {
            ++a;
        }
        else
        {
            ++b;
        }
    }

    return true;
}



} // namespace snap
// vim: ts=4 sw=4 et
