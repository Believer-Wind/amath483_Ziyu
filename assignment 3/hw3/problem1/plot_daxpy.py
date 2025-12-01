import pandas as pd
import matplotlib.pyplot as plt

# load data
df = pd.read_csv("daxpy_perf.csv")

# plot
plt.figure(figsize=(6, 4))
plt.plot(df["n"], df["flops_per_sec"]/1e9, marker="o", linestyle="-")
plt.xlabel("Vector length $n$")
plt.ylabel("Performance (GFLOP/s)")
plt.title("DAXPY Performance vs. Problem Size")
plt.grid(True)
plt.tight_layout()
plt.savefig("daxpy_perf.png", dpi=300)
plt.show()
