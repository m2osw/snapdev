// Copyright (c) 2022-2024  Made to Order Software Corp.  All Rights Reserved.
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
 * \brief Classes to serialize and deserialize data.
 *
 * This file implements a couple of classes used to serialize binary data to
 * a stream. In most cases, this is much more efficient than any other method
 * such as using XML, JSON, or even a simple .ini file.
 *
 * It is also very useful if you have data such as floating point numbers
 * which when saved in a text format can lose precision (unless you save
 * them as an integer).
 */

// libexcept
//
#include    <libexcept/exception.h>


// snapdev
//
#include    <snapdev/is_vector.h>
#include    <snapdev/not_used.h>
#include    <snapdev/sizeof_bitfield.h>


// C++
//
#include    <cstdint>
#include    <cstring>
#include    <functional>
#include    <limits>
#include    <map>
#include    <memory>
#include    <vector>



namespace snapdev
{



DECLARE_LOGIC_ERROR(brs_logic_error);

DECLARE_OUT_OF_RANGE(brs_out_of_range);

DECLARE_MAIN_EXCEPTION(brs_error);

DECLARE_EXCEPTION(brs_error, brs_cannot_be_empty);
DECLARE_EXCEPTION(brs_error, brs_magic_missing);
DECLARE_EXCEPTION(brs_error, brs_magic_unsupported);
DECLARE_EXCEPTION(brs_error, brs_map_name_cannot_be_empty);
DECLARE_EXCEPTION(brs_error, brs_unknown_type);


typedef std::uint32_t               magic_t;
typedef std::uint8_t                version_t;
typedef std::string                 name_t;

typedef std::uint32_t               type_t;

constexpr type_t const              TYPE_FIELD = 0;     // regular name=value
constexpr type_t const              TYPE_ARRAY = 1;     // item in an array (includes a 16 bit index)
constexpr type_t const              TYPE_MAP = 2;       // item in a map (includes a second name)

struct hunk_sizes_t
{
    std::uint32_t   f_type : 2;         // type of field (see TYPE_...)
    std::uint32_t   f_name : 7;         // size of this field name (1 to 127)
    std::uint32_t   f_hunk : 23;        // size of this field's data (up to 8Mb)
};


constexpr version_t const       BRS_ROOT = 0;       // indicate root buffer
constexpr version_t const       BRS_VERSION = 1;    // version of the format


constexpr magic_t build_magic(char endian)
{
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    return ('B' << 24) | ('R' << 16) | (endian <<  8) | (static_cast<unsigned char>(BRS_VERSION) <<  0);
#else
    return ('B' <<  0) | ('R' <<  8) | (endian << 16) | (static_cast<unsigned char>(BRS_VERSION) << 24);
#endif
}

constexpr magic_t const         BRS_MAGIC_BIG_ENDIAN    = build_magic('B');
constexpr magic_t const         BRS_MAGIC_LITTLE_ENDIAN = build_magic('L');

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
constexpr magic_t const         BRS_MAGIC = BRS_MAGIC_BIG_ENDIAN;
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
constexpr magic_t const         BRS_MAGIC = BRS_MAGIC_LITTLE_ENDIAN;
#else
#error "Unsupported endianess"
#endif



/** \brief Class to serialize your data.
 *
 * This class is used to serialize your data. You create a serializer and
 * then call add_value() with each one of your fields.
 *
 * The class supports named fields, fields representing arrays, and
 * fields representing maps.
 *
 * \code
 *     // create a serializer that writes to a stringstream
 *     //
 *     std::stringstream buffer;
 *     snapdev::serializer out(buffer);
 *
 *     // add a value as is; all basic types, strings, etc. are supported
 *     // in this case you pass 2 values
 *     //
 *     // note that my_value can be any basic struct type
 *     //
 *     // if you use a binary type (i.e. `std::int16_t`) then you must re-read
 *     // the value with the exact same type; if you need to change the type
 *     // later and its size is different, you can detect which type (old or
 *     // new) using the size of the field, otherwise make sure to change the
 *     // field name
 *     //
 *     out.add_value("field-name", my_value);
 *
 *     // for binary data in a buffer, pass a pointer and a size
 *     //
 *     out.add_value("field-name", my_buffer, size_of_my_buffer);
 *
 *     // add a value with an index (array)
 *     // in this case you pass 3 parameters
 *     // this also supports the buffer/size as above
 *     //
 *     out.add_value("field-name", 123, my_value);
 *     out.add_value("field-name", 123, my_buffer, size_of_my_buffer);
 *
 *     // add a value with a sub-field name (map)
 *     // in this case you pass 3 parameters, the second being a string as well
 *     // this also supports the buffer/size as above
 *     //
 *     out.add_value("field-name", "sub-field", my_value);
 *     out.add_value("field-name", "sub-field", my_buffer, size_of_my_buffer);
 *
 *     // the add to the output stream is immediate (no caching happening in
 *     // the serialize class itself)
 *     //
 *     // so to save it to a file you could do this
 *     // (of course, `out` could also just be set to that output file)
 *     //
 *     std::ofstream p("my-data.brs");
 *     p << buffer.str();
 * \endcode
 *
 * If you have a sub-class that you want to serialize within the parent,
 * you want to use the recursive class like so:
 *
 * \code
 *     out.add_value("type", f_type);     // regular field
 *
 *     // start the recursive entry
 *     {
 *         brs::recursive r(out, "headers");
 *         for(auto const & h : f_headers)
 *         {
 *             h.serialize(out);  // serialize one header
 *
 *             // for example, the header serialization may just be a name/value
 *             //     out.add_value("header:" + f_name, f_value);
 *         }
 *     }
 *     // end the recursive entry
 *
 *     out.add_value("body", f_body);     // another regular field
 * \endcode
 *
 * Place the brs::recursive inside a sub-block, that way when you hit the '}'
 * it closes the sub-field automatically. This is equivalent to calling the
 * start_subfield() and end_subfield() in a safe manner.
 *
 * \tparam S  The type of output stream to write the data to.
 */
template<typename S>
class serializer
{
public:
    static_assert(sizeof(typename S::char_type) == 1, "we only support 1 byte characters in the brs::serializer");

