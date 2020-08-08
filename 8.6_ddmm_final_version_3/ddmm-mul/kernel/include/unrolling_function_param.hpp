#ifndef __UNROLLING_FUNCTION_PARAM_HPP
#define __UNROLLING_FUNCTION_PARAM_HPP
#include <cstdint>
#include <cstring>
#include <bsg_manycore.h>
#include <bsg_tile_group_barrier.h>

inline void dram_to_local_simple_1(
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
        for (int j = 0; j < dim_x; j+= 1){
            
            float tmp0 = src[(row_idx + i) * matrix_width + (col_idx + j)]; // j is always 0 here
        
            asm volatile("": : : "memory"); // need to figure out what is this
            
            dest[row_offset + j] = tmp0;

        }
    }
}

inline void dram_to_local_simple_2(
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
        for (int j = 0; j < dim_x; j+= 2){
            
            float tmp0 = src[(row_idx + i) * matrix_width + (col_idx + j)]; // j is always 0 here
            float tmp1 = src[(row_idx + i) * matrix_width + (col_idx + j + 1)];
        
            asm volatile("": : : "memory"); // need to figure out what is this
            
            dest[row_offset + j] = tmp0;
            dest[row_offset + j + 1] = tmp1;

        }
    }
}

inline void dram_to_local_simple_3(
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
        for (int j = 0; j < dim_x; j+= 3){
            
            float tmp0 = src[(row_idx + i) * matrix_width + (col_idx + j)]; // j is always 0 here
            float tmp1 = src[(row_idx + i) * matrix_width + (col_idx + j + 1)];
            float tmp2 = src[(row_idx + i) * matrix_width + (col_idx + j + 2)];
        
            asm volatile("": : : "memory"); // need to figure out what is this
            
            dest[row_offset + j] = tmp0;
            dest[row_offset + j + 1] = tmp1;
            dest[row_offset + j + 2] = tmp2;

        }
    }
}

inline void dram_to_local_simple_4(
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
        for (int j = 0; j < dim_x; j+= 4){
            
            float tmp0 = src[(row_idx + i) * matrix_width + (col_idx + j)]; // j is always 0 here
            float tmp1 = src[(row_idx + i) * matrix_width + (col_idx + j + 1)];
            float tmp2 = src[(row_idx + i) * matrix_width + (col_idx + j + 2)];
            float tmp3 = src[(row_idx + i) * matrix_width + (col_idx + j + 3)];
        
            asm volatile("": : : "memory"); // need to figure out what is this
            
            dest[row_offset + j] = tmp0;
            dest[row_offset + j + 1] = tmp1;
            dest[row_offset + j + 2] = tmp2;
            dest[row_offset + j + 3] = tmp3;

        }
    }
}

inline void dram_to_local_simple_5(
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
        for (int j = 0; j < dim_x; j+= 5){
            
            float tmp0 = src[(row_idx + i) * matrix_width + (col_idx + j)]; // j is always 0 here
            float tmp1 = src[(row_idx + i) * matrix_width + (col_idx + j + 1)];
            float tmp2 = src[(row_idx + i) * matrix_width + (col_idx + j + 2)];
            float tmp3 = src[(row_idx + i) * matrix_width + (col_idx + j + 3)];
            float tmp4 = src[(row_idx + i) * matrix_width + (col_idx + j + 4)];
        
            asm volatile("": : : "memory"); // need to figure out what is this
            
            dest[row_offset + j] = tmp0;
            dest[row_offset + j + 1] = tmp1;
            dest[row_offset + j + 2] = tmp2;
            dest[row_offset + j + 3] = tmp3;
            dest[row_offset + j + 4] = tmp4;

        }
    }
}

inline void dram_to_local_simple_6(
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
        for (int j = 0; j < dim_x; j+= 6){
            
            float tmp0 = src[(row_idx + i) * matrix_width + (col_idx + j)]; // j is always 0 here
            float tmp1 = src[(row_idx + i) * matrix_width + (col_idx + j + 1)];
            float tmp2 = src[(row_idx + i) * matrix_width + (col_idx + j + 2)];
            float tmp3 = src[(row_idx + i) * matrix_width + (col_idx + j + 3)];
            float tmp4 = src[(row_idx + i) * matrix_width + (col_idx + j + 4)];
            float tmp5 = src[(row_idx + i) * matrix_width + (col_idx + j + 5)];
        
            asm volatile("": : : "memory"); // need to figure out what is this
            
            dest[row_offset + j] = tmp0;
            dest[row_offset + j + 1] = tmp1;
            dest[row_offset + j + 2] = tmp2;
            dest[row_offset + j + 3] = tmp3;
            dest[row_offset + j + 4] = tmp4;
            dest[row_offset + j + 5] = tmp5;

        }
    }
}

inline void dram_to_local_simple_7(
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
        for (int j = 0; j < dim_x; j+= 7){
            
            float tmp0 = src[(row_idx + i) * matrix_width + (col_idx + j)]; // j is always 0 here
            float tmp1 = src[(row_idx + i) * matrix_width + (col_idx + j + 1)];
            float tmp2 = src[(row_idx + i) * matrix_width + (col_idx + j + 2)];
            float tmp3 = src[(row_idx + i) * matrix_width + (col_idx + j + 3)];
            float tmp4 = src[(row_idx + i) * matrix_width + (col_idx + j + 4)];
            float tmp5 = src[(row_idx + i) * matrix_width + (col_idx + j + 5)];
            float tmp6 = src[(row_idx + i) * matrix_width + (col_idx + j + 6)];
        
            asm volatile("": : : "memory"); // need to figure out what is this
            
            dest[row_offset + j] = tmp0;
            dest[row_offset + j + 1] = tmp1;
            dest[row_offset + j + 2] = tmp2;
            dest[row_offset + j + 3] = tmp3;
            dest[row_offset + j + 4] = tmp4;
            dest[row_offset + j + 5] = tmp5;
            dest[row_offset + j + 6] = tmp6;

        }
    }
}

inline void dram_to_local_simple_8(
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
        for (int j = 0; j < dim_x; j+= 8){
            
            float tmp0 = src[(row_idx + i) * matrix_width + (col_idx + j)]; // j is always 0 here
            float tmp1 = src[(row_idx + i) * matrix_width + (col_idx + j + 1)];
            float tmp2 = src[(row_idx + i) * matrix_width + (col_idx + j + 2)];
            float tmp3 = src[(row_idx + i) * matrix_width + (col_idx + j + 3)];
            float tmp4 = src[(row_idx + i) * matrix_width + (col_idx + j + 4)];
            float tmp5 = src[(row_idx + i) * matrix_width + (col_idx + j + 5)];
            float tmp6 = src[(row_idx + i) * matrix_width + (col_idx + j + 6)];
            float tmp7 = src[(row_idx + i) * matrix_width + (col_idx + j + 7)];
        
            asm volatile("": : : "memory"); // need to figure out what is this
            
            dest[row_offset + j] = tmp0;
            dest[row_offset + j + 1] = tmp1;
            dest[row_offset + j + 2] = tmp2;
            dest[row_offset + j + 3] = tmp3;
            dest[row_offset + j + 4] = tmp4;
            dest[row_offset + j + 5] = tmp5;
            dest[row_offset + j + 6] = tmp6;
            dest[row_offset + j + 7] = tmp7;

        }
    }
}

