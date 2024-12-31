
Introduction
============

Enter ulab
----------

``ulab`` is a ``numpy``-like module for ``micropython`` and its
derivatives, meant to simplify and speed up common mathematical
operations on arrays. ``ulab`` implements a small subset of ``numpy``
and ``scipy``, as well as a number of functions manipulating byte
arrays. The functions were chosen such that they might be useful in the
context of a microcontroller. However, the project is a living one, and
suggestions for new features are always welcome.

This document discusses how you can use the library, starting from
building your own firmware, through questions like what affects the
firmware size, what are the trade-offs, and what are the most important
differences to ``numpy`` and ``scipy``, respectively. The document is
organised as follows:

The chapter after this one helps you with firmware customisation.

The third chapter gives a very concise summary of the ``ulab`` functions
and array methods. This chapter can be used as a quick reference.

The chapters after that are an in-depth review of most functions. Here
you can find usage examples, benchmarks, as well as a thorough
discussion of such concepts as broadcasting, and views versus copies.

The final chapter of this book can be regarded as the programming
manual. The inner working of ``ulab`` is dissected here, and you will
also find hints as to how to implement your own ``numpy``-compatible
functions.

Purpose
-------

Of course, the first question that one has to answer is, why on Earth
one would need a fast math library on a microcontroller. After all, it
is not expected that heavy number crunching is going to take place on
bare metal. It is not meant to. On a PC, the main reason for writing
fast code is the sheer amount of data that one wants to process. On a
microcontroller, the data volume is probably small, but it might lead to
catastrophic system failure, if these data are not processed in time,
because the microcontroller is supposed to interact with the outside
world in a timely fashion. In fact, this latter objective was the
initiator of this project: I needed the Fourier transform of a signal
coming from the ADC of the ``pyboard``, and all available options were
simply too slow.

In addition to speed, another issue that one has to keep in mind when
working with embedded systems is the amount of available RAM: I believe,
everything here could be implemented in pure ``python`` with relatively
little effort (in fact, there are a couple of ``python``-only
implementations of ``numpy`` functions out there), but the price we
would have to pay for that is not only speed, but RAM, too. ``python``
code, if is not frozen, and compiled into the firmware, has to be
compiled at runtime, which is not exactly a cheap process. On top of
that, if numbers are stored in a list or tuple, which would be the
high-level container, then they occupy 8 bytes, no matter, whether they
are all smaller than 100, or larger than one hundred million. This is
obviously a waste of resources in an environment, where resources are
scarce.

Finally, there is a reason for using ``micropython`` in the first place.
Namely, that a microcontroller can be programmed in a very elegant, and
*pythonic* way. But if it is so, why should we not extend this idea to
other tasks and concepts that might come up in this context? If there
was no other reason than this *elegance*, I would find that convincing
enough.

Based on the above-mentioned considerations, all functions in ``ulab``
are implemented in a way that

1. conforms to ``numpy`` as much as possible
2. is so frugal with RAM as possible,
3. and yet, fast. Much faster than pure python. Think of speed-ups of
   30-50!

The main points of ``ulab`` are

-  compact, iterable and slicable containers of numerical data in one to
   four dimensions. These containers support all the relevant unary and
   binary operators (e.g., ``len``, ==, +, \*, etc.)
-  vectorised computations on ``micropython`` iterables and numerical
   arrays (in ``numpy``-speak, universal functions)
-  computing statistical properties (mean, standard deviation etc.) on
   arrays
-  basic linear algebra routines (matrix inversion, multiplication,
   reshaping, transposition, determinant, and eigenvalues, Cholesky
   decomposition and so on)
-  polynomial fits to numerical data, and evaluation of polynomials
-  fast Fourier transforms
-  filtering of data (convolution and second-order filters)
-  function minimisation, fitting, and numerical approximation routines
-  interfacing between numerical data and peripheral hardware devices

``ulab`` implements close to a hundred functions and array methods. At
the time of writing this manual (for version 4.0.0), the library adds
approximately 120 kB of extra compiled code to the ``micropython``
(pyboard.v.1.17) firmware. However, if you are tight with flash space,
you can easily shave tens of kB off the firmware. In fact, if only a
small sub-set of functions are needed, you can get away with less than
10 kB of flash space. See the section on `customising
ulab <#Customising-the-firmware>`__.

Resources and legal matters
---------------------------

The source code of the module can be found under
https://github.com/v923z/micropython-ulab/tree/master/code. while the
source of this user manual is under
https://github.com/v923z/micropython-ulab/tree/master/docs.

The MIT licence applies to all material.

