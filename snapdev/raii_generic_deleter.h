// RAII Generic Deleter -- allow for any type of RAII deleter
// Copyright (c) 2018-2019  Made to Order Software Corp.  All Rights Reserved
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
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#pragma once

/** \file
 * \brief A set of templates to help with RAII.
 *
 * Resources that are pointers to C++ objects can easily be managed
 * with std::unique_ptr<>() and std::shared_ptr<>().
 *
 * Pointers to objects which are not C++ objects can be managed
 * by std::unique_ptr<>() and a deleter. To help with that case,
 * though, we add a template class which allows us to create a
 * typedef. Otherwise you have to specify the deleter to the
 * std::unique_ptr<>() each time you allocate such an object!
 *
 * Finally, many resources use a variable type which is not
 * a pointer at all. For those, it becomes very annoying to use
 * an std::unique_ptr<>() _by hand_ each time. Each instance
 * need to have access to that resource type deleter. Instead,
 * we offer a template which incorporates the deleter in one
 * typedef and it handles all the work of whether the value
 * is considered to be nullptr (i.e. a file descriptor obtained
 * with `open(2)` returns -1 as an equivalent to a nullptr.)
 */

// snapdev lib
//
#include "snapdev/not_used.h"

// C++ lib
//
#include <memory>
#include <iostream>

// C lib
//
#include <unistd.h>
#include <stdio.h>



namespace snap
{


/** \brief A template used to allow unique_ptr<>() of nearly any type.
 *
 * This template is used with nearly any type representing a resource
 * that you would like to automatically delete (RAII) by using a
 * unique_ptr<>() definition.
 *
 * Note that it only works with resources which type is not a pointer.
 * For example, a file descriptor, which uses an `int`, works with this
 * class.
 *
 * For resources that have a pointer, use the raii_pointer_deleter instead.
 *
 * For example, a file descriptor can use this template class as follow:
 *
 * \code
 *   typedef std::unique_ptr<int, snap::raii_generic_deleter<int, -1, decltype(&::close), &::close>> raii_fd_t;
 *
 *   raii_fd_t fd(open("/tmp/test.tmp", O_RDWR));
 * \endcode
 *
 * \note
 * We actually offer the raii_fd_t as a default since it can be used by
 * many other implementations.
 *
 * To access the value, use the `get()` function of your pointer.
 * For example:
 *
 * \code
 *  raii_fd_t safe_fd;
 *  safe_fd.reset(socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP));
 *  std::cout << safe_fd.get() << std::endl;
 *  write(safe_fd.get(), "MSG\n", 4);
 * \endcode
 *
 * \warning
 * This class doesn't properly handle a case where an invalid resource can
 * be represented by more than one value. For a file descriptor, we expect
 * the invalid value to be exactly -1. If a function call may return other
 * negative values, then this implementation currently fails to handle such.
 * It is your responsibility to fix the return value of the function or
 * use a two step initialization such as in:
 *
 * \code
 *  int r(this_func("open/something", "rw"));
 *  if(r < 0)
 *  {
 *      return; // it failed. r may not just be -1
 *  }
 *  raii_fd_t safe_fd(r);
 *  ...
 * \endcode
 *
 * \note
 * This template class is based on code found in this answer on Stackoverflow:
 * https://stackoverflow.com/questions/15756960/using-unique-ptr-to-control-a-file-descriptor#48575395
 *
 * \tparam T  The type being managed such as 'int'.
 * \param[in] null_value  The value representing "nullptr" for that type. For
 *                        a file descriptor this is -1.
 * \tparam D  The type of the deleter function.
 * \param[in] deleter  The deleter function (i.e. 'deleter(p)' gets rid of `T p`).
 */
template<class T, T null_value, class D, D deleter>
class raii_generic_deleter
{
public:
    /** \brief The pointer class.
     *
     * This class must be called "pointer" because the unique_ptr<>()
     * template definition checks the deleter using the following
     * code:
     *
     * \code
     * template<typename _Up>
     *     static typename _Up::pointer __test(typename _Up::pointer*);
     * [...]
     * typedef decltype(__test<_Del>(0)) type;
     * \endcode
     *
     * As we can see it looks for a class named pointer. If it exists,
     * then it defines the type using that pointer class pointer operator
     * (i.e. the `\<type> operator *`) instead of using an actual pointer
     * (i.e. it uses `deleter::pointer` instead of the default `T *`.)
     *
     * The C++ class is defined as a NullablePointer. It also needs one
     * operator function to retrieve the pointer (operator *).
     *
     * \see
     * https://en.cppreference.com/w/cpp/named_req/NullablePointer
     */
    class pointer
    {
    private:
        /** \brief The pointer being manage.
         *
         * This parameter is the resource pointer being managed.
         */
        T f_pointer = null_value;

