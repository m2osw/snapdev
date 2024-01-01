// Copyright (c) 2022-2024  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Various mathematical functions.
 *
 * These functions are extensions to the math functions offered by the C
 * and the C++ libraries.
 */

// self
//
#include    <snapdev/int128_literal.h>
#include    <snapdev/limits_int128.h>
#include    <snapdev/not_reached.h>


// C
//
#include    <byteswap.h>
#include    <signal.h>


namespace snapdev
{


/** \brief Compute a power of an integer number.
 *
 * This function multiplies the specified \p value by itself \p power times.
 *
 * This is a function to compute an integer power so \p power must be an
 * integer and is generally expected to be positive. Since a negative power
 * is legal, we handle the case, but in most likelihood, the result will
 * be zero in this case.
 *
 * \note
 * If \p power is 0, the result is always 1.
 *
 * \note
 * If \p power is negative, the result is 0 unless \p value is 1 or -1 in
 * which case the result is 1 or -1.
 *
 * \todo
 * The function does not check for overflow.
 *
 * \todo
 * Once all versions use g++ v10 or more, we can remove the is_same_v<>
 * against __int128.
 *
 * \param[in] value  An integer.
 * \param[in] power  The number of times to multiply \p value by itself.
 *
 * \return The resulting "value ** power".
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
template<typename T>
constexpr std::enable_if_t<(std::is_integral_v<T> && std::is_signed_v<T>)
              || std::is_same_v<T, __int128>, T> pow(T value, int power) noexcept
{
    using namespace snapdev::literals;

    if(power == 0)
    {
        return static_cast<T>(1);
    }
    if(power < 0)
    {
        // negative powers are similar to (1 / value ** power) which is
        // 1 or -1 if value is 1 or -1
        //
        switch(value)
        {
        case -1:
            return static_cast<T>((power & 1) == 0 ? 1 : -1);

        case 1:
            return static_cast<T>(1);

        default:
            return static_cast<T>(0);

        }
        snapdev::NOT_REACHED();
    }

    T result(static_cast<T>(1));
    for(;;)
    {
        if((power & 1) != 0)
        {
            result *= value;
        }
        power /= 2;
        if(power == 0)
        {
            return result;
        }
        value *= value;
    }
    snapdev::NOT_REACHED();
}
#pragma GCC diagnostic pop


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
template<typename T>
constexpr std::enable_if_t<(std::is_integral_v<T> && !std::is_signed_v<T>)
              || std::is_same_v<T, unsigned __int128>, T> pow(T value, int power)
{
    using namespace snapdev::literals;

    if(power == 0)
    {
        return static_cast<T>(1);
    }
    if(power < 0)
    {
        // negative powers are similar to (1 / value ** power) which is
        // 1 if value is 1
        //
        switch(value)
        {
        case 1:
            return static_cast<T>(1);

        default:
            return static_cast<T>(0);

        }
        snapdev::NOT_REACHED();
    }

    T result(static_cast<T>(1));
    for(;;)
    {
        if((power & 1) != 0)
        {
            result *= value;
        }
        power /= 2;
        if(power == 0)
        {
            return result;
        }
        value *= value;
    }
    snapdev::NOT_REACHED();
}
#pragma GCC diagnostic pop


/** \brief Add two signed numbers, return min or max on overflow.
 *
 * This function adds \p lhs to \p rhs and returns the total unless there
 * is an overflow in which case the minimum (if \p lhs and \p rhs are
 * negative) or the maximum (if \p lhs and \p rhs are positive) is returned.
 *
 * \note
 * This function is picked only if T is signed.
 *
 * \note
 * T can be `signed __int128`.
 *
 * \tparam T  The type of integer concerned. \p lhs and \p rhs must be of the
 * same type.
 * \param[in] lhs  The left handside number to add.
 * \param[in] rhs  The right handside number to add.
 *
 * \return Saturated lhs + rhs.
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
template<typename T>
std::enable_if_t<(std::is_integral_v<T> && std::is_signed_v<T>)
              || std::is_same_v<T, __int128>, T> saturated_add(T lhs, T rhs)
{
    T const result(lhs + rhs);
    T const sign(lhs ^ rhs);
    if(sign < 0)
    {
        // no possible overflow (lhs & rhs are opposite signs)
        //
        return result;
    }
    T const result_sign(result ^ lhs);
    if(result_sign >= 0)
    {
        // overflow did not happen (result is still the same sign)
        //
        return result;
    }
    return lhs < 0
            ? std::numeric_limits<T>::min()
            : std::numeric_limits<T>::max();
}
#pragma GCC diagnostic pop


/** \brief Add two unsigned numbers, return max on overflow.
 *
 * This function adds \p lhs to \p rhs and returns the total unless there
 * is an overflow in which case the maximum is returned.
 *
 * \note
 * This function is picked only if T is unsigned.
 *
 * \note
 * T can be `unsigned __int128`. This is currently checked explicitly
 * because older C++ standard libraries do not view __int128 as an integral
 * type (it is not included in the list of types checked).
 *
 * \tparam T  The type of integer concerned. \p lhs and \p rhs must be of the
 * same type.
 * \param[in] lhs  The left handside number to add.
 * \param[in] rhs  The right handside number to add.
 *
 * \return Saturated lhs + rhs.
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
template<typename T>
std::enable_if_t<(std::is_integral_v<T> && std::is_unsigned_v<T>)
               || std::is_same_v<T, unsigned __int128>, T> saturated_add(T lhs, T rhs)
{
    return lhs > std::numeric_limits<T>::max() - rhs
            ? std::numeric_limits<T>::max()
            : lhs + rhs;
}
#pragma GCC diagnostic pop


/** \brief Subtract an unsigned number from another, return min on overflow.
 *
 * This function subtract \p rhs from \p lhs and returns the difference
 * unless there is an underflow in which case the minimum (0) is returned.
 *
 * \note
 * This function is picked only if T is unsigned.
 *
 * \note
 * T can be `unsigned __int128`. This is currently checked explicitly
 * because older C++ standard libraries do not view __int128 as an integral
 * type (it is not included in the list of types checked).
 *
 * \tparam T  The type of integer concerned. \p lhs and \p rhs must be of the
 * same type.
 * \param[in] lhs  The left handside number to subtract from.
 * \param[in] rhs  The right handside number to subtract.
 *
 * \return Saturated lhs - rhs.
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
template<typename T>
std::enable_if_t<(std::is_integral_v<T> && std::is_unsigned_v<T>)
               || std::is_same_v<T, unsigned __int128>, T> saturated_subtract(T lhs, T rhs)
{
    return lhs < rhs ? 0 : lhs - rhs;
}
#pragma GCC diagnostic pop


/** \brief Rotate an integer to the left.
 *
 * This function rotates the specified integer (\p x) to the left by
 * \p r bits.
 *
 * In most cases, if the processor you are using has a `rotl` instruction
 * (i.e. x86, amd64 do) then that instration will be used by your compiler.
 *
 * \note
 * If r is negative, rotate to the right.
 *
 * \param[in] x  The value to be rotated.
 * \param[in] r  The number of bits to rotate the value by toward the left.
 *
 * \return The value \p x rotated by \p r bits.
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
template<typename T>
constexpr T rotl(T x, int r)
{
    std::size_t const s(sizeof(T) * 8);
    r = r % s;
    if(r < 0)
    {
        return (x >> r) | (x << (s - r));
    }
    else
    {
        return (x << r) | (x >> (s - r));
    }
}
#pragma GCC diagnostic pop


/** \brief Swap all the bytes of a 128 bit number.
 *
 * This function swaps the bytes of a 128 bit number. This is similar to
 * flipping the bytes between big endian and little endian.
 *
 * \note
 * See also bwap_64(), bswap_32(), bswap_16().
 *
 * \param[in] n  The 128 bit number to byte swap.
 *
 * \return The swapped __int128 value.
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
inline unsigned __int128 bswap_128(unsigned __int128 n)
{
    unsigned __int128 m;
    std::uint64_t const * src(reinterpret_cast<std::uint64_t const *>(&n));
    std::uint64_t * dest(reinterpret_cast<std::uint64_t*>(&m));
    dest[1] = bswap_64(src[0]);
    dest[0] = bswap_64(src[1]);
    return m;
}
#pragma GCC diagnostic pop



} // namespace snapdev
// vim: ts=4 sw=4 et
