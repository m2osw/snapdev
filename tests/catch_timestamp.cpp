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

/** \file
 * \brief Verify that the timestamp functions work as expected.
 *
 * This file implements tests for the Unix timestamp functions which
 * convert separate integers into a time_t value at compile.
 */

// self
//
#include    <snapdev/timestamp.h>

#include    <snapdev/math.h>

#include    "catch_main.h"


// snapdev
//
#include    <snapdev/not_reached.h>


// C++
//
#include    <algorithm>
#include    <iomanip>
#include    <ranges>


// last include
//
#include    <snapdev/poison.h>



namespace
{



int const g_leap_years[] = {
    1584, 1588, 1592, 1596, 1600, 1604, 1608, 1612,
    1616, 1620, 1624, 1628, 1632, 1636, 1640, 1644,
    1648, 1652, 1656, 1660, 1664, 1668, 1672, 1676,
    1680, 1684, 1688, 1692, 1696, 1704, 1708, 1712,
    1716, 1720, 1724, 1728, 1732, 1736, 1740, 1744,
    1748, 1752, 1756, 1760, 1764, 1768, 1772, 1776,
    1780, 1784, 1788, 1792, 1796, 1804, 1808, 1812,
    1816, 1820, 1824, 1828, 1832, 1836, 1840, 1844,
    1848, 1852, 1856, 1860, 1864, 1868, 1872, 1876,
    1880, 1884, 1888, 1892, 1896, 1904, 1908, 1912,
    1916, 1920, 1924, 1928, 1932, 1936, 1940, 1944,
    1948, 1952, 1956, 1960, 1964, 1968, 1972, 1976,
    1980, 1984, 1988, 1992, 1996, 2000, 2004, 2008,
    2012, 2016, 2020, 2024, 2028, 2032, 2036, 2040,
    2044, 2048, 2052, 2056, 2060, 2064, 2068, 2072,
    2076, 2080, 2084, 2088, 2092, 2096, 2104, 2108,
    2112, 2116, 2120, 2124, 2128, 2132, 2136, 2140,
    2144, 2148, 2152, 2156, 2160, 2164, 2168, 2172,
    2176, 2180, 2184, 2188, 2192, 2196, 2204, 2208,
    2212, 2216, 2220, 2224, 2228, 2232, 2236, 2240,
    2244, 2248, 2252, 2256, 2260, 2264, 2268, 2272,
    2276, 2280, 2284, 2288, 2292, 2296, 2304, 2308,
    2312, 2316, 2320, 2324, 2328, 2332, 2336, 2340,
    2344, 2348, 2352, 2356, 2360, 2364, 2368, 2372,
    2376, 2380, 2384, 2388, 2392, 2396, 2400,
};



}
// no name namespace



CATCH_TEST_CASE("timestamp", "[time][math]")
{
    CATCH_START_SECTION("timestamp: verify that the math is working as expected for February")
    {
        // the Gregorian leap years were introduced around 1600
        // where we started to see exceptions for the years dividable
        // by 100 (not a leap) or 400 (except those)
        //
        for(int year(1581); year <= 2403; ++year)
        {
            int const days(snapdev::unix_timestamp_february_days(year));
            if(days != (std::ranges::contains(g_leap_years, year) ? 29 : 28))
            {
                std::cerr << "error: for year " << year << ", expected "
                    << (std::ranges::contains(g_leap_years, days) ? 29 : 28)
                    << ", but got " << days << " instead." << std::endl;
            }
            CATCH_REQUIRE(days == (std::ranges::contains(g_leap_years, year) ? 29 : 28));
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("timestamp: check number of days in a month")
    {
        // random year
        //
        int const year(snapdev::random(1581, 2403));

        for(int month(1); month <= 12; ++month)
        {
            int const days(snapdev::unix_timestamp_month_days(year, month));

            switch(month)
            {
            case 1:
            case 3:
            case 5:
            case 7:
            case 8:
            case 10:
            case 12:
                CATCH_REQUIRE(days == 31);
                break;

            case 4:
            case 6:
            case 9:
            case 11:
                CATCH_REQUIRE(days == 30);
                break;

            case 2:
                {
                    int const expected_days(std::ranges::contains(g_leap_years, year) ? 29 : 28);

                    CATCH_REQUIRE(expected_days == days);
                }
                break;

            default:
                CATCH_FAIL("invalid month?!");
                break;

            }
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("timestamp: check some random time_t")
    {
        for(int count(0); count < 1'000; ++count)
        {
            time_t const org(snapdev::random(-1'000'000LL, 2'000'000'000LL));

            struct tm t;
            gmtime_r(&org, &t);

            time_t const timestamp(snapdev::unix_timestamp(t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec));

            CATCH_REQUIRE(org == timestamp);
        }
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
