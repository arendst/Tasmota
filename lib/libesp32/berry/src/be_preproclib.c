/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#include "be_object.h"
#include "berry.h"

#if BE_USE_PREPROCESSOR

#include "be_map.h"
#include "be_string.h"
#include "be_vm.h"

/* preproc.define(name [, value]) */
static int m_define(bvm *vm)
{
    int argc = be_top(vm);
    if (argc >= 1 && be_isstring(vm, 1)) {
        const char *name = be_tostring(vm, 1);
        const char *value = NULL;
        if (argc >= 2) {
            value = be_tostring(vm, 2);
        }
        be_preprocessor_define(vm, name, value);
        be_return_nil(vm);
    }
    be_raise(vm, "type_error", "name must be a string");
    be_return_nil(vm);
}

/* preproc.undef(name) */
static int m_undef(bvm *vm)
{
    if (be_top(vm) >= 1 && be_isstring(vm, 1)) {
        be_preprocessor_undef(vm, be_tostring(vm, 1));
        be_return_nil(vm);
    }
    be_raise(vm, "type_error", "name must be a string");
    be_return_nil(vm);
}

/* preproc.defined(name) -> bool */
static int m_defined(bvm *vm)
{
    if (be_top(vm) >= 1 && be_isstring(vm, 1)) {
        const char *name = be_tostring(vm, 1);
        bbool found = bfalse;
        if (vm->preprocessor) {
            bstring *key = be_newstr(vm, name);
            found = be_map_findstr(vm, vm->preprocessor, key) != NULL;
        }
        be_pushbool(vm, found);
        be_return(vm);
    }
    be_raise(vm, "type_error", "name must be a string");
    be_return_nil(vm);
}

/* preproc.value(name) -> int/string/nil */
static int m_value(bvm *vm)
{
    if (be_top(vm) >= 1 && be_isstring(vm, 1)) {
        const char *name = be_tostring(vm, 1);
        if (vm->preprocessor) {
            bstring *key = be_newstr(vm, name);
            bvalue *val = be_map_findstr(vm, vm->preprocessor, key);
            if (val) {
                if (var_isint(val)) {
                    be_pushint(vm, var_toint(val));
                } else if (var_isstr(val)) {
                    be_pushstring(vm, str(var_tostr(val)));
                } else {
                    be_pushnil(vm);
                }
                be_return(vm);
            }
        }
        be_pushnil(vm);
        be_return(vm);
    }
    be_raise(vm, "type_error", "name must be a string");
    be_return_nil(vm);
}

/* preproc.clear() */
static int m_clear(bvm *vm)
{
    be_preprocessor_clear(vm);
    be_return_nil(vm);
}

#if !BE_USE_PRECOMPILED_OBJECT
be_native_module_attr_table(preproc) {
    be_native_module_function("define", m_define),
    be_native_module_function("undef", m_undef),
    be_native_module_function("defined", m_defined),
    be_native_module_function("value", m_value),
    be_native_module_function("clear", m_clear)
};

be_define_native_module(preproc, NULL);
#else
/* @const_object_info_begin
module preproc (scope: global, depend: BE_USE_PREPROCESSOR) {
    define, func(m_define)
    undef, func(m_undef)
    defined, func(m_defined)
    value, func(m_value)
    clear, func(m_clear)
}
@const_object_info_end */
#include "../generate/be_fixed_preproc.h"
#endif

#endif /* BE_USE_PREPROCESSOR */
