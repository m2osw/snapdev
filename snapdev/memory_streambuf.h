// Copyright (c) 2019-2025  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Create a stream from a memory buffer.
 *
 * At times we have a memory buffer that we need to pass as a stream.
 * This template can be used to transform that buffer in a streambuf,
 * which can then be passed to a stream on creation or through rdbuf().
 */

// snapdev
//
#include    <snapdev/not_reached.h>
#include    <snapdev/not_used.h>


// C++
//
#include    <iostream>
#include    <ostream>
//#include    <sstream>
//#include    <string>



namespace snapdev
{



/** \brief Create a streambuf where the data comes from a memory buffer.
 *
 * This class is used to create a streambuf from a memory buffer.
 * If you are looking at capturing the output of a stream, you may be
 * interested by the ostream_to_buf instead. That other implementation
 * allows you to save the output of a stream in a string and then verify
 * that string.
 *
 * This implementation was created for input rather than output, allowing
 * you to create a buffer from a memory buffer.
 *
 * \code
 * {
 *     // Say you have a vector with data
 *     char data[]{ 1, 2, 3, 4, 5, 6, 7 };
 *
 *     // you create a vector_streambuf this way
 *     snapdev::memory_streambuf buf(data, sizeof(data));
 *
 *     // and then an input stream this way
 *     std::istream in(&buf);
 *
 *     // now you can read & seek as usual
 *
 *     // if you want to be able to do updates, use an iostream insead
 *     std::iostream in_out(&buf);
 * }
 * \endcode
 *
 * \note
 * The function expects the memory buffer to only contain data that can
 * be read/written as such. This means no pointers, not classes with
 * virtual functions, etc.
 *
 * \warning
 * The memory buffer must remain available and not change in size.
 *
 * \tparam CharT  The type of characters used by the stream.
 * \tparam Traits  The traits of the specified character type.
 */
template<
      typename CharT = char
    , typename Traits = std::char_traits<CharT>
> class memory_streambuf
    : public std::basic_streambuf<CharT, Traits>
{
public:
    typedef CharT                       char_type;
    typedef Traits                      traits_type;
    typedef typename Traits::int_type   int_type;
    typedef typename Traits::pos_type   pos_type;
    typedef typename Traits::off_type   off_type;

    typedef std::basic_streambuf<char_type, traits_type>    streambuf_type;

    /** \brief Initialize the streambuf from \p data.
     *
     * This function creates a streambuf that you can then use to initialize
     * an input or output stream with.
     *
     * This version of the constructor allows to read and write to the
     * stream buffer (a.k.a. the vector). You can use it with input or
     * an output stream.
     *
     * Note that the reading and writing happen from the start of the
     * buffer unless a corresponding seek is used.
     *
     * \todo
     * Note that the overflow() function does not yet know how to grow
     * the vector. If you attempt to write past the end, an exception
     * is generated.
     *
     * \param[in] data  A pointer to a memory buffer.
     * \param[in] size  The size of the data buffer in bytes.
     */
    memory_streambuf(void * data, std::size_t size)
    {
        char * ptr(reinterpret_cast<char *>(data));
        char_type * begin(reinterpret_cast<char_type *>(ptr));
        char_type * end(reinterpret_cast<char_type *>(ptr + size));
        this->setg(begin, begin, end);
        this->setp(begin, end);
    }

    /** \brief Initialize the streambuf from \p vec.
     *
     * This function creates a read-only streambuf that you can then use
     * to initialize an input stream with. Trying to write to that stream
     * generates an exception.
     *
     * \param[in] data  A pointer to a memory buffer.
     * \param[in] size  The size of the data buffer in bytes.
     */
    memory_streambuf(void const * data, std::size_t size)
        : f_read_only(true)
    {
        char * ptr(const_cast<char *>(reinterpret_cast<char const *>(data)));
        char_type * begin(reinterpret_cast<char_type *>(ptr));
        char_type * end(reinterpret_cast<char_type *>(ptr + size));
        this->setg(begin, begin, end);

        // a write calls overflow() if the start & end pointers are the same
        //
        this->setp(begin, begin);
    }

protected:
    /** \brief Function called when an overflow on a put() happens.
     *
     * This function is called if a put() attempts to write past the
     * end of the buffer. In that case, we throw an exception. It is
     * not possible for us to grow the buffer.
     *
     * \param[in] c  The character to output.
     *
     * \return The input character \p c or '\0' of \p c is EOF.
     */
    int_type overflow(int_type c)
    {
        snapdev::NOT_USED(c);
        throw std::ios_base::failure("this buffer is read-only, writing to the buffer is not available.");
        snapdev::NOT_REACHED();
    }

    virtual pos_type seekoff(
          off_type const offset
        , std::ios_base::seekdir const dir
        , std::ios_base::openmode const mode) override
    {
        off_type result(-1);

        if((mode & std::ios_base::in) != 0)
        {
            char_type * pos = nullptr;
            switch(dir)
            {
            case std::ios_base::cur:
                pos = this->gptr() + offset;
                break;

            case std::ios_base::end:
                pos = this->egptr() + offset;
                break;

            case std::ios_base::beg:
                pos = this->eback() + offset;
                break;

            default:
                throw std::ios_base::failure("unknown direction in seekoff() -- in");

            }
            if(pos < this->eback())
            {
                pos = this->eback();
            }
            if(pos > this->egptr())
            {
                pos = this->egptr();
            }
            this->setg(this->eback(), pos, this->egptr());
            result = pos - this->eback();
        }

        if((mode & std::ios_base::out) != 0)
        {
            char_type * pos = nullptr;
            switch(dir)
            {
            case std::ios_base::cur:
                pos = this->pptr() + offset;
                break;

            case std::ios_base::end:
                pos = this->epptr() + offset;
                break;

            case std::ios_base::beg:
                pos = this->pbase() + offset;
                break;

            default:
                throw std::ios_base::failure("unknown direction in seekoff() -- out");

            }
            if(pos < this->pbase())
            {
                pos = this->pbase();
            }
            if(pos > this->epptr())
            {
                pos = this->epptr();
            }
            this->setp(this->pbase(), this->epptr());
            result = pos - this->pbase();
            this->pbump(result);
        }

        return result;
    }

    virtual pos_type seekpos(pos_type offset, std::ios_base::openmode mode) override
    {
        return seekoff(offset, std::ios_base::beg, mode);
    }

private:
    /** \brief Whether the buffer is considered read-only or not.
     *
     * When creating the streambuf with a constant memory buffer, this
     * parameter is set to true. This prevents writes to the file. In
     * other words, the memory will never be updated.
     */
    bool                f_read_only = false;
};



} // namespace snapdev
// vim: ts=4 sw=4 et
