
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

The library is documented using Doxygen.

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

The source is covered by the MIT license. The debian folder is covered
by the GPL 2.0.

# Bugs

Submit bug reports and patches on
[github](https://github.com/m2osw/snapdev/issues).


_This file is part of the [snapcpp project](https://snapwebsites.org/)._