inline void dram_to_local_simple_9(
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
        for (int j = 0; j < dim_x; j+= 9){
            
            float tmp0 = src[(row_idx + i) * matrix_width + (col_idx + j)]; // j is always 0 here
            float tmp1 = src[(row_idx + i) * matrix_width + (col_idx + j + 1)];
            float tmp2 = src[(row_idx + i) * matrix_width + (col_idx + j + 2)];
            float tmp3 = src[(row_idx + i) * matrix_width + (col_idx + j + 3)];
            float tmp4 = src[(row_idx + i) * matrix_width + (col_idx + j + 4)];
            float tmp5 = src[(row_idx + i) * matrix_width + (col_idx + j + 5)];
            float tmp6 = src[(row_idx + i) * matrix_width + (col_idx + j + 6)];
            float tmp7 = src[(row_idx + i) * matrix_width + (col_idx + j + 7)];
            float tmp8 = src[(row_idx + i) * matrix_width + (col_idx + j + 8)];
        
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

        }
    }
}

inline void dram_to_local_simple_10(
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
        for (int j = 0; j < dim_x; j+= 10){
            
            float tmp0 = src[(row_idx + i) * matrix_width + (col_idx + j)]; // j is always 0 here
            float tmp1 = src[(row_idx + i) * matrix_width + (col_idx + j + 1)];
            float tmp2 = src[(row_idx + i) * matrix_width + (col_idx + j + 2)];
            float tmp3 = src[(row_idx + i) * matrix_width + (col_idx + j + 3)];
            float tmp4 = src[(row_idx + i) * matrix_width + (col_idx + j + 4)];
            float tmp5 = src[(row_idx + i) * matrix_width + (col_idx + j + 5)];
            float tmp6 = src[(row_idx + i) * matrix_width + (col_idx + j + 6)];
            float tmp7 = src[(row_idx + i) * matrix_width + (col_idx + j + 7)];
            float tmp8 = src[(row_idx + i) * matrix_width + (col_idx + j + 8)];
            float tmp9 = src[(row_idx + i) * matrix_width + (col_idx + j + 9)];
        
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

        }
    }
}

inline void dram_to_local_simple_11(
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
        for (int j = 0; j < dim_x; j+= 11){
            
            float tmp0 = src[(row_idx + i) * matrix_width + (col_idx + j)]; // j is always 0 here
            float tmp1 = src[(row_idx + i) * matrix_width + (col_idx + j + 1)];
            float tmp2 = src[(row_idx + i) * matrix_width + (col_idx + j + 2)];
            float tmp3 = src[(row_idx + i) * matrix_width + (col_idx + j + 3)];
            float tmp4 = src[(row_idx + i) * matrix_width + (col_idx + j + 4)];
            float tmp5 = src[(row_idx + i) * matrix_width + (col_idx + j + 5)];
            float tmp6 = src[(row_idx + i) * matrix_width + (col_idx + j + 6)];
            float tmp7 = src[(row_idx + i) * matrix_width + (col_idx + j + 7)];
            float tmp8 = src[(row_idx + i) * matrix_width + (col_idx + j + 8)];
            float tmp9 = src[(row_idx + i) * matrix_width + (col_idx + j + 9)];
            float tmp10 = src[(row_idx + i) * matrix_width + (col_idx + j + 10)];
        
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

        }
    }
}

inline void dram_to_local_simple_12(
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
        for (int j = 0; j < dim_x; j+= 12){
            
            float tmp0 = src[(row_idx + i) * matrix_width + (col_idx + j)]; // j is always 0 here
            float tmp1 = src[(row_idx + i) * matrix_width + (col_idx + j + 1)];
            float tmp2 = src[(row_idx + i) * matrix_width + (col_idx + j + 2)];
            float tmp3 = src[(row_idx + i) * matrix_width + (col_idx + j + 3)];
            float tmp4 = src[(row_idx + i) * matrix_width + (col_idx + j + 4)];
            float tmp5 = src[(row_idx + i) * matrix_width + (col_idx + j + 5)];
            float tmp6 = src[(row_idx + i) * matrix_width + (col_idx + j + 6)];
            float tmp7 = src[(row_idx + i) * matrix_width + (col_idx + j + 7)];
            float tmp8 = src[(row_idx + i) * matrix_width + (col_idx + j + 8)];
            float tmp9 = src[(row_idx + i) * matrix_width + (col_idx + j + 9)];
            float tmp10 = src[(row_idx + i) * matrix_width + (col_idx + j + 10)];
            float tmp11 = src[(row_idx + i) * matrix_width + (col_idx + j + 11)];
        
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

        }
    }
}

inline void dram_to_local_simple_13(
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
        for (int j = 0; j < dim_x; j+= 13){
            
            float tmp0 = src[(row_idx + i) * matrix_width + (col_idx + j)]; // j is always 0 here
            float tmp1 = src[(row_idx + i) * matrix_width + (col_idx + j + 1)];
            float tmp2 = src[(row_idx + i) * matrix_width + (col_idx + j + 2)];
            float tmp3 = src[(row_idx + i) * matrix_width + (col_idx + j + 3)];
            float tmp4 = src[(row_idx + i) * matrix_width + (col_idx + j + 4)];
            float tmp5 = src[(row_idx + i) * matrix_width + (col_idx + j + 5)];
            float tmp6 = src[(row_idx + i) * matrix_width + (col_idx + j + 6)];
            float tmp7 = src[(row_idx + i) * matrix_width + (col_idx + j + 7)];
            float tmp8 = src[(row_idx + i) * matrix_width + (col_idx + j + 8)];
            float tmp9 = src[(row_idx + i) * matrix_width + (col_idx + j + 9)];
            float tmp10 = src[(row_idx + i) * matrix_width + (col_idx + j + 10)];
            float tmp11 = src[(row_idx + i) * matrix_width + (col_idx + j + 11)];
            float tmp12 = src[(row_idx + i) * matrix_width + (col_idx + j + 12)];
        
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

        }
    }
}

inline void dram_to_local_simple_14(
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
        for (int j = 0; j < dim_x; j+= 14){
            
            float tmp0 = src[(row_idx + i) * matrix_width + (col_idx + j)]; // j is always 0 here
            float tmp1 = src[(row_idx + i) * matrix_width + (col_idx + j + 1)];
            float tmp2 = src[(row_idx + i) * matrix_width + (col_idx + j + 2)];
            float tmp3 = src[(row_idx + i) * matrix_width + (col_idx + j + 3)];
            float tmp4 = src[(row_idx + i) * matrix_width + (col_idx + j + 4)];
            float tmp5 = src[(row_idx + i) * matrix_width + (col_idx + j + 5)];
            float tmp6 = src[(row_idx + i) * matrix_width + (col_idx + j + 6)];
            float tmp7 = src[(row_idx + i) * matrix_width + (col_idx + j + 7)];
            float tmp8 = src[(row_idx + i) * matrix_width + (col_idx + j + 8)];
            float tmp9 = src[(row_idx + i) * matrix_width + (col_idx + j + 9)];
            float tmp10 = src[(row_idx + i) * matrix_width + (col_idx + j + 10)];
            float tmp11 = src[(row_idx + i) * matrix_width + (col_idx + j + 11)];
            float tmp12 = src[(row_idx + i) * matrix_width + (col_idx + j + 12)];
            float tmp13 = src[(row_idx + i) * matrix_width + (col_idx + j + 13)];     

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
        }
    }
}

