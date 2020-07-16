#ifndef __MATRIX_MULTIPLY_HPP
#define __MATRIX_MULTIPLY_HPP
#include <cstdint>
#include <cstring>
#include <bsg_manycore.h>
#include <bsg_tile_group_barrier.h>

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

  int8_t A_local[A_HEIGHT * A_WIDTH];
  int8_t B_local[A_WIDTH * B_WIDTH];

  memcpy (A_local, A, sizeof(A[0])*A_HEIGHT*A_WIDTH);
  memcpy (B_local, B, sizeof(B[0])*A_WIDTH*B_WIDTH);

  for (int row_A_id = tid; row_A_id < A_HEIGHT; row_A_id += num_threads) {
      for (uint32_t col_B_id = 0; col_B_id < B_WIDTH; col_B_id ++){
            sum = static_cast<TC>(0);
                  for (uint32_t k = 0; k < A_WIDTH; k ++) {
                        sum += A_local[row_A_id * A_WIDTH + k] * B_local[k * B_WIDTH + col_B_id]; 
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

  int8_t A_local[A_HEIGHT * A_WIDTH];
  int8_t B_local[A_WIDTH * B_WIDTH];

  memcpy (A_local, A, sizeof(A[0])*A_HEIGHT*A_WIDTH);
  memcpy (B_local, B, sizeof(B[0])*A_WIDTH*B_WIDTH);

  for (int col_B_id = tid; col_B_id < B_WIDTH; col_B_id += num_threads) {
      for (uint32_t row_A_id = 0; row_A_id < A_HEIGHT; row_A_id ++){
            sum = static_cast<TC>(0);
                  for (uint32_t k = 0; k < A_WIDTH; k ++) {
                        sum += A_local[row_A_id * A_WIDTH + k] * B_local[k * B_WIDTH + col_B_id]; //before: sum += A[row_A_id * A_WIDTH + k] * B[k * B_WIDTH + row_A_id];
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
  
  int8_t A_local[A_HEIGHT * A_WIDTH];
  int8_t B_local[A_WIDTH * B_WIDTH];

  memcpy (A_local, A, sizeof(A[0])*A_HEIGHT*A_WIDTH);
  memcpy (B_local, B, sizeof(B[0])*A_WIDTH*B_WIDTH);

  for (int row_A_id = tid; row_A_id < A_HEIGHT; row_A_id += num_threads) {
      for (uint32_t col_B_id = 0; col_B_id < B_WIDTH; col_B_id ++){
            sum = static_cast<TC>(0);
                  for (uint32_t k = 0; k < A_WIDTH; k ++) {
                        sum += A_local[row_A_id * A_WIDTH + k] * B_local[k * B_WIDTH + col_B_id]; 
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

  int8_t A_local[A_HEIGHT * A_WIDTH];
  int8_t B_local[A_WIDTH * B_WIDTH];
  // copy data
  memcpy (A_local, A, sizeof(A[0])*A_HEIGHT*A_WIDTH);
  memcpy (B_local, B, sizeof(B[0])*A_WIDTH*B_WIDTH);

  for (int col_B_id = tid; col_B_id < B_WIDTH; col_B_id += num_threads) {
      for (uint32_t row_A_id = 0; row_A_id < A_HEIGHT; row_A_id ++){
            sum = static_cast<TC>(0);
                  for (uint32_t k = 0; k < A_WIDTH; k ++) {
                        sum += A_local[row_A_id * A_WIDTH + k] * B_local[k * B_WIDTH + col_B_id]; //before: sum += A[row_A_id * A_WIDTH + k] * B[k * B_WIDTH + row_A_id];
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

  int8_t A_local[A_HEIGHT * A_WIDTH];
  int8_t B_local[A_WIDTH * B_WIDTH];
  // copy data
  memcpy (A_local, A, sizeof(A[0])*A_HEIGHT*A_WIDTH);
  memcpy (B_local, B, sizeof(B[0])*A_WIDTH*B_WIDTH);

  for (int row_A_id = tid; row_A_id < A_HEIGHT; row_A_id += num_threads) {
      for (uint32_t col_B_id = 0; col_B_id < B_WIDTH; col_B_id ++){
            sum = static_cast<TC>(0);
                  for (uint32_t k = 0; k < A_WIDTH; k ++) {
                        sum += A_local[row_A_id * A_WIDTH + k] * B_local[k * B_WIDTH + col_B_id]; 
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

  int8_t A_local[A_HEIGHT * A_WIDTH];
  int8_t B_local[A_WIDTH * B_WIDTH];
  // copy data
  memcpy (A_local, A, sizeof(A[0])*A_HEIGHT*A_WIDTH);
  memcpy (B_local, B, sizeof(B[0])*A_WIDTH*B_WIDTH);

  for (int col_B_id = tid; col_B_id < B_WIDTH; col_B_id += num_threads) {
      for (uint32_t row_A_id = 0; row_A_id < A_HEIGHT; row_A_id ++){
            sum = static_cast<TC>(0);
                  for (uint32_t k = 0; k < A_WIDTH; k ++) {
                        sum += A_local[row_A_id * A_WIDTH + k] * B_local[k * B_WIDTH + col_B_id]; //before: sum += A[row_A_id * A_WIDTH + k] * B[k * B_WIDTH + row_A_id];
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

  int8_t A_local[A_HEIGHT * A_WIDTH];
  int8_t B_local[A_WIDTH * B_WIDTH];
  // copy data
  memcpy (A_local, A, sizeof(A[0])*A_HEIGHT*A_WIDTH);
  memcpy (B_local, B, sizeof(B[0])*A_WIDTH*B_WIDTH);

  for (int row_B_id = tid; row_B_id < B_WIDTH; row_B_id += num_threads) {
      for (uint32_t row_A_id = 0; row_A_id < A_HEIGHT; row_A_id ++){
            sum = static_cast<TC>(0);
                  for (uint32_t k = 0; k < A_WIDTH; k ++) {
                        sum += A_local[row_A_id * A_WIDTH + k] * B_local[row_B_id * B_WIDTH + k]; //before: sum += A[row_A_id * A_WIDTH + k] * B[k * B_WIDTH + row_A_id];
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

  int8_t A_local[A_HEIGHT * A_WIDTH];
  int8_t B_local[A_WIDTH * B_WIDTH];
  // copy data
  memcpy (A_local, A, sizeof(A[0])*A_HEIGHT*A_WIDTH);
  memcpy (B_local, B, sizeof(B[0])*A_WIDTH*B_WIDTH);

  int A_row_top;
  A_row_top = (((tid + 1) * num_row_A_per_thread) < A_HEIGHT) ? ((tid + 1) * num_row_A_per_thread) : A_HEIGHT;
  for (int row_A_id = tid * num_row_A_per_thread; row_A_id < A_row_top; row_A_id ++) {
      for (uint32_t col_B_id = 0; col_B_id < B_WIDTH; col_B_id ++){
            sum = static_cast<TC>(0);
                  for (uint32_t k = 0; k < A_WIDTH; k ++) {
                        sum += A_local[row_A_id * A_WIDTH + k] * B_local[k * B_WIDTH + col_B_id]; 
                  }
                  C[row_A_id * B_WIDTH + col_B_id] = sum;
            }
  }


 return 0;
}

template <typename TA, typename TB, typename TC>
int  __attribute__ ((noinline)) matrix_multiply_group_v10(TA *A, TB *B, TC *C, // matrix multiplication for v10, v4 implement the data distribution along row side, with scratchpad
                                uint32_t A_HEIGHT, uint32_t A_WIDTH, uint32_t B_WIDTH)
{

  //TC C_local[A_HEIGHT * B_WIDTH];

  // get the absolute tid (which group then which tid within that group)
  int tid_x = __bsg_tile_group_id_x * BSG_TILE_GROUP_X_DIM + __bsg_x;
  int tid_y = __bsg_tile_group_id_y * BSG_TILE_GROUP_Y_DIM + __bsg_y;
  int num_threads_x = BSG_TILE_GROUP_X_DIM * __bsg_grid_dim_x;
  int num_threads_y = BSG_TILE_GROUP_Y_DIM * __bsg_grid_dim_y;
        
  int num_threads = num_threads_x * num_threads_y;
  int local_idx_cpy = 0;
  int local_idx_cal = 0;
  //float num_row_A_per_thread = A_HEIGHT / num_threads;
  int num_row_A_per_thread = A_HEIGHT / num_threads;
  int tid = tid_y * num_threads_x + tid_x;
  TC sum;

  int8_t A_local[num_row_A_per_thread * A_WIDTH];
  int8_t B_local[A_WIDTH * B_WIDTH];

  memcpy (B_local, B, sizeof(B[0])*A_WIDTH*B_WIDTH);

  for (int row_A_local_id = tid; row_A_local_id < A_HEIGHT; row_A_local_id += num_threads) {
      //for (int col_B_local_id = 0; col_B_local_id < B_WIDTH; col_B_local_id ++){
      memcpy(&A_local[local_idx_cpy * A_WIDTH], &A[row_A_local_id * A_WIDTH], sizeof(A[0]) * A_WIDTH);
      //}
      local_idx_cpy += 1;
  }

  for (int row_A_id = tid; row_A_id < A_HEIGHT; row_A_id += num_threads) {
      for (uint32_t col_B_id = 0; col_B_id < B_WIDTH; col_B_id ++){
            sum = static_cast<TC>(0);
                  for (uint32_t k = 0; k < A_WIDTH; k ++) {
                        sum += A_local[local_idx_cal * A_WIDTH + k] * B_local[k * B_WIDTH + col_B_id]; 
                  }
                  C[row_A_id * B_WIDTH + col_B_id] = sum;
      }
      local_idx_cal += 1;
  }

  return 0;
}

template <typename TA, typename TB, typename TC>
int  __attribute__ ((noinline)) matrix_multiply_group_v11(TA *A, TB *B, TC *C, // matrix multiplication for v3, v3 implement the data distribution along column side, without scratchpad
                                uint32_t A_HEIGHT, uint32_t A_WIDTH, uint32_t B_WIDTH)
{


  // get the absolute tid (which group then which tid within that group)
  int tid_x = __bsg_tile_group_id_x * BSG_TILE_GROUP_X_DIM + __bsg_x;
  int tid_y = __bsg_tile_group_id_y * BSG_TILE_GROUP_Y_DIM + __bsg_y;
  int num_threads_x = BSG_TILE_GROUP_X_DIM * __bsg_grid_dim_x;
  int num_threads_y = BSG_TILE_GROUP_Y_DIM * __bsg_grid_dim_y;
        
  int num_threads = num_threads_x * num_threads_y;

  int num_col_B_per_thread = B_WIDTH / num_threads;
  int tid = tid_y * num_threads_x + tid_x;
  TC sum;
  
  int local_idx_cpy = 0;
  int local_idx_cal = 0;

  int8_t A_local[A_HEIGHT * A_WIDTH];
  int8_t B_local[num_col_B_per_thread * B_WIDTH];
  // copy data
  memcpy (A_local, A, sizeof(A[0])*A_HEIGHT*A_WIDTH);
  
  for ( int row_B_local_id = tid; row_B_local_id < B_WIDTH; row_B_local_id += num_threads){
      memcpy(&B_local[local_idx_cpy * B_WIDTH], &B[row_B_local_id * B_WIDTH], sizeof(B[0]) * B_WIDTH);
      local_idx_cpy += 1;
  }

  for (int col_B_id = tid; col_B_id < B_WIDTH; col_B_id += num_threads) {
      for (uint32_t row_A_id = 0; row_A_id < A_HEIGHT; row_A_id ++){
            sum = static_cast<TC>(0);
                  for (uint32_t k = 0; k < A_WIDTH; k ++) {
                        sum += A_local[row_A_id * A_WIDTH + k] * B_local[local_idx_cal * B_WIDTH + k]; //before: sum += A[row_A_id * A_WIDTH + k] * B[k * B_WIDTH + row_A_id];
                  }
                  C[row_A_id * B_WIDTH + col_B_id] = sum;
      }
      local_idx_cal += 1;
  }


 return 0;
}

template <typename TA, typename TB, typename TC>
int  __attribute__ ((noinline)) matrix_multiply_group_v12(TA *A, TB *B, TC *C, // matrix multiplication for v3, v3 implement the data distribution along column side, without scratchpad
                                uint32_t A_HEIGHT, uint32_t A_WIDTH, uint32_t B_WIDTH)
{


  // get the absolute tid (which group then which tid within that group)
  int tid_x = __bsg_tile_group_id_x * BSG_TILE_GROUP_X_DIM + __bsg_x;
  int tid_y = __bsg_tile_group_id_y * BSG_TILE_GROUP_Y_DIM + __bsg_y;
  int num_threads_x = BSG_TILE_GROUP_X_DIM * __bsg_grid_dim_x;
  int num_threads_y = BSG_TILE_GROUP_Y_DIM * __bsg_grid_dim_y;
        
  int num_threads = num_threads_x * num_threads_y;

  int num_col_B_per_thread = B_WIDTH / num_threads;
  int tid = tid_y * num_threads_x + tid_x;
  TC sum;

  int local_idx_cpy = 0;
  int local_idx_cal = 0;

  int8_t A_local[A_HEIGHT * A_WIDTH];
  int8_t B_local[num_col_B_per_thread * B_WIDTH];
  // copy data
  memcpy (A_local, A, sizeof(A[0])*A_HEIGHT*A_WIDTH);
  
  for ( int col_B_local_id = tid; col_B_local_id < B_WIDTH; col_B_local_id += num_threads){
      for (int k = 0; k < B_WIDTH; k++){
            memcpy(&B_local[k * num_col_B_per_thread + local_idx_cpy], &B[k * B_WIDTH + col_B_local_id], sizeof(B[0]));
      }
      local_idx_cpy += 1;
  }

  for (int col_B_id = tid; col_B_id < B_WIDTH; col_B_id += num_threads) {
      for (uint32_t row_A_id = 0; row_A_id < A_HEIGHT; row_A_id ++){
            sum = static_cast<TC>(0);
                  for (uint32_t k = 0; k < A_WIDTH; k ++) {
                        sum += A_local[row_A_id * A_WIDTH + k] * B_local[k * num_col_B_per_thread + local_idx_cal]; //before: sum += A[row_A_id * A_WIDTH + k] * B[k * B_WIDTH + row_A_id];
                  }
                  C[row_A_id * B_WIDTH + col_B_id] = sum;
      }
      local_idx_cal += 1;
  }


 return 0;
}

template <typename TA, typename TB, typename TC>
int  __attribute__ ((noinline)) matrix_multiply_group_v13(TA *A, TB *B, TC *C, // matrix multiplication for v3, v3 implement the data distribution along column side, without scratchpad
                                uint32_t A_HEIGHT, uint32_t A_WIDTH, uint32_t B_WIDTH)
{


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

  int8_t A_local[A_WIDTH];
  int8_t B_local[B_WIDTH];
  // copy data
  //memcpy (A_local, A, sizeof(A[0]) * A_WIDTH);
  
  //for ( int row_B_local_id = tid; row_B_local_id < B_WIDTH; row_B_local_id += num_threads){
  //    memcpy(&B_local[0], &B[row_B_local_id * B_WIDTH], sizeof(B[0]) * B_WIDTH);
  //}

  for (int col_B_id = tid; col_B_id < B_WIDTH; col_B_id += num_threads) {
      memcpy(&B_local[0], &B[col_B_id * B_WIDTH], sizeof(B[0]) * B_WIDTH);
      for (uint32_t row_A_id = 0; row_A_id < A_HEIGHT; row_A_id ++){
            sum = static_cast<TC>(0);
            memcpy(&A_local[0], &A[row_A_id * A_WIDTH], sizeof(A[0]) * A_WIDTH);
                  for (uint32_t k = 0; k < A_WIDTH; k ++) {
                        sum += A_local[k] * B_local[k];
                  }
                  C[row_A_id * B_WIDTH + col_B_id] = sum;
      }
  }


 return 0;
}

template <typename TA, typename TB, typename TC>
int  __attribute__ ((noinline)) matrix_multiply_group_v15(TA *A, TB *B, TC *C, // matrix multiplication for v3, v3 implement the data distribution along column side, without scratchpad
                                uint32_t A_HEIGHT, uint32_t A_WIDTH, uint32_t B_WIDTH)
{


  // get the absolute tid (which group then which tid within that group)
  int tid_x = __bsg_tile_group_id_x * BSG_TILE_GROUP_X_DIM + __bsg_x;
  int tid_y = __bsg_tile_group_id_y * BSG_TILE_GROUP_Y_DIM + __bsg_y;
  int num_threads_x = BSG_TILE_GROUP_X_DIM * __bsg_grid_dim_x;
  int num_threads_y = BSG_TILE_GROUP_Y_DIM * __bsg_grid_dim_y;
        
  int num_threads = num_threads_x * num_threads_y;

  int num_col_B_per_thread = B_WIDTH / num_threads;
  int tid = tid_y * num_threads_x + tid_x;
  TC sum;

  int local_cpy_A_idx = 0;
  int local_cpy_B_idx = 0;

  int block_size = A_HEIGHT / BSG_TILE_GROUP_X_DIM;

  int8_t A_local[block_size * block_size];
  int8_t B_local[block_size * block_size];
  int8_t C_local[block_size * block_size];
  
  // copy matrix A block into scratchpad-stage 1
  
  int row_A_start = tid_y * block_size;
  int row_A_end = ((row_A_start + block_size) > A_HEIGHT) ? A_HEIGHT : (row_A_start + block_size);
  int col_A_start = tid_x * block_size;
  int col_A_end = ((col_A_start + block_size) > A_WIDTH) ? A_WIDTH : (col_A_start + block_size);

  for (int row_A_id = row_A_start; row_A_id < row_A_end; row_A_id ++){
        memcpy(&A_local[local_cpy_A_idx * block_size], &A[row_A_id * A_WIDTH + col_A_start], sizeof(A[0]) * block_size);
        local_cpy_A_idx ++;
  }

  // copy matrix B block into scratchpad-stage 1
  int row_B_start = (tid_x == 0) ? 0 : block_size;
  int row_B_end = row_B_start + block_size;
  int col_B_start = (tid_x == 0) ? 0 : block_size;
  int col_B_end = col_B_start + block_size;

  for (int row_B_id = row_B_start; row_B_id < row_B_end; row_B_id ++){
        memcpy(&B_local[local_cpy_B_idx * block_size], &B[row_B_id * B_WIDTH + col_B_start], sizeof(B[0]) * block_size);
        local_cpy_B_idx ++;
  }

  //perform stage 1 multiplication

  for (int A_local_row_id = 0; A_local_row_id < block_size; A_local_row_id ++) {
      for (uint32_t B_local_row_id = 0; B_local_row_id < block_size; B_local_row_id ++){
            sum = static_cast<TC>(0);
                  for (uint32_t k = 0; k < block_size; k ++) {
                        sum += A_local[A_local_row_id * block_size + k] * B_local[B_local_row_id * block_size + k];
                  }
                  C_local[A_local_row_id * block_size + B_local_row_id] = sum;
      }
  }

  local_cpy_A_idx = 0;
  local_cpy_B_idx = 0;
// copy matrix A block into scratchpad-stage 2
  
  row_A_start = tid_y * block_size;
  row_A_end = ((row_A_start + block_size) > A_HEIGHT) ? A_HEIGHT : (row_A_start + block_size);
  col_A_start = (tid_x == 0) ? block_size : 0;
  col_A_end = ((col_A_start + block_size) > A_HEIGHT ) ? A_HEIGHT : (col_A_start + block_size);
  

  for (int row_A_id = row_A_start; row_A_id < row_A_end; row_A_id ++){
        memcpy(&A_local[local_cpy_A_idx * block_size], &A[row_A_id * A_WIDTH + col_A_start], sizeof(A[0]) * block_size);
        local_cpy_A_idx ++;
  }

  // copy matrix B block into scratchpad-stage 2
  row_B_start = (tid_x == 0) ? 0 : block_size;
  row_B_end = row_B_start + block_size;
  col_B_start = (tid_x == 0) ? block_size : 0;
  col_B_end = col_B_start + block_size;

  for (int row_B_id = row_B_start; row_B_id < row_B_end; row_B_id ++){
        memcpy(&B_local[local_cpy_B_idx * block_size], &B[row_B_id * B_WIDTH + col_B_start], sizeof(B[0]) * block_size);
        local_cpy_B_idx ++;
  }

  //perform stage 2 multiplication

  for (int A_local_row_id = 0; A_local_row_id < block_size; A_local_row_id ++) {
      for (uint32_t B_local_row_id = 0; B_local_row_id < block_size; B_local_row_id ++){
            sum = C_local[A_local_row_id * block_size + B_local_row_id];
                  for (uint32_t k = 0; k < block_size; k ++) {
                        sum += A_local[A_local_row_id * block_size + k] * B_local[B_local_row_id * block_size + k];
                  }
                  C_local[A_local_row_id * block_size + B_local_row_id] = sum;
      }
  }

  int memback_row_idx = 0;
  int memback_col_idx = 0;
  //Write back the result
  for (int memory_row_id = tid_y * block_size; memory_row_id < (tid_y + 1) * block_size; memory_row_id ++){
        memback_col_idx = 0;
        for (int memory_col_id = tid_x * block_size; memory_col_id < (tid_x + 1) * block_size; memory_col_id ++){
              C[memory_row_id * B_WIDTH + memory_col_id] = C_local[memback_row_idx * block_size + memback_col_idx];
              memback_col_idx ++;
        }
        memback_row_idx ++;
  }


 return 0;
}

#endif //__MATRIX_MULTIPLY_HPP
