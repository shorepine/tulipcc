/*
 * This file is part of the micropython-ulab project,
 *
 * https://github.com/v923z/micropython-ulab
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2024 Harald Milz <hm@seneca.muc.de>
 *
 * References:
 * - Dr. Robert van Engelen, Improving the mp_float_t Exponential Quadrature Tanh-Sinh, Sinh-Sinh and Exp-Sinh Formulas,
 *   2021, https://www.genivia.com/qthsh.html 
 * - Borwein, Bailey & Girgensohn, "Experimentation in Mathematics - Computational Paths to Discovery", A K Peters,
 *   2003, pages 312-313
 * - Joren Vanherck, Bart Sorée, Wim Magnus, Tanh-sinh quadrature for single and multiple integration using 
 *   floating-point arithmetic, 2020, https://arxiv.org/abs/2007.15057
 * - Tanh-Sinh quadrature, Wikipedia, https://en.wikipedia.org/wiki/Tanh-sinh_quadrature
 * - Romberg's method, Wikipedia, https://en.wikipedia.org/wiki/Romberg%27s_method
 * - Adaptive Simpson's method, Wikipedia, https://en.wikipedia.org/wiki/Adaptive_Simpson%27s_method
 * - Gauss–Kronrod quadrature formula, Wikipedia, https://en.wikipedia.org/wiki/Gauss%E2%80%93Kronrod_quadrature_formula
 *  
 * This module provides four integration methods, and thus deviates from scipy.integrate a bit. 
 * As for the pros and cons of the different methods please consult the literature above. 
 * The code was ported to Micropython from Dr. Engelen's paper and used with his written kind permission
 * - quad    - Tanh-Sinh, Sinh-Sinh and Exp-Sinh quadrature
 * - romberg - Romberg quadrature
 * - simpson - Adaptive Simpson quadrature
 * - quadgk  - Adaptive Gauss-Kronrod (G10,K21) quadrature
 */

#include <math.h>
#include "py/obj.h"
#include "py/runtime.h"
#include "py/misc.h"
#include "py/objtuple.h"

#include "../../ndarray.h"
#include "../../ulab.h"
#include "../../ulab_tools.h"
#include "integrate.h"

#if MICROPY_FLOAT_IMPL == MICROPY_FLOAT_IMPL_DOUBLE
ULAB_DEFINE_FLOAT_CONST(etolerance, MICROPY_FLOAT_CONST(1e-14), 0x283424dcUL, 0x3e901b2b29a4692bULL);
#define MACHEPS        MICROPY_FLOAT_CONST(1e-17)
#else
ULAB_DEFINE_FLOAT_CONST(etolerance, MICROPY_FLOAT_CONST(1e-8), 0x358637cfUL, 0x3e7010c6f7d42d18ULL);
#define MACHEPS        MICROPY_FLOAT_CONST(1e-8)
#endif

#define ZERO           MICROPY_FLOAT_CONST(0.0)
#define POINT_TWO_FIVE MICROPY_FLOAT_CONST(0.25)
#define ONE            MICROPY_FLOAT_CONST(1.0)
#define TWO            MICROPY_FLOAT_CONST(2.0)
#define FOUR           MICROPY_FLOAT_CONST(4.0)
#define SIX            MICROPY_FLOAT_CONST(6.0)
#define TEN            MICROPY_FLOAT_CONST(10.0)
#define FIFTEEN        MICROPY_FLOAT_CONST(15.0)
#define EPS_5          MICROPY_FLOAT_CONST(1e-5)


static mp_float_t integrate_python_call(const mp_obj_type_t *type, mp_obj_t fun, mp_float_t x, mp_obj_t *fargs, uint8_t nparams) {
    // Helper function for calculating the value of f(x, a, b, c, ...),
    // where f is defined in python. Takes a float, returns a float.
    // The array of mp_obj_t type must be supplied, as must the number of parameters (a, b, c...) in nparams
    fargs[0] = mp_obj_new_float(x);
    return mp_obj_get_float(MP_OBJ_TYPE_GET_SLOT(type, call)(fun, nparams+1, 0, fargs));
}

// sign helper function
int sign(mp_float_t x) {
    if (x >= ZERO) 
        return 1;
    else
        return -1;
}        


#if ULAB_INTEGRATE_HAS_TANHSINH
// Tanh-Sinh, Sinh-Sinh and Exp-Sinh quadrature
// https://www.genivia.com/qthsh.html

