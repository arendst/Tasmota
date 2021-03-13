/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#include "be_object.h"
#include "be_string.h"
#include "be_strlib.h"
#include "be_list.h"
#include "be_func.h"
#include "be_exec.h"
#include "be_vm.h"
#include <string.h>

#define list_check_data(vm, argc)                       \
    if (!be_islist(vm, -1) || be_top(vm) - 1 < argc) {  \
        be_return_nil(vm);                              \
    }

#define list_check_ref(vm)                              \
    if (be_refcontains(vm, 1)) {                        \
        be_pushstring(vm, "[...]");                     \
        be_return(vm);                                  \
    }

static void list_getindex(bvm *vm, int index)
{
    if (!be_getindex(vm, index)) {
        be_raise(vm, "index_error", "list index out of range");
    }
}

static int m_init(bvm *vm)
{
    int i, argc = be_top(vm);
    if (argc > 1 && be_islist(vm, 2)) {
        be_pushvalue(vm, 2);
        be_setmember(vm, 1, ".p");
    } else {
        be_newlist(vm);
        be_setmember(vm, 1, ".p");
        for (i = 2; i <= argc; ++i) {
            be_pushvalue(vm, i);
            be_data_push(vm, -2);
            be_pop(vm, 1);
        }
    }
    be_return_nil(vm);
}

static void push_element(bvm *vm)
{
    be_toescape(vm, -1, 'x'); /* escape string */
    be_strconcat(vm, -3);
    be_pop(vm, 1);
}

static int m_tostring(bvm *vm)
{
    be_getmember(vm, 1, ".p");
    list_check_data(vm, 1);
    list_check_ref(vm);
    be_refpush(vm, 1);
    be_pushstring(vm, "[");
    be_pushiter(vm, -2);
    while (be_iter_hasnext(vm, -3)) {
        be_iter_next(vm, -3);
        push_element(vm);
        if (be_iter_hasnext(vm, -3)) {
            be_pushstring(vm, ", ");
            be_strconcat(vm, -3);
            be_pop(vm, 1);
        }
    }
    be_pop(vm, 1); /* pop iterator */
    be_pushstring(vm, "]");
    be_strconcat(vm, -2);
    be_pop(vm, 1);
    be_refpop(vm);
    be_return(vm);
}

static int m_push(bvm *vm)
{
    be_getmember(vm, 1, ".p");
    list_check_data(vm, 2);
    be_pushvalue(vm, 2);
    be_data_push(vm, -2);
    be_return_nil(vm);
}

static int m_pop(bvm *vm)
{
    int argc = be_top(vm);
    be_getmember(vm, 1, ".p");
    list_check_data(vm, 1);
    if (argc >= 2) {
        be_pushvalue(vm, 2);
    } else {
        be_pushint(vm, -1);
    }
    list_getindex(vm, -2);
    be_pushvalue(vm, -2);
    be_data_remove(vm, -4);
    be_pop(vm, 1);
    be_return(vm);
}

static int m_insert(bvm *vm)
{
    be_getmember(vm, 1, ".p");
    list_check_data(vm, 3);
    be_pushvalue(vm, 2);
    be_pushvalue(vm, 3);
    be_data_insert(vm, -3);
    be_return_nil(vm);
}

static int m_remove(bvm *vm)
{
    be_getmember(vm, 1, ".p");
    list_check_data(vm, 2);
    be_pushvalue(vm, 2);
    be_data_remove(vm, -2);
    be_return_nil(vm);
}

static int item_range(bvm *vm)
{
    bint lower, upper;
    bint size = be_data_size(vm, -1); /* get source list size */
    /* get index range */
    be_getmember(vm, 2, "__lower__");
    lower = be_toint(vm, -1);
    be_pop(vm, 1);
    be_getmember(vm, 2, "__upper__");
    upper = be_toint(vm, -1);
    be_pop(vm, 1);
    /* protection scope */
    upper = upper < size ? upper : size - 1;
    lower = lower < 0 ? 0 : lower;
    /* construction result list instance */
    be_newobject(vm, "list"); /* result list */
    be_getmember(vm, 1, ".p"); /* source list */
    /* copy elements */
    for (; lower <= upper; ++lower) {
        be_pushint(vm, lower);
        list_getindex(vm, -2);
        be_data_push(vm, -4);
        be_pop(vm, 2);
    }
    be_pop(vm, 2);
    be_return(vm);
}

