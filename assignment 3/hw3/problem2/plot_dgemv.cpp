#include "ref_dgemv.hpp"
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
    const double alpha = 2.5, beta = 1.5;

    std::mt19937_64 rng(20250423);
    std::uniform_real_distribution<double> dist(1.0, 2.0);

    std::ofstream out("dgemv_perf.csv");
    out << "n,flops_per_sec\n";

    for (int n = nmin; n <= nmax; ++n) 
    {
        std::vector<std::vector<double>> A(n, std::vector<double>(n));
        std::vector<double> x(n), y(n);
        for (int i = 0; i < n; ++i) 
        {
            x[i] = dist(rng);
            y[i] = dist(rng);
            for (int j = 0; j < n; ++j) 
            {
                A[i][j] = dist(rng);
            }
        }

        // warm-up
        dgemv(alpha, A, x, beta, y);

        // time trials
        long double total_time = 0;
        for (int t = 0; t < ntrials; ++t) 
        {
            auto t0 = hr_clock::now();
            dgemv(alpha, A, x, beta, y);
            auto t1 = hr_clock::now();
            total_time += std::chrono::duration<long double>(t1 - t0).count();
        }
        long double avg_time = total_time / ntrials;

        // flop count: per row = n mult + (n−1) add ≈ 2n ops, plus 1 mul+1 add for β·y
        long double flops = 2.0L * n * n + 2.0L * n;
        long double perf  = flops / avg_time;

        out << n << "," << perf << "\n";
        std::cout << "n="<<n<<"  "<<(perf/1e9L)<<" GFLOP/s\n";
    }
    out.close();
}
