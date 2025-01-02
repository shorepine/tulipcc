
/*
 * This file is part of the micropython-ulab project,
 *
 * https://github.com/v923z/micropython-ulab
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2019-2023 Zoltán Vörös
 *               2020-2023 Jeff Epler for Adafruit Industries
 *               2020 Scott Shawcroft for Adafruit Industries
 *               2020 Taku Fukada
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "py/runtime.h"
#include "py/binary.h"
#include "py/obj.h"
#include "py/objarray.h"

#include "../ulab.h"
#include "../ulab_tools.h"
#include "carray/carray_tools.h"
#include "vector.h"

//| """Element-by-element functions
//|
//| These functions can operate on numbers, 1-D iterables, and arrays of 1 to 4 dimensions by
//| applying the function to every element in the array.  This is typically
//| much more efficient than expressing the same operation as a Python loop."""
//|


#if ULAB_MATH_FUNCTIONS_OUT_KEYWORD
static mp_obj_t vector_generic_vector(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args, mp_float_t (*f)(mp_float_t)) {
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_rom_obj = MP_ROM_NONE} } ,
        { MP_QSTR_out, MP_ARG_KW_ONLY | MP_ARG_OBJ, { .u_rom_obj = MP_ROM_NONE } },
        // this keyword argument is not used; it's only here, so that functions that 
        // support the complex dtype can call vector_generic_vector directly
        { MP_QSTR_dtype, MP_ARG_KW_ONLY | MP_ARG_OBJ, { .u_rom_obj = MP_ROM_NONE } },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    mp_obj_t o_in = args[0].u_obj;

    // Return a single value, if o_in is not iterable
    if(mp_obj_is_float(o_in) || mp_obj_is_int(o_in)) {
        return mp_obj_new_float(f(mp_obj_get_float(o_in)));
    }
    mp_obj_t out = args[1].u_obj;

    ndarray_obj_t *target = NULL;
    ndarray_obj_t *source = NULL;

    if(mp_obj_is_type(o_in, &ulab_ndarray_type)) {
        source = MP_OBJ_TO_PTR(o_in);
        COMPLEX_DTYPE_NOT_IMPLEMENTED(source->dtype)
        if(out == mp_const_none) {
            target = ndarray_new_dense_ndarray(source->ndim, source->shape, NDARRAY_FLOAT);
        } else {
            if(!mp_obj_is_type(out, &ulab_ndarray_type)) {
                mp_raise_ValueError(MP_ERROR_TEXT("out must be an ndarray"));
            }
            target = MP_OBJ_TO_PTR(out);
            if(target->dtype != NDARRAY_FLOAT) {
                mp_raise_ValueError(MP_ERROR_TEXT("out must be of float dtype"));
            }
            if(target->ndim != source->ndim) {
                mp_raise_ValueError(MP_ERROR_TEXT("input and output dimensions differ"));
            }
            for(uint8_t d = 0; d < target->ndim; d++) {
                if(target->shape[ULAB_MAX_DIMS - 1 - d] != source->shape[ULAB_MAX_DIMS - 1 - d]) {
                    mp_raise_ValueError(MP_ERROR_TEXT("input and output shapes differ"));
                }
            }
        }
        mp_float_t *tarray = (mp_float_t *)target->array;
        int32_t *tstrides = m_new(int32_t, ULAB_MAX_DIMS);
        for(uint8_t d = 0; d < target->ndim; d++) {
            tstrides[ULAB_MAX_DIMS - 1 - d] = target->strides[ULAB_MAX_DIMS - 1 - d] / target->itemsize;
        }

        uint8_t *sarray = (uint8_t *)source->array;

        #if ULAB_VECTORISE_USES_FUN_POINTER

            mp_float_t (*func)(void *) = ndarray_get_float_function(source->dtype);

            #if ULAB_MAX_DIMS > 3
            size_t i = 0;
            do {
            #endif
                #if ULAB_MAX_DIMS > 2
                size_t j = 0;
                do {
                #endif
                    #if ULAB_MAX_DIMS > 1
                    size_t k = 0;
                    do {
                    #endif
                        size_t l = 0;
                        do {
                            mp_float_t value = func(sarray);
                            *tarray++ = f(value);
                            sarray += source->strides[ULAB_MAX_DIMS - 1];
                            l++;
                        } while(l < source->shape[ULAB_MAX_DIMS - 1]);
                    #if ULAB_MAX_DIMS > 1
                        sarray -= source->strides[ULAB_MAX_DIMS - 1] * source->shape[ULAB_MAX_DIMS-1];
                        sarray += source->strides[ULAB_MAX_DIMS - 2];
                        k++;
                    } while(k < source->shape[ULAB_MAX_DIMS - 2]);
                    #endif /* ULAB_MAX_DIMS > 1 */
                #if ULAB_MAX_DIMS > 2
                    sarray -= source->strides[ULAB_MAX_DIMS - 2] * source->shape[ULAB_MAX_DIMS-2];
                    sarray += source->strides[ULAB_MAX_DIMS - 3];
                    j++;
                } while(j < source->shape[ULAB_MAX_DIMS - 3]);
                #endif /* ULAB_MAX_DIMS > 2 */
            #if ULAB_MAX_DIMS > 3
                sarray -= source->strides[ULAB_MAX_DIMS - 3] * source->shape[ULAB_MAX_DIMS-3];
                sarray += source->strides[ULAB_MAX_DIMS - 4];
                i++;
            } while(i < source->shape[ULAB_MAX_DIMS - 4]);
            #endif /* ULAB_MAX_DIMS > 3 */
        #else
        if(source->dtype == NDARRAY_UINT8) {
            ITERATE_VECTOR(uint8_t, target, tarray, tstrides, source, sarray);
        } else if(source->dtype == NDARRAY_INT8) {
           ITERATE_VECTOR(int8_t, target, tarray, tstrides, source, sarray);
        } else if(source->dtype == NDARRAY_UINT16) {
            ITERATE_VECTOR(uint16_t, target, tarray, tstrides, source, sarray);
        } else if(source->dtype == NDARRAY_INT16) {
            ITERATE_VECTOR(int16_t, target, tarray, tstrides, source, sarray);
        } else {
            ITERATE_VECTOR(mp_float_t, target, tarray, tstrides, source, sarray);
        }
        #endif /* ULAB_VECTORISE_USES_FUN_POINTER */
    } else {
        target = ndarray_from_mp_obj(o_in, 0);
        mp_float_t *tarray = (mp_float_t *)target->array;
        for(size_t i = 0; i < target->len; i++) {
            *tarray = f(*tarray);
            tarray++;
        }
    }
    return MP_OBJ_FROM_PTR(target);
}

#else

