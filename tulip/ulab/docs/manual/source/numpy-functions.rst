
Numpy functions
===============

This section of the manual discusses those functions that were adapted
from ``numpy``. Functions with an asterisk accept complex arrays as
arguments, if the firmware was compiled with complex support.

1.  `numpy.all\* <#all>`__
2.  `numpy.any\* <#any>`__
3.  `numpy.argmax <#argmax>`__
4.  `numpy.argmin <#argmin>`__
5.  `numpy.argsort <#argsort>`__
6.  `numpy.asarray\* <#asarray>`__
7.  `numpy.bitwise_and <#bitwise_and>`__
8.  `numpy.bitwise_or <#bitwise_and>`__
9.  `numpy.bitwise_xor <#bitwise_and>`__
10. `numpy.clip <#clip>`__
11. `numpy.compress\* <#compress>`__
12. `numpy.conjugate\* <#conjugate>`__
13. `numpy.convolve\* <#convolve>`__
14. `numpy.delete <#delete>`__
15. `numpy.diff <#diff>`__
16. `numpy.dot <#dot>`__
17. `numpy.equal <#equal>`__
18. `numpy.flip\* <#flip>`__
19. `numpy.imag\* <#imag>`__
20. `numpy.interp <#interp>`__
21. `numpy.isfinite <#isfinite>`__
22. `numpy.isinf <#isinf>`__
23. `numpy.left_shift <#left_shift>`__
24. `numpy.load <#load>`__
25. `numpy.loadtxt <#loadtxt>`__
26. `numpy.max <#max>`__
27. `numpy.maximum <#maximum>`__
28. `numpy.mean <#mean>`__
29. `numpy.median <#median>`__
30. `numpy.min <#min>`__
31. `numpy.minimum <#minimum>`__
32. `numpy.nozero <#nonzero>`__
33. `numpy.not_equal <#equal>`__
34. `numpy.polyfit <#polyfit>`__
35. `numpy.polyval <#polyval>`__
36. `numpy.real\* <#real>`__
37. `numpy.right_shift <#right_shift>`__
38. `numpy.roll <#roll>`__
39. `numpy.save <#save>`__
40. `numpy.savetxt <#savetxt>`__
41. `numpy.size <#size>`__
42. `numpy.sort <#sort>`__
43. `numpy.sort_complex\* <#sort_complex>`__
44. `numpy.std <#std>`__
45. `numpy.sum <#sum>`__
46. `numpy.take\* <#take>`__
47. `numpy.trace <#trace>`__
48. `numpy.trapz <#trapz>`__
49. `numpy.where <#where>`__

all
---

``numpy``:
https://numpy.org/doc/stable/reference/generated/numpy.all.html

The function takes one positional, and one keyword argument, the
``axis``, with a default value of ``None``, and tests, whether *all*
array elements along the given axis evaluate to ``True``. If the keyword
argument is ``None``, the flattened array is inspected.

Elements of an array evaluate to ``True``, if they are not equal to
zero, or the Boolean ``False``. The return value if a Boolean
``ndarray``.

If the firmware was compiled with complex support, the function can
accept complex arrays.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array(range(12)).reshape((3, 4))
    
    print('\na:\n', a)
    
    b = np.all(a)
    print('\nall of the flattened array:\n', b)
    
    c = np.all(a, axis=0)
    print('\nall of a along 0th axis:\n', c)
    
    d = np.all(a, axis=1)
    print('\nall of a along 1st axis:\n', d)

.. parsed-literal::

    
    a:
     array([[0.0, 1.0, 2.0, 3.0],
           [4.0, 5.0, 6.0, 7.0],
           [8.0, 9.0, 10.0, 11.0]], dtype=float64)
    
    all of the flattened array:
     False
    
    all of a along 0th axis:
     array([False, True, True, True], dtype=bool)
    
    all of a along 1st axis:
     array([False, True, True], dtype=bool)
    
    


any
---

``numpy``:
https://numpy.org/doc/stable/reference/generated/numpy.any.html

The function takes one positional, and one keyword argument, the
``axis``, with a default value of ``None``, and tests, whether *any*
array element along the given axis evaluates to ``True``. If the keyword
argument is ``None``, the flattened array is inspected.

Elements of an array evaluate to ``True``, if they are not equal to
zero, or the Boolean ``False``. The return value if a Boolean
``ndarray``.

If the firmware was compiled with complex support, the function can
accept complex arrays.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array(range(12)).reshape((3, 4))
    
    print('\na:\n', a)
    
    b = np.any(a)
    print('\nany of the flattened array:\n', b)
    
    c = np.any(a, axis=0)
    print('\nany of a along 0th axis:\n', c)
    
    d = np.any(a, axis=1)
    print('\nany of a along 1st axis:\n', d)

.. parsed-literal::

    
    a:
     array([[0.0, 1.0, 2.0, 3.0],
           [4.0, 5.0, 6.0, 7.0],
           [8.0, 9.0, 10.0, 11.0]], dtype=float64)
    
    any of the flattened array:
     True
    
    any of a along 0th axis:
     array([True, True, True, True], dtype=bool)
    
    any of a along 1st axis:
     array([True, True, True], dtype=bool)
    
    


argmax
------

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.argmax.html

See `numpy.max <#max>`__.

argmin
------

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.argmin.html

See `numpy.max <#max>`__.

argsort
-------

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.argsort.html

Similarly to `sort <#sort>`__, ``argsort`` takes a positional, and a
keyword argument, and returns an unsigned short index array of type
``ndarray`` with the same dimensions as the input, or, if ``axis=None``,
as a row vector with length equal to the number of elements in the input
(i.e., the flattened array). The indices in the output sort the input in
ascending order. The routine in ``argsort`` is the same as in ``sort``,
therefore, the comments on computational expenses (time and RAM) also
apply. In particular, since no copy of the original data is required,
virtually no RAM beyond the output array is used.

Since the underlying container of the output array is of type
``uint16_t``, neither of the output dimensions should be larger than
65535. If that happens to be the case, the function will bail out with a
``ValueError``.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([[1, 12, 3, 0], [5, 3, 4, 1], [9, 11, 1, 8], [7, 10, 0, 1]], dtype=np.float)
    print('\na:\n', a)
    b = np.argsort(a, axis=0)
    print('\na sorted along vertical axis:\n', b)
    
    c = np.argsort(a, axis=1)
    print('\na sorted along horizontal axis:\n', c)
    
    c = np.argsort(a, axis=None)
    print('\nflattened a sorted:\n', c)

.. parsed-literal::

    
    a:
     array([[1.0, 12.0, 3.0, 0.0],
           [5.0, 3.0, 4.0, 1.0],
           [9.0, 11.0, 1.0, 8.0],
           [7.0, 10.0, 0.0, 1.0]], dtype=float64)
    
    a sorted along vertical axis:
     array([[0, 1, 3, 0],
           [1, 3, 2, 1],
           [3, 2, 0, 3],
           [2, 0, 1, 2]], dtype=uint16)
    
    a sorted along horizontal axis:
     array([[3, 0, 2, 1],
           [3, 1, 2, 0],
           [2, 3, 0, 1],
           [2, 3, 0, 1]], dtype=uint16)
    
    Traceback (most recent call last):
      File "/dev/shm/micropython.py", line 12, in <module>
    NotImplementedError: argsort is not implemented for flattened arrays
    


Since during the sorting, only the indices are shuffled, ``argsort``
does not modify the input array, as one can verify this by the following
example:

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([0, 5, 1, 3, 2, 4], dtype=np.uint8)
    print('\na:\n', a)
    b = np.argsort(a, axis=0)
    print('\nsorting indices:\n', b)
    print('\nthe original array:\n', a)