    /** \brief Initialize the stream with the magic header.
     *
     * This function adds the magic header at the beginning of your file.
     */
    serializer(S & output)
        : f_output(output)
    {
        magic_t const magic(BRS_MAGIC);
        f_output.write(
                  reinterpret_cast<typename S::char_type const *>(&magic)
                , sizeof(magic));
    }

    template<typename T>
    void add_value(name_t const & name, T const * ptr, std::size_t size)
    {
        if(name.length() == 0)
        {
            throw brs_cannot_be_empty("name cannot be an empty string");
        }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
        hunk_sizes_t const hunk_sizes = {
            .f_type = TYPE_FIELD,
            .f_name = static_cast<std::uint8_t>(name.length()),
            .f_hunk = static_cast<std::uint32_t>(size & ((1 << SIZEOF_BITFIELD(hunk_sizes_t, f_hunk)) - 1)),
        };
#pragma GCC diagnostic pop

        if(hunk_sizes.f_name != name.length()
        || hunk_sizes.f_hunk != size)
        {
            throw brs_out_of_range("name or hunk too large");
        }

        f_output.write(
                  reinterpret_cast<typename S::char_type const *>(&hunk_sizes)
                , sizeof(hunk_sizes));

        f_output.write(
                  reinterpret_cast<typename S::char_type const *>(name.c_str())
                , hunk_sizes.f_name);

        f_output.write(
                  reinterpret_cast<typename S::char_type const *>(ptr)
                , size);
    }


    template<typename T>
    void add_value(name_t const & name, int index, T const * ptr, std::size_t size)
    {
        if(name.length() == 0)
        {
            throw brs_cannot_be_empty("name cannot be an empty string");
        }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
        hunk_sizes_t const hunk_sizes = {
            .f_type = TYPE_ARRAY,
            .f_name = static_cast<std::uint8_t>(name.length()),
            .f_hunk = static_cast<std::uint32_t>(size & ((1 << SIZEOF_BITFIELD(hunk_sizes_t, f_hunk)) - 1)),
        };
#pragma GCC diagnostic pop
        std::uint16_t const idx(static_cast<std::uint16_t>(index));

        if(hunk_sizes.f_name != name.length()
        || hunk_sizes.f_hunk != size
        || index != idx)
        {
            throw brs_out_of_range("name, index, or hunk too large");
        }

        f_output.write(
                  reinterpret_cast<typename S::char_type const *>(&hunk_sizes)
                , sizeof(hunk_sizes));

        f_output.write(
                  reinterpret_cast<typename S::char_type const *>(&idx)
                , sizeof(idx));

        f_output.write(
                  reinterpret_cast<typename S::char_type const *>(name.c_str())
                , hunk_sizes.f_name);

        f_output.write(
                  reinterpret_cast<typename S::char_type const *>(ptr)
                , size);
    }

