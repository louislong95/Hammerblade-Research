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

#include <v24_function.hpp>

INIT_TILE_GROUP_BARRIER(r_barrier, c_barrier,
                        0, BSG_TILE_GROUP_X_DIM-1,
                        0, BSG_TILE_GROUP_Y_DIM-1);


extern "C" {
        int  __attribute__ ((noinline)) kernel_matrix_multiply_float(
                      float *A, float *B, float *C,
                      uint32_t A_HEIGHT, uint32_t A_WIDTH, uint32_t B_WIDTH, uint32_t NUM_REGISTER) {
                int rc;
                uint32_t B_HEIGHT = A_WIDTH;
                int num_reg = NUM_REGISTER;

                //calculate the greatest common divisor of (B_WIDTH/16) and A_HEIGHT to determine the block_size_row
                int num_tile = BSG_TILE_GROUP_X_DIM * BSG_TILE_GROUP_Y_DIM;
                int extra_blk_row = B_WIDTH % num_tile;
                int self_blk_row_per_tile = B_WIDTH / num_tile;
                int block_row = 0; // used as the real block row
                int block_size_row = 0;
                double flag_8 = 0;
                double flag_4 = 0;
                if(extra_blk_row == 0){
                        block_size_row = gcd( self_blk_row_per_tile, A_HEIGHT);
                        if(block_size_row != 1){
                                if(block_size_row == self_blk_row_per_tile){
                                        block_row = self_blk_row_per_tile;
                                }
                                else {  //block_size_row != self_blk_row_per_tile
                                        if(self_blk_row_per_tile % 8 == 0){
                                                block_row = 8;
                                        }
                                        else {
                                                flag_8 = (self_blk_row_per_tile / 8 + 1) * 8 * num_tile / B_WIDTH;
                                                flag_4 = (self_blk_row_per_tile / 4 + 1) * 4 * num_tile / B_WIDTH;
                                                if(flag_8 < 1.3){ // 1.3 need to be more precious with more experiment 
                                                        block_row = 8;
                                                }
                                                else if(flag_4 < 1.3){
                                                        block_row = 4;
                                                }
                                                else{
                                                        block_row = block_size_row;
                                                }
                                        }
                                }
                        }
                        else {  // block_size_row == 1
                                if(B_WIDTH == num_tile){
                                        block_row = 1;
                                }
                                else {  // B_width ! = num_tile
                                        if(B_WIDTH > 500){
                                                block_row = 8;
                                        }
                                        else {
                                                block_row = 4;
                                        }
                                }
                        }
 
                }
                else {
                        if(B_WIDTH > 500){
                                block_row = 8; // to change
                        }
                        else{
                                block_row = 4; // to change
                        }
                }

                bsg_cuda_print_stat_kernel_start();
                bsg_cuda_print_stat_start(0);
                if ( A_WIDTH < 2){              // if the matrix size is not so large, select the row by row multiplication
                        rc = matrix_multiply_group_row_mul(A, B, C,
                                A_HEIGHT, A_WIDTH, B_WIDTH);
                }else{                            // if the matrix size is large, select the v20 function
                        rc = matrix_multiply_group_v24(A, B, C,
                                A_HEIGHT, A_WIDTH, B_HEIGHT, B_WIDTH, block_row, num_reg);
                }
                
                bsg_cuda_print_stat_end(0);

                bsg_tile_group_barrier(&r_barrier, &c_barrier);

                bsg_cuda_print_stat_kernel_end();
                return rc;
        }
}
