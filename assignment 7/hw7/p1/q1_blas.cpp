// q1_blas.cpp
#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <cblas.h>

using namespace std;
using Clock = chrono::high_resolution_clock;

int main() {
    const int ntrial = 3;
    const int Nmin   = 2;
    const int Nmax   = 4096;
    const double alpha = 1.0, beta = 0.5;

    cout << "N,Function,GFLOPs,Time(s)\n";

    // For each problem size N = 2,4,8,...,4096
    for (int N = Nmin; N <= Nmax; N <<= 1) {
        // allocate and initialize
        vector<double> A(N*N), B(N*N), C(N*N), X(N), Y(N);
        for (int i = 0; i < N*N; ++i) {
            A[i] = rand() / (double)RAND_MAX;
            B[i] = rand() / (double)RAND_MAX;
            C[i] = rand() / (double)RAND_MAX;
        }
        for (int i = 0; i < N; ++i) {
            X[i] = rand() / (double)RAND_MAX;
            Y[i] = rand() / (double)RAND_MAX;
        }

        // 1) cblas_daxpy  (2*N flops)
        {
            double flops = 2.0 * N;
            cblas_daxpy(N, alpha, X.data(), 1, Y.data(), 1);
            double total_t = 0;
            for (int t = 0; t < ntrial; ++t) {
                auto t0 = Clock::now();
                cblas_daxpy(N, alpha, X.data(), 1, Y.data(), 1);
                auto t1 = Clock::now();
                total_t += chrono::duration<double>(t1 - t0).count();
            }
            double tm = total_t / ntrial;
            cout << N << ",daxpy," << (flops/tm)/1e9 << "," << tm << "\n";
        }

        // 2) cblas_dgemv (2*N*N + 2*N flops)
        {
            double flops = 2.0*N*N + 2.0*N;
            cblas_dgemv(CblasRowMajor, CblasNoTrans,
                        N, N, alpha,
                        A.data(), N,
                        X.data(), 1,
                        beta,  Y.data(), 1);
            double total_t = 0;
            for (int t = 0; t < ntrial; ++t) {
                auto t0 = Clock::now();
                cblas_dgemv(CblasRowMajor, CblasNoTrans,
                            N, N, alpha,
                            A.data(), N,
                            X.data(), 1,
                            beta, Y.data(), 1);
                auto t1 = Clock::now();
                total_t += chrono::duration<double>(t1 - t0).count();
            }
            double tm = total_t / ntrial;
            cout << N << ",dgemv," << (flops/tm)/1e9 << "," << tm << "\n";
        }

        // 3) cblas_dgemm (2*N^3 + 2*N^2 flops)
        {
            double flops = 2.0*N*N*N + 2.0*N*N;
            cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                        N, N, N,
                        alpha,
                        A.data(), N,
                        B.data(), N,
                        beta,
                        C.data(), N);
            double total_t = 0;
            for (int t = 0; t < ntrial; ++t) {
                auto t0 = Clock::now();
                cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                            N, N, N,
                            alpha,
                            A.data(), N,
                            B.data(), N,
                            beta,
                            C.data(), N);
                auto t1 = Clock::now();
                total_t += chrono::duration<double>(t1 - t0).count();
            }
            double tm = total_t / ntrial;
            cout << N << ",dgemm," << (flops/tm)/1e9 << "," << tm << "\n";
        }
    }

    return 0;
}
