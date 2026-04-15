"""
berry_port/be_libs.py — Port of src/be_libs.c + default/be_modtab.c

Library loader: registers all built-in libraries (baselib, listlib,
maplib, rangelib, byteslib) and native modules (string, json, math,
solidify, introspect, global, debug, gc, strict, undefined).

Original C code is included as comments.
"""

# /********************************************************************
# ** Copyright (c) 2018-2020 Guan Wenliang
# ** This file is part of the Berry default interpreter.
# ** skiars@qq.com, https://github.com/Skiars/berry
# ** See Copyright Notice in the LICENSE file or at
# ** https://github.com/Skiars/berry/blob/master/LICENSE
# ********************************************************************/

# ============================================================================
# Imports — lazy to avoid circular dependencies
# ============================================================================

def _lazy_be_api():
    import berry_port.be_api as m; return m

def _lazy_be_object():
    import berry_port.be_object as m; return m

def _lazy_be_module():
    import berry_port.be_module as m; return m

def _lazy_be_baselib():
    import berry_port.be_baselib as m; return m

def _lazy_be_listlib():
    import berry_port.be_listlib as m; return m

def _lazy_be_maplib():
    import berry_port.be_maplib as m; return m

def _lazy_be_rangelib():
    import berry_port.be_rangelib as m; return m

def _lazy_be_byteslib():
    import berry_port.be_byteslib as m; return m

def _lazy_be_int64lib():
    import berry_port.be_int64lib as m; return m

def _lazy_be_strlib():
    import berry_port.be_strlib as m; return m

def _lazy_be_jsonlib():
    import berry_port.be_jsonlib as m; return m

def _lazy_be_mathlib():
    import berry_port.be_mathlib as m; return m

def _lazy_be_solidifylib():
    import berry_port.be_solidifylib as m; return m

def _lazy_be_oslib():
    import berry_port.be_oslib as m; return m

def _lazy_be_relib():
    import berry_port.be_relib as m; return m

def _lazy_be_syslib():
    import berry_port.be_syslib as m; return m

def _lazy_be_debug():
    import berry_port.be_debug as m; return m

def _lazy_be_gc():
    import berry_port.be_gc as m; return m

def _lazy_be_string():
    import berry_port.be_string as m; return m

def _lazy_be_map():
    import berry_port.be_map as m; return m

def _lazy_be_var():
    import berry_port.be_var as m; return m

def _lazy_be_exec():
    import berry_port.be_exec as m; return m

def _lazy_be_class():
    import berry_port.be_class as m; return m

def _lazy_be_func():
    import berry_port.be_func as m; return m

def _lazy_be_vector():
    import berry_port.be_vector as m; return m

def _lazy_be_vm():
    import berry_port.be_vm as m; return m


# ============================================================================
# Native module function implementations
# ============================================================================
# These are the module-level functions for modules that don't have their own
# dedicated Python file (debug, gc, introspect, global, strict, undefined).
# They mirror the C implementations in be_debuglib.c, be_gclib.c, etc.


# ============================================================================
# gc module functions (from be_gclib.c)
# ============================================================================

# static int m_allocated(bvm *vm)
# {
#     size_t count = be_gc_memcount(vm);
#     if (count < 0x80000000) {
#         be_pushint(vm, (bint)count);
#     } else {
#         be_pushreal(vm, (breal)count);
#     }
#     be_return(vm);
# }
def _gc_m_allocated(vm):
    be_api = _lazy_be_api()
    be_gc = _lazy_be_gc()
    count = be_gc.be_gc_memcount(vm)
    if count < 0x80000000:
        be_api.be_pushint(vm, count)
    else:
        be_api.be_pushreal(vm, float(count))
    return be_api.be_returnvalue(vm)

# static int m_collect(bvm *vm)
# {
#     be_gc_collect(vm);
#     be_return_nil(vm);
# }
def _gc_m_collect(vm):
    be_api = _lazy_be_api()
    be_gc = _lazy_be_gc()
    be_gc.be_gc_collect(vm)
    return be_api.be_returnnilvalue(vm)


# ============================================================================
# global module functions (from be_globallib.c)
# ============================================================================

# static void dump_map_keys(bvm *vm, bmap *map)
def _global_dump_map_keys(vm, mp):
    be_api = _lazy_be_api()
    be_obj = _lazy_be_object()
    be_mp = _lazy_be_map()
    be_str = _lazy_be_string()
    if mp is None:
        return
    iter_ref = [be_obj.be_map_iter()]
    while True:
        node, iter_ref = be_mp.be_map_next(mp, iter_ref)
        if node is None:
            break
        if be_obj.var_isstr(node.key):
            # check if the global was not undefined/removed
            idx = be_obj.var_toidx(node.value)
            if idx >= 0:
                s = be_obj.var_tostr(node.key)
                be_api.be_pushstring(vm, be_str.be_str2cstr(s))
                be_api.be_data_push(vm, -2)
                be_api.be_pop(vm, 1)

