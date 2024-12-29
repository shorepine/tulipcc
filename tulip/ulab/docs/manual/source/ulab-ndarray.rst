
ndarray, the base class
=======================

The ``ndarray`` is the underlying container of numerical data. It can be
thought of as micropython’s own ``array`` object, but has a great number
of extra features starting with how it can be initialised, which
operations can be done on it, and which functions can accept it as an
argument. One important property of an ``ndarray`` is that it is also a
proper ``micropython`` iterable.

The ``ndarray`` consists of a short header, and a pointer that holds the
data. The pointer always points to a contiguous segment in memory
(``numpy`` is more flexible in this regard), and the header tells the
interpreter, how the data from this segment is to be read out, and what
the bytes mean. Some operations, e.g., ``reshape``, are fast, because
they do not operate on the data, they work on the header, and therefore,
only a couple of bytes are manipulated, even if there are a million data
entries. A more detailed exposition of how operators are implemented can
be found in the section titled `Programming ulab <#Programming_ula>`__.

Since the ``ndarray`` is a binary container, it is also compact, meaning
that it takes only a couple of bytes of extra RAM in addition to what is
required for storing the numbers themselves. ``ndarray``\ s are also
type-aware, i.e., one can save RAM by specifying a data type, and using
the smallest reasonable one. Five such types are defined, namely
``uint8``, ``int8``, which occupy a single byte of memory per datum,
``uint16``, and ``int16``, which occupy two bytes per datum, and
``float``, which occupies four or eight bytes per datum. The
precision/size of the ``float`` type depends on the definition of
``mp_float_t``. Some platforms, e.g., the PYBD, implement ``double``\ s,
but some, e.g., the pyboard.v.11, do not. You can find out, what type of
float your particular platform implements by looking at the output of
the `.itemsize <#.itemsize>`__ class property, or looking at the exact
``dtype``, when you print out an array.

In addition to the five above-mentioned numerical types, it is also
possible to define Boolean arrays, which can be used in the indexing of
data. However, Boolean arrays are really nothing but arrays of type
``uint8`` with an extra flag.

On the following pages, we will see how one can work with
``ndarray``\ s. Those familiar with ``numpy`` should find that the
nomenclature and naming conventions of ``numpy`` are adhered to as
closely as possible. We will point out the few differences, where
necessary.

For the sake of comparison, in addition to the ``ulab`` code snippets,
sometimes the equivalent ``numpy`` code is also presented. You can find
out, where the snippet is supposed to run by looking at its first line,
the header of the code block.

The ndinfo function
-------------------

A concise summary of a couple of the properties of an ``ndarray`` can be
printed out by calling the ``ndinfo`` function. In addition to finding
out what the *shape* and *strides* of the array array, we also get the
``itemsize``, as well as the type. An interesting piece of information
is the *data pointer*, which tells us, what the address of the data
segment of the ``ndarray`` is. We will see the significance of this in
the section `Slicing and indexing <#Slicing-and-indexing>`__.

Note that this function simply prints some information, but does not
return anything. If you need to get a handle of the data contained in
the printout, you should call the dedicated ``shape``, ``strides``, or
``itemsize`` functions directly.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array(range(5), dtype=np.float)
    b = np.array(range(25), dtype=np.uint8).reshape((5, 5))
    np.ndinfo(a)
    print('\n')
    np.ndinfo(b)

.. parsed-literal::

    class: ndarray
    shape: (5,)
    strides: (8,)
    itemsize: 8
    data pointer: 0x7f8f6fa2e240
    type: float
    
    
    class: ndarray
    shape: (5, 5)
    strides: (5, 1)
    itemsize: 1
    data pointer: 0x7f8f6fa2e2e0
    type: uint8
    
    


Initialising an array
---------------------

A new array can be created by passing either a standard micropython
iterable, or another ``ndarray`` into the constructor.

Initialising by passing iterables
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If the iterable is one-dimensional, i.e., one whose elements are
numbers, then a row vector will be created and returned. If the iterable
is two-dimensional, i.e., one whose elements are again iterables, a
matrix will be created. If the lengths of the iterables are not
consistent, a ``ValueError`` will be raised. Iterables of different
types can be mixed in the initialisation function.

If the ``dtype`` keyword with the possible
``uint8/int8/uint16/int16/float`` values is supplied, the new
``ndarray`` will have that type, otherwise, it assumes ``float`` as
default. In addition, if ``ULAB_SUPPORTS_COMPLEX`` is set to 1 in
`ulab.h <https://github.com/v923z/micropython-ulab/blob/master/code/ulab.h>`__,
the ``dtype`` can also take on the value of ``complex``.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = [1, 2, 3, 4, 5, 6, 7, 8]
    b = np.array(a)
    
    print("a:\t", a)
    print("b:\t", b)
    
    # a two-dimensional array with mixed-type initialisers
    c = np.array([range(5), range(20, 25, 1), [44, 55, 66, 77, 88]], dtype=np.uint8)
    print("\nc:\t", c)
    
    # and now we throw an exception
    d = np.array([range(5), range(10), [44, 55, 66, 77, 88]], dtype=np.uint8)
    print("\nd:\t", d)

.. parsed-literal::

    a:	 [1, 2, 3, 4, 5, 6, 7, 8]
    b:	 array([1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0], dtype=float64)
    
    c:	 array([[0, 1, 2, 3, 4],
           [20, 21, 22, 23, 24],
           [44, 55, 66, 77, 88]], dtype=uint8)
    
    Traceback (most recent call last):
      File "/dev/shm/micropython.py", line 15, in <module>
    ValueError: iterables are not of the same length
    


Initialising by passing arrays
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

An ``ndarray`` can be initialised by supplying another array. This
statement is almost trivial, since ``ndarray``\ s are iterables
themselves, though it should be pointed out that initialising through
arrays is a bit faster. This statement is especially true, if the
``dtype``\ s of the source and output arrays are the same, because then
the contents can simply be copied without further ado. While type
conversion is also possible, it will always be slower than straight
copying.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = [1, 2, 3, 4, 5, 6, 7, 8]
    b = np.array(a)
    c = np.array(b)
    d = np.array(b, dtype=np.uint8)
    
    print("a:\t", a)
    print("\nb:\t", b)
    print("\nc:\t", c)
    print("\nd:\t", d)

.. parsed-literal::

    a:	 [1, 2, 3, 4, 5, 6, 7, 8]
    
    b:	 array([1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0], dtype=float64)
    
    c:	 array([1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0], dtype=float64)
    
    d:	 array([1, 2, 3, 4, 5, 6, 7, 8], dtype=uint8)
    
    


Note that the default type of the ``ndarray`` is ``float``. Hence, if
the array is initialised from another array, type conversion will always
take place, except, when the output type is specifically supplied. I.e.,

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array(range(5), dtype=np.uint8)
    b = np.array(a)
    print("a:\t", a)
    print("\nb:\t", b)

.. parsed-literal::

    a:	 array([0, 1, 2, 3, 4], dtype=uint8)
    
    b:	 array([0.0, 1.0, 2.0, 3.0, 4.0], dtype=float64)
    
    


will iterate over the elements in ``a``, since in the assignment
``b = np.array(a)``, no output type was given, therefore, ``float`` was
assumed. On the other hand,

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array(range(5), dtype=np.uint8)
    b = np.array(a, dtype=np.uint8)
    print("a:\t", a)
    print("\nb:\t", b)

.. parsed-literal::

    a:	 array([0, 1, 2, 3, 4], dtype=uint8)
    
    b:	 array([0, 1, 2, 3, 4], dtype=uint8)
    
    


will simply copy the content of ``a`` into ``b`` without any iteration,
and will, therefore, be faster. Keep this in mind, whenever the output
type, or performance is important.

Array initialisation functions
==============================

There are nine functions that can be used for initialising an array.
Starred functions accept ``complex`` as the value of the ``dtype``, if
the firmware was compiled with complex support.

1.  `numpy.arange <#arange>`__
2.  `numpy.concatenate <#concatenate>`__
3.  `numpy.diag\* <#diag>`__
4.  `numpy.empty\* <#empty>`__
5.  `numpy.eye\* <#eye>`__
6.  `numpy.frombuffer <#frombuffer>`__
7.  `numpy.full\* <#full>`__
8.  `numpy.linspace\* <#linspace>`__
9.  `numpy.logspace <#logspace>`__
10. `numpy.ones\* <#ones>`__
11. `numpy.zeros\* <#zeros>`__

arange
------

``numpy``:
https://numpy.org/doc/stable/reference/generated/numpy.arange.html

