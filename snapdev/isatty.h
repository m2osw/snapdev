// Copyright (c) 2011-2022  Made to Order Software Corp.  All Rights Reserved
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

// C++
//
#include    <fstream>
#include    <iostream>
#include    <ext/stdio_filebuf.h>
#include    <ext/stdio_sync_filebuf.h>

// C
//
#include    <unistd.h>



namespace snapdev
{



namespace detail
{

template<typename _CharT
       , typename _Traits = std::char_traits<_CharT>>
class our_basic_filebuf
    : public std::basic_filebuf<_CharT, _Traits>
{
public:
    std::__c_file * file() throw()
    {
        //return std::basic_filebuf<_CharT, _Traits>::_M_file.file();
        return this->_M_file.file();
    }
};

} // namespace detail



/** \brief Check whether a C++ iostream is a TTY.
 *
 * This function checks the specified stream and if it represents a TTY
 * returns true.
 *
 * If the stream is not a standard file stream, then the function always
 * returns false, even if ultimately the stream does represent a TTY. This
 * is because we can really only handle the standard fstream's objects here.
 *
 * \note
 * The function dynamic casts to a the three different types of buffers
 * to see whether we can find a file() function. If so, then we check
 * that the file isatty() or not.
 *
 * \tparam _CharT  type of character this stream handles.
 * \tparam _Traits  traits of the characters based on CharT by default.
 * \param[in] stream  The stream to check.
 *
 * \return true if \p stream is a TTY.
 */
template<typename _CharT
       , typename _Traits = std::char_traits<_CharT>>
bool isatty(std::basic_ios<_CharT, _Traits> const & s)
{
    // in most cases, we use cin, cout, cerr, or clog which are all
    // stdio_sync_filebuf<> objects
    {
        typedef __gnu_cxx::stdio_sync_filebuf<_CharT, _Traits> io_sync_buffer_t;
        io_sync_buffer_t * buffer(dynamic_cast<io_sync_buffer_t *>(s.rdbuf()));
        if(buffer != nullptr)
        {
            return ::isatty(fileno(buffer->file()));
        }
    }

    // in this case, we first do a dynamic_cast<>() to make sure we find the
    // correct buffer; if present we can do a static_cast<>() to gain access
    // to the _M_file field and thus the file() function (there is also an
    // fd() function, but to be consistent with the other cases, I used a
    // file() like function only)
    {
        typedef std::basic_filebuf<_CharT, _Traits> file_buffer_t;
        file_buffer_t * file_buffer(dynamic_cast<file_buffer_t *>(s.rdbuf()));
        if(file_buffer != nullptr)
        {
            typedef detail::our_basic_filebuf<_CharT, _Traits> hack_buffer_t;
            hack_buffer_t * buffer(static_cast<hack_buffer_t *>(file_buffer));
            if(buffer != nullptr)
            {
                return ::isatty(fileno(buffer->file()));
            }
        }
    }

    // older versions (C++98) used this class -- it is probably never
    // going to be true so I put it last
    {
        typedef __gnu_cxx::stdio_filebuf<_CharT, _Traits> io_buffer_t;
        io_buffer_t * buffer(dynamic_cast<io_buffer_t *>(s.rdbuf()));
        if(buffer != nullptr)
        {
            return ::isatty(fileno(buffer->file()));
        }
    }

    return false;
}



} // namespace snapdev
// vim: ts=4 sw=4 et