Friendly request
----------------

If you use ``ulab``, and bump into a bug, or think that a particular
function is missing, or its behaviour does not conform to ``numpy``,
please, raise a `ulab
issue <#https://github.com/v923z/micropython-ulab/issues>`__ on github,
so that the community can profit from your experiences.

Even better, if you find the project to be useful, and think that it
could be made better, faster, tighter, and shinier, please, consider
contributing, and issue a pull request with the implementation of your
improvements and new features. ``ulab`` can only become successful, if
it offers what the community needs.

These last comments apply to the documentation, too. If, in your
opinion, the documentation is obscure, misleading, or not detailed
enough, please, let us know, so that *we* can fix it.

Differences between micropython-ulab and circuitpython-ulab
-----------------------------------------------------------

``ulab`` has originally been developed for ``micropython``, but has
since been integrated into a number of its flavours. Most of these are
simply forks of ``micropython`` itself, with some additional
functionality. One of the notable exceptions is ``circuitpython``, which
has slightly diverged at the core level, and this has some minor
consequences. Some of these concern the C implementation details only,
which all have been sorted out with the generous and enthusiastic
support of Jeff Epler from `Adafruit
Industries <http://www.adafruit.com>`__.

There are, however, a couple of instances, where the two environments
differ at the python level in how the class properties can be accessed.
We will point out the differences and possible workarounds at the
relevant places in this document.

Customising the firmware
========================

As mentioned above, ``ulab`` has considerably grown since its
conception, which also means that it might no longer fit on the
microcontroller of your choice. There are, however, a couple of ways of
customising the firmware, and thereby reducing its size.

All ``ulab`` options are listed in a single header file,
`ulab.h <https://github.com/v923z/micropython-ulab/blob/master/code/ulab.h>`__,
which contains pre-processor flags for each feature that can be
fine-tuned. The first couple of lines of the file look like this

.. code:: c

   // The pre-processor constants in this file determine how ulab behaves:
   //
   // - how many dimensions ulab can handle
   // - which functions are included in the compiled firmware
   // - whether the python syntax is numpy-like, or modular
   // - whether arrays can be sliced and iterated over
   // - which binary/unary operators are supported
   //
   // A considerable amount of flash space can be saved by removing (setting
   // the corresponding constants to 0) the unnecessary functions and features.

   // Values defined here can be overridden by your own config file as
   // make -DULAB_CONFIG_FILE="my_ulab_config.h"
   #if defined(ULAB_CONFIG_FILE)
   #include ULAB_CONFIG_FILE
   #endif

   // Adds support for complex ndarrays
   #ifndef ULAB_SUPPORTS_COMPLEX
   #define ULAB_SUPPORTS_COMPLEX               (1)
   #endif

   // Determines, whether scipy is defined in ulab. The sub-modules and functions
   // of scipy have to be defined separately
   #define ULAB_HAS_SCIPY                      (1)

   // The maximum number of dimensions the firmware should be able to support
   // Possible values lie between 1, and 4, inclusive
   #define ULAB_MAX_DIMS                       2

   // By setting this constant to 1, iteration over array dimensions will be implemented
   // as a function (ndarray_rewind_array), instead of writing out the loops in macros
   // This reduces firmware size at the expense of speed
   #define ULAB_HAS_FUNCTION_ITERATOR          (0)

   // If NDARRAY_IS_ITERABLE is 1, the ndarray object defines its own iterator function
   // This option saves approx. 250 bytes of flash space
   #define NDARRAY_IS_ITERABLE                 (1)

   // Slicing can be switched off by setting this variable to 0
   #define NDARRAY_IS_SLICEABLE                (1)

   // The default threshold for pretty printing. These variables can be overwritten
   // at run-time via the set_printoptions() function
   #define ULAB_HAS_PRINTOPTIONS               (1)
   #define NDARRAY_PRINT_THRESHOLD             10
   #define NDARRAY_PRINT_EDGEITEMS             3

   // determines, whether the dtype is an object, or simply a character
   // the object implementation is numpythonic, but requires more space
   #define ULAB_HAS_DTYPE_OBJECT               (0)

   // the ndarray binary operators
   #define NDARRAY_HAS_BINARY_OPS              (1)

   // Firmware size can be reduced at the expense of speed by using function
   // pointers in iterations. For each operator, he function pointer saves around
   // 2 kB in the two-dimensional case, and around 4 kB in the four-dimensional case.

   #define NDARRAY_BINARY_USES_FUN_POINTER     (0)

   #define NDARRAY_HAS_BINARY_OP_ADD           (1)
   #define NDARRAY_HAS_BINARY_OP_EQUAL         (1)
   #define NDARRAY_HAS_BINARY_OP_LESS          (1)
   #define NDARRAY_HAS_BINARY_OP_LESS_EQUAL    (1)
   #define NDARRAY_HAS_BINARY_OP_MORE          (1)
   #define NDARRAY_HAS_BINARY_OP_MORE_EQUAL    (1)
   #define NDARRAY_HAS_BINARY_OP_MULTIPLY      (1)
   #define NDARRAY_HAS_BINARY_OP_NOT_EQUAL     (1)
   #define NDARRAY_HAS_BINARY_OP_POWER         (1)
   #define NDARRAY_HAS_BINARY_OP_SUBTRACT      (1)
   #define NDARRAY_HAS_BINARY_OP_TRUE_DIVIDE   (1)
   ...     

