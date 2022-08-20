// Copyright (c) 2018-2022  Made to Order Software Corp.  All Rights Reserved
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
#include    <snapdev/timespec_ex.h>

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
        snapdev::timespec_ex a(timespec{  5L, 345L });

        snapdev::timespec_ex b;
        timespec raw{ 13L, 701L };
        b = raw;

        CATCH_REQUIRE(a.valid());
        CATCH_REQUIRE(b.valid());
        CATCH_REQUIRE_FALSE(a.negative());
        CATCH_REQUIRE_FALSE(b.negative());

        snapdev::timespec_ex c(a + b);

        a += b;

        CATCH_REQUIRE(a.tv_sec == 5L + 13L);
        CATCH_REQUIRE(a.tv_nsec == 345L + 701L);

        CATCH_REQUIRE(b.tv_sec == 13L);
        CATCH_REQUIRE(b.tv_nsec == 701L);

        CATCH_REQUIRE(c.tv_sec == 5L + 13L);
        CATCH_REQUIRE(c.tv_nsec == 345L + 701L);
        CATCH_REQUIRE(a == c);
        CATCH_REQUIRE(c == a);

        timespec d{ 3L, 55L };

        a += d;

        CATCH_REQUIRE(a.tv_sec == 5L + 13L + 3L);
        CATCH_REQUIRE(a.tv_nsec == 345L + 701L + 55L);

        a += 301L;

        CATCH_REQUIRE(a.tv_sec == 5L + 13L + 3L);
        CATCH_REQUIRE(a.tv_nsec == 345L + 701L + 55L + 301L);

        a -= 1'000'000'259L;

        CATCH_REQUIRE(a.tv_sec == 5L + 13L + 3L - 1L);
        CATCH_REQUIRE(a.tv_nsec == 345L + 701L + 55L + 301L - 259L);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("timespec-operations: simple subtract")
    {
        snapdev::timespec_ex a(timespec{ 25L, 1'345L });
        snapdev::timespec_ex b(timespec{ 13L,   701L });

        CATCH_REQUIRE(a.valid());
        CATCH_REQUIRE(b.valid());

        snapdev::timespec_ex c(a - b);
        snapdev::timespec_ex d(-b);
        snapdev::timespec_ex e(a + d);
        snapdev::timespec_ex f(a - 1'000L);   // -1us

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
        snapdev::timespec_ex now(timespec{ 1629652541L, 345L });
        snapdev::timespec_ex backward(timespec{ -86400L, 0L }); // -1 day

        CATCH_REQUIRE(!!now);
        CATCH_REQUIRE_FALSE(!backward);
        CATCH_REQUIRE(backward.negative());

        now += backward;

        CATCH_REQUIRE(now.tv_sec == 1629652541L - 86400L);
        CATCH_REQUIRE(now.tv_nsec == 345L);

        CATCH_REQUIRE(backward.tv_sec == -86400L);
        CATCH_REQUIRE(backward.tv_nsec == 0L);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("timespec-operations: add with nano overflow")
    {
        snapdev::timespec_ex now(timespec{ 1629652541L, 913'788'345L });
        snapdev::timespec_ex backward(timespec{ 86400L, 500'000'000L }); // +1 day and 0.5 seconds

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
        snapdev::timespec_ex a(13L,   701L);
        snapdev::timespec_ex b(25L, 1'345L);

        CATCH_REQUIRE(a.valid());
        CATCH_REQUIRE(b.valid());

        a -= b;

        CATCH_REQUIRE(a.tv_sec == 13L - 25L - 1L);
        CATCH_REQUIRE(a.tv_nsec == 701L - 1'345L + 1'000'000'000L);

        CATCH_REQUIRE(b.tv_sec == 25L);
        CATCH_REQUIRE(b.tv_nsec == 1'345L);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("timespec-operations: -1, 0, +1")
    {
        snapdev::timespec_ex a = {};

        CATCH_REQUIRE(!a);
        CATCH_REQUIRE_FALSE(a.negative());

        --a;

        CATCH_REQUIRE_FALSE(!a);
        CATCH_REQUIRE(a.negative());
        CATCH_REQUIRE(a.tv_sec == -1L);
        CATCH_REQUIRE(a.tv_nsec == 999'999'999L);

        ++a;

        CATCH_REQUIRE(!a);
        CATCH_REQUIRE(a.tv_sec == 0L);
        CATCH_REQUIRE(a.tv_nsec == 0L);

        ++a;

        CATCH_REQUIRE(!!a);
        CATCH_REQUIRE_FALSE(a.negative());
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
        snapdev::timespec_ex now(1629652541L, 913'788'345L);
        std::int64_t nsec(500'000'000L);

        CATCH_REQUIRE(!!now);

        snapdev::timespec_ex sum(now + nsec);

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

        snapdev::timespec_ex total(sum + nsec);

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

        snapdev::timespec_ex pre(++total);

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

        snapdev::timespec_ex post(now++);

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
        snapdev::timespec_ex now(1629652549L, 913'788'345L);

        std::int64_t nsec(now.to_nsec());

        CATCH_REQUIRE(nsec == 1629652549L * 1'000'000'000L + 913'788'345L);

        snapdev::timespec_ex save;
        CATCH_REQUIRE(!save);
        CATCH_REQUIRE(save.valid());
        save = nsec;

        CATCH_REQUIRE(nsec == 1629652549L * 1'000'000'000L + 913'788'345L);
        CATCH_REQUIRE(save.tv_sec == 1629652549L);
        CATCH_REQUIRE(save.tv_nsec == 913'788'345L);

        double seconds(33.0);
        save = seconds;
        CATCH_REQUIRE(save.tv_sec == 33L);
        CATCH_REQUIRE(save.tv_nsec == 0L);

        double precise_time(save.to_sec());
        bool precise_result(precise_time >= 33.0 && precise_time <= 33.0);
        CATCH_REQUIRE(precise_result);

        seconds = 81.325611932;
        save = seconds;
        CATCH_REQUIRE(save.tv_sec == 81L);
        CATCH_REQUIRE(save.tv_nsec == 325'611'932L);

        precise_time = save.to_sec();
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

        snapdev::timespec_ex plus(save + 3.000101);
        precise_time = plus.to_sec();
        precise_result = precise_time >= 84.325712930 && precise_time <= 84.325712931; // we lose some precision here
//std::cerr << "precise = " << std::setprecision(20) << std::setw(9) << precise_time
//<< " expected " << 81.325611932 + 3.000101 << "\n";
        CATCH_REQUIRE(precise_result);

        save += 3.000101;
        precise_time = save.to_sec();
        precise_result = precise_time >= 84.325712930 && precise_time <= 84.325712931; // we lose some precision here
//std::cerr << "precise = " << std::setprecision(20) << std::setw(9) << precise_time
//<< " expected " << 81.325611932 + 3.000101 << "\n";
        CATCH_REQUIRE(precise_result);
        CATCH_REQUIRE(save == plus);

        snapdev::timespec_ex minus(save - 1.20050001);
        precise_time = minus.to_sec();
        precise_result = precise_time >= 83.125212920 && precise_time <= 83.125212921;
//std::cerr << "precise = " << std::setprecision(20) << std::setw(9) << precise_time
//<< " expected " << 81.325611932 + 3.000101 - 1.20050001 << "\n";
        CATCH_REQUIRE(precise_result);

        save -= 1.20050001;
        precise_time = save.to_sec();
        precise_result = precise_time >= 83.125212920 && precise_time <= 83.125212921;
//std::cerr << "precise = " << std::setprecision(20) << std::setw(9) << precise_time
//<< " expected " << 81.325611932 + 3.000101 - 1.20050001 << "\n";
        CATCH_REQUIRE(precise_result);
        CATCH_REQUIRE(save == minus);

        double neg(-701.445123421);
        save = neg;
        CATCH_REQUIRE(save.tv_sec == -702L);
        precise_result = save.tv_nsec >= -445'123'420L + 1'000'000'000L
                      || save.tv_nsec <= -445'123'422L + 1'000'000'000L;
        CATCH_REQUIRE(precise_result);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("timespec-operations: negative + negative")
    {
        snapdev::timespec_ex pa(4511L, 913'788'345L);
        snapdev::timespec_ex pb( 311L, 301'225'198L);

        snapdev::timespec_ex na(-pa);
        snapdev::timespec_ex nb(-pb);

        snapdev::timespec_ex sum_ab(na + nb);
        snapdev::timespec_ex sum_ba(nb + na);
        snapdev::timespec_ex diff_ab(na - nb);
        snapdev::timespec_ex diff_ba(nb - na);

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

    CATCH_START_SECTION("timespec-operations: system time")
    {
        snapdev::timespec_ex now;
        timespec verify = {};

        now = snapdev::timespec_ex::gettime();
        clock_gettime(CLOCK_REALTIME, &verify);

        snapdev::timespec_ex diff(now - verify);
        snapdev::timespec_ex max_diff(100L);

        CATCH_REQUIRE(diff < max_diff);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("timespec-operations: ostream")
    {
        snapdev::timespec_ex a(timespec{ 4511L, 913'788'345L });

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
