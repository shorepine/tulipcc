from ulab import numpy as np

a = np.array(range(12)).reshape((3, 4))

print(np.all(a))
print(np.all(a, axis=0))
print(np.all(a, axis=1))

print(np.any(a))
print(np.any(a, axis=0))
print(np.any(a, axis=1))