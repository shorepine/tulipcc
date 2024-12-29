try:
    from ulab import numpy as np
except:
    import numpy as np

dtypes = (np.uint8, np.int8, np.uint16, np.int16, np.float)

for dtype in dtypes:
    a = np.ones((2, 2), dtype=dtype)
    print()
    for _dtype in dtypes:
        b = np.asarray(a, dtype=_dtype)
        print('a: ', a)
        print('b: ', b)
        print('a is b: {}\n'.format(a is b))
