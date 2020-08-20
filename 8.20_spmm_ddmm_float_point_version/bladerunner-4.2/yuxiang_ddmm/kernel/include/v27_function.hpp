#ifndef __V24_FUNCTION_HPP
#define __V24_FUNCTION_HPP
#include <cstdint>
#include <cstring>
#include <bsg_manycore.h>
#include <bsg_tile_group_barrier.h>

#include <unrolling_function_outer_product_v27.hpp>

int __attribute__ ((noinline)) gcd (uint32_t a, uint32_t b){ // calculate the greatest common divisor
       int temp = (a < b) ? a : b;
       a = b;
       b = temp;

       int gcd = 0;
       int t = 0;
       while (b != 0){
             t=a%b;
	       a=b;
	       b=t;
       }
       gcd = a;
       return gcd;

}


template <typename TA, typename TB, typename TC>
int  __attribute__ ((noinline)) matrix_multiply_group_row_mul(TA *A, TB *B, TC *C, 
                                uint32_t A_HEIGHT, uint32_t A_WIDTH, uint32_t B_WIDTH)
{

  int B_HEIGHT = A_WIDTH;
  // get the absolute tid (which group then which tid within that group)
  int tid_x = __bsg_tile_group_id_x * BSG_TILE_GROUP_X_DIM + __bsg_x;
  int tid_y = __bsg_tile_group_id_y * BSG_TILE_GROUP_Y_DIM + __bsg_y;
  int num_threads_x = BSG_TILE_GROUP_X_DIM * __bsg_grid_dim_x;
  int num_threads_y = BSG_TILE_GROUP_Y_DIM * __bsg_grid_dim_y;
        
  int num_threads = num_threads_x * num_threads_y;

  int num_col_B_per_thread = B_WIDTH / num_threads;
  int tid = tid_y * num_threads_x + tid_x;
  TC sum;
  
  //int local_idx_cpy = 0;
  //int local_idx_cal = 0;

  //----------------------------------------original function -----------------------------//
  
  float A_local[A_WIDTH];
  float B_local[B_HEIGHT];
  // copy data
  //memcpy (A_local, A, sizeof(A[0]) * A_WIDTH);
  
  //for ( int row_B_local_id = tid; row_B_local_id < B_WIDTH; row_B_local_id += num_threads){
  //    memcpy(&B_local[0], &B[row_B_local_id * B_WIDTH], sizeof(B[0]) * B_WIDTH);
  //}

  for (int col_B_id = tid; col_B_id < B_WIDTH; col_B_id += num_threads) {
      memcpy(&B_local[0], &B[col_B_id * B_HEIGHT], sizeof(B[0]) * B_HEIGHT);
      for (uint32_t row_A_id = 0; row_A_id < A_HEIGHT; row_A_id ++){
            sum = static_cast<TC>(0);
            memcpy(&A_local[0], &A[row_A_id * A_WIDTH], sizeof(A[0]) * A_WIDTH);
                  for (uint32_t k = 0; k < A_WIDTH; k ++) {
                        sum += A_local[k] * B_local[k];
                  }
                  C[row_A_id * B_WIDTH + col_B_id] = sum;
      }
  }
  
  //----------------------------------funciton end----------------------------------//

 return 0;
}