# static int m_globals(bvm *vm)
# {
#     be_newobject(vm, "list");
#     dump_map_keys(vm, global(vm).vtab);
#     be_pop(vm, 1);
#     be_return(vm);
# }
def _global_m_globals(vm):
    be_api = _lazy_be_api()
    be_api.be_newobject(vm, "list")
    _global_dump_map_keys(vm, vm.gbldesc.global_.vtab)
    be_api.be_pop(vm, 1)
    return be_api.be_returnvalue(vm)

# static int m_contains(bvm *vm)
def _global_m_contains(vm):
    be_api = _lazy_be_api()
    be_var = _lazy_be_var()
    be_str = _lazy_be_string()
    if be_api.be_top(vm) >= 1 and be_api.be_isstring(vm, 1):
        name = be_api.be_tostring(vm, 1)
        idx = be_var.be_global_find(vm, be_str.be_newstr(vm, name))
        be_api.be_pushbool(vm, idx >= 0)
        return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)

# static int m_findglobal(bvm *vm)
def _global_m_findglobal(vm):
    be_api = _lazy_be_api()
    if be_api.be_top(vm) >= 1 and be_api.be_isstring(vm, 1):
        name = be_api.be_tostring(vm, 1)
        be_api.be_getglobal(vm, name)
        return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)

# static int m_setglobal(bvm *vm)
def _global_m_setglobal(vm):
    be_api = _lazy_be_api()
    if be_api.be_top(vm) >= 2 and be_api.be_isstring(vm, 1):
        name = be_api.be_tostring(vm, 1)
        be_api.be_setglobal(vm, name)
    return be_api.be_returnnilvalue(vm)

# static int m_undef(bvm *vm)
def _global_m_undef(vm):
    be_api = _lazy_be_api()
    be_var = _lazy_be_var()
    be_str = _lazy_be_string()
    if be_api.be_top(vm) >= 1 and be_api.be_isstring(vm, 1):
        be_var.be_global_undef(vm, be_str.be_newstr(vm, be_api.be_tostring(vm, 1)))
    return be_api.be_returnnilvalue(vm)


# ============================================================================
# introspect module functions (from be_introspectlib.c)
# ============================================================================

def _introspect_dump_map_keys(vm, mp):
    """Dump map keys as strings into the list at top of stack."""
    be_api = _lazy_be_api()
    be_obj = _lazy_be_object()
    be_mp = _lazy_be_map()
    be_str = _lazy_be_string()
    if mp is None:
        return
    iter_ref = [be_obj.be_map_iter()]
    while True:
        node, iter_ref = be_mp.be_map_next(mp, iter_ref)
        if node is None:
            break
        if be_obj.var_isstr(node.key):
            s = be_obj.var_tostr(node.key)
            be_api.be_pushstring(vm, be_str.be_str2cstr(s))
            be_api.be_data_push(vm, -2)
            be_api.be_pop(vm, 1)

# static int m_attrlist(bvm *vm)
def _introspect_m_attrlist(vm):
    be_api = _lazy_be_api()
    be_obj = _lazy_be_object()
    top = be_api.be_top(vm)
    be_api.be_newobject(vm, "list")
    if top >= 1:
        v = vm.stack[be_api.be_indexof(vm, 1)]
        obj = be_obj.var_toobj(v)
        vtype = be_obj.var_type(v)
        if vtype == be_obj.BE_NIL:
            _introspect_dump_map_keys(vm, vm.gbldesc.global_.vtab)
        elif vtype == be_obj.BE_MODULE:
            _introspect_dump_map_keys(vm, obj.table)
        elif vtype == be_obj.BE_CLASS:
            _introspect_dump_map_keys(vm, obj.members)
        elif vtype == be_obj.BE_INSTANCE:
            _introspect_dump_map_keys(vm, obj._class.members)
    else:
        _introspect_dump_map_keys(vm, vm.gbldesc.global_.vtab)
    be_api.be_pop(vm, 1)
    return be_api.be_returnvalue(vm)

# static int m_findmember(bvm *vm)
def _introspect_m_findmember(vm):
    be_api = _lazy_be_api()
    be_exec = _lazy_be_exec()
    be_obj = _lazy_be_object()
    top = be_api.be_top(vm)
    protected = True
    if top >= 3:
        protected = not be_api.be_tobool(vm, 3)
    if top >= 2 and be_api.be_isstring(vm, 2):
        if protected:
            if be_api.be_isinstance(vm, 1) or be_api.be_ismodule(vm, 1) or be_api.be_isclass(vm, 1):
                def _do_getmember(vm2, data):
                    be_api.be_getmember(vm2, 1, data)
                ret = be_exec.be_execprotected(vm, _do_getmember, be_api.be_tostring(vm, 2))
                if ret == be_obj.BE_OK:
                    return be_api.be_returnvalue(vm)
            return be_api.be_returnnilvalue(vm)
        else:
            # run unprotected
            if be_api.be_getmember(vm, 1, be_api.be_tostring(vm, 2)):
                return be_api.be_returnvalue(vm)
            else:
                # not found, return module 'undefined'
                # be_getmodule is not in the public API; use module_load
                be_mod = _lazy_be_module()
                be_str = _lazy_be_string()
                ret = be_mod.be_module_load(vm, be_str.be_newstr(vm, "undefined"))
                if ret == be_obj.BE_OK:
                    return be_api.be_returnvalue(vm)
                return be_api.be_returnnilvalue(vm)
    return be_api.be_returnnilvalue(vm)

