# This file is part of the micropython-ulab project, https://github.com/v923z/micropython-ulab
#
# The MIT License (MIT)
#
# Copyright (c) 2022 Phil Jepsen

from ulab import numpy as np
from ..core.multiarray import (asarray)
from ..core.overrides import set_module

@set_module('numpy')
def append(arr, values, axis=None):
    arr = asarray(arr)
    values = asarray(values)
    if axis is None:
        if len(arr.shape) != 1:
            arr = arr.flatten()
        values = values.flatten()
        axis = len(arr.shape)-1
    return np.concatenate((arr, values), axis=axis)
