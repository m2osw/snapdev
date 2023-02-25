// Copyright (c) 2011-2023  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Extensions to handle QStrings.
 *
 * A set of functions to somewhat ease the mixing of QString and std::string
 * and std::ostream.
 */

// Qt
//
#include    <QString>


// C++
//
#include    <iostream>



/** \brief Allow printing of a QByteArray to an std::ostream.
 *
 * This function allows you to write a QByteArray as an std::string to
 * an std::ostream object.
 *
 * \param[in] os  The output stream where the QByteArray gets written to.
 * \param[in] qarray  The QByteArray to output.
 *
 * \return A reference to the \p os stream.
 */
inline std::ostream & operator << (std::ostream & os, QByteArray const & a)
{
    return os << a.data();
}


/** \brief Allow printing of a QString to an std::ostream.
 *
 * This function allows you to write a QString as an std::string to
 * an std::ostream object.
 *
 * \param[in] os  The output stream where the QString gets written to.
 * \param[in] s  The QString to output.
 *
 * \return A reference to the \p os stream.
 */
inline std::ostream & operator << (std::ostream & os, QString const & s)
{
    return os << s.toUtf8();
}


/** \brief Allow printing of a QStringRef to an std::ostream.
 *
 * This function allows you to write a QStringRef as an std::string to
 * an std::ostream object.
 *
 * \param[in] os  The output stream where the QStringRef gets written to.
 * \param[in] s  The QString to output.
 *
 * \return A reference to the \p os stream.
 */
inline std::ostream & operator << (std::ostream & os, QStringRef const & sr)
{
    return os << sr.toString();
}


/** \brief Concatenate a standard string with a QByteArray object.
 *
 * The function concatenates the standard string \p str and the array \p a.
 *
 * \param[in] str  The standard string.
 * \param[in] a  The QByteArray.
 *
 * \return A string with the concatenated string (\p str) and the array (\p a).
 */
inline std::string operator + (std::string const & str, QByteArray const & a)
{
    return str + a.data();
}


/** \brief Concatenate a QByteArray object with a standard string.
 *
 * The function concatenates the array \p a and the standard string \p str.
 *
 * \param[in] a  The QByteArray.
 * \param[in] str  The standard string.
 *
 * \return A string with the concatenated array (\p a) and the string (\p str).
 */
inline std::string operator + (QByteArray const & a, std::string const & str)
{
    return a.data() + str;
}


/** \brief Concatenate a standard string with a QString.
 *
 * The function concatenates the standard string \p str and the QString \p s.
 *
 * \param[in] str  The standard string.
 * \param[in] s  The QString.
 *
 * \return A string with the concatenated string (\p str) and the QString (\p s).
 */
inline std::string operator + (std::string const & str, QString const & s)
{
    return str + s.toUtf8();
}


/** \brief Concatenate a QString with a standard string.
 *
 * The function concatenates the QString \p s and the standard string \p str.
 *
 * \param[in] s  The QString.
 * \param[in] str  The standard string.
 *
 * \return A string with the concatenated QString (\p s) and the string (\p str).
 */
inline std::string operator + (QString const & s, std::string const & str)
{
    return s.toUtf8() + str;
}


/** \brief Concatenate a standard string with a QStringRef.
 *
 * The function concatenates the standard string \p str and
 * the QStringRef \p sr.
 *
 * \param[in] str  The standard string.
 * \param[in] sr  The QStringRef.
 *
 * \return A string with the string (\p str) and the concatenated QString (\p sr).
 */
inline std::string operator + (std::string const & str, QStringRef const & sr)
{
    return str + sr.toString();
}


/** \brief Concatenate a QStringRef with a standard string.
 *
 * The function concatenates the QStringRef \p sr and the standard
 * string \p str.
 *
 * \param[in] str  The standard string.
 * \param[in] sr  The QStringRef.
 *
 * \return A string with the string (\p str) and the concatenated QString (\p sr).
 */
inline std::string operator + (QStringRef const & sr, std::string const & str)
{
    return sr.toString() + str;
}


/** \brief Append a QByteArray to a standard string.
 *
 * The function appends the QByteArray \p a to the standard string \p str
 * and returns a reference to the standard string.
 *
 * \param[in] str  The standard string.
 * \param[in] a  The QByteArray.
 *
 * \return A reference to the input standard string \p str.
 */
inline std::string & operator += (std::string & str, QByteArray const & a)
{
    str = str + a;
    return str;
}


/** \brief Append a QString to a standard string.
 *
 * The function appends the QString \p s to the standard string \p str
 * and returns a reference to the standard string.
 *
 * \param[in] str  The standard string.
 * \param[in] a  The QString.
 *
 * \return A reference to the input standard string \p str.
 */
inline std::string & operator += (std::string & str, QString const & s)
{
    str = str + s;
    return str;
}


/** \brief Append a QStringRef to a standard string.
 *
 * The function appends the QStringRef \p sr to the standard string \p str
 * and returns a reference to the standard string.
 *
 * \param[in] str  The standard string.
 * \param[in] sr  The QStringRef.
 *
 * \return A reference to the input standard string \p str.
 */
inline std::string & operator += (std::string & str, QStringRef const & sr)
{
    str = str + sr;
    return str;
}


/** \brief Convert a standard string in a QString.
 *
 * The function converts the QString to a standard string in UTF-8.
 *
 * \param[in] str  The standard string.
 *
 * \return A QString with the contents of the standard string \p str.
 */
inline QString toQString(std::string const & str)
{
    return QString::fromUtf8(str.c_str(), str.length());
}


/** \brief Convert a QString in a standard string.
 *
 * The function converts the QString \p s to a standard string.
 *
 * \param[in] s  The QString input.
 *
 * \return A standard string with the contents of the QString \p s.
 */
inline std::string to_string(QString const & s)
{   
    QByteArray a(s.toUtf8());
    return std::string(a.data(), a.size());
}


// vim: ts=4 sw=4 et