# static int m_contains(bvm *vm)
def _introspect_m_contains(vm):
    be_api = _lazy_be_api()
    contains = False
    top = be_api.be_top(vm)
    if (top >= 2 and be_api.be_isstring(vm, 2) and
            (be_api.be_isinstance(vm, 1) or be_api.be_ismodule(vm, 1) or be_api.be_isclass(vm, 1))):
        if be_api.be_getmember(vm, 1, be_api.be_tostring(vm, 2)):
            contains = True
    be_api.be_pushbool(vm, contains)
    return be_api.be_returnvalue(vm)

# static int m_setmember(bvm *vm)
def _introspect_m_setmember(vm):
    be_api = _lazy_be_api()
    top = be_api.be_top(vm)
    if (top >= 3 and
            (be_api.be_isinstance(vm, 1) or be_api.be_ismodule(vm, 1) or be_api.be_isclass(vm, 1)) and
            be_api.be_isstring(vm, 2)):
        be_api.be_setmember(vm, 1, be_api.be_tostring(vm, 2))
        return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)

# static int m_toptr(bvm *vm)
def _introspect_m_toptr(vm):
    be_api = _lazy_be_api()
    be_obj = _lazy_be_object()
    top = be_api.be_top(vm)
    if top >= 1:
        v = vm.stack[be_api.be_indexof(vm, 1)]
        vtype = be_obj.var_type(v)
        if vtype == be_obj.BE_STRING:
            be_api.be_pushcomptr(vm, be_api.be_tostring(vm, 1))
            return be_api.be_returnvalue(vm)
        elif be_obj.var_basetype(v) >= be_obj.BE_FUNCTION or vtype == be_obj.BE_COMPTR:
            be_api.be_pushcomptr(vm, be_obj.var_toobj(v))
            return be_api.be_returnvalue(vm)
        elif vtype == be_obj.BE_INT:
            be_api.be_pushcomptr(vm, be_obj.var_toint(v))
            return be_api.be_returnvalue(vm)
        else:
            be_api.be_raise(vm, "value_error", "unsupported for this type")
    return be_api.be_returnnilvalue(vm)

# static int m_solidified(bvm *vm)
def _introspect_m_solidified(vm):
    be_api = _lazy_be_api()
    be_obj = _lazy_be_object()
    top = be_api.be_top(vm)
    if top >= 1:
        v = vm.stack[be_api.be_indexof(vm, 1)]
        if be_obj.var_basetype(v) >= be_obj.BE_FUNCTION or be_obj.var_type(v) == be_obj.BE_COMPTR:
            obj = be_obj.var_toobj(v)
            isconst = be_obj.gc_isconst(obj) if hasattr(obj, 'marked') else False
            be_api.be_pushbool(vm, isconst)
            return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)

# static int m_fromptr(bvm *vm)
def _introspect_m_fromptr(vm):
    be_api = _lazy_be_api()
    be_obj = _lazy_be_object()
    top = be_api.be_top(vm)
    if top >= 1:
        if be_api.be_iscomptr(vm, 1):
            v = be_api.be_tocomptr(vm, 1)
        else:
            v = be_api.be_toint(vm, 1)
        if v is not None and v:
            # In the Python port, comptr values that are gc objects
            # can be restored. This is a best-effort translation.
            if hasattr(v, 'type') and hasattr(v, 'marked'):
                top_val = be_api.be_incrtop(vm)
                be_obj.var_setobj(top_val, v.type, v)
            else:
                be_api.be_raise(vm, "value_error", "unsupported for this type")
            return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)

# static int m_getmodule(bvm *vm)
def _introspect_m_getmodule(vm):
    be_api = _lazy_be_api()
    be_obj = _lazy_be_object()
    be_mod = _lazy_be_module()
    top = be_api.be_top(vm)
    if top >= 1:
        v = vm.stack[be_api.be_indexof(vm, 1)]
        if be_obj.var_isstr(v):
            no_cache = False
            if top >= 2:
                no_cache = be_api.be_tobool(vm, 2)
            ret = be_mod.be_module_load_nocache(vm, be_obj.var_tostr(v), no_cache)
            if ret == be_obj.BE_OK:
                return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)

# static int m_setmodule(bvm *vm)
def _introspect_m_setmodule(vm):
    be_api = _lazy_be_api()
    be_obj = _lazy_be_object()
    be_mod = _lazy_be_module()
    top = be_api.be_top(vm)
    if top >= 2:
        v = vm.stack[be_api.be_indexof(vm, 1)]
        if be_obj.var_isstr(v):
            be_api.be_pushvalue(vm, 2)
            be_mod.be_cache_module(vm, be_obj.var_tostr(v))
    return be_api.be_returnnilvalue(vm)