The function returns a one-dimensional array with evenly spaced values.
Takes 3 positional arguments (two are optional), and the ``dtype``
keyword argument.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    print(np.arange(10))
    print(np.arange(2, 10))
    print(np.arange(2, 10, 3))
    print(np.arange(2, 10, 3, dtype=np.float))

.. parsed-literal::

    array([0, 1, 2, 3, 4, 5, 6, 7, 8, 9], dtype=int16)
    array([2, 3, 4, 5, 6, 7, 8, 9], dtype=int16)
    array([2, 5, 8], dtype=int16)
    array([2.0, 5.0, 8.0], dtype=float64)
    
    


concatenate
-----------

``numpy``:
https://numpy.org/doc/stable/reference/generated/numpy.concatenate.html

The function joins a sequence of arrays, if they are compatible in
shape, i.e., if all shapes except the one along the joining axis are
equal.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array(range(25), dtype=np.uint8).reshape((5, 5))
    b = np.array(range(15), dtype=np.uint8).reshape((3, 5))
    
    c = np.concatenate((a, b), axis=0)
    print(c)

.. parsed-literal::

    array([[0, 1, 2, 3, 4],
           [5, 6, 7, 8, 9],
           [10, 11, 12, 13, 14],
           [15, 16, 17, 18, 19],
           [20, 21, 22, 23, 24],
           [0, 1, 2, 3, 4],
           [5, 6, 7, 8, 9],
           [10, 11, 12, 13, 14]], dtype=uint8)
    
    


**WARNING**: ``numpy`` accepts arbitrary ``dtype``\ s in the sequence of
arrays, in ``ulab`` the ``dtype``\ s must be identical. If you want to
concatenate different types, you have to convert all arrays to the same
type first. Here ``b`` is of ``float`` type, so it cannot directly be
concatenated to ``a``. However, if we cast the ``dtype`` of ``b``, the
concatenation works:

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array(range(25), dtype=np.uint8).reshape((5, 5))
    b = np.array(range(15), dtype=np.float).reshape((5, 3))
    d = np.array(b+1, dtype=np.uint8)
    print('a: ', a)
    print('='*20 + '\nd: ', d)
    c = np.concatenate((d, a), axis=1)
    print('='*20 + '\nc: ', c)

.. parsed-literal::

    a:  array([[0, 1, 2, 3, 4],
           [5, 6, 7, 8, 9],
           [10, 11, 12, 13, 14],
           [15, 16, 17, 18, 19],
           [20, 21, 22, 23, 24]], dtype=uint8)
    ====================
    d:  array([[1, 2, 3],
           [4, 5, 6],
           [7, 8, 9],
           [10, 11, 12],
           [13, 14, 15]], dtype=uint8)
    ====================
    c:  array([[1, 2, 3, 0, 1, 2, 3, 4],
           [4, 5, 6, 5, 6, 7, 8, 9],
           [7, 8, 9, 10, 11, 12, 13, 14],
           [10, 11, 12, 15, 16, 17, 18, 19],
           [13, 14, 15, 20, 21, 22, 23, 24]], dtype=uint8)
    
    


diag
----

``numpy``:
https://numpy.org/doc/stable/reference/generated/numpy.diag.html

Extract a diagonal, or construct a diagonal array.

The function takes a positional argument, an ``ndarray``, or any
``micropython`` iterable, and an optional keyword argument, a shift,
with a default value of 0. If the first argument is a two-dimensional
array (or a two-dimensional iterable, e.g., a list of lists), the
function returns a one-dimensional array containing the diagonal
entries. The diagonal can be shifted by an amount given in the second
argument. If the shift is larger than the length of the corresponding
axis, an empty array is returned.

If the first argument is a one-dimensional array, the function returns a
two-dimensional square tensor with its diagonal elements given by the
first argument. Again, the diagonal be shifted by an amount given by the
keyword argument.

The ``diag`` function can accept a complex array, if the firmware was
compiled with complex support.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([1, 2, 3], dtype=np.uint8)
    print(np.diag(a))
    
    print('\ndiagonal shifted by 2')
    print(np.diag(a, k=2))
    
    print('\ndiagonal shifted by -2')
    print(np.diag(a, k=-2))

.. parsed-literal::

    array([[1, 0, 0],
           [0, 2, 0],
           [0, 0, 3]], dtype=uint8)
    
    diagonal shifted by 2
    array([[0, 0, 1, 0, 0],
           [0, 0, 0, 2, 0],
           [0, 0, 0, 0, 3],
           [0, 0, 0, 0, 0],
           [0, 0, 0, 0, 0]], dtype=uint8)
    
    diagonal shifted by -2
    array([[0, 0, 0, 0, 0],
           [0, 0, 0, 0, 0],
           [1, 0, 0, 0, 0],
           [0, 2, 0, 0, 0],
           [0, 0, 3, 0, 0]], dtype=uint8)
    
    


.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.arange(16).reshape((4, 4))
    print(a)
    print('\ndiagonal of a:')
    print(np.diag(a))
    
    print('\ndiagonal of a:')
    print(np.diag(a))
    
    print('\ndiagonal of a, shifted by 2')
    print(np.diag(a, k=2))
    
    print('\ndiagonal of a, shifted by 5')
    print(np.diag(a, k=5))

.. parsed-literal::

    array([[0, 1, 2, 3],
           [4, 5, 6, 7],
           [8, 9, 10, 11],
           [12, 13, 14, 15]], dtype=int16)
    
    diagonal of a:
    array([0, 5, 10, 15], dtype=int16)
    
    diagonal of a:
    array([0, 5, 10, 15], dtype=int16)
    
    diagonal of a, shifted by 2
    array([2, 7], dtype=int16)
    
    diagonal of a, shifted by 5
    array([], dtype=int16)
    
    


empty
-----

``numpy``:
https://numpy.org/doc/stable/reference/generated/numpy.empty.html

``empty`` is simply an alias for ``zeros``, i.e., as opposed to
``numpy``, the entries of the tensor will be initialised to zero.

The ``empty`` function can accept complex as the value of the dtype, if
the firmware was compiled with complex support.

eye
---

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.eye.html

Another special array method is the ``eye`` function, whose call
signature is

.. code:: python

   eye(N, M, k=0, dtype=float)

where ``N`` (``M``) specify the dimensions of the matrix (if only ``N``
is supplied, then we get a square matrix, otherwise one with ``M`` rows,
and ``N`` columns), and ``k`` is the shift of the ones (the main
diagonal corresponds to ``k=0``). Here are a couple of examples.

The ``eye`` function can accept ``complex`` as the value of the
``dtype``, if the firmware was compiled with complex support.

With a single argument
~~~~~~~~~~~~~~~~~~~~~~

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    print(np.eye(5))

.. parsed-literal::

    array([[1.0, 0.0, 0.0, 0.0, 0.0],
           [0.0, 1.0, 0.0, 0.0, 0.0],
           [0.0, 0.0, 1.0, 0.0, 0.0],
           [0.0, 0.0, 0.0, 1.0, 0.0],
           [0.0, 0.0, 0.0, 0.0, 1.0]], dtype=float64)
    
    


Specifying the dimensions of the matrix
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    print(np.eye(4, M=6, k=-1, dtype=np.int16))

.. parsed-literal::

    array([[0, 0, 0, 0, 0, 0],
           [1, 0, 0, 0, 0, 0],
           [0, 1, 0, 0, 0, 0],
           [0, 0, 1, 0, 0, 0]], dtype=int16)
    
    


.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    print(np.eye(4, M=6, dtype=np.int8))

.. parsed-literal::

    array([[1, 0, 0, 0, 0, 0],
           [0, 1, 0, 0, 0, 0],
           [0, 0, 1, 0, 0, 0],
           [0, 0, 0, 1, 0, 0]], dtype=int8)
    
    


frombuffer
----------

``numpy``:
https://numpy.org/doc/stable/reference/generated/numpy.frombuffer.html

The function interprets a contiguous buffer as a one-dimensional array,
and thus can be used for piping buffered data directly into an array.
This method of analysing, e.g., ADC data is much more efficient than
passing the ADC buffer into the ``array`` constructor, because
``frombuffer`` simply creates the ``ndarray`` header and blindly copies
the memory segment, without inspecting the underlying data.

