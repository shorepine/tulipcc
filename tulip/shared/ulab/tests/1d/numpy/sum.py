from ulab import numpy as np

r = range(15)

a = np.array(r, dtype=np.uint8)
print(np.sum(a))

a = np.array(r, dtype=np.int8)
print(np.sum(a))

a = np.array(r, dtype=np.uint16)
print(np.sum(a))

a = np.array(r, dtype=np.int16)
print(np.sum(a))

a = np.array(r, dtype=np.float)
print(np.sum(a))

a = np.array([False] + [True]*15, dtype=np.bool)
print(np.sum(a))
