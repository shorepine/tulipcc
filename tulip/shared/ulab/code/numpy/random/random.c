/*
 * This file is part of the micropython-ulab project,
 *
 * https://github.com/v923z/micropython-ulab
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2024 Zoltán Vörös
*/

#include <math.h>

#include "py/builtin.h"
#include "py/obj.h"
#include "py/runtime.h"

#include "random.h"

ULAB_DEFINE_FLOAT_CONST(random_zero, MICROPY_FLOAT_CONST(0.0), 0UL, 0ULL);
ULAB_DEFINE_FLOAT_CONST(random_one, MICROPY_FLOAT_CONST(1.0), 0x3f800000UL, 0x3ff0000000000000ULL);

// methods of the Generator object
static const mp_rom_map_elem_t random_generator_locals_dict_table[] = {
    #if ULAB_NUMPY_RANDOM_HAS_NORMAL
        { MP_ROM_QSTR(MP_QSTR_normal), MP_ROM_PTR(&random_normal_obj) },
    #endif
    #if ULAB_NUMPY_RANDOM_HAS_RANDOM
        { MP_ROM_QSTR(MP_QSTR_random), MP_ROM_PTR(&random_random_obj) },
    #endif
    #if ULAB_NUMPY_RANDOM_HAS_UNIFORM
        { MP_ROM_QSTR(MP_QSTR_uniform), MP_ROM_PTR(&random_uniform_obj) },
    #endif
};

static MP_DEFINE_CONST_DICT(random_generator_locals_dict, random_generator_locals_dict_table);

// random's Generator object is defined here
#if defined(MP_DEFINE_CONST_OBJ_TYPE)
MP_DEFINE_CONST_OBJ_TYPE(
    random_generator_type,
    MP_QSTR_generator,
    MP_TYPE_FLAG_NONE,
    print, random_generator_print,
    make_new, random_generator_make_new,
    locals_dict, &random_generator_locals_dict
);
#else
const mp_obj_type_t random_generator_type = {
    { &mp_type_type },
    .name = MP_QSTR_generator,
    .print = random_generator_print,
    .make_new = random_generator_make_new,
    .locals_dict = (mp_obj_dict_t*)&random_generator_locals_dict
};
#endif

void random_generator_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    (void)kind;
    random_generator_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(MP_PYTHON_PRINTER, "Gnerator() at 0x%p", self);
}

mp_obj_t random_generator_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    (void) type;
    mp_arg_check_num(n_args, n_kw, 0, 1, true);
    mp_map_t kw_args;
    mp_map_init_fixed_table(&kw_args, n_kw, args + n_args);

    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_OBJ, { .u_rom_obj = MP_ROM_NONE } },
    };
    mp_arg_val_t _args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, args, &kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, _args);


    if(args[0] == mp_const_none) {
        #ifndef MICROPY_PY_RANDOM_SEED_INIT_FUNC
        mp_raise_ValueError(MP_ERROR_TEXT("no default seed"));
        #else
        random_generator_obj_t *generator = m_new_obj(random_generator_obj_t);
        generator->base.type = &random_generator_type;
        generator->state = MICROPY_PY_RANDOM_SEED_INIT_FUNC;
        return MP_OBJ_FROM_PTR(generator);
        #endif
    } else if(mp_obj_is_int(args[0])) {
        random_generator_obj_t *generator = m_new_obj(random_generator_obj_t);
        generator->base.type = &random_generator_type;
        generator->state = (size_t)mp_obj_get_int(args[0]);
        return MP_OBJ_FROM_PTR(generator);
    } else if(mp_obj_is_type(args[0], &mp_type_tuple)){
        mp_obj_tuple_t *seeds = MP_OBJ_TO_PTR(args[0]);
        mp_obj_t *items = m_new(mp_obj_t, seeds->len);

        for(uint8_t i = 0; i < seeds->len; i++) {
            random_generator_obj_t *generator = m_new_obj(random_generator_obj_t);
            generator->base.type = &random_generator_type;
            generator->state = (size_t)mp_obj_get_int(seeds->items[i]);
            items[i] = generator;
        }
        return mp_obj_new_tuple(seeds->len, items);
    } else {
        mp_raise_TypeError(MP_ERROR_TEXT("argument must be None, an integer or a tuple of integers"));
    }
    // we should never end up here
    return mp_const_none;
}
// END OF GENERATOR COMPONENTS


