// Copyright (c) 2018-2024  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Verify that the timespec_ex operators function as expected.
 *
 * This file implements tests for the timespec_ex operator and other
 * functions.
 */

// self
//
#include    <snapdev/timespec_ex.h>

#include    "catch_main.h"


// snapdev
//
#include    <snapdev/not_reached.h>


// C++
//
#include    <iomanip>


// last include
//
#include    <snapdev/poison.h>



namespace
{



int g_state(-1);
int g_error(0);

char * wrap_nl_langinfo(nl_item item)
{
    switch(g_state++)
    {
    case 0:
        if(item != D_T_FMT)
        {
            g_error = 1;
        }
        return const_cast<char *>(""); // exercise the empty string

    case 10:
        if(item != D_T_FMT)
        {
            g_error = 1;
        }
        return const_cast<char *>("%T");

    case 20:
        if(item != D_T_FMT)
        {
            g_error = 1;
        }
        return const_cast<char *>("%r %X %EX");

    case 21:
        if(item != T_FMT_AMPM)
        {
            g_error = 1;
        }
        return const_cast<char *>("%I:%M:%S %p");

    case 22:
        if(item != T_FMT)
        {
            g_error = 1;
        }
        return const_cast<char *>("%H:%M:%S");

    case 23:
        if(item != ERA_T_FMT)
        {
            g_error = 1;
        }
        return const_cast<char *>("%H.%M.%S");

    default:
        throw std::logic_error("invalid state encountered.");

    }

    snapdev::NOT_REACHED();
}



}
// no name namespace



