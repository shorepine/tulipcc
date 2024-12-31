# This file is part of the micropython-ulab project, https://github.com/v923z/micropython-ulab
#
# The MIT License (MIT)
#
# Copyright (c) 2022 Phil Jepsen

from .overrides import set_module
from .multiarray import asarray
from ulab import numpy as np
from ... import numpy

def prod(arr):
    result = 1
    for x in arr:
        result = result * x
    return result

def size(a, axis=None):
    """
    Return the number of elements along a given axis.
    Parameters
    ----------
    a : array_like
        Input data.
    axis : int, optional
        Axis along which the elements are counted.  By default, give
        the total number of elements.
    Returns
    -------
    element_count : int
        Number of elements along the specified axis.
    See Also
    --------
    shape : dimensions of array
    ndarray.shape : dimensions of array
    ndarray.size : number of elements in array
    Examples
    --------
    >>> a = np.array([[1,2,3],[4,5,6]])
    >>> np.size(a)
    6
    >>> np.size(a,1)
    3
    >>> np.size(a,0)
    2
    """
    if axis is None:
        try:
            return a.size
        except AttributeError:
            return asarray(a).size
    else:
        try:
            return a.shape[axis]
        except AttributeError:
            return asarray(a).shape[axis]

def nonzero(a):
    if not isinstance(a,(np.ndarray)):
        a = asarray(a)
    x = a.shape
    row = x[0]
    if len(x) == 1:
       column = 0
    else:
       column = x[1]

    nonzero_row = np.array([],dtype=np.float)
    nonzero_col = np.array([],dtype=np.float)

    if column == 0:
        for i in range(0,row):
           if a[i] != 0:
             nonzero_row = numpy.append(nonzero_row,i)
        return (np.array(nonzero_row, dtype=np.int8),)

    for i in range(0,row):
        for j in range(0,column):
            if a[i,j] != 0:
                nonzero_row = numpy.append(nonzero_row,i)
                nonzero_col = numpy.append(nonzero_col,j)
                
    return (np.array(nonzero_row, dtype=np.int8), np.array(nonzero_col, dtype=np.int8))