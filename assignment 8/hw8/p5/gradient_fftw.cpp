// gradient_fftw.cpp
#include <iostream>
#include <cmath>
#include <chrono>
#include <fftw3.h>

using Clock = std::chrono::high_resolution_clock;

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " n ntrial\n";
        return 1;
    }
    int n      = std::atoi(argv[1]);   // 格点数 n （即立方体维度）
    int ntrial = std::atoi(argv[2]);   // 重复次数

    // 总点数
    long N = 1L * n * n * n;

    // 分配 FFTW 复数数组（FFTW 使用 fftw_complex）
    fftw_complex *data = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N);
    fftw_complex *kdata = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N);

    // 生成“平面波” f(i,j,k)=exp(i*2π*(i+j+k)/n)，放到 data 中
    const double TWO_PI = 2.0 * M_PI;
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            for(int k = 0; k < n; ++k) {
                long idx = ( (long)i * n + j ) * n + k;
                double phase = TWO_PI * (i + j + k) / n;
                data[idx][0] = std::cos(phase);   // 实部
                data[idx][1] = std::sin(phase);   // 虚部
            }
        }
    }

    // 创建 3D FFTW 计划（正变换 & 逆变换），FFTW_MEASURE 会事先测量最优算法
    fftw_plan plan_fwd = fftw_plan_dft_3d(n, n, n, data, kdata, FFTW_FORWARD, FFTW_MEASURE);
    fftw_plan plan_bwd = fftw_plan_dft_3d(n, n, n, kdata, data, FFTW_BACKWARD, FFTW_MEASURE);

    // “热身”调用：一次正变换 + 一次逆变换，剔除计划初始化开销
    fftw_execute(plan_fwd);
    fftw_execute(plan_bwd);

    // 测时循环
    double total_time = 0.0;
    for(int t = 0; t < ntrial; ++t) {
        auto t0 = Clock::now();

        // 1) 正变换
        fftw_execute(plan_fwd);

        // 2) 在频域乘以 (i * kx)，这里只演示 ∂/∂x 的一个方向：
        //    - kx = 2π * ki / n, 其中 ki = i ≤ n/2 ? i : (i - n)
        for(int i = 0; i < n; ++i) {
            int ki = (i <= n/2 ? i : i - n);
            double kx = TWO_PI * ki / n;
            for(int j = 0; j < n; ++j) {
                for(int k = 0; k < n; ++k) {
                    long idx = ((long)i * n + j) * n + k;
                    double re = kdata[idx][0];
                    double im = kdata[idx][1];
                    // 乘以 i*kx： (re + i im) * i*kx = -kx*im  + i*(kx*re)
                    kdata[idx][0] = -kx * im;
                    kdata[idx][1] =  kx * re;
                }
            }
        }

        // 3) 逆变换
        fftw_execute(plan_bwd);

        auto t1 = Clock::now();
        total_time += std::chrono::duration<double>(t1 - t0).count();
    }

    double avg_time = total_time / ntrial;

    // FLOP 计数近似：
    //   - 一个 N 点复数 3D FFT 约需 ≃ 5 N log2 N 浮点运算
    //   - 做正变换 + 逆变换 共 2 次 → 2 * (5 N log2 N) = 10 N log2 N
    //   - 频域乘以 i*kx 时，对每个点一次复数乘法约 6 次浮点运算 → 总计 6 N
    //   合计 ≃ (10 N log2 N + 6 N) 浮点运算
    double flop_count = 10.0 * N * std::log2((double)N) + 6.0 * N;
    double gflops = flop_count / avg_time / 1e9;

    // 输出 CSV：n, FLOPs, 平均时间（秒）
    std::cout << n << "," << gflops << "," << avg_time << "\n";

    // 清理
    fftw_destroy_plan(plan_fwd);
    fftw_destroy_plan(plan_bwd);
    fftw_free(data);
    fftw_free(kdata);

    return 0;
}
