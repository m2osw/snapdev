# Copyright (c) 2006-2022  Made to Order Software Corp.  All Rights Reserved.
#
# https://snapwebsites.org/project/snapdev
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
        brs.h
        callback_manager.h
        case_insensitive_string.h
        chownnm.h
        empty_set_intersection.h
        enumerate.h
        file_contents.h
        gethostname.h
        glob_to_list.h
        has_member_function.h
        hexadecimal_string.h
        init_structure.h
        int128_literal.h
        is_smart_pointer.h
        is_string_literal.h
        is_vector.h
        join_strings.h
        lockfile.h
        log2.h
        map_keyset.h
        matrix.h
        mkdir_p.h
        mounts.h
        not_reached.h
        not_used.h
        numeric_string.h
        ostream_int128.h
        ostream_to_buf.h
        pathinfo.h
        poison.h
        qcaseinsensitivestring.h
        qstring_extensions.h
        raii_generic_deleter.h
        reverse_cstring.h
        safe_assert.h
        safe_setenv.h
        safe_variable.h
        string_replace_many.h
        timespec_operations.h
        timestamp.h
        tokenize_string.h
        to_lower.h
        to_upper.h
        trim_string.h
        user_groups.h
        ${CMAKE_CURRENT_BINARY_DIR}/version.h

    DESTINATION
        include/snapdev
)

# vim: ts=4 sw=4 et nocindent
