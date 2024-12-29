try:
    from ulab import numpy as np
except:
    import numpy as np

dtypes = (np.uint8, np.int8, np.uint16, np.int16, np.float)

print(np.ones(3))
print(np.ones((3,3)))

print(np.eye(3))
print(np.eye(3, M=4))
print(np.eye(3, M=4, k=0))
print(np.eye(3, M=4, k=-1))
print(np.eye(3, M=4, k=-2))
print(np.eye(3, M=4, k=-3))
print(np.eye(3, M=4, k=1))
print(np.eye(3, M=4, k=2))
print(np.eye(3, M=4, k=3))
print(np.eye(4, M=4))
print(np.eye(4, M=3, k=0))
print(np.eye(4, M=3, k=-1))
print(np.eye(4, M=3, k=-2))
print(np.eye(4, M=3, k=-3))
print(np.eye(4, M=3, k=1))
print(np.eye(4, M=3, k=2))
print(np.eye(4, M=3, k=3))

for dtype in dtypes:
    print(np.eye(3, dtype=dtype))