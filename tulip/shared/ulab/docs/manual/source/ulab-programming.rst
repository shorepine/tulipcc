
Programming ulab
================

Earlier we have seen, how ``ulab``\ ’s functions and methods can be
accessed in ``micropython``. This last section of the book explains, how
these functions are implemented. By the end of this chapter, not only
would you be able to extend ``ulab``, and write your own
``numpy``-compatible functions, but through a deeper understanding of
the inner workings of the functions, you would also be able to see what
the trade-offs are at the ``python`` level.

Code organisation
-----------------

As mentioned earlier, the ``python`` functions are organised into
sub-modules at the C level. The C sub-modules can be found in
``./ulab/code/``.

The ``ndarray`` object
----------------------

General comments
~~~~~~~~~~~~~~~~

``ndarrays`` are efficient containers of numerical data of the same type
(i.e., signed/unsigned chars, signed/unsigned integers or
``mp_float_t``\ s, which, depending on the platform, are either C
``float``\ s, or C ``double``\ s). Beyond storing the actual data in the
void pointer ``*array``, the type definition has eight additional
members (on top of the ``base`` type). Namely, the ``dtype``, which
tells us, how the bytes are to be interpreted. Moreover, the
``itemsize``, which stores the size of a single entry in the array,
``boolean``, an unsigned integer, which determines, whether the arrays
is to be treated as a set of Booleans, or as numerical data, ``ndim``,
the number of dimensions (``uint8_t``), ``len``, the length of the array
(the number of entries), the shape (``*size_t``), the strides
(``*int32_t``). The length is simply the product of the numbers in
``shape``.

The type definition is as follows:

.. code:: c

   typedef struct _ndarray_obj_t {
       mp_obj_base_t base;
       uint8_t dtype;
       uint8_t itemsize;
       uint8_t boolean;
       uint8_t ndim;
       size_t len;
       size_t shape[ULAB_MAX_DIMS];
       int32_t strides[ULAB_MAX_DIMS];
       void *array;
   } ndarray_obj_t;

Memory layout
~~~~~~~~~~~~~

The values of an ``ndarray`` are stored in a contiguous segment in the
RAM. The ``ndarray`` can be dense, meaning that all numbers in the
linear memory segment belong to a linar combination of coordinates, and
it can also be sparse, i.e., some elements of the linear storage space
will be skipped, when the elements of the tensor are traversed.

In the RAM, the position of the item
:math:`M(n_1, n_2, ..., n_{k-1}, n_k)` in a dense tensor of rank
:math:`k` is given by the linear combination

:raw-latex:`\begin{equation}
P(n_1, n_2, ..., n_{k-1}, n_k) = n_1 s_1 + n_2 s_2 + ... + n_{k-1}s_{k-1} + n_ks_k = \sum_{i=1}^{k}n_is_i
\end{equation}` where :math:`s_i` are the strides of the tensor, defined
as

:raw-latex:`\begin{equation}
s_i = \prod_{j=i+1}^k l_j
\end{equation}`

where :math:`l_j` is length of the tensor along the :math:`j`\ th axis.
When the tensor is sparse (e.g., when the tensor is sliced), the strides
along a particular axis will be multiplied by a non-zero integer. If
this integer is different to :math:`\pm 1`, the linear combination above
cannot access all elements in the RAM, i.e., some numbers will be
skipped. Note that :math:`|s_1| > |s_2| > ... > |s_{k-1}| > |s_k|`, even
if the tensor is sparse. The statement is trivial for dense tensors, and
it follows from the definition of :math:`s_i`. For sparse tensors, a
slice cannot have a step larger than the shape along that axis. But for
dense tensors, :math:`s_i/s_{i+1} = l_i`.

When creating a *view*, we simply re-calculate the ``strides``, and
re-set the ``*array`` pointer.

Iterating over elements of a tensor
-----------------------------------

The ``shape`` and ``strides`` members of the array tell us how we have
to move our pointer, when we want to read out the numbers. For technical
reasons that will become clear later, the numbers in ``shape`` and in
``strides`` are aligned to the right, and begin on the right hand side,
i.e., if the number of possible dimensions is ``ULAB_MAX_DIMS``, then
``shape[ULAB_MAX_DIMS-1]`` is the length of the last axis,
``shape[ULAB_MAX_DIMS-2]`` is the length of the last but one axis, and
so on. If the number of actual dimensions, ``ndim < ULAB_MAX_DIMS``, the
first ``ULAB_MAX_DIMS - ndim`` entries in ``shape`` and ``strides`` will
be equal to zero, but they could, in fact, be assigned any value,
because these will never be accessed in an operation.

