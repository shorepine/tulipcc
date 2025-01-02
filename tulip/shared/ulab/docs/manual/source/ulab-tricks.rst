
Tricks
======

This section of the book discusses a couple of tricks that can be
exploited to either speed up computations, or save on RAM. However,
there is probably no silver bullet, and you have to evaluate your code
in terms of execution speed (if the execution is time critical), or RAM
used. You should also keep in mind that, if a particular code snippet is
optimised on some hardware, there is no guarantee that on another piece
of hardware, you will get similar improvements. Hardware implementations
are vastly different. Some microcontrollers do not even have an FPU, so
you should not be surprised that you get significantly different
benchmarks. Just to underline this statement, you can study the
`collection of benchmarks <https://github.com/thiagofe/ulab_samples>`__.

Use an ``ndarray``, if you can
------------------------------

Many functions in ``ulab`` are implemented in a universal fashion,
meaning that both generic ``micropython`` iterables, and ``ndarray``\ s
can be passed as an argument. E.g., both

.. code:: python

   from ulab import numpy as np

   np.sum([1, 2, 3, 4, 5])

and

.. code:: python

   from ulab import numpy as np

   a = np.array([1, 2, 3, 4, 5])
   np.sum(a)

will return the ``micropython`` variable 15 as the result. Still,
``np.sum(a)`` is evaluated significantly faster, because in
``np.sum([1, 2, 3, 4, 5])``, the interpreter has to fetch 5
``micropython`` variables, convert them to ``float``, and sum the
values, while the C type of ``a`` is known, thus the interpreter can
invoke a single ``for`` loop for the evaluation of the ``sum``. In the
``for`` loop, there are no function calls, the iteration simply walks
through the pointer holding the values of ``a``, and adds the values to
an accumulator. If the array ``a`` is already available, then you can
gain a factor of 3 in speed by calling ``sum`` on the array, instead of
using the list. Compared to the python implementation of the same
functionality, the speed-up is around 40 (again, this might depend on
the hardware).

On the other hand, if the array is not available, then there is not much
point in converting the list to an ``ndarray`` and passing that to the
function. In fact, you should expect a slow-down: the constructor has to
iterate over the list elements, and has to convert them to a numerical
type. On top of that, it also has to reserve RAM for the ``ndarray``.

Use a reasonable ``dtype``
--------------------------

Just as in ``numpy``, the default ``dtype`` is ``float``. But this does
not mean that that is the most suitable one in all scenarios. If data
are streamed from an 8-bit ADC, and you only want to know the maximum,
or the sum, then it is quite reasonable to use ``uint8`` for the
``dtype``. Storing the same data in ``float`` array would cost 4 or 8
times as much RAM, with absolutely no gain. Do not rely on the default
value of the constructor’s keyword argument, and choose one that fits!

Beware the axis!
----------------

Whenever ``ulab`` iterates over multi-dimensional arrays, the outermost
loop is the first axis, then the second axis, and so on. E.g., when the
``sum`` of

.. code:: python

   a = array([[1, 2, 3, 4],
              [5, 6, 7, 8], 
              [9, 10, 11, 12]], dtype=uint8)

is being calculated, first the data pointer walks along ``[1, 2, 3, 4]``
(innermost loop, last axis), then is moved back to the position, where 5
is stored (this is the nesting loop), and traverses ``[5, 6, 7, 8]``,
and so on. Moving the pointer back to 5 is more expensive, than moving
it along an axis, because the position of 5 has to be calculated,
whereas moving from 5 to 6 is simply an addition to the address. Thus,
while the matrix

.. code:: python

   b = array([[1, 5, 9],
              [2, 6, 10], 
              [3, 7, 11],
              [4, 8, 12]], dtype=uint8)

holds the same data as ``a``, the summation over the entries in ``b`` is
slower, because the pointer has to be re-wound three times, as opposed
to twice in ``a``. For small matrices the savings are not significant,
but you would definitely notice the difference, if you had

::

   a = array(range(2000)).reshape((2, 1000))
   b = array(range(2000)).reshape((1000, 2))

