// Copyright (c) 2021-2024  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Class extending the timespec structure.
 *
 * The C library includes a timespec structure and various functions used
 * to retrieve the system time. We also offer ways to compute durations
 * and add/remove a duration from a timespec.
 *
 * The class also supports convertions to string, negation, and a few
 * other features.
 */

// self
//
#include    <snapdev/join_strings.h>
#include    <snapdev/tokenize_format.h>



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
#include    <thread>
#include    <type_traits>


// C
//
#include    <langinfo.h>
#include    <stdlib.h>
#include    <string.h>
#include    <sys/time.h>
#include    <time.h>



extern "C" {
/** \brief Define the nl_langinfo() function type.
 *
 * This is primarily used to allow for testing with any format which
 * the test can control through an nl_langinfo() wrapper.
 *
 * See nl_langinfo(3) for details about the actual function.
 */
typedef char *(nl_langinfo_func_t)(nl_item item);
}


namespace snapdev
{

DECLARE_MAIN_EXCEPTION(timespec_ex_exception);

DECLARE_EXCEPTION(timespec_ex_exception, clock_error); // LCOV_EXCL_LINE
DECLARE_EXCEPTION(timespec_ex_exception, syntax_error);
DECLARE_EXCEPTION(timespec_ex_exception, overflow);


/** \brief Internal declarations.
 *
 * The ostream system makes use of a few internal variables and functions
 * which are defined here.
 */
namespace
{



/** \brief Set of flags attached to an ostream for a timespec_ex object.
 *
 * This structure holds data used to output a timespec_ex object in an ostream.
 *
 * The f_remove_trailing_zeroes parameter is used to call the to_timestamp()
 * member function.
 */
struct _ostream_info
{
    bool                f_remove_trailing_zeroes = true;
};


/** \brief Mutex used to allocate the ostream index.
 *
 * When getting the ostream index, it requires multiple instructions and
 * a function call. As a result we need a mutex to make sure that the
 * allocate happens only once in a single thread.
 */
inline std::mutex g_mutex = std::mutex();


/** \brief Whether the ostream index was already allocated or not.
 *
 * This variable is used to know whether we need to allocate the ostream
 * index (false) or whether that was already done.
 */
inline bool g_ostream_index_allocated = false;


/** \brief The ostream index.
 *
 * The timespec_ex object allows for one flag used to determine whether
 * trailing zeroes after the decimal point should be printed or not.
 * This flag is recorded in the ostream concerned using this index.
 */
inline int g_ostream_index = 0;


/** \brief Retrieve the ios_base index for the timespec_ex class.
 *
 * In order to allow for flags specific to the timespec_ex class in ostream
 * functions, we need an index which this function supplies. The index
 * is allocated whenever you first use one of the timespec_ex ostream
 * functions.
 *
 * \return The unique ostream index for the timespec_ex class.
 */
inline int get_ostream_index()
{
    std::lock_guard lock(g_mutex);

    if(!g_ostream_index_allocated)
    {
        g_ostream_index_allocated = true;
        g_ostream_index = std::ios_base::xalloc();
    }

    return g_ostream_index;
}



} // no name namespace


class timespec_ex;
inline timespec_ex now(clockid_t clk_id = CLOCK_REALTIME);


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


#if 0
// keeping this just in case, for older versions of g++ or other C++ compiler
// which may fail
//
    /** \brief Initialize a timespec_ex from an int in seconds.
     *
     * This constructors allows us to compile our tests.
     *
     * \bug
     * It is unfortunate that at the moment this constructor is required.
     * This is likely to cause bugs because the choice between int64_t
     * and int is not a good choice here.
     *
     * \param[in] sec  The seconds to copy to timespec_ex.
     */
    timespec_ex(int sec)
    {
        set(sec * static_cast<std::int64_t>(1'000'000'000));
    }
#endif


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


    /** \brief Initialize a timespec_ex from a timestamp.
     *
     * When using the ostream << operator, you generate a simple Unix
     * timestamp with a nanosecond precision. This is the converse
     * function which you can use to convert the value back to a
     * timespec_ex value.
     *
     * \warning
     * This will not work properly if you used the to_string() function,
     * even if you used the format "%s.%N". In that case, make sure to
     * use the from_string() function instead.
     *
     * \param[in] timestamp  The string to convert to a timespec_ex.
     */
    timespec_ex(std::string const & timestamp)
    {
        set(timestamp);
    }


    /** \brief Initialize a timespec_ex from a tm structure.
     *
     * This function initializes this timespec_ex object from a time
     * structure (struct tm). As a result, the nanosecond field is
     * always set to 0.
     *
     * \param[in] t  The tm structure to convert to this timespec_ex.
     */
    timespec_ex(tm const & t)
    {
        from_tm(t);
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


    /** \brief Set this timespec_ex to a timestamp.
     *
     * When using the ostream << operator, you generate a simple Unix
     * timestamp with a nanosecond precision. This is the converse
     * function which you can use to convert the value back to a
     * timespec_ex value.
     *
     * \warning
     * This will not work properly if you used the to_string() function,
     * even if you used the format "%s.%N". In that case, make sure to
     * use the from_string() function instead.
     *
     * \param[in] timestamp  The string to convert to a timespec_ex.
     *
     * \return A reference to this object.
     */
    timespec_ex & operator = (std::string const & timestamp)
    {
        set(timestamp);
        return *this;
    }


    /** \brief Set this timespec_ex to a tm structure.
     *
     * This function sets this timespec_ex object from a time
     * structure (struct tm). As a result, the nanosecond field is
     * always set to 0.
     *
     * \param[in] t  The tm structure to convert to this timespec_ex.
     */
    timespec_ex & operator = (tm const & t)
    {
        from_tm(t);
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

    /** \brief Convert a timestamp in a string to a timespec_ex structure.
     *
     * This function converts the timestamp as generated by the ostream
     * << operator back into a timespec_ex structure.
     *
     * The parser here assumes that the input is a valid Unix UTC timestamp
     * with seconds and nanoseconds separated by a period.
     *
     * The number may be negative (the number of seconds is a signed number).
     *
     * The number can end with an 's' representing the unit "second".
     *
     * The function trims the input string (ignores the leading and ending
     * spaces).
     *
     * Examples of valid input:
     *
     * \code
     *     "1685372468.564231883"
     *     "1685372468.564231883s"
     *     "  1685372468.564231883s  "
     *     "1685372468.564"
     *     "-1685372468.56423"
     *     "+1685372468.56423"
     * \endcode
     *
     * Note that the precision does not need to be to the nanosecond. The
     * function properly interprets the number after the decimal point
     * by reading the equivalent of 9 digits, using '0' when all 9 digits
     * are not present.
     *
     * \exception overflow
     * If the number of seconds is too large, this exception is raised.
     *
     * \exception syntax_error
     * The number of seconds must be indicated with at least one digit,
     * even if zero (0). If the number starts with any other character,
     * this exception is raised. Further, if the string includes any
     * character other than a sign ('+' or '-' at the start), digits,
     * a decimal point, and the 's' unit at the end, then a syntax error
     * is also raised.
     *
     * \param[in] timestamp  The string to convert to a timespec_ex.
     */
    void set(std::string const & timestamp)
    {
        char const * s(timestamp.c_str());
        while(isspace(*s))
        {
            ++s;
        }
        time_t sign(1);
        if(*s == '-')
        {
            ++s;
            sign = -1;
        }
        else if(*s == '+')
        {
            ++s;
        }
        time_t sec(0);
        long nsec(0);
        if(*s < '0' || *s > '9')
        {
            throw syntax_error("number of seconds must include at least one digit, even if '0'.");
        }
        for(; *s >= '0' && *s <= '9'; ++s)
        {
            sec = sec * 10 + (*s - '0');
            if(sec < 0)
            {
                throw overflow("number of seconds is too large.");
            }
        }
        if(*s == '.')
        {
            ++s;

            // we count and only read up to 9 digits which means we cannot
            // have an overflow or an invalid number (>= 1,000,000,000)
            //
            int count(0);
            for(; *s >= '0' && *s <= '9' && count < 9; ++s, ++count)
            {
                nsec = nsec * 10 + (*s - '0');
            }

            // skip additional digits
            //
            for(; *s >= '0' && *s <= '9'; ++s);

            // in case all 9 digits were not included
            //
            for(; count < 9; ++count)
            {
                nsec *= 10;
            }
        }

        // give user the ability to enter 's' for "second" as a unit
        //
        if(*s == 's')
        {
            ++s;
        }
        while(isspace(*s))
        {
            ++s;
        }
        if(*s != '\0')
        {
            throw syntax_error(std::string("number include unexpected characters (") + s + ").");
        }

        tv_sec = sec * sign;
        tv_nsec = nsec;
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
        int const r(clock_gettime(clk_id, &result));
        if(r != 0)
        {
            // LCOV_EXCL_START
            int const e(errno);
            throw clock_error(
                  "clock_gettime() failed: "
                + std::to_string(e)
                + ", "
                + strerror(e));
            // LCOV_EXCL_STOP
        }
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


    /** \brief Convert the timespec_ex to a simple Unix timestamp.
     *
     * This function converts this timespec_ex structure to a string.
     * The seconds and nanoseconds are not adjusted in any way making
     * it possible to use this function to serialize the timespec_ex
     * value. Use the set(std::string const & timestamp) function
     * to convert the string back to a timespec_ex value.
     *
     * If the \p remove_trailing_zeroes parameter is set to true, the
     * nanoseconds trailing zeroes are removed from the resulting string.
     * If there were no nanoseconds (0), then the period also gets
     * removed.
     *
     * \param remove_trailing_zeroes  Whether to keep or remove trailing
     * zeroes and the decimal point.
     *
     * \return A string representing this timespec_ex exactly.
     *
     * \sa set(std::string const & timestamp);
     */
    std::string to_timestamp(bool remove_trailing_zeroes = false) const
    {
        std::stringstream s;
        s << tv_sec << "." << std::setw(9) << std::setfill('0') << tv_nsec;
        std::string result(s.str());
        if(remove_trailing_zeroes)
        {
            while(result.back() == '0')
            {
                result.pop_back();
            }
            if(result.back() == '.')
            {
                result.pop_back();
            }
        }
        return result;
    }


    /** \brief Format the date to the specified format.
     *
     * \warning
     * This function uses strftime() which interprets the input time as
     * localtime, no matter what. You may want to consider using the
     * ostream<<() function if you want to save the time as a UTC string
     * as seconds & nanoseconds or use the to_timestamp() function.
     *
     * This function transforms the time in a string and returns that string.
     * The function uses the strftime(). See that manual page to define
     * the format properly.
     *
     * This function supports a format extension: `%N`, to output 9 digits
     * with the nanoseconds available in the `timespec_ex` object. Without
     * the `%N`, the precision is to the second.
     *
     * It is possible to retrieve the timezone using the `"%z"` or `"%Z"`
     * format. If you want the full name, since we are beyond C++20,
     * you can use `std::chrono` like so:
     *
     * \code
     *     std::chrono::current_zone()->name()
     * \endcode
     *
     * \warning
     * Internally, the function uses a buffer of 256 bytes maximum.
     * Make sure your format is limited to the date and time. Things
     * you want to prepend or append should be managed outside of this
     * call.
     *
     * \warning
     * The `%N` should be preceded by a period if included just after the
     * seconds (`%s` or `%S`). Some format arguments do not end with seconds,
     * such as the `%c`, `%r`, `%X`, `%EX`. If you want to use those, then
     * the `%N` should be separated by a space and probably followed by `ns`.
     * Note, however, that leading `0` are automatically added so `%N` is
     * always 9 characters at the moment.
     *
     * \exception overflow
     * In case the conversion of the `tv_sec` fails, this exception is raised.
     *
     * \param[in] format  The format used to transform the date and time in
     * a string.
     *
     * \return The formatted date and time.
     *
     * \sa from_string()
     * \sa to_timestamp()
     * \sa set(std::string const & timestamp)
     */
    template<nl_langinfo_func_t nl_langinfo_wrapper = nl_langinfo>
    std::string to_string(std::string const & format = std::string(), bool use_localtime = false) const
    {
        struct tm date_and_time = {};
        struct tm * ptr(nullptr);
        if(use_localtime)
        {
            ptr = localtime_r(&tv_sec, &date_and_time);
        }
        else
        {
            ptr = gmtime_r(&tv_sec, &date_and_time);
        }
        if(ptr == nullptr)
        {
            throw overflow("the specified number of seconds could not be transformed in a 'struct tm'."); // LCOV_EXCL_LINE
        }
        format_item<char>::list_t format_items;
        std::string f(format);
        if(f.empty())
        {
            // compute the default using the locale
            //
            // if there is a %r, we convert it to the T_FMT_AMPM
            // if there is a %X, we convert it to the T_FMT
            // if there is a %EX, we convert it to the ERA_T_FMT
            // and then search for "%T" or "%S" or "%s" and insert ".%N"
            // right after
            //
            // Note: our algorithm doesn't work very well if the format
            //       includes multiple %r, %X, %EX, %T, %S
            //
            f = nl_langinfo_wrapper(D_T_FMT);
            if(f.empty())
            {
                // use POSIX default if %c is not defined in the locale
                //
                f = "%a %b %e %H:%M:%S %Y";
            }
            format_items = tokenize_format<
                                  char
                                , snapdev::strftime_letter_traits<char>
                                , snapdev::strftime_flag_traits<char>>(f);

            // replace 'r', 'X', 'EX' with their content because those
            // will include the actual 'T', 'S', or 's'
            //
            // count the number of times we loop, if more than 10, forget
            // it; that means the locale is broken (creates an infinite loop)
            //
            int loop(0);
            for(auto it(format_items.begin()); it != format_items.end() && loop < 10; )
            {
                int t(0);
                switch(it->format())
                {
                case 'r':
                    // "r"
                    t = T_FMT_AMPM;
                    break;

                case 'X':
                    if(it->has_flags(snapdev::strftime_flag_traits<char>::FORMAT_FLAG_EXTENDED))
                    {
                        // "EX"
                        t = ERA_T_FMT;
                    }
                    else
                    {
                        // "X"
                        t = T_FMT;
                    }
                    break;

                default:
                    ++it;
                    continue;

                }
                std::string const sub_format(nl_langinfo_wrapper(t));
                if(!sub_format.empty())
                {
                    auto const & sub_format_items(tokenize_format<
                                      char
                                    , snapdev::strftime_letter_traits<char>
                                    , snapdev::strftime_flag_traits<char>>(sub_format));
                    format_items.insert(
                                      it
                                    , sub_format_items.begin()
                                    , sub_format_items.end());
                }
                it = format_items.erase(it);
                ++loop;
            }

            for(auto it(format_items.begin()); it != format_items.end(); ++it)
            {
                switch(it->format())
                {
                case 'T':
                case 'S':
                case 's':
                    {
                        ++it;

                        snapdev::format_item<char> period;
                        period.string(".");
                        format_items.insert(it, period);

                        snapdev::format_item<char> nanoseconds;
                        nanoseconds.string("%N");
                        nanoseconds.format('N');
                        it = format_items.insert(it, nanoseconds);
                    }
                    break;

                }
            }
        }
        else
        {
            // user format, do not temper with it, if no .%N, that's
            // the user's choice
            //
            format_items = tokenize_format<
                              char
                            , snapdev::strftime_letter_traits<char, true>
                            , snapdev::strftime_flag_traits<char>
                            , snapdev::strftime_number_traits<char>>(f);
        }

        // Add support for microseconds and milliseconds
        //
        for(auto it(format_items.begin()); it != format_items.end();)
        {
            if(it->format() == 'N')
            {
                std::string n(std::to_string(tv_nsec));
                if(n.length() > 9)
                {
                    throw overflow("tv_nsec is 1 billion or more, which is invalid.");
                }
                if(!it->has_flags(snapdev::strftime_flag_traits<char>::FORMAT_FLAG_NO_PAD))
                {
                    // prepend padding zeroes or spaces
                    //
                    char const pad(it->has_flags(snapdev::strftime_flag_traits<char>::FORMAT_FLAG_PAD_WITH_SPACES)
                                ? ' '
                                : '0');
                    std::string const indent(9 - n.length(), pad);
                    n = indent + n;
                }
                if(it->has_flags(snapdev::strftime_flag_traits<char>::FORMAT_FLAG_EXTENDED))
                {
                    // remove trailing zeroes
                    //
                    std::string::size_type const last_non_zero(n.find_last_not_of('0'));
                    if(last_non_zero == std::string::npos)
                    {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wrestrict"
                        n = "0";
#pragma GCC diagnostic pop
                    }
                    else
                    {
                        n = n.substr(0, last_non_zero + 1);
                    }
                }

                // replace the %N with the final nanoseconds string
                //
                snapdev::format_item<char> nanoseconds;
                nanoseconds.string(n);
                format_items.insert(it, nanoseconds);

                it = format_items.erase(it);
            }
            else
            {
                ++it;
            }
        }

        // convert the format items back in a format string
        //
        snapdev::format_item<char> empty_item;
        f = snapdev::join_strings(format_items, empty_item);

        char buf[256];
        std::size_t const sz(strftime(buf, sizeof(buf), f.c_str(), &date_and_time));
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


    /** \brief Convert a string with a date to a timespec_ex.
     *
     * This function is the converse of the to_string() function. It
     * converts a string (\p s) to a timespec_ex time and date using
     * the specified \p format.
     *
     * \note
     * The \p format is used with the strptime() function. Please refer
     * to that function for additional information. To some extend, the
     * function also supports our %N extension.
     *
     * \param[in] s  The string to be converted to this timespec_ex.
     * \param[in] format  The format to use to parse the input data.
     *
     * \sa to_string()
     */
    void from_string(std::string const & s, std::string const & format)
    {
        // I really have no clue how to properly support the %N option
        // without rewriting strptime() which I really don't want to do
        // (note: the strptime() function returns a pointer on the first
        // unconsumed character, so we may be able to use that %N is at
        // the end)
        //
        // one way is to look for the '.' (assuming the %N is preceded
        // by such) but some people write dates with those as in:
        //
        //    29.05.2023
        //
        if(format.find("%N") != std::string::npos)
        {
            throw libexcept::fixme("the from_string() %N extension is not yet implemented.");
        }

        struct tm t;
        strptime(s.c_str(), format.c_str(), &t);
        from_tm(t);
    }


    /** \brief Put the specifed tm data in this timespec_ex.
     *
     * This function takes a tm structure as input and converts it to
     * a timespec_ex. The input is expected to be a valid UTC time
     * structure.
     *
     * \param[in] t  The tm to convert to this timespec_ex.
     */
    void from_tm(tm const & t)
    {
        tm copy(t);
        tv_sec = timegm(&copy);
        tv_nsec = 0;
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
     * are equal (0), 'this' is smaller (negative) or \p rhs is smaller
     * (positive).
     *
     * \param[in] rhs  The right handside to compare.
     *
     * \return negative, 0, or positive depending on the order between
     *         \p lhs and \p rhs.
     */
    int compare(timespec_ex const & rhs) const
    {
        // see operator <=> ... catch2 seems to not accept these just yet
        //return tv_sec == rhs.tv_sec
        //            ? tv_nsec <=> rhs.tv_nsec
        //            : tv_sec <=> rhs.tv_sec;

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
     * \return true if the timespec_ex is not zero, false if zero.
     */
    operator bool () const
    {
        return tv_sec != 0 || tv_nsec != 0;
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
     * and the \p rhs timespec_ex and returns the result. The inputs
     * are not modified.
     *
     * \param[in] rhs  The right handside to add to this timespex_ex object.
     *
     * \return The sum of the inputs in a new timespec_ex object.
     */
    timespec_ex operator + (timespec_ex const & rhs) const
    {
        timespec_ex result(*this);
        result += rhs;
        return result;
    }


    /** \brief Add a number to this timespec_ex object and return the result.
     *
     * This function computes the addition of this timespec_ex object
     * and the \p rhs number and returns the result. The input timespec_ex
     * is not modified.
     *
     * \param[in] rhs  An integer or floating point number.
     *
     * \return The sum of the inputs in a new timespec_ex object.
     */
    template<typename T>
    std::enable_if_t<std::is_integral_v<T>
        || std::is_floating_point_v<T>, timespec_ex> operator + (T rhs) const
    {
        timespec_ex result(*this);
        result += rhs;
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
     * and the \p rhs timespec_ex object and returns the result. The inputs
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


    /** \brief Subtract a number from this timespec_ex object and return the result.
     *
     * This function computes the subtraction of this timespec_ex object
     * and the \p rhs number and returns the result. The input timespec_ex
     * is not modified.
     *
     * \param[in] rhs  An integer or floating point number.
     *
     * \return The subtraction of the inputs in a new timespec_ex object.
     */
    template<typename T>
    std::enable_if_t<std::is_integral_v<T>
        || std::is_floating_point_v<T>, timespec_ex> operator - (T rhs) const
    {
        timespec_ex result(*this);
        result -= rhs;
        return result;
    }


#if 0
    // it looks like catch is not quite ready for this one
    auto operator <=> (timespec_ex const & t) const
    {
        return compare(t);
    }
#endif

    /** \brief Compare whether the two timespec_ex are equal.
     *
     * \param[in] rhs  The time to compare against.
     *
     * \return true if both timespec_ex objects are equal.
     */
    bool operator == (timespec_ex const & rhs) const
    {
        return compare(rhs) == 0;
    }


    /** \brief Compare whether the timespec_ex equal a number.
     *
     * The input number is taken as a number of seconds. If a floating
     * point is used, you can add precision up to nanoseconds.
     *
     * \param[in] rhs  The number to compare against.
     *
     * \return true if both timespec_ex objects are equal.
     */
    template<typename T>
    std::enable_if_t<std::is_integral_v<T>
        || std::is_floating_point_v<T>, bool> operator == (T rhs) const
    {
        return compare(rhs) == 0;
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


    /** \brief Compare whether the timespec_ex is not equal to a number.
     *
     * The input number is taken as a number of seconds. If a floating
     * point is used, you can add precision up to nanoseconds.
     *
     * \param[in] rhs  The number to compare against.
     *
     * \return true if both timespec_ex objects are not equal.
     */
    template<typename T>
    std::enable_if_t<std::is_integral_v<T>
        || std::is_floating_point_v<T>, bool> operator != (T rhs) const
    {
        return compare(rhs) != 0;
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


    /** \brief Compare whether the timespec_ex is less than a number.
     *
     * The input number is taken as a number of seconds. If a floating
     * point is used, you can add precision up to nanoseconds.
     *
     * \param[in] rhs  The number to compare against.
     *
     * \return true if the timespec_ex object is less than the number.
     */
    template<typename T>
    std::enable_if_t<std::is_integral_v<T>
        || std::is_floating_point_v<T>, bool> operator < (T t) const
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


    /** \brief Compare whether the timespec_ex is less than or equal to a number.
     *
     * The input number is taken as a number of seconds. If a floating
     * point is used, you can add precision up to nanoseconds.
     *
     * \param[in] rhs  The number to compare against.
     *
     * \return true if the timespec_ex object is less than or equal to the number.
     */
    template<typename T>
    std::enable_if_t<std::is_integral_v<T>
        || std::is_floating_point_v<T>, bool> operator <= (T t) const
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


    /** \brief Compare whether the timespec_ex is greater than a number.
     *
     * The input number is taken as a number of seconds. If a floating
     * point is used, you can add precision up to nanoseconds.
     *
     * \param[in] rhs  The number to compare against.
     *
     * \return true if the timespec_ex object is greater than the number.
     */
    template<typename T>
    std::enable_if_t<std::is_integral_v<T>
        || std::is_floating_point_v<T>, bool> operator > (T t) const
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


    /** \brief Compare whether the timespec_ex is greater than or equal to a number.
     *
     * The input number is taken as a number of seconds. If a floating
     * point is used, you can add precision up to nanoseconds.
     *
     * \param[in] rhs  The number to compare against.
     *
     * \return true if the timespec_ex object is greater than or equal to the number.
     */
    template<typename T>
    std::enable_if_t<std::is_integral_v<T>
        || std::is_floating_point_v<T>, bool> operator >= (T t) const
    {
        return compare(t) >= 0;
    }


    /** \brief Return the minimum possible timespec.
     *
     * This function returns a timespec with the smallest possible time.
     *
     * \return A timespec_ex with the minimum possible time.
     */
    static timespec_ex min()
    {
        return timespec_ex(std::numeric_limits<std::int64_t>::min(), 0);
    }


    /** \brief Return the maximum possible timespec.
     *
     * This function returns a timespec with the largest possible time.
     *
     * \return A timespec_ex with the maximum possible time.
     */
    static timespec_ex max()
    {
        return timespec_ex(std::numeric_limits<std::int64_t>::max(), 999'999'999L);
    }


    /** \brief Check whether time is in the future.
     *
     * This function checks whether this timespec_ex represents a time in the
     * future.
     *
     * You may pass an \p epsilon parameter to allow a gap between now and
     * the time you consider in the future. By default, \p epsilon is set to
     * zero. If you may pass a negative time in which case the threshold is
     * moved in the past (i.e. a -5 second epsilon means this function returns
     * true if "this >= now - 5 second").
     *
     * \note
     * If this time is exactly equal to now(), then the function returns
     * false (assuming an epsilon of 0).
     *
     * \param[in] epsilon  The allowed discrepancy, 0 by default.
     *
     * \return true if time is in the future.
     */
    bool is_in_the_future(snapdev::timespec_ex const & epsilon = { 0, 0 }) const
    {
        snapdev::timespec_ex const threshold(now() + epsilon);
        return *this > threshold;
    }
};



/** \brief Create a timespec_ex object with "now".
 *
 * This function creates a timespec_ex object with the time set to "now".
 *
 * This is a wrapper of the clock_gettime(2) function.
 *
 * \exception clock_error
 * This function raises the clock_error if the clock could not be read.
 *
 * \param[in] clk_id  The type of clock to read. By default this is
 * CLOCK_REALTIME.
 *
 * \return A timespec_ex object with "now" as the time.
 */
inline timespec_ex now(clockid_t clk_id)
{
    timespec_ex n;
    int const r(clock_gettime(clk_id, &n));
    if(r != 0)
    {
        // LCOV_EXCL_START
        int const e(errno);
        throw clock_error(
              "clock_gettime() failed: "
            + std::to_string(e)
            + ", "
            + strerror(e));
        // LCOV_EXCL_STOP
    }
    return n;
}






/** \brief Callback for ostream extension handling.
 *
 * The ostream handling of timespec defined below supports additional
 * parameters. This uses an allocated buffer which we need to
 * delete and/or copy as required by the standard library. This
 * function handles those parameters.
 *
 * \param[in] e  The event to be handled.
 * \param[in,out] out  The standard stream.
 * \param[in] index  The index being handled.
 */
inline void basic_stream_event_callback(std::ios_base::event e, std::ios_base & out, int index)
{
    switch(e)
    {
    case std::ios_base::erase_event:
        delete static_cast<_ostream_info *>(out.pword(index));
        out.pword(index) = nullptr;
        break;

    case std::ios_base::copyfmt_event:
        {
            _ostream_info * info(static_cast<_ostream_info *>(out.pword(index)));
            if(info != nullptr)
            {
                _ostream_info * new_info(new _ostream_info(*info));
                out.pword(index) = new_info;
            }
        }
        break;

    default:
        // ignore imbue; we have nothing to do with the locale
        break;

    }
}


/** \brief Structure used to set the f_remove_trailing_zeroes in an ostream.
 *
 * This structure is used to set the f_remove_trailing_zeroes in the
 * _ostream_info structure. This makes it very easy to do using the
 * setremovetrailingzeroes() function as an ostream parameter:
 *
 * \code
 *      out << snapdev::setremovetrailingzeroes(false) << timestamp;
 * \endcode
 *
 * The default value is true, so you are more likely to use the function
 * to change it to false as shown above.
 *
 * This structure is considered private. You are not expected to create
 * an instance directly, Instead, you are supposed to use the
 * setremovetrailingzeroes() function.
 *
 * \private
 *
 * \sa setremovetrailingzeroes()
 */
struct _setremovetrailingzeroes
{
    bool                f_remove_trailing_zeroes = true;
};


/** \brief Change the output of timespec_ex to include all trailing zeroes.
 *
 * This function initializes a _setremovetrailingzeroes structure which can
 * be passed to an ostream in order to change how the trailing zeroes are
 * handled in the ostream.
 *
 * The default is to remove the trailing zeroes, so you are likely to
 * use this flag to set it to false like so:
 *
 * \code
 *     std::cout << snapdev::setremovetrailingzeroes(false) << timestamp << std::endl;
 * \endcode
 *
 * \param[in] remove_trailing_zeroes  Whether trailing zeroes are to be removed
 * (true--the default) or be kept (false).
 */
inline _setremovetrailingzeroes setremovetrailingzeroes(bool remove_trailing_zeroes)
{
    return { remove_trailing_zeroes };
}


/** \brief Change the current remove trailing zeroes flag.
 *
 * The value of the timespec_ex object can be printed with an ostream. This
 * parameter defines whether to printing trailing zeroes and the decimal
 * point when all zeroes. By default, the trailing zeroes are all removed.
 *
 * \tparam _CharT  The character type of this stream.
 * \tparam _Traits  The trait of this stream.
 * \param[in] os  The output stream.
 * \param[in] removetrailingzeroes  A setremovetrailingzeroes(bool) call.
 *
 * \sa setremovetrailingzeroes()
 */
template<typename _CharT, typename _Traits>
inline std::basic_ostream<_CharT, _Traits> &
operator << (std::basic_ostream<_CharT, _Traits> & os, _setremovetrailingzeroes removetrailingzeroes)
{
    int const index(get_ostream_index());
    _ostream_info * info(static_cast<_ostream_info *>(os.pword(index)));
    if(info == nullptr)
    {
        info = new _ostream_info;
        os.pword(index) = info;
        os.register_callback(basic_stream_event_callback, index);
    }
    info->f_remove_trailing_zeroes = removetrailingzeroes.f_remove_trailing_zeroes;
    return os;
}


/** \brief Output a timespec to a basic_ostream.
 *
 * This function allows one to print out a timespec. By default the function
 * prints the timespec as a floating point (\<seconds>.\<nanoseconds>).
 *
 * To write a date and time string instead, use the timespec_ex::to_string()
 * function as in:
 *
 * \code
 *     out << t.to_string("%Y/%m/%d %H:%M:%S.%N");
 * \endcode
 *
 * Just keep in mind that to_string() generates a local timestamp. The
 * timespec_ex::from_string() reverses the value back to a timespec_ex.
 *
 * \todo
 * Add a flag to determine whether the nanoseconds trailing zeroes should
 * be removed or not (warning: the timespec_ex::to_timestamp() cannot
 * be used as is because this function receives a timespec not our
 * timespec_ex structure).
 *
 * \tparam _CharT  The character type of this stream.
 * \tparam _Traits  The trait of this stream.
 * \param[in] os  The output stream where the timespec gets written.
 * \param[in] t  The actual timespec that is to be printed.
 *
 * \return A reference to the basic_ostream object.
 */
template<typename _CharT, typename _Traits>
std::basic_ostream<_CharT, _Traits> & operator << (std::basic_ostream<_CharT, _Traits> & os, timespec const & t)
{
    bool remove_trailing_zeroes(true);
    int const index(get_ostream_index());
    _ostream_info * info(static_cast<_ostream_info *>(os.pword(index)));
    if(info != nullptr)
    {
        remove_trailing_zeroes = info->f_remove_trailing_zeroes;
    }

    timespec_ex u(t);
    return os << u.to_timestamp(remove_trailing_zeroes);
}


} // namespace snapdev
// vim: ts=4 sw=4 et