With this definition of the strides, the linear combination in
:math:`P(n_1, n_2, ..., n_{k-1}, n_k)` is a one-to-one mapping from the
space of tensor coordinates, :math:`(n_1, n_2, ..., n_{k-1}, n_k)`, and
the coordinate in the linear array,
:math:`n_1s_1 + n_2s_2 + ... + n_{k-1}s_{k-1} + n_ks_k`, i.e., no two
distinct sets of coordinates will result in the same position in the
linear array.

Since the ``strides`` are given in terms of bytes, when we iterate over
an array, the void data pointer is usually cast to ``uint8_t``, and the
values are converted using the proper data type stored in
``ndarray->dtype``. However, there might be cases, when it makes perfect
sense to cast ``*array`` to a different type, in which case the
``strides`` have to be re-scaled by the value of ``ndarray->itemsize``.

Iterating using the unwrapped loops
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The following macro definition is taken from
`vector.h <https://github.com/v923z/micropython-ulab/blob/master/code/numpy/vector/vector.h>`__,
and demonstrates, how we can iterate over a single array in four
dimensions.

.. code:: c

   #define ITERATE_VECTOR(type, array, source, sarray) do {
       size_t i=0;
       do {
           size_t j = 0;
           do {
               size_t k = 0;
               do {
                   size_t l = 0;
                   do {
                       *(array)++ = f(*((type *)(sarray)));
                       (sarray) += (source)->strides[ULAB_MAX_DIMS - 1];
                       l++;
                   } while(l < (source)->shape[ULAB_MAX_DIMS-1]);
                   (sarray) -= (source)->strides[ULAB_MAX_DIMS - 1] * (source)->shape[ULAB_MAX_DIMS-1];
                   (sarray) += (source)->strides[ULAB_MAX_DIMS - 2];
                   k++;
               } while(k < (source)->shape[ULAB_MAX_DIMS-2]);
               (sarray) -= (source)->strides[ULAB_MAX_DIMS - 2] * (source)->shape[ULAB_MAX_DIMS-2];
               (sarray) += (source)->strides[ULAB_MAX_DIMS - 3];
               j++;
           } while(j < (source)->shape[ULAB_MAX_DIMS-3]);
           (sarray) -= (source)->strides[ULAB_MAX_DIMS - 3] * (source)->shape[ULAB_MAX_DIMS-3];
           (sarray) += (source)->strides[ULAB_MAX_DIMS - 4];
           i++;
       } while(i < (source)->shape[ULAB_MAX_DIMS-4]);
   } while(0)

We start with the innermost loop, the one recursing ``l``. ``array`` is
already of type ``mp_float_t``, while the source array, ``sarray``, has
been cast to ``uint8_t`` in the calling function. The numbers contained
in ``sarray`` have to be read out in the proper type dictated by
``ndarray->dtype``. This is what happens in the statement
``*((type *)(sarray))``, and this number is then fed into the function
``f``. Vectorised mathematical functions produce *dense* arrays, and for
this reason, we can simply advance the ``array`` pointer.

The advancing of the ``sarray`` pointer is a bit more involving: first,
in the innermost loop, we simply move forward by the amount given by the
last stride, which is ``(source)->strides[ULAB_MAX_DIMS - 1]``, because
the ``shape`` and the ``strides`` are aligned to the right. We move the
pointer as many times as given by ``(source)->shape[ULAB_MAX_DIMS-1]``,
which is the length of the very last axis. Hence the the structure of
the loop

.. code:: c

       size_t l = 0;
       do {
           ...
           l++;
       } while(l < (source)->shape[ULAB_MAX_DIMS-1]);

Once we have exhausted the last axis, we have to re-wind the pointer,
and advance it by an amount given by the last but one stride. Keep in
mind that in the the innermost loop we moved our pointer
``(source)->shape[ULAB_MAX_DIMS-1]`` times by
``(source)->strides[ULAB_MAX_DIMS - 1]``, i.e., we re-wind it by moving
it backwards by
``(source)->strides[ULAB_MAX_DIMS - 1] * (source)->shape[ULAB_MAX_DIMS-1]``.
In the next step, we move forward by
``(source)->strides[ULAB_MAX_DIMS - 2]``, which is the last but one
stride.

.. code:: c

       (sarray) -= (source)->strides[ULAB_MAX_DIMS - 1] * (source)->shape[ULAB_MAX_DIMS-1];
       (sarray) += (source)->strides[ULAB_MAX_DIMS - 2];

This pattern must be repeated for each axis of the array, and this is
how we arrive at the four nested loops listed above.

