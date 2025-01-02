
numpy.linalg
============

Functions in the ``linalg`` module can be called by prepending them by
``numpy.linalg.``. The module defines the following seven functions:

1. `numpy.linalg.cholesky <#cholesky>`__
2. `numpy.linalg.det <#det>`__
3. `numpy.linalg.eig <#eig>`__
4. `numpy.linalg.inv <#inv>`__
5. `numpy.linalg.norm <#norm>`__
6. `numpy.linalg.qr <#qr>`__

cholesky
--------

``numpy``:
https://docs.scipy.org/doc/numpy-1.17.0/reference/generated/numpy.linalg.cholesky.html

The function of the Cholesky decomposition takes a positive definite,
symmetric square matrix as its single argument, and returns the *square
root matrix* in the lower triangular form. If the input argument does
not fulfill the positivity or symmetry condition, a ``ValueError`` is
raised.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([[25, 15, -5], [15, 18,  0], [-5,  0, 11]])
    print('a: ', a)
    print('\n' + '='*20 + '\nCholesky decomposition\n', np.linalg.cholesky(a))

.. parsed-literal::

    a:  array([[25.0, 15.0, -5.0],
    	 [15.0, 18.0, 0.0],
    	 [-5.0, 0.0, 11.0]], dtype=float)
    
    ====================
    Cholesky decomposition
     array([[5.0, 0.0, 0.0],
    	 [3.0, 3.0, 0.0],
    	 [-1.0, 1.0, 3.0]], dtype=float)
    
    


det
---

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.linalg.det.html

The ``det`` function takes a square matrix as its single argument, and
calculates the determinant. The calculation is based on successive
elimination of the matrix elements, and the return value is a float,
even if the input array was of integer type.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([[1, 2], [3, 4]], dtype=np.uint8)
    print(np.linalg.det(a))

.. parsed-literal::

    -2.0
    


Benchmark
~~~~~~~~~

Since the routine for calculating the determinant is pretty much the
same as for finding the `inverse of a matrix <#inv>`__, the execution
times are similar:

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    @timeit
    def matrix_det(m):
        return np.linalg.inv(m)
    
    m = np.array([[1, 2, 3, 4, 5, 6, 7, 8], [0, 5, 6, 4, 5, 6, 4, 5], 
                  [0, 0, 9, 7, 8, 9, 7, 8], [0, 0, 0, 10, 11, 12, 11, 12], 
                 [0, 0, 0, 0, 4, 6, 7, 8], [0, 0, 0, 0, 0, 5, 6, 7], 
                 [0, 0, 0, 0, 0, 0, 7, 6], [0, 0, 0, 0, 0, 0, 0, 2]])
    
    matrix_det(m)

.. parsed-literal::

    execution time:  294  us
    


eig
---

``numpy``:
https://docs.scipy.org/doc/numpy/reference/generated/numpy.linalg.eig.html

The ``eig`` function calculates the eigenvalues and the eigenvectors of
a real, symmetric square matrix. If the matrix is not symmetric, a
``ValueError`` will be raised. The function takes a single argument, and
returns a tuple with the eigenvalues, and eigenvectors. With the help of
the eigenvectors, amongst other things, you can implement sophisticated
stabilisation routines for robots.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([[1, 2, 1, 4], [2, 5, 3, 5], [1, 3, 6, 1], [4, 5, 1, 7]], dtype=np.uint8)
    x, y = np.linalg.eig(a)
    print('eigenvectors of a:\n', y)
    print('\neigenvalues of a:\n', x)

.. parsed-literal::

    eigenvectors of a:
     array([[0.8151560042509081, -0.4499411232970823, -0.1644660242574522, 0.3256141906686505],
           [0.2211334179893007, 0.7846992598235538, 0.08372081379922657, 0.5730077734355189],
           [-0.1340114162071679, -0.3100776411558949, 0.8742786816656, 0.3486109343758527],
           [-0.5183258053659028, -0.292663481927148, -0.4489749870391468, 0.6664142156731531]], dtype=float)
    
    eigenvalues of a:
     array([-1.165288365404889, 0.8029365530314914, 5.585625756072663, 13.77672605630074], dtype=float)
    
    


The same matrix diagonalised with ``numpy`` yields:

.. code::

    # code to be run in CPython
    
    a = array([[1, 2, 1, 4], [2, 5, 3, 5], [1, 3, 6, 1], [4, 5, 1, 7]], dtype=np.uint8)
    x, y = eig(a)
    print('eigenvectors of a:\n', y)
    print('\neigenvalues of a:\n', x)

.. parsed-literal::

    eigenvectors of a:
     [[ 0.32561419  0.815156    0.44994112 -0.16446602]
     [ 0.57300777  0.22113342 -0.78469926  0.08372081]
     [ 0.34861093 -0.13401142  0.31007764  0.87427868]
     [ 0.66641421 -0.51832581  0.29266348 -0.44897499]]
    
    eigenvalues of a:
     [13.77672606 -1.16528837  0.80293655  5.58562576]


When comparing results, we should keep two things in mind:

1. the eigenvalues and eigenvectors are not necessarily sorted in the
   same way
2. an eigenvector can be multiplied by an arbitrary non-zero scalar, and
   it is still an eigenvector with the same eigenvalue. This is why all
   signs of the eigenvector belonging to 5.58, and 0.80 are flipped in
   ``ulab`` with respect to ``numpy``. This difference, however, is of
   absolutely no consequence.

Computation expenses
~~~~~~~~~~~~~~~~~~~~

