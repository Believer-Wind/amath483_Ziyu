import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

df = pd.read_csv("mem_swap_perf.csv")

plt.figure()
plt.plot(df["n"], np.log10(df["row_time"]), marker="o",
         label="Row swap")
plt.plot(df["n"], np.log10(df["col_time"]), marker="s",
         label="Column swap")

plt.xscale("log", base=2)
plt.xlabel("Matrix dimension n")
plt.ylabel("log10(Time per swap, seconds)")
plt.title("Row vs. Column swap timing (column‑major storage)")
plt.grid(True, which="both", ls="--", alpha=0.6)
plt.legend()
plt.tight_layout()
plt.savefig("mem_swap_plot.png", dpi=300)
print("Saved mem_swap_plot.png")
