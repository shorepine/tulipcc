from ulab import numpy as np


a = np.array(range(8))

print(np.where(a < 4, 1, 0))
print(np.where(a < 4, 2 * a, 0))

a = np.array(range(12)).reshape((3, 4))
print(np.where(a < 6, a, -1))

b = np.array(range(4))
print(np.where(a < 6, 10 + b, -1))

# test upcasting here
b = np.array(range(4), dtype=np.uint8)
c = np.array([25, 25, 25, 25], dtype=np.int16)
print(np.where(a < 6, b, c))