The function takes a single positional argument, the buffer, and three
keyword arguments. These are the ``dtype`` with a default value of
``float``, the ``offset``, with a default of 0, and the ``count``, with
a default of -1, meaning that all data are taken in.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    buffer = b'\x01\x02\x03\x04\x05\x06\x07\x08'
    print('buffer: ', buffer)
    
    a = np.frombuffer(buffer, dtype=np.uint8)
    print('a, all data read: ', a)
    
    b = np.frombuffer(buffer, dtype=np.uint8, offset=2)
    print('b, all data with an offset: ', b)
    
    c = np.frombuffer(buffer, dtype=np.uint8, offset=2, count=3)
    print('c, only 3 items with an offset: ', c)

.. parsed-literal::

    buffer:  b'\x01\x02\x03\x04\x05\x06\x07\x08'
    a, all data read:  array([1, 2, 3, 4, 5, 6, 7, 8], dtype=uint8)
    b, all data with an offset:  array([3, 4, 5, 6, 7, 8], dtype=uint8)
    c, only 3 items with an offset:  array([3, 4, 5], dtype=uint8)
    
    


full
----

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.full.html

The function returns an array of arbitrary dimension, whose elements are
all equal to the second positional argument. The first argument is a
tuple describing the shape of the tensor. The ``dtype`` keyword argument
with a default value of ``float`` can also be supplied.

The ``full`` function can accept a complex scalar, or ``complex`` as the
value of ``dtype``, if the firmware was compiled with complex support.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    # create an array with the default type
    print(np.full((2, 4), 3))
    
    print('\n' + '='*20 + '\n')
    # the array type is uint8 now
    print(np.full((2, 4), 3, dtype=np.uint8))

.. parsed-literal::

    array([[3.0, 3.0, 3.0, 3.0],
           [3.0, 3.0, 3.0, 3.0]], dtype=float64)
    
    ====================
    
    array([[3, 3, 3, 3],
           [3, 3, 3, 3]], dtype=uint8)
    
    


linspace
--------

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.linspace.html

This function returns an array, whose elements are uniformly spaced
between the ``start``, and ``stop`` points. The number of intervals is
determined by the ``num`` keyword argument, whose default value is 50.
With the ``endpoint`` keyword argument (defaults to ``True``) one can
include ``stop`` in the sequence. In addition, the ``dtype`` keyword can
be supplied to force type conversion of the output. The default is
``float``. Note that, when ``dtype`` is of integer type, the sequence is
not necessarily evenly spaced. This is not an error, rather a
consequence of rounding. (This is also the ``numpy`` behaviour.)

The ``linspace`` function can accept ``complex`` as the value of the
``dtype``, if the firmware was compiled with complex support. The output
``dtype`` is automatically complex, if either of the endpoints is a
complex scalar.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    # generate a sequence with defaults
    print('default sequence:\t', np.linspace(0, 10))
    
    # num=5
    print('num=5:\t\t\t', np.linspace(0, 10, num=5))
    
    # num=5, endpoint=False
    print('num=5:\t\t\t', np.linspace(0, 10, num=5, endpoint=False))
    
    # num=5, endpoint=False, dtype=uint8
    print('num=5:\t\t\t', np.linspace(0, 5, num=7, endpoint=False, dtype=np.uint8))

.. parsed-literal::

    default sequence:	 array([0.0, 0.2040816326530612, 0.4081632653061225, ..., 9.591836734693871, 9.795918367346932, 9.999999999999993], dtype=float64)
    num=5:			 array([0.0, 2.5, 5.0, 7.5, 10.0], dtype=float64)
    num=5:			 array([0.0, 2.0, 4.0, 6.0, 8.0], dtype=float64)
    num=5:			 array([0, 0, 1, 2, 2, 3, 4], dtype=uint8)
    
    


logspace
--------

``linspace``\ ’ equivalent for logarithmically spaced data is
``logspace``. This function produces a sequence of numbers, in which the
quotient of consecutive numbers is constant. This is a geometric
sequence.

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.logspace.html

This function returns an array, whose elements are uniformly spaced
between the ``start``, and ``stop`` points. The number of intervals is
determined by the ``num`` keyword argument, whose default value is 50.
With the ``endpoint`` keyword argument (defaults to ``True``) one can
include ``stop`` in the sequence. In addition, the ``dtype`` keyword can
be supplied to force type conversion of the output. The default is
``float``. Note that, exactly as in ``linspace``, when ``dtype`` is of
integer type, the sequence is not necessarily evenly spaced in log
space.

In addition to the keyword arguments found in ``linspace``, ``logspace``
also accepts the ``base`` argument. The default value is 10.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    # generate a sequence with defaults
    print('default sequence:\t', np.logspace(0, 3))
    
    # num=5
    print('num=5:\t\t\t', np.logspace(1, 10, num=5))
    
    # num=5, endpoint=False
    print('num=5:\t\t\t', np.logspace(1, 10, num=5, endpoint=False))
    
    # num=5, endpoint=False
    print('num=5:\t\t\t', np.logspace(1, 10, num=5, endpoint=False, base=2))

.. parsed-literal::

    default sequence:	 array([1.0, 1.151395399326447, 1.325711365590109, ..., 754.3120063354646, 868.5113737513561, 1000.000000000004], dtype=float64)
    num=5:			 array([10.0, 1778.279410038923, 316227.766016838, 56234132.5190349, 10000000000.0], dtype=float64)
    num=5:			 array([10.0, 630.9573444801933, 39810.71705534974, 2511886.431509581, 158489319.2461114], dtype=float64)
    num=5:			 array([2.0, 6.964404506368993, 24.25146506416637, 84.44850628946524, 294.066778879241], dtype=float64)
    
    


ones, zeros
-----------

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.zeros.html

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.ones.html

A couple of special arrays and matrices can easily be initialised by
calling one of the ``ones``, or ``zeros`` functions. ``ones`` and
``zeros`` follow the same pattern, and have the call signature

.. code:: python

   ones(shape, dtype=float)
   zeros(shape, dtype=float)

where shape is either an integer, or a tuple specifying the shape.

The ``ones/zeros`` functions can accept complex as the value of the
dtype, if the firmware was compiled with complex support.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    print(np.ones(6, dtype=np.uint8))
    
    print(np.zeros((6, 4)))

.. parsed-literal::

    array([1, 1, 1, 1, 1, 1], dtype=uint8)
    array([[0.0, 0.0, 0.0, 0.0],
           [0.0, 0.0, 0.0, 0.0],
           [0.0, 0.0, 0.0, 0.0],
           [0.0, 0.0, 0.0, 0.0],
           [0.0, 0.0, 0.0, 0.0],
           [0.0, 0.0, 0.0, 0.0]], dtype=float64)
    
    


When specifying the shape, make sure that the length of the tuple is not
larger than the maximum dimension of your firmware.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    import ulab
    
    print('maximum number of dimensions: ', ulab.__version__)
    
    print(np.zeros((2, 2, 2)))

.. parsed-literal::

    maximum number of dimensions:  2.1.0-2D
    
    Traceback (most recent call last):
      File "/dev/shm/micropython.py", line 7, in <module>
    TypeError: too many dimensions
    


Customising array printouts
===========================

``ndarray``\ s are pretty-printed, i.e., if the number of entries along
the last axis is larger than 10 (default value), then only the first and
last three entries will be printed. Also note that, as opposed to
``numpy``, the printout always contains the ``dtype``.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array(range(200))
    print("a:\t", a)

.. parsed-literal::

    a:	 array([0.0, 1.0, 2.0, ..., 197.0, 198.0, 199.0], dtype=float64)
    
    


set_printoptions
----------------

The default values can be overwritten by means of the
``set_printoptions`` function
`numpy.set_printoptions <https://numpy.org/doc/1.18/reference/generated/numpy.set_printoptions.html>`__,
which accepts two keywords arguments, the ``threshold``, and the
``edgeitems``. The first of these arguments determines the length of the
longest array that will be printed in full, while the second is the
number of items that will be printed on the left and right hand side of
the ellipsis, if the array is longer than ``threshold``.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array(range(20))
    print("a printed with defaults:\t", a)
    
    np.set_printoptions(threshold=200)
    print("\na printed in full:\t\t", a)
    
    np.set_printoptions(threshold=10, edgeitems=2)
    print("\na truncated with 2 edgeitems:\t", a)

.. parsed-literal::

    a printed with defaults:	 array([0.0, 1.0, 2.0, ..., 17.0, 18.0, 19.0], dtype=float64)
    
    a printed in full:		 array([0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0], dtype=float64)
    
    a truncated with 2 edgeitems:	 array([0.0, 1.0, ..., 18.0, 19.0], dtype=float64)
    
    


get_printoptions
----------------

