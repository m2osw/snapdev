// Copyright (c) 2013-2023  Made to Order Software Corp.  All Rights Reserved
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

// C++
//
#include    <memory>



namespace snapdev
{


/** \brief Check whether T is a shared pointer.
 *
 * We use a base template and a template specialization to discover whether
 * an object is a shared pointer.
 *
 * The base template represents false.
 *
 * \tparam T  The type of object other than the specialized shared pointer.
 */
template<typename T>
struct is_shared_ptr
    : std::false_type
{
};


/** \brief Check whether T is a shared pointer.
 *
 * This is a specialized template which checks whether the pointer is
 * a shared pointer.
 *
 * This specialized template represents true.
 *
 * \tparam T  The type of pointer in the shared pointer.
 */
template<typename T>
struct is_shared_ptr<std::shared_ptr<T>>
    : std::true_type
{
};


/** \brief Check whether T is a weak pointer.
 *
 * We use a base template and a template specialization to discover whether
 * an object is a weak pointer.
 *
 * The base template represents false.
 *
 * \tparam T  The type of pointer in the weak pointer.
 */
template<typename T>
struct is_weak_ptr
    : std::false_type
{
};


/** \brief Check whether T is a weak pointer.
 *
 * This is a specialized template which checks whether the pointer is
 * a weak pointer.
 *
 * This specialized template represents true.
 *
 * \tparam T  The type of pointer in the weak pointer.
 */
template<typename T>
struct is_weak_ptr<std::weak_ptr<T>>
    : std::true_type
{
};


/** \brief Check whether T is a unique pointer.
 *
 * We use a base template and a template specialization to discover whether
 * an object is a unique pointer.
 *
 * The base template represents false.
 *
 * \tparam T  The type of pointer in the unique pointer.
 */
template<typename T>
struct is_unique_ptr
    : std::false_type
{
};


/** \brief Check whether T is a unique pointer.
 *
 * This is a specialized template which checks whether the unique is
 * a unique pointer.
 *
 * This specialized template represents true.
 *
 * \tparam T  The type of pointer in the unique pointer.
 */
template<typename T>
struct is_unique_ptr<std::unique_ptr<T>>
    : std::true_type
{
};



} // namespace snapdev
// vim: ts=4 sw=4 et
