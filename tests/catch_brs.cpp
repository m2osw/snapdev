// Copyright (c) 2011-2022  Made to Order Software Corp.  All Rights Reserved
//
// https://snapwebsites.org/project/brs
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
 * \brief Verify the BRS functions.
 *
 * This file implements tests to verify that the BRS functions do what
 * they are expected to do.
 */

// self
//
#include    "catch_main.h"


// brs
//
#include    <brs/brs.h>


// C++
//
#include    <fstream>




CATCH_TEST_CASE("basic_types", "[basic]")
{
    CATCH_SECTION("push/restore char")
    {
        std::stringstream buffer;
        brs::serializer out(buffer);

        std::string data(buffer.str());
        CATCH_REQUIRE(data.length() == sizeof(brs::magic_t));

        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == brs::BRS_VERSION);

        char value = 33;
        out.add_value("orange", value);

        // make sure it did not get smashed
        data = buffer.str();
        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == brs::BRS_VERSION);

        CATCH_REQUIRE(data[4] == 6 << 2);  // hunk_sizes_t
        CATCH_REQUIRE(data[5] == 1 << 1);
        CATCH_REQUIRE(data[6] == 0);
        CATCH_REQUIRE(data[7] == 0);

        CATCH_REQUIRE(data[8] == 'o');    // name
        CATCH_REQUIRE(data[9] == 'r');
        CATCH_REQUIRE(data[10] == 'a');
        CATCH_REQUIRE(data[11] == 'n');
        CATCH_REQUIRE(data[12] == 'g');
        CATCH_REQUIRE(data[13] == 'e');

        CATCH_REQUIRE(data[14] == 33);   // value

        struct processor
        {
            static bool process_hunk(
                          brs::deserializer<std::stringstream> & in
                        , brs::field_t const & field)
            {
                CATCH_REQUIRE(field.f_name == "orange");
                CATCH_REQUIRE(field.f_sub_name.empty());
                CATCH_REQUIRE(field.f_index == -1);
                CATCH_REQUIRE(field.f_size == 1);
                char c;
                in.read_data(c);
                CATCH_REQUIRE(c == 33);
                return true;
            }
        };

        // Note: you don't usually end up re-using the serializer buffer
        //       but here it's practical
        //
        buffer.clear();

        brs::deserializer in(buffer);

        // WARNING: we want to use CATCH_...() macros inside the callback
        //          so make sure not to use one around unserialize_buffer().
        //
        brs::deserializer<std::stringstream>::process_hunk_t func(std::bind(
                      &processor::process_hunk
                    , std::placeholders::_1
                    , std::placeholders::_2));
        bool const r(in.deserialize(func));
        CATCH_REQUIRE(r);
    }

    CATCH_SECTION("push/restore signed char")
    {
        std::stringstream buffer;
        brs::serializer out(buffer);

        std::string data(buffer.str());
        CATCH_REQUIRE(data.length() == sizeof(brs::magic_t));

        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == brs::BRS_VERSION);

        signed char value = -43;
        out.add_value("orange", value);

        // make sure it did not get smashed
        data = buffer.str();
        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == brs::BRS_VERSION);

        CATCH_REQUIRE(data[4] == 6 << 2);  // hunk_sizes_t
        CATCH_REQUIRE(data[5] == 1 << 1);
        CATCH_REQUIRE(data[6] == 0);
        CATCH_REQUIRE(data[7] == 0);

        CATCH_REQUIRE(data[8] == 'o');    // name
        CATCH_REQUIRE(data[9] == 'r');
        CATCH_REQUIRE(data[10] == 'a');
        CATCH_REQUIRE(data[11] == 'n');
        CATCH_REQUIRE(data[12] == 'g');
        CATCH_REQUIRE(data[13] == 'e');

        CATCH_REQUIRE(static_cast<signed char>(data[14]) == -43);   // value

        struct processor
        {
            static bool process_hunk(
                          brs::deserializer<std::stringstream> & in
                        , brs::field_t const & field)
            {
                CATCH_REQUIRE(field.f_name == "orange");
                CATCH_REQUIRE(field.f_sub_name.empty());
                CATCH_REQUIRE(field.f_index == -1);
                CATCH_REQUIRE(field.f_size == 1);
                signed char c;
                in.read_data(c);
                CATCH_REQUIRE(c == -43);
                return true;
            }
        };

        // Note: you don't usually end up re-using the serializer buffer
        //       but here it's practical
        //
        buffer.clear();

        brs::deserializer in(buffer);

        // WARNING: we want to use CATCH_...() macros inside the callback
        //          so make sure not to use one around unserialize_buffer().
        //
        brs::deserializer<std::stringstream>::process_hunk_t func(std::bind(
                      &processor::process_hunk
                    , std::placeholders::_1
                    , std::placeholders::_2));
        bool const r(in.deserialize(func));
        CATCH_REQUIRE(r);
    }

    CATCH_SECTION("push/restore unsigned char")
    {
        std::stringstream buffer;
        brs::serializer out(buffer);

        std::string data(buffer.str());
        CATCH_REQUIRE(data.length() == sizeof(brs::magic_t));

        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == brs::BRS_VERSION);

        unsigned char value = 200;
        out.add_value("orange", value);

        // make sure it did not get smashed
        data = buffer.str();
        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == brs::BRS_VERSION);

        CATCH_REQUIRE(data[4] == 6 << 2);  // hunk_sizes_t
        CATCH_REQUIRE(data[5] == 1 << 1);
        CATCH_REQUIRE(data[6] == 0);
        CATCH_REQUIRE(data[7] == 0);

        CATCH_REQUIRE(data[8] == 'o');    // name
        CATCH_REQUIRE(data[9] == 'r');
        CATCH_REQUIRE(data[10] == 'a');
        CATCH_REQUIRE(data[11] == 'n');
        CATCH_REQUIRE(data[12] == 'g');
        CATCH_REQUIRE(data[13] == 'e');

        CATCH_REQUIRE(static_cast<unsigned char>(data[14]) == 200);   // value

        struct processor
        {
            static bool process_hunk(
                          brs::deserializer<std::stringstream> & in
                        , brs::field_t const & field)
            {
                CATCH_REQUIRE(field.f_name == "orange");
                CATCH_REQUIRE(field.f_sub_name.empty());
                CATCH_REQUIRE(field.f_index == -1);
                CATCH_REQUIRE(field.f_size == 1);
                unsigned char c;
                in.read_data(c);
                CATCH_REQUIRE(c == 200);
                return true;
            }
        };

        // Note: you don't usually end up re-using the serializer buffer
        //       but here it's practical
        //
        buffer.clear();

        brs::deserializer in(buffer);

        // WARNING: we want to use CATCH_...() macros inside the callback
        //          so make sure not to use one around unserialize_buffer().
        //
        brs::deserializer<std::stringstream>::process_hunk_t func(std::bind(
                      &processor::process_hunk
                    , std::placeholders::_1
                    , std::placeholders::_2));
        bool const r(in.deserialize(func));
        CATCH_REQUIRE(r);
    }

    CATCH_SECTION("push/restore shorts (16 bits)")
    {
        std::stringstream buffer;
        brs::serializer out(buffer);

        std::string data(buffer.str());
        CATCH_REQUIRE(data.length() == sizeof(brs::magic_t));

        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == brs::BRS_VERSION);

        std::int16_t const purple = 3003;
        out.add_value("purple", purple);

        std::uint16_t const black = 65001;
        out.add_value("black", black);

        // make sure it did not get smashed
        data = buffer.str();
        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == brs::BRS_VERSION);

        CATCH_REQUIRE(data[4] == 6 << 2);  // hunk_sizes_t
        CATCH_REQUIRE(data[5] == 2 << 1);
        CATCH_REQUIRE(data[6] == 0);
        CATCH_REQUIRE(data[7] == 0);

        CATCH_REQUIRE(data[8] == 'p');    // name
        CATCH_REQUIRE(data[9] == 'u');
        CATCH_REQUIRE(data[10] == 'r');
        CATCH_REQUIRE(data[11] == 'p');
        CATCH_REQUIRE(data[12] == 'l');
        CATCH_REQUIRE(data[13] == 'e');

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        CATCH_REQUIRE(static_cast<std::uint8_t>(data[14]) * 256 + static_cast<std::uint8_t>(data[15]) == 3003);   // value
#else
        CATCH_REQUIRE(static_cast<std::uint8_t>(data[14]) + static_cast<std::uint8_t>(data[15]) * 256 == 3003);   // value
