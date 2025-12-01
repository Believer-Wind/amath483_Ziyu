import pandas as pd
import matplotlib.pyplot as plt

# Read both CSV outputs and combine
df0 = pd.read_csv('results_O0.csv')
df3 = pd.read_csv('results_O3.csv')

# Ensure opt_level column is correctly labeled
# If already present in CSV, this will overwrite; adjust if needed
for df, label in [(df0, 'O0'), (df3, 'O3')]:
    if 'opt_level' not in df.columns:
        df['opt_level'] = label

df = pd.concat([df0, df3], ignore_index=True)

# Plot performance for each loop permutation
for perm in ['jki', 'kij']:
    sub_df = df[df['perm'] == perm]
    plt.figure()
    for t in ['float', 'double']:
        for opt in sorted(sub_df['opt_level'].unique()):
            sel = sub_df[(sub_df['type'] == t) & (sub_df['opt_level'] == opt)]
            plt.plot(sel['n'], sel['flops_per_sec'], label=f'{t}_{opt}')
    plt.xlabel('Matrix dimension n')
    plt.ylabel('FLOPs per second')
    plt.title(f'Performance: {perm} permutation')
    plt.legend()
    plt.grid(True)
    plt.savefig(f'perf_{perm}.png')
