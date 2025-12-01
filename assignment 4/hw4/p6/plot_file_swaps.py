import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('swap_times.csv')
plt.figure()
plt.plot(df['n'], df['row_time'], marker='o', label='row-swap')
plt.plot(df['n'], df['col_time'], marker='s', label='col-swap')
plt.xscale('log', base=2)
plt.yscale('log', base=10)
plt.xlabel('Matrix dimension n')
plt.ylabel('Time (s) [log10 scale]')
plt.title('File-based Row vs Column Swap Times')
plt.legend()
plt.grid(True, which='both', ls='--', lw=0.5)
plt.savefig('swap_times.png')