# static int m_ismethod(bvm *vm)
def _introspect_m_ismethod(vm):
    be_api = _lazy_be_api()
    be_obj = _lazy_be_object()
    top = be_api.be_top(vm)
    if top >= 1:
        v = vm.stack[be_api.be_indexof(vm, 1)]
        if be_obj.var_isclosure(v):
            cl = be_obj.var_toobj(v)
            pr = cl.proto
            be_api.be_pushbool(vm, bool(pr.varg & be_obj.BE_VA_METHOD))
            return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)

# static int m_name(bvm *vm)
def _introspect_m_name(vm):
    be_api = _lazy_be_api()
    be_obj = _lazy_be_object()
    be_str = _lazy_be_string()
    be_mod = _lazy_be_module()
    top = be_api.be_top(vm)
    if top >= 1:
        v = vm.stack[be_api.be_indexof(vm, 1)]
        name = None
        vtype = be_obj.var_type(v)
        if vtype == be_obj.BE_CLOSURE:
            cl = be_obj.var_toobj(v)
            name = be_str.be_str2cstr(cl.proto.name)
        elif vtype == be_obj.BE_CLASS:
            cls = be_obj.var_toobj(v)
            name = be_str.be_str2cstr(cls.name)
        elif vtype == be_obj.BE_MODULE:
            name = be_mod.be_module_name(be_obj.var_toobj(v))
        if name is not None:
            be_api.be_pushstring(vm, name)
            return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)


# ============================================================================
# debug module functions (from be_debuglib.c)
# ============================================================================

def _debug_dump_map(vm, mp):
    """Dump map contents for attrdump."""
    be_api = _lazy_be_api()
    be_obj = _lazy_be_object()
    be_mp = _lazy_be_map()
    be_str = _lazy_be_string()
    if mp is None:
        return
    iter_ref = [be_obj.be_map_iter()]
    while True:
        node, iter_ref = be_mp.be_map_next(mp, iter_ref)
        if node is None:
            break
        if be_obj.var_isstr(node.key):
            s = be_obj.var_tostr(node.key)
            name = be_str.be_str2cstr(s)
            be_api.be_writestring("\t")
            be_api.be_writestring(name)
            be_api.be_writestring(": <")
            be_vm = _lazy_be_vm()
            be_api.be_writestring(be_vm.be_vtype2str(node.value))
            be_api.be_writestring(">\n")

# static int m_attrdump(bvm *vm)
def _debug_m_attrdump(vm):
    be_api = _lazy_be_api()
    be_obj = _lazy_be_object()
    if be_api.be_top(vm) >= 1:
        v = vm.stack[be_api.be_indexof(vm, 1)]
        obj = be_obj.var_toobj(v)
        be_vm = _lazy_be_vm()
        be_api.be_writestring("value type <")
        be_api.be_writestring(be_vm.be_vtype2str(v))
        be_api.be_writestring(">, attributes:\n")
        vtype = be_obj.var_type(v)
        if vtype == be_obj.BE_MODULE:
            _debug_dump_map(vm, obj.table)
        elif vtype == be_obj.BE_CLASS:
            if obj.members:
                _debug_dump_map(vm, obj.members)
        elif vtype == be_obj.BE_INSTANCE:
            if obj._class and obj._class.members:
                _debug_dump_map(vm, obj._class.members)
    return be_api.be_returnnilvalue(vm)

# static int m_codedump(bvm *vm)
def _debug_m_codedump(vm):
    be_api = _lazy_be_api()
    be_obj = _lazy_be_object()
    be_dbg = _lazy_be_debug()
    if be_api.be_top(vm) >= 1:
        v = vm.stack[be_api.be_indexof(vm, 1)]
        if be_obj.var_isclosure(v):
            be_dbg.be_dumpclosure(be_obj.var_toobj(v))
    return be_api.be_returnnilvalue(vm)

# static int m_traceback(bvm *vm)
def _debug_m_traceback(vm):
    be_api = _lazy_be_api()
    be_dbg = _lazy_be_debug()
    be_dbg.be_tracestack(vm)
    return be_api.be_returnnilvalue(vm)

# static int m_caller(bvm *vm)
def _debug_m_caller(vm):
    be_api = _lazy_be_api()
    be_obj = _lazy_be_object()
    be_vec = _lazy_be_vector()
    depth = 1
    if be_api.be_top(vm) >= 1 and be_api.be_isint(vm, 1):
        depth = be_api.be_toint(vm, 1)
        if depth < 0:
            depth = -depth
    count = be_vec.be_stack_count(vm.callstack)
    cf_idx = count - 1 - depth
    if cf_idx >= 0:
        cf = be_vec.be_vector_at(vm.callstack, cf_idx)
        reg = be_api.be_incrtop(vm)
        func_val = cf.func if isinstance(cf.func, be_obj.bvalue) else vm.stack[cf.func]
        be_obj.var_setval(reg, func_val)
        return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)

