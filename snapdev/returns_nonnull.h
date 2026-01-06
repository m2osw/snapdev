// Copyright (c) 2016-2025  Made to Order Software Corp.  All Rights Reserved
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

// snapdev
//
#include    <algorithm>
#include    <functional>
#include    <iostream>
#include    <string>



namespace snapdev
{



DECLARE_MAIN_EXCEPTION(returns_nonnull_exception);

DECLARE_EXCEPTION(returns_nonnull_exception, found_nullptr);



/** \brief Verify that a pointer is not nullptr.
 *
 * This function explicitly verifies that a pointer is not a null pointer.
 * It is particularly useful when returning a pointer or when using a
 * pointer in a location where you expect the pointer to not be null.
 *
 * \code
 *     is_nonnull(p)->some_function();
 *        -- or --
 *     return is_nonnull(p);
 * \endcode
 *
 * Further, to help the compiler, we can use attributes which remove
 * some checks and assume that certain pointers are not nullptr.
 *
 * \code
 * // function returning a pointer, but never nullptr
 * T * my_func() __attribute__ (( __returns_nonnull__ ))
 * {
 *    ...
 *    return ptr;
 * }
 *
 * // function is never passed a null pointer
 * void my_funct( T * ptr) __attribute__ (( __nonnull__(1) ))
 * {
 *     ptr->some_function(); // ptr is not expected to be nullptr, no need to check
 * }
 * \endcode
 *
 * \exception found_nullptr
 * If the function detects that the pointer is a null pointer, then this
 * exception is raised.
 *
 * \param[in] ptr  The pointer to check and ensure as non-null.
 *
 * \return The input pointer.
 */
template<typename T>
T is_nonnull(T ptr)
{
    if(ptr == nullptr)
    {
        throw found_nullptr("this pointer is null, stopping now.");
    }

    return ptr;
}



} // namespace snapdev
// vim: ts=4 sw=4 et
