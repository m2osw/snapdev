// Snap Websites Server -- divert ostream output to a stringbuf for a while
// Copyright (c) 2019  Made to Order Software Corp.  All Rights Reserved
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
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#pragma once

// C++ lib
//
#include <ostream>

// C lib
//
#include <stdlib.h>



namespace snap
{


/** \brief Swap the output of an std::basic_ostream to a buffer.
 *
 * This RAII class is used to temporarilly switch the output buffer
 * of an std::basic_ostream to an std::stringbuf. The original is
 * saved while the object exists and restored on destruction.
 *
 * This is particularly useful in our tests where a function being
 * tested outputs data in a standard stream such as std::cout or
 * std::cerr.
 *
 * \code
 * {
 *     // save the existing cout and start capturing
 *     snap::ostream_to_buf<char> capture(std::cout);
 *
 *     ...run test sending data to std::cout...
 *
 *     // verify that the output is what we expected
 *     CATCH_REQUIRE(capture.str() == "2.0.1\n");
 * }
 * \endcode
 *
 * The class itself is an std::stringbuf so you can access all the
 * usual std::stringbuf functions (such as `str()`).
 *
 * \warning
 * This class is not thread safe.
 */
template<
      class CharT
    , class Traits = std::char_traits<CharT>
    , class Allocator = std::allocator<CharT>
>
class ostream_to_buf
    : public std::basic_stringbuf<CharT, Traits, Allocator>
{
public:
    typedef CharT                       char_type;
    typedef Traits                      traits_type;
    typedef typename Traits::int_type   int_type;
    typedef typename Traits::pos_type   pos_type;
    typedef typename Traits::off_type   off_type;
    typedef Allocator                   allocator_type;

    typedef std::basic_stringbuf<char_type, traits_type, allocator_type>    stringbuf_type;
    typedef std::basic_ostream<char_type, traits_type>                      stream_type;
    typedef std::basic_streambuf<char_type, traits_type>                    streambuf_type;
    typedef std::basic_string<char_type, traits_type, allocator_type>       string_type;

    ostream_to_buf<char_type, traits_type, allocator_type>(stream_type & out)
        : f_stream(out)
        , f_original(f_stream.rdbuf(this))
    {
    }

    ostream_to_buf<char_type, traits_type, allocator_type>(ostream_to_buf<char_type, traits_type, allocator_type> const & rhs) = delete;
    ostream_to_buf<char_type, traits_type, allocator_type> & operator = (ostream_to_buf<char_type, traits_type, allocator_type> const & rhs) = delete;

    ~ostream_to_buf()
    {
        f_stream.rdbuf(f_original);
    }

private:
    stream_type &       f_stream;
    streambuf_type *    f_original = nullptr;
};


} // namespace snap
// vim: ts=4 sw=4 et
