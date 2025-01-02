
scipy.signal
============

This module defines the single function:

1. `scipy.signal.sosfilt <#sosfilt>`__

sosfilt
-------

``scipy``:
https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.sosfilt.html

Filter data along one dimension using cascaded second-order sections.

The function takes two positional arguments, ``sos``, the filter
segments of length 6, and the one-dimensional, uniformly sampled data
set to be filtered. Returns the filtered data, or the filtered data and
the final filter delays, if the ``zi`` keyword arguments is supplied.
The keyword argument must be a float ``ndarray`` of shape
``(n_sections, 2)``. If ``zi`` is not passed to the function, the
initial values are assumed to be 0.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    from ulab import scipy as spy
    
    x = np.array([0, 1, 2, 3, 4, 5, 6, 7, 8, 9])
    sos = [[1, 2, 3, 1, 5, 6], [1, 2, 3, 1, 5, 6]]
    y = spy.signal.sosfilt(sos, x)
    print('y: ', y)

.. parsed-literal::

    y:  array([0.0, 1.0, -4.0, 24.0, -104.0, 440.0, -1728.0, 6532.000000000001, -23848.0, 84864.0], dtype=float)
    
    


.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    from ulab import scipy as spy
    
    x = np.array([0, 1, 2, 3, 4, 5, 6, 7, 8, 9])
    sos = [[1, 2, 3, 1, 5, 6], [1, 2, 3, 1, 5, 6]]
    # initial conditions of the filter
    zi = np.array([[1, 2], [3, 4]])
    
    y, zf = spy.signal.sosfilt(sos, x, zi=zi)
    print('y: ', y)
    print('\n' + '='*40 + '\nzf: ', zf)

.. parsed-literal::

    y:  array([4.0, -16.0, 63.00000000000001, -227.0, 802.9999999999999, -2751.0, 9271.000000000001, -30775.0, 101067.0, -328991.0000000001], dtype=float)
    
    ========================================
    zf:  array([[37242.0, 74835.0],
    	 [1026187.0, 1936542.0]], dtype=float)
    
    

