/*
 * This kernel performs matrix multiplication.
 */

// BSG_TILE_GROUP_X_DIM and BSG_TILE_GROUP_Y_DIM must be defined
// before bsg_manycore.h and bsg_tile_group_barrier.h are
// included. bsg_tiles_X and bsg_tiles_Y must also be defined for
// legacy reasons, but they are deprecated.
#define BSG_TILE_GROUP_X_DIM 4
#define BSG_TILE_GROUP_Y_DIM 4
#define bsg_tiles_X BSG_TILE_GROUP_X_DIM
#define bsg_tiles_Y BSG_TILE_GROUP_Y_DIM
#include <bsg_manycore.h>
#include <bsg_tile_group_barrier.h>
#include <cstdint>
#include <cstring>

#include <v20_function.hpp>

INIT_TILE_GROUP_BARRIER(r_barrier, c_barrier,
                        0, BSG_TILE_GROUP_X_DIM-1,
                        0, BSG_TILE_GROUP_Y_DIM-1);


extern "C" {
        int  __attribute__ ((noinline)) kernel_matrix_multiply_float(
                      float *A, float *B, float *C,
                      uint32_t A_HEIGHT, uint32_t A_WIDTH, uint32_t B_WIDTH) {
                int rc;
                uint32_t B_HEIGHT = A_WIDTH;
                
                //calculate the greatest common divisor of (B_WIDTH/16) and A_HEIGHT to determine the block_size_row
                int num_tile = BSG_TILE_GROUP_X_DIM * BSG_TILE_GROUP_Y_DIM;
                int B_block_num = (B_WIDTH % num_tile == 0) ? (B_WIDTH / num_tile) : 1;
                int block_size_row = gcd( B_block_num, A_HEIGHT);
                if (block_size_row > 18){
                        block_size_row = 1;
                }
                int block_size_col = (A_WIDTH % 9 == 0) ? 9 : (A_WIDTH % 8 == 0) ? 8 : (A_WIDTH % 7 == 0) ? 7 : \
                                        (A_WIDTH % 6 == 0) ? 6 : (A_WIDTH % 5 == 0) ? 5 : (A_WIDTH % 4 == 0) ? 4 :  \
                                        (A_WIDTH % 3 == 0) ? 3 : (A_WIDTH % 2 == 0) ? 2 : 1;
       
                bsg_cuda_print_stat_kernel_start();
                bsg_cuda_print_stat_start(0);
                rc = matrix_multiply_group_v20(A, B, C,
                                           A_HEIGHT, A_WIDTH, B_HEIGHT, B_WIDTH,
                                           block_size_row, block_size_col);
                bsg_cuda_print_stat_end(0);

                bsg_tile_group_barrier(&r_barrier, &c_barrier);

                bsg_cuda_print_stat_kernel_end();
                return rc;
        }
}
