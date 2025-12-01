#include "file_swaps.hpp"
#include <fstream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <iostream>
#include <random>

int main(int argc, char* argv[]) {
    // Usage: ./file_swaps_test results.csv
    std::string out_csv = (argc > 1 ? argv[1] : "swap_times.csv");
    std::ofstream csv(out_csv);
    csv << "n,row_time,col_time\n";

    std::mt19937_64 rng(12345);
    for (int n = 16; n <= 8192; n *= 2) {
        std::vector<double> M(n * n);
        // initialize column-major
        for (int col = 0; col < n; ++col)
            for (int row = 0; row < n; ++row)
                M[col*n + row] = row + col*n;

        const std::string fname = "tmp_swap.bin";
        // write initial matrix
        {
            std::ofstream out(fname, std::ios::binary);
            out.write(reinterpret_cast<const char*>(M.data()), n*n*sizeof(double));
        }

        // pick two indices to swap (fixed for consistency)
        int i = 0, j = n-1;
        const int ntrial = 3;

        // measure row-swap
        double total_row = 0;
        for (int t = 0; t < ntrial; ++t) {
            std::fstream fs(fname, std::ios::in|std::ios::out|std::ios::binary);
            auto t0 = std::chrono::high_resolution_clock::now();
            swapRowsInFile(fs, n, n, i, j);
            auto t1 = std::chrono::high_resolution_clock::now();
            total_row += std::chrono::duration<double>(t1 - t0).count();
            fs.close();
        }

        // measure col-swap
        double total_col = 0;
        for (int t = 0; t < ntrial; ++t) {
            std::fstream fs(fname, std::ios::in|std::ios::out|std::ios::binary);
            auto t0 = std::chrono::high_resolution_clock::now();
            swapColsInFile(fs, n, n, i, j);
            auto t1 = std::chrono::high_resolution_clock::now();
            total_col += std::chrono::duration<double>(t1 - t0).count();
            fs.close();
        }

        double avg_row = total_row / ntrial;
        double avg_col = total_col / ntrial;
        csv << n << "," << avg_row << "," << avg_col << "\n";
        std::remove(fname.c_str());
        std::cout << "n="<<n
                  << " row="<<avg_row<<"s"
                  << " col="<<avg_col<<"s\n";
    }
    return 0;
}