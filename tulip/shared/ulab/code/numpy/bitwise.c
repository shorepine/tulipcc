/*
 * This file is part of the micropython-ulab project,
 *
 * https://github.com/v923z/micropython-ulab
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2023 Zoltán Vörös
 *
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "py/obj.h"
#include "py/runtime.h"

#include "bitwise.h"


#if ULAB_NUMPY_HAS_BITWISE_AND
ndarray_obj_t *bitwise_bitwise_and_loop(ndarray_obj_t *lhs, ndarray_obj_t *rhs, uint8_t ndim, size_t *shape, int32_t *lstrides, int32_t *rstrides) {
    // AND is commutative, so simply swap the order, if a particular combination has already been inspected

    ndarray_obj_t *results = NULL;
    uint8_t *larray = (uint8_t *)lhs->array;
    uint8_t *rarray = (uint8_t *)rhs->array;

    if(lhs->dtype == NDARRAY_UINT8) {
        if(rhs->dtype == NDARRAY_UINT8) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_UINT8);
            BINARY_LOOP(results, uint8_t, uint8_t, uint8_t, larray, lstrides, rarray, rstrides, &);
        } else if(rhs->dtype == NDARRAY_INT8) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_INT16);
            BINARY_LOOP(results, int16_t, uint8_t, int8_t, larray, lstrides, rarray, rstrides, &);
        } else if(rhs->dtype == NDARRAY_UINT16) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_UINT16);
            BINARY_LOOP(results, uint16_t, uint8_t, uint16_t, larray, lstrides, rarray, rstrides, &);
        } else if(rhs->dtype == NDARRAY_INT16) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_INT16);
            BINARY_LOOP(results, int16_t, uint8_t, int16_t, larray, lstrides, rarray, rstrides, &);
        } 
    } else if(lhs->dtype == NDARRAY_INT8) {
        if(rhs->dtype == NDARRAY_INT8) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_INT8);
            BINARY_LOOP(results, int8_t, int8_t, int8_t, larray, lstrides, rarray, rstrides, &);
        } else if(rhs->dtype == NDARRAY_UINT16) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_UINT16);
            BINARY_LOOP(results, uint16_t, int8_t, uint16_t, larray, lstrides, rarray, rstrides, &);
        } else if(rhs->dtype == NDARRAY_INT16) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_INT16);
            BINARY_LOOP(results, int16_t, int8_t, int16_t, larray, lstrides, rarray, rstrides, &);
        } else {
            return bitwise_bitwise_and_loop(rhs, lhs, ndim, shape, rstrides, lstrides);
        }
    } else if(lhs->dtype == NDARRAY_UINT16) {
        if(rhs->dtype == NDARRAY_UINT16) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_UINT16);
            BINARY_LOOP(results, uint16_t, uint16_t, uint16_t, larray, lstrides, rarray, rstrides, &);
        } else if(rhs->dtype == NDARRAY_INT16) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_INT16);
            BINARY_LOOP(results, int16_t, uint16_t, int16_t, larray, lstrides, rarray, rstrides, &);
        } else {
            return bitwise_bitwise_and_loop(rhs, lhs, ndim, shape, rstrides, lstrides);
        }
    } else if(lhs->dtype == NDARRAY_INT16) {
        if(rhs->dtype == NDARRAY_INT16) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_INT16);
            BINARY_LOOP(results, int16_t, int16_t, int16_t, larray, lstrides, rarray, rstrides, &);
        } else {
            return bitwise_bitwise_and_loop(rhs, lhs, ndim, shape, rstrides, lstrides);
        }
    }
    return results;
}
#endif

#if ULAB_NUMPY_HAS_BITWISE_OR
ndarray_obj_t *bitwise_bitwise_or_loop(ndarray_obj_t *lhs, ndarray_obj_t *rhs, uint8_t ndim, size_t *shape, int32_t *lstrides, int32_t *rstrides) {
    // OR is commutative, so simply swap the order, if a particular combination has already been inspected

    ndarray_obj_t *results = NULL;
    uint8_t *larray = (uint8_t *)lhs->array;
    uint8_t *rarray = (uint8_t *)rhs->array;

    if(lhs->dtype == NDARRAY_UINT8) {
        if(rhs->dtype == NDARRAY_UINT8) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_UINT8);
            BINARY_LOOP(results, uint8_t, uint8_t, uint8_t, larray, lstrides, rarray, rstrides, |);
        } else if(rhs->dtype == NDARRAY_INT8) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_INT16);
            BINARY_LOOP(results, int16_t, uint8_t, int8_t, larray, lstrides, rarray, rstrides, |);
        } else if(rhs->dtype == NDARRAY_UINT16) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_UINT16);
            BINARY_LOOP(results, uint16_t, uint8_t, uint16_t, larray, lstrides, rarray, rstrides, |);
        } else if(rhs->dtype == NDARRAY_INT16) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_INT16);
            BINARY_LOOP(results, int16_t, uint8_t, int16_t, larray, lstrides, rarray, rstrides, |);
        } 
    } else if(lhs->dtype == NDARRAY_INT8) {
        if(rhs->dtype == NDARRAY_INT8) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_INT8);
            BINARY_LOOP(results, int8_t, int8_t, int8_t, larray, lstrides, rarray, rstrides, |);
        } else if(rhs->dtype == NDARRAY_UINT16) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_UINT16);
            BINARY_LOOP(results, uint16_t, int8_t, uint16_t, larray, lstrides, rarray, rstrides, |);
        } else if(rhs->dtype == NDARRAY_INT16) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_INT16);
            BINARY_LOOP(results, int16_t, int8_t, int16_t, larray, lstrides, rarray, rstrides, |);
        } else {
            return bitwise_bitwise_or_loop(rhs, lhs, ndim, shape, rstrides, lstrides);
        }
    } else if(lhs->dtype == NDARRAY_UINT16) {
        if(rhs->dtype == NDARRAY_UINT16) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_UINT16);
            BINARY_LOOP(results, uint16_t, uint16_t, uint16_t, larray, lstrides, rarray, rstrides, |);
        } else if(rhs->dtype == NDARRAY_INT16) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_INT16);
            BINARY_LOOP(results, int16_t, uint16_t, int16_t, larray, lstrides, rarray, rstrides, |);
        } else {
            return bitwise_bitwise_or_loop(rhs, lhs, ndim, shape, rstrides, lstrides);
        }
    } else if(lhs->dtype == NDARRAY_INT16) {
        if(rhs->dtype == NDARRAY_INT16) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_INT16);
            BINARY_LOOP(results, int16_t, int16_t, int16_t, larray, lstrides, rarray, rstrides, |);
        } else {
            return bitwise_bitwise_or_loop(rhs, lhs, ndim, shape, rstrides, lstrides);
        }
    }
    return results;
}
#endif


#if ULAB_NUMPY_HAS_BITWISE_XOR
ndarray_obj_t *bitwise_bitwise_xor_loop(ndarray_obj_t *lhs, ndarray_obj_t *rhs, uint8_t ndim, size_t *shape, int32_t *lstrides, int32_t *rstrides) {
    // OR is commutative, so simply swap the order, if a particular combination has already been inspected

    ndarray_obj_t *results = NULL;
    uint8_t *larray = (uint8_t *)lhs->array;
    uint8_t *rarray = (uint8_t *)rhs->array;

    if(lhs->dtype == NDARRAY_UINT8) {
        if(rhs->dtype == NDARRAY_UINT8) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_UINT8);
            BINARY_LOOP(results, uint8_t, uint8_t, uint8_t, larray, lstrides, rarray, rstrides, ^);
        } else if(rhs->dtype == NDARRAY_INT8) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_INT16);
            BINARY_LOOP(results, int16_t, uint8_t, int8_t, larray, lstrides, rarray, rstrides, ^);
        } else if(rhs->dtype == NDARRAY_UINT16) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_UINT16);
            BINARY_LOOP(results, uint16_t, uint8_t, uint16_t, larray, lstrides, rarray, rstrides, ^);
        } else if(rhs->dtype == NDARRAY_INT16) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_INT16);
            BINARY_LOOP(results, int16_t, uint8_t, int16_t, larray, lstrides, rarray, rstrides, ^);
        } 
    } else if(lhs->dtype == NDARRAY_INT8) {
        if(rhs->dtype == NDARRAY_INT8) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_INT8);
            BINARY_LOOP(results, int8_t, int8_t, int8_t, larray, lstrides, rarray, rstrides, ^);
        } else if(rhs->dtype == NDARRAY_UINT16) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_UINT16);
            BINARY_LOOP(results, uint16_t, int8_t, uint16_t, larray, lstrides, rarray, rstrides, ^);
        } else if(rhs->dtype == NDARRAY_INT16) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_INT16);
            BINARY_LOOP(results, int16_t, int8_t, int16_t, larray, lstrides, rarray, rstrides, ^);
        } else {
            return bitwise_bitwise_xor_loop(rhs, lhs, ndim, shape, rstrides, lstrides);
        }
    } else if(lhs->dtype == NDARRAY_UINT16) {
        if(rhs->dtype == NDARRAY_UINT16) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_UINT16);
            BINARY_LOOP(results, uint16_t, uint16_t, uint16_t, larray, lstrides, rarray, rstrides, ^);
        } else if(rhs->dtype == NDARRAY_INT16) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_INT16);
            BINARY_LOOP(results, int16_t, uint16_t, int16_t, larray, lstrides, rarray, rstrides, ^);
        } else {
            return bitwise_bitwise_xor_loop(rhs, lhs, ndim, shape, rstrides, lstrides);
        }
    } else if(lhs->dtype == NDARRAY_INT16) {
        if(rhs->dtype == NDARRAY_INT16) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_INT16);
            BINARY_LOOP(results, int16_t, int16_t, int16_t, larray, lstrides, rarray, rstrides, ^);
        } else {
            return bitwise_bitwise_xor_loop(rhs, lhs, ndim, shape, rstrides, lstrides);
        }
    }
    return results;
}
#endif

#if ULAB_NUMPY_HAS_LEFT_SHIFT
ndarray_obj_t *bitwise_left_shift_loop(ndarray_obj_t *lhs, ndarray_obj_t *rhs, uint8_t ndim, size_t *shape, int32_t *lstrides, int32_t *rstrides) {
    ndarray_obj_t *results = NULL;
    uint8_t *larray = (uint8_t *)lhs->array;
    uint8_t *rarray = (uint8_t *)rhs->array;

    if(lhs->dtype == NDARRAY_UINT8) {
        if(rhs->dtype == NDARRAY_UINT8) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_UINT8);
            BINARY_LOOP(results, uint8_t, uint8_t, uint8_t, larray, lstrides, rarray, rstrides, <<);
        } else if(rhs->dtype == NDARRAY_INT8) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_INT16);
            BINARY_LOOP(results, int16_t, uint8_t, int8_t, larray, lstrides, rarray, rstrides, <<);
        } else if(rhs->dtype == NDARRAY_UINT16) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_UINT16);
            BINARY_LOOP(results, uint16_t, uint8_t, uint16_t, larray, lstrides, rarray, rstrides, <<);
        } else {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_INT16);
            BINARY_LOOP(results, int16_t, uint8_t, int16_t, larray, lstrides, rarray, rstrides, <<);
        } 
    } else if(lhs->dtype == NDARRAY_INT8) {
        if(rhs->dtype == NDARRAY_UINT8) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_INT16);
            BINARY_LOOP(results, int16_t, int8_t, uint8_t, larray, lstrides, rarray, rstrides, <<);
        } else if(rhs->dtype == NDARRAY_INT8) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_INT8);
            BINARY_LOOP(results, int8_t, int8_t, int8_t, larray, lstrides, rarray, rstrides, <<);
        } else if(rhs->dtype == NDARRAY_UINT16) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_UINT16);
            BINARY_LOOP(results, uint16_t, int8_t, uint16_t, larray, lstrides, rarray, rstrides, <<);
        } else {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_INT16);
            BINARY_LOOP(results, int16_t, int8_t, int16_t, larray, lstrides, rarray, rstrides, <<);
        }
    } else if(lhs->dtype == NDARRAY_UINT16) {
        if(rhs->dtype == NDARRAY_UINT8) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_UINT16);
            BINARY_LOOP(results, uint16_t, uint16_t, uint8_t, larray, lstrides, rarray, rstrides, <<);
        } else if(rhs->dtype == NDARRAY_INT8) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_INT8);
            BINARY_LOOP(results, int8_t, int8_t, int8_t, larray, lstrides, rarray, rstrides, <<);
        } else if(rhs->dtype == NDARRAY_UINT16) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_UINT16);
            BINARY_LOOP(results, uint16_t, uint16_t, uint16_t, larray, lstrides, rarray, rstrides, <<);
        } else {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_INT16);
            BINARY_LOOP(results, int16_t, uint16_t, int16_t, larray, lstrides, rarray, rstrides, <<);
        }
    } else if(lhs->dtype == NDARRAY_INT16) {
        if(rhs->dtype == NDARRAY_UINT8) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_INT16);
            BINARY_LOOP(results, int16_t, int16_t, uint8_t, larray, lstrides, rarray, rstrides, <<);
        } else if(rhs->dtype == NDARRAY_INT8) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_INT16);
            BINARY_LOOP(results, int16_t, int16_t, int8_t, larray, lstrides, rarray, rstrides, <<);
        } else if(rhs->dtype == NDARRAY_UINT16) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_INT16);
            BINARY_LOOP(results, uint16_t, uint16_t, int16_t, larray, lstrides, rarray, rstrides, <<);
        } else {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_INT16);
            BINARY_LOOP(results, int16_t, int16_t, int16_t, larray, lstrides, rarray, rstrides, <<);
        }
    }
    return results;
}
#endif

#if ULAB_NUMPY_HAS_RIGHT_SHIFT
ndarray_obj_t *bitwise_right_shift_loop(ndarray_obj_t *lhs, ndarray_obj_t *rhs, uint8_t ndim, size_t *shape, int32_t *lstrides, int32_t *rstrides) {
    ndarray_obj_t *results = NULL;
    uint8_t *larray = (uint8_t *)lhs->array;
    uint8_t *rarray = (uint8_t *)rhs->array;

    if(lhs->dtype == NDARRAY_UINT8) {
        if(rhs->dtype == NDARRAY_UINT8) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_UINT8);
            BINARY_LOOP(results, uint8_t, uint8_t, uint8_t, larray, lstrides, rarray, rstrides, >>);
        } else if(rhs->dtype == NDARRAY_INT8) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_INT16);
            BINARY_LOOP(results, int16_t, uint8_t, int8_t, larray, lstrides, rarray, rstrides, >>);
        } else if(rhs->dtype == NDARRAY_UINT16) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_UINT16);
            BINARY_LOOP(results, uint16_t, uint8_t, uint16_t, larray, lstrides, rarray, rstrides, >>);
        } else {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_INT16);
            BINARY_LOOP(results, int16_t, uint8_t, int16_t, larray, lstrides, rarray, rstrides, >>);
        } 
    } else if(lhs->dtype == NDARRAY_INT8) {
        if(rhs->dtype == NDARRAY_UINT8) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_INT16);
            BINARY_LOOP(results, int16_t, int8_t, uint8_t, larray, lstrides, rarray, rstrides, >>);
        } else if(rhs->dtype == NDARRAY_INT8) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_INT8);
            BINARY_LOOP(results, int8_t, int8_t, int8_t, larray, lstrides, rarray, rstrides, >>);
        } else if(rhs->dtype == NDARRAY_UINT16) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_UINT16);
            BINARY_LOOP(results, uint16_t, int8_t, uint16_t, larray, lstrides, rarray, rstrides, >>);
        } else {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_INT16);
            BINARY_LOOP(results, int16_t, int8_t, int16_t, larray, lstrides, rarray, rstrides, >>);
        }
    } else if(lhs->dtype == NDARRAY_UINT16) {
        if(rhs->dtype == NDARRAY_UINT8) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_UINT16);
            BINARY_LOOP(results, uint16_t, uint16_t, uint8_t, larray, lstrides, rarray, rstrides, >>);
        } else if(rhs->dtype == NDARRAY_INT8) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_INT8);
            BINARY_LOOP(results, int8_t, int8_t, int8_t, larray, lstrides, rarray, rstrides, >>);
        } else if(rhs->dtype == NDARRAY_UINT16) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_UINT16);
            BINARY_LOOP(results, uint16_t, uint16_t, uint16_t, larray, lstrides, rarray, rstrides, >>);
        } else {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_INT16);
            BINARY_LOOP(results, int16_t, uint16_t, int16_t, larray, lstrides, rarray, rstrides, >>);
        }
    } else if(lhs->dtype == NDARRAY_INT16) {
        if(rhs->dtype == NDARRAY_UINT8) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_INT16);
            BINARY_LOOP(results, int16_t, int16_t, uint8_t, larray, lstrides, rarray, rstrides, >>);
        } else if(rhs->dtype == NDARRAY_INT8) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_INT16);
            BINARY_LOOP(results, int16_t, int16_t, int8_t, larray, lstrides, rarray, rstrides, >>);
        } else if(rhs->dtype == NDARRAY_UINT16) {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_INT16);
            BINARY_LOOP(results, uint16_t, uint16_t, int16_t, larray, lstrides, rarray, rstrides, >>);
        } else {
            results = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_INT16);
            BINARY_LOOP(results, int16_t, int16_t, int16_t, larray, lstrides, rarray, rstrides, >>);
        }
    }
    return results;
}
#endif

mp_obj_t *bitwise_binary_operators(mp_obj_t x1, mp_obj_t x2, uint8_t optype) {
    ndarray_obj_t *lhs = ndarray_from_mp_obj(x1, 0);
    ndarray_obj_t *rhs = ndarray_from_mp_obj(x2, 0);
    
    #if ULAB_SUPPORTS_COMPLEX
    if((lhs->dtype == NDARRAY_FLOAT) || (rhs->dtype == NDARRAY_FLOAT) || (lhs->dtype == NDARRAY_COMPLEX) || (rhs->dtype == NDARRAY_COMPLEX)) {
        mp_raise_ValueError(MP_ERROR_TEXT("not supported for input types"));
    }
    #else
    if((lhs->dtype == NDARRAY_FLOAT) || (rhs->dtype == NDARRAY_FLOAT)) {
        mp_raise_ValueError(MP_ERROR_TEXT("not supported for input types"));
    }
    #endif
    
    uint8_t ndim = 0;
    size_t *shape = m_new(size_t, ULAB_MAX_DIMS);
    int32_t *lstrides = m_new0(int32_t, ULAB_MAX_DIMS);
    int32_t *rstrides = m_new0(int32_t, ULAB_MAX_DIMS);
    
    if(!ndarray_can_broadcast(lhs, rhs, &ndim, shape, lstrides, rstrides)) {
        m_del(size_t, shape, ULAB_MAX_DIMS);
        m_del(int32_t, lstrides, ULAB_MAX_DIMS);
        m_del(int32_t, rstrides, ULAB_MAX_DIMS);
        mp_raise_ValueError(MP_ERROR_TEXT("operands could not be broadcast together"));
    }

    ndarray_obj_t *results = NULL;
    
    switch(optype) {
        #if ULAB_NUMPY_HAS_BITWISE_AND
        case BITWISE_AND:
            results = bitwise_bitwise_and_loop(lhs, rhs, ndim, shape, lstrides, rstrides);
            break;
        #endif
        #if ULAB_NUMPY_HAS_BITWISE_OR
        case BITWISE_OR:
            results = bitwise_bitwise_or_loop(lhs, rhs, ndim, shape, lstrides, rstrides);
            break;
        #endif
        #if ULAB_NUMPY_HAS_BITWISE_XOR
        case BITWISE_XOR:
            results = bitwise_bitwise_xor_loop(lhs, rhs, ndim, shape, lstrides, rstrides);
            break;
        #endif
        #if ULAB_NUMPY_HAS_LEFT_SHIFT
        case BITWISE_LEFT_SHIFT:
            results = bitwise_left_shift_loop(lhs, rhs, ndim, shape, lstrides, rstrides);
            break;
        #endif
        #if ULAB_NUMPY_HAS_RIGHT_SHIFT
        case BITWISE_RIGHT_SHIFT:
            results = bitwise_right_shift_loop(lhs, rhs, ndim, shape, lstrides, rstrides);
            break;
        #endif
        default:
            break; 
    }

    m_del(size_t, shape, ULAB_MAX_DIMS);
    m_del(int32_t, lstrides, ULAB_MAX_DIMS);
    m_del(int32_t, rstrides, ULAB_MAX_DIMS);

    return MP_OBJ_FROM_PTR(results);
}

#if ULAB_NUMPY_HAS_BITWISE_AND
mp_obj_t bitwise_bitwise_and(mp_obj_t x1, mp_obj_t x2) {
    return bitwise_binary_operators(x1, x2, BITWISE_AND);
}

MP_DEFINE_CONST_FUN_OBJ_2(bitwise_bitwise_and_obj, bitwise_bitwise_and);
#endif

#if ULAB_NUMPY_HAS_BITWISE_OR
mp_obj_t bitwise_bitwise_or(mp_obj_t x1, mp_obj_t x2) {
    return bitwise_binary_operators(x1, x2, BITWISE_OR);
}

MP_DEFINE_CONST_FUN_OBJ_2(bitwise_bitwise_or_obj, bitwise_bitwise_or);
#endif

#if ULAB_NUMPY_HAS_BITWISE_XOR
mp_obj_t bitwise_bitwise_xor(mp_obj_t x1, mp_obj_t x2) {
    return bitwise_binary_operators(x1, x2, BITWISE_XOR);
}

MP_DEFINE_CONST_FUN_OBJ_2(bitwise_bitwise_xor_obj, bitwise_bitwise_xor);
#endif

#if ULAB_NUMPY_HAS_LEFT_SHIFT
mp_obj_t bitwise_left_shift(mp_obj_t x1, mp_obj_t x2) {
    return bitwise_binary_operators(x1, x2, BITWISE_LEFT_SHIFT);
}

MP_DEFINE_CONST_FUN_OBJ_2(left_shift_obj, bitwise_left_shift);
#endif

#if ULAB_NUMPY_HAS_RIGHT_SHIFT
mp_obj_t bitwise_right_shift(mp_obj_t x1, mp_obj_t x2) {
    return bitwise_binary_operators(x1, x2, BITWISE_RIGHT_SHIFT);
}

MP_DEFINE_CONST_FUN_OBJ_2(right_shift_obj, bitwise_right_shift);
#endif