# plot_dgemm.py
import pandas as pd
import matplotlib.pyplot as plt

# load data
df = pd.read_csv("dgemm_perf.csv")

plt.figure(figsize=(8, 5))
plt.plot(df["n"], df["flops_per_sec"]/1e9, marker='o', linestyle='-')
plt.xlabel("Matrix size n")
plt.ylabel("Performance (GFLOP/s)")
plt.title("DGEMM Performance vs n")
plt.grid(True)
plt.tight_layout()
plt.savefig("dgemm_perf.png", dpi=300)
plt.show()
