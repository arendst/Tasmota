"""
Berry public API.
Mirrors: src/be_api.c

This module implements the public C API for the Berry interpreter,
providing VM lifecycle, stack manipulation, type checking, value
retrieval, code loading, member access, container operations,
iterators, upvalue access, reference stack, return helpers,
error handling, comparison, and native registration functions.

Original C code is included as comments for each function.
"""

# /********************************************************************
# ** Copyright (c) 2018-2020 Guan Wenliang
# ** This file is part of the Berry default interpreter.
# ** skiars@qq.com, https://github.com/Skiars/berry
# ** See Copyright Notice in the LICENSE file or at
# ** https://github.com/Skiars/berry/blob/master/LICENSE
# ********************************************************************/

import sys

from berry_port.be_object import (
    # Type constants
    BE_NONE, BE_NIL, BE_INT, BE_REAL, BE_BOOL, BE_COMPTR, BE_INDEX,
    BE_FUNCTION, BE_STRING, BE_CLASS, BE_INSTANCE, BE_PROTO,
    BE_LIST, BE_MAP, BE_MODULE, BE_COMOBJ,
    BE_NTVFUNC, BE_CLOSURE, BE_NTVCLOS, BE_CTYPE_FUNC,
    # Error codes
    BE_OK, BE_EXIT, BE_MALLOC_FAIL, BE_EXCEPTION,
    BE_SYNTAX_ERROR, BE_EXEC_ERROR, BE_IO_ERROR,
    # GC constants
    GC_CONST,
    # Structures
    bvalue, bcommomobj, bnfuncinfo,
    # var_* helpers
    var_type, var_basetype, var_primetype, var_istype,
    var_isnil, var_isbool, var_isint, var_isreal, var_isstr,
    var_isclosure, var_isntvclos, var_isntvfunc, var_isfunction,
    var_isproto, var_isclass, var_isinstance, var_islist, var_ismap,
    var_ismodule, var_isnumber, var_iscomptr,
    var_setnil, var_setval, var_setbool, var_setint, var_setreal,
    var_setstr, var_setobj, var_setclass, var_setclosure, var_setntvclos,
    var_setntvfunc, var_setlist, var_setinstance,
    var_toint, var_toreal, var_tobool, var_tostr, var_toobj, var_toidx,
    # GC helpers
    gc_isconst,
    # Class/instance helpers
    be_class_name, be_class_super, be_class_setsuper,
    be_instance_name, be_instance_class, be_instance_super,
    # List helpers
    be_list_data, be_list_count, be_list_at, be_list_end,
    # Map helpers
    be_map_count, be_map_key2value,
    # String helpers
    str_len,
    # Cast
    cast_int, basetype,
)
from berry_port.berry_conf import BE_STACK_FREE_MIN


# ============================================================================
# Lazy imports to avoid circular dependencies
# ============================================================================

def _lazy_be_string():
    import berry_port.be_string as m
    return m

def _lazy_be_class():
    import berry_port.be_class as m
    return m

def _lazy_be_func():
    import berry_port.be_func as m
    return m

def _lazy_be_gc():
    import berry_port.be_gc as m
    return m

def _lazy_be_exec():
    import berry_port.be_exec as m
    return m

def _lazy_be_var():
    import berry_port.be_var as m
    return m

def _lazy_be_vector():
    import berry_port.be_vector as m
    return m

def _lazy_be_list():
    import berry_port.be_list as m
    return m

def _lazy_be_map():
    import berry_port.be_map as m
    return m

def _lazy_be_module():
    import berry_port.be_module as m
    return m

def _lazy_be_vm():
    import berry_port.be_vm as m
    return m


# ============================================================================
# Internal: retreg macro
# ============================================================================

# #define retreg(vm)      ((vm)->cf->func)
def _retreg(vm):
    """Return index of the return register (the function slot)."""
    return vm.cf.func


# ============================================================================
# Internal: be_indexof — resolve stack index
# ============================================================================

# bvalue* be_indexof(bvm *vm, int idx)
# {
#     if (idx > 0) {
#         return vm->reg + idx - 1;
#     }
#     return vm->top + idx;
# }
def be_indexof(vm, idx):
    """Resolve a stack index to an absolute index into vm.stack.
    Positive indices are 1-based from reg; negative from top.
    """
    if idx > 0:
        return vm.reg_idx + idx - 1
    return vm.top_idx + idx


# ============================================================================
# Internal: be_incrtop — increment top, return old top index
# ============================================================================

def be_incrtop(vm):
    """Increment top-of-stack. Returns old top index (before increment)."""
    old_top = vm.top_idx
    vm.top_idx += 1
    # Ensure stack list is large enough
    while vm.top_idx > len(vm.stack):
        vm.stack.append(bvalue())
    return old_top


# ============================================================================
# Internal: be_stackpop
# ============================================================================

def be_stackpop(vm, n):
    """Pop n values from the stack."""
    vm.top_idx -= n


# ============================================================================
# Internal: stack space helpers
# ============================================================================

def _ensure_stack(vm, count):
    """Ensure vm.stack has room for at least `count` items above current top."""
    needed = vm.top_idx + count
    while needed >= len(vm.stack):
        vm.stack.append(bvalue())



# ============================================================================
# Solidification helper struct
# ============================================================================

# struct solidfuncinfo {
#     const char *name;
#     bclosure *function;
# };
class _solidfuncinfo:
    __slots__ = ('name', 'function')
    def __init__(self, name=None, function=None):
        self.name = name
        self.function = function


# ============================================================================
# class_init — bind native methods/members to a class
# ============================================================================

# static void class_init(bvm *vm, bclass *c, const bnfuncinfo *lib)
# {
#     if (lib) {
#         while (lib->name) {
#             bstring *s = be_newstr(vm, lib->name);
#             if (lib->function) { /* method */
#                 be_class_native_method_bind(vm, c, s, lib->function);
#             } else {
#                 be_class_member_bind(vm, c, s, btrue); /* member */
#             }
#             ++lib;
#         }
#         if (lib->function == (bntvfunc)BE_CLOSURE) {
#             /* next section is closures */
#             struct solidfuncinfo *slib = (struct solidfuncinfo*)++lib;
#             while (slib->name) {
#                 if (slib->function) { /* method */
#                     bstring *s = be_newstr(vm, slib->name);
#                     be_class_closure_method_bind(vm, c, s, slib->function);
#                 }
#                 ++slib;
#             }
#         }
#         be_map_compact(vm, c->members); /* clear space */
#     }
# }
def _class_init(vm, c, lib):
    """Bind native methods/members from lib list to class c."""
    if lib is None:
        return
    be_str = _lazy_be_string()
    be_cls = _lazy_be_class()
    be_mp = _lazy_be_map()

    # Support both tuple format (name, func) and object format (.name, .function)
    def _get_name(entry):
        if isinstance(entry, tuple):
            return entry[0]
        return entry.name

    def _get_func(entry):
        if isinstance(entry, tuple):
            return entry[1]
        return entry.function

    i = 0
    while i < len(lib) and _get_name(lib[i]) is not None:
        entry = lib[i]
        s = be_str.be_newstr(vm, _get_name(entry))
        if _get_func(entry) is not None:
            be_cls.be_class_native_method_bind(vm, c, s, _get_func(entry))
        else:
            be_cls.be_class_member_bind(vm, c, s, True)
        i += 1

    # Check for closure section sentinel: name=None, function=BE_CLOSURE
    if i < len(lib) and _get_name(lib[i]) is None and _get_func(lib[i]) == BE_CLOSURE:
        i += 1
        # Remaining entries are _solidfuncinfo-like (name, function=bclosure)
        while i < len(lib) and _get_name(lib[i]) is not None:
            entry = lib[i]
            if _get_func(entry) is not None:
                s = be_str.be_newstr(vm, _get_name(entry))
                be_cls.be_class_closure_method_bind(vm, c, s, _get_func(entry))
            i += 1

    if c.members is not None:
        be_mp.be_map_compact(vm, c.members)


# ============================================================================
# class_auto_make — create or retrieve a cached native class
# ============================================================================

