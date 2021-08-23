// Copyright (c) 2011-2021  Made to Order Software Corp.  All Rights Reserved
//
// https://snapwebsites.org/project/snapdev
// contact@m2osw.com
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#pragma once

// self
//
#include    <snapdev/mkdir_p.h>


// C++ lib
//
#include    <fstream>
#include    <ios>


// C lib
//
#include    <unistd.h>



namespace snap
{



class file_contents
{
public:
    /** \brief Initialize a content file.
     *
     * The constructor initialize the file content object with a filename.
     * The filename is used by the read_all() and write_all() functions.
     *
     * If the file_content is setup to be a temporary file, then the
     * destructor also makes use of the filename to delete the file
     * at that time. By default a file is not marked as temporary.
     *
     * \exception std::invalid_argument
     * The \p filename parameter cannot be an empty string.
     *
     * \exception std::ios::failure
     * The function checks whether all the folders exist. If not then the
     * file can't be create or read so there is no valid file_content()
     * possible with that path. This exception only occurs if the
     * \p create_missing_directories is true and the creation of any
     * of the directories fails.
     *
     * \param[in] filename  The name of the file to read and write.
     * \param[in] create_missing_directories  Whether to create missing directories
     *            as found in the path (see mkdir_p()).
     * \param[in] temporary  Whether the file is temporary.
     *
     * \sa snap::mkdir_p()
     */
    file_contents(
              std::string const & filename
            , bool create_missing_directories = false
            , bool temporary = false)
        : f_filename(filename)
        , f_temporary(temporary)
    {
        if(f_filename.empty())
        {
            throw std::invalid_argument("snapdev::file_contents: the filename of a file_contents object cannot be the empty string.");
        }

        if(create_missing_directories)
        {
            int const r(mkdir_p(f_filename, true));
            if(r != 0)
            {
                throw std::ios::failure("snapdev::file_contents: the full path to filename for a file_contents object could not be created");
            }
        }
    }


    /** \brief Clean up as required.
     *
     * If the file_content was marked as temporary, then the destructor
     * deletes the file on disk before returning.
     *
     * If the file does not exist and it was marked as temporary, the
     * deletion will fail silently. Otherwise you get a warning in the
     * logs with more details about the error (i.e. permission, bad
     * filename, etc.)
     */
    ~file_contents()
    {
        if(f_temporary)
        {
            if(unlink(f_filename.c_str()) != 0)
            {
                if(errno != ENOENT)
                {
                    // TODO: what should we do here?!
                }
            }
        }
    }


    /** \brief Retrieve the filename.
     *
     * This function returns a refrence to the filename used by this
     * object. Note that the filename cannot be modified.
     *
     * \return A reference to this file content filename.
     */
    std::string const & filename() const
    {
        return f_filename;
    }


    /** \brief Check whether this file exists.
     *
     * This function checks whether the file exists on disk. If the
     * read_all() function returns false and this function returns true,
     * then you probably do not have permissions to read the file or it
     * is a directory.
     *
     * \return true if the file exists and can be read.
     *
     * \sa read_all()
     */
    bool exists() const
    {
        return access(f_filename.c_str(), R_OK) == 0;
    }


    /** \brief Read the entire file in a buffer.
     *
     * This function reads the entire file in memory. It saves the data
     * in a buffer which you can get with the contents() function.
     * The file can be binary in which case remember that c_str()
     * and other similar function will not work right.
     *
     * The function may return false if the file could not be opened or
     * read in full.
     *
     * \return true if the file could be read, false otherwise.
     *
     * \sa write_all()
     * \sa contents() const
     * \sa last_error()
     */
    bool read_all()
    {
        // try to open the file
        //
        std::ifstream in;
        in.open(f_filename, std::ios::in | std::ios::binary);
        if(!in.is_open())
        {
            f_error = "could not open file \""
                + f_filename
                + "\" for reading.";
            return false;
        }

        // get size
        //
        in.seekg(0, std::ios::end);
        std::ifstream::pos_type const size(in.tellg());
        in.seekg(0, std::ios::beg);
        if(std::ifstream::pos_type(-1) == size)
        {
            f_error = "could not get size of file \""           // LCOV_EXCL_LINE
                + f_filename                                    // LCOV_EXCL_LINE
                + "\".";                                        // LCOV_EXCL_LINE
            return false;                                       // LCOV_EXCL_LINE
        }

        // resize the buffer accordingly
        //
        try
        {
            f_contents.resize(size, '\0');
        }
        catch(std::bad_alloc const & e)                         // LCOV_EXCL_LINE
        {
            f_error = "cannot allocate buffer of "              // LCOV_EXCL_LINE
                + std::to_string(size)                          // LCOV_EXCL_LINE
                + " bytes to read file.";                       // LCOV_EXCL_LINE
            f_contents.clear();                                 // LCOV_EXCL_LINE
            return false;                                       // LCOV_EXCL_LINE
        }

        // read the data
        //
        in.read(f_contents.data(), size);

        if(in.fail()) // note: eof() will be true here          // LCOV_EXCL_LINE
        {
            f_error = "an I/O error occurred reading \""        // LCOV_EXCL_LINE
                + f_filename                                    // LCOV_EXCL_LINE
                + "\".";                                        // LCOV_EXCL_LINE
            return false;                                       // LCOV_EXCL_LINE
        }

        f_error.clear();

        return true;
    }


