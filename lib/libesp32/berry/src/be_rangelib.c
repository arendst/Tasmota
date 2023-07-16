/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#include "be_object.h"
#include "be_func.h"
#include "be_vm.h"

static int m_init(bvm *vm)
{
    int argc = be_top(vm);
    if (argc < 3) { be_raise(vm, "value_error", "missing arguments"); }
    if (!be_isint(vm, 2) || !be_isint(vm, 3)) { be_raise(vm, "value_error", "arguments must be 'int'"); }
    be_pushvalue(vm, 2);
    be_setmember(vm, 1, "__lower__");
    be_pop(vm, 1);
    be_pushvalue(vm, 3);
    be_setmember(vm, 1, "__upper__");
    int incr = 1;   /* default increment is '1' */
    if (argc >= 4) {
        if (!be_isint(vm, 4)) { be_raise(vm, "value_error", "arguments must be 'int'"); }
        incr = be_toint(vm, 4);
        if (incr == 0) { be_raise(vm, "value_error", "increment cannot be zero"); }
    }
    be_pushint(vm, incr);
    be_setmember(vm, 1, "__incr__");
    be_return_nil(vm);
}

static int m_tostring(bvm *vm)
{
    be_getmember(vm, 1, "__incr__");
    int incr = be_toint(vm, -1);
    be_pop(vm, 1);
    if (incr == 1) {
        be_pushstring(vm, "(");
        be_getmember(vm, 1, "__lower__");
        be_tostring(vm, -1);
        be_strconcat(vm, -2);
        be_pop(vm, 1);
        be_pushstring(vm, "..");
        be_strconcat(vm, -2);
        be_pop(vm, 1);
        be_getmember(vm, 1, "__upper__");
        be_tostring(vm, -1);
        be_strconcat(vm, -2);
        be_pop(vm, 1);
        be_pushstring(vm, ")");
        be_strconcat(vm, -2);
        be_pop(vm, 1);
    } else {
        be_pushstring(vm, "range(");
        be_getmember(vm, 1, "__lower__");
        be_tostring(vm, -1);
        be_strconcat(vm, -2);
        be_pop(vm, 1);
        be_pushstring(vm, ", ");
        be_strconcat(vm, -2);
        be_pop(vm, 1);
        be_getmember(vm, 1, "__upper__");
        be_tostring(vm, -1);
        be_strconcat(vm, -2);
        be_pop(vm, 1);
        be_pushstring(vm, ", ");
        be_strconcat(vm, -2);
        be_pop(vm, 1);
        be_getmember(vm, 1, "__incr__");
        be_tostring(vm, -1);
        be_strconcat(vm, -2);
        be_pop(vm, 1);
        be_pushstring(vm, ")");
        be_strconcat(vm, -2);
        be_pop(vm, 1);
    }
    be_return(vm);
}

static int m_upper(bvm *vm)
{
    be_getmember(vm, 1, "__upper__");
    be_return(vm);
}

static int m_lower(bvm *vm)
{
    be_getmember(vm, 1, "__lower__");
    be_return(vm);
}

static int m_incr(bvm *vm)
{
    be_getmember(vm, 1, "__incr__");
    be_return(vm);
}

static int m_setrange(bvm *vm)
{
    int argc = be_top(vm);
    if (argc < 3) { be_raise(vm, "value_error", "missing arguments"); }
    if (!be_isint(vm, 2) || !be_isint(vm, 3)) { be_raise(vm, "value_error", "arguments must be 'int'"); }
    be_pushvalue(vm, 2);
    be_setmember(vm, 1, "__lower__");
    be_pop(vm, 1);
    be_pushvalue(vm, 3);
    be_setmember(vm, 1, "__upper__");
    int incr = 1;   /* default increment is '1' */
    if (argc >= 4) {
        if (!be_isint(vm, 4)) { be_raise(vm, "value_error", "arguments must be 'int'"); }
        incr = be_toint(vm, 4);
        if (incr == 0) { be_raise(vm, "value_error", "increment cannot be zero"); }
    }
    be_pushint(vm, incr);
    be_setmember(vm, 1, "__incr__");
    be_return_nil(vm);
}

static int iter_closure(bvm *vm)
{
    /* for better performance, we operate the upvalues
     * directly without using by the stack. */
    bntvclos *func = var_toobj(vm->cf->func);
    bvalue *uv0 = be_ntvclos_upval(func, 0)->value;
    bvalue *uv1 = be_ntvclos_upval(func, 1)->value;
    bvalue *uv2 = be_ntvclos_upval(func, 2)->value;
    bint lower = var_toint(uv0); /* upvalue[0] => lower */
    bint upper = var_toint(uv1); /* upvalue[1] => upper */
    bint incr = var_toint(uv2); /* upvalue[2] => incr */
    if ((incr > 0 && lower > upper) || (incr < 0 && lower < upper)) {
        be_stop_iteration(vm);
    }
    var_toint(uv0) = lower + incr; /* set upvale[0] */
    be_pushint(vm, lower); /* push the return value */
    be_return(vm);
}

static int m_iter(bvm *vm)
{
    be_pushntvclosure(vm, iter_closure, 3);
    be_getmember(vm, 1, "__lower__");
    be_setupval(vm, -2, 0);
    be_pop(vm, 1);
    be_getmember(vm, 1, "__upper__");
    be_setupval(vm, -2, 1);
    be_pop(vm, 1);
    be_getmember(vm, 1, "__incr__");
    be_setupval(vm, -2, 2);
    be_pop(vm, 1);
    be_return(vm);
}

#if !BE_USE_PRECOMPILED_OBJECT
void be_load_rangelib(bvm *vm)
{
    static const bnfuncinfo members[] = {
        { "__lower__", NULL },
        { "__upper__", NULL },
        { "__incr__", NULL },
        { "init", m_init },
        { "tostring", m_tostring },
        { "lower", m_lower },
        { "upper", m_upper },
        { "setrange", m_setrange },
        { "iter", m_iter },
        { NULL, NULL }
    };
    be_regclass(vm, "range", members);
}
#else
/* @const_object_info_begin
class be_class_range (scope: global, name: range) {
    __lower__, var
    __upper__, var
    __incr__, var
    init, func(m_init)
    tostring, func(m_tostring)
    lower, func(m_lower)
    upper, func(m_upper)
    incr, func(m_incr)
    setrange, func(m_setrange)
    iter, func(m_iter)
}
@const_object_info_end */
#include "../generate/be_fixed_be_class_range.h"
#endif
