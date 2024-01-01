// Copyright (c) 2013-2024  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Template used to manage a list of callbacks.
 *
 * Often, you want to add a callback to a class. Later, you realize that
 * several other objects want to listen to that same event. This template
 * allows you to switch from a simple function pointer to a vector of
 * functions.
 *
 * This implementation allows you to generate an event and safely add or
 * remove listeners from the list while the event is being processed.
 */

// self
//
#include    <snapdev/is_smart_pointer.h>
#include    <snapdev/has_member_function.h>


// C++
//
#include    <algorithm>
#include    <cstdint>
#include    <functional>
#include    <iostream>
#include    <list>



namespace snapdev
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
 *     struct foo
 *     {
 *         typedef std::shared_ptr<foo> pointer_t;
 *
 *         bool member_function(int, float, std::string const &)
 *         {
 *             ...
 *             return true;
 *         }
 *     };
 *     callback_manager<foo::pointer_t> callbacks;
 *
 *     callbacks.add_callback(obj1);
 *     snap::callback_manager::callback_id_t save_id(callbacks.add_callback(obj2));
 *     callbacks.add_callback(obj3);
 *
 *     // call member_function() on obj1, obj2, obj3 with parameters p1, p2, p3
 *     //
 *     callbacks.call(&T::member_function, p1, p2, p3);
 *
 *     callbacks.remove_callback(save_id);  // won't call functions on obj2 anymore
 * \endcode
 *
 * If you instead want to add direct function calls, you can use a function
 * type and directly add functions. This works with objects as well when used
 * with std::bind() but you can only call those functions (opposed to any one
 * member function in the previous example).
 *
 * \code
 *     // note that the function MUST return `bool`
 *     //
 *     typedef std::function<bool(int, float, std::string const &)> F;
 *     callback_manager<F> callbacks;
 *
 *     snap::callback_manager::callback_id_t save1(callbacks.add_callback(std::bind(&T::member_function, obj1, std::placeholders::_1, std::placeholders::_2, ...)));
 *     snap::callback_manager::callback_id_t save2(callbacks.add_callback(my_func));
 *
 *     // call the functions added earlier with p1, p2, p3
 *     //
 *     callbacks.call(p1, p2, p3);
 *
 *     callbacks.remove_callback(save2);
 * \endcode
 *
 * Note that to be able to remove a callback, you must save the identifier
 * returned by the add_callback(). This works whatever the type of callback
 * you add (shared pointer, direct function, std::function, std::bind).
 *
 * \code
 *     auto c = std::bind(...);
 *     auto id = callbacks.add_callback(c);
 *
 *     ...
 *
 *     callbacks.remove_callback(id);
 * \endcode
 *
 * Of course, you may use the clear() function as well. However, that is not
 * always what you want.
 *
 * \tparam T  The type of function or object to manage.
 */
template<class T>
class callback_manager
{
public:
    typedef std::shared_ptr<callback_manager<T>>    pointer_t;
    typedef T                                       value_type;
    typedef std::uint32_t                           callback_id_t;
    typedef std::int32_t                            priority_t;

    static constexpr callback_id_t const            NULL_CALLBACK_ID = 0;
    static constexpr priority_t const               DEFAULT_PRIORITY = 0;

private:
    struct item_t
    {
        item_t(
                  callback_id_t id
                , value_type callback
                , priority_t priority)
            : f_id(id)
            , f_callback(callback)
            , f_priority(priority)
        {
        }

        callback_id_t   f_id = NULL_CALLBACK_ID;
        value_type      f_callback = value_type();
        priority_t      f_priority = DEFAULT_PRIORITY;
    };


