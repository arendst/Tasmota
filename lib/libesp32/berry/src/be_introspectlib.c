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
#include "be_exec.h"
#include "be_gc.h"
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

static void m_findmember_protected(bvm *vm, void* data)
{
    be_getmember(vm, 1, (const char*) data);
}

static int m_findmember(bvm *vm)
{
    int top = be_top(vm);
    bbool protected = btrue; /* run protected, i.e. don't raise an exception if not found */
    if (top >= 3) {
        protected = !be_tobool(vm, 3);
    }
    if (top >= 2 && be_isstring(vm, 2)) {
        if (protected) {
            if (be_isinstance(vm, 1) || be_ismodule(vm, 1) || be_isclass(vm, 1)) {
                int ret = be_execprotected(vm, &m_findmember_protected, (void*) be_tostring(vm, 2));
                if (ret == BE_OK) {
                    be_return(vm);
                }
            }
            be_return_nil(vm);
        } else {
            /* run unprotected */
            if (be_getmember(vm, 1, be_tostring(vm, 2))) {
                be_return(vm);
            } else {
                /* not found, return module 'undefined' */
                be_getmodule(vm, "undefined");
                be_return(vm);
            }
        }
    }
    be_return_nil(vm);
}

static int m_contains(bvm *vm)
{
    bbool contains = bfalse;
    int top = be_top(vm);
    if (top >= 2 && be_isstring(vm, 2) && (be_isinstance(vm, 1) || be_ismodule(vm, 1) || be_isclass(vm, 1))) {
        if (be_getmember(vm, 1, be_tostring(vm, 2))) {
            contains = btrue;
        }
    }
    be_pushbool(vm, contains);
    be_return(vm);
}

static int m_setmember(bvm *vm)
{
    int top = be_top(vm);
    if (top >= 3 && (be_isinstance(vm, 1) || be_ismodule(vm, 1) || be_isclass(vm, 1)) && be_isstring(vm, 2)) {
        be_setmember(vm, 1, be_tostring(vm, 2));
        be_return(vm);
    }
    be_return_nil(vm);
}

static int m_toptr(bvm *vm)
{
    int top = be_top(vm);
    if (top >= 1) {
        bvalue *v = be_indexof(vm, 1);
        if (var_type(v) == BE_STRING) {
            be_pushcomptr(vm, (void*)be_tostring(vm, 1));
            be_return(vm);
        } else if (var_basetype(v) >= BE_FUNCTION || var_type(v) == BE_COMPTR) {
            be_pushcomptr(vm, var_toobj(v));
            be_return(vm);
        } else if (var_type(v) == BE_INT) {
            be_pushcomptr(vm, (void*) (intptr_t) var_toint(v));
            be_return(vm);
        } else {
            be_raise(vm, "value_error", "unsupported for this type");
        }
    }
    be_return_nil(vm);
}

static int m_fromptr(bvm *vm)
{
    int top = be_top(vm);
    if (top >= 1) {
        void* v;
        if (be_iscomptr(vm, 1)) {
            v = be_tocomptr(vm, 1);
        } else {
            v = (void*) (intptr_t) be_toint(vm, 1);
        }
        if (v) {
            bgcobject *ptr = (bgcobject*)v;
            if (var_basetype(ptr) >= BE_GCOBJECT) {
                bvalue *top = be_incrtop(vm);
                var_setobj(top, ptr->type, ptr);
            } else {
                be_raise(vm, "value_error", "unsupported for this type");
            }
            be_return(vm);
        }
    }
    be_return_nil(vm);
}

/* load module by name, like `import` would do. But don't create a global variable from it. */
static int m_getmodule(bvm *vm)
{
    int top = be_top(vm);
    if (top >= 1) {
        bvalue *v = be_indexof(vm, 1);
        if (var_isstr(v)) {
            int ret = be_module_load(vm, var_tostr(v));
            if (ret == BE_OK) {
                be_return(vm);
            }
        }
    }
    be_return_nil(vm);
}

/* set or chang the cached value for the named module, this allows monkey patching. **USE WITH CARE** */
static int m_setmodule(bvm *vm)
{
    int top = be_top(vm);
    if (top >= 2) {
        bvalue *v = be_indexof(vm, 1);
        if (var_isstr(v)) {
            be_pushvalue(vm, 2);  /* ensure the second arg is at top of stack */
            be_cache_module(vm, var_tostr(v));
        }
    }
    be_return_nil(vm);
}

/* checks if the function (berry bytecode bproto only) is hinted as a method */
static int m_ismethod(bvm *vm)
{
    int top = be_top(vm);
    if (top >= 1) {
        bvalue *v = be_indexof(vm, 1);
        if (var_isclosure(v)) {
            bclosure *cl = var_toobj(v);
            bproto *pr = cl->proto;
            be_pushbool(vm, pr->varg & BE_VA_METHOD);
            be_return(vm);
        }
    }
    be_return_nil(vm);
}

static int m_name(bvm *vm)
{
    int top = be_top(vm);
    if (top >= 1) {
        bvalue *v = be_indexof(vm, 1);
        const char* name = NULL;
        switch (var_type(v)) {
            case BE_CLOSURE:
                name = str(((bclosure*) var_toobj(v))->proto->name);
                break;
            case BE_CLASS:
                name = str(((bclass*) var_toobj(v))->name);
                break;
            case BE_MODULE:
                name = be_module_name(var_toobj(v));
                break;
        }
        if (name) {
            be_pushstring(vm, name);
            be_return(vm);
        }
    }
    be_return_nil(vm);
}

#if !BE_USE_PRECOMPILED_OBJECT
be_native_module_attr_table(introspect) {
    be_native_module_function("members", m_attrlist),

    be_native_module_function("get", m_findmember),
    be_native_module_function("set", m_setmember),
    be_native_module_function("contains", m_contains),

    be_native_module_function("module", m_getmodule),
    be_native_module_function("setmodule", m_setmodule),

    be_native_module_function("toptr", m_toptr),
    be_native_module_function("fromptr", m_fromptr),

    be_native_module_function("name", m_name),

    be_native_module_function("ismethod", m_ismethod),
};

be_define_native_module(introspect, NULL);
#else
/* @const_object_info_begin
module introspect (scope: global, depend: BE_USE_INTROSPECT_MODULE) {
    members, func(m_attrlist)

    get, func(m_findmember)
    set, func(m_setmember)
    contains, func(m_contains)

    module, func(m_getmodule)
    setmodule, func(m_setmodule)

    toptr, func(m_toptr)
    fromptr, func(m_fromptr)

    name, func(m_name)

    ismethod, func(m_ismethod)
}
@const_object_info_end */
#include "../generate/be_fixed_introspect.h"
#endif

#endif /* BE_USE_INTROSPECT_MODULE */