static mp_obj_t vector_generic_vector(mp_obj_t o_in, mp_float_t (*f)(mp_float_t)) {
    // Return a single value, if o_in is not iterable
    if(mp_obj_is_float(o_in) || mp_obj_is_int(o_in)) {
        return mp_obj_new_float(f(mp_obj_get_float(o_in)));
    }
    ndarray_obj_t *ndarray = NULL;
    if(mp_obj_is_type(o_in, &ulab_ndarray_type)) {
        ndarray_obj_t *source = MP_OBJ_TO_PTR(o_in);
        COMPLEX_DTYPE_NOT_IMPLEMENTED(source->dtype)
        uint8_t *sarray = (uint8_t *)source->array;
        ndarray = ndarray_new_dense_ndarray(source->ndim, source->shape, NDARRAY_FLOAT);
        mp_float_t *array = (mp_float_t *)ndarray->array;

        #if ULAB_VECTORISE_USES_FUN_POINTER

            mp_float_t (*func)(void *) = ndarray_get_float_function(source->dtype);

            #if ULAB_MAX_DIMS > 3
            size_t i = 0;
            do {
            #endif
                #if ULAB_MAX_DIMS > 2
                size_t j = 0;
                do {
                #endif
                    #if ULAB_MAX_DIMS > 1
                    size_t k = 0;
                    do {
                    #endif
                        size_t l = 0;
                        do {
                            mp_float_t value = func(sarray);
                            *array++ = f(value);
                            sarray += source->strides[ULAB_MAX_DIMS - 1];
                            l++;
                        } while(l < source->shape[ULAB_MAX_DIMS - 1]);
                    #if ULAB_MAX_DIMS > 1
                        sarray -= source->strides[ULAB_MAX_DIMS - 1] * source->shape[ULAB_MAX_DIMS-1];
                        sarray += source->strides[ULAB_MAX_DIMS - 2];
                        k++;
                    } while(k < source->shape[ULAB_MAX_DIMS - 2]);
                    #endif /* ULAB_MAX_DIMS > 1 */
                #if ULAB_MAX_DIMS > 2
                    sarray -= source->strides[ULAB_MAX_DIMS - 2] * source->shape[ULAB_MAX_DIMS-2];
                    sarray += source->strides[ULAB_MAX_DIMS - 3];
                    j++;
                } while(j < source->shape[ULAB_MAX_DIMS - 3]);
                #endif /* ULAB_MAX_DIMS > 2 */
            #if ULAB_MAX_DIMS > 3
                sarray -= source->strides[ULAB_MAX_DIMS - 3] * source->shape[ULAB_MAX_DIMS-3];
                sarray += source->strides[ULAB_MAX_DIMS - 4];
                i++;
            } while(i < source->shape[ULAB_MAX_DIMS - 4]);
            #endif /* ULAB_MAX_DIMS > 3 */
        #else
        if(source->dtype == NDARRAY_UINT8) {
            ITERATE_VECTOR(uint8_t, array, source, sarray);
        } else if(source->dtype == NDARRAY_INT8) {
            ITERATE_VECTOR(int8_t, array, source, sarray);
        } else if(source->dtype == NDARRAY_UINT16) {
            ITERATE_VECTOR(uint16_t, array, source, sarray);
        } else if(source->dtype == NDARRAY_INT16) {
            ITERATE_VECTOR(int16_t, array, source, sarray);
        } else {
            ITERATE_VECTOR(mp_float_t, array, source, sarray);
        }
        #endif /* ULAB_VECTORISE_USES_FUN_POINTER */
    } else {
        ndarray = ndarray_from_mp_obj(o_in, 0);
        mp_float_t *narray = (mp_float_t *)ndarray->array;
        for(size_t i = 0; i < ndarray->len; i++) {
            *narray = f(*narray);
            narray++;
        }
    }
    return MP_OBJ_FROM_PTR(ndarray);
}
#endif /* ULAB_MATH_FUNCTIONS_OUT_KEYWORD */


#if ULAB_NUMPY_HAS_ACOS
//| def acos(a: _ScalarOrArrayLike) -> _ScalarOrNdArray:
//|    """Computes the inverse cosine function"""
//|    ...
//|

MATH_FUN_1(acos, acos);
#if ULAB_MATH_FUNCTIONS_OUT_KEYWORD
MP_DEFINE_CONST_FUN_OBJ_KW(vector_acos_obj, 1, vector_acos);
#else
MP_DEFINE_CONST_FUN_OBJ_1(vector_acos_obj, vector_acos);
#endif /* ULAB_MATH_FUNCTIONS_OUT_KEYWORD */
#endif /* ULAB_NUMPY_HAS_ACOS */

#if ULAB_NUMPY_HAS_ACOSH
//| def acosh(a: _ScalarOrArrayLike) -> _ScalarOrNdArray:
//|    """Computes the inverse hyperbolic cosine function"""
//|    ...
//|

MATH_FUN_1(acosh, acosh);
#if ULAB_MATH_FUNCTIONS_OUT_KEYWORD
MP_DEFINE_CONST_FUN_OBJ_KW(vector_acosh_obj, 1, vector_acosh);
#else
MP_DEFINE_CONST_FUN_OBJ_1(vector_acosh_obj, vector_acosh);
#endif /* ULAB_MATH_FUNCTIONS_OUT_KEYWORD */
#endif /* ULAB_NUMPY_HAS_ACOSH */

#if ULAB_NUMPY_HAS_ASIN
//| def asin(a: _ScalarOrArrayLike) -> _ScalarOrNdArray:
//|    """Computes the inverse sine function"""
//|    ...
//|

MATH_FUN_1(asin, asin);
#if ULAB_MATH_FUNCTIONS_OUT_KEYWORD
MP_DEFINE_CONST_FUN_OBJ_KW(vector_asin_obj, 1, vector_asin);
#else
MP_DEFINE_CONST_FUN_OBJ_1(vector_asin_obj, vector_asin);
#endif /* ULAB_MATH_FUNCTIONS_OUT_KEYWORD */
#endif /* ULAB_NUMPY_HAS_ASIN */

#if ULAB_NUMPY_HAS_ASINH
//| def asinh(a: _ScalarOrArrayLike) -> _ScalarOrNdArray:
//|    """Computes the inverse hyperbolic sine function"""
//|    ...
//|

MATH_FUN_1(asinh, asinh);
#if ULAB_MATH_FUNCTIONS_OUT_KEYWORD
MP_DEFINE_CONST_FUN_OBJ_KW(vector_asinh_obj, 1, vector_asinh);
#else
MP_DEFINE_CONST_FUN_OBJ_1(vector_asinh_obj, vector_asinh);
#endif /* ULAB_MATH_FUNCTIONS_OUT_KEYWORD */
#endif /* ULAB_NUMPY_HAS_ASINH */

#if ULAB_NUMPY_HAS_AROUND
//| def around(a: ulab.numpy.ndarray, *, decimals: int = 0) -> ulab.numpy.ndarray:
//|    """Returns a new float array in which each element is rounded to
//|       ``decimals`` places."""
//|    ...
//|

