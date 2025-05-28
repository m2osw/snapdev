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
 * \brief A set of templates to help with initializing structures.
 *
 * Whenever you initialize a structure, it is much better to make use of
 * the proper type, ensure valid data inter-dependency, etc. This can
 * be accomplished with C++ functions, however, you generally don't
 * initialize structures that way.
 *
 * With the templates found in this file, you can create classes you
 * use to initialize your structures in a much safer manner than with
 * direct values in a structure. Especially, you have a single location
 * which ensures the order of the values. You do not have to enter all
 * the values that can be given a valid default. Values that are mandatory
 * must be given a value. And probably best of all, you can verify that
 * your structure makes sense.
 *
 * Here is an example:
 *
 * \code
 *     #include <snapdev/init_structure.h>
 *
 *     // Your structure
 *     struct foo
 *     {
 *         char const *     f_name = nullptr;
 *         int              f_size = 0;
 *     };
 *
 *     // class to setup the f_name field
 *     class FieldName
 *         : public snapdev::StructureValue<char const *>
 *     {
 *     public:
 *         constexpr FieldName()
 *             : snapdev::StructureValue<char const *>(nullptr)
 *         {
 *         }
 *
 *         constexpr FieldName(char const * name)
 *             : snapdev::StructureValue<char const *>(name)
 *         {
 *         }
 *     };
 *
 *     // class to setup the f_size field
 *     class FieldSize
 *         : public snapdev::StructureValue<int>
 *     {
 *     public:
 *         constexpr FieldSize()
 *             : snapdev::StructureValue<int>(0)
 *         {
 *         }
 *
 *         constexpr FieldSize(int flags)
 *             : snapdev::StructureValue<int>(flags)
 *         {
 *         }
 *     };
 *
 *     template<class ...ARGS>
 *     constexpr foo define_foo(ARGS ...args)
 *     {
 *     // until we get C++20, we need this warning off
 *     #pragma GCC diagnostic push
 *     #pragma GCC diagnostic ignored "-Wpedantic"
 *         foo s =
 *         {
 *             .f_name = snapdev::find_field<FieldName>(args...),                // no default, mandatory
 *             .f_size = snapdev::find_field<FieldSize>(args..., FieldSize()),   // include a default
 *         };
 *     #pragma GCC diagnostic pop
 *
 *         // TODO: once possible (C++17/20?) add verification tests here
 *
 *         // size must be positive or null
 *         //if(f_size < 0)
 *         //{
 *         //    throw std::logic_error("you can't have a negative size");
 *         //}
 *
 *         return s;
 *     }
 * \endcode
 *
 * The `define_foo()` function is used to declare a structure. If you have
 * an array, just separate multiple calls by commas. For example:
 *
 * \code
 *     constexpr foo const g_my_foos[] =
 *     {
 *         define_foo(
 *             FieldName("name-1")
 *             // no need to specify size if the default is good
 *         ),
 *         define_foo(
 *             FieldName("another-name"),
 *             FieldSize(67)
 *         ),
 *         define_foo(
 *             // the order doesn't matter, which is cool, no need to learn
 *             // the order of each one of your structures
 *             FieldSize(222),
 *             FieldName("last-name")
 *         ),
 *         end_foo()
 *     };
 * \endcode
 *
 * To end your arrays, we suggest an `end_foo()` function. But you are also
 * welcome to use the `std::size(g_my_foos)` to get the number of entries in
 * your arrays (with templates you can also access the counter but I won't
 * talk about this).
 *
 * Here is an example of an `end_foo()` function. The main idea is to
 * setup one of the fields to a value which means this is the end of
 * your array. Here we set the name to `nullptr`. Remember that the
 * `FieldName()` is a required field. This is why it has to appear in
 * the end_foo() function.
 *
 * \code
 *     constexpr foo end_foo()
 *     {
 *         return define_foo(
 *                   FieldName(nullptr)
 *             );
 *     }
 * \endcode
 *
 * At this time, you can find examples of usage of these structures in
 * the advgetopt and snapdatabase projects.
 *
 * \note
 * At time of writing, the example above compiled. It should still do.
 * If not, let me know.
 */


// snapdev
//
#include    "snapdev/not_used.h"


// C++
//
#include    <algorithm>



namespace snapdev
{



/** \brief Base class for the find_field() functions.
 *
 * This structure is what gets searched. The value is set by the
 * constructor. It is always mandatory. Your class can have a default
 * by having a constructor with no arguments.
 *
 * The class has a get function which the find_field() uses to retrieve
 * the value we want to save in the structure field. Notice how all
 * the functions are `constexpr`. This is important to make sure that
 * everything happens at compile time.
 *
 * \tparam T  The type of the data for this specific field value.
 */
template<typename T>
class StructureValue
{
public:
    typedef T   value_t;

    constexpr StructureValue(T const v)
        : f_value(v)
    {
    }

    constexpr value_t get() const
    {
        return f_value;
    }

private:
    value_t     f_value;
};



/** \brief Found a matching option.
 *
 * The list of `ARGS` is the list of `Field<...>` classes you defined in your
 * `define_blah()` function.
 *
 * This instance of find_field() is used by the compiler when the
 * first argument matches the type being searched (`T`). This is the
 * function which returns the value we have been looking for.
 *
 * \param[in] first  The first of the ARGS arguments, the argument we're
 * trying to match.
 * \param[in] args  The arguments being searched.
 *
 * \tparam T  The type of the field.
 * \tparam F  The field we're trying to match.
 * \tparam ARGS  The list of possible arguments.
 *
 * \return The value of the first argument to save in the structure.
 */
template<typename T, typename F, class ...ARGS>
constexpr typename std::enable_if<std::is_same<T, F>::value, typename T::value_t>::type find_field(F first, ARGS ...args)
{
    NOT_USED(args...);
    return first.get();
}



/** \brief Look for an option.
 *
 * The list of `ARGS` is the list of `Field<...>` classes you defined in your
 * `define_blah()` function.
 *
 * This instance is used whenever the first argument doesn't match. This
 * means we want to try the next field.
 *
 * \param[in] first  The first of the ARGS arguments, the argument we're
 * trying to match.
 * \param[in] args  The arguments being searched.
 *
 * \tparam T  The type of the field.
 * \tparam F  The field we're trying to match.
 * \tparam ARGS  The list of possible arguments.
 *
 * \return The value of one of the arguments to save in the structure.
 */
template<typename T, typename F, class ...ARGS>
constexpr typename std::enable_if<!std::is_same<T, F>::value, typename T::value_t>::type find_field(F first, ARGS ...args)
{
    NOT_USED(first);
    return find_field<T>(args...);
}



} // namespace snapdev
// vim: ts=4 sw=4 et
