# 7.16 (tile group dimension 4x4)
The group_matrix_matrix_multiply implements a dense matrix and dense matrix multiplication, the data distribution is along the row side, the dimension of the matrix could be changed as parameters. (data type is float point)

kernel version:

1. v0 (4 x 4)implement the data distribution along row side, with scratchpad;
2. v1 (4 x 4)implement the data distribution along column side, with scratchpad;
3. v2 (4 x 4)implement the data distribution along row side, without scratchpad;
4. v3 (4 x 4)implement the data distribution along column side, without scratchpad;
5. v4 (4 x 4)implement the data distribution along row side, with scratchpad;
6. v5 (4 x 4)implement the data distribution along column side, with scratchpad;
7. v6 (4 x 4)implement the data distribution along row side, with scratchpad;
8. v7 (4 x 4)implement the data distribution along column side, with scratchpad;
9. v8 (4 x 4)implement the data distribution along column side, with scratchpad, make use of Matrix tranpose
10. v9 (4 x 4)implement the data distribution with row block, with scratchpad
11. v10 (4 x 4)implement the data distribution with row side, with scratchpad storing specific data instead of the whole matrix
12. v11 (4 x 4)implement the data distribution with col side, with scratchpad storing specific data instead of the whole matrix, matrix transposing to allow the whole row to be stored in the scratchpad
13. v12 (4 x 4)implement the data distribution with col side, with scratchpad storing specific data instead of the whole matrix
14. v13 (4 x 4)implement the data distribution with col side, with scratchpad storing temperal value in order to perform large matrix multiplication, matrix transposing to allow the whole row to be stored in the scratchpad
15. v15 (2 x 2)implement the data distribution with 2 x 2 matrix distribution, with scratchpad loading the matrix block, matrix B transposing
16. v16 (4 x 4)implement the data distribution along column side, make use of Matrix B tranpose, with shared_memory
17. v17 (2 x 2)implement the data distribution with 2 x 2 matrix distribution, with shared_memory loading the matrix block, matrix B transposing
18. v18 (4 x 4)implement the dd matrix mulitplication with blocking strategy, using scratchpad to store the matrix blocks, matrix B transposing
19. v19 (4 x 4)implement the dd matrix mulitplication with blocking strategy, using scratchpad to store the matrix blocks, matrix B transposing,suitable for non-square block size
20. v20 (4 x 4)implement the dd matrix mulitplication with blocking strategy, using scratchpad to store the matrix row or matrix blocks, can switch between row by row multiplication and blocking strategy, matrix B transposing,suitable for all matrix size
21. v21 (4 x 4)implement the dd matrix mulitplication with improved blocking strategy, using scratchpad to store the matrix blocks, matrix B transposing,suitable for all matrix size, block size must be square, but could also deal with the condition when the matrix size could not be divided by the block size