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
//
#pragma once

/** \file
 * \brief Template used to manage a possibly growing buffer.
 *
 * Many low level C functions accept a simple buffer. You pass the pointer
 * to that buffer and its current size to that function. The function may
 * then fail if the buffer is too small.
 *
 * In many cases we can allocate a small buffer on the stack instead of
 * allocating memory. Over many instances, this saves many cycles. This
 * template is to be used to handle that case. It does allocate a buffer
 * on the stack and if too small, it then gets allocated on the heap
 * instead.
 */

// C++
//
//#include    <algorithm>
//#include    <string>



namespace snapdev
{


/** \brief Allocate a static buffer and enlarge it if necessary.
 *
 * This template allocates a static buffer on the stack. You can then try
 * running that C function and if it fails because of a buffer overflow
 * (i.e. the static buffer is too small), then you can ask the object
 * to enlarge the buffer and try calling the function again.
 *
 * If the function is capable of giving you the correct size, then you can
 * just pass that size to allocate the next buffer. If no such size is
 * return you can either double or increment with a set size.
 *
 * \tparam BufferT  The type of buffer.
 * \tparam SIZE  The size of static buffer.
 *
 * \return The string with only lowercase characters.
 */
template<class BufferT, int SIZE = 1024>
class static_to_dynamic_buffer
{
public:
    typedef BufferT *           buffer_type;

    static_to_dynamic_buffer()
    {
        static_assert(SIZE > 0);
    }

    static_to_dynamic_buffer(static_to_dynamic_buffer const & rhs) = delete;
    static_to_dynamic_buffer operator = (static_to_dynamic_buffer const & rhs) = delete;

    /** \brief Make sure to clean up.
     *
     * This class may allocate a buffer on the heap. The destructor makes
     * sure to clear that buffer if present.
     */
    ~static_to_dynamic_buffer()
    {
        delete [] f_pointer;
    }

    /** \brief Retrieve the pointer.
     *
     * This function returns the static or dynamic pointer.
     *
     * \return The static or dynamic pointer.
     *
     * \sa resize()
     * \sa size() const
     */
    buffer_type get() const
    {
        if(f_size == 0)
        {
            return const_cast<buffer_type>(f_buffer);
        }
        return f_pointer;
    }

    /** \brief The size of the buffer.
     *
     * This function returns the size of the buffer returned by the get()
     * function.
     *
     * \return The size of the buffer returned by the get() function.
     *
     * \sa get()
     * \sa resize()
     */
    std::size_t size() const
    {
        if(f_size == 0)
        {
            return SIZE;
        }
        return f_size;
    }

    /** \brief Double the size of the buffer.
     *
     * This function doubles the size of the buffer. The function uses the
     * size as spectified in the template parameters (no buffer was allocated
     * yet, so we return the size of the buffer) when no buffer was allocated
     * yet.
     */
    void double_size()
    {
        resize(f_size == 0 ? SIZE * 2 : f_size * 2);
    }

    /** \brief Increase the size.
     *
     * This function increases the size of the buffer by \p incr.
     *
     * \note
     * The \p incr parameter can be negative. If as a result the size is
     * smaller than the template size parameter.
     *
     * \param[in] incr  The number of bytes to add to the current size.
     */
    void increase_size(ssize_t incr)
    {
        resize(f_size == 0 ? SIZE + incr : f_size + incr);
    }

    /** \brief Resize the dynamic buffer.
     *
     * This function takes a \p sz parameter representing the new size of
     * the buffer. If \p sz is smaller or equal to the static buffer, the
     * dynamic buffer gets deallocated and the static buffer gets used.
     *
     * If the size is larger than the static buffer, then a dynamic buffer
     * is allocated and returned by the get() function.
     *
     * If the input size is negative then it is smaller than the template
     * size which must be positive.
     *
     * \param[in] sz  The new size.
     */
    void resize(ssize_t sz)
    {
        clear();
        if(sz > 0
        && sz > SIZE)
        {
            f_pointer = new BufferT[f_size];
            f_size = sz;
        }
    }

    /** \brief Clear the dynamic buffer.
     *
     * This function resets the dynamic buffer pointer.
     *
     * It also resets the size parameter to zero as it is at the start
     * before the buffer gets allocated.
     */
    void clear()
    {
        delete [] f_pointer;
        f_pointer = nullptr;
        f_size = 0;
    }

    /** \brief Return the size of the static buffer.
     *
     * This function returns the SIZE template parameter.
     */
    std::size_t static_buffer_size() const
    {
        return SIZE;
    }

private:
    BufferT                     f_buffer[SIZE] = {};
    std::size_t                 f_size = 0;
    buffer_type                 f_pointer = nullptr;
};



} // namespace snapdev
// vim: ts=4 sw=4 et