inline void dram_to_local_simple_15(
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
        for (int j = 0; j < dim_x; j+= 15){
            
            float tmp0 = src[(row_idx + i) * matrix_width + (col_idx + j)]; // j is always 0 here
            float tmp1 = src[(row_idx + i) * matrix_width + (col_idx + j + 1)];
            float tmp2 = src[(row_idx + i) * matrix_width + (col_idx + j + 2)];
            float tmp3 = src[(row_idx + i) * matrix_width + (col_idx + j + 3)];
            float tmp4 = src[(row_idx + i) * matrix_width + (col_idx + j + 4)];
            float tmp5 = src[(row_idx + i) * matrix_width + (col_idx + j + 5)];
            float tmp6 = src[(row_idx + i) * matrix_width + (col_idx + j + 6)];
            float tmp7 = src[(row_idx + i) * matrix_width + (col_idx + j + 7)];
            float tmp8 = src[(row_idx + i) * matrix_width + (col_idx + j + 8)];
            float tmp9 = src[(row_idx + i) * matrix_width + (col_idx + j + 9)];
            float tmp10 = src[(row_idx + i) * matrix_width + (col_idx + j + 10)];
            float tmp11 = src[(row_idx + i) * matrix_width + (col_idx + j + 11)];
            float tmp12 = src[(row_idx + i) * matrix_width + (col_idx + j + 12)];
            float tmp13 = src[(row_idx + i) * matrix_width + (col_idx + j + 13)];
            float tmp14 = src[(row_idx + i) * matrix_width + (col_idx + j + 14)];
        
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

        }
    }
}

inline void dram_to_local_simple_16(
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
        for (int j = 0; j < dim_x; j+= 16){
            
            float tmp0 = src[(row_idx + i) * matrix_width + (col_idx + j)]; // j is always 0 here
            float tmp1 = src[(row_idx + i) * matrix_width + (col_idx + j + 1)];
            float tmp2 = src[(row_idx + i) * matrix_width + (col_idx + j + 2)];
            float tmp3 = src[(row_idx + i) * matrix_width + (col_idx + j + 3)];
            float tmp4 = src[(row_idx + i) * matrix_width + (col_idx + j + 4)];
            float tmp5 = src[(row_idx + i) * matrix_width + (col_idx + j + 5)];
            float tmp6 = src[(row_idx + i) * matrix_width + (col_idx + j + 6)];
            float tmp7 = src[(row_idx + i) * matrix_width + (col_idx + j + 7)];
            float tmp8 = src[(row_idx + i) * matrix_width + (col_idx + j + 8)];
            float tmp9 = src[(row_idx + i) * matrix_width + (col_idx + j + 9)];
            float tmp10 = src[(row_idx + i) * matrix_width + (col_idx + j + 10)];
            float tmp11 = src[(row_idx + i) * matrix_width + (col_idx + j + 11)];
            float tmp12 = src[(row_idx + i) * matrix_width + (col_idx + j + 12)];
            float tmp13 = src[(row_idx + i) * matrix_width + (col_idx + j + 13)];
            float tmp14 = src[(row_idx + i) * matrix_width + (col_idx + j + 14)];
            float tmp15 = src[(row_idx + i) * matrix_width + (col_idx + j + 15)];
        
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

        }
    }
}

inline void dram_to_local_simple_17(
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
        for (int j = 0; j < dim_x; j+= 17){
            
            float tmp0 = src[(row_idx + i) * matrix_width + (col_idx + j)]; // j is always 0 here
            float tmp1 = src[(row_idx + i) * matrix_width + (col_idx + j + 1)];
            float tmp2 = src[(row_idx + i) * matrix_width + (col_idx + j + 2)];
            float tmp3 = src[(row_idx + i) * matrix_width + (col_idx + j + 3)];
            float tmp4 = src[(row_idx + i) * matrix_width + (col_idx + j + 4)];
            float tmp5 = src[(row_idx + i) * matrix_width + (col_idx + j + 5)];
            float tmp6 = src[(row_idx + i) * matrix_width + (col_idx + j + 6)];
            float tmp7 = src[(row_idx + i) * matrix_width + (col_idx + j + 7)];
            float tmp8 = src[(row_idx + i) * matrix_width + (col_idx + j + 8)];
            float tmp9 = src[(row_idx + i) * matrix_width + (col_idx + j + 9)];
            float tmp10 = src[(row_idx + i) * matrix_width + (col_idx + j + 10)];
            float tmp11 = src[(row_idx + i) * matrix_width + (col_idx + j + 11)];
            float tmp12 = src[(row_idx + i) * matrix_width + (col_idx + j + 12)];
            float tmp13 = src[(row_idx + i) * matrix_width + (col_idx + j + 13)];
            float tmp14 = src[(row_idx + i) * matrix_width + (col_idx + j + 14)];
            float tmp15 = src[(row_idx + i) * matrix_width + (col_idx + j + 15)];
            float tmp16 = src[(row_idx + i) * matrix_width + (col_idx + j + 16)];
        
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

        }
    }
}

inline void dram_to_local_simple_18(
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
        for (int j = 0; j < dim_x; j+= 18){
            
            float tmp0 = src[(row_idx + i) * matrix_width + (col_idx + j)]; // j is always 0 here
            float tmp1 = src[(row_idx + i) * matrix_width + (col_idx + j + 1)];
            float tmp2 = src[(row_idx + i) * matrix_width + (col_idx + j + 2)];
            float tmp3 = src[(row_idx + i) * matrix_width + (col_idx + j + 3)];
            float tmp4 = src[(row_idx + i) * matrix_width + (col_idx + j + 4)];
            float tmp5 = src[(row_idx + i) * matrix_width + (col_idx + j + 5)];
            float tmp6 = src[(row_idx + i) * matrix_width + (col_idx + j + 6)];
            float tmp7 = src[(row_idx + i) * matrix_width + (col_idx + j + 7)];
            float tmp8 = src[(row_idx + i) * matrix_width + (col_idx + j + 8)];
            float tmp9 = src[(row_idx + i) * matrix_width + (col_idx + j + 9)];
            float tmp10 = src[(row_idx + i) * matrix_width + (col_idx + j + 10)];
            float tmp11 = src[(row_idx + i) * matrix_width + (col_idx + j + 11)];
            float tmp12 = src[(row_idx + i) * matrix_width + (col_idx + j + 12)];
            float tmp13 = src[(row_idx + i) * matrix_width + (col_idx + j + 13)];
            float tmp14 = src[(row_idx + i) * matrix_width + (col_idx + j + 14)];
            float tmp15 = src[(row_idx + i) * matrix_width + (col_idx + j + 15)];
            float tmp16 = src[(row_idx + i) * matrix_width + (col_idx + j + 16)];
            float tmp17 = src[(row_idx + i) * matrix_width + (col_idx + j + 17)];
        
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

        }
    }
}

