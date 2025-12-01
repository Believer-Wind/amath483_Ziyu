// plot_daxpy.cpp
#include "ref_daxpy.hpp"
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

using hr_clock = std::chrono::high_resolution_clock;

int main() {
    const int nmin = 2, nmax = 512;
    const int ntrials = 3;
    const double alpha = 2.5;

    std::mt19937_64 rng(12345);
    std::uniform_real_distribution<double> dist(1.0, 2.0);

    std::ofstream out("daxpy_perf.csv");
    out << "n,flops_per_sec\n";

    for (int n = nmin; n <= nmax; ++n) {
        // initialize x,y
        std::vector<double> x(n), y(n);
        for (int i = 0; i < n; ++i) {
            x[i] = dist(rng);
            y[i] = dist(rng);
        }

        // warm-up
        daxpy(alpha, x, y);

        // time trials
        long double total_time = 0;
        for (int t = 0; t < ntrials; ++t) {
            auto t0 = hr_clock::now();
            daxpy(alpha, x, y);
            auto t1 = hr_clock::now();
            total_time += std::chrono::duration<long double>(t1 - t0).count();
        }
        long double avg_time = total_time / ntrials;  // in seconds

        // compute performance
        long double flops = 2.0L * n;                  // 2n flops per call
        long double perf = flops / avg_time;           // FLOPs/sec

        out << n << "," << perf << "\n";
        std::cout << "n=" << n
                  << "  " << (perf/1e9L) << " GFLOP/s\n";
    }
    return 0;
}