The set value of the ``threshold`` and ``edgeitems`` can be retrieved by
calling the ``get_printoptions`` function with no arguments. The
function returns a *dictionary* with two keys.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    np.set_printoptions(threshold=100, edgeitems=20)
    print(np.get_printoptions())

.. parsed-literal::

    {'threshold': 100, 'edgeitems': 20}
    
    


Methods and properties of ndarrays
==================================

Arrays have several *properties* that can queried, and some methods that
can be called. With the exception of the flatten and transpose
operators, properties return an object that describe some feature of the
array, while the methods return a new array-like object. The ``imag``,
and ``real`` properties are included in the firmware only, when it was
compiled with complex support.

1.  `.byteswap <#.byteswap>`__
2.  `.copy <#.copy>`__
3.  `.dtype <#.dtype>`__
4.  `.flat <#.flat>`__
5.  `.flatten <#.flatten>`__
6.  `.imag\* <#.imag>`__
7.  `.itemsize <#.itemsize>`__
8.  `.real\* <#.real>`__
9.  `.reshape <#.reshape>`__
10. `.shape <#.shape>`__
11. `.size <#.size>`__
12. `.T <#.transpose>`__
13. `.tobytes <#.tobytes>`__
14. `.tolist <#.tolist>`__
15. `.transpose <#.transpose>`__
16. `.sort <#.sort>`__

.byteswap
---------

``numpy``
https://numpy.org/doc/stable/reference/generated/numpy.char.chararray.byteswap.html

The method takes a single keyword argument, ``inplace``, with values
``True`` or ``False``, and swaps the bytes in the array. If
``inplace = False``, a new ``ndarray`` is returned, otherwise the
original values are overwritten.

The ``frombuffer`` function is a convenient way of receiving data from
peripheral devices that work with buffers. However, it is not guaranteed
that the byte order (in other words, the *endianness*) of the peripheral
device matches that of the microcontroller. The ``.byteswap`` method
makes it possible to change the endianness of the incoming data stream.

Obviously, byteswapping makes sense only for those cases, when a datum
occupies more than one byte, i.e., for the ``uint16``, ``int16``, and
``float`` ``dtype``\ s. When ``dtype`` is either ``uint8``, or ``int8``,
the method simply returns a view or copy of self, depending upon the
value of ``inplace``.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    buffer = b'\x01\x02\x03\x04\x05\x06\x07\x08'
    print('buffer: ', buffer)
    
    a = np.frombuffer(buffer, dtype=np.uint16)
    print('a: ', a)
    b = a.byteswap()
    print('b: ', b)

.. parsed-literal::

    buffer:  b'\x01\x02\x03\x04\x05\x06\x07\x08'
    a:  array([513, 1027, 1541, 2055], dtype=uint16)
    b:  array([258, 772, 1286, 1800], dtype=uint16)
    
    


.copy
-----

The ``.copy`` method creates a new *deep copy* of an array, i.e., the
entries of the source array are *copied* into the target array.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([1, 2, 3, 4], dtype=np.int8)
    b = a.copy()
    print('a: ', a)
    print('='*20)
    print('b: ', b)

.. parsed-literal::

    a:  array([1, 2, 3, 4], dtype=int8)
    ====================
    b:  array([1, 2, 3, 4], dtype=int8)
    
    


.dtype
------

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.ndarray.dtype.htm

The ``.dtype`` property is the ``dtype`` of an array. This can then be
used for initialising another array with the matching type. ``ulab``
implements two versions of ``dtype``; one that is ``numpy``-like, i.e.,
one, which returns a ``dtype`` object, and one that is significantly
cheaper in terms of flash space, but does not define a ``dtype`` object,
and holds a single character (number) instead.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([1, 2, 3, 4], dtype=np.int8)
    b = np.array([5, 6, 7], dtype=a.dtype)
    print('a: ', a)
    print('dtype of a: ', a.dtype)
    print('\nb: ', b)

.. parsed-literal::

    a:  array([1, 2, 3, 4], dtype=int8)
    dtype of a:  dtype('int8')
    
    b:  array([5, 6, 7], dtype=int8)
    
    


If the ``ulab.h`` header file sets the pre-processor constant
``ULAB_HAS_DTYPE_OBJECT`` to 0 as

.. code:: c

   #define ULAB_HAS_DTYPE_OBJECT               (0)

then the output of the previous snippet will be

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([1, 2, 3, 4], dtype=np.int8)
    b = np.array([5, 6, 7], dtype=a.dtype)
    print('a: ', a)
    print('dtype of a: ', a.dtype)
    print('\nb: ', b)

.. parsed-literal::

    a:  array([1, 2, 3, 4], dtype=int8)
    dtype of a:  98
    
    b:  array([5, 6, 7], dtype=int8)
    
    


Here 98 is nothing but the ASCII value of the character ``b``, which is
the type code for signed 8-bit integers. The object definition adds
around 600 bytes to the firmware.

.flat
-----

numpy:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.ndarray.flat.htm

``.flat`` returns the array’s flat iterator. For one-dimensional objects
the flat iterator is equivalent to the standart iterator, while for
higher dimensional tensors, it amounts to first flattening the array,
and then iterating over it. Note, however, that the flat iterator does
not consume RAM beyond what is required for holding the position of the
iterator itself, while flattening produces a new copy.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([1, 2, 3, 4], dtype=np.int8)
    for _a in a:
        print(_a)
    
    a = np.array([[1, 2, 3, 4], [5, 6, 7, 8]], dtype=np.int8)
    print('a:\n', a)
    
    for _a in a:
        print(_a)
    
    for _a in a.flat:
        print(_a)

.. parsed-literal::

    1
    2
    3
    4
    a:
     array([[1, 2, 3, 4],
           [5, 6, 7, 8]], dtype=int8)
    array([1, 2, 3, 4], dtype=int8)
    array([5, 6, 7, 8], dtype=int8)
    1
    2
    3
    4
    5
    6
    7
    8
    
    


.flatten
--------

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.ndarray.flatten.htm

``.flatten`` returns the flattened array. The array can be flattened in
``C`` style (i.e., moving along the last axis in the tensor), or in
``fortran`` style (i.e., moving along the first axis in the tensor).

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([1, 2, 3, 4], dtype=np.int8)
    print("a: \t\t", a)
    print("a flattened: \t", a.flatten())
    
    b = np.array([[1, 2, 3], [4, 5, 6]], dtype=np.int8)
    print("\nb:", b)
    
    print("b flattened (C): \t", b.flatten())
    print("b flattened (F): \t", b.flatten(order='F'))

.. parsed-literal::

    a: 		 array([1, 2, 3, 4], dtype=int8)
    a flattened: 	 array([1, 2, 3, 4], dtype=int8)
    
    b: array([[1, 2, 3],
           [4, 5, 6]], dtype=int8)
    b flattened (C): 	 array([1, 2, 3, 4, 5, 6], dtype=int8)
    b flattened (F): 	 array([1, 4, 2, 5, 3, 6], dtype=int8)
    
    


.imag
-----

``numpy``:
https://numpy.org/doc/stable/reference/generated/numpy.ndarray.imag.html

The ``.imag`` property is defined only, if the firmware was compiled
with complex support, and returns a copy with the imaginary part of an
array. If the array is real, then the output is straight zeros with the
``dtype`` of the input. If the input is complex, the output ``dtype`` is
always ``float``, irrespective of the values.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([1, 2, 3], dtype=np.uint16)
    print("a:\t", a)
    print("a.imag:\t", a.imag)
    
    b = np.array([1, 2+1j, 3-1j], dtype=np.complex)
    print("\nb:\t", b)
    print("b.imag:\t", b.imag)

.. parsed-literal::

    a:	 array([1, 2, 3], dtype=uint16)
    a.imag:	 array([0, 0, 0], dtype=uint16)
    
    b:	 array([1.0+0.0j, 2.0+1.0j, 3.0-1.0j], dtype=complex)
    b.imag:	 array([0.0, 1.0, -1.0], dtype=float64)
    
    


.itemsize
---------

``numpy``:
https://numpy.org/doc/stable/reference/generated/numpy.ndarray.itemsize.html

The ``.itemsize`` property is an integer with the size of elements in
the array.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([1, 2, 3], dtype=np.int8)
    print("a:\n", a)
    print("itemsize of a:", a.itemsize)
    
    b= np.array([[1, 2], [3, 4]], dtype=np.float)
    print("\nb:\n", b)
    print("itemsize of b:", b.itemsize)

