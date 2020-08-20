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

  data_t sum;
  data_t data_B_row[A_Width];
  //int tid = __bsg_x + __bsg_y * BSG_TILE_GROUP_X_DIM + __bsg_tile_group_id * __bsg_grid_dim_x * __bsg_grid_dim_y;
  bsg_cuda_print_stat_kernel_start();

  for (int row_B_id = tid; row_B_id < B_Width; row_B_id += num_threads){
    memcpy(data_B_row, &B[row_B_id * A_Width], sizeof(data_t)*A_Width); 
    for (int row_A_id = 0; row_A_id < A_Height; row_A_id ++){
      sum = 0;
      for (int i = sparse_mat_get_idx_ptr(A,row_A_id); i < sparse_mat_get_idx_ptr(A,row_A_id + 1); i++) {
        int col_A_id = sparse_mat_get_col(A,i);
        sum += sparse_mat_get_val(A, i) * data_B_row[col_A_id];
      }
      Out[row_A_id * B_Width + row_B_id] = sum;
    }
  }
  bsg_tile_group_barrier(&r_barrier, &c_barrier);
  bsg_cuda_print_stat_kernel_end();
  
  return 0;
}

} // extern "C"
