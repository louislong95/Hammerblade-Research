#ifndef __UNROLLING_FUNCTION_HPP
#define __UNROLLING_FUNCTION_HPP
#include <cstdint>
#include <cstring>
#include <bsg_manycore.h>
#include <bsg_tile_group_barrier.h>

inline void dram_to_local_simple(
    float *dest,
    float *src,
    int matrix_height,
    int matrix_width,
    int block_size_row,
    int block_size_col,
    int dim_y,
    int dim_x,
    int blk_row_idx,
    int blk_col_idx
){
    int row_idx = blk_row_idx * block_size_row;
    int col_idx = blk_col_idx * block_size_col;
    for(int i = 0; i < dim_y; i++){
        int row_offset = i * dim_x;
        //for (int j = 0; j < dim_x; j+= 20){
        for (int j = 0; j < dim_x; j+= 20){
            register float tmp0 = src[(row_idx + i) * matrix_width + (col_idx + j)]; // j is always 0 here
            register float tmp1 = src[(row_idx + i) * matrix_width + (col_idx + j + 1)];
            register float tmp2 = src[(row_idx + i) * matrix_width + (col_idx + j + 2)];
            register float tmp3 = src[(row_idx + i) * matrix_width + (col_idx + j + 3)];
            register float tmp4 = src[(row_idx + i) * matrix_width + (col_idx + j + 4)];
            register float tmp5 = src[(row_idx + i) * matrix_width + (col_idx + j + 5)];
            register float tmp6 = src[(row_idx + i) * matrix_width + (col_idx + j + 6)];
            register float tmp7 = src[(row_idx + i) * matrix_width + (col_idx + j + 7)];
            register float tmp8 = src[(row_idx + i) * matrix_width + (col_idx + j + 8)];
            register float tmp9 = src[(row_idx + i) * matrix_width + (col_idx + j + 9)];
            register float tmp10 = src[(row_idx + i) * matrix_width + (col_idx + j + 10)];
            register float tmp11 = src[(row_idx + i) * matrix_width + (col_idx + j + 11)];
            register float tmp12 = src[(row_idx + i) * matrix_width + (col_idx + j + 12)];
            register float tmp13 = src[(row_idx + i) * matrix_width + (col_idx + j + 13)];
            register float tmp14 = src[(row_idx + i) * matrix_width + (col_idx + j + 14)];
            register float tmp15 = src[(row_idx + i) * matrix_width + (col_idx + j + 15)];
            register float tmp16 = src[(row_idx + i) * matrix_width + (col_idx + j + 16)];
            register float tmp17 = src[(row_idx + i) * matrix_width + (col_idx + j + 17)];
            register float tmp18 = src[(row_idx + i) * matrix_width + (col_idx + j + 18)];
            register float tmp19 = src[(row_idx + i) * matrix_width + (col_idx + j + 19)];
            asm volatile("": : : "memory"); // need to figure out what is this
            dest[row_offset + j] = tmp0;
            dest[row_offset + j + 1] = tmp1;
            dest[row_offset + j + 2] = tmp2;
            dest[row_offset + j + 3] = tmp3;
            dest[row_offset + j + 4] = tmp4;
            dest[row_offset + j + 5] = tmp5;
            dest[row_offset + j + 6] = tmp6;
            dest[row_offset + j + 7] = tmp7;
            dest[row_offset + j + 8] = tmp8;
            dest[row_offset + j + 9] = tmp9;
            dest[row_offset + j + 10] = tmp10;
            dest[row_offset + j + 11] = tmp11;
            dest[row_offset + j + 12] = tmp12;
            dest[row_offset + j + 13] = tmp13;
            dest[row_offset + j + 14] = tmp14;
            dest[row_offset + j + 15] = tmp15;
            dest[row_offset + j + 16] = tmp16;
            dest[row_offset + j + 17] = tmp17;
            dest[row_offset + j + 18] = tmp18;
            dest[row_offset + j + 19] = tmp19;

        }
    }
}

