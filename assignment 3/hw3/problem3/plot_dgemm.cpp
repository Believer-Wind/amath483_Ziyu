#include "ref_dgemm.hpp"
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

using hr_clock = std::chrono::high_resolution_clock;

int main() {
    const int nmin    = 2;
    const int nmax    = 512;
    const int ntrials = 3;
    const double alpha = 1.2, beta = 0.8;

    std::mt19937_64 rng(123456);
    std::uniform_real_distribution<double> dist(0.5, 1.5);

    std::ofstream out("dgemm_perf.csv");
    out << "n,flops_per_sec\n";

    for (int n = nmin; n <= nmax; ++n) 
    {
        // allocate and init A, B, C
        std::vector<std::vector<double>> A(n, std::vector<double>(n)),
                                        B(n, std::vector<double>(n)),
                                        C(n, std::vector<double>(n));
        for (int i = 0; i < n; ++i) 
        {
            for (int j = 0; j < n; ++j) 
            {
                A[i][j] = dist(rng);
                B[i][j] = dist(rng);
                C[i][j] = dist(rng);
            }
        }

        // warm-up
        dgemm(alpha, A, B, beta, C);

        // time ntrials
        long double total = 0;
        for (int t = 0; t < ntrials; ++t) 
        {
            auto t0 = hr_clock::now();
            dgemm(alpha, A, B, beta, C);
            auto t1 = hr_clock::now();
            total += std::chrono::duration<long double>(t1 - t0).count();
        }
        long double avg = total / ntrials;  // seconds

        // flop count: for each C[i][j], k→n multiplications + (n-1) adds ≈ 2n,
        // over n^2 elements → ≈2n^3 ops, plus β·C gives 2n^2 more.
        long double flops = 2.0L*n*n*n + 2.0L*n*n;
        long double perf  = flops / avg;    // FLOPs/sec

        out << n << "," << perf << "\n";
        std::cout << "n=" << n << " -> " << (perf/1e9L) << " GFLOP/s\n";
    }
    out.close();
    return 0;
}