.. parsed-literal::

    a:
     array([1, 2, 3], dtype=int8)
    itemsize of a: 1
    
    b:
     array([[1.0, 2.0],
           [3.0, 4.0]], dtype=float64)
    itemsize of b: 8
    
    


.real
-----

numpy:
https://numpy.org/doc/stable/reference/generated/numpy.ndarray.real.html

The ``.real`` property is defined only, if the firmware was compiled
with complex support, and returns a copy with the real part of an array.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([1, 2, 3], dtype=np.uint16)
    print("a:\t", a)
    print("a.real:\t", a.real)
    
    b = np.array([1, 2+1j, 3-1j], dtype=np.complex)
    print("\nb:\t", b)
    print("b.real:\t", b.real)

.. parsed-literal::

    a:	 array([1, 2, 3], dtype=uint16)
    a.real:	 array([1, 2, 3], dtype=uint16)
    
    b:	 array([1.0+0.0j, 2.0+1.0j, 3.0-1.0j], dtype=complex)
    b.real:	 array([1.0, 2.0, 3.0], dtype=float64)
    
    


.reshape
--------

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.reshape.html

``reshape`` re-writes the shape properties of an ``ndarray``, but the
array will not be modified in any other way. The function takes a single
2-tuple with two integers as its argument. The 2-tuple should specify
the desired number of rows and columns. If the new shape is not
consistent with the old, a ``ValueError`` exception will be raised.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([[1, 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12], [13, 14, 15, 16]], dtype=np.uint8)
    print('a (4 by 4):', a)
    print('a (2 by 8):', a.reshape((2, 8)))
    print('a (1 by 16):', a.reshape((1, 16)))

.. parsed-literal::

    a (4 by 4): array([[1, 2, 3, 4],
           [5, 6, 7, 8],
           [9, 10, 11, 12],
           [13, 14, 15, 16]], dtype=uint8)
    a (2 by 8): array([[1, 2, 3, 4, 5, 6, 7, 8],
           [9, 10, 11, 12, 13, 14, 15, 16]], dtype=uint8)
    a (1 by 16): array([[1, 2, 3, ..., 14, 15, 16]], dtype=uint8)
    
    


.. code::

    # code to be run in CPython
    
    Note that `ndarray.reshape()` can also be called by assigning to `ndarray.shape`.
.shape
------

``numpy``:
https://numpy.org/doc/stable/reference/generated/numpy.ndarray.shape.html

The ``.shape`` property is a tuple whose elements are the length of the
array along each axis.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([1, 2, 3, 4], dtype=np.int8)
    print("a:\n", a)
    print("shape of a:", a.shape)
    
    b= np.array([[1, 2], [3, 4]], dtype=np.int8)
    print("\nb:\n", b)
    print("shape of b:", b.shape)

.. parsed-literal::

    a:
     array([1, 2, 3, 4], dtype=int8)
    shape of a: (4,)
    
    b:
     array([[1, 2],
           [3, 4]], dtype=int8)
    shape of b: (2, 2)
    
    


By assigning a tuple to the ``.shape`` property, the array can be
``reshape``\ d:

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([1, 2, 3, 4, 5, 6, 7, 8, 9])
    print('a:\n', a)
    
    a.shape = (3, 3)
    print('\na:\n', a)

.. parsed-literal::

    a:
     array([1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0], dtype=float64)
    
    a:
     array([[1.0, 2.0, 3.0],
           [4.0, 5.0, 6.0],
           [7.0, 8.0, 9.0]], dtype=float64)
    
    


.size
-----

``numpy``:
https://numpy.org/doc/stable/reference/generated/numpy.ndarray.size.html

The ``.size`` property is an integer specifying the number of elements
in the array.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([1, 2, 3], dtype=np.int8)
    print("a:\n", a)
    print("size of a:", a.size)
    
    b= np.array([[1, 2], [3, 4]], dtype=np.int8)
    print("\nb:\n", b)
    print("size of b:", b.size)

.. parsed-literal::

    a:
     array([1, 2, 3], dtype=int8)
    size of a: 3
    
    b:
     array([[1, 2],
    	 [3, 4]], dtype=int8)
    size of b: 4
    
    


.T

The ``.T`` property of the ``ndarray`` is equivalent to
`.transpose <#.transpose>`__.

.tobytes
--------

``numpy``:
https://numpy.org/doc/stable/reference/generated/numpy.ndarray.tobytes.html

The ``.tobytes`` method can be used for acquiring a handle of the
underlying data pointer of an array, and it returns a new ``bytearray``
that can be fed into any method that can accep a ``bytearray``, e.g.,
ADC data can be buffered into this ``bytearray``, or the ``bytearray``
can be fed into a DAC. Since the ``bytearray`` is really nothing but the
bare data container of the array, any manipulation on the ``bytearray``
automatically modifies the array itself.

Note that the method raises a ``ValueError`` exception, if the array is
not dense (i.e., it has already been sliced).

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array(range(8), dtype=np.uint8)
    print('a: ', a)
    b = a.tobytes()
    print('b: ', b)
    
    # modify b
    b[0] = 13
    
    print('='*20)
    print('b: ', b)
    print('a: ', a)

.. parsed-literal::

    a:  array([0, 1, 2, 3, 4, 5, 6, 7], dtype=uint8)
    b:  bytearray(b'\x00\x01\x02\x03\x04\x05\x06\x07')
    ====================
    b:  bytearray(b'\r\x01\x02\x03\x04\x05\x06\x07')
    a:  array([13, 1, 2, 3, 4, 5, 6, 7], dtype=uint8)
    
    


.tolist
-------

``numpy``:
https://numpy.org/doc/stable/reference/generated/numpy.ndarray.tolist.html

The ``.tolist`` method can be used for converting the numerical array
into a (nested) ``python`` lists.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array(range(4), dtype=np.uint8)
    print('a: ', a)
    b = a.tolist()
    print('b: ', b)
    
    c = a.reshape((2, 2))
    print('='*20)
    print('c: ', c)
    d = c.tolist()
    print('d: ', d)

.. parsed-literal::

    a:  array([0, 1, 2, 3], dtype=uint8)
    b:  [0, 1, 2, 3]
    ====================
    c:  array([[0, 1],
           [2, 3]], dtype=uint8)
    d:  [[0, 1], [2, 3]]
    
    


.transpose
----------

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.transpose.html

Returns the transposed array. Only defined, if the number of maximum
dimensions is larger than 1.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([[1, 2, 3], [4, 5, 6], [7, 8, 9], [10, 11, 12]], dtype=np.uint8)
    print('a:\n', a)
    print('shape of a:', a.shape)
    a.transpose()
    print('\ntranspose of a:\n', a)
    print('shape of a:', a.shape)

.. parsed-literal::

    a:
     array([[1, 2, 3],
    	 [4, 5, 6],
    	 [7, 8, 9],
    	 [10, 11, 12]], dtype=uint8)
    shape of a: (4, 3)
    
    transpose of a:
     array([[1, 4, 7, 10],
    	 [2, 5, 8, 11],
    	 [3, 6, 9, 12]], dtype=uint8)
    shape of a: (3, 4)
    
    


The transpose of the array can also be gotten through the ``T``
property:

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([[1, 2, 3], [4, 5, 6], [7, 8, 9]], dtype=np.uint8)
    print('a:\n', a)
    print('\ntranspose of a:\n', a.T)

.. parsed-literal::

    a:
     array([[1, 2, 3],
           [4, 5, 6],
           [7, 8, 9]], dtype=uint8)
    
    transpose of a:
     array([[1, 4, 7],
           [2, 5, 8],
           [3, 6, 9]], dtype=uint8)
    
    


.sort
-----

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.sort.html

In-place sorting of an ``ndarray``. For a more detailed exposition, see
`sort <#sort>`__.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([[1, 12, 3, 0], [5, 3, 4, 1], [9, 11, 1, 8], [7, 10, 0, 1]], dtype=np.uint8)
    print('\na:\n', a)
    a.sort(axis=0)
    print('\na sorted along vertical axis:\n', a)
    
    a = np.array([[1, 12, 3, 0], [5, 3, 4, 1], [9, 11, 1, 8], [7, 10, 0, 1]], dtype=np.uint8)
    a.sort(axis=1)
    print('\na sorted along horizontal axis:\n', a)
    
    a = np.array([[1, 12, 3, 0], [5, 3, 4, 1], [9, 11, 1, 8], [7, 10, 0, 1]], dtype=np.uint8)
    a.sort(axis=None)
    print('\nflattened a sorted:\n', a)