    typedef std::list<item_t>                       callbacks_t;


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
     * returns `false`, then the loop ends early and the
     * call_member_pointer() function returns `false`.
     *
     * \tparam F  The type of the member function to call.
     * \tparam ARGS  The types of the list of arguments.
     * \param[in] func  The member function that gets called.
     * \param[in] args  The arguments to pass to the member function.
     *
     * \return true if all the callbacks returned true, false otherwise.
     */
    template<typename F, typename ... ARGS>
    bool call_member_pointer(F func, ARGS ... args)
    {
        auto callbacks(f_callbacks);
        for(auto & c : callbacks)
        {
            if(!(c.f_callback.get()->*func)(args...))
            {
                return false;
            }
        }
        return true;
    }


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
        auto callbacks(f_callbacks);
        for(auto & c : callbacks)
        {
            if(!(c.f_callback.*func)(args...))
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
     * call_function() returns `true`.
     *
     * \tparam ARGS  The types of the arguments to pass to the callbacks.
     * \param[in] args  The arguments to pass to the callbacks.
     *
     * \return true if all the callbacks return true.
     */
    template<typename ... ARGS>
    bool call_function(ARGS ... args)
    {
        auto callbacks(f_callbacks);
        for(auto & c : callbacks)
        {
            if(!std::invoke(c.f_callback, args...))
            {
                return false;
            }
        }
        return true;
    }


public:
    /** \brief Add a callback to this manager.
     *
     * This function inserts the given callback to this manager. By default,
     * the new callback goes at the end of the list of callbacks. To add the
     * item at a different location, the callback can be given a priority.
     * Higher numbers get added first. Callbacks with the same priority
     * get sorted in the order they are added.
     *
     * The number of callbacks is not limited.
     *
     * It is possible to add a callback while within a callback function.
     * However, the new callback will not be seen until the next time an
     * event occurs and the call() function gets called.
     *
     * \note
     * If the callback type is an object share pointer, then you will be
     * able to call any member function of that object with the call()
     * function. On the other hand, for direct functions, only that one
     * specific function is called. Direct functions can use an std::bind()
     * in order to attach the function to an object at runtime.
     *
     * \note
     * It is possible to add the same callback any number of time. It
     * will get called a number of time equal to the number of times
     * you added it to the callback manager. Duplicity is not easy to
     * test for objects such as std::function and std::bind.
     *
     * \note
     * The identifier is allowed to wrap around, although you need to
     * call the function 2^32 times before it happens. Right now, we
     * assume this never happens so the function doesn't check whether
     * two callbacks get the same identifier.
     *
     * \param[in] callback  The callback to be added to the manager.
     * \param[in] priority  The priority to sort the callback by.
     *
     * \return The callback identifier you can use to remove the callback.
     *
     * \sa remove_callback()
     * \sa call()
     */
    callback_id_t add_callback(value_type callback, priority_t priority = DEFAULT_PRIORITY)
    {
        ++f_next_id;
        if(f_next_id == NULL_CALLBACK_ID)
        {
            ++f_next_id;  // LCOV_EXCL_LINE
        }

        auto it(std::find_if(
              f_callbacks.begin()
            , f_callbacks.end()
            , [priority](auto const & c)
                {
                    // assuming f_next_id doesn't wrap, this is sufficient
                    //
                    return c.f_priority < priority;
                }));

        f_callbacks.emplace(
                      it
                    , f_next_id
                    , callback
                    , priority
                );

        return f_next_id;
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
     * The parameter used to remove a callback is the callback identifier
     * returned by the add_callback() function.
     *
     * \note
     * Calling the function more than once with the same identifier is
     * allowed, although after the first call, nothing happens.
     *
     * \param[in] callback_id  The identifier of the callback to be removed.
     *
     * \return true if a callback was removed, false otherwise.
     *
     * \sa add_callback()
     * \sa call()
     */
    bool remove_callback(callback_id_t callback_id)
    {
        auto it(std::find_if(
              f_callbacks.begin()
            , f_callbacks.end()
            , [callback_id](auto const & c)
                {
                    return c.f_id == callback_id;
                }));
        if(it == f_callbacks.end())
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
     * \tparam U  A copy of the callback type.
     * \param[in] func  The member function to be called.
     * \param[in] args  The arguments to pass to the callback functions.
     *
     * \return true if all the callback functions returned true.
     */
    template<typename F, typename ... ARGS, typename U = T>
    typename std::enable_if<std::is_same<U, T>::value
                && std::is_member_function_pointer<F>::value
                && is_shared_ptr<U>::value
            , bool>::type
    call(F func, ARGS ... args)
    {
        return call_member_pointer(func, args...);
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
     * \tparam F  The type of member function.
     * \tparam ARGS  The type of each of the function arguments.
     * \tparam U  A copy of the callback type.
     * \param[in] func  The member function to be called.
     * \param[in] args  The arguments to pass to the callback functions.
     *
     * \return true if all the callback functions returned true.
     */
    template<typename F, typename ... ARGS, typename U = T>
    typename std::enable_if<std::is_same<U, T>::value
                && std::is_member_function_pointer<F>::value
                && !is_shared_ptr<U>::value
            , bool>::type
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
     * \tparam U  A copy of the callback type.
     * \param[in] args  The arguments to pass to the callback functions.
     *
     * \return true if all the callback functions returned true.
     */
    template<typename ... ARGS, typename U = T>
    typename std::enable_if<std::is_same<U, T>::value
            && !is_shared_ptr<U>::value, bool>::type
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
    callbacks_t     f_callbacks = callbacks_t();


    /** \brief The next idenfitier.
     *
     * Each time you call the add_callback() function, this identifier gets
     * incremented by one. You can record that number to later remove that
     * specific callback from the list. If you never need to remove the
     * callback or you can call the clear() function, then there is no need
     * to save the callback identifier on return.
     */
    callback_id_t   f_next_id = NULL_CALLBACK_ID;
};



} // namespace snapdev
// vim: ts=4 sw=4 et
