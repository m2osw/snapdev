// Copyright (c) 2021-2025  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Verify the various __int128 extension functions.
 *
 * This file implements tests to verify:
 *
 * * support printing out 128 bit numbers
 * * support of 128 bit literals
 * * support of 128 bit powers
 */

// self
//
#include    "catch_main.h"



// snapdev
//
#include    <snapdev/ostream_int128.h>
#include    <snapdev/int128_literal.h>
#include    <snapdev/math.h>


// last include
//
#include    <snapdev/poison.h>


// __int128 is not ISO C++ yet
#pragma GCC diagnostic ignored "-Wpedantic"


CATCH_TEST_CASE("ostream_int128", "[stream][int128]")
{
    CATCH_START_SECTION("ostream_int128: small numbers (-10 to +10)")
    {
        for(int i(-10); i <= 10; ++i)
        {
            __int128 l(i);

            // decimal
            {
                std::stringstream v;
                v << l;
                CATCH_REQUIRE(std::to_string(i) == v.str());
            }
            {
                std::stringstream v;
                v << std::showpos << l;

                // WARNING: I broke up the following on multiple lines because
                //          there seems to be a huge bug in g++12.3.0 which
                //          otherwise generates a buffer overflow error
                //
                //std::string e((i >= 0 ? "+" : "") + std::to_string(i));
                std::string e(std::to_string(i));
                if(i >= 0)
                {
                    e = "+" + e;
                }

                CATCH_REQUIRE(e == v.str());
            }

            // hexadecimal
            {
                std::stringstream v, e;
                v << std::hex << l;
                e << (i < 0 ? "ffffffffffffffffffffffff" : "") << std::hex << i;
                CATCH_REQUIRE(e.str() == v.str());
            }
            {
                std::stringstream v, e;
                v << std::showpos << std::hex << l;
                e << (i < 0 ? "ffffffffffffffffffffffff" : "") << std::hex << i;
                CATCH_REQUIRE(v.str() == e.str());
            }
            {
                std::stringstream v, e;
                v << std::hex << std::uppercase << l;
                e << (i < 0 ? "FFFFFFFFFFFFFFFFFFFFFFFF" : "") << std::hex << std::uppercase << i;
                CATCH_REQUIRE(v.str() == e.str());
            }
            {
                std::stringstream v, e;
                v << std::hex << std::showbase << l;
                e << (i < 0 ? "0xffffffffffffffffffffffff" : "0x") << std::hex << i;
                CATCH_REQUIRE(v.str() == (i == 0 ? "0" : e.str()));
            }
            {
                std::stringstream v, e;
                v << std::hex << std::showbase << std::uppercase << std::showpos << l;
                e << (i < 0 ? "0XFFFFFFFFFFFFFFFFFFFFFFFF" : "0X") << std::hex << std::uppercase << i;
                CATCH_REQUIRE(v.str() == (i == 0 ? "0" : e.str()));
            }

            // octal
            {
                std::stringstream v, e;
                v << std::oct << l;
                e << (i < 0 ? "37777777777777777777777777777777" : "") << std::oct << (i & 0x1FFFFFFFF);
                CATCH_REQUIRE(v.str() == e.str());
            }
            {
                std::stringstream v, e;
                v << std::oct << std::showbase << l;
                e << (i < 0 ? "037777777777777777777777777777777" : "0") << std::oct << (i & 0x1FFFFFFFF);
                CATCH_REQUIRE(v.str() == (i == 0 ? "0" : e.str()));
            }
            {
                std::stringstream v, e;
                v << std::oct << std::showpos << std::showbase << l;
                e << (i < 0 ? "037777777777777777777777777777777" : "0") << std::oct << (i & 0x1FFFFFFFF);
                CATCH_REQUIRE(v.str() == (i == 0 ? "0" : e.str()));
            }
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("ostream_int128: positive numbers")
    {
        for(int i(0); i < 1000; ++i)
        {
            std::int32_t v((rand() & 0x7FFFFFFF));
            __int128 l(v);
            std::stringstream ss;
            ss << l;
            CATCH_REQUIRE(ss.str() == std::to_string(v));
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("ostream_int128: negative numbers")
    {
        for(int i(0); i < 1000; ++i)
        {
            std::int32_t v(-(rand() & 0x7FFFFFFF));
            __int128 l(v);
            std::stringstream ss;
            ss << l;
            CATCH_REQUIRE(ss.str() == std::to_string(v));
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("ostream_int128: most positive number")
    {
        unsigned __int128 l(0);
        l = ~l;
        {
            std::stringstream ss;
            ss << l;
            CATCH_REQUIRE(ss.str() == "340282366920938463463374607431768211455");
        }
        {
            std::stringstream ss;
            ss << std::hex << l;
            CATCH_REQUIRE(ss.str() == "ffffffffffffffffffffffffffffffff");
        }
        {
            std::stringstream ss;
            ss << std::hex << std::uppercase << l;
            CATCH_REQUIRE(ss.str() == "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");
        }
        {
            std::stringstream ss;
            ss << std::hex << std::showbase << l;
            CATCH_REQUIRE(ss.str() == "0xffffffffffffffffffffffffffffffff");
        }
        {
            std::stringstream ss;
            ss << std::hex << std::uppercase << std::showbase << l;
            CATCH_REQUIRE(ss.str() == "0XFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("ostream_int128: most negative number")
    {
        __int128 l(1);
        l <<= 127;
        {
            std::stringstream ss;
            ss << l;
            CATCH_REQUIRE(ss.str() == "-170141183460469231731687303715884105728");
        }
        {
            std::stringstream ss;
            ss << std::hex << l;
            CATCH_REQUIRE(ss.str() == "80000000000000000000000000000000");
        }
    }
    CATCH_END_SECTION()
}


CATCH_TEST_CASE("int128_literal", "[literal][int128]")
{
    CATCH_START_SECTION("int128_literal: zero and powers of two")
    {
        using namespace snapdev::literals;

        __int128 m0(0_int128);
        CATCH_REQUIRE(m0 == 0);

        __int128 v(1);
        {
            __int128 l(1_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("1"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(2_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("2"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(4_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("4"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(8_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("8"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(16_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("16"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(32_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("32"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(64_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("64"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(128_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("128"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(256_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("256"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(512_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("512"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(1024_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("1024"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(2048_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("2048"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(4096_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("4096"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(8192_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("8192"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(16384_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("16384"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(32768_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("32768"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(65536_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("65536"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(131072_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("131072"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(262144_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("262144"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(524288_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("524288"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(1048576_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("1048576"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(2097152_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("2097152"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(4194304_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("4194304"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(8388608_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("8388608"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(16777216_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("16777216"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(33554432_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("33554432"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(67108864_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("67108864"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(134217728_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("134217728"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(268435456_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("268435456"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(536870912_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("536870912"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(1073741824_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("1073741824"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(2147483648_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("2147483648"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(4294967296_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("4294967296"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(8589934592_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("8589934592"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(17179869184_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("17179869184"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(34359738368_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("34359738368"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(68719476736_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("68719476736"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(137438953472_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("137438953472"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(274877906944_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("274877906944"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(549755813888_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("549755813888"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(1099511627776_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("1099511627776"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(2199023255552_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("2199023255552"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(4398046511104_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("4398046511104"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(8796093022208_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("8796093022208"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(17592186044416_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("17592186044416"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(35184372088832_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("35184372088832"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(70368744177664_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("70368744177664"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(140737488355328_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("140737488355328"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(281474976710656_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("281474976710656"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(562949953421312_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("562949953421312"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(1125899906842624_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("1125899906842624"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(2251799813685248_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("2251799813685248"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(4503599627370496_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("4503599627370496"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(9007199254740992_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("9007199254740992"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(18014398509481984_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("18014398509481984"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(36028797018963968_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("36028797018963968"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(72057594037927936_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("72057594037927936"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(144115188075855872_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("144115188075855872"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(288230376151711744_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("288230376151711744"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(576460752303423488_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("576460752303423488"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(1152921504606846976_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("1152921504606846976"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(2305843009213693952_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("2305843009213693952"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(4611686018427387904_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("4611686018427387904"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(9223372036854775808_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("9223372036854775808"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(18446744073709551616_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("18446744073709551616"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(36893488147419103232_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("36893488147419103232"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(73786976294838206464_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("73786976294838206464"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(147573952589676412928_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("147573952589676412928"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(295147905179352825856_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("295147905179352825856"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(590295810358705651712_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("590295810358705651712"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(1180591620717411303424_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("1180591620717411303424"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(2361183241434822606848_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("2361183241434822606848"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(4722366482869645213696_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("4722366482869645213696"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(9444732965739290427392_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("9444732965739290427392"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(18889465931478580854784_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("18889465931478580854784"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(37778931862957161709568_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("37778931862957161709568"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(75557863725914323419136_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("75557863725914323419136"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(151115727451828646838272_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("151115727451828646838272"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(302231454903657293676544_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("302231454903657293676544"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(604462909807314587353088_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("604462909807314587353088"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(1208925819614629174706176_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("1208925819614629174706176"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(2417851639229258349412352_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("2417851639229258349412352"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(4835703278458516698824704_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("4835703278458516698824704"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(9671406556917033397649408_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("9671406556917033397649408"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(19342813113834066795298816_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("19342813113834066795298816"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(38685626227668133590597632_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("38685626227668133590597632"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(77371252455336267181195264_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("77371252455336267181195264"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(154742504910672534362390528_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("154742504910672534362390528"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(309485009821345068724781056_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("309485009821345068724781056"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(618970019642690137449562112_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("618970019642690137449562112"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(1237940039285380274899124224_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("1237940039285380274899124224"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(2475880078570760549798248448_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("2475880078570760549798248448"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(4951760157141521099596496896_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("4951760157141521099596496896"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(9903520314283042199192993792_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("9903520314283042199192993792"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(19807040628566084398385987584_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("19807040628566084398385987584"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(39614081257132168796771975168_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("39614081257132168796771975168"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(79228162514264337593543950336_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("79228162514264337593543950336"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(158456325028528675187087900672_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("158456325028528675187087900672"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(316912650057057350374175801344_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("316912650057057350374175801344"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(633825300114114700748351602688_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("633825300114114700748351602688"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(1267650600228229401496703205376_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("1267650600228229401496703205376"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(2535301200456458802993406410752_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("2535301200456458802993406410752"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(5070602400912917605986812821504_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("5070602400912917605986812821504"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(10141204801825835211973625643008_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("10141204801825835211973625643008"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(20282409603651670423947251286016_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("20282409603651670423947251286016"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(40564819207303340847894502572032_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("40564819207303340847894502572032"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(81129638414606681695789005144064_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("81129638414606681695789005144064"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(162259276829213363391578010288128_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("162259276829213363391578010288128"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(324518553658426726783156020576256_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("324518553658426726783156020576256"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(649037107316853453566312041152512_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("649037107316853453566312041152512"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(1298074214633706907132624082305024_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("1298074214633706907132624082305024"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(2596148429267413814265248164610048_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("2596148429267413814265248164610048"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(5192296858534827628530496329220096_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("5192296858534827628530496329220096"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(10384593717069655257060992658440192_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("10384593717069655257060992658440192"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(20769187434139310514121985316880384_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("20769187434139310514121985316880384"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(41538374868278621028243970633760768_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("41538374868278621028243970633760768"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(83076749736557242056487941267521536_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("83076749736557242056487941267521536"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(166153499473114484112975882535043072_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("166153499473114484112975882535043072"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(332306998946228968225951765070086144_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("332306998946228968225951765070086144"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(664613997892457936451903530140172288_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("664613997892457936451903530140172288"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(1329227995784915872903807060280344576_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("1329227995784915872903807060280344576"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(2658455991569831745807614120560689152_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("2658455991569831745807614120560689152"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(5316911983139663491615228241121378304_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("5316911983139663491615228241121378304"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(10633823966279326983230456482242756608_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("10633823966279326983230456482242756608"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(21267647932558653966460912964485513216_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("21267647932558653966460912964485513216"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(42535295865117307932921825928971026432_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("42535295865117307932921825928971026432"_int128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            __int128 l(85070591730234615865843651857942052864_int128);
            CATCH_REQUIRE(l == v);
        }
        {
            __int128 l("85070591730234615865843651857942052864"_int128);
            CATCH_REQUIRE(l == v);
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("uint128_literal: unsigned zero and powers of two")
    {
        using namespace snapdev::literals;

        unsigned __int128 m0(0_uint128);
        CATCH_REQUIRE(m0 == 0);

        unsigned __int128 v(1);
        {
            unsigned __int128 l(1_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("1"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(2_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("2"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(4_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("4"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(8_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("8"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(16_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("16"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(32_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("32"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(64_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("64"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(128_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("128"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(256_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("256"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(512_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("512"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(1024_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("1024"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(2048_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("2048"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(4096_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("4096"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(8192_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("8192"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(16384_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("16384"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(32768_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("32768"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(65536_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("65536"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(131072_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("131072"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(262144_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("262144"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(524288_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("524288"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(1048576_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("1048576"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(2097152_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("2097152"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(4194304_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("4194304"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(8388608_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("8388608"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(16777216_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("16777216"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(33554432_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("33554432"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(67108864_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("67108864"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(134217728_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("134217728"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(268435456_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("268435456"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(536870912_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("536870912"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(1073741824_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("1073741824"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(2147483648_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("2147483648"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(4294967296_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("4294967296"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(8589934592_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("8589934592"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(17179869184_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("17179869184"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(34359738368_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("34359738368"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(68719476736_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("68719476736"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(137438953472_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("137438953472"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(274877906944_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("274877906944"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(549755813888_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("549755813888"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(1099511627776_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("1099511627776"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(2199023255552_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("2199023255552"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(4398046511104_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("4398046511104"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(8796093022208_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("8796093022208"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(17592186044416_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("17592186044416"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(35184372088832_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("35184372088832"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(70368744177664_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("70368744177664"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(140737488355328_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("140737488355328"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(281474976710656_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("281474976710656"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(562949953421312_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("562949953421312"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(1125899906842624_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("1125899906842624"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(2251799813685248_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("2251799813685248"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(4503599627370496_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("4503599627370496"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(9007199254740992_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("9007199254740992"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(18014398509481984_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("18014398509481984"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(36028797018963968_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("36028797018963968"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(72057594037927936_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("72057594037927936"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(144115188075855872_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("144115188075855872"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(288230376151711744_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("288230376151711744"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(576460752303423488_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("576460752303423488"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(1152921504606846976_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("1152921504606846976"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(2305843009213693952_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("2305843009213693952"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(4611686018427387904_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("4611686018427387904"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(9223372036854775808_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("9223372036854775808"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(18446744073709551616_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("18446744073709551616"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(36893488147419103232_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("36893488147419103232"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(73786976294838206464_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("73786976294838206464"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(147573952589676412928_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("147573952589676412928"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(295147905179352825856_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("295147905179352825856"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(590295810358705651712_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("590295810358705651712"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(1180591620717411303424_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("1180591620717411303424"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(2361183241434822606848_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("2361183241434822606848"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(4722366482869645213696_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("4722366482869645213696"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(9444732965739290427392_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("9444732965739290427392"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(18889465931478580854784_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("18889465931478580854784"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(37778931862957161709568_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("37778931862957161709568"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(75557863725914323419136_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("75557863725914323419136"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(151115727451828646838272_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("151115727451828646838272"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(302231454903657293676544_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("302231454903657293676544"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(604462909807314587353088_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("604462909807314587353088"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(1208925819614629174706176_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("1208925819614629174706176"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(2417851639229258349412352_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("2417851639229258349412352"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(4835703278458516698824704_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("4835703278458516698824704"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(9671406556917033397649408_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("9671406556917033397649408"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(19342813113834066795298816_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("19342813113834066795298816"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(38685626227668133590597632_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("38685626227668133590597632"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(77371252455336267181195264_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("77371252455336267181195264"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(154742504910672534362390528_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("154742504910672534362390528"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(309485009821345068724781056_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("309485009821345068724781056"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(618970019642690137449562112_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("618970019642690137449562112"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(1237940039285380274899124224_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("1237940039285380274899124224"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(2475880078570760549798248448_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("2475880078570760549798248448"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(4951760157141521099596496896_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("4951760157141521099596496896"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(9903520314283042199192993792_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("9903520314283042199192993792"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(19807040628566084398385987584_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("19807040628566084398385987584"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(39614081257132168796771975168_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("39614081257132168796771975168"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(79228162514264337593543950336_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("79228162514264337593543950336"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(158456325028528675187087900672_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("158456325028528675187087900672"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(316912650057057350374175801344_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("316912650057057350374175801344"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(633825300114114700748351602688_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("633825300114114700748351602688"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(1267650600228229401496703205376_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("1267650600228229401496703205376"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(2535301200456458802993406410752_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("2535301200456458802993406410752"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(5070602400912917605986812821504_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("5070602400912917605986812821504"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(10141204801825835211973625643008_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("10141204801825835211973625643008"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(20282409603651670423947251286016_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("20282409603651670423947251286016"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(40564819207303340847894502572032_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("40564819207303340847894502572032"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(81129638414606681695789005144064_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("81129638414606681695789005144064"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(162259276829213363391578010288128_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("162259276829213363391578010288128"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(324518553658426726783156020576256_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("324518553658426726783156020576256"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(649037107316853453566312041152512_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("649037107316853453566312041152512"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(1298074214633706907132624082305024_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("1298074214633706907132624082305024"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(2596148429267413814265248164610048_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("2596148429267413814265248164610048"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(5192296858534827628530496329220096_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("5192296858534827628530496329220096"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(10384593717069655257060992658440192_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("10384593717069655257060992658440192"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(20769187434139310514121985316880384_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("20769187434139310514121985316880384"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(41538374868278621028243970633760768_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("41538374868278621028243970633760768"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(83076749736557242056487941267521536_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("83076749736557242056487941267521536"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(166153499473114484112975882535043072_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("166153499473114484112975882535043072"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(332306998946228968225951765070086144_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("332306998946228968225951765070086144"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(664613997892457936451903530140172288_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("664613997892457936451903530140172288"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(1329227995784915872903807060280344576_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("1329227995784915872903807060280344576"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(2658455991569831745807614120560689152_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("2658455991569831745807614120560689152"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(5316911983139663491615228241121378304_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("5316911983139663491615228241121378304"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(10633823966279326983230456482242756608_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("10633823966279326983230456482242756608"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(21267647932558653966460912964485513216_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("21267647932558653966460912964485513216"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(42535295865117307932921825928971026432_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("42535295865117307932921825928971026432"_uint128);
            CATCH_REQUIRE(l == v);
        }
        v <<= 1;
        {
            unsigned __int128 l(85070591730234615865843651857942052864_uint128);
            CATCH_REQUIRE(l == v);
        }
        {
            unsigned __int128 l("85070591730234615865843651857942052864"_uint128);
            CATCH_REQUIRE(l == v);
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("[u]int128_literal: large numbers in decimal, hexadecimal, octal and binary")
    {
        using namespace snapdev::literals;

        // test a negative decimal number of 128 bits
        // (the minus is arithmetic in the compiler, not part of the literal)
        {
            __int128 const top(0x1485B9AD5F387C22);
            __int128 const bottom(0x1A94732C11DA7DAF);
            __int128 const v((top << 64UL) | bottom);
            __int128 l(-27278901379601602040904188621408075183_int128);
            CATCH_REQUIRE(l == -v);
            __int128 s(-"27278901379601602040904188621408075183"_int128);
            CATCH_REQUIRE(s == -v);
        }

        // test a hexadecimal number of 128 bits
        {
            unsigned __int128 const top(0xFA83B93D5C387C22);
            unsigned __int128 const bottom(0x1A94C329111A73AF);
            unsigned __int128 const v((top << 64UL) | bottom);
            unsigned __int128 l(0xFA83B93D5C387c221A94C329111a73AF_uint128);
            CATCH_REQUIRE(l == v);
            unsigned __int128 s("0xFA83B93D5C387c221A94C329111a73AF"_uint128);
            CATCH_REQUIRE(s == v);
        }

        // test an octal number of 126 bits
        {
            unsigned __int128 const top(0352035623653416076042); // this is only 63 bits (21 digits each representing 3 bits)
            unsigned __int128 const bottom(0152246062442106471657);
            unsigned __int128 const v((top << 63UL) | bottom);
            unsigned __int128 l(0352035623653416076042152246062442106471657_uint128);
            CATCH_REQUIRE(l == v);
            unsigned __int128 s("0352035623653416076042152246062442106471657"_uint128);
            CATCH_REQUIRE(s == v);
        }

        // test a binary number of 126 bits
        {
            unsigned __int128 const top(0352535624657416076142); // this is only 63 bits (21 digits each representing 3 bits)
            unsigned __int128 const bottom(0132245062742106471635);
            unsigned __int128 const v((top << 63UL) | bottom);
            unsigned __int128 l(0b11101010101011101110010100110101111100001110000111110001100010001011010010100101000110010111100010001000110100111001110011101_uint128);
            CATCH_REQUIRE(l == v);
            unsigned __int128 s("0b11101010101011101110010100110101111100001110000111110001100010001011010010100101000110010111100010001000110100111001110011101"_uint128);
            CATCH_REQUIRE(s == v);
        }
    }
    CATCH_END_SECTION()
}


CATCH_TEST_CASE("int128_powers", "[math][int128]")
{
    CATCH_START_SECTION("int128: 2^n with n from 0 to 127")
    {
        using namespace snapdev::literals;

        __int128 v(1);
        for(int p(0); p < 128; ++p, v *= 2)
        {
            CATCH_REQUIRE(snapdev::pow(2_int128, p) == v);
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("uint128: 2^n with n from 0 to 127")
    {
        using namespace snapdev::literals;

        unsigned __int128 v(1);
        for(int p(0); p < 128; ++p, v *= 2)
        {
            CATCH_REQUIRE(snapdev::pow(2_uint128, p) == v);
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("[u]int128: n^0 where n is a random number")
    {
        using namespace snapdev::literals;

        for(int repeat(0); repeat < 10; ++repeat)
        {
            unsigned __int128 u(0_uint128);
            for(int p(0); p < 16; ++p)
            {
                u <<= 8;
                u |= rand() & 0x00FF;
            }
            CATCH_REQUIRE(snapdev::pow(u, 0) == 1);

            __int128 v(0_uint128);
            for(int p(0); p < 16; ++p)
            {
                v <<= 8;
                v |= rand() & 0x00FF;
            }
            if((repeat & 1) == 0 && v > 0)
            {
                // make sure the value is negative at times
                //
                v = -v;
            }
            CATCH_REQUIRE(snapdev::pow(v, 0) == 1);
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("int128: 1^n with n < 0")
    {
        using namespace snapdev::literals;

        for(int p(-128); p < 0; ++p)
        {
            CATCH_REQUIRE(snapdev::pow(1_int128, p) == 1);
            CATCH_REQUIRE(snapdev::pow(1_uint128, p) == 1);

            // for any other number, it becomes 0
            //
            CATCH_REQUIRE(snapdev::pow(2_int128, p) == 0);
            CATCH_REQUIRE(snapdev::pow(2_uint128, p) == 0);
            CATCH_REQUIRE(snapdev::pow(3_int128, p) == 0);
            CATCH_REQUIRE(snapdev::pow(3_uint128, p) == 0);
            CATCH_REQUIRE(snapdev::pow(4_int128, p) == 0);
            CATCH_REQUIRE(snapdev::pow(4_uint128, p) == 0);
            CATCH_REQUIRE(snapdev::pow(5_int128, p) == 0);
            CATCH_REQUIRE(snapdev::pow(5_uint128, p) == 0);
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("int128: -1^n with n < 0")
    {
        using namespace snapdev::literals;

        for(int p(-128); p < 0; ++p)
        {
            if((p & 1) == 0)
            {
                CATCH_REQUIRE(snapdev::pow(-1_int128, p) == 1);
            }
            else
            {
                CATCH_REQUIRE(snapdev::pow(-1_int128, p) == -1);
            }

            // for any other number, it becomes 0
            //
            CATCH_REQUIRE(snapdev::pow(-2_int128, p) == 0);
            CATCH_REQUIRE(snapdev::pow(-3_int128, p) == 0);
            CATCH_REQUIRE(snapdev::pow(-4_int128, p) == 0);
            CATCH_REQUIRE(snapdev::pow(-5_int128, p) == 0);
        }
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