inline void dram_to_local_simple_19(
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
        for (int j = 0; j < dim_x; j+= 19){
            
            float tmp0 = src[(row_idx + i) * matrix_width + (col_idx + j)]; // j is always 0 here
            float tmp1 = src[(row_idx + i) * matrix_width + (col_idx + j + 1)];
            float tmp2 = src[(row_idx + i) * matrix_width + (col_idx + j + 2)];
            float tmp3 = src[(row_idx + i) * matrix_width + (col_idx + j + 3)];
            float tmp4 = src[(row_idx + i) * matrix_width + (col_idx + j + 4)];
            float tmp5 = src[(row_idx + i) * matrix_width + (col_idx + j + 5)];
            float tmp6 = src[(row_idx + i) * matrix_width + (col_idx + j + 6)];
            float tmp7 = src[(row_idx + i) * matrix_width + (col_idx + j + 7)];
            float tmp8 = src[(row_idx + i) * matrix_width + (col_idx + j + 8)];
            float tmp9 = src[(row_idx + i) * matrix_width + (col_idx + j + 9)];
            float tmp10 = src[(row_idx + i) * matrix_width + (col_idx + j + 10)];
            float tmp11 = src[(row_idx + i) * matrix_width + (col_idx + j + 11)];
            float tmp12 = src[(row_idx + i) * matrix_width + (col_idx + j + 12)];
            float tmp13 = src[(row_idx + i) * matrix_width + (col_idx + j + 13)];
            float tmp14 = src[(row_idx + i) * matrix_width + (col_idx + j + 14)];
            float tmp15 = src[(row_idx + i) * matrix_width + (col_idx + j + 15)];
            float tmp16 = src[(row_idx + i) * matrix_width + (col_idx + j + 16)];
            float tmp17 = src[(row_idx + i) * matrix_width + (col_idx + j + 17)];
            float tmp18 = src[(row_idx + i) * matrix_width + (col_idx + j + 18)];
        
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

        }
    }
}

