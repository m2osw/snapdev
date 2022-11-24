
<p align="center">
<img alt="snapdev" title="SnapDev is a project of C++ headers we use throughout our Snap! C++ projects"
src="https://snapwebsites.org/sites/all/modules/mo_m2osw/snap-medium-icon.png" width="70" height="70"/>
</p>

# Introduction

The snapdev package includes a set of C++ headers we use in many of our
other Snap! C++ projects. At this time this is really only headers.
Later we may include some libraries.

The first 14 headers were first in the libsnapwebsites. We wanted to 
be able to use some of them in some of our contrib projects. Hence
the creation of this separate project.

# Documentation

Each function found in this library is documented using Doxygen.

The project currently includes:

* `as_root.h`

  An RAII class one can use to run code as root and come back to the
  user one was before starting. This is not currently multithread safe.

* `brs.h`

  A set of classes used to transform structures in a binary buffer
  (i.e. binary serialization and unserialization).

* `callback_manager.h`

  A few classes used to manage callbacks in your own class. I noticed that
  I would often duplicate this work so I placed it in a template now. It
  allows you to either manage one callback or a set of callbacks. In either
  case, the class manages the set for you.

* `case_insensitive_string.h`

  An std::string implementation which replaces the default compare() with a
  case insensitive version allowing for adding strings to an std::set or
  an std::map and see them sorted case insensitively.

* `chownnm.h`

  Change the owner and group of a file either by identifiers or by name.
  This is practical as in C we only have it by identifiers meaning that
  each time you have to search for the UID and the GID before calling the
  `chown()` function.

* `empty_set_intersection.h`

  Compute the intersection of two sets and if empty (none of each set's
  elements match) then return `true`. This implementation is very fast
  since it does not actually compute an intersection. It also checks to
  see whether any one element is found in both sets.

* `enumerate.h`

  Many languages support an enumeration process with a callback. This
  function allows you to call the callback one for each one of the
  elements found in a container.

* `file_contents.h`

  Read or write a complete file at once. This class manages a binary buffer
  (std::string) which it can read and/or write to file in one go.

* `gethostname.h`

  The `gethostname()` C function may return a null or fail. This
  implementation returns an `std::string` and allows for two possible
  sources for the hostname.

* `glob_to_list.h`

  Read a directory with a glob pattern and return a container with the
  resulting filenames. You choose the type of container (vector, list, set,
  etc.) and you get full paths making it easier to access the files found.

* `has_member_function.h`

  A template to test whether a class or template has a given member function.

* `hexadecimal_string.h`

  Handle strings representing hexadecimal code in ASCII. (i.e. 03BED781....)
  Such strings are often used for checksums such as and MD5 or SHA512.

* `init_structure.h`

  Structure initialization helper templates. When your project allows for the
  initialization of a structure in place, you can use these templates to make
  it easier and safer. Especially, this allows the user to use functions to
  enter the data which in the end is still compiled as if directly written
  in a structure. The advantage is that said functions can verify the data
  and also the main function can make sure that all the fields will work
  together as expected. All of that work is done at compile time.

* `int128_literal.h`

  Allow for entering literals representing 128 bit numbers. The g++ compiler
  supports 128 bits computations, but not literals. So this fills up this gap.

* `isatty.h`

  Check whether a standard C++ stream is connected to a TTY. This is similar
  to the C `isatty()` only it works with C++ streams.

* `is_smart_pointer.h`

  Templates to check whether an object is a smart pointer or not. This can
  be used to implement two versions of a function: one accept smart pointers
  and one accepting bare pointers.

* `is_string_literal.h`

  Template to check whether a pointer is a string literal at compile time.

* `is_vector.h`

  Template to check whether an object is an `std::vector<>()` at compile time.

* `join_strings.h`

  Function used to join strings with a separate. This function goes fast as
  it preallocates one destination string of the right size and then just
  copies the input strings as required.

* `limits_int128.h`

  The `<limits>` header is missing the `__int128` limits. This is a copy of
  the newer version of the C++ standard library with those functions. That
  way I can use it until I get C++20 or C++22, whichever has those definitions
  available.

