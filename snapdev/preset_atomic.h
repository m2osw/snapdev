// Copyright (c) 2026  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Class extending std::atomic with specific memory orders.
 *
 * The C++ standard library provides an std::atomic which allows us
 * to read and write values atomically. However, when a value read
 * and write can all be relaxed, for example, we have to use the
 * heavy syntax:
 *
 * \code
 *     a = value.load(std::memory_order::relaxed);
 *     value.store(a, std::memory_order::relaxed);
 * \endcode
 *
 * If you have hundred of atomic variables, that becomes quite tedious.
 * This class helps automate the default memory order.
 *
 * \code
 *     // create a totally relaxed field
 *     //
 *     snapdev::atomic<
 *               int
 *             , std::memory_order::relaxed
 *             , std::memory_order::relaxed
 *             , std::memory_order::relaxed>    f_field{123}
 *
 *     a = f_field; // equivalent to f_field.load(std::memory_order::relaxed);
 *     f_field = a; // equivalent to f_field.store(a, std::memory_order::relaxed);
 *     ++f_field;   // equivalent to f_field.fetch_add(1, std::memory_order::relaxed);
 * \endcode
 *
 * The template uses three `memory_order` entries. One for the READ,
 * one for the WRITE, and one for READ-MODIFY-WRITE.
 *
 * The first is used by `operator T()`.
 *
 * The second is used by the `T operator = (T)`.
 *
 * The READ-MODIFY-WRITE is used by the operator ++ (), operator -- (),
 * operator += (), and operator -= ().
 *
 * All the standard library functions of std::atomic<T> are available
 * to you directly from the library. We only overloaded the operators
 * that would default to the `std::memory_order::seq_cst` `memory_order`.
 *
 * Note: if you don't specify the `memory_order` then you get the
 *       default with this template as well.
 *
 * The template defines three new functions used to determine the
 * `memory_order` selected in the template:
 *
 * \code
 * std::memory_order get_read_memory_order() const;
 * std::memory_order get_write_memory_order() const
 * std::memory_order get_read_modify_write_memory_order() const
 * \endcode
 *
 * It is not possible to change the memory order at run time.
 *
 * \warning
 * The atomic template accepts volatile integers and floating points.
 * To define such a value, you must place the volatile in the wrong
 * of the definition like so:
 *
 * \code
 * volatile snapdev::atomic<int> var;
 * \endcode
 */

// C++
//
#include    <atomic>