inline void compute_10(
    float *dest,
    float *A_local_mat,
    float *B_local_mat,
    int A_local_row,
    int A_local_col,
    int B_local_row,
    int B_local_col
){
    for (int i = 0; i < A_local_row; i++){
        int dest_row_offset = i * B_local_row;
        int A_row_offset = i * A_local_col;
        register float res0 = dest[dest_row_offset + 0];
        register float res1 = dest[dest_row_offset + 1];
        register float res2 = dest[dest_row_offset + 2];
        register float res3 = dest[dest_row_offset + 3];
        register float res4 = dest[dest_row_offset + 4];
        register float res5 = dest[dest_row_offset + 5];
        register float res6 = dest[dest_row_offset + 6];
        register float res7 = dest[dest_row_offset + 7];
        register float res8 = dest[dest_row_offset + 8];
        register float res9 = dest[dest_row_offset + 9];
        for (int j = 0; j < B_local_col; j++){
            register float A_mat = A_local_mat[A_row_offset + j];
            res0 += (A_mat * B_local_mat[0 + j]);
            res1 += (A_mat * B_local_mat[1 * B_local_col + j]);
            res2 += (A_mat * B_local_mat[2 * B_local_col + j]);
            res3 += (A_mat * B_local_mat[3 * B_local_col + j]);
            res4 += (A_mat * B_local_mat[4 * B_local_col + j]);
            res5 += (A_mat * B_local_mat[5 * B_local_col + j]);
            res6 += (A_mat * B_local_mat[6 * B_local_col + j]);
            res7 += (A_mat * B_local_mat[7 * B_local_col + j]);
            res8 += (A_mat * B_local_mat[8 * B_local_col + j]);
            res9 += (A_mat * B_local_mat[9 * B_local_col + j]);
        }
        dest[dest_row_offset + 0] = res0;
        dest[dest_row_offset + 1] = res1;
        dest[dest_row_offset + 2] = res2;
        dest[dest_row_offset + 3] = res3;
        dest[dest_row_offset + 4] = res4;
        dest[dest_row_offset + 5] = res5;
        dest[dest_row_offset + 6] = res6;
        dest[dest_row_offset + 7] = res7;
        dest[dest_row_offset + 8] = res8;
        dest[dest_row_offset + 9] = res9;
    }

}

inline void compute_9(
    float *dest,
    float *A_local_mat,
    float *B_local_mat,
    int A_local_row,
    int A_local_col,
    int B_local_row,
    int B_local_col
){
    for (int i = 0; i < A_local_row; i++){
        int dest_row_offset = i * B_local_row;
        int A_row_offset = i * A_local_col;
        register float res0 = dest[dest_row_offset + 0];
        register float res1 = dest[dest_row_offset + 1];
        register float res2 = dest[dest_row_offset + 2];
        register float res3 = dest[dest_row_offset + 3];
        register float res4 = dest[dest_row_offset + 4];
        register float res5 = dest[dest_row_offset + 5];
        register float res6 = dest[dest_row_offset + 6];
        register float res7 = dest[dest_row_offset + 7];
        register float res8 = dest[dest_row_offset + 8];
        for (int j = 0; j < B_local_col; j++){
            register float A_mat = A_local_mat[A_row_offset + j];
            res0 += (A_mat * B_local_mat[0 + j]);
            res1 += (A_mat * B_local_mat[1 * B_local_col + j]);
            res2 += (A_mat * B_local_mat[2 * B_local_col + j]);
            res3 += (A_mat * B_local_mat[3 * B_local_col + j]);
            res4 += (A_mat * B_local_mat[4 * B_local_col + j]);
            res5 += (A_mat * B_local_mat[5 * B_local_col + j]);
            res6 += (A_mat * B_local_mat[6 * B_local_col + j]);
            res7 += (A_mat * B_local_mat[7 * B_local_col + j]);
            res8 += (A_mat * B_local_mat[8 * B_local_col + j]);
        }
        dest[dest_row_offset + 0] = res0;
        dest[dest_row_offset + 1] = res1;
        dest[dest_row_offset + 2] = res2;
        dest[dest_row_offset + 3] = res3;
        dest[dest_row_offset + 4] = res4;
        dest[dest_row_offset + 5] = res5;
        dest[dest_row_offset + 6] = res6;
        dest[dest_row_offset + 7] = res7;
        dest[dest_row_offset + 8] = res8;
    }

}

