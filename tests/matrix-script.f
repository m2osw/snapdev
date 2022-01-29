( This is a Forth-like script used by the snapmatrix tool to run computation
  on matrix with the snapdev::matrix implementation.
)

180 deg2rad rotatez
dup
-1 -1 0 1 4 1 matrix
swap
*
.

dup
-1 1 0 1 4 1 matrix
swap
*
.

dup
1 1 0 1 4 1 matrix
swap
*
.

dup
-1 -1 0 1 4 1 matrix
swap
*
.

dup
1 1 0 1 4 1 matrix
swap
*
.

dup
1 -1 0 1 4 1 matrix
swap
*
.

\ vim: syntax=forth
