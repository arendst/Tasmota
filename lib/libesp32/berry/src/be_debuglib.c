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
#include <string.h>

#if BE_USE_DEBUG_MODULE

static void dump_map(bmap *map)
{
    bmapnode *node;
    bmapiter iter = be_map_iter();
    while ((node = be_map_next(map, &iter)) != NULL) {
        if (var_isstr(&node->key)) {
            bstring *s = var_tostr(&node->key);
            be_writestring("\t");
            be_writebuffer(str(s), str_len(s));
            be_writestring(": <");
            be_writestring(be_vtype2str(&node->value));
            be_writestring(">\n");
        }
    }
}

static void dump_module(bmodule *module)
{
    dump_map(module->table);
}

static void dump_class(bclass *class)
{
    if (class->members) {
        dump_map(class->members);
    }
}

static void dump_instanse(binstance *ins)
{
    dump_class(ins->_class);
}

static void dump_value(bvalue *value)
{
    be_writestring("value type <");
    be_writestring(be_vtype2str(value));
    be_writestring(">, attributes:\n");
}

static int m_attrdump(bvm *vm)
{
    if (be_top(vm) >= 1) {
        bvalue *v = be_indexof(vm, 1);
        void *obj = var_toobj(v);
        dump_value(v);
        switch (var_type(v)) {
        case BE_MODULE: dump_module(obj); break;
        case BE_CLASS: dump_class(obj); break;
        case BE_INSTANCE: dump_instanse(obj); break;
        default: break;
        }
    }
    be_return_nil(vm);
}

static int m_codedump(bvm *vm)
{
    if (be_top(vm) >= 1) {
        bvalue *v = be_indexof(vm, 1);
        if (var_isclosure(v)) {
            be_dumpclosure(var_toobj(v));
        }
    }
    be_return_nil(vm);
}

static int m_gcdebug(bvm *vm) {
    int argc = be_top(vm);
    if (argc >= 1 && be_isbool(vm, 1)) {
        if (be_tobool(vm, 1)) {
            comp_set_gc_debug(vm);
        } else {
            comp_clear_gc_debug(vm);
        }
    }
    be_pushbool(vm, comp_is_gc_debug(vm));
    be_return(vm);
}

static int m_traceback(bvm *vm)
{
    be_tracestack(vm);
    be_return_nil(vm);
}

static int m_caller(bvm *vm)
{
    int depth = 1;
    if (be_top(vm) >= 1 && be_isint(vm, 1)) {
        depth = be_toint(vm, 1);
        if (depth < 0) {
            depth = -depth;         /* take absolute value */
        }
    }
    bcallframe *cf = (bcallframe*)be_stack_top(&vm->callstack) - depth;
    bcallframe *base = be_stack_base(&vm->callstack);
    if (cf >= base) {
        bvalue *reg = be_incrtop(vm);
        var_setval(reg, cf->func);
        be_return(vm);
    } else {
        be_return_nil(vm);
    }
}

#if BE_USE_DEBUG_HOOK
static int m_sethook(bvm *vm)
{
    if (be_top(vm) >= 2) {
        be_pushvalue(vm, 1);
        be_sethook(vm, be_tostring(vm, 2));
    } else {
        be_sethook(vm, NULL);
    }
    be_return_nil(vm);
}
#endif

static int m_top(bvm *vm)
{
    bint top = vm->top - vm->stack + 1;
    be_pushint(vm, top);
    be_return(vm);
}

static int m_calldepth(bvm *vm)
{
    bint depth = be_stack_count(&vm->callstack);
    be_pushint(vm, depth);
    be_return(vm);
}

#if BE_DEBUG_VAR_INFO
static int v_getname(bvm *vm, bbool(*getter)(bvm *vm, int, int))
{
    int index, level = 1;
    if (be_top(vm) < 1)
        be_raise(vm, "value_error", "too few arguments");
    if (!be_isint(vm, 1) || (be_top(vm) >= 2 && !be_isint(vm, 2)))
        be_raise(vm, "value_error", "invalid argument(s) value");
    if (be_top(vm) >= 2)
        level = be_toindex(vm, 2);
    index = be_toindex(vm, 1);
    if (index < 0)
        be_raise(vm, "value_error", "variable index cannot be less than 0");
    if (level < 1 || level >= be_stack_count(&vm->callstack))
        be_raise(vm, "value_error", "invalid call depth level");
    if (getter(vm, level + 1, index)) {
        be_return(vm);
    }
    be_return_nil(vm);
}

