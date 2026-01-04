// Copyright (c) 2016-2025  Made to Order Software Corp.  All Rights Reserved
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

// C++
//
#include    <algorithm>
#include    <functional>
#include    <iostream>
#include    <string>

// C
//
#include    <unistd.h>



namespace snapdev
{



/** \brief Retrieve a value from the environment.
 *
 * This function is the default callback which converts a variable name
 * to its content from the environment variables.
 *
 * If the variable is not defined, then an empty string is returned.
 *
 * \param[in] name  The name of the variable to retrieve.
 *
 * \return The value of the \p name'd environment variable.
 */
std::string getenv_values(std::string const & name)
{
    char const * const e(getenv(name.c_str()));
    return e == nullptr ?  std::string() : std::string(e);
}


/** \brief Replace variable references with their value.
 *
 * This function parses a string for variable references similar to a
 * shell script:
 *
 * \code
 *     $VARNAME
 *     -- or --
 *     ${VARNAME}
 * \endcode
 *
 * When not using the curly braces, the variable names are all the characters
 * that are supported in a variable: A-Z, a-z, 0-9, and the underscore. The
 * name cannot start with a number.
 *
 * The variable gets replaced by calling the specified function. By default
 * this is using getenv().
 *
 * We also support the following special variables:
 *
 * \li `$$` -- the pid of the current process (getpid()); in this case
 *     the callback does not get called
 *
 * \note
 * A $ at the end of the input string is kept as is.
 *
 * \note
 * An unterminated `${...}` reference is included in the output as is (no
 * replacement). Note that such a reference ends if a character which is
 * not a valid variable name character is found in the input. If that
 * character is '}' and the name is not empty, then we replace the variable
 * with its value. In all other cases we keep the input as is.
 *
 * \param[in] input  The input string where replacements occur.
 * \param[in] callback  The function used to get the variable values.
 *
 * \return A new string with the variables replaced.
 */
template<class StringT>
StringT string_replace_variables(
      StringT const & input
    , std::function<std::string(std::string const &)> callback = getenv_values)
{
    // note: the variable names only support ASCII (A-Z, a-z, 0-9, _) so
    //       there is no need to handle UTF-8 here
    //
    auto is_variable_start_character = [](char c)
    {
        return (c >= 'A' && c <= 'Z')
            || (c >= 'a' && c <= 'z')
            || c == '_';
    };
    auto is_variable_character = [is_variable_start_character](char c)
    {
        return is_variable_start_character(c) || (c >= '0' && c <= '9');
    };

    StringT result;
    result.reserve(input.length() * 2);

    for(char const * s(input.c_str()); *s != '\0'; )
    {
        if(*s == '$')
        {
            char const * start(s);
            ++s;
            if(*s == '\0')
            {
                // special case of an input string ending with '$'
                //
                result += '$';
                break;
            }
            std::string name;
            switch(*s)
            {
            case '{':
                // in this case, we search for the '}'
                //
                // TODO: add support for ':' + flags (i.e. default value, etc.)
                //
                ++s;
                if(is_variable_start_character(*s))
                {
                    for(++s; is_variable_character(*s); ++s);
                }
                if(*s == '}')
                {
                    // we got a valid name
                    //
                    name = std::string(start + 2, s - start - 2);
                    ++s;
                }
                break;

            case '$':
                result += std::to_string(getpid());
                ++s;
                continue;

            default:
                for(; is_variable_character(*s); ++s);
                name = std::string(start + 1, s - start - 1);
                break;

            }
            if(name.empty())
            {
                // not a valid variable reference
                //
                result += std::string(start, s - start);
            }
            else
            {
                result += callback(name);
            }
        }
        else
        {
            result += *s;
            ++s;
        }
    }

    return result;
} // LCOV_EXCL_LINE



} // namespace snapdev
// vim: ts=4 sw=4 et
