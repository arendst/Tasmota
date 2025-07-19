/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#include "be_object.h"
#include "be_func.h"
#include "be_exec.h"
#include "be_map.h"
#include "be_vm.h"

#define map_check_data(vm, argc)                        \
    if (!be_ismap(vm, -1) || be_top(vm) - 1 < argc) {   \
        be_return_nil(vm);                              \
    }

#define map_check_ref(vm)                               \
    if (be_refcontains(vm, 1)) {                        \
        be_pushstring(vm, "{...}");                     \
        be_return(vm);                                  \
    }

static int m_init(bvm *vm)
{
    if (be_top(vm) > 1 && be_ismap(vm, 2)) {
        be_pushvalue(vm, 2);
        be_setmember(vm, 1, ".p");
    } else {
        be_newmap(vm);
        be_setmember(vm, 1, ".p");
    }
    be_return_nil(vm);
}

static void push_key(bvm *vm)
{
    be_toescape(vm, -2, 'x'); /* escape string */
    be_pushvalue(vm, -2); /* push to top */
    be_strconcat(vm, -5);
    be_pop(vm, 1);
}

static void push_value(bvm *vm)
{
    be_toescape(vm, -1, 'x'); /* escape string */
    be_strconcat(vm, -4);
    be_pop(vm, 2);
    if (be_iter_hasnext(vm, -3)) {
        be_pushstring(vm, ", ");
        be_strconcat(vm, -3);
        be_pop(vm, 1);
    }
}

static int m_tostring(bvm *vm)
{
    be_getmember(vm, 1, ".p");
    map_check_data(vm, 1);
    map_check_ref(vm);
    be_refpush(vm, 1);
    be_pushstring(vm, "{");
    be_pushiter(vm, -2); /* map iterator use 1 register */
    while (be_iter_hasnext(vm, -3)) {
        be_iter_next(vm, -3);
        push_key(vm); /* key.tostring() */
        be_pushstring(vm, ": "); /* add ': ' */
        be_strconcat(vm, -5);
        be_pop(vm, 1);
        push_value(vm); /* value.tostring() */
    }
    be_pop(vm, 1); /* pop iterator */
    be_pushstring(vm, "}");
    be_strconcat(vm, -2);
    be_pop(vm, 1);
    be_refpop(vm);
    be_return(vm);
}

static int m_remove(bvm *vm)
{
    be_getmember(vm, 1, ".p");
    map_check_data(vm, 2);
    be_pushvalue(vm, 2);
    be_data_remove(vm, -2);
    be_return_nil(vm);
}

static int m_item(bvm *vm)
{
    be_getmember(vm, 1, ".p");
    map_check_data(vm, 2);
    be_pushvalue(vm, 2);
    if (!be_getindex(vm, -2)) {
        be_raise(vm, "key_error", be_tostring(vm, 2));
    }
    be_return(vm);
}

static int m_setitem(bvm *vm)
{
    be_getmember(vm, 1, ".p");
    map_check_data(vm, 3);
    be_pushvalue(vm, 2);
    be_pushvalue(vm, 3);
    be_setindex(vm, -3);
    be_return_nil(vm);
}

static int m_find(bvm *vm)
{
    int argc = be_top(vm);
    be_getmember(vm, 1, ".p");
    map_check_data(vm, 2);
    be_pushvalue(vm, 2);
    /* not find and has default value */
    if (!be_getindex(vm, -2) && argc >= 3) {
        be_pushvalue(vm, 3);
    }
    be_return(vm);
}

static int m_contains(bvm *vm)
{
    be_getmember(vm, 1, ".p");
    map_check_data(vm, 2);
    be_pushvalue(vm, 2);
    be_pushbool(vm, be_getindex(vm, -2));
    be_return(vm);
}

static int m_insert(bvm *vm)
{
    bbool res;
    be_getmember(vm, 1, ".p");
    map_check_data(vm, 3);
    be_pushvalue(vm, 2);
    be_pushvalue(vm, 3);
    res = be_data_insert(vm, -3);
    be_pushbool(vm, res);
    be_return(vm);
}

