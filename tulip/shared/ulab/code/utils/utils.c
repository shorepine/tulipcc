/*
 * This file is part of the micropython-ulab project,
 *
 * https://github.com/v923z/micropython-ulab
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2020-2024 Zoltán Vörös
*/

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "py/obj.h"
#include "py/runtime.h"
#include "py/misc.h"
#include "utils.h"

#include "../ulab_tools.h"
#include "../numpy/fft/fft_tools.h"

#if ULAB_HAS_UTILS_MODULE

enum UTILS_BUFFER_TYPE {
    UTILS_INT16_BUFFER,
    UTILS_UINT16_BUFFER,
    UTILS_INT32_BUFFER,
    UTILS_UINT32_BUFFER,
};

#if ULAB_UTILS_HAS_FROM_INT16_BUFFER | ULAB_UTILS_HAS_FROM_UINT16_BUFFER | ULAB_UTILS_HAS_FROM_INT32_BUFFER | ULAB_UTILS_HAS_FROM_UINT32_BUFFER
static mp_obj_t utils_from_intbuffer_helper(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args, uint8_t buffer_type) {
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_rom_obj = MP_ROM_NONE } } ,
        { MP_QSTR_count, MP_ARG_KW_ONLY | MP_ARG_OBJ, { .u_rom_obj = MP_ROM_INT(-1) } },
        { MP_QSTR_offset, MP_ARG_KW_ONLY | MP_ARG_OBJ, { .u_rom_obj = MP_ROM_INT(0) } },
        { MP_QSTR_out, MP_ARG_OBJ, { .u_rom_obj = MP_ROM_NONE } },
        { MP_QSTR_byteswap, MP_ARG_OBJ, { .u_rom_obj = MP_ROM_FALSE } },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    ndarray_obj_t *ndarray = NULL;

    if(args[3].u_obj != mp_const_none) {
        ndarray = MP_OBJ_TO_PTR(args[3].u_obj);
        if((ndarray->dtype != NDARRAY_FLOAT) || !ndarray_is_dense(ndarray)) {
            mp_raise_TypeError(MP_ERROR_TEXT("out must be a float dense array"));
        }
    }

    size_t offset = mp_obj_get_int(args[2].u_obj);

    mp_buffer_info_t bufinfo;
    if(mp_get_buffer(args[0].u_obj, &bufinfo, MP_BUFFER_READ)) {
        if(bufinfo.len < offset) {
            mp_raise_ValueError(MP_ERROR_TEXT("offset is too large"));
        }
        uint8_t sz = sizeof(int16_t);
        #if ULAB_UTILS_HAS_FROM_INT32_BUFFER | ULAB_UTILS_HAS_FROM_UINT32_BUFFER
        if((buffer_type == UTILS_INT32_BUFFER) ||  (buffer_type == UTILS_UINT32_BUFFER)) {
            sz = sizeof(int32_t);
        }
        #endif

        size_t len = (bufinfo.len - offset) / sz;
        if((len * sz) != (bufinfo.len - offset)) {
            mp_raise_ValueError(MP_ERROR_TEXT("buffer size must be a multiple of element size"));
        }
        if(mp_obj_get_int(args[1].u_obj) > 0) {
            size_t count = mp_obj_get_int(args[1].u_obj);
            if(len < count) {
                mp_raise_ValueError(MP_ERROR_TEXT("buffer is smaller than requested size"));
            } else {
                len = count;
            }
        }
        if(args[3].u_obj == mp_const_none) {
            ndarray = ndarray_new_linear_array(len, NDARRAY_FLOAT);
        } else {
            if(ndarray->len < len) {
                mp_raise_ValueError(MP_ERROR_TEXT("out array is too small"));
            }
        }
        uint8_t *buffer = bufinfo.buf;

        mp_float_t *array = (mp_float_t *)ndarray->array;
        if(args[4].u_obj == mp_const_true) {
            // swap the bytes before conversion
            uint8_t *tmpbuff = m_new(uint8_t, sz);
            #if ULAB_UTILS_HAS_FROM_INT16_BUFFER | ULAB_UTILS_HAS_FROM_UINT16_BUFFER
            if((buffer_type == UTILS_INT16_BUFFER) || (buffer_type == UTILS_UINT16_BUFFER)) {
                for(size_t i = 0; i < len; i++) {
                    tmpbuff += sz;
                    for(uint8_t j = 0; j < sz; j++) {
                        memcpy(--tmpbuff, buffer++, 1);
                    }
                    if(buffer_type == UTILS_INT16_BUFFER) {
                        *array++ = (mp_float_t)(*(int16_t *)tmpbuff);
                    } else {
                        *array++ = (mp_float_t)(*(uint16_t *)tmpbuff);
                    }
                }
            }
            #endif
            #if ULAB_UTILS_HAS_FROM_INT32_BUFFER | ULAB_UTILS_HAS_FROM_UINT32_BUFFER
            if((buffer_type == UTILS_INT32_BUFFER) || (buffer_type == UTILS_UINT32_BUFFER)) {
                for(size_t i = 0; i < len; i++) {
                    tmpbuff += sz;
                    for(uint8_t j = 0; j < sz; j++) {
                        memcpy(--tmpbuff, buffer++, 1);
                    }
                    if(buffer_type == UTILS_INT32_BUFFER) {
                        *array++ = (mp_float_t)(*(int32_t *)tmpbuff);
                    } else {
                        *array++ = (mp_float_t)(*(uint32_t *)tmpbuff);
                    }
                }
            }
            #endif
        } else {
            #if ULAB_UTILS_HAS_FROM_INT16_BUFFER
            if(buffer_type == UTILS_INT16_BUFFER) {
                for(size_t i = 0; i < len; i++) {
                    *array++ = (mp_float_t)(*(int16_t *)buffer);
                    buffer += sz;
                }
            }
            #endif
            #if ULAB_UTILS_HAS_FROM_UINT16_BUFFER
            if(buffer_type == UTILS_UINT16_BUFFER) {
                for(size_t i = 0; i < len; i++) {
                    *array++ = (mp_float_t)(*(uint16_t *)buffer);
                    buffer += sz;
                }
            }
            #endif
            #if ULAB_UTILS_HAS_FROM_INT32_BUFFER
            if(buffer_type == UTILS_INT32_BUFFER) {
                for(size_t i = 0; i < len; i++) {
                    *array++ = (mp_float_t)(*(int32_t *)buffer);
                    buffer += sz;
                }
            }
            #endif
            #if ULAB_UTILS_HAS_FROM_UINT32_BUFFER
            if(buffer_type == UTILS_UINT32_BUFFER) {
                for(size_t i = 0; i < len; i++) {
                    *array++ = (mp_float_t)(*(uint32_t *)buffer);
                    buffer += sz;
                }
            }
            #endif
        }
        return MP_OBJ_FROM_PTR(ndarray);
    }
    return mp_const_none;
}