Re-winding arrays by means of a function
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In addition to un-wrapping the iteration loops by means of macros, there
is another way of traversing all elements of a tensor: we note that,
since :math:`|s_1| > |s_2| > ... > |s_{k-1}| > |s_k|`,
:math:`P(n1, n2, ..., n_{k-1}, n_k)` changes most slowly in the last
coordinate. Hence, if we start from the very beginning, (:math:`n_i = 0`
for all :math:`i`), and walk along the linear RAM segment, we increment
the value of :math:`n_k` as long as :math:`n_k < l_k`. Once
:math:`n_k = l_k`, we have to reset :math:`n_k` to 0, and increment
:math:`n_{k-1}` by one. After each such round, :math:`n_{k-1}` will be
incremented by one, as long as :math:`n_{k-1} < l_{k-1}`. Once
:math:`n_{k-1} = l_{k-1}`, we reset both :math:`n_k`, and
:math:`n_{k-1}` to 0, and increment :math:`n_{k-2}` by one.

Rewinding the arrays in this way is implemented in the function
``ndarray_rewind_array`` in
`ndarray.c <https://github.com/v923z/micropython-ulab/blob/master/code/ndarray.c>`__.

.. code:: c

   void ndarray_rewind_array(uint8_t ndim, uint8_t *array, size_t *shape, int32_t *strides, size_t *coords) {
       // resets the data pointer of a single array, whenever an axis is full
       // since we always iterate over the very last axis, we have to keep track of
       // the last ndim-2 axes only
       array -= shape[ULAB_MAX_DIMS - 1] * strides[ULAB_MAX_DIMS - 1];
       array += strides[ULAB_MAX_DIMS - 2];
       for(uint8_t i=1; i < ndim-1; i++) {
           coords[ULAB_MAX_DIMS - 1 - i] += 1;
           if(coords[ULAB_MAX_DIMS - 1 - i] == shape[ULAB_MAX_DIMS - 1 - i]) { // we are at a dimension boundary
               array -= shape[ULAB_MAX_DIMS - 1 - i] * strides[ULAB_MAX_DIMS - 1 - i];
               array += strides[ULAB_MAX_DIMS - 2 - i];
               coords[ULAB_MAX_DIMS - 1 - i] = 0;
               coords[ULAB_MAX_DIMS - 2 - i] += 1;
           } else { // coordinates can change only, if the last coordinate changes
               return;
           }
       }
   }

and the function would be called as in the snippet below. Note that the
innermost loop is factored out, so that we can save the ``if(...)``
statement for the last axis.

.. code:: c

       size_t *coords = ndarray_new_coords(results->ndim);
       for(size_t i=0; i < results->len/results->shape[ULAB_MAX_DIMS -1]; i++) {
           size_t l = 0;
           do {
               ...
               l++;
           } while(l < results->shape[ULAB_MAX_DIMS - 1]);
           ndarray_rewind_array(results->ndim, array, results->shape, strides, coords);
       } while(0)

The advantage of this method is that the implementation is independent
of the number of dimensions: the iteration requires more or less the
same flash space for 2 dimensions as for 22. However, the price we have
to pay for this convenience is the extra function call.

Iterating over two ndarrays simultaneously: broadcasting
--------------------------------------------------------

Whenever we invoke a binary operator, call a function with two arguments
of ``ndarray`` type, or assign something to an ``ndarray``, we have to
iterate over two views at the same time. The task is trivial, if the two
``ndarray``\ s in question have the same shape (but not necessarily the
same set of strides), because in this case, we can still iterate in the
same loop. All that happens is that we move two data pointers in sync.

The problem becomes a bit more involving, when the shapes of the two
``ndarray``\ s are not identical. For such cases, ``numpy`` defines
so-called broadcasting, which boils down to two rules.

1. The shapes in the tensor with lower rank has to be prepended with
   axes of size 1 till the two ranks become equal.
2. Along all axes the two tensors should have the same size, or one of
   the sizes must be 1.

If, after applying the first rule the second is not satisfied, the two
``ndarray``\ s cannot be broadcast together.

Now, let us suppose that we have two compatible ``ndarray``\ s, i.e.,
after applying the first rule, the second is satisfied. How do we
iterate over the elements in the tensors?

We should recall, what exactly we do, when iterating over a single
array: normally, we move the data pointer by the last stride, except,
when we arrive at a dimension boundary (when the last axis is
exhausted). At that point, we move the pointer by an amount dictated by
the strides. And this is the key: *dictated by the strides*. Now, if we
have two arrays that are originally not compatible, we define new
strides for them, and use these in the iteration. With that, we are back
to the case, where we had two compatible arrays.

Now, let us look at the second broadcasting rule: if the two arrays have
the same size, we take both ``ndarray``\ s’ strides along that axis. If,
on the other hand, one of the ``ndarray``\ s is of length 1 along one of
its axes, we set the corresponding strides to 0. This will ensure that
that data pointer is not moved, when we iterate over both ``ndarray``\ s
at the same time.

Thus, in order to implement broadcasting, we first have to check,
whether the two above-mentioned rules can be satisfied, and if so, we
have to find the two new sets strides.

The ``ndarray_can_broadcast`` function from
`ndarray.c <https://github.com/v923z/micropython-ulab/blob/master/code/ndarray.c>`__
takes two ``ndarray``\ s, and returns ``true``, if the two arrays can be
broadcast together. At the same time, it also calculates new strides for
the two arrays, so that they can be iterated over at the same time.

