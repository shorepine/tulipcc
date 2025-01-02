
/*
 * This file is part of the micropython-ulab project,
 *
 * https://github.com/v923z/micropython-ulab
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2019-2021 Zoltán Vörös
 *               2020 Jeff Epler for Adafruit Industries
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "py/runtime.h"
#include "py/binary.h"
#include "py/obj.h"
#include "py/objarray.h"

#include "ulab.h"
#include "ndarray.h"
#include "ndarray_properties.h"
#include "numpy/create.h"
#include "numpy/ndarray/ndarray_iter.h"

#include "numpy/numpy.h"
#include "scipy/scipy.h"
// TODO: we should get rid of this; array.sort depends on it
#include "numpy/numerical.h"

#include "user/user.h"
#include "utils/utils.h"

#define ULAB_VERSION 6.7.0
#define xstr(s) str(s)
#define str(s) #s

#if ULAB_SUPPORTS_COMPLEX
#define ULAB_VERSION_STRING xstr(ULAB_VERSION) xstr(-) xstr(ULAB_MAX_DIMS) xstr(D-c)
#else
#define ULAB_VERSION_STRING xstr(ULAB_VERSION) xstr(-) xstr(ULAB_MAX_DIMS) xstr(D)
#endif

static MP_DEFINE_STR_OBJ(ulab_version_obj, ULAB_VERSION_STRING);

#ifdef ULAB_HASH
static MP_DEFINE_STR_OBJ(ulab_sha_obj, xstr(ULAB_HASH));
#endif

static const mp_rom_map_elem_t ulab_ndarray_locals_dict_table[] = {
    #if ULAB_MAX_DIMS > 1
        #if NDARRAY_HAS_RESHAPE
            { MP_ROM_QSTR(MP_QSTR_reshape), MP_ROM_PTR(&ndarray_reshape_obj) },
        #endif
        #if NDARRAY_HAS_TRANSPOSE
            { MP_ROM_QSTR(MP_QSTR_transpose), MP_ROM_PTR(&ndarray_transpose_obj) },
        #endif
    #endif
    #if NDARRAY_HAS_BYTESWAP
        { MP_ROM_QSTR(MP_QSTR_byteswap), MP_ROM_PTR(&ndarray_byteswap_obj) },
    #endif
    #if NDARRAY_HAS_COPY
        { MP_ROM_QSTR(MP_QSTR_copy), MP_ROM_PTR(&ndarray_copy_obj) },
    #endif
    #if NDARRAY_HAS_FLATTEN
        { MP_ROM_QSTR(MP_QSTR_flatten), MP_ROM_PTR(&ndarray_flatten_obj) },
    #endif
    #if NDARRAY_HAS_TOBYTES
        { MP_ROM_QSTR(MP_QSTR_tobytes), MP_ROM_PTR(&ndarray_tobytes_obj) },
    #endif
    #if NDARRAY_HAS_TOLIST
        { MP_ROM_QSTR(MP_QSTR_tolist), MP_ROM_PTR(&ndarray_tolist_obj) },
    #endif
    #if NDARRAY_HAS_SORT
        { MP_ROM_QSTR(MP_QSTR_sort), MP_ROM_PTR(&numerical_sort_inplace_obj) },
    #endif
};

static MP_DEFINE_CONST_DICT(ulab_ndarray_locals_dict, ulab_ndarray_locals_dict_table);

#if defined(MP_DEFINE_CONST_OBJ_TYPE)
// MicroPython after-b41aaaa (Sept 19 2022).

#if NDARRAY_IS_SLICEABLE
#define NDARRAY_TYPE_SUBSCR subscr, ndarray_subscr,
#else
#define NDARRAY_TYPE_SUBSCR
#endif
#if NDARRAY_IS_ITERABLE
#define NDARRAY_TYPE_ITER iter, ndarray_getiter,
#define NDARRAY_TYPE_ITER_FLAGS MP_TYPE_FLAG_ITER_IS_GETITER
#else
#define NDARRAY_TYPE_ITER
#define NDARRAY_TYPE_ITER_FLAGS 0
#endif
#if NDARRAY_HAS_UNARY_OPS
#define NDARRAY_TYPE_UNARY_OP unary_op, ndarray_unary_op,
#else
#define NDARRAY_TYPE_UNARY_OP
#endif
#if NDARRAY_HAS_BINARY_OPS
#define NDARRAY_TYPE_BINARY_OP binary_op, ndarray_binary_op,
#else
#define NDARRAY_TYPE_BINARY_OP
#endif

MP_DEFINE_CONST_OBJ_TYPE(
    ulab_ndarray_type,
    MP_QSTR_ndarray,
    MP_TYPE_FLAG_EQ_CHECKS_OTHER_TYPE | MP_TYPE_FLAG_EQ_HAS_NEQ_TEST | NDARRAY_TYPE_ITER_FLAGS,
    print, ndarray_print,
    make_new, ndarray_make_new,
    locals_dict, &ulab_ndarray_locals_dict,
    NDARRAY_TYPE_SUBSCR
    NDARRAY_TYPE_ITER
    NDARRAY_TYPE_UNARY_OP
    NDARRAY_TYPE_BINARY_OP
    attr, ndarray_properties_attr,
    buffer, ndarray_get_buffer
);

#else
// CircuitPython and earlier MicroPython revisions.
const mp_obj_type_t ulab_ndarray_type = {
    { &mp_type_type },
    .flags = MP_TYPE_FLAG_EXTENDED
    #if defined(MP_TYPE_FLAG_EQ_CHECKS_OTHER_TYPE) && defined(MP_TYPE_FLAG_EQ_HAS_NEQ_TEST)
        | MP_TYPE_FLAG_EQ_CHECKS_OTHER_TYPE | MP_TYPE_FLAG_EQ_HAS_NEQ_TEST
    #endif
        ,
    .name = MP_QSTR_ndarray,
    .print = ndarray_print,
    .make_new = ndarray_make_new,
    .locals_dict = (mp_obj_dict_t*)&ulab_ndarray_locals_dict,
    MP_TYPE_EXTENDED_FIELDS(
    #if NDARRAY_IS_SLICEABLE
    .subscr = ndarray_subscr,
    #endif
    #if NDARRAY_IS_ITERABLE
    .getiter = ndarray_getiter,
    #endif
    #if NDARRAY_HAS_UNARY_OPS
    .unary_op = ndarray_unary_op,
    #endif
    #if NDARRAY_HAS_BINARY_OPS
    .binary_op = ndarray_binary_op,
    #endif
    .attr = ndarray_properties_attr,
    .buffer_p = { .get_buffer = ndarray_get_buffer, },
    )
};
#endif

#if ULAB_HAS_DTYPE_OBJECT

#if defined(MP_DEFINE_CONST_OBJ_TYPE)
MP_DEFINE_CONST_OBJ_TYPE(
    ulab_dtype_type,
    MP_QSTR_dtype,
    MP_TYPE_FLAG_NONE,
    print, ndarray_dtype_print,
    make_new, ndarray_dtype_make_new
);
#else
const mp_obj_type_t ulab_dtype_type = {
    { &mp_type_type },
    .name = MP_QSTR_dtype,
    .print = ndarray_dtype_print,
    .make_new = ndarray_dtype_make_new,
};
#endif
#endif

#if NDARRAY_HAS_FLATITER
#if defined(MP_DEFINE_CONST_OBJ_TYPE)
MP_DEFINE_CONST_OBJ_TYPE(
    ndarray_flatiter_type,
    MP_QSTR_flatiter,
    MP_TYPE_FLAG_ITER_IS_GETITER,
    iter, ndarray_get_flatiterator
);
#else
const mp_obj_type_t ndarray_flatiter_type = {
    { &mp_type_type },
    .name = MP_QSTR_flatiter,
    MP_TYPE_EXTENDED_FIELDS(
    .getiter = ndarray_get_flatiterator,
    )
};
#endif
#endif

static const mp_rom_map_elem_t ulab_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_ulab) },
    { MP_ROM_QSTR(MP_QSTR___version__), MP_ROM_PTR(&ulab_version_obj) },
    #ifdef ULAB_HASH
    { MP_ROM_QSTR(MP_QSTR___sha__), MP_ROM_PTR(&ulab_sha_obj) },
    #endif
    #if ULAB_HAS_DTYPE_OBJECT
        { MP_ROM_QSTR(MP_QSTR_dtype), MP_ROM_PTR(&ulab_dtype_type) },
    #else
        #if NDARRAY_HAS_DTYPE
        { MP_ROM_QSTR(MP_QSTR_dtype), MP_ROM_PTR(&ndarray_dtype_obj) },
        #endif /* NDARRAY_HAS_DTYPE */
    #endif /* ULAB_HAS_DTYPE_OBJECT */
        { MP_ROM_QSTR(MP_QSTR_numpy), MP_ROM_PTR(&ulab_numpy_module) },
    #if ULAB_HAS_SCIPY
        { MP_ROM_QSTR(MP_QSTR_scipy), MP_ROM_PTR(&ulab_scipy_module) },
    #endif
    #if ULAB_HAS_USER_MODULE
        { MP_ROM_QSTR(MP_QSTR_user), MP_ROM_PTR(&ulab_user_module) },
    #endif
    #if ULAB_HAS_UTILS_MODULE
        { MP_ROM_QSTR(MP_QSTR_utils), MP_ROM_PTR(&ulab_utils_module) },
    #endif
};

static MP_DEFINE_CONST_DICT (
    mp_module_ulab_globals,
    ulab_globals_table
);

#ifdef OPENMV
const struct _mp_obj_module_t ulab_user_cmodule = {
#else
const mp_obj_module_t ulab_user_cmodule = {
#endif
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_module_ulab_globals,
};

MP_REGISTER_MODULE(MP_QSTR_ulab, ulab_user_cmodule);
