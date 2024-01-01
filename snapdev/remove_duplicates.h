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
 * We offer several solutions to the problem of removing duplicates:
 *
 * \li The default algorithm which sorts the values first; this is the
 * \em normal algorithm for such a feature.
 * \li Consider using a container that automatically sorts such as
 * `std::set<T>` and `std::map<T,bool>`.
 * \li There are unsorted versions of containers that you may be able
 * to use for the purpose.
 */

// C++
//
#include    <algorithm>



namespace snapdev
{


/** \brief Remove duplicates in the unsorted container.
 *
 * This function goes through the elements of your container and first
 * sorts and then eliminates duplicates.
 *
 * This function works well with vectors.
 *
 * \note
 * Maps and sets are already sorted and already do not include duplicates.
 * In other words, this function is useless for such containers.
 *
 * \warning
 * On return, the elements in the container are sorted.
 *
 * \tparam ContainerT  The type of container to de-dup.
 * \param[in,out] container  The container where duplicates get removed.
 *
 * \return The reference to the input container (NOT A COPY).
 *
 * \sa sorted_remove_duplicates()
 * \sa unsorted_remove_duplicates()
 */
template<typename ContainerT>
ContainerT & sort_and_remove_duplicates(ContainerT & container)
{
    std::sort(container.begin(), container.end());

    container.erase(std::unique(
                          container.begin()
                        , container.end())
                    , container.end());

    return container;
}


/** \brief Remove duplicates in the sorted container.
 *
 * This function goes through the elements of your container and eliminates
 * duplicates. The input must be sorted for the algorithm to work properly.
 *
 * \note
 * Maps and sets are already sorted and already do not include duplicates.
 * In other words, this function is useless for such containers.
 *
 * \tparam ContainerT  The type of container to de-dup.
 * \param[in,out] container  The container where duplicates get removed.
 *
 * \return The reference to the input container (NOT A COPY).
 *
 * \sa sort_and_remove_duplicates()
 */
template<typename ContainerT>
ContainerT & sorted_remove_duplicates(ContainerT & container)
{
    container.erase(std::unique(
                          container.begin()
                        , container.end())
                    , container.end());

    return container;
}


/** \brief Function to replace duplicate objects.
 *
 * This function goes through the items of a container and overwrite
 * entries as duplicates are found.
 *
 * \warning
 * Like std::unique(), this function does not itself remove anything.
 * You must make sure to use the std::erase() to remove the last few
 * items. This is done by the unsorted_remove_duplicates() function.
 *
 * \rparam ForwardIterator  The type of iterator from your container.
 * \param[in] first  Forward iterator to the container's first element.
 * \param[in] end  Forward iterator to the container's last element + 1.
 *
 * \return The new end of your container.
 *
 * \sa https://stackoverflow.com/questions/49863158/c-remove-duplicate-values-from-a-vector-without-sorting
 * \sa unsorted_remove_duplicates()
 */
template<typename ForwardIterator>
ForwardIterator remove_duplicates(ForwardIterator first, ForwardIterator end)
{
    auto new_end(first);
    for(auto current(first); current != end; ++current)
    {
        if(std::find(first, new_end, *current) == new_end)
        {
            if(new_end != current)
            {
                *new_end = *current;
            }
            ++new_end;
        }
    }

    return new_end;
}


/** \brief Remove duplicates without sorting the container.
 *
 * This function goes through the elements of your container and eliminates
 * duplicates. The input does not have to be sorted and the returned function
 * does not change the order.
 *
 * The algorithm is O(n^2). If sorting is okay, then use the
 * sort_and_remove_duplicates() directly.
 *
 * \tparam ContainerT  The type of container to de-dup.
 * \param[in,out] container  The container where duplicates get removed.
 *
 * \return The reference to the input container (NOT A COPY).
 *
 * \sa remove_duplicates()
 */
template<typename ContainerT>
ContainerT & unsorted_remove_duplicates(ContainerT & container)
{
    container.erase(remove_duplicates(
                          container.begin()
                        , container.end())
                    , container.end());

    return container;
}



} // namespace snapdev
// vim: ts=4 sw=4 et
