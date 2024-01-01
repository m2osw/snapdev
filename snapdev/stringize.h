// Copyright (c) 2023-2024  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Transform a value in a string at preprocessor time.
 *
 * It is often useful to transform a value to a string at compile time so
 * it can integrated in a string. This preprocessor macro can be used for
 * that purpose.
 *
 * \todo
 * Eventually offer versions that work with all preprocessors--i.e. see
 * the corresponding boost file:
 *
 * \code
 *     /usr/include/boost/preprocessor/stringize.hpp
 * \endcode
 */

#define SNAPDEV_STRINGIZE(text) SNAPDEV_STRINGIZE_I(text)
#define SNAPDEV_STRINGIZE_I(text) #text

// vim: ts=4 sw=4 et
