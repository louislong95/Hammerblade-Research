#include "dmdm_host.hpp"

// All matrices are square
#define A_HEIGHT 16
#define A_WIDTH  16
#define B_HEIGHT A_WIDTH
#define B_WIDTH  16
#define C_HEIGHT A_HEIGHT
#define C_WIDTH  B_WIDTH
#define NUM_ITER 4   // remain to see what it is

// Host Matrix multiplication code (to compare results)
template <typename TA, typename TB, typename TC>
void matrix_mult (TA *A, TB *B, TC *C, uint64_t M, uint64_t N, uint64_t P) {
    for (uint64_t y = 0; y < M; y ++) {
        for (uint64_t x = 0; x < P; x ++) {
            TC res = 0.0f;
            for (uint64_t k = 0; k < N; k++) {
                res += A[y * N + k] * B[k * P + x];
            }
            C[y * P + x] = res;
        }
    }
    return;
}

// Compute the sum of squared error between matrix A and B (M x N)
template <typename T>
double matrix_sse (const T *A, const T *B, uint64_t M, uint64_t N) {
    double sum = 0;
    for (uint64_t y = 0; y < M; y ++) {
        for (uint64_t x = 0; x < N; x ++) {
            T diff = A[y * N + x] - B[y * N + x];
            if(std::isnan(diff)){
                return diff;
            }
            sum += diff * diff;
        }
    }
    return sum;
}

// Print matrix A (M x N). Here we assume matrix A and B are the same, so we only need to print one of them
template <typename T>
void matrix_print(T *A, uint64_t M, uint64_t N) {
    T sum = 0;
    for (uint64_t y = 0; y < M; y ++) {
        for (uint64_t x = 0; x < N; x ++) {
            //std::cout << A[y * N + x] << " ";
            printf("%d ", A[y * N + x]);
        }
        //std::cout << '\n';
        printf("\n");
    }
}

// Matrix utility functions.

// Transpose Matrix A (M x N) into Matrix B (N x M)
template <typename T>
void matrix_transpose (const T *A, T *B, uint64_t M, uint64_t N) {
    double sum = 0;
    for (uint64_t y = 0; y < M; y ++) {
        for (uint64_t x = 0; x < N; x ++) {
            B[x * N + y] = A[y * M + x];
        }
    }
}

// Run a Matrix-Matrix Multiply test on the Manycore, and compare the result.  A
// and B are the input matricies, C is the destination, and gold is the
// known-good result computed by the host.
template<typename TA, typename TB, typename TC>
int run_test(hb_mc_device_t &device, const char* kernel,
             const TA *A, const TB *B, TC *C, const TC *gold,
             const eva_t &A_device,
             const eva_t &B_device,
             const eva_t &C_device,
             const hb_mc_dimension_t &tg_dim,
             const hb_mc_dimension_t &grid_dim,
             const unsigned int tag){
    int rc;
    //uint32_t block_size_x = 4;
    //uint32_t block_size_y = 4;
    // Copy A & B from host onto device DRAM.
    void *dst = (void *) ((intptr_t) A_device);
    void *src = (void *) &A[0];
    rc = hb_mc_device_memcpy (&device, dst, src,
                              (A_HEIGHT * A_WIDTH) * sizeof(TA),
                              HB_MC_MEMCPY_TO_DEVICE);
    if (rc != HB_MC_SUCCESS) {
        bsg_pr_test_err("failed to copy memory to device.\n");
        return rc;
    }
    
    
    dst = (void *) ((intptr_t) B_device);
    src = (void *) &B[0];
    rc = hb_mc_device_memcpy (&device, dst, src,
                              (B_HEIGHT * B_WIDTH) * sizeof(TB),
                              HB_MC_MEMCPY_TO_DEVICE);
    if (rc != HB_MC_SUCCESS) {
        bsg_pr_test_err("failed to copy memory to device.\n");
        return rc;
    }
    
    
    // Prepare list of input arguments for kernel. See the kernel source
    // file for the argument uses.
    uint32_t cuda_argv[6] = {A_device, B_device, C_device,
        A_HEIGHT, A_WIDTH, B_WIDTH
        };
    
    // Enquque grid of tile groups, pass in grid and tile group dimensions,
    // kernel name, number and list of input arguments
    rc = hb_mc_kernel_enqueue (&device, grid_dim, tg_dim,
                               kernel, 6, cuda_argv);
    if (rc != HB_MC_SUCCESS) {
        bsg_pr_test_err("failed to initialize grid.\n");
        return rc;
    }
    
    
    // Launch and execute all tile groups on device and wait for all to
    // finish.
    rc = hb_mc_device_tile_groups_execute(&device);
    if (rc != HB_MC_SUCCESS) {
        bsg_pr_test_err("failed to execute tile groups.\n");
        return rc;
    }
    
    
    // Copy result matrix back from device DRAM into host memory.
    src = (void *) ((intptr_t) C_device);
    dst = (void *) &C[0];
    rc = hb_mc_device_memcpy (&device, (void *) dst, src,
                              (C_HEIGHT * C_WIDTH) * sizeof(TC),
                              HB_MC_MEMCPY_TO_HOST);
    if (rc != HB_MC_SUCCESS) {
        bsg_pr_test_err("failed to copy memory from device.\n");
        return rc;
    }
    
    // Compare the known-correct matrix (gold) and the result matrix (C)
    float max = 0.1;
    double sse = matrix_sse(gold, C, C_HEIGHT, C_WIDTH);
    
    if (std::isnan(sse) || sse > max) {
        bsg_pr_test_err(BSG_RED("Matrix Mismatch. SSE: %f\n"), sse);
        return HB_MC_FAIL;
    }
    bsg_pr_test_info(BSG_GREEN("Matrix Match.\n"));
}

