/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#include "be_object.h"
#include <math.h>
#include <limits.h>
#include <stdlib.h>

#if BE_USE_MATH_MODULE

#ifdef M_PI
  #undef M_PI
#endif
#define M_PI        3.141592653589793238462643383279

#if BE_INTGER_TYPE == 0 /* int */
  #define M_IMAX    INT_MAX
  #define M_IMIN    INT_MIN
#elif BE_INTGER_TYPE == 1 /* long */
  #define M_IMAX    LONG_MAX
  #define M_IMIN    LONG_MIN
#else /* int64_t (long long) */
  #define M_IMAX    LLONG_MAX
  #define M_IMIN    LLONG_MIN
#endif

#if BE_USE_SINGLE_FLOAT
  #define mathfunc(func)        func##f
#else
  #define mathfunc(func)        func
#endif

static int m_abs(bvm *vm)
{
    if (be_top(vm) >= 1 && be_isnumber(vm, 1)) {
        breal x = be_toreal(vm, 1);
        be_pushreal(vm, mathfunc(fabs)(x));
    } else {
        be_pushreal(vm, (breal)0.0);
    }
    be_return(vm);
}

static int m_ceil(bvm *vm)
{
    if (be_top(vm) >= 1 && be_isnumber(vm, 1)) {
        breal x = be_toreal(vm, 1);
        be_pushreal(vm, mathfunc(ceil)(x));
    } else {
        be_pushreal(vm, (breal)0.0);
    }
    be_return(vm);
}

static int m_floor(bvm *vm)
{
    if (be_top(vm) >= 1 && be_isnumber(vm, 1)) {
        breal x = be_toreal(vm, 1);
        be_pushreal(vm, mathfunc(floor)(x));
    } else {
        be_pushreal(vm, (breal)0.0);
    }
    be_return(vm);
}

static int m_sin(bvm *vm)
{
    if (be_top(vm) >= 1 && be_isnumber(vm, 1)) {
        breal x = be_toreal(vm, 1);
        be_pushreal(vm, mathfunc(sin)(x));
    } else {
        be_pushreal(vm, (breal)0.0);
    }
    be_return(vm);
}

static int m_cos(bvm *vm)
{
    if (be_top(vm) >= 1 && be_isnumber(vm, 1)) {
        breal x = be_toreal(vm, 1);
        be_pushreal(vm, mathfunc(cos)(x));
    } else {
        be_pushreal(vm, (breal)0.0);
    }
    be_return(vm);
}

static int m_tan(bvm *vm)
{
    if (be_top(vm) >= 1 && be_isnumber(vm, 1)) {
        breal x = be_toreal(vm, 1);
        be_pushreal(vm, mathfunc(tan)(x));
    } else {
        be_pushreal(vm, (breal)0.0);
    }
    be_return(vm);
}

static int m_asin(bvm *vm)
{
    if (be_top(vm) >= 1 && be_isnumber(vm, 1)) {
        breal x = be_toreal(vm, 1);
        be_pushreal(vm, mathfunc(asin)(x));
    } else {
        be_pushreal(vm, (breal)0.0);
    }
    be_return(vm);
}

static int m_acos(bvm *vm)
{
    if (be_top(vm) >= 1 && be_isnumber(vm, 1)) {
        breal x = be_toreal(vm, 1);
        be_pushreal(vm, mathfunc(acos)(x));
    } else {
        be_pushreal(vm, (breal)0.0);
    }
    be_return(vm);
}

static int m_atan(bvm *vm)
{
    if (be_top(vm) >= 1 && be_isnumber(vm, 1)) {
        breal x = be_toreal(vm, 1);
        be_pushreal(vm, mathfunc(atan)(x));
    } else {
        be_pushreal(vm, (breal)0.0);
    }
    be_return(vm);
}

static int m_sinh(bvm *vm)
{
    if (be_top(vm) >= 1 && be_isnumber(vm, 1)) {
        breal x = be_toreal(vm, 1);
        be_pushreal(vm, mathfunc(sinh)(x));
    } else {
        be_pushreal(vm, (breal)0.0);
    }
    be_return(vm);
}

static int m_cosh(bvm *vm)
{
    if (be_top(vm) >= 1 && be_isnumber(vm, 1)) {
        breal x = be_toreal(vm, 1);
        be_pushreal(vm, mathfunc(cosh)(x));
    } else {
        be_pushreal(vm, (breal)0.0);
    }
    be_return(vm);
}

static int m_tanh(bvm *vm)
{
    if (be_top(vm) >= 1 && be_isnumber(vm, 1)) {
        breal x = be_toreal(vm, 1);
        be_pushreal(vm, mathfunc(tanh)(x));
    } else {
        be_pushreal(vm, (breal)0.0);
    }
    be_return(vm);
}