* `lockfile.h`

  A simple implementation of a shared or exclusive lock on a file. This is
  done in an RAII manner. It also allows you to copy a lock in another object.

* `log2.h`

  A fast implementation of `log2()` for integers.

* `map_keyset.h`

  Copy the keys of a an `std::map` in another container.

* `math.h`

  Various additional mathematic related functions. See also `log2.h` and
  `matrix.h`.

* `matrix.h`

  A class to handle matrix computations. It supports matrices of any dimensions
  and has some specializations for 4x4 matrices (i.e. for color computations).

* `mkdir_p.h`

  A `mkdir()` extension which can create all the missing parent directories.
  You can even pass a path that includes a filename and the function
  ignores it.

* `mounts.h`

  Retrieve the list of active mount points on this computer.

* `not_reached.h`

  Mark code location which are expected to never be reached. For example,
  when you create a loop which should never exit, place a:
  `snapdev::NOT_REACHED();` just after the loop. If somehow you add a break
  at the wrong place, your process will call the `NOT_REACHED()` function
  and terminate your program letting you know that you have a bug somewhere.

* `not_used.h`

  When using many virtual functions, you often end up with one or two unused
  variables. You explicitly can mark them as unused with this:
  `snapdev::NOT_USED(a, b, ...);`. This way you avoid the warning and can
  go on with your program.

* `numeric_string.h`

  Create a `char const *` from a number at compile time. This is useful if
  you have numbers defined in a macro or a constant variable (`constexpr`)
  and want to use it as a string somewhere. Tranforming at compile time
  saves you a lot of time while running your software (i.e. this is equivalent
  to `std::to_string()` without runtime processing).

* `ostream_int128.h`

  Allow writing `__int128` numbers to a standard stream.

* `ostream_to_buf.h`

  This is a stream buffer which automatically diverts the data written to an
  output stream to itself. On destruction, it restores the default destination.

  This is particularly useful for our tests which want to capture `std::cout`
  or `std::cerr` output to verify it.

* `pathinfo.h`

  Various functions that parse a valid Unix path and handle transformations
  such as returning the basename, the dirname, the extension and replacing
  those parts.

* `poison.h`

  The `poison.h` is expected to be included by all your .cpp files after all
  the other `#include`s. This allows for poisoning many unsafe C functions
  suchas a `sprintf()` which require magic to make sure you don't overflow
  your output buffer. The `strcat()` and `strncat()` functions are another
  two that use magic to know whether the output buffer will be large enough.

  Of course, you can avoid including and still use those forbidden functions
  safely. It's just much harder to do and C++ offers much more powerful
  replacements anyway.

* `qcaseinsensitivestring.h`

  A Qt extension that defines a `QString` which is case insensitive. This is
  similar to the `case_insensitive_string.h` for Qt string.

* `qstring_extensions.h`

  Various extension for QString, except for the QCaseInsensitiveString.
  Especially, this includes standard stream extensions allowing for
  sending `QString` directly to such a stream.

* `raii_generic_deleter.h`

  Many C resources have to released once used. In most cases, this could
  make it very tedious (i.e. have a `free()` before each `return` statement)
  or you have to use a `goto` to end the functions and do the cleanup in one
  place, or leak the resources.

  This class allows you to use smart pointers to handle all such cases. The
  header includes a class handling file descriptors (an integer rather than
  a pointer). It also includes examples with other types of pointers. The
  main idea is to define a deleter and then use `typedef` once. From there
  on you can use that type which will auto-delete the value. Otherwise, each
  time you create an `std::unique_ptr<>()` or an `std::shared_ptr<>()`, you
  have to remember to define the deleter... (and its syntax, etc.)

* `reverse_cstring.h`

  Allow us to interate through a standard C string (`char const *`) in reverse.
  (i.e. the `begin()` points to the last character and the `end()` points
  just before the first character).

