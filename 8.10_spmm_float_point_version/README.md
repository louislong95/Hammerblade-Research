# 8.10 (tile group dimension 4x4)
The group_matrix_matrix_multiply implements a sparse matrix and dense matrix multiplication, the data distribution is ..., the dimension of the matrix could be changed as parameters. (data type is float point)

kernel version:

1. v0 (4 x 4) perform row by row multiplication
2. v1 (4 x 4) perform blocking strategy, block size could be flexible, and could deal with the condtion when matrix size could not be divided by the block size
3. v2 (4 x 4) perform row by column outer product strategy
4. v3 (4 x 4) perform blocking strategy using outer product strategy