.. code:: c

   bool ndarray_can_broadcast(ndarray_obj_t *lhs, ndarray_obj_t *rhs, uint8_t *ndim, size_t *shape, int32_t *lstrides, int32_t *rstrides) {
       // returns True or False, depending on, whether the two arrays can be broadcast together
       // numpy's broadcasting rules are as follows:
       //
       // 1. the two shapes are either equal
       // 2. one of the shapes is 1
       memset(lstrides, 0, sizeof(size_t)*ULAB_MAX_DIMS);
       memset(rstrides, 0, sizeof(size_t)*ULAB_MAX_DIMS);
       lstrides[ULAB_MAX_DIMS - 1] = lhs->strides[ULAB_MAX_DIMS - 1];
       rstrides[ULAB_MAX_DIMS - 1] = rhs->strides[ULAB_MAX_DIMS - 1];
       for(uint8_t i=ULAB_MAX_DIMS; i > 0; i--) {
           if((lhs->shape[i-1] == rhs->shape[i-1]) || (lhs->shape[i-1] == 0) || (lhs->shape[i-1] == 1) ||
           (rhs->shape[i-1] == 0) || (rhs->shape[i-1] == 1)) {
               shape[i-1] = MAX(lhs->shape[i-1], rhs->shape[i-1]);
               if(shape[i-1] > 0) (*ndim)++;
               if(lhs->shape[i-1] < 2) {
                   lstrides[i-1] = 0;
               } else {
                   lstrides[i-1] = lhs->strides[i-1];
               }
               if(rhs->shape[i-1] < 2) {
                   rstrides[i-1] = 0;
               } else {
                   rstrides[i-1] = rhs->strides[i-1];
               }
           } else {
               return false;
           }
       }
       return true;
   }

A good example of how the function would be called can be found in
`vector.c <https://github.com/v923z/micropython-ulab/blob/master/code/numpy/vector/vector.c>`__,
in the ``vector_arctan2`` function:

.. code:: c

   mp_obj_t vector_arctan2(mp_obj_t y, mp_obj_t x) {
       ...
       uint8_t ndim = 0;
       size_t *shape = m_new(size_t, ULAB_MAX_DIMS);
       int32_t *xstrides = m_new(int32_t, ULAB_MAX_DIMS);
       int32_t *ystrides = m_new(int32_t, ULAB_MAX_DIMS);
       if(!ndarray_can_broadcast(ndarray_x, ndarray_y, &ndim, shape, xstrides, ystrides)) {
           mp_raise_ValueError(translate("operands could not be broadcast together"));
           m_del(size_t, shape, ULAB_MAX_DIMS);
           m_del(int32_t, xstrides, ULAB_MAX_DIMS);
           m_del(int32_t, ystrides, ULAB_MAX_DIMS);
       }

       uint8_t *xarray = (uint8_t *)ndarray_x->array;
       uint8_t *yarray = (uint8_t *)ndarray_y->array;
       
       ndarray_obj_t *results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_FLOAT);
       mp_float_t *rarray = (mp_float_t *)results->array;
       ...

After the new strides have been calculated, the iteration loop is
identical to what we discussed in the previous section.

Contracting an ``ndarray``
--------------------------

There are many operations that reduce the number of dimensions of an
``ndarray`` by 1, i.e., that remove an axis from the tensor. The drill
is the same as before, with the exception that first we have to remove
the ``strides`` and ``shape`` that corresponds to the axis along which
we intend to contract. The ``numerical_reduce_axes`` function from
`numerical.c <https://github.com/v923z/micropython-ulab/blob/master/code/numerical/numerical.c>`__
does that.

.. code:: c

   static void numerical_reduce_axes(ndarray_obj_t *ndarray, int8_t axis, size_t *shape, int32_t *strides) {
       // removes the values corresponding to a single axis from the shape and strides array
       uint8_t index = ULAB_MAX_DIMS - ndarray->ndim + axis;
       if((ndarray->ndim == 1) && (axis == 0)) {
           index = 0;
           shape[ULAB_MAX_DIMS - 1] = 0;
           return;
       }
       for(uint8_t i = ULAB_MAX_DIMS - 1; i > 0; i--) {
           if(i > index) {
               shape[i] = ndarray->shape[i];
               strides[i] = ndarray->strides[i];
           } else {
               shape[i] = ndarray->shape[i-1];
               strides[i] = ndarray->strides[i-1];
           }
       }
   }

Once the reduced ``strides`` and ``shape`` are known, we place the axis
in question in the innermost loop, and wrap it with the loops, whose
coordinates are in the ``strides``, and ``shape`` arrays. The
``RUN_STD`` macro from
`numerical.h <https://github.com/v923z/micropython-ulab/blob/master/code/numpy/numerical/numerical.h>`__
is a good example. The macro is expanded in the
``numerical_sum_mean_std_ndarray`` function.

