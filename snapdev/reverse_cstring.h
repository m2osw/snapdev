// Copyright (c) 2011-2022  Made to Order Software Corp.  All Rights Reserved
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

/** \file
 * \brief Iterator to search a string in reverse.
 *
 * This implementation is used to search characters starting the
 * search from the end of the string. However, the iterator looks
 * like it is used in a normal way which better matches certain
 * function which would not otherwise accept rbegin() and rend().
 */

// C++
//
#include    <iterator>

namespace snapdev
{

/** \brief Iterate a standard C string in reverse.
 *
 * This function lets you iterate a standard C string, such as "12345",
 * in reverse order. The begin() and end() functions return a reverse
 * iterator (watch out!) We do not offer a rbegin() and rend() since
 * in most cases, using the string pointers as is would have to exact
 * same effect.
 *
 * \note
 * There may already be something in STL to be able to do that without
 * a specific implementation, if so I have not found it yet. This
 * class satisfy various needs such as finding the last character
 * that do not match a set of characters:
 *
 * \code
 *   std::string match(":,/");
 *   reverse_cstring rstr(start, start + strlen(start));
 *   auto last = std::find_if_not(
 *                      rstr.begin(),
 *                      rstr.end(),
 *                      [](char const c)
 *                      {
 *                          return match.find(c) != std::string::npos;
 *                      });
 *   // last.get() == "first character found in 'match' at the end part of string"
 *   // for example, in "/,:test:,/" would return a pointer on the last ':'
 * \endcode
 *
 * \warning
 * This function uses the specified start and end pointer of a C string
 * that must be valid as long as these iterators are used.
 *
 * \tparam T  The type of string container.
 */
template <typename T>
class reverse_cstring
{
public:

    /** \brief Reverse C-string iterator.
     *
     * This iterator allows us to go through a C-string (i.e. a `char *`
     * pointer to a null terminated string) in reverse (i.e. from the
     * end to the start.)
     *
     * This is a very efficient iterator that allows us to avoid a full
     * copy of the string in an std::string. At this time, it is primarily
     * used to trim the end of strings.
     */
    class iterator
        : public std::iterator<std::random_access_iterator_tag, T>
    {
    public:
        // Iterator traits
        //
        typedef std::ptrdiff_t      difference_type;
        typedef T                   value_type;
        typedef T *                 pointer;
        typedef T &                 reference;

        // ForwardIterator
        //
        reference operator * ()
        {
            return i_[-1];
        }

        value_type operator * () const
        {
            return i_[-1];
        }

        iterator operator ++ (int)
        {
            iterator const copy(*this);
            --i_;
            return copy;
        }

        iterator & operator ++ ()
        {
            --i_;
            return *this;
        }

        // BidirectionalIterator
        //
        iterator operator -- (int)
        {
            iterator const copy(*this);
            ++i_;
            return copy;
        }

        iterator & operator -- ()
        {
            ++i_;
            return *this;
        }

        // RandomAccessIterator
        //
        iterator & operator += (int n)
        {
            i_ -= n;
            return *this;
        }

        iterator operator + (int n) const
        {
            return iterator(i_ - n);
        }

        friend iterator operator + (int n, iterator & rhs)
        {
            return iterator(rhs.i_ - n);
        }

        iterator & operator -= (int n)
        {
            i_ += n;
            return *this;
        }

        iterator operator - (int n) const
        {
            return iterator(i_ + n);
        }

        friend iterator operator - (int n, iterator & rhs)
        {
            return iterator(rhs.i_ + n);
        }

        difference_type operator - (iterator const & rhs) const
        {
            return rhs.i_ - i_;
        }

        reference operator [] (int idx)
        {
            return i_[-idx - 1];
        }

        value_type operator [] (int idx) const
        {
            return i_[-idx - 1];
        }

        bool operator < (iterator const & rhs) const
        {
            return i_ > rhs.i_;
        }

        bool operator > (iterator const & rhs) const
        {
            return i_ < rhs.i_;
        }

        bool operator <= (iterator const & rhs) const
        {
            return i_ >= rhs.i_;
        }

        bool operator >= (iterator const & rhs) const
        {
            return i_ <= rhs.i_;
        }

        // Other
        //
        bool operator == (iterator const & rhs) const
        {
            return i_ == rhs.i_;
        }

        bool operator != (iterator const & rhs) const
        {
            return i_ != rhs.i_;
        }

        T * get() const
        {
            return i_;
        }

     private:
         friend class reverse_cstring;

         iterator(T * it)
             : i_(it)
         {
         }

         T *    i_;
    };

    reverse_cstring(T * start, T * end)
        : s_(start)
        , e_(end)
    {
    }

    iterator begin() const
    {
        return iterator(e_);
    }

    iterator end() const
    {
        return iterator(s_);
    }

