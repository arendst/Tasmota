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

#if !BE_USE_PRECOMPILED_OBJECT
be_native_module_attr_table(introspect) {
    be_native_module_function("members", m_attrlist),

    be_native_module_function("get", m_findmember),
    be_native_module_function("set", m_setmember),
};

be_define_native_module(introspect, NULL);
#else
/* @const_object_info_begin
module introspect (scope: global, depend: BE_USE_INTROSPECT_MODULE) {
    members, func(m_attrlist)

    get, func(m_findmember)
    set, func(m_setmember)
}
@const_object_info_end */
#include "../generate/be_fixed_introspect.h"
#endif

#endif /* BE_USE_INTROSPECT_MODULE */