# static int m_gcdebug(bvm *vm)
def _debug_m_gcdebug(vm):
    be_api = _lazy_be_api()
    from berry_port.be_object import COMP_GC_DEBUG
    argc = be_api.be_top(vm)
    if argc >= 1 and be_api.be_isbool(vm, 1):
        if be_api.be_tobool(vm, 1):
            # comp_set_gc_debug(vm)
            vm.compopt |= (1 << COMP_GC_DEBUG)
        else:
            # comp_clear_gc_debug(vm)
            vm.compopt &= ~(1 << COMP_GC_DEBUG)
    # comp_is_gc_debug(vm)
    be_api.be_pushbool(vm, bool(vm.compopt & (1 << COMP_GC_DEBUG)))
    return be_api.be_returnvalue(vm)

# static int m_top(bvm *vm)
def _debug_m_top(vm):
    be_api = _lazy_be_api()
    top = vm.top_idx - 0 + 1  # vm->top - vm->stack + 1
    be_api.be_pushint(vm, top)
    return be_api.be_returnvalue(vm)

# static int m_calldepth(bvm *vm)
def _debug_m_calldepth(vm):
    be_api = _lazy_be_api()
    be_vec = _lazy_be_vector()
    depth = be_vec.be_stack_count(vm.callstack)
    be_api.be_pushint(vm, depth)
    return be_api.be_returnvalue(vm)

# static int m_varname(bvm *vm)
def _debug_m_varname(vm):
    be_api = _lazy_be_api()
    be_dbg = _lazy_be_debug()
    be_vec = _lazy_be_vector()
    if be_api.be_top(vm) < 1:
        be_api.be_raise(vm, "value_error", "too few arguments")
    if not be_api.be_isint(vm, 1) or (be_api.be_top(vm) >= 2 and not be_api.be_isint(vm, 2)):
        be_api.be_raise(vm, "value_error", "invalid argument(s) value")
    level = 1
    if be_api.be_top(vm) >= 2:
        level = be_api.be_toindex(vm, 2)
    index = be_api.be_toindex(vm, 1)
    if index < 0:
        be_api.be_raise(vm, "value_error", "variable index cannot be less than 0")
    if level < 1 or level >= be_vec.be_stack_count(vm.callstack):
        be_api.be_raise(vm, "value_error", "invalid call depth level")
    if be_dbg.be_debug_varname(vm, level + 1, index):
        return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)

# static int m_upvname(bvm *vm)
def _debug_m_upvname(vm):
    be_api = _lazy_be_api()
    be_dbg = _lazy_be_debug()
    be_vec = _lazy_be_vector()
    if be_api.be_top(vm) < 1:
        be_api.be_raise(vm, "value_error", "too few arguments")
    if not be_api.be_isint(vm, 1) or (be_api.be_top(vm) >= 2 and not be_api.be_isint(vm, 2)):
        be_api.be_raise(vm, "value_error", "invalid argument(s) value")
    level = 1
    if be_api.be_top(vm) >= 2:
        level = be_api.be_toindex(vm, 2)
    index = be_api.be_toindex(vm, 1)
    if index < 0:
        be_api.be_raise(vm, "value_error", "variable index cannot be less than 0")
    if level < 1 or level >= be_vec.be_stack_count(vm.callstack):
        be_api.be_raise(vm, "value_error", "invalid call depth level")
    if be_dbg.be_debug_upvname(vm, level + 1, index):
        return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)

# static int m_counters(bvm *vm)
def _debug_m_counters(vm):
    be_api = _lazy_be_api()
    from berry_port.berry_conf import BE_USE_PERF_COUNTERS
    if not BE_USE_PERF_COUNTERS:
        return be_api.be_returnnilvalue(vm)
    be_api.be_newobject(vm, "map")
    _map_insert = lambda key, val: (
        be_api.be_pushstring(vm, key),
        be_api.be_pushint(vm, val),
        be_api.be_data_insert(vm, -3),
        be_api.be_pop(vm, 2),
    )
    _map_insert("instruction", getattr(vm, 'counter_ins', 0))
    _map_insert("vmenter", getattr(vm, 'counter_enter', 0))
    _map_insert("call", getattr(vm, 'counter_call', 0))
    _map_insert("get", getattr(vm, 'counter_get', 0))
    _map_insert("set", getattr(vm, 'counter_set', 0))
    _map_insert("getgbl", getattr(vm, 'counter_get_global', 0))
    _map_insert("try", getattr(vm, 'counter_try', 0))
    _map_insert("raise", getattr(vm, 'counter_exc', 0))
    _map_insert("objects", getattr(vm, 'counter_gc_kept', 0))
    _map_insert("mem_alloc", getattr(vm, 'counter_mem_alloc', 0))
    _map_insert("mem_free", getattr(vm, 'counter_mem_free', 0))
    _map_insert("mem_realloc", getattr(vm, 'counter_mem_realloc', 0))
    be_api.be_pop(vm, 1)
    return be_api.be_returnvalue(vm)

