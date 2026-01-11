# Copyright (c) 2006-2025  Made to Order Software Corp.  All Rights Reserved.
#
# https://snapwebsites.org/project/snapdev
# contact@m2osw.com
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

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
        as_root.h
        brs.h
        callback_manager.h
        case_insensitive_string.h
        chownnm.h
        clocale.h
        concat_strings.h
        empty_set_intersection.h
        enumerate.h
        enum_class_math.h
        escape_special_regex_characters.h
        file_contents.h
        floating_point_to_string.h
        gethostname.h
        glob_to_list.h
        has_member_function.h
        hexadecimal_string.h
        init_structure.h
        int128_literal.h
        isatty.h
        is_nonnull.h
        is_smart_pointer.h
        is_string_literal.h
        is_vector.h
        join_strings.h
        limits_int128.h
        lockfile.h
        log2.h
        map_keyset.h
        math.h
        matrix.h
        memory_streambuf.h
        mkdir_p.h
        mounts.h
        not_reached.h
        not_used.h
        number_to_string.h
        numeric_string.h
        ostream_int128.h
        ostream_to_buf.h
        pathinfo.h
        poison.h
        qcaseinsensitivestring.h
        qstring_extensions.h
        raii_generic_deleter.h
        remove_duplicates.h
        reverse_cstring.h
        rm_r.h
        safe_assert.h
        safe_chdir.h
        safe_object.h
        safe_setenv.h
        safe_stream.h
        safe_variable.h
        sizeof_bitfield.h
        static_to_dynamic_buffer.h
        stream_fd.h
        stringize.h
        string_literal_length.h
        string_replace_many.h
        string_replace_variables.h
        timespec_ex.h
        timestamp.h
        tokenize_format.h
        tokenize_string.h
        to_lower.h
        to_string_literal.h
        to_upper.h
        trim_string.h
        unique_number.h
        user_groups.h
        username.h
        vector_streambuf.h
        ${CMAKE_CURRENT_BINARY_DIR}/version.h

    DESTINATION
        include/snapdev
)

# vim: ts=4 sw=4 et nocindent