.. parsed-literal::

    
    a:
     array([[1, 12, 3, 0],
           [5, 3, 4, 1],
           [9, 11, 1, 8],
           [7, 10, 0, 1]], dtype=uint8)
    
    a sorted along vertical axis:
     array([[1, 3, 0, 0],
           [5, 10, 1, 1],
           [7, 11, 3, 1],
           [9, 12, 4, 8]], dtype=uint8)
    
    a sorted along horizontal axis:
     array([[0, 1, 3, 12],
           [1, 3, 4, 5],
           [1, 8, 9, 11],
           [0, 1, 7, 10]], dtype=uint8)
    
    flattened a sorted:
     array([0, 0, 1, ..., 10, 11, 12], dtype=uint8)
    
    


Unary operators
===============

With the exception of ``len``, which returns a single number, all unary
operators manipulate the underlying data element-wise.

len
---

This operator takes a single argument, the array, and returns either the
length of the first axis.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([1, 2, 3, 4, 5], dtype=np.uint8)
    b = np.array([range(5), range(5), range(5), range(5)], dtype=np.uint8)
    
    print("a:\t", a)
    print("length of a: ", len(a))
    print("shape of a: ", a.shape)
    print("\nb:\t", b)
    print("length of b: ", len(b))
    print("shape of b: ", b.shape)

.. parsed-literal::

    a:	 array([1, 2, 3, 4, 5], dtype=uint8)
    length of a:  5
    shape of a:  (5,)
    
    b:	 array([[0, 1, 2, 3, 4],
           [0, 1, 2, 3, 4],
           [0, 1, 2, 3, 4],
           [0, 1, 2, 3, 4]], dtype=uint8)
    length of b:  2
    shape of b:  (4, 5)
    
    


The number returned by ``len`` is also the length of the iterations,
when the array supplies the elements for an iteration (see later).

invert
------

The function is defined for integer data types (``uint8``, ``int8``,
``uint16``, and ``int16``) only, takes a single argument, and returns
the element-by-element, bit-wise inverse of the array. If a ``float`` is
supplied, the function raises a ``ValueError`` exception.

With signed integers (``int8``, and ``int16``), the results might be
unexpected, as in the example below:

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([0, -1, -100], dtype=np.int8)
    print("a:\t\t", a)
    print("inverse of a:\t", ~a)
    
    a = np.array([0, 1, 254, 255], dtype=np.uint8)
    print("\na:\t\t", a)
    print("inverse of a:\t", ~a)

.. parsed-literal::

    a:		 array([0, -1, -100], dtype=int8)
    inverse of a:	 array([-1, 0, 99], dtype=int8)
    
    a:		 array([0, 1, 254, 255], dtype=uint8)
    inverse of a:	 array([255, 254, 1, 0], dtype=uint8)
    
    


abs
---

This function takes a single argument, and returns the
element-by-element absolute value of the array. When the data type is
unsigned (``uint8``, or ``uint16``), a copy of the array will be
returned immediately, and no calculation takes place.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([0, -1, -100], dtype=np.int8)
    print("a:\t\t\t ", a)
    print("absolute value of a:\t ", abs(a))

.. parsed-literal::

    a:			  array([0, -1, -100], dtype=int8)
    absolute value of a:	  array([0, 1, 100], dtype=int8)
    
    


neg
---

This operator takes a single argument, and changes the sign of each
element in the array. Unsigned values are wrapped.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([10, -1, 1], dtype=np.int8)
    print("a:\t\t", a)
    print("negative of a:\t", -a)
    
    b = np.array([0, 100, 200], dtype=np.uint8)
    print("\nb:\t\t", b)
    print("negative of b:\t", -b)

.. parsed-literal::

    a:		 array([10, -1, 1], dtype=int8)
    negative of a:	 array([-10, 1, -1], dtype=int8)
    
    b:		 array([0, 100, 200], dtype=uint8)
    negative of b:	 array([0, 156, 56], dtype=uint8)
    
    


pos
---

This function takes a single argument, and simply returns a copy of the
array.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([10, -1, 1], dtype=np.int8)
    print("a:\t\t", a)
    print("positive of a:\t", +a)

.. parsed-literal::

    a:		 array([10, -1, 1], dtype=int8)
    positive of a:	 array([10, -1, 1], dtype=int8)
    
    


Binary operators
================

``ulab`` implements the ``+``, ``-``, ``*``, ``/``, ``**``, ``<``,
``>``, ``<=``, ``>=``, ``==``, ``!=``, ``+=``, ``-=``, ``*=``, ``/=``,
``**=`` binary operators, as well as the ``AND``, ``OR``, ``XOR``
bit-wise operators that work element-wise. Note that the bit-wise
operators will raise an exception, if either of the operands is of
``float`` or ``complex`` type.

Broadcasting is available, meaning that the two operands do not even
have to have the same shape. If the lengths along the respective axes
are equal, or one of them is 1, or the axis is missing, the element-wise
operation can still be carried out. A thorough explanation of
broadcasting can be found under
https://numpy.org/doc/stable/user/basics.broadcasting.html.

**WARNING**: note that relational operators (``<``, ``>``, ``<=``,
``>=``, ``==``, ``!=``) should have the ``ndarray`` on their left hand
side, when compared to scalars. This means that the following works

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([1, 2, 3])
    print(a > 2)

.. parsed-literal::

    array([False, False, True], dtype=bool)
    
    


while the equivalent statement, ``2 < a``, will raise a ``TypeError``
exception:

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([1, 2, 3])
    print(2 < a)

.. parsed-literal::

    
    Traceback (most recent call last):
      File "/dev/shm/micropython.py", line 5, in <module>
    TypeError: unsupported types for __lt__: 'int', 'ndarray'
    


**WARNING:** ``circuitpython`` users should use the ``equal``, and
``not_equal`` operators instead of ``==``, and ``!=``. See the section
on `array comparison <#Comparison-of-arrays>`__ for details.

Upcasting
---------

Binary operations require special attention, because two arrays with
different typecodes can be the operands of an operation, in which case
it is not trivial, what the typecode of the result is. This decision on
the result’s typecode is called upcasting. Since the number of typecodes
in ``ulab`` is significantly smaller than in ``numpy``, we have to
define new upcasting rules. Where possible, I followed ``numpy``\ ’s
conventions.

``ulab`` observes the following upcasting rules:

1. Operations on two ``ndarray``\ s of the same ``dtype`` preserve their
   ``dtype``, even when the results overflow.

2. if either of the operands is a float, the result is automatically a
   float

3. When one of the operands is a scalar, it will internally be turned
   into a single-element ``ndarray`` with the *smallest* possible
   ``dtype``. Thus, e.g., if the scalar is 123, it will be converted
   into an array of ``dtype`` ``uint8``, while -1000 will be converted
   into ``int16``. An ``mp_obj_float``, will always be promoted to
   ``dtype`` ``float``. Similarly, if ``ulab`` supports complex arrays,
   the result of a binary operation involving a ``complex`` array is
   always complex. Other ``micropython`` types (e.g., lists, tuples,
   etc.) raise a ``TypeError`` exception.

4. 

============== =============== =========== ============
left hand side right hand side ulab result numpy result
============== =============== =========== ============
``uint8``      ``int8``        ``int16``   ``int16``
``uint8``      ``int16``       ``int16``   ``int16``
``uint8``      ``uint16``      ``uint16``  ``uint16``
``int8``       ``int16``       ``int16``   ``int16``
``int8``       ``uint16``      ``uint16``  ``int32``
``uint16``     ``int16``       ``float``   ``int32``
============== =============== =========== ============

Note that the last two operations are promoted to ``int32`` in
``numpy``.

**WARNING:** Due to the lower number of available data types, the
upcasting rules of ``ulab`` are slightly different to those of
``numpy``. Watch out for this, when porting code!

Upcasting can be seen in action in the following snippet:

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([1, 2, 3, 4], dtype=np.uint8)
    b = np.array([1, 2, 3, 4], dtype=np.int8)
    print("a:\t", a)
    print("b:\t", b)
    print("a+b:\t", a+b)
    
    c = np.array([1, 2, 3, 4], dtype=np.float)
    print("\na:\t", a)
    print("c:\t", c)
    print("a*c:\t", a*c)

.. parsed-literal::

    a:	 array([1, 2, 3, 4], dtype=uint8)
    b:	 array([1, 2, 3, 4], dtype=int8)
    a+b:	 array([2, 4, 6, 8], dtype=int16)
    
    a:	 array([1, 2, 3, 4], dtype=uint8)
    c:	 array([1.0, 2.0, 3.0, 4.0], dtype=float64)
    a*c:	 array([1.0, 4.0, 9.0, 16.0], dtype=float64)
    
    