.. parsed-literal::

    
    a:
     array([0, 5, 1, 3, 2, 4], dtype=uint8)
    
    sorting indices:
     array([0, 2, 4, 3, 5, 1], dtype=uint16)
    
    the original array:
     array([0, 5, 1, 3, 2, 4], dtype=uint8)
    
    


asarray
-------

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.asarray.html

The function takes a single positional argument, and an optional keyword
argument, ``dtype``, with a default value of ``None``.

If the positional argument is an ``ndarray``, and its ``dtypes`` is
identical to the value of the keyword argument, or if the keyword
argument is ``None``, then the positional argument is simply returned.
If the original ``dtype``, and the value of the keyword argument are
different, then a copy is returned, with appropriate ``dtype``
conversion.

If the positional argument is an iterable, then the function is simply
an alias for ``array``.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array(range(9), dtype=np.uint8)
    b = np.asarray(a)
    c = np.asarray(a, dtype=np.int8)
    print('a:{}'.format(a))
    print('b:{}'.format(b))
    print('a == b: {}'.format(a is b))
    
    print('\nc:{}'.format(c))
    print('a == c: {}'.format(a is c))

.. parsed-literal::

    a:array([0, 1, 2, 3, 4, 5, 6, 7, 8], dtype=uint8)
    b:array([0, 1, 2, 3, 4, 5, 6, 7, 8], dtype=uint8)
    a == b: True
    
    c:array([0, 1, 2, 3, 4, 5, 6, 7, 8], dtype=int8)
    a == c: False
    
    


bitwise_and
-----------

``numpy``: https://numpy.org/doc/stable/reference/routines.bitwise.html

``numpy``:
https://numpy.org/doc/stable/reference/generated/numpy.bitwise_and.html

``numpy``:
https://numpy.org/doc/stable/reference/generated/numpy.bitwise_or.html

``numpy``:
https://numpy.org/doc/stable/reference/generated/numpy.bitwise_xor.html

Each of ``bitwise_and``, ``bitwise_or``, and ``bitwise_xor`` takes two
integer-type ``ndarray``\ s as arguments, and returns the element-wise
results of the ``AND``, ``OR``, and ``XOR`` operators. Broadcasting is
supported. If the ``dtype`` of the input arrays is not an integer, and
exception will be raised.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array(range(8), dtype=np.uint8)
    b = a + 1
    
    print(a)
    print(b)
    print('\nbitwise_and:\n', np.bitwise_and(a, b))
    print('\nbitwise_or:\n', np.bitwise_or(a, b))
    print('\nbitwise_xor:\n', np.bitwise_xor(a, b))

.. parsed-literal::

    array([0, 1, 2, 3, 4, 5, 6, 7], dtype=uint8)
    array([1, 2, 3, 4, 5, 6, 7, 8], dtype=uint8)
    
    bitwise_and:
     array([0, 0, 2, 0, 4, 4, 6, 0], dtype=uint8)
    
    bitwise_or:
     array([1, 3, 3, 7, 5, 7, 7, 15], dtype=uint8)
    
    bitwise_xor:
     array([1, 3, 1, 7, 1, 3, 1, 15], dtype=uint8)
    
    


clip
----

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.clip.html

Clips an array, i.e., values that are outside of an interval are clipped
to the interval edges. The function is equivalent to
``maximum(a_min, minimum(a, a_max))`` broadcasting takes place exactly
as in `minimum <#minimum>`__. If the arrays are of different ``dtype``,
the output is upcast as in `Binary operators <#Binary-operators>`__.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array(range(9), dtype=np.uint8)
    print('a:\t\t', a)
    print('clipped:\t', np.clip(a, 3, 7))
    
    b = 3 * np.ones(len(a), dtype=np.float)
    print('\na:\t\t', a)
    print('b:\t\t', b)
    print('clipped:\t', np.clip(a, b, 7))

.. parsed-literal::

    a:		 array([0, 1, 2, 3, 4, 5, 6, 7, 8], dtype=uint8)
    clipped:	 array([3, 3, 3, 3, 4, 5, 6, 7, 7], dtype=uint8)
    
    a:		 array([0, 1, 2, 3, 4, 5, 6, 7, 8], dtype=uint8)
    b:		 array([3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0], dtype=float64)
    clipped:	 array([3.0, 3.0, 3.0, 3.0, 4.0, 5.0, 6.0, 7.0, 7.0], dtype=float64)
    
    


compress
--------

``numpy``:
https://numpy.org/doc/stable/reference/generated/numpy.compress.html

The function returns selected slices of an array along given axis. If
the axis keyword is ``None``, the flattened array is used.

If the firmware was compiled with complex support, the function can
accept complex arguments.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array(range(6)).reshape((2, 3))
    
    print('a:\n', a)
    print('\ncompress(a):\n', np.compress([0, 1], a, axis=0))

.. parsed-literal::

    a:
     array([[0.0, 1.0, 2.0],
           [3.0, 4.0, 5.0]], dtype=float64)
    
    compress(a):
     array([[3.0, 4.0, 5.0]], dtype=float64)
    
    


conjugate
---------

``numpy``:
https://numpy.org/doc/stable/reference/generated/numpy.conjugate.html

If the firmware was compiled with complex support, the function
calculates the complex conjugate of the input array. If the input array
is of real ``dtype``, then the output is simply a copy, preserving the
``dtype``.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([1, 2, 3, 4], dtype=np.uint8)
    b = np.array([1+1j, 2-2j, 3+3j, 4-4j], dtype=np.complex)
    
    print('a:\t\t', a)
    print('conjugate(a):\t', np.conjugate(a))
    print()
    print('b:\t\t', b)
    print('conjugate(b):\t', np.conjugate(b))

.. parsed-literal::

    a:		 array([1, 2, 3, 4], dtype=uint8)
    conjugate(a):	 array([1, 2, 3, 4], dtype=uint8)
    
    b:		 array([1.0+1.0j, 2.0-2.0j, 3.0+3.0j, 4.0-4.0j], dtype=complex)
    conjugate(b):	 array([1.0-1.0j, 2.0+2.0j, 3.0-3.0j, 4.0+4.0j], dtype=complex)
    
    


convolve
--------

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.convolve.html

Returns the discrete, linear convolution of two one-dimensional arrays.

Only the ``full`` mode is supported, and the ``mode`` named parameter is
not accepted. Note that all other modes can be had by slicing a ``full``
result.

If the firmware was compiled with complex support, the function can
accept complex arrays.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    x = np.array((1, 2, 3))
    y = np.array((1, 10, 100, 1000))
    
    print(np.convolve(x, y))

.. parsed-literal::

    array([1.0, 12.0, 123.0, 1230.0, 2300.0, 3000.0], dtype=float64)
    
    


delete
------

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.delete.html

The function returns a new array with sub-arrays along an axis deleted.
It takes two positional arguments, the array, and the indices, which
will be removed, as well as the ``axis`` keyword argument with a default
value of ``None``. If the ``axis`` is ``None``, the will be flattened
first.

The second positional argument can be a scalar, or any ``micropython``
iterable. Since ``range`` can also be passed in place of the indices,
slicing can be emulated. If the indices are negative, the elements are
counted from the end of the axis.

Note that the function creates a copy of the indices first, because it
is not guaranteed that the indices are ordered. Keep this in mind, when
working with large arrays.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array(range(25), dtype=np.uint8).reshape((5,5))
    print('a:\n', a)
    print('\naxis = 0\n', np.delete(a, 2, axis=0))
    print('\naxis = 1\n', np.delete(a, -2, axis=1))
    print('\naxis = None\n', np.delete(a, [0, 1, 2, 22]))

