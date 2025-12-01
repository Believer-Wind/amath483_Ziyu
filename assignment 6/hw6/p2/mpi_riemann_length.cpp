// mpi_riemann_length.cpp

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <chrono>
#include <mpi.h>

// 被积函数：sqrt(1 + [f'(x)]^2)，其中 f'(x) = 1/x - x/4
inline double integrand(double x) {
    return std::sqrt(1.0 + std::pow(1.0/x - x/4.0, 2));
}

// 在区间 [a, b] 上做 n 个等距小区间的左端点 Riemann 求和
double riemann_segment(double a, double b, long long n) {
    double h = (b - a) / static_cast<double>(n);
    double sum = 0.0;
    for (long long i = 0; i < n; ++i) {
        double x = a + i * h;
        sum += integrand(x);
    }
    return sum * h;
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 2) {
        if (rank == 0)
            std::cerr << "Usage: " << argv[0] << " <npoints>\n";
        MPI_Finalize();
        return 1;
    }

    // 全局参数
    const double a = 1.0, b = 6.0;
    long long n = std::atoll(argv[1]);
    const double exact = 4.375 + std::log(6.0);

    // 每个进程负责的分点数和区间
    long long local_n = n / size;
    double local_a = a + rank * (b - a) / size;
    double local_b = a + (rank + 1) * (b - a) / size;

    // 全局同步，保证几乎同时开始计时
    MPI_Barrier(MPI_COMM_WORLD);
    auto t0 = std::chrono::high_resolution_clock::now();

    // 计算本地部分
    double local_sum = riemann_segment(local_a, local_b, local_n);

    // 汇总局部求和
    double global_sum = 0.0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE,
               MPI_SUM, 0, MPI_COMM_WORLD);

    // 计时结束并同步
    auto t1 = std::chrono::high_resolution_clock::now();
    MPI_Barrier(MPI_COMM_WORLD);

    // 计算耗时（纳秒）
    long long local_time = std::chrono::duration_cast<
        std::chrono::nanoseconds>(t1 - t0).count();

    // 收集所有进程的耗时到 rank 0
    std::vector<long long> times;
    if (rank == 0) times.resize(size);
    MPI_Gather(&local_time, 1, MPI_LONG_LONG,
               times.data(),   1, MPI_LONG_LONG,
               0, MPI_COMM_WORLD);

    if (rank == 0) {
        long long max_time = *std::max_element(times.begin(), times.end());
        double elapsed = max_time * 1e-9;
        double rel_err = std::abs(global_sum - exact) / exact;

        std::cout << "MPI Parallel: L=" << global_sum
                  << "  rel_err=" << rel_err
                  << "  time=" << elapsed << "s\n";
    }

    MPI_Finalize();
    return 0;
}

