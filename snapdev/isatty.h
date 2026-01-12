// Copyright (c) 2011-2026  Made to Order Software Corp.  All Rights Reserved
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

// self
//
#include    <snapdev/stream_fd.h>



// C
//
#include    <unistd.h>



namespace snapdev
{



/** \brief Check whether a C++ iostream is a TTY.
 *
 * This function checks the specified stream and if it represents a TTY
 * returns true.
 *
 * If the stream is not a standard file stream, then the function always
 * returns false, even if ultimately the stream does represent a TTY. This
 * is because we can really only handle the standard fstream's objects here.
 *
 * \tparam _CharT  type of character this stream handles.
 * \tparam _Traits  traits of the characters; based on CharT by default.
 * \param[in] stream  The stream to check.
 *
 * \return true if \p stream is a TTY.
 */
template<typename _CharT
       , typename _Traits = std::char_traits<_CharT>>
bool isatty(std::basic_ios<_CharT, _Traits> const & stream)
{
    int const r(stream_fd(stream));
    if(r < 0)
    {
        return false;
    }

    return ::isatty(r);
}



} // namespace snapdev
// vim: ts=4 sw=4 et

