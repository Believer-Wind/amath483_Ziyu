#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <string>
#include "mm_loop_permutations.hpp"

int main(int argc, char* argv[])
{
    // Number of trials for timing
    const int ntrial = 3;

    std::string output_file = "results.csv";
    std::string opt_level    = "UNKNOWN";

    // argv[1]: output filename; argv[2]: optimization label
    if (argc > 1) output_file = argv[1];
    if (argc > 2) opt_level  = argv[2];

    std::ofstream csv(output_file);
    csv << "opt_level,perm,type,n,flops_per_sec\n";

    for (auto perm : {"jki", "kij"}) {
        for (auto type : {"float", "double"}) {
            for (int n = 2; n <= 512; ++n) {
                int m = n, p = n;
                if (type == std::string("float")) {
                    using T = float;
                    std::vector<T> A(m*p, T(1)), B(p*n, T(1)), C(m*n, T(0));
                    double total_time = 0;
                    for (int t = 0; t < ntrial; ++t) {
                        auto start = std::chrono::high_resolution_clock::now();
                        if (perm == std::string("jki"))
                            mm_jki<T>(T(1), A, B, T(0), C, m, p, n);
                        else
                            mm_kij<T>(T(1), A, B, T(0), C, m, p, n);
                        auto end = std::chrono::high_resolution_clock::now();
                        total_time += std::chrono::duration<double>(end - start).count();
                    }
                    double avg_time = total_time / ntrial;
                    double flops = 2.0 * m * n * p;
                    csv << opt_level << "," << perm << "," << type
                        << "," << n << "," << flops/avg_time << "\n";
                } else {
                    using T = double;
                    std::vector<T> A(m*p, T(1)), B(p*n, T(1)), C(m*n, T(0));
                    double total_time = 0;
                    for (int t = 0; t < ntrial; ++t) {
                        auto start = std::chrono::high_resolution_clock::now();
                        if (perm == std::string("jki"))
                            mm_jki<T>(T(1), A, B, T(0), C, m, p, n);
                        else
                            mm_kij<T>(T(1), A, B, T(0), C, m, p, n);
                        auto end = std::chrono::high_resolution_clock::now();
                        total_time += std::chrono::duration<double>(end - start).count();
                    }
                    double avg_time = total_time / ntrial;
                    double flops = 2.0 * m * n * p;
                    csv << opt_level << "," << perm << "," << type
                        << "," << n << "," << flops/avg_time << "\n";
                }
            }
        }
    }

    return 0;
}