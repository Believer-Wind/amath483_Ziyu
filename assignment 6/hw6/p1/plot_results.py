# plot_results.py
import matplotlib.pyplot as plt
import math

# 这里假设你已经把运行结果记录成两个列表：
threads = [1, 2, 4, 8, 16]
times_parallel = [1.50154, 0.780942, 0.409269, 0.320756, 0.306943]
points  = [10, 100, 1e3, 1e4, 1e5, 1e6]
errors = [0.0136834, 0.00165633, 0.000168588, 1.68884e-05, 1.68913e-06, 1.68916e-07]
log_errors = [math.log(e) for e in errors]

# 强缩放图
plt.figure()
plt.plot(threads, times_parallel, '-o')
plt.xlabel('Threads')
plt.ylabel('Time (s)')
plt.title('Strong Scaling')
plt.grid(True)
plt.savefig('scaling.png')

# 误差收敛图
plt.figure()
plt.plot(points, log_errors, 'r-o')
# plt.xscale('log')
plt.xlabel('Number of points (log scale)')
plt.ylabel('log(Relative Error)')
plt.title('Error vs Points')
plt.grid(True)
plt.savefig('error_convergence.png')