inline void compute_8(
    float *dest,
    float *A_local_mat,
    float *B_local_mat,
    int A_local_row,
    int A_local_col,
    int B_local_row,
    int B_local_col
){
    for (int i = 0; i < A_local_row; i++){
        int dest_row_offset = i * B_local_row;
        int A_row_offset = i * A_local_col;
        register float res0 = dest[dest_row_offset + 0];
        register float res1 = dest[dest_row_offset + 1];
        register float res2 = dest[dest_row_offset + 2];
        register float res3 = dest[dest_row_offset + 3];
        register float res4 = dest[dest_row_offset + 4];
        register float res5 = dest[dest_row_offset + 5];
        register float res6 = dest[dest_row_offset + 6];
        register float res7 = dest[dest_row_offset + 7];
        for (int j = 0; j < B_local_col; j++){
            register float A_mat = A_local_mat[A_row_offset + j];
            res0 += (A_mat * B_local_mat[0 + j]);
            res1 += (A_mat * B_local_mat[1 * B_local_col + j]);
            res2 += (A_mat * B_local_mat[2 * B_local_col + j]);
            res3 += (A_mat * B_local_mat[3 * B_local_col + j]);
            res4 += (A_mat * B_local_mat[4 * B_local_col + j]);
            res5 += (A_mat * B_local_mat[5 * B_local_col + j]);
            res6 += (A_mat * B_local_mat[6 * B_local_col + j]);
            res7 += (A_mat * B_local_mat[7 * B_local_col + j]);
        }
        dest[dest_row_offset + 0] = res0;
        dest[dest_row_offset + 1] = res1;
        dest[dest_row_offset + 2] = res2;
        dest[dest_row_offset + 3] = res3;
        dest[dest_row_offset + 4] = res4;
        dest[dest_row_offset + 5] = res5;
        dest[dest_row_offset + 6] = res6;
        dest[dest_row_offset + 7] = res7;
    }

}

inline void compute_7(
    float *dest,
    float *A_local_mat,
    float *B_local_mat,
    int A_local_row,
    int A_local_col,
    int B_local_row,
    int B_local_col
){
    for (int i = 0; i < A_local_row; i++){
        int dest_row_offset = i * B_local_row;
        int A_row_offset = i * A_local_col;
        register float res0 = dest[dest_row_offset + 0];
        register float res1 = dest[dest_row_offset + 1];
        register float res2 = dest[dest_row_offset + 2];
        register float res3 = dest[dest_row_offset + 3];
        register float res4 = dest[dest_row_offset + 4];
        register float res5 = dest[dest_row_offset + 5];
        register float res6 = dest[dest_row_offset + 6];
        for (int j = 0; j < B_local_col; j++){
            register float A_mat = A_local_mat[A_row_offset + j];
            res0 += (A_mat * B_local_mat[0 + j]);
            res1 += (A_mat * B_local_mat[1 * B_local_col + j]);
            res2 += (A_mat * B_local_mat[2 * B_local_col + j]);
            res3 += (A_mat * B_local_mat[3 * B_local_col + j]);
            res4 += (A_mat * B_local_mat[4 * B_local_col + j]);
            res5 += (A_mat * B_local_mat[5 * B_local_col + j]);
            res6 += (A_mat * B_local_mat[6 * B_local_col + j]);
        }
        dest[dest_row_offset + 0] = res0;
        dest[dest_row_offset + 1] = res1;
        dest[dest_row_offset + 2] = res2;
        dest[dest_row_offset + 3] = res3;
        dest[dest_row_offset + 4] = res4;
        dest[dest_row_offset + 5] = res5;
        dest[dest_row_offset + 6] = res6;
    }

}

inline void compute_6(
    float *dest,
    float *A_local_mat,
    float *B_local_mat,
    int A_local_row,
    int A_local_col,
    int B_local_row,
    int B_local_col
){
    for (int i = 0; i < A_local_row; i++){
        int dest_row_offset = i * B_local_row;
        int A_row_offset = i * A_local_col;
        register float res0 = dest[dest_row_offset + 0];
        register float res1 = dest[dest_row_offset + 1];
        register float res2 = dest[dest_row_offset + 2];
        register float res3 = dest[dest_row_offset + 3];
        register float res4 = dest[dest_row_offset + 4];
        register float res5 = dest[dest_row_offset + 5];
        for (int j = 0; j < B_local_col; j++){
            register float A_mat = A_local_mat[A_row_offset + j];
            res0 += (A_mat * B_local_mat[0 + j]);
            res1 += (A_mat * B_local_mat[1 * B_local_col + j]);
            res2 += (A_mat * B_local_mat[2 * B_local_col + j]);
            res3 += (A_mat * B_local_mat[3 * B_local_col + j]);
            res4 += (A_mat * B_local_mat[4 * B_local_col + j]);
            res5 += (A_mat * B_local_mat[5 * B_local_col + j]);
        }
        dest[dest_row_offset + 0] = res0;
        dest[dest_row_offset + 1] = res1;
        dest[dest_row_offset + 2] = res2;
        dest[dest_row_offset + 3] = res3;
        dest[dest_row_offset + 4] = res4;
        dest[dest_row_offset + 5] = res5;
    }

}