static inline uint32_t pcg32_next(uint64_t *state) {
    uint64_t old_state = *state;
    *state = old_state * PCG_MULTIPLIER_64 + PCG_INCREMENT_64;
    uint32_t value = (uint32_t)((old_state ^ (old_state >> 18)) >> 27);
    int rot = old_state >> 59;
    return rot ? (value >> rot) | (value << (32 - rot)) : value;
}

#if MICROPY_FLOAT_IMPL == MICROPY_FLOAT_IMPL_DOUBLE
static inline uint64_t pcg32_next64(uint64_t *state) {
    uint64_t value = pcg32_next(state);
    value <<= 32;
    value |= pcg32_next(state);
    return value;
}
#endif

#if ULAB_NUMPY_RANDOM_HAS_NORMAL
static mp_obj_t random_normal(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_OBJ, { .u_rom_obj = MP_ROM_NONE } },
        { MP_QSTR_loc, MP_ARG_OBJ, { .u_rom_obj = ULAB_REFERENCE_FLOAT_CONST(random_zero) } },
        { MP_QSTR_scale, MP_ARG_OBJ, { .u_rom_obj = ULAB_REFERENCE_FLOAT_CONST(random_one) } },
        { MP_QSTR_size, MP_ARG_OBJ, { .u_rom_obj = MP_ROM_NONE } },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    random_generator_obj_t *self = MP_OBJ_TO_PTR(args[0].u_obj);
    mp_float_t loc = mp_obj_get_float(args[1].u_obj);
    mp_float_t scale = mp_obj_get_float(args[2].u_obj);
    mp_obj_t size = args[3].u_obj;

    ndarray_obj_t *ndarray = NULL;
    mp_float_t u, v, value;

    if(size != mp_const_none) {
        if(mp_obj_is_int(size)) {
            ndarray = ndarray_new_linear_array((size_t)mp_obj_get_int(size), NDARRAY_FLOAT);
        } else if(mp_obj_is_type(size, &mp_type_tuple)) {
            mp_obj_tuple_t *_shape = MP_OBJ_TO_PTR(size);
            if(_shape->len > ULAB_MAX_DIMS) {
                mp_raise_ValueError(MP_ERROR_TEXT("maximum number of dimensions is " MP_STRINGIFY(ULAB_MAX_DIMS)));
            }
            ndarray = ndarray_new_ndarray_from_tuple(_shape, NDARRAY_FLOAT);
        } else { // input type not supported
            mp_raise_TypeError(MP_ERROR_TEXT("shape must be None, and integer or a tuple of integers"));
        }
    } else {
        // return single value
        #if MICROPY_FLOAT_IMPL == MICROPY_FLOAT_IMPL_FLOAT
        uint32_t x = pcg32_next(&self->state);
        u = (float)(int32_t)(x >> 8) * 0x1.0p-24f;
        x = pcg32_next(&self->state);
        v = (float)(int32_t)(x >> 8) * 0x1.0p-24f;
        #else
        uint64_t x = pcg32_next64(&self->state);
        u = (double)(int64_t)(x >> 11) * 0x1.0p-53;
        x = pcg32_next64(&self->state);
        v = (double)(int64_t)(x >> 11) * 0x1.0p-53;
        #endif
        mp_float_t sqrt_log = MICROPY_FLOAT_C_FUN(sqrt)(-MICROPY_FLOAT_CONST(2.0) * MICROPY_FLOAT_C_FUN(log)(u));
        value = sqrt_log * MICROPY_FLOAT_C_FUN(cos)(MICROPY_FLOAT_CONST(2.0) * MP_PI * v);
        return mp_obj_new_float(loc + scale * value);
    }

    mp_float_t *array = (mp_float_t *)ndarray->array;

    // numpy's random supports only dense output arrays, so we can simply
    // loop through the elements in a linear fashion
    for(size_t i = 0; i < ndarray->len; i = i + 2) {
        #if MICROPY_FLOAT_IMPL == MICROPY_FLOAT_IMPL_FLOAT
        uint32_t x = pcg32_next(&self->state);
        u = (float)(int32_t)(x >> 8) * 0x1.0p-24f;
        x = pcg32_next(&self->state);
        v = (float)(int32_t)(x >> 8) * 0x1.0p-24f;
        #else
        uint64_t x = pcg32_next64(&self->state);
        u = (double)(int64_t)(x >> 11) * 0x1.0p-53;
        x = pcg32_next64(&self->state);
        v = (double)(int64_t)(x >> 11) * 0x1.0p-53;
        #endif
        mp_float_t sqrt_log = MICROPY_FLOAT_C_FUN(sqrt)(-MICROPY_FLOAT_CONST(2.0) * MICROPY_FLOAT_C_FUN(log)(u));
        value = sqrt_log * MICROPY_FLOAT_C_FUN(cos)(MICROPY_FLOAT_CONST(2.0) * MP_PI * v);
        *array++ = loc + scale * value;
        if((i & 1) == 0) {
            value = sqrt_log * MICROPY_FLOAT_C_FUN(sin)(MICROPY_FLOAT_CONST(2.0) * MP_PI * v);
            *array++ = loc + scale * value;
        }
    }
    return MP_OBJ_FROM_PTR(ndarray);
}