// return optimized Exp-Sinh integral split point d
mp_float_t exp_sinh_opt_d(mp_float_t (*fun)(mp_float_t), mp_float_t a, mp_float_t eps, mp_float_t d) {
    const mp_obj_type_t *type = mp_obj_get_type(fun);
    mp_obj_t fargs[1];
    mp_float_t h2 = integrate_python_call(type, fun, a + d/2, fargs, 0) - integrate_python_call(type, fun, (a + d*2)*4, fargs, 0);
    int i = 1, j = 32;                   // j=32 is optimal to find r
    if (isfinite(h2) && MICROPY_FLOAT_C_FUN(fabs)(h2) > EPS_5) {    // if |h2| > 2^-16
        mp_float_t r, fl, fr, h, s = 0, lfl, lfr, lr = 2;
        do {                                  // find max j such that fl and fr are finite
            j /= 2;
            r = 1 << (i + j);
            fl = integrate_python_call(type, fun, a + d/r, fargs, 0);
            fr = integrate_python_call(type, fun, (a + d*r)*r*r, fargs, 0);
            h = fl - fr;
        } while (j > 1 && !isfinite(h));
        if (j > 1 && isfinite(h) && sign(h) != sign(h2)) {
            lfl = fl;                         // last fl=f(a+d/r)
            lfr = fr;                         // last fr=f(a+d*r)*r*r
            do {                              // bisect in 4 iterations
                j /= 2;
                r = 1 << (i + j);
                fl = integrate_python_call(type, fun, a + d/r, fargs, 0);
                fr = integrate_python_call(type, fun, (a + d*r)*r*r, fargs, 0);
                h = fl - fr;
                if (isfinite(h)) {
                    s += MICROPY_FLOAT_C_FUN(fabs)(h);  // sum |h| to remove noisy cases
                    if (sign(h) == sign(h2)) {
                        i += j;               // search right half
                    }
                    else {                    // search left half
                        lfl = fl;             // record last fl=f(a+d/r)
                        lfr = fr;             // record last fl=f(a+d*r)*r*r
                        lr = r;               // record last r
                    }
                }
            } while (j > 1);
            if (s > eps) {                    // if sum of |h| > eps
                h = lfl - lfr;                // use last fl and fr before the sign change
                r = lr;                       // use last r before the sign change
                if (h != ZERO)                   // if last diff != 0, back up r by one step
                    r /= TWO;
                if (MICROPY_FLOAT_C_FUN(fabs)(lfl) < MICROPY_FLOAT_C_FUN(fabs)(lfr))
                    d /= r;                   // move d closer to the finite endpoint
                else
                    d *= r;                   // move d closer to the infinite endpoint
            }
        }
    }
    return d;
}