template <typename TA, typename TB, typename TC>
int  __attribute__ ((noinline)) matrix_multiply_group_v27(TA *A, TB *B, TC *C, // matrix multiplication for v3, v3 implement the data distribution along column side, without scratchpad
                                uint32_t A_HEIGHT, uint32_t A_WIDTH, uint32_t B_HEIGHT, uint32_t B_WIDTH, uint32_t block_row)
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

  int local_A_col_start = 0;
  int local_B_col_start = 0;

  int local_cpy_A_idx = 0;
  int local_cpy_B_idx = 0;

  //int block_size_row = block_row; // 8, 4, 1
  int block_size_row = 2; // 8, 4, 2
  int block_size_col = 20;

  // calculate the number of row and col blocks in each matrix
  int A_num_blk_per_row = (A_WIDTH + block_size_col - 1) / block_size_col;
  int A_num_blk_per_col = (A_HEIGHT + block_size_row - 1) / block_size_row;
  int B_num_blk_per_row = (B_HEIGHT + block_size_col - 1) / block_size_col;
  int B_num_blk_per_col = (B_WIDTH + block_size_row - 1) / block_size_row;

  // calculate the dimension of the last block size in each matrix
  int A_last_blk_col = (A_WIDTH % block_size_col == 0) ? block_size_col : (A_WIDTH % block_size_col);
  int A_last_blk_row = (A_HEIGHT % block_size_row == 0) ? block_size_row : (A_HEIGHT % block_size_row);
  int B_last_blk_col = (B_HEIGHT % block_size_col == 0) ? block_size_col : (B_HEIGHT % block_size_col);
  int B_last_blk_row = (B_WIDTH % block_size_row == 0) ? block_size_row : (B_WIDTH % block_size_row);
  

  for (int A_blk_row_id = tid; A_blk_row_id < A_num_blk_per_col; A_blk_row_id += num_threads){
        for (int B_blk_col_id = 0; B_blk_col_id < B_num_blk_per_row; B_blk_col_id ++){
              // set the sratchpad to store the result block
              int res_row = (A_blk_row_id == A_num_blk_per_col - 1) ? A_last_blk_row : block_size_row;
              int mid_size = (B_blk_col_id == B_num_blk_per_row - 1) ? B_last_blk_col : block_size_col;
              
              float A_blk[res_row * mid_size];
              //float tmp_result[res_row * res_col];
              
              local_cpy_A_idx = 0;
              local_cpy_B_idx = 0;
              if ((res_row == block_size_row) && (mid_size == block_size_col)){
                    dram_to_local_simple(A_blk, A, A_HEIGHT, A_WIDTH, block_size_row, block_size_col, res_row, mid_size, A_blk_row_id, B_blk_col_id);
              }
              else {
                    local_cpy_A_idx = 0;
                    local_A_col_start = B_blk_col_id * block_size_col;
                    for (int local_A_row_start = A_blk_row_id * block_size_row; local_A_row_start < (A_blk_row_id * block_size_row + res_row); local_A_row_start ++){
                      memcpy(&A_blk[local_cpy_A_idx * mid_size], &A[local_A_row_start * A_WIDTH + local_A_col_start], sizeof(A[0]) * mid_size);
                      local_cpy_A_idx ++;
                    }
              }
              

              for (int inside_blk_id = 0; inside_blk_id < B_num_blk_per_col; inside_blk_id ++){

                    int res_col = (inside_blk_id == B_num_blk_per_col - 1) ? B_last_blk_row : block_size_row;

                    float tmp_result[res_row * res_col];

                    // initial the tmp_result to 0
                    //memset(tmp_result, 0, res_row * res_col * sizeof(float));

                    // set the scratchpad to store A block and B block
                    
                    float B_blk[res_col * mid_size];

                    local_cpy_A_idx = 0;
                    local_cpy_B_idx = 0;
                    
                    if ((res_row == block_size_row) && (res_col == block_size_row) && (mid_size == block_size_col)){
                          //dram_to_local_simple(A_blk, A, A_HEIGHT, A_WIDTH, block_size_row, block_size_col, res_row, mid_size, A_blk_row_id, B_blk_col_id);
                          dram_to_local_simple(B_blk, B, B_WIDTH, B_HEIGHT, block_size_row, block_size_col, res_col, mid_size, inside_blk_id, B_blk_col_id);
                          compute_2(tmp_result, C, A_blk, B_blk, res_row, mid_size, res_col, mid_size, A_blk_row_id, inside_blk_id, B_WIDTH);
                    }
                    
                    else {
                          //copy remaining matrix B block into the A_local scratchpad

                          local_B_col_start = B_blk_col_id * block_size_col;
                          for (int local_B_row_start = inside_blk_id * block_size_row; local_B_row_start < (inside_blk_id * block_size_row + res_col); local_B_row_start ++){
                              memcpy(&B_blk[local_cpy_B_idx * mid_size], &B[local_B_row_start * B_HEIGHT + local_B_col_start], sizeof(B[0]) * mid_size);
                              local_cpy_B_idx ++;
                          }
                          
                          // perform multiplication  
                          compute_flex(C, A_blk, B_blk, res_row, mid_size, res_col, mid_size, A_blk_row_id, inside_blk_id, B_WIDTH, block_size_row, block_size_col);
                    }
                  
              }
        }
  }

  return 0;
  
}
#endif //__MATRIX_MULTIPLY_HPP
