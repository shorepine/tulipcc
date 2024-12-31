
/*
 * This file is part of the micropython-ulab project,
 *
 * https://github.com/v923z/micropython-ulab
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 Jeff Epler for Adafruit Industries
 *               2020-2021 Zoltán Vörös
*/

#ifndef _NDARRAY_PROPERTIES_
#define _NDARRAY_PROPERTIES_

#include "py/runtime.h"
#include "py/binary.h"
#include "py/obj.h"
#include "py/objarray.h"

#include "ulab.h"
#include "ndarray.h"
#include "numpy/ndarray/ndarray_iter.h"

void ndarray_properties_attr(mp_obj_t , qstr , mp_obj_t *);

#if NDARRAY_HAS_DTYPE
MP_DEFINE_CONST_FUN_OBJ_1(ndarray_dtype_obj, ndarray_dtype);
#endif

#if NDARRAY_HAS_FLATITER
MP_DEFINE_CONST_FUN_OBJ_1(ndarray_flatiter_make_new_obj, ndarray_flatiter_make_new);
#endif

#if NDARRAY_HAS_ITEMSIZE
MP_DEFINE_CONST_FUN_OBJ_1(ndarray_itemsize_obj, ndarray_itemsize);
#endif

#if NDARRAY_HAS_SHAPE
MP_DEFINE_CONST_FUN_OBJ_1(ndarray_shape_obj, ndarray_shape);
#endif

#if NDARRAY_HAS_SIZE
MP_DEFINE_CONST_FUN_OBJ_1(ndarray_size_obj, ndarray_size);
#endif

#if NDARRAY_HAS_STRIDES
MP_DEFINE_CONST_FUN_OBJ_1(ndarray_strides_obj, ndarray_strides);
#endif

#endif
