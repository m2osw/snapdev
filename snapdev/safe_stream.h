// Copyright (c) 2005-2023  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Save stream flags.
 *
 * When writing to a stream, you may change the flags with functions such
 * as `std::hex` or `std::setw()`. This class helps you save the current
 * state in order to be able to restore it later.
 *
 * Another way to avoid changing the flags of a stream is to use an
 * `std::stringstream`, do all the necessary write in that stream and
 * then copy the result to the other stream. This is not always possible
 * and this is when this class comes in handy.
 */

// C++
//
#include    <sstream>



namespace snapdev
{


/** \class safe_stream
 * \brief A class used to safely handle stream flags, width, and precision.
 *
 * Create an object of this type on your stack, and the flags, width,
 * and precision of your standard streams will be safe-guarded.
 *
 * See the constructor for an example.
 *
 * \note
 * Although you may use this class within another class, it is not likely
 * to work well. It is really meant to be used on the stack only.
 */
template<typename CharT, typename Traits>
class safe_stream
{
public:
    /** \brief Save the current format of a stream.
     *
     * This function saves the various format data of a stream. This
     * includes the flags, precision, and width as well as the user
     * defined format data.
     *
     * The destructor automatically restores the flags. The
     * restore() function can also be called early, although that
     * eventually breaks the RAII feature since restore only
     * restores the flags once. Further calls to the restore()
     * function do nothing.
     *
     * To use:
     *
     * \code
     *   {
     *      as2js::safe_stream stream_flags(std::cout);
     *      ...
     *      // this call changes the flags to write numbers in hexadecimal
     *      std::cout << std::hex << 123 << ...;
     *      ...
     *   } // here all flags, width, precision get restored automatically
     * \endcode
     *
     * \param[in] s  The stream of which flags are to be saved.
     */
    safe_stream(std::basic_ios<CharT, Traits> & s)
        : f_stream(&s)
        //, f_flags(s.flags())
        //, f_precision(s.precision())
        //, f_width(s.width())
    {
        f_copyfmt.copyfmt(*f_stream);
    }


    /** \brief Restore the flags, width, and precision of a stream.
     *
     * The destructor automatically restores the stream flags, width,
     * and precision when called. Putting such an object on the stack
     * is the safest way to make sure that your function does not leak
     * the stream flags, width, and precision.
     *
     * This function calls the restore() function. Note that restore()
     * has no effect when called more than once.
     */
    ~safe_stream()
    {
        restore();
    }


    /** \brief The restore function copies the flags, width, and precision
     *         back in the stream.
     *
     * This function restores the flags, width, and precision of the stream
     * as they were when the object was passed to the constructor of this
     * object.
     *
     * The function can be called any number of time, however, it only
     * restores the flags, width, and precision the first time it is called.
     *
     * In most cases, you want to let your safe_stream object
     * destructor call this restore() function automatically, although
     * you may need to restore the format early once in a while.
     */
    void restore()
    {
        if(f_stream != nullptr)
        {
            f_stream->copyfmt(f_copyfmt);
        }
    }


    /** \brief Copying these objects is not allowed.
     *
     * This object cannot be duplicated.
     */
    safe_stream(safe_stream const & rhs) = delete;


    /** \brief Copying these objects is not allowed.
     *
     * This object cannot be duplicated.
     */
    safe_stream & operator = (safe_stream const  & rhs) = delete;


private:
    /** \brief Stream being made safe.
     *
     * This points to the stream for which this class saves and
     * restores the state.
     *
     * We use a bare pointer because we want to be able to set it to
     * a nullptr once it was restored. We can only restore once. After
     * that, you are on your own.
     */
    std::basic_ios<CharT, Traits> * f_stream = nullptr;

    /** \brief This other stream is used to save the state.
     *
     * Any stream will do, I use the stringstream because it works.
     * If there is a simpler \em thing I can use, let me know!
     */
    std::basic_stringstream<CharT, Traits> f_copyfmt = std::basic_stringstream<CharT, Traits>();
};



} // namespace snapdev
// vim: ts=4 sw=4 et