# static bclass* class_auto_make(bvm *vm, bstring *name, const bnfuncinfo *lib)
def _class_auto_make(vm, name, lib):
    """Create or retrieve a cached native class from vm.ntvclass."""
    be_mp = _lazy_be_map()
    be_cls = _lazy_be_class()

    key = bvalue()
    var_setobj(key, BE_COMPTR, id(lib) if lib is not None else None)

    if vm.ntvclass is None:
        vm.ntvclass = be_mp.be_map_new(vm)

    res = be_mp.be_map_find(vm, vm.ntvclass, key)
    if res is None or not var_isclass(res):
        # Insert class to native class table
        res = be_mp.be_map_insert(vm, vm.ntvclass, key, None)
        var_setnil(res)
        c = be_cls.be_newclass(vm, name, None)
        var_setclass(res, c)
        _class_init(vm, c, lib)
        return c
    return var_toobj(res)


# ============================================================================
# be_regfunc — register a native function as a global/builtin
# ============================================================================

# BERRY_API void be_regfunc(bvm *vm, const char *name, bntvfunc f)
def be_regfunc(vm, name, f):
    """Register a Python callable as a Berry native function.

    For builtins (idx < builtin_count): set the value in the builtin
    vector only — no global shadow slot.  A shadow would cause
    _global_find to return an index >= builtin_count for builtin
    names, breaking the GETGBL / GETNGBL decision in the compiler.

    For non-builtins: create a normal global slot.
    """
    be_str = _lazy_be_string()
    be_var = _lazy_be_var()

    s = be_str.be_newstr(vm, name)
    idx = be_var.be_global_find(vm, s)
    if idx >= 0 and idx < be_var.be_builtin_count(vm):
        # Builtin — write only to the builtin vector
        blt_var = be_var.be_global_var(vm, idx)
        var_setntvfunc(blt_var, f)
    else:
        # Non-builtin — create a global slot
        gidx = be_var.be_global_new(vm, s)
        var_ = be_var.be_global_var(vm, gidx)
        var_setntvfunc(var_, f)


# ============================================================================
# be_regclass — register a native class as a global/builtin
# ============================================================================

# BERRY_API void be_regclass(bvm *vm, const char *name, const bnfuncinfo *lib)
def be_regclass(vm, name, lib):
    """Register a native class with methods from lib.

    Same approach as be_regfunc: builtin-only for builtins,
    global slot for non-builtins (e.g. re_pattern).
    """
    be_str = _lazy_be_string()
    be_var = _lazy_be_var()

    s = be_str.be_newstr(vm, name)
    idx = be_var.be_global_find(vm, s)
    if idx >= 0 and idx < be_var.be_builtin_count(vm):
        # Builtin — write only to the builtin vector
        cls = _class_auto_make(vm, s, lib)
        blt_var = be_var.be_global_var(vm, idx)
        var_setclass(blt_var, cls)
    else:
        # Non-builtin (e.g. re_pattern) — create a global slot
        cls = _class_auto_make(vm, s, lib)
        gidx = be_var.be_global_new(vm, s)
        var_ = be_var.be_global_var(vm, gidx)
        var_setclass(var_, cls)


# ============================================================================
# Stack query: be_top
# ============================================================================

# BERRY_API int be_top(bvm *vm)
# {
#     return cast_int(vm->top - vm->reg);
# }
def be_top(vm):
    """Return the number of values on the stack above the base register."""
    return vm.top_idx - vm.reg_idx


# ============================================================================
# Stack manipulation: be_pop
# ============================================================================

# BERRY_API void be_pop(bvm *vm, int n)
# {
#     be_assert(n <= vm->top - vm->reg);
#     be_stackpop(vm, n);
# }
def be_pop(vm, n):
    """Pop n values from the stack."""
    be_stackpop(vm, n)


# ============================================================================
# be_absindex
# ============================================================================

# BERRY_API int be_absindex(bvm *vm, int index)
# {
#     if (index > 0) {
#         return index;
#     }
#     be_assert(vm->reg <= vm->top + index);
#     return cast_int(vm->top + index - vm->reg + 1);
# }
def be_absindex(vm, index):
    """Convert a possibly-negative stack index to a positive (1-based) index."""
    if index > 0:
        return index
    return vm.top_idx + index - vm.reg_idx + 1


# ============================================================================
# Type checking functions
# ============================================================================

# BERRY_API bbool be_isnil(bvm *vm, int index)
# { bvalue *v = be_indexof(vm, index); return var_isnil(v); }
def be_isnil(vm, index):
    v = vm.stack[be_indexof(vm, index)]
    return var_isnil(v)

# BERRY_API bbool be_isbool(bvm *vm, int index)
def be_isbool(vm, index):
    v = vm.stack[be_indexof(vm, index)]
    return var_isbool(v)

# BERRY_API bbool be_isint(bvm *vm, int index)
def be_isint(vm, index):
    v = vm.stack[be_indexof(vm, index)]
    return var_isint(v)

# BERRY_API bbool be_isreal(bvm *vm, int index)
def be_isreal(vm, index):
    v = vm.stack[be_indexof(vm, index)]
    return var_isreal(v)

# BERRY_API bbool be_isnumber(bvm *vm, int index)
def be_isnumber(vm, index):
    v = vm.stack[be_indexof(vm, index)]
    return var_isnumber(v)

# BERRY_API bbool be_isstring(bvm *vm, int index)
def be_isstring(vm, index):
    v = vm.stack[be_indexof(vm, index)]
    return var_isstr(v)

# BERRY_API bbool be_isclosure(bvm *vm, int index)
def be_isclosure(vm, index):
    v = vm.stack[be_indexof(vm, index)]
    return var_isclosure(v)

# BERRY_API bbool be_isntvclos(bvm *vm, int index)
def be_isntvclos(vm, index):
    v = vm.stack[be_indexof(vm, index)]
    return var_isntvclos(v)

# BERRY_API bbool be_isfunction(bvm *vm, int index)
def be_isfunction(vm, index):
    v = vm.stack[be_indexof(vm, index)]
    return var_isfunction(v)

# BERRY_API bbool be_isproto(bvm *vm, int index)
def be_isproto(vm, index):
    v = vm.stack[be_indexof(vm, index)]
    return var_isproto(v)

# BERRY_API bbool be_isclass(bvm *vm, int index)
def be_isclass(vm, index):
    v = vm.stack[be_indexof(vm, index)]
    return var_isclass(v)

# BERRY_API bbool be_isinstance(bvm *vm, int index)
def be_isinstance(vm, index):
    v = vm.stack[be_indexof(vm, index)]
    return var_isinstance(v)

# static bbool be_isinstanceofbuiltin(bvm *vm, int rel_index, const char *classname)
def _be_isinstanceofbuiltin(vm, rel_index, classname):
    """Check if value at rel_index is an instance of a builtin class."""
    index = be_absindex(vm, rel_index)
    if be_isinstance(vm, index):
        be_getbuiltin(vm, classname)
        if be_isderived(vm, index):
            be_pop(vm, 1)
            return True
        be_pop(vm, 1)
    return False

# BERRY_API bbool be_ismapinstance(bvm *vm, int index)
def be_ismapinstance(vm, index):
    return _be_isinstanceofbuiltin(vm, index, "map")

# BERRY_API bbool be_islistinstance(bvm *vm, int index)
def be_islistinstance(vm, index):
    return _be_isinstanceofbuiltin(vm, index, "list")

# BERRY_API bbool be_ismodule(bvm *vm, int index)
def be_ismodule(vm, index):
    v = vm.stack[be_indexof(vm, index)]
    return var_ismodule(v)

# BERRY_API bbool be_islist(bvm *vm, int index)
def be_islist(vm, index):
    v = vm.stack[be_indexof(vm, index)]
    return var_islist(v)

# BERRY_API bbool be_ismap(bvm *vm, int index)
def be_ismap(vm, index):
    v = vm.stack[be_indexof(vm, index)]
    return var_ismap(v)

# BERRY_API bbool be_iscomptr(bvm *vm, int index)
def be_iscomptr(vm, index):
    v = vm.stack[be_indexof(vm, index)]
    return var_istype(v, BE_COMPTR)

# BERRY_API bbool be_iscomobj(bvm *vm, int index)
def be_iscomobj(vm, index):
    v = vm.stack[be_indexof(vm, index)]
    return var_istype(v, BE_COMOBJ)


# ============================================================================
# Value retrieval functions
# ============================================================================

# BERRY_API bint be_toint(bvm *vm, int index)
# { bvalue *v = be_indexof(vm, index); return var_toint(v); }
def be_toint(vm, index):
    v = vm.stack[be_indexof(vm, index)]
    return var_toint(v)

