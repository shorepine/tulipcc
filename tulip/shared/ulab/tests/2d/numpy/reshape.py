try:
    from ulab import numpy as np
except ImportError:
    import numpy as np

dtypes = (np.uint8, np.int8, np.uint16, np.int16, np.float)

for dtype in dtypes:
    print()
    print('=' * 50)
    a = np.array(range(12), dtype=dtype).reshape((3, 4))
    print(a)
    b = a[0,:]
    print(b.reshape((1,4)))
    b = a[:,0]
    print(b.reshape((1,3)))

