// Copyright (c) 2011-2025  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Retrieve the next unique number.
 *
 * The unique_number function reads a file for a unique number. That number
 * is managed by this function. The file is locked so any number of threads
 * and processes can call the function safely.
 *
 * The number is managed as a 128 bits number. You may cast it down to any
 * number of bits as required by your function.
 *
 * The function also accepts an index which gives you the ability to create
 * any number of counters within one file. This is particularly useful if
 * your service makes use of many different counter. Instead of create a
 * separate file for each counter, reuse the same file with varying
 * indexes. This is likely faster since the kernel only needs to cache one
 * single file.
 */

// libexcept
//
#include    <libexcept/exception.h>


// snapdev
//
#include    <snapdev/lockfile.h>
#include    <snapdev/raii_generic_deleter.h>


// C
//
#include    <sys/file.h>
//#include    <unistd.h>
//#include    <string.h>



namespace snapdev
{

DECLARE_MAIN_EXCEPTION(unique_number_error);

DECLARE_EXCEPTION(unique_number_error, io_error);
DECLARE_EXCEPTION(unique_number_error, out_of_range);
DECLARE_EXCEPTION(unique_number_error, path_missing);



/** \brief Limit the size of one counter file to 1Mb.
 *
 * This is the limit of the index passed to the unique_number(). If you
 * pass a negative number of a number equal or larger to this number,
 * then the function raises an error.
 */
constexpr int const COUNTER_MAXIMUM_INDEX = 65536;


/** \brief Manage a unique number within a file.
 *
 * This function handles a unique number. It creates a file if it
 * does not exist yet and starts counting. Each time you call the
 * function, it returns the current counter value plus 1. The very
 * first time, it returns 1. The counter is 128 bits so rather
 * unlikely to wrap around.
 *
 * The \p counter parameter holds the path and filename to the
 * counter which gets saved to file.
 *
 * The \p index parameter can be set to a number larger than 0 to
 * reference a different counter. That way, you can make use of the
 * same file for many counters, which is likely going to be faster
 * and reduce the amount of disk space necessary to manage the counters.
 *
 * The index cannot be negative or larger or equal to COUNTER_MAXIMUM_INDEX.
 *
 * For a specific counter, the number returned is unique (assuming no
 * wrapping occurs) for the computer it exists on. To make it
 * unique in a cluster, consider adding the computer host name. Similarly,
 * to make it unique anywhere, also add the cluster unique identifier.
 *
 * \note
 * The index can be used to create counters at any index. Intermediate
 * counters do not need to exist. So if you use 100, it will create a
 * counter at offset 100 * 16 (unsigned __int128 is 16 bytes). This
 * means the file may end up being a sparse file.
 *
 * \exception path_missing
 * This exception is raised if the \p counter path is an empty string.
 *
 * \exception out_of_range
 * This exception is raised if the index is negative or larger or
 * equal to the limit, COUNTER_MAXIMUM_INDEX.
 *
 * \exception io_error
 * This exception is raised if an I/O error is detected. This includes
 * opening, seeking, reading, and writing to the file.
 *
 * \param[in] counter  The path to the counter file.
 * \param[in] index  The counter index starting at 0.
 *
 * \return The current counter after 1 was added to it.
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
inline unsigned __int128 unique_number(std::string const & counter, int index = 0)
{
    if(counter.empty())
    {
        throw path_missing("a counter filename must be specified when calling snapdev::unique_number.");
    }
    if(static_cast<std::size_t>(index) >= static_cast<std::size_t>(COUNTER_MAXIMUM_INDEX))
    {
        throw out_of_range(
                  "counter index in unique_number must be defined between 0 and "
                + std::to_string(COUNTER_MAXIMUM_INDEX - 1)
                + " inclusive.");
    }

    raii_fd_t fd(::open(counter.c_str(), O_RDWR | O_CREAT | O_CLOEXEC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH));
    if(fd == nullptr)
    {
        throw io_error("could not open unique_number file \"" + counter + "\".");
    }

    // make sure we are exclusive
    //
    lockfd lock(fd.get(), operation_t::OPERATION_EXCLUSIVE);

    unsigned __int128 result(0);
    off_t const position(index * sizeof(result));
    if(lseek(fd.get(), position, SEEK_SET) != position)
    {
        throw io_error("could not properly lseek() unique_number file \"" + counter + "\" to read the counter."); // LCOV_EXCL_LINE
    }

    // read errors are expected when calling the function for the first time
    //
    if(read(fd.get(), &result, sizeof(result)) != sizeof(result))
    {
        result = 0;
    }

    ++result;

    if(lseek(fd.get(), position, SEEK_SET) != position)
    {
        throw io_error("could not properly lseek() unique_number file \"" + counter + "\" to write the counter."); // LCOV_EXCL_LINE
    }
    if(write(fd.get(), &result, sizeof(result)) != sizeof(result))
    {
        throw io_error("could not properly save() unique_number to file \"" + counter + "\"."); // LCOV_EXCL_LINE
    }

    return result;
}
#pragma GCC diagnostic pop



} // namespace snapdev
// vim: ts=4 sw=4 et