mp_obj_t vector_around(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_rom_obj = MP_ROM_NONE} },
        { MP_QSTR_decimals, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 0 } },
        #if ULAB_MATH_FUNCTIONS_OUT_KEYWORD
        { MP_QSTR_out, MP_ARG_KW_ONLY | MP_ARG_OBJ, { .u_rom_obj = MP_ROM_NONE } }
        #endif  
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);
    if(!mp_obj_is_type(args[0].u_obj, &ulab_ndarray_type)) {
        mp_raise_TypeError(MP_ERROR_TEXT("first argument must be an ndarray"));
    }
    int8_t n = args[1].u_int;
    mp_float_t mul = MICROPY_FLOAT_C_FUN(pow)(10.0, n);
    ndarray_obj_t *source = MP_OBJ_TO_PTR(args[0].u_obj);
    COMPLEX_DTYPE_NOT_IMPLEMENTED(source->dtype)
    #if ULAB_MATH_FUNCTIONS_OUT_KEYWORD
    mp_obj_t out = args[2].u_obj;
    if(out != mp_const_none) {
        mp_raise_ValueError(MP_ERROR_TEXT("out keyword is not supported for function"));
    }
    #endif /* ULAB_MATH_FUNCTIONS_OUT_KEYWORD */
    ndarray_obj_t *ndarray = ndarray_new_dense_ndarray(source->ndim, source->shape, NDARRAY_FLOAT);
    mp_float_t *narray = (mp_float_t *)ndarray->array;
    uint8_t *sarray = (uint8_t *)source->array;

    mp_float_t (*func)(void *) = ndarray_get_float_function(source->dtype);

    #if ULAB_MAX_DIMS > 3
    size_t i = 0;
    do {
    #endif
        #if ULAB_MAX_DIMS > 2
        size_t j = 0;
        do {
        #endif
            #if ULAB_MAX_DIMS > 1
            size_t k = 0;
            do {
            #endif
                size_t l = 0;
                do {
                    mp_float_t f = func(sarray);
                    *narray++ = MICROPY_FLOAT_C_FUN(round)(f * mul) / mul;
                    sarray += source->strides[ULAB_MAX_DIMS - 1];
                    l++;
                } while(l < source->shape[ULAB_MAX_DIMS - 1]);
            #if ULAB_MAX_DIMS > 1
                sarray -= source->strides[ULAB_MAX_DIMS - 1] * source->shape[ULAB_MAX_DIMS-1];
                sarray += source->strides[ULAB_MAX_DIMS - 2];
                k++;
            } while(k < source->shape[ULAB_MAX_DIMS - 2]);
            #endif
        #if ULAB_MAX_DIMS > 2
            sarray -= source->strides[ULAB_MAX_DIMS - 2] * source->shape[ULAB_MAX_DIMS-2];
            sarray += source->strides[ULAB_MAX_DIMS - 3];
            j++;
        } while(j < source->shape[ULAB_MAX_DIMS - 3]);
        #endif
    #if ULAB_MAX_DIMS > 3
        sarray -= source->strides[ULAB_MAX_DIMS - 3] * source->shape[ULAB_MAX_DIMS-3];
        sarray += source->strides[ULAB_MAX_DIMS - 4];
        i++;
    } while(i < source->shape[ULAB_MAX_DIMS - 4]);
    #endif
    return MP_OBJ_FROM_PTR(ndarray);
}

MP_DEFINE_CONST_FUN_OBJ_KW(vector_around_obj, 1, vector_around);
#endif /* ULAB_NUMPY_HAS_AROUND */

#if ULAB_NUMPY_HAS_ATAN
//| def atan(a: _ScalarOrArrayLike) -> _ScalarOrNdArray:
//|    """Computes the inverse tangent function; the return values are in the
//|       range [-pi/2,pi/2]."""
//|    ...
//|

MATH_FUN_1(atan, atan);
#if ULAB_MATH_FUNCTIONS_OUT_KEYWORD
MP_DEFINE_CONST_FUN_OBJ_KW(vector_atan_obj, 1, vector_atan);
#else
MP_DEFINE_CONST_FUN_OBJ_1(vector_atan_obj, vector_atan);
#endif /* ULAB_MATH_FUNCTIONS_OUT_KEYWORD */
#endif /* ULAB_NUMPY_HAS_ATAN */


#if ULAB_NUMPY_HAS_ATANH
//| def atanh(a: _ScalarOrArrayLike) -> _ScalarOrNdArray:
//|    """Computes the inverse hyperbolic tangent function"""
//|    ...
//|

MATH_FUN_1(atanh, atanh);
#if ULAB_MATH_FUNCTIONS_OUT_KEYWORD
MP_DEFINE_CONST_FUN_OBJ_KW(vector_atanh_obj, 1, vector_atanh);
#else
MP_DEFINE_CONST_FUN_OBJ_1(vector_atanh_obj, vector_atanh);
#endif /* ULAB_MATH_FUNCTIONS_OUT_KEYWORD */
#endif /* ULAB_NUMPY_HAS_ATANH */

#if ULAB_NUMPY_HAS_ARCTAN2
//| def arctan2(ya: _ScalarOrArrayLike, xa: _ScalarOrArrayLike) -> _ScalarOrNdArray:
//|    """Computes the inverse tangent function of y/x; the return values are in
//|       the range [-pi, pi]."""
//|    ...
//|

mp_obj_t vector_arctan2(mp_obj_t y, mp_obj_t x) {
    if((mp_obj_is_float(y) || mp_obj_is_int(y)) &&
        (mp_obj_is_float(x) || mp_obj_is_int(x))) {
        mp_float_t _y = mp_obj_get_float(y);
        mp_float_t _x = mp_obj_get_float(x);
        return mp_obj_new_float(MICROPY_FLOAT_C_FUN(atan2)(_y, _x));
    }

    ndarray_obj_t *ndarray_x = ndarray_from_mp_obj(x, 0);
    COMPLEX_DTYPE_NOT_IMPLEMENTED(ndarray_x->dtype)

    ndarray_obj_t *ndarray_y = ndarray_from_mp_obj(y, 0);
    COMPLEX_DTYPE_NOT_IMPLEMENTED(ndarray_y->dtype)

    uint8_t ndim = 0;
    size_t *shape = m_new(size_t, ULAB_MAX_DIMS);
    int32_t *xstrides = m_new(int32_t, ULAB_MAX_DIMS);
    int32_t *ystrides = m_new(int32_t, ULAB_MAX_DIMS);
    if(!ndarray_can_broadcast(ndarray_x, ndarray_y, &ndim, shape, xstrides, ystrides)) {
        mp_raise_ValueError(MP_ERROR_TEXT("operands could not be broadcast together"));
        m_del(size_t, shape, ULAB_MAX_DIMS);
        m_del(int32_t, xstrides, ULAB_MAX_DIMS);
        m_del(int32_t, ystrides, ULAB_MAX_DIMS);
    }

    uint8_t *xarray = (uint8_t *)ndarray_x->array;
    uint8_t *yarray = (uint8_t *)ndarray_y->array;

    ndarray_obj_t *results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_FLOAT);
    mp_float_t *rarray = (mp_float_t *)results->array;

    mp_float_t (*funcx)(void *) = ndarray_get_float_function(ndarray_x->dtype);
    mp_float_t (*funcy)(void *) = ndarray_get_float_function(ndarray_y->dtype);

    #if ULAB_MAX_DIMS > 3
    size_t i = 0;
    do {
    #endif
        #if ULAB_MAX_DIMS > 2
        size_t j = 0;
        do {
        #endif
            #if ULAB_MAX_DIMS > 1
            size_t k = 0;
            do {
            #endif
                size_t l = 0;
                do {
                    mp_float_t _x = funcx(xarray);
                    mp_float_t _y = funcy(yarray);
                    *rarray++ = MICROPY_FLOAT_C_FUN(atan2)(_y, _x);
                    xarray += xstrides[ULAB_MAX_DIMS - 1];
                    yarray += ystrides[ULAB_MAX_DIMS - 1];
                    l++;
                } while(l < results->shape[ULAB_MAX_DIMS - 1]);
            #if ULAB_MAX_DIMS > 1
                xarray -= xstrides[ULAB_MAX_DIMS - 1] * results->shape[ULAB_MAX_DIMS-1];
                xarray += xstrides[ULAB_MAX_DIMS - 2];
                yarray -= ystrides[ULAB_MAX_DIMS - 1] * results->shape[ULAB_MAX_DIMS-1];
                yarray += ystrides[ULAB_MAX_DIMS - 2];
                k++;
            } while(k < results->shape[ULAB_MAX_DIMS - 2]);
            #endif
        #if ULAB_MAX_DIMS > 2
            xarray -= xstrides[ULAB_MAX_DIMS - 2] * results->shape[ULAB_MAX_DIMS-2];
            xarray += xstrides[ULAB_MAX_DIMS - 3];
            yarray -= ystrides[ULAB_MAX_DIMS - 2] * results->shape[ULAB_MAX_DIMS-2];
            yarray += ystrides[ULAB_MAX_DIMS - 3];
            j++;
        } while(j < results->shape[ULAB_MAX_DIMS - 3]);
        #endif
    #if ULAB_MAX_DIMS > 3
        xarray -= xstrides[ULAB_MAX_DIMS - 3] * results->shape[ULAB_MAX_DIMS-3];
        xarray += xstrides[ULAB_MAX_DIMS - 4];
        yarray -= ystrides[ULAB_MAX_DIMS - 3] * results->shape[ULAB_MAX_DIMS-3];
        yarray += ystrides[ULAB_MAX_DIMS - 4];
        i++;
    } while(i < results->shape[ULAB_MAX_DIMS - 4]);
    #endif

    return MP_OBJ_FROM_PTR(results);
}

