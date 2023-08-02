/********************************************************************
** Copyright (c) 2018-2021 Guan Wenliang & Stephan Hadinger
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
#include "be_var.h"
#include <string.h>

#if BE_USE_GLOBAL_MODULE

#define global(vm)      ((vm)->gbldesc.global)

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

static int m_globals(bvm *vm)
{
    be_newobject(vm, "list");
    dump_map_keys(vm, global(vm).vtab);
    be_pop(vm, 1);
    be_return(vm);
}

static int m_contains(bvm *vm)
{
    int top = be_top(vm);
    if (top >= 1 && be_isstring(vm, 1)) {
        const char * name = be_tostring(vm, 1);
        int idx = be_global_find(vm, be_newstr(vm, name));
        be_pushbool(vm, idx > -1);
        be_return(vm);
    }
    be_return_nil(vm);
}

static int m_findglobal(bvm *vm)
{
    int top = be_top(vm);
    if (top >= 1 && be_isstring(vm, 1)) {
        const char * name = be_tostring(vm, 1);
        be_getglobal(vm, name);
        be_return(vm);
    }
    be_return_nil(vm);
}

static int m_setglobal(bvm *vm)
{
    int top = be_top(vm);
    if (top >= 2 && be_isstring(vm, 1)) {
        const char * name = be_tostring(vm, 1);
        be_setglobal(vm, name);
    }
    be_return_nil(vm);
}

#if !BE_USE_PRECOMPILED_OBJECT
be_native_module_attr_table(global) {
    be_native_module_function("()", m_globals),
    be_native_module_function("contains", m_contains),
    be_native_module_function("member", m_findglobal),
    be_native_module_function("setmember", m_setglobal),
};

be_define_native_module(global, NULL);
#else
/* @const_object_info_begin
module global (scope: global, depend: BE_USE_GLOBAL_MODULE) {
    (), func(m_globals)
    contains, func(m_contains)
    member, func(m_findglobal)
    setmember, func(m_setglobal)
}
@const_object_info_end */
#include "../generate/be_fixed_global.h"
#endif

#endif /* BE_USE_GLOBAL_MODULE */