// integrate function f, range a..b, max levels n, error tolerance eps
mp_float_t tanhsinh(mp_float_t (*fun)(mp_float_t), mp_float_t a, mp_float_t b, uint16_t n, mp_float_t eps, mp_float_t *e) {
    const mp_obj_type_t *type = mp_obj_get_type(fun);
    mp_obj_t fargs[1];
    const mp_float_t tol = TEN*eps;
    mp_float_t c = ZERO, d = ONE, s, sign = ONE, v, h = TWO;
    int k = 0, mode = 0;                   // Tanh-Sinh = 0, Exp-Sinh = 1, Sinh-Sinh = 2
    if (b < a) {                                // swap bounds
        v = b;
        b = a;
        a = v;
        sign = -1;
    }
    if (isfinite(a) && isfinite(b)) {
        c = (a+b)/TWO;
        d = (b-a)/TWO;
        v = c;
    }
    else if (isfinite(a)) {
        mode = 1;                               // Exp-Sinh
        d = exp_sinh_opt_d(fun, a, eps, d);
        c = a;
        v = a+d;
    }
    else if (isfinite(b)) {
        mode = 1;                               // Exp-Sinh
        // d = -d;
        d = exp_sinh_opt_d(fun, b, eps, -d);
        sign = -sign;
        c = b;
        v = b+d;
    }
    else {
        mode = 2;                               // Sinh-Sinh
        v = ZERO;
    }
    s = integrate_python_call(type, fun, v, fargs, 0);
    do {
        mp_float_t p = ZERO, q, fp = ZERO, fm = ZERO, t, eh;
        h /= TWO;
        t = eh = MICROPY_FLOAT_C_FUN(exp)(h);
        if (k > ZERO)
           eh *= eh;
        if (mode == 0) {                        // Tanh-Sinh
            do {
                mp_float_t u = MICROPY_FLOAT_C_FUN(exp)(ONE / t - t); // = exp(-2*sinh(j*h)) = 1/exp(sinh(j*h))^2
                mp_float_t r = TWO * u / (ONE + u);                   // = 1 - tanh(sinh(j*h))
                mp_float_t w = (t + ONE / t) * r / (ONE + u);         // = cosh(j*h)/cosh(sinh(j*h))^2
                mp_float_t x = d*r;
                if (a+x > a) {                  // if too close to a then reuse previous fp
                    mp_float_t y = integrate_python_call(type, fun, a+x, fargs, 0);
                    if (isfinite(y))
                        fp = y;                 // if f(x) is finite, add to local sum
                }
                if (b-x < b) {                  // if too close to a then reuse previous fp
                    mp_float_t y = integrate_python_call(type, fun, b-x, fargs, 0);
                    if (isfinite(y))
                        fm = y;                 // if f(x) is finite, add to local sum
                }
                q = w*(fp+fm);
                p += q;
                t *= eh;
            } while (MICROPY_FLOAT_C_FUN(fabs)(q) > eps*MICROPY_FLOAT_C_FUN(fabs)(p));
        }
        else {
            t /= TWO;
            do {
                mp_float_t r = MICROPY_FLOAT_C_FUN(exp)(t - POINT_TWO_FIVE / t);    // = exp(sinh(j*h))
                mp_float_t x, y, w = r;
                q = ZERO;
                if (mode == 1) {                // Exp-Sinh
                    x = c + d/r;
                    if (x == c)                 // if x hit the finite endpoint then break
                        break;
                    y = integrate_python_call(type, fun, x, fargs, 0);
                    if (isfinite(y))    // if f(x) is finite, add to local sum
                        q += y/w;
                }
                else {                          // Sinh-Sinh
                    r = (r - ONE / r) / TWO;              // = sinh(sinh(j*h))
                    w = (w + ONE / w) / TWO;              // = cosh(sinh(j*h))
                    x = c - d*r;
                    y = integrate_python_call(type, fun, x, fargs, 0);
                    if (isfinite(y))    // if f(x) is finite, add to local sum
                        q += y*w;
                }
                x = c + d*r;
                y = integrate_python_call(type, fun, x, fargs, 0);
                if (isfinite(y))        // if f(x) is finite, add to local sum
                    q += y*w;
                q *= t + POINT_TWO_FIVE / t;                   // q *= cosh(j*h)
                p += q;
                t *= eh;
            } while (MICROPY_FLOAT_C_FUN(fabs)(q) > eps*MICROPY_FLOAT_C_FUN(fabs)(p));
        }
        v = s-p;
        s += p;
        ++k;
    } while (MICROPY_FLOAT_C_FUN(fabs)(v) > tol*MICROPY_FLOAT_C_FUN(fabs)(s) && k <= n);
    // return the error estimate by reference
    *e = MICROPY_FLOAT_C_FUN(fabs)(v)/(MICROPY_FLOAT_C_FUN(fabs)(s)+eps);
    return sign*d*s*h;                          // result with estimated relative error e
}

//| def tanhsinh(
//|     fun: Callable[[float], float],
//|     a: float,
//|     b: float,
//|     *,
//|     levels: int = 6
//|     eps: float = etolerance
//| ) -> float:
//|     """
//|     :param callable f: The function to integrate
//|     :param float a: The lower integration limit
//|     :param float b: The upper integration limit
//|     :param float levels: The number of levels to perform (6..7 is optimal)
//|     :param float eps: The error tolerance value 
//|
//|     Find a quadrature of the function ``f(x)`` on the interval
//|     (``a``..``b``) using an optimized double exponential.  The result is accurate to within
//|     ``eps`` unless more than ``levels`` levels are required."""
//|


