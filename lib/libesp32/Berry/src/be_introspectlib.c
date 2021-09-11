/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#include "be_object.h"
#include "be_module.h"
#include "be_string.h"
#include "be_vector.h"
#include "be_class.h"
#include "be_debug.h"
#include "be_map.h"
#include "be_vm.h"
#include <string.h>

#if BE_USE_INTROSPECT_MODULE

#define global(vm)      ((vm)->gbldesc.global)
#define builtin(vm)     ((vm)->gbldesc.builtin)

static void dump_map_keys(bvm *vm, bmap *map)
{
    if (!map) { return; }   /* protect agains potential null pointer */
    bmapnode *node;
    bmapiter iter = be_map_iter();
    while ((node = be_map_next(map, &iter)) != NULL) {
        if (var_isstr(&node->key)) {
            bstring *s = var_tostr(&node->key);
            be_pushstring(vm, str(s));
            be_data_push(vm, -2);
            be_pop(vm, 1);
        }
    }
}

static int m_attrlist(bvm *vm)
{
    int top = be_top(vm);
    be_newobject(vm, "list");
    if (top >= 1) {
        bvalue *v = be_indexof(vm, 1);
        void *obj = var_toobj(v);
        switch (var_type(v)) {
        case BE_NIL: dump_map_keys(vm, global(vm).vtab); break;
        case BE_MODULE: dump_map_keys(vm, ((bmodule*)obj)->table); break;
        case BE_CLASS: dump_map_keys(vm, ((bclass*)obj)->members); break;
        case BE_INSTANCE: dump_map_keys(vm, ((binstance*)obj)->_class->members); break;
        default: break;
        }
    } else {    /* if no parameter, then dump globals */
        dump_map_keys(vm, global(vm).vtab);
    }
    be_pop(vm, 1);
    be_return(vm);
}

static int m_findmember(bvm *vm)
{
    int top = be_top(vm);
    if (top >= 2 && (be_isinstance(vm, 1) || be_ismodule(vm, 1) || be_isclass(vm, 1)) && be_isstring(vm, 2)) {
        be_getmember(vm, 1, be_tostring(vm, 2));
        be_return(vm);
    }
    be_return_nil(vm);
}

static int m_setmember(bvm *vm)
{
    int top = be_top(vm);
    if (top >= 3 && (be_isinstance(vm, 1) || be_ismodule(vm, 1)) && be_isstring(vm, 2)) {
        be_setmember(vm, 1, be_tostring(vm, 2));
        be_return(vm);
    }
    be_return_nil(vm);
}

/* call a function with variable number of arguments */
/* first argument is a callable object (function, closure, native function, native closure) */
/* then all subsequent arguments are pushed except the last one */
/* If the last argument is a 'list', then all elements are pushed as arguments */
/* otherwise the last argument is pushed as well */
static int m_vcall(bvm *vm)
{
    int top = be_top(vm);
    if (top >= 1 && be_isfunction(vm, 1)) {
        size_t arg_count = top - 1;  /* we have at least 'top - 1' arguments */
        /* test if last argument is a list */

        if (top > 1 && be_isinstance(vm, top) && be_getmember(vm, top, ".p") && be_islist(vm, top + 1)) {
            int32_t list_size = be_data_size(vm, top + 1);

            if (list_size > 0) {
                be_stack_require(vm, list_size + 3);   /* make sure we don't overflow the stack */
                for (int i = 0; i < list_size; i++) {
                    be_pushnil(vm);
                }
                be_moveto(vm, top + 1, top + 1 + list_size);
                be_moveto(vm, top, top + list_size);

                be_refpush(vm, -2);
                be_pushiter(vm, -1);
                while (be_iter_hasnext(vm, -2)) {
                    be_iter_next(vm, -2);
                    be_moveto(vm, -1, top);
                    top++;
                    be_pop(vm, 1);
                }
                be_pop(vm, 1);  /* remove iterator */
                be_refpop(vm);
            }
            be_pop(vm, 2);
            arg_count = arg_count - 1 + list_size;
        }
        /* actual call */
        be_call(vm, arg_count);
        /* remove args */
        be_pop(vm, arg_count);
        /* return value */

        be_return(vm);
    }
    be_raise(vm, "value_error", "first argument must be a function");
    be_return_nil(vm);
}

#if !BE_USE_PRECOMPILED_OBJECT
be_native_module_attr_table(introspect) {
    be_native_module_function("members", m_attrlist),

    be_native_module_function("get", m_findmember),
    be_native_module_function("set", m_setmember),
    be_native_module_function("vcall", m_vcall),
};

be_define_native_module(introspect, NULL);
#else
/* @const_object_info_begin
module introspect (scope: global, depend: BE_USE_INTROSPECT_MODULE) {
    members, func(m_attrlist)

    get, func(m_findmember)
    set, func(m_setmember)
    vcall, func(m_vcall)
}
@const_object_info_end */
#include "../generate/be_fixed_introspect.h"
#endif

#endif /* BE_USE_INTROSPECT_MODULE */