.. parsed-literal::

    a:
     array([[0, 1, 2, 3, 4],
           [5, 6, 7, 8, 9],
           [10, 11, 12, 13, 14],
           [15, 16, 17, 18, 19],
           [20, 21, 22, 23, 24]], dtype=uint8)
    
    axis = 0
     array([[0, 1, 2, 3, 4],
           [5, 6, 7, 8, 9],
           [15, 16, 17, 18, 19],
           [20, 21, 22, 23, 24]], dtype=uint8)
    
    axis = 1
     array([[0, 1, 2, 4],
           [5, 6, 7, 9],
           [10, 11, 12, 14],
           [15, 16, 17, 19],
           [20, 21, 22, 24]], dtype=uint8)
    
    axis = None
     array([3, 4, 5, ..., 21, 23, 24], dtype=uint8)
    
    


diff
----

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.diff.html

The ``diff`` function returns the numerical derivative of the forward
scheme, or more accurately, the differences of an ``ndarray`` along a
given axis. The order of derivative can be stipulated with the ``n``
keyword argument, which should be between 0, and 9. Default is 1. If
higher order derivatives are required, they can be gotten by repeated
calls to the function. The ``axis`` keyword argument should be -1 (last
axis, in ``ulab`` equivalent to the second axis, and this also happens
to be the default value), 0, or 1.

Beyond the output array, the function requires only a couple of bytes of
extra RAM for the differentiation stencil. (The stencil is an ``int8``
array, one byte longer than ``n``. This also explains, why the highest
order is 9: the coefficients of a ninth-order stencil all fit in signed
bytes, while 10 would require ``int16``.) Note that as usual in
numerical differentiation (and also in ``numpy``), the length of the
respective axis will be reduced by ``n`` after the operation. If ``n``
is larger than, or equal to the length of the axis, an empty array will
be returned.

**WARNING**: the ``diff`` function does not implement the ``prepend``
and ``append`` keywords that can be found in ``numpy``.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array(range(9), dtype=np.uint8)
    a[3] = 10
    print('a:\n', a)
    
    print('\nfirst derivative:\n', np.diff(a, n=1))
    print('\nsecond derivative:\n', np.diff(a, n=2))
    
    c = np.array([[1, 2, 3, 4], [4, 3, 2, 1], [1, 4, 9, 16], [0, 0, 0, 0]])
    print('\nc:\n', c)
    print('\nfirst derivative, first axis:\n', np.diff(c, axis=0))
    print('\nfirst derivative, second axis:\n', np.diff(c, axis=1))

.. parsed-literal::

    a:
     array([0, 1, 2, 10, 4, 5, 6, 7, 8], dtype=uint8)
    
    first derivative:
     array([1, 1, 8, 250, 1, 1, 1, 1], dtype=uint8)
    
    second derivative:
     array([0, 249, 14, 249, 0, 0, 0], dtype=uint8)
    
    c:
     array([[1.0, 2.0, 3.0, 4.0],
           [4.0, 3.0, 2.0, 1.0],
           [1.0, 4.0, 9.0, 16.0],
           [0.0, 0.0, 0.0, 0.0]], dtype=float64)
    
    first derivative, first axis:
     array([[3.0, 1.0, -1.0, -3.0],
           [-3.0, 1.0, 7.0, 15.0],
           [-1.0, -4.0, -9.0, -16.0]], dtype=float64)
    
    first derivative, second axis:
     array([[1.0, 1.0, 1.0],
           [-1.0, -1.0, -1.0],
           [3.0, 5.0, 7.0],
           [0.0, 0.0, 0.0]], dtype=float64)
    
    


dot
---

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.dot.html

**WARNING:** numpy applies upcasting rules for the multiplication of
matrices, while ``ulab`` simply returns a float matrix.

Once you can invert a matrix, you might want to know, whether the
inversion is correct. You can simply take the original matrix and its
inverse, and multiply them by calling the ``dot`` function, which takes
the two matrices as its arguments. If the matrix dimensions do not
match, the function raises a ``ValueError``. The result of the
multiplication is expected to be the unit matrix, which is demonstrated
below.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    m = np.array([[1, 2, 3], [4, 5, 6], [7, 10, 9]], dtype=np.uint8)
    n = np.linalg.inv(m)
    print("m:\n", m)
    print("\nm^-1:\n", n)
    # this should be the unit matrix
    print("\nm*m^-1:\n", np.dot(m, n))

.. parsed-literal::

    m:
     array([[1, 2, 3],
           [4, 5, 6],
           [7, 10, 9]], dtype=uint8)
    
    m^-1:
     array([[-1.25, 1.0, -0.25],
           [0.4999999999999998, -1.0, 0.5],
           [0.4166666666666668, 0.3333333333333333, -0.25]], dtype=float64)
    
    m*m^-1:
     array([[1.0, 0.0, 0.0],
           [4.440892098500626e-16, 1.0, 0.0],
           [8.881784197001252e-16, 0.0, 1.0]], dtype=float64)
    
    


Note that for matrix multiplication you don’t necessarily need square
matrices, it is enough, if their dimensions are compatible (i.e., the
the left-hand-side matrix has as many columns, as does the
right-hand-side matrix rows):

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    m = np.array([[1, 2, 3, 4], [5, 6, 7, 8]], dtype=np.uint8)
    n = np.array([[1, 2], [3, 4], [5, 6], [7, 8]], dtype=np.uint8)
    print(m)
    print(n)
    print(np.dot(m, n))

.. parsed-literal::

    array([[1, 2, 3, 4],
           [5, 6, 7, 8]], dtype=uint8)
    array([[1, 2],
           [3, 4],
           [5, 6],
           [7, 8]], dtype=uint8)
    array([[50.0, 60.0],
           [114.0, 140.0]], dtype=float64)
    
    


equal
-----

``numpy``:
https://numpy.org/doc/stable/reference/generated/numpy.equal.html

``numpy``:
https://numpy.org/doc/stable/reference/generated/numpy.not_equal.html

In ``micropython``, equality of arrays or scalars can be established by
utilising the ``==``, ``!=``, ``<``, ``>``, ``<=``, or ``=>`` binary
operators. In ``circuitpython``, ``==`` and ``!=`` will produce
unexpected results. In order to avoid this discrepancy, and to maintain
compatibility with ``numpy``, ``ulab`` implements the ``equal`` and
``not_equal`` operators that return the same results, irrespective of
the ``python`` implementation.

These two functions take two ``ndarray``\ s, or scalars as their
arguments. No keyword arguments are implemented.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array(range(9))
    b = np.zeros(9)
    
    print('a: ', a)
    print('b: ', b)
    print('\na == b: ', np.equal(a, b))
    print('a != b: ', np.not_equal(a, b))
    
    # comparison with scalars
    print('a == 2: ', np.equal(a, 2))

.. parsed-literal::

    a:  array([0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0], dtype=float64)
    b:  array([0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0], dtype=float64)
    
    a == b:  array([True, False, False, False, False, False, False, False, False], dtype=bool)
    a != b:  array([False, True, True, True, True, True, True, True, True], dtype=bool)
    a == 2:  array([False, False, True, False, False, False, False, False, False], dtype=bool)
    
    


flip
----

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.flip.html

The ``flip`` function takes one positional, an ``ndarray``, and one
keyword argument, ``axis = None``, and reverses the order of elements
along the given axis. If the keyword argument is ``None``, the matrix’
entries are flipped along all axes. ``flip`` returns a new copy of the
array.

