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
#include    <snapdev/is_smart_pointer.h>
#include    <snapdev/has_member_function.h>


// C++ lib
//
#include    <algorithm>
#include    <functional>



namespace snap
{

/** \brief Manage a set of callbacks.
 *
 * This class is capable of handling a container of objects, either direct
 * functions or your objects with member functions you'd like to call on
 * an event.
 *
 * The following shows how objects are added:
 *
 * \code
 *     callback_manager<std::set<T::pointer_t>> callbacks;
 *
 *     callbacks.add_callback(obj1);
 *     callbacks.add_callback(obj2);
 *     callbacks.add_callback(obj3);
 *
 *     // call member_function() on obj1, obj2, obj3 with parameters p1, p2, p3
 *     //
 *     callbacks.call(&T::member_function, p1, p2, p3);
 *
 *     callbacks.remove_callback(obj2);  // won't call functions on obj2
 * \endcode
 *
 * If you instead want to add direct function calls, you can use a container
 * of functions and directly add functions. This works with objects as well
 * when used with std::bind() but you can only call those functions (opposed
 * to any one member function in the previous example).
 *
 * \code
 *     callback_manager<std::list<F>> callbacks;
 *
 *     callbacks.add_callback(std::bind(&T::member_function, obj1, std::placeholders::_1, std::placeholders::_2, ...));
 *     callbacks.add_callback(my_func);
 *
 *     // call the functions added earlier with p1, p2, p3
 *     //
 *     callbacks.call(p1, p2, p3);
 *
 *     callbacks.remove_callback(my_func);
 * \endcode
 *
 * Note that to be able to remove an std::bind() function, you must save that
 * object in a parameter. Each time you call std::bind() you get a new object
 * so it is necessary to save it to be able to reference it later.
 *
 * \code
 *     auto c = std::bind(...);
 *     callbacks.add_callback(c);
 *
 *     ...
 *
 *     callbacks.remove_callback(c);
 * \endcode
 *
 * Of course, you may use the clear() function as well. However, that is not
 * always what you want.
 *
 * \warning
 * For certain types of functions, the DUPLICATES must be set to `true`,
 * otherwise the add_callbacks() will fail. This is especially true if you
 * use std::function or std::bind functions. These do not compare against
 * nullptr or each others. The solution is to set DUPLICATES to `true`
 * and not try to use the remove_callback() function. Also, that means
 * those types of functions can be added to an std::set which requires
 * the == operator to exist.
 *
 * \tparam C  The type of container to use (std::vector, std::list, ...).
 * \tparam DUPLICATES  Whether duplicates are allowed (true) or not (false).
 */
template<class C, bool DUPLICATES = false>
class callback_manager
{
    /** \brief Function used when the "callbacks" are objects.
     *
     * In this case, we are managing a container of shared pointers to
     * objects. The call() function expects the first parameter to be
     * a member function \em pointer (it's really an offset), instead of
     * a parameter to the callback.
     *
     * The callback does not need to include any arguments.
     *
     * If you have more than one callback registered, they all get called
     * as long as the previous callback returned `true`. If a callback
     * returns `false`, then the loop ends early and the call_member()
     * function returns `false`.
     *
     * \tparam F  The type of the member function to call.
     * \tparam ARGS  The types of the list of arguments.
     * \param[in] func  The member function that gets called.
     * \param[in] args  The arguments to pass to the member function.
     *
     * \return true if all the callbacks returned true, false otherwise.
     */
    template<typename F, typename ... ARGS>
    bool call_member(F func, ARGS ... args)
    {
        C callbacks(f_callbacks);
        for(auto c : callbacks)
        {
            if(!(c.get()->*func)(args...))
            {
                return false;
            }
        }
        return true;
    }


