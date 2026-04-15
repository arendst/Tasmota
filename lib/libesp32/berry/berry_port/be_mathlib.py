"""
berry_port/be_mathlib.py — Port of src/be_mathlib.c

Math module: trigonometric, hyperbolic, exponential, logarithmic,
rounding, power, random, min/max functions and constants.

Original C code is included as comments.
"""

# ============================================================================
# Imports — lazy to avoid circular dependencies
# ============================================================================

import math
import random

def _lazy_be_api():
    import berry_port.be_api as m; return m

from berry_port.berry_conf import BE_INTGER_TYPE

# ============================================================================
# Constants mirroring C macros
# ============================================================================

# #ifdef M_PI
#   #undef M_PI
# #endif
# #define M_PI        3.141592653589793238462643383279
M_PI = 3.141592653589793238462643383279

# #if BE_INTGER_TYPE == 0 /* int */
#   #define M_IMAX    INT_MAX
#   #define M_IMIN    INT_MIN
# #elif BE_INTGER_TYPE == 1 /* long */
#   #define M_IMAX    LONG_MAX
#   #define M_IMIN    LONG_MIN
# #else /* int64_t (long long) */
#   #define M_IMAX    LLONG_MAX
#   #define M_IMIN    LLONG_MIN
# #endif
if BE_INTGER_TYPE == 0:
    M_IMAX = 2147483647          # INT_MAX
    M_IMIN = -2147483648         # INT_MIN
elif BE_INTGER_TYPE == 1:
    M_IMAX = 2147483647          # LONG_MAX (32-bit long)
    M_IMIN = -2147483648         # LONG_MIN
else:
    M_IMAX = 9223372036854775807     # LLONG_MAX
    M_IMIN = -9223372036854775808    # LLONG_MIN

# ============================================================================
# m_isnan
# ============================================================================

# static int m_isnan(bvm *vm)
# {
#     if (be_top(vm) >= 1 && be_isreal(vm, 1)) {
#         breal x = be_toreal(vm, 1);
#         be_pushbool(vm, isnan(x));
#     } else {
#         be_pushbool(vm, bfalse);
#     }
#     be_return(vm);
# }
def m_isnan(vm):
    be_api = _lazy_be_api()
    if be_api.be_top(vm) >= 1 and be_api.be_isreal(vm, 1):
        x = be_api.be_toreal(vm, 1)
        be_api.be_pushbool(vm, math.isnan(x))
    else:
        be_api.be_pushbool(vm, False)
    return be_api.be_returnvalue(vm)

# ============================================================================
# m_isinf
# ============================================================================

# static int m_isinf(bvm *vm)
# {
#     if (be_top(vm) >= 1 && be_isreal(vm, 1)) {
#         breal x = be_toreal(vm, 1);
#         be_pushbool(vm, isinf(x));
#     } else {
#         be_pushbool(vm, bfalse);
#     }
#     be_return(vm);
# }
def m_isinf(vm):
    be_api = _lazy_be_api()
    if be_api.be_top(vm) >= 1 and be_api.be_isreal(vm, 1):
        x = be_api.be_toreal(vm, 1)
        be_api.be_pushbool(vm, math.isinf(x))
    else:
        be_api.be_pushbool(vm, False)
    return be_api.be_returnvalue(vm)

# ============================================================================
# Single-argument math functions (number → real)
# ============================================================================

def _math_unary(vm, func):
    """Helper for single-argument math functions that take a number and push a real."""
    be_api = _lazy_be_api()
    if be_api.be_top(vm) >= 1 and be_api.be_isnumber(vm, 1):
        x = be_api.be_toreal(vm, 1)
        be_api.be_pushreal(vm, func(x))
    else:
        be_api.be_pushreal(vm, 0.0)
    return be_api.be_returnvalue(vm)

# static int m_abs(bvm *vm)
# {
#     if (be_top(vm) >= 1 && be_isnumber(vm, 1)) {
#         breal x = be_toreal(vm, 1);
#         be_pushreal(vm, mathfunc(fabs)(x));
#     } else {
#         be_pushreal(vm, (breal)0.0);
#     }
#     be_return(vm);
# }
def m_abs(vm):
    return _math_unary(vm, math.fabs)