static int m_sqrt(bvm *vm)
{
    if (be_top(vm) >= 1 && be_isnumber(vm, 1)) {
        breal x = be_toreal(vm, 1);
        be_pushreal(vm, mathfunc(sqrt)(x));
    } else {
        be_pushreal(vm, (breal)0.0);
    }
    be_return(vm);
}

static int m_exp(bvm *vm)
{
    if (be_top(vm) >= 1 && be_isnumber(vm, 1)) {
        breal x = be_toreal(vm, 1);
        be_pushreal(vm, mathfunc(exp)(x));
    } else {
        be_pushreal(vm, (breal)0.0);
    }
    be_return(vm);
}

static int m_log(bvm *vm)
{
    if (be_top(vm) >= 1 && be_isnumber(vm, 1)) {
        breal x = be_toreal(vm, 1);
        be_pushreal(vm, mathfunc(log)(x));
    } else {
        be_pushreal(vm, (breal)0.0);
    }
    be_return(vm);
}

static int m_log10(bvm *vm)
{
    if (be_top(vm) >= 1 && be_isnumber(vm, 1)) {
        breal x = be_toreal(vm, 1);
        be_pushreal(vm, mathfunc(log10)(x));
    } else {
        be_pushreal(vm, (breal)0.0);
    }
    be_return(vm);
}

static int m_deg(bvm *vm)
{
    if (be_top(vm) >= 1 && be_isnumber(vm, 1)) {
        breal x = be_toreal(vm, 1);
        be_pushreal(vm, x * (breal)(180.0 / M_PI));
    } else {
        be_pushreal(vm, (breal)0.0);
    }
    be_return(vm);
}

static int m_rad(bvm *vm)
{
    if (be_top(vm) >= 1 && be_isnumber(vm, 1)) {
        breal x = be_toreal(vm, 1);
        be_pushreal(vm, x * (breal)(M_PI / 180.0));
    } else {
        be_pushreal(vm, (breal)0.0);
    }
    be_return(vm);
}

static int m_pow(bvm *vm)
{
    if (be_top(vm) >= 2 && be_isnumber(vm, 1) && be_isnumber(vm, 2)) {
        breal x = be_toreal(vm, 1);
        breal y = be_toreal(vm, 2);
        be_pushreal(vm, mathfunc(pow)(x, y));
    } else {
        be_pushreal(vm, (breal)0.0);
    }
    be_return(vm);
}

static int m_srand(bvm *vm)
{
    if (be_top(vm) >= 1 && be_isint(vm, 1)) {
        srand((unsigned int)be_toint(vm, 1));
    }
    be_return_nil(vm);
}

static int m_rand(bvm *vm)
{
    be_pushint(vm, rand());
    be_return(vm);
}

#if !BE_USE_PRECOMPILED_OBJECT
be_native_module_attr_table(math) {
    be_native_module_function("abs", m_abs),
    be_native_module_function("ceil", m_ceil),
    be_native_module_function("floor", m_floor),
    be_native_module_function("sin", m_sin),
    be_native_module_function("cos", m_cos),
    be_native_module_function("tan", m_tan),
    be_native_module_function("asin", m_asin),
    be_native_module_function("acos", m_acos),
    be_native_module_function("atan", m_atan),
    be_native_module_function("sinh", m_sinh),
    be_native_module_function("cosh", m_cosh),
    be_native_module_function("tanh", m_tanh),
    be_native_module_function("sqrt", m_sqrt),
    be_native_module_function("exp", m_exp),
    be_native_module_function("log", m_log),
    be_native_module_function("log10", m_log10),
    be_native_module_function("deg", m_deg),
    be_native_module_function("rad", m_rad),
    be_native_module_function("pow", m_pow),
    be_native_module_function("srand", m_srand),
    be_native_module_function("rand", m_rand),
    be_native_module_real("pi", M_PI),
    be_native_module_int("imax", M_IMAX),
    be_native_module_int("imin", M_IMIN),
};

be_define_native_module(math, NULL);
#else
/* @const_object_info_begin
module math (scope: global, depend: BE_USE_MATH_MODULE) {
    abs, func(m_abs)
    ceil, func(m_ceil)
    floor, func(m_floor)
    sin, func(m_sin)
    cos, func(m_cos)
    tan, func(m_tan)
    asin, func(m_asin)
    acos, func(m_acos)
    atan, func(m_atan)
    sinh, func(m_sinh)
    cosh, func(m_cosh)
    tanh, func(m_tanh)
    sqrt, func(m_sqrt)
    exp, func(m_exp)
    log, func(m_log)
    log10, func(m_log10)
    deg, func(m_deg)
    rad, func(m_rad)
    pow, func(m_pow)
    srand, func(m_srand)
    rand, func(m_rand)
    pi, real(M_PI)
    imax, int(M_IMAX)
    imin, int(M_IMIN)
}
@const_object_info_end */
#include "../generate/be_fixed_math.h"
#endif

#endif /* BE_USE_MATH_MODULE */