    /** \brief Call the direct functions.
     *
     * This function is used to call all the functions found in the container.
     * The function is a loop which goes through all the registered functions
     * and call them with the specified \p args.
     *
     * If one of the functions returns `false`, then the loop stops
     * immediately and this function returns `false`. If all the
     * callback functions returns `true`, then all get called and
     * call_function() returns `true.
     *
     * \tparam ARGS  The types of the arguments to pass to the callbacks.
     * \param[in] args  The arguments to pass to the callbacks.
     *
     * \return true if all the callbacks return true.
     */
    template<typename ... ARGS>
    bool call_function(ARGS ... args)
    {
        C callbacks(f_callbacks);
        for(auto c : callbacks)
        {
            if(!std::invoke(c, args...))
            {
                return false;
            }
        }
        return true;
    }


public:
    typedef C value_type;
    typedef typename C::size_type size_type;
    typedef typename C::difference_type difference_type;
    typedef value_type & reference;
    typedef value_type const & const_reference;
    typedef typename C::allocator_type allocator_type;
    typedef typename C::pointer pointer;
    typedef typename C::const_pointer const_pointer;

    /** \brief Add a callback to this manager.
     *
     * This function inserts the given callback to this manager. By default,
     * the new callback goes at the end of the container. To add the
     * item at a different location, the container needs to be capable of
     * sorting the items in some way (i.e. a priority).
     *
     * The number of callbacks is not limited.
     *
     * It is possible to add a callback while within a callback function.
     * The new callback will not be seen until the next time an event
     * occurs and the call() function gets called.
     *
     * \note
     * If the callback type is an object share pointer, then you will be
     * able to call any member function of that object with the call()
     * function. On the other hand, for direct functions, only that one
     * specific function is called. Direct functions can use an std::bind()
     * in order to attach the function to an object at runtime.
     *
     * \note
     * By default the manager doesn't re-add a callback more than once.
     * This behavior is specific to your instance of the manager. It can
     * be changed by setting the DUPLICATE template parameter to true.
     * If you allow duplication of callbacks, then the same callback ends
     * up being called for each instance appearing in the manager.
     * However, the std::bind() function is likely to generate a different
     * object each time, so in most cases, the duplicity test is not really
     * going to work for direct functions.
     *
     * \note
     * This version of the add_callback() is used when the DUPLICATES was
     * set to false. This function checks whether the \p callback being
     * added is already present in the container. If so, then it does not
     * get re-added.
     *
     * \param[in] callback  The callback to be added to the manager.
     *
     * \return true if the callback was added, false otherwise.
     *
     * \sa remove_callback()
     * \sa call()
     */
    template<bool D = DUPLICATES>
        typename std::enable_if<!D, bool>::type
    add_callback(typename C::value_type callback)
    {
        if(callback == nullptr)
        {
            return false;
        }

        if(!DUPLICATES)
        {
            if(std::find(
                  begin(f_callbacks)
                , end(f_callbacks)
                , callback) != end(f_callbacks))
            {
                return false;
            }
        }

        // insert "at the end" (i.e. for std::set, it doesn't get at the end)
        //
        f_callbacks.insert(
                  f_callbacks.end()
                , callback);

        return true;
    }

    /** \brief Add a callback no matter what.
     *
     * This instance of the add_callback() function allows for the addition
     * of any callback, whether it is a duplicate or not.
     *
     * If you are adding direct functions using advance C++ features, such
     * as std::bind(), then having DUPLICATES set to false is likely to
     * fail because the std::bind() type has no `operator == ()` that works
     * on it.
     *
     * \tparam D  A copy of the duplicate flag.
     * \param[in] callback  The callback to be added.
     *
     * \return Always true since we cannot verify duplication.
     */
    template<bool D = DUPLICATES>
        typename std::enable_if<D, bool>::type
    add_callback(typename C::value_type callback)
    {
        // insert "at the end" (i.e. for std::set, it doesn't get at the end)
        //
        f_callbacks.insert(
                  f_callbacks.end()
                , callback);

        return true;
    }