#ifdef ULAB_UTILS_HAS_FROM_INT16_BUFFER
static mp_obj_t utils_from_int16_buffer(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    return utils_from_intbuffer_helper(n_args, pos_args, kw_args, UTILS_INT16_BUFFER);
}

MP_DEFINE_CONST_FUN_OBJ_KW(utils_from_int16_buffer_obj, 1, utils_from_int16_buffer);
#endif

#ifdef ULAB_UTILS_HAS_FROM_UINT16_BUFFER
static mp_obj_t utils_from_uint16_buffer(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    return utils_from_intbuffer_helper(n_args, pos_args, kw_args, UTILS_UINT16_BUFFER);
}

MP_DEFINE_CONST_FUN_OBJ_KW(utils_from_uint16_buffer_obj, 1, utils_from_uint16_buffer);
#endif

#ifdef ULAB_UTILS_HAS_FROM_INT32_BUFFER
static mp_obj_t utils_from_int32_buffer(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    return utils_from_intbuffer_helper(n_args, pos_args, kw_args, UTILS_INT32_BUFFER);
}

MP_DEFINE_CONST_FUN_OBJ_KW(utils_from_int32_buffer_obj, 1, utils_from_int32_buffer);
#endif

#ifdef ULAB_UTILS_HAS_FROM_UINT32_BUFFER
static mp_obj_t utils_from_uint32_buffer(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    return utils_from_intbuffer_helper(n_args, pos_args, kw_args, UTILS_UINT32_BUFFER);
}

MP_DEFINE_CONST_FUN_OBJ_KW(utils_from_uint32_buffer_obj, 1, utils_from_uint32_buffer);
#endif

#endif /* ULAB_UTILS_HAS_FROM_INT16_BUFFER | ULAB_UTILS_HAS_FROM_UINT16_BUFFER | ULAB_UTILS_HAS_FROM_INT32_BUFFER | ULAB_UTILS_HAS_FROM_UINT32_BUFFER */

#if ULAB_UTILS_HAS_SPECTROGRAM
//| import ulab.numpy
//|
//| def spectrogram(r: ulab.numpy.ndarray) -> ulab.numpy.ndarray:
//|     """
//|     :param ulab.numpy.ndarray r: A 1-dimension array of values whose size is a power of 2
//|
//|     Computes the spectrum of the input signal.  This is the absolute value of the (complex-valued) fft of the signal.
//|     This function is similar to scipy's ``scipy.signal.welch`` https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.welch.html."""
//|     ...
//|

