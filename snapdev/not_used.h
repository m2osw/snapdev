// Copyright (c) 2011-2022  Made to Order Software Corp.  All Rights Reserved
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
// 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#pragma once

namespace snap
{


[[deprecated]] inline constexpr void NOTUSED()
{
}


template <class T, class ...ARGS>
[[deprecated]] inline constexpr void NOTUSED( T && first, ARGS && ...args )
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
    static_cast<void>( first );
#pragma GCC diagnostic pop
    NOTUSED(args...);
}


inline constexpr void NOT_USED()
{
}


template <class T, class ...ARGS>
inline constexpr void NOT_USED( T && first, ARGS && ...args )
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
    static_cast<void>( first );
#pragma GCC diagnostic pop
    NOT_USED(args...);
}



}
//namespace snap

// vim: ts=4 sw=4 et
