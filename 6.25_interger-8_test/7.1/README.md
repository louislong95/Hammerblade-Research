# 6.20
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
9. v8 (2 x 2)implement the data distribution along column side, with scratchpad, make use of Matrix tranpose
10. v9 (2 x 2)implement the data distribution with row block, with scratchpad
11. v10 (2 x 2)implement the data distribution with row side, with scratchpad storing specific data instead of the whole matrix
12. v11 (2 x 2)implement the data distribution with col side, with scratchpad storing specific data instead of the whole matrix, matrix transposing to allow the whole row to be stored in the scratchpad
13. v12 (2 x 2)implement the data distribution with col side, with scratchpad storing specific data instead of the whole matrix
14. v13 (2 x 2)implement the data distribution with col side, with scratchpad storing temperal value in order to perform large matrix multiplication, matrix transposing to allow the whole row to be stored in the scratchpad