#endif

        struct processor
        {
            static bool process_hunk(
                          brs::deserializer<std::stringstream> & in
                        , brs::field_t const & field
                        , std::int16_t purple
                        , std::uint16_t black)
            {
                CATCH_REQUIRE(field.f_sub_name.empty());
                CATCH_REQUIRE(field.f_index == -1);
                CATCH_REQUIRE(field.f_size == 2);
                if(field.f_name == "purple")
                {
                    std::int16_t value;
                    in.read_data(value);
                    CATCH_REQUIRE(value == purple);
                }
                else if(field.f_name == "black")
                {
                    std::uint16_t value;
                    in.read_data(value);
                    CATCH_REQUIRE(value == black);
                }
                else
                {
                    CATCH_REQUIRE(field.f_name == "?unknown?");
                }
                return true;
            }
        };

        // Note: you don't usually end up re-using the serializer buffer
        //       but here it's practical
        //
        buffer.clear();

        brs::deserializer in(buffer);

        // WARNING: we want to use CATCH_...() macros inside the callback
        //          so make sure not to use one around unserialize_buffer().
        //
        brs::deserializer<std::stringstream>::process_hunk_t func(std::bind(
                      &processor::process_hunk
                    , std::placeholders::_1
                    , std::placeholders::_2
                    , purple
                    , black));
        bool const r(in.deserialize(func));
        CATCH_REQUIRE(r);
    }

    CATCH_SECTION("push/restore ints (32 bits)")
    {
        std::stringstream buffer;
        brs::serializer out(buffer);

        std::string data(buffer.str());
        CATCH_REQUIRE(data.length() == sizeof(brs::magic_t));

        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == brs::BRS_VERSION);

        std::int32_t const red = static_cast<std::int32_t>(SNAP_CATCH2_NAMESPACE::rand_int64());
        out.add_value("red", red);

        std::uint32_t const blue = static_cast<std::int32_t>(SNAP_CATCH2_NAMESPACE::rand_int64());
        out.add_value("blue", blue);

        // make sure it did not get smashed
        data = buffer.str();
        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == brs::BRS_VERSION);

        CATCH_REQUIRE(data[4] == 3 << 2);  // hunk_sizes_t
        CATCH_REQUIRE(data[5] == 4 << 1);
        CATCH_REQUIRE(data[6] == 0);
        CATCH_REQUIRE(data[7] == 0);

        CATCH_REQUIRE(data[8] == 'r');    // name
        CATCH_REQUIRE(data[9] == 'e');
        CATCH_REQUIRE(data[10] == 'd');

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        CATCH_REQUIRE(
                  static_cast<std::uint8_t>(data[11]) * 0x1000000
                + static_cast<std::uint8_t>(data[12]) * 0x10000
                + static_cast<std::uint8_t>(data[13]) * 0x100
                + static_cast<std::uint8_t>(data[14]) * 0x1 == red);   // value
