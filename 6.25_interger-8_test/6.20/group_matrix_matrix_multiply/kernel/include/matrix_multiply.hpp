#ifndef __MATRIX_MULTIPLY_HPP
#define __MATRIX_MULTIPLY_HPP
#include <cstdint>
#include <cstring>

template <typename TA, typename TB, typename TC>
int  __attribute__ ((noinline)) matrix_multiply_group_v0(TA *A, TB *B, TC *C, // matrix multiplication for v0, v0 implement the data distribution along row side, with scratchpad
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
                        sum += A[row_A_id * A_WIDTH + k] * B[k * B_WIDTH + col_B_id]; 
                  }
                  C[row_A_id * B_WIDTH + col_B_id] = sum;
            }
  }


 return 0;
}

template <typename TA, typename TB, typename TC>
int  __attribute__ ((noinline)) matrix_multiply_group_v1(TA *A, TB *B, TC *C, // matrix multiplication for v3, v3 implement the data distribution along column side, without scratchpad
                                uint32_t A_HEIGHT, uint32_t A_WIDTH, uint32_t B_WIDTH)
{


  // get the absolute tid (which group then which tid within that group)
  int tid_x = __bsg_tile_group_id_x * BSG_TILE_GROUP_X_DIM + __bsg_x;
  int tid_y = __bsg_tile_group_id_y * BSG_TILE_GROUP_Y_DIM + __bsg_y;
  int num_threads_x = BSG_TILE_GROUP_X_DIM * __bsg_grid_dim_x;
  int num_threads_y = BSG_TILE_GROUP_Y_DIM * __bsg_grid_dim_y;
        
  int num_threads = num_threads_x * num_threads_y;

  float num_col_B_per_thread = B_WIDTH / num_threads;
  int tid = tid_y * num_threads_x + tid_x;
  TC sum;
  for (int col_B_id = tid; col_B_id < B_WIDTH; col_B_id += num_threads) {
      for (uint32_t row_A_id = 0; row_A_id < A_HEIGHT; row_A_id ++){
            sum = static_cast<TC>(0);
                  for (uint32_t k = 0; k < A_WIDTH; k ++) {
                        sum += A[row_A_id * A_WIDTH + k] * B[k * B_WIDTH + col_B_id]; //before: sum += A[row_A_id * A_WIDTH + k] * B[k * B_WIDTH + row_A_id];
                  }
                  C[row_A_id * B_WIDTH + col_B_id] = sum;
            }
  }


 return 0;
}

template <typename TA, typename TB, typename TC>
int  __attribute__ ((noinline)) matrix_multiply_group_v2(TA *A, TB *B, TC *C,    // matrix multiplication for v2, v2 implement the data distribution along row side, without scratchpad
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

template <typename TA, typename TB, typename TC>
int  __attribute__ ((noinline)) matrix_multiply_group_v3(TA *A, TB *B, TC *C, // matrix multiplication for v3, v3 implement the data distribution along column side, without scratchpad
                                uint32_t A_HEIGHT, uint32_t A_WIDTH, uint32_t B_WIDTH)
{


  // get the absolute tid (which group then which tid within that group)
  int tid_x = __bsg_tile_group_id_x * BSG_TILE_GROUP_X_DIM + __bsg_x;
  int tid_y = __bsg_tile_group_id_y * BSG_TILE_GROUP_Y_DIM + __bsg_y;
  int num_threads_x = BSG_TILE_GROUP_X_DIM * __bsg_grid_dim_x;
  int num_threads_y = BSG_TILE_GROUP_Y_DIM * __bsg_grid_dim_y;
        
  int num_threads = num_threads_x * num_threads_y;

  float num_col_B_per_thread = B_WIDTH / num_threads;
  int tid = tid_y * num_threads_x + tid_x;
  TC sum;
  for (int col_B_id = tid; col_B_id < B_WIDTH; col_B_id += num_threads) {
      for (uint32_t row_A_id = 0; row_A_id < A_HEIGHT; row_A_id ++){
            sum = static_cast<TC>(0);
                  for (uint32_t k = 0; k < A_WIDTH; k ++) {
                        sum += A[row_A_id * A_WIDTH + k] * B[k * B_WIDTH + col_B_id]; //before: sum += A[row_A_id * A_WIDTH + k] * B[k * B_WIDTH + row_A_id];
                  }
                  C[row_A_id * B_WIDTH + col_B_id] = sum;
            }
  }


 return 0;
}

template <typename TA, typename TB, typename TC>
int  __attribute__ ((noinline)) matrix_multiply_group_v4(TA *A, TB *B, TC *C, // matrix multiplication for v4, v4 implement the data distribution along row side, with scratchpad
                                uint32_t A_HEIGHT, uint32_t A_WIDTH, uint32_t B_WIDTH)
{

  //TC C_local[A_HEIGHT * B_WIDTH];

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
                        sum += A[row_A_id * A_WIDTH + k] * B[k * B_WIDTH + col_B_id]; 
                  }
                  C[row_A_id * B_WIDTH + col_B_id] = sum;
            }
  }


 return 0;
}

