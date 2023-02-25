// Copyright (c) 2011-2023  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Verify the BRS functions.
 *
 * This file implements tests to verify that the BRS functions do what
 * they are expected to do.
 */

// self
//
#include    "catch_main.h"


// snapdev
//
#include    <snapdev/brs.h>


// C++
//
#include    <fstream>




CATCH_TEST_CASE("bitfield_size", "[bitfield]")
{
    CATCH_START_SECTION("brs: push/restore char")
    {
        constexpr std::size_t const sizeof_type(SIZEOF_BITFIELD(snapdev::hunk_sizes_t, f_type));
        CATCH_REQUIRE(sizeof_type == 2);

        constexpr std::size_t const sizeof_name(SIZEOF_BITFIELD(snapdev::hunk_sizes_t, f_name));
        CATCH_REQUIRE(sizeof_name == 7);

        constexpr std::size_t const sizeof_hunk(SIZEOF_BITFIELD(snapdev::hunk_sizes_t, f_hunk));
        CATCH_REQUIRE(sizeof_hunk == 23);
    }
    CATCH_END_SECTION()
}


CATCH_TEST_CASE("basic_types", "[basic]")
{
    CATCH_START_SECTION("brs: push/restore char")
    {
        std::stringstream buffer;
        snapdev::serializer out(buffer);

        std::string data(buffer.str());
        CATCH_REQUIRE(data.length() == sizeof(snapdev::magic_t));

        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == snapdev::BRS_VERSION);

        char value = 33;
        out.add_value("orange", value);

        CATCH_REQUIRE_THROWS_MATCHES(
                  out.add_value(std::string(), &value, sizeof(value))
                , snapdev::brs_cannot_be_empty
                , Catch::Matchers::ExceptionMessage(
                          "brs_error: name cannot be an empty string"));

        // make sure it did not get smashed
        data = buffer.str();
        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == snapdev::BRS_VERSION);

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
                          snapdev::deserializer<std::stringstream> & in
                        , snapdev::field_t const & field)
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

        snapdev::deserializer in(buffer);

        // WARNING: we want to use CATCH_...() macros inside the callback
        //          so make sure not to use one around unserialize_buffer().
        //
        snapdev::deserializer<std::stringstream>::process_hunk_t func(std::bind(
                      &processor::process_hunk
                    , std::placeholders::_1
                    , std::placeholders::_2));
        bool const r(in.deserialize(func));
        CATCH_REQUIRE(r);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("brs: push/restore signed char")
    {
        std::stringstream buffer;
        snapdev::serializer out(buffer);

        std::string data(buffer.str());
        CATCH_REQUIRE(data.length() == sizeof(snapdev::magic_t));

        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == snapdev::BRS_VERSION);

        signed char value = -43;
        out.add_value("orange", value);

        // make sure it did not get smashed
        data = buffer.str();
        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == snapdev::BRS_VERSION);

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
                          snapdev::deserializer<std::stringstream> & in
                        , snapdev::field_t const & field)
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

        snapdev::deserializer in(buffer);

        // WARNING: we want to use CATCH_...() macros inside the callback
        //          so make sure not to use one around unserialize_buffer().
        //
        snapdev::deserializer<std::stringstream>::process_hunk_t func(std::bind(
                      &processor::process_hunk
                    , std::placeholders::_1
                    , std::placeholders::_2));
        bool const r(in.deserialize(func));
        CATCH_REQUIRE(r);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("brs: push/restore unsigned char")
    {
        std::stringstream buffer;
        snapdev::serializer out(buffer);

        std::string data(buffer.str());
        CATCH_REQUIRE(data.length() == sizeof(snapdev::magic_t));

        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == snapdev::BRS_VERSION);

        unsigned char value = 200;
        out.add_value("orange", value);

        // make sure it did not get smashed
        data = buffer.str();
        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == snapdev::BRS_VERSION);

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
                          snapdev::deserializer<std::stringstream> & in
                        , snapdev::field_t const & field)
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

        snapdev::deserializer in(buffer);

        // WARNING: we want to use CATCH_...() macros inside the callback
        //          so make sure not to use one around unserialize_buffer().
        //
        snapdev::deserializer<std::stringstream>::process_hunk_t func(std::bind(
                      &processor::process_hunk
                    , std::placeholders::_1
                    , std::placeholders::_2));
        bool const r(in.deserialize(func));
        CATCH_REQUIRE(r);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("brs: push/restore shorts (16 bits)")
    {
        std::stringstream buffer;
        snapdev::serializer out(buffer);

        std::string data(buffer.str());
        CATCH_REQUIRE(data.length() == sizeof(snapdev::magic_t));

        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == snapdev::BRS_VERSION);

        std::int16_t const purple = 3003;
        out.add_value("purple", purple);

        std::uint16_t const black = 65001;
        out.add_value("black", black);

        // make sure it did not get smashed
        data = buffer.str();
        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == snapdev::BRS_VERSION);

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
                          snapdev::deserializer<std::stringstream> & in
                        , snapdev::field_t const & field
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

        snapdev::deserializer in(buffer);

        // WARNING: we want to use CATCH_...() macros inside the callback
        //          so make sure not to use one around unserialize_buffer().
        //
        snapdev::deserializer<std::stringstream>::process_hunk_t func(std::bind(
                      &processor::process_hunk
                    , std::placeholders::_1
                    , std::placeholders::_2
                    , purple
                    , black));
        bool const r(in.deserialize(func));
        CATCH_REQUIRE(r);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("brs: push/restore ints (32 bits)")
    {
        std::stringstream buffer;
        snapdev::serializer out(buffer);

        std::string data(buffer.str());
        CATCH_REQUIRE(data.length() == sizeof(snapdev::magic_t));

        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == snapdev::BRS_VERSION);

        std::int32_t const red = static_cast<std::int32_t>(SNAP_CATCH2_NAMESPACE::rand_int64());
        out.add_value("red", red);

        std::uint32_t const blue = static_cast<std::int32_t>(SNAP_CATCH2_NAMESPACE::rand_int64());
        out.add_value("blue", blue);

        // make sure it did not get smashed
        data = buffer.str();
        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == snapdev::BRS_VERSION);

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
                          snapdev::deserializer<std::stringstream> & in
                        , snapdev::field_t const & field
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

        snapdev::deserializer in(buffer);

        // WARNING: we want to use CATCH_...() macros inside the callback
        //          so make sure not to use one around unserialize_buffer().
        //
        snapdev::deserializer<std::stringstream>::process_hunk_t func(std::bind(
                      &processor::process_hunk
                    , std::placeholders::_1
                    , std::placeholders::_2
                    , red
                    , blue));
        bool const r(in.deserialize(func));
        CATCH_REQUIRE(r);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("brs: push/restore ints (64 bits)")
    {
        std::stringstream buffer;
        snapdev::serializer out(buffer);

        std::string data(buffer.str());
        CATCH_REQUIRE(data.length() == sizeof(snapdev::magic_t));

        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == snapdev::BRS_VERSION);

        std::int64_t const white = static_cast<std::int32_t>(SNAP_CATCH2_NAMESPACE::rand_int64());
        out.add_value("white", white);

        std::uint64_t const gray = static_cast<std::int32_t>(SNAP_CATCH2_NAMESPACE::rand_int64());
        out.add_value("gray", gray);

        // make sure it did not get smashed
        data = buffer.str();
        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == snapdev::BRS_VERSION);

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
                          snapdev::deserializer<std::stringstream> & in
                        , snapdev::field_t const & field
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

        snapdev::deserializer in(buffer);

        // WARNING: we want to use CATCH_...() macros inside the callback
        //          so make sure not to use one around unserialize_buffer().
        //
        snapdev::deserializer<std::stringstream>::process_hunk_t func(std::bind(
                      &processor::process_hunk
                    , std::placeholders::_1
                    , std::placeholders::_2
                    , white
                    , gray));
        bool const r(in.deserialize(func));
        CATCH_REQUIRE(r);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("brs: push/restore floats")
    {
        std::stringstream buffer;
        snapdev::serializer out(buffer);

        std::string data(buffer.str());
        CATCH_REQUIRE(data.length() == sizeof(snapdev::magic_t));

        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == snapdev::BRS_VERSION);

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
        CATCH_REQUIRE(data[3] == snapdev::BRS_VERSION);

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
                          snapdev::deserializer<std::stringstream> & in
                        , snapdev::field_t const & field
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

        snapdev::deserializer in(buffer);

        // WARNING: we want to use CATCH_...() macros inside the callback
        //          so make sure not to use one around unserialize_buffer().
        //
        snapdev::deserializer<std::stringstream>::process_hunk_t func(std::bind(
                      &processor::process_hunk
                    , std::placeholders::_1
                    , std::placeholders::_2
                    , green
                    , yellow
                    , fushia));
        bool const r(in.deserialize(func));
        CATCH_REQUIRE(r);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("brs: push/restore string")
    {
        std::stringstream buffer;
        snapdev::serializer out(buffer);

        std::string data(buffer.str());
        CATCH_REQUIRE(data.length() == sizeof(snapdev::magic_t));

        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == snapdev::BRS_VERSION);

        std::string message("this is the message we are going to serialize");
        out.add_value("message",  message);

        // make sure it did not get smashed
        data = buffer.str();
        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == snapdev::BRS_VERSION);

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
                          snapdev::deserializer<std::stringstream> & in
                        , snapdev::field_t const & field
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

        snapdev::deserializer in(buffer);

        // WARNING: we want to use CATCH_...() macros inside the callback
        //          so make sure not to use one around unserialize_buffer().
        //
        snapdev::deserializer<std::stringstream>::process_hunk_t func(std::bind(
                      &processor::process_hunk
                    , std::placeholders::_1
                    , std::placeholders::_2
                    , message));
        bool const r(in.deserialize(func));
        CATCH_REQUIRE(r);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("brs: push/restore array (varying name)")
    {
        std::stringstream buffer;
        snapdev::serializer out(buffer);

        std::string data(buffer.str());
        CATCH_REQUIRE(data.length() == sizeof(snapdev::magic_t));

        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == snapdev::BRS_VERSION);

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
        CATCH_REQUIRE(data[3] == snapdev::BRS_VERSION);

        struct processor
        {
            static bool process_hunk(
                          snapdev::deserializer<std::stringstream> & in
                        , snapdev::field_t const & field
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

        snapdev::deserializer in(buffer);

        // WARNING: we want to use CATCH_...() macros inside the callback
        //          so make sure not to use one around unserialize_buffer().
        //
        snapdev::deserializer<std::stringstream>::process_hunk_t func(std::bind(
                      &processor::process_hunk
                    , std::placeholders::_1
                    , std::placeholders::_2
                    , values));
        bool const r(in.deserialize(func));
        CATCH_REQUIRE(r);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("brs: push/restore array (same name)")
    {
        std::stringstream buffer;
        snapdev::serializer out(buffer);

        std::string data(buffer.str());
        CATCH_REQUIRE(data.length() == sizeof(snapdev::magic_t));

        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == snapdev::BRS_VERSION);

        // order does not matter and we can have gaps, to test that, create
        // a map of a few values and corresponding strings
        //
        std::vector<int> order;
        typedef std::map<int, std::string> value_t;
        value_t values;
        int index(-1);
        int const size(25);
        for(int i(0); i < size; ++i)
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

            if((index & 1) != 0)
            {
                // pass as a 'char const *'
                out.add_value("unique", index, str.c_str());
            }
            else
            {
                // pass as an std::string
                out.add_value("unique", index, str);
            }

            // nullptr does nothing
            out.add_value("unique", index + size, nullptr);
        }

        // make sure it did not get smashed
        data = buffer.str();
        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == snapdev::BRS_VERSION);

        struct processor
        {
            static bool process_hunk(
                          snapdev::deserializer<std::stringstream> & in
                        , snapdev::field_t const & field
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

        snapdev::deserializer in(buffer);

        // WARNING: we want to use CATCH_...() macros inside the callback
        //          so make sure not to use one around unserialize_buffer().
        //
        snapdev::deserializer<std::stringstream>::process_hunk_t func(std::bind(
                      &processor::process_hunk
                    , std::placeholders::_1
                    , std::placeholders::_2
                    , values));
        bool const r(in.deserialize(func));
        CATCH_REQUIRE(r);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("brs: push/restore map of strings")
    {
        std::stringstream buffer;
        snapdev::serializer out(buffer);

        std::string data(buffer.str());
        CATCH_REQUIRE(data.length() == sizeof(snapdev::magic_t));

        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == snapdev::BRS_VERSION);

        // order does not matter and we can have gaps, to test that, create
        // a map of a few values and corresponding strings
        //
        std::vector<std::string> order;
        typedef std::map<std::string, std::string> value_t;
        value_t values;
        int empty(rand() % 25);
        for(int i(0); i < 25; ++i)
        {
            int max(0);
            std::string index;
            for(;;)
            {
                max = rand() % 25 + 1;
                for(int j(0); j < max; ++j)
                {
                    index += ' ' + rand() % ('~' + 1 - ' '); // ASCII except controls
                }
                if(values.find(index) == values.end())
                {
                    // index is unique, use it
                    break;
                }
            }

            std::string str;
            if(i != empty)
            {
                max = rand() % 25;
                for(int j(0); j < max; ++j)
                {
                    str += ' ' + rand() % ('~' + 1 - ' '); // ASCII except controls
                }
            }

            values[index] = str;
            order.push_back(index);

            if((i & 1) == 0)
            {
                if(str.empty()
                || (rand() & 1) != 0)
                {
                    out.add_value("mapping", index, str);
                }
                else
                {
                    out.add_value_if_not_empty("mapping", index, str);
                }
            }
            else
            {
                // bare string
                //
                out.add_value("mapping", index, str.c_str());
            }

            out.add_value("mapping", index + "a", nullptr);
        }
        out.add_value_if_not_empty("mapping", "last-index", std::string());

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
        CATCH_REQUIRE(data[3] == snapdev::BRS_VERSION);

        struct processor
        {
            static bool process_hunk(
                          snapdev::deserializer<std::stringstream> & in
                        , snapdev::field_t const & field
                        , value_t & values)
            {
                CATCH_REQUIRE(field.f_name == "mapping");
                CATCH_REQUIRE_FALSE(field.f_sub_name.empty());
                CATCH_REQUIRE(field.f_index == -1);
                CATCH_REQUIRE(values.find(field.f_sub_name) != values.end());

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

        snapdev::deserializer in(buffer);

        // WARNING: we want to use CATCH_...() macros inside the callback
        //          so make sure not to use one around unserialize_buffer().
        //
        snapdev::deserializer<std::stringstream>::process_hunk_t func(std::bind(
                      &processor::process_hunk
                    , std::placeholders::_1
                    , std::placeholders::_2
                    , values));
        bool const r(in.deserialize(func));
        CATCH_REQUIRE(r);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("brs: push/restore map of struct")
    {
        struct data_t
        {
            std::uint32_t   f_color = 0xaa648b;
            float           f_height = 1.3f;
            std::uint8_t    f_status = 3;
            double          f_width = 3.7;

            bool operator == (data_t const & rhs) const
            {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
                return f_color == rhs.f_color
                    && f_height == rhs.f_height
                    && f_status == rhs.f_status
                    && f_width == rhs.f_width;
#pragma GCC diagnostic pop
            }
        };

        std::stringstream buffer;
        snapdev::serializer out(buffer);

        std::string data(buffer.str());
        CATCH_REQUIRE(data.length() == sizeof(snapdev::magic_t));

        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == snapdev::BRS_VERSION);

        // order does not matter and we can have gaps, to test that, create
        // a map of a few values and corresponding strings
        //
        std::vector<std::string> order;
        typedef std::map<std::string, data_t> value_t;
        value_t values;
        std::size_t const count(rand() % 100 + 25);
        for(std::size_t i(0); i < count; ++i)
        {
            std::string index;
            for(;;)
            {
                int const max(rand() % 25 + 1);
                for(int j(0); j < max; ++j)
                {
                    index += ' ' + rand() % ('~' + 1 - ' '); // ASCII except controls
                }
                if(values.find(index) == values.end())
                {
                    // index is unique, use it
                    break;
                }
            }

            data_t my_data;
            my_data.f_color = rand(),
            my_data.f_height = rand(),
            my_data.f_status = rand(),
            my_data.f_width = rand(),

            values[index] = my_data;
            order.push_back(index);

            out.add_value("set", index, my_data);
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
        CATCH_REQUIRE(data[3] == snapdev::BRS_VERSION);

        struct processor
        {
            static bool process_hunk(
                          snapdev::deserializer<std::stringstream> & in
                        , snapdev::field_t const & field
                        , value_t & values)
            {
                CATCH_REQUIRE(field.f_name == "set");
                CATCH_REQUIRE_FALSE(field.f_sub_name.empty());
                CATCH_REQUIRE(field.f_index == -1);
                CATCH_REQUIRE(values.find(field.f_sub_name) != values.end());

                CATCH_REQUIRE(field.f_size == sizeof(data_t));

                data_t value;
                in.read_data(value);
                data_t expected(values[field.f_sub_name]);
                CATCH_REQUIRE(value == expected);

                return true;
            }
        };

        // Note: you don't usually end up re-using the serializer buffer
        //       but here it's practical
        //
        buffer.clear();

        snapdev::deserializer in(buffer);

        // WARNING: we want to use CATCH_...() macros inside the callback
        //          so make sure not to use one around unserialize_buffer().
        //
        snapdev::deserializer<std::stringstream>::process_hunk_t func(std::bind(
                      &processor::process_hunk
                    , std::placeholders::_1
                    , std::placeholders::_2
                    , values));
        bool const r(in.deserialize(func));
        CATCH_REQUIRE(r);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("brs: push/restore recursive")
    {
        class t1
        {
        public:
            void serialize(snapdev::serializer<std::stringstream> & out) const
            {
                out.add_value("name", f_name);
            }

            bool process_hunk(
                  snapdev::deserializer<std::stringstream> & in
                , snapdev::field_t const & field)
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
            void serialize(snapdev::serializer<std::stringstream> & out) const
            {
                for(std::size_t idx(0); idx < std::size(f_sizes); ++idx)
                {
                    out.add_value("size", idx, f_sizes[idx]);
                }
            }

            bool process_hunk(
                  snapdev::deserializer<std::stringstream> & in
                , snapdev::field_t const & field)
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
                int const max(rand() % 5 + 3);
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

                int const array_max(rand() % 90 + 10);
                for(int idx(0); idx < array_max; ++idx)
                {
                    float nominator(static_cast<float>(rand()));
                    float denominator(0.0f);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
                    while(denominator == 0.0f)
                    {
                        denominator = static_cast<float>(rand());
                    }
#pragma GCC diagnostic pop
                    f_t3.push_back(nominator / denominator);
                }
            }

            void serialize(snapdev::serializer<std::stringstream> & out) const
            {
                out.add_value("count", f_count);
                out.add_value("age", f_age);

                {
                    int const max(static_cast<int>(f_t1.size()));
                    for(int idx(0); idx < max; ++idx)
                    {
                        snapdev::recursive r(out, "t1_array");
                        f_t1[idx].serialize(out);
                    }
                }

                out.add_value("float-array", f_t3);

                {
                    snapdev::recursive r(out, "t2");
                    f_t2.serialize(out);
                }

                out.add_value("flower-color", std::string("bluish"));
                out.add_value("flower-height", std::string());  // unknown
                out.add_value_if_not_empty("flower-name", std::string());  // unknown -- not added

                out.add_value("dog-color", "red");
                out.add_value("dog-height", "");  // unknown
                out.add_value_if_not_empty("dog-name", "");  // unknown -- not added
                out.add_value("dog-nothing", nullptr);  // unknown -- not added
                out.add_value_if_not_empty("dog-eye-color", "green");
            }

            bool process_hunk(
                  snapdev::deserializer<std::stringstream> & in
                , snapdev::field_t const & field)
            {
                if(field.f_name == "count")
                {
                    int value;
                    in.read_data(value);
                    CATCH_REQUIRE(f_count == value);
                }
                else if(field.f_name == "age")
                {
                    int value;
                    in.read_data(value);
                    CATCH_REQUIRE(f_age == value);
                }
                else if(field.f_name == "t1_array")
                {
                    // this is a set of sub-classes so we do not have an index
                    //
                    CATCH_REQUIRE(field.f_index == -1);
                    CATCH_REQUIRE(field.f_sub_name == std::string());

                    // in a normal implementation, here we would create a new
                    // item and append it if the load succeeds; the test will
                    // instead verify that the f_t1_index is still valid and
                    // test load that next f_t1 object
                    //
                    CATCH_REQUIRE(f_t1_index < f_t1.size());

                    snapdev::deserializer<std::stringstream>::process_hunk_t func(std::bind(
                                  &t1::process_hunk
                                , &f_t1[f_t1_index]
                                , std::placeholders::_1
                                , std::placeholders::_2));
                    bool const r(in.deserialize(func));
                    CATCH_REQUIRE(r);

                    ++f_t1_index;
                }
                else if(field.f_name == "float-array")
                {
                    CATCH_REQUIRE(field.f_index == -1);
                    CATCH_REQUIRE(field.f_sub_name == std::string());

                    std::vector<float> v;
                    in.read_data(v);
                    CATCH_REQUIRE(v.size() == f_t3.size());
                    CATCH_REQUIRE(v == f_t3);
                }
                else if(field.f_name == "t2")
                {
                    snapdev::deserializer<std::stringstream>::process_hunk_t func(std::bind(
                                  &t2::process_hunk
                                , &f_t2
                                , std::placeholders::_1
                                , std::placeholders::_2));
                    bool const r(in.deserialize(func));
                    CATCH_REQUIRE(r);
                }
                else if(field.f_name == "flower-color")
                {
                    std::string value;
                    in.read_data(value);
                    CATCH_REQUIRE(value.length() == std::string("bluish").length());
                    CATCH_REQUIRE(value == std::string("bluish"));
                }
                else if(field.f_name == "flower-height")
                {
                    std::string value;
                    in.read_data(value);
                    CATCH_REQUIRE(value == std::string());
                }
                else if(field.f_name == "flower-name")
                {
                    // this field should not be added since the value is
                    // the empty string and we on purpose avoid adding
                    // it (i.e. "add value (only) if not empty")
                    //
                    CATCH_REQUIRE(field.f_name == "not-expected");
                }
                else if(field.f_name == "dog-color")
                {
                    std::string value;
                    in.read_data(value);
                    CATCH_REQUIRE(value.length() == std::string("red").length());
                    CATCH_REQUIRE(value == std::string("red"));
                }
                else if(field.f_name == "dog-height")
                {
                    std::string value;
                    in.read_data(value);
                    CATCH_REQUIRE(value == std::string());
                }
                else if(field.f_name == "dog-name"
                     || field.f_name == "dog-nothing")
                {
                    // this field should not be added since the value is
                    // the empty string and we on purpose avoid adding
                    // it (i.e. "add value (only) if not empty")
                    //
                    CATCH_REQUIRE(field.f_name == "not-expected");
                }
                else if(field.f_name == "dog-eye-color")
                {
                    std::string value;
                    in.read_data(value);
                    CATCH_REQUIRE(value == std::string("green"));
                }
                else
                {
                    CATCH_REQUIRE(field.f_name == "?unknown?");
                }

                return true;
            }

        private:
            int                 f_count = rand();
            int                 f_age = rand() % 100;
            std::vector<t1>     f_t1 = std::vector<t1>();
            t2                  f_t2 = t2();
            std::vector<float>  f_t3 = {};
            std::size_t         f_t1_index = 0;
        };

        c o;

        std::stringstream buffer;
        snapdev::serializer out(buffer);

        std::string data(buffer.str());
        CATCH_REQUIRE(data.length() == sizeof(snapdev::magic_t));

        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == snapdev::BRS_VERSION);

        o.serialize(out);

        // Note: you don't usually end up re-using the serializer buffer
        //       but here it's practical
        //
        buffer.clear();

        snapdev::deserializer in(buffer);

        // WARNING: we want to use CATCH_...() macros inside the callback
        //          so make sure not to use one around unserialize_buffer().
        //
        snapdev::deserializer<std::stringstream>::process_hunk_t func(std::bind(
                      &c::process_hunk
                    , &o
                    , std::placeholders::_1
                    , std::placeholders::_2));
        bool const r(in.deserialize(func));
        CATCH_REQUIRE(r);
    }
    CATCH_END_SECTION()
}


CATCH_TEST_CASE("brs_invalid", "[basic]")
{
    CATCH_START_SECTION("brs: name missing")
    {
        std::stringstream buffer;
        snapdev::serializer out(buffer);

        std::string data(buffer.str());
        CATCH_REQUIRE(data.length() == sizeof(snapdev::magic_t));

        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == snapdev::BRS_VERSION);

        char value = 33;

        // FIELD
        CATCH_REQUIRE_THROWS_MATCHES(
                  out.add_value(std::string(), &value, sizeof(value))
                , snapdev::brs_cannot_be_empty
                , Catch::Matchers::ExceptionMessage(
                          "brs_error: name cannot be an empty string"));

        // ARRAY
        CATCH_REQUIRE_THROWS_MATCHES(
                  out.add_value(std::string(), 5, &value, sizeof(value))
                , snapdev::brs_cannot_be_empty
                , Catch::Matchers::ExceptionMessage(
                          "brs_error: name cannot be an empty string"));

        // MAP (main name)
        CATCH_REQUIRE_THROWS_MATCHES(
                  out.add_value(std::string(), std::string(), &value, sizeof(value))
                , snapdev::brs_cannot_be_empty
                , Catch::Matchers::ExceptionMessage(
                          "brs_error: name cannot be an empty string"));

        // MAP (sub-name)
        CATCH_REQUIRE_THROWS_MATCHES(
                  out.add_value("good-name", std::string(), &value, sizeof(value))
                , snapdev::brs_cannot_be_empty
                , Catch::Matchers::ExceptionMessage(
                          "brs_error: sub-name cannot be an empty string"));

        // SUB-FIELD
        CATCH_REQUIRE_THROWS_MATCHES(
                  out.start_subfield(std::string())
                , snapdev::brs_cannot_be_empty
                , Catch::Matchers::ExceptionMessage(
                          "brs_error: name cannot be an empty string"));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("brs: name too long")
    {
        std::stringstream buffer;
        snapdev::serializer out(buffer);

        std::string data(buffer.str());
        CATCH_REQUIRE(data.length() == sizeof(snapdev::magic_t));

        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == snapdev::BRS_VERSION);

        std::string name;
        std::size_t count(1 << SIZEOF_BITFIELD(snapdev::hunk_sizes_t, f_name));
        std::size_t const max(count + rand() % 200);
        for(std::size_t idx(0); idx < max; ++idx)
        {
            name += rand() % 26 + 'a';
        }

        char value = 33;

        // FIELD
        CATCH_REQUIRE_THROWS_MATCHES(
                  out.add_value(name, &value, sizeof(value))
                , snapdev::brs_out_of_range
                , Catch::Matchers::ExceptionMessage(
                          "brs_out_of_range: name or hunk too large"));

        // ARRAY
        CATCH_REQUIRE_THROWS_MATCHES(
                  out.add_value(name, 17, &value, sizeof(value))
                , snapdev::brs_out_of_range
                , Catch::Matchers::ExceptionMessage(
                          "brs_out_of_range: name, index, or hunk too large"));

        // MAP (main name)
        CATCH_REQUIRE_THROWS_MATCHES(
                  out.add_value(name, "name-too-long", &value, sizeof(value))
                , snapdev::brs_out_of_range
                , Catch::Matchers::ExceptionMessage(
                          "brs_out_of_range: name, sub-name, or hunk too large"));

        // MAP (sub-name)
        //
        // sub-names make use of an std::uint8_t for their size
        //
        std::string sub_name;
        std::size_t const sub_max(std::numeric_limits<std::uint8_t>::max() + 1 + rand() % 200);
        for(std::size_t idx(0); idx < sub_max; ++idx)
        {
            sub_name += rand() % 26 + 'a';
        }

        CATCH_REQUIRE_THROWS_MATCHES(
                  out.add_value("sub-name-too-long", sub_name, &value, sizeof(value))
                , snapdev::brs_out_of_range
                , Catch::Matchers::ExceptionMessage(
                          "brs_out_of_range: name, sub-name, or hunk too large"));

        // SUB-FIELD
        CATCH_REQUIRE_THROWS_MATCHES(
                  out.start_subfield(name)
                , snapdev::brs_out_of_range
                , Catch::Matchers::ExceptionMessage(
                          "brs_out_of_range: name too large"));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("brs: hunk too long")
    {
        std::stringstream buffer;
        snapdev::serializer out(buffer);

        std::string data(buffer.str());
        CATCH_REQUIRE(data.length() == sizeof(snapdev::magic_t));

        CATCH_REQUIRE(data[0] == 'B');
        CATCH_REQUIRE(data[1] == 'R');
        CATCH_REQUIRE(data[2] == 'L');
        CATCH_REQUIRE(data[3] == snapdev::BRS_VERSION);

        char value = 33;
        for(std::size_t extra(0); extra < 10; ++extra)
        {
            CATCH_REQUIRE_THROWS_MATCHES(
                      out.add_value("large-hunk", &value, (1 << SIZEOF_BITFIELD(snapdev::hunk_sizes_t, f_hunk)) + extra)
                    , snapdev::brs_out_of_range
                    , Catch::Matchers::ExceptionMessage(
                              "brs_out_of_range: name or hunk too large"));

            CATCH_REQUIRE_THROWS_MATCHES(
                      out.add_value("large-hunk", 33, &value, (1 << SIZEOF_BITFIELD(snapdev::hunk_sizes_t, f_hunk)) + extra)
                    , snapdev::brs_out_of_range
                    , Catch::Matchers::ExceptionMessage(
                              "brs_out_of_range: name, index, or hunk too large"));

            CATCH_REQUIRE_THROWS_MATCHES(
                      out.add_value("large-hunk", "sub-name", &value, (1 << SIZEOF_BITFIELD(snapdev::hunk_sizes_t, f_hunk)) + extra)
                    , snapdev::brs_out_of_range
                    , Catch::Matchers::ExceptionMessage(
                              "brs_out_of_range: name, sub-name, or hunk too large"));
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("brs: empty input")
    {
        std::stringstream buffer;

        CATCH_REQUIRE_THROWS_MATCHES(
                  snapdev::deserializer<std::stringstream>(buffer)
                , snapdev::brs_magic_missing
                , Catch::Matchers::ExceptionMessage(
                          "brs_error: magic missing from the start of the buffer."));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("brs: magic unrecognized")
    {
        std::stringstream buffer;

        buffer << 'G';  // pretend it's a GIF
        buffer << 'I';
        buffer << 'F';
        buffer << '8';
        buffer << '9';
        buffer << 'a';

        CATCH_REQUIRE_THROWS_MATCHES(
                  snapdev::deserializer<std::stringstream>(buffer)
                , snapdev::brs_magic_unsupported
                , Catch::Matchers::ExceptionMessage(
                          "brs_error: magic unsupported."));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("brs: unknown hunk type")
    {
        std::stringstream buffer;

        // magic
        buffer << 'B';
        buffer << 'R';
        buffer << 'L';
        buffer << snapdev::BRS_VERSION;

        // size hunk
        // type 3 is not recognized at the moment
        std::uint32_t hunk(static_cast<std::uint32_t>((0x03 << 0) | (3 << 2) | (11 << 9)));
        buffer.write(reinterpret_cast<char const *>(&hunk), 4);

        struct processor
        {
            static bool process_hunk(
                          snapdev::deserializer<std::stringstream> & in
                        , snapdev::field_t const & field)
            {
                snapdev::NOT_USED(in, field);
                throw std::logic_error("process_hunk() was called!");
            }
        };

        snapdev::deserializer<std::stringstream> in(buffer);
        snapdev::deserializer<std::stringstream>::process_hunk_t func(std::bind(
                      &processor::process_hunk
                    , std::placeholders::_1
                    , std::placeholders::_2));

        CATCH_REQUIRE_THROWS_MATCHES(
                  in.deserialize(func)
                , snapdev::brs_unknown_type
                , Catch::Matchers::ExceptionMessage(
                          "brs_error: read a field with an unknown type."));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("brs: field missing name")
    {
        std::stringstream buffer;

        // magic
        buffer << 'B';
        buffer << 'R';
        buffer << 'L';
        buffer << snapdev::BRS_VERSION;

        // size hunk
        // type 3 is not recognized at the moment
        std::uint32_t hunk(static_cast<std::uint32_t>((snapdev::TYPE_FIELD << 0) | (3 << 2) | (11 << 9)));
        buffer.write(reinterpret_cast<char const *>(&hunk), 4);

        struct processor
        {
            static bool process_hunk(
                          snapdev::deserializer<std::stringstream> & in
                        , snapdev::field_t const & field)
            {
                snapdev::NOT_USED(in, field);
                throw std::logic_error("process_hunk() was called!");
            }
        };

        snapdev::deserializer<std::stringstream> in(buffer);
        snapdev::deserializer<std::stringstream>::process_hunk_t func(std::bind(
                      &processor::process_hunk
                    , std::placeholders::_1
                    , std::placeholders::_2));

        bool r(in.deserialize(func));
        CATCH_REQUIRE_FALSE(r);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("brs: field data mismatch reading")
    {
        std::stringstream buffer;

        // magic
        buffer << 'B';
        buffer << 'R';
        buffer << 'L';
        buffer << snapdev::BRS_VERSION;

        // size hunk
        // type 3 is not recognized at the moment
        std::uint32_t hunk(static_cast<std::uint32_t>((snapdev::TYPE_FIELD << 0) | (3 << 2) | (1 << 9)));
        buffer.write(reinterpret_cast<char const *>(&hunk), 4);
        char const * name("ABC");
        buffer.write(name, 3);
        std::uint8_t value(5);
        buffer.write(reinterpret_cast<char const *>(&value), 1);

        struct processor
        {
            static bool process_hunk(
                          snapdev::deserializer<std::stringstream> & in
                        , snapdev::field_t const & field)
            {
                CATCH_REQUIRE(field.f_name == "ABC");
                CATCH_REQUIRE(field.f_sub_name == std::string());
                CATCH_REQUIRE(field.f_index == -1);
                CATCH_REQUIRE(field.f_size == 1);

                // reading the data other than the byte creates errors
                // (incorrect size--so you may be calling the read_data()
                // function with the wrong type)
                //

                std::int16_t s;
                CATCH_REQUIRE_THROWS_MATCHES(
                          in.read_data(s)
                        , snapdev::brs_logic_error
                        , Catch::Matchers::ExceptionMessage(
                                  "brs_logic_error: hunk size is 1, but you are trying to read 2."));

                std::uint16_t us;
                CATCH_REQUIRE_THROWS_MATCHES(
                          in.read_data(us)
                        , snapdev::brs_logic_error
                        , Catch::Matchers::ExceptionMessage(
                                  "brs_logic_error: hunk size is 1, but you are trying to read 2."));

                std::int32_t i;
                CATCH_REQUIRE_THROWS_MATCHES(
                          in.read_data(i)
                        , snapdev::brs_logic_error
                        , Catch::Matchers::ExceptionMessage(
                                  "brs_logic_error: hunk size is 1, but you are trying to read 4."));

                std::uint32_t ui;
                CATCH_REQUIRE_THROWS_MATCHES(
                          in.read_data(ui)
                        , snapdev::brs_logic_error
                        , Catch::Matchers::ExceptionMessage(
                                  "brs_logic_error: hunk size is 1, but you are trying to read 4."));

                std::int64_t l;
                CATCH_REQUIRE_THROWS_MATCHES(
                          in.read_data(l)
                        , snapdev::brs_logic_error
                        , Catch::Matchers::ExceptionMessage(
                                  "brs_logic_error: hunk size is 1, but you are trying to read 8."));

                std::uint64_t ul;
                CATCH_REQUIRE_THROWS_MATCHES(
                          in.read_data(ul)
                        , snapdev::brs_logic_error
                        , Catch::Matchers::ExceptionMessage(
                                  "brs_logic_error: hunk size is 1, but you are trying to read 8."));

                float f;
                CATCH_REQUIRE_THROWS_MATCHES(
                          in.read_data(f)
                        , snapdev::brs_logic_error
                        , Catch::Matchers::ExceptionMessage(
                                  "brs_logic_error: hunk size is 1, but you are trying to read 4."));

                double d;
                CATCH_REQUIRE_THROWS_MATCHES(
                          in.read_data(d)
                        , snapdev::brs_logic_error
                        , Catch::Matchers::ExceptionMessage(
                                  "brs_logic_error: hunk size is 1, but you are trying to read 8."));

                long double ld;
                CATCH_REQUIRE_THROWS_MATCHES(
                          in.read_data(ld)
                        , snapdev::brs_logic_error
                        , Catch::Matchers::ExceptionMessage(
                                  "brs_logic_error: hunk size is 1, but you are trying to read 16."));

                std::vector<std::int64_t> vl;
                CATCH_REQUIRE_THROWS_MATCHES(
                          in.read_data(vl)
                        , snapdev::brs_logic_error
                        , Catch::Matchers::ExceptionMessage(
                                  "brs_logic_error: hunk size (1) is not a multiple of the vector item size: 8."));

                std::uint8_t b(0);
                CATCH_REQUIRE(in.read_data(b));
                CATCH_REQUIRE(b == 5);

                return true;
            }
        };

        snapdev::deserializer<std::stringstream> in(buffer);
        snapdev::deserializer<std::stringstream>::process_hunk_t func(std::bind(
                      &processor::process_hunk
                    , std::placeholders::_1
                    , std::placeholders::_2));

        bool r(in.deserialize(func));
        CATCH_REQUIRE(r);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("brs: missing array index")
    {
        std::stringstream buffer;

        // magic
        buffer << 'B';
        buffer << 'R';
        buffer << 'L';
        buffer << snapdev::BRS_VERSION;

        // size hunk
        // type 3 is not recognized at the moment
        std::uint32_t hunk(static_cast<std::uint32_t>((snapdev::TYPE_ARRAY << 0) | (3 << 2) | (11 << 9)));
        buffer.write(reinterpret_cast<char const *>(&hunk), 4);

        struct processor
        {
            static bool process_hunk(
                          snapdev::deserializer<std::stringstream> & in
                        , snapdev::field_t const & field)
            {
                snapdev::NOT_USED(in, field);
                throw std::logic_error("process_hunk() was called!");
            }
        };

        snapdev::deserializer<std::stringstream> in(buffer);
        snapdev::deserializer<std::stringstream>::process_hunk_t func(std::bind(
                      &processor::process_hunk
                    , std::placeholders::_1
                    , std::placeholders::_2));

        bool r(in.deserialize(func));
        CATCH_REQUIRE_FALSE(r);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("brs: missing array field name")
    {
        std::stringstream buffer;

        // magic
        buffer << 'B';
        buffer << 'R';
        buffer << 'L';
        buffer << snapdev::BRS_VERSION;

        // size hunk
        // type 3 is not recognized at the moment
        std::uint32_t hunk(static_cast<std::uint32_t>((snapdev::TYPE_ARRAY << 0) | (3 << 2) | (11 << 9)));
        buffer.write(reinterpret_cast<char const *>(&hunk), 4);
        std::uint16_t size(10);
        buffer.write(reinterpret_cast<char const *>(&size), 2);

        struct processor
        {
            static bool process_hunk(
                          snapdev::deserializer<std::stringstream> & in
                        , snapdev::field_t const & field)
            {
                snapdev::NOT_USED(in, field);
                throw std::logic_error("process_hunk() was called!");
            }
        };

        snapdev::deserializer<std::stringstream> in(buffer);
        snapdev::deserializer<std::stringstream>::process_hunk_t func(std::bind(
                      &processor::process_hunk
                    , std::placeholders::_1
                    , std::placeholders::_2));

        bool r(in.deserialize(func));
        CATCH_REQUIRE_FALSE(r);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("brs: missing map sub-name length")
    {
        std::stringstream buffer;

        // magic
        buffer << 'B';
        buffer << 'R';
        buffer << 'L';
        buffer << snapdev::BRS_VERSION;

        // size hunk
        // type 3 is not recognized at the moment
        std::uint32_t hunk(static_cast<std::uint32_t>((snapdev::TYPE_MAP << 0) | (3 << 2) | (11 << 9)));
        buffer.write(reinterpret_cast<char const *>(&hunk), 4);

        struct processor
        {
            static bool process_hunk(
                          snapdev::deserializer<std::stringstream> & in
                        , snapdev::field_t const & field)
            {
                snapdev::NOT_USED(in, field);
                throw std::logic_error("process_hunk() was called!");
            }
        };

        snapdev::deserializer<std::stringstream> in(buffer);
        snapdev::deserializer<std::stringstream>::process_hunk_t func(std::bind(
                      &processor::process_hunk
                    , std::placeholders::_1
                    , std::placeholders::_2));

        bool r(in.deserialize(func));
        CATCH_REQUIRE_FALSE(r);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("brs: map sub-name length is zero")
    {
        std::stringstream buffer;

        // magic
        buffer << 'B';
        buffer << 'R';
        buffer << 'L';
        buffer << snapdev::BRS_VERSION;

        // size hunk
        // type 3 is not recognized at the moment
        std::uint32_t hunk(static_cast<std::uint32_t>((snapdev::TYPE_MAP << 0) | (3 << 2) | (11 << 9)));
        buffer.write(reinterpret_cast<char const *>(&hunk), 4);
        std::uint8_t len(0);
        buffer.write(reinterpret_cast<char const *>(&len), 1);

        struct processor
        {
            static bool process_hunk(
                          snapdev::deserializer<std::stringstream> & in
                        , snapdev::field_t const & field)
            {
                snapdev::NOT_USED(in, field);
                throw std::logic_error("process_hunk() was called!");
            }
        };

        snapdev::deserializer<std::stringstream> in(buffer);
        snapdev::deserializer<std::stringstream>::process_hunk_t func(std::bind(
                      &processor::process_hunk
                    , std::placeholders::_1
                    , std::placeholders::_2));

        CATCH_REQUIRE_THROWS_MATCHES(
                  in.deserialize(func)
                , snapdev::brs_map_name_cannot_be_empty
                , Catch::Matchers::ExceptionMessage(
                          "brs_error: the length of a map's field name cannot be zero."));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("brs: missing map sub-name")
    {
        std::stringstream buffer;

        // magic
        buffer << 'B';
        buffer << 'R';
        buffer << 'L';
        buffer << snapdev::BRS_VERSION;

        // size hunk
        // type 3 is not recognized at the moment
        std::uint32_t hunk(static_cast<std::uint32_t>((snapdev::TYPE_MAP << 0) | (3 << 2) | (11 << 9)));
        buffer.write(reinterpret_cast<char const *>(&hunk), 4);
        std::uint8_t len(10);
        buffer.write(reinterpret_cast<char const *>(&len), 1);

        struct processor
        {
            static bool process_hunk(
                          snapdev::deserializer<std::stringstream> & in
                        , snapdev::field_t const & field)
            {
                snapdev::NOT_USED(in, field);
                throw std::logic_error("process_hunk() was called!");
            }
        };

        snapdev::deserializer<std::stringstream> in(buffer);
        snapdev::deserializer<std::stringstream>::process_hunk_t func(std::bind(
                      &processor::process_hunk
                    , std::placeholders::_1
                    , std::placeholders::_2));

        bool r(in.deserialize(func));
        CATCH_REQUIRE_FALSE(r);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("brs: missing map name")
    {
        std::stringstream buffer;

        // magic
        buffer << 'B';
        buffer << 'R';
        buffer << 'L';
        buffer << snapdev::BRS_VERSION;

        // size hunk
        // type 3 is not recognized at the moment
        std::uint32_t hunk(static_cast<std::uint32_t>((snapdev::TYPE_MAP << 0) | (3 << 2) | (11 << 9)));
        buffer.write(reinterpret_cast<char const *>(&hunk), 4);
        std::uint8_t len(3);
        buffer.write(reinterpret_cast<char const *>(&len), 1);
        char const * name = "SUB";
        buffer.write(name, 3);

        struct processor
        {
            static bool process_hunk(
                          snapdev::deserializer<std::stringstream> & in
                        , snapdev::field_t const & field)
            {
                snapdev::NOT_USED(in, field);
                throw std::logic_error("process_hunk() was called!");
            }
        };

        snapdev::deserializer<std::stringstream> in(buffer);
        snapdev::deserializer<std::stringstream>::process_hunk_t func(std::bind(
                      &processor::process_hunk
                    , std::placeholders::_1
                    , std::placeholders::_2));

        bool r(in.deserialize(func));
        CATCH_REQUIRE_FALSE(r);
    }
    CATCH_END_SECTION()
}


// vim: ts=4 sw=4 et