    /** \brief Remove a callback.
     *
     * This function removes a callback that was previously added by the
     * add_callback() function.
     *
     * If is possible to call this function from within a callback function
     * that just got called (i.e. a function can remove itself). Note that
     * the removal is not seen by the call() function until it gets called
     * again. In other words, if you remove a function that wasn't yet
     * called, it will still get called this time around.
     *
     * \note
     * If duplicity is allowed in your manager, this function only removes
     * the first callback that matches.
     *
     * \note
     * The function is not available if the type C::value_type
     *
     * \param[in] callback  The callback to be removed.
     *
     * \return true if a callback was removed, false otherwise.
     *
     * \sa add_callback()
     * \sa call()
     */
    // the following breaks; I think that the has_equal_operator<> doesn't
    // work correctly at this point...
    //
    //template<typename ... ARGS, typename T = C>
    //typename std::enable_if<std::is_same<T, C>::value
    //        && snap::has_equal_operator<typename T::value_type>::value
    //    , bool>::type
    bool remove_callback(typename C::value_type callback)
    {
        auto it(std::find(
                  begin(f_callbacks)
                , end(f_callbacks)
                , callback));
        if(it == end(f_callbacks))
        {
            return false;
        }

        f_callbacks.erase(it);

        return true;
    }


    /** \brief Clear the list of callbacks.
     *
     * This function clears the container holding callbacks. This is an
     * equivalent to the remove_callback() called once for each callback
     * that was added with the add_callback() function.
     *
     * \return true if the list wasn't empty on entry.
     */
    bool clear()
    {
        if(f_callbacks.empty())
        {
            return false;
        }

        f_callbacks.clear();
        return true;
    }


    /** \brief Get the size of the list of callbacks.
     *
     * This function returns the number of callbacks currently managed.
     * On creating of the callback_manager, the size is 0. The size
     * increases by one each time you call the add_callback() and it
     * returns true. The size decreases by one each time you call the
     * remove_callback() and it returns true.
     *
     * You can reset the size to zero by calling the clear() function.
     *
     * \return The current size of the list of callbacks.
     */
    size_t size() const
    {
        return f_callbacks.size();
    }


    /** \brief Check whether the list of callbacks is empty or not.
     *
     * This function returns true if the add_callback() was never called
     * or all the callbacks were removed either by remove_callback()
     * or using the clear() function.
     *
     * \return true if the list of callbacks is empty.
     */
    bool empty() const
    {
        return f_callbacks.empty();
    }


    /** \brief Call the managed callbacks.
     *
     * This function calls all the managed callbacks with the specified
     * \p args.
     *
     * This version of the function calls the specified member function
     * (the very first argument) of the objects this callback_manager
     * handles.
     *
     * \code
     *     call(&foo::member_function, arg1, arg2, arg3, ...);
     * \endcode
     *
     * \note
     * This function verifies that the first parameter is a member function.
     * If not, then no matching call() function is found and the compiler
     * fails.
     *
     * \tparam F  The type of member functon.
     * \tparam ARGS  The type of each of the function arguments.
     * \tparam T  A copy of the container type.
     * \param[in] func  The member function to be called.
     * \param[in] args  The arguments to pass to the callback functions.
     *
     * \return true if all the callback functions returned true.
     */
    template<typename F, typename ... ARGS, typename T = C>
    typename std::enable_if<std::is_same<T, C>::value
            && std::is_member_function_pointer<F>::value
            && is_shared_ptr<typename T::value_type>::value, bool>::type
    call(F func, ARGS ... args)
    {
        return call_member(func, args...);
    }


    /** \brief Call the managed callbacks.
     *
     * This function calls all the managed callbacks with the specified
     * arguments.
     *
     * This version of the function calls a direct function which accepts
     * one or more arguments.
     *
     * We distinguish the first parameter in order to be able to test it
     * as a member function or not.
     *
     * \tparam ARGS  The type of each of the function arguments.
     * \tparam T  A copy of the container type.
     * \param[in] args  The arguments to pass to the callback functions.
     *
     * \return true if all the callback functions returned true.
     */
    template<typename ... ARGS, typename T = C>
    typename std::enable_if<std::is_same<T, C>::value
            && !is_shared_ptr<typename T::value_type>::value, bool>::type
    call(ARGS ... args)
    {
        return call_function(args...);
    }


private:
    /** \brief The list of callbacks.
     *
     * This variable holds the list of callbacks added by the add_callback()
     * function. By default it is empty, meaning that the call() function
     * does nothing. The list can be shrunk using the remove_callback()
     * or the clear() functions.
     */
    C       f_callbacks = C();
};



}
// snap namespace
// vim: ts=4 sw=4 et
