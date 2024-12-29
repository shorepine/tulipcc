
scipy.optimize
==============

Functions in the ``optimize`` module can be called by prepending them by
``scipy.optimize.``. The module defines the following three functions:

1. `scipy.optimize.bisect <#bisect>`__
2. `scipy.optimize.fmin <#fmin>`__
3. `scipy.optimize.newton <#newton>`__

Note that routines that work with user-defined functions still have to
call the underlying ``python`` code, and therefore, gains in speed are
not as significant as with other vectorised operations. As a rule of
thumb, a factor of two can be expected, when compared to an optimised
``python`` implementation.

bisect
------

``scipy``:
https://docs.scipy.org/doc/scipy/reference/generated/scipy.optimize.bisect.html

``bisect`` finds the root of a function of one variable using a simple
bisection routine. It takes three positional arguments, the function
itself, and two starting points. The function must have opposite signs
at the starting points. Returned is the position of the root.

Two keyword arguments, ``xtol``, and ``maxiter`` can be supplied to
control the accuracy, and the number of bisections, respectively.

.. code::
        
    # code to be run in micropython
    
    from ulab import scipy as spy
        
    def f(x):
        return x*x - 1
    
    print(spy.optimize.bisect(f, 0, 4))
    
    print('only 8 bisections: ',  spy.optimize.bisect(f, 0, 4, maxiter=8))
    
    print('with 0.1 accuracy: ',  spy.optimize.bisect(f, 0, 4, xtol=0.1))

.. parsed-literal::

    0.9999997615814209
    only 8 bisections:  0.984375
    with 0.1 accuracy:  0.9375
    
    


Performance
~~~~~~~~~~~

Since the ``bisect`` routine calls user-defined ``python`` functions,
the speed gain is only about a factor of two, if compared to a purely
``python`` implementation.

.. code::
        
    # code to be run in micropython
    
    from ulab import scipy as spy
    
    def f(x):
        return (x-1)*(x-1) - 2.0
    
    def bisect(f, a, b, xtol=2.4e-7, maxiter=100):
        if f(a) * f(b) > 0:
            raise ValueError
    
        rtb = a if f(a) < 0.0 else b
        dx = b - a if f(a) < 0.0 else a - b
        for i in range(maxiter):
            dx *= 0.5
            x_mid = rtb + dx
            mid_value = f(x_mid)
            if mid_value < 0:
                rtb = x_mid
            if abs(dx) < xtol:
                break
    
        return rtb
    
    @timeit
    def bisect_scipy(f, a, b):
        return spy.optimize.bisect(f, a, b)
    
    @timeit
    def bisect_timed(f, a, b):
        return bisect(f, a, b)
    
    print('bisect running in python')
    bisect_timed(f, 3, 2)
    
    print('bisect running in C')
    bisect_scipy(f, 3, 2)

.. parsed-literal::

    bisect running in python
    execution time:  1270  us
    bisect running in C
    execution time:  642  us
    


fmin
----

``scipy``:
https://docs.scipy.org/doc/scipy/reference/generated/scipy.optimize.fmin.html

The ``fmin`` function finds the position of the minimum of a
user-defined function by using the downhill simplex method. Requires two
positional arguments, the function, and the initial value. Three keyword
arguments, ``xatol``, ``fatol``, and ``maxiter`` stipulate conditions
for stopping.

.. code::
        
    # code to be run in micropython
    
    from ulab import scipy as spy
    
    def f(x):
        return (x-1)**2 - 1
    
    print(spy.optimize.fmin(f, 3.0))
    print(spy.optimize.fmin(f, 3.0, xatol=0.1))

.. parsed-literal::

    0.9996093749999952
    1.199999999999996
    
    


newton
------

``scipy``:https://docs.scipy.org/doc/scipy/reference/generated/scipy.optimize.newton.html

``newton`` finds a zero of a real, user-defined function using the
Newton-Raphson (or secant or Halley’s) method. The routine requires two
positional arguments, the function, and the initial value. Three keyword
arguments can be supplied to control the iteration. These are the
absolute and relative tolerances ``tol``, and ``rtol``, respectively,
and the number of iterations before stopping, ``maxiter``. The function
retuns a single scalar, the position of the root.

.. code::
        
    # code to be run in micropython
    
    from ulab import scipy as spy
        
    def f(x):
        return x*x*x - 2.0
    
    print(spy.optimize.newton(f, 3., tol=0.001, rtol=0.01))

.. parsed-literal::

    1.260135727246117
    
    

