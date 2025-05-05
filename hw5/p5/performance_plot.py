import matplotlib.pyplot as plt

# Default input file
file_in = "results.csv"

ns = []
mflops = []
with open(file_in) as f:
    for line in f:
        # skip header or comments
        if line.startswith('#') or line.startswith('n,'):
            continue
        n, v = line.strip().split(',')
        ns.append(float(n))
        mflops.append(float(v) / 1e6)

plt.figure()
plt.loglog(ns, mflops, marker='o')
plt.xlabel('Matrix dimension n')
plt.ylabel('MFLOPS')
plt.title('Strassen Performance')
plt.grid(True, which='both', linestyle='--')
plt.savefig('strassen_performance.png', dpi=300)
print('Saved plot as strassen_performance.png')
