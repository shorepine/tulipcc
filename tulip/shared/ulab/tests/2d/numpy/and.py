try:
    from ulab import numpy as np
except ImportError:
    import numpy as np

dtypes = (np.uint8, np.int8, np.uint16, np.int16)

for dtype_a in dtypes:
    a = np.array(range(5), dtype=dtype_a)
    for dtype_b in dtypes:
        b = np.array(range(250, 255), dtype=dtype_b)
        try:
            print('a & b: ', a & b)
        except Exception as e:
            print(e)

    b = np.array([False, True, False, True, False], dtype=np.bool)
    try:
        print('a & b (bool): ', a & b)
    except Exception as e:
        print(e)