    template<typename T>
    void add_value(name_t const & name, name_t const & sub_name, T const * ptr, std::size_t size)
    {
        if(name.empty())
        {
            throw brs_cannot_be_empty("name cannot be an empty string");
        }

        if(sub_name.empty())
        {
            throw brs_cannot_be_empty("sub-name cannot be an empty string");
        }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
        hunk_sizes_t const hunk_sizes = {
            .f_type = TYPE_MAP,
            .f_name = static_cast<std::uint8_t>(name.length()),
            .f_hunk = static_cast<std::uint32_t>(size & ((1 << SIZEOF_BITFIELD(hunk_sizes_t, f_hunk)) - 1)),
        };
#pragma GCC diagnostic pop
        std::uint8_t const len(static_cast<std::uint8_t>(sub_name.length()));

        if(hunk_sizes.f_name != name.length()
        || hunk_sizes.f_hunk != size
        || sub_name.length() != len)
        {
            throw brs_out_of_range("name, sub-name, or hunk too large");
        }

        f_output.write(
                  reinterpret_cast<typename S::char_type const *>(&hunk_sizes)
                , sizeof(hunk_sizes));

        f_output.write(
                  reinterpret_cast<typename S::char_type const *>(&len)
                , sizeof(len));

        f_output.write(
                  reinterpret_cast<typename S::char_type const *>(sub_name.c_str())
                , len);

        f_output.write(
                  reinterpret_cast<typename S::char_type const *>(name.c_str())
                , hunk_sizes.f_name);

        f_output.write(
                  reinterpret_cast<typename S::char_type const *>(ptr)
                , size);
    }


// *** FIELDS ***
    /** \brief Save a basic type or struct of basic types.
     *
     * This one function saves the specified value as is. It is expected to be
     * a basic type such as an int or a double. It also supports structures
     * that are only composed of basic types. Structures and classes with
     * complex types such as a string need to be handled manually.
     *
     * \tparam T  The type of value,
     * \param[in] name  The name of the field to be saved.
     * \param[in] value  The value to be saved with that name.
     */
    template<typename T>
    typename std::enable_if_t<!snapdev::is_vector_v<T>
                            && !std::is_same_v<char const *, T>
                            && !std::is_same_v<std::string const &, T>, void>
    add_value(name_t const & name, T const & value)
    {
        add_value(name, &value, sizeof(value));
    }


    void add_value(name_t const & name, std::nullptr_t)
    {
        NOT_USED(name);
    }


    void add_value(name_t const & name, char const * value)
    {
        add_value(name, value, strlen(value));
    }


    void add_value(name_t const & name, std::string const & value)
    {
        add_value(name, value.c_str(), value.length());
    }


    void add_value_if_not_empty(name_t const & name, std::string const & value)
    {
        if(!value.empty())
        {
            add_value(name, value.c_str(), value.length());
        }
    }


    template<typename T>
    typename std::enable_if_t<snapdev::is_vector_v<T>>
    add_value(name_t const & name, T const & value)
    {
        add_value(name, value.data(), value.size() * sizeof(typename T::value_type));
    }


// *** INDEXES ***
    template<typename T>
    typename std::enable_if_t<!snapdev::is_vector_v<T>
                            && !std::is_same_v<char const *, T>, void>
    add_value(name_t const & name, int index, T const & value)
    {
        add_value(name, index, &value, sizeof(value));
    }


    void add_value(name_t const & name, int index, std::nullptr_t)
    {
        NOT_USED(name, index);
    }


    void add_value(name_t const & name, int index, char const * value)
    {
        add_value(name, index, value, strlen(value));
    }


    void add_value(name_t const & name, int index, std::string const & value)
    {
        add_value(name, index, value.c_str(), value.length());
    }


// *** MAPS ***
    template<typename T>
    typename std::enable_if_t<!std::is_same_v<T, typename std::string>, void>
    add_value(name_t const & name, name_t const & sub_name, T const & value)
    {
        add_value(name, sub_name, &value, sizeof(value));
    }


