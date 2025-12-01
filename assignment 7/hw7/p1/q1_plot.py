# q1_plot.py

import matplotlib.pyplot as plt


# 手动将下面列表替换为你在终端运行 ./q1_blas 后复制的真实数据
sizes = [2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096]
daxpy = [0.00066346, 0.15894, 0.317881, 0.872727, 1.28, 2.25882, 3.49091, 3, 6.26939, 6.67101, 6.93063, 7.39128]
dgemv = [0.00185864, 0.427046, 1.39355, 4.29474, 11.9322, 22.8781, 0.00436777, 23.6733, 24.2608, 0.637251, 1.38514, 10.2751]
dgemm = [0.00199313, 1.77778, 9.84615, 35.2389, 53.5531, 54.4551, 46.6789, 148.701, 188.766, 196.827, 144.983, 138.781]

plt.figure()
plt.loglog(sizes, daxpy, 'o-', label='daxpy')
plt.loglog(sizes, dgemv, 's--', label='dgemv')
plt.loglog(sizes, dgemm, 'd-.', label='dgemm')
plt.xlabel('Problem Size N')
plt.ylabel('Performance (GFLOP/s)')
plt.title('OpenBLAS L1/L2/L3 Performance')
plt.grid(True, which='both')
plt.legend()
plt.xticks(sizes, rotation=90)
plt.tight_layout()
plt.savefig('q1_performance.png')
print("Saved q1_performance.png")