static int m_size(bvm *vm)
{
    be_getmember(vm, 1, ".p");
    map_check_data(vm, 1);
    be_pushint(vm, be_data_size(vm, -1));
    be_return(vm);
}

static int m_tobool(bvm *vm)
{
    be_getmember(vm, 1, ".p");
    map_check_data(vm, 1);
    be_pushbool(vm, be_data_size(vm, -1) > 0);
    be_return(vm);
}

static int iter_closure(bvm *vm)
{
    /* for better performance, we operate the upvalues
     * directly without using by the stack. */
    bntvclos *func = var_toobj(vm->cf->func);
    bvalue *uv0 = be_ntvclos_upval(func, 0)->value; /* list value */
    bvalue *uv1 = be_ntvclos_upval(func, 1)->value; /* iter value */
    bmapiter iter = var_toobj(uv1);
    bmapnode *next = be_map_next(var_toobj(uv0), &iter);
    if (next == NULL) {
        be_stop_iteration(vm);
        be_return_nil(vm); /* will not be executed */
    }
    var_setobj(uv1, BE_COMPTR, iter); /* set upvale[1] (iter value) */
    /* push next value to top */
    var_setval(vm->top, &next->value);
    be_incrtop(vm);
    be_return(vm);
}

static int m_iter(bvm *vm)
{
    be_pushntvclosure(vm, iter_closure, 2);
    be_getmember(vm, 1, ".p");
    be_setupval(vm, -2, 0);
    be_pushiter(vm, -1);
    be_setupval(vm, -3, 1);
    be_pop(vm, 2);
    be_return(vm);
}

static int keys_iter_closure(bvm *vm)
{
    /* for better performance, we operate the upvalues
     * directly without using by the stack. */
    bntvclos *func = var_toobj(vm->cf->func);
    bvalue *uv0 = be_ntvclos_upval(func, 0)->value; /* list value */
    bvalue *uv1 = be_ntvclos_upval(func, 1)->value; /* iter value */
    bmapiter iter = var_toobj(uv1);
    bmapnode *next = be_map_next(var_toobj(uv0), &iter);
    if (next == NULL) {
        be_stop_iteration(vm);
        be_return_nil(vm); /* will not be executed */
    }
    var_setobj(uv1, BE_COMPTR, iter); /* set upvale[1] (iter value) */
    /* push next value to top */
    var_setobj(vm->top, next->key.type, next->key.v.p);
    be_incrtop(vm);
    be_return(vm);
}

static int m_keys(bvm *vm)
{
    be_pushntvclosure(vm, keys_iter_closure, 2);
    be_getmember(vm, 1, ".p");
    be_setupval(vm, -2, 0);
    be_pushiter(vm, -1);
    be_setupval(vm, -3, 1);
    be_pop(vm, 2);
    be_return(vm);
}

#if !BE_USE_PRECOMPILED_OBJECT
void be_load_maplib(bvm *vm)
{
    static const bnfuncinfo members[] = {
        { ".p", NULL },
        { "init", m_init },
        { "tostring", m_tostring },
        { "remove", m_remove },
        { "item", m_item },
        { "setitem", m_setitem },
        { "find", m_find },
        { "contains", m_contains },
        { "has", m_contains },  /* deprecated */
        { "size", m_size },
        { "insert", m_insert },
        { "iter", m_iter },
        { "keys", m_keys },
        { "tobool", m_tobool }
        { NULL, NULL }
    };
    be_regclass(vm, "map", members);
}
#else
/* @const_object_info_begin
class be_class_map (scope: global, name: map) {
    .p, var
    init, func(m_init)
    tostring, func(m_tostring)
    remove, func(m_remove)
    item, func(m_item)
    setitem, func(m_setitem)
    find, func(m_find)
    contains, func(m_contains)
    has, func(m_contains)
    size, func(m_size)
    insert, func(m_insert)
    iter, func(m_iter)
    keys, func(m_keys)
    tobool, func(m_tobool)
}
@const_object_info_end */
#include "../generate/be_fixed_be_class_map.h"
#endif