The meaning of flags with names ``_HAS_`` should be obvious, so we will
just explain the other options.

To see how much you can gain by un-setting the functions that you do not
need, here are some pointers. In four dimensions, including all
functions adds around 120 kB to the ``micropython`` firmware. On the
other hand, if you are interested in Fourier transforms only, and strip
everything else, you get away with less than 5 kB extra.

Compatibility with numpy
------------------------

The functions implemented in ``ulab`` are organised in four sub-modules
at the C level, namely, ``numpy``, ``scipy``, ``utils``, and ``user``.
This modularity is elevated to ``python``, meaning that in order to use
functions that are part of ``numpy``, you have to import ``numpy`` as

.. code:: python

   from ulab import numpy as np

   x = np.array([4, 5, 6])
   p = np.array([1, 2, 3])
   np.polyval(p, x)

There are a couple of exceptions to this rule, namely ``fft``,
``linalg``, and ``random``, which are sub-modules even in ``numpy``,
thus you have to write them out as

.. code:: python

   from ulab import numpy as np

   A = np.array([1, 2, 3, 4]).reshape()
   np.linalg.trace(A)

Some of the functions in ``ulab`` are re-implementations of ``scipy``
functions, and they are to be imported as

.. code:: python

   from ulab import numpy as np
   from ulab import scipy as spy


   x = np.array([1, 2, 3])
   spy.special.erf(x)

``numpy``-compatibility has an enormous benefit : namely, by
``try``\ ing to ``import``, we can guarantee that the same, unmodified
code runs in ``CPython``, as in ``micropython``. The following snippet
is platform-independent, thus, the ``python`` code can be tested and
debugged on a computer before loading it onto the microcontroller.

.. code:: python


   try:
       from ulab import numpy as np
       from ulab import scipy as spy
   except ImportError:
       import numpy as np
       import scipy as spy
       
   x = np.array([1, 2, 3])
   spy.special.erf(x)    

The impact of dimensionality
----------------------------

Reducing the number of dimensions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

``ulab`` supports tensors of rank four, but this is expensive in terms
of flash: with all available functions and options, the library adds
around 100 kB to the firmware. However, if such high dimensions are not
required, significant reductions in size can be gotten by changing the
value of

.. code:: c

   #define ULAB_MAX_DIMS                   2

Two dimensions cost a bit more than half of four, while you can get away
with around 20 kB of flash in one dimension, because all those functions
that don’t make sense (e.g., matrix inversion, eigenvalues etc.) are
automatically stripped from the firmware.

Using the function iterator
~~~~~~~~~~~~~~~~~~~~~~~~~~~

In higher dimensions, the firmware size increases, because each
dimension (axis) adds another level of nested loops. An example of this
is the macro of the binary operator in three dimensions

.. code:: c

   #define BINARY_LOOP(results, type_out, type_left, type_right, larray, lstrides, rarray, rstrides, OPERATOR)
       type_out *array = (type_out *)results->array;
       size_t j = 0;
       do {
           size_t k = 0;
           do {
               size_t l = 0;
               do {
                   *array++ = *((type_left *)(larray)) OPERATOR *((type_right *)(rarray));
                   (larray) += (lstrides)[ULAB_MAX_DIMS - 1];
                   (rarray) += (rstrides)[ULAB_MAX_DIMS - 1];
                   l++;
               } while(l < (results)->shape[ULAB_MAX_DIMS - 1]);
               (larray) -= (lstrides)[ULAB_MAX_DIMS - 1] * (results)->shape[ULAB_MAX_DIMS-1];
               (larray) += (lstrides)[ULAB_MAX_DIMS - 2];
               (rarray) -= (rstrides)[ULAB_MAX_DIMS - 1] * (results)->shape[ULAB_MAX_DIMS-1];
               (rarray) += (rstrides)[ULAB_MAX_DIMS - 2];
               k++;
           } while(k < (results)->shape[ULAB_MAX_DIMS - 2]);
           (larray) -= (lstrides)[ULAB_MAX_DIMS - 2] * results->shape[ULAB_MAX_DIMS-2];
           (larray) += (lstrides)[ULAB_MAX_DIMS - 3];
           (rarray) -= (rstrides)[ULAB_MAX_DIMS - 2] * results->shape[ULAB_MAX_DIMS-2];
           (rarray) += (rstrides)[ULAB_MAX_DIMS - 3];
           j++;
       } while(j < (results)->shape[ULAB_MAX_DIMS - 3]);