If the firmware was compiled with complex support, the function can
accept complex arrays.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([1, 2, 3, 4, 5])
    print("a: \t", a)
    print("a flipped:\t", np.flip(a))
    
    a = np.array([[1, 2, 3], [4, 5, 6], [7, 8, 9]], dtype=np.uint8)
    print("\na flipped horizontally\n", np.flip(a, axis=1))
    print("\na flipped vertically\n", np.flip(a, axis=0))
    print("\na flipped horizontally+vertically\n", np.flip(a))

.. parsed-literal::

    a: 	 array([1.0, 2.0, 3.0, 4.0, 5.0], dtype=float64)
    a flipped:	 array([5.0, 4.0, 3.0, 2.0, 1.0], dtype=float64)
    
    a flipped horizontally
     array([[3, 2, 1],
           [6, 5, 4],
           [9, 8, 7]], dtype=uint8)
    
    a flipped vertically
     array([[7, 8, 9],
           [4, 5, 6],
           [1, 2, 3]], dtype=uint8)
    
    a flipped horizontally+vertically
     array([9, 8, 7, 6, 5, 4, 3, 2, 1], dtype=uint8)
    
    


imag
----

``numpy``:
https://numpy.org/doc/stable/reference/generated/numpy.imag.html

The ``imag`` function returns the imaginary part of an array, or scalar.
It cannot accept a generic iterable as its argument. The function is
defined only, if the firmware was compiled with complex support.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([1, 2, 3], dtype=np.uint16)
    print("a:\t\t", a)
    print("imag(a):\t", np.imag(a))
    
    b = np.array([1, 2+1j, 3-1j], dtype=np.complex)
    print("\nb:\t\t", b)
    print("imag(b):\t", np.imag(b))

.. parsed-literal::

    a:		 array([1, 2, 3], dtype=uint16)
    imag(a):	 array([0, 0, 0], dtype=uint16)
    
    b:		 array([1.0+0.0j, 2.0+1.0j, 3.0-1.0j], dtype=complex)
    imag(b):	 array([0.0, 1.0, -1.0], dtype=float64)
    
    


interp
------

``numpy``: https://docs.scipy.org/doc/numpy/numpy.interp

The ``interp`` function returns the linearly interpolated values of a
one-dimensional numerical array. It requires three positional
arguments,\ ``x``, at which the interpolated values are evaluated,
``xp``, the array of the independent data variable, and ``fp``, the
array of the dependent values of the data. ``xp`` must be a
monotonically increasing sequence of numbers.

Two keyword arguments, ``left``, and ``right`` can also be supplied;
these determine the return values, if ``x < xp[0]``, and ``x > xp[-1]``,
respectively. If these arguments are not supplied, ``left``, and
``right`` default to ``fp[0]``, and ``fp[-1]``, respectively.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    x = np.array([1, 2, 3, 4, 5]) - 0.2
    xp = np.array([1, 2, 3, 4])
    fp = np.array([1, 2, 3, 5])
    
    print(x)
    print(np.interp(x, xp, fp))
    print(np.interp(x, xp, fp, left=0.0))
    print(np.interp(x, xp, fp, right=10.0))

.. parsed-literal::

    array([0.8, 1.8, 2.8, 3.8, 4.8], dtype=float64)
    array([1.0, 1.8, 2.8, 4.6, 5.0], dtype=float64)
    array([0.0, 1.8, 2.8, 4.6, 5.0], dtype=float64)
    array([1.0, 1.8, 2.8, 4.6, 10.0], dtype=float64)
    
    


isfinite
--------

``numpy``:
https://numpy.org/doc/stable/reference/generated/numpy.isfinite.html

Returns a Boolean array of the same shape as the input, or a
``True/False``, if the input is a scalar. In the return value, all
elements are ``True`` at positions, where the input value was finite.
Integer types are automatically finite, therefore, if the input is of
integer type, the output will be the ``True`` tensor.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    print('isfinite(0): ', np.isfinite(0))
    
    a = np.array([1, 2, np.nan])
    print('\n' + '='*20)
    print('a:\n', a)
    print('\nisfinite(a):\n', np.isfinite(a))
    
    b = np.array([1, 2, np.inf])
    print('\n' + '='*20)
    print('b:\n', b)
    print('\nisfinite(b):\n', np.isfinite(b))
    
    c = np.array([1, 2, 3], dtype=np.uint16)
    print('\n' + '='*20)
    print('c:\n', c)
    print('\nisfinite(c):\n', np.isfinite(c))

.. parsed-literal::

    isfinite(0):  True
    
    ====================
    a:
     array([1.0, 2.0, nan], dtype=float64)
    
    isfinite(a):
     array([True, True, False], dtype=bool)
    
    ====================
    b:
     array([1.0, 2.0, inf], dtype=float64)
    
    isfinite(b):
     array([True, True, False], dtype=bool)
    
    ====================
    c:
     array([1, 2, 3], dtype=uint16)
    
    isfinite(c):
     array([True, True, True], dtype=bool)
    
    


isinf
-----

``numpy``:
https://numpy.org/doc/stable/reference/generated/numpy.isinf.html

Similar to `isfinite <#isfinite>`__, but the output is ``True`` at
positions, where the input is infinite. Integer types return the
``False`` tensor.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    print('isinf(0): ', np.isinf(0))
    
    a = np.array([1, 2, np.nan])
    print('\n' + '='*20)
    print('a:\n', a)
    print('\nisinf(a):\n', np.isinf(a))
    
    b = np.array([1, 2, np.inf])
    print('\n' + '='*20)
    print('b:\n', b)
    print('\nisinf(b):\n', np.isinf(b))
    
    c = np.array([1, 2, 3], dtype=np.uint16)
    print('\n' + '='*20)
    print('c:\n', c)
    print('\nisinf(c):\n', np.isinf(c))

.. parsed-literal::

    isinf(0):  False
    
    ====================
    a:
     array([1.0, 2.0, nan], dtype=float64)
    
    isinf(a):
     array([False, False, False], dtype=bool)
    
    ====================
    b:
     array([1.0, 2.0, inf], dtype=float64)
    
    isinf(b):
     array([False, False, True], dtype=bool)
    
    ====================
    c:
     array([1, 2, 3], dtype=uint16)
    
    isinf(c):
     array([False, False, False], dtype=bool)
    
    


left_shift
----------

``numpy``:
https://numpy.org/doc/stable/reference/generated/numpy.left_shift.html

``numpy``:
https://numpy.org/doc/stable/reference/generated/numpy.right_shift.html

``left_shift``, and ``right_shift`` both take two integer-type
``ndarray``\ s, and bit-wise shift the elements of the first array by an
amount given by the second array to the left, and right, respectively.
Broadcasting is supported. If the ``dtype`` of the input arrays is not
an integer, and exception will be raised.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.ones(7, dtype=np.uint8)
    b = np.zeros(7, dtype=np.uint8) + 255
    c = np.array(range(7), dtype=np.uint8) + 1
    
    print('a: ', a)
    print('b: ', b)
    print('c: ', c)
    print('\na left shifted by c:\n', np.left_shift(a, c))
    print('\nb right shifted by c:\n', np.right_shift(b, c))

.. parsed-literal::

    a:  array([1, 1, 1, 1, 1, 1, 1], dtype=uint8)
    b:  array([255, 255, 255, 255, 255, 255, 255], dtype=uint8)
    c:  array([1, 2, 3, 4, 5, 6, 7], dtype=uint8)
    
    a left shifted by c:
     array([2, 4, 8, 16, 32, 64, 128], dtype=uint8)
    
    b right shifted by c:
     array([127, 63, 31, 15, 7, 3, 1], dtype=uint8)
    
    