    /** \brief Write the contents to the file.
     *
     * This function writes the file contents data to the file. If a new
     * filename is specified, the contents is saved there instead. Note
     * that the filename does not get modified, but it allows for creating
     * a backup before making changes and save the new file:
     *
     * \code
     *      file_content fc("filename.here");
     *      fc.read_all();
     *      fc.write_all("filename.bak");  // create backup
     *      std::string content(fc.contents());
     *      ... // make changes on 'contents'
     *      fc.contents(contents);
     *      fc.write_all();
     * \endcode
     *
     * \warning
     * If you marked the file_contents object as managing a temporary file
     * and specify a filename here which is not exactly equal to the
     * filename passed to the constructor, then the file you are writing
     * now will not be deleted automatically.
     *
     * \param[in] filename  The name to use or an empty string (or nothing)
     *                      to use the filename defined in the constructor.
     *
     * \return true if the file was saved successfully.
     *
     * \sa contents()
     * \sa contents(std::string const & new_contents)
     * \sa read_all()
     * \sa last_error()
     */
    bool write_all(std::string const & filename = std::string())
    {
        // select filename
        //
        std::string const name(filename.empty() ? f_filename : filename);

        // try to open the file
        //
        std::ofstream out;
        out.open(name, std::ios::trunc | std::ios::out | std::ios::binary);
        if(!out.is_open())
        {
            f_error = "could not open file \""
                + name
                + "\" for writing.";
            return false;
        }

        // write the data
        //
        out.write(f_contents.data(), f_contents.length());

        if(out.fail())
        {
            f_error = "could not write "                    // LCOV_EXCL_LINE
                + std::to_string(f_contents.length())       // LCOV_EXCL_LINE
                + " bytes to \"" + name + "\".";            // LCOV_EXCL_LINE
            return false;                                   // LCOV_EXCL_LINE
        }

        f_error.clear();

        return true;
    }


    /** \brief Change the contents with \p new_contents.
     *
     * This function replaces the current file contents with
     * \p new_contents.
     *
     * If \p new_contents is empty, then the file will become empty on a
     * write_all(). If you instead wanted to delete the file on
     * destruction of the function, set the temporary flag of the
     * construction to true to do that.
     *
     * \param[in] new_contents  The new contents of the file.
     *
     * \sa contents()
     * \sa write_all()
     */
    void contents(std::string const & new_contents)
    {
        f_contents = new_contents;
    }


    /** \brief Get a constant reference to the contents.
     *
     * This function gives you read access to the existing contents of the
     * file.
     *
     * The string represents the contents of the file only if you called
     * the read_all() function first. It is not mandatory to do so in case
     * you are creating a new file.
     *
     * \return A constant reference to this file contents.
     *
     * \sa contents(std::string const & new_contents)
     * \sa read_all()
     */
    std::string const & contents() const
    {
        return f_contents;
    }


    /** \brief Get a reference to the contents.
     *
     * This function gives you direct read/write access to the existing
     * contents of the file.
     *
     * The string represents the contents of the file only if you called
     * the read_all() function first. It is not mandatory to do so in case
     * you are creating a new file.
     *
     * \return A constant reference to this file contents.
     *
     * \sa contents() const
     * \sa contents(std::string const & new_contents)
     * \sa read_all()
     */
    std::string & contents()
    {
        return f_contents;
    }


    /** \brief Retrieve the last error message.
     *
     * This function returns a copy of the last error message generated
     * by one of read_all() or write_all().
     *
     * \return The last error generated.
     */
    std::string const & last_error() const
    {
        return f_error;
    }

private:
    std::string         f_filename = std::string();
    std::string         f_contents = std::string();
    std::string         f_error = std::string();
    bool                f_temporary = false;
};



} // namespace snap
// vim: ts=4 sw=4 et
