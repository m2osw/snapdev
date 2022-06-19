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

/** \file
 * \brief Implements the limits of int128 and unsigned int128.
 *
 * The default std library on Ubuntu 18.04 does not yet define the int128
 * limits so I have this code here which is expected to add that
 * functionality.
 *
 * The __INT_N() definition is from the Ubuntu 18.04 standard library. The
 * __INT_N() implementation is from Ubuntu 22.04.
 *
 * We can get rid of this once we move away from Ubuntu 18.04 (which should
 * be soon, but I'm still somewhat tighted up to it at the moment).
 */

// C++
//
#include    <limits>

// this test is probably wrong, Ubuntu 18.04 is 7.5 and it's not there yet
// I also tested with 20.04, it's 9.3 and it's not there yet
// I checked on Debian 11 which has g++ 10.2.1 and it has it
// I know that 22.04 has it, it's g++ 11.x
// so for now I use version 10 here
//
#if (__GNUC__ < 10)


namespace std _GLIBCXX_VISIBILITY(default)
{

#define __INT_N(TYPE, BITSIZE, EXT, UEXT)                                     \
  template<>                                                                  \
    struct numeric_limits<TYPE>                                               \
    {                                                                         \
      static _GLIBCXX_USE_CONSTEXPR bool is_specialized = true;               \
                                                                              \
      static _GLIBCXX_CONSTEXPR TYPE                                          \
      min() _GLIBCXX_USE_NOEXCEPT { return __glibcxx_min_b (TYPE, BITSIZE); } \
                                                                              \
      static _GLIBCXX_CONSTEXPR TYPE                                          \
      max() _GLIBCXX_USE_NOEXCEPT { return __glibcxx_max_b (TYPE, BITSIZE); } \
                                                                              \
      static _GLIBCXX_USE_CONSTEXPR int digits                                \
       = BITSIZE - 1;                                                         \
      static _GLIBCXX_USE_CONSTEXPR int digits10                              \
       = (BITSIZE - 1) * 643L / 2136;                                         \
                                                                              \
      static _GLIBCXX_USE_CONSTEXPR bool is_signed = true;                    \
      static _GLIBCXX_USE_CONSTEXPR bool is_integer = true;                   \
      static _GLIBCXX_USE_CONSTEXPR bool is_exact = true;                     \
      static _GLIBCXX_USE_CONSTEXPR int radix = 2;                            \
                                                                              \
      static _GLIBCXX_CONSTEXPR TYPE                                          \
      epsilon() _GLIBCXX_USE_NOEXCEPT { return 0; }                           \
                                                                              \
      static _GLIBCXX_CONSTEXPR TYPE                                          \
      round_error() _GLIBCXX_USE_NOEXCEPT { return 0; }                       \
                                                                              \
      EXT                                                                     \
                                                                              \
      static _GLIBCXX_USE_CONSTEXPR int min_exponent = 0;                     \
      static _GLIBCXX_USE_CONSTEXPR int min_exponent10 = 0;                   \
      static _GLIBCXX_USE_CONSTEXPR int max_exponent = 0;                     \
      static _GLIBCXX_USE_CONSTEXPR int max_exponent10 = 0;                   \
                                                                              \
      static _GLIBCXX_USE_CONSTEXPR bool has_infinity = false;                \
      static _GLIBCXX_USE_CONSTEXPR bool has_quiet_NaN = false;               \
      static _GLIBCXX_USE_CONSTEXPR bool has_signaling_NaN = false;           \
      static _GLIBCXX_USE_CONSTEXPR float_denorm_style has_denorm             \
       = denorm_absent;                                                       \
      static _GLIBCXX_USE_CONSTEXPR bool has_denorm_loss = false;             \
                                                                              \
      static _GLIBCXX_CONSTEXPR TYPE                                          \
      infinity() _GLIBCXX_USE_NOEXCEPT                                        \
      { return static_cast<TYPE>(0); }                                        \
                                                                              \
      static _GLIBCXX_CONSTEXPR TYPE                                          \
      quiet_NaN() _GLIBCXX_USE_NOEXCEPT                                       \
      { return static_cast<TYPE>(0); }                                        \
                                                                              \
      static _GLIBCXX_CONSTEXPR TYPE                                          \
      signaling_NaN() _GLIBCXX_USE_NOEXCEPT                                   \
      { return static_cast<TYPE>(0); }                                        \
                                                                              \
      static _GLIBCXX_CONSTEXPR TYPE                                          \
      denorm_min() _GLIBCXX_USE_NOEXCEPT                                      \
      { return static_cast<TYPE>(0); }                                        \
                                                                              \
      static _GLIBCXX_USE_CONSTEXPR bool is_iec559 = false;                   \
      static _GLIBCXX_USE_CONSTEXPR bool is_bounded = true;                   \
      static _GLIBCXX_USE_CONSTEXPR bool is_modulo = false;                   \
                                                                              \
      static _GLIBCXX_USE_CONSTEXPR bool traps                                \
       = __glibcxx_integral_traps;                                            \
      static _GLIBCXX_USE_CONSTEXPR bool tinyness_before = false;             \
      static _GLIBCXX_USE_CONSTEXPR float_round_style round_style             \
       = round_toward_zero;                                                   \
    };                                                                        \
                                                                              \
  template<>                                                                  \
    struct numeric_limits<unsigned TYPE>                                      \
    {                                                                         \
      static _GLIBCXX_USE_CONSTEXPR bool is_specialized = true;               \
                                                                              \
      static _GLIBCXX_CONSTEXPR unsigned TYPE                                 \
      min() _GLIBCXX_USE_NOEXCEPT { return 0; }                               \
                                                                              \
      static _GLIBCXX_CONSTEXPR unsigned TYPE                                 \
      max() _GLIBCXX_USE_NOEXCEPT                                             \
      { return  __glibcxx_max_b (unsigned TYPE, BITSIZE); }                   \
                                                                              \
      UEXT                                                                    \
                                                                              \
      static _GLIBCXX_USE_CONSTEXPR int digits                                \
       = BITSIZE;                                                             \
      static _GLIBCXX_USE_CONSTEXPR int digits10                              \
       = BITSIZE * 643L / 2136;                                               \
      static _GLIBCXX_USE_CONSTEXPR bool is_signed = false;                   \
      static _GLIBCXX_USE_CONSTEXPR bool is_integer = true;                   \
      static _GLIBCXX_USE_CONSTEXPR bool is_exact = true;                     \
      static _GLIBCXX_USE_CONSTEXPR int radix = 2;                            \
                                                                              \
      static _GLIBCXX_CONSTEXPR unsigned TYPE                                 \
      epsilon() _GLIBCXX_USE_NOEXCEPT { return 0; }                           \
                                                                              \
      static _GLIBCXX_CONSTEXPR unsigned TYPE                                 \
      round_error() _GLIBCXX_USE_NOEXCEPT { return 0; }                       \
                                                                              \
      static _GLIBCXX_USE_CONSTEXPR int min_exponent = 0;                     \
      static _GLIBCXX_USE_CONSTEXPR int min_exponent10 = 0;                   \
      static _GLIBCXX_USE_CONSTEXPR int max_exponent = 0;                     \
      static _GLIBCXX_USE_CONSTEXPR int max_exponent10 = 0;                   \
                                                                              \
      static _GLIBCXX_USE_CONSTEXPR bool has_infinity = false;                \
      static _GLIBCXX_USE_CONSTEXPR bool has_quiet_NaN = false;               \
      static _GLIBCXX_USE_CONSTEXPR bool has_signaling_NaN = false;           \
      static _GLIBCXX_USE_CONSTEXPR float_denorm_style has_denorm             \
       = denorm_absent;                                                       \
      static _GLIBCXX_USE_CONSTEXPR bool has_denorm_loss = false;             \
                                                                              \
      static _GLIBCXX_CONSTEXPR unsigned TYPE                                 \
      infinity() _GLIBCXX_USE_NOEXCEPT                                        \
      { return static_cast<unsigned TYPE>(0); }                               \
                                                                              \
      static _GLIBCXX_CONSTEXPR unsigned TYPE                                 \
      quiet_NaN() _GLIBCXX_USE_NOEXCEPT                                       \
      { return static_cast<unsigned TYPE>(0); }                               \
                                                                              \
      static _GLIBCXX_CONSTEXPR unsigned TYPE                                 \
      signaling_NaN() _GLIBCXX_USE_NOEXCEPT                                   \
      { return static_cast<unsigned TYPE>(0); }                               \
                                                                              \
      static _GLIBCXX_CONSTEXPR unsigned TYPE                                 \
      denorm_min() _GLIBCXX_USE_NOEXCEPT                                      \
      { return static_cast<unsigned TYPE>(0); }                               \
                                                                              \
      static _GLIBCXX_USE_CONSTEXPR bool is_iec559 = false;                   \
      static _GLIBCXX_USE_CONSTEXPR bool is_bounded = true;                   \
      static _GLIBCXX_USE_CONSTEXPR bool is_modulo = true;                    \
                                                                              \
      static _GLIBCXX_USE_CONSTEXPR bool traps = __glibcxx_integral_traps;    \
      static _GLIBCXX_USE_CONSTEXPR bool tinyness_before = false;             \
      static _GLIBCXX_USE_CONSTEXPR float_round_style round_style             \
       = round_toward_zero;                                                   \
    };

#define __INT_N_201103(TYPE)                                            \
      static constexpr TYPE                                             \
      lowest() noexcept { return min(); }                               \
      static constexpr int max_digits10 = 0;

#define __INT_N_U201103(TYPE)                                           \
      static constexpr unsigned TYPE                                    \
      lowest() noexcept { return min(); }                               \
      static constexpr int max_digits10 = 0;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
__INT_N(__int128, 128,
          __INT_N_201103 (__int128),
          __INT_N_U201103 (__int128))
#pragma GCC diagnostic pop
#endif

} // std namespace
// vim: ts=4 sw=4 et