# static int m_ceil(bvm *vm)
# {
#     if (be_top(vm) >= 1 && be_isnumber(vm, 1)) {
#         breal x = be_toreal(vm, 1);
#         be_pushreal(vm, mathfunc(ceil)(x));
#     } else {
#         be_pushreal(vm, (breal)0.0);
#     }
#     be_return(vm);
# }
def m_ceil(vm):
    return _math_unary(vm, math.ceil)

# static int m_floor(bvm *vm)
# {
#     if (be_top(vm) >= 1 && be_isnumber(vm, 1)) {
#         breal x = be_toreal(vm, 1);
#         be_pushreal(vm, mathfunc(floor)(x));
#     } else {
#         be_pushreal(vm, (breal)0.0);
#     }
#     be_return(vm);
# }
def m_floor(vm):
    return _math_unary(vm, math.floor)

# static int m_round(bvm *vm)
# {
#     if (be_top(vm) >= 1 && be_isnumber(vm, 1)) {
#         breal x = be_toreal(vm, 1);
#         be_pushreal(vm, mathfunc(round)(x));
#     } else {
#         be_pushreal(vm, (breal)0.0);
#     }
#     be_return(vm);
# }
def m_round(vm):
    # C round() uses "round half away from zero", Python round() uses banker's rounding.
    # Use math-compatible rounding to match C behavior.
    def c_round(x):
        if x >= 0:
            return math.floor(x + 0.5)
        else:
            return math.ceil(x - 0.5)
    return _math_unary(vm, c_round)

# static int m_sin(bvm *vm) { ... }
def m_sin(vm):
    return _math_unary(vm, math.sin)

# static int m_cos(bvm *vm) { ... }
def m_cos(vm):
    return _math_unary(vm, math.cos)

# static int m_tan(bvm *vm) { ... }
def m_tan(vm):
    return _math_unary(vm, math.tan)

# static int m_asin(bvm *vm) { ... }
def m_asin(vm):
    return _math_unary(vm, math.asin)

# static int m_acos(bvm *vm) { ... }
def m_acos(vm):
    return _math_unary(vm, math.acos)

# static int m_atan(bvm *vm) { ... }
def m_atan(vm):
    return _math_unary(vm, math.atan)

# static int m_sinh(bvm *vm) { ... }
def m_sinh(vm):
    return _math_unary(vm, math.sinh)

# static int m_cosh(bvm *vm) { ... }
def m_cosh(vm):
    return _math_unary(vm, math.cosh)

# static int m_tanh(bvm *vm) { ... }
def m_tanh(vm):
    return _math_unary(vm, math.tanh)

# static int m_sqrt(bvm *vm) { ... }
def m_sqrt(vm):
    return _math_unary(vm, math.sqrt)

# static int m_exp(bvm *vm) { ... }
def m_exp(vm):
    return _math_unary(vm, math.exp)

# static int m_log(bvm *vm) { ... }
def m_log(vm):
    return _math_unary(vm, math.log)

# static int m_log10(bvm *vm) { ... }
def m_log10(vm):
    return _math_unary(vm, math.log10)

# static int m_deg(bvm *vm)
# {
#     if (be_top(vm) >= 1 && be_isnumber(vm, 1)) {
#         breal x = be_toreal(vm, 1);
#         be_pushreal(vm, x * (breal)(180.0 / M_PI));
#     } else {
#         be_pushreal(vm, (breal)0.0);
#     }
#     be_return(vm);
# }
def m_deg(vm):
    return _math_unary(vm, lambda x: x * (180.0 / M_PI))

# static int m_rad(bvm *vm)
# {
#     if (be_top(vm) >= 1 && be_isnumber(vm, 1)) {
#         breal x = be_toreal(vm, 1);
#         be_pushreal(vm, x * (breal)(M_PI / 180.0));
#     } else {
#         be_pushreal(vm, (breal)0.0);
#     }
#     be_return(vm);
# }
def m_rad(vm):
    return _math_unary(vm, lambda x: x * (M_PI / 180.0))