mp_obj_t utils_spectrogram(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ, { .u_rom_obj = MP_ROM_NONE }} ,
        #if !ULAB_FFT_IS_NUMPY_COMPATIBLE
        { MP_QSTR_, MP_ARG_OBJ, { .u_rom_obj = MP_ROM_NONE } },
        #endif
        { MP_QSTR_scratchpad, MP_ARG_KW_ONLY | MP_ARG_OBJ, { .u_rom_obj = MP_ROM_NONE } },
        { MP_QSTR_out, MP_ARG_KW_ONLY | MP_ARG_OBJ, { .u_rom_obj = MP_ROM_NONE } },
        { MP_QSTR_log, MP_ARG_KW_ONLY | MP_ARG_OBJ, { .u_rom_obj = MP_ROM_FALSE } },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    if(!mp_obj_is_type(args[0].u_obj, &ulab_ndarray_type)) {
        mp_raise_NotImplementedError(MP_ERROR_TEXT("spectrogram is defined for ndarrays only"));
    }
    ndarray_obj_t *in = MP_OBJ_TO_PTR(args[0].u_obj);

    #if ULAB_MAX_DIMS > 1
    if(in->ndim != 1) {
        mp_raise_TypeError(MP_ERROR_TEXT("spectrogram is implemented for 1D arrays only"));
    }
    #endif

    size_t len = in->len;
    // Check if input is of length of power of 2
    if((len & (len-1)) != 0) {
        mp_raise_ValueError(MP_ERROR_TEXT("input array length must be power of 2"));
    }
    
    ndarray_obj_t *out = NULL;

    #if ULAB_FFT_IS_NUMPY_COMPATIBLE
    mp_obj_t scratchpad_object = args[1].u_obj;
    mp_obj_t out_object = args[2].u_obj;
    mp_obj_t log_object = args[3].u_obj;
    #else
    mp_obj_t scratchpad_object = args[2].u_obj;
    mp_obj_t out_object = args[3].u_obj;
    mp_obj_t log_object = args[4].u_obj;
    #endif

    if(out_object != mp_const_none) {
        if(!mp_obj_is_type(out_object, &ulab_ndarray_type)) {
            mp_raise_TypeError(MP_ERROR_TEXT("out must be an ndarray"));
        }
        
        out = MP_OBJ_TO_PTR(out_object);
        if((out->dtype != NDARRAY_FLOAT) || (out->ndim != 1)){
            mp_raise_TypeError(MP_ERROR_TEXT("out array must be a 1D array of float type"));
        }
        if(len != out->len) {
            mp_raise_ValueError(MP_ERROR_TEXT("input and out arrays must have same length"));
        }
    } else {
        out = ndarray_new_linear_array(len, NDARRAY_FLOAT);
    }

    ndarray_obj_t *scratchpad = NULL;
    mp_float_t *tmp = NULL;

    if(scratchpad_object != mp_const_none) {
        if(!mp_obj_is_type(scratchpad_object, &ulab_ndarray_type)) {
            mp_raise_TypeError(MP_ERROR_TEXT("scratchpad must be an ndarray"));
        }

        scratchpad = MP_OBJ_TO_PTR(scratchpad_object);
        if(!ndarray_is_dense(scratchpad) || (scratchpad->ndim != 1) || (scratchpad->dtype != NDARRAY_FLOAT)) {
            mp_raise_ValueError(MP_ERROR_TEXT("scratchpad must be a 1D dense float array"));
        }
        if(scratchpad->len != 2 * len) {
            mp_raise_ValueError(MP_ERROR_TEXT("scratchpad must be twice as long as input"));
        }

        tmp = (mp_float_t *)scratchpad->array;
    } else {
        tmp = m_new0(mp_float_t, 2 * len);
    }

    uint8_t *array = (uint8_t *)in->array;

    #if ULAB_FFT_IS_NUMPY_COMPATIBLE & ULAB_SUPPORTS_COMPLEX
    if(in->dtype == NDARRAY_COMPLEX) {
        uint8_t sz = 2 * sizeof(mp_float_t);
        for(size_t i = 0; i < len; i++) {
            memcpy(tmp, array, sz);
            tmp += 2;
            array += in->strides[ULAB_MAX_DIMS - 1];
        }
    } else {
        mp_float_t (*func)(void *) = ndarray_get_float_function(in->dtype);
        for(size_t i = 0; i < len; i++) {
            *tmp++ = func(array);   // real part
            *tmp++ = 0;             // imaginary part, clear
            array += in->strides[ULAB_MAX_DIMS - 1];
        }
    }
    
    tmp -= 2 * len;
    fft_kernel(tmp, len, 1);
    #else // we might have two real input vectors

    ndarray_obj_t *in2 = NULL;

    if(n_args == 2) {
        if(!mp_obj_is_type(args[1].u_obj, &ulab_ndarray_type)) {
            mp_raise_NotImplementedError(MP_ERROR_TEXT("spectrogram is defined for ndarrays only"));
        }
        in2 = MP_OBJ_TO_PTR(args[1].u_obj);

        #if ULAB_MAX_DIMS > 1
        if(in2->ndim != 1) {
            mp_raise_TypeError(MP_ERROR_TEXT("spectrogram is implemented for 1D arrays only"));
        }
        #endif
        if(len != in2->len) {
            mp_raise_TypeError(MP_ERROR_TEXT("input arrays are not compatible"));
        }
    } 

    mp_float_t (*func)(void *) = ndarray_get_float_function(in->dtype);

    for(size_t i = 0; i < len; i++) {
        *tmp++ = func(array);       // real part; imageinary will be cleared later
        array += in->strides[ULAB_MAX_DIMS - 1];
    }
    
    if(n_args == 2) {
        mp_float_t (*func2)(void *) = ndarray_get_float_function(in2->dtype);
        array = (uint8_t *)in2->array;
        for(size_t i = 0; i < len; i++) {
            *tmp++ = func2(array);
            array += in2->strides[ULAB_MAX_DIMS - 1];
        }
        tmp -= len;
    } else {
        // if there is only one input argument, clear the imaginary part
        memset(tmp, 0, len * sizeof(mp_float_t));
    }

    tmp -= len;

    fft_kernel(tmp, tmp + len, len, 1);
    #endif /* ULAB_FFT_IS_NUMPY_COMPATIBLE */

    mp_float_t *spectrum = (mp_float_t *)out->array;
    uint8_t spectrum_sz = out->strides[ULAB_MAX_DIMS - 1] / sizeof(mp_float_t);

    for(size_t i = 0; i < len; i++) {
        #if ULAB_FFT_IS_NUMPY_COMPATIBLE
        *spectrum = MICROPY_FLOAT_C_FUN(sqrt)(*tmp * *tmp + *(tmp + 1) * *(tmp + 1));
        tmp += 2;
        #else
        *spectrum = MICROPY_FLOAT_C_FUN(sqrt)(*tmp * *tmp + *(tmp + len) * *(tmp + len));
        tmp++;
        #endif
        if(log_object == mp_const_true) {
            *spectrum = MICROPY_FLOAT_C_FUN(log)(*spectrum);
        }
        spectrum += spectrum_sz;
    }

    if(scratchpad_object == mp_const_none) {
        tmp -= len;
        #if ULAB_FFT_IS_NUMPY_COMPATIBLE
        tmp -= len;
        #endif
        m_del(mp_float_t, tmp, 2 * len);
    }
    return MP_OBJ_FROM_PTR(out);
}

