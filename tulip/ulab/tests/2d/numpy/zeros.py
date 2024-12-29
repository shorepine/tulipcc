try:
    from ulab import numpy as np
except:
    import numpy as np

dtypes = (np.uint8, np.int8, np.uint16, np.int16, np.float)

print(np.zeros(3))
print(np.zeros((3,3)))

for dtype in dtypes:
    print(np.zeros((3,3), dtype=dtype))
    print(np.zeros((4,2), dtype=dtype))

try:
    np.zeros((1<<31, 1<<31))
except ValueError:
    print("ValueError")

try:
    np.zeros((2147483653, 2147483649))
except ValueError:
    print("ValueError")

try:
    np.zeros((194899806, 189294637612))
except ValueError:
    print("ValueError")
