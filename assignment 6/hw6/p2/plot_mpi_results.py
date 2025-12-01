# plot_mpi_results.py
import math
import matplotlib.pyplot as plt

# 手动填入数据（用你自己测得的数值替换）
points = [10, 100, 1000, 10000, 100000, 1000000]
errors = [0.0161601, 0.00165633, 0.000168588, 1.68884e-05, 1.68913e-06, 1.68916e-07]
log_errors = [math.log(e) for e in errors]

mpi_tasks = [1, 2, 4, 8, 16, 32]
times = [1.22068, 0.599027, 0.324864, 0.151086, 0.0780165, 0.0475977]

# 绘制 Points vs log(Error)
plt.figure()
plt.plot(points, log_errors, 'r-o')
# plt.xscale('log')
plt.xlabel('Number of points (log scale)')
plt.ylabel('log(Relative Error)')
plt.title('MPI: Points vs log(Error)')
plt.grid(True)
plt.savefig('mpi_error_convergence.png')

# 绘制 MPI tasks vs Time
plt.figure()
plt.plot(mpi_tasks, times, '-o')
plt.xlabel('MPI Tasks')
plt.ylabel('Time (s)')
plt.title('MPI: Tasks vs Time')
plt.grid(True)
plt.savefig('mpi_strong_scaling.png')
