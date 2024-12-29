
/*
 * This file is part of the micropython-ulab project,
 *
 * https://github.com/v923z/micropython-ulab
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2024 Harald Milz <hm@seneca.muc.de>
 *
*/

#ifndef _SCIPY_INTEGRATE_
#define _SCIPY_INTEGRATE_

#include "../../ulab_tools.h"

extern const mp_obj_module_t ulab_scipy_integrate_module;

#if ULAB_INTEGRATE_HAS_TANHSINH
MP_DECLARE_CONST_FUN_OBJ_KW(optimize_tanhsinh_obj);
#endif
#if ULAB_INTEGRATE_HAS_ROMBERG
MP_DECLARE_CONST_FUN_OBJ_KW(optimize_romberg_obj);
#endif
#if ULAB_INTEGRATE_HAS_SIMPSON
MP_DECLARE_CONST_FUN_OBJ_KW(optimize_simpson_obj);
#endif
#if ULAB_INTEGRATE_HAS_QUAD
MP_DECLARE_CONST_FUN_OBJ_KW(optimize_quad_obj);
#endif

#endif /* _SCIPY_INTEGRATE_ */