MP_DEFINE_CONST_FUN_OBJ_KW(random_normal_obj, 1, random_normal);
#endif /* ULAB_NUMPY_RANDOM_HAS_NORMAL */

#if ULAB_NUMPY_RANDOM_HAS_RANDOM
static mp_obj_t random_random(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_OBJ, { .u_rom_obj = MP_ROM_NONE } },
        { MP_QSTR_size, MP_ARG_OBJ, { .u_rom_obj = MP_ROM_NONE } },
        { MP_QSTR_out, MP_ARG_KW_ONLY | MP_ARG_OBJ, { .u_rom_obj = MP_ROM_NONE } },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    random_generator_obj_t *self = MP_OBJ_TO_PTR(args[0].u_obj);

    mp_obj_t size = args[1].u_obj;
    mp_obj_t out = args[2].u_obj;

    ndarray_obj_t *ndarray = NULL;
    size_t *shape = m_new(size_t, ULAB_MAX_DIMS);
    uint8_t ndim = 1;

    if(size != mp_const_none) {
        if(mp_obj_is_int(size)) {
            shape[ULAB_MAX_DIMS - 1] = (size_t)mp_obj_get_int(size);
        } else if(mp_obj_is_type(size, &mp_type_tuple)) {
            mp_obj_tuple_t *_shape = MP_OBJ_TO_PTR(size);
            if(_shape->len > ULAB_MAX_DIMS) {
                mp_raise_ValueError(MP_ERROR_TEXT("maximum number of dimensions is " MP_STRINGIFY(ULAB_MAX_DIMS)));
            }
            ndim = _shape->len;
            for(size_t i = 0; i < ULAB_MAX_DIMS; i++) {
                if(i >= ndim) {
                    shape[ULAB_MAX_DIMS - 1 - i] = 0;
                } else {
                    shape[ULAB_MAX_DIMS - 1 - i] = mp_obj_get_int(_shape->items[i]);
                }
            }
        } else { // input type not supported
            mp_raise_TypeError(MP_ERROR_TEXT("shape must be None, and integer or a tuple of integers"));
        }
    }

    if(out != mp_const_none) {
        if(!mp_obj_is_type(out, &ulab_ndarray_type)) {
            mp_raise_TypeError(MP_ERROR_TEXT("out has wrong type"));
        }

        ndarray = MP_OBJ_TO_PTR(out);

        if(ndarray->dtype != NDARRAY_FLOAT) {
            mp_raise_TypeError(MP_ERROR_TEXT("output array has wrong type"));
        }
        if(size != mp_const_none) {
            for(uint8_t i = 0; i < ULAB_MAX_DIMS; i++) {
                if(ndarray->shape[i] != shape[i]) {
                    mp_raise_ValueError(MP_ERROR_TEXT("size must match out.shape when used together"));
                }
            }
        }
        if(!ndarray_is_dense(ndarray)) {
            mp_raise_ValueError(MP_ERROR_TEXT("output array must be contiguous"));
        }
    } else { // out == None
        if(size != mp_const_none) {
            ndarray = ndarray_new_dense_ndarray(ndim, shape, NDARRAY_FLOAT);
        } else {
            // return single value
            mp_float_t value;
            #if MICROPY_FLOAT_IMPL == MICROPY_FLOAT_IMPL_FLOAT
            uint32_t x = pcg32_next(&self->state);
            value = (float)(int32_t)(x >> 8) * 0x1.0p-24f;
            #else
            uint64_t x = pcg32_next64(&self->state);
            value = (double)(int64_t)(x >> 11) * 0x1.0p-53;
            #endif
            return mp_obj_new_float(value);
        }
    }

    mp_float_t *array = (mp_float_t *)ndarray->array;

    // numpy's random supports only dense output arrays, so we can simply
    // loop through the elements in a linear fashion
    for(size_t i = 0; i < ndarray->len; i++) {

        #if MICROPY_FLOAT_IMPL == MICROPY_FLOAT_IMPL_FLOAT
        uint32_t x = pcg32_next(&self->state);
        *array = (float)(int32_t)(x >> 8) * 0x1.0p-24f;
        #else
        uint64_t x = pcg32_next64(&self->state);
        *array = (double)(int64_t)(x >> 11) * 0x1.0p-53;
        #endif

        array++;
    }
    return MP_OBJ_FROM_PTR(ndarray);
}

