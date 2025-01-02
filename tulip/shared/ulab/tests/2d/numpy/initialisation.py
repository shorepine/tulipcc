try:
    from ulab import numpy as np
except ImportError:
    import numpy as np

dtypes = (np.uint8, np.int8, np.uint16, np.int16, np.float)

for dtype1 in dtypes:
    for dtype2 in dtypes:
        print(np.array(np.array(range(5), dtype=dtype1), dtype=dtype2))