load
----

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.load.html

The function reads data from a file in ``numpy``\ ’s
`platform-independent
format <https://numpy.org/doc/stable/reference/generated/numpy.lib.format.html#module-numpy.lib.format>`__,
and returns the generated array. If the endianness of the data in the
file and the microcontroller differ, the bytes are automatically
swapped.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.load('a.npy')
    print(a)

.. parsed-literal::

    array([[0.0, 1.0, 2.0, 3.0, 4.0],
           [5.0, 6.0, 7.0, 8.0, 9.0],
           [10.0, 11.0, 12.0, 13.0, 14.0],
           [15.0, 16.0, 17.0, 18.0, 19.0],
           [20.0, 21.0, 22.0, 23.0, 24.0]], dtype=float64)
    
    


loadtxt
-------

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.loadtxt.html

The function reads data from a text file, and returns the generated
array. It takes a file name as the single positional argument, and the
following keyword arguments:

1. ``comments='#'``
2. ``dtype=float``
3. ``delimiter=','``
4. ``max_rows`` (with a default of all rows)
5. ``skip_rows=0``
6. ``usecols`` (with a default of all columns)

If ``dtype`` is supplied and is not ``float``, the data entries will be
converted to the appropriate integer type by rounding the values.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    print('read all data')
    print(np.loadtxt('loadtxt.dat'))
    
    print('\nread maximum 5 rows (first row is a comment line)')
    print(np.loadtxt('loadtxt.dat', max_rows=5))
    
    print('\nread maximum 5 rows, convert dtype (first row is a comment line)')
    print(np.loadtxt('loadtxt.dat', max_rows=5, dtype=np.uint8))
    
    print('\nskip the first 3 rows, convert dtype (first row is a comment line)')
    print(np.loadtxt('loadtxt.dat', skiprows=3, dtype=np.uint8))

.. parsed-literal::

    read all data
    array([[0.0, 1.0, 2.0, 3.0],
           [4.0, 5.0, 6.0, 7.0],
           [8.0, 9.0, 10.0, 11.0],
           [12.0, 13.0, 14.0, 15.0],
           [16.0, 17.0, 18.0, 19.0],
           [20.0, 21.0, 22.0, 23.0],
           [24.0, 25.0, 26.0, 27.0],
           [28.00000000000001, 29.0, 30.0, 31.0],
           [32.0, 33.0, 34.00000000000001, 35.0]], dtype=float64)
    
    read maximum 5 rows (first row is a comment line)
    array([[0.0, 1.0, 2.0, 3.0],
           [4.0, 5.0, 6.0, 7.0],
           [8.0, 9.0, 10.0, 11.0],
           [12.0, 13.0, 14.0, 15.0]], dtype=float64)
    
    read maximum 5 rows, convert dtype (first row is a comment line)
    array([[0, 1, 2, 3],
           [4, 5, 6, 7],
           [8, 9, 10, 11],
           [12, 13, 14, 15]], dtype=uint8)
    
    skip the first 3 rows, convert dtype (first row is a comment line)
    array([[8, 9, 10, 11],
           [12, 13, 14, 15],
           [16, 17, 18, 19],
           [20, 21, 22, 23],
           [24, 25, 26, 27],
           [28, 29, 30, 31],
           [32, 33, 34, 35]], dtype=uint8)
    
    


mean
----

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.mean.html

If the axis keyword is not specified, it assumes the default value of
``None``, and returns the result of the computation for the flattened
array. Otherwise, the calculation is along the given axis.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([[1, 2, 3], [4, 5, 6], [7, 8, 9]])
    print('a: \n', a)
    print('mean, flat: ', np.mean(a))
    print('mean, horizontal: ', np.mean(a, axis=1))
    print('mean, vertical: ', np.mean(a, axis=0))

.. parsed-literal::

    a: 
     array([[1.0, 2.0, 3.0],
           [4.0, 5.0, 6.0],
           [7.0, 8.0, 9.0]], dtype=float64)
    mean, flat:  5.0
    mean, horizontal:  array([2.0, 5.0, 8.0], dtype=float64)
    mean, vertical:  array([4.0, 5.0, 6.0], dtype=float64)
    
    


max
---

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.max.html

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.argmax.html

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.min.html

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.argmin.html

**WARNING:** Difference to ``numpy``: the ``out`` keyword argument is
not implemented.

These functions follow the same pattern, and work with generic
iterables, and ``ndarray``\ s. ``min``, and ``max`` return the minimum
or maximum of a sequence. If the input array is two-dimensional, the
``axis`` keyword argument can be supplied, in which case the
minimum/maximum along the given axis will be returned. If ``axis=None``
(this is also the default value), the minimum/maximum of the flattened
array will be determined.

``argmin/argmax`` return the position (index) of the minimum/maximum in
the sequence.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([1, 2, 0, 1, 10])
    print('a:', a)
    print('min of a:', np.min(a))
    print('argmin of a:', np.argmin(a))
    
    b = np.array([[1, 2, 0], [1, 10, -1]])
    print('\nb:\n', b)
    print('min of b (flattened):', np.min(b))
    print('min of b (axis=0):', np.min(b, axis=0))
    print('min of b (axis=1):', np.min(b, axis=1))

.. parsed-literal::

    a: array([1.0, 2.0, 0.0, 1.0, 10.0], dtype=float64)
    min of a: 0.0
    argmin of a: 2
    
    b:
     array([[1.0, 2.0, 0.0],
           [1.0, 10.0, -1.0]], dtype=float64)
    min of b (flattened): -1.0
    min of b (axis=0): array([1.0, 2.0, -1.0], dtype=float64)
    min of b (axis=1): array([0.0, -1.0], dtype=float64)
    
    


median
------

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.median.html

The function computes the median along the specified axis, and returns
the median of the array elements. If the ``axis`` keyword argument is
``None``, the arrays is flattened first. The ``dtype`` of the results is
always float.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array(range(12), dtype=np.int8).reshape((3, 4))
    print('a:\n', a)
    print('\nmedian of the flattened array: ', np.median(a))
    print('\nmedian along the vertical axis: ', np.median(a, axis=0))
    print('\nmedian along the horizontal axis: ', np.median(a, axis=1))

.. parsed-literal::

    a:
     array([[0, 1, 2, 3],
           [4, 5, 6, 7],
           [8, 9, 10, 11]], dtype=int8)
    
    median of the flattened array:  5.5
    
    median along the vertical axis:  array([4.0, 5.0, 6.0, 7.0], dtype=float64)
    
    median along the horizontal axis:  array([1.5, 5.5, 9.5], dtype=float64)
    
    


min
---

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.min.html

See `numpy.max <#max>`__.

minimum
-------

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.minimum.html

See `numpy.maximum <#maximum>`__

maximum
-------

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.maximum.html

Returns the maximum of two arrays, or two scalars, or an array, and a
scalar. If the arrays are of different ``dtype``, the output is upcast
as in `Binary operators <#Binary-operators>`__. If both inputs are
scalars, a scalar is returned. Only positional arguments are
implemented.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([1, 2, 3, 4, 5], dtype=np.uint8)
    b = np.array([5, 4, 3, 2, 1], dtype=np.float)
    print('minimum of a, and b:')
    print(np.minimum(a, b))
    
    print('\nmaximum of a, and b:')
    print(np.maximum(a, b))
    
    print('\nmaximum of 1, and 5.5:')
    print(np.maximum(1, 5.5))

