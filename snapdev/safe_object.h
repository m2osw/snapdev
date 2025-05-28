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
 * \brief Implementation of a safe object template.
 *
 * Once in a while, an object (such as a C resource) is created and it is
 * a bare pointer for some time. This template allows you to make sure that
 * the object will either be deleted (i.e. if an exception or early return
 * happens) or safely saved in a place where it will be remembered properly.
 *
 * You want to supply the bare pointer type, the pointer itself, and when
 * required a deleter. By default, the deleter is simply uses the delete
 * operator.
 *
 * The main difference with this safe_object template, compared to using
 * a smart pointer, is that you have the ability to safely take the pointer
 * out of the safe_object with the release() function. After that one
 * call, the object pointer is set to nullptr and thus the safe_object
 * will not release it.
 *
 * \code
 *     {
 *         snapdev::safe_object<my_type *> so;  // create the safe object first
 *         my_type * obj(new my_type);          // create your object
 *         so.make_safe(obj);                   // make it safe for a while
 *
 *         ...your statements while obj isn't safe...
 *
 *         // if an interrupt happened before this statement, then obj
 *         // was safely deleted
 *         //
 *         so.release();
 *     }
 * \endcode
 *
 * You should not try to use the `safe_object<>` template as a replacement
 * of the `shared_ptr<>` template. It is not the same. You cannot
 * copy a `safe_object<>`.
 */

namespace snapdev
{

namespace detail
{



template<typename T>
void default_safe_object_deleter(T ptr)
{
    delete ptr;
}



} // namespace detail


/** \brief Make a pointer of a given type safe.
 *
 * This class is used to make a variable safe. This is important if you
 * allocate a resource, do further initialization that can throw, and then
 * safe the resource pointer as required.
 *
 * This is particularly useful if you need to allocate multiple objects
 * in a row and need to save pointers until all can safely be saved in a
 * final destination.
 *
 * To handle a bare pointer, make sure to pass that as the type T.
 *
 * \tparam T  The type of the value to save and restore.
 * \tparam deleter  A function called when the object needs to be deleted.
 */
template<typename T, void(deleter)(T) = detail::default_safe_object_deleter>
class safe_object
{
public:
    /** \brief Create a safe_object on your stack.
     *
     * This object is expected to be created on your stack.
     *
     * An object of this type cannot be copied. It is expected to only be
     * used on the stack so it goes out of scope at the next `}`.
     *
     * \note
     * For that reason the copy and assignment operators are deleted.
     */
    safe_object()
    {
    }

    /** \brief Prevent copying safe_object's.
     *
     * Delete the copy operator so you can't copy these objects. There is not
     * reference counter or similar feature so we need to make sure you
     * cannot copy (like unique_ptr) or transfer (like auto_ptr) ownership.
     */
    safe_object(safe_object const &) = delete;

    /** \brief Prevent assignment of safe_object's.
     *
     * Delete the assignment operator so you can't copy these objects. There
     * is not reference counter or similar feature so we need to make sure you
     * cannot copy (like unique_ptr) or transfer (like auto_ptr) ownership.
     */
    safe_object & operator = (safe_object const &) = delete;

    /** \brief Delete the value is not yet released.
     *
     * On destruction, the value gets deleted by the defaulr ot user
     * specified deleter.
     */
    ~safe_object()
    {
        cleanup();
    }

    /** \brief Update the value through the safe object.
     *
     * This function allows you to update the value. Since you are
     * expected to have direct access to the variable saved in this
     * object, it is likely not very useful to call this function.
     * It is here primarily for completeness.
     *
     * \warning
     * This function calls the cleanup() function to make sure that
     * if a previous object was present, it gets deleted.
     *
     * \param[in] object  The object to protect from exceptions.
     */
    void make_safe(T object)
    {
        if(object != f_object)
        {
            cleanup();
            f_released = false;
            f_object = object;
        }
    }

    /** \brief Release the value.
     *
     * This function releases the value, which means the deleter will
     * not be called against it.
     *
     * This or another object can be re-added by calling the
     * make_safe() function again.
     */
    void release()
    {
        f_released = true;
    }

    /** \brief Get a reference to the saved value.
     *
     * This function returns a reference to the saved value. Since it
     * is a constant, returning a reference is safe even in a
     * multi-threaded environment.
     *
     * If you used the constructor with a `restore` parameter, then that
     * value is returned by this function.
     *
     * \note
     * It is not possible to change the saved value.
     *
     * \return The protected object.
     */
    T protected_object() const
    {
        return f_object;
    }

    /** \brief Clean up the object if not yet released.
     *
     * This function makes sure to call the deleter on the currently
     * protected object, contrary to the release() function which
     * makes sure not to call the deleter.
     */
    void cleanup()
    {
        if(!f_released)
        {
            f_released = true;
            deleter(f_object);
        }
    }

private:
    /** \brief The object to protect.
     *
     * This field is the object to protect. In most cases, this is going
     * to be a pointer. It could very well be an identifier or another type
     * of resource handle.
     */
    T f_object = T();

    /** \brief Whether the value was released or not.
     *
     * This value tells us whether the release() function was called (true)
     * or not (false).
     *
     * When this field is `true` the deleter is not called against
     * f_object. By default, this value is true.
     */
    bool f_released = true;
};



} // namespace snapdev
// vim: ts=4 sw=4 et
