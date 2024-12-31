
numpy.fft
=========

Functions related to Fourier transforms can be called by prepending them
with ``numpy.fft.``. The module defines the following two functions:

1. `numpy.fft.fft <#fft>`__
2. `numpy.fft.ifft <#ifft>`__

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.fft.ifft.html

fft
---

Since ``ulab``\ ’s ``ndarray`` does not support complex numbers, the
invocation of the Fourier transform differs from that in ``numpy``. In
``numpy``, you can simply pass an array or iterable to the function, and
it will be treated as a complex array:

.. code::

    # code to be run in CPython
    
    fft.fft([1, 2, 3, 4, 1, 2, 3, 4])



.. parsed-literal::

    array([20.+0.j,  0.+0.j, -4.+4.j,  0.+0.j, -4.+0.j,  0.+0.j, -4.-4.j,
            0.+0.j])



**WARNING:** The array returned is also complex, i.e., the real and
imaginary components are cast together. In ``ulab``, the real and
imaginary parts are treated separately: you have to pass two
``ndarray``\ s to the function, although, the second argument is
optional, in which case the imaginary part is assumed to be zero.

**WARNING:** The function, as opposed to ``numpy``, returns a 2-tuple,
whose elements are two ``ndarray``\ s, holding the real and imaginary
parts of the transform separately.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    x = np.linspace(0, 10, num=1024)
    y = np.sin(x)
    z = np.zeros(len(x))
    
    a, b = np.fft.fft(x)
    print('real part:\t', a)
    print('\nimaginary part:\t', b)
    
    c, d = np.fft.fft(x, z)
    print('\nreal part:\t', c)
    print('\nimaginary part:\t', d)

.. parsed-literal::

    real part:	 array([5119.996, -5.004663, -5.004798, ..., -5.005482, -5.005643, -5.006577], dtype=float)
    
    imaginary part:	 array([0.0, 1631.333, 815.659, ..., -543.764, -815.6588, -1631.333], dtype=float)
    
    real part:	 array([5119.996, -5.004663, -5.004798, ..., -5.005482, -5.005643, -5.006577], dtype=float)
    
    imaginary part:	 array([0.0, 1631.333, 815.659, ..., -543.764, -815.6588, -1631.333], dtype=float)
    


ulab with complex support
~~~~~~~~~~~~~~~~~~~~~~~~~

If the ``ULAB_SUPPORTS_COMPLEX``, and ``ULAB_FFT_IS_NUMPY_COMPATIBLE``
pre-processor constants are set to 1 in
`ulab.h <https://github.com/v923z/micropython-ulab/blob/master/code/ulab.h>`__
as

.. code:: c

   // Adds support for complex ndarrays
   #ifndef ULAB_SUPPORTS_COMPLEX
   #define ULAB_SUPPORTS_COMPLEX               (1)
   #endif

.. code:: c

   #ifndef ULAB_FFT_IS_NUMPY_COMPATIBLE
   #define ULAB_FFT_IS_NUMPY_COMPATIBLE    (1)
   #endif

then the FFT routine will behave in a ``numpy``-compatible way: the
single input array can either be real, in which case the imaginary part
is assumed to be zero, or complex. The output is also complex.

While ``numpy``-compatibility might be a desired feature, it has one
side effect, namely, the FFT routine consumes approx. 50% more RAM. The
reason for this lies in the implementation details.

ifft
----

The above-mentioned rules apply to the inverse Fourier transform. The
inverse is also normalised by ``N``, the number of elements, as is
customary in ``numpy``. With the normalisation, we can ascertain that
the inverse of the transform is equal to the original array.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    x = np.linspace(0, 10, num=1024)
    y = np.sin(x)
    
    a, b = np.fft.fft(y)
    
    print('original vector:\t', y)
    
    y, z = np.fft.ifft(a, b)
    # the real part should be equal to y
    print('\nreal part of inverse:\t', y)
    # the imaginary part should be equal to zero
    print('\nimaginary part of inverse:\t', z)

.. parsed-literal::

    original vector:	 array([0.0, 0.009775016, 0.0195491, ..., -0.5275068, -0.5357859, -0.5440139], dtype=float)
    
    real part of inverse:	 array([-2.980232e-08, 0.0097754, 0.0195494, ..., -0.5275064, -0.5357857, -0.5440133], dtype=float)
    
    imaginary part of inverse:	 array([-2.980232e-08, -1.451171e-07, 3.693752e-08, ..., 6.44871e-08, 9.34986e-08, 2.18336e-07], dtype=float)
    


Note that unlike in ``numpy``, the length of the array on which the
Fourier transform is carried out must be a power of 2. If this is not
the case, the function raises a ``ValueError`` exception.

ulab with complex support
~~~~~~~~~~~~~~~~~~~~~~~~~

The ``fft.ifft`` function can also be made ``numpy``-compatible by
setting the ``ULAB_SUPPORTS_COMPLEX``, and
``ULAB_FFT_IS_NUMPY_COMPATIBLE`` pre-processor constants to 1.

Computation and storage costs
-----------------------------

RAM
~~~

The FFT routine of ``ulab`` calculates the transform in place. This
means that beyond reserving space for the two ``ndarray``\ s that will
be returned (the computation uses these two as intermediate storage
space), only a handful of temporary variables, all floats or 32-bit
integers, are required.

Speed of FFTs
~~~~~~~~~~~~~

A comment on the speed: a 1024-point transform implemented in python
would cost around 90 ms, and 13 ms in assembly, if the code runs on the
pyboard, v.1.1. You can gain a factor of four by moving to the D series
https://github.com/peterhinch/micropython-fourier/blob/master/README.md#8-performance.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    x = np.linspace(0, 10, num=1024)
    y = np.sin(x)
    
    @timeit
    def np_fft(y):
        return np.fft.fft(y)
    
    a, b = np_fft(y)

.. parsed-literal::

    execution time:  1985  us
    


The C implementation runs in less than 2 ms on the pyboard (we have just
measured that), and has been reported to run in under 0.8 ms on the D
series board. That is an improvement of at least a factor of four.