static mp_obj_t integrate_tanhsinh(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_rom_obj = MP_ROM_NONE } },
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_rom_obj = MP_ROM_NONE } },
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_rom_obj = MP_ROM_NONE } },
        { MP_QSTR_levels, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 6} },
        { MP_QSTR_eps, MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_rom_obj = ULAB_REFERENCE_FLOAT_CONST(etolerance)} },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    mp_obj_t fun = args[0].u_obj;
    const mp_obj_type_t *type = mp_obj_get_type(fun);
    if(!MP_OBJ_TYPE_HAS_SLOT(type, call)) {
        mp_raise_TypeError(MP_ERROR_TEXT("first argument must be a callable"));
    }

	// iterate over args 1, 2, and 4
	// arg 3 will be handled by MP_ARG_INT above. 
	for (int i=1; i<=4; i*=2) {
		type = mp_obj_get_type(args[i].u_obj); 
		if (type != &mp_type_float && type != &mp_type_int) {
	        mp_raise_msg_varg(&mp_type_TypeError,
	            MP_ERROR_TEXT("can't convert arg %d from %s to float"), i, mp_obj_get_type_str(args[i].u_obj));
		}			
	}
    mp_float_t a = mp_obj_get_float(args[1].u_obj);
    mp_float_t b = mp_obj_get_float(args[2].u_obj);
    uint16_t n = (uint16_t)args[3].u_int;
	if (n < 1) {
		mp_raise_ValueError(MP_ERROR_TEXT("levels needs to be a positive integer"));
    }			
    mp_float_t eps = mp_obj_get_float(args[4].u_obj);
    
    mp_obj_t res[2];
    mp_float_t e;
    res[0] = mp_obj_new_float(tanhsinh(fun, a, b, n, eps, &e));
    res[1] = mp_obj_new_float(e);
    return mp_obj_new_tuple(2, res); 
}

MP_DEFINE_CONST_FUN_OBJ_KW(integrate_tanhsinh_obj, 2, integrate_tanhsinh);
#endif /* ULAB_INTEGRATE_HAS_TANHSINH */

#if ULAB_INTEGRATE_HAS_ROMBERG
// Romberg quadrature
// This function is deprecated as of SciPy 1.12.0 and will be removed in SciPy 1.15.0. Please use scipy.integrate.quad instead. 
// https://en.wikipedia.org/wiki/Romberg%27s_method, https://www.genivia.com/qthsh.html, 
// https://docs.scipy.org/doc/scipy/reference/generated/scipy.integrate.romberg.html (which is different 
// insofar as the latter expects an array of function values). 

mp_float_t qromb(mp_float_t (*fun)(mp_float_t), mp_float_t a, mp_float_t b, uint16_t n, mp_float_t eps) {
    const mp_obj_type_t *type = mp_obj_get_type(fun);
    mp_obj_t fargs[1];
    mp_float_t R1[n], R2[n];
    mp_float_t *Ro = &R1[0], *Ru = &R2[0];
    mp_float_t h = b-a;
    uint16_t i, j;
    Ro[0] = (integrate_python_call(type, fun, a, fargs, 0) + integrate_python_call(type, fun, b, fargs, 0)) * h/2;
    for (i = 1; i < n; ++i) {
        unsigned long long k = 1UL << i;
        unsigned long long s = 1;
        mp_float_t sum = ZERO;
        mp_float_t *Rt;
        h /= TWO;
        for (j = 1; j < k; j += 2)
            sum += integrate_python_call(type, fun, a+j*h, fargs, 0);
        Ru[0] = h*sum + Ro[0] / TWO;
        for (j = 1; j <= i; ++j) {
            s <<= 2;
            Ru[j] = (s*Ru[j-1] - Ro[j-1])/(s-1);
        }
        if (i > 2 && MICROPY_FLOAT_C_FUN(fabs)(Ro[i-1]-Ru[i]) <= eps*MICROPY_FLOAT_C_FUN(fabs)(Ru[i])+eps)
            return Ru[i];
        Rt = Ro;
        Ro = Ru;
        Ru = Rt;
    }
    return Ro[n-1];
}

//| def romberg(
//|     fun: Callable[[float], float],
//|     a: float,
//|     b: float,
//|     *,
//|     steps: int = 100
//|     eps: float = etolerance
//| ) -> float:
//|     """
//|     :param callable f: The function to integrate
//|     :param float a: The lower integration limit
//|     :param float b: The upper integration limit
//|     :param float steps: The number of equidistant steps
//|     :param float eps: The tolerance value
//|
//|     Find a quadrature of the function ``f(x)`` on the interval
//|     (``a``..``b``) using the Romberg method.  The result is accurate to within
//|     ``eps`` unless more than ``steps`` steps are required."""
//|     ...
//|