# BERRY_API breal be_toreal(bvm *vm, int index)
# {
#     bvalue *v = be_indexof(vm, index);
#     if (var_isreal(v)) { return var_toreal(v); }
#     if (var_isint(v)) { return cast(breal, var_toint(v)); }
#     return cast(breal, 0.0);
# }
def be_toreal(vm, index):
    v = vm.stack[be_indexof(vm, index)]
    if var_isreal(v):
        return var_toreal(v)
    if var_isint(v):
        return float(var_toint(v))
    return 0.0

# BERRY_API int be_toindex(bvm *vm, int index)
def be_toindex(vm, index):
    v = vm.stack[be_indexof(vm, index)]
    return var_toidx(v)

# BERRY_API bbool be_tobool(bvm *vm, int index)
def be_tobool(vm, index):
    be_vm = _lazy_be_vm()
    v = vm.stack[be_indexof(vm, index)]
    return be_vm.be_value2bool(vm, v)

# BERRY_API const char* be_tostring(bvm *vm, int index)
# {
#     bvalue *v = be_indexof(vm, index);
#     if (!var_isstr(v)) {
#         be_val2str(vm, index);
#         v = be_indexof(vm, index);
#     }
#     return str(var_tostr(v));
# }
def be_tostring(vm, index):
    """Convert value to string if needed, return the Python string."""
    be_vm = _lazy_be_vm()
    v = vm.stack[be_indexof(vm, index)]
    if not var_isstr(v):
        be_vm.be_val2str(vm, index)
        v = vm.stack[be_indexof(vm, index)]
    s = var_tostr(v)
    be_str = _lazy_be_string()
    return be_str.be_str2cstr(s)

# BERRY_API void* be_tocomptr(bvm *vm, int index)
# {
#     bvalue *v = be_indexof(vm, index);
#     if (var_istype(v, BE_COMPTR)) { return var_toobj(v); }
#     if (var_istype(v, BE_COMOBJ)) { bcommomobj *obj = var_toobj(v); return obj->data; }
#     return NULL;
# }
def be_tocomptr(vm, index):
    v = vm.stack[be_indexof(vm, index)]
    if var_istype(v, BE_COMPTR):
        return var_toobj(v)
    if var_istype(v, BE_COMOBJ):
        obj = var_toobj(v)
        return obj.data
    return None


# ============================================================================
# Stack manipulation: move, push, remove
# ============================================================================

# BERRY_API void be_moveto(bvm *vm, int from, int to)
# { bvalue *src = be_indexof(vm, from); bvalue *dst = be_indexof(vm, to); var_setval(dst, src); }
def be_moveto(vm, from_idx, to_idx):
    src = vm.stack[be_indexof(vm, from_idx)]
    dst = vm.stack[be_indexof(vm, to_idx)]
    var_setval(dst, src)

# BERRY_API void be_pushnil(bvm *vm)
# { bvalue *reg = be_incrtop(vm); var_setnil(reg); }
def be_pushnil(vm):
    _ensure_stack(vm, 1)
    idx = be_incrtop(vm)
    var_setnil(vm.stack[idx])

# BERRY_API void be_pushbool(bvm *vm, int b)
# { bvalue *reg = be_incrtop(vm); var_setbool(reg, b != bfalse); }
def be_pushbool(vm, b):
    _ensure_stack(vm, 1)
    idx = be_incrtop(vm)
    var_setbool(vm.stack[idx], b)

# BERRY_API void be_pushint(bvm *vm, bint i)
# { bvalue *reg = be_incrtop(vm); var_setint(reg, i); }
def be_pushint(vm, i):
    _ensure_stack(vm, 1)
    idx = be_incrtop(vm)
    var_setint(vm.stack[idx], i)

# BERRY_API void be_pushreal(bvm *vm, breal r)
# { bvalue *reg = be_incrtop(vm); var_setreal(reg, r); }
def be_pushreal(vm, r):
    _ensure_stack(vm, 1)
    idx = be_incrtop(vm)
    var_setreal(vm.stack[idx], r)

# BERRY_API void be_pushstring(bvm *vm, const char *str)
# {
#     bstring *s = be_newstr(vm, str);
#     bvalue *reg = be_incrtop(vm);
#     var_setstr(reg, s);
# }
def be_pushstring(vm, s):
    """Push a string onto the stack. Creates a Berry string first to be GC-safe."""
    be_str = _lazy_be_string()
    bs = be_str.be_newstr(vm, s)
    _ensure_stack(vm, 1)
    idx = be_incrtop(vm)
    var_setstr(vm.stack[idx], bs)

# BERRY_API void be_pushnstring(bvm *vm, const char *str, size_t n)
# {
#     bstring *s = be_newstrn(vm, str, n);
#     bvalue *reg = be_incrtop(vm);
#     var_setstr(reg, s);
# }
def be_pushnstring(vm, s, n):
    """Push a string of length n onto the stack."""
    be_str = _lazy_be_string()
    bs = be_str.be_newstrn(vm, s[:n], n)
    _ensure_stack(vm, 1)
    idx = be_incrtop(vm)
    var_setstr(vm.stack[idx], bs)

# BERRY_API const char* be_pushfstring(bvm *vm, const char *format, ...)
def be_pushfstring(vm, fmt, *args):
    """Format a string and push it onto the stack. Returns the Python string."""
    try:
        result = fmt % args if args else fmt
    except (TypeError, ValueError):
        result = fmt
    be_pushstring(vm, result)
    return result

# BERRY_API const char* be_pushvfstr(bvm *vm, const char *format, va_list arg_ptr)
def be_pushvfstr(vm, fmt, args):
    """Push a formatted string (va_list version). In Python, args is a tuple."""
    try:
        result = fmt % args if args else fmt
    except (TypeError, ValueError):
        result = fmt
    be_pushstring(vm, result)
    return result

# BERRY_API void* be_pushbuffer(bvm *vm, size_t size)
# {
#     bstring *s = be_newlongstr(vm, NULL, size);
#     bvalue *reg = be_incrtop(vm);
#     var_setstr(reg, s);
#     return (void*)str(s);
# }
def be_pushbuffer(vm, size):
    """Push a buffer (long string) of given size. Returns the string data."""
    be_str = _lazy_be_string()
    s = be_str.be_newlongstr(vm, None, size)
    _ensure_stack(vm, 1)
    idx = be_incrtop(vm)
    var_setstr(vm.stack[idx], s)
    return be_str.be_str2cstr(s)

# BERRY_API void be_pushvalue(bvm *vm, int index)
# {
#     bvalue *reg = vm->top;
#     var_setval(reg, be_indexof(vm, index));
#     be_incrtop(vm);
# }
def be_pushvalue(vm, index):
    """Push a copy of the value at the given stack index."""
    _ensure_stack(vm, 1)
    src = vm.stack[be_indexof(vm, index)]
    idx = vm.top_idx
    var_setval(vm.stack[idx], src)
    be_incrtop(vm)

# BERRY_API void be_pushclosure(bvm *vm, void *cl)
# {
#     bvalue *reg = be_incrtop(vm);
#     bclosure * closure = (bclosure*) cl;
#     var_setclosure(reg, closure);
# }
def be_pushclosure(vm, cl):
    _ensure_stack(vm, 1)
    idx = be_incrtop(vm)
    var_setclosure(vm.stack[idx], cl)

# BERRY_API void be_pushntvclosure(bvm *vm, bntvfunc f, int nupvals)
# {
#     bntvclos *cl = be_newntvclosure(vm, f, nupvals);
#     bvalue *top = be_incrtop(vm);
#     var_setntvclos(top, cl);
# }
def be_pushntvclosure(vm, f, nupvals):
    """Push a new native closure with nupvals upvalue slots."""
    be_func = _lazy_be_func()
    cl = be_func.be_newntvclosure(vm, f, nupvals)
    _ensure_stack(vm, 1)
    idx = be_incrtop(vm)
    var_setntvclos(vm.stack[idx], cl)

# BERRY_API void be_pushntvfunction(bvm *vm, bntvfunc f)
# { bvalue *top = be_incrtop(vm); var_setntvfunc(top, f); }
def be_pushntvfunction(vm, f):
    _ensure_stack(vm, 1)
    idx = be_incrtop(vm)
    var_setntvfunc(vm.stack[idx], f)

