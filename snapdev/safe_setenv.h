// Copyright (c) 2013-2022  Made to Order Software Corp.  All Rights Reserved
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

namespace snapdev
{

/** \brief Set an environment variable, context safe.
 *
 * This class is used to create objects setting an environment variable
 * safely. That is, if an exception occurs or when the current block
 * exits, the class will unset the variable automatically.
 *
 * \code
 *     {
 *         safe_setenv myvar("TEST_XYZ", "123");
 *
 *         ...your test(s)...
 *     }
 *     // here the variable was already unset and it won't pollute your
 *     // next test
 * \endcode
 *
 * Note that the old value is lost once your exit the block, destroy your
 * object, etc. If you instead want to restore the old value, use the
 * transparent_setenv class which saves the old value and restores it
 * for you.
 *
 * \note
 * This class is not thread safe.
 *
 * \sa transparent_setenv
 */
class safe_setenv
{
public:
    /** \brief Define the name and value of the environment variable.
     *
     * This function sets an environment variable. By default, the value
     * of the variable gets overwritten even if it is already set.
     *
     * On destruction, the variable gets unset (i.e. old values are
     * not restored.)
     *
     * \param[in] name  The name of the variable being set.
     * \param[in] value  The value of the variable.
     * \param[in] overwrite  Whether to overwrite the value or not.
     */
    safe_setenv(std::string const & name, std::string const & value, bool overwrite = true)
        : f_name(name)
    {
        if(f_name.empty())
        {
            throw std::logic_error("the name of an environment variable cannot be empty.");
        }

        if(setenv(name.c_str(), value.c_str(), overwrite ? 1 : 0) != 0)
        {
            if(errno == EINVAL)
            {
                throw std::logic_error("the name of an environment variable cannot include an equal character.");
            }
            if(errno == ENOMEM)
            {
                throw std::bad_alloc();
            }
            int const e(errno);
            throw std::runtime_error("setenv() generated an unknown error: "
                                   + std::to_string(e)
                                   + ", "
                                   + strerror(e));
        }
    }

    /** \brief Delete the environment variable.
     *
     * The destructor makes sure that the environment variable gets deleted.
     */
    ~safe_setenv()
    {
        unsetenv(f_name.c_str());
    }

    safe_setenv(safe_setenv const & rhs) = delete;
    safe_setenv & operator = (safe_setenv const & rhs) = delete;

private:
    /** \brief The name of the variable we're dealing with.
     *
     * This variable member holds the name of the variable. We need it
     * on destruction which is why we keep it around.
     */
    std::string     f_name = std::string();
};



/** \brief Change the value of an environment variable within a context.
 *
 * This class is used to set an environment within a block and restore
 * its value once the block is exited. This can be particularly useful
 * for variables such as `HOME`.
 *
 * This is very similar to the safe_setenv except that the old value
 * gets restored on destruction.
 *
 * \code
 *     {
 *         transparent_setenv myvar("TEST_XYZ", "123");
 *
 *         ...your test(s)...
 *     }
 *     // here the variable was set back to its old value
 * \endcode
 *
 * \note
 * This class is not thread safe.
 *
 * \sa safe_setenv
 */
class transparent_setenv
{
public:
    /** \brief Define the name and value of the environment variable.
     *
     * This function sets an environment variable. By default, the value
     * of the variable gets overwritten even if it is already set.
     *
     * On destruction, the variable gets unset (i.e. old values are
     * not restored.)
     *
     * \param[in] name  The name of the variable being set.
     * \param[in] value  The value of the variable.
     * \param[in] overwrite  Whether to overwrite the value or not.
     */
    transparent_setenv(std::string const & name, std::string const & value, bool overwrite = true)
        : f_name(name)
    {
        if(f_name.empty())
        {
            throw std::logic_error("the name of an environment variable cannot be empty.");
        }

        char const * old_value(getenv(name.c_str()));
        f_has_old_value = old_value != nullptr;
        if(f_has_old_value)
        {
            f_old_value = old_value;
        }

        if(setenv(name.c_str(), value.c_str(), overwrite ? 1 : 0) != 0)
        {
            if(errno == EINVAL)
            {
                throw std::logic_error("the name of an environment variable cannot include an equal character.");
            }
            if(errno == ENOMEM)
            {
                throw std::bad_alloc();
            }
            int const e(errno);
            throw std::runtime_error("setenv() generated an unknown error: "
                                   + std::to_string(e)
                                   + ", "
                                   + strerror(e));
        }
    }

    /** \brief Restore the environment variable to its old state.
     *
     * The destructor restore the environment variable to its old value.
     *
     * If the variable did not exist at the time the transparent_setenv
     * object was created, then this function unset()'s the variable.
     */
    ~transparent_setenv()
    {
        if(f_has_old_value)
        {
            setenv(f_name.c_str(), f_old_value.c_str(), 1);
        }
        else
        {
            unsetenv(f_name.c_str());
        }
    }

    transparent_setenv(transparent_setenv const & rhs) = delete;
    transparent_setenv & operator = (transparent_setenv const & rhs) = delete;

private:
    /** \brief The name of the variable we're dealing with.
     *
     * This variable member holds the name of the variable. We need it
     * on destruction which is why we keep it around.
     */
    std::string     f_name = std::string();

    /** \brief The old value of the variable.
     *
     * This string holds the old value of the variable so it can be restored
     * on destruction of this object.
     */
    std::string     f_old_value = std::string();

    /** \brief Whether an old value was defined.
     *
     * Since the old value could be an empty string, we have a flag to
     * know whether the value was set or not (i.e. `getenv()` returned
     * a nullptr or not.)
     */
    bool            f_has_old_value = false;
};



} // namespace snapdev
// vim: ts=4 sw=4 et
