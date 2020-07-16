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
int  __attribute__ ((noinline)) matrix_multiply_group_v17(TA *A, TB *B, TC *C, // matrix multiplication for v3, v3 implement the data distribution along column side, without scratchpad
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
  
  int block_size = A_HEIGHT / BSG_TILE_GROUP_X_DIM;

  bsg_tile_group_shared_mem (TA, sh_A11, (block_size * block_size));
  bsg_tile_group_shared_mem (TA, sh_A12, (block_size * block_size));
  bsg_tile_group_shared_mem (TA, sh_A21, (block_size * block_size));
  bsg_tile_group_shared_mem (TA, sh_A22, (block_size * block_size));

  bsg_tile_group_shared_mem (TA, sh_B11, (block_size * block_size));
  bsg_tile_group_shared_mem (TA, sh_B12, (block_size * block_size));
  bsg_tile_group_shared_mem (TA, sh_B21, (block_size * block_size));
  bsg_tile_group_shared_mem (TA, sh_B22, (block_size * block_size));

  int8_t C_local[block_size * block_size]; // store the temporal product
  
  int cpy_row_idx = 0;
  int cpy_col_idx = 0;

  //copy matrix A11 to shared memory
  for (int row_A11_id = 0; row_A11_id < block_size; row_A11_id ++){
        cpy_col_idx = 0;
        for (int col_A11_id = 0; col_A11_id < block_size; col_A11_id ++){
              bsg_tile_group_shared_store (TA, sh_A11, (cpy_row_idx * block_size + cpy_col_idx), A[row_A11_id * A_WIDTH + col_A11_id]);
              cpy_col_idx ++;
        }
        cpy_row_idx ++;
  }

  cpy_row_idx = 0;
  cpy_col_idx = 0;

  //copy matrix A12 to shared memory
  for (int row_A12_id = 0; row_A12_id < block_size; row_A12_id ++){
        cpy_col_idx = 0;
        for (int col_A12_id = block_size; col_A12_id < A_WIDTH; col_A12_id ++){
              bsg_tile_group_shared_store (TA, sh_A12, (cpy_row_idx * block_size + cpy_col_idx), A[row_A12_id * A_WIDTH + col_A12_id]);
              cpy_col_idx ++;
        }
        cpy_row_idx ++;
  }

  cpy_row_idx = 0;
  cpy_col_idx = 0;

  //copy matrix A21 to shared memory
  for (int row_A21_id = block_size; row_A21_id < A_HEIGHT; row_A21_id ++){
        cpy_col_idx = 0;
        for (int col_A21_id = 0; col_A21_id < block_size; col_A21_id ++){
              bsg_tile_group_shared_store (TA, sh_A21, (cpy_row_idx * block_size + cpy_col_idx), A[row_A21_id * A_WIDTH + col_A21_id]);
              cpy_col_idx ++;
        }
        cpy_row_idx ++;
  }

  cpy_row_idx = 0;
  cpy_col_idx = 0;

  //copy matrix A22 to shared memory
  for (int row_A22_id = block_size; row_A22_id < A_HEIGHT; row_A22_id ++){
        cpy_col_idx = 0;
        for (int col_A22_id = block_size; col_A22_id < A_WIDTH; col_A22_id ++){
              bsg_tile_group_shared_store (TA, sh_A22, (cpy_row_idx * block_size + cpy_col_idx), A[row_A22_id * A_WIDTH + col_A22_id]);
              cpy_col_idx ++;
        }
        cpy_row_idx ++;
  }

  cpy_row_idx = 0;
  cpy_col_idx = 0;

  //copy matrix B11 to shared memory
  for (int row_B11_id = 0; row_B11_id < block_size; row_B11_id ++){
        cpy_col_idx = 0;
        for (int col_B11_id = 0; col_B11_id < block_size; col_B11_id ++){
              bsg_tile_group_shared_store (TA, sh_B11, (cpy_row_idx * block_size + cpy_col_idx), B[row_B11_id * A_WIDTH + col_B11_id]);
              cpy_col_idx ++;
        }
        cpy_row_idx ++;
  }

  cpy_row_idx = 0;
  cpy_col_idx = 0;

  //copy matrix B12 to shared memory
  for (int row_B12_id = 0; row_B12_id < block_size; row_B12_id ++){
        cpy_col_idx = 0;
        for (int col_B12_id = block_size; col_B12_id < A_WIDTH; col_B12_id ++){
              bsg_tile_group_shared_store (TA, sh_B12, (cpy_row_idx * block_size + cpy_col_idx), B[row_B12_id * A_WIDTH + col_B12_id]);
              cpy_col_idx ++;
        }
        cpy_row_idx ++;
  }

  cpy_row_idx = 0;
  cpy_col_idx = 0;

  //copy matrix B21 to shared memory
  for (int row_B21_id = block_size; row_B21_id < A_HEIGHT; row_B21_id ++){
        cpy_col_idx = 0;
        for (int col_B21_id = 0; col_B21_id < block_size; col_B21_id ++){
              bsg_tile_group_shared_store (TA, sh_B21, (cpy_row_idx * block_size + cpy_col_idx), B[row_B21_id * A_WIDTH + col_B21_id]);
              cpy_col_idx ++;
        }
        cpy_row_idx ++;
  }

  cpy_row_idx = 0;
  cpy_col_idx = 0;

  //copy matrix B22 to shared memory
  for (int row_B22_id = block_size; row_B22_id < A_HEIGHT; row_B22_id ++){
        cpy_col_idx = 0;
        for (int col_B22_id = block_size; col_B22_id < A_WIDTH; col_B22_id ++){
              bsg_tile_group_shared_store (TA, sh_B22, (cpy_row_idx * block_size + cpy_col_idx), B[row_B22_id * A_WIDTH + col_B22_id]);
              cpy_col_idx ++;
        }
        cpy_row_idx ++;
  }

  bsg_cuda_print_stat_kernel_start();
  bsg_cuda_print_stat_start(0);
  bsg_tile_group_barrier (&r_barrier, &c_barrier);

  //perform stage 1 multiplication
  TA tmp_A;
  TB tmp_B;
  
  if(tid == 0){
        for (int A_local_row_id = 0; A_local_row_id < block_size; A_local_row_id ++) {
            for (uint32_t B_local_row_id = 0; B_local_row_id < block_size; B_local_row_id ++){
                  sum = static_cast<TC>(0);
                  for (uint32_t k = 0; k < block_size; k ++){
                        bsg_tile_group_shared_load (TA, sh_A11, (A_local_row_id * block_size + k), tmp_A);
                        bsg_tile_group_shared_load (TA, sh_B11, (B_local_row_id * block_size + k), tmp_B);
                        sum += tmp_A * tmp_B;
                  }
                  C_local[A_local_row_id * block_size + B_local_row_id] = sum;
            }
      }
  }else if(tid == 1){
        for (int A_local_row_id = 0; A_local_row_id < block_size; A_local_row_id ++) {
            for (uint32_t B_local_row_id = 0; B_local_row_id < block_size; B_local_row_id ++){
                  sum = static_cast<TC>(0);
                  for (uint32_t k = 0; k < block_size; k ++){
                        bsg_tile_group_shared_load (TA, sh_A12, (A_local_row_id * block_size + k), tmp_A);
                        bsg_tile_group_shared_load (TA, sh_B22, (B_local_row_id * block_size + k), tmp_B);
                        sum += tmp_A * tmp_B;
                  }
                  C_local[A_local_row_id * block_size + B_local_row_id] = sum;
            }
      }
  }else if(tid == 2){
        for (int A_local_row_id = 0; A_local_row_id < block_size; A_local_row_id ++) {
            for (uint32_t B_local_row_id = 0; B_local_row_id < block_size; B_local_row_id ++){
                  sum = static_cast<TC>(0);
                  for (uint32_t k = 0; k < block_size; k ++){
                        bsg_tile_group_shared_load (TA, sh_A21, (A_local_row_id * block_size + k), tmp_A);
                        bsg_tile_group_shared_load (TA, sh_B11, (B_local_row_id * block_size + k), tmp_B);
                        sum += tmp_A * tmp_B;
                  }
                  C_local[A_local_row_id * block_size + B_local_row_id] = sum;
            }
      }
  }else {
        for (int A_local_row_id = 0; A_local_row_id < block_size; A_local_row_id ++) {
            for (uint32_t B_local_row_id = 0; B_local_row_id < block_size; B_local_row_id ++){
                  sum = static_cast<TC>(0);
                  for (uint32_t k = 0; k < block_size; k ++){
                        bsg_tile_group_shared_load (TA, sh_A22, (A_local_row_id * block_size + k), tmp_A);
                        bsg_tile_group_shared_load (TA, sh_B22, (B_local_row_id * block_size + k), tmp_B);
                        sum += tmp_A * tmp_B;
                  }
                  C_local[A_local_row_id * block_size + B_local_row_id] = sum;
            }
      }
  }
  
  //perform stage 2 multiplication
  if(tid == 0){
        for (int A_local_row_id = 0; A_local_row_id < block_size; A_local_row_id ++) {
            for (uint32_t B_local_row_id = 0; B_local_row_id < block_size; B_local_row_id ++){
                  sum = C_local[A_local_row_id * block_size + B_local_row_id];
                  for (uint32_t k = 0; k < block_size; k ++){
                        bsg_tile_group_shared_load (TA, sh_A12, (A_local_row_id * block_size + k), tmp_A);
                        bsg_tile_group_shared_load (TA, sh_B12, (B_local_row_id * block_size + k), tmp_B);
                        sum += tmp_A * tmp_B;
                  }
                  C_local[A_local_row_id * block_size + B_local_row_id] = sum;
            }
      }
  }else if(tid == 1){
        for (int A_local_row_id = 0; A_local_row_id < block_size; A_local_row_id ++) {
            for (uint32_t B_local_row_id = 0; B_local_row_id < block_size; B_local_row_id ++){
                  sum = C_local[A_local_row_id * block_size + B_local_row_id];
                  for (uint32_t k = 0; k < block_size; k ++){
                        bsg_tile_group_shared_load (TA, sh_A11, (A_local_row_id * block_size + k), tmp_A);
                        bsg_tile_group_shared_load (TA, sh_B21, (B_local_row_id * block_size + k), tmp_B);
                        sum += tmp_A * tmp_B;
                  }
                  C_local[A_local_row_id * block_size + B_local_row_id] = sum;
            }
      }
  }else if(tid == 2){
        for (int A_local_row_id = 0; A_local_row_id < block_size; A_local_row_id ++) {
            for (uint32_t B_local_row_id = 0; B_local_row_id < block_size; B_local_row_id ++){
                  sum = C_local[A_local_row_id * block_size + B_local_row_id];
                  for (uint32_t k = 0; k < block_size; k ++){
                        bsg_tile_group_shared_load (TA, sh_A22, (A_local_row_id * block_size + k), tmp_A);
                        bsg_tile_group_shared_load (TA, sh_B12, (B_local_row_id * block_size + k), tmp_B);
                        sum += tmp_A * tmp_B;
                  }
                  C_local[A_local_row_id * block_size + B_local_row_id] = sum;
            }
      }
  }else {
        for (int A_local_row_id = 0; A_local_row_id < block_size; A_local_row_id ++) {
            for (uint32_t B_local_row_id = 0; B_local_row_id < block_size; B_local_row_id ++){
                  sum = C_local[A_local_row_id * block_size + B_local_row_id];
                  for (uint32_t k = 0; k < block_size; k ++){
                        bsg_tile_group_shared_load (TA, sh_A21, (A_local_row_id * block_size + k), tmp_A);
                        bsg_tile_group_shared_load (TA, sh_B21, (B_local_row_id * block_size + k), tmp_B);
                        sum += tmp_A * tmp_B;
                  }
                  C_local[A_local_row_id * block_size + B_local_row_id] = sum;
            }
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

extern "C" {
        int  __attribute__ ((noinline)) kernel_matrix_multiply_int8(
                      int8_t *A, int8_t *B, int8_t *C,
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
                rc = matrix_multiply_group_v17(A, B, C,
                                           A_HEIGHT, A_WIDTH, B_WIDTH
                                           );
                bsg_cuda_print_stat_end(0);

                bsg_tile_group_barrier(&r_barrier, &c_barrier);

                bsg_cuda_print_stat_kernel_end();
                return rc;
        }
}
