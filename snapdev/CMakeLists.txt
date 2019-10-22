#
# File:
#      snapdev/CMakeLists.txt
#
# Description:
#      Build the snapdev library
#
# License:
#      Copyright (c) 2006-2019  Made to Order Software Corp.  All Rights Reserved.
#
#      https://snapwebsites.org/
#      contact@m2osw.com
# 
#      Permission is hereby granted, free of charge, to any person obtaining a copy
#      of this software and associated documentation files (the "Software"), to deal
#      in the Software without restriction, including without limitation the rights
#      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#      copies of the Software, and to permit persons to whom the Software is
#      furnished to do so, subject to the following conditions:
# 
#      The above copyright notice and this permission notice shall be included in
#      all copies or substantial portions of the Software.
# 
#      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
#      THE SOFTWARE.
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
        join_strings.h
        lockfile.h
        matrix.h
        not_reached.h
        not_used.h
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