MP_DEFINE_CONST_FUN_OBJ_KW(random_random_obj, 1, random_random);
#endif /* ULAB_NUMPY_RANDOM_HAS_RANDOM */

#if ULAB_NUMPY_RANDOM_HAS_UNIFORM
static mp_obj_t random_uniform(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_OBJ, { .u_rom_obj = MP_ROM_NONE } },
        { MP_QSTR_low, MP_ARG_OBJ, { .u_rom_obj = ULAB_REFERENCE_FLOAT_CONST(random_zero) } },
        { MP_QSTR_high, MP_ARG_OBJ, { .u_rom_obj = ULAB_REFERENCE_FLOAT_CONST(random_one) } },
        { MP_QSTR_size, MP_ARG_OBJ, { .u_rom_obj = MP_ROM_NONE } },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    random_generator_obj_t *self = MP_OBJ_TO_PTR(args[0].u_obj);
    mp_float_t low = mp_obj_get_float(args[1].u_obj);
    mp_float_t high = mp_obj_get_float(args[2].u_obj);
    mp_obj_t size = args[3].u_obj;

    ndarray_obj_t *ndarray = NULL;

    if(size == mp_const_none) {
        // return single value
        mp_float_t value;
        #if MICROPY_FLOAT_IMPL == MICROPY_FLOAT_IMPL_FLOAT
        uint32_t x = pcg32_next(&self->state);
        value = (float)(int32_t)(x >> 8) * 0x1.0p-24f;
        #else
        uint64_t x = pcg32_next64(&self->state);
        value = (double)(int64_t)(x >> 11) * 0x1.0p-53;
        #endif
        return mp_obj_new_float(value);
    } else if(mp_obj_is_type(size, &mp_type_tuple)) {
        mp_obj_tuple_t *_shape = MP_OBJ_TO_PTR(size);
        // TODO: this could be reduced, if the inspection was in the ndarray_new_ndarray_from_tuple function
        if(_shape->len > ULAB_MAX_DIMS) {
            mp_raise_ValueError(MP_ERROR_TEXT("maximum number of dimensions is " MP_STRINGIFY(ULAB_MAX_DIMS)));
        }
        ndarray = ndarray_new_ndarray_from_tuple(_shape, NDARRAY_FLOAT);
    } else { // input type not supported
        mp_raise_TypeError(MP_ERROR_TEXT("shape must be None, and integer or a tuple of integers"));
    }

    mp_float_t *array = (mp_float_t *)ndarray->array;
    mp_float_t diff = high - low;
    for(size_t i = 0; i < ndarray->len; i++) {
        #if MICROPY_FLOAT_IMPL == MICROPY_FLOAT_IMPL_FLOAT
        uint32_t x = pcg32_next(&self->state);
        *array = (float)(int32_t)(x >> 8) * 0x1.0p-24f;
        #else
        uint64_t x = pcg32_next64(&self->state);
        *array = (double)(int64_t)(x >> 11) * 0x1.0p-53;
        #endif
        *array = low + diff * *array;
        array++;
    }
    return MP_OBJ_FROM_PTR(ndarray);
}

MP_DEFINE_CONST_FUN_OBJ_KW(random_uniform_obj, 1, random_uniform);
#endif /* ULAB_NUMPY_RANDOM_HAS_UNIFORM */


static const mp_rom_map_elem_t ulab_numpy_random_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_random) },
    { MP_ROM_QSTR(MP_QSTR_Generator), MP_ROM_PTR(&random_generator_type) },
};

static MP_DEFINE_CONST_DICT(mp_module_ulab_numpy_random_globals, ulab_numpy_random_globals_table);

const mp_obj_module_t ulab_numpy_random_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_module_ulab_numpy_random_globals,
};