inline void dram_to_local_simple_20(
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
            
            float tmp0 = src[(row_idx + i) * matrix_width + (col_idx + j)]; // j is always 0 here
            float tmp1 = src[(row_idx + i) * matrix_width + (col_idx + j + 1)];
            float tmp2 = src[(row_idx + i) * matrix_width + (col_idx + j + 2)];
            float tmp3 = src[(row_idx + i) * matrix_width + (col_idx + j + 3)];
            float tmp4 = src[(row_idx + i) * matrix_width + (col_idx + j + 4)];
            float tmp5 = src[(row_idx + i) * matrix_width + (col_idx + j + 5)];
            float tmp6 = src[(row_idx + i) * matrix_width + (col_idx + j + 6)];
            float tmp7 = src[(row_idx + i) * matrix_width + (col_idx + j + 7)];
            float tmp8 = src[(row_idx + i) * matrix_width + (col_idx + j + 8)];
            float tmp9 = src[(row_idx + i) * matrix_width + (col_idx + j + 9)];
            float tmp10 = src[(row_idx + i) * matrix_width + (col_idx + j + 10)];
            float tmp11 = src[(row_idx + i) * matrix_width + (col_idx + j + 11)];
            float tmp12 = src[(row_idx + i) * matrix_width + (col_idx + j + 12)];
            float tmp13 = src[(row_idx + i) * matrix_width + (col_idx + j + 13)];
            float tmp14 = src[(row_idx + i) * matrix_width + (col_idx + j + 14)];
            float tmp15 = src[(row_idx + i) * matrix_width + (col_idx + j + 15)];
            float tmp16 = src[(row_idx + i) * matrix_width + (col_idx + j + 16)];
            float tmp17 = src[(row_idx + i) * matrix_width + (col_idx + j + 17)];
            float tmp18 = src[(row_idx + i) * matrix_width + (col_idx + j + 18)];
            float tmp19 = src[(row_idx + i) * matrix_width + (col_idx + j + 19)];
        
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
        for (int j =0; j < B_local_row; j++){
            for (int k = 0; k < A_local_col; k+=1){
                int A_local_idx = A_row_offset;
                int B_local_idx = j * B_local_col;
                
                float tmp0 = A_local_mat[A_local_idx] * B_local_mat[B_local_idx];
                dest[dest_row_offset + j] += (tmp0);
                                                   
            }
        }
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
        for (int j =0; j < B_local_row; j++){
            for (int k = 0; k < A_local_col; k+=2){
                int A_local_idx = A_row_offset;
                int B_local_idx = j * B_local_col;
                
                float tmp0 = A_local_mat[A_local_idx] * B_local_mat[B_local_idx];
                float tmp1 = A_local_mat[A_local_idx + 1] * B_local_mat[B_local_idx + 1];
                dest[dest_row_offset + j] += (tmp0 + tmp1);
                                                   
            }
        }
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
        for (int j =0; j < B_local_row; j++){
            for (int k = 0; k < A_local_col; k+=3){
                int A_local_idx = A_row_offset;
                int B_local_idx = j * B_local_col;
                
                float tmp0 = A_local_mat[A_local_idx] * B_local_mat[B_local_idx];
                float tmp1 = A_local_mat[A_local_idx + 1] * B_local_mat[B_local_idx + 1];
                float tmp2 = A_local_mat[A_local_idx + 2] * B_local_mat[B_local_idx + 2];
                dest[dest_row_offset + j] += (tmp0 + tmp1 + tmp2);
                                                   
            }
        }
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
        for (int j =0; j < B_local_row; j++){
            for (int k = 0; k < A_local_col; k+=4){
                int A_local_idx = A_row_offset;
                int B_local_idx = j * B_local_col;
                
                float tmp0 = A_local_mat[A_local_idx] * B_local_mat[B_local_idx];
                float tmp1 = A_local_mat[A_local_idx + 1] * B_local_mat[B_local_idx + 1];
                float tmp2 = A_local_mat[A_local_idx + 2] * B_local_mat[B_local_idx + 2];
                float tmp3 = A_local_mat[A_local_idx + 3] * B_local_mat[B_local_idx + 3];
                dest[dest_row_offset + j] += (tmp0 + tmp1 + tmp2 + tmp3);
                                                   
            }
        }
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
        for (int j =0; j < B_local_row; j++){
            for (int k = 0; k < A_local_col; k+=5){
                int A_local_idx = A_row_offset;
                int B_local_idx = j * B_local_col;
                
                float tmp0 = A_local_mat[A_local_idx] * B_local_mat[B_local_idx];
                float tmp1 = A_local_mat[A_local_idx + 1] * B_local_mat[B_local_idx + 1];
                float tmp2 = A_local_mat[A_local_idx + 2] * B_local_mat[B_local_idx + 2];
                float tmp3 = A_local_mat[A_local_idx + 3] * B_local_mat[B_local_idx + 3];
                float tmp4 = A_local_mat[A_local_idx + 4] * B_local_mat[B_local_idx + 4];
                dest[dest_row_offset + j] += (tmp0 + tmp1 + tmp2 + tmp3 + tmp4);
                                                   
            }
        }
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
        for (int j =0; j < B_local_row; j++){
            for (int k = 0; k < A_local_col; k+=6){
                int A_local_idx = A_row_offset;
                int B_local_idx = j * B_local_col;
                
                float tmp0 = A_local_mat[A_local_idx] * B_local_mat[B_local_idx];
                float tmp1 = A_local_mat[A_local_idx + 1] * B_local_mat[B_local_idx + 1];
                float tmp2 = A_local_mat[A_local_idx + 2] * B_local_mat[B_local_idx + 2];
                float tmp3 = A_local_mat[A_local_idx + 3] * B_local_mat[B_local_idx + 3];
                float tmp4 = A_local_mat[A_local_idx + 4] * B_local_mat[B_local_idx + 4];
                float tmp5 = A_local_mat[A_local_idx + 5] * B_local_mat[B_local_idx + 5];
                dest[dest_row_offset + j] += (tmp0 + tmp1 + tmp2 + tmp3 + tmp4 + tmp5);
                                                   
            }
        }
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
        for (int j =0; j < B_local_row; j++){
            for (int k = 0; k < A_local_col; k+=7){
                int A_local_idx = A_row_offset;
                int B_local_idx = j * B_local_col;
                
                float tmp0 = A_local_mat[A_local_idx] * B_local_mat[B_local_idx];
                float tmp1 = A_local_mat[A_local_idx + 1] * B_local_mat[B_local_idx + 1];
                float tmp2 = A_local_mat[A_local_idx + 2] * B_local_mat[B_local_idx + 2];
                float tmp3 = A_local_mat[A_local_idx + 3] * B_local_mat[B_local_idx + 3];
                float tmp4 = A_local_mat[A_local_idx + 4] * B_local_mat[B_local_idx + 4];
                float tmp5 = A_local_mat[A_local_idx + 5] * B_local_mat[B_local_idx + 5];
                float tmp6 = A_local_mat[A_local_idx + 6] * B_local_mat[B_local_idx + 6];
                dest[dest_row_offset + j] += (tmp0 + tmp1 + tmp2 + tmp3 + tmp4 + tmp5 + tmp6);
                                                   
            }
        }
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
        for (int j =0; j < B_local_row; j++){
            for (int k = 0; k < A_local_col; k+=8){
                int A_local_idx = A_row_offset;
                int B_local_idx = j * B_local_col;
                
                float tmp0 = A_local_mat[A_local_idx] * B_local_mat[B_local_idx];
                float tmp1 = A_local_mat[A_local_idx + 1] * B_local_mat[B_local_idx + 1];
                float tmp2 = A_local_mat[A_local_idx + 2] * B_local_mat[B_local_idx + 2];
                float tmp3 = A_local_mat[A_local_idx + 3] * B_local_mat[B_local_idx + 3];
                float tmp4 = A_local_mat[A_local_idx + 4] * B_local_mat[B_local_idx + 4];
                float tmp5 = A_local_mat[A_local_idx + 5] * B_local_mat[B_local_idx + 5];
                float tmp6 = A_local_mat[A_local_idx + 6] * B_local_mat[B_local_idx + 6];
                float tmp7 = A_local_mat[A_local_idx + 7] * B_local_mat[B_local_idx + 7];
                dest[dest_row_offset + j] += (tmp0 + tmp1 + tmp2 + tmp3 + tmp4 + tmp5 + tmp6 + tmp7);
                                                   
            }
        }
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
        for (int j =0; j < B_local_row; j++){
            for (int k = 0; k < A_local_col; k+=9){
                int A_local_idx = A_row_offset;
                int B_local_idx = j * B_local_col;
                
                float tmp0 = A_local_mat[A_local_idx] * B_local_mat[B_local_idx];
                float tmp1 = A_local_mat[A_local_idx + 1] * B_local_mat[B_local_idx + 1];
                float tmp2 = A_local_mat[A_local_idx + 2] * B_local_mat[B_local_idx + 2];
                float tmp3 = A_local_mat[A_local_idx + 3] * B_local_mat[B_local_idx + 3];
                float tmp4 = A_local_mat[A_local_idx + 4] * B_local_mat[B_local_idx + 4];
                float tmp5 = A_local_mat[A_local_idx + 5] * B_local_mat[B_local_idx + 5];
                float tmp6 = A_local_mat[A_local_idx + 6] * B_local_mat[B_local_idx + 6];
                float tmp7 = A_local_mat[A_local_idx + 7] * B_local_mat[B_local_idx + 7];
                float tmp8 = A_local_mat[A_local_idx + 8] * B_local_mat[B_local_idx + 8];
                dest[dest_row_offset + j] += (tmp0 + tmp1 + tmp2 + tmp3 + tmp4 + tmp5 + tmp6 + tmp7 + tmp8 );
                                                   
            }
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
        for (int j =0; j < B_local_row; j++){
            for (int k = 0; k < A_local_col; k+=10){
                int A_local_idx = A_row_offset;
                int B_local_idx = j * B_local_col;
                
                float tmp0 = A_local_mat[A_local_idx] * B_local_mat[B_local_idx];
                float tmp1 = A_local_mat[A_local_idx + 1] * B_local_mat[B_local_idx + 1];
                float tmp2 = A_local_mat[A_local_idx + 2] * B_local_mat[B_local_idx + 2];
                float tmp3 = A_local_mat[A_local_idx + 3] * B_local_mat[B_local_idx + 3];
                float tmp4 = A_local_mat[A_local_idx + 4] * B_local_mat[B_local_idx + 4];
                float tmp5 = A_local_mat[A_local_idx + 5] * B_local_mat[B_local_idx + 5];
                float tmp6 = A_local_mat[A_local_idx + 6] * B_local_mat[B_local_idx + 6];
                float tmp7 = A_local_mat[A_local_idx + 7] * B_local_mat[B_local_idx + 7];
                float tmp8 = A_local_mat[A_local_idx + 8] * B_local_mat[B_local_idx + 8];
                float tmp9 = A_local_mat[A_local_idx + 9] * B_local_mat[B_local_idx + 9];
                dest[dest_row_offset + j] += (tmp0 + tmp1 + tmp2 + tmp3 + tmp4 + tmp5 + tmp6 + tmp7 + tmp8 + tmp9 );
                                                   
            }
        }
    }

}