static mp_obj_t integrate_romberg(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_rom_obj = MP_ROM_NONE } },
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_rom_obj = MP_ROM_NONE } },
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_rom_obj = MP_ROM_NONE } },
        { MP_QSTR_steps, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 100} },
        { MP_QSTR_eps, MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_rom_obj = ULAB_REFERENCE_FLOAT_CONST(etolerance)} },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    mp_obj_t fun = args[0].u_obj;
    const mp_obj_type_t *type = mp_obj_get_type(fun);
    if(!MP_OBJ_TYPE_HAS_SLOT(type, call)) {
        mp_raise_TypeError(MP_ERROR_TEXT("first argument must be a callable"));
    }

	// iterate over args 1, 2, and 4
	// arg 3 will be handled by MP_ARG_INT above. 
	for (int i=1; i<=4; i*=2) {
		type = mp_obj_get_type(args[i].u_obj); 
		if (type != &mp_type_float && type != &mp_type_int) {
	        mp_raise_msg_varg(&mp_type_TypeError,
	            MP_ERROR_TEXT("can't convert arg %d from %s to float"), i, mp_obj_get_type_str(args[i].u_obj));
		}			
	}
    mp_float_t a = mp_obj_get_float(args[1].u_obj);
    mp_float_t b = mp_obj_get_float(args[2].u_obj);
    uint16_t steps = (uint16_t)args[3].u_int;
	if (steps < 1) {
		mp_raise_ValueError(MP_ERROR_TEXT("steps needs to be a positive integer"));
    }			
    mp_float_t eps = mp_obj_get_float(args[4].u_obj);
    
    return mp_obj_new_float(qromb(fun, a, b, steps, eps)); 
}

MP_DEFINE_CONST_FUN_OBJ_KW(integrate_romberg_obj, 2, integrate_romberg);
#endif /* ULAB_INTEGRATE_HAS_ROMBERG */

#if ULAB_INTEGRATE_HAS_SIMPSON
// Adaptive Simpson quadrature
// https://en.wikipedia.org/wiki/Adaptive_Simpson%27s_method, https://www.genivia.com/qthsh.html

mp_float_t as(mp_float_t (*fun)(mp_float_t), mp_float_t a, mp_float_t b, mp_float_t fa, mp_float_t fm,
              mp_float_t fb, mp_float_t v, mp_float_t eps, int n, mp_float_t t) {
    const mp_obj_type_t *type = mp_obj_get_type(fun);
    mp_obj_t fargs[1];
    mp_float_t h = (b-a) / TWO;
    mp_float_t f1 = integrate_python_call(type, fun, a + h / TWO, fargs, 0);
    mp_float_t f2 = integrate_python_call(type, fun, b - h / TWO, fargs, 0);
    mp_float_t sl = h*(fa + FOUR * f1 + fm) / SIX;
    mp_float_t sr = h*(fm + FOUR * f2 + fb) / SIX;
    mp_float_t s = sl+sr;
    mp_float_t d = (s-v) / FIFTEEN;
    mp_float_t m = a+h;
    if (n <= 0 || MICROPY_FLOAT_C_FUN(fabs)(d) < eps)
        return t + s + d; // note: fabs(d) can be used as error estimate
    eps /= TWO;
    --n;
    t = as(fun, a, m, fa, f1, fm, sl, eps, n, t);
    return as(fun, m, b, fm, f2, fb, sr, eps, n, t);
}

mp_float_t qasi(mp_float_t (*fun)(mp_float_t), mp_float_t a, mp_float_t b, int n, mp_float_t eps) {
    const mp_obj_type_t *type = mp_obj_get_type(fun);
    mp_obj_t fargs[1];
    mp_float_t fa = integrate_python_call(type, fun, a, fargs, 0);
    mp_float_t fm = integrate_python_call(type, fun, (a+b)/2, fargs, 0);
    mp_float_t fb = integrate_python_call(type, fun, b, fargs, 0);
    mp_float_t v = (fa + FOUR * fm + fb) * (b-a) / SIX;
    return as(fun, a, b, fa, fm, fb, v, eps, n, 0);
}