// Run a series of Matrix-Matrix multiply tsts on the Manycore device
int kernel_matrix_matrix_multiply (int argc, char **argv) {
    int rc;
    char *bin_path, *test_name;
    struct arguments_path args = {NULL, NULL};
    
    argp_parse (&argp_path, argc, argv, 0, 0, &args);
    bin_path = args.path;
    test_name = args.name;
    
    bsg_pr_test_info("Running CUDA Matrix-Matrix Multiplication "
                     "on a single tile.\n");
    
    // Define tg_dim_x/y: number of tiles in each tile group
    // Calculate grid_dim_x/y: number of tile groups needed
    hb_mc_dimension_t tg_dim = { .x = 1, .y = 1 };
    hb_mc_dimension_t grid_dim = { .x = 1, .y = 1 };
    //uint32_t block_size_x = 4;
    //uint32_t block_size_y = 4;
    if(!strcmp("v0", test_name)){        // v0 (1 x 4)implement the data distribution along row side, with scratchpad
        grid_dim = { .x = 1, .y = 1};
        tg_dim = { .x = 1, .y = 4 };
    }else if(!strcmp("v1", test_name)){  // v1 (1 x 4)implement the data distribution along column side, with scratchpad
        grid_dim = { .x = 1, .y = 1};
        tg_dim = { .x = 1, .y = 4 };
    }else if(!strcmp("v2", test_name)){  // v2 (2 x 2)implement the data distribution along row side, without scratchpad
        grid_dim = { .x = 1, .y = 1};
        tg_dim = { .x = 2, .y = 2 };
    }else if(!strcmp("v3", test_name)){  // v3 (2 x 2)implement the data distribution along column side, without scratchpad
        grid_dim = { .x = 1, .y = 1};
        tg_dim = { .x = 2, .y = 2 };
    }else if(!strcmp("v4", test_name)){  // v4 (2 x 2)implement the data distribution along row side, with scratchpad
        grid_dim = { .x = 1, .y = 1};
        tg_dim = { .x = 2, .y = 2 };
    }else if(!strcmp("v5", test_name)){  // v5 (2 x 2)implement the data distribution along column side, with scratchpad
        grid_dim = { .x = 1, .y = 1};
        tg_dim = { .x = 2, .y = 2 };
    }else if(!strcmp("v6", test_name)){  // v6 (4 x 1)implement the data distribution along row side, with scratchpad
        grid_dim = { .x = 1, .y = 1};
        tg_dim = { .x = 4, .y = 1 };
    }else if(!strcmp("v7", test_name)){  // v7 (4 x 1)implement the data distribution along column side, with scratchpad
        grid_dim = { .x = 1, .y = 1};
        tg_dim = { .x = 4, .y = 1 };
    }else if(!strcmp("v8", test_name)){  // v8 (2 x 2)implement the data distribution along column side, with scratchpad, make use of Matrix tranpose
        grid_dim = { .x = 1, .y = 1};
        tg_dim = { .x = 2, .y = 2 };
    }else if(!strcmp("v9", test_name)){  // v9 (2 x 2)implement the data distribution with row block, with scratchpad
        grid_dim = { .x = 1, .y = 1};
        tg_dim = { .x = 2, .y = 2 };
    }else if(!strcmp("v10", test_name)){  // v10 (2 x 2)implement the data distribution with row side, with scratchpad storing specific data instead of the whole matrix
        grid_dim = { .x = 1, .y = 1};
        tg_dim = { .x = 2, .y = 2 };
    }else if(!strcmp("v11", test_name)){  // v11 (2 x 2)implement the data distribution with col side, with scratchpad storing specific data instead of the whole matrix, matrix transposing to allow the whole row to be stored in the scratchpad
        grid_dim = { .x = 1, .y = 1};
        tg_dim = { .x = 2, .y = 2 };
    }else if(!strcmp("v12", test_name)){  // v12 (2 x 2)implement the data distribution with col side, with scratchpad storing specific data instead of the whole matrix
        grid_dim = { .x = 1, .y = 1};
        tg_dim = { .x = 2, .y = 2 };
    }else if(!strcmp("v13", test_name)){  // v13 (2 x 2)implement the data distribution with col side, with scratchpad storing temperal value in order to perform large matrix multiplication, matrix transposing to allow the whole row to be stored in the scratchpad
        grid_dim = { .x = 1, .y = 1};
        tg_dim = { .x = 2, .y = 2 };
    }else if(!strcmp("v15", test_name)){  // v15 (2 x 2)implement the data distribution with 2 x 2 matrix distribution, with scratchpad loading the matrix block, matrix B transposing
        grid_dim = { .x = 1, .y = 1};
        tg_dim = { .x = 2, .y = 2 };
    }else {
        bsg_pr_test_err("Invalid version provided!.\n");
        return HB_MC_INVALID;
    }
    
    // Initialize the random number generators
    std::numeric_limits<int8_t> lim; // Used to get INT_MIN and INT_MAX in C++
    std::default_random_engine generator;
    generator.seed(42);
    std::uniform_real_distribution<float> distribution(lim.min(),lim.max());
    
    // Allocate A, B, BT (B-Transposed), C and R (result) on the host for each datatype.
    // Allocate pointers for B to abstract between when we use B or BT. B is
    // used on kernel versions v0, v1, and v2. BT is used on all subsequent versions.
    
    // only test the 8-bit integer in order to save test time
    
    int8_t A_8[A_HEIGHT * A_WIDTH];
    int8_t B_8[B_HEIGHT * B_WIDTH], BT_8[B_HEIGHT * B_WIDTH], *B_8p;
    int8_t C_8[C_HEIGHT * C_WIDTH];
    int8_t R_8[C_HEIGHT * C_WIDTH];
    
    // Here we use B type or BT type
    if( !strcmp("v0", test_name) || !strcmp("v1", test_name) || !strcmp("v2", test_name) || !strcmp("v3", test_name) || !strcmp("v4", test_name) || !strcmp("v5", test_name)
                                 || !strcmp("v6", test_name) || !strcmp("v7", test_name) || !strcmp("v9", test_name) || !strcmp("v10", test_name) || !strcmp("v12", test_name)
                                 ){
        B_8p = B_8;
    } else {
        B_8p = BT_8; //  v15 implement matrix B transposing
    }
    
    
    // Generate random numbers. Since the Manycore can't handle infinities,
    // subnormal numbers, or NANs, filter those out.
    auto res = distribution(generator);
    
    // Generate numbers for A matrix
    for (uint64_t i = 0; i < A_HEIGHT * A_WIDTH; i++) {
        do{
            res = distribution(generator);
        }while(!std::isnormal(res) ||
               !std::isfinite(res) ||
               std::isnan(res));
        
        A_8[i] = static_cast<int8_t>(res);
    }
    
    // Generate numbers for B matrix
    for (uint64_t i = 0; i < B_HEIGHT * B_WIDTH; i++) {
        do{
            res = distribution(generator);
        }while(!std::isnormal(res) ||  // std::ismornal means that if <res> is not normal, it will return 0
               !std::isfinite(res) ||
               std::isnan(res));
        
        B_8[i] = static_cast<int8_t>(res); // static_cast is a type transfer function, here res is transfered to int8 type
    }
    
    // Generate the known-correct results on the host
    matrix_mult (A_8, B_8, R_8, A_HEIGHT, A_WIDTH, B_WIDTH);
    
    matrix_transpose(B_8, BT_8, B_HEIGHT, B_WIDTH);
    
    // Initialize device, load binary and unfreeze tiles.
    printf("start creating device\n");
    hb_mc_device_t device;
    rc = hb_mc_device_init(&device, test_name, 0);
    if (rc != HB_MC_SUCCESS) {
        bsg_pr_test_err("failed to initialize device.\n");
        return rc;
    }
    
    
    // Initialize the device with a kernel file
    printf("start doing hb_mc_device_program_init\n");
    rc = hb_mc_device_program_init(&device, bin_path, "default_allocator", 0);
    if (rc != HB_MC_SUCCESS) {
        bsg_pr_test_err("failed to initialize program.\n");
        return rc;
    }
    
    // Allocate memory on the device for A, B and C. Since sizeof(float) ==
    // sizeof(int32_t) > sizeof(int16_t) > sizeof(int8_t) we'll reuse the
    // same buffers for each test
    
    eva_t A_device, B_device, C_device;
    
    // Allocate A on the device
    rc = hb_mc_device_malloc(&device,
                             A_HEIGHT * A_WIDTH * sizeof(uint32_t),
                             &A_device);
    if (rc != HB_MC_SUCCESS) {
        bsg_pr_test_err("failed to allocate memory on device.\n");
        return rc;
    }
    
    // Allocate B on the device
    rc = hb_mc_device_malloc(&device,
                             B_HEIGHT * B_WIDTH * sizeof(uint32_t),
                             &B_device);
    if (rc != HB_MC_SUCCESS) {
        bsg_pr_test_err("failed to allocate memory on device.\n");
        return rc;
    }
    
    // Allocate C on the device
    rc = hb_mc_device_malloc(&device,
                             C_HEIGHT * C_WIDTH * sizeof(uint32_t),
                             &C_device);
    if (rc != HB_MC_SUCCESS) {
        bsg_pr_test_err("failed to allocate memory on device.\n");
        return rc;
    }
    
    
    // Run the 8-bit integer test and check the result
    rc = run_test(device, "kernel_matrix_multiply_int8",
                  A_8, B_8p, C_8, R_8,
                  A_device, B_device, C_device,
                  tg_dim, grid_dim, 1);

    printf("\n");
    printf("---------------------------------Matrix A--------------------------------------\n");
    matrix_print(A_8, A_HEIGHT, A_WIDTH);
    printf("---------------------------------Matrix B--------------------------------------\n");
    matrix_print(B_8p, A_HEIGHT, A_WIDTH);
    printf("---------------------------------Matrix C--------------------------------------\n");
    matrix_print(C_8, A_HEIGHT, A_WIDTH);
    printf("-----------------------------Reference result--------------------------------------\n");
    matrix_print(R_8, A_HEIGHT, A_WIDTH);

    if (rc != HB_MC_SUCCESS) {
        bsg_pr_test_err("int8_t test failed\n");
        return rc;
    }
    bsg_pr_test_info("int8_t test passed!\n");
    
    // Freeze the tiles and memory manager cleanup.
    rc = hb_mc_device_finish(&device);
    if (rc != HB_MC_SUCCESS) {
        bsg_pr_test_err("failed to de-initialize device.\n");
        return rc;
    }
    
    return HB_MC_SUCCESS;
}


#ifdef COSIM
void cosim_main(uint32_t *exit_code, char * args) {
    // We aren't passed command line arguments directly so we parse them
    // from *args. args is a string from VCS - to pass a string of arguments
    // to args, pass c_args to VCS as follows: +c_args="<space separated
    // list of args>"
    int argc = get_argc(args);
    char *argv[argc];
    get_argv(args, argc, argv);
    
    svScope scope;
    scope = svGetScopeFromName("tb");
    svSetScope(scope);
    
    int rc = kernel_matrix_matrix_multiply(argc, argv);
    *exit_code = rc;
    bsg_pr_test_pass_fail(rc == HB_MC_SUCCESS);
    return;
}
#else
int main(int argc, char ** argv) {
    int rc = kernel_matrix_matrix_multiply(argc, argv);
    bsg_pr_test_pass_fail(rc == HB_MC_SUCCESS);
    return rc;
}
#endif
