# Copyright (c) 2013-2022  Made to Order Software Corp.  All Rights Reserved.
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
# 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA


##
## build the snapmatrix command line tool
##
project(snapmatrix)

add_executable(${PROJECT_NAME}
    snapmatrix.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${SNAPDEV_INCLUDE_DIRS}
        ${BOOST_INCLUDE_DIRS}
        ${LIBEXCEPT_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    ${LIBEXCEPT_LIBRARIES}
)

install(
    TARGETS
        ${PROJECT_NAME}

    RUNTIME DESTINATION
        bin

    COMPONENT
        runtime
)


##
## build the 128bits command line tool
##
project(int128)

add_executable(${PROJECT_NAME}
    int128.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${SNAPDEV_INCLUDE_DIRS}
        ${BOOST_INCLUDE_DIRS}
        ${LIBEXCEPT_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    ${LIBEXCEPT_LIBRARIES}
)

install(
    TARGETS
        ${PROJECT_NAME}

    RUNTIME DESTINATION
        bin

    COMPONENT
        runtime
)


# vim: ts=4 sw=4 et nocindent