# BERRY_API void be_pushclass(bvm *vm, const char *name, const bnfuncinfo *lib)
# {
#     bclass *c;
#     bstring *s = be_newstr(vm, name);
#     bvalue *dst = be_incrtop(vm);
#     var_setstr(dst, s);
#     c = class_auto_make(vm, s, lib);
#     var_setclass(vm->top - 1, c);
# }
def be_pushclass(vm, name, lib):
    be_str = _lazy_be_string()
    s = be_str.be_newstr(vm, name)
    _ensure_stack(vm, 1)
    idx = be_incrtop(vm)
    var_setstr(vm.stack[idx], s)
    c = _class_auto_make(vm, s, lib)
    var_setclass(vm.stack[vm.top_idx - 1], c)

# BERRY_API void be_pushntvclass(bvm *vm, const struct bclass * c)
# { bvalue *top = be_incrtop(vm); var_setclass(top, (bclass *) c); }
def be_pushntvclass(vm, c):
    _ensure_stack(vm, 1)
    idx = be_incrtop(vm)
    var_setclass(vm.stack[idx], c)

# BERRY_API void be_pushcomptr(bvm *vm, void *ptr)
# { bvalue *top = be_incrtop(vm); var_setobj(top, BE_COMPTR, ptr); }
def be_pushcomptr(vm, ptr):
    _ensure_stack(vm, 1)
    idx = be_incrtop(vm)
    var_setobj(vm.stack[idx], BE_COMPTR, ptr)

# BERRY_API void be_remove(bvm *vm, int index)
# {
#     bvalue *v = be_indexof(vm, index);
#     bvalue *top = --vm->top;
#     while (v < top) { *v = v[1]; ++v; }
# }
def be_remove(vm, index):
    """Remove the value at the given stack index, shifting values down."""
    abs_idx = be_indexof(vm, index)
    vm.top_idx -= 1
    i = abs_idx
    while i < vm.top_idx:
        var_setval(vm.stack[i], vm.stack[i + 1])
        i += 1


# ============================================================================
# String operations
# ============================================================================

# BERRY_API void be_strconcat(bvm *vm, int index)
# {
#     bstring *s;
#     bvalue *dst = be_indexof(vm, index);
#     bvalue *src = be_indexof(vm, -1);
#     s = be_strcat(vm, var_tostr(dst), var_tostr(src));
#     var_setstr(dst, s);
# }
def be_strconcat(vm, index):
    """Concatenate the string at top-1 onto the string at index."""
    be_vm = _lazy_be_vm()
    dst = vm.stack[be_indexof(vm, index)]
    src = vm.stack[be_indexof(vm, -1)]
    s = be_vm.be_strcat(vm, var_tostr(dst), var_tostr(src))
    var_setstr(dst, s)

# BERRY_API int be_strlen(bvm *vm, int index)
# {
#     bvalue *v = be_indexof(vm, index);
#     if (var_isstr(v)) { return str_len(var_tostr(v)); }
#     return 0;
# }
def be_strlen(vm, index):
    v = vm.stack[be_indexof(vm, index)]
    if var_isstr(v):
        return str_len(var_tostr(v))
    return 0


# ============================================================================
# Super/derived/classof/classname/typename
# ============================================================================

# BERRY_API bbool be_setsuper(bvm *vm, int index)
# {
#     bvalue *v = be_indexof(vm, index);
#     bvalue *top = be_indexof(vm, -1);
#     if (var_isclass(v) && var_isclass(top)) {
#         bclass *c = var_toobj(v);
#         if (!gc_isconst(c)) {
#             bclass *super = var_toobj(top);
#             be_class_setsuper(c, super);
#             return btrue;
#         }
#     }
#     return bfalse;
# }
def be_setsuper(vm, index):
    v = vm.stack[be_indexof(vm, index)]
    top = vm.stack[be_indexof(vm, -1)]
    if var_isclass(v) and var_isclass(top):
        c = var_toobj(v)
        if not gc_isconst(c):
            super_ = var_toobj(top)
            be_class_setsuper(c, super_)
            return True
    return False

# BERRY_API void be_getsuper(bvm *vm, int index)
# {
#     bvalue *v = be_indexof(vm, index);
#     bvalue *top = be_incrtop(vm);
#     if (var_isclass(v)) {
#         bclass *c = var_toobj(v);
#         c = be_class_super(c);
#         if (c) { var_setclass(top, c); return; }
#     } else if (var_isinstance(v)) {
#         binstance *o = var_toobj(v);
#         o = be_instance_super(o);
#         if (o) { var_setinstance(top, o); return; }
#     }
#     var_setnil(top);
# }
def be_getsuper(vm, index):
    v = vm.stack[be_indexof(vm, index)]
    _ensure_stack(vm, 1)
    idx = be_incrtop(vm)
    top = vm.stack[idx]
    if var_isclass(v):
        c = var_toobj(v)
        c = be_class_super(c)
        if c:
            var_setclass(top, c)
            return
    elif var_isinstance(v):
        o = var_toobj(v)
        o = be_instance_super(o)
        if o:
            var_setinstance(top, o)
            return
    var_setnil(top)

# static bclass* _getclass(bvalue *v)
def _getclass(v):
    if var_isinstance(v):
        ins = var_toobj(v)
        return be_instance_class(ins)
    return var_toobj(v) if var_isclass(v) else None

# BERRY_API bbool be_isderived(bvm *vm, int index)
# {
#     bclass *sup = _getclass(be_indexof(vm, -1));
#     if (sup) {
#         bclass *c = _getclass(be_indexof(vm, index));
#         while (c && c != sup) c = be_class_super(c);
#         return c != NULL;
#     }
#     return bfalse;
# }
def be_isderived(vm, index):
    sup = _getclass(vm.stack[be_indexof(vm, -1)])
    if sup:
        c = _getclass(vm.stack[be_indexof(vm, index)])
        while c is not None and c is not sup:
            c = be_class_super(c)
        return c is not None
    return False

# BERRY_API const char *be_typename(bvm *vm, int index)
def be_typename(vm, index):
    be_vm = _lazy_be_vm()
    v = vm.stack[be_indexof(vm, index)]
    return be_vm.be_vtype2str(v)

# BERRY_API const char *be_classname(bvm *vm, int index)
# {
#     bvalue *v = be_indexof(vm, index);
#     if (var_isclass(v)) { bclass *c = var_toobj(v); return str(be_class_name(c)); }
#     if (var_isinstance(v)) { binstance *i = var_toobj(v); return str(be_instance_name(i)); }
#     return NULL;
# }
def be_classname(vm, index):
    be_str = _lazy_be_string()
    v = vm.stack[be_indexof(vm, index)]
    if var_isclass(v):
        c = var_toobj(v)
        return be_str.be_str2cstr(be_class_name(c))
    if var_isinstance(v):
        i = var_toobj(v)
        return be_str.be_str2cstr(be_instance_name(i))
    return None

# BERRY_API bbool be_classof(bvm *vm, int index)
# {
#     bvalue *v = be_indexof(vm, index);
#     if (var_isinstance(v)) {
#         bvalue *top = be_incrtop(vm);
#         binstance *ins = var_toobj(v);
#         var_setclass(top, be_instance_class(ins));
#         return btrue;
#     }
#     return bfalse;
# }
def be_classof(vm, index):
    v = vm.stack[be_indexof(vm, index)]
    if var_isinstance(v):
        _ensure_stack(vm, 1)
        idx = be_incrtop(vm)
        ins = var_toobj(v)
        var_setclass(vm.stack[idx], be_instance_class(ins))
        return True
    return False


# ============================================================================
# Container creation
# ============================================================================

# BERRY_API void be_newlist(bvm *vm)
# { blist *list = be_list_new(vm); bvalue *top = be_incrtop(vm); var_setlist(top, list); }
def be_newlist(vm):
    be_lst = _lazy_be_list()
    lst = be_lst.be_list_new(vm)
    _ensure_stack(vm, 1)
    idx = be_incrtop(vm)
    var_setlist(vm.stack[idx], lst)

# BERRY_API void be_newmap(bvm *vm)
# { bmap *map = be_map_new(vm); bvalue *top = be_incrtop(vm); var_setobj(top, BE_MAP, map); }
def be_newmap(vm):
    be_mp = _lazy_be_map()
    m = be_mp.be_map_new(vm)
    _ensure_stack(vm, 1)
    idx = be_incrtop(vm)
    var_setobj(vm.stack[idx], BE_MAP, m)

