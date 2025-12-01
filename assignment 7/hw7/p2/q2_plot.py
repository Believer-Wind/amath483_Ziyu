# q2_plot.py
import math
import matplotlib.pyplot as plt

# 手动将下面列表替换为你在终端运行 ./q2_zgesv 后复制的真实数据
sizes = [16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192]
residuals = [4.98228e-16, 7.93347e-16, 1.82246e-15, 2.94719e-15,
             4.73308e-15, 7.98731e-15, 1.53611e-14, 2.65907e-14,
             5.23041e-14, 1.03084e-13]
normalized_errs = [0.362231, 0.376051, 0.658828, 0.627657,
                   0.64737, 0.837603, 0.432081, 0.768613,
                   1.48831, 1.63889]

# 对数变换
log_res = [math.log10(r) for r in residuals]
log_err = [math.log10(e) for e in normalized_errs]

plt.figure()
plt.plot(sizes, log_res,  'o-', label='log10(Residual)')
plt.plot(sizes, log_err,  's--', label='log10(Normalized Error)')
plt.xscale('log', base=2)
plt.xlabel('Matrix Dimension N')
plt.ylabel('Value (log10 scale)')
plt.title('LAPACKE_zgesv: Residual & Normalized Error')
plt.grid(True, which='both')
plt.legend()
plt.xticks(sizes, rotation=45)
plt.tight_layout()
plt.savefig('q2_error.png')
print("Saved q2_error.png")