MP_DEFINE_CONST_FUN_OBJ_2(vector_arctan2_obj, vector_arctan2);
#endif /* ULAB_VECTORISE_HAS_ARCTAN2 */

#if ULAB_NUMPY_HAS_CEIL
//| def ceil(a: _ScalarOrArrayLike) -> _ScalarOrNdArray:
//|    """Rounds numbers up to the next whole number"""
//|    ...
//|

MATH_FUN_1(ceil, ceil);
#if ULAB_MATH_FUNCTIONS_OUT_KEYWORD
MP_DEFINE_CONST_FUN_OBJ_KW(vector_ceil_obj, 1, vector_ceil);
#else
MP_DEFINE_CONST_FUN_OBJ_1(vector_ceil_obj, vector_ceil);
#endif /* ULAB_MATH_FUNCTIONS_OUT_KEYWORD */
#endif /* ULAB_NUMPY_HAS_CEIL */

#if ULAB_NUMPY_HAS_COS
//| def cos(a: _ScalarOrArrayLike) -> _ScalarOrNdArray:
//|    """Computes the cosine function"""
//|    ...
//|

MATH_FUN_1(cos, cos);
#if ULAB_MATH_FUNCTIONS_OUT_KEYWORD
MP_DEFINE_CONST_FUN_OBJ_KW(vector_cos_obj, 1, vector_cos);
#else
MP_DEFINE_CONST_FUN_OBJ_1(vector_cos_obj, vector_cos);
#endif /* ULAB_MATH_FUNCTIONS_OUT_KEYWORD */
#endif /* ULAB_NUMPY_HAS_COS */

#if ULAB_NUMPY_HAS_COSH
//| def cosh(a: _ScalarOrArrayLike) -> _ScalarOrNdArray:
//|    """Computes the hyperbolic cosine function"""
//|    ...
//|

MATH_FUN_1(cosh, cosh);
#if ULAB_MATH_FUNCTIONS_OUT_KEYWORD
MP_DEFINE_CONST_FUN_OBJ_KW(vector_cosh_obj, 1, vector_cosh);
#else
MP_DEFINE_CONST_FUN_OBJ_1(vector_cosh_obj, vector_cosh);
#endif /* ULAB_MATH_FUNCTIONS_OUT_KEYWORD */
#endif /* ULAB_NUMPY_HAS_COSH */

#if ULAB_NUMPY_HAS_DEGREES
//| def degrees(a: _ScalarOrArrayLike) -> _ScalarOrNdArray:
//|    """Converts angles from radians to degrees"""
//|    ...
//|

static mp_float_t vector_degrees_(mp_float_t value) {
    return value * MICROPY_FLOAT_CONST(180.0) / MP_PI;
}

#if ULAB_MATH_FUNCTIONS_OUT_KEYWORD
static mp_obj_t vector_degrees(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    return vector_generic_vector(n_args, pos_args, kw_args, vector_degrees_);
}
MP_DEFINE_CONST_FUN_OBJ_KW(vector_degrees_obj, 1, vector_degrees);
#else
static mp_obj_t vector_degrees(mp_obj_t x_obj) {
    return vector_generic_vector(x_obj, vector_degrees_);
}
MP_DEFINE_CONST_FUN_OBJ_1(vector_degrees_obj, vector_degrees);
#endif /* ULAB_MATH_FUNCTIONS_OUT_KEYWORD */
#endif /* ULAB_NUMPY_HAS_DEGREES */

#if ULAB_SCIPY_SPECIAL_HAS_ERF
//| def erf(a: _ScalarOrArrayLike) -> _ScalarOrNdArray:
//|    """Computes the error function, which has applications in statistics"""
//|    ...
//|

MATH_FUN_1(erf, erf);
#if ULAB_MATH_FUNCTIONS_OUT_KEYWORD
MP_DEFINE_CONST_FUN_OBJ_KW(vector_erf_obj, 1, vector_erf);
#else
MP_DEFINE_CONST_FUN_OBJ_1(vector_erf_obj, vector_erf);
#endif /* ULAB_MATH_FUNCTIONS_OUT_KEYWORD */
#endif /* ULAB_SCIPY_SPECIAL_HAS_ERF */

#if ULAB_SCIPY_SPECIAL_HAS_ERFC
//| def erfc(a: _ScalarOrArrayLike) -> _ScalarOrNdArray:
//|    """Computes the complementary error function, which has applications in statistics"""
//|    ...
//|

MATH_FUN_1(erfc, erfc);
#if ULAB_MATH_FUNCTIONS_OUT_KEYWORD
MP_DEFINE_CONST_FUN_OBJ_KW(vector_erfc_obj, 1, vector_erfc);
#else
MP_DEFINE_CONST_FUN_OBJ_1(vector_erfc_obj, vector_erfc);
#endif /* ULAB_MATH_FUNCTIONS_OUT_KEYWORD */
#endif /* ULAB_SCIPY_SPECIAL_HAS_ERFC */