.. code:: c

   static mp_obj_t numerical_sum_mean_std_ndarray(ndarray_obj_t *ndarray, mp_obj_t axis, uint8_t optype, size_t ddof) {
       uint8_t *array = (uint8_t *)ndarray->array;
       size_t *shape = m_new(size_t, ULAB_MAX_DIMS);
       memset(shape, 0, sizeof(size_t)*ULAB_MAX_DIMS);
       int32_t *strides = m_new(int32_t, ULAB_MAX_DIMS);
       memset(strides, 0, sizeof(uint32_t)*ULAB_MAX_DIMS);

       int8_t ax = mp_obj_get_int(axis);
       if(ax < 0) ax += ndarray->ndim;
       if((ax < 0) || (ax > ndarray->ndim - 1)) {
           mp_raise_ValueError(translate("index out of range"));
       }
       numerical_reduce_axes(ndarray, ax, shape, strides);
       uint8_t index = ULAB_MAX_DIMS - ndarray->ndim + ax;
       ndarray_obj_t *results = NULL;
       uint8_t *rarray = NULL;
       ...

Here is the macro for the three-dimensional case:

.. code:: c

   #define RUN_STD(ndarray, type, array, results, r, shape, strides, index, div) do {
       size_t k = 0;
       do {
           size_t l = 0;
           do {
               RUN_STD1((ndarray), type, (array), (results), (r), (index), (div));
               (array) -= (ndarray)->strides[(index)] * (ndarray)->shape[(index)];
               (array) += (strides)[ULAB_MAX_DIMS - 1];
               l++;
           } while(l < (shape)[ULAB_MAX_DIMS - 1]);
           (array) -= (strides)[ULAB_MAX_DIMS - 2] * (shape)[ULAB_MAX_DIMS-2];
           (array) += (strides)[ULAB_MAX_DIMS - 3];
           k++;
       } while(k < (shape)[ULAB_MAX_DIMS - 2]);
   } while(0)

In ``RUN_STD``, we simply move our pointers; the calculation itself
happens in the ``RUN_STD1`` macro below. (Note that this is the
implementation of the numerically stable Welford algorithm.)

.. code:: c

   #define RUN_STD1(ndarray, type, array, results, r, index, div)
   ({
       mp_float_t M, m, S = 0.0, s = 0.0;
       M = m = *(mp_float_t *)((type *)(array));
       for(size_t i=1; i < (ndarray)->shape[(index)]; i++) {
           (array) += (ndarray)->strides[(index)];
           mp_float_t value = *(mp_float_t *)((type *)(array));
           m = M + (value - M) / (mp_float_t)i;
           s = S + (value - M) * (value - m);
           M = m;
           S = s;
       }
       (array) += (ndarray)->strides[(index)];
       *(r)++ = MICROPY_FLOAT_C_FUN(sqrt)((ndarray)->shape[(index)] * s / (div));
   })

Upcasting
---------

When in an operation the ``dtype``\ s of two arrays are different, the
result’s ``dtype`` will be decided by the following upcasting rules:

1. Operations with two ``ndarray``\ s of the same ``dtype`` preserve
   their ``dtype``, even when the results overflow.

2. if either of the operands is a float, the result automatically
   becomes a float

3. otherwise

   -  ``uint8`` + ``int8`` => ``int16``,

   -  ``uint8`` + ``int16`` => ``int16``

   -  ``uint8`` + ``uint16`` => ``uint16``

   -  ``int8`` + ``int16`` => ``int16``

   -  ``int8`` + ``uint16`` => ``uint16`` (in numpy, the result is a
      ``int32``)

   -  ``uint16`` + ``int16`` => ``float`` (in numpy, the result is a
      ``int32``)

4. When one operand of a binary operation is a generic scalar
   ``micropython`` variable, i.e., ``mp_obj_int``, or ``mp_obj_float``,
   it will be converted to a linear array of length 1, and with the
   smallest ``dtype`` that can accommodate the variable in question.
   After that the broadcasting rules apply, as described in the section
   `Iterating over two ndarrays simultaneously:
   broadcasting <#Iterating_over_two_ndarrays_simultaneously:_broadcasting>`__

Upcasting is resolved in place, wherever it is required. Notable
examples can be found in
`ndarray_operators.c <https://github.com/v923z/micropython-ulab/blob/master/code/ndarray_operators.c>`__

Slicing and indexing
--------------------

An ``ndarray`` can be indexed with three types of objects: integer
scalars, slices, and another ``ndarray``, whose elements are either
integer scalars, or Booleans. Since slice and integer indices can be
thought of as modifications of the ``strides``, these indices return a
view of the ``ndarray``. This statement does not hold for ``ndarray``
indices, and therefore, the return a copy of the array.