namespace snapdev
{



template<
      typename T
    , std::memory_order read_order = std::memory_order::seq_cst
    , std::memory_order write_order = std::memory_order::seq_cst
    , std::memory_order read_modify_write_order = write_order
>
requires
       std::is_integral_v<T>
    || std::is_floating_point_v<T>
    || std::is_pointer_v<T>
class preset_atomic
    : public std::atomic<T>
{
public:
    operator T() const noexcept
    {
        return std::atomic<T>::load(read_order);
    }

    operator T() const volatile noexcept
    {
        return std::atomic<T>::load(read_order);
    }

    bool store_if_smaller(T const v) noexcept
    {
        for(T current_value(std::atomic<T>::load(read_order)); current_value > v;)
        {
            if(std::atomic<T>::compare_exchange_weak(
                      current_value
                    , v
                    , read_modify_write_order))
            {
                return true;
            }
        }
        return false;
    }

    bool store_if_smaller(T const v) volatile noexcept
    {
        for(T current_value(std::atomic<T>::load(read_order)); current_value > v;)
        {
            if(std::atomic<T>::compare_exchange_weak(
                      current_value
                    , v
                    , read_modify_write_order))
            {
                return true;
            }
        }
        return false;
    }

    bool store_if_larger(T const v) noexcept
    {
        for(T current_value(std::atomic<T>::load(read_order)); current_value < v;)
        {
            if(std::atomic<T>::compare_exchange_weak(
                      current_value
                    , v
                    , read_modify_write_order))
            {
                return true;
            }
        }
        return false;
    }

    bool store_if_larger(T const v) volatile noexcept
    {
        for(T current_value(std::atomic<T>::load(read_order)); current_value < v;)
        {
            if(std::atomic<T>::compare_exchange_weak(
                      current_value
                    , v
                    , read_modify_write_order))
            {
                return true;
            }
        }
        return false;
    }

// effective C++ verifies that we `return *this` but for atomic
// we return T which is unusual but that is the expected interface
//
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
    T operator = (T const v) noexcept
    {
        std::atomic<T>::store(v, write_order);
        return v;
    }

    T operator = (T const v) volatile noexcept
    {
        std::atomic<T>::store(v, write_order);
        return v;
    }

    T operator += (T arg) noexcept
    requires std::is_integral_v<T> || std::is_floating_point_v<T>
    {
        return std::atomic<T>::fetch_add(arg, read_modify_write_order) + arg;
    }

    T operator += (T arg) volatile noexcept
    requires std::is_integral_v<T> || std::is_floating_point_v<T>
    {
        return std::atomic<T>::fetch_add(arg, read_modify_write_order) + arg;
    }

    T operator -= (T arg) noexcept
    requires std::is_integral_v<T> || std::is_floating_point_v<T>
    {
        return std::atomic<T>::fetch_sub(arg, read_modify_write_order) - arg;
    }

    T operator -= (T arg) volatile noexcept
    requires std::is_integral_v<T> || std::is_floating_point_v<T>
    {
        return std::atomic<T>::fetch_sub(arg, read_modify_write_order) - arg;
    }

    T operator ++ () noexcept
    requires std::is_integral_v<T> || std::is_floating_point_v<T>
    {
        return std::atomic<T>::fetch_add(T(1), read_modify_write_order) + T(1);
    }

    T operator ++ () volatile noexcept
    requires std::is_integral_v<T> || std::is_floating_point_v<T>
    {
        return std::atomic<T>::fetch_add(T(1), read_modify_write_order) + T(1);
    }

    T operator ++ (int) noexcept
    requires std::is_integral_v<T> || std::is_floating_point_v<T>
    {
        return std::atomic<T>::fetch_add(T(1), read_modify_write_order);
    }

    T operator ++ (int) volatile noexcept
    requires std::is_integral_v<T> || std::is_floating_point_v<T>
    {
        return std::atomic<T>::fetch_add(T(1), read_modify_write_order);
    }

    T operator -- () noexcept
    requires std::is_integral_v<T> || std::is_floating_point_v<T>
    {
        return std::atomic<T>::fetch_sub(T(1), read_modify_write_order) - T(1);
    }

    T operator -- () volatile noexcept
    requires std::is_integral_v<T> || std::is_floating_point_v<T>
    {
        return std::atomic<T>::fetch_sub(T(1), read_modify_write_order) - T(1);
    }

    T operator -- (int) noexcept
    requires std::is_integral_v<T> || std::is_floating_point_v<T>
    {
        return std::atomic<T>::fetch_sub(T(1), read_modify_write_order);
    }

    T operator -- (int) volatile noexcept
    requires std::is_integral_v<T> || std::is_floating_point_v<T>
    {
        return std::atomic<T>::fetch_sub(T(1), read_modify_write_order);
    }

    T operator += (std::ptrdiff_t arg) noexcept
    requires std::is_pointer_v<T>
    {
        return std::atomic<T>::fetch_add(arg, read_modify_write_order) + arg;
    }

    T operator += (std::ptrdiff_t arg) volatile noexcept
    requires std::is_pointer_v<T>
    {
        return std::atomic<T>::fetch_add(arg, read_modify_write_order) + arg;
    }

    T operator -= (std::ptrdiff_t arg) noexcept
    requires std::is_pointer_v<T>
    {
        return std::atomic<T>::fetch_sub(arg, read_modify_write_order) - arg;
    }

    T operator -= (std::ptrdiff_t arg) volatile noexcept
    requires std::is_pointer_v<T>
    {
        return std::atomic<T>::fetch_sub(arg, read_modify_write_order) - arg;
    }

    T operator ++ () noexcept
    requires std::is_pointer_v<T>
    {
        return std::atomic<T>::fetch_add(std::ptrdiff_t(1), read_modify_write_order) + std::ptrdiff_t(1);
    }

    T operator ++ () volatile noexcept
    requires std::is_pointer_v<T>
    {
        return std::atomic<T>::fetch_add(std::ptrdiff_t(1), read_modify_write_order) + std::ptrdiff_t(1);
    }

    T operator ++ (int) noexcept
    requires std::is_pointer_v<T>
    {
        return std::atomic<T>::fetch_add(std::ptrdiff_t(1), read_modify_write_order);
    }

    T operator ++ (int) volatile noexcept
    requires std::is_pointer_v<T>
    {
        return std::atomic<T>::fetch_add(std::ptrdiff_t(1), read_modify_write_order);
    }

    T operator -- () noexcept
    requires std::is_pointer_v<T>
    {
        return std::atomic<T>::fetch_sub(std::ptrdiff_t(1), read_modify_write_order) - std::ptrdiff_t(1);
    }

    T operator -- () volatile noexcept
    requires std::is_pointer_v<T>
    {
        return std::atomic<T>::fetch_sub(std::ptrdiff_t(1), read_modify_write_order) - std::ptrdiff_t(1);
    }

    T operator -- (int) noexcept
    requires std::is_pointer_v<T>
    {
        return std::atomic<T>::fetch_sub(std::ptrdiff_t(1), read_modify_write_order);
    }

    T operator -- (int) volatile noexcept
    requires std::is_pointer_v<T>
    {
        return std::atomic<T>::fetch_sub(std::ptrdiff_t(1), read_modify_write_order);
    }

    T operator &= (T arg) noexcept
    requires std::is_integral_v<T>
    {
        return std::atomic<T>::fetch_and(arg, read_modify_write_order) & arg;
    }

    T operator &= (T arg) volatile noexcept
    requires std::is_integral_v<T>
    {
        return std::atomic<T>::fetch_and(arg, read_modify_write_order) & arg;
    }

    T operator |= (T arg) noexcept
    requires std::is_integral_v<T>
    {
        return std::atomic<T>::fetch_or(arg, read_modify_write_order) | arg;
    }

    T operator |= (T arg) volatile noexcept
    requires std::is_integral_v<T>
    {
        return std::atomic<T>::fetch_or(arg, read_modify_write_order) | arg;
    }

    T operator ^= (T arg) noexcept
    requires std::is_integral_v<T>
    {
        return std::atomic<T>::fetch_xor(arg, read_modify_write_order) ^ arg;
    }

    T operator ^= (T arg) volatile noexcept
    requires std::is_integral_v<T>
    {
        return std::atomic<T>::fetch_xor(arg, read_modify_write_order) ^ arg;
    }
#pragma GCC diagnostic pop

    std::memory_order get_read_memory_order() const
    {
        return read_order;
    }

    std::memory_order get_read_memory_order() const volatile
    {
        return read_order;
    }

    std::memory_order get_write_memory_order() const
    {
        return write_order;
    }

    std::memory_order get_write_memory_order() const volatile
    {
        return write_order;
    }

    std::memory_order get_read_modify_write_memory_order() const
    {
        return read_modify_write_order;
    }

    std::memory_order get_read_modify_write_memory_order() const volatile
    {
        return read_modify_write_order;
    }
};



} // namespace snapdev
// vim: ts=4 sw=4 et