The moral is that, in order to improve on the execution speed, whenever
possible, you should try to make the last axis the longest. As a side
note, ``numpy`` can re-arrange its loops, and puts the longest axis in
the innermost loop. This is why the longest axis is sometimes referred
to as the fast axis. In ``ulab``, the order of the axes is fixed.

Reduce the number of artifacts
------------------------------

Before showing a real-life example, let us suppose that we want to
interpolate uniformly sampled data, and the absolute magnitude is not
really important, we only care about the ratios between neighbouring
value. One way of achieving this is calling the ``interp`` functions.
However, we could just as well work with slices.

.. code::

    # code to be run in CPython
    
    a = array([0, 10, 2, 20, 4], dtype=np.uint8)
    b = np.zeros(9, dtype=np.uint8)
    
    b[::2] = 2 * a
    b[1::2] = a[:-1] + a[1:]
    
    b //= 2
    b



.. parsed-literal::

    array([ 0,  5, 10,  6,  2, 11, 20, 12,  4], dtype=uint8)



``b`` now has values from ``a`` at every even position, and interpolates
the values on every odd position. If only the relative magnitudes are
important, then we can even save the division by 2, and we end up with

.. code::

    # code to be run in CPython
    
    a = array([0, 10, 2, 20, 4], dtype=np.uint8)
    b = np.zeros(9, dtype=np.uint8)
    
    b[::2] = 2 * a
    b[1::2] = a[:-1] + a[1:]
    
    b



.. parsed-literal::

    array([ 0, 10, 20, 12,  4, 22, 40, 24,  8], dtype=uint8)



Importantly, we managed to keep the results in the smaller ``dtype``,
``uint8``. Now, while the two assignments above are terse and pythonic,
the code is not the most efficient: the right hand sides are compound
statements, generating intermediate results. To store them, RAM has to
be allocated. This takes time, and leads to memory fragmentation. Better
is to write out the assignments in 4 instructions:

.. code::

    # code to be run in CPython
    
    b = np.zeros(9, dtype=np.uint8)
    
    b[::2] = a
    b[::2] += a
    b[1::2] = a[:-1]
    b[1::2] += a[1:]
    
    b



.. parsed-literal::

    array([ 0, 10, 20, 12,  4, 22, 40, 24,  8], dtype=uint8)



The results are the same, but no extra RAM is allocated, except for the
views ``a[:-1]``, and ``a[1:]``, but those had to be created even in the
origin implementation.

Upscaling images
~~~~~~~~~~~~~~~~

And now the example: there are low-resolution thermal cameras out there.
Low resolution might mean 8 by 8 pixels. Such a small number of pixels
is just not reasonable to plot, no matter how small the display is. If
you want to make the camera image a bit more pleasing, you can upscale
(stretch) it in both dimensions. This can be done exactly as we
up-scaled the linear array:

.. code::

    # code to be run in CPython
    
    b = np.zeros((15, 15), dtype=np.uint8)
    
    b[1::2,::2] = a[:-1,:]
    b[1::2,::2] += a[1:, :]
    b[1::2,::2] //= 2
    b[::,1::2] = a[::,:-1:2]
    b[::,1::2] += a[::,2::2]
    b[::,1::2] //= 2
Up-scaling by larger numbers can be done in a similar fashion, you
simply have more assignments.

There are cases, when one cannot do away with the intermediate results.
Two prominent cases are the ``where`` function, and indexing by means of
a Boolean array. E.g., in

.. code::

    # code to be run in CPython
    
    a = array([1, 2, 3, 4, 5])
    b = a[a < 4]
    b



.. parsed-literal::

    array([1, 2, 3])



the expression ``a < 4`` produces the Boolean array,

.. code::

    # code to be run in CPython
    
    a < 4



.. parsed-literal::

    array([ True,  True,  True, False, False])



If you repeatedly have such conditions in a loop, you might have to
peridically call the garbage collector to remove the Boolean arrays that
are used only once.

.. code::

    # code to be run in CPython
    
