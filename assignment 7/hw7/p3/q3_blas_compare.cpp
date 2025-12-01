// q3_blas_compare.cpp

#include <iostream>
#include <cstdlib>
#include <chrono>
#include <cblas.h>
#include <cuda_runtime.h>
#include <cublas_v2.h>

// CUDA error checking macro
#define CUDA_CHECK(call)                                                     \
  do {                                                                        \
    cudaError_t err = call;                                                   \
    if (err != cudaSuccess) {                                                 \
      std::cerr << "CUDA error: " << cudaGetErrorString(err)                  \
                << " at " << __FILE__ << ":" << __LINE__ << std::endl;        \
      std::exit(1);                                                           \
    }                                                                         \
  } while (0)

// cuBLAS error checking macro
#define CUBLAS_CHECK(call)                                                   \
  do {                                                                        \
    cublasStatus_t status = call;                                             \
    if (status != CUBLAS_STATUS_SUCCESS) {                                    \
      std::cerr << "cuBLAS error: " << status                                 \
                << " at " << __FILE__ << ":" << __LINE__ << std::endl;        \
      std::exit(1);                                                           \
    }                                                                         \
  } while (0)

using Clock = std::chrono::high_resolution_clock;

// Initialize a square matrix (row-major) with random values in [0,1)
void initializeMatrix(double *M, int N) {
    for (int i = 0; i < N*N; ++i) {
        M[i] = std::rand() / static_cast<double>(RAND_MAX);
    }
}

// Perform one DGEMM on CPU and return average elapsed seconds
double cpu_gemm(double *A, double *B, double *C, int N, int ntrial) {
    const double alpha = 1.0, beta = 0.5;
    // Warm-up
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                N, N, N, alpha, A, N, B, N, beta, C, N);

    double total = 0.0;
    for (int t = 0; t < ntrial; ++t) {
        auto t0 = Clock::now();
        cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                    N, N, N, alpha, A, N, B, N, beta, C, N);
        auto t1 = Clock::now();
        total += std::chrono::duration<double>(t1 - t0).count();
    }
    return total / ntrial;
}

// Perform DGEMM on GPU via cuBLAS and return average elapsed seconds
double gpu_gemm(cublasHandle_t handle,
                double *d_A, double *d_B, double *d_C,
                int N, int ntrial)
{
    const double alpha = 1.0, beta = 0.5;
    cudaEvent_t start, stop;
    CUDA_CHECK(cudaEventCreate(&start));
    CUDA_CHECK(cudaEventCreate(&stop));

    // Warm-up
    CUBLAS_CHECK(cublasDgemm(handle, CUBLAS_OP_N, CUBLAS_OP_N,
                             N, N, N,
                             &alpha, d_A, N,
                             d_B, N, &beta, d_C, N));
    CUDA_CHECK(cudaDeviceSynchronize());

    // Time ntrial calls in one batch
    CUDA_CHECK(cudaEventRecord(start, 0));
    for (int t = 0; t < ntrial; ++t) {
        CUBLAS_CHECK(cublasDgemm(handle, CUBLAS_OP_N, CUBLAS_OP_N,
                                 N, N, N,
                                 &alpha, d_A, N,
                                 d_B, N, &beta, d_C, N));
    }
    CUDA_CHECK(cudaDeviceSynchronize());
    CUDA_CHECK(cudaEventRecord(stop, 0));
    CUDA_CHECK(cudaEventSynchronize(stop));

    float ms = 0.f;
    CUDA_CHECK(cudaEventElapsedTime(&ms, start, stop));
    CUDA_CHECK(cudaEventDestroy(start));
    CUDA_CHECK(cudaEventDestroy(stop));

    // Return average time in seconds
    return (ms / ntrial) * 1e-3;
}

int main() {
    const int ntrial = 3;
    const int Nmin   = 2;
    const int Nmax   = 16384;

    std::srand(0);
    cublasHandle_t handle;
    CUBLAS_CHECK(cublasCreate(&handle));

    std::cout << "N,OpenBLAS_GFLOPS,CUBLAS_GFLOPS,CPU_Time,CUDA_Time\n";

    for (int N = Nmin; N <= Nmax; N <<= 1) {
        // allocate host matrices
        double *A = new double[N*N];
        double *B = new double[N*N];
        double *C = new double[N*N];
        initializeMatrix(A, N);
        initializeMatrix(B, N);
        initializeMatrix(C, N);

        // CPU GEMM
        double cpu_time = cpu_gemm(A, B, C, N, ntrial);
        double cpu_flop = (2.0 * N * N * N + 2.0 * N * N) / cpu_time / 1e9;

        // allocate device matrices
        double *d_A, *d_B, *d_C;
        CUDA_CHECK(cudaMalloc(&d_A, N*N*sizeof(double)));
        CUDA_CHECK(cudaMalloc(&d_B, N*N*sizeof(double)));
        CUDA_CHECK(cudaMalloc(&d_C, N*N*sizeof(double)));

        // copy to device
        CUDA_CHECK(cudaMemcpy(d_A, A, N*N*sizeof(double), cudaMemcpyHostToDevice));
        CUDA_CHECK(cudaMemcpy(d_B, B, N*N*sizeof(double), cudaMemcpyHostToDevice));
        CUDA_CHECK(cudaMemcpy(d_C, C, N*N*sizeof(double), cudaMemcpyHostToDevice));

        // GPU GEMM
        double gpu_time = gpu_gemm(handle, d_A, d_B, d_C, N, ntrial);
        double gpu_flop = (2.0 * N * N * N + 2.0 * N * N) / gpu_time / 1e9;

        // output CSV line
        std::cout << N << ","
                  << cpu_flop << "," << gpu_flop << ","
                  << cpu_time << "," << gpu_time << "\n";

        // cleanup
        delete[] A; delete[] B; delete[] C;
        CUDA_CHECK(cudaFree(d_A));
        CUDA_CHECK(cudaFree(d_B));
        CUDA_CHECK(cudaFree(d_C));
    }

    CUBLAS_CHECK(cublasDestroy(handle));
    return 0;
}