.. parsed-literal::

    minimum of a, and b:
    array([1.0, 2.0, 3.0, 2.0, 1.0], dtype=float64)
    
    maximum of a, and b:
    array([5.0, 4.0, 3.0, 4.0, 5.0], dtype=float64)
    
    maximum of 1, and 5.5:
    5.5
    
    


nonzero
-------

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.nonzero.html

``nonzero`` returns the indices of the elements of an array that are not
zero. If the number of dimensions of the array is larger than one, a
tuple of arrays is returned, one for each dimension, containing the
indices of the non-zero elements in that dimension.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array(range(9)) - 5
    print('a:\n', a)
    print(np.nonzero(a))
    
    a = a.reshape((3,3))
    print('\na:\n', a)
    print(np.nonzero(a))

.. parsed-literal::

    a:
     array([-5.0, -4.0, -3.0, -2.0, -1.0, 0.0, 1.0, 2.0, 3.0], dtype=float64)
    (array([0, 1, 2, 3, 4, 6, 7, 8], dtype=uint16),)
    
    a:
     array([[-5.0, -4.0, -3.0],
           [-2.0, -1.0, 0.0],
           [1.0, 2.0, 3.0]], dtype=float64)
    (array([0, 0, 0, 1, 1, 2, 2, 2], dtype=uint16), array([0, 1, 2, 0, 1, 0, 1, 2], dtype=uint16))
    
    


not_equal
---------

See `numpy.equal <#equal>`__.

polyfit
-------

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.polyfit.html

``polyfit`` takes two, or three arguments. The last one is the degree of
the polynomial that will be fitted, the last but one is an array or
iterable with the ``y`` (dependent) values, and the first one, an array
or iterable with the ``x`` (independent) values, can be dropped. If that
is the case, ``x`` will be generated in the function as
``range(len(y))``.

If the lengths of ``x``, and ``y`` are not the same, the function raises
a ``ValueError``.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    x = np.array([0, 1, 2, 3, 4, 5, 6])
    y = np.array([9, 4, 1, 0, 1, 4, 9])
    print('independent values:\t', x)
    print('dependent values:\t', y)
    print('fitted values:\t\t', np.polyfit(x, y, 2))
    
    # the same with missing x
    print('\ndependent values:\t', y)
    print('fitted values:\t\t', np.polyfit(y, 2))

.. parsed-literal::

    independent values:	 array([0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0], dtype=float64)
    dependent values:	 array([9.0, 4.0, 1.0, 0.0, 1.0, 4.0, 9.0], dtype=float64)
    fitted values:		 array([1.0, -6.0, 9.000000000000004], dtype=float64)
    
    dependent values:	 array([9.0, 4.0, 1.0, 0.0, 1.0, 4.0, 9.0], dtype=float64)
    fitted values:		 array([1.0, -6.0, 9.000000000000004], dtype=float64)
    
    


Execution time
~~~~~~~~~~~~~~

