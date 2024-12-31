try:
    from ulab import numpy as np
except:
    import numpy as np

dtypes = (np.uint8, np.int8, np.uint16, np.int16, np.float)

for dtype in dtypes:
    print(np.arange(10, dtype=dtype))
    print(np.arange(2, 10, dtype=dtype))
    print(np.arange(2, 10, 3, dtype=dtype))
    # test empty range
    print(np.arange(0, 0, dtype=dtype))

# test for ZeroDivisionError exception
try:
    np.arange(0, 10, 0)
except ZeroDivisionError as e:
    print('ZeroDivisionError: ', e)

# test for NAN length exception
try:
    np.arange(0, np.nan)
except ValueError as e:
    print('ValueError: ', e)
