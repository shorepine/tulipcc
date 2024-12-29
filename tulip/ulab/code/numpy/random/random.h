/*
 * This file is part of the micropython-ulab project,
 *
 * https://github.com/v923z/micropython-ulab
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2024 Zoltán Vörös
*/

#include "../../ndarray.h"

#ifndef _NUMPY_RANDOM_
#define _NUMPY_RANDOM_


#define PCG_MULTIPLIER_64       6364136223846793005ULL
#define PCG_INCREMENT_64        1442695040888963407ULL

extern const mp_obj_module_t ulab_numpy_random_module;

extern const mp_obj_type_t random_generator_type;

typedef struct _random_generator_obj_t {
    mp_obj_base_t base;
    uint64_t state;
} random_generator_obj_t;

mp_obj_t random_generator_make_new(const mp_obj_type_t *, size_t , size_t , const mp_obj_t *);
void random_generator_print(const mp_print_t *, mp_obj_t , mp_print_kind_t );


MP_DECLARE_CONST_FUN_OBJ_KW(random_normal_obj);
MP_DECLARE_CONST_FUN_OBJ_KW(random_random_obj);
MP_DECLARE_CONST_FUN_OBJ_KW(random_uniform_obj);

#endif