inline void compute_11(
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
            for (int k = 0; k < A_local_col; k+=11){
                int A_local_idx = A_row_offset;
                int B_local_idx = j * B_local_col;
                
                float tmp0 = A_local_mat[A_local_idx] * B_local_mat[B_local_idx];
                float tmp1 = A_local_mat[A_local_idx + 1] * B_local_mat[B_local_idx + 1];
                float tmp2 = A_local_mat[A_local_idx + 2] * B_local_mat[B_local_idx + 2];
                float tmp3 = A_local_mat[A_local_idx + 3] * B_local_mat[B_local_idx + 3];
                float tmp4 = A_local_mat[A_local_idx + 4] * B_local_mat[B_local_idx + 4];
                float tmp5 = A_local_mat[A_local_idx + 5] * B_local_mat[B_local_idx + 5];
                float tmp6 = A_local_mat[A_local_idx + 6] * B_local_mat[B_local_idx + 6];
                float tmp7 = A_local_mat[A_local_idx + 7] * B_local_mat[B_local_idx + 7];
                float tmp8 = A_local_mat[A_local_idx + 8] * B_local_mat[B_local_idx + 8];
                float tmp9 = A_local_mat[A_local_idx + 9] * B_local_mat[B_local_idx + 9];
                float tmp10 = A_local_mat[A_local_idx + 10] * B_local_mat[B_local_idx + 10];
                dest[dest_row_offset + j] += (tmp0 + tmp1 + tmp2 + tmp3 + tmp4 + tmp5 + tmp6 + tmp7 + tmp8 + tmp9 + tmp10
                                                   );
            }
        }
    }

}

inline void compute_12(
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
            for (int k = 0; k < A_local_col; k+=12){
                int A_local_idx = A_row_offset;
                int B_local_idx = j * B_local_col;
                
                float tmp0 = A_local_mat[A_local_idx] * B_local_mat[B_local_idx];
                float tmp1 = A_local_mat[A_local_idx + 1] * B_local_mat[B_local_idx + 1];
                float tmp2 = A_local_mat[A_local_idx + 2] * B_local_mat[B_local_idx + 2];
                float tmp3 = A_local_mat[A_local_idx + 3] * B_local_mat[B_local_idx + 3];
                float tmp4 = A_local_mat[A_local_idx + 4] * B_local_mat[B_local_idx + 4];
                float tmp5 = A_local_mat[A_local_idx + 5] * B_local_mat[B_local_idx + 5];
                float tmp6 = A_local_mat[A_local_idx + 6] * B_local_mat[B_local_idx + 6];
                float tmp7 = A_local_mat[A_local_idx + 7] * B_local_mat[B_local_idx + 7];
                float tmp8 = A_local_mat[A_local_idx + 8] * B_local_mat[B_local_idx + 8];
                float tmp9 = A_local_mat[A_local_idx + 9] * B_local_mat[B_local_idx + 9];
                float tmp10 = A_local_mat[A_local_idx + 10] * B_local_mat[B_local_idx + 10];
                float tmp11 = A_local_mat[A_local_idx + 11] * B_local_mat[B_local_idx + 11];
                dest[dest_row_offset + j] += (tmp0 + tmp1 + tmp2 + tmp3 + tmp4 + tmp5 + tmp6 + tmp7 + tmp8 + tmp9 + tmp10
                                                   + tmp11);
            }
        }
    }

}

inline void compute_13(
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
            for (int k = 0; k < A_local_col; k+=13){
                int A_local_idx = A_row_offset;
                int B_local_idx = j * B_local_col;
                
                float tmp0 = A_local_mat[A_local_idx] * B_local_mat[B_local_idx];
                float tmp1 = A_local_mat[A_local_idx + 1] * B_local_mat[B_local_idx + 1];
                float tmp2 = A_local_mat[A_local_idx + 2] * B_local_mat[B_local_idx + 2];
                float tmp3 = A_local_mat[A_local_idx + 3] * B_local_mat[B_local_idx + 3];
                float tmp4 = A_local_mat[A_local_idx + 4] * B_local_mat[B_local_idx + 4];
                float tmp5 = A_local_mat[A_local_idx + 5] * B_local_mat[B_local_idx + 5];
                float tmp6 = A_local_mat[A_local_idx + 6] * B_local_mat[B_local_idx + 6];
                float tmp7 = A_local_mat[A_local_idx + 7] * B_local_mat[B_local_idx + 7];
                float tmp8 = A_local_mat[A_local_idx + 8] * B_local_mat[B_local_idx + 8];
                float tmp9 = A_local_mat[A_local_idx + 9] * B_local_mat[B_local_idx + 9];
                float tmp10 = A_local_mat[A_local_idx + 10] * B_local_mat[B_local_idx + 10];
                float tmp11 = A_local_mat[A_local_idx + 11] * B_local_mat[B_local_idx + 11];
                float tmp12 = A_local_mat[A_local_idx + 12] * B_local_mat[B_local_idx + 12];
                dest[dest_row_offset + j] += (tmp0 + tmp1 + tmp2 + tmp3 + tmp4 + tmp5 + tmp6 + tmp7 + tmp8 + tmp9 + tmp10
                                                   + tmp11 + tmp12);
            }
        }
    }

}

inline void compute_14(
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
            for (int k = 0; k < A_local_col; k+=14){
                int A_local_idx = A_row_offset;
                int B_local_idx = j * B_local_col;
                
                float tmp0 = A_local_mat[A_local_idx] * B_local_mat[B_local_idx];
                float tmp1 = A_local_mat[A_local_idx + 1] * B_local_mat[B_local_idx + 1];
                float tmp2 = A_local_mat[A_local_idx + 2] * B_local_mat[B_local_idx + 2];
                float tmp3 = A_local_mat[A_local_idx + 3] * B_local_mat[B_local_idx + 3];
                float tmp4 = A_local_mat[A_local_idx + 4] * B_local_mat[B_local_idx + 4];
                float tmp5 = A_local_mat[A_local_idx + 5] * B_local_mat[B_local_idx + 5];
                float tmp6 = A_local_mat[A_local_idx + 6] * B_local_mat[B_local_idx + 6];
                float tmp7 = A_local_mat[A_local_idx + 7] * B_local_mat[B_local_idx + 7];
                float tmp8 = A_local_mat[A_local_idx + 8] * B_local_mat[B_local_idx + 8];
                float tmp9 = A_local_mat[A_local_idx + 9] * B_local_mat[B_local_idx + 9];
                float tmp10 = A_local_mat[A_local_idx + 10] * B_local_mat[B_local_idx + 10];
                float tmp11 = A_local_mat[A_local_idx + 11] * B_local_mat[B_local_idx + 11];
                float tmp12 = A_local_mat[A_local_idx + 12] * B_local_mat[B_local_idx + 12];
                float tmp13 = A_local_mat[A_local_idx + 13] * B_local_mat[B_local_idx + 13];
                dest[dest_row_offset + j] += (tmp0 + tmp1 + tmp2 + tmp3 + tmp4 + tmp5 + tmp6 + tmp7 + tmp8 + tmp9 + tmp10
                                                   + tmp11 + tmp12 + tmp13);
            }
        }
    }

}