* `safe_assert.h`

  A function used to write an assert. This is safer because all the parameters
  will always be evaluated, even in release mode. If you do not want
  evaluations in your assert, using this `safe_assert()` or the regular
  `assert()` will be exactly the same. Our function is just safer in case
  you missed something important.

* `safe_setenv.h`

  This class is an RAII way of setting an environment variable at runtime
  in your process. Enviuronment variables make use of your input string.
  If you change the input string, it can break everything. Especially, if
  you free that string early (which it is still in the environment) then
  you end up with a dangling pointer. This class allows you to handle
  the `setenv()` safely and in an RAII way too.

* `safe_variable.h`

  Once in a while, you enter a function where you want a variable to be set
  to a different value while running in that function. Once done, you want
  the variable to be restored to its original value. This class allows you
  to do that automatically, using RAII.

* `sizeof_bitfield.h`

  This function computes the size of a bitfield. This is an equivalent to
  the `sizeof <type>` operator, but for bitfields defined in a structure.
  It returns a size in bits. It may not be a multiple of 8.

* `string_replace_many.h`

  A function to replace many segments of a string in one pass. This function
  accepts one input string and a map of `key` and `replacement` strings. If
  `key` is found in the input string, it gets replaced by `replacement`.

* `timespec_ex.h`

  The `timespec` structure is annoying to work with since it presents a
  fixed number. The bottom 9 digits are defined in a separate integer.
  The `timespec_ex` class allows you to seemlessly do all sorts of
  operations (additions, subtractions, etc.) with such numbers.

* `timestamp.h`

  Compute a `time_t` from a date and time at compile time. This is useful
  for our plugins which require a timestamp to define whether they were
  upgraded or not.

* `tokenize_string.h`

  This template parses a strings for tokens and returns them in a specified
  container. The container can be a vector (tokens remain in order) or a
  set (tokens are sorted).

* `to_lower.h`

  Transforms a string to lowercase.

* `to_string_literal.h`

  Transform string and numbers to a string literal at compile time. This
  is useful for filling out structures that expect a `char const *` pointer.

* `to_upper.h`

  Transforms a string to uppercase.

* `trim_string.h`

  Trim a string: at the start, the end, and inside (i.e. keep one space).
  By default, spaces are removed, but you can specify your own set of
  characters that need to be trimmed.

* `user_groups.h`

  Retrieve the list of groups a user is a part of.

* `version.h.in`

  The version of the snapdev library.


## snapmatrix tool

In order to do some matrix math in your console, you can use the snapmatrix
tool. This uses a Forth like language (WARNING: very simplified) which
supports Snap matrices as numbers.

### Create a 4x4 Rotation Matrix

You can create rotation matrices using the `rotate[x|y|z]` command and
a radian angle. For example:

    180 deg2rad rotatez

Creates a rotation matrix of 180° around the Z axis.

### Create a Matrix

Enter each value on the stack, then define the height (number of rows)
and the width (number of columns) of the matrix, finally put the word
`matrix`.

    -1 -1 0 1 4 1 matrix

Creates the matrix (x, y, z, w) = (-1, -1, 0, 1).

You can create an identity matrix using the `identity` command. The
command expects one number representing the number of columns and rows
(since to create an identity matrix you need a square matrix).

    3 identity

This creates a 3x3 matrix.

### Stack Handling

`dup` duplicate the top of the stack.

`swap` swap the two top most elements on the stack.

### Print Content of Stack

Use the `.` command to print the top of the stack.

### Operations

The supported operations are `+`, `-`, `*` and `/`.

The `negate` command can be used to negate a number or matrix.

The `inverse` command can be used to calculate the inverse of a number or
matrix.

### Comments

You can write multiple line comments between parenthesis. Remember that the
opening parenthesis must be followed by at least one space.

    ( This is a multi-
    line comment
    )

You can write a one line command by starting it with a backslash (`\`) and
a space.

    \ A one liner uses a backslash

# License

This project is covered by the GPL 3.0.

# Bugs

Submit bug reports and patches on
[github](https://github.com/m2osw/snapdev/issues).


_This file is part of the [snapcpp project](https://snapwebsites.org/)._
