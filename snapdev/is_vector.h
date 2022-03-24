// Copyright (c) 2022  Made to Order Software Corp.  All Rights Reserved
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
#pragma once

// C++ lib
//
#include    <vector>



namespace snapdev
{


/** \brief Check whether you have a vector with elements of type T.
 *
 * We use a base template and a template specialization to discover whether
 * an object is a vector or not. The type of the elements is arbitrary so
 * we need to specify that parameter.
 *
 * This implementation does not check whether the vector is of a specific
 * type. To do so, you can use the std::is_same<> template as in:
 *
 * \code
 *     std::is_same<std::vector<int>, T>::value
 * \endcode
 *
 * \tparam T  The type of objects found in the vector.
 */
template<typename T>
struct is_vector
{
    static const bool value = false;
};


/** \brief Check whether you have a vector with elements of type T.
 *
 * This is a specialized template which checks whether the type is
 * a vector with elements of type T.
 *
 * This specialized template represents true.
 *
 * \tparam T  The type of objects found in the vector.
 */
template<typename T>
struct is_vector<std::vector<T>>
{
    static const bool value = true;
};



} // namespace snapdev
// vim: ts=4 sw=4 et
