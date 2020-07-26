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
            register float tmp8 = src[(blk_row_idx * block_size_row + i) * matrix_width + (blk_col_idx * block_size_col + j + 8)];
            register float tmp9 = src[(blk_row_idx * block_size_row + i) * matrix_width + (blk_col_idx * block_size_col + j + 9)];
            register float tmp10 = src[(blk_row_idx * block_size_row + i) * matrix_width + (blk_col_idx * block_size_col + j + 10)];
            register float tmp11 = src[(blk_row_idx * block_size_row + i) * matrix_width + (blk_col_idx * block_size_col + j + 11)];
            register float tmp12 = src[(blk_row_idx * block_size_row + i) * matrix_width + (blk_col_idx * block_size_col + j + 12)];
            register float tmp13 = src[(blk_row_idx * block_size_row + i) * matrix_width + (blk_col_idx * block_size_col + j + 13)];
            register float tmp14 = src[(blk_row_idx * block_size_row + i) * matrix_width + (blk_col_idx * block_size_col + j + 14)];
            register float tmp15 = src[(blk_row_idx * block_size_row + i) * matrix_width + (blk_col_idx * block_size_col + j + 15)];
            register float tmp16 = src[(blk_row_idx * block_size_row + i) * matrix_width + (blk_col_idx * block_size_col + j + 16)];
            register float tmp17 = src[(blk_row_idx * block_size_row + i) * matrix_width + (blk_col_idx * block_size_col + j + 17)];
            register float tmp18 = src[(blk_row_idx * block_size_row + i) * matrix_width + (blk_col_idx * block_size_col + j + 18)];
            register float tmp19 = src[(blk_row_idx * block_size_row + i) * matrix_width + (blk_col_idx * block_size_col + j + 19)];
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

inline void compute(
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
        for (int j =0; j < B_local_row; j++){
            for (int k = 0; k < A_local_col; k+=20){
                int A_local_idx = A_row_offset;
                int B_local_idx = j * B_local_col;
                register float tmp0 = A_local_mat[A_local_idx] * B_local_mat[B_local_idx];
                register float tmp1 = A_local_mat[A_local_idx + 1] * B_local_mat[B_local_idx + 1];
                register float tmp2 = A_local_mat[A_local_idx + 2] * B_local_mat[B_local_idx + 2];
                register float tmp3 = A_local_mat[A_local_idx + 3] * B_local_mat[B_local_idx + 3];
                register float tmp4 = A_local_mat[A_local_idx + 4] * B_local_mat[B_local_idx + 4];
                register float tmp5 = A_local_mat[A_local_idx + 5] * B_local_mat[B_local_idx + 5];
                register float tmp6 = A_local_mat[A_local_idx + 6] * B_local_mat[B_local_idx + 6];
                register float tmp7 = A_local_mat[A_local_idx + 7] * B_local_mat[B_local_idx + 7];
                register float tmp8 = A_local_mat[A_local_idx + 8] * B_local_mat[B_local_idx + 8];
                register float tmp9 = A_local_mat[A_local_idx + 9] * B_local_mat[B_local_idx + 9];
                register float tmp10 = A_local_mat[A_local_idx + 10] * B_local_mat[B_local_idx + 10];
                register float tmp11 = A_local_mat[A_local_idx + 11] * B_local_mat[B_local_idx + 11];
                register float tmp12 = A_local_mat[A_local_idx + 12] * B_local_mat[B_local_idx + 12];
                register float tmp13 = A_local_mat[A_local_idx + 13] * B_local_mat[B_local_idx + 13];
                register float tmp14 = A_local_mat[A_local_idx + 14] * B_local_mat[B_local_idx + 14];
                register float tmp15 = A_local_mat[A_local_idx + 15] * B_local_mat[B_local_idx + 15];
                register float tmp16 = A_local_mat[A_local_idx + 16] * B_local_mat[B_local_idx + 16];
                register float tmp17 = A_local_mat[A_local_idx + 17] * B_local_mat[B_local_idx + 17];
                register float tmp18 = A_local_mat[A_local_idx + 18] * B_local_mat[B_local_idx + 18];
                register float tmp19 = A_local_mat[A_local_idx + 19] * B_local_mat[B_local_idx + 19];
                dest[dest_row_offset + j] += (tmp0 + tmp1 + tmp2 + tmp3 + tmp4 + tmp5 + tmp6 + tmp7 + tmp8 + tmp9 + tmp10
                                                   + tmp11 + tmp12 + tmp13 + tmp14 + tmp15 + tmp16 + tmp17 + tmp18 + tmp19);
            }
        }
    }

}

#endif 