    public:
        /** \brief Create a pointer with a value of type T.
         *
         * This constructor creates an explicit instance of a pointer with
         * a type T object.
         *
         * \param[in] p  The pointer being managed by this pointer class.
         */
        pointer(T p)
            : f_pointer(p)
        {
        }

        /** \brief Allow assigning the nullptr to this type.
         *
         * This constructor allows for the nullptr value to be assigned to
         * this type. This is a test run at compile time. If not present
         * the resulting std::unique_ptr<>() would not compile.
         *
         * \param[in] p  The null pointer.
         */
        pointer(std::nullptr_t p = nullptr)
            : f_pointer(null_value)
        {
            NOTUSED(p);
        }


//        /** \brief Assign the nullptr to this pointer.
//         *
//         * This function is used to assign `nullptr` to this pointer.
//         *
//         * \warning
//         * It is assumed that this is called only when the pointer
//         * was released by a call to the `deleter` function.
//         *
//         * \param[in] h  The `nullptr` being assigned.
//         *
//         * \return A reference to this pointer.
//         */
//        pointer & operator = (std::nullptr_t h)
//        {
//            NOTUSED(h);
//
//            f_pointer = null_value;
//
//            return *this;
//        }

        /** \brief Convert pointer to bool.
         *
         * This operator allows for converting the pointer to a bool
         * which is often done in an `if(ptr)` or equivalent statement.
         *
         * \return true if the pointer is not `null_value`.
         */
        explicit operator bool () const
        {
            return f_pointer != null_value;
        }

        /** \brief Convert pointer to "not bool".
         *
         * This operator allows for converting the pointer to a bool
         * when preceded by a Boolean NOT operator, which is often
         * done in an `if(!ptr)` or equivalent statement.
         *
         * \return true if the pointer is `null_value`.
         */
        bool operator ! () const
        {
            return f_pointer == null_value;
        }

        /** \brief Compare two pointers against each other for equality.
         *
         * This function compares the 'this' and \p rhs pointers against
         * each other. If equal then it returns true.
         *
         * \param[in] rhs  The right hand side pointer being compared against
         *                 `this` pointer.
         *
         * \return true if both pointers are equal.
         */
        bool operator == (pointer const rhs) const
        {
            return f_pointer == rhs.f_pointer;
        }

        /** \brief Compare two pointers against each other for inequality.
         *
         * This function compares the `this` and \p rhs pointers against
         * each other. If not equal then it returns true.
         *
         * \param[in] rhs  The right hand side pointer being compared against
         *                 `this` pointer.
         *
         * \return true if both pointers are not equal.
         */
        bool operator != (pointer const rhs) const
        {
            return f_pointer != rhs.f_pointer;
        }

        /** \brief Compare this pointer against nullptr for equality.
         *
         * This function is used whenever this pointer gets compared
         * against the special value `nullptr`.
         *
         * \note
         * From what I've seen, this function doesn't get called.
         * Instead, they do: "_internal_ptr == pointer()".
         *
         * \param[in] rhs  The nullptr.
         *
         * \return true when this pointer is the `nullptr`.
         */
        bool operator == (std::nullptr_t rhs) const
        {
            NOTUSED(rhs);
            return f_pointer == null_value;
        }

        /** \brief Compare this pointer against nullptr for inequality.
         *
         * This function is used whenever this pointer gets compared
         * against the special value `nullptr` for inequality.
         *
         * \note
         * This function gets called in many circumstances where the
         * unique_ptr<>() implementation checks whether the pointer
         * is null or not. It's a simplification that may help the
         * compiler.
         *
         * \param[in] rhs  The nullptr.
         *
         * \return true when this pointer is not the `nullptr`.
         */
        bool operator != (std::nullptr_t rhs) const
        {
            NOTUSED(rhs);
            return f_pointer != null_value;
        }

        /** \brief Retrieve the "pointer".
         *
         * This function is used to retrieve the "pointer" value.
         *
         * \note
         * This class does not offer an `operator * ()` because that
         * would need to return a reference to the value which is not
         * possible with a value other than an actual real pointer.
         * Otherwise it becomes a reference to a temporary pointer.
         * This is because unique_ptr<>() is implemented as:
         *
         * \code
         * pointer get() const { ...}
         * reference operator * () const { return get(); }
         * \endcode
         *
         * \par
         * As we can see, the `get()` returns a `pointer` and the
         * `operator * ()` would return a reference on that temporary.
         *
         * \return A copy of the resource "pointer".
         */
        operator T () const
        {
            return f_pointer;
        }
    };

