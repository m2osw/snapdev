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

// self
//
#include    <snapdev/safe_tcattr.h>
#include    <snapdev/timespec_ex.h>


// C
//
//#include <stdio.h>
//#include <stdlib.h>
//#include <unistd.h>
//#include <termios.h>
#include    <poll.h>


namespace snapdev
{



/** \brief Do a getch() with a timeout.
 *
 * This function allows for a timed getch(). Give the user a small amount
 * of time to answer a question, otherwise operate with some defaults.
 *
 * \todo
 * We may want to extend this so stdin is not implied.
 *
 * \param[in] timeout  The maximum amount of time to wait.
 * \param[in] echo  Whether to echo letters typed by the user.
 *
 * \return the character, 0 on a timeout, or -1 on error
 */
int timed_getchar(timespec_ex const & timeout, bool echo = true)
{
    // save current terminal settings
    //
    safe_tcattr safe_terminal_attributes;

    // disable canonical mode (line buffering)
    //
    struct termios attr;
    safe_terminal_attributes.get_original_attributes(attr);
    attr.c_lflag &= ~ICANON;
    if(!echo)
    {
        // if requested, also disable echoing
        //
        attr.c_lflag &= ~ECHO;
    }
    int r(safe_terminal_attributes.set_attributes(TCSAFLUSH, attr));
    if(r != 0)
    {
        return -1;
    }

    // set up the file descriptor to listen to
    //
    struct pollfd fds = {
        .fd = safe_terminal_attributes.get_terminal(),
        .events = POLLIN,
        .revents = 0,
    };

    // wait for input or timeout
    //
    r = ppoll(&fds, 1, &timeout, nullptr);
    if(r > 0)
    {
        // input is available; return it
        //
        return getchar();
    }
    if(r == 0)
    {
        // timed out
        //
        return 0;
    }

    return -1;
}



} // namespace snapdev
// vim: ts=4 sw=4 et