# BERRY_API void be_newmodule(bvm *vm)
# { bmodule *mod = be_module_new(vm); bvalue *top = be_incrtop(vm); var_setobj(top, BE_MODULE, mod); }
def be_newmodule(vm):
    be_mod = _lazy_be_module()
    mod = be_mod.be_module_new(vm)
    _ensure_stack(vm, 1)
    idx = be_incrtop(vm)
    var_setobj(vm.stack[idx], BE_MODULE, mod)

# BERRY_API void be_newobject(bvm *vm, const char *name)
# {
#     be_getbuiltin(vm, name);
#     be_call(vm, 0);
#     be_getmember(vm, -1, ".p");
# }
def be_newobject(vm, name):
    be_getbuiltin(vm, name)
    be_call(vm, 0)
    be_getmember(vm, -1, ".p")

# BERRY_API bbool be_setname(bvm *vm, int index, const char *name)
# {
#     bvalue *v = be_indexof(vm, index);
#     if (var_ismodule(v)) {
#         bmodule *module = var_toobj(v);
#         return be_module_setname(module, be_newstr(vm, name));
#     }
#     return bfalse;
# }
def be_setname(vm, index, name):
    be_str = _lazy_be_string()
    be_mod = _lazy_be_module()
    v = vm.stack[be_indexof(vm, index)]
    if var_ismodule(v):
        module = var_toobj(v)
        return be_mod.be_module_setname(module, be_str.be_newstr(vm, name))
    return False


# ============================================================================
# Global/builtin variable access
# ============================================================================

# BERRY_API bbool be_getglobal(bvm *vm, const char *name)
# {
#     int idx = be_global_find(vm, be_newstr(vm, name));
#     bvalue *top = be_incrtop(vm);
#     if (idx >= 0) { *top = *be_global_var(vm, idx); return btrue; }
#     var_setnil(top);
#     return bfalse;
# }
def be_getglobal(vm, name):
    be_str = _lazy_be_string()
    be_var = _lazy_be_var()
    idx = be_var.be_global_find(vm, be_str.be_newstr(vm, name))
    _ensure_stack(vm, 1)
    top_idx = be_incrtop(vm)
    if idx >= 0:
        gvar = be_var.be_global_var(vm, idx)
        var_setval(vm.stack[top_idx], gvar)
        return True
    var_setnil(vm.stack[top_idx])
    return False

# BERRY_API void be_setglobal(bvm *vm, const char *name)
# {
#     int idx;
#     bstring *s = be_newstr(vm, name);
#     bvalue *v = be_incrtop(vm);
#     var_setstr(v, s);
#     idx = be_global_new(vm, s);
#     v = be_global_var(vm, idx);
#     *v = *be_indexof(vm, -2);
#     be_stackpop(vm, 1);
# }
def be_setglobal(vm, name):
    be_str = _lazy_be_string()
    be_var = _lazy_be_var()
    s = be_str.be_newstr(vm, name)
    _ensure_stack(vm, 1)
    v_idx = be_incrtop(vm)
    var_setstr(vm.stack[v_idx], s)
    idx = be_var.be_global_new(vm, s)
    gvar = be_var.be_global_var(vm, idx)
    src = vm.stack[be_indexof(vm, -2)]
    var_setval(gvar, src)
    be_stackpop(vm, 1)

# BERRY_API bbool be_getbuiltin(bvm *vm, const char *name)
# {
#     int idx = be_builtin_find(vm, be_newstr(vm, name));
#     bvalue *top = be_incrtop(vm);
#     if (idx > -1) { *top = *be_global_var(vm, idx); return btrue; }
#     var_setnil(top);
#     return bfalse;
# }
def be_getbuiltin(vm, name):
    be_str = _lazy_be_string()
    be_var = _lazy_be_var()
    idx = be_var.be_builtin_find(vm, be_str.be_newstr(vm, name))
    _ensure_stack(vm, 1)
    top_idx = be_incrtop(vm)
    if idx > -1:
        gvar = be_var.be_global_var(vm, idx)
        var_setval(vm.stack[top_idx], gvar)
        return True
    var_setnil(vm.stack[top_idx])
    return False


# ============================================================================
# Member access
# ============================================================================

# BERRY_API bbool be_setmember(bvm *vm, int index, const char *k)
# {
#     bvalue *o = be_indexof(vm, index);
#     bvalue *v = be_indexof(vm, -1);
#     if (var_isinstance(o)) {
#         bstring *key = be_newstr(vm, k);
#         binstance *obj = var_toobj(o);
#         return be_instance_setmember(vm, obj, key, v);
#     } else if (var_ismodule(o)) {
#         bstring *key = be_newstr(vm, k);
#         bmodule *mod = var_toobj(o);
#         return be_module_setmember(vm, mod, key, v);
#     } else if (var_isclass(o)) {
#         bstring *key = be_newstr(vm, k);
#         bclass *cl = var_toobj(o);
#         return be_class_setmember(vm, cl, key, v);
#     }
#     return bfalse;
# }
def be_setmember(vm, index, k):
    be_str = _lazy_be_string()
    be_cls = _lazy_be_class()
    be_mod = _lazy_be_module()
    o = vm.stack[be_indexof(vm, index)]
    v = vm.stack[be_indexof(vm, -1)]
    if var_isinstance(o):
        key = be_str.be_newstr(vm, k)
        obj = var_toobj(o)
        return be_cls.be_instance_setmember(vm, obj, key, v)
    elif var_ismodule(o):
        key = be_str.be_newstr(vm, k)
        mod = var_toobj(o)
        return be_mod.be_module_setmember(vm, mod, key, v)
    elif var_isclass(o):
        key = be_str.be_newstr(vm, k)
        cl = var_toobj(o)
        return be_cls.be_class_setmember(vm, cl, key, v)
    return False

# BERRY_API bbool be_copy(bvm *vm, int index)
# {
#     bvalue *v = be_indexof(vm, index);
#     bvalue *top = be_incrtop(vm);
#     if (var_type(v) == BE_LIST) {
#         blist *list = be_list_copy(vm, var_toobj(v));
#         var_setlist(top, list);
#         return btrue;
#     }
#     var_setnil(top);
#     return bfalse;
# }
def be_copy(vm, index):
    be_lst = _lazy_be_list()
    v = vm.stack[be_indexof(vm, index)]
    _ensure_stack(vm, 1)
    idx = be_incrtop(vm)
    top = vm.stack[idx]
    if var_type(v) == BE_LIST:
        lst = be_lst.be_list_copy(vm, var_toobj(v))
        var_setlist(top, lst)
        return True
    var_setnil(top)
    return False

# static int ins_member(bvm *vm, int index, const char *k, bbool onlyins)
def _ins_member(vm, index, k, onlyins):
    """Internal member lookup. Returns the type found (or BE_NIL)."""
    be_str = _lazy_be_string()
    be_cls = _lazy_be_class()
    be_mod = _lazy_be_module()

    type_ = BE_NIL
    o = vm.stack[be_indexof(vm, index)]
    _ensure_stack(vm, 1)
    top_idx = be_incrtop(vm)
    top = vm.stack[top_idx]
    var_setnil(top)

    if var_isinstance(o):
        obj = var_toobj(o)
        type_ = be_cls.be_instance_member(vm, obj, be_str.be_newstr(vm, k), top)
    elif var_isclass(o) and not onlyins:
        cl = var_toobj(o)
        type_ = be_cls.be_class_member(vm, cl, be_str.be_newstr(vm, k), top)
    elif var_ismodule(o) and not onlyins:
        module = var_toobj(o)
        type_ = be_mod.be_module_attr(vm, module, be_str.be_newstr(vm, k), top)
    return type_

# BERRY_API bbool be_getmember(bvm *vm, int index, const char *k)
# { return ins_member(vm, index, k, bfalse) != BE_NONE; }
def be_getmember(vm, index, k):
    return _ins_member(vm, index, k, False) != BE_NONE

# BERRY_API bbool be_getmethod(bvm *vm, int index, const char *k)
# { return basetype(ins_member(vm, index, k, btrue)) == BE_FUNCTION; }
def be_getmethod(vm, index, k):
    return basetype(_ins_member(vm, index, k, True)) == BE_FUNCTION


# ============================================================================
# Index access (get/set)
# ============================================================================