static int item_list(bvm *vm)
{
    int i, srcsize, idxsize;
    be_getmember(vm, 2, ".p"); /* get index list */
    srcsize = be_data_size(vm, -2); /* get source list size */
    idxsize = be_data_size(vm, -1); /* get index list size */
    /* construction result list instance */
    be_newobject(vm, "list"); /* result list */
    be_getmember(vm, 1, ".p"); /* source list */
    /* copy elements */
    for (i = 0; i < idxsize; ++i) {
        be_pushint(vm, i);
        be_getindex(vm, -5);
        if (be_isint(vm, -1)) {
            int idx = be_toindex(vm, -1);
            if (idx >= 0 && idx < srcsize) {
                be_getindex(vm, -3);
            } else {
                be_pushnil(vm);
            }
        } else {
            be_pushnil(vm);
        }
        be_data_push(vm, -5);
        be_pop(vm, 3);
    }
    be_pop(vm, 2);
    be_return(vm);
}

static int m_item(bvm *vm)
{
    be_getmember(vm, 1, ".p");
    list_check_data(vm, 2);
    if (be_isint(vm, 2)) {
        be_pushvalue(vm, 2);
        list_getindex(vm, -2);
        be_return(vm);
    }
    if (be_isinstance(vm, 2)) {
        const char *cname = be_classname(vm, 2);
        if (!strcmp(cname, "range")) {
            return item_range(vm);
        }
        if (!strcmp(cname, "list")) {
            return item_list(vm);
        }
    }
    be_raise(vm, "index_error", "list index out of range");
    be_return_nil(vm);
}

static int m_find(bvm *vm)
{
    be_getmember(vm, 1, ".p");
    list_check_data(vm, 2);
    if (be_isint(vm, 2)) {
        be_pushvalue(vm, 2);
        if (be_getindex(vm, -2)) {
            be_return(vm);
        }
    }
    be_return_nil(vm);
}

static int m_setitem(bvm *vm)
{
    be_getmember(vm, 1, ".p");
    list_check_data(vm, 3);
    be_pushvalue(vm, 2);
    be_pushvalue(vm, 3);
    if (!be_setindex(vm, -3)) {
        be_raise(vm, "index_error", "list assignment index out of range");
    }
    be_return_nil(vm);
}

static int m_size(bvm *vm)
{
    be_getmember(vm, 1, ".p");
    list_check_data(vm, 1);
    be_pushint(vm, be_data_size(vm, -1));
    be_return(vm);
}

static int m_resize(bvm *vm)
{
    be_getmember(vm, 1, ".p");
    list_check_data(vm, 2);
    be_pushvalue(vm, 2);
    be_data_resize(vm, -2);
    be_return_nil(vm);
}

static int m_clear(bvm *vm)
{
    be_getmember(vm, 1, ".p");
    list_check_data(vm, 1);
    be_pushint(vm, 0);
    be_data_resize(vm, -2);
    be_return_nil(vm);
}