In order to reduce firmware size, it *might* make sense in higher
dimensions to make use of the function iterator by setting the

.. code:: c

   #define ULAB_HAS_FUNCTION_ITERATOR      (1)

constant to 1. This allows the compiler to call the
``ndarray_rewind_array`` function, so that it doesn’t have to unwrap the
loops for ``k``, and ``j``. Instead of the macro above, we now have

.. code:: c

   #define BINARY_LOOP(results, type_out, type_left, type_right, larray, lstrides, rarray, rstrides, OPERATOR)
       type_out *array = (type_out *)(results)->array;
       size_t *lcoords = ndarray_new_coords((results)->ndim);
       size_t *rcoords = ndarray_new_coords((results)->ndim);
       for(size_t i=0; i < (results)->len/(results)->shape[ULAB_MAX_DIMS -1]; i++) {
           size_t l = 0;
           do {
               *array++ = *((type_left *)(larray)) OPERATOR *((type_right *)(rarray));
               (larray) += (lstrides)[ULAB_MAX_DIMS - 1];
               (rarray) += (rstrides)[ULAB_MAX_DIMS - 1];
               l++;
           } while(l < (results)->shape[ULAB_MAX_DIMS - 1]);
           ndarray_rewind_array((results)->ndim, larray, (results)->shape, lstrides, lcoords);
           ndarray_rewind_array((results)->ndim, rarray, (results)->shape, rstrides, rcoords);
       } while(0)

Since the ``ndarray_rewind_array`` function is implemented only once, a
lot of space can be saved. Obviously, function calls cost time, thus
such trade-offs must be evaluated for each application. The gain also
depends on which functions and features you include. Operators and
functions that involve two arrays are expensive, because at the C level,
the number of cases that must be handled scales with the squares of the
number of data types. As an example, the innocent-looking expression

.. code:: python


   from ulab import numpy as np

   a = np.array([1, 2, 3])
   b = np.array([4, 5, 6])

   c = a + b

requires 25 loops in C, because the ``dtypes`` of both ``a``, and ``b``
can assume 5 different values, and the addition has to be resolved for
all possible cases. Hint: each binary operator costs between 3 and 4 kB
in two dimensions.

The ulab version string
-----------------------

As is customary with ``python`` packages, information on the package
version can be found be querying the ``__version__`` string.

.. code::
        
    # code to be run in micropython
    
    import ulab
    
    print('you are running ulab version', ulab.__version__)

.. parsed-literal::

    you are running ulab version 2.1.0-2D
    
    


The first three numbers indicate the major, minor, and sub-minor
versions of ``ulab`` (defined by the ``ULAB_VERSION`` constant in
`ulab.c <https://github.com/v923z/micropython-ulab/blob/master/code/ulab.c>`__).
We usually change the minor version, whenever a new function is added to
the code, and the sub-minor version will be incremented, if a bug fix is
implemented.

``2D`` tells us that the particular firmware supports tensors of rank 2
(defined by ``ULAB_MAX_DIMS`` in
`ulab.h <https://github.com/v923z/micropython-ulab/blob/master/code/ulab.h>`__).

If you find a bug, please, include the version string in your report!

Should you need the numerical value of ``ULAB_MAX_DIMS``, you can get it
from the version string in the following way:

.. code::
        
    # code to be run in micropython
    
    import ulab
    
    version = ulab.__version__
    version_dims = version.split('-')[1]
    version_num = int(version_dims.replace('D', ''))
    
    print('version string: ', version)
    print('version dimensions: ', version_dims)
    print('numerical value of dimensions: ', version_num)

.. parsed-literal::

    version string:  2.1.0-2D
    version dimensions:  2D
    numerical value of dimensions:  2
    
    


ulab with complex arrays
~~~~~~~~~~~~~~~~~~~~~~~~

