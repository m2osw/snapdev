( sample positioning a shrunk rectangle
)

3 identity

\ 2D rotation (or rotation around z): cos -sin 0 / sin cos 0 / 0 0 1
-1 0 0 0 -1 0 0 0 1 3 3 matrix

18 5 1 1 3 matrix
dup
.
*

\ 1 0 0.3 0 1 0.4 0 0 1 3 3 matrix *
\ 2.5 *
\ 1 0 -0.3 0 1 -0.4 0 0 1 3 3 matrix *

.

\ vim: syntax=forth
