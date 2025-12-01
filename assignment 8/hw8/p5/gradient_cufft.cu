// gradient_cufft.cu
#include <iostream>
#include <cmath>
#include <chrono>
#include <cuda_runtime.h>
#include <cufft.h>

#define CUDA_CHECK(call)                                                     \
  do {                                                                        \
    cudaError_t err = call;                                                   \
    if (err != cudaSuccess) {                                                 \
      std::cerr << "CUDA error: " << cudaGetErrorString(err)                  \
                << " at " << __FILE__ << ":" << __LINE__ << std::endl;        \
      std::exit(1);                                                           \
    }                                                                         \
  } while (0)

// cuFFT error checking macro
#define CUFFT_CHECK(call)                                                    \
  do {                                                                        \
    cufftResult err = call;                                                   \
    if (err != CUFFT_SUCCESS) {                                               \
      std::cerr << "cuFFT error: " << err                                      \
                << " at " << __FILE__ << ":" << __LINE__ << std::endl;        \
      std::exit(1);                                                           \
    }                                                                         \
  } while (0)

using Clock = std::chrono::high_resolution_clock;

// CUDA Kernel: 将频域数据乘以 (i * kx)，只做 x 方向上梯度
__global__ void multiply_grad(cufftDoubleComplex* d_k, int n) {
    long idx = (long)blockIdx.x * blockDim.x + threadIdx.x;
    long N   = (long)n * n * n;
    if (idx >= N) return;

    // 先算出三维坐标 (i,j,k)：
    int local = idx;
    int ix = local / (n * n);
    local %= (n * n);
    int iy = local / n;
    int iz = local % n;

    // FFTW 排序方式： (- (n/2) … -1,   0,  1, …, n/2 -1 )
    int ki = (ix <= n/2 ? ix : ix - n);
    double TWO_PI = 2.0 * M_PI;
    double kx = TWO_PI * ki / n;

    double re = d_k[idx].x;
    double im = d_k[idx].y;
    // 乘以 i*kx： (re + i·im) * i*kx = −kx·im  + i·(kx·re)
    d_k[idx].x = -kx * im;
    d_k[idx].y =  kx * re;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " n ntrial\n";
        return 1;
    }
    int n      = std::atoi(argv[1]);
    int ntrial = std::atoi(argv[2]);
    long N     = (long)n * n * n;
    size_t bytes = sizeof(cufftDoubleComplex) * N;

    // 在主机端生成“平面波”
    // f(i,j,k) = exp(i*2π*(i+j+k)/n)
    cufftDoubleComplex* h_data = (cufftDoubleComplex*)malloc(bytes);
    const double TWO_PI = 2.0 * M_PI;
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            for(int k = 0; k < n; ++k) {
                long idx = ((long)i * n + j) * n + k;
                double phase = TWO_PI * (i + j + k) / n;
                h_data[idx].x = std::cos(phase);
                h_data[idx].y = std::sin(phase);
            }
        }
    }

    // 将数据拷到 GPU
    cufftDoubleComplex* d_data = nullptr;
    CUDA_CHECK(cudaMalloc(&d_data, bytes));
    CUDA_CHECK(cudaMemcpy(d_data, h_data, bytes, cudaMemcpyHostToDevice));

    // 为 3D 复数 → 复数 FFT 建立 cuFFT 计划
    cufftHandle plan;
    CUFFT_CHECK(cufftPlan3d(&plan, n, n, n, CUFFT_Z2Z));

    // “热身”调用：避免首次调用的初始化开销
    CUFFT_CHECK(cufftExecZ2Z(plan, d_data, d_data, CUFFT_FORWARD));
    CUDA_CHECK(cudaDeviceSynchronize());
    CUFFT_CHECK(cufftExecZ2Z(plan, d_data, d_data, CUFFT_INVERSE));
    CUDA_CHECK(cudaDeviceSynchronize());

    // 测时循环
    double total_time = 0.0;
    for(int t = 0; t < ntrial; ++t) {
        auto t0 = Clock::now();

        // 1) 正变换
        CUFFT_CHECK(cufftExecZ2Z(plan, d_data, d_data, CUFFT_FORWARD));

        // 2) 频域乘以 (i*kx)，按网格并行执行
        int threads = 256;
        int blocks  = (N + threads - 1) / threads;
        multiply_grad<<<blocks, threads>>>(d_data, n);
        CUDA_CHECK(cudaDeviceSynchronize());

        // 3) 逆变换
        CUFFT_CHECK(cufftExecZ2Z(plan, d_data, d_data, CUFFT_INVERSE));
        CUDA_CHECK(cudaDeviceSynchronize());

        auto t1 = Clock::now();
        total_time += std::chrono::duration<double>(t1 - t0).count();
    }
    double avg_time = total_time / ntrial;

    // FLOP 估算：与 FFTW 部分同样
    //   - 两次 3D FFT：2*(5 N log2 N) = 10 N log2 N  
    //   - N 点复数乘法：6 N  
    double flop_count = 10.0 * N * std::log2((double)N) + 6.0 * N;
    double gflops = flop_count / avg_time / 1e9;

    std::cout << n << "," << gflops << "," << avg_time << "\n";

    // 清理
    CUFFT_CHECK(cufftDestroy(plan));
    CUDA_CHECK(cudaFree(d_data));
    free(h_data);
    return 0;
}