# BERRY_API bbool be_getindex(bvm *vm, int index)
# {
#     bvalue *o = be_indexof(vm, index);
#     bvalue *k = be_indexof(vm, -1);
#     bvalue *dst = be_incrtop(vm);
#     switch (var_type(o)) {
#     case BE_LIST:
#         if (var_isint(k)) {
#             blist *list = cast(blist*, var_toobj(o));
#             int idx = var_toidx(k);
#             bvalue *src = be_list_index(list, idx);
#             if (src) { var_setval(dst, src); return btrue; }
#         }
#         break;
#     case BE_MAP:
#         if (!var_isnil(k)) {
#             bmap *map = cast(bmap*, var_toobj(o));
#             bvalue *src = be_map_find(vm, map, k);
#             if (src) { var_setval(dst, src); return btrue; }
#         }
#         break;
#     default: break;
#     }
#     var_setnil(dst);
#     return bfalse;
# }
def be_getindex(vm, index):
    be_lst = _lazy_be_list()
    be_mp = _lazy_be_map()
    o = vm.stack[be_indexof(vm, index)]
    k = vm.stack[be_indexof(vm, -1)]
    _ensure_stack(vm, 1)
    dst_idx = be_incrtop(vm)
    dst = vm.stack[dst_idx]
    t = var_type(o)
    if t == BE_LIST:
        if var_isint(k):
            lst = var_toobj(o)
            idx = var_toidx(k)
            src = be_lst.be_list_index(lst, idx)
            if src is not None:
                var_setval(dst, src)
                return True
    elif t == BE_MAP:
        if not var_isnil(k):
            mp = var_toobj(o)
            src = be_mp.be_map_find(vm, mp, k)
            if src is not None:
                var_setval(dst, src)
                return True
    var_setnil(dst)
    return False

# static bvalue* list_setindex(blist *list, bvalue *key)
def _list_setindex(lst, key):
    idx = var_toidx(key)
    if idx < 0:
        idx = lst.count + idx
    if idx < 0 or idx >= lst.count:
        return None
    return be_list_at(lst, idx)

# BERRY_API bbool be_setindex(bvm *vm, int index)
# {
#     bvalue *dst = NULL;
#     bvalue *o = be_indexof(vm, index);
#     bvalue *k = be_indexof(vm, -2);
#     bvalue *v = be_indexof(vm, -1);
#     switch (var_type(o)) {
#     case BE_LIST:
#         if (var_isint(k)) { blist *list = var_toobj(o); dst = list_setindex(list, k); }
#         break;
#     case BE_MAP:
#         if (!var_isnil(k)) { bmap *map = var_toobj(o); dst = be_map_insert(vm, map, k, NULL); }
#         break;
#     default: break;
#     }
#     if (dst) { var_setval(dst, v); return btrue; }
#     return bfalse;
# }
def be_setindex(vm, index):
    be_mp = _lazy_be_map()
    o = vm.stack[be_indexof(vm, index)]
    k = vm.stack[be_indexof(vm, -2)]
    v = vm.stack[be_indexof(vm, -1)]
    dst = None
    t = var_type(o)
    if t == BE_LIST:
        if var_isint(k):
            lst = var_toobj(o)
            dst = _list_setindex(lst, k)
    elif t == BE_MAP:
        if not var_isnil(k):
            mp = var_toobj(o)
            dst = be_mp.be_map_insert(vm, mp, k, None)
    if dst is not None:
        var_setval(dst, v)
        return True
    return False


# ============================================================================
# Upvalue access
# ============================================================================

# BERRY_API void be_getupval(bvm *vm, int index, int pos)
# {
#     bvalue *f = index ? be_indexof(vm, index) : vm->cf->func;
#     bvalue *uv, *top = be_incrtop(vm);
#     if (var_istype(f, BE_NTVCLOS)) {
#         bntvclos *nf = var_toobj(f);
#         uv = be_ntvclos_upval(nf, pos)->value;
#         var_setval(top, uv);
#     } else {
#         var_setnil(top);
#     }
# }
def be_getupval(vm, index, pos):
    if index:
        f = vm.stack[be_indexof(vm, index)]
    else:
        f = vm.stack[vm.cf.func]
    _ensure_stack(vm, 1)
    top_idx = be_incrtop(vm)
    top = vm.stack[top_idx]
    if var_istype(f, BE_NTVCLOS):
        nf = var_toobj(f)
        uv = nf.upvals[pos].value
        var_setval(top, uv)
    else:
        var_setnil(top)

# BERRY_API bbool be_setupval(bvm *vm, int index, int pos)
# {
#     bvalue *f = index ? be_indexof(vm, index) : vm->cf->func;
#     bvalue *uv, *v = be_indexof(vm, -1);
#     if (var_istype(f, BE_NTVCLOS)) {
#         bntvclos *nf = var_toobj(f);
#         uv = be_ntvclos_upval(nf, pos)->value;
#         var_setval(uv, v);
#         return btrue;
#     }
#     return bfalse;
# }
def be_setupval(vm, index, pos):
    if index:
        f = vm.stack[be_indexof(vm, index)]
    else:
        f = vm.stack[vm.cf.func]
    v = vm.stack[be_indexof(vm, -1)]
    if var_istype(f, BE_NTVCLOS):
        nf = var_toobj(f)
        uv = nf.upvals[pos].value
        var_setval(uv, v)
        return True
    return False


# ============================================================================
# Container data operations
# ============================================================================

# BERRY_API int be_data_size(bvm *vm, int index)
def be_data_size(vm, index):
    be_mp = _lazy_be_map()
    v = vm.stack[be_indexof(vm, index)]
    if var_islist(v):
        lst = var_toobj(v)
        return be_list_count(lst)
    elif var_ismap(v):
        mp = var_toobj(v)
        return be_map_count(mp)
    return -1

# BERRY_API void be_data_push(bvm *vm, int index)
def be_data_push(vm, index):
    be_lst = _lazy_be_list()
    o = vm.stack[be_indexof(vm, index)]
    v = vm.stack[be_indexof(vm, -1)]
    if var_islist(o):
        lst = var_toobj(o)
        be_lst.be_list_push(vm, lst, v)

# BERRY_API bbool be_data_insert(bvm *vm, int index)
def be_data_insert(vm, index):
    be_lst = _lazy_be_list()
    be_mp = _lazy_be_map()
    o = vm.stack[be_indexof(vm, index)]
    k = vm.stack[be_indexof(vm, -2)]
    v = vm.stack[be_indexof(vm, -1)]
    t = var_type(o)
    if t == BE_MAP:
        if not var_isnil(k):
            mp = var_toobj(o)
            dst = be_mp.be_map_find(vm, mp, k)
            if dst is None:
                return be_mp.be_map_insert(vm, mp, k, v) is not None
    elif t == BE_LIST:
        if var_isint(k):
            lst = var_toobj(o)
            return be_lst.be_list_insert(vm, lst, var_toidx(k), v) is not None
    return False

# BERRY_API bbool be_data_remove(bvm *vm, int index)
def be_data_remove(vm, index):
    be_lst = _lazy_be_list()
    be_mp = _lazy_be_map()
    o = vm.stack[be_indexof(vm, index)]
    k = vm.stack[be_indexof(vm, -1)]
    t = var_type(o)
    if t == BE_MAP:
        if not var_isnil(k):
            mp = var_toobj(o)
            return be_mp.be_map_remove(vm, mp, k)
    elif t == BE_LIST:
        if var_isint(k):
            lst = var_toobj(o)
            return be_lst.be_list_remove(vm, lst, var_toidx(k))
    return False

# BERRY_API bbool be_data_merge(bvm *vm, int index)
def be_data_merge(vm, index):
    be_lst = _lazy_be_list()
    a = vm.stack[be_indexof(vm, index)]
    b = vm.stack[be_indexof(vm, -1)]
    if var_islist(a) and var_islist(b):
        dst = var_toobj(a)
        src = var_toobj(b)
        be_lst.be_list_merge(vm, dst, src)
        return True
    return False

# BERRY_API void be_data_resize(bvm *vm, int index)
def be_data_resize(vm, index):
    be_lst = _lazy_be_list()
    o = vm.stack[be_indexof(vm, index)]
    v = vm.stack[be_indexof(vm, -1)]
    if var_islist(o):
        lst = var_toobj(o)
        if var_isint(v):
            be_lst.be_list_resize(vm, lst, var_toidx(v))