``polyfit`` is based on the inversion of a matrix (there is more on the
background in https://en.wikipedia.org/wiki/Polynomial_regression), and
it requires the intermediate storage of ``2*N*(deg+1)`` floats, where
``N`` is the number of entries in the input array, and ``deg`` is the
fit’s degree. The additional computation costs of the matrix inversion
discussed in `linalg.inv <#inv>`__ also apply. The example from above
needs around 150 microseconds to return:

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    @timeit
    def time_polyfit(x, y, n):
        return np.polyfit(x, y, n)
    
    x = np.array([0, 1, 2, 3, 4, 5, 6])
    y = np.array([9, 4, 1, 0, 1, 4, 9])
    
    time_polyfit(x, y, 2)

.. parsed-literal::

    execution time:  153  us


polyval
-------

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.polyval.html

``polyval`` takes two arguments, both arrays or generic ``micropython``
iterables returning scalars.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    p = [1, 1, 1, 0]
    x = [0, 1, 2, 3, 4]
    print('coefficients: ', p)
    print('independent values: ', x)
    print('\nvalues of p(x): ', np.polyval(p, x))
    
    # the same works with one-dimensional ndarrays
    a = np.array(x)
    print('\nndarray (a): ', a)
    print('value of p(a): ', np.polyval(p, a))

.. parsed-literal::

    coefficients:  [1, 1, 1, 0]
    independent values:  [0, 1, 2, 3, 4]
    
    values of p(x):  array([0.0, 3.0, 14.0, 39.0, 84.0], dtype=float64)
    
    ndarray (a):  array([0.0, 1.0, 2.0, 3.0, 4.0], dtype=float64)
    value of p(a):  array([0.0, 3.0, 14.0, 39.0, 84.0], dtype=float64)
    
    


real
----

``numpy``:
https://numpy.org/doc/stable/reference/generated/numpy.real.html

The ``real`` function returns the real part of an array, or scalar. It
cannot accept a generic iterable as its argument. The function is
defined only, if the firmware was compiled with complex support.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([1, 2, 3], dtype=np.uint16)
    print("a:\t\t", a)
    print("real(a):\t", np.real(a))
    
    b = np.array([1, 2+1j, 3-1j], dtype=np.complex)
    print("\nb:\t\t", b)
    print("real(b):\t", np.real(b))

.. parsed-literal::

    a:		 array([1, 2, 3], dtype=uint16)
    real(a):	 array([1, 2, 3], dtype=uint16)
    
    b:		 array([1.0+0.0j, 2.0+1.0j, 3.0-1.0j], dtype=complex)
    real(b):	 array([1.0, 2.0, 3.0], dtype=float64)
    
    


roll
----

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.roll.html

The roll function shifts the content of a vector by the positions given
as the second argument. If the ``axis`` keyword is supplied, the shift
is applied to the given axis.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([1, 2, 3, 4, 5, 6, 7, 8])
    print("a:\t\t\t", a)
    
    a = np.roll(a, 2)
    print("a rolled to the left:\t", a)
    
    # this should be the original vector
    a = np.roll(a, -2)
    print("a rolled to the right:\t", a)

.. parsed-literal::

    a:			 array([1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0], dtype=float64)
    a rolled to the left:	 array([7.0, 8.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0], dtype=float64)
    a rolled to the right:	 array([1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0], dtype=float64)
    
    


Rolling works with matrices, too. If the ``axis`` keyword is 0, the
matrix is rolled along its vertical axis, otherwise, horizontally.

Horizontal rolls are faster, because they require fewer steps, and
larger memory chunks are copied, however, they also require more RAM:
basically the whole row must be stored internally. Most expensive are
the ``None`` keyword values, because with ``axis = None``, the array is
flattened first, hence the row’s length is the size of the whole matrix.

Vertical rolls require two internal copies of single columns.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array(range(12)).reshape((3, 4))
    print("a:\n", a)
    a = np.roll(a, 2, axis=0)
    print("\na rolled up:\n", a)
    
    a = np.array(range(12)).reshape((3, 4))
    print("a:\n", a)
    a = np.roll(a, -1, axis=1)
    print("\na rolled to the left:\n", a)
    
    a = np.array(range(12)).reshape((3, 4))
    print("a:\n", a)
    a = np.roll(a, 1, axis=None)
    print("\na rolled with None:\n", a)

.. parsed-literal::

    a:
     array([[0.0, 1.0, 2.0, 3.0],
           [4.0, 5.0, 6.0, 7.0],
           [8.0, 9.0, 10.0, 11.0]], dtype=float64)
    
    a rolled up:
     array([[4.0, 5.0, 6.0, 7.0],
           [8.0, 9.0, 10.0, 11.0],
           [0.0, 1.0, 2.0, 3.0]], dtype=float64)
    a:
     array([[0.0, 1.0, 2.0, 3.0],
           [4.0, 5.0, 6.0, 7.0],
           [8.0, 9.0, 10.0, 11.0]], dtype=float64)
    
    a rolled to the left:
     array([[1.0, 2.0, 3.0, 0.0],
           [5.0, 6.0, 7.0, 4.0],
           [9.0, 10.0, 11.0, 8.0]], dtype=float64)
    a:
     array([[0.0, 1.0, 2.0, 3.0],
           [4.0, 5.0, 6.0, 7.0],
           [8.0, 9.0, 10.0, 11.0]], dtype=float64)
    
    a rolled with None:
     array([[11.0, 0.0, 1.0, 2.0],
           [3.0, 4.0, 5.0, 6.0],
           [7.0, 8.0, 9.0, 10.0]], dtype=float64)
    
    


save
----

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.save.html

With the help of this function, numerical array can be saved in
``numpy``\ ’s `platform-independent
format <https://numpy.org/doc/stable/reference/generated/numpy.lib.format.html#module-numpy.lib.format>`__.

The function takes two positional arguments, the name of the output
file, and the array.

.. code::

    # code to be run in CPython
    
    a = np.array(range(25)).reshape((5, 5))
    np.save('a.npy', a)
savetxt
-------

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.savetxt.html

With the help of this function, numerical array can be saved in a text
file. The function takes two positional arguments, the name of the
output file, and the array, and also implements the ``comments='#'``
``delimiter=' '``, the ``header=''``, and ``footer=''`` keyword
arguments. The input is treated as of type ``float``, i.e., the output
is always in the floating point representation.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array(range(12), dtype=np.uint8).reshape((3, 4))
    np.savetxt('savetxt.dat', a)
    
    with open('savetxt.dat', 'r') as fin:
        print(fin.read())
        
    np.savetxt('savetxt.dat', a, 
               comments='!', 
               delimiter=';', 
               header='col1;col2;col3;col4', 
               footer='saved data')
    
    with open('savetxt.dat', 'r') as fin:
        print(fin.read())

.. parsed-literal::

    0.000000000000000 1.000000000000000 2.000000000000000 3.000000000000000
    4.000000000000000 5.000000000000000 6.000000000000000 7.000000000000000
    8.000000000000000 9.000000000000000 10.000000000000000 11.000000000000000
    
    !col1;col2;col3;col4
    0.000000000000000;1.000000000000000;2.000000000000000;3.000000000000000
    4.000000000000000;5.000000000000000;6.000000000000000;7.000000000000000
    8.000000000000000;9.000000000000000;10.000000000000000;11.000000000000000
    !saved data
    
    
    


size
----

The function takes a single positional argument, and an optional keyword
argument, ``axis``, with a default value of ``None``, and returns the
size of an array along that axis. If ``axis`` is ``None``, the total
length of the array (the product of the elements of its shape) is
returned.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.ones((2, 3))
    
    print(a)
    print('size(a, axis=0): ', np.size(a, axis=0))
    print('size(a, axis=1): ', np.size(a, axis=1))
    print('size(a, axis=None): ', np.size(a, axis=None))

.. parsed-literal::

    array([[1.0, 1.0, 1.0],
           [1.0, 1.0, 1.0]], dtype=float64)
    size(a, axis=0):  2
    size(a, axis=1):  3
    size(a, axis=None):  6
    
    


sort
----

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.sort.html

The sort function takes an ndarray, and sorts its elements in ascending
order along the specified axis using a heap sort algorithm. As opposed
to the ``.sort()`` method discussed earlier, this function creates a
copy of its input before sorting, and at the end, returns this copy.
Sorting takes place in place, without auxiliary storage. The ``axis``
keyword argument takes on the possible values of -1 (the last axis, in
``ulab`` equivalent to the second axis, and this also happens to be the
default value), 0, 1, or ``None``. The first three cases are identical
to those in `diff <#diff>`__, while the last one flattens the array
before sorting.

If descending order is required, the result can simply be ``flip``\ ped,
see `flip <#flip>`__.

**WARNING:** ``numpy`` defines the ``kind``, and ``order`` keyword
arguments that are not implemented here. The function in ``ulab`` always
uses heap sort, and since ``ulab`` does not have the concept of data
fields, the ``order`` keyword argument would have no meaning.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([[1, 12, 3, 0], [5, 3, 4, 1], [9, 11, 1, 8], [7, 10, 0, 1]], dtype=np.float)
    print('\na:\n', a)
    b = np.sort(a, axis=0)
    print('\na sorted along vertical axis:\n', b)
    
    c = np.sort(a, axis=1)
    print('\na sorted along horizontal axis:\n', c)
    
    c = np.sort(a, axis=None)
    print('\nflattened a sorted:\n', c)

.. parsed-literal::

    
    a:
     array([[1.0, 12.0, 3.0, 0.0],
           [5.0, 3.0, 4.0, 1.0],
           [9.0, 11.0, 1.0, 8.0],
           [7.0, 10.0, 0.0, 1.0]], dtype=float64)
    
    a sorted along vertical axis:
     array([[1.0, 3.0, 0.0, 0.0],
           [5.0, 10.0, 1.0, 1.0],
           [7.0, 11.0, 3.0, 1.0],
           [9.0, 12.0, 4.0, 8.0]], dtype=float64)
    
    a sorted along horizontal axis:
     array([[0.0, 1.0, 3.0, 12.0],
           [1.0, 3.0, 4.0, 5.0],
           [1.0, 8.0, 9.0, 11.0],
           [0.0, 1.0, 7.0, 10.0]], dtype=float64)
    
    flattened a sorted:
     array([0.0, 0.0, 1.0, ..., 10.0, 11.0, 12.0], dtype=float64)
    
    


Heap sort requires :math:`\sim N\log N` operations, and notably, the
worst case costs only 20% more time than the average. In order to get an
order-of-magnitude estimate, we will take the sine of 1000 uniformly
spaced numbers between 0, and two pi, and sort them:

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    @timeit
    def sort_time(array):
        return nup.sort(array)
    
    b = np.sin(np.linspace(0, 6.28, num=1000))
    print('b: ', b)
    sort_time(b)
    print('\nb sorted:\n', b)
sort_complex
------------

``numpy``:
https://numpy.org/doc/stable/reference/generated/numpy.sort_complex.html

If the firmware was compiled with complex support, the functions sorts
the input array first according to its real part, and then the imaginary
part. The input must be a one-dimensional array. The output is always of
``dtype`` complex, even if the input was real integer.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([5, 4, 3, 2, 1], dtype=np.int16)
    print('a:\t\t\t', a)
    print('sort_complex(a):\t', np.sort_complex(a))
    print()
    
    b = np.array([5, 4+3j, 4-2j, 0, 1j], dtype=np.complex)
    print('b:\t\t\t', b)
    print('sort_complex(b):\t', np.sort_complex(b))

.. parsed-literal::

    a:			 array([5, 4, 3, 2, 1], dtype=int16)
    sort_complex(a):	 array([1.0+0.0j, 2.0+0.0j, 3.0+0.0j, 4.0+0.0j, 5.0+0.0j], dtype=complex)
    
    b:			 array([5.0+0.0j, 4.0+3.0j, 4.0-2.0j, 0.0+0.0j, 0.0+1.0j], dtype=complex)
    sort_complex(b):	 array([0.0+0.0j, 0.0+1.0j, 4.0-2.0j, 4.0+3.0j, 5.0+0.0j], dtype=complex)
    
    


std
---

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.std.html

If the axis keyword is not specified, it assumes the default value of
``None``, and returns the result of the computation for the flattened
array. Otherwise, the calculation is along the given axis.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([[1, 2, 3], [4, 5, 6], [7, 8, 9]])
    print('a: \n', a)
    print('sum, flat array: ', np.std(a))
    print('std, vertical: ', np.std(a, axis=0))
    print('std, horizonal: ', np.std(a, axis=1))

.. parsed-literal::

    a: 
     array([[1.0, 2.0, 3.0],
           [4.0, 5.0, 6.0],
           [7.0, 8.0, 9.0]], dtype=float64)
    sum, flat array:  2.581988897471611
    std, vertical:  array([2.449489742783178, 2.449489742783178, 2.449489742783178], dtype=float64)
    std, horizonal:  array([0.8164965809277261, 0.8164965809277261, 0.8164965809277261], dtype=float64)
    
    


sum
---

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.sum.html

If the axis keyword is not specified, it assumes the default value of
``None``, and returns the result of the computation for the flattened
array. Otherwise, the calculation is along the given axis.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([[1, 2, 3], [4, 5, 6], [7, 8, 9]])
    print('a: \n', a)
    
    print('sum, flat array: ', np.sum(a))
    print('sum, horizontal: ', np.sum(a, axis=1))
    print('std, vertical: ', np.sum(a, axis=0))

.. parsed-literal::

    a: 
     array([[1.0, 2.0, 3.0],
           [4.0, 5.0, 6.0],
           [7.0, 8.0, 9.0]], dtype=float64)
    sum, flat array:  45.0
    sum, horizontal:  array([6.0, 15.0, 24.0], dtype=float64)
    std, vertical:  array([12.0, 15.0, 18.0], dtype=float64)
    
    


take
----

``numpy``:
https://numpy.org/doc/stable/reference/generated/numpy.take.html

The ``take`` method takes elements from an array along an axis. The
function accepts two positional arguments, the array, and the indices,
which is either a ``python`` iterable, or a one-dimensional ``ndarray``,
as well as three keyword arguments, the ``axis``, which can be ``None``,
or an integer, ``out``, which can be ``None``, or an ``ndarray`` with
the proper dimensions, and ``mode``, which can be one of the strings
``raise``, ``wrap``, or ``clip``. This last argument determines how
out-of-bounds indices will be treated. The default value is ``raise``,
which raises an exception. ``wrap`` takes the indices modulo the length
of the ``axis``, while ``clip`` pegs the values at the 0, and the length
of the ``axis``. If ``axis`` is ``None``, then ``take`` operates on the
flattened array.

The function can be regarded as a method of advanced slicing: as opposed
to standard slicing, where the indices are distributed uniformly and in
either increasing or decreasing order, ``take`` can take indices in an
arbitrary order.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array(range(12)).reshape((3, 4))
    print('\na:', a)
    
    print('\nslices taken along first axis')
    print(np.take(a, (0, 2, 2, 1), axis=0))
    
    print('\nslices taken along second axis')
    print(np.take(a, (0, 2, 2, 1), axis=1))

.. parsed-literal::

    
    a: array([[0.0, 1.0, 2.0, 3.0],
           [4.0, 5.0, 6.0, 7.0],
           [8.0, 9.0, 10.0, 11.0]], dtype=float64)
    
    slices taken along first axis
    array([[0.0, 1.0, 2.0, 3.0],
           [8.0, 9.0, 10.0, 11.0],
           [8.0, 9.0, 10.0, 11.0],
           [4.0, 5.0, 6.0, 7.0]], dtype=float64)
    
    slices taken along second axis
    array([[0.0, 2.0, 2.0, 1.0],
           [2.0, 3.0, 4.0, 5.0],
           [6.0, 7.0, 8.0, 9.0]], dtype=float64)
    
    


trace
-----

``numpy``:
https://numpy.org/doc/stable/reference/generated/numpy.trace.html

The ``trace`` function returns the sum of the diagonal elements of a
square matrix. If the input argument is not a square matrix, an
exception will be raised.

The scalar so returned will inherit the type of the input array, i.e.,
integer arrays have integer trace, and floating point arrays a floating
point trace.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([[25, 15, -5], [15, 18,  0], [-5,  0, 11]], dtype=np.int8)
    print('a: ', a)
    print('\ntrace of a: ', np.trace(a))
    
    b = np.array([[25, 15, -5], [15, 18,  0], [-5,  0, 11]], dtype=np.float)
    
    print('='*20 + '\nb: ', b)
    print('\ntrace of b: ', np.trace(b))

.. parsed-literal::

    a:  array([[25, 15, -5],
           [15, 18, 0],
           [-5, 0, 11]], dtype=int8)
    
    trace of a:  54
    ====================
    b:  array([[25.0, 15.0, -5.0],
           [15.0, 18.0, 0.0],
           [-5.0, 0.0, 11.0]], dtype=float64)
    
    trace of b:  54.0
    
    


trapz
-----

``numpy``:
https://numpy.org/doc/stable/reference/generated/numpy.trapz.html

The function takes one or two one-dimensional ``ndarray``\ s, and
integrates the dependent values (``y``) using the trapezoidal rule. If
the independent variable (``x``) is given, that is taken as the sample
points corresponding to ``y``.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    x = np.linspace(0, 9, num=10)
    y = x*x
    
    print('x: ',  x)
    print('y: ',  y)
    print('============================')
    print('integral of y: ', np.trapz(y))
    print('integral of y at x: ', np.trapz(y, x=x))

.. parsed-literal::

    x:  array([0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0], dtype=float64)
    y:  array([0.0, 1.0, 4.0, 9.0, 16.0, 25.0, 36.0, 49.0, 64.0, 81.0], dtype=float64)
    ============================
    integral of y:  244.5
    integral of y at x:  244.5
    
    


where
-----

``numpy``:
https://numpy.org/doc/stable/reference/generated/numpy.where.html

The function takes three positional arguments, ``condition``, ``x``, and
``y``, and returns a new ``ndarray``, whose values are taken from either
``x``, or ``y``, depending on the truthness of ``condition``. The three
arguments are broadcast together, and the function raises a
``ValueError`` exception, if broadcasting is not possible.

The function is implemented for ``ndarray``\ s only: other iterable
types can be passed after casting them to an ``ndarray`` by calling the
``array`` constructor.

If the ``dtype``\ s of ``x``, and ``y`` differ, the output is upcast as
discussed earlier.

Note that the ``condition`` is expanded into an Boolean ``ndarray``.
This means that the storage required to hold the condition should be
taken into account, whenever the function is called.

The following example returns an ``ndarray`` of length 4, with 1 at
positions, where ``condition`` is smaller than 3, and with -1 otherwise.

.. code::
        
    # code to be run in micropython
    
    
    from ulab import numpy as np
    
    condition = np.array([1, 2, 3, 4], dtype=np.uint8)
    print(np.where(condition < 3, 1, -1))

.. parsed-literal::

    array([1, 1, -1, -1], dtype=int16)
    
    


The next snippet shows, how values from two arrays can be fed into the
output:

.. code::
        
    # code to be run in micropython
    
    
    from ulab import numpy as np
    
    condition = np.array([1, 2, 3, 4], dtype=np.uint8)
    x = np.array([11, 22, 33, 44], dtype=np.uint8)
    y = np.array([1, 2, 3, 4], dtype=np.uint8)
    print(np.where(condition < 3, x, y))

.. parsed-literal::

    array([11, 22, 3, 4], dtype=uint8)
    
    

