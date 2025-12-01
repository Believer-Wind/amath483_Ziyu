// riemann_length.cpp
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <cmath>
#include <chrono>

// 原函数导数 f'(x) = 1/x - x/4
// 曲线长度 L = ∫_a^b sqrt(1 + [f'(x)]^2) dx
inline double integrand(double x) {
    return std::sqrt(1.0 + std::pow(1.0/x - x/4.0, 2));
}

// 串行 Riemann 和
double riemann_serial(double a, double b, long long n) {
    double h = (b - a) / static_cast<double>(n);
    double sum = 0.0;
    for (long long i = 0; i < n; ++i) {
        double x = a + i * h;
        sum += integrand(x);
    }
    return sum * h;
}

// 每个线程计算 partial sum 的函数
void worker(double& global_sum, std::mutex& mtx,
            double a, double h, long long n,
            int num_threads, int tid)
{
    long long chunk = n / num_threads;
    long long start = tid * chunk;
    long long end   = (tid == num_threads - 1) ? n : start + chunk;
    double local = 0.0;
    for (long long i = start; i < end; ++i) {
        double x = a + i * h;
        local += integrand(x);
    }
    local *= h;
    // 合并到全局
    std::lock_guard<std::mutex> lock(mtx);
    global_sum += local;
}

// 并行 Riemann 和
double riemann_parallel(double a, double b, long long n, int num_threads) {
    double h = (b - a) / static_cast<double>(n);
    double total = 0.0;
    std::mutex mtx;
    std::vector<std::thread> threads;
    threads.reserve(num_threads);
    for (int t = 0; t < num_threads; ++t) {
        threads.emplace_back(worker,
                             std::ref(total), std::ref(mtx),
                             a, h, n, num_threads, t);
    }
    for (auto& th : threads) th.join();
    return total;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0]
                  << " <nthreads> <npoints>\n";
        return 1;
    }
    int nt = std::atoi(argv[1]);
    long long n = std::atoll(argv[2]);
    const double a = 1.0, b = 6.0;
    // 解析解：L = (1/4)[ x^2/2 + 4 ln x ]_1^6 = 4.375 + ln(6)
    const double exact = 4.375 + std::log(6.0);

    // 串行计时
    auto t0 = std::chrono::high_resolution_clock::now();
    double s_val = riemann_serial(a, b, n);
    auto t1 = std::chrono::high_resolution_clock::now();
    double t_serial = std::chrono::duration<double>(t1 - t0).count();
    double err_s = std::abs((s_val - exact) / exact);
    std::cout << "Serial:   L=" << s_val
              << "  rel_err=" << err_s
              << "  time=" << t_serial << "s\n";

    // 并行计时
    t0 = std::chrono::high_resolution_clock::now();
    double p_val = riemann_parallel(a, b, n, nt);
    t1 = std::chrono::high_resolution_clock::now();
    double t_parallel = std::chrono::duration<double>(t1 - t0).count();
    double err_p = std::abs((p_val - exact) / exact);
    std::cout << "Parallel: L=" << p_val
              << "  rel_err=" << err_p
              << "  time=" << t_parallel << "s\n";

    return 0;
}
