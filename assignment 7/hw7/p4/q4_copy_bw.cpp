// q4_copy_bw.cpp
#include <iostream>
#include <cuda_runtime.h>

#define NTRIAL 10

// CUDA error checking macro
#define CUDA_CHECK(call)                            \
    do {                                            \
        cudaError_t status = call;                  \
        if (status != cudaSuccess) {                \
            std::cerr << "CUDA error: "             \
                      << cudaGetErrorString(status) \
                      << " at line " << __LINE__    \
                      << std::endl;                 \
            exit(1);                                \
        }                                           \
    } while (0)

int main() {
    const size_t MIN_SIZE  = 8;                  // 8 bytes
    const size_t MAX_SIZE  = 256ULL * 1024 * 1024; // 256 MB

    // find CUDA devices
    int deviceCount = 0;
    CUDA_CHECK(cudaGetDeviceCount(&deviceCount));
    if (deviceCount == 0) {
        std::cerr << "No CUDA devices found." << std::endl;
        return 1;
    }
    CUDA_CHECK(cudaSetDevice(0));

    std::cout << "Size(B)\tH2D(ms)\tH2D(MB/s)\tD2H(ms)\tD2H(MB/s)\n";

    for (size_t size = MIN_SIZE; size <= MAX_SIZE; size <<= 1) {
        // host alloc
        char *hostData = new char[size];
        // device alloc
        char *deviceData;
        CUDA_CHECK(cudaMalloc(&deviceData, size));

        // measure H2D
        float totalH2D = 0.f;
        for (int t = 0; t < NTRIAL; ++t) {
            cudaEvent_t start, stop;
            CUDA_CHECK(cudaEventCreate(&start));
            CUDA_CHECK(cudaEventCreate(&stop));
            CUDA_CHECK(cudaEventRecord(start, 0));
            CUDA_CHECK(cudaMemcpy(deviceData, hostData, size, cudaMemcpyHostToDevice));
            CUDA_CHECK(cudaEventRecord(stop, 0));
            CUDA_CHECK(cudaEventSynchronize(stop));
            float ms = 0.f;
            CUDA_CHECK(cudaEventElapsedTime(&ms, start, stop));
            totalH2D += ms;
            CUDA_CHECK(cudaEventDestroy(start));
            CUDA_CHECK(cudaEventDestroy(stop));
        }
        float h2d_ms = totalH2D / NTRIAL;
        float h2d_bw = (size / (h2d_ms/1000.0f)) / (1024*1024);

        // measure D2H
        float totalD2H = 0.f;
        for (int t = 0; t < NTRIAL; ++t) {
            cudaEvent_t start, stop;
            CUDA_CHECK(cudaEventCreate(&start));
            CUDA_CHECK(cudaEventCreate(&stop));
            CUDA_CHECK(cudaEventRecord(start, 0));
            CUDA_CHECK(cudaMemcpy(hostData, deviceData, size, cudaMemcpyDeviceToHost));
            CUDA_CHECK(cudaEventRecord(stop, 0));
            CUDA_CHECK(cudaEventSynchronize(stop));
            float ms = 0.f;
            CUDA_CHECK(cudaEventElapsedTime(&ms, start, stop));
            totalD2H += ms;
            CUDA_CHECK(cudaEventDestroy(start));
            CUDA_CHECK(cudaEventDestroy(stop));
        }
        float d2h_ms = totalD2H / NTRIAL;
        float d2h_bw = (size / (d2h_ms/1000.0f)) / (1024*1024);

        // print one line
        std::cout
            << size << "\t"
            << h2d_ms << "\t" << h2d_bw << "\t"
            << d2h_ms << "\t" << d2h_bw << "\n";

        // cleanup
        CUDA_CHECK(cudaFree(deviceData));
        delete[] hostData;
    }
    return 0;
}
