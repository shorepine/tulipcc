
from ulab import numpy as np
from ..core.multiarray import (asarray)
from ..core.overrides import set_module

@set_module('numpy')

# This file is part of the micropython-ulab project, https://github.com/v923z/micropython-ulab
#
# The MIT License (MIT)
#
# Copyright (c) 2022 Phil Jepsen

def _isreal(a):
    result = []
    for x in a:
        if isinstance(x, float):
          result.append(True)
        elif isinstance(x, complex) and x.imag == 0:
              result.append(True)
        else:
          result.append(False)
    return result

def isreal(x):
    """
    Returns a bool array, where True if input element is real.
    If element has complex type with zero complex part, the return value
    for that element is True.
    Parameters
    ----------
    x : array_like
        Input array.
    Returns
    -------
    out : ndarray, bool
        Boolean array of same shape as `x`.
    Notes
    -----
    `isreal` may behave unexpectedly for string or object arrays (see examples)
    See Also
    --------
    iscomplex
    isrealobj : Return True if x is not a complex type.
    Examples
    --------
    >>> a = np.array([1+1j, 1+0j, 4.5, 3, 2, 2j], dtype=complex)
    >>> np.isreal(a)
    array([False,  True,  True,  True,  True, False])

    The function does not work on string arrays.
    >>> a = np.array([2j, "a"], dtype="U")
    >>> np.isreal(a)  # Warns about non-elementwise comparison
    False

    Returns True for all elements in input array of ``dtype=object`` even if
    any of the elements is complex.
    >>> a = np.array([1, "2", 3+4j], dtype=object)
    >>> np.isreal(a)
    array([ True,  True,  True])

    isreal should not be used with object arrays

    >>> a = np.array([1+2j, 2+1j], dtype=object)
    >>> np.isreal(a)
    array([ True,  True])
    """
    x = asarray(x)
    result =  _isreal(x)
    return result if len(result) > 1 else result[0]
