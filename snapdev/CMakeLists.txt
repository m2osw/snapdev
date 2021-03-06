#
# Copyright (c) 2006-2019  Made to Order Software Corp.  All Rights Reserved.
#
# https://snapwebsites.org/
# contact@m2osw.com
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#

##
## SnapDev headers
##
project(snapdev)

# Put the version in the header file
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/version.h
)


install(
    FILES
        empty_set_intersection.h
        glob_to_list.h
        hexadecimal_string.h
        init_structure.h
        join_strings.h
        lockfile.h
        log2.h
        matrix.h
        not_reached.h
        not_used.h
        ostream_int128.h
        ostream_to_buf.h
        poison.h
        qcaseinsensitivestring.h
        raii_generic_deleter.h
        reverse_cstring.h
        safe_setenv.h
        safe_variable.h
        string_pathinfo.h
        string_replace_many.h
        tokenize_string.h
        version.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/version.h

    DESTINATION
        include/snapdev
)

# vim: ts=4 sw=4 et nocindent