    /** \brief The function called to delete/release this pointer.
     *
     * This function gets called whenever the unique_ptr<>() or
     * shared_ptr<>() decides to delete the pointer.
     *
     * \note
     * The function gets called only if p does not represent nullptr
     * and the handle gets cleaned in the unique_ptr<>() as required.
     *
     * \param[in] p  The pointer to delete.
     */
    void operator () (pointer p)
    {
        // we use static_cast<T>(p) in case the deleter does not
        // use a clear type as its parameter (these damn C functions...)
        //
        deleter(static_cast<T>(p));
    }
};


/** \brief Define a smart pointer for `fd`.
 *
 * The Unix system uses many file descriptors. This declaration
 * automatically handles the close() of the specified `fd` resource.
 *
 * It is expected that the "null pointer" is -1.
 */
typedef std::unique_ptr<int, raii_generic_deleter<int, -1, decltype(&::close), &::close>>     raii_fd_t;



/** \brief A templated used to easily delete a unique_ptr<>() resource.
 *
 * This template is used with any type if resources represented by a pointer.
 * It automatically deletes (RAII) the resource one the unique_ptr<>()
 * goes out of scope.
 *
 * Note that it only works with resources which type is a pointer.
 * For example, a FILE object which uses a `FILE *` works with this
 * class.
 *
 * For resources that do not have a pointer, use the raii_generic_deleter
 * instead.
 *
 * For example, a FILE object can use this template class as follow:
 *
 * \code
 *      typedef std::unique_ptr<FILE, raii_pointer_deleter<FILE, decltype(&::fclose), &::fclose>> raii_file;
 *
 *      raii_file f(fopen("/tmp/test.tmp", "rw")); // f is automatically closed on a return or an interrupt
 * \endcode
 *
 * \tparam T  The type being managed such as 'int'.
 * \tparam D  The type of the deleter function.
 * \param[in] deleter  The deleter function (i.e. 'deleter(p)' gets rid of `T * p`).
 */
template<class T, class D, D deleter>
class raii_pointer_deleter
{
public:
    /** \brief The function called to delete/release this type of pointer.
     *
     * This function gets called whenever the unique_ptr<>() or
     * shared_ptr<>() decides to delete the pointer.
     *
     * \note
     * The function gets called only if p does not represent nullptr
     * and the pointer gets cleaned in the unique_ptr<>() as required.
     *
     * \param[in] p  The pointer to delete.
     */
    void operator () (T * p)
    {
        deleter(p);
    }
};


/** \brief A shared pointer with a deleter.
 *
 * A big problem with the std::shared_ptr<> is that it does not accept a
 * deleter, somehow. Yet, there should be no reason for such a limitation.
 * This class allows you to create generic deleter typedefs of smart
 * pointers with deleters.
 *
 * \code
 *     // we need a special deleter because of the required pointer to pointer
 *     void av_frame_free_ptr(AVFrame * ptr)
 *     {
 *         ::av_frame_free(&ptr);
 *     }
 *     typedef snap::raii_pointer_deleter<AVFrame, decl(&av_frame_free_ptr), &av_frame_free_ptr> av_frame_deleter_t;
 *     typedef snap::shared_ptr_with_deleter<AVFrame, av_frame_deleter_t> av_frame_t;
 * \endcode
 *
 * Source:
 *
 * https://stackoverflow.com/questions/27331315
 */
template<class T, class D = std::default_delete<T>>
struct shared_ptr_with_deleter
    : public std::shared_ptr<T>
{
    /** \brief The constructor accepts a type t.
     *
     * Create the shared pointer with the deleter as specified in the
     * template.
     *
     * \param[in] t  The bare pointer to save in this smart pointer.
     */
    explicit shared_ptr_with_deleter(T * t = nullptr)
        : std::shared_ptr<T>(t, D())
    {
    }

    /** \brief Call the reset function with the deleter.
     *
     * The std::shared_ptr<>() reset function also doesn't know anything
     * about the deleter and it has to be specified on the call.
     *
     * \param[in] t  Another bare pointer to save in this smart pointer.
     */
    void reset(T* t = nullptr)
    {
        std::shared_ptr<T>::reset(t, D());
    }
};


/** \brief Handle the closure of a FILE handle.
 *
 * One of the common type of file handle is the FILE object. It manages
 * a file including an efficient client side buffering mechanism.
 *
 * This typedef makes sure that the file gets closed whenever the
 * handle goes out of scope.
 *
 * This is a pointer so the null (a.k.a. closed, already released) is
 * expected to be represented by a nullptr.
 */
typedef std::unique_ptr<FILE, raii_pointer_deleter<FILE, decltype(&::fclose), &::fclose>>     raii_file_t;


} // namespace snap
// vim: ts=4 sw=4 et
