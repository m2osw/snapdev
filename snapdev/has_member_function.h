// Copyright (c) 2013-2021  Made to Order Software Corp.  All Rights Reserved
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

// self
//
//#include    <snapdev/has_member_function.h>


// C++ lib
//
#include    <algorithm>
#include    <functional>



namespace snap
{


template<typename T, typename M, typename ... A>
class has_member_function
{
public:
    //struct No {};

    //enum
    //{
    //    value = !std::is_same<
    //              decltype(std::declval<M>()(std::declval<T>(), std::declval<T>()))
    //            , No>::value
    //};

    // the following is more generic but I could not make it work

    template<typename TT = T>
    static auto test(TT &&t, A && ... args)
            -> decltype(t.*M(std::forward<A>(args) ...)
                      , void()
                      , std::true_type{});

    static auto test(...) -> std::false_type;

    using type = decltype(test(
                          std::declval<T>()
                        , std::declval<A>()...));

    enum
    {
        value = std::is_same<type, std::true_type>::value
    };
};


//template<typename T, typename ...ARGS>
//class has_equal_operator
//    : public has_member_function<T, std::equal_to<T>>::type
//{
//};

template<typename T>
class has_equal_operator
    : public has_member_function<
                      T
                    , std::equal_to<T>
                    , T
                    , T>
{
};



}
// snap namespace
// vim: ts=4 sw=4 et
