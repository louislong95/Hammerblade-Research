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
#include <cstring>

#include <matrix_multiply.hpp>

INIT_TILE_GROUP_BARRIER(r_barrier, c_barrier,
                        0, BSG_TILE_GROUP_X_DIM-1,
                        0, BSG_TILE_GROUP_Y_DIM-1);


extern "C" {
        int  __attribute__ ((noinline)) kernel_matrix_multiply_float(
                      float *A, float *B, float *C,
                      uint32_t A_HEIGHT, uint32_t A_WIDTH, uint32_t B_WIDTH) {
                int rc;

                // make use of the local memory in the tile, copy the matrix into the local memory in the tiles
                // inital the memory space
                //int8_t A_local[A_HEIGHT * A_WIDTH];
                //int8_t B_local[A_WIDTH * B_WIDTH];
                //int8_t C_local[A_HEIGHT * B_WIDTH];
                // copy data
                //memcpy (A_local, A, sizeof(A[0])*A_HEIGHT*A_WIDTH);
                //memcpy (B_local, B, sizeof(B[0])*A_WIDTH*B_WIDTH);

                bsg_cuda_print_stat_kernel_start();
                bsg_cuda_print_stat_start(0);
                rc = matrix_multiply_group_v8(A, B, C,
                                           A_HEIGHT, A_WIDTH, B_WIDTH
                                           );
                bsg_cuda_print_stat_end(0);

                bsg_tile_group_barrier(&r_barrier, &c_barrier);

                bsg_cuda_print_stat_kernel_end();
                return rc;
        }
}
