/*
 * This kernel performs smdm.
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
INIT_TILE_GROUP_BARRIER(r_barrier, c_barrier, 0, bsg_tiles_X-1, 0, bsg_tiles_Y-1);

/////////////////////////////////////////////////
// CSR sparse matrix data structure and utilities
/////////////////////////////////////////////////
typedef float data_t;

typedef struct sparse_mat_t {
  // number of non-zero
  int nnz;

  // the dimensions of the matrix
  // m is x, n is y
  int m;
  int n;

  // the non-zero values in the matrix
  data_t *nz_data;

  // y(n) + 1 size, the number of non-zero elements in the ALL of previous rows
  // this is used for lookup into into a row
  int *idx_ptrs;

  // nnz size, the column of the correspnding non-zero value
  int *nz_cols;

} __attribute__((packed)) sparse_mat_t;

void sparse_mat_set_nz_val(sparse_mat_t *mat, int idx, data_t val) {
  (mat->nz_data)[idx] = val;
}

int sparse_mat_get_col(sparse_mat_t *mat, int idx) {
  return mat->nz_cols[idx];
}

int sparse_mat_get_idx_ptr(sparse_mat_t *mat, int row) {
  return mat->idx_ptrs[row];
}

data_t sparse_mat_get_val(sparse_mat_t *mat, int idx) {
  return mat->nz_data[idx];
}

extern int bsg_printf(const char*, ...);

/* We wrap all external-facing C++ kernels with `extern "C"` to
 * prevent name mangling
 */
extern "C" {

// A: m by n, CSR
// B: n by k, CSR
// Out: m by n, dense
int  __attribute__ ((noinline)) kernel_sm_dm(sparse_mat_t *A,
                                              data_t *B,
                                              data_t *Out,
                                              int A_Height,
                                              int A_Width,
                                              int B_Width) {
  // get the absolute tid (which group then which tid within that group)
  int tid_x = __bsg_tile_group_id_x * BSG_TILE_GROUP_X_DIM + __bsg_x;
  int tid_y = __bsg_tile_group_id_y * BSG_TILE_GROUP_Y_DIM + __bsg_y;
  int num_threads_x = BSG_TILE_GROUP_X_DIM * __bsg_grid_dim_x;
  int num_threads_y = BSG_TILE_GROUP_Y_DIM * __bsg_grid_dim_y;

  int num_threads = num_threads_x * num_threads_y;
  // assume m % num = 0
  //float num_col_B_per_thread = n / num_threads;
  int tid = tid_y * num_threads_x + tid_x;

  int B_Height = A_Width;

  //int tid = __bsg_x + __bsg_y * BSG_TILE_GROUP_X_DIM + __bsg_tile_group_id * __bsg_grid_dim_x * __bsg_grid_dim_y;
  bsg_cuda_print_stat_kernel_start();

  int block_row_size = 2;  // need to be set according to the matrix size, matrix size must be divided by the block row size
  int block_col_size = 250;  // need to be set according to the matrix size, matrix size must be divided by the block col size

  // calculate the number of row and col blocks in matrix A and matrix B
  int A_num_blk_per_row = (A_Width + block_col_size - 1) / block_col_size;
  int A_num_blk_per_col = (A_Height + block_row_size - 1) / block_row_size;
  int B_num_blk_per_row = (B_Width + block_row_size - 1) / block_row_size;
  int B_num_blk_per_col = (B_Height + block_col_size - 1) / block_col_size;

  // calculate the dimension of the last block size in each matrix
  int A_last_blk_col = (A_Width % block_col_size == 0) ? block_col_size : (A_Width % block_col_size);
  int A_last_blk_row = (A_Height % block_row_size == 0) ? block_row_size : (A_Height % block_row_size);
  int B_last_blk_col = (B_Width % block_row_size == 0) ? block_row_size : (B_Width % block_row_size);
  int B_last_blk_row = (B_Height % block_col_size == 0) ? block_col_size : (B_Height % block_col_size);

  int res_row = 0;
  int res_col = 0;

  // iterate over blocks
  for (int A_blk_row_id = tid; A_blk_row_id < A_num_blk_per_col; A_blk_row_id += num_threads){
    for (int B_blk_col_id = 0; B_blk_col_id < B_num_blk_per_row; B_num_blk_per_row ++){
      res_row = (A_blk_row_id == A_num_blk_per_col - 1) ? A_last_blk_row : block_row_size;
      res_col = (B_blk_col_id == B_num_blk_per_row - 1) ? B_last_blk_col : block_col_size;

      float Res_local[res_row * res_col];
      memset(Res_local, 0, res_row * res_col * sizeof(float));
      
    }
  }

  bsg_tile_group_barrier(&r_barrier, &c_barrier);
  bsg_cuda_print_stat_kernel_end();
  
  return 0;
}

} // extern "C"
