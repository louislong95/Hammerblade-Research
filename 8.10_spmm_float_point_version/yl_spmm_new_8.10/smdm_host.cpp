#include "smdm_host.hpp"
#include <string>

// All matrices are square
#define A_HEIGHT    10
#define A_WIDTH     500
#define B_WIDTH     16
#define OUT_WIDTH   B_WIDTH
#define NNZ_PER_ROW 150

// Print matrix A (M x N).10
template <typename T>
void matrix_print(T *A, uint64_t M, uint64_t N) {
    //int sum = 0;
    for (uint64_t y = 0; y < M; y ++) {
        for (uint64_t x = 0; x < N; x ++) {
            //std::cout << A[y * N + x] << " ";
            printf("%f ", A[y * N + x]);
        }
        //std::cout << '\n';
        printf("\n");
    }
}

/////////////////////////////////////////////////
// CSR sparse matrix data structure and utilities
/////////////////////////////////////////////////
typedef float data_t;

using namespace std;

typedef struct sparse_mat_t {
  // number of non-zero
  int nnz;

  // the dimensions of the matrix
  // m is y, n is x
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


/////////////////////////////////////////////////
// sparse matrix utilities on host
/////////////////////////////////////////////////
sparse_mat_t *create_from_sparse(data_t *nz_data, int *idx_ptrs,
                                 int *nz_cols, int m, int n, int nnz) {
  sparse_mat_t *mat = (sparse_mat_t*)malloc(sizeof(sparse_mat_t));
  mat->m = m;
  mat->n = n;
  mat->nnz = nnz;
  // hardcopy all of the arrays
  mat->idx_ptrs = (int*)malloc(sizeof(int) * m + 1);
  for (int i = 0; i < m + 1; i++) {
    mat->idx_ptrs[i] = idx_ptrs[i];
  }
  mat->nz_cols = (int*)malloc(sizeof(int) * nnz);
  mat->nz_data = (data_t*)malloc(sizeof(data_t) * nnz);
  for (int i = 0; i < nnz; i++) {
    mat->nz_cols[i] = nz_cols[i];
    mat->nz_data[i] = (data_t)nz_data[i];
  }
  return mat;
}

void destroy_sparse_mat(sparse_mat_t *mat) {
  free(mat->idx_ptrs);
  free(mat->nz_cols);
  free(mat->nz_data);
  free(mat);
}

int size_of_device_fields() {
  return sizeof(uint32_t) * 6;
}

// ret a pointer to the struct
eva_t sparse_mat_memcpy_to(hb_mc_device_t *device, sparse_mat_t *mat) {
  int err;
  // malloc the backing arrays
  eva_t nz_data_dev, nz_cols_dev, idx_ptrs_dev;
  const int num_bytes_data = sizeof(uint32_t) * mat->nnz; 
  const int num_bytes_cols = num_bytes_data;
  const int num_bytes_ptrs = sizeof(uint32_t) * (mat->m + 1);
  err  = hb_mc_device_malloc(device, num_bytes_data, &nz_data_dev);
  err |= hb_mc_device_malloc(device, num_bytes_cols, &nz_cols_dev);
  err |= hb_mc_device_malloc(device, num_bytes_ptrs, &idx_ptrs_dev);
  // copy the backing arrays to the device
  /*
  err |= hb_mc_device_memcpy (device, (void*)((intptr_t)nz_data_dev), (void*)(mat->nz_data),
                        num_bytes_data, HB_MC_MEMCPY_TO_DEVICE);
  err |= hb_mc_device_memcpy (device, (void*)((intptr_t)nz_cols_dev), (void*)(mat->nz_cols),
                        num_bytes_cols, HB_MC_MEMCPY_TO_DEVICE);
  err |= hb_mc_device_memcpy (device, (void*)((intptr_t)idx_ptrs_dev), (void*)(mat->idx_ptrs),
                        num_bytes_ptrs, HB_MC_MEMCPY_TO_DEVICE);
  */
  printf("start copying sparse matrix to devide\n");
  hb_mc_dma_htod_t nz_data_dma = {nz_data_dev, (void*)(mat->nz_data), num_bytes_data};
  hb_mc_dma_htod_t nz_cols_dma = {nz_cols_dev, (void*)(mat->nz_cols), num_bytes_cols};
  hb_mc_dma_htod_t idx_ptrs_dma = {idx_ptrs_dev, (void*)(mat->idx_ptrs), num_bytes_ptrs};
  err |= hb_mc_device_dma_to_device(device, &nz_data_dma, 1);
  err |= hb_mc_device_dma_to_device(device, &nz_cols_dma, 1);
  err |= hb_mc_device_dma_to_device(device, &idx_ptrs_dma, 1);
  // set the pointers in the struct
  // explicitly assign pointer values to the struct
  int mat_fields[6];
  mat_fields[0] = mat->nnz;
  mat_fields[1] = mat->m; //row
  mat_fields[2] = mat->n; //column
  mat_fields[3] = nz_data_dev;
  mat_fields[4] = idx_ptrs_dev;
  mat_fields[5] = nz_cols_dev;
  // allocate and copy the struct to the device
  eva_t struct_dev;
  const int num_bytes_struct = size_of_device_fields();
  err |= hb_mc_device_malloc(device, num_bytes_struct, &struct_dev);
  /*
  err |= hb_mc_device_memcpy (device, (void*)((intptr_t)struct_dev), (void*)(mat_fields),
                        num_bytes_struct, HB_MC_MEMCPY_TO_DEVICE);
  if (err != HB_MC_SUCCESS) {
    printf("ERROR: failed to copy sparse mat to device\n");
  }
  */
  hb_mc_dma_htod_t struct_dev_dma = {struct_dev, (void*)(mat_fields), num_bytes_struct};
  err |= hb_mc_device_dma_to_device(device, &struct_dev_dma, 1);

  if (err != HB_MC_SUCCESS) {
    printf("ERROR: failed to copy sparse mat to device\n");
  }

  return struct_dev;
}

void sparse_mat_memcpy_from(hb_mc_device_t *device, eva_t struct_dev, sparse_mat_t *mat) {
  int err;
  const int num_bytes_struct = size_of_device_fields(); 
  int mat_fields[6];
  err = hb_mc_device_memcpy(device, (void*)(mat_fields), (void*)((intptr_t)struct_dev),
                            num_bytes_struct, HB_MC_MEMCPY_TO_HOST);
  mat->nnz = mat_fields[0];
  mat->m   = mat_fields[1];
  mat->n   = mat_fields[2];
  eva_t nz_data_dev = mat_fields[3];
  eva_t idx_ptrs_dev = mat_fields[4];
  eva_t nz_cols_dev = mat_fields[5];
  // track down the data arrays
  const int num_bytes_data = sizeof(uint32_t) * mat->nnz;
  const int num_bytes_cols = num_bytes_data;
  const int num_bytes_ptrs = sizeof(uint32_t) * (mat->m + 1);
  mat->nz_data = (data_t*)malloc(num_bytes_data);
  mat->nz_cols = (int*)malloc(num_bytes_cols);
  mat->idx_ptrs = (int*)malloc(num_bytes_ptrs);
  // copy the backing arrays to the host
  err |= hb_mc_device_memcpy (device, (void*)(mat->nz_data), (void*)((intptr_t)nz_data_dev),
                         num_bytes_data, HB_MC_MEMCPY_TO_HOST);
  err |= hb_mc_device_memcpy (device, (void*)(mat->nz_cols), (void*)((intptr_t)nz_cols_dev),
                        num_bytes_cols, HB_MC_MEMCPY_TO_HOST);
  err |= hb_mc_device_memcpy (device, (void*)(mat->idx_ptrs), (void*)((intptr_t)idx_ptrs_dev),
                        num_bytes_ptrs, HB_MC_MEMCPY_TO_HOST);
  if (err != HB_MC_SUCCESS) {
    printf("ERROR: failed to copy sparse mat to host\n");
  }
  
}

// Host spgemm (to compare results)
void smdm_reference(sparse_mat_t *A, data_t *B, data_t *Out, int M, int N, int K) { // M is y of A, N is x of A, K is x f B
  for (int row_A_id = 0; row_A_id < M; row_A_id++) {
    for (int j = 0; j < K; j++) {
      Out[row_A_id * K + j] = 0;
    }
  }
  for (int row_A_id = 0; row_A_id < M; row_A_id++) {
    for (int col_B_id = 0; col_B_id < K; col_B_id ++){
      for (int i = sparse_mat_get_idx_ptr(A, row_A_id); i < sparse_mat_get_idx_ptr(A, row_A_id + 1); i++){
        data_t elem_A = sparse_mat_get_val(A, i);
        int row_B_id = sparse_mat_get_col(A, i);
        data_t elem_B = B[row_B_id * K + col_B_id];
        Out[row_A_id * K + col_B_id] += elem_A * elem_B;
      }
    }
  }
}

// Compute the sum of squared error between matrix A and B (M x N)
double check(const data_t *A, const data_t *B, int M, int N) {
  double sum = 0;
  for (int x = 0; x < M * N; x++) {
    //cout << x << endl; 
    //cout << A[x] << endl; 
    //cout << B[x] << endl;
    data_t diff = A[x] - B[x];
    if(std::isnan(diff)){
        return diff;
    }
    sum += diff * diff;
  }
  return sum;
}

//sparse_mat_t *read_sparse_matrix(string filename) {

sparse_mat_t *read_sparse_matrix(string filename) {
  printf("start reading the sparse matrix\n");
  int m = 0, k = 0;
  int nnz = 0;

  int section = 0;
  string line;
  ifstream rf(filename);
  //rf.open("testcsr.txt");
  //printf("filename read check = %s\n", filename);
  if (rf.is_open()){
    while (getline (rf, line)) {
      if(line.compare("shape:") == 0){
	      getline (rf, line);
	      string token = line.substr(1, line.length() - 2);
	      m = stoi(token.substr(0, token.find(", ")));
        printf("the value of m = %d\n", m);
	      k = stoi(token.substr(token.find(", ") + 2, token.length() - (token.find(", ") + 2)));
        printf("the value of k = %d\n", k);
      }
      if(line.compare("nnz:") == 0){
	      getline (rf, line);
	      nnz = stoi(line);
        printf("the value of nnz = %d\n", nnz);
	      break;
      }
    }
  }
  else{
    printf("cannot open file\n");
  }
  
  int idx_ptrs[m + 1];
  data_t nz_data[nnz];
  int nz_cols[nnz];
  
  while (getline (rf, line)) {
    if(line.compare("indptr:") == 0){
      int idx = 0;
      while(idx < m + 1){
	      getline (rf, line);
	      idx_ptrs[idx] = stoi(line);
	      idx++;
      }
    }
    if(line.compare("indices:") == 0){
      int idx = 0;
      while(idx < nnz){
	      getline (rf, line);
	      nz_cols[idx] = stoi(line);
	      idx++;
      }
    }
    if(line.compare("values:") == 0){
      int idx = 0;
      while(idx < nnz){
	      getline (rf, line);
	      nz_data[idx] = stof(line);
	      idx++;
      }
    }
  }

  // Close the file
  rf.close();
  return create_from_sparse(nz_data, idx_ptrs, nz_cols, m, k, nnz);
}


void matrix_transpose (data_t *A, data_t *B, int M, int N) {
  for (int y = 0; y < M; y ++) {
    for (int x = 0; x < N; x ++) {
      B[x * M + y] = A[y * N + x];
    }
  }
}

int test_smdm(int argc, char *argv[]) {
  char *bin_path, *test_name;
  struct arguments_path args = {NULL, NULL};
  argp_parse(&argp_path, argc, argv, 0, 0, &args);
  bin_path = args.path;
  test_name = args.name;

  printf("Running sparse matrix dense matrix.\n\n");
  int err;
  
  // generate the custom sparse matrix using the generator (begin here)
  /*
  int m = A_HEIGHT, n = A_WIDTH, k = B_WIDTH;
  int nnz_per_row = NNZ_PER_ROW;
  int nnz = m * nnz_per_row;
  int idx_ptrs[m + 1];
  data_t nz_data[nnz];
  int nz_cols[nnz];
  for (int i = 0; i < m + 1; i++) {
    idx_ptrs[i] = i * nnz_per_row;
  }
  for (int i = 0; i < nnz; i++) {
    nz_data[i] = (float)(rand()%100 + 200) / 100.0f;
    nz_cols[i] = i % nnz_per_row;
  }
  
  sparse_mat_t *sparse_matrix = create_from_sparse(nz_data, idx_ptrs, nz_cols, m, n, nnz);
  */
  // generate the custom sparse matrix using the generator (end here)

  // read in the sparse matrix txt file (begin here)
  int m = 0, n = 0, k = 0;
  sparse_mat_t *sparse_matrix = read_sparse_matrix("testcsr.txt");

  m = sparse_matrix->m;
  n = sparse_matrix->n;
  k = B_WIDTH;

  // read in the sparse matrix txt file (end here)
  

  std::numeric_limits<int8_t> lim;
  std::default_random_engine generator;
  generator.seed(1);
  std::uniform_real_distribution<float> distribution(lim.min(),lim.max());
  data_t *dense_matrix;
  data_t dense_data_matrix[n * k];
  data_t out_matrix_input[m * k];
  auto res = distribution(generator);

  for (uint64_t i = 0; i < n * k; i++) {
    do{
      res = distribution(generator);
    }while(!std::isnormal(res) ||
	   !std::isfinite(res) ||
	   std::isnan(res));
    
    dense_data_matrix[i] = static_cast<data_t>(res);
    //printf("dense value = %f\n", res);
  }  
  
  data_t Out_reference[m * k];
  // Generate the known-correct results on the host
  smdm_reference(sparse_matrix, dense_data_matrix, Out_reference, m, n, k);

  data_t dense_tran_matrix[k * n];
  if(!strcmp("v0", test_name) || !strcmp("v1", test_name)){
    matrix_transpose(dense_data_matrix,dense_tran_matrix, n, k);
    dense_matrix = dense_tran_matrix;
  }else{
    dense_matrix = dense_data_matrix; // v2 v3 no tranposed
  }

  for (int i = 0; i < m * k; i++){
    out_matrix_input[i] = 0;
  }
  //sparse_mat_t *sparse_matrix = create_from_sparse(nz_data, idx_ptrs, nz_cols, m, k, nnz);

  
  
  printf("start creating device\n");
  hb_mc_device_t device;
  err = hb_mc_device_init(&device, test_name, 0);
  if (err != HB_MC_SUCCESS) {
    printf("failed to initialize device.\n");
    return err;
  }

  printf("start doing hb_mc_device_program_init\n");
  err = hb_mc_device_program_init(&device, bin_path, "default_allocator", 0);
  if (err != HB_MC_SUCCESS) {
    printf("failed to initialize program.\n");
    return err;
  }

  /*---------------------------------------------------------------------
  * Copy the the matrices to the hammerblade
  *-------------------------------------------------------------------*/
  printf("start sparse_mat_memcpy_to\n");
  eva_t A_matrix_dev;
  A_matrix_dev = sparse_mat_memcpy_to(&device, sparse_matrix);

  eva_t B_matrix_dev;
  err = hb_mc_device_malloc(&device, n * k * sizeof(data_t), &B_matrix_dev);
  if (err != HB_MC_SUCCESS) {
    printf("failed to allocate memory on device.\n");
    return err;
  }
  /*
  err = hb_mc_device_memcpy(&device, (void*)((intptr_t)B_matrix_dev), (void*)dense_matrix,
                            k * n * sizeof(data_t), HB_MC_MEMCPY_TO_DEVICE);
  if (err != HB_MC_SUCCESS) {
    printf("failed to copy memory to device.\n");
    return err;
  }
  */
  hb_mc_dma_htod_t B_matrix_dma = {B_matrix_dev, (void*)dense_matrix, n * k * sizeof(data_t)};
  err = hb_mc_device_dma_to_device(&device, &B_matrix_dma, 1);

  if (err != HB_MC_SUCCESS) {
    printf("failed to copy memory to device.\n");
    return err;
  }

  eva_t Out_matrix_dev;
  err = hb_mc_device_malloc(&device, m * k * sizeof(data_t), &Out_matrix_dev);
  if (err != HB_MC_SUCCESS) {
    printf("failed to allocate memory on device.\n");
    return err;
  }
  /*
  err = hb_mc_device_memcpy(&device, (void*)((intptr_t)Out_matrix_dev), (void*)out_matrix_input, m * n * sizeof(data_t), HB_MC_MEMCPY_TO_DEVICE);
  if (err != HB_MC_SUCCESS) {
    printf("failed to copy memory to device.\n");
    return err;
  }
  */
  hb_mc_dma_htod_t out_matrix_dma = {Out_matrix_dev, (void*)out_matrix_input, m * k * sizeof(data_t)};
  err = hb_mc_device_dma_to_device(&device, &out_matrix_dma, 1);

  if (err != HB_MC_SUCCESS) {
    printf("failed to copy memory to device.\n");
    return err;
  }

  // debug flags
  /*
  eva_t debug_flags_dev;
  int debug_flags[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  err = hb_mc_device_malloc(&device, 10 * sizeof(int), &debug_flags_dev);
  if (err != HB_MC_SUCCESS) {
    printf("failed to allocate memory on device.\n");
    return err;
  }
  
  err = hb_mc_device_memcpy(&device, (void*)((intptr_t)debug_flags_dev), (void*)debug_flags,
                            10 * sizeof(int), HB_MC_MEMCPY_TO_DEVICE);
  if (err != HB_MC_SUCCESS) {
    printf("failed to copy memory to device.\n");
    return err;
  }
  */

  hb_mc_dimension_t grid_dim = { .x = 0, .y = 0};
  hb_mc_dimension_t tg_dim = { .x = 0, .y = 0 };
  hb_mc_dimension_t block_size = { .x = 0, .y = 0 };
  
  if(!strcmp("v0", test_name)){
    grid_dim = { .x = 1, .y = 1};
    tg_dim = { .x = 4, .y = 4 };  // v0 perform row by row multiplication
  }else if(!strcmp("v1", test_name)){
    grid_dim = { .x = 1, .y = 1};
    tg_dim = { .x = 4, .y = 4 };  // v1 perform blocking strategy, block size could be flexible, and could deal with the condtion when matrix size could not be divided by the block size
  }else if(!strcmp("v2", test_name)){
    grid_dim = { .x = 1, .y = 1};
    tg_dim = { .x = 4, .y = 4 };  // v2 perform row by column outer product strategy
  }else if(!strcmp("v3", test_name)){
    grid_dim = { .x = 1, .y = 1};
    tg_dim = { .x = 4, .y = 4 };  // v3 perform blocking strategy using outer product strategy
  }
  else {
    bsg_pr_test_err("Invalid version provided!.\n");
    return HB_MC_INVALID;
  }
  /*---------------------------------------------------------------------
  * Prepare list of input arguments for kernel
  *---------------------------------------------------------------------*/
  uint32_t cuda_argv[6] = {A_matrix_dev, B_matrix_dev, Out_matrix_dev,
                           m, n, k};
  int cuda_argc = 6;
  printf("hb_mc_kernel_enqueue\n");
  err = hb_mc_kernel_enqueue(&device, grid_dim, tg_dim, "kernel_sm_dm", cuda_argc, cuda_argv);
  if (err != HB_MC_SUCCESS) {
    printf("failed to hb_mc_kernel_enqueue.\n");
    return err;
  }

  printf("hb_mc_device_tile_groups_execute\n");
  err = hb_mc_device_tile_groups_execute(&device);
  if (err != HB_MC_SUCCESS) {
    printf("failed to execute tile groups.\n");
    return err;
  }

  /*--------------------------------------------------------------------
  * Copy result matrix back from device DRAM into host memory.
  *-------------------------------------------------------------------*/
  data_t Out_host[m * k];
  err = hb_mc_device_memcpy(&device, (void *)Out_host, (void *)((intptr_t)Out_matrix_dev),
                            m * k * sizeof(data_t),
                            HB_MC_MEMCPY_TO_HOST);
  if (err != HB_MC_SUCCESS) {
    bsg_pr_test_err("failed to copy memory from device.\n");
    return err;
  }
  
  /*
  data_t *Out_true;
  data_t Out_tran[m * k];
  if(!strcmp("v0", test_name)){
    matrix_transpose(Out_host,Out_tran,m,k);
    Out_true = Out_tran;
  }else{
    Out_true = Out_host;
  }
  */

  printf("\n");
  //printf("---------------------------------Matrix A--------------------------------------\n");
  //matrix_print(sparse_matrix, m, n);
  printf("---------------------------------Matrix B--------------------------------------\n"); // for non-transposeed B
  matrix_print(dense_matrix, n, k);
  printf("---------------------------------Matrix C--------------------------------------\n");
  matrix_print(Out_host, m, k);
  printf("-----------------------------Reference result--------------------------------------\n");
  matrix_print(Out_reference, m, k);

  double tolerance = 0.1;
  double error = check(Out_reference, Out_host, m, k);
  //cout << error << endl;
  if (error > tolerance) {
    bsg_pr_test_err(BSG_RED("Mismatch. Error: %f\n"), error);
    return HB_MC_FAIL;
  }
  bsg_pr_test_info(BSG_GREEN("Match.\n"));

  // err = hb_mc_device_memcpy(&device, (void *)debug_flags, (void*)((intptr_t)debug_flags_dev),
  //                           10 * sizeof(int), HB_MC_MEMCPY_TO_HOST);
  // if (err != HB_MC_SUCCESS) {
  //   printf("failed to copy memory from device.\n");
  //   return err;
  // }

  // printf("debug flags\n");
  // for (int i = 0; i < 10; i++) {
  //   printf("%d ", debug_flags[i]);
  // }
  // printf("\n");

  /*--------------------------------------------------------------------
  * Freeze the tiles and memory manager cleanup.
  *-------------------------------------------------------------------*/
  err = hb_mc_device_finish(&device);
  if (err != HB_MC_SUCCESS) {
    printf("failed to de-initialize device.\n");
    return err;
  }

  destroy_sparse_mat(sparse_matrix);

  return HB_MC_SUCCESS;
}


#ifdef COSIM
void cosim_main(uint32_t *exit_code, char * args) {
  int argc = get_argc(args);
  char *argv[argc];
  get_argv(args, argc, argv);
#ifdef VCS
  svScope scope;
  scope = svGetScopeFromName("tb");
  svSetScope(scope);
#endif
  bsg_pr_test_info("test_smdm Regression Test (COSIMULATION)\n");
  int rc = test_smdm(argc, argv);
  *exit_code = rc;
  bsg_pr_test_pass_fail(rc == HB_MC_SUCCESS);
  return;
}
#else
int main(int argc, char ** argv) {
  bsg_pr_test_info("test_smdm Regression Test (F1)\n");
  int rc = test_smdm(argc, argv);
  bsg_pr_test_pass_fail(rc == HB_MC_SUCCESS);
  return rc;
}
#endif
