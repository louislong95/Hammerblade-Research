#ifndef __V19_FUNCTION_HPP
#define __V19_FUNCTION_HPP
#include <cstdint>
#include <cstring>
#include <bsg_manycore.h>
#include <bsg_tile_group_barrier.h>

template <typename TA, typename TB, typename TC>
int  __attribute__ ((noinline)) matrix_multiply_group_v19(TA *A, TB *B, TC *C, // matrix multiplication for v3, v3 implement the data distribution along column side, without scratchpad
                                uint32_t A_HEIGHT, uint32_t A_WIDTH, uint32_t B_HEIGHT, uint32_t B_WIDTH)
{

  // A_HEIGHT = 16, A_WIDTH = 400, B_HEIGHT = 400, B_WIDTH = 768 (before transposed)
  // get the absolute tid (which group then which tid within that group)
  int tid_x = __bsg_tile_group_id_x * BSG_TILE_GROUP_X_DIM + __bsg_x;
  int tid_y = __bsg_tile_group_id_y * BSG_TILE_GROUP_Y_DIM + __bsg_y;
  int num_threads_x = BSG_TILE_GROUP_X_DIM * __bsg_grid_dim_x;
  int num_threads_y = BSG_TILE_GROUP_Y_DIM * __bsg_grid_dim_y;
        
  int num_threads = num_threads_x * num_threads_y;

  int num_col_B_per_thread = B_WIDTH / num_threads;
  int tid = tid_y * num_threads_x + tid_x;
  TC sum;

  int block_size_row = 4;
  int block_size_col = 8;

  // calculate the most suitable block size for the given matrix and tile group dimension

  
  //define the local scratchpad
  float A_local[block_size_row * block_size_col];
  float B_local[block_size_row * block_size_col];
  float C_local[block_size_row * block_size_col];

  //calculate number of row and col blocks in each matrix
  int A_num_blk_per_row = (A_WIDTH) / block_size_col;
  int A_num_blk_per_col = (A_HEIGHT) / block_size_row;
  int B_num_blk_per_row = (B_HEIGHT) / block_size_col;
  int B_num_blk_per_col = (B_WIDTH) / block_size_row;
  
  int local_A_col_start = 0;
  int local_B_col_start = 0;

  int local_cpy_A_idx = 0;
  int local_cpy_B_idx = 0;

  //iterate over blocks
  for (int A_blk_row_id = 0; A_blk_row_id < A_num_blk_per_col; A_blk_row_id ++){
        for (int B_blk_col_id = tid; B_blk_col_id < B_num_blk_per_col; B_blk_col_id += num_threads){
              
              //intitial the local_C to all 0
              for (int local_C_row = 0; local_C_row < block_size_row; local_C_row ++){
                    for (int local_C_col = 0; local_C_col < block_size_col; local_C_col ++){
                          C_local[local_C_row * block_size_col + local_C_col] = 0;
                    }
              }
              
              local_cpy_A_idx = 0;
              local_cpy_B_idx = 0;

              //iterate inside kernels
              for (int inside_blk_id = 0; inside_blk_id < A_num_blk_per_row; inside_blk_id ++){

                    local_cpy_A_idx = 0;
                    local_cpy_B_idx = 0;

                    //copy matrix A block into the A_local scratchpad
                    local_A_col_start = inside_blk_id * block_size_col;
                    for (int local_A_row = A_blk_row_id * block_size_row; local_A_row < (A_blk_row_id + 1) * block_size_row; local_A_row ++){
                          memcpy(&A_local[local_cpy_A_idx * block_size_col], &A[local_A_row * A_WIDTH + local_A_col_start], sizeof(A[0]) * block_size_col);
                          local_cpy_A_idx ++;
                    }

                    //copy matrix B block into the B_local scratchpad
                    local_B_col_start = inside_blk_id * block_size_col;
                    for (int local_B_row = B_blk_col_id * block_size_row; local_B_row < (B_blk_col_id + 1) * block_size_row; local_B_row ++){
                          memcpy(&B_local[local_cpy_B_idx * block_size_col], &B[local_B_row * B_HEIGHT + local_B_col_start], sizeof(B[0]) * block_size_col);
                          local_cpy_B_idx ++;
                    }


                    for (int A_local_row_id = 0; A_local_row_id < block_size_row; A_local_row_id ++) {
                         for (uint32_t B_local_row_id = 0; B_local_row_id < block_size_row; B_local_row_id ++){
                               sum = C_local[A_local_row_id * block_size_col + B_local_row_id];
                               for (uint32_t k = 0; k < block_size_col; k ++) {
                                     sum += A_local[A_local_row_id * block_size_col + k] * B_local[B_local_row_id * block_size_col + k];
                               }
                               C_local[A_local_row_id * block_size_col + B_local_row_id] = sum;
                         }
                    }

              }
              
              int memback_row_idx = 0;
              int memback_col_idx = 0;
              //write back the C_local to memory
              for (int memory_row_id = A_blk_row_id * block_size_row; memory_row_id < (A_blk_row_id + 1) * block_size_row; memory_row_id ++){
                  memback_col_idx = 0;
                  for (int memory_col_id = B_blk_col_id * block_size_row; memory_col_id < (B_blk_col_id + 1) * block_size_row; memory_col_id ++){
                        C[memory_row_id * B_WIDTH + memory_col_id] = C_local[memback_row_idx * block_size_col + memback_col_idx];
                        memback_col_idx ++;
                  }
                  memback_row_idx ++;
              }




        }
  }

  return 0;
  
}
#endif //__MATRIX_MULTIPLY_HPP