# static int m_allocs(bvm *vm)
def _debug_m_allocs(vm):
    be_api = _lazy_be_api()
    from berry_port.berry_conf import BE_USE_PERF_COUNTERS
    if BE_USE_PERF_COUNTERS:
        be_api.be_pushint(vm, getattr(vm, 'counter_mem_alloc', 0))
        return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)

# static int m_frees(bvm *vm)
def _debug_m_frees(vm):
    be_api = _lazy_be_api()
    from berry_port.berry_conf import BE_USE_PERF_COUNTERS
    if BE_USE_PERF_COUNTERS:
        be_api.be_pushint(vm, getattr(vm, 'counter_mem_free', 0))
        return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)

# static int m_reallocs(bvm *vm)
def _debug_m_reallocs(vm):
    be_api = _lazy_be_api()
    from berry_port.berry_conf import BE_USE_PERF_COUNTERS
    if BE_USE_PERF_COUNTERS:
        be_api.be_pushint(vm, getattr(vm, 'counter_mem_realloc', 0))
        return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)


# ============================================================================
# strict module functions (from be_strictlib.c)
# ============================================================================

# static int m_init(bvm *vm)
# {
#     comp_set_strict(vm);    /* enable compiler strict mode */
#     be_return_nil(vm);
# }
def _strict_m_init(vm):
    be_api = _lazy_be_api()
    be_var = _lazy_be_var()
    be_var.comp_set_strict(vm)
    return be_api.be_returnnilvalue(vm)


# ============================================================================
# Native module table builders
# ============================================================================
# Each function returns a list of (name, func_or_value) tuples that define
# the module's attribute table, mirroring the C be_native_module_attr_table.

def _build_string_module_table():
    """Build the string module attribute table (from be_strlib.c)."""
    strlib = _lazy_be_strlib()
    return [
        ("format", strlib.be_str_format),
        ("count", strlib.str_count),
        ("split", strlib.str_split),
        ("find", strlib.str_find),
        ("hex", strlib.str_i2hex),
        ("byte", strlib.str_byte),
        ("char", strlib.str_char),
        ("tolower", strlib.str_tolower),
        ("toupper", strlib.str_toupper),
        ("tr", strlib.str_tr),
        ("escape", strlib.str_escape),
        ("replace", strlib.str_replace),
        ("startswith", strlib.str_startswith),
        ("endswith", strlib.str_endswith),
    ]

def _build_json_module_table():
    """Build the json module attribute table (from be_jsonlib.c)."""
    return _lazy_be_jsonlib().be_json_module_table()

def _build_math_module_table():
    """Build the math module attribute table (from be_mathlib.c)."""
    return _lazy_be_mathlib().be_math_module_table()

def _build_math_module_constants():
    """Build the math module constant attributes."""
    return _lazy_be_mathlib().be_math_module_constants()

def _build_solidify_module_table():
    """Build the solidify module attribute table (from be_solidifylib.c)."""
    solidify = _lazy_be_solidifylib()
    return [
        ("dump", solidify.m_dump),
        ("compact", solidify.m_compact),
        ("nocompact", solidify.m_nocompact),
    ]

def _build_debug_module_table():
    """Build the debug module attribute table (from be_debuglib.c)."""
    from berry_port.berry_conf import (BE_USE_DEBUG_HOOK, BE_USE_PERF_COUNTERS,
                                        BE_DEBUG_VAR_INFO)
    table = [
        ("attrdump", _debug_m_attrdump),
        ("codedump", _debug_m_codedump),
        ("traceback", _debug_m_traceback),
    ]
    if BE_USE_DEBUG_HOOK:
        pass  # sethook not ported (requires debug hook infrastructure)
    if BE_USE_PERF_COUNTERS:
        table.append(("counters", _debug_m_counters))
    table.append(("calldepth", _debug_m_calldepth))
    table.append(("top", _debug_m_top))
    if BE_DEBUG_VAR_INFO:
        table.append(("varname", _debug_m_varname))
        table.append(("upvname", _debug_m_upvname))
    table.append(("caller", _debug_m_caller))
    table.append(("allocs", _debug_m_allocs))
    table.append(("frees", _debug_m_frees))
    table.append(("reallocs", _debug_m_reallocs))
    table.append(("gcdebug", _debug_m_gcdebug))
    return table

def _build_gc_module_table():
    """Build the gc module attribute table (from be_gclib.c)."""
    return [
        ("allocated", _gc_m_allocated),
        ("collect", _gc_m_collect),
    ]

def _build_global_module_table():
    """Build the global module attribute table (from be_globallib.c)."""
    return [
        ("()", _global_m_globals),
        ("contains", _global_m_contains),
        ("member", _global_m_findglobal),
        ("setmember", _global_m_setglobal),
        ("undef", _global_m_undef),
    ]