#else
        CATCH_REQUIRE(
                  static_cast<std::uint8_t>(data[14]) * 0x1000000
                + static_cast<std::uint8_t>(data[13]) * 0x10000
                + static_cast<std::uint8_t>(data[12]) * 0x100
                + static_cast<std::uint8_t>(data[11]) * 0x1 == red);   // value
#endif

        struct processor
        {
            static bool process_hunk(
                          brs::deserializer<std::stringstream> & in
                        , brs::field_t const & field
                        , std::int32_t red
                        , std::uint32_t blue)
            {
                CATCH_REQUIRE(field.f_sub_name.empty());
                CATCH_REQUIRE(field.f_index == -1);
                CATCH_REQUIRE(field.f_size == 4);
                if(field.f_name == "red")
                {
                    std::int32_t value;
                    in.read_data(value);
                    CATCH_REQUIRE(value == red);
                }
                else if(field.f_name == "blue")
                {
                    std::uint32_t value;
                    in.read_data(value);
                    CATCH_REQUIRE(value == blue);
                }
                else
                {
                    CATCH_REQUIRE(field.f_name == "?unknown?");
                }
                return true;
            }
        };

        // Note: you don't usually end up re-using the serializer buffer
        //       but here it's practical
        //
        buffer.clear();

        brs::deserializer in(buffer);

        // WARNING: we want to use CATCH_...() macros inside the callback
        //          so make sure not to use one around unserialize_buffer().
        //
        brs::deserializer<std::stringstream>::process_hunk_t func(std::bind(
                      &processor::process_hunk
                    , std::placeholders::_1
                    , std::placeholders::_2
                    , red
                    , blue));
        bool const r(in.deserialize(func));
        CATCH_REQUIRE(r);
    }

    CATCH_SECTION("push/restore ints (64 bits)")
    {
        std::stringstream buffer;
        brs::serializer out(buffer);

        std::string data(buffer.str());
        CATCH_REQUIRE(data.length() == sizeof(brs::magic_t));

        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == brs::BRS_VERSION);

        std::int64_t const white = static_cast<std::int32_t>(SNAP_CATCH2_NAMESPACE::rand_int64());
        out.add_value("white", white);

        std::uint64_t const gray = static_cast<std::int32_t>(SNAP_CATCH2_NAMESPACE::rand_int64());
        out.add_value("gray", gray);

        // make sure it did not get smashed
        data = buffer.str();
        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == brs::BRS_VERSION);

        CATCH_REQUIRE(data[4] == 5 << 2);  // hunk_sizes_t
        CATCH_REQUIRE(data[5] == 8 << 1);
        CATCH_REQUIRE(data[6] == 0);
        CATCH_REQUIRE(data[7] == 0);

        CATCH_REQUIRE(data[8] == 'w');    // name
        CATCH_REQUIRE(data[9] == 'h');
        CATCH_REQUIRE(data[10] == 'i');
        CATCH_REQUIRE(data[11] == 't');
        CATCH_REQUIRE(data[12] == 'e');

        struct processor
        {
            static bool process_hunk(
                          brs::deserializer<std::stringstream> & in
                        , brs::field_t const & field
                        , std::int64_t white
                        , std::uint64_t gray)
            {
                CATCH_REQUIRE(field.f_sub_name.empty());
                CATCH_REQUIRE(field.f_index == -1);
                CATCH_REQUIRE(field.f_size == 8);
                if(field.f_name == "white")
                {
                    std::int64_t value;
                    in.read_data(value);
                    CATCH_REQUIRE(value == white);
                }
                else if(field.f_name == "gray")
                {
                    std::uint64_t value;
                    in.read_data(value);
                    CATCH_REQUIRE(value == gray);
                }
                else
                {
                    CATCH_REQUIRE(field.f_name == "?unknown?");
                }
                return true;
            }
        };

        // Note: you don't usually end up re-using the serializer buffer
        //       but here it's practical
        //
        buffer.clear();

        brs::deserializer in(buffer);

        // WARNING: we want to use CATCH_...() macros inside the callback
        //          so make sure not to use one around unserialize_buffer().
        //
        brs::deserializer<std::stringstream>::process_hunk_t func(std::bind(
                      &processor::process_hunk
                    , std::placeholders::_1
                    , std::placeholders::_2
                    , white
                    , gray));
        bool const r(in.deserialize(func));
        CATCH_REQUIRE(r);
    }

    CATCH_SECTION("push/restore floats")
    {
        std::stringstream buffer;
        brs::serializer out(buffer);

        std::string data(buffer.str());
        CATCH_REQUIRE(data.length() == sizeof(brs::magic_t));

        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == brs::BRS_VERSION);

        float const green = static_cast<float>(SNAP_CATCH2_NAMESPACE::rand_int64())
                          / static_cast<float>(SNAP_CATCH2_NAMESPACE::rand_int64());
        out.add_value("green", green);

        double const yellow = static_cast<float>(SNAP_CATCH2_NAMESPACE::rand_int64())
                            / static_cast<float>(SNAP_CATCH2_NAMESPACE::rand_int64());
        out.add_value("yellow", yellow);

        long double const fushia = static_cast<float>(SNAP_CATCH2_NAMESPACE::rand_int64())
                                 / static_cast<float>(SNAP_CATCH2_NAMESPACE::rand_int64());
        out.add_value("fushia", fushia);

        // make sure it did not get smashed
        data = buffer.str();
        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == brs::BRS_VERSION);

        CATCH_REQUIRE(data[4] == 5 << 2);  // hunk_sizes_t
        CATCH_REQUIRE(data[5] == 4 << 1);
        CATCH_REQUIRE(data[6] == 0);
        CATCH_REQUIRE(data[7] == 0);

        CATCH_REQUIRE(data[8] == 'g');    // name
        CATCH_REQUIRE(data[9] == 'r');
        CATCH_REQUIRE(data[10] == 'e');
        CATCH_REQUIRE(data[11] == 'e');
        CATCH_REQUIRE(data[12] == 'n');

        struct processor
        {
            static bool process_hunk(
                          brs::deserializer<std::stringstream> & in
                        , brs::field_t const & field
                        , float green
                        , double yellow
                        , long double fushia)
            {
                CATCH_REQUIRE(field.f_sub_name.empty());
                CATCH_REQUIRE(field.f_index == -1);
                if(field.f_name == "green")
                {
                    CATCH_REQUIRE(field.f_size == 4);

                    float value;
                    in.read_data(value);
                    CATCH_REQUIRE(SNAP_CATCH2_NAMESPACE::nearly_equal(value, green, 0.0f));
                }
                else if(field.f_name == "yellow")
                {
                    CATCH_REQUIRE(field.f_size == 8);

                    double value;
                    in.read_data(value);
                    CATCH_REQUIRE(SNAP_CATCH2_NAMESPACE::nearly_equal(value, yellow, 0.0));
                }
                else if(field.f_name == "fushia")
                {
                    CATCH_REQUIRE(field.f_size == 16);

                    long double value;
                    in.read_data(value);
                    CATCH_REQUIRE(SNAP_CATCH2_NAMESPACE::nearly_equal(value, fushia, 0.0L));
                }
                else
                {
                    CATCH_REQUIRE(field.f_name == "?unknown?");
                }
                return true;
            }
        };

        // Note: you don't usually end up re-using the serializer buffer
        //       but here it's practical
        //
        buffer.clear();

        brs::deserializer in(buffer);

        // WARNING: we want to use CATCH_...() macros inside the callback
        //          so make sure not to use one around unserialize_buffer().
        //
        brs::deserializer<std::stringstream>::process_hunk_t func(std::bind(
                      &processor::process_hunk
                    , std::placeholders::_1
                    , std::placeholders::_2
                    , green
                    , yellow
                    , fushia));
        bool const r(in.deserialize(func));
        CATCH_REQUIRE(r);
    }

    CATCH_SECTION("push/restore string")
    {
        std::stringstream buffer;
        brs::serializer out(buffer);

        std::string data(buffer.str());
        CATCH_REQUIRE(data.length() == sizeof(brs::magic_t));

        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == brs::BRS_VERSION);

        std::string message("this is the message we are going to serialize");
        out.add_value("message",  message);

        // make sure it did not get smashed
        data = buffer.str();
        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == brs::BRS_VERSION);

        CATCH_REQUIRE(data[4] == 7 << 2);  // hunk_sizes_t
        CATCH_REQUIRE(data[5] == 45 << 1);
        CATCH_REQUIRE(data[6] == 0);
        CATCH_REQUIRE(data[7] == 0);

        CATCH_REQUIRE(data[8] == 'm');    // name
        CATCH_REQUIRE(data[9] == 'e');
        CATCH_REQUIRE(data[10] == 's');
        CATCH_REQUIRE(data[11] == 's');
        CATCH_REQUIRE(data[12] == 'a');
        CATCH_REQUIRE(data[13] == 'g');
        CATCH_REQUIRE(data[14] == 'e');

        struct processor
        {
            static bool process_hunk(
                          brs::deserializer<std::stringstream> & in
                        , brs::field_t const & field
                        , std::string message)
            {
                CATCH_REQUIRE(field.f_name == "message");
                CATCH_REQUIRE(field.f_sub_name.empty());
                CATCH_REQUIRE(field.f_index == -1);
                CATCH_REQUIRE(field.f_size == 45);

                std::string value;
                in.read_data(value);
                CATCH_REQUIRE(value == message);

                return true;
            }
        };

        // Note: you don't usually end up re-using the serializer buffer
        //       but here it's practical
        //
        buffer.clear();

        brs::deserializer in(buffer);

        // WARNING: we want to use CATCH_...() macros inside the callback
        //          so make sure not to use one around unserialize_buffer().
        //
        brs::deserializer<std::stringstream>::process_hunk_t func(std::bind(
                      &processor::process_hunk
                    , std::placeholders::_1
                    , std::placeholders::_2
                    , message));
        bool const r(in.deserialize(func));
        CATCH_REQUIRE(r);
    }

    CATCH_SECTION("push/restore array (varying name)")
    {
        std::stringstream buffer;
        brs::serializer out(buffer);

        std::string data(buffer.str());
        CATCH_REQUIRE(data.length() == sizeof(brs::magic_t));

        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == brs::BRS_VERSION);

        std::vector<int> order;
        typedef std::map<int, std::string> value_t;
        value_t values;
        int index(-1);
        for(int i(0); i < 25; ++i)
        {
            for(;;)
            {
                index = rand() % 256;
                if(values.find(index) == values.end())
                {
                    break;
                }
            }
            std::string str;
            int max(rand() % 25 + 1);
            for(int j(0); j < max; ++j)
            {
                str += ' ' + rand() % ('~' + 1 - ' '); // ASCII except controls
            }
            values[index] = str;
            order.push_back(index);

            out.add_value("str" + std::to_string(index), index, str);
        }

        // make sure it did not get smashed
        data = buffer.str();
        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == brs::BRS_VERSION);

        struct processor
        {
            static bool process_hunk(
                          brs::deserializer<std::stringstream> & in
                        , brs::field_t const & field
                        , value_t & values)
            {
                CATCH_REQUIRE(field.f_sub_name.empty());
                CATCH_REQUIRE(field.f_index != -1);

                std::string expected_name("str" + std::to_string(field.f_index));
                CATCH_REQUIRE(field.f_name == expected_name);

                CATCH_REQUIRE(field.f_size == values[field.f_index].length());

                std::string value;
                in.read_data(value);
                CATCH_REQUIRE(value == values[field.f_index]);

                return true;
            }
        };

        // Note: you don't usually end up re-using the serializer buffer
        //       but here it's practical
        //
        buffer.clear();

        brs::deserializer in(buffer);

        // WARNING: we want to use CATCH_...() macros inside the callback
        //          so make sure not to use one around unserialize_buffer().
        //
        brs::deserializer<std::stringstream>::process_hunk_t func(std::bind(
                      &processor::process_hunk
                    , std::placeholders::_1
                    , std::placeholders::_2
                    , values));
        bool const r(in.deserialize(func));
        CATCH_REQUIRE(r);
    }

    CATCH_SECTION("push/restore array (same name)")
    {
        std::stringstream buffer;
        brs::serializer out(buffer);

        std::string data(buffer.str());
        CATCH_REQUIRE(data.length() == sizeof(brs::magic_t));

        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == brs::BRS_VERSION);

        // order does not matter and we can have gaps, to test that, create
        // a map of a few values and corresponding strings
        //
        std::vector<int> order;
        typedef std::map<int, std::string> value_t;
        value_t values;
        int index(-1);
        for(int i(0); i < 25; ++i)
        {
            for(;;)
            {
                index = rand() % 256;
                if(values.find(index) == values.end())
                {
                    break;
                }
            }
            std::string str;
            int max(rand() % 25 + 1);
            for(int j(0); j < max; ++j)
            {
                str += ' ' + rand() % ('~' + 1 - ' '); // ASCII except controls
            }
            values[index] = str;
            order.push_back(index);

            out.add_value("unique", index, str);
        }

        // make sure it did not get smashed
        data = buffer.str();
        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == brs::BRS_VERSION);

        struct processor
        {
            static bool process_hunk(
                          brs::deserializer<std::stringstream> & in
                        , brs::field_t const & field
                        , value_t & values)
            {
                CATCH_REQUIRE(field.f_name == "unique");
                CATCH_REQUIRE(field.f_sub_name.empty());
                CATCH_REQUIRE(field.f_index != -1);

                CATCH_REQUIRE(field.f_size == values[field.f_index].length());

                std::string value;
                in.read_data(value);
                CATCH_REQUIRE(value == values[field.f_index]);

                return true;
            }
        };

        // Note: you don't usually end up re-using the serializer buffer
        //       but here it's practical
        //
        buffer.clear();

        brs::deserializer in(buffer);

        // WARNING: we want to use CATCH_...() macros inside the callback
        //          so make sure not to use one around unserialize_buffer().
        //
        brs::deserializer<std::stringstream>::process_hunk_t func(std::bind(
                      &processor::process_hunk
                    , std::placeholders::_1
                    , std::placeholders::_2
                    , values));
        bool const r(in.deserialize(func));
        CATCH_REQUIRE(r);
    }

    CATCH_SECTION("push/restore map")
    {
        std::stringstream buffer;
        brs::serializer out(buffer);

        std::string data(buffer.str());
        CATCH_REQUIRE(data.length() == sizeof(brs::magic_t));

        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == brs::BRS_VERSION);

        // order does not matter and we can have gaps, to test that, create
        // a map of a few values and corresponding strings
        //
        std::vector<std::string> order;
        typedef std::map<std::string, std::string> value_t;
        value_t values;
        for(int i(0); i < 25; ++i)
        {
            std::string index;
            int max(rand() % 25 + 1);
            for(int j(0); j < max; ++j)
            {
                index += ' ' + rand() % ('~' + 1 - ' '); // ASCII except controls
            }

            std::string str;
            max = rand() % 25 + 1;
            for(int j(0); j < max; ++j)
            {
                str += ' ' + rand() % ('~' + 1 - ' '); // ASCII except controls
            }

            values[index] = str;
            order.push_back(index);

            out.add_value("mapping", index, str);
        }
