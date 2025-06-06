// Copyright (c) 2013-2025  Made to Order Software Corp.  All Rights Reserved
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
 * \brief An extension to the QString class to support case insensitive strings.
 *
 * It is often useful to sort strings in a case insensitive manner. This
 * class is useful for that purpose.
 *
 * \note
 * We have an equivalent for std::basic_string which you should use; see
 * case_insensitive_string.h.
 */

// Qt
//
#include <QString>



/** \brief Case insensitive QString.
 *
 * This class is an overload of the QString class which allows you to
 * create case insensitive strings as far as the operators are concerned.
 * All the other functions still work the same way.
 *
 * This is particularly useful if you manage a QMap<> with a string as
 * the key, string which should not be case sensitive.
 *
 * Note that only operators are overloaded. All the other functions work
 * the same way at this point. Later we may decide to overload all the
 * functions that have a Qt::CaseSensitivity parameter to default to
 * Qt::CaseInsensitive (which would make sense to have a really complete
 * implementation...)
 *
 * \note
 * We are looking into removing all Qt functions, including QString
 * related ones. For an std::string version, look at the libutf8 library.
 * The libutf8 library offers a u8casecmp() function and a
 * case_insensitive_basic_string class template which handles all the
 * comparison operators (==, !=, etc.) in a case insensitive manner.
 */
class QCaseInsensitiveString : public QString
{
public:
    QCaseInsensitiveString()
        : QString()
    {
    }

    QCaseInsensitiveString(QChar const * uni, int sz)
        : QString(uni, sz)
    {
    }

    QCaseInsensitiveString(QChar const * uni)
        : QString(uni)
    {
    }

    QCaseInsensitiveString(QChar ch)
        : QString(ch)
    {
    }

    QCaseInsensitiveString(int sz, QChar ch)
        : QString(sz, ch)
    {
    }

    QCaseInsensitiveString(QLatin1String const & str)
        : QString(str)
    {
    }

    QCaseInsensitiveString(QString const & other)
        : QString(other)
    {
    }

    QCaseInsensitiveString(QCaseInsensitiveString const & other)
        : QString(other)
    {
    }

    QCaseInsensitiveString(char const * str)
        : QString(str)
    {
    }

    QCaseInsensitiveString(QByteArray const & ba)
        : QString(ba)
    {
    }

    QCaseInsensitiveString & operator = (QChar ch)
    {
        QString::operator = (ch);
        return *this;
    }

    QCaseInsensitiveString & operator = (QLatin1String const & str)
    {
        QString::operator = (str);
        return *this;
    }

    QCaseInsensitiveString & operator = (QString const & other)
    {
        QString::operator = (other);
        return *this;
    }

    QCaseInsensitiveString & operator = (QCaseInsensitiveString const & other)
    {
        QString::operator = (other);
        return *this;
    }

    QCaseInsensitiveString & operator = (char const * str)
    {
        QString::operator = (str);
        return *this;
    }

    bool operator == (QString const & rhs) const
    {
        return compare(rhs, Qt::CaseInsensitive) == 0;
    }
    bool operator == (QCaseInsensitiveString const & rhs) const
    {
        return compare(rhs, Qt::CaseInsensitive) == 0;
    }
    bool operator == (char const * rhs) const
    {
        return compare(rhs, Qt::CaseInsensitive) == 0;
    }

    bool operator != (QString const & rhs) const
    {
        return compare(rhs, Qt::CaseInsensitive) != 0;
    }
    bool operator != (QCaseInsensitiveString const & rhs) const
    {
        return compare(rhs, Qt::CaseInsensitive) != 0;
    }
    bool operator != (char const * rhs) const
    {
        return compare(rhs, Qt::CaseInsensitive) != 0;
    }

    bool operator < (QString const & rhs) const
    {
        return compare(rhs, Qt::CaseInsensitive) < 0;
    }
    bool operator < (QCaseInsensitiveString const & rhs) const
    {
        return compare(rhs, Qt::CaseInsensitive) < 0;
    }

    bool operator <= (QString const & rhs) const
    {
        return compare(rhs, Qt::CaseInsensitive) <= 0;
    }
    bool operator <= (QCaseInsensitiveString const & rhs) const
    {
        return compare(rhs, Qt::CaseInsensitive) <= 0;
    }

    bool operator > (QString const & rhs) const
    {
        return compare(rhs, Qt::CaseInsensitive) > 0;
    }
    bool operator > (QCaseInsensitiveString const & rhs) const
    {
        return compare(rhs, Qt::CaseInsensitive) > 0;
    }

    bool operator >= (QString const & rhs) const
    {
        return compare(rhs, Qt::CaseInsensitive) >= 0;
    }
    bool operator >= (QCaseInsensitiveString const & rhs) const
    {
        return compare(rhs, Qt::CaseInsensitive) >= 0;
    }
};

// vim: ts=4 sw=4 et
