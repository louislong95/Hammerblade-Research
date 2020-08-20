/*
 * This kernel performs matrix multiplication.
 */

// BSG_TILE_GROUP_X_DIM and BSG_TILE_GROUP_Y_DIM must be defined
// before bsg_manycore.h and bsg_tile_group_barrier.h are
// included. bsg_tiles_X and bsg_tiles_Y must also be defined for
// legacy reasons, but they are deprecated.
#define BSG_TILE_GROUP_X_DIM 2
#define BSG_TILE_GROUP_Y_DIM 2
#define bsg_tiles_X BSG_TILE_GROUP_X_DIM
#define bsg_tiles_Y BSG_TILE_GROUP_Y_DIM
#include <bsg_manycore.h>
#include <bsg_tile_group_barrier.h>
#include <cstdint>

#include <matrix_multiply.hpp>

// I <3 Hacks! Since bsg_manycore_arch.h can't handle the awesomeness
// of C++ templates I wrote this temporary replacement.
#ifdef bsg_tile_group_remote_ptr
#undef bsg_tile_group_remote_ptr
#include <type_traits>
#define bsg_tile_group_remote_ptr(__type,x,y,local_addr) \
        ( (typename std::add_pointer<__type>::type)                     \
        (   (REMOTE_EPA_PREFIX << REMOTE_EPA_MASK_SHIFTS)               \
            | ((y) << Y_CORD_SHIFTS )                                   \
            | ((x) << X_CORD_SHIFTS )                                   \
            | ((uint32_t) (local_addr)   )                              \
            )                                                           \
                                                           )
#endif

INIT_TILE_GROUP_BARRIER(r_barrier, c_barrier,
                        0, BSG_TILE_GROUP_X_DIM-1,
                        0, BSG_TILE_GROUP_Y_DIM-1);

template <typename TA, typename TB, typename TC>
int  __attribute__ ((noinline)) matrix_multiply_group_v16(TA *A, TB *B, TC *C, // matrix multiplication for v3, v3 implement the data distribution along column side, without scratchpad
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
  
  bsg_tile_group_shared_mem (TA, sh_A, (A_HEIGHT * A_WIDTH));
  bsg_tile_group_shared_mem (TA, sh_B, (A_HEIGHT * B_WIDTH));
  
  //copy matrix A to shared memory
  for (int row_A_id = 0; row_A_id < A_HEIGHT; row_A_id ++){
        for (int col_A_id = 0; col_A_id < A_WIDTH; col_A_id ++){
              bsg_tile_group_shared_store (TA, sh_A, (row_A_id * A_WIDTH + col_A_id), A[row_A_id * A_WIDTH + col_A_id]);
        }
  }

  //copy matrix B to shared memory
  for (int row_B_id = 0; row_B_id < A_HEIGHT; row_B_id ++){
        for (int col_B_id = 0; col_B_id < B_WIDTH; col_B_id ++){
              bsg_tile_group_shared_store (TA, sh_B, (row_B_id * B_WIDTH + col_B_id), B[row_B_id * B_WIDTH + col_B_id]);
        }
  }

  bsg_tile_group_barrier (&r_barrier, &c_barrier);

  bsg_cuda_print_stat_kernel_start();
  bsg_cuda_print_stat_start(0);
  
  //perform element multiplication
  TA tmp_A;
  TB tmp_B;

  for (int row_B_id = tid; row_B_id < B_WIDTH; row_B_id += num_threads) {
      for (uint32_t row_A_id = 0; row_A_id < A_HEIGHT; row_A_id ++){
            sum = static_cast<TC>(0);
                  for (uint32_t k = 0; k < A_WIDTH; k ++) {
                        bsg_tile_group_shared_load (TA, sh_A, (row_A_id * A_WIDTH + k), tmp_A);
                        bsg_tile_group_shared_load (TB, sh_B, (row_B_id * B_WIDTH + k), tmp_B);
                        sum += tmp_A * tmp_B;
                  }
                  C[row_A_id * B_WIDTH + row_B_id] = sum;
            }
  }
 

 return 0;
}

extern "C" {
        int  __attribute__ ((noinline)) kernel_matrix_multiply_float(
                      float *A, float *B, float *C,
                      uint32_t A_HEIGHT, uint32_t A_WIDTH, uint32_t B_WIDTH) {
                int rc;

                // make use of the local memory in the tile, copy the matrix into the local memory in the tiles
                // inital the memory space
                //int8_t A_local[(A_HEIGHT / BSG_TILE_GROUP_X_DIM) * (A_WIDTH / BSG_TILE_GROUP_Y_DIM)];
                //int8_t B_local[(B_WIDTH / BSG_TILE_GROUP_X_DIM) * (B_WIDTH / BSG_TILE_GROUP_Y_DIM)];
                //int8_t C_local[(A_HEIGHT / BSG_TILE_GROUP_X_DIM) * (B_WIDTH / BSG_TILE_GROUP_Y_DIM)];
                // copy data
                //memcpy (A_local, A, sizeof(A[0])*A_HEIGHT*A_WIDTH);
                //memcpy (B_local, B, sizeof(B[0])*A_WIDTH*B_WIDTH);

                //bsg_cuda_print_stat_kernel_start();
                //bsg_cuda_print_stat_start(0);
                rc = matrix_multiply_group_v16(A, B, C,
                                           A_HEIGHT, A_WIDTH, B_WIDTH
                                           );
                bsg_cuda_print_stat_end(0);

                bsg_tile_group_barrier(&r_barrier, &c_barrier);

                bsg_cuda_print_stat_kernel_end();
                return rc;
        }
}