# ============================================================================
# Two-argument math functions
# ============================================================================

# static int m_atan2(bvm *vm)
# {
#     if (be_top(vm) >= 2 && be_isnumber(vm, 1) && be_isnumber(vm, 2)) {
#         breal y = be_toreal(vm, 1);
#         breal x = be_toreal(vm, 2);
#         be_pushreal(vm, mathfunc(atan2)(y, x));
#     } else {
#         be_pushreal(vm, (breal)0.0);
#     }
#     be_return(vm);
# }
def m_atan2(vm):
    be_api = _lazy_be_api()
    if be_api.be_top(vm) >= 2 and be_api.be_isnumber(vm, 1) and be_api.be_isnumber(vm, 2):
        y = be_api.be_toreal(vm, 1)
        x = be_api.be_toreal(vm, 2)
        be_api.be_pushreal(vm, math.atan2(y, x))
    else:
        be_api.be_pushreal(vm, 0.0)
    return be_api.be_returnvalue(vm)

# static int m_pow(bvm *vm)
# {
#     if (be_top(vm) >= 2 && be_isnumber(vm, 1) && be_isnumber(vm, 2)) {
#         breal x = be_toreal(vm, 1);
#         breal y = be_toreal(vm, 2);
#         be_pushreal(vm, mathfunc(pow)(x, y));
#     } else {
#         be_pushreal(vm, (breal)0.0);
#     }
#     be_return(vm);
# }
def m_pow(vm):
    be_api = _lazy_be_api()
    if be_api.be_top(vm) >= 2 and be_api.be_isnumber(vm, 1) and be_api.be_isnumber(vm, 2):
        x = be_api.be_toreal(vm, 1)
        y = be_api.be_toreal(vm, 2)
        be_api.be_pushreal(vm, math.pow(x, y))
    else:
        be_api.be_pushreal(vm, 0.0)
    return be_api.be_returnvalue(vm)

# ============================================================================
# Random number functions
# ============================================================================

# static int m_srand(bvm *vm)
# {
#     if (be_top(vm) >= 1 && be_isint(vm, 1)) {
#         srand((unsigned int)be_toint(vm, 1));
#     }
#     be_return_nil(vm);
# }
def m_srand(vm):
    be_api = _lazy_be_api()
    if be_api.be_top(vm) >= 1 and be_api.be_isint(vm, 1):
        seed = be_api.be_toint(vm, 1)
        random.seed(seed & 0xFFFFFFFF)
    return be_api.be_returnnilvalue(vm)

# static int m_rand(bvm *vm)
# {
#     be_pushint(vm, rand());
#     be_return(vm);
# }
def m_rand(vm):
    be_api = _lazy_be_api()
    # C rand() returns [0, RAND_MAX] where RAND_MAX is at least 32767
    be_api.be_pushint(vm, random.randint(0, 2147483647))
    return be_api.be_returnvalue(vm)

# ============================================================================
# min / max
# ============================================================================

# /* check that all arguments are either int or real, and return true if at least one is real */
# static int m_check_int_or_has_real(bvm *vm)
# {
#     int argc = be_top(vm);
#     int has_real = 0;
#     for (int i = 1; i <= argc; ++i) {
#         if (be_isreal(vm, i)) {
#             has_real = 1;
#         } else if (!be_isint(vm, i)) {
#             be_raise(vm, "type_error", "arguments must be numbers");
#         }
#     }
#     return has_real;
# }
def m_check_int_or_has_real(vm):
    be_api = _lazy_be_api()
    argc = be_api.be_top(vm)
    has_real = False
    for i in range(1, argc + 1):
        if be_api.be_isreal(vm, i):
            has_real = True
        elif not be_api.be_isint(vm, i):
            be_api.be_raise(vm, "type_error", "arguments must be numbers")
    return has_real