    void add_value(name_t const & name, name_t const & sub_name, std::nullptr_t)
    {
        NOT_USED(name, sub_name);
    }


    void add_value(name_t const & name, name_t const & sub_name, char const * value)
    {
        add_value(name, sub_name, value, strlen(value));
    }


    void add_value(name_t const & name, name_t const & sub_name, std::string const & value)
    {
        add_value(name, sub_name, value.c_str(), value.length());
    }


    void add_value_if_not_empty(name_t const & name, name_t const & sub_name, std::string const & value)
    {
        if(!value.empty())
        {
            add_value(name, sub_name, value.c_str(), value.length());
        }
    }


// *** SUB-FIELDS ***
    void start_subfield(name_t const & name)
    {
        if(name.empty())
        {
            throw brs_cannot_be_empty("name cannot be an empty string");
        }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
        hunk_sizes_t const hunk_sizes = {
            .f_type = TYPE_FIELD,
            .f_name = static_cast<std::uint8_t>(name.length()),
            .f_hunk = 0,
        };
#pragma GCC diagnostic pop

        if(hunk_sizes.f_name != name.length())
        {
            throw brs_out_of_range("name too large");
        }

        f_output.write(
                  reinterpret_cast<typename S::char_type const *>(&hunk_sizes)
                , sizeof(hunk_sizes));

        f_output.write(
                  reinterpret_cast<typename S::char_type const *>(name.c_str())
                , hunk_sizes.f_name);
    }


    void end_subfield()
    {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
        hunk_sizes_t const hunk_sizes = {
            .f_type = TYPE_FIELD,
            .f_name = 0,
            .f_hunk = 0,
        };
#pragma GCC diagnostic pop

        f_output.write(
                  reinterpret_cast<typename S::char_type const *>(&hunk_sizes)
                , sizeof(hunk_sizes));
    }


private:
    S &         f_output = S();
};


template<typename S>
class recursive
{
public:
    recursive(serializer<S> & s, name_t const & name)
        : f_serializer(s)
    {
        f_serializer.start_subfield(name);
    }

    ~recursive()
    {
        f_serializer.end_subfield();
    }

private:
    serializer<S> &     f_serializer;
};








/** \brief When deserializing, the data is saved in a field.
 *
 * This field holds the metadata of one field.
 *
 * Note that the field data will still be in the stream and it's up to you
 * to read it in your variable. That way, we can avoid a copy. The read
 * can use the read_data() functions which support all basic types (int,
 * float, etc.), the std::string, and std::vector<>.
 */
struct field_t
{
    void reset()
    {
        f_name.clear();
        f_sub_name.clear();
        f_index = -1;
        f_size = 0;
    }

    std::string     f_name = std::string();
    std::string     f_sub_name = std::string();
    int             f_index = -1;
    std::size_t     f_size = 0;         // size of the data (still in stream)
};


/** \brief Unserialize the specified buffer.
 *
 * The deserialize() function reads each hunk and calls the specified
 * callback with the data. That function can save the data in your object
 * fields as required.
 *
 * The buffer must start with the BRS magic code.
 */
template<typename S>
class deserializer
{
public:
    typedef std::function<bool(deserializer<S> &, field_t const &)>    process_hunk_t;

    /** \brief Initialize the deserializer.
     *
     * The deserializer keeps a reference to your input stream. You must
     * make sure that the input stream remains valid for the duration
     * of the deserialization process.
     *
     * \exception brs_magic_missing
     * The function reads and verifies the magic. If that fails, this
     * exception is raised.
     *
     * \param[in,out] input  The input stream to parse.
     */
    deserializer(S & input)
        : f_input(input)
    {
        magic_t magic = {};
        f_input.read(reinterpret_cast<typename S::char_type *>(&magic), sizeof(magic));
        if(!f_input || f_input.gcount() != sizeof(magic))
        {
            throw brs_magic_missing("magic missing at the start of the buffer.");
        }

        // once we have multiple versions, this is where we'll start splitting
        // hairs to make it all work; for now, we have one so it's easy
        //
        if(magic != BRS_MAGIC)
        {
            throw brs_magic_unsupported("magic unsupported.");
        }
    }

