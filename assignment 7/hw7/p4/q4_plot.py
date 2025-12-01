# q4_plot.py

import matplotlib.pyplot as plt

# -------------------------------------------------------------------
# 手动将下面列表替换为你在终端运行 ./q4_copy_bw 后复制的真实数据
sizes = [
    8, 16, 32, 64, 128, 256, 512, 1024,
    2048, 4096, 8192, 16384, 32768, 65536,
    131072, 262144, 524288, 1048576,
    2097152, 4194304, 8388608, 16777216,
    33554432, 67108864, 134217728, 268435456
]

h2d_bw = [0.665045, 1.77725, 3.59064, 7.15703, 14.1442, 28.5531,
          57.9081, 113.49, 218.061, 397.106, 693.976, 1089.18,
          1539.35, 1940.9, 2795.17, 3258.2, 3458.08, 3607.42,
          4390.4, 4918.35, 5068.07, 5018.39, 5099.73, 5116.72,
          5141.12, 5178.45]

d2h_bw = [0.757365, 1.60605, 3.23279, 6.44157, 12.9928, 26.1908,
          51.9714, 98.7944, 200.576, 381.231, 662.705, 1085.07,
          1603.81, 2093.61, 2429.41, 2701.7, 2881.46, 2984.95,
          3434.92, 3552.35, 3737.73, 3827.92, 3863.17, 3883.66,
          3884.18, 3902.38]
# -------------------------------------------------------------------

plt.figure()
# plt.loglog(sizes, h2d_bw, 'o-', label='Host→Device (H2D)')
# plt.loglog(sizes, d2h_bw, 's--', label='Device→Host (D2H)')
plt.plot(sizes, h2d_bw, 'o-', label='Host→Device (H2D)')
plt.plot(sizes, d2h_bw, 's--', label='Device→Host (D2H)')
plt.xscale('log', base=2)
plt.xlabel('Transfer Size (bytes)')
plt.ylabel('Bandwidth (MB/s)')
plt.title('CPU↔GPU Transfer Bandwidth')
plt.grid(True, which='both', linestyle=':')
plt.xticks(sizes, rotation=90)
plt.legend()
plt.tight_layout()
plt.savefig('q4_bandwidth.png')
print("Saved q4_bandwidth.png")