# static int m_min_max(bvm *vm, int is_min) {
#     int argc = be_top(vm);
#     if (argc > 0) {
#         /* see if at least one argument is float, else they are all ints */
#         int has_real = m_check_int_or_has_real(vm);
#         if (has_real) {
#             breal bound = be_toreal(vm, 1);
#             for (int i = 2; i <= argc; ++i) {
#                 breal x = be_toreal(vm, i);
#                 if (is_min ? (x < bound) : (x > bound)) {
#                     bound = x;
#                 }
#             }
#             be_pushreal(vm, bound);
#         } else {
#             bint bound = be_toint(vm, 1);
#             for (int i = 2; i <= argc; ++i) {
#                 bint x = be_toint(vm, i);
#                 if (is_min ? (x < bound) : (x > bound)) {
#                     bound = x;
#                 }
#             }
#             be_pushint(vm, bound);
#         }
#         be_return(vm);
#     }
#     be_return_nil(vm);
# }
def m_min_max(vm, is_min):
    be_api = _lazy_be_api()
    argc = be_api.be_top(vm)
    if argc > 0:
        has_real = m_check_int_or_has_real(vm)
        if has_real:
            bound = be_api.be_toreal(vm, 1)
            for i in range(2, argc + 1):
                x = be_api.be_toreal(vm, i)
                if (x < bound) if is_min else (x > bound):
                    bound = x
            be_api.be_pushreal(vm, bound)
        else:
            bound = be_api.be_toint(vm, 1)
            for i in range(2, argc + 1):
                x = be_api.be_toint(vm, i)
                if (x < bound) if is_min else (x > bound):
                    bound = x
            be_api.be_pushint(vm, bound)
        return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)

# int m_min(bvm *vm)
# {
#     return m_min_max(vm, 1);
# }
def m_min(vm):
    return m_min_max(vm, True)

# int m_max(bvm *vm)
# {
#     return m_min_max(vm, 0);
# }
def m_max(vm):
    return m_min_max(vm, False)

# ============================================================================
# Module table
# ============================================================================

# @const_object_info_begin
# module math (scope: global, depend: BE_USE_MATH_MODULE) {
#     isnan, func(m_isnan)
#     isinf, func(m_isinf)
#     abs, func(m_abs)
#     ceil, func(m_ceil)
#     floor, func(m_floor)
#     round, func(m_round)
#     sin, func(m_sin)
#     cos, func(m_cos)
#     tan, func(m_tan)
#     asin, func(m_asin)
#     acos, func(m_acos)
#     atan, func(m_atan)
#     atan2, func(m_atan2)
#     sinh, func(m_sinh)
#     cosh, func(m_cosh)
#     tanh, func(m_tanh)
#     sqrt, func(m_sqrt)
#     exp, func(m_exp)
#     log, func(m_log)
#     log10, func(m_log10)
#     deg, func(m_deg)
#     rad, func(m_rad)
#     pow, func(m_pow)
#     srand, func(m_srand)
#     rand, func(m_rand)
#     min, func(m_min)
#     max, func(m_max)
#     pi, real(M_PI)
#     nan, real(NAN)
#     inf, real(INFINITY)
#     imax, int(M_IMAX)
#     imin, int(M_IMIN)
# }
# @const_object_info_end

def be_math_module_table():
    """Return the native module attribute table for the math module."""
    return [
        ("isnan", m_isnan),
        ("isinf", m_isinf),
        ("abs", m_abs),
        ("ceil", m_ceil),
        ("floor", m_floor),
        ("round", m_round),
        ("sin", m_sin),
        ("cos", m_cos),
        ("tan", m_tan),
        ("asin", m_asin),
        ("acos", m_acos),
        ("atan", m_atan),
        ("atan2", m_atan2),
        ("sinh", m_sinh),
        ("cosh", m_cosh),
        ("tanh", m_tanh),
        ("sqrt", m_sqrt),
        ("exp", m_exp),
        ("log", m_log),
        ("log10", m_log10),
        ("deg", m_deg),
        ("rad", m_rad),
        ("pow", m_pow),
        ("srand", m_srand),
        ("rand", m_rand),
        ("min", m_min),
        ("max", m_max),
    ]

def be_math_module_constants():
    """Return the constant attributes for the math module (reals and ints)."""
    return [
        ("pi", float(M_PI)),
        ("nan", float('nan')),
        ("inf", float('inf')),
        ("imax", M_IMAX),
        ("imin", M_IMIN),
    ]