# BERRY_API void be_data_reverse(bvm *vm, int index)
def be_data_reverse(vm, index):
    be_lst = _lazy_be_list()
    v = vm.stack[be_indexof(vm, index)]
    if var_type(v) == BE_LIST:
        be_lst.be_list_reverse(var_toobj(v))


# ============================================================================
# Iterator functions
# ============================================================================

# BERRY_API bbool be_pushiter(bvm *vm, int index)
# {
#     bvalue *v = be_indexof(vm, index);
#     if (var_ismap(v)) {
#         bvalue *iter = be_incrtop(vm);
#         var_setobj(iter, BE_COMPTR, NULL);
#         return btrue;
#     } else if (var_islist(v)) {
#         blist *list = var_toobj(v);
#         bvalue *iter = be_incrtop(vm);
#         var_setobj(iter, BE_COMPTR, be_list_data(list) - 1);
#         return btrue;
#     }
#     return bfalse;
# }
def be_pushiter(vm, index):
    v = vm.stack[be_indexof(vm, index)]
    if var_ismap(v):
        _ensure_stack(vm, 1)
        idx = be_incrtop(vm)
        var_setobj(vm.stack[idx], BE_COMPTR, None)
        return True
    elif var_islist(v):
        _ensure_stack(vm, 1)
        idx = be_incrtop(vm)
        # Store iterator as integer index: -1 means "before first element"
        var_setobj(vm.stack[idx], BE_COMPTR, -1)
        return True
    return False

# static int list_next(bvm *vm)
def _list_next(vm):
    """Advance list iterator and push the next value. Returns 1."""
    iter_val = vm.stack[be_indexof(vm, -1)]
    cur = var_toobj(iter_val)  # current index
    next_idx = cur + 1
    var_setobj(iter_val, BE_COMPTR, next_idx)
    # Get the list from the stack position before the iterator
    # The list is at index -2 relative to the iterator push
    # But we need to find it — the caller passes the list index
    # Actually, list_next in C uses the raw pointer. In Python we store
    # the index into list.data. We need the list object.
    # The list is at the index passed to be_iter_next, which is before the iter.
    # We'll get it from the calling context. For now, push the value.
    _ensure_stack(vm, 1)
    dst_idx = be_incrtop(vm)
    dst = vm.stack[dst_idx]
    # We need the list — it's stored at the index the user passed to be_iter_next
    # The convention is: list at `index`, iter at top-1
    # We'll store the list ref in a helper. Actually, let's look at how C does it:
    # list_next just uses the iter pointer to walk through list data.
    # In Python, we store the index. We need the list to get data[next_idx].
    # The list is passed via the `o` parameter in be_iter_next.
    # We'll handle this in be_iter_next directly.
    var_setnil(dst)
    return 1

# static bbool list_hasnext(bvm *vm, bvalue *v)
def _list_hasnext(vm, v):
    """Check if list iterator has more elements."""
    iter_val = vm.stack[be_indexof(vm, -1)]
    lst = var_toobj(v)
    cur = var_toobj(iter_val)  # current index
    next_idx = cur + 1
    return 0 <= next_idx < lst.count

# static int map_next(bvm *vm, bvalue *v)
def _map_next(vm, v):
    """Advance map iterator and push key+value. Returns 2 or 0."""
    be_mp = _lazy_be_map()
    itvar = vm.stack[be_indexof(vm, -1)]
    iter_ref = [var_toobj(itvar)]
    mp = var_toobj(v)
    entry, iter_ref = be_mp.be_map_next(mp, iter_ref)
    var_setobj(itvar, BE_COMPTR, iter_ref[0])
    if entry is not None:
        _ensure_stack(vm, 2)
        dst_idx = vm.top_idx
        dst = vm.stack[dst_idx]
        be_map_key2value(dst, entry)
        var_setval(vm.stack[dst_idx + 1], entry.value)
        vm.top_idx += 2
        # Ensure stack is large enough
        while vm.top_idx > len(vm.stack):
            vm.stack.append(bvalue())
        return 2
    return 0

# static bbool map_hasnext(bvm *vm, bvalue *v)
def _map_hasnext(vm, v):
    be_mp = _lazy_be_map()
    node = vm.stack[be_indexof(vm, -1)]
    iter_ref = [var_toobj(node)]
    mp = var_toobj(v)
    entry, _ = be_mp.be_map_next(mp, iter_ref)
    return entry is not None

# BERRY_API int be_iter_next(bvm *vm, int index)
# {
#     bvalue *o = be_indexof(vm, index);
#     if (var_islist(o)) { return list_next(vm); }
#     else if (var_ismap(o)) { return map_next(vm, o); }
#     return 0;
# }
def be_iter_next(vm, index):
    o = vm.stack[be_indexof(vm, index)]
    if var_islist(o):
        # Inline list_next with access to the list object
        iter_val = vm.stack[be_indexof(vm, -1)]
        cur = var_toobj(iter_val)
        next_idx = cur + 1
        var_setobj(iter_val, BE_COMPTR, next_idx)
        lst = var_toobj(o)
        _ensure_stack(vm, 1)
        dst_idx = be_incrtop(vm)
        if 0 <= next_idx < lst.count:
            var_setval(vm.stack[dst_idx], lst.data[next_idx])
        else:
            var_setnil(vm.stack[dst_idx])
        return 1
    elif var_ismap(o):
        return _map_next(vm, o)
    return 0

# BERRY_API bbool be_iter_hasnext(bvm *vm, int index)
# {
#     bvalue *o = be_indexof(vm, index);
#     if (var_islist(o)) { return list_hasnext(vm, o); }
#     else if (var_ismap(o)) { return map_hasnext(vm, o); }
#     return bfalse;
# }
def be_iter_hasnext(vm, index):
    o = vm.stack[be_indexof(vm, index)]
    if var_islist(o):
        return _list_hasnext(vm, o)
    elif var_ismap(o):
        return _map_hasnext(vm, o)
    return False


# ============================================================================
# Reference stack
# ============================================================================

# BERRY_API bbool be_refcontains(bvm *vm, int index)
# {
#     bvalue *v = be_indexof(vm, index);
#     binstance **ref = be_stack_base(&vm->refstack);
#     binstance **top = be_stack_top(&vm->refstack);
#     binstance *ins = var_toobj(v);
#     if (ref) {
#         while (ref <= top && *ref != ins) { ++ref; }
#         return ref <= top;
#     }
#     return bfalse;
# }
def be_refcontains(vm, index):
    be_vec = _lazy_be_vector()
    v = vm.stack[be_indexof(vm, index)]
    ins = var_toobj(v)
    if vm.refstack is None or vm.refstack.count == 0:
        return False
    base = be_vec.be_stack_base(vm.refstack)
    if base is None:
        return False
    for i in range(vm.refstack.count):
        if vm.refstack.data[i] is ins:
            return True
    return False

# BERRY_API void be_refpush(bvm *vm, int index)
# {
#     bvalue *v = be_indexof(vm, index);
#     binstance *ins = var_toobj(v);
#     be_stack_push(vm, &vm->refstack, &ins);
# }
def be_refpush(vm, index):
    be_vec = _lazy_be_vector()
    v = vm.stack[be_indexof(vm, index)]
    ins = var_toobj(v)
    be_vec.be_stack_push(vm, vm.refstack, ins)

# BERRY_API void be_refpop(bvm *vm)
# {
#     be_stack_pop(&vm->refstack);
#     if (be_stack_isempty(&vm->refstack)) {
#         be_vector_release(vm, &vm->refstack);
#     }
# }
def be_refpop(vm):
    be_vec = _lazy_be_vector()
    be_vec.be_stack_pop(vm.refstack)
    if be_vec.be_stack_isempty(vm.refstack):
        be_vec.be_vector_release(vm, vm.refstack)


# ============================================================================
# Return helpers
# ============================================================================

# BERRY_API int be_returnvalue(bvm *vm)
# {
#     bvalue *src = vm->top - 1;
#     bvalue *ret = retreg(vm);
#     var_setval(ret, src);
#     return 0;
# }
def be_returnvalue(vm):
    """Copy top-of-stack value to the return register. Returns 0."""
    src = vm.stack[vm.top_idx - 1]
    ret_idx = _retreg(vm)
    var_setval(vm.stack[ret_idx], src)
    return 0

