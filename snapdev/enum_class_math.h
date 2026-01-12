// Copyright (c) 2023-2026  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Functions to do additions/subtraction with enumerations.
 *
 * Very often, when I write tests, I have to double cast a variable to
 * do ++ or -- when that variable represents an enumeration.
 *
 * Here is a template that should help with that so the casts can be hidden.
 * It is expected to really only be used in our tests but is available
 * anywhere you need such a computation.
 */

// C++
//
#include    <type_traits>



/** \brief Add the specified value to an enumeration variable.
 *
 * This template allows you to add the specified \p offset to an
 * enumeration avoiding all the casting that are usually required.
 *
 * \warning
 * This is really only useful for enumerations which do not have gaps.
 * Otherwise you can end up with an intermediate value which does not
 * exist in the enumeration.
 *
 * \param[in] e  The enumeration to which offset gets added.
 * \param[in] offset  The value added to the enumeration.
 *
 * \return The input enumeration plus offset as that enumeration.
 */
template<typename T>
constexpr std::enable_if_t<std::is_enum<T>::value, T> operator + (T e, int offset)
{
    return static_cast<T>(static_cast<int>(e) + offset);
}


template<typename T>
constexpr std::enable_if_t<std::is_enum<T>::value, T> & operator += (T & e, int offset)
{
    return e = e + offset;
}


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
template<typename T>
constexpr std::enable_if_t<std::is_enum<T>::value, T> & operator ++ (T & e)
{
    e = e + 1;
    return e;
}


template<typename T>
constexpr std::enable_if_t<std::is_enum<T>::value, T> operator ++ (T & e, int)
{
    T old(e);
    e = e + 1;
    return old;
}
#pragma GCC diagnostic pop


/** \brief Subtract the specified value from an enumeration variable.
 *
 * This template allows you to subtract the specified \p offset from an
 * enumeration avoiding all the casting that are usually required.
 *
 * \warning
 * This is really only useful for enumerations which do not have gaps.
 * Otherwise you can end up with an intermediate value which does not
 * exist in the enumeration.
 *
 * \param[in] e  The enumeration from which offset gets subtracted.
 * \param[in] offset  The value subtracted from the enumeration.
 *
 * \return The input enumeration minus offset as that enumeration.
 */
template<typename T>
constexpr std::enable_if_t<std::is_enum<T>::value, T> operator - (T e, int offset)
{
    return static_cast<T>(static_cast<int>(e) - offset);
}


template<typename T>
constexpr std::enable_if_t<std::is_enum<T>::value, T> & operator -= (T & e, int offset)
{
    return e = e - offset;
}


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
template<typename T>
constexpr std::enable_if_t<std::is_enum<T>::value, T> & operator -- (T & e)
{
    e = e - 1;
    return e;
}


template<typename T>
constexpr std::enable_if_t<std::is_enum<T>::value, T> operator -- (T & e, int)
{
    T old(e);
    e = e - 1;
    return old;
}
#pragma GCC diagnostic pop



// vim: ts=4 sw=4 et
