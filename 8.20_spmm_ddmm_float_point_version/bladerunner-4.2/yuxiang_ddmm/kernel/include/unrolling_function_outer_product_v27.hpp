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

inline void compute_flex(
    float *src,
    float *A_local_mat,
    float *B_local_mat,
    int A_local_row,
    int A_local_col,
    int B_local_row,
    int B_local_col,
    int mem_row_blk_id,
    int mem_col_blk_id,
    int B_Width,
    int blk_size_row,
    int blk_size_col
){
    int src_row_offset = (mem_row_blk_id * blk_size_row);
    int src_col_offset = (mem_col_blk_id * blk_size_row);
    int tmp_idx = 0;
    float tmp_result[A_local_row * B_local_row];
    float sum = 0;
    for (int src_row_id = src_row_offset; src_row_id < src_row_offset + A_local_row; src_row_id ++){
        memcpy(&tmp_result[tmp_idx * B_local_row], &src[src_row_id * B_Width + src_col_offset], sizeof(float) * B_local_row);
        tmp_idx ++;
    }
    for (int i = 0; i < A_local_row; i++){
        for (int j = 0; j < B_local_row; j++){
            sum = tmp_result[i * B_local_row + j];
            for (int k = 0; k < A_local_col; k++){
                sum += A_local_mat[i * A_local_col + k] * B_local_mat[j * B_local_col + k];
            }
            src[(src_row_offset + i) * B_Width + src_col_offset + j] = sum;
        }
        
    }

}

inline void compute_8(
    float *dest,
    float *src,
    float *A_local_mat,
    float *B_local_mat,
    int A_local_row,
    int A_local_col,
    int B_local_row,
    int B_local_col,
    int mem_row_blk_id,
    int mem_col_blk_id,
    int B_Width
){
    for (int i = 0; i < A_local_row; i++){
        int src_row_offset = (mem_row_blk_id * A_local_row + i) * B_Width;
        int src_col_offset = (mem_col_blk_id * B_local_row);
        int dest_row_offset = i * B_local_row;
        int A_row_offset = i * A_local_col;
        register float res0 = src[src_row_offset + src_col_offset + 0];
        register float res1 = src[src_row_offset + src_col_offset + 1];
        register float res2 = src[src_row_offset + src_col_offset + 2];
        register float res3 = src[src_row_offset + src_col_offset + 3];
        register float res4 = src[src_row_offset + src_col_offset + 4];
        register float res5 = src[src_row_offset + src_col_offset + 5];
        register float res6 = src[src_row_offset + src_col_offset + 6];
        register float res7 = src[src_row_offset + src_col_offset + 7];
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
        src[src_row_offset + src_col_offset + 0] = res0;
        src[src_row_offset + src_col_offset + 1] = res1;
        src[src_row_offset + src_col_offset + 2] = res2;
        src[src_row_offset + src_col_offset + 3] = res3;
        src[src_row_offset + src_col_offset + 4] = res4;
        src[src_row_offset + src_col_offset + 5] = res5;
        src[src_row_offset + src_col_offset + 6] = res6;
        src[src_row_offset + src_col_offset + 7] = res7;
    }

}

inline void compute_4(
    float *dest,
    float *src,
    float *A_local_mat,
    float *B_local_mat,
    int A_local_row,
    int A_local_col,
    int B_local_row,
    int B_local_col,
    int mem_row_blk_id,
    int mem_col_blk_id,
    int B_Width
){
    for (int i = 0; i < A_local_row; i++){
        int src_row_offset = (mem_row_blk_id * A_local_row + i) * B_Width;
        int src_col_offset = (mem_col_blk_id * B_local_row);
        int dest_row_offset = i * B_local_row;
        int A_row_offset = i * A_local_col;
        register float res0 = src[src_row_offset + src_col_offset + 0];
        register float res1 = src[src_row_offset + src_col_offset + 1];
        register float res2 = src[src_row_offset + src_col_offset + 2];
        register float res3 = src[src_row_offset + src_col_offset + 3];
        for (int j = 0; j < B_local_col; j++){
            register float A_mat = A_local_mat[A_row_offset + j];
            res0 += (A_mat * B_local_mat[0 + j]);
            res1 += (A_mat * B_local_mat[1 * B_local_col + j]);
            res2 += (A_mat * B_local_mat[2 * B_local_col + j]);
            res3 += (A_mat * B_local_mat[3 * B_local_col + j]);
        }
        src[src_row_offset + src_col_offset + 0] = res0;
        src[src_row_offset + src_col_offset + 1] = res1;
        src[src_row_offset + src_col_offset + 2] = res2;
        src[src_row_offset + src_col_offset + 3] = res3;
    }

}

inline void compute_2(
    float *dest,
    float *src,
    float *A_local_mat,
    float *B_local_mat,
    int A_local_row,
    int A_local_col,
    int B_local_row,
    int B_local_col,
    int mem_row_blk_id,
    int mem_col_blk_id,
    int B_Width
){
    for (int i = 0; i < A_local_row; i++){
        int src_row_offset = (mem_row_blk_id * A_local_row + i) * B_Width;
        int src_col_offset = (mem_col_blk_id * B_local_row);
        int dest_row_offset = i * B_local_row;
        int A_row_offset = i * A_local_col;
        register float res0 = src[src_row_offset + src_col_offset + 0];
        register float res1 = src[src_row_offset + src_col_offset + 1];
        for (int j = 0; j < B_local_col; j++){
            register float A_mat = A_local_mat[A_row_offset + j];
            res0 += (A_mat * B_local_mat[0 + j]);
            res1 += (A_mat * B_local_mat[1 * B_local_col + j]);
        }
        //dest[dest_row_offset + 0] = res0;
        //dest[dest_row_offset + 1] = res1;
        src[src_row_offset + src_col_offset + 0] = res0;
        src[src_row_offset + src_col_offset + 1] = res1;

    }

}

inline void compute_1(
    float *dest,
    float *src,
    float *A_local_mat,
    float *B_local_mat,
    int A_local_row,
    int A_local_col,
    int B_local_row,
    int B_local_col,
    int mem_row_blk_id,
    int mem_col_blk_id,
    int B_Width
){
    for (int i = 0; i < A_local_row; i++){
        int src_row_offset = (mem_row_blk_id * A_local_row + i) * B_Width;
        int src_col_offset = (mem_col_blk_id * B_local_row);
        int dest_row_offset = i * B_local_row;
        int A_row_offset = i * A_local_col;
        register float res0 = src[src_row_offset + src_col_offset + 0];
        for (int j = 0; j < B_local_col; j++){
            register float A_mat = A_local_mat[A_row_offset + j];
            res0 += (A_mat * B_local_mat[0 + j]);
        }
        //dest[dest_row_offset + 0] = res0;
        //dest[dest_row_offset + 1] = res1;
        src[src_row_offset + src_col_offset + 0] = res0;

    }

}

#endif 
