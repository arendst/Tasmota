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
    be_pushvalue(vm, 2);
    be_setmember(vm, 1, "__lower__");
    be_pop(vm, 1);
    be_pushvalue(vm, 3);
    be_setmember(vm, 1, "__upper__");
    be_return_nil(vm);
}

static int m_tostring(bvm *vm)
{
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

static int m_setrange(bvm *vm)
{
    be_pushvalue(vm, 2);
    be_setmember(vm, 1, "__lower__");
    be_pop(vm, 1);
    be_pushvalue(vm, 3);
    be_setmember(vm, 1, "__upper__");
    be_return_nil(vm);
}

static int iter_closure(bvm *vm)
{
    /* for better performance, we operate the upvalues
     * directly without using by the stack. */
    bntvclos *func = var_toobj(vm->cf->func);
    bvalue *uv0 = be_ntvclos_upval(func, 0)->value;
    bvalue *uv1 = be_ntvclos_upval(func, 1)->value;
    bint lower = var_toint(uv0); /* upvalue[0] => lower */
    bint upper = var_toint(uv1); /* upvalue[1] => upper */
    if (lower > upper) {
        be_stop_iteration(vm);
    }
    var_toint(uv0) = lower + 1; /* set upvale[0] */
    be_pushint(vm, lower); /* push the return value */
    be_return(vm);
}

static int m_iter(bvm *vm)
{
    be_pushntvclosure(vm, iter_closure, 2);
    be_getmember(vm, 1, "__lower__");
    be_setupval(vm, -2, 0);
    be_pop(vm, 1);
    be_getmember(vm, 1, "__upper__");
    be_setupval(vm, -2, 1);
    be_pop(vm, 1);
    be_return(vm);
}

#if !BE_USE_PRECOMPILED_OBJECT
void be_load_rangelib(bvm *vm)
{
    static const bnfuncinfo members[] = {
        { "__lower__", NULL },
        { "__upper__", NULL },
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
    init, func(m_init)
    tostring, func(m_tostring)
    lower, func(m_lower)
    upper, func(m_upper)
    setrange, func(m_setrange)
    iter, func(m_iter)
}
@const_object_info_end */
#include "../generate/be_fixed_be_class_range.h"
#endif