Benchmarks
----------

The following snippet compares the performance of binary operations to a
possible implementation in python. For the time measurement, we will
take the following snippet from the micropython manual:

.. code::
        
    # code to be run in micropython
    
    import utime
    
    def timeit(f, *args, **kwargs):
        func_name = str(f).split(' ')[1]
        def new_func(*args, **kwargs):
            t = utime.ticks_us()
            result = f(*args, **kwargs)
            print('execution time: ', utime.ticks_diff(utime.ticks_us(), t), ' us')
            return result
        return new_func

.. parsed-literal::

    


.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    @timeit
    def py_add(a, b):
        return [a[i]+b[i] for i in range(1000)]
    
    @timeit
    def py_multiply(a, b):
        return [a[i]*b[i] for i in range(1000)]
    
    @timeit
    def ulab_add(a, b):
        return a + b
    
    @timeit
    def ulab_multiply(a, b):
        return a * b
    
    a = [0.0]*1000
    b = range(1000)
    
    print('python add:')
    py_add(a, b)
    
    print('\npython multiply:')
    py_multiply(a, b)
    
    a = np.linspace(0, 10, num=1000)
    b = np.ones(1000)
    
    print('\nulab add:')
    ulab_add(a, b)
    
    print('\nulab multiply:')
    ulab_multiply(a, b)

.. parsed-literal::

    python add:
    execution time:  10051  us
    
    python multiply:
    execution time:  14175  us
    
    ulab add:
    execution time:  222  us
    
    ulab multiply:
    execution time:  213  us
    


The python implementation above is not perfect, and certainly, there is
much room for improvement. However, the factor of 50 difference in
execution time is very spectacular. This is nothing but a consequence of
the fact that the ``ulab`` functions run ``C`` code, with very little
python overhead. The factor of 50 appears to be quite universal: the FFT
routine obeys similar scaling (see `Speed of FFTs <#Speed-of-FFTs>`__),
and this number came up with font rendering, too: `fast font rendering
on graphical
displays <https://forum.micropython.org/viewtopic.php?f=15&t=5815&p=33362&hilit=ufont#p33383>`__.

Comparison operators
====================

The smaller than, greater than, smaller or equal, and greater or equal
operators return a vector of Booleans indicating the positions
(``True``), where the condition is satisfied.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([1, 2, 3, 4, 5, 6, 7, 8], dtype=np.uint8)
    print(a < 5)

.. parsed-literal::

    array([True, True, True, True, False, False, False, False], dtype=bool)
    
    


**WARNING**: at the moment, due to ``micropython``\ ’s implementation
details, the ``ndarray`` must be on the left hand side of the relational
operators.

That is, while ``a < 5`` and ``5 > a`` have the same meaning, the
following code will not work:

.. code::
        
    # code to be run in micropython
    
    import ulab as np
    
    a = np.array([1, 2, 3, 4, 5, 6, 7, 8], dtype=np.uint8)
    print(5 > a)

.. parsed-literal::

    
    Traceback (most recent call last):
      File "/dev/shm/micropython.py", line 5, in <module>
    TypeError: unsupported types for __gt__: 'int', 'ndarray'
    


Iterating over arrays
=====================

``ndarray``\ s are iterable, which means that their elements can also be
accessed as can the elements of a list, tuple, etc. If the array is
one-dimensional, the iterator returns scalars, otherwise a new
reduced-dimensional *view* is created and returned.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([1, 2, 3, 4, 5], dtype=np.uint8)
    b = np.array([range(5), range(10, 15, 1), range(20, 25, 1), range(30, 35, 1)], dtype=np.uint8)
    
    print("a:\t", a)
    
    for i, _a in enumerate(a):
        print("element %d in a:"%i, _a)
        
    print("\nb:\t", b)
    
    for i, _b in enumerate(b):
        print("element %d in b:"%i, _b)

.. parsed-literal::

    a:	 array([1, 2, 3, 4, 5], dtype=uint8)
    element 0 in a: 1
    element 1 in a: 2
    element 2 in a: 3
    element 3 in a: 4
    element 4 in a: 5
    
    b:	 array([[0, 1, 2, 3, 4],
           [10, 11, 12, 13, 14],
           [20, 21, 22, 23, 24],
           [30, 31, 32, 33, 34]], dtype=uint8)
    element 0 in b: array([0, 1, 2, 3, 4], dtype=uint8)
    element 1 in b: array([10, 11, 12, 13, 14], dtype=uint8)
    element 2 in b: array([20, 21, 22, 23, 24], dtype=uint8)
    element 3 in b: array([30, 31, 32, 33, 34], dtype=uint8)
    
    


Slicing and indexing
====================

Views vs. copies
----------------

``numpy`` has a very important concept called *views*, which is a
powerful extension of ``python``\ ’s own notion of slicing. Slices are
special python objects of the form

.. code:: python

   slice = start:end:stop

where ``start``, ``end``, and ``stop`` are (not necessarily
non-negative) integers. Not all of these three numbers must be specified
in an index, in fact, all three of them can be missing. The interpreter
takes care of filling in the missing values. (Note that slices cannot be
defined in this way, only there, where an index is expected.) For a good
explanation on how slices work in python, you can read the stackoverflow
question
https://stackoverflow.com/questions/509211/understanding-slice-notation.

In order to see what slicing does, let us take the string
``a = '012345679'``! We can extract every second character by creating
the slice ``::2``, which is equivalent to ``0:len(a):2``, i.e.,
increments the character pointer by 2 starting from 0, and traversing
the string up to the very end.

.. code::

    # code to be run in CPython
    
    string = '0123456789'
    string[::2]



.. parsed-literal::

    '02468'



Now, we can do the same with numerical arrays.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array(range(10), dtype=np.uint8)
    print('a:\t', a)
    
    print('a[::2]:\t', a[::2])

.. parsed-literal::

    a:	 array([0, 1, 2, 3, 4, 5, 6, 7, 8, 9], dtype=uint8)
    a[::2]:	 array([0, 2, 4, 6, 8], dtype=uint8)
    
    


This looks similar to ``string`` above, but there is a very important
difference that is not so obvious. Namely, ``string[::2]`` produces a
partial copy of ``string``, while ``a[::2]`` only produces a *view* of
``a``. What this means is that ``a``, and ``a[::2]`` share their data,
and the only difference between the two is, how the data are read out.
In other words, internally, ``a[::2]`` has the same data pointer as
``a``. We can easily convince ourselves that this is indeed the case by
calling the `ndinfo <#The_ndinfo_function>`__ function: the *data
pointer* entry is the same in the two printouts.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array(range(10), dtype=np.uint8)
    print('a: ', a, '\n')
    np.ndinfo(a)
    print('\n' + '='*20)
    print('a[::2]: ', a[::2], '\n')
    np.ndinfo(a[::2])

.. parsed-literal::

    a:  array([0, 1, 2, 3, 4, 5, 6, 7, 8, 9], dtype=uint8) 
    
    class: ndarray
    shape: (10,)
    strides: (1,)
    itemsize: 1
    data pointer: 0x7ff6c6193220
    type: uint8
    
    ====================
    a[::2]:  array([0, 2, 4, 6, 8], dtype=uint8) 
    
    class: ndarray
    shape: (5,)
    strides: (2,)
    itemsize: 1
    data pointer: 0x7ff6c6193220
    type: uint8
    
    


If you are still a bit confused about the meaning of *views*, the
section `Slicing and assigning to
slices <#Slicing-and-assigning-to-slices>`__ should clarify the issue.

Indexing
--------

The simplest form of indexing is specifying a single integer between the
square brackets as in

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array(range(10), dtype=np.uint8)
    print("a: ", a)
    print("the first, and last element of a:\n", a[0], a[-1])
    print("the second, and last but one element of a:\n", a[1], a[-2])

.. parsed-literal::

    a:  array([0, 1, 2, 3, 4, 5, 6, 7, 8, 9], dtype=uint8)
    the first, and last element of a:
     0 9
    the second, and last but one element of a:
     1 8
    
    


Indexing can be applied to higher-dimensional tensors, too. When the
length of the indexing sequences is smaller than the number of
dimensions, a new *view* is returned, otherwise, we get a single number.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array(range(9), dtype=np.uint8).reshape((3, 3))
    print("a:\n", a)
    print("a[0]:\n", a[0])
    print("a[1,1]: ", a[1,1])