# BERRY_API int be_returnnilvalue(bvm *vm)
# {
#     bvalue *ret = retreg(vm);
#     var_setnil(ret);
#     return 0;
# }
def be_returnnilvalue(vm):
    """Set the return register to nil. Returns 0."""
    ret_idx = _retreg(vm)
    var_setnil(vm.stack[ret_idx])
    return 0


# ============================================================================
# Code loading and execution
# ============================================================================

# BERRY_API void be_call(bvm *vm, int argc)
# {
#     bvalue *fval = vm->top - argc - 1;
#     be_dofunc(vm, fval, argc);
# }
def be_call(vm, argc):
    be_vm = _lazy_be_vm()
    fval_idx = vm.top_idx - argc - 1
    be_vm.be_dofunc(vm, fval_idx, argc)

# BERRY_API int be_pcall(bvm *vm, int argc)
# {
#     bvalue *f = vm->top - argc - 1;
#     return be_protectedcall(vm, f, argc);
# }
def be_pcall(vm, argc):
    be_exec = _lazy_be_exec()
    f_idx = vm.top_idx - argc - 1
    return be_exec.be_protectedcall(vm, f_idx, argc)

# BERRY_API void be_loadbuffer(...)  — delegated to be_exec
def be_loadbuffer(vm, name, buffer, length=None):
    """Load Berry source from a buffer string."""
    be_exec = _lazy_be_exec()
    if length is None:
        length = len(buffer)
    return be_exec.be_loadbuffer(vm, name, buffer, length)

# BERRY_API void be_loadmode(...)  — delegated to be_exec
def be_loadmode(vm, name, islocal):
    """Load Berry source from a file."""
    be_exec = _lazy_be_exec()
    return be_exec.be_loadmode(vm, name, islocal)


# ============================================================================
# Error handling
# ============================================================================

# BERRY_API void be_raise(bvm *vm, const char *except, const char *msg)
# {
#     be_pushstring(vm, except);
#     if (msg) { be_pushstring(vm, msg); }
#     else { be_pushnil(vm); }
#     be_pop(vm, 2);
#     be_save_stacktrace(vm);
#     be_throw(vm, BE_EXCEPTION);
# }
def be_raise(vm, except_name, msg=None):
    """Raise a Berry exception with the given name and optional message."""
    be_exec = _lazy_be_exec()
    be_pushstring(vm, except_name)
    if msg is not None:
        be_pushstring(vm, msg)
    else:
        be_pushnil(vm)
    be_pop(vm, 2)
    be_exec.be_save_stacktrace(vm)
    be_exec.be_throw(vm, BE_EXCEPTION)

# BERRY_API void be_stop_iteration(bvm *vm)
# { be_raise(vm, "stop_iteration", NULL); }
def be_stop_iteration(vm):
    be_raise(vm, "stop_iteration", None)

# BERRY_API int be_getexcept(bvm *vm, int code)
# {
#     if (code == BE_EXCEPTION) {
#         if (be_isstring(vm, -2)) {
#             const char *except = be_tostring(vm, -2);
#             if (!strcmp(except, "syntax_error")) return BE_SYNTAX_ERROR;
#             if (!strcmp(except, "io_error")) return BE_IO_ERROR;
#         }
#         return BE_EXEC_ERROR;
#     }
#     return code;
# }
def be_getexcept(vm, code):
    if code == BE_EXCEPTION:
        if be_isstring(vm, -2):
            except_name = be_tostring(vm, -2)
            if except_name == "syntax_error":
                return BE_SYNTAX_ERROR
            if except_name == "io_error":
                return BE_IO_ERROR
        return BE_EXEC_ERROR
    return code


# ============================================================================
# Dump value / dump exception
# ============================================================================

# static int _dvfunc(bvm *vm, bbool esc)
def _dvfunc(vm, esc):
    """Internal: convert value at index 1 to string and write it."""
    s = be_tostring(vm, 1)
    be_writestring(s)
    ret_idx = _retreg(vm)
    var_setnil(vm.stack[ret_idx])
    return 0

def _dumpesc(vm):
    return _dvfunc(vm, True)

def _dumpdir(vm):
    return _dvfunc(vm, False)

# static int dump_value(bvm *vm, int index, bbool esc)
def _dump_value(vm, index, esc):
    top = be_top(vm) + 1
    index = be_absindex(vm, index)
    be_pushntvfunction(vm, _dumpesc if esc else _dumpdir)
    be_pushvalue(vm, index)
    res = be_pcall(vm, 1)
    be_remove(vm, top)
    be_remove(vm, top)
    if res == BE_EXCEPTION:
        be_dumpexcept(vm)
    return res

# BERRY_API void be_dumpvalue(bvm *vm, int index)
def be_dumpvalue(vm, index):
    if _dump_value(vm, index, True) == BE_OK:
        be_writenewline()

# BERRY_API void be_dumpexcept(bvm *vm)
# {
#     do {
#         if (dump_value(vm, -2, bfalse)) break;
#         be_writestring(": ");
#         if (dump_value(vm, -1, bfalse)) break;
#         be_writenewline();
#         be_tracestack(vm);
#     } while (0);
#     be_pop(vm, 2);
# }
def be_dumpexcept(vm):
    ok = True
    if _dump_value(vm, -2, False) == BE_OK:
        be_writestring(": ")
        if _dump_value(vm, -1, False) == BE_OK:
            be_writenewline()
            be_tracestack(vm)
    be_pop(vm, 2)


# ============================================================================
# I/O helpers (from be_port.c — simple Python equivalents)
# ============================================================================

def be_writestring(s):
    """Write a string to stdout (mirrors be_writestring from be_port.c)."""
    sys.stdout.write(s)

def be_writenewline():
    """Write a newline to stdout."""
    sys.stdout.write("\n")
    sys.stdout.flush()

def be_tracestack(vm):
    """Print stack traceback. Delegates to be_debug.be_tracestack."""
    from berry_port.be_debug import be_tracestack as _be_tracestack
    _be_tracestack(vm)


# ============================================================================
# Comparison functions
# ============================================================================

# BERRY_API bbool be_iseq(bvm *vm)
# { return be_vm_iseq(vm, vm->top - 2, vm->top - 1); }
def be_iseq(vm):
    be_vm = _lazy_be_vm()
    return be_vm.be_vm_iseq(vm, vm.stack[vm.top_idx - 2], vm.stack[vm.top_idx - 1])

# BERRY_API bbool be_isneq(bvm *vm)
def be_isneq(vm):
    be_vm = _lazy_be_vm()
    return be_vm.be_vm_isneq(vm, vm.stack[vm.top_idx - 2], vm.stack[vm.top_idx - 1])

# BERRY_API bbool be_islt(bvm *vm)
def be_islt(vm):
    be_vm = _lazy_be_vm()
    return be_vm.be_vm_islt(vm, vm.stack[vm.top_idx - 2], vm.stack[vm.top_idx - 1])

# BERRY_API bbool be_isle(bvm *vm)
def be_isle(vm):
    be_vm = _lazy_be_vm()
    return be_vm.be_vm_isle(vm, vm.stack[vm.top_idx - 2], vm.stack[vm.top_idx - 1])

# BERRY_API bbool be_isgt(bvm *vm)
def be_isgt(vm):
    be_vm = _lazy_be_vm()
    return be_vm.be_vm_isgt(vm, vm.stack[vm.top_idx - 2], vm.stack[vm.top_idx - 1])

# BERRY_API bbool be_isge(bvm *vm)
def be_isge(vm):
    be_vm = _lazy_be_vm()
    return be_vm.be_vm_isge(vm, vm.stack[vm.top_idx - 2], vm.stack[vm.top_idx - 1])


# ============================================================================
# Stack require (delegated)
# ============================================================================

def be_stack_require(vm, count):
    """Ensure the VM value stack can hold `count` more items above top."""
    be_exec = _lazy_be_exec()
    be_exec.be_stack_require(vm, count)


# ============================================================================
# VM lifecycle (delegated to be_vm.py)
# ============================================================================

def be_vm_new():
    """Create and initialize a new Berry VM instance."""
    be_vm = _lazy_be_vm()
    return be_vm.be_vm_new()

def be_vm_delete(vm):
    """Destroy a Berry VM instance."""
    be_vm = _lazy_be_vm()
    be_vm.be_vm_delete(vm)