static int iter_closure(bvm *vm)
{
    /* for better performance, we operate the upvalues
     * directly without using by the stack. */
    bntvclos *func = var_toobj(vm->cf->func);
    bvalue *uv0 = be_ntvclos_upval(func, 0)->value; /* list value */
    bvalue *uv1 = be_ntvclos_upval(func, 1)->value; /* iter value */
    bvalue *next = cast(bvalue*, var_toobj(uv1)) + 1;
    blist *list = var_toobj(uv0);
    if (next >= be_list_end(list)) {
        be_stop_iteration(vm);
    }
    var_toobj(uv1) = next; /* set upvale[1] (iter value) */
    /* push next value to top */
    var_setval(vm->top, next);
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

static int m_connect(bvm *vm)
{
    int argc = be_top(vm);
    if (argc >= 2) {
        be_getmember(vm, 1, ".p");
        be_pushvalue(vm, 2);
        be_data_push(vm, -2);
        be_pop(vm, argc + 1);
    }
    be_return(vm); /* return self */
}

static int m_merge(bvm *vm)
{
    int argc = be_top(vm);
    if (argc >= 2) {
        be_newobject(vm, "list"); /* stack contains instance and .p */
        be_getmember(vm, 1, ".p");
        be_data_merge(vm, -2);
        be_getmember(vm, 2, ".p");
        if (!be_islist(vm, -1)) {
            be_raise(vm, "type_error", "operand must be a list");
        }
        be_data_merge(vm, -3);
        be_pop(vm, 3);
    }
    be_return(vm); /* return self */
}

static void connect(bvm *vm, bvalue *begin, bvalue *end)
{
    size_t l0 = be_strlen(vm, -1), len = l0;
    char *buf, *p;
    bvalue *it;
    for (it = begin; it < end; ++it) {
        len += str_len(var_tostr(it));
    }
    buf = be_pushbuffer(vm, len);
    memcpy(buf, be_tostring(vm, -2), l0);
    p = buf + l0;
    for (it = begin; it < end; ++it) {
        bstring *s = var_tostr(it);
        size_t l = str_len(s);
        memcpy(p, str(s), l);
        p += l;
    }
    be_pushstring(vm, buf);
    be_moveto(vm, -1, -3);
    be_pop(vm, 2);
}

static void list_concat(bvm *vm, blist *list)
{
    bvalue *it, *begin = be_list_data(list);
    bvalue *end = be_list_end(list);
    be_pushstring(vm, ""); /* push a empty string */
    for (it = begin; it < end;) {
        for (; it < end && var_isstr(it); ++it);
        connect(vm, begin, it); /* connect string list */
        if (it < end) {
            /* connect other value */
            var_setval(vm->top, it);
            be_incrtop(vm);
            be_val2str(vm, -1);
            be_strconcat(vm, -2);
            be_pop(vm, 1);
            begin = ++it;
        }
    }
}

static int m_concat(bvm *vm)
{
    bvalue *value;
    be_getmember(vm, 1, ".p");
    list_check_data(vm, 1);
    value = be_indexof(vm, -1);
    list_concat(vm, var_toobj(value));
    be_return(vm);
}

static int m_reverse(bvm *vm)
{
    int top = be_top(vm);
    be_getmember(vm, 1, ".p");
    list_check_data(vm, 1);
    be_data_reverse(vm, -1);
    be_pop(vm, top);
    be_return(vm);
}

static int m_copy(bvm *vm)
{
    be_getmember(vm, 1, ".p");
    list_check_data(vm, 1);
    be_getbuiltin(vm, "list");
    be_copy(vm, -2);
    be_call(vm, 1);
    be_pop(vm, 1);
    be_return(vm);
}

static int list_equal(bvm *vm, bbool iseq)
{
    int i, j, res;
    bbool (*eqfunc)(bvm*) = iseq ? be_iseq : be_isneq;
    be_getmember(vm, 1, ".p");
    be_getmember(vm, 2, ".p");
    i = be_data_size(vm, -2);
    j = be_data_size(vm, -1);
    if (i == j) {
        res = iseq;
        for (i = 0; res == iseq && i < j; ++i) {
            be_pushint(vm, i);
            be_getindex(vm, -3);
            be_pushint(vm, i);
            be_getindex(vm, -4);
            be_remove(vm, -2);
            res = eqfunc(vm);
            be_pop(vm, 3);
        }
    } else {
        res = !iseq;
    }
    be_pushbool(vm, res);
    be_return(vm);
}

static int m_equal(bvm *vm)
{
    return list_equal(vm, btrue);
}

static int m_nequal(bvm *vm)
{
    return list_equal(vm, bfalse);
}

#if !BE_USE_PRECOMPILED_OBJECT
void be_load_listlib(bvm *vm)
{
    static const bnfuncinfo members[] = {
        { ".p", NULL },
        { "init", m_init },
        { "tostring", m_tostring },
        { "push", m_push },
        { "pop", m_pop },
        { "insert", m_insert },
        { "remove", m_remove },
        { "item", m_item },
        { "find", m_find },
        { "setitem", m_setitem },
        { "size", m_size },
        { "resize", m_resize },
        { "clear", m_clear },
        { "iter", m_iter },
        { "concat", m_concat },
        { "reverse", m_reverse },
        { "copy", m_copy },
        { "..", m_connect },
        { "+", m_merge },
        { "==", m_equal },
        { "!=", m_nequal },
        { NULL, NULL }
    };
    be_regclass(vm, "list", members);
}
#else
/* @const_object_info_begin
class be_class_list (scope: global, name: list) {
    .p, var
    init, func(m_init)
    tostring, func(m_tostring)
    push, func(m_push)
    pop, func(m_pop)
    insert, func(m_insert)
    remove, func(m_remove)
    item, func(m_item)
    find, func(m_find)
    setitem, func(m_setitem)
    size, func(m_size)
    resize, func(m_resize)
    clear, func(m_clear)
    iter, func(m_iter)
    concat, func(m_concat)
    reverse, func(m_reverse)
    copy, func(m_copy)
    .., func(m_connect)
    +, func(m_merge)
    ==, func(m_equal)
    !=, func(m_nequal)
}
@const_object_info_end */
#include "../generate/be_fixed_be_class_list.h"
#endif