//| def simpson(
//|     fun: Callable[[float], float],
//|     a: float,
//|     b: float,
//|     *,
//|     steps: int = 100
//|     eps: float = etolerance
//| ) -> float:
//|     """
//|     :param callable f: The function to integrate
//|     :param float a: The lower integration limit
//|     :param float b: The upper integration limit
//|     :param float steps: The number of equidistant steps
//|     :param float eps: The tolerance value
//|
//|     Find a quadrature of the function ``f(x)`` on the interval
//|     (``a``..``b``) using the Adaptive Simpson's method.  The result is accurate to within
//|     ``eps`` unless more than ``steps`` steps are required."""
//|     ...
//|

static mp_obj_t integrate_simpson(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_rom_obj = MP_ROM_NONE } },
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_rom_obj = MP_ROM_NONE } },
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_rom_obj = MP_ROM_NONE } },
        { MP_QSTR_steps, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 100} },
        { MP_QSTR_eps, MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_rom_obj = ULAB_REFERENCE_FLOAT_CONST(etolerance)} },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    mp_obj_t fun = args[0].u_obj;
    const mp_obj_type_t *type = mp_obj_get_type(fun);
    if(!MP_OBJ_TYPE_HAS_SLOT(type, call)) {
        mp_raise_TypeError(MP_ERROR_TEXT("first argument must be a function"));
    }

	// iterate over args 1, 2, and 4
	// arg 3 will be handled by MP_ARG_INT above. 
	for (int i=1; i<=4; i*=2) {
		type = mp_obj_get_type(args[i].u_obj); 
		if (type != &mp_type_float && type != &mp_type_int) {
	        mp_raise_msg_varg(&mp_type_TypeError,
	            MP_ERROR_TEXT("can't convert arg %d from %s to float"), i, mp_obj_get_type_str(args[i].u_obj));
		}			
	}
    mp_float_t a = mp_obj_get_float(args[1].u_obj);
    mp_float_t b = mp_obj_get_float(args[2].u_obj);
    uint16_t steps = (uint16_t)args[3].u_int;
	if (steps < 1) {
		mp_raise_ValueError(MP_ERROR_TEXT("steps needs to be a positive integer"));
    }			
    mp_float_t eps = mp_obj_get_float(args[4].u_obj);
    
    return mp_obj_new_float(qasi(fun, a, b, steps, eps)); 
}

MP_DEFINE_CONST_FUN_OBJ_KW(integrate_simpson_obj, 2, integrate_simpson);
#endif /* ULAB_INTEGRATE_HAS_SIMPSON */

#if ULAB_INTEGRATE_HAS_QUAD
// Adaptive Gauss-Kronrod (G10,K21) quadrature
// https://en.wikipedia.org/wiki/Gauss%E2%80%93Kronrod_quadrature_formula, https://www.genivia.com/qthsh.html

