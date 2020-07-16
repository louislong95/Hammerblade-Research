# 6.19
The group_matrix_matrix_multiply implements a dense matrix and dense matrix multiplication, the data distribution is along the row side, the dimension of the matrix could be changed as parameters.

kernel version:

1. v0 (1 x 4)implement the data distribution along row side, with scratchpad;
2. v1 (1 x 4)implement the data distribution along column side, with scratchpad;
3. v2 (2 x 2)implement the data distribution along row side, without scratchpad;
4. v3 (2 x 2)implement the data distribution along column side, without scratchpad;
5. v4 (2 x 2)implement the data distribution along row side, with scratchpad;
6. v5 (2 x 2)implement the data distribution along column side, with scratchpad;
7. v6 (4 x 1)implement the data distribution along row side, with scratchpad;
8. v7 (4 x 1)implement the data distribution along column side, with scratchpad;
