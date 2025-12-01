import matplotlib.pyplot as plt
import numpy as np


norm_values = np.array([0.25, 0.3125, 0.375, 0.4375, 0.5, 0.625, 0.75, 0.875, 1, 1.25, 1.5, 1.75, 2, 2.5, 3, 3.5, 4, 5, 6, 7, 8, 10, 12, 14])
denorm_values = np.array([0.0625, 0.125, 0.1875])
plt.scatter(norm_values, np.zeros_like(norm_values), marker='o', color='b', label='Normalized')
plt.scatter(denorm_values, np.zeros_like(denorm_values), marker='x', color='r', label='Denormalized')
plt.axvline(x=0, color='grey', linestyle='--')
plt.xlabel('Value')
plt.title('Representation of Floating-Point Values')
plt.legend()
plt.show()
