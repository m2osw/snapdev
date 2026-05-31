// Copyright (c) 2026  Made to Order Software Corp.  All Rights Reserved
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
 * \brief An RAII tcsetattr() class.
 *
 * This class allows you to save a copy of the current terminal settings and
 * once the class is destroyed, to restore those settings automatically.
 */

// self
//
#include    <snapdev/timespec_ex.h>


// C
//
#include    <termios.h>



namespace snapdev
{

/** \brief Save the current terminal settings.
 *
 * This class is used to safely modify terminal settings until done with a
 * task and then restore them.
 */
class safe_tcattr
{
public:
    /** \brief Save the current terminal settings.
     *
     * This function saves the current terminal settings.
     *
     * \param[in] terminal  The file descriptor of the concerned terminal.
     */
    safe_tcattr(int terminal = STDIN_FILENO)
        : f_terminal(terminal)
    {
        int const r(tcgetattr(f_terminal, &f_attributes));
        if(r != 0)
        {
            int const e(errno);
            throw std::runtime_error(
                      "tcgetattr() failed: "
                    + std::to_string(e)
                    + ", "
                    + strerror(e));
        }
    }

    /** \brief Restore the terminal settings.
     *
     * The destructor restores the terminal settings as they were at the
     * time the safe_tcattr object was created.
     *
     * \note
     * Failure to restore the current path is ignored.
     */
    ~safe_tcattr()
    {
        NOT_USED(restore_attributes());
    }

    safe_tcattr(safe_tcattr const & rhs) = delete;
    safe_tcattr & operator = (safe_tcattr const & rhs) = delete;

    /** \brief Restore the attributes as they were on construction.
     *
     * This function can be used an any time to restore the attributes
     * as they were at the time the safe_tcattr object was created.
     * This is automatically called when the class is destroyed.
     *
     * Calling this function gives you access to the returned value.
     *
     * \todo
     * The restore uses TCSANOW as the action. It is not clear whether
     * a different action would be necessary in this case (probably
     * if we are dealing with output data).
     *
     * \return 0 when no error occurs; 1 on error.
     */
    int restore_attributes()
    {
        return tcsetattr(f_terminal, TCSANOW, &f_attributes);
    }

    /** \brief Retrieve the terminal file descriptor.
     *
     * This function can be used to retrieve the terminal file descriptor
     * as specified in the constructor.
     *
     * \return The terminal file descriptor.
     */
    int get_terminal() const
    {
        return f_terminal;
    }

    /** \brief Get a copy of the attributes.
     *
     * This function retrieves a copy of the attributes as collected
     * on construction.
     *
     * \param[out] attr  Where to save the attributes.
     */
    void get_original_attributes(termios & attr)
    {
        attr = f_attributes;
    }

    /** \brief Retrieve the current terminal attributes.
     *
     * This function can be used to retrieve the current attributes
     * of the terminal attached to this safe_tcattr object.
     *
     * \param[in] attr  The structure where the attributes are saved.
     *
     * \return 0 on success, 1 on error
     */
    int get_attributes(termios & attr)
    {
        return tcgetattr(f_terminal, &attr);
    }

    /** \brief Set the specified attributes in the terminal.
     *
     * This function calls the tcsetattr() with the terminal specified in
     * the constructor so you can be sure you are referring the same terminal
     * when changing the attributes.
     *
     * The optional_actions may be set to:
     *
     * \li 0 -- no specific action
     * \li TCSANOW -- apply changes immediately
     * \li TCSADRAIN -- change applies after existing output is transmitted
     * \li TCSAFLUSH -- like TCSADRAIN and also discard unread input
     *
     * \return 0 when no error occurred, -1 otherwise
     */
    int set_attributes(int optional_actions, termios const & attr)
    {
        return tcsetattr(f_terminal, optional_actions, &attr);
    }

private:
    /** \brief The terminal file descriptor.
     *
     * This parameter holds the terminal which is being tweaked. In most
     * likelihood, this is STDIN_FILENO but it can be any other terminal.
     */
    int             f_terminal = 0;

    /** \brief The original attributes.
     *
     * This variable member holds the original attributes.
     */
    termios         f_attributes = {};
};



} // namespace snapdev
// vim: ts=4 sw=4 et