    std::size_t size() const
    {
        return e_ - s_;
    }

private:
    T *       s_;
    T *       e_;
};



/** \fn reverse_cstring::reverse_cstring(T * start, T * end)
 * \brief Initialize the reverse string with a start and end pointers.
 *
 * This constructor initializes a reverse string. You can then retrieve
 * the `begin()` and `end()` iterators to walk the string. Remember that
 * you will be walking the string going backward using what looks like
 * a usual forward iterator.
 *
 * The start is the smaller pointer. So if you have a C-string, you want
 * to create a reverse cstring with:
 *
 * \code
 *     char const * str("a C-string");
 *     reverse_cstring(str, str + strlen(str));
 * \endcode
 *
 * Of course, it is not mandatory to pass the entire string to this
 * constructor. However, we do not make a copy of the string. We use
 * your bare pointers as is. You must make sure that this string
 * remains available for the whole time you want to use the
 * reverse_cstring.
 *
 * \param[in] start  The start of the string.
 * \param[in] end  The end of the string.
 */

/** \fn reverse_cstring::begin() const
 * \brief Create an iterator at the beginning of the C-string.
 *
 * This function creates an iterator at the beginning of the reversed string.
 *
 * \note
 * The begin() actually uses the end pointer as passed to the constructor.
 *
 * \return A reverse_cstring iterator to the beginning of the string.
 */

/** \fn reverse_cstring::end() const
 * \brief Create an iterator at the end of the C-string.
 *
 * This function creates an iterator at the end of the reverse string.
 *
 * \note
 * The end() actually uses the begin pointer as passed to the constructor.
 *
 * \return A reverse_cstring iterator to the end of the string.
 */

/** \fn reverse_cstring::size() const
 * \brief Return the total size of the reverse C-string.
 *
 * The size of the string calculated as the end pointer minus the start
 * pointer.
 *
 * \return The size of the reverse C-string.
 */

/** \var reverse_cstring::s_
 * \brief The start of the string.
 *
 * This variable member points to the start of the string. This is the
 * pointer used to represent the end() iterator.
 */

/** \var reverse_cstring::e_
 * \brief The end of the string.
 *
 * This variable member points to the end of the string. Usually the
 * last character + 1. This is the pointer used to represent the
 * begin() iterator.
 */









/** \typedef reverse_cstring::iterator::difference_type
 * \brief Type used to calculate the difference between iterators.
 *
 * This type is expected to fit a pointer and to be signed.
 */


/** \typedef reverse_cstring::iterator::value_type
 * \brief Type of each item in the string.
 *
 * This type represents the type of characters in the string.
 */


/** \typedef reverse_cstring::iterator::pointer
 * \brief Direct pointer to the string characters.
 *
 * This type represents a direct pointer (not a smart pointer) to the
 * string data.
 *
 * It is expected that the person using this template will make sure
 * that the string remains available for the whole duration.
 */


/** \typedef reverse_cstring::iterator::reference
 * \brief Reference to the string characters.
 *
 * This type represents a reference to the string character type.
 */

/** \fn reverse_cstring::iterator::operator * ()
 * \brief Retrieve a reference to the character at the current position.
 *
 * This function returns a reference to the string character at the
 * current location. It can be used to modify the character.
 *
 * \return A reference to the current character.
 */

/** \fn reverse_cstring::iterator::operator * () const
 * \brief Retrieve the character at the current position.
 *
 * This function returns a copy of the character at the current position.
 *
 * \return A copy of the current character.
 */

/** \fn reverse_cstring::iterator::operator ++ (int)
 * \brief Run a post increment.
 *
 * This function makes a copy of the current position, then it increments
 * it. Since we are the "reverse string" the position is actually
 * decremented.
 *
 * \return A reference to this iterator.
 */

/** \fn reverse_cstring::iterator::operator ++ ()
 * \brief Run a pre-increment.
 *
 * This function increments the position and then returns a reference to
 * the iterator. Since we are the "reverse string" the position is actually
 * decremented.
 *
 * \return A reference to this iterator.
 */

/** \fn reverse_cstring::iterator::operator -- (int)
 * \brief Run a post decrement.
 *
 * This function makes a copy of the current position, then it decrements
 * it. Since we are the "reverse string" the position is actually
 * incremented.
 *
 * \return A reference to this iterator.
 */

/** \fn reverse_cstring::iterator::operator -- ()
 * \brief Run a pre-decrement.
 *
 * This function decrements the position and then returns a reference to
 * the iterator. Since we are the "reverse string" the position is actually
 * incremented.
 *
 * \return A reference to this iterator.
 */

/** \fn reverse_cstring::iterator::operator += (int n) const
 * \brief Add an offset to the position of this iterator and returns the copy.
 *
 * This function increments the position by n and then returns a
 * reference. Since we are the "reverse string" the position is
 * actually decrementing the position.
 *
 * \param[in] n  The number to add.
 *
 * \return A reference to this iterator.
 */

/** \fn reverse_cstring::iterator::operator + (int n) const
 * \brief Add an offset to the position of this iterator and returns the copy.
 *
 * This function increments the position by n and then returns the copy to
 * the iterator. Since we are the "reverse string" the position is actually
 * decrementing the position.
 *
 * \param[in] n  The number to add.
 *
 * \return A new iterator.
 */

/** \fn reverse_cstring::iterator::operator + (int n, iterator & rhs)
 * \brief Add an offset to the position of this iterator and returns the copy.
 *
 * This function increments the position by n and then returns the copy to
 * the iterator. Since we are the "reverse string" the position is actually
 * decrementing the position.
 *
 * \param[in] n  The number to add.
 * \param[in] rhs  The right hand side iterator.
 *
 * \return A new iterator.
 */

/** \fn reverse_cstring::iterator::operator -= (int n) const
 * \brief Subtract an offset from the position of this iterator and returns the copy.
 *
 * This function decrements the position by n and then returns a
 * reference. Since we are the "reverse string" the position is
 * actually incrementing the position.
 *
 * \param[in] n  The number to subtract.
 *
 * \return A reference to this iterator.
 */

/** \fn reverse_cstring::iterator::operator - (int n) const
 * \brief Subtract an offset from the position of this iterator and returns the copy.
 *
 * This function decrements the position by n and then returns the copy to
 * the iterator. Since we are the "reverse string" the position is actually
 * incrementing the position.
 *
 * \param[in] n  The number to subtract.
 *
 * \return A new iterator.
 */

/** \fn reverse_cstring::iterator::operator - (int n, iterator & rhs)
 * \brief Subtract an offset from the position of this iterator and returns the copy.
 *
 * This function decrements the position by n and then returns the copy to
 * the iterator. Since we are the "reverse string" the position is actually
 * incrementing the position.
 *
 * \param[in] n  The number to add.
 * \param[in] rhs  The right hand side.
 *
 * \return A new iterator.
 */

/** \fn reverse_cstring::iterator::operator - (iterator const & rhs) const
 * \brief Compare the difference between two iterators.
 *
 * This function calculates the difference between two iterators.
 *
 * \param[in] rhs  The right hand side.
 *
 * \return A new iterator.
 */

/** \fn reverse_cstring::iterator::operator [] (int idx)
 * \brief Get a reference to the character at the current position.
 *
 * This function gets a reference to the character at the current position.
 * It can be used to read or write the character.
 *
 * \param[in] idx  The index of the character to retrieve.
 *
 * \return A reference to the index character.
 */

/** \fn reverse_cstring::iterator::operator [] (int idx) const
 * \brief Get a copy of the character at the current position.
 *
 * This function returns a copy of the character at the current position.
 *
 * \param[in] idx  The index of the character to retrieve.
 *
 * \return A of the character at the \p idx position.
 */

/** \fn reverse_cstring::iterator::operator < (iterator const & rhs) const
 * \brief Compare two iterators.
 *
 * This function returns true if the left hand side iterator is at an
 * earlier position than the right hand side.
 *
 * \param[in] rhs  The right hand side.
 *
 * \return true if this is less than right hand side.
 */

/** \fn reverse_cstring::iterator::operator > (iterator const & rhs) const
 * \brief Compare two iterators.
 *
 * This function returns true if the left hand side iterator is at a
 * later position than the right hand side.
 *
 * \param[in] rhs  The right hand side.
 *
 * \return true if this is more than right hand side.
 */

/** \fn reverse_cstring::iterator::operator <= (iterator const & rhs) const
 * \brief Compare two iterators.
 *
 * This function returns true if the left hand side iterator is at an
 * earlier or equal position than the right hand side.
 *
 * \param[in] rhs  The right hand side.
 *
 * \return true if this is less or equal than right hand side.
 */

/** \fn reverse_cstring::iterator::operator >= (iterator const & rhs) const
 * \brief Compare two iterators.
 *
 * This function returns true if the left hand side iterator is at a
 * later or equal position than the right hand side.
 *
 * \param[in] rhs  The right hand side.
 *
 * \return true if this is larger or equal than right hand side.
 */

/** \fn reverse_cstring::iterator::operator == (iterator const & rhs) const
 * \brief Compare two iterators.
 *
 * This function returns true if the left hand side iterator is at
 * the same position than the right hand side.
 *
 * \param[in] rhs  The right hand side.
 *
 * \return true if this is equal the right hand side.
 */

/** \fn reverse_cstring::iterator::operator != (iterator const & rhs) const
 * \brief Compare two iterators.
 *
 * This function returns true if the left hand side iterator is not at
 * the same position than the right hand side.
 *
 * \param[in] rhs  The right hand side.
 *
 * \return true if this is not equal the right hand side.
 */

/** \fn reverse_cstring::iterator::get() const
 * \brief Get the current position, which is a pointer to a character.
 *
 * This function returns the current iterator position. This is a
 * pointer right after a character in the string. If you want to 
 * retrieve the character, may sure to use `pointer[-1]`.
 *
 * \return The pointer to the string at the current iterator location.
 */

/** \fn reverse_cstring::iterator::iterator(T * it)
 * \brief Initialize the iterator.
 *
 * This function initialize the iterator with the specified pointer.
 *
 * \param[in] it  The string pointer the iterator likes to use.
 */

/** \var reverse_cstring::iterator::i_
 * \brief The position in the iterator.
 *
 * This variable member is a pointer to the character in the string
 * this iterator points to. The character is actually at `i_[-1]`
 * since we are in reverse.
 */





} // namespace snapdev
// vim: ts=4 sw=4 et
