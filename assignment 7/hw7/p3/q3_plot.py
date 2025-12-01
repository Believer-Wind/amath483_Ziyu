# q3_plot.py

import matplotlib.pyplot as plt

# 手动粘贴运行结果到这几个列表：
sizes = [2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384]

# 从 terminal 复制出来的 CPU_GFLOPS 列表（14 个值）
cpu_flops = [0.16, 1.37143, 9.79037, 34.1333, 72.3597, 35.7713, 111.077,
             176.595, 232.931, 300.404, 307.851, 320.956, 277.617, 324.736]

# 从 terminal 复制出来的 GPU_GFLOPS 列表（14 个值）
gpu_flops = [0.00205667, 0.0195313, 0.140625, 1.0625, 1.15895, 5.18219, 38.057,
             308.159, 351.159, 364.85, 395.377, 546.183, 544.793, 532.673]

plt.figure()
# plt.loglog(sizes, cpu_flops,  'o-', label='OpenBLAS (CPU)')
# plt.loglog(sizes, gpu_flops,  's--', label='cuBLAS (GPU)')
plt.plot(sizes, cpu_flops,  'o-', label='OpenBLAS (CPU)')
plt.plot(sizes, gpu_flops,  's--', label='cuBLAS (GPU)')
plt.xscale('log', base=2)
plt.xlabel('Matrix Dimension N')
plt.ylabel('Performance (GFLOP/s)')
plt.title('OpenBLAS vs. cuBLAS GEMM Performance')
plt.grid(True, which='both')
plt.legend()
plt.xticks(sizes, rotation=45)
plt.tight_layout()
plt.savefig('q3_performance.png')
print("Saved q3_performance.png")