#if ULAB_NUMPY_HAS_EXP
//| def exp(a: _ScalarOrArrayLike) -> _ScalarOrNdArray:
//|    """Computes the exponent function."""
//|    ...
//|
#if ULAB_MATH_FUNCTIONS_OUT_KEYWORD
static mp_obj_t vector_exp(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
#else
static mp_obj_t vector_exp(mp_obj_t o_in) {
#endif
    #if ULAB_SUPPORTS_COMPLEX
    
    #if ULAB_MATH_FUNCTIONS_OUT_KEYWORD
    // since the complex case is dissimilar to the rest, we've got to do the parsing of the keywords here
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_rom_obj = MP_ROM_NONE} } ,
        { MP_QSTR_out, MP_ARG_KW_ONLY | MP_ARG_OBJ, { .u_rom_obj = MP_ROM_NONE } },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);
    mp_obj_t o_in = args[0].u_obj;
    mp_obj_t out = args[1].u_obj;
    #endif /* ULAB_MATH_FUNCTIONS_OUT_KEYWORD */
    
    if(mp_obj_is_type(o_in, &mp_type_complex)) {
        mp_float_t real, imag;
        mp_obj_get_complex(o_in, &real, &imag);
        mp_float_t exp_real = MICROPY_FLOAT_C_FUN(exp)(real);
        return mp_obj_new_complex(exp_real * MICROPY_FLOAT_C_FUN(cos)(imag), exp_real * MICROPY_FLOAT_C_FUN(sin)(imag));
    } else if(mp_obj_is_type(o_in, &ulab_ndarray_type)) {
        ndarray_obj_t *source = MP_OBJ_TO_PTR(o_in);
        
        #if ULAB_MATH_FUNCTIONS_OUT_KEYWORD
        if((out != mp_const_none) && (source->dtype == NDARRAY_COMPLEX)){
            mp_raise_ValueError(MP_ERROR_TEXT("out keyword is not supported for complex dtype"));
        }
        #endif /* ULAB_MATH_FUNCTIONS_OUT_KEYWORD */

        if(source->dtype == NDARRAY_COMPLEX) {
            uint8_t *sarray = (uint8_t *)source->array;
            ndarray_obj_t *ndarray = ndarray_new_dense_ndarray(source->ndim, source->shape, NDARRAY_COMPLEX);
            mp_float_t *array = (mp_float_t *)ndarray->array;
            uint8_t itemsize = sizeof(mp_float_t);

            #if ULAB_MAX_DIMS > 3
            size_t i = 0;
            do {
            #endif
                #if ULAB_MAX_DIMS > 2
                size_t j = 0;
                do {
                #endif
                    #if ULAB_MAX_DIMS > 1
                    size_t k = 0;
                    do {
                    #endif
                        size_t l = 0;
                        do {
                            mp_float_t real = *(mp_float_t *)sarray;
                            mp_float_t imag = *(mp_float_t *)(sarray + itemsize);
                            mp_float_t exp_real = MICROPY_FLOAT_C_FUN(exp)(real);
                            *array++ = exp_real * MICROPY_FLOAT_C_FUN(cos)(imag);
                            *array++ = exp_real * MICROPY_FLOAT_C_FUN(sin)(imag);
                            sarray += source->strides[ULAB_MAX_DIMS - 1];
                            l++;
                        } while(l < source->shape[ULAB_MAX_DIMS - 1]);
                    #if ULAB_MAX_DIMS > 1
                        sarray -= source->strides[ULAB_MAX_DIMS - 1] * source->shape[ULAB_MAX_DIMS-1];
                        sarray += source->strides[ULAB_MAX_DIMS - 2];
                        k++;
                    } while(k < source->shape[ULAB_MAX_DIMS - 2]);
                    #endif /* ULAB_MAX_DIMS > 1 */
                #if ULAB_MAX_DIMS > 2
                    sarray -= source->strides[ULAB_MAX_DIMS - 2] * source->shape[ULAB_MAX_DIMS-2];
                    sarray += source->strides[ULAB_MAX_DIMS - 3];
                    j++;
                } while(j < source->shape[ULAB_MAX_DIMS - 3]);
                #endif /* ULAB_MAX_DIMS > 2 */
            #if ULAB_MAX_DIMS > 3
                sarray -= source->strides[ULAB_MAX_DIMS - 3] * source->shape[ULAB_MAX_DIMS-3];
                sarray += source->strides[ULAB_MAX_DIMS - 4];
                i++;
            } while(i < source->shape[ULAB_MAX_DIMS - 4]);
            #endif /* ULAB_MAX_DIMS > 3 */
            return MP_OBJ_FROM_PTR(ndarray);
        }
    }
    #endif /* ULAB_SUPPORTS_COMPLEX */
    #if ULAB_MATH_FUNCTIONS_OUT_KEYWORD
    return vector_generic_vector(n_args, pos_args, kw_args, MICROPY_FLOAT_C_FUN(exp));   
    #else
    return vector_generic_vector(o_in, MICROPY_FLOAT_C_FUN(exp));
    #endif /* ULAB_MATH_FUNCTIONS_OUT_KEYWORD */
}

#if ULAB_MATH_FUNCTIONS_OUT_KEYWORD
MP_DEFINE_CONST_FUN_OBJ_KW(vector_exp_obj, 1, vector_exp);
#else
MP_DEFINE_CONST_FUN_OBJ_1(vector_exp_obj, vector_exp);
#endif /* ULAB_MATH_FUNCTIONS_OUT_KEYWORD */
#endif /* ULAB_NUMPY_HAS_EXP */

#if ULAB_NUMPY_HAS_EXPM1
//| def expm1(a: _ScalarOrArrayLike) -> _ScalarOrNdArray:
//|    """Computes $e^x-1$.  In certain applications, using this function preserves numeric accuracy better than the `exp` function."""
//|    ...
//|

MATH_FUN_1(expm1, expm1);
#if ULAB_MATH_FUNCTIONS_OUT_KEYWORD
MP_DEFINE_CONST_FUN_OBJ_KW(vector_expm1_obj, 1, vector_expm1);
#else
MP_DEFINE_CONST_FUN_OBJ_1(vector_expm1_obj, vector_expm1);
#endif /* ULAB_MATH_FUNCTIONS_OUT_KEYWORD */
#endif /* ULAB_NUMPY_HAS_EXPM1 */

#if ULAB_NUMPY_HAS_FLOOR
//| def floor(a: _ScalarOrArrayLike) -> _ScalarOrNdArray:
//|    """Rounds numbers up to the next whole number"""
//|    ...
//|

MATH_FUN_1(floor, floor);
#if ULAB_MATH_FUNCTIONS_OUT_KEYWORD
MP_DEFINE_CONST_FUN_OBJ_KW(vector_floor_obj, 1, vector_floor);
#else
MP_DEFINE_CONST_FUN_OBJ_1(vector_floor_obj, vector_floor);
#endif /* ULAB_MATH_FUNCTIONS_OUT_KEYWORD */
#endif /* ULAB_NUMPY_HAS_FLOOR */

#if ULAB_SCIPY_SPECIAL_HAS_GAMMA
//| def gamma(a: _ScalarOrArrayLike) -> _ScalarOrNdArray:
//|    """Computes the gamma function"""
//|    ...
//|

MATH_FUN_1(gamma, tgamma);
#if ULAB_MATH_FUNCTIONS_OUT_KEYWORD
MP_DEFINE_CONST_FUN_OBJ_KW(vector_gamma_obj, 1, vector_gamma);
#else
MP_DEFINE_CONST_FUN_OBJ_1(vector_gamma_obj, vector_gamma);
#endif /* ULAB_MATH_FUNCTIONS_OUT_KEYWORD */
#endif /* ULAB_SCIPY_SPECIAL_HAS_GAMMA */

#if ULAB_SCIPY_SPECIAL_HAS_GAMMALN
//| def lgamma(a: _ScalarOrArrayLike) -> _ScalarOrNdArray:
//|    """Computes the natural log of the gamma function"""
//|    ...
//|

MATH_FUN_1(lgamma, lgamma);
#if ULAB_MATH_FUNCTIONS_OUT_KEYWORD
MP_DEFINE_CONST_FUN_OBJ_KW(vector_lgamma_obj, 1, vector_lgamma);
#else
MP_DEFINE_CONST_FUN_OBJ_1(vector_lgamma_obj, vector_lgamma);
#endif /* ULAB_MATH_FUNCTIONS_OUT_KEYWORD */
#endif /* ULAB_SCIPY_SEPCIAL_HAS_GAMMALN */

#if ULAB_NUMPY_HAS_LOG
//| def log(a: _ScalarOrArrayLike) -> _ScalarOrNdArray:
//|    """Computes the natural log"""
//|    ...
//|

MATH_FUN_1(log, log);
#if ULAB_MATH_FUNCTIONS_OUT_KEYWORD
MP_DEFINE_CONST_FUN_OBJ_KW(vector_log_obj, 1, vector_log);
#else
MP_DEFINE_CONST_FUN_OBJ_1(vector_log_obj, vector_log);
#endif /* ULAB_MATH_FUNCTIONS_OUT_KEYWORD */
#endif /* ULAB_NUMPY_HAS_LOG */

#if ULAB_NUMPY_HAS_LOG10
//| def log10(a: _ScalarOrArrayLike) -> _ScalarOrNdArray:
//|    """Computes the log base 10"""
//|    ...
//|

