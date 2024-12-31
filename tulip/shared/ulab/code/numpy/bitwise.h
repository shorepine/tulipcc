
/*
 * This file is part of the micropython-ulab project,
 *
 * https://github.com/v923z/micropython-ulab
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2023 Zoltán Vörös
*/

#ifndef _BITWISE_
#define _BITWISE_

#include "../ulab.h"
#include "../ndarray.h"

enum BITWISE_FUNCTION_TYPE {
    BITWISE_AND,
    BITWISE_OR,
    BITWISE_XOR,
    BITWISE_LEFT_SHIFT,
    BITWISE_RIGHT_SHIFT,   
};

MP_DECLARE_CONST_FUN_OBJ_2(bitwise_bitwise_and_obj);
MP_DECLARE_CONST_FUN_OBJ_2(bitwise_bitwise_or_obj);
MP_DECLARE_CONST_FUN_OBJ_2(bitwise_bitwise_xor_obj);
MP_DECLARE_CONST_FUN_OBJ_2(left_shift_obj);
MP_DECLARE_CONST_FUN_OBJ_2(right_shift_obj);

#endif  /* _BITWISE_ */