Extending ulab
--------------

The ``user`` module is disabled by default, as can be seen from the last
couple of lines of
`ulab.h <https://github.com/v923z/micropython-ulab/blob/master/code/ulab.h>`__

.. code:: c

   // user-defined module
   #ifndef ULAB_USER_MODULE
   #define ULAB_USER_MODULE                (0)
   #endif

The module contains a very simple function, ``user_dummy``, and this
function is bound to the module itself. In other words, even if the
module is enabled, one has to ``import``:

.. code:: python


   import ulab
   from ulab import user

   user.dummy_function(2.5)

which should just return 5.0. Even if ``numpy``-compatibility is
required (i.e., if most functions are bound at the top level to ``ulab``
directly), having to ``import`` the module has a great advantage.
Namely, only the
`user.h <https://github.com/v923z/micropython-ulab/blob/master/code/user/user.h>`__
and
`user.c <https://github.com/v923z/micropython-ulab/blob/master/code/user/user.c>`__
files have to be modified, thus it should be relatively straightforward
to update your local copy from
`github <https://github.com/v923z/micropython-ulab/blob/master/>`__.

Now, let us see, how we can add a more meaningful function.

Creating a new ndarray
----------------------

In the `General comments <#General_comments>`__ sections we have seen
the type definition of an ``ndarray``. This structure can be generated
by means of a couple of functions listed in
`ndarray.c <https://github.com/v923z/micropython-ulab/blob/master/code/ndarray.c>`__.

ndarray_new_ndarray
~~~~~~~~~~~~~~~~~~~

The ``ndarray_new_ndarray`` functions is called by all other
array-generating functions. It takes the number of dimensions, ``ndim``,
a ``uint8_t``, the ``shape``, a pointer to ``size_t``, the ``strides``,
a pointer to ``int32_t``, and ``dtype``, another ``uint8_t`` as its
arguments, and returns a new array with all entries initialised to 0.

Assuming that ``ULAB_MAX_DIMS > 2``, a new dense array of dimension 3,
of ``shape`` (3, 4, 5), of ``strides`` (1000, 200, 10), and ``dtype``
``uint16_t`` can be generated by the following instructions

.. code:: c

   size_t *shape = m_new(size_t, ULAB_MAX_DIMS);
   shape[ULAB_MAX_DIMS - 1] = 5;
   shape[ULAB_MAX_DIMS - 2] = 4;
   shape[ULAB_MAX_DIMS - 3] = 3;

   int32_t *strides = m_new(int32_t, ULAB_MAX_DIMS);
   strides[ULAB_MAX_DIMS - 1] = 10;
   strides[ULAB_MAX_DIMS - 2] = 200;
   strides[ULAB_MAX_DIMS - 3] = 1000;

   ndarray_obj_t *new_ndarray = ndarray_new_ndarray(3, shape, strides, NDARRAY_UINT16);

ndarray_new_dense_ndarray
~~~~~~~~~~~~~~~~~~~~~~~~~

The functions simply calculates the ``strides`` from the ``shape``, and
calls ``ndarray_new_ndarray``. Assuming that ``ULAB_MAX_DIMS > 2``, a
new dense array of dimension 3, of ``shape`` (3, 4, 5), and ``dtype``
``mp_float_t`` can be generated by the following instructions

.. code:: c

   size_t *shape = m_new(size_t, ULAB_MAX_DIMS);
   shape[ULAB_MAX_DIMS - 1] = 5;
   shape[ULAB_MAX_DIMS - 2] = 4;
   shape[ULAB_MAX_DIMS - 3] = 3;

   ndarray_obj_t *new_ndarray = ndarray_new_dense_ndarray(3, shape, NDARRAY_FLOAT);

ndarray_new_linear_array
~~~~~~~~~~~~~~~~~~~~~~~~

Since the dimensions of a linear array are known (1), the
``ndarray_new_linear_array`` takes the ``length``, a ``size_t``, and the
``dtype``, an ``uint8_t``. Internally, ``ndarray_new_linear_array``
generates the ``shape`` array, and calls ``ndarray_new_dense_array``
with ``ndim = 1``.

A linear array of length 100, and ``dtype`` ``uint8`` could be created
by the function call

.. code:: c

   ndarray_obj_t *new_ndarray = ndarray_new_linear_array(100, NDARRAY_UINT8)

ndarray_new_ndarray_from_tuple
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This function takes a ``tuple``, which should hold the lengths of the
axes (in other words, the ``shape``), and the ``dtype``, and calls
internally ``ndarray_new_dense_array``. A new ``ndarray`` can be
generated by calling

.. code:: c

   ndarray_obj_t *new_ndarray = ndarray_new_ndarray_from_tuple(shape, NDARRAY_FLOAT);