inline void compute_15(
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
            for (int k = 0; k < A_local_col; k+=15){
                int A_local_idx = A_row_offset;
                int B_local_idx = j * B_local_col;
                
                float tmp0 = A_local_mat[A_local_idx] * B_local_mat[B_local_idx];
                float tmp1 = A_local_mat[A_local_idx + 1] * B_local_mat[B_local_idx + 1];
                float tmp2 = A_local_mat[A_local_idx + 2] * B_local_mat[B_local_idx + 2];
                float tmp3 = A_local_mat[A_local_idx + 3] * B_local_mat[B_local_idx + 3];
                float tmp4 = A_local_mat[A_local_idx + 4] * B_local_mat[B_local_idx + 4];
                float tmp5 = A_local_mat[A_local_idx + 5] * B_local_mat[B_local_idx + 5];
                float tmp6 = A_local_mat[A_local_idx + 6] * B_local_mat[B_local_idx + 6];
                float tmp7 = A_local_mat[A_local_idx + 7] * B_local_mat[B_local_idx + 7];
                float tmp8 = A_local_mat[A_local_idx + 8] * B_local_mat[B_local_idx + 8];
                float tmp9 = A_local_mat[A_local_idx + 9] * B_local_mat[B_local_idx + 9];
                float tmp10 = A_local_mat[A_local_idx + 10] * B_local_mat[B_local_idx + 10];
                float tmp11 = A_local_mat[A_local_idx + 11] * B_local_mat[B_local_idx + 11];
                float tmp12 = A_local_mat[A_local_idx + 12] * B_local_mat[B_local_idx + 12];
                float tmp13 = A_local_mat[A_local_idx + 13] * B_local_mat[B_local_idx + 13];
                float tmp14 = A_local_mat[A_local_idx + 14] * B_local_mat[B_local_idx + 14];
                dest[dest_row_offset + j] += (tmp0 + tmp1 + tmp2 + tmp3 + tmp4 + tmp5 + tmp6 + tmp7 + tmp8 + tmp9 + tmp10
                                                   + tmp11 + tmp12 + tmp13 + tmp14);
            }
        }
    }

}

inline void compute_16(
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
            for (int k = 0; k < A_local_col; k+=16){
                int A_local_idx = A_row_offset;
                int B_local_idx = j * B_local_col;
                
                float tmp0 = A_local_mat[A_local_idx] * B_local_mat[B_local_idx];
                float tmp1 = A_local_mat[A_local_idx + 1] * B_local_mat[B_local_idx + 1];
                float tmp2 = A_local_mat[A_local_idx + 2] * B_local_mat[B_local_idx + 2];
                float tmp3 = A_local_mat[A_local_idx + 3] * B_local_mat[B_local_idx + 3];
                float tmp4 = A_local_mat[A_local_idx + 4] * B_local_mat[B_local_idx + 4];
                float tmp5 = A_local_mat[A_local_idx + 5] * B_local_mat[B_local_idx + 5];
                float tmp6 = A_local_mat[A_local_idx + 6] * B_local_mat[B_local_idx + 6];
                float tmp7 = A_local_mat[A_local_idx + 7] * B_local_mat[B_local_idx + 7];
                float tmp8 = A_local_mat[A_local_idx + 8] * B_local_mat[B_local_idx + 8];
                float tmp9 = A_local_mat[A_local_idx + 9] * B_local_mat[B_local_idx + 9];
                float tmp10 = A_local_mat[A_local_idx + 10] * B_local_mat[B_local_idx + 10];
                float tmp11 = A_local_mat[A_local_idx + 11] * B_local_mat[B_local_idx + 11];
                float tmp12 = A_local_mat[A_local_idx + 12] * B_local_mat[B_local_idx + 12];
                float tmp13 = A_local_mat[A_local_idx + 13] * B_local_mat[B_local_idx + 13];
                float tmp14 = A_local_mat[A_local_idx + 14] * B_local_mat[B_local_idx + 14];
                float tmp15 = A_local_mat[A_local_idx + 15] * B_local_mat[B_local_idx + 15];
                dest[dest_row_offset + j] += (tmp0 + tmp1 + tmp2 + tmp3 + tmp4 + tmp5 + tmp6 + tmp7 + tmp8 + tmp9 + tmp10
                                                   + tmp11 + tmp12 + tmp13 + tmp14 + tmp15);
            }
        }
    }

}

inline void compute_17(
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
            for (int k = 0; k < A_local_col; k+=17){
                int A_local_idx = A_row_offset;
                int B_local_idx = j * B_local_col;
                
                float tmp0 = A_local_mat[A_local_idx] * B_local_mat[B_local_idx];
                float tmp1 = A_local_mat[A_local_idx + 1] * B_local_mat[B_local_idx + 1];
                float tmp2 = A_local_mat[A_local_idx + 2] * B_local_mat[B_local_idx + 2];
                float tmp3 = A_local_mat[A_local_idx + 3] * B_local_mat[B_local_idx + 3];
                float tmp4 = A_local_mat[A_local_idx + 4] * B_local_mat[B_local_idx + 4];
                float tmp5 = A_local_mat[A_local_idx + 5] * B_local_mat[B_local_idx + 5];
                float tmp6 = A_local_mat[A_local_idx + 6] * B_local_mat[B_local_idx + 6];
                float tmp7 = A_local_mat[A_local_idx + 7] * B_local_mat[B_local_idx + 7];
                float tmp8 = A_local_mat[A_local_idx + 8] * B_local_mat[B_local_idx + 8];
                float tmp9 = A_local_mat[A_local_idx + 9] * B_local_mat[B_local_idx + 9];
                float tmp10 = A_local_mat[A_local_idx + 10] * B_local_mat[B_local_idx + 10];
                float tmp11 = A_local_mat[A_local_idx + 11] * B_local_mat[B_local_idx + 11];
                float tmp12 = A_local_mat[A_local_idx + 12] * B_local_mat[B_local_idx + 12];
                float tmp13 = A_local_mat[A_local_idx + 13] * B_local_mat[B_local_idx + 13];
                float tmp14 = A_local_mat[A_local_idx + 14] * B_local_mat[B_local_idx + 14];
                float tmp15 = A_local_mat[A_local_idx + 15] * B_local_mat[B_local_idx + 15];
                float tmp16 = A_local_mat[A_local_idx + 16] * B_local_mat[B_local_idx + 16];
                dest[dest_row_offset + j] += (tmp0 + tmp1 + tmp2 + tmp3 + tmp4 + tmp5 + tmp6 + tmp7 + tmp8 + tmp9 + tmp10
                                                   + tmp11 + tmp12 + tmp13 + tmp14 + tmp15 + tmp16);
            }
        }
    }

}

