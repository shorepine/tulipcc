from ulab import numpy as np

a = np.array([1, 2, 3, 4, 5], dtype=np.uint8)
b = np.array([5, 4, 3, 2, 1], dtype=np.float)
print(np.minimum(a, b))
print(np.maximum(a, b))
print(np.maximum(1, 5.5))

a = np.array(range(9), dtype=np.uint8)
print(np.clip(a, 3, 7))

b = 3 * np.ones(len(a), dtype=np.float)
print(np.clip(a, b, 7))
