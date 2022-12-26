// Copyright (c) 2019-2022  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Implementation of a safe variable template.
 *
 * This header defines a safe variable which we can use to save a
 * variable while changing its value within a C++ context (i.e. while
 * running a function, a block of statement, or even in a class.)
 *
 * The only restriction is for the \em safe variable to exist for the
 * entire time the context exists.
 *
 * It is often used to mark a process as active as in, turning a flag on
 * while executing a block of statements.
 *
 * \code
 *     // for a simple boolean flag
 *     {
 *         safe_variable<bool> safe(my_flag, true, false);
 *
 *         ...your statements while `my_flag = true;`...
 *
 *     } // `my_flag = false;` happens here
 *
 *     // any type is supported
 *     {
 *         safe_variable<decltype(my_var)> safe(my_var, 123);
 *
 *         ...your statements while `my_var = 123`...
 *
 *     } // here my_var gets restored to the value it had on entry of this block
 * \endcode
 *
 * The value used to restore your safe variable can be specified on
 * creation of the safe_variable and updated later within your
 * statements. This is quite practical if managing a state.
 */

namespace snapdev
{

/** \brief Set a value context safe.
 *
 * This class is used to save a variable, set it to a specific value,
 * then restore it when leaving the current context. This is very
 * useful to, for example, set a flag to true (or false) while doing
 * some work and then restoring that flag once the work is done.
 *
 * \tparam T  The type of the value to save and restore.
 */
template<typename T>
class safe_variable
{
public:
    /** \brief Safely set a value in a context.
     *
     * This function saves the current value of the \p variable_reference
     * variable and then changes its content to \p value.
     *
     * This is safe because whenever the current context is exited (whether
     * with a break, continue, goto, return, or an exception), the saved
     * value always gets restored in the \p variable_reference variable.
     *
     * \note
     * The referenced variable may be a temporary variable.
     *
     * \param[in] variable_reference  A reference to a variable.
     * \param[in] value  The value to set the variable to.
     */
    safe_variable(T & variable_reference, T const & value)
        : f_variable_reference(variable_reference)
        , f_saved_value(variable_reference)
    {
        f_variable_reference = value;
    }

    /** \brief Safely set and restore a value in a context.
     *
     * This function changes the \p variable_reference variable content
     * to \p value. It also saves the value of \p restore which it will
     * use to set the \p variable_reference once the context exits.
     *
     * This is safe because whenever the current context is exited (whether
     * with a break, continue, goto, return, or an exception), the saved
     * value always gets restored in the \p variable_reference variable.
     *
     * \note
     * The referenced variable may be a temporary variable.
     *
     * \param[in] variable_reference  A reference to a variable.
     * \param[in] value  The value to set the variable to.
     * \param[in] restore  The value to set the variable to on destruction.
     */
    safe_variable(T & variable_reference, T const & value, T const & restore)
        : f_variable_reference(variable_reference)
        , f_saved_value(restore)
    {
        f_variable_reference = value;
    }

    /** \brief Restore the value.
     *
     * On destruction, the value gets restored.
     *
     * The value to restore to may be the old value of the variable or
     * a specific value as specified in the restore parameter.
     *
     * You can restore the value early by calling the restore function
     * explicitly.
     */
    ~safe_variable()
    {
        f_variable_reference = f_saved_value;
    }

    /** \brief Update the value through the safe object.
     *
     * This function allows you to update the value. Since you are
     * expected to have direct access to the variable saved in this
     * object, it is likely not very useful to call this function.
     * It is here primarily for completeness.
     *
     * \param[in] value  The new value to save in the variable.
     */
    void set_value(T const & value)
    {
        f_variable_reference = value;
    }

    /** \brief Restore the value.
     *
     * This function is used to restore the value early. At times
     * you need to have a safe change which does not exactly end with
     * your current context. This function can be used to make the
     * change back to \em normal at an early stage.
     */
    void restore()
    {
        f_variable_reference = f_saved_value;
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
     * \return The saved value.
     */
    T const & saved_value() const
    {
        return f_saved_value;
    }

private:
    /** \brief Reference to the managed variable.
     *
     * This reference points to the variable that you want to handle
     * safely. It is used to read and write to the variable through
     * this class.
     *
     * Especially, it gets used whenever the destructor is called so
     * the value of the variable gets restored to what was saved.
     */
    T &     f_variable_reference;

    /** \brief The value used to restore the variable.
     *
     * This value is what is used to restore the variable at the end
     * of the context.
     *
     * It is a read-only variable member because the restore value
     * cannot be changed en route.
     */
    T const f_saved_value;
};



} // namespace snapdev
// vim: ts=4 sw=4 et
