# Copyright (c) 2006-2024  Made to Order Software Corp.  All Rights Reserved
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
## SnapDev Unit Tests
##
project(unittest)

find_package(SnapCatch2)

if(SnapCatch2_FOUND)

    add_executable(${PROJECT_NAME}
        catch_main.cpp

        catch_as_root.cpp
        catch_assert.cpp
        catch_brs.cpp
        catch_callback_manager.cpp
        catch_change_owner.cpp
        catch_file_contents.cpp
        catch_glob_to_list.cpp
        catch_hexadecimal_string.cpp
        catch_int128.cpp
        catch_isatty.cpp
        catch_join_strings.cpp
        catch_log2.cpp
        catch_matrix.cpp
        catch_memsearch.cpp
        catch_mkdir_p.cpp
        catch_number_to_string.cpp
        catch_pathinfo.cpp
        catch_remove_duplicates.cpp
        catch_rm_r.cpp
        catch_safe_object.cpp
        catch_safe_stream.cpp
        catch_saturated_add.cpp
        catch_saturated_subtract.cpp
        catch_stringize.cpp
        catch_timespec_ex.cpp
        catch_tokenize_format.cpp
        catch_tokenize_string.cpp
        catch_to_string_literal.cpp
        catch_trim_string.cpp
        catch_user_groups.cpp
        catch_version.cpp
    )

    target_include_directories(${PROJECT_NAME}
        PUBLIC
            ${SNAPCATCH2_INCLUDE_DIRS}
            ${LIBEXCEPT_INCLUDE_DIRS}
    )

    target_link_libraries(${PROJECT_NAME}
        ${LIBEXCEPT_LIBRARIES}
        ${SNAPCATCH2_LIBRARIES}
    )

    add_test(unitest ${PROJECT_NAME})

else(SnapCatch2_FOUND)

    message("snapcatch2 not found... no test will be built.")

endif(SnapCatch2_FOUND)

# vim: ts=4 sw=4 et
