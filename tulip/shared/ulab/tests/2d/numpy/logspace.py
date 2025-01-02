try:
    from ulab import numpy as np
except:
    import numpy as np

dtypes = (np.uint8, np.int8, np.uint16, np.int16, np.float)

for dtype in dtypes:
    print(np.logspace(0, 10, num=5, endpoint=False, dtype=dtype))
    print(np.logspace(0, 10, num=5, endpoint=True, dtype=dtype))