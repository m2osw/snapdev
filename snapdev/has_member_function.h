// Copyright (c) 2013-2023  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Check whether a class has a named member function.
 *
 * This template allows you to determine whether a given object has a
 * given member function. This allows you to compile a template only
 * if that is true and use said function as required. Otherwise, you
 * can have another template to handle the case when the function is
 * not available.
 */

// C++
//
#include    <algorithm>
#include    <functional>



namespace snapdev
{


// the following is based on answers from:
// https://stackoverflow.com/questions/6534041/how-to-check-whether-operator-exists
//
// however, at this point all the answers seem too specialized to make them
// work consistently; right now, these classes do not work as expected...
//
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



} // namespace snapdev
// vim: ts=4 sw=4 et