Since the function is based on `Givens
rotations <https://en.wikipedia.org/wiki/Givens_rotation>`__ and runs
till convergence is achieved, or till the maximum number of allowed
rotations is exhausted, there is no universal estimate for the time
required to find the eigenvalues. However, an order of magnitude can, at
least, be guessed based on the measurement below:

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    @timeit
    def matrix_eig(a):
        return np.linalg.eig(a)
    
    a = np.array([[1, 2, 1, 4], [2, 5, 3, 5], [1, 3, 6, 1], [4, 5, 1, 7]], dtype=np.uint8)
    
    matrix_eig(a)

.. parsed-literal::

    execution time:  111  us
    


inv
---

``numpy``:
https://docs.scipy.org/doc/numpy-1.17.0/reference/generated/numpy.linalg.inv.html

A square matrix, provided that it is not singular, can be inverted by
calling the ``inv`` function that takes a single argument. The inversion
is based on successive elimination of elements in the lower left
triangle, and raises a ``ValueError`` exception, if the matrix turns out
to be singular (i.e., one of the diagonal entries is zero).

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    m = np.array([[1, 2, 3, 4], [4, 5, 6, 4], [7, 8.6, 9, 4], [3, 4, 5, 6]])
    
    print(np.linalg.inv(m))

.. parsed-literal::

    array([[-2.166666666666667, 1.500000000000001, -0.8333333333333337, 1.0],
           [1.666666666666667, -3.333333333333335, 1.666666666666668, -0.0],
           [0.1666666666666666, 2.166666666666668, -0.8333333333333337, -1.0],
           [-0.1666666666666667, -0.3333333333333333, 0.0, 0.5]], dtype=float64)
    
    


Computation expenses
~~~~~~~~~~~~~~~~~~~~

Note that the cost of inverting a matrix is approximately twice as many
floats (RAM), as the number of entries in the original matrix, and
approximately as many operations, as the number of entries. Here are a
couple of numbers:

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    @timeit
    def invert_matrix(m):
        return np.linalg.inv(m)
    
    m = np.array([[1, 2,], [4, 5]])
    print('2 by 2 matrix:')
    invert_matrix(m)
    
    m = np.array([[1, 2, 3, 4], [4, 5, 6, 4], [7, 8.6, 9, 4], [3, 4, 5, 6]])
    print('\n4 by 4 matrix:')
    invert_matrix(m)
    
    m = np.array([[1, 2, 3, 4, 5, 6, 7, 8], [0, 5, 6, 4, 5, 6, 4, 5], 
                  [0, 0, 9, 7, 8, 9, 7, 8], [0, 0, 0, 10, 11, 12, 11, 12], 
                 [0, 0, 0, 0, 4, 6, 7, 8], [0, 0, 0, 0, 0, 5, 6, 7], 
                 [0, 0, 0, 0, 0, 0, 7, 6], [0, 0, 0, 0, 0, 0, 0, 2]])
    print('\n8 by 8 matrix:')
    invert_matrix(m)

.. parsed-literal::

    2 by 2 matrix:
    execution time:  65  us
    
    4 by 4 matrix:
    execution time:  105  us
    
    8 by 8 matrix:
    execution time:  299  us
    


The above-mentioned scaling is not obeyed strictly. The reason for the
discrepancy is that the function call is still the same for all three
cases: the input must be inspected, the output array must be created,
and so on.

norm
----

``numpy``:
https://numpy.org/doc/stable/reference/generated/numpy.linalg.norm.html

The function takes a vector or matrix without options, and returns its
2-norm, i.e., the square root of the sum of the square of the elements.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    a = np.array([1, 2, 3, 4, 5])
    b = np.array([[1, 2, 3], [4, 5, 6], [7, 8, 9]])
    
    print('norm of a:', np.linalg.norm(a))
    print('norm of b:', np.linalg.norm(b))

.. parsed-literal::

    norm of a: 7.416198487095663
    norm of b: 16.88194301613414
    
    


qr
--

``numpy``:
https://numpy.org/doc/stable/reference/generated/numpy.linalg.qr.html

The function computes the QR decomposition of a matrix ``m`` of
dimensions ``(M, N)``, i.e., it returns two such matrices, ``q``\ ’, and
``r``, that ``m = qr``, where ``q`` is orthonormal, and ``r`` is upper
triangular. In addition to the input matrix, which is the first
positional argument, the function accepts the ``mode`` keyword argument
with a default value of ``reduced``. If ``mode`` is ``reduced``, ``q``,
and ``r`` are returned in the reduced representation. Otherwise, the
outputs will have dimensions ``(M, M)``, and ``(M, N)``, respectively.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    
    A = np.arange(6).reshape((3, 2))
    print('A: \n', A)
    
    print('complete decomposition')
    q, r = np.linalg.qr(A, mode='complete')
    print('q: \n', q)
    print()
    print('r: \n', r)
    
    print('\n\nreduced decomposition')
    q, r = np.linalg.qr(A, mode='reduced')
    print('q: \n', q)
    print()
    print('r: \n', r)

.. parsed-literal::

    A: 
     array([[0, 1],
           [2, 3],
           [4, 5]], dtype=int16)
    complete decomposition
    q: 
     array([[0.0, -0.9128709291752768, 0.408248290463863],
           [-0.447213595499958, -0.3651483716701107, -0.8164965809277261],
           [-0.8944271909999159, 0.1825741858350553, 0.408248290463863]], dtype=float64)
    
    r: 
     array([[-4.47213595499958, -5.813776741499454],
           [0.0, -1.095445115010332],
           [0.0, 0.0]], dtype=float64)
    
    
    reduced decomposition
    q: 
     array([[0.0, -0.9128709291752768],
           [-0.447213595499958, -0.3651483716701107],
           [-0.8944271909999159, 0.1825741858350553]], dtype=float64)
    
    r: 
     array([[-4.47213595499958, -5.813776741499454],
           [0.0, -1.095445115010332]], dtype=float64)
    
    