//{
//std::ofstream p("t1.txt");
//std::string s(buffer.str());
//p << s;
//}

        // make sure it did not get smashed
        data = buffer.str();
        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == brs::BRS_VERSION);

        struct processor
        {
            static bool process_hunk(
                          brs::deserializer<std::stringstream> & in
                        , brs::field_t const & field
                        , value_t & values)
            {
                CATCH_REQUIRE(field.f_name == "mapping");
                CATCH_REQUIRE_FALSE(field.f_sub_name.empty());
                CATCH_REQUIRE(field.f_index == -1);

                CATCH_REQUIRE(field.f_size == values[field.f_sub_name].length());

                std::string value;
                in.read_data(value);
                CATCH_REQUIRE(value == values[field.f_sub_name]);

                return true;
            }
        };

        // Note: you don't usually end up re-using the serializer buffer
        //       but here it's practical
        //
        buffer.clear();

        brs::deserializer in(buffer);

        // WARNING: we want to use CATCH_...() macros inside the callback
        //          so make sure not to use one around unserialize_buffer().
        //
        brs::deserializer<std::stringstream>::process_hunk_t func(std::bind(
                      &processor::process_hunk
                    , std::placeholders::_1
                    , std::placeholders::_2
                    , values));
        bool const r(in.deserialize(func));
        CATCH_REQUIRE(r);
    }

    CATCH_SECTION("push/restore recursive")
    {
        class t1
        {
        public:
            void serialize(brs::serializer<std::stringstream> & out) const
            {
                out.add_value("name", f_name);
            }

            bool process_hunk(
                  brs::deserializer<std::stringstream> & in
                , brs::field_t const & field)
            {
                CATCH_REQUIRE(field.f_sub_name.empty());
                CATCH_REQUIRE(field.f_index == -1);

                if(field.f_name == "name")
                {
                    std::string value;
                    in.read_data(value);
                    CATCH_REQUIRE(f_name == value);
                }
                else
                {
                    CATCH_REQUIRE(field.f_name == "?unknown?");
                }

                return true;
            }

            void set_name(std::string const & name)
            {
                f_name = name;
            }

        private:
            std::string     f_name = "--undefined--";
        };

        class t2
        {
        public:
            void serialize(brs::serializer<std::stringstream> & out) const
            {
                for(std::size_t idx(0); idx < std::size(f_sizes); ++idx)
                {
                    out.add_value("size", idx, f_sizes[idx]);
                }
            }

            bool process_hunk(
                  brs::deserializer<std::stringstream> & in
                , brs::field_t const & field)
            {
                if(field.f_name == "size")
                {
                    CATCH_REQUIRE(static_cast<std::size_t>(field.f_index) < std::size(f_sizes));

                    std::int32_t value;
                    in.read_data(value);
                    CATCH_REQUIRE(value == f_sizes[field.f_index]);
                }
                else
                {
                    CATCH_REQUIRE(field.f_name == "?unknown?");
                }

                return true;
            }

        private:
            std::int32_t    f_sizes[10] = {
                    rand(), rand(), rand(), rand(), rand(),
                    rand(), rand(), rand(), rand(), rand(),
                };
        };

        // c includes an array of t1's and one t2
        class c
        {
        public:
            c()
            {
                int max(rand() % 5 + 3);
                for(int idx(0); idx < max; ++idx)
                {
                    std::string name;
                    int len(rand() % 25 + 10);
                    for(int j(0); j < len; ++j)
                    {
                        name += 'a' + rand() % 26;
                    }
                    t1 t;
                    t.set_name(name);
                    f_t1.push_back(t);
                }
            }

            void serialize(brs::serializer<std::stringstream> & out) const
            {
                out.add_value("count", f_count);

                int const max(static_cast<int>(f_t1.size()));
                for(int idx(0); idx < max; ++idx)
                {
                    brs::recursive r(out, "t1_array");
                    f_t1[idx].serialize(out);
                }

                {
                    brs::recursive r(out, "t2");
                    f_t2.serialize(out);
                }
            }

            bool process_hunk(
                  brs::deserializer<std::stringstream> & in
                , brs::field_t const & field)
            {
                if(field.f_name == "count")
                {
                    std::int32_t value;
                    in.read_data(value);
                    CATCH_REQUIRE(f_count == value);
                }
                else if(field.f_name == "t1_array")
                {
                    CATCH_REQUIRE(static_cast<std::size_t>(field.f_index) < f_t1.size());

                    brs::deserializer<std::stringstream>::process_hunk_t func(std::bind(
                                  &t1::process_hunk
                                , &f_t1[field.f_index]
                                , std::placeholders::_1
                                , std::placeholders::_2));
                    bool const r(in.deserialize(func));
                    CATCH_REQUIRE(r);
                }
                else if(field.f_name == "t2")
                {
                    brs::deserializer<std::stringstream>::process_hunk_t func(std::bind(
                                  &t2::process_hunk
                                , &f_t2
                                , std::placeholders::_1
                                , std::placeholders::_2));
                    bool const r(in.deserialize(func));
                    CATCH_REQUIRE(r);
                }
                else
                {
                    CATCH_REQUIRE(field.f_name == "?unknown?");
                }

                return true;
            }

        private:
            int                 f_count = rand();
            std::vector<t1>     f_t1 = std::vector<t1>();
            t2                  f_t2 = t2();
        };

        c o;

        std::stringstream buffer;
        brs::serializer out(buffer);

        std::string data(buffer.str());
        CATCH_REQUIRE(data.length() == sizeof(brs::magic_t));

        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == brs::BRS_VERSION);

        // Note: you don't usually end up re-using the serializer buffer
        //       but here it's practical
        //
        buffer.clear();

        brs::deserializer in(buffer);

        // WARNING: we want to use CATCH_...() macros inside the callback
        //          so make sure not to use one around unserialize_buffer().
        //
        brs::deserializer<std::stringstream>::process_hunk_t func(std::bind(
                      &c::process_hunk
                    , &o
                    , std::placeholders::_1
                    , std::placeholders::_2));
        bool const r(in.deserialize(func));
        CATCH_REQUIRE(r);
    }
}


// vim: ts=4 sw=4 et