template <typename TA, typename TB, typename TC>
int  __attribute__ ((noinline)) matrix_multiply_group_v5(TA *A, TB *B, TC *C, // matrix multiplication for v3, v3 implement the data distribution along column side, without scratchpad
                                uint32_t A_HEIGHT, uint32_t A_WIDTH, uint32_t B_WIDTH)
{


  // get the absolute tid (which group then which tid within that group)
  int tid_x = __bsg_tile_group_id_x * BSG_TILE_GROUP_X_DIM + __bsg_x;
  int tid_y = __bsg_tile_group_id_y * BSG_TILE_GROUP_Y_DIM + __bsg_y;
  int num_threads_x = BSG_TILE_GROUP_X_DIM * __bsg_grid_dim_x;
  int num_threads_y = BSG_TILE_GROUP_Y_DIM * __bsg_grid_dim_y;
        
  int num_threads = num_threads_x * num_threads_y;

  float num_col_B_per_thread = B_WIDTH / num_threads;
  int tid = tid_y * num_threads_x + tid_x;
  TC sum;
  for (int col_B_id = tid; col_B_id < B_WIDTH; col_B_id += num_threads) {
      for (uint32_t row_A_id = 0; row_A_id < A_HEIGHT; row_A_id ++){
            sum = static_cast<TC>(0);
                  for (uint32_t k = 0; k < A_WIDTH; k ++) {
                        sum += A[row_A_id * A_WIDTH + k] * B[k * B_WIDTH + col_B_id]; //before: sum += A[row_A_id * A_WIDTH + k] * B[k * B_WIDTH + row_A_id];
                  }
                  C[row_A_id * B_WIDTH + col_B_id] = sum;
            }
  }


 return 0;
}

template <typename TA, typename TB, typename TC>
int  __attribute__ ((noinline)) matrix_multiply_group_v6(TA *A, TB *B, TC *C, // matrix multiplication for v0, v0 implement the data distribution along row side, with scratchpad
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
                        sum += A[row_A_id * A_WIDTH + k] * B[k * B_WIDTH + col_B_id]; 
                  }
                  C[row_A_id * B_WIDTH + col_B_id] = sum;
            }
  }


 return 0;
}