def _build_introspect_module_table():
    """Build the introspect module attribute table (from be_introspectlib.c)."""
    return [
        ("members", _introspect_m_attrlist),
        ("get", _introspect_m_findmember),
        ("set", _introspect_m_setmember),
        ("contains", _introspect_m_contains),
        ("module", _introspect_m_getmodule),
        ("setmodule", _introspect_m_setmodule),
        ("toptr", _introspect_m_toptr),
        ("fromptr", _introspect_m_fromptr),
        ("solidified", _introspect_m_solidified),
        ("name", _introspect_m_name),
        ("ismethod", _introspect_m_ismethod),
    ]

def _build_strict_module_table():
    """Build the strict module attribute table (from be_strictlib.c)."""
    return [
        ("init", _strict_m_init),
    ]

def _build_os_path_module_table():
    """Build the os.path sub-module attribute table (from be_oslib.c)."""
    oslib = _lazy_be_oslib()
    return oslib.be_os_path_module_table()

def _build_os_module_table():
    """Build the os module attribute table (from be_oslib.c)."""
    oslib = _lazy_be_oslib()
    return oslib.be_os_module_table()

def _build_re_module_table():
    """Build the re module attribute table (from be_re_lib.c)."""
    relib = _lazy_be_relib()
    return relib.be_re_module_table()

def _build_sys_module_table():
    """Build the sys module attribute table (from be_syslib.c)."""
    syslib = _lazy_be_syslib()
    return syslib.be_sys_module_table()

def _build_undefined_module_table():
    """Build the undefined module attribute table (from be_undefinedlib.c)."""
    return [
        (".p", None),  # nil attribute — placeholder
    ]


# ============================================================================
# _register_native_module — helper to create and register a native module
# ============================================================================

def _register_native_module(name, func_table, const_table=None, submodules=None):
    """Create a bntvmodule from a function table and register it in be_module_table.

    func_table: list of (name, callable_or_None) — functions and nil attrs
    const_table: optional list of (name, value) — int/real constants
    submodules: optional list of (name, bntvmodule) — sub-module entries

    Mirrors the C be_native_module_attr_table + be_define_native_module pattern.
    """
    be_obj = _lazy_be_object()
    be_mod = _lazy_be_module()

    # Build the attrs list as bntvmodobj entries
    attrs = []
    for attr_name, attr_val in func_table:
        node = be_obj.bntvmodobj()
        node.name = attr_name
        if attr_val is None:
            node.type = be_obj.BE_CNIL
            node.u = None
        elif callable(attr_val):
            node.type = be_obj.BE_CFUNCTION
            node.u = attr_val
        else:
            # Should not happen for function tables
            node.type = be_obj.BE_CNIL
            node.u = None
        attrs.append(node)

    # Add constant attributes (int/real)
    if const_table:
        for attr_name, attr_val in const_table:
            node = be_obj.bntvmodobj()
            node.name = attr_name
            if isinstance(attr_val, float):
                node.type = be_obj.BE_CREAL
                node.u = attr_val
            elif isinstance(attr_val, int):
                node.type = be_obj.BE_CINT
                node.u = attr_val
            else:
                node.type = be_obj.BE_CNIL
                node.u = None
            attrs.append(node)

    # Add sub-module attributes (BE_CMODULE)
    if submodules:
        for attr_name, submod in submodules:
            node = be_obj.bntvmodobj()
            node.name = attr_name
            node.type = be_obj.BE_CMODULE
            node.u = submod  # bntvmodule object
            attrs.append(node)

    nm = be_obj.bntvmodule(
        name=name,
        attrs=attrs,
        size=len(attrs),
        module=None,
    )
    be_mod.be_module_table.append(nm)
    return nm


def _create_native_submodule(name, func_table):
    """Create a bntvmodule from a function table WITHOUT registering it.

    Used for sub-modules (e.g., os.path) that are embedded inside a parent module.
    """
    be_obj = _lazy_be_object()

    attrs = []
    for attr_name, attr_val in func_table:
        node = be_obj.bntvmodobj()
        node.name = attr_name
        if attr_val is None:
            node.type = be_obj.BE_CNIL
            node.u = None
        elif callable(attr_val):
            node.type = be_obj.BE_CFUNCTION
            node.u = attr_val
        else:
            node.type = be_obj.BE_CNIL
            node.u = None
        attrs.append(node)

    return be_obj.bntvmodule(
        name=name,
        attrs=attrs,
        size=len(attrs),
        module=None,
    )


# ============================================================================
# _register_module_table — populate be_module_table
# ============================================================================
# Mirrors default/be_modtab.c: registers all native modules into the
# be_module_table list, respecting BE_USE_* configuration flags.

