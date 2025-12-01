// io_bandwidth.cpp
#include <vector>
#include <fstream>
#include <chrono>
#include <iostream>
#include <iomanip>

using Clock = std::chrono::high_resolution_clock;

// Write a square matrix (column-major) of size n×n to filename.
// Returns duration in seconds.
double writeMatrixBinary(const std::string &filename,
                         const std::vector<double> &mat,
                         int n)
{
    std::ofstream out(filename, std::ios::binary | std::ios::out);
    auto t0 = Clock::now();
    out.write(reinterpret_cast<const char*>(mat.data()),
              sizeof(double)*mat.size());
    out.close();
    auto t1 = Clock::now();
    return std::chrono::duration<double>(t1 - t0).count();
}

// Read a square matrix from filename into mat.
// Returns duration in seconds.
double readMatrixBinary(const std::string &filename,
                        std::vector<double> &mat,
                        int n)
{
    std::ifstream in(filename, std::ios::binary | std::ios::in);
    auto t0 = Clock::now();
    in.read(reinterpret_cast<char*>(mat.data()),
            sizeof(double)*mat.size());
    in.close();
    auto t1 = Clock::now();
    return std::chrono::duration<double>(t1 - t0).count();
}

int main()
{
    const std::string fname = "tmp_matrix.bin";
    const int ntrial = 3;

    std::cout << "n,write_bw,read_bw\n";
    for(int n = 32; n <= 16384; n *= 2) {
        std::vector<double> mat(n*(size_t)n);
        // Fill with something (zeros are fine)
        std::fill(mat.begin(), mat.end(), 0.0);

        double sum_w = 0.0, sum_r = 0.0;
        size_t bytes = mat.size() * sizeof(double);

        for(int t = 0; t < ntrial; ++t) {
            // Write
            double dtw = writeMatrixBinary(fname, mat, n);
            sum_w += bytes / dtw;  // bytes per second

            // Read
            std::vector<double> buffer(n*(size_t)n);
            double dtr = readMatrixBinary(fname, buffer, n);
            sum_r += bytes / dtr;
        }
        // Average bandwidth
        double bw_w = sum_w / ntrial;
        double bw_r = sum_r / ntrial;

        // Print CSV line
        std::cout << n << ","
                  << std::fixed << std::setprecision(6) << bw_w << ","
                  << std::fixed << std::setprecision(6) << bw_r << "\n";

        // Clean up
        std::remove(fname.c_str());
    }
    return 0;
}