MATH_FUN_1(log10, log10);
#if ULAB_MATH_FUNCTIONS_OUT_KEYWORD
MP_DEFINE_CONST_FUN_OBJ_KW(vector_log10_obj, 1, vector_log10);
#else
MP_DEFINE_CONST_FUN_OBJ_1(vector_log10_obj, vector_log10);
#endif /* ULAB_MATH_FUNCTIONS_OUT_KEYWORD */
#endif /* ULAB_NUMPY_HAS_LOG10 */

#if ULAB_NUMPY_HAS_LOG2
//| def log2(a: _ScalarOrArrayLike) -> _ScalarOrNdArray:
//|    """Computes the log base 2"""
//|    ...
//|

MATH_FUN_1(log2, log2);
#if ULAB_MATH_FUNCTIONS_OUT_KEYWORD
MP_DEFINE_CONST_FUN_OBJ_KW(vector_log2_obj, 1, vector_log2);
#else
MP_DEFINE_CONST_FUN_OBJ_1(vector_log2_obj, vector_log2);
#endif /* ULAB_MATH_FUNCTIONS_OUT_KEYWORD */
#endif /* ULAB_NUMPY_HAS_LOG2 */

#if ULAB_NUMPY_HAS_RADIANS
//| def radians(a: _ScalarOrArrayLike) -> _ScalarOrNdArray:
//|    """Converts angles from degrees to radians"""
//|    ...
//|

static mp_float_t vector_radians_(mp_float_t value) {
    return value * MP_PI / MICROPY_FLOAT_CONST(180.0);
}

#if ULAB_MATH_FUNCTIONS_OUT_KEYWORD
static mp_obj_t vector_radians(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    return vector_generic_vector(n_args, pos_args, kw_args, vector_radians_);
}
MP_DEFINE_CONST_FUN_OBJ_KW(vector_radians_obj, 1, vector_radians);
#else
static mp_obj_t vector_radians(mp_obj_t x_obj) {
    return vector_generic_vector(x_obj, vector_radians_);
}
MP_DEFINE_CONST_FUN_OBJ_1(vector_radians_obj, vector_radians);
#endif /* ULAB_MATH_FUNCTIONS_OUT_KEYWORD */
#endif /* ULAB_NUMPY_HAS_RADIANS */

#if ULAB_NUMPY_HAS_SIN
//| def sin(a: _ScalarOrArrayLike) -> _ScalarOrNdArray:
//|    """Computes the sine function"""
//|    ...
//|

MATH_FUN_1(sin, sin);
#if ULAB_MATH_FUNCTIONS_OUT_KEYWORD
MP_DEFINE_CONST_FUN_OBJ_KW(vector_sin_obj, 1, vector_sin);
#else
MP_DEFINE_CONST_FUN_OBJ_1(vector_sin_obj, vector_sin);
#endif /* ULAB_MATH_FUNCTIONS_OUT_KEYWORD */
#endif /* ULAB_NUMPY_HAS_SIN */

#if ULAB_NUMPY_HAS_SINC
//| def sinc(a: _ScalarOrArrayLike) -> _ScalarOrNdArray:
//|    """Computes the normalized sinc function"""
//|    ...
//|

static mp_float_t vector_sinc1(mp_float_t x) {
    if (x == MICROPY_FLOAT_CONST(0.)) {
        return MICROPY_FLOAT_CONST(1.);
    }
    x *= MP_PI;
    return MICROPY_FLOAT_C_FUN(sin)(x) / x;
}

#if ULAB_MATH_FUNCTIONS_OUT_KEYWORD
static mp_obj_t vector_sinc(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    return vector_generic_vector(n_args, pos_args, kw_args, vector_sinc1);
}
MP_DEFINE_CONST_FUN_OBJ_KW(vector_sinc_obj, 1, vector_sinc);
#else
static mp_obj_t vector_sinc(mp_obj_t x_obj) {
    return vector_generic_vector(x_obj, vector_sinc1);
}
MP_DEFINE_CONST_FUN_OBJ_1(vector_sinc_obj, vector_sinc);
#endif /* ULAB_MATH_FUNCTIONS_OUT_KEYWORD */
#endif /* ULAB_NUMPY_HAS_SINC */

#if ULAB_NUMPY_HAS_SINH
//| def sinh(a: _ScalarOrArrayLike) -> _ScalarOrNdArray:
//|    """Computes the hyperbolic sine"""
//|    ...
//|

MATH_FUN_1(sinh, sinh);
#if ULAB_MATH_FUNCTIONS_OUT_KEYWORD
MP_DEFINE_CONST_FUN_OBJ_KW(vector_sinh_obj, 1, vector_sinh);
#else
MP_DEFINE_CONST_FUN_OBJ_1(vector_sinh_obj, vector_sinh);
#endif /* ULAB_MATH_FUNCTIONS_OUT_KEYWORD */
#endif /* ULAB_NUMPY_HAS_SINH */


#if ULAB_NUMPY_HAS_SQRT
//| def sqrt(a: _ScalarOrArrayLike) -> _ScalarOrNdArray:
//|    """Computes the square root"""
//|    ...
//|

