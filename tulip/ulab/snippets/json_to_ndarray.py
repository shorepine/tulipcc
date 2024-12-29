# This file is part of the micropython-ulab project, https://github.com/v923z/micropython-ulab
#
# The MIT License (MIT)
#
# Copyright (c) 2022 Zoltán Vörös

import sys

use_ulab = False

try:
    from ubinascii import a2b_base64 as b64decode
    from ubinascii import unhexlify
    import ujson as json
    from ulab import numpy as np
    use_ulab = True
except:
    from base64 import b64decode
    import json
    import numpy as np
    from numpy.lib.format import descr_to_dtype

def ulab_descr_to_dtype(descriptor):
    descriptor = descriptor[1:]

    if descriptor == 'u1':
        return np.uint8
    elif descriptor == 'i1':
        return np.int8
    if descriptor == 'u2':
        return np.uint16
    if descriptor == 'i2':
        return np.int16
    elif descriptor == 'f8':
        if np.float != ord('d'):
            raise TypeError('doubles are not supported')
        else:
            return np.float
    elif descriptor == 'f16':
        if np.float != ord('f'):
            raise TypeError('')
        else:
            return np.float
    else:
        raise TypeError('descriptor could not be decoded')


def json_to_ndarray(json_string, b64=True):
    """
    Turn a json string into an ndarray
    The string must be the representation of a dictionary with the three keys

    - dtype: a valid numpy dtype string (one of |u1, |i1, <u2, <i2, <f4, <f8, <c8, <c16, >u2, >i2, >f4, >f8, >c8, >c16)
    - array: the hexified, or base64-encoded raw data array
    - shape: the shape of the array (a list or tuple of integers)

    Usage:
        str = '{"dtype": "<f8", "array": "AAAAAAAAAAAAAAAAAADwPwAAAAAAAABAAAAAAAAACEAAAAAAAAAQQAAAAAAAABRAAAAAAAAAGEAAAAAAAAAcQAAAAAAAACBA\n", "shape": [3, 3]}'
        json_to_ndarray(str, b64=True)
    """
    obj = json.loads(json_string)

    if not isinstance(obj, dict):
         raise TypeError('input argument must be a dictionary')
    if set(obj.keys()) != {'array', 'dtype', 'shape'}:
        raise ValueError('input must have the keys "array", "dtype", "shape"')

    descriptor = obj['dtype']
    if use_ulab:
        dtype = ulab_descr_to_dtype(descriptor)
    else:
        dtype = descr_to_dtype(descriptor)

    if not b64:
        data = unhexlify(obj['array'])
    else:
        data = b64decode(obj['array'])

    ndarray = np.frombuffer(data, dtype=dtype).reshape(tuple(obj['shape']))

    if dtype in (np.uint16, np.int16, np.float):
        if sys.byteorder != descriptor[1]:
            ndarray.byteswap()

    return ndarray
