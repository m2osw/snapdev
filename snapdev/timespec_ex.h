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

// libexcept
//
#include    <libexcept/exception.h>


// C++
//
#include    <cmath>
#include    <cstdint>
#include    <iomanip>
#include    <iostream>
#include    <sstream>


// C
//
#include    <stdlib.h>
#include    <sys/time.h>
#include    <time.h>




namespace snapdev
{

DECLARE_MAIN_EXCEPTION(timespec_ex_exception);

DECLARE_EXCEPTION(timespec_ex_exception, overflow);


class timespec_ex
    : public timespec
{
public:
    /** \brief Initialize a timespec_ex to zero.
     *
     * This constructor is used to initialize a new timespec_ex to the
     * default value, which is 0.
     */
    timespec_ex()
    {
        set(0L);
    }


    /** \brief Initialize a timespec_ex from another.
     *
     * This constructors allows you to directly copy a timespec_ex
     * in another new timespec_ex object.
     *
     * \param[in] t  The timespec_ex to directly copy.
     */
    timespec_ex(timespec_ex const & t)
    {
        set(t);
    }


    /** \brief Initialize a timespec_ex from a timespec structure.
     *
     * This constructors allows you to directly set a timespec_ex
     * to the specified timespec values.
     *
     * \param[in] t  The timespec to directly copy.
     */
    timespec_ex(timespec const & t)
    {
        set(t);
    }


    /** \brief Initialize a timespec_ex from seconds and nanoseconds.
     *
     * This constructors allows you to directly initialize a timespec_ex
     * from seconds and nanoseconds.
     *
     * To create a valid timespec_ex object, you must pass a number
     * between 0 and 999'999'999 for the \p nsec parameter.
     *
     * \param[in] sec  The number of seconds.
     * \param[in] nsec  The number of nano-seconds.
     */
    timespec_ex(time_t sec, long nsec)
    {
        set(sec, nsec);
    }


    /** \brief Initialize a timespec_ex from an int64_t in nanoseconds.
     *
     * This constructors allows you to directly set a timespec_ex
     * to the specified \p nsec value.
     *
     * \param[in] nsec  The nano-seconds to copy to timespec_ex.
     */
    timespec_ex(std::int64_t nsec)
    {
        set(nsec);
    }


    /** \brief Initialize a timespec_ex from an double in seconds.
     *
     * This constructors allows you to directly set a timespec_ex
     * to the specified \p sec value.
     *
     * \param[in] sec  The seconds to copy to timespec_ex.
     */
    timespec_ex(double sec)
    {
        set(sec);
    }


    /** \brief Set the timespec_ex to the specified timespec_ex.
     *
     * This function copies the specified timespec_ex (\p t) to this
     * timespec_ex object.
     *
     * \param[in] t  The timespec to copy in this timespec_ex.
     *
     * \return A reference to this object.
     */
    timespec_ex & operator = (timespec_ex t)
    {
        tv_sec = t.tv_sec;
        tv_nsec = t.tv_nsec;
        return *this;
    }


    /** \brief Set the timespec_ex to the specified timespec.
     *
     * This function copies the specified timespec to this timespec_ex
     * object.
     *
     * \param[in] t  The timespec to copy in this timespec_ex.
     *
     * \return A reference to this object.
     */
    timespec_ex & operator = (timespec const & t)
    {
        tv_sec = t.tv_sec;
        tv_nsec = t.tv_nsec;
        return *this;
    }


    /** \brief Set the timespec_ex to the number of nanoseconds.
     *
     * This function saves the number of nanoseconds in \p nsec as a
     * tv_sec and tv_nsec representation.
     *
     * \param[in] nsec  The nano-seconds to save in this timespec_ex.
     *
     * \return A reference to this object.
     */
    timespec_ex & operator = (std::int64_t nsec)
    {
        set(nsec);
        return *this;
    }


    /** \brief Set this timespec_ex to the number of seconds in \p sec.
     *
     * This function transforms the specified double \p sec in a timespec_ex.
     *
     * \note
     * At this time, the number of nanoseconds is floored.
     *
     * \param[in] sec  The number of seconds defined in a double.
     *
     * \return A reference to this object.
     */
    timespec_ex & operator = (double sec)
    {
        set(sec);
        return *this;
    }


    /** \brief Set the timespec_ex to the number of nanoseconds.
     *
     * This function saves the number of nanoseconds in \p nsec as a
     * tv_sec and tv_nsec representation.
     *
     * \param[in] t  The nano-seconds to save in this timespec_ex.
     *
     * \return A reference to this object.
     */
    void set(timespec_ex const & t)
    {
        tv_sec = t.tv_sec;
        tv_nsec = t.tv_nsec;
    }


    /** \brief Set the timespec_ex to the specified timespec.
     *
     * This function copies the timespec in \p t in this timespec_ex object.
     *
     * \param[in] t  The timespec to save in this timespec_ex.
     *
     * \return A reference to this object.
     */
    void set(timespec const & t)
    {
        tv_sec = t.tv_sec;
        tv_nsec = t.tv_nsec;
    }


    /** \brief Set the timespec_ex to the specified values.
     *
     * This function allows you to set the timespec_ex to the specified
     * number of seconds (\p sec) and nano-seconds (\p nsec).
     *
     * \param[in] sec  The new number of seconds.
     * \param[in] nsec  The new number of nano-seconds.
     */
    void set(time_t sec, long nsec)
    {
        tv_sec = sec;
        tv_nsec = nsec;
    }


    /** \brief Set the timespec_ex to the number of nanoseconds.
     *
     * This function saves the number of nanoseconds in \p nsec as a
     * tv_sec and tv_nsec representation.
     *
     * \param[in] nsec  The nano-seconds to save in this timespec_ex.
     *
     * \return A reference to this object.
     */
    void set(std::int64_t nsec)
    {
        bool const neg(nsec < 0);
        if(neg)
        {
            nsec = -nsec;
        }
        tv_sec = static_cast<time_t>(nsec / 1'000'000'000LL);
        tv_nsec = static_cast<long>(nsec % 1'000'000'000LL);
        if(neg)
        {
            *this = -*this;
        }
    }


    /** \brief Set this timespec_ex to the number of seconds in \p sec.
     *
     * This function transforms the specified double \p sec in a timespec_ex.
     *
     * \note
     * At this time, the number of nanoseconds is floored.
     *
     * \param[in] sec  The number of seconds defined in a double.
     *
     * \return A reference to this object.
     */
    void set(double sec)
    {
        bool const neg(sec < 0.0);
        if(neg)
        {
            sec = -sec;
        }
        tv_sec = static_cast<time_t>(floor(sec));
        tv_nsec = static_cast<long>((sec - floor(sec)) * 1.0e9);
        if(neg)
        {
            *this = -*this;
        }
    }

    /** \brief Get system time.
     *
     * This function reads the system time and saves it into this
     * timespec_ex object.
     *
     * \todo
     * Look into whether we want to return an error if clock_gettime()
     * fails.
     *
     * \param[in] clk_id  The type of clock you want to query.
     *
     * \return A timespec_ex representing the specified \p clk_id.
     */
    static timespec_ex gettime(clockid_t clk_id = CLOCK_REALTIME)
    {
        timespec_ex result;
        clock_gettime(clk_id, &result);
        return result;
    }


    /** \brief Extract the timespec_ex as an int64_t value.
     *
     * This function transforms a timespec_ex structure in an int64_t
     * in nanoseconds.
     *
     * \return This timespec_ex converted to nanoseconds.
     */
    std::int64_t to_nsec() const
    {
        return tv_nsec
             + tv_sec * 1'000'000'000LL;
    }


    /** \brief Extract the timespec_ex as an int64_t value.
     *
     * This function transforms a timespec_ex structure in an int64_t
     * in microseconds. The bottom 3 digits are lost. No rounding
     * happens.
     *
     * \note
     * To can round the value up by first adding 500 (round) or 999 (ceil)
     * to the timespec_ex value.
     *
     * \return This timespec_ex converted to microseconds.
     */
    std::int64_t to_usec() const
    {
        return tv_nsec / 1'000LL
             + tv_sec * 1'000'000LL;
    }


    /** \brief Extract the timespec_ex as a double value.
     *
     * This function transforms a timespec_ex structure into a double
     * in seconds. The nanoseconds are added as one billionth of a
     * second.
     *
     * \return The timespec_ex converted the seconds.
     */
    double to_sec() const
    {
        return static_cast<double>(tv_sec)
             + static_cast<double>(tv_nsec) / 1.0e9;
    }


    /** \brief Format the date to the specified format.
     *
     * This function transforms the time in a string and returns that string.
     * The function uses the strftime(). See that manual page to define
     * the format properly.
     *
     * Unless you use the `%N` extension, the output will not include the
     * precision available in the timespec (i.e. without the %N, the output
     * is to the second, the timespec has nanoseconds available).
     *
     * \param[in] format  The format used to transform the date and time in
     * a string.
     * \param[in] local  Whether to generate a local time or use UTC.
     *
     * \return The formatted date and time.
     */
    std::string to_string(std::string const & format = std::string(), bool local = false) const
    {
        struct tm date_and_time = {};
        struct tm * ptr(nullptr);
        if(local)
        {
            ptr = localtime_r(&tv_sec, &date_and_time);
        }
        else
        {
            ptr = gmtime_r(&tv_sec, &date_and_time);
        }
        if(ptr == nullptr)
        {
            throw overflow("the specified number of seconds could not be transformed in a 'struct tm'.");
        }
        std::string f(format);
        if(f.empty())
        {
            // TODO: actually retrieve the locale() format and
            //       search for "%T" or "%S" and insert ".%N"
            //       right after either
            //
            f = "%c.%N";
        }
        std::string::size_type pos(f.find("%N"));
        if(pos != std::string::npos)
        {
            std::string n(std::to_string(tv_nsec));
            if(n.length() > 9)
            {
                throw overflow("tv_nsec is 1 billion or more, which is invalid.");
            }
            std::string const indent(9 - n.length(), '0');
            n = indent + n;
            f = f.substr(0, pos) + n + f.substr(pos + 2);
        }
        char buf[256];
        size_t const sz(strftime(buf, sizeof(buf), f.c_str(), &date_and_time));
        if(sz == 0)
        {
            // this happens with just a "%p" and "wrong locale"
            // or when the buffer is too small, which should not
            // be the case unless you add much more than the format
            // in that string
            //
            throw overflow(
                  "the specified strftime() format \""
                + format
                + "\" failed.");
        }

        return std::string(buf, sz);
    }


    /** \brief Validate this timespec_ex structure.
     *
     * This function returns true if this timespec_ex structure is considered
     * valid.
     *
     * At this time, the validation consists of verifying that the
     * nanoseconds is a number between 0 and 1 billion (maximum excluded).
     *
     * \note
     * Negative timespec_ex are represented by a negative tv_sec. The
     * tv_nsec can never be negative after a valid operation.
     *
     * \return true if the timespec_ex is considered valid.
     */
    bool valid() const
    {
        return tv_nsec < 1'000'000'000LL;
    }


    /** \brief Check whether this timespec_ex is negative.
     *
     * This function checks whether the number represents a negative
     * timespec_ex. This is true if the number of seconds is negative.
     *
     * \note
     * The first negative timespec_ex is { -1, 999,999,999 }.
     *
     * \return true if the timespec_ex is considered negative.
     */
    bool negative() const
    {
        return tv_sec < 0LL;
    }


    /** \brief Add two timespec_ex together.
     *
     * This function adds \p rhs to this timespec_ex value and returns a
     * new timespec_ex with the result. This timespec_ex is not modified.
     *
     * \param[in] rhs  The right handside to add to this number.
     *
     * \return A new timespec_ex representing the sum of 'this' and rhs.
     */
    timespec_ex add(timespec_ex const & rhs) const
    {
        bool const lneg(negative());
        bool const rneg(rhs.negative());

        timespec_ex lp(lneg ? -*this : *this);
        timespec_ex rp(rneg ? -rhs : rhs);

        timespec_ex result;

        switch((lneg ? 1 : 0) + (rneg ? 2 : 0))
        {
        case 0:     // positive + positive
        case 3:     // negative + negative
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


    /** \brief Compare two timespec_ex together.
     *
     * This function compares two timespecs and determine whether they
     * are equal (0), 'this' is smaller (-1) or \p rhs is smaller (1).
     *
     * \param[in] rhs  The right handside to compare.
     *
     * \return -1, 0, or 1 depending on the order between \p lhs and \p rhs.
     */
    int compare(timespec_ex const & rhs) const
    {
        if(tv_sec == rhs.tv_sec)
        {
            return tv_nsec == rhs.tv_nsec
                    ? 0
                    : (tv_nsec < rhs.tv_nsec ? -1 : 1);
        }

        return tv_sec < rhs.tv_sec ? -1 : 1;
    }


    /** \brief Check whether the timespec_ex is zero.
     *
     * This function returns true if the timespec_ex represents zero
     * (i.e. zero seconds and zero nano-seconds).
     *
     * \return true if the timespec_ex is zero, false if not zero.
     */
    bool operator ! () const
    {
        return tv_sec == 0 && tv_nsec == 0;
    }


    /** \brief Add the right handside to this timespec_ex.
     *
     * This operator adds the right handside to this object.
     *
     * \param[in] rhs  Another timespec_ex to add to this one.
     *
     * \return A reference to this timespec_ex object.
     */
    timespec_ex & operator += (timespec_ex const & rhs)
    {
        *this = add(rhs);
        return *this;
    }


    /** \brief Add 1 nanosecond to this timespec_ex object.
     *
     * This function adds exactly one nanonsecond to this timespec_ex
     * object.
     *
     * \return A reference to this timespec_ex object.
     */
    timespec_ex & operator ++ ()
    {
        *this += 1L;
        return *this;
    }


    /** \brief Add 1 nanosecond to this timespec_ex object.
     *
     * This function adds exactly one nanonsecond to this timespec_ex
     * object and returns the original value.
     *
     * \return A copy of this timespec_ex object before the add() occurs.
     */
    timespec_ex operator ++ (int)
    {
        timespec_ex result(*this);
        *this += 1L;
        return result;
    }


    /** \brief Add two timespec_ex objects and return the result.
     *
     * This function computes the addition of this timespec_ex object
     * and the \p t timespec_ex and returns the result. The inputs
     * are not modified.
     *
     * \param[in] t  The right handside to add to this timespex_ex object.
     *
     * \return The sum of the inputs in a new timespec_ex object.
     */
    timespec_ex operator + (timespec_ex const & t) const
    {
        timespec_ex result(*this);
        result += t;
        return result;
    }


    /** \brief Subtract the right handside from this timespec_ex.
     *
     * This operator subtracts the right handside from this object.
     *
     * \param[in] rhs  Another timespec_ex to subtract from this one.
     *
     * \return A reference to this timespec_ex object.
     */
    timespec_ex & operator -= (timespec_ex const & rhs)
    {
        *this = add(-rhs);
        return *this;
    }


    /** \brief Subtract 1 nanosecond from timespec_ex object.
     *
     * This function subtracts exactly one nanonsecond from this
     * timespec_ex object.
     *
     * \return A reference to this timespec_ex object.
     */
    timespec_ex & operator -- ()
    {
        *this -= 1L;
        return *this;
    }


    /** \brief Subtract 1 nanosecond from timespec_ex object.
     *
     * This function subtracts exactly one nanonsecond from this
     * timespec_ex object and returns the original value.
     *
     * \return A copy of this timespec_ex object before the subtract occurs.
     */
    timespec_ex operator -- (int)
    {
        timespec_ex result(*this);
        *this -= 1L;
        return result;
    }


    /** \brief Compute the additive opposite of the right handside timespec_ex.
     *
     * This function computers the opposite of the right handside timespec_ex
     * and returns a copy with the result.
     *
     * This is equivalent to computing `0 - t`.
     *
     * \param[in] t  The right handside time to negate.
     *
     * \return A timespec_ex representing the additive opposite of the input.
     */
    timespec_ex operator - () const
    {
        timespec_ex result(timespec{ -tv_sec, -tv_nsec });
        if(result.tv_nsec < 0)
        {
            --result.tv_sec;
            result.tv_nsec += 1'000'000'000L;
        }
        return result;
    }


    /** \brief Subtract \p t from this timespec_ex object.
     *
     * This function computes the difference of this timespec_ex object
     * and the \p t timespec_ex object and returns the result. The inputs
     * are not modified.
     *
     * \param[in] rhs  The right handside to subtract from this timespex_ex
     * object.
     *
     * \return The different of the inputs in a new timespec_ex object.
     */
    timespec_ex operator - (timespec_ex const & rhs) const
    {
        timespec_ex result(*this);
        result -= rhs;
        return result;
    }


    /** \brief Compare whether the two timespec_ex are equal.
     *
     * \param[in] t  The time to compare against.
     *
     * \return true if both timespec_ex objects are equal.
     */
    bool operator == (timespec_ex const & t) const
    {
        return compare(t) == 0;
    }


    /** \brief Compare whether the two timespec_ex are not equal.
     *
     * \param[in] t  The time to compare against.
     *
     * \return true if both timespec_ex objects are not equal.
     */
    bool operator != (timespec_ex const & t) const
    {
        return compare(t) != 0;
    }


    /** \brief Compare whether the left handside is smaller.
     *
     * \param[in] t  The time to compare against.
     *
     * \return true if the left handside timespec_ex object is smaller.
     */
    bool operator < (timespec_ex const & t) const
    {
        return compare(t) == -1;
    }


    /** \brief Compare whether the left handside is smaller or equal.
     *
     * \param[in] t  The time to compare against.
     *
     * \return true if the left handside timespec_ex object is smaller
     * or equal.
     */
    bool operator <= (timespec_ex const & t) const
    {
        return compare(t) <= 0;
    }


    /** \brief Compare whether the left handside is larger.
     *
     * \param[in] t  The time to compare against.
     *
     * \return true if the left handside timespec_ex object is larger.
     */
    bool operator > (timespec_ex const & t) const
    {
        return compare(t) == 1;
    }


    /** \brief Compare whether the left handside is larger or equal.
     *
     * \param[in] t  The time to compare against.
     *
     * \return true if the left handside timespec_ex object is larger
     * or equal.
     */
    bool operator >= (timespec_ex const & t) const
    {
        return compare(t) >= 0;
    }
};







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


} // namespace snapdev
// vim: ts=4 sw=4 et