inline void compute_5(
    float *dest,
    float *A_local_mat,
    float *B_local_mat,
    int A_local_row,
    int A_local_col,
    int B_local_row,
    int B_local_col
){
    for (int i = 0; i < A_local_row; i++){
        int dest_row_offset = i * B_local_row;
        int A_row_offset = i * A_local_col;
        register float res0 = dest[dest_row_offset + 0];
        register float res1 = dest[dest_row_offset + 1];
        register float res2 = dest[dest_row_offset + 2];
        register float res3 = dest[dest_row_offset + 3];
        register float res4 = dest[dest_row_offset + 4];
        for (int j = 0; j < B_local_col; j++){
            register float A_mat = A_local_mat[A_row_offset + j];
            res0 += (A_mat * B_local_mat[0 + j]);
            res1 += (A_mat * B_local_mat[1 * B_local_col + j]);
            res2 += (A_mat * B_local_mat[2 * B_local_col + j]);
            res3 += (A_mat * B_local_mat[3 * B_local_col + j]);
            res4 += (A_mat * B_local_mat[4 * B_local_col + j]);
        }
        dest[dest_row_offset + 0] = res0;
        dest[dest_row_offset + 1] = res1;
        dest[dest_row_offset + 2] = res2;
        dest[dest_row_offset + 3] = res3;
        dest[dest_row_offset + 4] = res4;
    }

}

inline void compute_4(
    float *dest,
    float *A_local_mat,
    float *B_local_mat,
    int A_local_row,
    int A_local_col,
    int B_local_row,
    int B_local_col
){
    for (int i = 0; i < A_local_row; i++){
        int dest_row_offset = i * B_local_row;
        int A_row_offset = i * A_local_col;
        register float res0 = dest[dest_row_offset + 0];
        register float res1 = dest[dest_row_offset + 1];
        register float res2 = dest[dest_row_offset + 2];
        register float res3 = dest[dest_row_offset + 3];
        for (int j = 0; j < B_local_col; j++){
            register float A_mat = A_local_mat[A_row_offset + j];
            res0 += (A_mat * B_local_mat[0 + j]);
            res1 += (A_mat * B_local_mat[1 * B_local_col + j]);
            res2 += (A_mat * B_local_mat[2 * B_local_col + j]);
            res3 += (A_mat * B_local_mat[3 * B_local_col + j]);
        }
        dest[dest_row_offset + 0] = res0;
        dest[dest_row_offset + 1] = res1;
        dest[dest_row_offset + 2] = res2;
        dest[dest_row_offset + 3] = res3;
    }

}

inline void compute_3(
    float *dest,
    float *A_local_mat,
    float *B_local_mat,
    int A_local_row,
    int A_local_col,
    int B_local_row,
    int B_local_col
){
    for (int i = 0; i < A_local_row; i++){
        int dest_row_offset = i * B_local_row;
        int A_row_offset = i * A_local_col;
        register float res0 = dest[dest_row_offset + 0];
        register float res1 = dest[dest_row_offset + 1];
        register float res2 = dest[dest_row_offset + 2];
        for (int j = 0; j < B_local_col; j++){
            register float A_mat = A_local_mat[A_row_offset + j];
            res0 += (A_mat * B_local_mat[0 + j]);
            res1 += (A_mat * B_local_mat[1 * B_local_col + j]);
            res2 += (A_mat * B_local_mat[2 * B_local_col + j]);
        }
        dest[dest_row_offset + 0] = res0;
        dest[dest_row_offset + 1] = res1;
        dest[dest_row_offset + 2] = res2;
    }

}

inline void compute_2(
    float *dest,
    float *A_local_mat,
    float *B_local_mat,
    int A_local_row,
    int A_local_col,
    int B_local_row,
    int B_local_col
){
    for (int i = 0; i < A_local_row; i++){
        int dest_row_offset = i * B_local_row;
        int A_row_offset = i * A_local_col;
        register float res0 = dest[dest_row_offset + 0];
        register float res1 = dest[dest_row_offset + 1];
        for (int j = 0; j < B_local_col; j++){
            register float A_mat = A_local_mat[A_row_offset + j];
            res0 += (A_mat * B_local_mat[0 + j]);
            res1 += (A_mat * B_local_mat[1 * B_local_col + j]);
        }
        dest[dest_row_offset + 0] = res0;
        dest[dest_row_offset + 1] = res1;
    }

}

inline void compute_1(
    float *dest,
    float *A_local_mat,
    float *B_local_mat,
    int A_local_row,
    int A_local_col,
    int B_local_row,
    int B_local_col
){
    for (int i = 0; i < A_local_row; i++){
        int dest_row_offset = i * B_local_row;
        int A_row_offset = i * A_local_col;
        register float res0 = dest[dest_row_offset + 0];
        for (int j = 0; j < B_local_col; j++){
            register float A_mat = A_local_mat[A_row_offset + j];
            res0 += (A_mat * B_local_mat[0 + j]);
        }
        dest[dest_row_offset + 0] = res0;
    }

}


#endif 