#if ULAB_SUPPORTS_COMPLEX | ULAB_MATH_FUNCTIONS_OUT_KEYWORD
mp_obj_t vector_sqrt(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_rom_obj = MP_ROM_NONE } },
        #if ULAB_MATH_FUNCTIONS_OUT_KEYWORD
        { MP_QSTR_out, MP_ARG_KW_ONLY | MP_ARG_OBJ, { .u_rom_obj = MP_ROM_NONE } },
        #endif
        #if ULAB_SUPPORTS_COMPLEX
        { MP_QSTR_dtype, MP_ARG_KW_ONLY | MP_ARG_OBJ, { .u_rom_obj = MP_ROM_INT(NDARRAY_FLOAT) } },
        #endif
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);


    #if ULAB_SUPPORTS_COMPLEX
    mp_obj_t o_in = args[0].u_obj;
    uint8_t dtype = mp_obj_get_int(args[2].u_obj);
    if((dtype != NDARRAY_FLOAT) && (dtype != NDARRAY_COMPLEX)) {
        mp_raise_TypeError(MP_ERROR_TEXT("dtype must be float, or complex"));
    }
    
    if(mp_obj_is_type(o_in, &mp_type_complex)) {
        mp_float_t real, imag;
        mp_obj_get_complex(o_in, &real, &imag);
        mp_float_t sqrt_abs = MICROPY_FLOAT_C_FUN(sqrt)(real * real + imag * imag);
        sqrt_abs = MICROPY_FLOAT_C_FUN(sqrt)(sqrt_abs);
        mp_float_t theta = MICROPY_FLOAT_CONST(0.5) * MICROPY_FLOAT_C_FUN(atan2)(imag, real);
        return mp_obj_new_complex(sqrt_abs * MICROPY_FLOAT_C_FUN(cos)(theta), sqrt_abs * MICROPY_FLOAT_C_FUN(sin)(theta));
    } else if(mp_obj_is_type(o_in, &ulab_ndarray_type)) {
        ndarray_obj_t *source = MP_OBJ_TO_PTR(o_in);
        if((source->dtype == NDARRAY_COMPLEX) && (dtype == NDARRAY_FLOAT)) {
            mp_raise_TypeError(MP_ERROR_TEXT("can't convert complex to float"));
        }

        if(dtype == NDARRAY_COMPLEX) {
            #if ULAB_MATH_FUNCTIONS_OUT_KEYWORD
            mp_obj_t out = args[1].u_obj;
            if(out != mp_const_none) {
                mp_raise_ValueError(MP_ERROR_TEXT("out keyword is not supported for complex dtype"));
            }
            #endif
            if(source->dtype == NDARRAY_COMPLEX) {
                uint8_t *sarray = (uint8_t *)source->array;
                ndarray_obj_t *ndarray = ndarray_new_dense_ndarray(source->ndim, source->shape, NDARRAY_COMPLEX);
                mp_float_t *array = (mp_float_t *)ndarray->array;
                uint8_t itemsize = sizeof(mp_float_t);

                #if ULAB_MAX_DIMS > 3
                size_t i = 0;
                do {
                #endif
                    #if ULAB_MAX_DIMS > 2
                    size_t j = 0;
                    do {
                    #endif
                        #if ULAB_MAX_DIMS > 1
                        size_t k = 0;
                        do {
                        #endif
                            size_t l = 0;
                            do {
                                mp_float_t real = *(mp_float_t *)sarray;
                                mp_float_t imag = *(mp_float_t *)(sarray + itemsize);
                                mp_float_t sqrt_abs = MICROPY_FLOAT_C_FUN(sqrt)(real * real + imag * imag);
                                sqrt_abs = MICROPY_FLOAT_C_FUN(sqrt)(sqrt_abs);
                                mp_float_t theta = MICROPY_FLOAT_CONST(0.5) * MICROPY_FLOAT_C_FUN(atan2)(imag, real);
                                *array++ = sqrt_abs * MICROPY_FLOAT_C_FUN(cos)(theta);
                                *array++ = sqrt_abs * MICROPY_FLOAT_C_FUN(sin)(theta);
                                sarray += source->strides[ULAB_MAX_DIMS - 1];
                                l++;
                            } while(l < source->shape[ULAB_MAX_DIMS - 1]);
                        #if ULAB_MAX_DIMS > 1
                            sarray -= source->strides[ULAB_MAX_DIMS - 1] * source->shape[ULAB_MAX_DIMS-1];
                            sarray += source->strides[ULAB_MAX_DIMS - 2];
                            k++;
                        } while(k < source->shape[ULAB_MAX_DIMS - 2]);
                        #endif /* ULAB_MAX_DIMS > 1 */
                    #if ULAB_MAX_DIMS > 2
                        sarray -= source->strides[ULAB_MAX_DIMS - 2] * source->shape[ULAB_MAX_DIMS-2];
                        sarray += source->strides[ULAB_MAX_DIMS - 3];
                        j++;
                    } while(j < source->shape[ULAB_MAX_DIMS - 3]);
                    #endif /* ULAB_MAX_DIMS > 2 */
                #if ULAB_MAX_DIMS > 3
                    sarray -= source->strides[ULAB_MAX_DIMS - 3] * source->shape[ULAB_MAX_DIMS-3];
                    sarray += source->strides[ULAB_MAX_DIMS - 4];
                    i++;
                } while(i < source->shape[ULAB_MAX_DIMS - 4]);
                #endif /* ULAB_MAX_DIMS > 3 */
                return MP_OBJ_FROM_PTR(ndarray);
            } else if(source->dtype == NDARRAY_FLOAT) {
                uint8_t *sarray = (uint8_t *)source->array;
                ndarray_obj_t *ndarray = ndarray_new_dense_ndarray(source->ndim, source->shape, NDARRAY_COMPLEX);
                mp_float_t *array = (mp_float_t *)ndarray->array;

                #if ULAB_MAX_DIMS > 3
                size_t i = 0;
                do {
                #endif
                    #if ULAB_MAX_DIMS > 2
                    size_t j = 0;
                    do {
                    #endif
                        #if ULAB_MAX_DIMS > 1
                        size_t k = 0;
                        do {
                        #endif
                            size_t l = 0;
                            do {
                                mp_float_t value = *(mp_float_t *)sarray;
                                if(value >= MICROPY_FLOAT_CONST(0.0)) {
                                    *array++ = MICROPY_FLOAT_C_FUN(sqrt)(value);
                                    array++;
                                } else {
                                    array++;
                                    *array++ = MICROPY_FLOAT_C_FUN(sqrt)(-value);
                                }
                                sarray += source->strides[ULAB_MAX_DIMS - 1];
                                l++;
                            } while(l < source->shape[ULAB_MAX_DIMS - 1]);
                        #if ULAB_MAX_DIMS > 1
                            sarray -= source->strides[ULAB_MAX_DIMS - 1] * source->shape[ULAB_MAX_DIMS-1];
                            sarray += source->strides[ULAB_MAX_DIMS - 2];
                            k++;
                        } while(k < source->shape[ULAB_MAX_DIMS - 2]);
                        #endif /* ULAB_MAX_DIMS > 1 */
                    #if ULAB_MAX_DIMS > 2
                        sarray -= source->strides[ULAB_MAX_DIMS - 2] * source->shape[ULAB_MAX_DIMS-2];
                        sarray += source->strides[ULAB_MAX_DIMS - 3];
                        j++;
                    } while(j < source->shape[ULAB_MAX_DIMS - 3]);
                    #endif /* ULAB_MAX_DIMS > 2 */
                #if ULAB_MAX_DIMS > 3
                    sarray -= source->strides[ULAB_MAX_DIMS - 3] * source->shape[ULAB_MAX_DIMS-3];
                    sarray += source->strides[ULAB_MAX_DIMS - 4];
                    i++;
                } while(i < source->shape[ULAB_MAX_DIMS - 4]);
                #endif /* ULAB_MAX_DIMS > 3 */
                return MP_OBJ_FROM_PTR(ndarray);
            } else {
                mp_raise_TypeError(MP_ERROR_TEXT("input dtype must be float or complex"));
            }
        }
    }
    #endif /* ULAB_SUPPORTS_COMPLEX */
    return vector_generic_vector(n_args, pos_args, kw_args, MICROPY_FLOAT_C_FUN(sqrt));
}

MP_DEFINE_CONST_FUN_OBJ_KW(vector_sqrt_obj, 1, vector_sqrt);
#else
MATH_FUN_1(sqrt, sqrt);
MP_DEFINE_CONST_FUN_OBJ_1(vector_sqrt_obj, vector_sqrt);
#endif /* ULAB_MATH_FUNCTIONS_OUT_KEYWORD | ULAB_SUPPORTS_COMPLEX */
#endif /* ULAB_NUMPY_HAS_SQRT */

#if ULAB_NUMPY_HAS_TAN
//| def tan(a: _ScalarOrArrayLike) -> _ScalarOrNdArray:
//|    """Computes the tangent"""
//|    ...
//|

MATH_FUN_1(tan, tan);
#if ULAB_MATH_FUNCTIONS_OUT_KEYWORD
MP_DEFINE_CONST_FUN_OBJ_KW(vector_tan_obj, 1, vector_tan);
#else
MP_DEFINE_CONST_FUN_OBJ_1(vector_tan_obj, vector_tan);
#endif /* ULAB_MATH_FUNCTIONS_OUT_KEYWORD */
#endif /* ULAB_NUMPY_HAS_TAN */

#if ULAB_NUMPY_HAS_TANH
//| def tanh(a: _ScalarOrArrayLike) -> _ScalarOrNdArray:
//|    """Computes the hyperbolic tangent"""
//|    ...

