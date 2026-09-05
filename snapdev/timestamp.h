// Copyright (c) 2011-2026  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Compute a timestamp at compile time.
 *
 * At times, you want to create a time_t value. You could use the command line:
 *
 * \code
 * date +%s -d <date>
 * \endcode
 *
 * and copy/paste the 64 bit value. Only that means nothing to most people.
 *
 * With the unix_timestamp() function, you can instead define a time with
 * a year, month, day, hour, minute, and second and that gets converted to
 * a number of seconds at compile time.
 */

// C++
//
#include    <ctime>
#include    <stdexcept>
#include    <string>



namespace snapdev
{



/** \brief Compute the number of days in the month of February.
 *
 * The month of February is used to adjust the date by 1 day over leap
 * years. Years are leap years when multiple of 4, but not if multiple
 * of 100, except if it is also a multiple of 400.
 *
 * The computation of a leap year is documented on Wikipedia:
 * http://www.wikipedia.org/wiki/Leap_year
 *
 * \param[in] year  The year of the date to convert.
 *
 * \return 28 or 29 depending on whether the year is a leap year
 */
constexpr int unix_timestamp_february_days(int year)
{
    // this is not compatible with gmtime() or timegm() so ignore it
    //
    //if(year <= 1752 || 1582)
    //{
    //    // before the updated Gregorian calendar, the Julian calendar used
    //    // a really simple algorithm -- however, the US/UK did it in 1752
    //    // only while many of the European countries did it in 1582
    //    // so this is a complicated matter -- our tests assume that the
    //    // Gregorian calendar was used since 1582
    //    //
    //    return year % 4 == 0 ? 29 : 28;
    //}

    if(year % 400 == 0)
    {
        return 29LL;
    }

    if(year % 100 == 0)
    {
        return 28LL;
    }

    if(year % 4 == 0)
    {
        return 29LL;
    }

    return 28LL;
}


/** \brief Determine the number of days in a month.
 *
 * This function returns the number of days in the specified month of
 * the specified year. The number of days changes for February. In all
 * other cases, the number is invariant.
 *
 * \param[in] year  The 4 digits year concerned.
 * \param[in] month  The month (1 to 12).
 *
 * \return The number of days in that month for the specified year.
 */
constexpr int unix_timestamp_month_days(int year, int month)
{
    if(month < 1 || month > 12)
    {
        throw std::logic_error(
              "snapdev::unix_timestamp_month_days() called with "
            + std::to_string(month)
            + " which is not a valid month number.");
    }

    if(month == 2)
    {
        return unix_timestamp_february_days(year);
    }
    else
    {
        // at the moment I don't think this is useful
        //
        // anything between and before those dates should not really
        // happen in our world (we don't work on historic events... and
        // if you do you probably want your own date handling library)
        //
        //if((month == 9 && year == 1752)
        //|| (month == 10 && year == 1582))
        //{
        //    // we cannot handle this nice one here:
        //    //
        //    // - the Gregorian calendar went in effect on Oct 1582 when
        //    //   most of Europe skipped 10 days (Oct 5 to Oct 14 1582
        //    //   do not officially exist)
        //    // - The US and UK finally accepted the new Gregorian calendar
        //    //   in 1752 and dropped days 3 to 13 in September to adjust
        //    //   to the Gregorian calendar
        //    //
        //    throw std::logic_error(
        //          "snapdev::unix_timestamp_mont_days() called with year 1752 and month of September. That month does not have a practical number of days.");
        //}

        static constexpr signed char const month_days[12] = {
            31, 0, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
        };
        return month_days[month - 1];
    }
}


/** \brief Compute the number of days in a year.
 *
 * This function returns the number of days from the beginning of the
 * year the (year, month, day) value represents.
 *
 * \warning
 * The function does not verify the validity of the input parameters.
 *
 * \param[in] year  The 4 digits year concerned.
 * \param[in] month  The month (1 to 12).
 * \param[in] day  The day of the month (1 to 31)
 *
 * \return The number of days within that year (starting at 1).
 */
constexpr time_t unix_timestamp_year_days(int year, int month, int day)
{
    return
        /* January */    static_cast<time_t>(day)
        /* February */ + ((month) >=  2 ? 31LL : 0LL)
        /* March */    + ((month) >=  3 ? unix_timestamp_february_days(year) : 0LL)
        /* April */    + ((month) >=  4 ? 31LL : 0LL)
        /* May */      + ((month) >=  5 ? 30LL : 0LL)
        /* June */     + ((month) >=  6 ? 31LL : 0LL)
        /* July */     + ((month) >=  7 ? 30LL : 0LL)
        /* August */   + ((month) >=  8 ? 31LL : 0LL)
        /* September */+ ((month) >=  9 ? 31LL : 0LL)
        /* October */  + ((month) >= 10 ? 30LL : 0LL)
        /* November */ + ((month) >= 11 ? 31LL : 0LL)
        /* December */ + ((month) >= 12 ? 30LL : 0LL)
    ;
}



/** \brief Compute a Unix date from a hard coded date.
 *
 * This function is used to compute a Unix date from a date defined as 6
 * numbers:
 *
 * * year
 * * month
 * * day
 * * hour
 * * minute
 * * second
 *
 * Each number is expected to be an integer although it could very well be
 * an expression. The computation takes the year and month in account to
 * compute the year day.
 *
 * The year is expected to be written as a 4 digit number (1998, 2012, etc.)
 *
 * Each number is expected to represent a valid date. If a number is out of
 * range, then the date is still computed. It still represents a valid Unix
 * timestamp, just not exactly what you wrote down.
 *
 * The math used in this function comes from a FreeBSD implementation of
 * (mktime)[http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap04.html#tag_04_15].
 *
 * \code
 * tm_sec + tm_min*60 + tm_hour*3600 + tm_yday*86400 +
 *   (tm_year-70)*31536000 + ((tm_year-69)/4)*86400 -
 *   ((tm_year-1)/100)*86400 + ((tm_year+299)/400)*86400
 * \endcode
 *
 * Note that we expect the year to be 1970 and not 0, 2000 and not 30, etc.
 * For this reason our macro subtract values from the year that are different
 * from those shown in the FreeBSD sample code.
 *
 * Also the month must be a number from 1 to 12 and not 0 to 11 as used
 * in various Unix structures.
 *
 * \param[in] year  The year representing this Unix timestamp.
 * \param[in] month  The month representing this Unix timestamp.
 * \param[in] day  The day representing this Unix timestamp.
 * \param[in] hour  The hour representing this Unix timestamp.
 * \param[in] minute  The minute representing this Unix timestamp.
 * \param[in] second  The second representing this Unix timestamp.
 *
 * \return A Unix timestamp representing the specified broken up time in UTC.
 */
constexpr time_t unix_timestamp(
          int year
        , int month
        , int day
        , int hour
        , int minute
        , int second)
{
    return
/* time */                     static_cast<time_t>(second)
                             + static_cast<time_t>(minute) * 60LL
                             + static_cast<time_t>(hour) * 3600LL
/* year day (month + day) */ + (unix_timestamp_year_days(year, month, day) - 1) * 86400LL
/* year */                   + (static_cast<time_t>(year) - 1970LL) * 31536000LL
                             + ((static_cast<time_t>(year) - 1969LL) / 4LL) * 86400LL
                             - ((static_cast<time_t>(year) - 1901LL) / 100LL) * 86400LL
                             + ((static_cast<time_t>(year) - 1601LL) / 400LL) * 86400LL
    ;
}





} //namespace snapdev
// vim: ts=4 sw=4 et