template <typename TA, typename TB, typename TC>
int  __attribute__ ((noinline)) matrix_multiply_group_v7(TA *A, TB *B, TC *C, // matrix multiplication for v3, v3 implement the data distribution along column side, without scratchpad
                                uint32_t A_HEIGHT, uint32_t A_WIDTH, uint32_t B_WIDTH)
{


  // get the absolute tid (which group then which tid within that group)
  int tid_x = __bsg_tile_group_id_x * BSG_TILE_GROUP_X_DIM + __bsg_x;
  int tid_y = __bsg_tile_group_id_y * BSG_TILE_GROUP_Y_DIM + __bsg_y;
  int num_threads_x = BSG_TILE_GROUP_X_DIM * __bsg_grid_dim_x;
  int num_threads_y = BSG_TILE_GROUP_Y_DIM * __bsg_grid_dim_y;
        
  int num_threads = num_threads_x * num_threads_y;

  float num_col_B_per_thread = B_WIDTH / num_threads;
  int tid = tid_y * num_threads_x + tid_x;
  TC sum;
  for (int col_B_id = tid; col_B_id < B_WIDTH; col_B_id += num_threads) {
      for (uint32_t row_A_id = 0; row_A_id < A_HEIGHT; row_A_id ++){
            sum = static_cast<TC>(0);
                  for (uint32_t k = 0; k < A_WIDTH; k ++) {
                        sum += A[row_A_id * A_WIDTH + k] * B[k * B_WIDTH + col_B_id]; //before: sum += A[row_A_id * A_WIDTH + k] * B[k * B_WIDTH + row_A_id];
                  }
                  C[row_A_id * B_WIDTH + col_B_id] = sum;
            }
  }


 return 0;
}

template <typename TA, typename TB, typename TC>
int  __attribute__ ((noinline)) matrix_multiply_group_v8(TA *A, TB *B, TC *C, // matrix multiplication for v3, v3 implement the data distribution along column side, without scratchpad
                                uint32_t A_HEIGHT, uint32_t A_WIDTH, uint32_t B_WIDTH)
{


  // get the absolute tid (which group then which tid within that group)
  int tid_x = __bsg_tile_group_id_x * BSG_TILE_GROUP_X_DIM + __bsg_x;
  int tid_y = __bsg_tile_group_id_y * BSG_TILE_GROUP_Y_DIM + __bsg_y;
  int num_threads_x = BSG_TILE_GROUP_X_DIM * __bsg_grid_dim_x;
  int num_threads_y = BSG_TILE_GROUP_Y_DIM * __bsg_grid_dim_y;
        
  int num_threads = num_threads_x * num_threads_y;

  float num_col_B_per_thread = B_WIDTH / num_threads;
  int tid = tid_y * num_threads_x + tid_x;
  TC sum;
  for (int row_B_id = tid; row_B_id < B_WIDTH; row_B_id += num_threads) {
      for (uint32_t row_A_id = 0; row_A_id < A_HEIGHT; row_A_id ++){
            sum = static_cast<TC>(0);
                  for (uint32_t k = 0; k < A_WIDTH; k ++) {
                        sum += A[row_A_id * A_WIDTH + k] * B[row_B_id * B_WIDTH + k]; //before: sum += A[row_A_id * A_WIDTH + k] * B[k * B_WIDTH + row_A_id];
                  }
                  C[row_A_id * B_WIDTH + row_B_id] = sum;
            }
  }


 return 0;
}

template <typename TA, typename TB, typename TC>
int  __attribute__ ((noinline)) matrix_multiply_group_v9(TA *A, TB *B, TC *C, // matrix multiplication for v4, v4 implement the data distribution along row side, with scratchpad
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
  int A_row_top;
  A_row_top = (((tid + 1) * num_row_A_per_thread) < A_HEIGHT) ? ((tid + 1) * num_row_A_per_thread) : A_HEIGHT;
  for (int row_A_id = tid * num_row_A_per_thread; row_A_id < A_row_top; row_A_id ++) {
      for (uint32_t col_B_id = 0; col_B_id < B_WIDTH; col_B_id ++){
            sum = static_cast<TC>(0);
                  for (uint32_t k = 0; k < A_WIDTH; k ++) {
                        sum += A[row_A_id * A_WIDTH + k] * B[k * B_WIDTH + col_B_id]; 
                  }
                  C[row_A_id * B_WIDTH + col_B_id] = sum;
            }
  }


 return 0;
}

#endif //__MATRIX_MULTIPLY_HPP
