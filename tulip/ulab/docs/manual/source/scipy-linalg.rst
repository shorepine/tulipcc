
scipy.linalg
============

``scipy``\ ’s ``linalg`` module contains two functions,
``solve_triangular``, and ``cho_solve``. The functions can be called by
prepending them by ``scipy.linalg.``.

1. `scipy.linalg.solve_cho <#cho_solve>`__
2. `scipy.linalg.solve_triangular <#solve_triangular>`__

cho_solve
---------

``scipy``:
https://docs.scipy.org/doc/scipy/reference/generated/scipy.linalg.cho_solve.html

Solve the linear equations

:raw-latex:`\begin{equation}
\mathbf{A}\cdot\mathbf{x} = \mathbf{b}
\end{equation}`

given the Cholesky factorization of :math:`\mathbf{A}`. As opposed to
``scipy``, the function simply takes the Cholesky-factorised matrix,
:math:`\mathbf{A}`, and :math:`\mathbf{b}` as inputs.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    from ulab import scipy as spy
    
    A = np.array([[3, 0, 0, 0], [2, 1, 0, 0], [1, 0, 1, 0], [1, 2, 1, 8]])
    b = np.array([4, 2, 4, 2])
    
    print(spy.linalg.cho_solve(A, b))

.. parsed-literal::

    array([-0.01388888888888906, -0.6458333333333331, 2.677083333333333, -0.01041666666666667], dtype=float64)
    
    


solve_triangular
----------------

``scipy``:
https://docs.scipy.org/doc/scipy/reference/generated/scipy.linalg.solve_triangular.html

Solve the linear equation

:raw-latex:`\begin{equation}
\mathbf{a}\cdot\mathbf{x} = \mathbf{b}
\end{equation}`

with the assumption that :math:`\mathbf{a}` is a triangular matrix. The
two position arguments are :math:`\mathbf{a}`, and :math:`\mathbf{b}`,
and the optional keyword argument is ``lower`` with a default value of
``False``. ``lower`` determines, whether data are taken from the lower,
or upper triangle of :math:`\mathbf{a}`.

Note that :math:`\mathbf{a}` itself does not have to be a triangular
matrix: if it is not, then the values are simply taken to be 0 in the
upper or lower triangle, as dictated by ``lower``. However,
:math:`\mathbf{a}\cdot\mathbf{x}` will yield :math:`\mathbf{b}` only,
when :math:`\mathbf{a}` is triangular. You should keep this in mind,
when trying to establish the validity of the solution by back
substitution.

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    from ulab import scipy as spy
    
    a = np.array([[3, 0, 0, 0], [2, 1, 0, 0], [1, 0, 1, 0], [1, 2, 1, 8]])
    b = np.array([4, 2, 4, 2])
    
    print('a:\n')
    print(a)
    print('\nb: ', b)
    
    x = spy.linalg.solve_triangular(a, b, lower=True)
    
    print('='*20)
    print('x: ', x)
    print('\ndot(a, x): ', np.dot(a, x))

.. parsed-literal::

    a:
    
    array([[3.0, 0.0, 0.0, 0.0],
           [2.0, 1.0, 0.0, 0.0],
           [1.0, 0.0, 1.0, 0.0],
           [1.0, 2.0, 1.0, 8.0]], dtype=float64)
    
    b:  array([4.0, 2.0, 4.0, 2.0], dtype=float64)
    ====================
    x:  array([1.333333333333333, -0.6666666666666665, 2.666666666666667, -0.08333333333333337], dtype=float64)
    
    dot(a, x):  array([4.0, 2.0, 4.0, 2.0], dtype=float64)
    
    


With get the same solution, :math:`\mathbf{x}`, with the following
matrix, but the dot product of :math:`\mathbf{a}`, and
:math:`\mathbf{x}` is no longer :math:`\mathbf{b}`:

.. code::
        
    # code to be run in micropython
    
    from ulab import numpy as np
    from ulab import scipy as spy
    
    a = np.array([[3, 2, 1, 0], [2, 1, 0, 1], [1, 0, 1, 4], [1, 2, 1, 8]])
    b = np.array([4, 2, 4, 2])
    
    print('a:\n')
    print(a)
    print('\nb: ', b)
    
    x = spy.linalg.solve_triangular(a, b, lower=True)
    
    print('='*20)
    print('x: ', x)
    print('\ndot(a, x): ', np.dot(a, x))

.. parsed-literal::

    a:
    
    array([[3.0, 2.0, 1.0, 0.0],
           [2.0, 1.0, 0.0, 1.0],
           [1.0, 0.0, 1.0, 4.0],
           [1.0, 2.0, 1.0, 8.0]], dtype=float64)
    
    b:  array([4.0, 2.0, 4.0, 2.0], dtype=float64)
    ====================
    x:  array([1.333333333333333, -0.6666666666666665, 2.666666666666667, -0.08333333333333337], dtype=float64)
    
    dot(a, x):  array([5.333333333333334, 1.916666666666666, 3.666666666666667, 2.0], dtype=float64)
    
    

