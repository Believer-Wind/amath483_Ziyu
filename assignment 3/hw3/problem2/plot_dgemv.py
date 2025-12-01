# plot_dgemv.py
import pandas as pd
import matplotlib.pyplot as plt

# Load the performance data
df = pd.read_csv("dgemv_perf.csv")

# Plot
plt.figure(figsize=(8, 5))
plt.plot(df["n"], df["flops_per_sec"] / 1e9, marker='o', linestyle='-')
plt.xlabel("Matrix dimension n")
plt.ylabel("Performance (GFLOP/s)")
plt.title("DGEMV Performance vs Matrix Size")
plt.grid(True)
plt.tight_layout()
plt.savefig("dgemv_perf.png", dpi=300)
plt.show()
