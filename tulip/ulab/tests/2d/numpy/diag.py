try:
    from ulab import numpy as np
except:
    import numpy as np

a = np.arange(25).reshape((5,5))

print(np.diag(a))
print(np.diag(a, k=2))
print(np.diag(a, k=-2))
print(np.diag(a, k=10))
print(np.diag(a, k=-10))

a = np.arange(4)

print(np.diag(a))
print(np.diag(a, k=2))
print(np.diag(a, k=-2))


dtypes = (np.uint8, np.int8, np.uint16, np.int16, np.float)

for dtype in dtypes:
    a = np.array(range(4), dtype=dtype)
    print(np.diag(a))

for dtype in dtypes:
    a = np.array(range(16), dtype=dtype).reshape((4,4))
    print(np.diag(a))