mp_float_t gk(mp_float_t (*fun)(mp_float_t), mp_float_t c, mp_float_t d, mp_float_t *err) {
// abscissas and weights pre-calculated with Legendre Stieltjes polynomials
    static const mp_float_t abscissas[21] = {
        MICROPY_FLOAT_CONST(0.00000000000000000e+00),
        MICROPY_FLOAT_CONST(7.65265211334973338e-02),
        MICROPY_FLOAT_CONST(1.52605465240922676e-01),
        MICROPY_FLOAT_CONST(2.27785851141645078e-01),
        MICROPY_FLOAT_CONST(3.01627868114913004e-01),
        MICROPY_FLOAT_CONST(3.73706088715419561e-01),
        MICROPY_FLOAT_CONST(4.43593175238725103e-01),
        MICROPY_FLOAT_CONST(5.10867001950827098e-01),
        MICROPY_FLOAT_CONST(5.75140446819710315e-01),
        MICROPY_FLOAT_CONST(6.36053680726515025e-01),
        MICROPY_FLOAT_CONST(6.93237656334751385e-01),
        MICROPY_FLOAT_CONST(7.46331906460150793e-01),
        MICROPY_FLOAT_CONST(7.95041428837551198e-01),
        MICROPY_FLOAT_CONST(8.39116971822218823e-01),
        MICROPY_FLOAT_CONST(8.78276811252281976e-01),
        MICROPY_FLOAT_CONST(9.12234428251325906e-01),
        MICROPY_FLOAT_CONST(9.40822633831754754e-01),
        MICROPY_FLOAT_CONST(9.63971927277913791e-01),
        MICROPY_FLOAT_CONST(9.81507877450250259e-01),
        MICROPY_FLOAT_CONST(9.93128599185094925e-01),
        MICROPY_FLOAT_CONST(9.98859031588277664e-01),
    };
    static const mp_float_t weights[21] = {
        MICROPY_FLOAT_CONST(7.66007119179996564e-02),
        MICROPY_FLOAT_CONST(7.63778676720807367e-02),
        MICROPY_FLOAT_CONST(7.57044976845566747e-02),
        MICROPY_FLOAT_CONST(7.45828754004991890e-02),
        MICROPY_FLOAT_CONST(7.30306903327866675e-02),
        MICROPY_FLOAT_CONST(7.10544235534440683e-02),
        MICROPY_FLOAT_CONST(6.86486729285216193e-02),
        MICROPY_FLOAT_CONST(6.58345971336184221e-02),
        MICROPY_FLOAT_CONST(6.26532375547811680e-02),
        MICROPY_FLOAT_CONST(5.91114008806395724e-02),
        MICROPY_FLOAT_CONST(5.51951053482859947e-02),
        MICROPY_FLOAT_CONST(5.09445739237286919e-02),
        MICROPY_FLOAT_CONST(4.64348218674976747e-02),
        MICROPY_FLOAT_CONST(4.16688733279736863e-02),
        MICROPY_FLOAT_CONST(3.66001697582007980e-02),
        MICROPY_FLOAT_CONST(3.12873067770327990e-02),
        MICROPY_FLOAT_CONST(2.58821336049511588e-02),
        MICROPY_FLOAT_CONST(2.03883734612665236e-02),
        MICROPY_FLOAT_CONST(1.46261692569712530e-02),
        MICROPY_FLOAT_CONST(8.60026985564294220e-03),
        MICROPY_FLOAT_CONST(3.07358371852053150e-03),
    };
    static const mp_float_t gauss_weights[10] = {
        MICROPY_FLOAT_CONST(1.52753387130725851e-01),
        MICROPY_FLOAT_CONST(1.49172986472603747e-01),
        MICROPY_FLOAT_CONST(1.42096109318382051e-01),
        MICROPY_FLOAT_CONST(1.31688638449176627e-01),
        MICROPY_FLOAT_CONST(1.18194531961518417e-01),
        MICROPY_FLOAT_CONST(1.01930119817240435e-01),
        MICROPY_FLOAT_CONST(8.32767415767047487e-02),
        MICROPY_FLOAT_CONST(6.26720483341090636e-02),
        MICROPY_FLOAT_CONST(4.06014298003869413e-02),
        MICROPY_FLOAT_CONST(1.76140071391521183e-02),
    };
    const mp_obj_type_t *type = mp_obj_get_type(fun);
    mp_obj_t fargs[1];
    mp_float_t p = ZERO; // kronrod quadrature sum
    mp_float_t q = ZERO; // gauss quadrature sum
    mp_float_t fp, fm;
    mp_float_t e;
    int i;
    fp = integrate_python_call(type, fun, c, fargs, 0);
    p = fp * weights[0];
    for (i = 1; i < 21; i += 2) {
        fp = integrate_python_call(type, fun, c + d * abscissas[i], fargs, 0);
        fm = integrate_python_call(type, fun, c - d * abscissas[i], fargs, 0);
        p += (fp + fm) * weights[i];
        q += (fp + fm) * gauss_weights[i/2];
    }
    for (i = 2; i < 21; i += 2) {
        fp = integrate_python_call(type, fun, c + d * abscissas[i], fargs, 0);
        fm = integrate_python_call(type, fun, c - d * abscissas[i], fargs, 0);
        p += (fp + fm) * weights[i];
    }
    *err = MICROPY_FLOAT_C_FUN(fabs)(p - q);
    e = MICROPY_FLOAT_C_FUN(fabs)(2 * p * MACHEPS); // optional, to take 1e-17 MachEps prec. into account
    if (*err < e)
        *err = e;
    return p;
}

mp_float_t qakro(mp_float_t (*fun)(mp_float_t), mp_float_t a, mp_float_t b, int n, mp_float_t tol, mp_float_t eps, mp_float_t *err) {
    mp_float_t c = (a+b) / TWO;
    mp_float_t d = (b-a) / TWO;
    mp_float_t e;
    mp_float_t r = gk(fun, c, d, &e);
    mp_float_t s = d*r;
    mp_float_t t = MICROPY_FLOAT_C_FUN(fabs)(s*tol);
    if (tol == ZERO)
        tol = t;
    if (n > 0 && t < e && tol < e) {
        s = qakro(fun, a, c, n-1, t / TWO, eps, err);
        s += qakro(fun, c, b, n-1, t / TWO, eps, &e);
        *err += e;
        return s;
    }
    *err = e;
    return s;
}


