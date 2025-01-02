
scipy.special
=============

``scipy``\ ’s ``special`` module defines several functions that behave
as do the standard mathematical functions of the ``numpy``, i.e., they
can be called on any scalar, scalar-valued iterable (ranges, lists,
tuples containing numbers), and on ``ndarray``\ s without having to
change the call signature. In all cases the functions return a new
``ndarray`` of typecode ``float`` (since these functions usually
generate float values, anyway).

At present, ``ulab``\ ’s ``special`` module contains the following
functions:

``erf``, ``erfc``, ``gamma``, and ``gammaln``, and they can be called by
prepending them by ``scipy.special.``.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    from ulab import scipy as spy
    
    a = range(9)
    b = np.array(a)
    
    print('a: ', a)
    print(spy.special.erf(a))
    
    print('\nb: ', b)
    print(spy.special.erfc(b))

.. parsed-literal::

    a:  range(0, 9)
    array([0.0, 0.8427007929497149, 0.9953222650189527, 0.9999779095030014, 0.9999999845827421, 1.0, 1.0, 1.0, 1.0], dtype=float64)
    
    b:  array([0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0], dtype=float64)
    array([1.0, 0.1572992070502851, 0.004677734981047265, 2.209049699858544e-05, 1.541725790028002e-08, 1.537459794428035e-12, 2.151973671249892e-17, 4.183825607779414e-23, 1.122429717298293e-29], dtype=float64)
    
    

