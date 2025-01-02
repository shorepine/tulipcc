# This file is part of the micropython-ulab project, https://github.com/v923z/micropython-ulab
#
# The MIT License (MIT)
#
# Copyright (c) 2022 Phil Jepsen

from ..core import (atleast_1d, asarray)
from ..core.overrides import set_module
from ulab import numpy as np

@set_module('numpy')
def poly(seq_of_zeros):
    seq_of_zeros = atleast_1d(seq_of_zeros)
    sh = seq_of_zeros.shape
    
    if len(sh) == 2 and sh[0] == sh[1] and sh[0] != 0:
        seq_of_zeros = np.linalg.eig(seq_of_zeros)
    elif len(sh) == 1:
        dt = seq_of_zeros.dtype
        # Let object arrays slip through, e.g. for arbitrary precision
        if dt != object:
            seq_of_zeros = seq_of_zeros #seq_of_zeros.astype(mintypecode(dt.char))
    else:
        raise ValueError("input must be 1d or non-empty square 2d array.")

    if len(seq_of_zeros) == 0:
        return 1.0
    dt = seq_of_zeros.dtype
    a = np.ones((1,), dtype=dt)

    for k in range(len(seq_of_zeros)):
        a = np.convolve(a, np.array([1, -seq_of_zeros[k]], dtype=dt))

    if a.dtype == np.complex:
        # if complex roots are all complex conjugates, the roots are real.
        roots = asarray(seq_of_zeros, complex)
        p = np.sort_complex(roots)
        c = np.real(p) - np.imag(p) * 1j
        q = np.sort_complex(c)
        if np.all(p == q):
            a = a.real.copy()
    return a