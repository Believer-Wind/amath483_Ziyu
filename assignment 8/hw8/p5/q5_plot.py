# q5_plot.py

import matplotlib.pyplot as plt

# -------------------------------------------------------------------
# 1) 在 Hyak 交互节点运行：
#      ./gradient_fftw  16  3
#      ./gradient_fftw  32  3
#      ./gradient_fftw  64  3
#      ./gradient_fftw 128  3
#      ./gradient_fftw 256  3
#   然后把输出的 “n,GFLOPS,avg_time” 那列中的 GFLOPS 值填到 fftw_gflops 列表里。
#
# 2) 同理在 GPU 节点执行：
#      ./gradient_cufft  16  3
#      ./gradient_cufft  32  3
#      ./gradient_cufft  64  3
#      ./gradient_cufft 128  3
#      ./gradient_cufft 256  3
#   把输出的 GFLOPS 填到 cufft_gflops 列表里。
#
# 下面预留 5 个格点 n = [16, 32, 64, 128, 256]
# -------------------------------------------------------------------

sizes = [16, 32, 64, 128, 256]

# 手动填入 FFTW 在各个 n 下测得的 GFLOPS:
fftw_gflops = [8.81693, 9.17245, 7.69196, 7.54409, 6.86002]

# 手动填入 cuFFT 在各个 n 下测得的 GFLOPS:
cufft_gflops = [0.104741, 0.407336, 167.441, 198.701, 238.533]

# 将 GFLOPS 转换为 FLOP/s（乘以 1e9）
fftw_flops = [g * 1e9 for g in fftw_gflops]
cufft_flops = [g * 1e9 for g in cufft_gflops]

# 构造 x 轴标签为 “n³” 的形式
xtick_labels = [f"{n}³" for n in sizes]

plt.figure(figsize=(8, 6))
plt.loglog(sizes, fftw_flops,  'o-', label='FFTW (CPU)')
plt.loglog(sizes, cufft_flops, 's--', label='cuFFT (GPU)')
plt.xlabel('Grid Size n (3D problem on $n^3$ points)')
plt.ylabel('Performance (FLOP/s)')
plt.title('3D Plane-Wave Gradient via FFT: FFTW vs. cuFFT')
plt.grid(True, which='both', linestyle=':')
plt.legend()
plt.xticks(sizes, xtick_labels, rotation=0)
plt.savefig('q5_performance.png')
print("Saved q5_performance.png")