If the firmware supports complex arrays, ``-c`` is appended to the
version string as can be seen below.

.. code::
        
    # code to be run in micropython
    
    import ulab
    
    version = ulab.__version__
    
    print('version string: ', version)

.. parsed-literal::

    version string:  4.0.0-2D-c
    
    


Finding out what your firmware supports
---------------------------------------

``ulab`` implements a number of array operators and functions, but this
does not mean that all of these functions and methods are actually
compiled into the firmware. You can fine-tune your firmware by
setting/unsetting any of the ``_HAS_`` constants in
`ulab.h <https://github.com/v923z/micropython-ulab/blob/master/code/ulab.h>`__.

Functions included in the firmware
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The version string will not tell you everything about your firmware,
because the supported functions and sub-modules can still arbitrarily be
included or excluded. One way of finding out what is compiled into the
firmware is calling ``dir`` with ``ulab`` as its argument.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    from ulab import scipy as spy
    
    
    print('===== constants, functions, and modules of numpy =====\n\n', dir(np))
    
    # since fft and linalg are sub-modules, print them separately
    print('\nfunctions included in the fft module:\n', dir(np.fft))
    print('\nfunctions included in the linalg module:\n', dir(np.linalg))
    
    print('\n\n===== modules of scipy =====\n\n', dir(spy))
    print('\nfunctions included in the optimize module:\n', dir(spy.optimize))
    print('\nfunctions included in the signal module:\n', dir(spy.signal))
    print('\nfunctions included in the special module:\n', dir(spy.special))

.. parsed-literal::

    ===== constants, functions, and modules of numpy =====
    
     ['__class__', '__name__', 'bool', 'sort', 'sum', 'acos', 'acosh', 'arange', 'arctan2', 'argmax', 'argmin', 'argsort', 'around', 'array', 'asin', 'asinh', 'atan', 'atanh', 'ceil', 'clip', 'concatenate', 'convolve', 'cos', 'cosh', 'cross', 'degrees', 'diag', 'diff', 'e', 'equal', 'exp', 'expm1', 'eye', 'fft', 'flip', 'float', 'floor', 'frombuffer', 'full', 'get_printoptions', 'inf', 'int16', 'int8', 'interp', 'linalg', 'linspace', 'log', 'log10', 'log2', 'logspace', 'max', 'maximum', 'mean', 'median', 'min', 'minimum', 'nan', 'ndinfo', 'not_equal', 'ones', 'pi', 'polyfit', 'polyval', 'radians', 'roll', 'set_printoptions', 'sin', 'sinh', 'sqrt', 'std', 'tan', 'tanh', 'trapz', 'uint16', 'uint8', 'vectorize', 'zeros']
    
    functions included in the fft module:
     ['__class__', '__name__', 'fft', 'ifft']
    
    functions included in the linalg module:
     ['__class__', '__name__', 'cholesky', 'det', 'dot', 'eig', 'inv', 'norm', 'trace']
    
    
    ===== modules of scipy =====
    
     ['__class__', '__name__', 'optimize', 'signal', 'special']
    
    functions included in the optimize module:
     ['__class__', '__name__', 'bisect', 'fmin', 'newton']
    
    functions included in the signal module:
     ['__class__', '__name__', 'sosfilt', 'spectrogram']
    
    functions included in the special module:
     ['__class__', '__name__', 'erf', 'erfc', 'gamma', 'gammaln']
    
    


Methods included in the firmware
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The ``dir`` function applied to the module or its sub-modules gives
information on what the module and sub-modules include, but is not
enough to find out which methods the ``ndarray`` class supports. We can
list the methods by calling ``dir`` with the ``array`` object itself:

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    print(dir(np.array))

.. parsed-literal::

    ['__class__', '__name__', 'copy', 'sort', '__bases__', '__dict__', 'dtype', 'flatten', 'itemsize', 'reshape', 'shape', 'size', 'strides', 'tobytes', 'transpose']
    
    


Operators included in the firmware
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A list of operators cannot be generated as shown above. If you really
need to find out, whether, e.g., the ``**`` operator is supported by the
firmware, you have to ``try`` it:

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([1, 2, 3])
    b = np.array([4, 5, 6])
    
    try:
        print(a ** b)
    except Exception as e:
        print('operator is not supported: ', e)

.. parsed-literal::

    operator is not supported:  unsupported types for __pow__: 'ndarray', 'ndarray'
    
    


The exception above would be raised, if the firmware was compiled with
the

.. code:: c

   #define NDARRAY_HAS_BINARY_OP_POWER         (0)

definition.