.. parsed-literal::

    a:
     array([[0, 1, 2],
    	[3, 4, 5],
    	[6, 7, 8]], dtype=uint8)
    a[0]:
     array([[0, 1, 2]], dtype=uint8)
    a[1,1]:  4
    
    


Indices can also be a list of Booleans. By using a Boolean list, we can
select those elements of an array that satisfy a specific condition. At
the moment, such indexing is defined for row vectors only; when the rank
of the tensor is higher than 1, the function raises a
``NotImplementedError`` exception, though this will be rectified in a
future version of ``ulab``.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array(range(9), dtype=np.float)
    print("a:\t", a)
    print("a[a < 5]:\t", a[a < 5])

.. parsed-literal::

    a:	 array([0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0], dtype=float)
    a[a < 5]:	 array([0.0, 1.0, 2.0, 3.0, 4.0], dtype=float)
    
    


Indexing with Boolean arrays can take more complicated expressions. This
is a very concise way of comparing two vectors, e.g.:

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array(range(9), dtype=np.uint8)
    b = np.array([4, 4, 4, 3, 3, 3, 13, 13, 13], dtype=np.uint8)
    print("a:\t", a)
    print("\na**2:\t", a*a)
    print("\nb:\t", b)
    print("\n100*sin(b):\t", np.sin(b)*100.0)
    print("\na[a*a > np.sin(b)*100.0]:\t", a[a*a > np.sin(b)*100.0])

.. parsed-literal::

    a:	 array([0, 1, 2, 3, 4, 5, 6, 7, 8], dtype=uint8)
    
    a**2:	 array([0, 1, 4, 9, 16, 25, 36, 49, 64], dtype=uint16)
    
    b:	 array([4, 4, 4, 3, 3, 3, 13, 13, 13], dtype=uint8)
    
    100*sin(b):	 array([-75.68024953079282, -75.68024953079282, -75.68024953079282, 14.11200080598672, 14.11200080598672, 14.11200080598672, 42.01670368266409, 42.01670368266409, 42.01670368266409], dtype=float)
    
    a[a*a > np.sin(b)*100.0]:	 array([0, 1, 2, 4, 5, 7, 8], dtype=uint8)
    
    


Boolean indices can also be used in assignments, if the array is
one-dimensional. The following example replaces the data in an array,
wherever some condition is fulfilled.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array(range(9), dtype=np.uint8)
    b = np.array(range(9)) + 12
    
    print(a[b < 15])
    
    a[b < 15] = 123
    print(a)

.. parsed-literal::

    array([0, 1, 2], dtype=uint8)
    array([123, 123, 123, 3, 4, 5, 6, 7, 8], dtype=uint8)
    
    


On the right hand side of the assignment we can even have another array.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array(range(9), dtype=np.uint8)
    b = np.array(range(9)) + 12
    
    print(a[b < 15], b[b < 15])
    
    a[b < 15] = b[b < 15]
    print(a)

.. parsed-literal::

    array([0, 1, 2], dtype=uint8) array([12.0, 13.0, 14.0], dtype=float)
    array([12, 13, 14, 3, 4, 5, 6, 7, 8], dtype=uint8)
    
    


Slicing and assigning to slices
-------------------------------

You can also generate sub-arrays by specifying slices as the index of an
array. Slices are special python objects of the form

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([[1, 2, 3], [4, 5, 6], [7, 8, 9]], dtype=np.uint8)
    print('a:\n', a)
    
    # the first row
    print('\na[0]:\n', a[0])
    
    # the first two elements of the first row
    print('\na[0,:2]:\n', a[0,:2])
    
    # the zeroth element in each row (also known as the zeroth column)
    print('\na[:,0]:\n', a[:,0])
    
    # the last row
    print('\na[-1]:\n', a[-1])
    
    # the last two rows backwards
    print('\na[-1:-3:-1]:\n', a[-1:-3:-1])

.. parsed-literal::

    a:
     array([[1, 2, 3],
    	[4, 5, 6],
    	[7, 8, 9]], dtype=uint8)
    
    a[0]:
     array([[1, 2, 3]], dtype=uint8)
    
    a[0,:2]:
     array([[1, 2]], dtype=uint8)
    
    a[:,0]:
     array([[1],
    	[4],
    	[7]], dtype=uint8)
    
    a[-1]:
     array([[7, 8, 9]], dtype=uint8)
    
    a[-1:-3:-1]:
     array([[7, 8, 9],
    	[4, 5, 6]], dtype=uint8)
    
    


Assignment to slices can be done for the whole slice, per row, and per
column. A couple of examples should make these statements clearer:

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.zeros((3, 3), dtype=np.uint8)
    print('a:\n', a)
    
    # assigning to the whole row
    a[0] = 1
    print('\na[0] = 1\n', a)
    
    a = np.zeros((3, 3), dtype=np.uint8)
    
    # assigning to a column
    a[:,2] = 3.0
    print('\na[:,0]:\n', a)

.. parsed-literal::

    a:
     array([[0, 0, 0],
    	[0, 0, 0],
    	[0, 0, 0]], dtype=uint8)
    
    a[0] = 1
     array([[1, 1, 1],
    	[0, 0, 0],
    	[0, 0, 0]], dtype=uint8)
    
    a[:,0]:
     array([[0, 0, 3],
    	[0, 0, 3],
    	[0, 0, 3]], dtype=uint8)
    
    


Now, you should notice that we re-set the array ``a`` after the first
assignment. Do you care to see what happens, if we do not do that? Well,
here are the results:

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.zeros((3, 3), dtype=np.uint8)
    b = a[:,:]
    # assign 1 to the first row
    b[0] = 1
    
    # assigning to the last column
    b[:,2] = 3
    print('a: ', a)

.. parsed-literal::

    a:  array([[1, 1, 3],
    	[0, 0, 3],
    	[0, 0, 3]], dtype=uint8)
    
    


Note that both assignments involved ``b``, and not ``a``, yet, when we
print out ``a``, its entries are updated. This proves our earlier
statement about the behaviour of *views*: in the statement
``b = a[:,:]`` we simply created a *view* of ``a``, and not a *deep*
copy of it, meaning that whenever we modify ``b``, we actually modify
``a``, because the underlying data container of ``a`` and ``b`` are
shared between the two object. Having a single data container for two
seemingly different objects provides an extremely powerful way of
manipulating sub-sets of numerical data.

If you want to work on a *copy* of your data, you can use the ``.copy``
method of the ``ndarray``. The following snippet should drive the point
home:

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.zeros((3, 3), dtype=np.uint8)
    b = a.copy()
    
    # get the address of the underlying data pointer
    
    np.ndinfo(a)
    print()
    np.ndinfo(b)
    
    # assign 1 to the first row of b, and do not touch a
    b[0] = 1
    
    print()
    print('a: ', a)
    print('='*20)
    print('b: ', b)

.. parsed-literal::

    class: ndarray
    shape: (3, 3)
    strides: (3, 1)
    itemsize: 1
    data pointer: 0x7ff737ea3220
    type: uint8
    
    class: ndarray
    shape: (3, 3)
    strides: (3, 1)
    itemsize: 1
    data pointer: 0x7ff737ea3340
    type: uint8
    
    a:  array([[0, 0, 0],
    	[0, 0, 0],
    	[0, 0, 0]], dtype=uint8)
    ====================
    b:  array([[1, 1, 1],
    	[0, 0, 0],
    	[0, 0, 0]], dtype=uint8)
    
    


The ``.copy`` method can also be applied to views: below, ``a[0]`` is a
*view* of ``a``, out of which we create a *deep copy* called ``b``. This
is a row vector now. We can then do whatever we want to with ``b``, and
that leaves ``a`` unchanged.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.zeros((3, 3), dtype=np.uint8)
    b = a[0].copy()
    print('b: ', b)
    print('='*20)
    # assign 1 to the first entry of b, and do not touch a
    b[0] = 1
    print('a: ', a)
    print('='*20)
    print('b: ', b)

.. parsed-literal::

    b:  array([0, 0, 0], dtype=uint8)
    ====================
    a:  array([[0, 0, 0],
    	[0, 0, 0],
    	[0, 0, 0]], dtype=uint8)
    ====================
    b:  array([1, 0, 0], dtype=uint8)
    
    


The fact that the underlying data of a view is the same as that of the
original array has another important consequence, namely, that the
creation of a view is cheap. Both in terms of RAM, and execution time. A
view is really nothing but a short header with a data array that already
exists, and is filled up. Hence, creating the view requires only the
creation of its header. This operation is fast, and uses virtually no
RAM.
