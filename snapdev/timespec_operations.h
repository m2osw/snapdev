// Copyright (c) 2021-2022  Made to Order Software Corp.  All Rights Reserved
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
#include    <cmath>
#include    <cstdint>
#include    <iomanip>
#include    <iostream>
#include    <sstream>


// C lib
//
#include    <stdlib.h>
#include    <sys/time.h>




inline timespec operator - (timespec const & t)
{
    // equivalent to `0 - t`
    //
    timespec result{ -t.tv_sec, -t.tv_nsec };
    if(result.tv_nsec < 0)
    {
        --result.tv_sec;
        result.tv_nsec += 1'000'000'000L;
    }
    return result;
}


namespace snapdev
{


inline bool valid_timespec(timespec const & t)
{
    return t.tv_nsec < 1'000'000'000LL;
}


inline bool negative_timespec(timespec const & t)
{
    return t.tv_sec < 0LL;
}


inline timespec add_timespec(timespec const & lhs, timespec const & rhs)
{
    bool const lneg(negative_timespec(lhs));
    bool const rneg(negative_timespec(rhs));

    timespec lp(lneg ? -lhs : lhs);
    timespec rp(rneg ? -rhs : rhs);

    timespec result = {};

    switch((lneg ? 1 : 0) + (rneg ? 2 : 0))
    {
    case 0:     // positive + positive
        result.tv_sec = lp.tv_sec + rp.tv_sec;
        result.tv_nsec = lp.tv_nsec + rp.tv_nsec;
        break;

    case 1:     // negative + positive
        result.tv_sec = rp.tv_sec - lp.tv_sec;
        result.tv_nsec = rp.tv_nsec - lp.tv_nsec;
        break;

    case 2:     // positive + negative
        result.tv_sec = lp.tv_sec - rp.tv_sec;
        result.tv_nsec = lp.tv_nsec - rp.tv_nsec;
        break;

    case 3:     // negative + negative
        result.tv_sec = lp.tv_sec + rp.tv_sec;
        result.tv_nsec = lp.tv_nsec + rp.tv_nsec;
        break;

    }

    if(result.tv_nsec < 0)
    {
        --result.tv_sec;
        result.tv_nsec += 1'000'000'000L;
    }
    else if(result.tv_nsec >= 1'000'000'000)
    {
        ++result.tv_sec;
        result.tv_nsec -= 1'000'000'000;
    }

    if(lneg && rneg)
    {
        result = -result;
    }

    return result;
}


int compare_timespec(timespec const & lhs, timespec const & rhs)
{
    if(lhs.tv_sec == rhs.tv_sec)
    {
        return lhs.tv_nsec == rhs.tv_nsec
                ? 0
                : (lhs.tv_nsec < rhs.tv_nsec ? -1 : 1);
    }

    return lhs.tv_sec < rhs.tv_sec ? -1 : 1;
}



} // namespace snapdev




// This one is not possible as a static function
//inline operator bool (timespec const & t)
//{
//    return t.tv_sec != 0 || tv.tv_nsec != 0;
//}


inline bool operator ! (timespec const & t)
{
    return t.tv_sec == 0 && t.tv_nsec == 0;
}


inline timespec & operator <<= (timespec & lhs, std::int64_t nsec)
{
    bool const neg(nsec < 0);
    if(neg)
    {
        nsec = -nsec;
    }
    lhs.tv_sec = static_cast<time_t>(nsec / 1'000'000'000LL);
    lhs.tv_nsec = static_cast<long>(llabs(nsec) % 1'000'000'000LL);
    if(neg)
    {
        lhs = -lhs;
    }
    return lhs;
}


inline std::int64_t operator >>= (timespec & t, std::int64_t & nsec)
{
    nsec = t.tv_sec * 1'000'000'000LL + t.tv_nsec;
    return nsec;
}


inline timespec & operator <<= (timespec & lhs, double sec)
{
    bool const neg(sec < 0.0);
    if(neg)
    {
        sec = -sec;
    }
    lhs.tv_sec = static_cast<time_t>(floor(sec));
    lhs.tv_nsec = static_cast<long>((sec - floor(sec)) * 1.0e9);
    if(neg)
    {
        lhs = -lhs;
    }
    return lhs;
}


inline std::int64_t operator >>= (timespec & t, double & nsec)
{
    nsec = t.tv_sec + t.tv_nsec / 1'000'000'000.0;
    return nsec;
}


inline timespec & operator += (timespec & lhs, timespec const & rhs)
{
    lhs = snapdev::add_timespec(lhs, rhs);
    return lhs;
}


inline timespec & operator += (timespec & lhs, std::int64_t rhs)
{
    timespec t;
    t <<= rhs;
    lhs += t;
    return lhs;
}


inline timespec & operator += (timespec & lhs, double rhs)
{
    timespec t;
    t <<= rhs;
    lhs += t;
    return lhs;
}


inline timespec & operator ++ (timespec & lhs)
{
    lhs += 1L;
    return lhs;
}


inline timespec operator ++ (timespec & lhs, int)
{
    timespec result(lhs);
    lhs += 1L;
    return result;
}


inline timespec operator + (timespec const & lhs, timespec const & rhs)
{
    timespec result(lhs);
    result += rhs;
    return result;
}


inline timespec operator + (timespec const & lhs, std::int64_t rhs)
{
    timespec result;
    result <<= rhs;
    result += lhs;
    return result;
}


inline timespec operator + (timespec const & lhs, double rhs)
{
    timespec result;
    result <<= rhs;
    result += lhs;
    return result;
}


inline timespec & operator -= (timespec & lhs, timespec const & rhs)
{
    timespec const neg(-rhs);
    lhs = snapdev::add_timespec(lhs, neg);
    return lhs;
}


inline timespec & operator -= (timespec & lhs, std::int64_t rhs)
{
    timespec result;
    result <<= rhs;
    lhs -= result;
    return lhs;
}


inline timespec & operator -= (timespec & lhs, double rhs)
{
    timespec result;
    result <<= rhs;
    lhs -= result;
    return lhs;
}


inline timespec & operator -- (timespec & lhs)
{
    lhs -= 1L;
    return lhs;
}


inline timespec operator -- (timespec & lhs, int)
{
    timespec result(lhs);
    lhs -= 1L;
    return result;
}


inline timespec operator - (timespec const & lhs, timespec const & rhs)
{
    timespec result(lhs);
    result -= rhs;
    return result;
}


inline timespec operator - (timespec const & lhs, std::int64_t rhs)
{
    timespec result;
    result <<= -rhs;
    result += lhs;
    return result;
}


inline timespec operator - (timespec const & lhs, double rhs)
{
    timespec result;
    result <<= -rhs;
    result += lhs;
    return result;
}


inline bool operator == (timespec const & lhs, timespec const & rhs)
{
    return snapdev::compare_timespec(lhs, rhs) == 0;
}


inline bool operator == (timespec const & lhs, std::int64_t rhs)
{
    timespec t;
    t <<= rhs;
    return snapdev::compare_timespec(lhs, t) == 0;
}


inline bool operator == (timespec const & lhs, double rhs)
{
    timespec t;
    t <<= rhs;
    return snapdev::compare_timespec(lhs, t) == 0;
}


inline bool operator != (timespec const & lhs, timespec const & rhs)
{
    return snapdev::compare_timespec(lhs, rhs) != 0;
}


inline bool operator != (timespec const & lhs, std::int64_t rhs)
{
    timespec t;
    t <<= rhs;
    return snapdev::compare_timespec(lhs, t) != 0;
}


inline bool operator != (timespec const & lhs, double rhs)
{
    timespec t;
    t <<= rhs;
    return snapdev::compare_timespec(lhs, t) != 0;
}


inline bool operator < (timespec const & lhs, timespec const & rhs)
{
    return snapdev::compare_timespec(lhs, rhs) == -1;
}


inline bool operator < (timespec const & lhs, std::int64_t rhs)
{
    timespec t;
    t <<= rhs;
    return snapdev::compare_timespec(lhs, t) == -1;
}


inline bool operator < (timespec const & lhs, double rhs)
{
    timespec t;
    t <<= rhs;
    return snapdev::compare_timespec(lhs, t) == -1;
}


inline bool operator <= (timespec const & lhs, timespec const & rhs)
{
    return snapdev::compare_timespec(lhs, rhs) <= 0;
}


inline bool operator <= (timespec const & lhs, std::int64_t rhs)
{
    timespec t;
    t <<= rhs;
    return snapdev::compare_timespec(lhs, t) <= 0;
}


inline bool operator <= (timespec const & lhs, double rhs)
{
    timespec t;
    t <<= rhs;
    return snapdev::compare_timespec(lhs, t) <= 0;
}


inline bool operator > (timespec const & lhs, timespec const & rhs)
{
    return snapdev::compare_timespec(lhs, rhs) == 1;
}


inline bool operator > (timespec const & lhs, std::int64_t rhs)
{
    timespec t;
    t <<= rhs;
    return snapdev::compare_timespec(lhs, t) == 1;
}


inline bool operator > (timespec const & lhs, double rhs)
{
    timespec t;
    t <<= rhs;
    return snapdev::compare_timespec(lhs, t) == 1;
}


inline bool operator >= (timespec const & lhs, timespec const & rhs)
{
    return snapdev::compare_timespec(lhs, rhs) >= 0;
}


inline bool operator >= (timespec const & lhs, std::int64_t rhs)
{
    timespec t;
    t <<= rhs;
    return snapdev::compare_timespec(lhs, t) >= 0;
}


inline bool operator >= (timespec const & lhs, double rhs)
{
    timespec t;
    t <<= rhs;
    return snapdev::compare_timespec(lhs, t) >= 0;
}




/** \brief Output a timespec to a basic_ostream.
 *
 * This function allows one to print out a timespec. By default the function
 * prints the timespec as a floating point.
 *
 * \todo
 * Look into the possibility to write the data as Y/M/D H:M:S.nanosecs
 *
 * \param[in] out  The output stream where the timespec gets written.
 * \param[in] t  The actual timespec that is to be printed.
 *
 * \return A reference to the basic_ostream object.
 */
template<typename CharT, typename Traits>
std::basic_ostream<CharT, Traits> & operator << (std::basic_ostream<CharT, Traits> & out, timespec const & t)
{
    // write to a string buffer first
    //
    std::basic_ostringstream<CharT, Traits, std::allocator<CharT> > s;

    // setup the string output like the out stream
    //
    s.flags(out.flags());
    s.imbue(out.getloc());
    s.precision(out.precision());
    s << t.tv_sec << "." << std::setw(9) << std::setfill('0') << t.tv_nsec;

    // buffer is ready, display in output in one go
    //
    return out << s.str();
}



// vim: ts=4 sw=4 et
