#include "mem_swaps.hpp"
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <random>
#include <utility>
#include <vector>
#include <iostream>

constexpr int ntrial = 3;

// Return two distinct random integers in [0,n-1].
std::pair<int,int> getRandomIndices(int n)
{
    int i = std::rand() % n;
    int j = std::rand() % (n - 1);
    if (j >= i) ++j;
    return {i, j};
}

int main()
{
    std::ofstream csv("mem_swap_perf.csv");
    csv << "n,row_time,col_time\n";

    std::mt19937_64 rng(123);               // repeatability
    std::uniform_real_distribution<double>  dist(0.0, 1.0);

    for (int n = 16; n <= 4096; n *= 2)
    {
        const std::size_t N = static_cast<std::size_t>(n) * n;
        std::vector<double> A(N);

        // Fill with pseudorandom numbers
        for (double& x : A) x = dist(rng);

        /* ---------- Row‑swap timing ---------- */
        auto t0 = std::chrono::high_resolution_clock::now();
        for (int k = 0; k < ntrial; ++k)
        {
            auto [i,j] = getRandomIndices(n);
            swapRows(A, n, n, i, j);
        }
        auto t1 = std::chrono::high_resolution_clock::now();
        double rowTime =
            std::chrono::duration<double>(t1 - t0).count() / ntrial;

        /* ---------- Column‑swap timing ---------- */
        auto t2 = std::chrono::high_resolution_clock::now();
        for (int k = 0; k < ntrial; ++k)
        {
            auto [i,j] = getRandomIndices(n);
            swapCols(A, n, n, i, j);
        }
        auto t3 = std::chrono::high_resolution_clock::now();
        double colTime =
            std::chrono::duration<double>(t3 - t2).count() / ntrial;

        csv << n << ',' << rowTime << ',' << colTime << '\n';
        std::cout << "n=" << n
                  << "  row=" << rowTime
                  << "  col=" << colTime << '\n';
    }
}
