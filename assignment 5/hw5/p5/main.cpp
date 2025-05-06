#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <fstream>

// forward declaration of Strassen
template <typename T>
std::vector<std::vector<T>> strassenMultiply(
    const std::vector<std::vector<T>>&,
    const std::vector<std::vector<T>>&);

int main() {
    std::mt19937_64 rng(123);
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    const int ntrial = 3;

    std::ofstream out("results.csv");
    out << "n,flops_per_sec" << '\n';

    for(int n = 2; n <= 512; n += 2) {
        std::vector<std::vector<double>> A(n, std::vector<double>(n));
        std::vector<std::vector<double>> B(n, std::vector<double>(n));
        for(int i = 0; i < n; i++)
            for(int j = 0; j < n; j++) {
                A[i][j] = dist(rng);
                B[i][j] = dist(rng);
            }
        // warmup
        auto C0 = strassenMultiply(A, B);
        double total_time = 0.0;
        for(int t = 0; t < ntrial; t++) {
            auto t1 = std::chrono::high_resolution_clock::now();
            auto C  = strassenMultiply(A, B);
            auto t2 = std::chrono::high_resolution_clock::now();
            total_time += std::chrono::duration<double>(t2 - t1).count();
        }
        double avg = total_time / ntrial;
        double flops = 2.0 * n * n * n;
        out << n << "," << flops / avg << '\n';
    }

    out.close();
    std::cout << "Results written to results.csv";
    return 0;
}