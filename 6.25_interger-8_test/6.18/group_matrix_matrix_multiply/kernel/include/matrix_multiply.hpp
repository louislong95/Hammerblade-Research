#ifndef __MATRIX_MULTIPLY_HPP
#define __MATRIX_MULTIPLY_HPP
#include <cstdint>

template <typename TA, typename TB, typename TC>
int  __attribute__ ((noinline)) matrix_multiply_group_v2(TA *A, TB *B, TC *C,
                                uint32_t A_HEIGHT, uint32_t A_WIDTH, uint32_t B_WIDTH)
{


  // get the absolute tid (which group then which tid within that group)
  int tid_x = __bsg_tile_group_id_x * BSG_TILE_GROUP_X_DIM + __bsg_x;
  int tid_y = __bsg_tile_group_id_y * BSG_TILE_GROUP_Y_DIM + __bsg_y;
  int num_threads_x = BSG_TILE_GROUP_X_DIM * __bsg_grid_dim_x;
  int num_threads_y = BSG_TILE_GROUP_Y_DIM * __bsg_grid_dim_y;
        
  int num_threads = num_threads_x * num_threads_y;

  float num_row_A_per_thread = A_HEIGHT / num_threads;
  int tid = tid_y * num_threads_x + tid_x;
  TC sum;
  for (int row_A_id = tid; row_A_id < A_HEIGHT; row_A_id += num_threads) {
      for (uint32_t col_B_id = 0; col_B_id < B_WIDTH; col_B_id ++){
            sum = static_cast<TC>(0);
                  for (uint32_t k = 0; k < A_WIDTH; k ++) {
                        sum += A[row_A_id * A_WIDTH + k] * B[k * B_WIDTH + col_B_id]; //before: sum += A[row_A_id * A_WIDTH + k] * B[k * B_WIDTH + row_A_id];
                  }
                  C[row_A_id * B_WIDTH + col_B_id] = sum;
            }
  }


 return 0;
}

#endif //__MATRIX_MULTIPLY_HPP