//| def quad(
//|     fun: Callable[[float], float],
//|     a: float,
//|     b: float,
//|     *,
//|     order: int = 5
//|     eps: float = etolerance
//| ) -> float:
//|     """
//|     :param callable f: The function to integrate
//|     :param float a: The lower integration limit
//|     :param float b: The upper integration limit
//|     :param float order: Order of quadrature integration. Default is 5.
//|     :param float eps: The tolerance value
//|
//|     Find a quadrature of the function ``f(x)`` on the interval
//|     (``a``..``b``) using the Adaptive Gauss-Kronrod method.  The result is accurate to within
//|     ``eps`` unless a higher order than ``order`` is required."""
//|     ...
//|

static mp_obj_t integrate_quad(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_rom_obj = MP_ROM_NONE } },
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_rom_obj = MP_ROM_NONE } },
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_rom_obj = MP_ROM_NONE } },
        { MP_QSTR_order, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 5} },
        { MP_QSTR_eps, MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_rom_obj = ULAB_REFERENCE_FLOAT_CONST(etolerance)} },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    mp_obj_t fun = args[0].u_obj;
    const mp_obj_type_t *type = mp_obj_get_type(fun);
    if(!MP_OBJ_TYPE_HAS_SLOT(type, call)) {
        mp_raise_TypeError(MP_ERROR_TEXT("first argument must be a callable"));
    }

	// iterate over args 1, 2, and 4
	// arg 3 will be handled by MP_ARG_INT above. 
	for (int i=1; i<=4; i*=2) {
		type = mp_obj_get_type(args[i].u_obj); 
		if (type != &mp_type_float && type != &mp_type_int) {
	        mp_raise_msg_varg(&mp_type_TypeError,
	            MP_ERROR_TEXT("can't convert arg %d from %s to float"), i, mp_obj_get_type_str(args[i].u_obj));
		}			
	}
    mp_float_t a = mp_obj_get_float(args[1].u_obj);
    mp_float_t b = mp_obj_get_float(args[2].u_obj);
    uint16_t order = (uint16_t)args[3].u_int;
	if (order < 1) {
		mp_raise_ValueError(MP_ERROR_TEXT("order needs to be a positive integer"));
    }			
    mp_float_t eps = mp_obj_get_float(args[4].u_obj);
    
    mp_obj_t res[2];
    mp_float_t e;
    res[0] = mp_obj_new_float(qakro(fun, a, b, order, 0, eps, &e));
    res[1] = mp_obj_new_float(e);
    return mp_obj_new_tuple(2, res); 
}

MP_DEFINE_CONST_FUN_OBJ_KW(integrate_quad_obj, 2, integrate_quad);
#endif /* ULAB_INTEGRATE_HAS_QUAD */

static const mp_rom_map_elem_t ulab_scipy_integrate_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_integrate) },
#if ULAB_INTEGRATE_HAS_TANHSINH
    { MP_ROM_QSTR(MP_QSTR_tanhsinh), MP_ROM_PTR(&integrate_tanhsinh_obj) },
#endif
#if ULAB_INTEGRATE_HAS_ROMBERG	
    { MP_ROM_QSTR(MP_QSTR_romberg), MP_ROM_PTR(&integrate_romberg_obj) },
#endif	
#if ULAB_INTEGRATE_HAS_SIMPSON	
    { MP_ROM_QSTR(MP_QSTR_simpson), MP_ROM_PTR(&integrate_simpson_obj) },
#endif
#if ULAB_INTEGRATE_HAS_QUAD	
    { MP_ROM_QSTR(MP_QSTR_quad), MP_ROM_PTR(&integrate_quad_obj) },
#endif	
};

static MP_DEFINE_CONST_DICT(mp_module_ulab_scipy_integrate_globals, ulab_scipy_integrate_globals_table);

const mp_obj_module_t ulab_scipy_integrate_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_module_ulab_scipy_integrate_globals,
};
#if CIRCUITPY_ULAB
MP_REGISTER_MODULE(MP_QSTR_ulab_dot_scipy_dot_integrate, ulab_scipy_integrate_module);
#endif