    /** \brief Deserialize the input stream specified on the constructor.
     *
     * The function goes through all the hunks found in the input stream.
     * For each hunk, it calls the \p callback function with the hunk
     * definition saved in a field_t structure. Your callback is expected
     * to call the read_data() corresponding to the field being read.
     *
     * If any hunk looks invalid, then the function returns false. If
     * a hunk is found, but it cannot be read properly, then the function
     * may throw.
     *
     * \exception brs_map_name_cannot_be_empty
     * Found a size of 0 for the name of a map item.
     *
     * \exception brs_unknown_type
     * The hunk type is not currently supported.
     *
     * \param[in] callback  The callback function called with each field
     *                      found in the input stream.
     *
     * \return true if the unserialization succeeded, false otherwise.
     */
    bool deserialize(process_hunk_t & callback)
    {
        for(;;)
        {
            hunk_sizes_t hunk_sizes = {};
            f_input.read(reinterpret_cast<typename S::char_type *>(&hunk_sizes), sizeof(hunk_sizes));
            if(!f_input || f_input.gcount() != sizeof(hunk_sizes))
            {
                return f_input.eof() && f_input.gcount() == 0;
            }

            f_field.reset();
            f_field.f_size = hunk_sizes.f_hunk;

            switch(hunk_sizes.f_type)
            {
            case TYPE_FIELD:
                if(hunk_sizes.f_name == 0
                && hunk_sizes.f_hunk == 0)
                {
                    // we found an "end sub-field" entry
                    //
                    return true;
                }
                break;

            case TYPE_ARRAY:
                {
                    std::uint16_t idx(0);
                    f_input.read(reinterpret_cast<typename S::char_type *>(&idx), sizeof(idx));
                    if(!f_input || f_input.gcount() != sizeof(idx))
                    {
                        return false;
                    }
                    f_field.f_index = idx;
                }
                break;

            case TYPE_MAP:
                {
                    std::uint8_t len(0);
                    f_input.read(reinterpret_cast<typename S::char_type *>(&len), sizeof(len));
                    if(!f_input || f_input.gcount() != sizeof(len))
                    {
                        return false;
                    }
                    if(len == 0)
                    {
                        throw brs_map_name_cannot_be_empty("the length of a map's field name cannot be zero.");
                    }
                    f_field.f_sub_name.resize(len);
                    f_input.read(reinterpret_cast<typename S::char_type *>(f_field.f_sub_name.data()), len);
                    if(!f_input || f_input.gcount() != len)
                    {
                        return false;
                    }
                }
                break;

            default:
                throw brs_unknown_type("read a field with an unknown type.");

            }

            f_field.f_name.resize(hunk_sizes.f_name);
            f_input.read(reinterpret_cast<typename S::char_type *>(f_field.f_name.data()), hunk_sizes.f_name);
            if(!f_input || f_input.gcount() != hunk_sizes.f_name)
            {
                return false;
            }

            callback(*this, f_field);
        }
    }

    template<typename T>
    bool read_data(T & data)
    {
        if(f_field.f_size != sizeof(data))
        {
            throw brs_logic_error(
                      "hunk size is "
                    + std::to_string(f_field.f_size)
                    + ", but you are trying to read "
                    + std::to_string(sizeof(data))
                    + '.');
        }

        f_input.read(reinterpret_cast<typename S::char_type *>(&data), sizeof(data));
        return verify_size(sizeof(data));
    }

    bool read_data(std::string & data)
    {
        data.resize(f_field.f_size);
        f_input.read(reinterpret_cast<typename S::char_type *>(data.data()), f_field.f_size);
        return verify_size(f_field.f_size);
    }

    template<typename T>
    bool read_data(std::vector<T> & data)
    {
        if(f_field.f_size % sizeof(T) != 0)
        {
            throw brs_logic_error(
                      "hunk size ("
                    + std::to_string(f_field.f_size)
                    + ") is not a multiple of the vector item size: "
                    + std::to_string(sizeof(T))
                    + '.');
        }

        data.resize(f_field.f_size / sizeof(T));
        f_input.read(reinterpret_cast<typename S::char_type *>(data.data()), f_field.f_size);
        return verify_size(f_field.f_size);
    }

private:
    bool verify_size(std::size_t expected_size)
    {
        return f_input && static_cast<ssize_t>(expected_size) == f_input.gcount();
    }

    S &         f_input;
    field_t     f_field = field_t();
};



} // namespace snapdev
// vim: ts=4 sw=4 et