where ``shape`` is a tuple.

ndarray_new_view
~~~~~~~~~~~~~~~~

This function crates a *view*, and takes the source, an ``ndarray``, the
number of dimensions, an ``uint8_t``, the ``shape``, a pointer to
``size_t``, the ``strides``, a pointer to ``int32_t``, and the offset,
an ``int32_t`` as arguments. The offset is the number of bytes by which
the void ``array`` pointer is shifted. E.g., the ``python`` statement

.. code:: python

   a = np.array([0, 1, 2, 3, 4, 5], dtype=uint8)
   b = a[1::2]

produces the array

.. code:: python

   array([1, 3, 5], dtype=uint8)

which holds its data at position ``x0 + 1``, if ``a``\ ’s pointer is at
``x0``. In this particular case, the offset is 1.

The array ``b`` from the example above could be generated as

.. code:: c

   size_t *shape = m_new(size_t, ULAB_MAX_DIMS);
   shape[ULAB_MAX_DIMS - 1] = 3;

   int32_t *strides = m_new(int32_t, ULAB_MAX_DIMS);
   strides[ULAB_MAX_DIMS - 1] = 2;

   int32_t offset = 1;
   uint8_t ndim = 1;

   ndarray_obj_t *new_ndarray = ndarray_new_view(ndarray_a, ndim, shape, strides, offset);

ndarray_copy_array
~~~~~~~~~~~~~~~~~~

The ``ndarray_copy_array`` function can be used for copying the contents
of an array. Note that the target array has to be created beforehand.
E.g., a one-to-one copy can be gotten by

.. code:: c

   ndarray_obj_t *new_ndarray = ndarray_new_ndarray(source->ndim, source->shape, source->strides, source->dtype);
   ndarray_copy_array(source, new_ndarray);

Note that the function cannot be used for forcing type conversion, i.e.,
the input and output types must be identical, because the function
simply calls the ``memcpy`` function. On the other hand, the input and
output ``strides`` do not necessarily have to be equal.

ndarray_copy_view
~~~~~~~~~~~~~~~~~

The ``ndarray_obj_t *new_ndarray = ...`` instruction can be saved by
calling the ``ndarray_copy_view`` function with the single ``source``
argument.

Accessing data in the ndarray
-----------------------------

Having seen, how arrays can be generated and copied, it is time to look
at how the data in an ``ndarray`` can be accessed and modified.

For starters, let us suppose that the object in question comes from the
user (i.e., via the ``micropython`` interface), First, we have to
acquire a pointer to the ``ndarray`` by calling

.. code:: c

   ndarray_obj_t *ndarray = MP_OBJ_TO_PTR(object_in);

If it is not clear, whether the object is an ``ndarray`` (e.g., if we
want to write a function that can take ``ndarray``\ s, and other
iterables as its argument), we find this out by evaluating

.. code:: c

   mp_obj_is_type(object_in, &ulab_ndarray_type)

which should return ``true``. Once the pointer is at our disposal, we
can get a pointer to the underlying numerical array as discussed
earlier, i.e.,

.. code:: c

   uint8_t *array = (uint8_t *)ndarray->array;

If you need to find out the ``dtype`` of the array, you can get it by
accessing the ``dtype`` member of the ``ndarray``, i.e.,

.. code:: c

   ndarray->dtype

should be equal to ``B``, ``b``, ``H``, ``h``, or ``f``. The size of a
single item is stored in the ``itemsize`` member. This number should be
equal to 1, if the ``dtype`` is ``B``, or ``b``, 2, if the ``dtype`` is
``H``, or ``h``, 4, if the ``dtype`` is ``f``, and 8 for ``d``.

Boilerplate
-----------

In the next section, we will construct a function that generates the
element-wise square of a dense array, otherwise, raises a ``TypeError``
exception. Dense arrays can easily be iterated over, since we do not
have to care about the ``shape`` and the ``strides``. If the array is
sparse, the section `Iterating over elements of a
tensor <#Iterating-over-elements-of-a-tensor>`__ should contain hints as
to how the iteration can be implemented.

The function is listed under
`user.c <https://github.com/v923z/micropython-ulab/tree/master/code/user/>`__.
The ``user`` module is bound to ``ulab`` in
`ulab.c <https://github.com/v923z/micropython-ulab/tree/master/code/ulab.c>`__
in the lines

.. code:: c

       #if ULAB_USER_MODULE
           { MP_ROM_QSTR(MP_QSTR_user), MP_ROM_PTR(&ulab_user_module) },
       #endif

which assumes that at the very end of
`ulab.h <https://github.com/v923z/micropython-ulab/tree/master/code/ulab.h>`__
the

.. code:: c

   // user-defined module
   #ifndef ULAB_USER_MODULE
   #define ULAB_USER_MODULE                (1)
   #endif