static int m_varname(bvm *vm)
{
    return v_getname(vm, be_debug_varname);
}

static int m_upvname(bvm *vm)
{
    return v_getname(vm, be_debug_upvname);
}
#endif


#if BE_USE_PERF_COUNTERS
static void map_insert(bvm *vm, const char *key, int value)
{
    be_pushstring(vm, key);
    be_pushint(vm, value);
    be_data_insert(vm, -3);
    be_pop(vm, 2);
}

static int m_counters(bvm *vm)
{
    be_newobject(vm, "map");
    map_insert(vm, "instruction", vm->counter_ins);
    map_insert(vm, "vmenter", vm->counter_enter);
    map_insert(vm, "call", vm->counter_call);
    map_insert(vm, "get", vm->counter_get);
    map_insert(vm, "set", vm->counter_set);
    map_insert(vm, "getgbl", vm->counter_get_global);
    map_insert(vm, "try", vm->counter_try);
    map_insert(vm, "raise", vm->counter_exc);
    map_insert(vm, "objects", vm->counter_gc_kept);
    map_insert(vm, "mem_alloc", vm->counter_mem_alloc);
    map_insert(vm, "mem_free", vm->counter_mem_free);
    map_insert(vm, "mem_realloc", vm->counter_mem_realloc);
    be_pop(vm, 1);
    be_return(vm);
}
#endif

static int m_allocs(bvm *vm) {
#if BE_USE_PERF_COUNTERS
    be_pushint(vm, vm->counter_mem_alloc);
    be_return(vm);
#else
    be_return_nil(vm);
#endif
}

static int m_frees(bvm *vm) {
#if BE_USE_PERF_COUNTERS
    be_pushint(vm, vm->counter_mem_free);
    be_return(vm);
#else
    be_return_nil(vm);
#endif
}

static int m_reallocs(bvm *vm) {
#if BE_USE_PERF_COUNTERS
    be_pushint(vm, vm->counter_mem_realloc);
    be_return(vm);
#else
    be_return_nil(vm);
#endif
}

#if !BE_USE_PRECOMPILED_OBJECT
be_native_module_attr_table(debug) {
    be_native_module_function("attrdump", m_attrdump),
    be_native_module_function("codedump", m_codedump),
    be_native_module_function("traceback", m_traceback),
#if BE_USE_DEBUG_HOOK
    be_native_module_function("sethook", m_sethook),
#endif
#if BE_USE_PERF_COUNTERS
    be_native_module_function("counters", m_counters),
#endif
    be_native_module_function("calldepth", m_calldepth),
    be_native_module_function("top", m_top),
#if BE_DEBUG_VAR_INFO
    be_native_module_function("varname", m_varname),
    be_native_module_function("upvname", m_upvname)
#endif
    be_native_module_function("caller", m_caller),
    be_native_module_function("gcdebug", m_gcdebug)
};

be_define_native_module(debug, NULL);
#else
/* @const_object_info_begin
module debug (scope: global, depend: BE_USE_DEBUG_MODULE) {
    attrdump, func(m_attrdump)
    codedump, func(m_codedump)
    traceback, func(m_traceback)
    sethook, func(m_sethook), BE_USE_DEBUG_HOOK
    counters, func(m_counters), BE_USE_PERF_COUNTERS
    calldepth, func(m_calldepth)
    top, func(m_top)
    varname, func(m_varname), BE_DEBUG_VAR_INFO
    upvname, func(m_upvname), BE_DEBUG_VAR_INFO
    caller, func(m_caller)
    // individual counters
    allocs, func(m_allocs)
    frees, func(m_frees)
    reallocs, func(m_reallocs)
    // GC debug mode
    gcdebug, func(m_gcdebug)
}
@const_object_info_end */
#include "../generate/be_fixed_debug.h"
#endif

#endif /* BE_USE_DEBUG_MODULE */
