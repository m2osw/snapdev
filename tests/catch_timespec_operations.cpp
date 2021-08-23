// Copyright (c) 2018-2021  Made to Order Software Corp.  All Rights Reserved
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
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

/** \file
 * \brief Verify that the timespec operators function.
 *
 * This file implements test for the timespec operator functions.
 */

// self
//
#include    <snapdev/timespec_operations.h>

#include    "catch_main.h"


// C++ lib
//
#include    <iomanip>


// last include
//
#include    <snapdev/poison.h>






CATCH_TEST_CASE("timespec-operations", "[math]")
{
    CATCH_START_SECTION("timespec-operations: simple add")
    {
        timespec a{  5L, 345L };
        timespec b{ 13L, 701L };

        CATCH_REQUIRE(valid_timespec(a));
        CATCH_REQUIRE(valid_timespec(b));
        CATCH_REQUIRE_FALSE(negative_timespec(a));
        CATCH_REQUIRE_FALSE(negative_timespec(b));

        timespec c(a + b);

        a += b;

        CATCH_REQUIRE(a.tv_sec == 5L + 13L);
        CATCH_REQUIRE(a.tv_nsec == 345L + 701L);

        CATCH_REQUIRE(b.tv_sec == 13L);
        CATCH_REQUIRE(b.tv_nsec == 701L);

        CATCH_REQUIRE(c.tv_sec == 5L + 13L);
        CATCH_REQUIRE(c.tv_nsec == 345L + 701L);
        CATCH_REQUIRE(a == c);
        CATCH_REQUIRE(c == a);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("timespec-operations: simple subtract")
    {
        timespec a{ 25L, 1'345L };
        timespec b{ 13L,   701L };

        CATCH_REQUIRE(valid_timespec(a));
        CATCH_REQUIRE(valid_timespec(b));

        timespec c(a - b);
        timespec d(-b);
        timespec e(a + d);
        timespec f(a - 1'000L);   // -1us

        a -= b;

        CATCH_REQUIRE(a.tv_sec == 25L - 13L);
        CATCH_REQUIRE(a.tv_nsec == 1'345L - 701L);

        CATCH_REQUIRE(b.tv_sec == 13L);
        CATCH_REQUIRE(b.tv_nsec == 701L);

        CATCH_REQUIRE(c.tv_sec == 25L - 13L);
        CATCH_REQUIRE(c.tv_nsec == 1'345L - 701L);
        CATCH_REQUIRE(a == c);
        CATCH_REQUIRE(c == a);
        CATCH_REQUIRE(a == e);
        CATCH_REQUIRE(e == c);

        CATCH_REQUIRE(f.tv_sec == 25L);
        CATCH_REQUIRE(f.tv_nsec == 1'345L - 1'000L);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("timespec-operations: add \"minus one day\"")
    {
        timespec now{ 1629652541L, 345L };
        timespec backward{ -86400L, 0L }; // -1 day

        CATCH_REQUIRE(!!now);
        CATCH_REQUIRE_FALSE(!backward);
        CATCH_REQUIRE(negative_timespec(backward));

        now += backward;

        CATCH_REQUIRE(now.tv_sec == 1629652541L - 86400L);
        CATCH_REQUIRE(now.tv_nsec == 345L);

        CATCH_REQUIRE(backward.tv_sec == -86400L);
        CATCH_REQUIRE(backward.tv_nsec == 0L);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("timespec-operations: add with nano overflow")
    {
        timespec now{ 1629652541L, 913'788'345L };
        timespec backward{ 86400L, 500'000'000L }; // +1 day and 0.5 seconds

        CATCH_REQUIRE(!!now);
        CATCH_REQUIRE(!!backward);

        now += backward;

        CATCH_REQUIRE(now.tv_sec == 1629652541L + 86400L + 1L);  // +1 for the overflow
        CATCH_REQUIRE(now.tv_nsec == 913'788'345L + 500'000'000L - 1'000'000'000L);

        CATCH_REQUIRE(backward.tv_sec == 86400L);
        CATCH_REQUIRE(backward.tv_nsec == 500'000'000L);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("timespec-operations: subtract with nano underflow")
    {
        timespec a{ 13L,   701L };
        timespec b{ 25L, 1'345L };

        CATCH_REQUIRE(valid_timespec(a));
        CATCH_REQUIRE(valid_timespec(b));

        a -= b;

        CATCH_REQUIRE(a.tv_sec == 13L - 25L - 1L);
        CATCH_REQUIRE(a.tv_nsec == 701L - 1'345L + 1'000'000'000L);

        CATCH_REQUIRE(b.tv_sec == 25L);
        CATCH_REQUIRE(b.tv_nsec == 1'345L);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("timespec-operations: -1, 0, +1")
    {
        timespec a = {};

        CATCH_REQUIRE(!a);
        CATCH_REQUIRE_FALSE(negative_timespec(a));

        --a;

        CATCH_REQUIRE_FALSE(!a);
        CATCH_REQUIRE(negative_timespec(a));
        CATCH_REQUIRE(a.tv_sec == -1L);
        CATCH_REQUIRE(a.tv_nsec == 999'999'999L);

        ++a;

        CATCH_REQUIRE(!a);
        CATCH_REQUIRE(a.tv_sec == 0L);
        CATCH_REQUIRE(a.tv_nsec == 0L);

        ++a;

        CATCH_REQUIRE(!!a);
        CATCH_REQUIRE_FALSE(negative_timespec(a));
        CATCH_REQUIRE(a.tv_sec == 0L);
        CATCH_REQUIRE(a.tv_nsec == 1L);

        --a;

        CATCH_REQUIRE(!a);
        CATCH_REQUIRE(a.tv_sec == 0L);
        CATCH_REQUIRE(a.tv_nsec == 0L);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("timespec-operations: add nanos")
    {
        timespec now{ 1629652541L, 913'788'345L };
        std::int64_t nsec(500'000'000L);

        CATCH_REQUIRE(!!now);

        timespec sum(now + nsec);

        now += nsec;

        CATCH_REQUIRE(now.tv_sec == 1629652541L + 1L);  // +1 for the overflow
        CATCH_REQUIRE(now.tv_nsec == 913'788'345L + 500'000'000L - 1'000'000'000L);

        CATCH_REQUIRE(now == sum);
        CATCH_REQUIRE_FALSE(now != sum);
        CATCH_REQUIRE_FALSE(now < sum);
        CATCH_REQUIRE(now <= sum);
        CATCH_REQUIRE_FALSE(now > sum);
        CATCH_REQUIRE(now >= sum);

        CATCH_REQUIRE(nsec == 500'000'000L);

        nsec += 86400L * 1'000'000'000L;

        timespec total(sum + nsec);

        now += nsec;

        CATCH_REQUIRE(now.tv_sec == 1629652541L + 1L + 86400L);  // +1 for the overflow above
        CATCH_REQUIRE(now.tv_nsec == 913'788'345L + 500'000'000L - 1'000'000'000L + 500'000'000L);

        CATCH_REQUIRE(nsec == 500'000'000L + 86400L * 1'000'000'000L);

        CATCH_REQUIRE_FALSE(now == sum);
        CATCH_REQUIRE(now != sum);
        CATCH_REQUIRE_FALSE(now < sum);
        CATCH_REQUIRE_FALSE(now <= sum);
        CATCH_REQUIRE(now > sum);
        CATCH_REQUIRE(now >= sum);

        CATCH_REQUIRE(now == total);
        CATCH_REQUIRE_FALSE(now != total);
        CATCH_REQUIRE_FALSE(now < total);
        CATCH_REQUIRE(now <= total);
        CATCH_REQUIRE_FALSE(now > total);
        CATCH_REQUIRE(now >= total);

        timespec pre(++total);

        CATCH_REQUIRE(pre == total);
        CATCH_REQUIRE_FALSE(pre != total);
        CATCH_REQUIRE_FALSE(pre < total);
        CATCH_REQUIRE(pre <= total);
        CATCH_REQUIRE_FALSE(pre > total);
        CATCH_REQUIRE(pre >= total);

        CATCH_REQUIRE_FALSE(now == total);
        CATCH_REQUIRE(now != total);
        CATCH_REQUIRE(now < total);
        CATCH_REQUIRE(now <= total);
        CATCH_REQUIRE_FALSE(now > total);
        CATCH_REQUIRE_FALSE(now >= total);

        timespec post(now++);

        CATCH_REQUIRE_FALSE(post == total);
        CATCH_REQUIRE(post != total);
        CATCH_REQUIRE(post < total);
        CATCH_REQUIRE(post <= total);
        CATCH_REQUIRE_FALSE(post > total);
        CATCH_REQUIRE_FALSE(post >= total);

        CATCH_REQUIRE(now == total);
        CATCH_REQUIRE_FALSE(now != total);
        CATCH_REQUIRE_FALSE(now < total);
        CATCH_REQUIRE(now <= total);
        CATCH_REQUIRE_FALSE(now > total);
        CATCH_REQUIRE(now >= total);

        pre = --total;

        CATCH_REQUIRE(pre == total);
        CATCH_REQUIRE_FALSE(pre != total);
        CATCH_REQUIRE_FALSE(pre < total);
        CATCH_REQUIRE(pre <= total);
        CATCH_REQUIRE_FALSE(pre > total);
        CATCH_REQUIRE(pre >= total);

        CATCH_REQUIRE_FALSE(now == total);
        CATCH_REQUIRE(now != total);
        CATCH_REQUIRE_FALSE(now < total);
        CATCH_REQUIRE_FALSE(now <= total);
        CATCH_REQUIRE(now > total);
        CATCH_REQUIRE(now >= total);

        post = now--;

        CATCH_REQUIRE_FALSE(post == total);
        CATCH_REQUIRE(post != total);
        CATCH_REQUIRE_FALSE(post < total);
        CATCH_REQUIRE_FALSE(post <= total);
        CATCH_REQUIRE(post > total);
        CATCH_REQUIRE(post >= total);

        CATCH_REQUIRE(now == total);
        CATCH_REQUIRE_FALSE(now != total);
        CATCH_REQUIRE_FALSE(now < total);
        CATCH_REQUIRE(now <= total);
        CATCH_REQUIRE_FALSE(now > total);
        CATCH_REQUIRE(now >= total);

    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("timespec-operations: load/save")
    {
        timespec now{ 1629652549L, 913'788'345L };

        std::int64_t nsec(0);
        now >>= nsec;

        CATCH_REQUIRE(nsec == 1629652549L * 1'000'000'000L + 913'788'345L);

        timespec save = {};
        CATCH_REQUIRE(!save);
        CATCH_REQUIRE(valid_timespec(save));
        save <<= nsec;

        CATCH_REQUIRE(nsec == 1629652549L * 1'000'000'000L + 913'788'345L);
        CATCH_REQUIRE(save.tv_sec == 1629652549L);
        CATCH_REQUIRE(save.tv_nsec == 913'788'345L);

        double seconds(33.0);
        save <<= seconds;
        CATCH_REQUIRE(save.tv_sec == 33L);
        CATCH_REQUIRE(save.tv_nsec == 0L);

        double precise_time(0.0);
        save >>= precise_time;
        bool precise_result(precise_time >= 33.0 && precise_time <= 33.0);
        CATCH_REQUIRE(precise_result);

        seconds = 81.325611932;
        save <<= seconds;
        CATCH_REQUIRE(save.tv_sec == 81L);
        CATCH_REQUIRE(save.tv_nsec == 325'611'932L);

        save >>= precise_time;
        precise_result = precise_time >= 81.325611932 && precise_time <= 81.325611932;
        CATCH_REQUIRE(precise_result);
        CATCH_REQUIRE(save == 81.325611932);
        CATCH_REQUIRE_FALSE(save == 83.5);
        CATCH_REQUIRE(save != 83.5);
        CATCH_REQUIRE_FALSE(save != 81.325611932);
        CATCH_REQUIRE(save < 83.5);
        CATCH_REQUIRE(save <= 81.33);
        CATCH_REQUIRE(save <= 81.325611932);
        CATCH_REQUIRE(save > 71.5);
        CATCH_REQUIRE(save >= 81.324);
        CATCH_REQUIRE(save >= 81.325611932);

        timespec plus(save + 3.000101);
        plus >>= precise_time;
        precise_result = precise_time >= 84.325712930 && precise_time <= 84.325712931; // we lose some precision here
//std::cerr << "precise = " << std::setprecision(20) << std::setw(9) << precise_time
//<< " expected " << 81.325611932 + 3.000101 << "\n";
        CATCH_REQUIRE(precise_result);

        save += 3.000101;
        save >>= precise_time;
        precise_result = precise_time >= 84.325712930 && precise_time <= 84.325712931; // we lose some precision here
//std::cerr << "precise = " << std::setprecision(20) << std::setw(9) << precise_time
//<< " expected " << 81.325611932 + 3.000101 << "\n";
        CATCH_REQUIRE(precise_result);
        CATCH_REQUIRE(save == plus);

        timespec minus(save - 1.20050001);
        minus >>= precise_time;
        precise_result = precise_time >= 83.125212920 && precise_time <= 83.125212921;
//std::cerr << "precise = " << std::setprecision(20) << std::setw(9) << precise_time
//<< " expected " << 81.325611932 + 3.000101 - 1.20050001 << "\n";
        CATCH_REQUIRE(precise_result);

        save -= 1.20050001;
        save >>= precise_time;
        precise_result = precise_time >= 83.125212920 && precise_time <= 83.125212921;
//std::cerr << "precise = " << std::setprecision(20) << std::setw(9) << precise_time
//<< " expected " << 81.325611932 + 3.000101 - 1.20050001 << "\n";
        CATCH_REQUIRE(precise_result);
        CATCH_REQUIRE(save == minus);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("timespec-operations: negative + negative")
    {
        timespec pa{ 4511L, 913'788'345L };
        timespec pb{  311L, 301'225'198L };

        timespec na(-pa);
        timespec nb(-pb);

        timespec sum_ab(na + nb);
        timespec sum_ba(nb + na);
        timespec diff_ab(na - nb);
        timespec diff_ba(nb - na);

        CATCH_REQUIRE(sum_ab.tv_sec != -4511L + -311L);
        CATCH_REQUIRE(sum_ab.tv_sec == -4511L + -1L + -311L + -1L);
        CATCH_REQUIRE(sum_ab.tv_nsec != -913'788'345L + -301'225'198L);
        CATCH_REQUIRE(sum_ab.tv_nsec == (1'000'000'000 - 913'788'345L) + (1'000'000'000L - 301'225'198L));

        CATCH_REQUIRE(sum_ab       == (-4511L + -1L + -311L + -1L) * 1'000'000'000 + (1'000'000'000 - 913'788'345L) + (1'000'000'000L - 301'225'198L));
        CATCH_REQUIRE_FALSE(sum_ab != (-4511L + -1L + -311L + -1L) * 1'000'000'000 + (1'000'000'000 - 913'788'345L) + (1'000'000'000L - 301'225'198L));
        CATCH_REQUIRE_FALSE(sum_ab == (-4511L +  0L + -311L + -1L) * 1'000'000'000 + (1'000'000'000 - 913'788'345L) + (1'000'000'000L - 301'225'198L));
        CATCH_REQUIRE(sum_ab       != (-4511L + -1L + -311L + -1L) * 1'000'000'000 + (1'000'000'000 - 913'788'344L) + (1'000'000'000L - 301'225'198L));

        CATCH_REQUIRE(sum_ba.tv_sec != -311L + -4511L);
        CATCH_REQUIRE(sum_ba.tv_sec == -311L + -1L + -4511L + -1L);
        CATCH_REQUIRE(sum_ba.tv_nsec != -301'225'198L + -913'788'345L);
        CATCH_REQUIRE(sum_ba.tv_nsec == (1'000'000'000L - 301'225'198L) + (1'000'000'000 - 913'788'345L));

        CATCH_REQUIRE(diff_ab.tv_sec == -4511L + -1L + 311L);
        CATCH_REQUIRE(diff_ab.tv_nsec == (1'000'000'000 - 913'788'345L) + 301'225'198L);

        CATCH_REQUIRE(diff_ab       >= (-4511L + -1L + 311L) * 1'000'000'000 + (1'000'000'000 - 913'788'345L) + 301'225'198L);
        CATCH_REQUIRE_FALSE(diff_ab >  (-4511L + -1L + 311L) * 1'000'000'000 + (1'000'000'000 - 913'788'345L) + 301'225'198L);
        CATCH_REQUIRE(diff_ab       <= (-4511L + -1L + 311L) * 1'000'000'000 + (1'000'000'000 - 913'788'345L) + 301'225'198L);
        CATCH_REQUIRE_FALSE(diff_ab <  (-4511L + -1L + 311L) * 1'000'000'000 + (1'000'000'000 - 913'788'345L) + 301'225'198L);

        CATCH_REQUIRE(diff_ba.tv_sec == -311L + -1L + 4511L + 1L);
        CATCH_REQUIRE(diff_ba.tv_nsec == (((1'000'000'000 - 301'225'198L) + 913'788'345L) - 1'000'000'000));

        CATCH_REQUIRE(diff_ba       >= (-311L + -1L + 4511L + 1L) * 1'000'000'000 + (((1'000'000'000 - 301'225'198L) + 913'788'345L) - 1'000'000'000));
        CATCH_REQUIRE_FALSE(diff_ba >  (-311L + -1L + 4511L + 1L) * 1'000'000'000 + (((1'000'000'000 - 301'225'198L) + 913'788'345L) - 1'000'000'000));
        CATCH_REQUIRE(diff_ba       <= (-311L + -1L + 4511L + 1L) * 1'000'000'000 + (((1'000'000'000 - 301'225'198L) + 913'788'345L) - 1'000'000'000));
        CATCH_REQUIRE_FALSE(diff_ba <  (-311L + -1L + 4511L + 1L) * 1'000'000'000 + (((1'000'000'000 - 301'225'198L) + 913'788'345L) - 1'000'000'000));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("timespec-operations: ostream")
    {
        timespec a{ 4511L, 913'788'345L };

        std::stringstream ss;
        ss << a;

        CATCH_REQUIRE(ss.str() == "4511.913788345");

        ss.str(std::string());
        a.tv_sec = 83207L;
        a.tv_nsec = 0;
        ss << a;

        CATCH_REQUIRE(ss.str() == "83207.000000000");

        ss.str(std::string());
        a.tv_sec = 0L;
        a.tv_nsec = 101;
        ss << a;

        CATCH_REQUIRE(ss.str() == "0.000000101");
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