MP_DEFINE_CONST_FUN_OBJ_KW(utils_spectrogram_obj, 1, utils_spectrogram);

#endif /* ULAB_UTILS_HAS_SPECTROGRAM */


static const mp_rom_map_elem_t ulab_utils_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_utils) },
    #if ULAB_UTILS_HAS_FROM_INT16_BUFFER
        { MP_ROM_QSTR(MP_QSTR_from_int16_buffer), MP_ROM_PTR(&utils_from_int16_buffer_obj) },
    #endif
    #if ULAB_UTILS_HAS_FROM_UINT16_BUFFER
        { MP_ROM_QSTR(MP_QSTR_from_uint16_buffer), MP_ROM_PTR(&utils_from_uint16_buffer_obj) },
    #endif
    #if ULAB_UTILS_HAS_FROM_INT32_BUFFER
        { MP_ROM_QSTR(MP_QSTR_from_int32_buffer), MP_ROM_PTR(&utils_from_int32_buffer_obj) },
    #endif
    #if ULAB_UTILS_HAS_FROM_UINT32_BUFFER
        { MP_ROM_QSTR(MP_QSTR_from_uint32_buffer), MP_ROM_PTR(&utils_from_uint32_buffer_obj) },
    #endif
    #if ULAB_UTILS_HAS_SPECTROGRAM
        { MP_ROM_QSTR(MP_QSTR_spectrogram), MP_ROM_PTR(&utils_spectrogram_obj) },
    #endif
};

static MP_DEFINE_CONST_DICT(mp_module_ulab_utils_globals, ulab_utils_globals_table);

const mp_obj_module_t ulab_utils_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_module_ulab_utils_globals,
};
#if CIRCUITPY_ULAB
MP_REGISTER_MODULE(MP_QSTR_ulab_dot_utils, ulab_utils_module);
#endif

#endif /* ULAB_HAS_UTILS_MODULE */