CATCH_TEST_CASE("timespec_ex_math", "[time][math]")
{
    CATCH_START_SECTION("timespec_ex_math: simple add")
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

    CATCH_START_SECTION("timespec_ex_math: simple subtract")
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

    CATCH_START_SECTION("timespec_ex_math: add \"minus one day\"")
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

    CATCH_START_SECTION("timespec_ex_math: add with nano overflow")
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

    CATCH_START_SECTION("timespec_ex_math: subtract with nano underflow")
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

    CATCH_START_SECTION("timespec_ex_math: -1, 0, +1")
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

    CATCH_START_SECTION("timespec_ex_math: add nanos")
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

    CATCH_START_SECTION("timespec_ex_math: load/save")
    {
        snapdev::timespec_ex now(1629652549L, 913'788'345L);

        std::int64_t nsec(now.to_nsec());
        CATCH_REQUIRE(nsec == 1629652549L * 1'000'000'000L + 913'788'345L);

        std::int64_t usec(now.to_usec());
        CATCH_REQUIRE(usec == 1629652549L * 1'000'000 + 913'788L);

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

    CATCH_START_SECTION("timespec_ex_math: negative + negative")
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

    CATCH_START_SECTION("timespec_ex_math: system time")
    {
        snapdev::timespec_ex now;
        timespec verify = {};

        now = snapdev::timespec_ex::gettime();
        clock_gettime(CLOCK_REALTIME, &verify);

        snapdev::timespec_ex const diff(now - verify);
        snapdev::timespec_ex const max_diff(100L);

        CATCH_REQUIRE(diff < max_diff);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("timespec_ex_math: system time")
    {
        timespec verify = {};

        snapdev::timespec_ex const now(snapdev::now());
        clock_gettime(CLOCK_REALTIME, &verify);

        snapdev::timespec_ex const diff(now - verify);
        snapdev::timespec_ex const max_diff(0, 100L);

        CATCH_REQUIRE(diff < max_diff);
    }
    CATCH_END_SECTION()
}


CATCH_TEST_CASE("timespec_ex_string", "[time][string]")
{
    CATCH_START_SECTION("timespec_ex_string: ostream")
    {
        // 4511.913788345
        snapdev::timespec_ex a(timespec{ 4511L, 913'788'345L });
        snapdev::timespec_ex b;

        std::stringstream ss;
        ss << a;

        CATCH_REQUIRE(ss.str() == "4511.913788345");
        CATCH_REQUIRE(a.to_timestamp() == "4511.913788345");
        CATCH_REQUIRE(a.to_timestamp(true) == "4511.913788345");

        b.set(ss.str());
        CATCH_REQUIRE(a == b);

        b = "4511.913788345144763"; // ignore digits after 9 decimals
        CATCH_REQUIRE(a == b);

        b = "4511.91378834598771"; // try again with an odd number of digits
        CATCH_REQUIRE(a == b);

        // 83207.000000000
        ss.str(std::string());
        a.tv_sec = 83207L;
        a.tv_nsec = 0;
        ss << a;

        CATCH_REQUIRE(ss.str() == "83207");
        CATCH_REQUIRE(a.to_timestamp() == "83207.000000000");
        CATCH_REQUIRE(a.to_timestamp(true) == "83207");

        b.set(ss.str());
        CATCH_REQUIRE(a == b);
        b.set("   83207.000000000  ");
        CATCH_REQUIRE(a == b);
        b.set("   83207.0  ");
        CATCH_REQUIRE(a == b);
        b.set("   83207.  ");
        CATCH_REQUIRE(a == b);
        b.set("   83207  ");
        CATCH_REQUIRE(a == b);
        b.set("   +83207s  ");
        CATCH_REQUIRE(a == b);
        b.set("   83207.s  ");
        CATCH_REQUIRE(a == b);
        b.set("   83207.0s  ");
        CATCH_REQUIRE(a == b);

        snapdev::timespec_ex c("+83207.0s");
        CATCH_REQUIRE(a == c);

        // 0.000000101
        ss.str(std::string());
        a.tv_sec = 0L;
        a.tv_nsec = 101;
        ss << a;

        CATCH_REQUIRE(ss.str() == "0.000000101");
        CATCH_REQUIRE(a.to_timestamp() == "0.000000101");
        CATCH_REQUIRE(a.to_timestamp(true) == "0.000000101");

        b.set(ss.str());
        CATCH_REQUIRE(a == b);

        // 1000000000.781200000
        ss.str(std::string());
        a.tv_sec = 1000000000L;
        a.tv_nsec = 781200000;
        ss << snapdev::setremovetrailingzeroes(false) << a;

        CATCH_REQUIRE(ss.str() == "1000000000.781200000");
        CATCH_REQUIRE(a.to_timestamp() == "1000000000.781200000");
        CATCH_REQUIRE(a.to_timestamp(true) == "1000000000.7812");

        b.set(ss.str());
        CATCH_REQUIRE(a == b);

        // -259.900000000
        ss.str(std::string());
        a.tv_sec = -259L;
        a.tv_nsec = 900000000;
        ss << a;

        CATCH_REQUIRE(ss.str() == "-259.900000000");
        CATCH_REQUIRE(a.to_timestamp() == "-259.900000000");
        CATCH_REQUIRE(a.to_timestamp(true) == "-259.9");

        b.set(ss.str());
        CATCH_REQUIRE(a == b);
        b.set("    -259.900000000   ");
        CATCH_REQUIRE(a == b);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("timespec_ex_string: convert to string")
    {
        snapdev::timespec_ex a(timespec{ 4511L, 913'788'345L });

//        struct tm date_and_time = {};
//        struct tm * ptr(nullptr);
//        ptr = localtime_r(&a.tv_sec, &date_and_time);
//std::cerr << "a.tv_sec = " << a.tv_sec << " but " << date_and_time.tm_sec << "\n";

        std::string out(a.to_string("%s.%N"));
        CATCH_REQUIRE(out == "4511.913788345");

        out = a.to_string();
//std::cerr << "seconds misplaced in: " << out << "?\n";
        CATCH_REQUIRE(out.find("11.913788345") != std::string::npos);

        a.tv_nsec = 913'000'000;
        out = a.to_string("%s.%N");
        CATCH_REQUIRE(out == "4511.913000000");
        out = a.to_string("%s.%EN");
        CATCH_REQUIRE(out == "4511.913");

        a.tv_nsec = 0;
        out = a.to_string("%s.%N");
        CATCH_REQUIRE(out == "4511.000000000");
        out = a.to_string("%s.%EN");
        CATCH_REQUIRE(out == "4511.0");
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("timespec_ex_string: convert to string when nl_langinfo(\"D_T_FMT\") returns \"\".")
    {
        snapdev::timespec_ex a(timespec{ 4511L, 913'788'345L });

        time_t date(4511L);
        struct tm t = *localtime(&date);
        char expected[256];
        strftime(expected, sizeof(expected), "%a %b %e %H:%M:%S.913788345 %Y", &t);

        g_state = 0;
        g_error = 0;
        std::string out(a.to_string<wrap_nl_langinfo>());
        CATCH_REQUIRE(g_error == 0);
        CATCH_REQUIRE(out == expected);

    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("timespec_ex_string: convert to string when nl_langinfo(\"D_T_FMT\") returns \"%T\".")
    {
        snapdev::timespec_ex a(timespec{ 4511L, 913'788'345L });

        time_t date(4511L);
        struct tm t = *localtime(&date);
        char expected[256];
        strftime(expected, sizeof(expected), "%T.913788345", &t);

        g_state = 10;
        g_error = 0;
        std::string out(a.to_string<wrap_nl_langinfo>());
        CATCH_REQUIRE(g_error == 0);
        CATCH_REQUIRE(out == expected);

    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("timespec_ex_string: convert to string when nl_langinfo(\"D_T_FMT\") returns \"%r %X %EX\".")
    {
        snapdev::timespec_ex a(timespec{ 4511L, 913'788'345L });

        time_t date(4511L);
        struct tm t = *localtime(&date);
        char expected[256];
        strftime(
              expected
            , sizeof(expected)
            , "%I:%M:%S.913788345 %p"       // %r
              " %H:%M:%S.913788345"         // %X
              " %H.%M.%S.913788345"         // %EX
            , &t);

        g_state = 20;
        g_error = 0;
        std::string out(a.to_string<wrap_nl_langinfo>());
        CATCH_REQUIRE(g_error == 0);
        CATCH_REQUIRE(out == expected);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("timespec_ex_string: to and from string.")
    {
        snapdev::timespec_ex a(timespec{ rand(), rand() % 1'000'000'000 });
        snapdev::timespec_ex b;

        std::string const format("%D %T");
        std::string const s(a.to_string(format));
        b.from_string(s, format);

        // we do not have support for %N just yet in the "from_string()"
        // so here we instead just test the seconds
        //
        int const diff(labs(a.tv_sec - b.tv_sec));
        bool const result(diff == 0 || diff == 3600);
        CATCH_REQUIRE(result); // this is not correct, many countries are "off" the timezone by 30 or 15 min.
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("timespec_ex_string: to and from string with %N.")
    {
        snapdev::timespec_ex a(timespec{ rand(), rand() % 1'000'000'000 });
        snapdev::timespec_ex b;

        std::string format("%D %T.%N");
        std::string s(a.to_string(format));
        //b.from_string(s, format);
        CATCH_REQUIRE_THROWS_MATCHES(
                  b.from_string(s, format)
                , libexcept::fixme
                , Catch::Matchers::ExceptionMessage(
                          "fixme: the from_string() %N extension is not yet implemented."));

        // once it works:
        //
        //CATCH_REQUIRE(a == b);
    }
    CATCH_END_SECTION()
}


CATCH_TEST_CASE("timespec_ex_from_string_error", "[time][string][error]")
{
    CATCH_START_SECTION("timespec_ex_from_string_error: string does not start with a sign or digit")
    {
        CATCH_REQUIRE_THROWS_MATCHES(
                  snapdev::timespec_ex("@34.506")
                , snapdev::syntax_error
                , Catch::Matchers::ExceptionMessage(
                          "timespec_ex_exception: number of seconds must include at least one digit, even if '0'."));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("timespec_ex_from_string_error: seconds overflow")
    {
        CATCH_REQUIRE_THROWS_MATCHES(
                  snapdev::timespec_ex("-9223372036854775808.506")
                , snapdev::overflow
                , Catch::Matchers::ExceptionMessage(
                          "timespec_ex_exception: number of seconds is too large."));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("timespec_ex_from_string_error: bad unit")
    {
        CATCH_REQUIRE_THROWS_MATCHES(
                  snapdev::timespec_ex("-2036854775808.506sec")
                , snapdev::syntax_error
                , Catch::Matchers::ExceptionMessage(
                          "timespec_ex_exception: number include unexpected characters (ec)."));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("timespec_ex_from_string_error: bad unit")
    {
        snapdev::timespec_ex t;
        t.tv_sec = 34471;
        t.tv_nsec = 1000000000;
        CATCH_REQUIRE_THROWS_MATCHES(
                  t.to_string("%s.%N")
                , snapdev::overflow
                , Catch::Matchers::ExceptionMessage(
                          "timespec_ex_exception: tv_nsec is 1 billion or more, which is invalid."));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("timespec_ex_from_string_error: output too long")
    {
        snapdev::timespec_ex t;
        t.tv_sec = rand();
        t.tv_nsec = rand() % 1000000000;
        std::string const format(
                "If we use the format string as a string too, then we can end up with a string that's just way way way too long."
                " Our current limit is 256 characters so that limits our message. The truth is that you limit yourself"
                " to just a date and time string rather than a whole message in this format string."
                " It's probably easier to handle too... The date is: %D. The time is: %T."
                " And we also have %F, %c, %r, %X, %EX for the alternative format, etc.");
        CATCH_REQUIRE_THROWS_MATCHES(
                  t.to_string(format)
                , snapdev::overflow
                , Catch::Matchers::ExceptionMessage(
                          "timespec_ex_exception: the specified strftime() format \""
                        + format
                        + "\" failed."));
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