MATH_FUN_1(tanh, tanh);
#if ULAB_MATH_FUNCTIONS_OUT_KEYWORD
MP_DEFINE_CONST_FUN_OBJ_KW(vector_tanh_obj, 1, vector_tanh);
#else
MP_DEFINE_CONST_FUN_OBJ_1(vector_tanh_obj, vector_tanh);
#endif /* ULAB_MATH_FUNCTIONS_OUT_KEYWORD */
#endif /* ULAB_NUMPY_HAS_TANH */

#if ULAB_NUMPY_HAS_VECTORIZE
static mp_obj_t vector_vectorized_function_call(mp_obj_t self_in, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    (void) n_args;
    (void) n_kw;
    vectorized_function_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_obj_t avalue[1];
    mp_obj_t fvalue;
    if(mp_obj_is_type(args[0], &ulab_ndarray_type)) {
        ndarray_obj_t *source = MP_OBJ_TO_PTR(args[0]);
        COMPLEX_DTYPE_NOT_IMPLEMENTED(source->dtype)

        ndarray_obj_t *ndarray = ndarray_new_dense_ndarray(source->ndim, source->shape, self->otypes);
        uint8_t *sarray = (uint8_t *)source->array;
        uint8_t *narray = (uint8_t *)ndarray->array;
        
        #if ULAB_MAX_DIMS > 3
        size_t i = 0;
        do {
        #endif
            #if ULAB_MAX_DIMS > 2
            size_t j = 0;
            do {
            #endif
                #if ULAB_MAX_DIMS > 1
                size_t k = 0;
                do {
                #endif
                    size_t l = 0;
                    do {
                        avalue[0] = mp_binary_get_val_array(source->dtype, sarray, 0);
                        fvalue = MP_OBJ_TYPE_GET_SLOT(self->type, call)(self->fun, 1, 0, avalue);
                        ndarray_set_value(self->otypes, narray, 0, fvalue);
                        sarray += source->strides[ULAB_MAX_DIMS - 1];
                        narray += ndarray->itemsize;
                        l++;
                    } while(l < source->shape[ULAB_MAX_DIMS - 1]);
                #if ULAB_MAX_DIMS > 1
                    sarray -= source->strides[ULAB_MAX_DIMS - 1] * source->shape[ULAB_MAX_DIMS - 1];
                    sarray += source->strides[ULAB_MAX_DIMS - 2];
                    k++;
                } while(k < source->shape[ULAB_MAX_DIMS - 2]);
                #endif /* ULAB_MAX_DIMS > 1 */
            #if ULAB_MAX_DIMS > 2
                sarray -= source->strides[ULAB_MAX_DIMS - 2] * source->shape[ULAB_MAX_DIMS - 2];
                sarray += source->strides[ULAB_MAX_DIMS - 3];
                j++;
            } while(j < source->shape[ULAB_MAX_DIMS - 3]);
            #endif /* ULAB_MAX_DIMS > 2 */
        #if ULAB_MAX_DIMS > 3
            sarray -= source->strides[ULAB_MAX_DIMS - 3] * source->shape[ULAB_MAX_DIMS - 3];
            sarray += source->strides[ULAB_MAX_DIMS - 4];
            i++;
        } while(i < source->shape[ULAB_MAX_DIMS - 4]);
        #endif /* ULAB_MAX_DIMS > 3 */
        
        return MP_OBJ_FROM_PTR(ndarray);
    } else if(mp_obj_is_type(args[0], &mp_type_tuple) || mp_obj_is_type(args[0], &mp_type_list) ||
        mp_obj_is_type(args[0], &mp_type_range)) { // i.e., the input is a generic iterable
        size_t len = (size_t)mp_obj_get_int(mp_obj_len_maybe(args[0]));
        ndarray_obj_t *ndarray = ndarray_new_linear_array(len, self->otypes);
        mp_obj_iter_buf_t iter_buf;
        mp_obj_t iterable = mp_getiter(args[0], &iter_buf);
        size_t i=0;
        while ((avalue[0] = mp_iternext(iterable)) != MP_OBJ_STOP_ITERATION) {
            fvalue = MP_OBJ_TYPE_GET_SLOT(self->type, call)(self->fun, 1, 0, avalue);
            ndarray_set_value(self->otypes, ndarray->array, i, fvalue);
            i++;
        }
        return MP_OBJ_FROM_PTR(ndarray);
    } else if(mp_obj_is_int(args[0]) || mp_obj_is_float(args[0])) {
        ndarray_obj_t *ndarray = ndarray_new_linear_array(1, self->otypes);
        fvalue = MP_OBJ_TYPE_GET_SLOT(self->type, call)(self->fun, 1, 0, args);
        ndarray_set_value(self->otypes, ndarray->array, 0, fvalue);
        return MP_OBJ_FROM_PTR(ndarray);
    } else {
        mp_raise_ValueError(MP_ERROR_TEXT("wrong input type"));
    }
    return mp_const_none;
}

#if defined(MP_DEFINE_CONST_OBJ_TYPE)
MP_DEFINE_CONST_OBJ_TYPE(
    vector_function_type,
    MP_QSTR_,
    MP_TYPE_FLAG_NONE,
    call, vector_vectorized_function_call
);
#else
const mp_obj_type_t vector_function_type = {
    { &mp_type_type },
    .flags = MP_TYPE_FLAG_EXTENDED,
    .name = MP_QSTR_,
    MP_TYPE_EXTENDED_FIELDS(
    .call = vector_vectorized_function_call,
    )
};
#endif

//| def vectorize(
//|     f: Union[Callable[[int], _float], Callable[[_float], _float]],
//|     *,
//|     otypes: Optional[_DType] = None
//| ) -> Callable[[_ScalarOrArrayLike], ulab.numpy.ndarray]:
//|    """
//|    :param callable f: The function to wrap
//|    :param otypes: List of array types that may be returned by the function.  None is interpreted to mean the return value is float.
//|
//|    Wrap a Python function ``f`` so that it can be applied to arrays or scalars. A scalar passed to the wrapped function is treated as a single-element 1-D array.
//|    The callable must return only values of the types specified by ``otypes``, or the result is undefined."""
//|    ...
//|

static mp_obj_t vector_vectorize(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_rom_obj = MP_ROM_NONE} },
        { MP_QSTR_otypes, MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_rom_obj = MP_ROM_NONE} }
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);
    const mp_obj_type_t *type = mp_obj_get_type(args[0].u_obj);
    if(!MP_OBJ_TYPE_HAS_SLOT(type, call)) {
        mp_raise_TypeError(MP_ERROR_TEXT("first argument must be a callable"));
    }
    mp_obj_t _otypes = args[1].u_obj;
    uint8_t otypes = NDARRAY_FLOAT;
    if(_otypes == mp_const_none) {
        // TODO: is this what numpy does?
        otypes = NDARRAY_FLOAT;
    } else if(mp_obj_is_int(_otypes)) {
        otypes = mp_obj_get_int(_otypes);
        if(otypes != NDARRAY_FLOAT && otypes != NDARRAY_UINT8 && otypes != NDARRAY_INT8 &&
            otypes != NDARRAY_UINT16 && otypes != NDARRAY_INT16) {
                mp_raise_ValueError(MP_ERROR_TEXT("wrong output type"));
        }
    }
    else {
        mp_raise_ValueError(MP_ERROR_TEXT("wrong output type"));
    }
    vectorized_function_obj_t *function = m_new_obj(vectorized_function_obj_t);
    function->base.type = &vector_function_type;
    function->otypes = otypes;
    function->fun = args[0].u_obj;
    function->type = type;
    return MP_OBJ_FROM_PTR(function);
}

MP_DEFINE_CONST_FUN_OBJ_KW(vector_vectorize_obj, 1, vector_vectorize);
#endif