def _register_module_table():
    """Populate be_module_table with all native modules.

    Mirrors the C be_module_table[] array in default/be_modtab.c.
    Called once during be_loadlibs.
    """
    from berry_port.berry_conf import (
        BE_USE_STRING_MODULE, BE_USE_JSON_MODULE, BE_USE_MATH_MODULE,
        BE_USE_OS_MODULE, BE_USE_RE_MODULE, BE_USE_SYS_MODULE,
        BE_USE_GLOBAL_MODULE, BE_USE_DEBUG_MODULE, BE_USE_GC_MODULE,
        BE_USE_SOLIDIFY_MODULE, BE_USE_INTROSPECT_MODULE,
        BE_USE_STRICT_MODULE,
    )
    be_mod = _lazy_be_module()

    # Clear any previous registrations (safety for re-init)
    be_mod.be_module_table.clear()

    # /* default modules register */
    # #if BE_USE_STRING_MODULE
    #     &be_native_module(string),
    # #endif
    if BE_USE_STRING_MODULE:
        _register_native_module("string", _build_string_module_table())

    # #if BE_USE_JSON_MODULE
    #     &be_native_module(json),
    # #endif
    if BE_USE_JSON_MODULE:
        _register_native_module("json", _build_json_module_table())

    # #if BE_USE_MATH_MODULE
    #     &be_native_module(math),
    # #endif
    if BE_USE_MATH_MODULE:
        _register_native_module("math", _build_math_module_table(),
                                _build_math_module_constants())

    # Note: time module is not ported (platform-specific)
    # #if BE_USE_TIME_MODULE ... #endif

    # #if BE_USE_OS_MODULE
    #     &be_native_module(os),
    # #endif
    if BE_USE_OS_MODULE:
        path_submod = _create_native_submodule("path", _build_os_path_module_table())
        _register_native_module("os", _build_os_module_table(),
                                submodules=[("path", path_submod)])

    # #if BE_USE_RE_MODULE
    #     &be_native_module(re),
    # #endif
    if BE_USE_RE_MODULE:
        _register_native_module("re", _build_re_module_table())

    # #if BE_USE_GLOBAL_MODULE
    #     &be_native_module(global),
    # #endif
    if BE_USE_GLOBAL_MODULE:
        _register_native_module("global", _build_global_module_table())

    # #if BE_USE_SYS_MODULE
    #     &be_native_module(sys),
    # #endif
    if BE_USE_SYS_MODULE:
        _register_native_module("sys", _build_sys_module_table())

    # #if BE_USE_DEBUG_MODULE
    #     &be_native_module(debug),
    # #endif
    if BE_USE_DEBUG_MODULE:
        _register_native_module("debug", _build_debug_module_table())

    # #if BE_USE_GC_MODULE
    #     &be_native_module(gc),
    # #endif
    if BE_USE_GC_MODULE:
        _register_native_module("gc", _build_gc_module_table())

    # #if BE_USE_SOLIDIFY_MODULE
    #     &be_native_module(solidify),
    # #endif
    if BE_USE_SOLIDIFY_MODULE:
        _register_native_module("solidify", _build_solidify_module_table())

    # #if BE_USE_INTROSPECT_MODULE
    #     &be_native_module(introspect),
    # #endif
    if BE_USE_INTROSPECT_MODULE:
        _register_native_module("introspect", _build_introspect_module_table())

    # #if BE_USE_STRICT_MODULE
    #     &be_native_module(strict),
    # #endif
    if BE_USE_STRICT_MODULE:
        _register_native_module("strict", _build_strict_module_table())

    # &be_native_module(undefined),  /* always registered */
    _register_native_module("undefined", _build_undefined_module_table())

    # Note: preproc module not registered here — preprocessor is handled
    # internally by the lexer.


# ============================================================================
# be_loadlibs — main entry point
# ============================================================================

# void be_loadlibs(bvm *vm)
# {
#     be_load_baselib(vm);
#     be_load_listlib(vm);
#     be_load_maplib(vm);
#     be_load_rangelib(vm);
#     be_load_filelib(vm);
#     be_load_byteslib(vm);
#     be_load_baselib_next(vm);
# }
def be_loadlibs(vm):
    """Register all built-in libraries and native modules.

    Mirrors src/be_libs.c be_loadlibs() + default/be_modtab.c module table.
    Called once during VM initialization (be_vm_new).
    """
    # 1. Register the native module table (equivalent to be_modtab.c)
    _register_module_table()

    # 2. Load built-in libraries (equivalent to be_libs.c)
    _lazy_be_baselib().be_load_baselib(vm)
    _lazy_be_listlib().be_load_listlib(vm)
    _lazy_be_maplib().be_load_maplib(vm)
    _lazy_be_rangelib().be_load_rangelib(vm)
    from berry_port import be_filelib as _be_filelib
    _be_filelib.be_load_filelib(vm)
    _lazy_be_byteslib().be_load_byteslib(vm)
    _lazy_be_int64lib().be_load_int64lib(vm)
    _lazy_be_baselib().be_load_baselib_next(vm)

    # 3. Load additional classes (re_pattern for the re module)
    from berry_port.berry_conf import BE_USE_RE_MODULE
    if BE_USE_RE_MODULE:
        _lazy_be_relib().be_load_relib(vm)
