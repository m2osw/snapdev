// Copyright (c) 2022-2026  Made to Order Software Corp.  All Rights Reserved
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


// C++
//
#include    <random>


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
 * (i.e. x86, amd64 do) then that instruction will be used by your compiler.
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
 * See also bswap_64(), bswap_32(), bswap_16().
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


/** \brief Compare two floating point numbers for equality.
 *
 * This function quietly compares two floating point numbers for equality.
 * This means you can do:
 *
 * \code
 *     if(snapdev::quiet_floating_point_equal(a, b))
 *     {
 *         ...
 *     }
 * \endcode
 *
 * as an equivalent to:
 *
 * \code
 * #pragma GCC diagnostic push
 * #pragma GCC diagnostic ignored "-Wfloat-equal"
 *     if(a == b)
 *     {
 *         ...
 *     }
 * #pragma GCC diagnostic pop
 * \endcode
 *
 * \note
 * The function prevents comparing a floating point number with an integer.
 *
 * \tparam L  The type of the left hand side floating point number
 * \tparam R  The type of the right hand side floating point number
 * \param[in] a  The left hand side floating point number to compare
 * \param[in] b  The right hand side floating point number to compare
 *
 * \return true if a is exactly equal to b.
 */
template<
      typename L
    , typename R>
constexpr std::enable_if_t<std::is_floating_point_v<L>
        && std::is_floating_point_v<R>, bool> quiet_floating_point_equal(L a, R b) noexcept
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
    return a == b;
#pragma GCC diagnostic pop
}


/** \brief Compare two floating point numbers for inequality.
 *
 * This function quietly compares two floating point numbers for inequality.
 * This means you can do:
 *
 * \code
 *     if(snapdev::quiet_floating_point_not_equal(a, b))
 *     {
 *         ...
 *     }
 * \endcode
 *
 * as an equivalent to:
 *
 * \code
 * #pragma GCC diagnostic push
 * #pragma GCC diagnostic ignored "-Wfloat-equal"
 *     if(a != b)
 *     {
 *         ...
 *     }
 * #pragma GCC diagnostic pop
 * \endcode
 *
 * \note
 * The function prevents comparing a floating point number with an integer.
 *
 * \tparam L  The type of the left hand side floating point number
 * \tparam R  The type of the right hand side floating point number
 * \param[in] a  The left hand side floating point number to compare
 * \param[in] b  The right hand side floating point number to compare
 *
 * \return true if a is not exactly equal to b.
 */
template<
      typename L
    , typename R>
constexpr std::enable_if_t<std::is_floating_point_v<L>
        && std::is_floating_point_v<R>, bool> quiet_floating_point_not_equal(L a, R b) noexcept
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
    return a != b;
#pragma GCC diagnostic pop
}


/** \brief Initialize a Mersenne random generator.
 *
 * This function initializes a Mersenne random generator and returns a
 * reference to it. You should always use this function to avoid having
 * new sequences started each time.
 *
 * See the random() functions to generate floating point or integral
 * numbers.
 *
 * The seeding of the pseudo random generator is done using a random_device,
 * meaning that it is expected to be different on each run of your
 * application (seeded from a hardware device).
 *
 * \warning
 * It is possible to copy the generator. Just keep in mind that if you
 * call this function and make a copy each time, you reset the state each
 * time. Instead, you want to keep a copy of the generator in your object
 * and reuse that copy over and over again or just don't make a copy and
 * call this function each time you need the reference.
 *
 * \return A reference to a statically initialized Mersenne pseudo random
 * number generator.
 */
inline std::mt19937_64 & get_random_generator()
{
    // static variables to initialize the random number generator only once
    //
    static std::random_device rd;       // random "device" such as /dev/random
    static std::mt19937_64 gen(rd());   // a Mersenne generator seeded from the random device
    return gen;
}


/** \brief Generate a floating point random number.
 *
 * This function generates a floating point number of type T using the
 * C++ random number generator (mt19937).
 *
 * By default, the generator uses the range [0.0, 1.0). You may specify
 * other numbers as the minimum and maximum.
 *
 * \tparam T  The type of floating point (float, double, or long double).
 * \param[in] min  The minimum value to return, inclusive.
 * \param[in] max  The maximum value to return, exclusive.
 *
 * \return The randomly generated floating point.
 */
template<typename T>
std::enable_if_t<std::is_floating_point_v<T>, T> random(
      T min = 0.0
    , T max = 1.0)
{
    std::uniform_real_distribution<T> dis(min, max);
    return dis(get_random_generator());
}


/** \brief Generate a pseudo integral random number.
 *
 * This function generates an integer of type T using the C++ random
 * number generator (mt19937).
 *
 * By default the generator uses the range 0 to maximum value of that
 * integer type. If you do not specify the range, you must specify the
 * type when using the function (`random<int>()`).
 *
 * \tparam T  The type of the integral number to generate.
 * \param[in] min  The minimum value, inclusive.
 * \param[in] max  The maximum value, inclusive.
 */
template<typename T>
std::enable_if_t<std::is_integral_v<T>, T> random(
      T min = 0
    , T max = std::numeric_limits<T>::max())
{
    std::uniform_int_distribution<T> dis(min, max);
    return dis(get_random_generator());
}



} // namespace snapdev
// vim: ts=4 sw=4 et
