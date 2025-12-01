import csv
import matplotlib.pyplot as plt

# Read data
ns, w_bw, r_bw = [], [], []
with open("io_bw.csv") as f:
    reader = csv.DictReader(f)
    for row in reader:
        ns.append(int(row["n"]))
        w_bw.append(float(row["write_bw"]))
        r_bw.append(float(row["read_bw"]))

# Plot
plt.figure()
plt.plot(ns, w_bw, marker='o', label='Write bandwidth')
plt.plot(ns, r_bw, marker='s', label='Read bandwidth')
plt.xscale('log', base=2)
plt.yscale('log')
plt.xlabel('Matrix dimension n')
plt.ylabel('Bandwidth (bytes/sec)')
plt.title('I/O Bandwidth vs. Matrix Size')
plt.legend()
plt.grid(True, which='both', linestyle='--', linewidth=0.5)
plt.tight_layout()
plt.savefig("io_bandwidth.png", dpi=300)
