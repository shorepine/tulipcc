try:
    from ulab import numpy as np
except:
    import numpy as np

dtypes = (np.uint8, np.int8, np.uint16, np.int16, np.float)

print(np.ones(3))
print(np.ones((3,3)))

for dtype in dtypes:
    print(np.ones((3,3), dtype=dtype))
    print(np.ones((4,2), dtype=dtype))