inline void compute_18(
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
            for (int k = 0; k < A_local_col; k+=18){
                int A_local_idx = A_row_offset;
                int B_local_idx = j * B_local_col;
                
                float tmp0 = A_local_mat[A_local_idx] * B_local_mat[B_local_idx];
                float tmp1 = A_local_mat[A_local_idx + 1] * B_local_mat[B_local_idx + 1];
                float tmp2 = A_local_mat[A_local_idx + 2] * B_local_mat[B_local_idx + 2];
                float tmp3 = A_local_mat[A_local_idx + 3] * B_local_mat[B_local_idx + 3];
                float tmp4 = A_local_mat[A_local_idx + 4] * B_local_mat[B_local_idx + 4];
                float tmp5 = A_local_mat[A_local_idx + 5] * B_local_mat[B_local_idx + 5];
                float tmp6 = A_local_mat[A_local_idx + 6] * B_local_mat[B_local_idx + 6];
                float tmp7 = A_local_mat[A_local_idx + 7] * B_local_mat[B_local_idx + 7];
                float tmp8 = A_local_mat[A_local_idx + 8] * B_local_mat[B_local_idx + 8];
                float tmp9 = A_local_mat[A_local_idx + 9] * B_local_mat[B_local_idx + 9];
                float tmp10 = A_local_mat[A_local_idx + 10] * B_local_mat[B_local_idx + 10];
                float tmp11 = A_local_mat[A_local_idx + 11] * B_local_mat[B_local_idx + 11];
                float tmp12 = A_local_mat[A_local_idx + 12] * B_local_mat[B_local_idx + 12];
                float tmp13 = A_local_mat[A_local_idx + 13] * B_local_mat[B_local_idx + 13];
                float tmp14 = A_local_mat[A_local_idx + 14] * B_local_mat[B_local_idx + 14];
                float tmp15 = A_local_mat[A_local_idx + 15] * B_local_mat[B_local_idx + 15];
                float tmp16 = A_local_mat[A_local_idx + 16] * B_local_mat[B_local_idx + 16];
                float tmp17 = A_local_mat[A_local_idx + 17] * B_local_mat[B_local_idx + 17];
                dest[dest_row_offset + j] += (tmp0 + tmp1 + tmp2 + tmp3 + tmp4 + tmp5 + tmp6 + tmp7 + tmp8 + tmp9 + tmp10
                                                   + tmp11 + tmp12 + tmp13 + tmp14 + tmp15 + tmp16 + tmp17);
            }
        }
    }

}

inline void compute_19(
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
            for (int k = 0; k < A_local_col; k+=19){
                int A_local_idx = A_row_offset;
                int B_local_idx = j * B_local_col;
                
                float tmp0 = A_local_mat[A_local_idx] * B_local_mat[B_local_idx];
                float tmp1 = A_local_mat[A_local_idx + 1] * B_local_mat[B_local_idx + 1];
                float tmp2 = A_local_mat[A_local_idx + 2] * B_local_mat[B_local_idx + 2];
                float tmp3 = A_local_mat[A_local_idx + 3] * B_local_mat[B_local_idx + 3];
                float tmp4 = A_local_mat[A_local_idx + 4] * B_local_mat[B_local_idx + 4];
                float tmp5 = A_local_mat[A_local_idx + 5] * B_local_mat[B_local_idx + 5];
                float tmp6 = A_local_mat[A_local_idx + 6] * B_local_mat[B_local_idx + 6];
                float tmp7 = A_local_mat[A_local_idx + 7] * B_local_mat[B_local_idx + 7];
                float tmp8 = A_local_mat[A_local_idx + 8] * B_local_mat[B_local_idx + 8];
                float tmp9 = A_local_mat[A_local_idx + 9] * B_local_mat[B_local_idx + 9];
                float tmp10 = A_local_mat[A_local_idx + 10] * B_local_mat[B_local_idx + 10];
                float tmp11 = A_local_mat[A_local_idx + 11] * B_local_mat[B_local_idx + 11];
                float tmp12 = A_local_mat[A_local_idx + 12] * B_local_mat[B_local_idx + 12];
                float tmp13 = A_local_mat[A_local_idx + 13] * B_local_mat[B_local_idx + 13];
                float tmp14 = A_local_mat[A_local_idx + 14] * B_local_mat[B_local_idx + 14];
                float tmp15 = A_local_mat[A_local_idx + 15] * B_local_mat[B_local_idx + 15];
                float tmp16 = A_local_mat[A_local_idx + 16] * B_local_mat[B_local_idx + 16];
                float tmp17 = A_local_mat[A_local_idx + 17] * B_local_mat[B_local_idx + 17];
                float tmp18 = A_local_mat[A_local_idx + 18] * B_local_mat[B_local_idx + 18];
                dest[dest_row_offset + j] += (tmp0 + tmp1 + tmp2 + tmp3 + tmp4 + tmp5 + tmp6 + tmp7 + tmp8 + tmp9 + tmp10
                                                   + tmp11 + tmp12 + tmp13 + tmp14 + tmp15 + tmp16 + tmp17 + tmp18);
            }
        }
    }

}

inline void compute_20(
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
                
                float tmp0 = A_local_mat[A_local_idx] * B_local_mat[B_local_idx];
                float tmp1 = A_local_mat[A_local_idx + 1] * B_local_mat[B_local_idx + 1];
                float tmp2 = A_local_mat[A_local_idx + 2] * B_local_mat[B_local_idx + 2];
                float tmp3 = A_local_mat[A_local_idx + 3] * B_local_mat[B_local_idx + 3];
                float tmp4 = A_local_mat[A_local_idx + 4] * B_local_mat[B_local_idx + 4];
                float tmp5 = A_local_mat[A_local_idx + 5] * B_local_mat[B_local_idx + 5];
                float tmp6 = A_local_mat[A_local_idx + 6] * B_local_mat[B_local_idx + 6];
                float tmp7 = A_local_mat[A_local_idx + 7] * B_local_mat[B_local_idx + 7];
                float tmp8 = A_local_mat[A_local_idx + 8] * B_local_mat[B_local_idx + 8];
                float tmp9 = A_local_mat[A_local_idx + 9] * B_local_mat[B_local_idx + 9];
                float tmp10 = A_local_mat[A_local_idx + 10] * B_local_mat[B_local_idx + 10];
                float tmp11 = A_local_mat[A_local_idx + 11] * B_local_mat[B_local_idx + 11];
                float tmp12 = A_local_mat[A_local_idx + 12] * B_local_mat[B_local_idx + 12];
                float tmp13 = A_local_mat[A_local_idx + 13] * B_local_mat[B_local_idx + 13];
                float tmp14 = A_local_mat[A_local_idx + 14] * B_local_mat[B_local_idx + 14];
                float tmp15 = A_local_mat[A_local_idx + 15] * B_local_mat[B_local_idx + 15];
                float tmp16 = A_local_mat[A_local_idx + 16] * B_local_mat[B_local_idx + 16];
                float tmp17 = A_local_mat[A_local_idx + 17] * B_local_mat[B_local_idx + 17];
                float tmp18 = A_local_mat[A_local_idx + 18] * B_local_mat[B_local_idx + 18];
                float tmp19 = A_local_mat[A_local_idx + 19] * B_local_mat[B_local_idx + 19];
                dest[dest_row_offset + j] += (tmp0 + tmp1 + tmp2 + tmp3 + tmp4 + tmp5 + tmp6 + tmp7 + tmp8 + tmp9 + tmp10
                                                   + tmp11 + tmp12 + tmp13 + tmp14 + tmp15 + tmp16 + tmp17 + tmp18 + tmp19);
            }
        }
    }

}

#endif 