constant has been set to 1. After compilation, you can call a particular
``user`` function in ``python`` by importing the module first, i.e.,

.. code:: python

   from ulab import numpy as np
   from ulab import user

   user.some_function(...)

This separation of user-defined functions from the rest of the code
ensures that the integrity of the main module and all its functions are
always preserved. Even in case of a catastrophic failure, you can
exclude the ``user`` module, and start over.

And now the function:

.. code:: c

   static mp_obj_t user_square(mp_obj_t arg) {
       // the function takes a single dense ndarray, and calculates the 
       // element-wise square of its entries
       
       // raise a TypeError exception, if the input is not an ndarray
       if(!mp_obj_is_type(arg, &ulab_ndarray_type)) {
           mp_raise_TypeError(translate("input must be an ndarray"));
       }
       ndarray_obj_t *ndarray = MP_OBJ_TO_PTR(arg);
       
       // make sure that the input is a dense array
       if(!ndarray_is_dense(ndarray)) {
           mp_raise_TypeError(translate("input must be a dense ndarray"));
       }
       
       // if the input is a dense array, create `results` with the same number of 
       // dimensions, shape, and dtype
       ndarray_obj_t *results = ndarray_new_dense_ndarray(ndarray->ndim, ndarray->shape, ndarray->dtype);
       
       // since in a dense array the iteration over the elements is trivial, we 
       // can cast the data arrays ndarray->array and results->array to the actual type
       if(ndarray->dtype == NDARRAY_UINT8) {
           uint8_t *array = (uint8_t *)ndarray->array;
           uint8_t *rarray = (uint8_t *)results->array;
           for(size_t i=0; i < ndarray->len; i++, array++) {
               *rarray++ = (*array) * (*array);
           }
       } else if(ndarray->dtype == NDARRAY_INT8) {
           int8_t *array = (int8_t *)ndarray->array;
           int8_t *rarray = (int8_t *)results->array;
           for(size_t i=0; i < ndarray->len; i++, array++) {
               *rarray++ = (*array) * (*array);
           }
       } else if(ndarray->dtype == NDARRAY_UINT16) {
           uint16_t *array = (uint16_t *)ndarray->array;
           uint16_t *rarray = (uint16_t *)results->array;
           for(size_t i=0; i < ndarray->len; i++, array++) {
               *rarray++ = (*array) * (*array);
           }
       } else if(ndarray->dtype == NDARRAY_INT16) {
           int16_t *array = (int16_t *)ndarray->array;
           int16_t *rarray = (int16_t *)results->array;
           for(size_t i=0; i < ndarray->len; i++, array++) {
               *rarray++ = (*array) * (*array);
           }
       } else { // if we end up here, the dtype is NDARRAY_FLOAT
           mp_float_t *array = (mp_float_t *)ndarray->array;
           mp_float_t *rarray = (mp_float_t *)results->array;
           for(size_t i=0; i < ndarray->len; i++, array++) {
               *rarray++ = (*array) * (*array);
           }        
       }
       // at the end, return a micropython object
       return MP_OBJ_FROM_PTR(results);
   }

To summarise, the steps for *implementing* a function are

1. If necessary, inspect the type of the input object, which is always a
   ``mp_obj_t`` object
2. If the input is an ``ndarray_obj_t``, acquire a pointer to it by
   calling ``ndarray_obj_t *ndarray = MP_OBJ_TO_PTR(arg);``
3. Create a new array, or modify the existing one; get a pointer to the
   data by calling ``uint8_t *array = (uint8_t *)ndarray->array;``, or
   something equivalent
4. Once the new data have been calculated, return a ``micropython``
   object by calling ``MP_OBJ_FROM_PTR(...)``.

The listing above contains the implementation of the function, but as
such, it cannot be called from ``python``: it still has to be bound to
the name space. This we do by first defining a function object in

.. code:: c

   MP_DEFINE_CONST_FUN_OBJ_1(user_square_obj, user_square);

``micropython`` defines a number of ``MP_DEFINE_CONST_FUN_OBJ_N`` macros
in
`obj.h <https://github.com/micropython/micropython/blob/master/py/obj.h>`__.
``N`` is always the number of arguments the function takes. We had a
function definition ``static mp_obj_t user_square(mp_obj_t arg)``, i.e.,
we dealt with a single argument.

Finally, we have to bind this function object in the globals table of
the ``user`` module:

.. code:: c

   static const mp_rom_map_elem_t ulab_user_globals_table[] = {
       { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_user) },
       { MP_OBJ_NEW_QSTR(MP_QSTR_square), (mp_obj_t)&user_square_obj },
   };

Thus, the three steps required for the definition of a user-defined
function are

1. The low-level implementation of the function itself
2. The definition of a function object by calling
   MP_DEFINE_CONST_FUN_OBJ_N()
3. Binding this function object to the namespace in the
   ``ulab_user_globals_table[]``
