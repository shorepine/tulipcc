Sun, 24 Nov 2024

version 6.6.1

    fix compilation error, for complexes

Wed, 9 Oct 2024

version 6.6.0

    add numpy.take

Sat, 14 Sep 2024

version 6.5.5

    add scratchpad, out, log keyword arguments to spectrum

Sat, 14 Sep 2024

version 6.5.4

    fix roll, when shift is 0

Wed, 6 Mar 2024

version 6.5.2

    allow loadtxt to parse numbers, even if built-in complexes are not supported

Tue, 9 Jan 2024

version 6.5.0

    add random module

Mon, 25 Dec 2023

version 6.4.3

    fix the 'np.delete' error that occurs when passing an empty iterable object as the second positional argument (#653)

Thu, 11 Dec 2023

version 6.4.2

    fix upcasting with two uint8 operands (#650)

Thu, 10 Aug 2023

version 6.4.1

  fix BOOLEAN issue, which would cause numpy.where funciton abnormally on RP2040(#643)

Thu, 20 Jul 2023

version 6.4.0

    implement AND, OR, and XOR binary operators

Sat, 1 Jul 2023

version 6.3.5

    allow function itertor in math functions with the out keyword

Fri, 12 May 2023

version 6.3.4

    fix compile error when COMPLEX support not enabled

version 6.3.3

    Polyval handles non-array as second argument (#601)

version 6.3.2

    fix out of bound read

version 6.3.1

    fix integer overflows

version 6.3.0

    add bitwise operators

Wed, 17 May 2023

version 6.1.1

    fix ndarray subscription, when value is NULL

Tue, 16 May 2023

version 6.1.0

    add sinc function

Fri, 12 May 2023

version 6.0.13

    add bitwise operators

Sun, 7 May 2023

version 6.0.12

    ndarray_from_mp_obj correctly treats Boolean arguments

Sat, 6 May 2023

version 6.0.11

    .reshape can now interpret unknown shape dimension

Sat, 6 May 2023

version 6.0.10

    fix binary division

Sun, 21 Jan 2023

version 6.0.6

    raise proper exception in arange

Sun, 21 Jan 2023

version 6.0.7

    treat empty arrays in sort_complex correctly

Sun, 21 Jan 2023

version 6.0.5

    fix ones()/zeros() method when the amount of memory to allocate overflows

Sun, 15 Jan 2023

version 6.0.4

    fix dot function

Sat, 14 Jan 2023

version 6.0.3

    fix how concatenate deals with scalar inputs

Tue, 3 Jan 2023

version 6.0.2

    fix vectorize

Sat, 5 Nov 2022

version 6.0.1

    fix fft.ifft

Wed, 21 Sep 2022

version 6.0.0

    bring ulab in line with the latest version of micropython

Thu, 4 Aug 2022

version 5.1.1

    fix how arctan2 treats scalars

Mon, 25 July 2022

version 5.1.0

    add nonzero

Mon, 16 May 2022

version 5.0.7

    fix in-place assignment from slices

Thu, 14 Apr 2022

version 5.0.6

    use m_new0 conditionally

Thu, 14 Apr 2022

version 5.0.5

    fix sorting on empty arrays

Fri, 18 Feb 2022

version 5.0.4

    fix the handling of empty arrays in binary_op

Thu, 10 Feb 2022

version 5.0.3

    fix complex slicing

Tue, 8 Feb 2022

version 5.0.2

    fix np.diag

Thu, 3 Feb 2022

version 5.0.1

    add optional ULAB_HASH string

Tue, 1 Feb 2022

version 5.0.0

    move scipy.signal.spectrogram to utils.spectrogram

Tue, 1 Feb 2022

version 4.4.2

    add skiprows keyword to loadtxt

Sat, 29 Jan 2022

version 4.4.1

    add dtype keyword to loadtxt

Thu, 27 Jan 2022

version 4.4.0

    implement numpy.savetxt, numpy.loadtxt

Tue, 15 Jan 2022

version 4.3.2

    fix rp2 port compilation

Wed, 19 Jan 2022

version 4.3.1

    fix signal.sosfilt

Wed, 19 Jan 2022

version 4.3.0

    implement numpy.save, numpy.load

Tue, 18 Jan 2022

version 4.2.1

    fix ndarray_copy_view for Boolean dtypes

Fri, 14 Jan 2022

version 4.2.0

    add numpy.size, asarray

Wed, 12 Jan 2022

    version 4.2.0
    
    implement numpy.save, numpy.load

Wed, 12 Jan 2022

version 4.1.1

    fix complex printout for long arrays

Wed, 12 Jan 2022

version 4.1.0

    add numpy.delete

Sat, 8 Jan 2022

version 4.0.0

    add complex support, .tolist() method, .imag, .real array properties, compress, conjugate, imag, real, sort_complex functions

Fri, 3 Dec 2021

version 3.3.8

    fix any/all function

Tue, 30 Nov 2021

version 3.3.7

    fix sum() for integer/Boolean types

Sat, 20 Nov 2021

version 3.3.6

    fix .shape for arrays of zero length (#454)

Sun, 07 Nov 2021

version 3.3.5

    fix cast in numpy/compare.c:compare_function()

Sat, 07 Aug 2021

version 3.3.4

    change default keyword value in linalg.qr

Fri, 23 Jul 2021

version 3.3.3

    fix compilation for one dimension

Thu, 22 Jul 2021

version 3.3.2

    fix compilation error on SAMD devices

Thu, 22 Jul 2021

version 3.3.1

    fix sum for 4D arrays

Thu, 22 Jul 2021

version 3.3.0

    add QR decomposition

Tue, 13 Jul 2021

version 3.2.0

    add flatiter/flat to ndarray methods

Tue, 22 Jun 2021

version 3.1.1

    fix float comparison in scipy/linalg.c

Sat, 19 Jun 2021

version 3.1.0

    ndarray.shape can now be assigned to

Thu, 17 Jun 2021

version 3.0.1

    add the .T ndarray property

Wed, 9 Jun 2021

version 3.0.0

    implement property getter/setter for micropython

Thu, 3 Jun 2021

version 2.9.0

    add empty as alias for zeros

Thu, 3 Jun 2021

version 2.8.8

    allow functions in approx to take iterables as argument

Thu, 3 Jun 2021

version 2.8.7

    simplify vectorised function code

Wed, 2 Jun 2021

version 2.8.6

    factor out array creation from iterables, so that generic iterables can be passed to numerical functions

Tue, 1 Jun 2021

version 2.8.5

    fix upcasting rules for ndarray + scalar

Mon, 31 May 2021

version 2.8.4

    initialise arange values via macro

Mon, 24 May 2021

version 2.8.3

    fix nan return value

Sat, 22 May 2021

version 2.8.2

    fix all/any/median for empty arrays

Tue, 18 May 2021

version 2.8.1

    fix array initialisation/print with empty iterables

Sun, 16 May 2021

version 2.8.0

    added cho_solve function in scipy.linalg module

Thu, 13 May 2021

version 2.7.1

    fix garbage collection problem

Wed, 5 May 2021

version 2.7.0

    added linalg module in scipy with solve_triangular function

Mon, 26 Apr 2021

version 2.6.2

    fix optimize zero condition

Sat, 23 Apr 2021

version 2.6.1

    fix implementation of math constants


Mon, 22 Mar 2021

version 2.6.0

    add where function

Mon, 8 Mar 2021

version 2.5.1

    fix linspace/logspace/arange for Boolean dtypes

Wed, 03 Mar 2021

version 2.5.0

    added utils sub-module with from_intbuffer function

Tue, 23 Feb 2021

version 2.4.5

    fix dot function

Sun, 21 Feb 2021

version 2.4.3

    re-introduce ndarray_get_buffer, and buffer protocol

Sun, 21 Feb 2021

version 2.4.2

    fix ndarray_is_dense, eye, ones, full, and zeros for Boolean type

Sat, 13 Feb 2021

version 2.4.1

    fixed dot error

Fri, 12 Feb 2021

version 2.4.0

    added byteswap method

Sun, 14 Feb 2021

version 2.3.7

    fixed frombuffer implementation glitch

Sat, 13 Feb 2021

version 2.3.6

    moved trace and dot to the top level

Wed, 10 Feb 2021

version 2.3.5

    fixed invisible error in tools_reduce_axes, simplified the implementation of all/any

Tue, 9 Feb 2021

version 2.3.4

    removed redundant exception from linalg.norm, fixed exception message in tools_reduce_axes

Tue, 9 Feb 2021

version 2.3.3

    linalg.norm should now work with the axis keyword argument

Mon, 8 Feb 2021

version 2.3.2

    improved the accuracy of linalg.norm, and extended it to generic iterables

Mon, 8 Feb 2021

version 2.3.1

    partially fix https://github.com/v923z/micropython-ulab/issues/304, and len unary operator

Mon, 8 Feb 2021

version 2.3.0

    added any and all functions

Fri, 29 Jan 2021

version 2.2.0

    added isinf/infinite functions

Fri, 29 Jan 2021

version 2.1.5

    fixed error, when calculating standard deviation of iterables

wed, 27 Jan 2021

version 2.1.4

    arrays can now be initialised from nested iterables

Thu, 21 Jan 2021

version 2.1.3

    added ifndef/endif wrappers in ulab.h

Fri, 15 Jan 2021

version 2.1.2

    fixed small error in frombuffer

Thu, 14 Jan 2021

version 2.1.1

    fixed bad error in diff

Thu, 26 Nov 2020

version 2.1.0

    implemented frombuffer

Tue, 24 Nov 2020

version 2.0.0

    implemented numpy/scipy compatibility

Tue, 24 Nov 2020

version 1.6.0

    added Boolean initialisation option

Mon, 23 Nov 2020

version 1.5.1

    fixed nan definition

version 1.5.0

    added nan/inf class level constants

version 1.4.10

    fixed sosfilt

version 1.4.9

    added in-place sort

version 1.4.8

    fixed convolve

version 1.4.7.

    fixed iteration loop in norm

Fri, 20 Nov 2020

version 1.4.6

    fixed interp

Thu, 19 Nov 2020

version 1.4.5

    eliminated fatal micropython error in ndarray_init_helper

version 1.4.4

    fixed min, max

version 1.4.3

    fixed full, zeros, ones

version 1.4.2

    fixed dtype

Wed, 18 Nov 2020

version 1.4.1.

    fixed std

version 1.4.0

    removed size from linalg

version 1.3.8

    fixed trapz

Tue, 17 Nov 2020

version 1.3.7

    fixed in-place power, in-place divide, roll

Mon, 16 Nov 2020

version 1.3.6

    fixed eye

Mon, 16 Nov 2020

version 1.3.5

    fixed trace

Mon, 16 Nov 2020

version 1.3.4

    fixed clip

Mon, 16 Nov 2020

version 1.3.3

    added function pointer option to some binary operators

Fri, 13 Nov 2020

version 1.3.2

    implemented function pointer option in vectorise

Thu, 12 Nov 2020

version 1.3.1

    factored out some of the math functions in re-usable form

Wed, 11 Nov 2020

version 1.3.0

    added dtype function/method/property

Wed, 11 Nov 2020

version 1.2.8

    improved the accuracy of sum for float types

Wed, 11 Nov 2020

version 1.2.7

    fixed transpose
    improved the accuracy of trapz

Tue, 10 Nov 2020

version 1.2.6

    fixed slicing

Mon, 9 Nov 2020

version 1.2.5

    fixed array casting glitch in make_new_core

Mon, 9 Nov 2020

version 1.2.4

    sum/mean/std can flatten the arrays now

Tue, 3 Nov 2020

version 1.2.1

    fixed pointer issue in eig, and corrected the docs

Tue, 3 Nov 2020

version 1.2.0

    added median function

Tue, 3 Nov 2020

version 1.1.4

    fixed norm and shape

Mon, 2 Nov 2020

version 1.1.3

    fixed small glitch in diagonal, and ndarray_make_new_core

Sun, 1 Nov 2020

version 1.1.1

    fixed compilation error for 4D

Sat, 31 Oct 2020

version 1.1.0

    added the diagonal function

Fri, 30 Oct 2020

version 1.0.0

    added :
        support for tensors of rank 4
        proper broadcasting
        views
        .tobytes()
        concatenate
        cross
        full
        logspace
        in-place operators

Sat, 25 Oct 2020

version 0.54.5

    wrong type in slices raise TypeError exception

Fri, 23 Oct 2020

version 0.54.4

    fixed indexing error in slices

Mon, 17 Aug 2020

version 0.54.3

    fixed small error in linalg

Mon, 03 Aug 2020

version 0.54.2

    argsort throws an error, if the array is longer than 65535

Wed, 29 Jul 2020

version 0.54.1

    changed to size_t for the length of arrays

Thu, 23 Jul 2020

version 0.54.0

    added norm to linalg

Wed, 22 Jul 2020

version 0.53.2

    added circuitpython documentation stubs to the source files

Wed, 22 Jul 2020

version 0.53.1

    fixed arange with negative steps

Mon, 20 Jul 2020

version 0.53.0

    added arange to create.c

Thu, 16 Jul 2020

version 0.52.0

    added trapz to approx

Mon, 29 Jun 2020

version 0.51.1

    fixed argmin/argmax issue

Fri, 19 Jun 2020

version 0.51.0

    add sosfilt to the filter sub-module

Fri, 12 Jun 2020

version 0.50.2

    fixes compilation error in openmv

Mon, 1 Jun 2020

version 0.50.1

    fixes error in numerical max/min

Mon, 18 May 2020

version 0.50.0

    move interp to the approx sub-module

Wed, 06 May 2020

version 0.46.0

    add curve_fit to the approx sub-module

version 0.44.0

    add approx sub-module with newton, fmin, and bisect functions

Thu, 30 Apr 2020

version 0.44.0

    add approx sub-module with newton, fmin, and bisect functions

Tue, 19 May 2020

version 0.46.1

    fixed bad error in binary_op

Wed, 6 May 2020

version 0.46

    added vectorisation of python functions

Sat, 2 May 2020

version 0.45.0

    add equal/not_equal to the compare module

Tue, 21 Apr 2020

version 0.42.0

    add minimum/maximum/clip functions

Mon, 20 Apr 2020

version 0.41.6

    argument handling improvement in polyfit

Mon, 20 Apr 2020

version 0.41.5

    fix compilation errors due to https://github.com/micropython/micropython/commit/30840ebc9925bb8ef025dbc2d5982b1bfeb75f1b

Sat, 18 Apr 2020

version 0.41.4

    fix compilation error on hardware ports

Tue, 14 Apr 2020

version 0.41.3

    fix indexing error in dot function

Thu, 9 Apr 2020

version 0.41.2

    fix transpose function

Tue, 7 Apr 2020

version 0.41.2

    fix discrepancy in argmin/argmax behaviour

Tue, 7 Apr 2020

version 0.41.1

    fix error in argsort

Sat, 4 Apr 2020

version 0.41.0

    implemented == and != binary operators

Fri, 3 Apr 2020

version 0.40.0

    added trace to linalg

Thu, 2 Apr 2020

version 0.39.0

    added the ** operator, and operand swapping in binary operators

Thu, 2 Apr 2020

version 0.38.1

    added fast option, when initialising from ndarray_properties

Thu, 12 Mar 2020

version 0.38.0

    added initialisation from ndarray, and the around function

Tue, 10 Mar 2020

version 0.37.0

    added Cholesky decomposition to linalg.c

Thu, 27 Feb 2020

version 0.36.0

    moved zeros, ones, eye and linspace into separate module (they are still bound at the top level)

Thu, 27 Feb 2020

version 0.35.0

    Move zeros, ones back into top level ulab module

Tue, 18 Feb 2020

version 0.34.0

    split ulab into multiple modules

Sun, 16 Feb 2020

version 0.33.2

    moved properties into ndarray_properties.h, implemented pointer arithmetic in fft.c to save some time

Fri, 14 Feb 2020

version 0.33.1

    added the __name__attribute to all sub-modules

Thu, 13 Feb 2020

version 0.33.0

    sub-modules are now proper sub-modules of ulab

Mon, 17 Feb 2020

version 0.32.1

    temporary fix for issue #40

Tue, 11 Feb 2020

version 0.32.0

    added itemsize, size and shape attributes to ndarrays, and removed rawsize

Mon, 10 Feb 2020

version 0.31.0

    removed asbytearray, and added buffer protocol to ndarrays, fixed bad error in filter.c

Sun, 09 Feb 2020

version 0.30.2

    fixed slice_length in ndarray.c

Sat, 08 Feb 2020

version 0.30.1

    fixed typecode error, added variable inspection, and replaced ternary operators in filter.c

Fri, 07 Feb 2020

version 0.30.0

    ulab functions can arbitrarily be excluded from the firmware via the ulab.h configuration file

Thu, 06 Feb 2020

version 0.27.0

    add convolve, the start of a 'filter' functionality group

Wed, 29 Jan 2020

version 0.26.7

    fixed indexing error in linalg.dot

Mon, 20 Jan 2020

version 0.26.6

    replaced MP_ROM_PTR(&mp_const_none_obj), so that module can be compiled for the nucleo board

Tue, 7 Jan 2020

version 0.26.5

    fixed glitch in numerical.c, numerical.h

Mon, 6 Jan 2020

version 0.26.4

    switched version constant to string

Tue, 31 Dec 2019

version 0.263

    changed declaration of ulab_ndarray_type to extern

Fri, 29 Nov 2019

version 0.262

    fixed error in macro in vectorise.h

Thu, 28 Nov 2019

version 0.261

    fixed bad indexing error in linalg.dot

Tue, 6 Nov 2019

version 0.26

    added in-place sorting (method of ndarray), and argsort

Mon, 4 Nov 2019

version 0.25

    added first implementation of sort, and fixed section on compiling the module in the manual

Thu, 31 Oct 2019

version 0.24

    added diff to numerical.c

Tue, 29 Oct 2019

version 0.23

    major revamp of subscription method

Sat, 19 Oct 2019

version 0.21

    fixed trivial bug in .rawsize()

Sat, 19 Oct 2019

version 0.22

    fixed small error in linalg_det, and implemented linalg_eig.


Thu, 17 Oct 2019

version 0.21

    implemented uniform interface for fft, and spectrum, and added ifft.

Wed, 16 Oct 2019

version 0.20

    Added flip function to numerical.c, and moved the size function to linalg. In addition,
    size is a function now, and not a method.

Tue, 15 Oct 2019

version 0.19

    fixed roll in numerical.c: it can now accept the axis=None keyword argument, added determinant to linalg.c

Mon, 14 Oct 2019

version 0.18

    fixed min/man function in numerical.c; it conforms to numpy behaviour

Fri, 11 Oct 2019

version 0.171

    found and fixed small bux in roll function

Fri, 11 Oct 2019

version 0.17

    universal function can now take arbitrary typecodes

Fri, 11 Oct 2019

version 0.161

    fixed bad error in iterator, and make_new_ndarray

Thu, 10 Oct 2019

varsion 0.16

    changed ndarray to array in ulab.c, so as to conform to numpy's notation
    extended subscr method to include slices (partially works)

Tue, 8 Oct 2019

version 0.15

    added inv, neg, pos, and abs unary operators to ndarray.c

Mon, 7 Oct 2019

version 0.14

    made the internal binary_op function tighter, and added keyword arguments to linspace

Sat, 4 Oct 2019

version 0.13

    added the <, <=, >, >= binary operators to ndarray

Fri, 4 Oct 2019

version 0.12

    added .flatten to ndarray, ones, zeros, and eye to linalg

Thu, 3 Oct 2019

version 0.11

    binary operators are now based on macros
