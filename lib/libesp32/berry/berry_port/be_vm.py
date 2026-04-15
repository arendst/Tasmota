"""
Berry virtual machine.
Mirrors: src/be_vm.c / src/be_vm.h

This module implements the Berry register-based bytecode VM, including:
- bvm structure with all fields
- bcallframe structure
- vm_exec dispatch loop with all 48 opcodes
- be_dofunc entry point, precall, push_closure, ret_native
- Comparison and operator overloading helpers
- String/range construction helpers

Original C code is included as comments for each function.
"""

import math

from berry_port.berry_conf import (
    BE_STACK_FREE_MIN,
    BE_STACK_START,
    BE_BYTES_MAX_SIZE,
    BE_USE_PERF_COUNTERS,
    BE_USE_DEBUG_HOOK,
    BE_USE_PREPROCESSOR,
    BE_VM_OBSERVABILITY_SAMPLING,
)

from berry_port.be_object import (
    # Type constants
    BE_NONE, BE_NIL, BE_INT, BE_REAL, BE_BOOL, BE_COMPTR, BE_INDEX,
    BE_FUNCTION, BE_STRING, BE_CLASS, BE_INSTANCE, BE_PROTO, BE_LIST,
    BE_MAP, BE_MODULE, BE_COMOBJ,
    BE_NTVFUNC, BE_CLOSURE, BE_NTVCLOS, BE_CTYPE_FUNC, BE_STATIC,
    # Varg flags
    BE_VA_VARARG,
    # GC constants
    GC_CONST,
    # Error codes
    BE_OK, BE_EXCEPTION, BE_MALLOC_FAIL,
    # Call frame flags
    NONE_FLAG, BASE_FRAME, PRIM_FUNC,
    # Structures
    bvalue, bcallframe, bcallsnapshot, bgc, bstringtable, bmoduledesc,
    bglobaldesc, bexecptframe, bupval,
    # var_* helpers
    var_type, var_basetype, var_primetype, var_isstatic,
    var_isnil, var_isbool, var_isint, var_isreal, var_isstr,
    var_isclosure, var_isntvclos, var_isntvfunc, var_isfunction,
    var_isclass, var_isinstance, var_islist, var_ismap, var_ismodule,
    var_iscomptr, var_isnumber, var_isindex,
    var_setnil, var_setval, var_setbool, var_setint, var_setreal,
    var_setstr, var_setinstance, var_setclass, var_setclosure,
    var_setntvclos, var_setntvfunc, var_setlist, var_setmap,
    var_setmodule, var_setindex, var_setproto, var_setcomptr,
    var_settype, var_markstatic, var_setobj,
    var_tobool, var_toint, var_toreal, var_tostr, var_toobj,
    var_tontvfunc, var_toidx,
    # GC helpers
    gc_isconst,
    # Object helpers
    basetype, cast_int,
    be_class_name, be_class_setsuper,
    be_instance_name, be_instance_class,
    str_len,
)

from berry_port.be_decoder import (
    IGET_OP, IGET_RA, IGET_RKB, IGET_RKC, IGET_Bx, IGET_sBx,
    isKB, isKC, KR2idx, isK,
    # Opcodes
    OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD, OP_LT, OP_LE, OP_EQ,
    OP_NE, OP_GT, OP_GE, OP_CONNECT, OP_AND, OP_OR, OP_XOR,
    OP_SHL, OP_SHR, OP_NEG, OP_FLIP, OP_LDNIL, OP_LDBOOL,
    OP_LDINT, OP_LDCONST, OP_GETGBL, OP_GETNGBL, OP_SETNGBL,
    OP_SETGBL, OP_GETUPV, OP_SETUPV, OP_MOVE, OP_JMP, OP_JMPT,
    OP_JMPF, OP_CLOSURE, OP_CLASS, OP_GETMBR, OP_GETMET, OP_SETMBR,
    OP_GETIDX, OP_SETIDX, OP_SETSUPER, OP_CLOSE, OP_IMPORT,
    OP_CATCH, OP_RAISE, OP_EXBLK, OP_CALL, OP_RET,
)

# ============================================================================
# Lazy imports to break circular dependencies
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

def _lazy_be_mem():
    import berry_port.be_mem as m
    return m

def _lazy_be_list():
    import berry_port.be_list as m
    return m

def _lazy_be_map():
    import berry_port.be_map as m
    return m

def _lazy_be_api():
    import berry_port.be_api as m
    return m


# ============================================================================
# NOT_METHOD sentinel
# ============================================================================

# #define NOT_METHOD          BE_NONE
NOT_METHOD = BE_NONE


# ============================================================================
# Forward-declared string utility stubs
# ============================================================================
# These functions belong to be_strlib.py (task 9.9) and be_api.py (task 9.4)
# which are not yet implemented. We provide minimal stubs here so the VM
# can function. They will be replaced by proper imports when those modules
# are ported.

def str_literal(vm, s):
    """Equivalent to C macro: str_literal(vm, s) -> be_newstrn(vm, s, len(s))"""
    mod = _lazy_be_string()
    return mod.be_newstrn(vm, s, len(s))

def be_str2cstr(s):
    """Get the C string (Python str) from a bstring object."""
    mod = _lazy_be_string()
    return mod.be_str2cstr(s)


def be_vtype2str(v):
    """Return a human-readable type name string for a bvalue.

    Mirrors: src/be_object.c be_vtype2str()
    """
    # const char* be_vtype2str(bvalue *v)
    # {
    #     switch(var_primetype(v)) {
    #     case BE_NIL: return "nil";
    #     ...
    #     }
    # }
    t = var_primetype(v)
    if t == BE_NIL:
        return "nil"
    elif t == BE_INT:
        return "int"
    elif t == BE_REAL:
        return "real"
    elif t == BE_BOOL:
        return "bool"
    elif t in (BE_CLOSURE, BE_NTVCLOS, BE_CTYPE_FUNC, BE_NTVFUNC):
        return "function"
    elif t == BE_PROTO:
        return "proto"
    elif t == BE_CLASS:
        return "class"
    elif t == BE_STRING:
        return "string"
    elif t == BE_LIST:
        return "list"
    elif t == BE_MAP:
        return "map"
    elif t == BE_INSTANCE:
        return "instance"
    elif t == BE_MODULE:
        return "module"
    elif t == BE_INDEX:
        return "var"
    elif t == BE_COMPTR:
        return "ptr"
    else:
        return "invalid type"


def be_pushfstring(vm, fmt, *args):
    """Format a string and push it onto the VM stack.

    Stub — full implementation in be_strlib.py / be_api.py (tasks 9.4/9.9).
    Returns the formatted Python string for use in error messages.
    """
    try:
        result = fmt % args if args else fmt
    except (TypeError, ValueError):
        result = fmt
    return result


def be_strcat(vm, s1, s2):
    """Concatenate two bstrings, returning a new interned bstring.

    Stub — full implementation in be_strlib.py (task 9.9).
    """
    mod = _lazy_be_string()
    c1 = mod.be_str2cstr(s1)
    c2 = mod.be_str2cstr(s2)
    return mod.be_newstr(vm, c1 + c2)


def be_strmul(vm, s, n):
    """Multiply a bstring by integer n, returning a new bstring.

    Stub — full implementation in be_strlib.py (task 9.9).
    """
    mod = _lazy_be_string()
    c = mod.be_str2cstr(s)
    if n <= 0:
        return mod.be_newstr(vm, "")
    return mod.be_newstr(vm, c * n)


def be_strcmp(s1, s2):
    """Compare two bstrings lexicographically. Returns <0, 0, or >0.

    Stub — full implementation in be_strlib.py (task 9.9).
    """
    mod = _lazy_be_string()
    c1 = mod.be_str2cstr(s1)
    c2 = mod.be_str2cstr(s2)
    if c1 < c2:
        return -1
    elif c1 > c2:
        return 1
    return 0


def be_val2str(vm, index):
    """Convert value at stack index to string in-place.

    Mirrors C be_val2str from be_strlib.c.
    For instances, calls the tostring() method.
    For simple types, formats directly.
    """
    # /* void be_val2str(bvm *vm, int index)
    # {
    #     bstring *s;
    #     int idx = be_absindex(vm, index) - 1;
    #     bvalue *v = vm->reg + idx;
    #     if (var_isstr(v)) return;
    #     s = var_isinstance(v) ? ins2str(vm, idx) : sim2str(vm, v);
    #     v = vm->reg + idx;
    #     var_setstr(v, s);
    # } */
    mod = _lazy_be_string()
    # Compute absolute index (1-based to 0-based offset from reg)
    if index > 0:
        idx = index - 1
    else:
        idx = vm.top_idx - vm.reg_idx + index
    v = vm.stack[vm.reg_idx + idx]
    if var_isstr(v):
        return
    if var_isinstance(v):
        s = _ins2str(vm, idx)
    else:
        s = _sim2str(vm, v)
    # Re-fetch v (stack may have moved)
    v = vm.stack[vm.reg_idx + idx]
    var_setstr(v, s)


def _sim2str(vm, v):
    """Convert a simple (non-instance) value to a bstring."""
    mod = _lazy_be_string()
    t = var_type(v)
    if t == BE_NIL:
        return mod.be_newstr(vm, "nil")
    elif t == BE_BOOL:
        return mod.be_newstr(vm, "true" if var_tobool(v) else "false")
    elif t == BE_INT or t == BE_INDEX:
        return mod.be_newstr(vm, str(var_toint(v)))
    elif t == BE_REAL:
        return mod.be_newstr(vm, _format_real(var_toreal(v)))
    elif t in (BE_CLOSURE, BE_NTVCLOS, BE_NTVFUNC, BE_CTYPE_FUNC):
        return mod.be_newstr(vm, "<function: %s>" % hex(id(var_toobj(v))))
    elif t == BE_CLASS:
        cls = var_toobj(v)
        name = be_str2cstr(be_class_name(cls))
        return mod.be_newstr(vm, "<class: %s>" % name)
    elif t == BE_MODULE:
        mname = be_module_name(var_toobj(v))
        if mname:
            return mod.be_newstr(vm, "<module: %s>" % mname)
        else:
            return mod.be_newstr(vm, "<module: %s>" % hex(id(var_toobj(v))))
    elif t == BE_COMPTR:
        return mod.be_newstr(vm, "<ptr: %s>" % hex(id(v.v)))
    else:
        return mod.be_newstr(vm, "(unknown value)")


def _ins2str(vm, idx):
    """Convert an instance value to a bstring by calling its tostring() method.

    Mirrors C ins2str from be_strlib.c.
    """
    # /* static bstring* ins2str(bvm *vm, int idx) */
    mod = _lazy_be_string()
    be_cls = _lazy_be_class()
    s = str_literal(vm, "tostring")
    obj = var_toobj(vm.stack[vm.reg_idx + idx])
    # Get method 'tostring'
    dst = bvalue()
    mtype = be_cls.be_instance_member(vm, obj, s, dst)
    # Push the method to stack top
    from berry_port import be_exec as _be_exec
    if vm.top_idx + 2 >= len(vm.stack):
        _be_exec.be_stack_expansion(vm, 4)
    var_setval(vm.stack[vm.top_idx], dst)
    vm.top_idx += 1  # be_incrtop
    if basetype(mtype) != BE_FUNCTION:
        # No tostring method — return "<instance: ClassName()>"
        name = be_str2cstr(be_class_name(be_instance_class(obj)))
        vm.top_idx -= 1  # pop the method
        return mod.be_newstr(vm, "<instance: %s()>" % name)
    else:
        # Push self as argument: *vm->top = vm->reg[idx]
        var_setval(vm.stack[vm.top_idx], vm.stack[vm.reg_idx + idx])
        # be_dofunc(vm, vm->top - 1, 1) — call method at top-1 with 1 arg
        be_dofunc(vm, vm.top_idx - 1, 1)
        # After be_dofunc, return value is at the function slot (top_idx - 1)
        # be_stackpop(vm, 1) — pop one value
        vm.top_idx -= 1
        # Now vm->top points to the return value
        result = vm.stack[vm.top_idx]
        if not var_isstr(result):
            name = be_str2cstr(be_instance_name(obj))
            if not name:
                name = "<anonymous>"
            be_raise(vm, "runtime_error",
                     "the value of `%s::tostring()` is not a 'string'" % name)
        return var_tostr(result)


def _format_real(r):
    """Format a real number matching C's %g format."""
    s = "%g" % r
    return s


def be_strindex(vm, s, idx):
    """String subscript operation s[idx].

    Stub — full implementation in be_strlib.py (task 9.9).
    """
    mod = _lazy_be_string()
    c = mod.be_str2cstr(s)
    if var_isint(idx):
        i = var_toint(idx)
        if i < 0:
            i = len(c) + i
        if 0 <= i < len(c):
            return mod.be_newstr(vm, c[i])
        else:
            vm_error(vm, "index_error", "string index out of range")
    else:
        vm_error(vm, "type_error",
                 "string indices must be integers, not '%s'",
                 be_vtype2str(idx))
    return mod.be_newstr(vm, "")  # unreachable


# ============================================================================
# Module functions — delegated to be_module.py
# ============================================================================

def _lazy_be_module():
    import berry_port.be_module as m
    return m


def be_module_attr(vm, module, attr, dst):
    """Look up attribute in module. Returns type or BE_NONE."""
    return _lazy_be_module().be_module_attr(vm, module, attr, dst)


def be_module_name(module):
    """Get module name string."""
    return _lazy_be_module().be_module_name(module)


def be_module_setmember(vm, module, attr, src):
    """Set a member in a module. Returns True on success."""
    return _lazy_be_module().be_module_setmember(vm, module, attr, src)


def be_module_load(vm, name):
    """Load a module by name. Returns BE_OK or error code."""
    return _lazy_be_module().be_module_load(vm, name)


# ============================================================================
# be_loadlibs — delegates to be_libs.py (task 15.1)
# ============================================================================

def be_loadlibs(vm):
    """Register all built-in libraries and native modules.

    Delegates to be_libs.be_loadlibs (task 15.1).
    """
    from berry_port.be_libs import be_loadlibs as _be_loadlibs
    _be_loadlibs(vm)


# ============================================================================
# Stub for be_raise (task 9.4 — be_api.py)
# ============================================================================

def be_raise(vm, except_name, message):
    """Raise a Berry exception with the given name and message.

    Stub — full implementation in be_api.py (task 9.4).
    This minimal version pushes exception info and throws.
    """
    be_exec = _lazy_be_exec()
    # Push exception name string
    smod = _lazy_be_string()
    exc_str = smod.be_newstr(vm, except_name)
    var_setstr(vm.stack[vm.top_idx], exc_str)
    vm.top_idx += 1
    # Push message string
    if message is not None:
        msg_str = smod.be_newstr(vm, str(message))
        var_setstr(vm.stack[vm.top_idx], msg_str)
    else:
        var_setnil(vm.stack[vm.top_idx])
    vm.top_idx += 1
    # Pop 2 — values remain on stack but top is restored (matches C be_pop)
    vm.top_idx -= 2
    be_exec.be_throw(vm, BE_EXCEPTION)


# ============================================================================
# vm_error macro
# ============================================================================

# #define vm_error(vm, except, ...) \
#     be_raise(vm, except, be_pushfstring(vm, __VA_ARGS__))
def vm_error(vm, except_name, fmt, *args):
    msg = be_pushfstring(vm, fmt, *args)
    be_raise(vm, except_name, msg)


# ============================================================================
# Helper macros translated to Python
# ============================================================================

# #define var2cl(_v)          cast(bclosure*, var_toobj(_v))
def var2cl(v):
    return var_toobj(v)

# #define var2real(_v)        (var_isreal(_v) ? (_v)->v.r : (breal)(_v)->v.i)
def var2real(v):
    if var_isreal(v):
        return var_toreal(v)
    return float(var_toint(v))

# #define val2bool(v)         ((v) ? btrue : bfalse)
def val2bool(v):
    return bool(v)

# #define ibinop(op, a, b)    ((a)->v.i op (b)->v.i)
# Implemented inline in each opcode handler.


# ============================================================================
# Error helper functions
# ============================================================================

# static void attribute_error(bvm *vm, const char *t, bvalue *b, bvalue *c)
# {
#     const char *attr = var_isstr(c) ? str(var_tostr(c)) : be_vtype2str(c);
#     vm_error(vm, "attribute_error",
#         "'%s' value has no %s '%s'", be_vtype2str(b), t, attr);
# }
def attribute_error(vm, t, b, c):
    attr = be_str2cstr(var_tostr(c)) if var_isstr(c) else be_vtype2str(c)
    vm_error(vm, "attribute_error",
             "'%s' value has no %s '%s'", be_vtype2str(b), t, attr)

# static void binop_error(bvm *vm, const char *op, bvalue *a, bvalue *b)
# {
#     vm_error(vm, "type_error",
#         "unsupported operand type(s) for %s: '%s' and '%s'",
#         op, be_vtype2str(a), be_vtype2str(b));
# }
def binop_error(vm, op, a, b):
    vm_error(vm, "type_error",
             "unsupported operand type(s) for %s: '%s' and '%s'",
             op, be_vtype2str(a), be_vtype2str(b))

# static void unop_error(bvm *vm, const char *op, bvalue *a)
# {
#     vm_error(vm, "type_error",
#         "unsupported operand type(s) for %s: '%s'",
#         op, be_vtype2str(a));
# }
def unop_error(vm, op, a):
    vm_error(vm, "type_error",
             "unsupported operand type(s) for %s: '%s'",
             op, be_vtype2str(a))

# static void call_error(bvm *vm, bvalue *v)
# {
#     vm_error(vm, "type_error",
#         "'%s' value is not callable", be_vtype2str(v));
# }
def call_error(vm, v):
    vm_error(vm, "type_error",
             "'%s' value is not callable", be_vtype2str(v))

# static void check_bool(bvm *vm, binstance *obj, const char *method)
# {
#     if (!var_isbool(vm->top)) {
#         const char *name = str(be_instance_name(obj));
#         vm_error(vm, "type_error",
#             "`%s::%s` return value error, the expected type is 'bool'",
#             strlen(name) ? name : "<anonymous>", method);
#     }
# }
def check_bool(vm, obj, method):
    top_val = vm.stack[vm.top_idx]
    if not var_isbool(top_val):
        name = be_str2cstr(be_instance_name(obj))
        if not name:
            name = "<anonymous>"
        vm_error(vm, "type_error",
                 "`%s::%s` return value error, the expected type is 'bool'",
                 name, method)


# ============================================================================
# precall / push_closure / ret_native
# ============================================================================

# static void precall(bvm *vm, bvalue *func, int nstack, int mode)
# {
#     bcallframe *cf;
#     int expan = nstack + BE_STACK_FREE_MIN;
#     if (vm->stacktop < func + expan) {
#         size_t fpos = func - vm->stack;
#         be_stack_expansion(vm, expan);
#         func = vm->stack + fpos;
#     }
#     be_stack_push(vm, &vm->callstack, NULL);
#     cf = be_stack_top(&vm->callstack);
#     cf->func = func - mode;
#     cf->top = vm->top;
#     cf->reg = vm->reg;
#     vm->reg = func + 1;
#     vm->top = vm->reg + nstack;
#     vm->cf = cf;
# }
def precall(vm, func_idx, nstack, mode):
    """Prepare the stack for a function/method call.

    func_idx is the index into vm.stack of the function value.
    """
    be_exec = _lazy_be_exec()
    be_vector = _lazy_be_vector()

    expan = nstack + BE_STACK_FREE_MIN
    if vm.stacktop_idx < func_idx + expan:
        be_exec.be_stack_expansion(vm, expan)

    cf = bcallframe()
    be_vector.be_stack_push(vm, vm.callstack, cf)
    cf.func = func_idx - mode
    cf.top = vm.top_idx
    cf.reg = vm.reg_idx
    vm.reg_idx = func_idx + 1
    vm.top_idx = vm.reg_idx + nstack
    vm.cf = cf


# static void push_closure(bvm *vm, bvalue *func, int nstack, int mode)
# {
#     bclosure *cl = var_toobj(func);
#     precall(vm, func, nstack, mode);
#     vm->cf->ip = vm->ip;
#     vm->cf->status = NONE_FLAG;
#     vm->ip = cl->proto->code;
# }
def push_closure(vm, func_idx, nstack, mode):
    """Prepare call of closure, setting the instruction pointer (ip)."""
    cl = var_toobj(vm.stack[func_idx])
    precall(vm, func_idx, nstack, mode)
    vm.cf.ip = vm.ip
    vm.cf.ip_idx = vm.ip_idx
    vm.cf.status = NONE_FLAG
    vm.ip = cl.proto.code
    vm.ip_idx = 0


# static void ret_native(bvm *vm)
# {
#     bcallframe *_cf = vm->cf;
#     vm->reg = _cf->reg;
#     vm->top = _cf->top;
#     be_stack_pop(&vm->callstack);
#     vm->cf = be_stack_top(&vm->callstack);
# }
def ret_native(vm):
    """Return from a native function call."""
    be_vector = _lazy_be_vector()
    _cf = vm.cf
    vm.reg_idx = _cf.reg
    vm.top_idx = _cf.top
    be_vector.be_stack_pop(vm.callstack)
    if not be_vector.be_stack_isempty(vm.callstack):
        vm.cf = vm.callstack.data[vm.callstack.end]
    else:
        vm.cf = None


# ============================================================================
# Value to bool conversion
# ============================================================================

# static bbool obj2bool(bvm *vm, bvalue *var)
# {
#     binstance *obj = var_toobj(var);
#     bstring *tobool = str_literal(vm, "tobool");
#     int type = be_instance_member(vm, obj, tobool, vm->top);
#     if (type != BE_NONE && type != BE_NIL) {
#         vm->top[1] = *var;
#         be_dofunc(vm, vm->top, 1);
#         check_bool(vm, obj, "tobool");
#         return var_tobool(vm->top);
#     }
#     return btrue;
# }
def obj2bool(vm, var_val):
    be_cls = _lazy_be_class()
    obj = var_toobj(var_val)
    tobool_str = str_literal(vm, "tobool")
    type_ = be_cls.be_instance_member(vm, obj, tobool_str, vm.stack[vm.top_idx])
    if type_ != BE_NONE and type_ != BE_NIL:
        var_setval(vm.stack[vm.top_idx + 1], var_val)  # move self to argv[0]
        be_dofunc(vm, vm.top_idx, 1)
        check_bool(vm, obj, "tobool")
        return var_tobool(vm.stack[vm.top_idx])
    return True


# bbool be_value2bool(bvm *vm, bvalue *v)
# {
#     switch (var_basetype(v)) {
#     case BE_NIL: return bfalse;
#     case BE_BOOL: return var_tobool(v);
#     case BE_INT: return val2bool(v->v.i);
#     case BE_REAL: return val2bool(v->v.r);
#     case BE_STRING: return str_len(var_tostr(v)) != 0;
#     case BE_COMPTR: return var_toobj(v) != NULL;
#     case BE_COMOBJ: return ((bcommomobj*)var_toobj(v))->data != NULL;
#     case BE_INSTANCE: return obj2bool(vm, v);
#     default: return btrue;
#     }
# }
def be_value2bool(vm, v):
    bt = var_basetype(v)
    if bt == BE_NIL:
        return False
    elif bt == BE_BOOL:
        return var_tobool(v)
    elif bt == BE_INT:
        return bool(var_toint(v))
    elif bt == BE_REAL:
        return bool(var_toreal(v))
    elif bt == BE_STRING:
        return str_len(var_tostr(v)) != 0
    elif bt == BE_COMPTR:
        return var_toobj(v) is not None
    elif bt == BE_COMOBJ:
        obj = var_toobj(v)
        return obj is not None and obj.data is not None
    elif bt == BE_INSTANCE:
        return obj2bool(vm, v)
    else:
        return True


# ============================================================================
# Object method / attribute lookup helpers
# ============================================================================

# static void obj_method(bvm *vm, bvalue *o, bstring *attr, bvalue *dst)
# {
#     binstance *obj = var_toobj(o);
#     int type = be_instance_member_simple(vm, obj, attr, dst);
#     if (basetype(type) != BE_FUNCTION) {
#         vm_error(vm, "attribute_error",
#             "the '%s' object has no method '%s'",
#             str(be_instance_name(obj)), str(attr));
#     }
# }
def obj_method(vm, o_idx, attr, dst_idx):
    """Look up a method on an instance. Raises if not found."""
    be_cls = _lazy_be_class()
    obj = var_toobj(vm.stack[o_idx])
    type_ = be_cls.be_instance_member_simple(vm, obj, attr, vm.stack[dst_idx])
    if basetype(type_) != BE_FUNCTION:
        vm_error(vm, "attribute_error",
                 "the '%s' object has no method '%s'",
                 be_str2cstr(be_instance_name(obj)), be_str2cstr(attr))


# static int obj_attribute(bvm *vm, bvalue *o, bstring *attr, bvalue *dst)
# {
#     binstance *obj = var_toobj(o);
#     int type = be_instance_member(vm, obj, attr, dst);
#     if (type == BE_NONE) {
#         vm_error(vm, "attribute_error",
#             "the '%s' object has no attribute '%s'",
#             str(be_instance_name(obj)), str(attr));
#     }
#     return type;
# }
def obj_attribute(vm, o_idx, attr, dst):
    """Look up an attribute on an instance. Raises if not found. Returns type."""
    be_cls = _lazy_be_class()
    obj = var_toobj(vm.stack[o_idx])
    type_ = be_cls.be_instance_member(vm, obj, attr, dst)
    if type_ == BE_NONE:
        vm_error(vm, "attribute_error",
                 "the '%s' object has no attribute '%s'",
                 be_str2cstr(be_instance_name(obj)), be_str2cstr(attr))
    return type_


# static int class_attribute(bvm *vm, bvalue *o, bvalue *c, bvalue *dst)
# {
#     bstring *attr = var_tostr(c);
#     bclass *obj = var_toobj(o);
#     int type = be_class_member(vm, obj, attr, dst);
#     if (type == BE_NONE || type == BE_INDEX) {
#         vm_error(vm, "attribute_error",
#             "the '%s' class has no static attribute '%s'",
#             str(obj->name), str(attr));
#     }
#     return type;
# }
def class_attribute(vm, o_idx, c_idx, dst):
    """Look up a static attribute on a class. Raises if not found."""
    be_cls = _lazy_be_class()
    attr = var_tostr(vm.stack[c_idx])
    obj = var_toobj(vm.stack[o_idx])
    type_ = be_cls.be_class_member(vm, obj, attr, dst)
    if type_ == BE_NONE or type_ == BE_INDEX:
        vm_error(vm, "attribute_error",
                 "the '%s' class has no static attribute '%s'",
                 be_str2cstr(obj.name), be_str2cstr(attr))
    return type_


# static int module_attribute(bvm *vm, bvalue *o, bvalue *c, bvalue *dst)
# {
#     bstring *attr = var_tostr(c);
#     bmodule *module = var_toobj(o);
#     int type = be_module_attr(vm, module, attr, dst);
#     if (type == BE_NONE) {
#         vm_error(vm, "attribute_error",
#             "module '%s' has no member '%s'",
#             be_module_name(module), str(attr));
#     }
#     return type;
# }
def module_attribute(vm, o_idx, c_idx, dst):
    """Look up an attribute on a module. Raises if not found."""
    attr = var_tostr(vm.stack[c_idx])
    module = var_toobj(vm.stack[o_idx])
    type_ = be_module_attr(vm, module, attr, dst)
    if type_ == BE_NONE:
        vm_error(vm, "attribute_error",
                 "module '%s' has no member '%s'",
                 be_module_name(module), be_str2cstr(attr))
    return type_


# ============================================================================
# Operator overloading helpers
# ============================================================================

# static bbool object_eqop(bvm *vm,
#     const char *op, bbool iseq, bvalue *a, bvalue *b)
# {
#     binstance *o = var_toobj(a);
#     bvalue self = *a, other = *b;
#     bbool isself = var_isinstance(b) && o == var_toobj(b);
#     int type = be_instance_member(vm, o, be_newstr(vm, op), vm->top);
#     if (basetype(type) == BE_FUNCTION) {
#         bvalue *top = vm->top;
#         top[1] = self;
#         top[2] = other;
#         be_incrtop(vm);
#         be_dofunc(vm, top, 2);
#         be_stackpop(vm, 1);
#         check_bool(vm, o, op);
#         return var_tobool(vm->top);
#     }
#     return iseq == isself;
# }
def object_eqop(vm, op, iseq, a_val, b_val):
    be_cls = _lazy_be_class()
    be_exec = _lazy_be_exec()
    smod = _lazy_be_string()
    o = var_toobj(a_val)
    isself = var_isinstance(b_val) and o is var_toobj(b_val)
    op_str = smod.be_newstr(vm, op)
    type_ = be_cls.be_instance_member(vm, o, op_str, vm.stack[vm.top_idx])
    if basetype(type_) == BE_FUNCTION:
        top_idx = vm.top_idx
        var_setval(vm.stack[top_idx + 1], a_val)   # self
        var_setval(vm.stack[top_idx + 2], b_val)    # other
        be_exec.be_incrtop(vm)  # prevent collection
        be_dofunc(vm, top_idx, 2)
        be_exec.be_stackpop(vm, 1)
        check_bool(vm, o, op)
        return var_tobool(vm.stack[vm.top_idx])
    return iseq == isself


# static void object_binop(bvm *vm, const char *op, bvalue self, bvalue other)
# {
#     bvalue *top = vm->top;
#     obj_method(vm, &self, be_newstr(vm, op), vm->top);
#     top[1] = self;
#     top[2] = other;
#     be_incrtop(vm);
#     be_dofunc(vm, top, 2);
#     be_stackpop(vm, 1);
# }
def object_binop(vm, op, self_val, other_val):
    be_exec = _lazy_be_exec()
    smod = _lazy_be_string()
    top_idx = vm.top_idx
    # We need self on the stack temporarily for obj_method lookup
    var_setval(vm.stack[top_idx + 1], self_val)  # temp: put self at top+1
    op_str = smod.be_newstr(vm, op)
    obj_method(vm, top_idx + 1, op_str, top_idx)
    var_setval(vm.stack[top_idx + 1], self_val)   # argv[0]
    var_setval(vm.stack[top_idx + 2], other_val)  # argv[1]
    be_exec.be_incrtop(vm)
    be_dofunc(vm, top_idx, 2)
    be_exec.be_stackpop(vm, 1)


# static void ins_unop(bvm *vm, const char *op, bvalue self)
# {
#     bvalue *top = vm->top;
#     obj_method(vm, &self, be_newstr(vm, op), vm->top);
#     top[1] = self;
#     be_dofunc(vm, top, 1);
# }
def ins_unop(vm, op, self_val):
    smod = _lazy_be_string()
    top_idx = vm.top_idx
    var_setval(vm.stack[top_idx + 1], self_val)
    op_str = smod.be_newstr(vm, op)
    obj_method(vm, top_idx + 1, op_str, top_idx)
    var_setval(vm.stack[top_idx + 1], self_val)
    be_dofunc(vm, top_idx, 1)


# ins_binop macro — used inside vm_exec
def ins_binop(vm, op, reg_idx, ktab, ins):
    """Inline binary operator overload for instances."""
    rkb_idx = _rkb_idx(reg_idx, ktab, ins)
    rkc_idx = _rkc_idx(reg_idx, ktab, ins)
    object_binop(vm, op, vm.stack[rkb_idx], vm.stack[rkc_idx])
    # After call, reg may have changed
    new_reg_idx = vm.reg_idx
    ra_idx = new_reg_idx + IGET_RA(ins)
    var_setval(vm.stack[ra_idx], vm.stack[vm.top_idx])
    return new_reg_idx


# ============================================================================
# Comparison operators
# ============================================================================

# bbool be_vm_iseq(bvm *vm, bvalue *a, bvalue *b)
# {
#     equal_rule(==, btrue);
# }
def be_vm_iseq(vm, a, b):
    # equal_rule(==, btrue)
    if var_isint(a) and var_isint(b):
        return var_toint(a) == var_toint(b)
    elif var_isnumber(a) and var_isnumber(b):
        return var2real(a) == var2real(b)
    elif var_isinstance(a) and not var_isnil(b):
        return object_eqop(vm, "==", True, a, b)
    elif var_primetype(a) == var_primetype(b):
        if var_isnil(a):
            return True  # nil == nil
        elif var_isbool(a):
            return var_tobool(a) == var_tobool(b)
        elif var_isstr(a):
            smod = _lazy_be_string()
            return smod.be_eqstr(var_tostr(a), var_tostr(b))
        elif var_isclass(a) or var_isfunction(a) or var_iscomptr(a):
            return var_toobj(a) is var_toobj(b)
        else:
            binop_error(vm, "==", a, b)
            return False
    else:
        return False  # different types


# bbool be_vm_isneq(bvm *vm, bvalue *a, bvalue *b)
# {
#     equal_rule(!=, bfalse);
# }
def be_vm_isneq(vm, a, b):
    if var_isint(a) and var_isint(b):
        return var_toint(a) != var_toint(b)
    elif var_isnumber(a) and var_isnumber(b):
        return var2real(a) != var2real(b)
    elif var_isinstance(a) and not var_isnil(b):
        return object_eqop(vm, "!=", False, a, b)
    elif var_primetype(a) == var_primetype(b):
        if var_isnil(a):
            return False  # nil != nil is false
        elif var_isbool(a):
            return var_tobool(a) != var_tobool(b)
        elif var_isstr(a):
            smod = _lazy_be_string()
            return not smod.be_eqstr(var_tostr(a), var_tostr(b))
        elif var_isclass(a) or var_isfunction(a) or var_iscomptr(a):
            return var_toobj(a) is not var_toobj(b)
        else:
            binop_error(vm, "!=", a, b)
            return False
    else:
        return True  # different types


# bbool be_vm_islt(bvm *vm, bvalue *a, bvalue *b)
# {
#     relop_rule(<);
# }
def be_vm_islt(vm, a, b):
    if var_isint(a) and var_isint(b):
        return var_toint(a) < var_toint(b)
    elif var_isnumber(a) and var_isnumber(b):
        return var2real(a) < var2real(b)
    elif var_isstr(a) and var_isstr(b):
        return be_strcmp(var_tostr(a), var_tostr(b)) < 0
    elif var_isinstance(a):
        obj = var_toobj(a)
        object_binop(vm, "<", a, b)
        check_bool(vm, obj, "<")
        return var_tobool(vm.stack[vm.top_idx])
    else:
        binop_error(vm, "<", a, b)
        return False


# bbool be_vm_isle(bvm *vm, bvalue *a, bvalue *b)
def be_vm_isle(vm, a, b):
    if var_isint(a) and var_isint(b):
        return var_toint(a) <= var_toint(b)
    elif var_isnumber(a) and var_isnumber(b):
        return var2real(a) <= var2real(b)
    elif var_isstr(a) and var_isstr(b):
        return be_strcmp(var_tostr(a), var_tostr(b)) <= 0
    elif var_isinstance(a):
        obj = var_toobj(a)
        object_binop(vm, "<=", a, b)
        check_bool(vm, obj, "<=")
        return var_tobool(vm.stack[vm.top_idx])
    else:
        binop_error(vm, "<=", a, b)
        return False


# bbool be_vm_isgt(bvm *vm, bvalue *a, bvalue *b)
def be_vm_isgt(vm, a, b):
    if var_isint(a) and var_isint(b):
        return var_toint(a) > var_toint(b)
    elif var_isnumber(a) and var_isnumber(b):
        return var2real(a) > var2real(b)
    elif var_isstr(a) and var_isstr(b):
        return be_strcmp(var_tostr(a), var_tostr(b)) > 0
    elif var_isinstance(a):
        obj = var_toobj(a)
        object_binop(vm, ">", a, b)
        check_bool(vm, obj, ">")
        return var_tobool(vm.stack[vm.top_idx])
    else:
        binop_error(vm, ">", a, b)
        return False


# bbool be_vm_isge(bvm *vm, bvalue *a, bvalue *b)
def be_vm_isge(vm, a, b):
    if var_isint(a) and var_isint(b):
        return var_toint(a) >= var_toint(b)
    elif var_isnumber(a) and var_isnumber(b):
        return var2real(a) >= var2real(b)
    elif var_isstr(a) and var_isstr(b):
        return be_strcmp(var_tostr(a), var_tostr(b)) >= 0
    elif var_isinstance(a):
        obj = var_toobj(a)
        object_binop(vm, ">=", a, b)
        check_bool(vm, obj, ">=")
        return var_tobool(vm.stack[vm.top_idx])
    else:
        binop_error(vm, ">=", a, b)
        return False


# ============================================================================
# make_range, multiply_str, connect_str
# ============================================================================

# static void make_range(bvm *vm, bvalue lower, bvalue upper)
# {
#     int idx = be_builtin_find(vm, str_literal(vm, "range"));
#     bvalue *top = vm->top;
#     top[0] = *be_global_var(vm, idx);
#     top[1] = lower;
#     top[2] = upper;
#     vm->top += 3;
#     be_dofunc(vm, top, 2);
#     vm->top -= 3;
# }
def make_range(vm, lower_val, upper_val):
    be_var = _lazy_be_var()
    idx = be_var.be_builtin_find(vm, str_literal(vm, "range"))
    top_idx = vm.top_idx
    var_setval(vm.stack[top_idx], be_var.be_global_var(vm, idx))
    var_setval(vm.stack[top_idx + 1], lower_val)
    var_setval(vm.stack[top_idx + 2], upper_val)
    vm.top_idx += 3
    be_dofunc(vm, top_idx, 2)
    vm.top_idx -= 3


# static void multiply_str(bvm *vm, bvalue *a_value, bvalue *count)
# {
#     bint n = 0;
#     bstring *result;
#     bstring *str = var_tostr(a_value);
#     if (var_isint(count)) {
#         n = var_toint(count);
#     } else if (var_isbool(count)) {
#         n = var_tobool(count) ? 1 : 0;
#     } else {
#         binop_error(vm, "*", a_value, count);
#     }
#     result = be_strmul(vm, str, n);
#     var_setstr(vm->top, result);
# }
def multiply_str(vm, a_value, count):
    s = var_tostr(a_value)
    if var_isint(count):
        n = var_toint(count)
    elif var_isbool(count):
        n = 1 if var_tobool(count) else 0
    else:
        binop_error(vm, "*", a_value, count)
        return
    result = be_strmul(vm, s, n)
    var_setstr(vm.stack[vm.top_idx], result)


# static void connect_str(bvm *vm, bstring *a, bvalue *b)
# {
#     bstring *s;
#     if (var_isstr(b)) {
#         s = be_strcat(vm, a, var_tostr(b));
#         var_setstr(vm->top, s);
#     } else {
#         *vm->top++ = *b;
#         be_val2str(vm, -1);
#         b = vm->top - 1;
#         s = be_strcat(vm, a, var_tostr(b));
#         var_setstr(b, s);
#         vm->top -= 1;
#     }
# }
def connect_str(vm, a_str, b_val):
    if var_isstr(b_val):
        s = be_strcat(vm, a_str, var_tostr(b_val))
        var_setstr(vm.stack[vm.top_idx], s)
    else:
        var_setval(vm.stack[vm.top_idx], b_val)
        vm.top_idx += 1
        be_val2str(vm, -1)
        b_idx = vm.top_idx - 1
        s = be_strcat(vm, a_str, var_tostr(vm.stack[b_idx]))
        var_setstr(vm.stack[b_idx], s)
        vm.top_idx -= 1


# ============================================================================
# Register/constant index helpers for the dispatch loop
# ============================================================================

def _rkb_idx(reg_idx, ktab, ins):
    """Compute stack index for RKB operand."""
    raw = IGET_RKB(ins)
    if isKB(ins):
        return ktab + KR2idx(raw)
    else:
        return reg_idx + KR2idx(raw)

def _rkc_idx(reg_idx, ktab, ins):
    """Compute stack index for RKC operand."""
    raw = IGET_RKC(ins)
    if isKC(ins):
        return ktab + KR2idx(raw)
    else:
        return reg_idx + KR2idx(raw)


# ============================================================================
# bvm class — the Berry virtual machine
# ============================================================================
# In the C implementation, bvm is a struct allocated on the heap.
# In Python, we use a class with the same field names. The value stack
# is a Python list of bvalue objects, with integer indices replacing
# C pointers (reg, top, stacktop).

class bvm:
    """Berry virtual machine instance.

    Mirrors: struct bvm (be_vm.h)
    """
    # struct bvm {
    #     bglobaldesc gbldesc;
    #     bvalue *stack;
    #     bvalue *stacktop;
    #     bupval *upvalist;
    #     bstack callstack;
    #     bstack exceptstack;
    #     bcallframe *cf;
    #     bvalue *reg;
    #     bvalue *top;
    #     binstruction *ip;
    #     struct blongjmp *errjmp;
    #     bstack refstack;
    #     struct bmoduledesc module;
    #     struct bstringtable strtab;
    #     bstack tracestack;
    #     bmap *ntvclass;
    #     struct bgc gc;
    #     bctypefunc ctypefunc;
    #     bbyte compopt;
    #     size_t bytesmaxsize;
    #     bobshook obshook;
    #     ... (perf counters, debug hook)
    # };
    __slots__ = (
        'gbldesc', 'stack', 'stacktop_idx',
        'upvalist', 'callstack', 'exceptstack',
        'cf', 'reg_idx', 'top_idx',
        'ip', 'ip_idx', 'errjmp',
        'refstack', 'module', 'strtab',
        'tracestack', 'ntvclass',
        'gc', 'ctypefunc', 'compopt',
        'bytesmaxsize', 'obshook',
        'preprocessor',
        # Perf counters
        'counter_ins', 'counter_enter', 'counter_call',
        'counter_get', 'counter_set', 'counter_get_global',
        'counter_try', 'counter_exc',
        'counter_gc_kept', 'counter_gc_freed',
        'counter_mem_alloc', 'counter_mem_free', 'counter_mem_realloc',
        # GC mark counters
        'gc_mark_string', 'gc_mark_class', 'gc_mark_proto',
        'gc_mark_instance', 'gc_mark_map', 'gc_mark_list',
        'gc_mark_closure', 'gc_mark_ntvclos', 'gc_mark_module',
        'gc_mark_comobj',
    )

    def __init__(self):
        self.gbldesc = bglobaldesc()
        self.stack = []           # list of bvalue
        self.stacktop_idx = 0     # index past end of allocated stack
        self.upvalist = None      # open upvalue list head
        self.callstack = None     # bvector used as stack of bcallframe
        self.exceptstack = None   # bvector used as stack of bexecptframe
        self.cf = None            # current call frame (bcallframe)
        self.reg_idx = 0          # base register index into stack
        self.top_idx = 0          # top register index into stack
        self.ip = None            # instruction list (Python list of ints)
        self.ip_idx = 0           # current instruction index
        self.errjmp = None        # error jump point (Python: not used directly)
        self.refstack = None      # bvector used as reference stack
        self.module = bmoduledesc()
        self.strtab = bstringtable()
        self.tracestack = None    # bvector used as trace stack
        self.ntvclass = None      # native class table (bmap)
        self.gc = bgc()
        self.ctypefunc = None
        self.compopt = 0
        self.bytesmaxsize = BE_BYTES_MAX_SIZE
        self.obshook = None
        self.preprocessor = None
        # Perf counters
        self.counter_ins = 0
        self.counter_enter = 0
        self.counter_call = 0
        self.counter_get = 0
        self.counter_set = 0
        self.counter_get_global = 0
        self.counter_try = 0
        self.counter_exc = 0
        self.counter_gc_kept = 0
        self.counter_gc_freed = 0
        self.counter_mem_alloc = 0
        self.counter_mem_free = 0
        self.counter_mem_realloc = 0
        self.gc_mark_string = 0
        self.gc_mark_class = 0
        self.gc_mark_proto = 0
        self.gc_mark_instance = 0
        self.gc_mark_map = 0
        self.gc_mark_list = 0
        self.gc_mark_closure = 0
        self.gc_mark_ntvclos = 0
        self.gc_mark_module = 0
        self.gc_mark_comobj = 0

    # ---- Property aliases ------------------------------------------------
    # be_exec.py (and other modules) use vm.top / vm.reg / vm.stacktop
    # without the _idx suffix.  These properties keep both names working.
    @property
    def top(self):
        return self.top_idx

    @top.setter
    def top(self, value):
        self.top_idx = value

    @property
    def reg(self):
        return self.reg_idx

    @reg.setter
    def reg(self, value):
        self.reg_idx = value

    @property
    def stacktop(self):
        return self.stacktop_idx

    @stacktop.setter
    def stacktop(self, value):
        self.stacktop_idx = value


# ============================================================================
# VM lifecycle
# ============================================================================

# BERRY_API bvm* be_vm_new(void)
# {
#     bvm *vm = be_os_malloc(sizeof(bvm));
#     memset(vm, 0, sizeof(bvm));
#     be_gc_init(vm);
#     be_string_init(vm);
#     be_stack_init(vm, &vm->callstack, sizeof(bcallframe));
#     be_stack_init(vm, &vm->refstack, sizeof(binstance*));
#     be_stack_init(vm, &vm->exceptstack, sizeof(struct bexecptframe));
#     be_stack_init(vm, &vm->tracestack, sizeof(bcallsnapshot));
#     vm->stack = be_malloc(vm, sizeof(bvalue) * BE_STACK_START);
#     vm->stacktop = vm->stack + BE_STACK_START;
#     vm->reg = vm->stack;
#     vm->top = vm->reg;
#     be_globalvar_init(vm);
#     be_gc_setpause(vm, 1);
#     be_loadlibs(vm);
#     vm->compopt = 0;
#     vm->bytesmaxsize = BE_BYTES_MAX_SIZE;
#     vm->obshook = NULL;
#     vm->ctypefunc = NULL;
#     return vm;
# }
def be_vm_new():
    """Create and initialize a new Berry VM instance."""
    be_gc_mod = _lazy_be_gc()
    be_string_mod = _lazy_be_string()
    be_vector_mod = _lazy_be_vector()
    be_var_mod = _lazy_be_var()

    from berry_port.be_object import bvector as bvector_cls

    vm = bvm()

    # Initialize GC
    be_gc_mod.be_gc_init(vm)

    # Initialize string table
    be_string_mod.be_string_init(vm)

    # Initialize stacks
    vm.callstack = bvector_cls()
    be_vector_mod.be_stack_init(vm, vm.callstack, 1)  # size param is element size, not used in Python
    vm.refstack = bvector_cls()
    be_vector_mod.be_stack_init(vm, vm.refstack, 1)
    vm.exceptstack = bvector_cls()
    be_vector_mod.be_stack_init(vm, vm.exceptstack, 1)
    vm.tracestack = bvector_cls()
    be_vector_mod.be_stack_init(vm, vm.tracestack, 1)

    # Initialize value stack
    vm.stack = [bvalue() for _ in range(BE_STACK_START)]
    vm.stacktop_idx = BE_STACK_START
    vm.reg_idx = 0
    vm.top_idx = 0

    # Initialize global variables
    be_var_mod.be_globalvar_init(vm)

    # Set GC pause
    be_gc_mod.be_gc_setpause(vm, 1)

    # Load built-in libraries
    be_loadlibs(vm)

    vm.compopt = 0
    vm.bytesmaxsize = BE_BYTES_MAX_SIZE
    vm.obshook = None
    vm.ctypefunc = None

    return vm


# BERRY_API void be_vm_delete(bvm *vm)
# {
#     be_gc_deleteall(vm);
#     be_string_deleteall(vm);
#     be_stack_delete(vm, &vm->callstack);
#     be_stack_delete(vm, &vm->refstack);
#     be_stack_delete(vm, &vm->exceptstack);
#     be_stack_delete(vm, &vm->tracestack);
#     be_free(vm, vm->stack, (vm->stacktop - vm->stack) * sizeof(bvalue));
#     be_globalvar_deinit(vm);
#     be_gc_free_memory_pools(vm);
#     be_os_free(vm);
# }
def be_vm_delete(vm):
    """Destroy a Berry VM instance and free all resources."""
    be_gc_mod = _lazy_be_gc()
    be_string_mod = _lazy_be_string()
    be_vector_mod = _lazy_be_vector()
    be_var_mod = _lazy_be_var()
    be_mem_mod = _lazy_be_mem()

    be_gc_mod.be_gc_deleteall(vm)
    be_string_mod.be_string_deleteall(vm)
    be_vector_mod.be_stack_delete(vm, vm.callstack)
    be_vector_mod.be_stack_delete(vm, vm.refstack)
    be_vector_mod.be_stack_delete(vm, vm.exceptstack)
    be_vector_mod.be_stack_delete(vm, vm.tracestack)
    vm.stack = []
    be_var_mod.be_globalvar_deinit(vm)
    be_mem_mod.be_gc_free_memory_pools(vm)


# ============================================================================
# vm_exec — the main dispatch loop
# ============================================================================

# static void vm_exec(bvm *vm)
# {
#     bclosure *clos;
#     bvalue *ktab, *reg;
#     binstruction ins;
#     vm->cf->status |= BASE_FRAME;
# newframe:
#     clos = var_toobj(vm->cf->func);
#     ktab = clos->proto->ktab;
#     reg = vm->reg;
#     vm_exec_loop() { ... }
# }
def vm_exec(vm):
    """Execute bytecode starting from the current call frame.

    This is the main interpreter dispatch loop. It processes all 48
    Berry opcodes. The C implementation uses goto for dispatch; the
    Python port uses a while/if-elif chain.
    """
    be_exec = _lazy_be_exec()
    be_vector = _lazy_be_vector()
    be_cls = _lazy_be_class()
    be_func = _lazy_be_func()
    be_var = _lazy_be_var()
    be_list_mod = _lazy_be_list()
    smod = _lazy_be_string()

    vm.cf.status |= BASE_FRAME

    # newframe label — in Python we use a nested function and loop
    while True:  # newframe loop
        clos = var_toobj(vm.stack[vm.cf.func])
        ktab = clos.proto.ktab       # list of bvalue constants
        ktab_base = 0                # ktab is a list, index from 0
        reg_idx = vm.reg_idx

        if BE_USE_PERF_COUNTERS:
            vm.counter_enter += 1

        while True:  # instruction dispatch loop
          try:
            if BE_USE_PERF_COUNTERS:
                vm.counter_ins += 1

            ins = vm.ip[vm.ip_idx]
            vm.ip_idx += 1
            op = IGET_OP(ins)

            # Helper lambdas for register access (inlined for performance)
            ra_idx = reg_idx + IGET_RA(ins)

            if op == OP_LDNIL:
                # opcase(LDNIL): var_setnil(RA());
                var_setnil(vm.stack[ra_idx])

            elif op == OP_LDBOOL:
                # opcase(LDBOOL): var_setbool(RA(), IGET_RKB(ins));
                #   if (IGET_RKC(ins)) vm->ip += 1;
                var_setbool(vm.stack[ra_idx], IGET_RKB(ins))
                if IGET_RKC(ins):
                    vm.ip_idx += 1

            elif op == OP_LDINT:
                # opcase(LDINT): var_setint(RA(), IGET_sBx(ins));
                var_setint(vm.stack[ra_idx], IGET_sBx(ins))

            elif op == OP_LDCONST:
                # opcase(LDCONST): *dst = ktab[IGET_Bx(ins)];
                var_setval(vm.stack[ra_idx], ktab[IGET_Bx(ins)])

            elif op == OP_GETGBL:
                # opcase(GETGBL): *v = *be_global_var(vm, IGET_Bx(ins));
                idx = IGET_Bx(ins)
                gv = be_var.be_global_var(vm, idx)
                var_setval(vm.stack[ra_idx], gv)

            elif op == OP_GETNGBL:
                # Get global by name
                if BE_USE_PERF_COUNTERS:
                    vm.counter_get_global += 1
                b_idx = _rkb_idx(reg_idx, ktab_base, ins)
                b_val = vm.stack[b_idx] if b_idx < len(vm.stack) else ktab[b_idx - ktab_base] if isinstance(b_idx, int) else None
                # Resolve b_val properly
                b_val = _resolve_rkb(vm, reg_idx, ktab, ins)
                if var_isstr(b_val):
                    name = var_tostr(b_val)
                    gidx = be_var.be_global_find(vm, name)
                    if gidx >= 0:
                        var_setval(vm.stack[ra_idx], be_var.be_global_var(vm, gidx))
                    else:
                        vm_error(vm, "attribute_error", "'%s' undeclared", be_str2cstr(name))
                else:
                    vm_error(vm, "internal_error", "global name must be a string")

            elif op == OP_SETNGBL:
                # Set global by name
                b_val = _resolve_rkb(vm, reg_idx, ktab, ins)
                if var_isstr(b_val):
                    name = var_tostr(b_val)
                    gidx = be_var.be_global_new(vm, name)
                    var_setval(be_var.be_global_var(vm, gidx), vm.stack[ra_idx])
                else:
                    vm_error(vm, "internal_error", "global name must be a string")

            elif op == OP_SETGBL:
                # opcase(SETGBL): *be_global_var(vm, IGET_Bx(ins)) = *RA();
                idx = IGET_Bx(ins)
                var_setval(be_var.be_global_var(vm, idx), vm.stack[ra_idx])

            elif op == OP_GETUPV:
                # opcase(GETUPV): *v = *clos->upvals[idx]->value;
                idx = IGET_Bx(ins)
                uv = clos.upvals[idx]
                if isinstance(uv.value, int):
                    # Open upvalue — value is a stack index
                    var_setval(vm.stack[ra_idx], vm.stack[uv.value])
                else:
                    # Closed upvalue — value stored in u_value
                    var_setval(vm.stack[ra_idx], uv.u_value)

            elif op == OP_SETUPV:
                # opcase(SETUPV): *clos->upvals[idx]->value = *RA();
                idx = IGET_Bx(ins)
                uv = clos.upvals[idx]
                if isinstance(uv.value, int):
                    # Open upvalue — write to stack slot
                    var_setval(vm.stack[uv.value], vm.stack[ra_idx])
                else:
                    # Closed upvalue — write to u_value
                    var_setval(uv.u_value, vm.stack[ra_idx])

            elif op == OP_MOVE:
                # opcase(MOVE): *dst = *RKB();
                b_val = _resolve_rkb(vm, reg_idx, ktab, ins)
                var_setval(vm.stack[ra_idx], b_val)

            elif op == OP_ADD:
                _exec_add(vm, reg_idx, ktab, ins, ra_idx)
                reg_idx = vm.reg_idx

            elif op == OP_SUB:
                _exec_sub(vm, reg_idx, ktab, ins, ra_idx)
                reg_idx = vm.reg_idx

            elif op == OP_MUL:
                _exec_mul(vm, reg_idx, ktab, ins, ra_idx)
                reg_idx = vm.reg_idx

            elif op == OP_DIV:
                _exec_div(vm, reg_idx, ktab, ins, ra_idx)
                reg_idx = vm.reg_idx

            elif op == OP_MOD:
                _exec_mod(vm, reg_idx, ktab, ins, ra_idx)
                reg_idx = vm.reg_idx

            elif op == OP_LT:
                b_val = _resolve_rkb(vm, reg_idx, ktab, ins)
                c_val = _resolve_rkc(vm, reg_idx, ktab, ins)
                res = be_vm_islt(vm, b_val, c_val)
                reg_idx = vm.reg_idx
                ra_idx = reg_idx + IGET_RA(ins)
                var_setbool(vm.stack[ra_idx], res)

            elif op == OP_LE:
                b_val = _resolve_rkb(vm, reg_idx, ktab, ins)
                c_val = _resolve_rkc(vm, reg_idx, ktab, ins)
                res = be_vm_isle(vm, b_val, c_val)
                reg_idx = vm.reg_idx
                ra_idx = reg_idx + IGET_RA(ins)
                var_setbool(vm.stack[ra_idx], res)

            elif op == OP_EQ:
                b_val = _resolve_rkb(vm, reg_idx, ktab, ins)
                c_val = _resolve_rkc(vm, reg_idx, ktab, ins)
                res = be_vm_iseq(vm, b_val, c_val)
                reg_idx = vm.reg_idx
                ra_idx = reg_idx + IGET_RA(ins)
                var_setbool(vm.stack[ra_idx], res)

            elif op == OP_NE:
                b_val = _resolve_rkb(vm, reg_idx, ktab, ins)
                c_val = _resolve_rkc(vm, reg_idx, ktab, ins)
                res = be_vm_isneq(vm, b_val, c_val)
                reg_idx = vm.reg_idx
                ra_idx = reg_idx + IGET_RA(ins)
                var_setbool(vm.stack[ra_idx], res)

            elif op == OP_GT:
                b_val = _resolve_rkb(vm, reg_idx, ktab, ins)
                c_val = _resolve_rkc(vm, reg_idx, ktab, ins)
                res = be_vm_isgt(vm, b_val, c_val)
                reg_idx = vm.reg_idx
                ra_idx = reg_idx + IGET_RA(ins)
                var_setbool(vm.stack[ra_idx], res)

            elif op == OP_GE:
                b_val = _resolve_rkb(vm, reg_idx, ktab, ins)
                c_val = _resolve_rkc(vm, reg_idx, ktab, ins)
                res = be_vm_isge(vm, b_val, c_val)
                reg_idx = vm.reg_idx
                ra_idx = reg_idx + IGET_RA(ins)
                var_setbool(vm.stack[ra_idx], res)

            elif op == OP_CONNECT:
                _exec_connect(vm, reg_idx, ktab, ins, ra_idx)
                reg_idx = vm.reg_idx

            elif op == OP_AND:
                _exec_bitwise(vm, reg_idx, ktab, ins, ra_idx, '&')
                reg_idx = vm.reg_idx

            elif op == OP_OR:
                _exec_bitwise(vm, reg_idx, ktab, ins, ra_idx, '|')
                reg_idx = vm.reg_idx

            elif op == OP_XOR:
                _exec_bitwise(vm, reg_idx, ktab, ins, ra_idx, '^')
                reg_idx = vm.reg_idx

            elif op == OP_SHL:
                _exec_bitwise(vm, reg_idx, ktab, ins, ra_idx, '<<')
                reg_idx = vm.reg_idx

            elif op == OP_SHR:
                _exec_bitwise(vm, reg_idx, ktab, ins, ra_idx, '>>')
                reg_idx = vm.reg_idx

            elif op == OP_NEG:
                _exec_neg(vm, reg_idx, ktab, ins, ra_idx)
                reg_idx = vm.reg_idx

            elif op == OP_FLIP:
                _exec_flip(vm, reg_idx, ktab, ins, ra_idx)
                reg_idx = vm.reg_idx

            elif op == OP_JMP:
                vm.ip_idx += IGET_sBx(ins)

            elif op == OP_JMPT:
                if be_value2bool(vm, vm.stack[ra_idx]):
                    vm.ip_idx += IGET_sBx(ins)

            elif op == OP_JMPF:
                if not be_value2bool(vm, vm.stack[ra_idx]):
                    vm.ip_idx += IGET_sBx(ins)

            elif op == OP_CLOSURE:
                _exec_closure(vm, clos, reg_idx, ins, ra_idx)
                reg_idx = vm.reg_idx

            elif op == OP_CLASS:
                c = var_toobj(ktab[IGET_Bx(ins)])
                be_cls.be_class_upvalue_init(vm, c)

            elif op == OP_GETMBR:
                _exec_getmbr(vm, reg_idx, ktab, ins, ra_idx)
                reg_idx = vm.reg_idx

            elif op == OP_GETMET:
                _exec_getmet(vm, reg_idx, ktab, ins, ra_idx)
                reg_idx = vm.reg_idx

            elif op == OP_SETMBR:
                _exec_setmbr(vm, reg_idx, ktab, ins, ra_idx)
                reg_idx = vm.reg_idx

            elif op == OP_GETIDX:
                _exec_getidx(vm, reg_idx, ktab, ins, ra_idx)
                reg_idx = vm.reg_idx

            elif op == OP_SETIDX:
                _exec_setidx(vm, reg_idx, ktab, ins, ra_idx)
                reg_idx = vm.reg_idx

            elif op == OP_SETSUPER:
                _exec_setsuper(vm, reg_idx, ktab, ins, ra_idx)

            elif op == OP_CLOSE:
                be_func.be_upvals_close(vm, ra_idx)

            elif op == OP_IMPORT:
                _exec_import(vm, reg_idx, ktab, ins, ra_idx)
                reg_idx = vm.reg_idx

            elif op == OP_CATCH:
                _exec_catch(vm, reg_idx, ins, ra_idx)

            elif op == OP_RAISE:
                _exec_raise(vm, reg_idx, ktab, ins)

            elif op == OP_EXBLK:
                result = _exec_exblk(vm, ins)
                if result == 'newframe':
                    reg_idx = vm.reg_idx
                    break  # break inner loop to restart newframe
                reg_idx = vm.reg_idx

            elif op == OP_CALL:
                result = _exec_call(vm, reg_idx, ktab, ins, ra_idx, clos)
                if result == 'newframe':
                    reg_idx = vm.reg_idx
                    break  # break inner loop to restart newframe
                reg_idx = vm.reg_idx

            elif op == OP_RET:
                result = _exec_ret(vm, reg_idx, ktab, ins)
                if result == 'return':
                    return
                # result == 'newframe': continue outer loop
                reg_idx = vm.reg_idx
                break  # break inner loop to restart newframe

            else:
                vm_error(vm, "internal_error", "unknown opcode %d", op)

          except be_exec.BerryException as _exblk_exc:
            # Exception caught — equivalent to setjmp returning non-zero
            # in the C OP_EXBLK handler.
            if be_vector.be_stack_count(vm.exceptstack) > 0:
                # Save exception values from top of stack
                top_idx = vm.top_idx
                e1 = bvalue()
                e2 = bvalue()
                if top_idx < len(vm.stack):
                    e1.type = vm.stack[top_idx].type
                    e1.v = vm.stack[top_idx].v
                if top_idx + 1 < len(vm.stack):
                    e2.type = vm.stack[top_idx + 1].type
                    e2.v = vm.stack[top_idx + 1].v
                # Resume: restore VM state to the exception frame
                be_exec.be_except_block_resume(vm, _exblk_exc.errorcode)
                # Restore exception values at new top
                top_idx = vm.top_idx
                vm.stack[top_idx] = e1
                vm.stack[top_idx + 1] = e2
                # Restart newframe loop (goto newframe)
                reg_idx = vm.reg_idx
                break
            else:
                # No exception frame — re-raise
                raise

        # end of inner dispatch loop — if we broke out, continue outer newframe loop


# ============================================================================
# RKB / RKC value resolution helpers
# ============================================================================

def _resolve_rkb(vm, reg_idx, ktab, ins):
    """Resolve RKB operand to a bvalue reference."""
    raw = IGET_RKB(ins)
    if isKB(ins):
        return ktab[KR2idx(raw)]
    else:
        return vm.stack[reg_idx + KR2idx(raw)]

def _resolve_rkc(vm, reg_idx, ktab, ins):
    """Resolve RKC operand to a bvalue reference."""
    raw = IGET_RKC(ins)
    if isKC(ins):
        return ktab[KR2idx(raw)]
    else:
        return vm.stack[reg_idx + KR2idx(raw)]


# ============================================================================
# Arithmetic opcode helpers
# ============================================================================

def _exec_add(vm, reg_idx, ktab, ins, ra_idx):
    a = _resolve_rkb(vm, reg_idx, ktab, ins)
    b = _resolve_rkc(vm, reg_idx, ktab, ins)
    dst = vm.stack[ra_idx]
    if var_isint(a) and var_isint(b):
        var_setint(dst, var_toint(a) + var_toint(b))
    elif var_isnumber(a) and var_isnumber(b):
        var_setreal(dst, var2real(a) + var2real(b))
    elif var_isstr(a) and var_isstr(b):
        s = be_strcat(vm, var_tostr(a), var_tostr(b))
        reg_idx = vm.reg_idx
        ra_idx = reg_idx + IGET_RA(ins)
        var_setstr(vm.stack[ra_idx], s)
    elif var_isinstance(a):
        ins_binop(vm, "+", reg_idx, 0, ins)
    elif var_iscomptr(a) and var_isint(b):
        # comptr + int: pointer arithmetic (not meaningful in Python, but preserved)
        var_setcomptr(dst, var_toobj(a))  # simplified
    else:
        binop_error(vm, "+", a, b)


def _exec_sub(vm, reg_idx, ktab, ins, ra_idx):
    a = _resolve_rkb(vm, reg_idx, ktab, ins)
    b = _resolve_rkc(vm, reg_idx, ktab, ins)
    dst = vm.stack[ra_idx]
    if var_isint(a) and var_isint(b):
        var_setint(dst, var_toint(a) - var_toint(b))
    elif var_isnumber(a) and var_isnumber(b):
        var_setreal(dst, var2real(a) - var2real(b))
    elif var_isinstance(a):
        ins_binop(vm, "-", reg_idx, 0, ins)
    else:
        binop_error(vm, "-", a, b)


def _exec_mul(vm, reg_idx, ktab, ins, ra_idx):
    a = _resolve_rkb(vm, reg_idx, ktab, ins)
    b = _resolve_rkc(vm, reg_idx, ktab, ins)
    dst = vm.stack[ra_idx]
    if var_isint(a) and var_isint(b):
        var_setint(dst, var_toint(a) * var_toint(b))
    elif var_isnumber(a) and var_isnumber(b):
        var_setreal(dst, var2real(a) * var2real(b))
    elif var_isstr(a) and (var_isint(b) or var_isbool(b)):
        multiply_str(vm, a, b)
        reg_idx = vm.reg_idx
        ra_idx = reg_idx + IGET_RA(ins)
        var_setval(vm.stack[ra_idx], vm.stack[vm.top_idx])
    elif var_isinstance(a):
        ins_binop(vm, "*", reg_idx, 0, ins)
    else:
        binop_error(vm, "*", a, b)


def _exec_div(vm, reg_idx, ktab, ins, ra_idx):
    a = _resolve_rkb(vm, reg_idx, ktab, ins)
    b = _resolve_rkc(vm, reg_idx, ktab, ins)
    dst = vm.stack[ra_idx]
    if var_isint(a) and var_isint(b):
        x, y = var_toint(a), var_toint(b)
        if y == 0:
            vm_error(vm, "divzero_error", "division by zero")
        else:
            # C integer division truncates toward zero
            if (x < 0) != (y < 0) and x % y != 0:
                var_setint(dst, x // y + 1)
            else:
                var_setint(dst, x // y)
    elif var_isnumber(a) and var_isnumber(b):
        x, y = var2real(a), var2real(b)
        if y == 0.0:
            vm_error(vm, "divzero_error", "division by zero")
        else:
            var_setreal(dst, x / y)
    elif var_isinstance(a):
        ins_binop(vm, "/", reg_idx, 0, ins)
    else:
        binop_error(vm, "/", a, b)


def _exec_mod(vm, reg_idx, ktab, ins, ra_idx):
    a = _resolve_rkb(vm, reg_idx, ktab, ins)
    b = _resolve_rkc(vm, reg_idx, ktab, ins)
    dst = vm.stack[ra_idx]
    if var_isint(a) and var_isint(b):
        x, y = var_toint(a), var_toint(b)
        if y == 0:
            vm_error(vm, "divzero_error", "division by zero")
        else:
            # C modulo: result has same sign as dividend
            r = x % y
            if r != 0 and (r > 0) != (x > 0):
                r -= y
            var_setint(dst, r)
    elif var_isnumber(a) and var_isnumber(b):
        x, y = var2real(a), var2real(b)
        if y == 0.0:
            vm_error(vm, "divzero_error", "division by zero")
        else:
            var_setreal(dst, math.fmod(x, y))
    elif var_isinstance(a):
        ins_binop(vm, "%", reg_idx, 0, ins)
    else:
        binop_error(vm, "%", a, b)


# ============================================================================
# Bitwise, connect, neg, flip opcode helpers
# ============================================================================

def _exec_bitwise(vm, reg_idx, ktab, ins, ra_idx, op_str):
    # bitwise_block(op)
    a = _resolve_rkb(vm, reg_idx, ktab, ins)
    b = _resolve_rkc(vm, reg_idx, ktab, ins)
    dst = vm.stack[ra_idx]
    if var_isint(a) and var_isint(b):
        ai, bi = var_toint(a), var_toint(b)
        if op_str == '&':
            var_setint(dst, ai & bi)
        elif op_str == '|':
            var_setint(dst, ai | bi)
        elif op_str == '^':
            var_setint(dst, ai ^ bi)
        elif op_str == '<<':
            var_setint(dst, ai << bi)
        elif op_str == '>>':
            var_setint(dst, ai >> bi)
    elif var_isinstance(a):
        ins_binop(vm, op_str, reg_idx, 0, ins)
    else:
        binop_error(vm, op_str, a, b)


def _exec_connect(vm, reg_idx, ktab, ins, ra_idx):
    a = _resolve_rkb(vm, reg_idx, ktab, ins)
    b = _resolve_rkc(vm, reg_idx, ktab, ins)
    if var_isint(a) and var_isint(b):
        make_range(vm, a, b)
    elif var_isstr(a):
        connect_str(vm, var_tostr(a), b)
    elif var_isinstance(a):
        object_binop(vm, "..", a, b)
    else:
        binop_error(vm, "..", a, b)
    reg_idx = vm.reg_idx
    ra_idx = reg_idx + IGET_RA(ins)
    var_setval(vm.stack[ra_idx], vm.stack[vm.top_idx])


def _exec_neg(vm, reg_idx, ktab, ins, ra_idx):
    a = _resolve_rkb(vm, reg_idx, ktab, ins)
    dst = vm.stack[ra_idx]
    if var_isint(a):
        var_setint(dst, -var_toint(a))
    elif var_isreal(a):
        var_setreal(dst, -var_toreal(a))
    elif var_isinstance(a):
        ins_unop(vm, "-*", a)
        reg_idx = vm.reg_idx
        ra_idx = reg_idx + IGET_RA(ins)
        var_setval(vm.stack[ra_idx], vm.stack[vm.top_idx])
    else:
        unop_error(vm, "-", a)


def _exec_flip(vm, reg_idx, ktab, ins, ra_idx):
    a = _resolve_rkb(vm, reg_idx, ktab, ins)
    dst = vm.stack[ra_idx]
    if var_isint(a):
        var_setint(dst, ~var_toint(a))
    elif var_isinstance(a):
        ins_unop(vm, "~", a)
        reg_idx = vm.reg_idx
        ra_idx = reg_idx + IGET_RA(ins)
        var_setval(vm.stack[ra_idx], vm.stack[vm.top_idx])
    else:
        unop_error(vm, "~", a)


# ============================================================================
# CLOSURE opcode helper
# ============================================================================

def _exec_closure(vm, clos, reg_idx, ins, ra_idx):
    be_func = _lazy_be_func()
    p = clos.proto.ptab[IGET_Bx(ins)]
    cl = be_func.be_newclosure(vm, p.nupvals)
    cl.proto = p
    reg_idx = vm.reg_idx
    ra_idx = reg_idx + IGET_RA(ins)
    var_setclosure(vm.stack[ra_idx], cl)
    be_func.be_initupvals(vm, cl)


# ============================================================================
# GETMBR opcode helper
# ============================================================================

def _exec_getmbr(vm, reg_idx, ktab, ins, ra_idx):
    if BE_USE_PERF_COUNTERS:
        vm.counter_get += 1
    result = bvalue()
    b_val = _resolve_rkb(vm, reg_idx, ktab, ins)
    c_val = _resolve_rkc(vm, reg_idx, ktab, ins)
    if var_isinstance(b_val) and var_isstr(c_val):
        # Store b_idx for obj_attribute — need to put b on stack temporarily
        _push_temp = vm.top_idx
        var_setval(vm.stack[_push_temp], b_val)
        obj_attribute(vm, _push_temp, var_tostr(c_val), result)
        reg_idx = vm.reg_idx
    elif var_isclass(b_val) and var_isstr(c_val):
        be_cls = _lazy_be_class()
        attr = var_tostr(c_val)
        obj = var_toobj(b_val)
        type_ = be_cls.be_class_member(vm, obj, attr, result)
        if type_ == BE_NONE or type_ == BE_INDEX:
            vm_error(vm, "attribute_error",
                     "the '%s' class has no static attribute '%s'",
                     be_str2cstr(obj.name), be_str2cstr(attr))
        reg_idx = vm.reg_idx
    elif var_ismodule(b_val) and var_isstr(c_val):
        attr = var_tostr(c_val)
        module = var_toobj(b_val)
        type_ = be_module_attr(vm, module, attr, result)
        if type_ == BE_NONE:
            vm_error(vm, "attribute_error",
                     "module '%s' has no member '%s'",
                     be_module_name(module), be_str2cstr(attr))
        reg_idx = vm.reg_idx
    else:
        attribute_error(vm, "attribute", b_val, c_val)
    ra_idx = reg_idx + IGET_RA(ins)
    var_setval(vm.stack[ra_idx], result)


# ============================================================================
# GETMET opcode helper
# ============================================================================

def _exec_getmet(vm, reg_idx, ktab, ins, ra_idx):
    if BE_USE_PERF_COUNTERS:
        vm.counter_get += 1
    be_cls = _lazy_be_class()
    result = bvalue()
    b_val = _resolve_rkb(vm, reg_idx, ktab, ins)
    c_val = _resolve_rkc(vm, reg_idx, ktab, ins)

    if var_isinstance(b_val) and var_isstr(c_val):
        obj = var_toobj(b_val)
        attr = var_tostr(c_val)
        # Use obj_attribute logic inline to get type
        type_ = be_cls.be_instance_member(vm, obj, attr, result)
        if type_ == BE_NONE:
            vm_error(vm, "attribute_error",
                     "the '%s' object has no attribute '%s'",
                     be_str2cstr(be_instance_name(obj)), be_str2cstr(attr))
        reg_idx = vm.reg_idx
        ra_idx = reg_idx + IGET_RA(ins)
        a = vm.stack[ra_idx]
        var_setval(a, result)
        if var_basetype(a) == BE_FUNCTION:
            if (type_ & BE_STATIC) or (type_ == BE_INDEX):
                # Static method — don't bother with instance
                var_setval(vm.stack[ra_idx + 1], result)
                var_settype(a, NOT_METHOD)
            else:
                # Real method — get lowest subclass
                while obj.sub:
                    obj = obj.sub
                var_setinstance(vm.stack[ra_idx + 1], obj)
        elif var_isclass(a):
            var_setval(vm.stack[ra_idx + 1], result)
            var_settype(a, NOT_METHOD)
        else:
            vm_error(vm, "attribute_error",
                     "class '%s' has no method '%s'",
                     be_str2cstr(be_instance_name(obj)), be_str2cstr(attr))

    elif var_isclass(b_val) and var_isstr(c_val):
        attr = var_tostr(c_val)
        obj = var_toobj(b_val)
        type_ = be_cls.be_class_member(vm, obj, attr, result)
        if type_ == BE_NONE or type_ == BE_INDEX:
            vm_error(vm, "attribute_error",
                     "the '%s' class has no static attribute '%s'",
                     be_str2cstr(obj.name), be_str2cstr(attr))
        reg_idx = vm.reg_idx
        ra_idx = reg_idx + IGET_RA(ins)
        var_setval(vm.stack[ra_idx + 1], result)
        var_settype(vm.stack[ra_idx], NOT_METHOD)

    elif var_ismodule(b_val) and var_isstr(c_val):
        attr = var_tostr(c_val)
        module = var_toobj(b_val)
        type_ = be_module_attr(vm, module, attr, result)
        if type_ == BE_NONE:
            vm_error(vm, "attribute_error",
                     "module '%s' has no member '%s'",
                     be_module_name(module), be_str2cstr(attr))
        reg_idx = vm.reg_idx
        ra_idx = reg_idx + IGET_RA(ins)
        var_setval(vm.stack[ra_idx + 1], result)
        var_settype(vm.stack[ra_idx], NOT_METHOD)

    else:
        attribute_error(vm, "method", b_val, c_val)


# ============================================================================
# SETMBR opcode helper
# ============================================================================

def _exec_setmbr(vm, reg_idx, ktab, ins, ra_idx):
    if BE_USE_PERF_COUNTERS:
        vm.counter_set += 1
    be_cls = _lazy_be_class()
    a_val = vm.stack[ra_idx]
    b_val = _resolve_rkb(vm, reg_idx, ktab, ins)
    c_val = _resolve_rkc(vm, reg_idx, ktab, ins)

    if var_isinstance(a_val) and var_isstr(b_val):
        obj = var_toobj(a_val)
        attr = var_tostr(b_val)
        result = bvalue()
        var_setval(result, c_val)
        if var_isfunction(result):
            var_markstatic(result)
        if not be_cls.be_instance_setmember(vm, obj, attr, result):
            vm_error(vm, "attribute_error",
                     "class '%s' cannot assign to attribute '%s'",
                     be_str2cstr(be_instance_name(obj)), be_str2cstr(attr))
        return

    if var_isclass(a_val) and var_isstr(b_val):
        obj = var_toobj(a_val)
        attr = var_tostr(b_val)
        result = bvalue()
        var_setval(result, c_val)
        if var_isfunction(result):
            var_markstatic(result)
        if not be_cls.be_class_setmember(vm, obj, attr, result):
            vm_error(vm, "attribute_error",
                     "class '%s' cannot assign to static attribute '%s'",
                     be_str2cstr(be_class_name(obj)), be_str2cstr(attr))
        return

    if var_ismodule(a_val) and var_isstr(b_val):
        obj = var_toobj(a_val)
        attr = var_tostr(b_val)
        if be_module_setmember(vm, obj, attr, c_val):
            return
        # fall through to error

    attribute_error(vm, "writable attribute", a_val, b_val)


# ============================================================================
# GETIDX / SETIDX opcode helpers
# ============================================================================

def _exec_getidx(vm, reg_idx, ktab, ins, ra_idx):
    b_val = _resolve_rkb(vm, reg_idx, ktab, ins)
    c_val = _resolve_rkc(vm, reg_idx, ktab, ins)
    if var_isinstance(b_val):
        top_idx = vm.top_idx
        # Put b on stack for obj_method
        var_setval(vm.stack[top_idx + 1], b_val)
        obj_method(vm, top_idx + 1, str_literal(vm, "item"), top_idx)
        var_setval(vm.stack[top_idx + 1], b_val)   # argv[0] = self
        var_setval(vm.stack[top_idx + 2], c_val)    # argv[1] = key
        vm.top_idx += 3
        be_dofunc(vm, top_idx, 2)
        vm.top_idx -= 3
        reg_idx = vm.reg_idx
        ra_idx = reg_idx + IGET_RA(ins)
        var_setval(vm.stack[ra_idx], vm.stack[vm.top_idx])
    elif var_isstr(b_val):
        s = be_strindex(vm, var_tostr(b_val), c_val)
        reg_idx = vm.reg_idx
        ra_idx = reg_idx + IGET_RA(ins)
        var_setstr(vm.stack[ra_idx], s)
    elif var_iscomptr(b_val) and var_isint(c_val):
        # comptr[int] — not meaningful in Python, stub
        var_setint(vm.stack[ra_idx], 0)
    else:
        vm_error(vm, "type_error",
                 "value '%s' does not support subscriptable",
                 be_vtype2str(b_val))


def _exec_setidx(vm, reg_idx, ktab, ins, ra_idx):
    a_val = vm.stack[ra_idx]
    b_val = _resolve_rkb(vm, reg_idx, ktab, ins)
    c_val = _resolve_rkc(vm, reg_idx, ktab, ins)
    if var_isinstance(a_val):
        top_idx = vm.top_idx
        var_setval(vm.stack[top_idx + 1], a_val)
        obj_method(vm, top_idx + 1, str_literal(vm, "setitem"), top_idx)
        var_setval(vm.stack[top_idx + 1], a_val)    # argv[0] = self
        var_setval(vm.stack[top_idx + 2], b_val)    # argv[1] = key
        var_setval(vm.stack[top_idx + 3], c_val)    # argv[2] = value
        vm.top_idx += 4
        be_dofunc(vm, top_idx, 3)
        vm.top_idx -= 4
    elif var_iscomptr(a_val) and var_isint(b_val) and var_isint(c_val):
        pass  # comptr[int] = int — not meaningful in Python
    else:
        vm_error(vm, "type_error",
                 "value '%s' does not support index assignment",
                 be_vtype2str(a_val))


# ============================================================================
# SETSUPER / IMPORT / CATCH / RAISE / EXBLK opcode helpers
# ============================================================================

def _exec_setsuper(vm, reg_idx, ktab, ins, ra_idx):
    a_val = vm.stack[ra_idx]
    b_val = _resolve_rkb(vm, reg_idx, ktab, ins)
    if var_isclass(a_val) and var_isclass(b_val):
        obj = var_toobj(a_val)
        if not gc_isconst(obj):
            be_class_setsuper(obj, var_toobj(b_val))
        else:
            vm_error(vm, "internal_error",
                     "cannot change superclass of a read-only class")
    else:
        vm_error(vm, "type_error",
                 "value '%s' does not support set super",
                 be_vtype2str(b_val))


def _exec_import(vm, reg_idx, ktab, ins, ra_idx):
    be_exec = _lazy_be_exec()
    b_val = _resolve_rkb(vm, reg_idx, ktab, ins)
    if var_isstr(b_val):
        name = var_tostr(b_val)
        res = be_module_load(vm, name)
        reg_idx = vm.reg_idx
        ra_idx = reg_idx + IGET_RA(ins)
        if res == BE_OK:
            be_exec.be_stackpop(vm, 1)
            var_setval(vm.stack[ra_idx], vm.stack[vm.top_idx])
        elif res == BE_EXCEPTION:
            # Pop exception value and message, then rethrow
            vm.top_idx -= 2
            be_exec.be_throw(vm, BE_EXCEPTION)
        else:
            vm_error(vm, "import_error", "module '%s' not found",
                     be_str2cstr(name))
    else:
        vm_error(vm, "type_error",
                 "import '%s' does not support import",
                 be_vtype2str(b_val))


def _exec_catch(vm, reg_idx, ins, ra_idx):
    base_idx = ra_idx
    top_idx = vm.top_idx
    ecnt = IGET_RKB(ins)
    vcnt = IGET_RKC(ins)
    i = 0
    while i < ecnt and not be_vm_iseq(vm, vm.stack[top_idx], vm.stack[base_idx + i]):
        i += 1
    if not ecnt or i < ecnt:
        # Exception caught
        base_idx = reg_idx + IGET_RA(ins)  # recalc after possible GC
        top_idx = vm.top_idx
        for j in range(vcnt):
            var_setval(vm.stack[base_idx + j], vm.stack[top_idx + j])
        vm.ip_idx += 1  # skip next instruction


def _exec_raise(vm, reg_idx, ktab, ins):
    be_exec = _lazy_be_exec()
    if IGET_RA(ins) < 2:
        top_idx = vm.top_idx
        b_val = _resolve_rkb(vm, reg_idx, ktab, ins)
        var_setval(vm.stack[top_idx], b_val)
        if IGET_RA(ins):
            c_val = _resolve_rkc(vm, reg_idx, ktab, ins)
            var_setval(vm.stack[top_idx + 1], c_val)
        else:
            var_setnil(vm.stack[top_idx + 1])
        be_exec.be_save_stacktrace(vm)
    be_exec.be_throw(vm, BE_EXCEPTION)


def _exec_exblk(vm, ins):
    """Handle EXBLK opcode."""
    be_exec = _lazy_be_exec()
    if BE_USE_PERF_COUNTERS:
        vm.counter_try += 1
    if not IGET_RA(ins):
        # Setup exception block — push frame with current state.
        # In C, setjmp returns 0 here (setup). When longjmp is called,
        # execution resumes at the setjmp call site with non-zero.
        # In Python, the BerryException is caught by the try/except
        # wrapper in the vm_exec dispatch loop.
        be_exec.be_except_block_setup(vm)
    else:
        # Close exception block
        be_exec.be_except_block_close(vm, IGET_Bx(ins))
    return None


# ============================================================================
# CALL opcode helper
# ============================================================================

def _exec_call(vm, reg_idx, ktab, ins, ra_idx, clos):
    """Handle CALL opcode. Returns 'newframe' if a closure call needs dispatch."""
    be_cls = _lazy_be_class()
    be_vector = _lazy_be_vector()

    if BE_USE_PERF_COUNTERS:
        vm.counter_call += 1

    var_idx = ra_idx
    mode = 0
    argc = IGET_RKB(ins)

    # recall loop — mirrors the C goto recall
    while True:
        var_val = vm.stack[var_idx]
        vt = var_type(var_val)

        if vt == NOT_METHOD:
            # NOT_METHOD: shift arguments
            var_setval(vm.stack[var_idx], vm.stack[var_idx + 1])
            var_idx += 1
            argc -= 1
            mode = 1
            continue  # recall

        elif vt == BE_CLASS:
            # Instantiate class and call constructor
            cls_obj = var_toobj(var_val)
            argc += 1
            if be_cls.be_class_newobj(vm, cls_obj, var_idx - vm.reg_idx, argc, mode):
                reg_idx = vm.reg_idx + mode
                mode = 0
                var_idx = reg_idx + IGET_RA(ins) + 1
                reg_idx = vm.reg_idx
                continue  # recall — call constructor
            break  # no constructor, done

        elif vt == BE_INSTANCE:
            # Call '()' method on instance
            obj = var_toobj(var_val)
            temp = bvalue()
            # Put instance on stack for obj_method
            var_setval(vm.stack[vm.top_idx], var_val)
            obj_method(vm, vm.top_idx, str_literal(vm, "()"), vm.top_idx + 1)
            temp_val = vm.stack[vm.top_idx + 1]
            # Shift arguments up by 1
            for v_i in range(var_idx + argc - 1, var_idx - 1, -1):
                var_setval(vm.stack[v_i + 1], vm.stack[v_i])
            var_setval(vm.stack[var_idx], temp_val)
            argc += 1
            continue  # recall

        elif vt == BE_CLOSURE:
            # Call Berry closure
            prep_closure(vm, var_idx - vm.reg_idx, argc, mode)
            reg_idx = vm.reg_idx
            return 'newframe'

        elif vt == BE_NTVCLOS:
            # Call native closure
            f = var_toobj(var_val)
            precall(vm, var_idx, argc, mode)
            vm.cf.status = PRIM_FUNC
            f.f(vm)
            ret_native(vm)
            break

        elif vt == BE_NTVFUNC:
            # Call native function
            f = var_tontvfunc(var_val)
            precall(vm, var_idx, argc, mode)
            vm.cf.status = PRIM_FUNC
            f(vm)
            ret_native(vm)
            break

        elif vt == BE_CTYPE_FUNC:
            if vm.ctypefunc:
                args = var_toobj(var_val)
                precall(vm, var_idx, argc, mode)
                vm.cf.status = PRIM_FUNC
                vm.ctypefunc(vm, args)
                ret_native(vm)
            else:
                vm_error(vm, "internal_error", "missing ctype_func handler")
            break

        elif vt == BE_MODULE:
            # Call '()' on module
            attr = bvalue()
            var_setstr(attr, str_literal(vm, "()"))
            module = var_toobj(var_val)
            type_ = be_module_attr(vm, module, var_tostr(attr), vm.stack[var_idx])
            if type_ == BE_NONE:
                vm_error(vm, "attribute_error",
                         "module '%s' has no member '()'",
                         be_module_name(module))
            continue  # recall

        else:
            call_error(vm, var_val)
            break

    return None  # normal dispatch continues


# ============================================================================
# RET opcode helper
# ============================================================================

def _exec_ret(vm, reg_idx, ktab, ins):
    """Handle RET opcode. Returns 'return' to exit vm_exec, or 'newframe'."""
    be_vector = _lazy_be_vector()
    cf = vm.cf
    ret_idx = cf.func

    # Copy return value
    if IGET_RA(ins):
        b_val = _resolve_rkb(vm, reg_idx, ktab, ins)
        var_setval(vm.stack[ret_idx], b_val)
    else:
        var_setnil(vm.stack[ret_idx])

    vm.reg_idx = cf.reg
    vm.top_idx = cf.top
    vm.ip = cf.ip
    vm.ip_idx = cf.ip_idx
    be_vector.be_stack_pop(vm.callstack)

    if cf.status & BASE_FRAME:
        # Entrance function — return from vm_exec
        if not be_vector.be_stack_isempty(vm.callstack):
            vm.cf = vm.callstack.data[vm.callstack.end]
        return 'return'

    vm.cf = vm.callstack.data[vm.callstack.end]
    return 'newframe'


# ============================================================================
# prep_closure — prepare closure call with vararg support
# ============================================================================

# static void prep_closure(bvm *vm, int pos, int argc, int mode)
# {
#     bvalue *v, *end;
#     bproto *proto = var2cl(vm->reg + pos)->proto;
#     push_closure(vm, vm->reg + pos, proto->nstack, mode);
#     end = vm->reg + proto->argc;
#     for (v = vm->reg + argc; v <= end; ++v) {
#         var_setnil(v);
#     }
#     if (proto->varg & BE_VA_VARARG) { ... }
# }
def prep_closure(vm, pos, argc, mode):
    """Prepare a closure call, filling missing args with nil and building vararg list."""
    be_exec = _lazy_be_exec()
    be_list_mod = _lazy_be_list()

    proto = var2cl(vm.stack[vm.reg_idx + pos]).proto
    push_closure(vm, vm.reg_idx + pos, proto.nstack, mode)

    # Fill missing arguments with nil
    end_idx = vm.reg_idx + proto.argc
    v_idx = vm.reg_idx + argc
    while v_idx <= end_idx:
        var_setnil(vm.stack[v_idx])
        v_idx += 1

    if proto.varg & BE_VA_VARARG:
        # Build vararg list from extra arguments
        be_exec.be_stack_require(vm, argc + 4)
        top_save_idx = vm.top_idx
        vm.top_idx = v_idx

        # Create a proper list instance via be_newobject (mirrors C impl).
        # be_newobject pushes: [instance, raw_list] on the stack.
        be_api = _lazy_be_api()
        be_api.be_newobject(vm, "list")
        # vm.stack[vm.top_idx - 1] is the raw blist (.p member)
        list_obj = var_toobj(vm.stack[vm.top_idx - 1])
        # Push vararg values into the raw list
        va_start = vm.reg_idx + proto.argc - 1
        for vi in range(va_start, vm.reg_idx + argc):
            be_list_mod.be_list_push(vm, list_obj, vm.stack[vi])
        # Store the list *instance* (top - 2) at the vararg parameter slot
        var_setval(vm.stack[vm.reg_idx + proto.argc - 1], vm.stack[vm.top_idx - 2])

        vm.top_idx = top_save_idx


# ============================================================================
# do_closure, do_ntvclos, do_ntvfunc, do_cfunc, do_class
# ============================================================================

# static void do_closure(bvm *vm, int pos, int argc)
def do_closure(vm, pos, argc):
    prep_closure(vm, pos, argc, 0)
    vm_exec(vm)

# static void do_ntvclos(bvm *vm, int pos, int argc)
def do_ntvclos(vm, pos, argc):
    f = var_toobj(vm.stack[vm.reg_idx + pos])
    precall(vm, vm.reg_idx + pos, argc, 0)
    vm.cf.status = PRIM_FUNC
    f.f(vm)
    ret_native(vm)

# static void do_ntvfunc(bvm *vm, int pos, int argc)
def do_ntvfunc(vm, pos, argc):
    f = var_tontvfunc(vm.stack[vm.reg_idx + pos])
    precall(vm, vm.reg_idx + pos, argc, 0)
    vm.cf.status = PRIM_FUNC
    f(vm)
    ret_native(vm)

# static void do_cfunc(bvm *vm, int pos, int argc)
def do_cfunc(vm, pos, argc):
    if vm.ctypefunc:
        args = var_toobj(vm.stack[vm.reg_idx + pos])
        precall(vm, vm.reg_idx + pos, argc, 0)
        vm.cf.status = PRIM_FUNC
        vm.ctypefunc(vm, args)
        ret_native(vm)
    else:
        vm_error(vm, "internal_error", "missing ctype_func handler")

# static void do_class(bvm *vm, int pos, int argc)
def do_class(vm, pos, argc):
    be_cls = _lazy_be_class()
    be_exec = _lazy_be_exec()
    argc += 1
    if be_cls.be_class_newobj(vm, var_toobj(vm.stack[vm.reg_idx + pos]), pos, argc, 0):
        be_exec.be_incrtop(vm)
        be_dofunc(vm, vm.reg_idx + pos + 1, argc)
        be_exec.be_stackpop(vm, 1)


# ============================================================================
# be_dofunc — main entry point for calling Berry functions
# ============================================================================

# void be_dofunc(bvm *vm, bvalue *v, int argc)
# {
#     int pos = v - vm->reg;
#     switch (var_type(v)) {
#     case BE_CLASS: do_class(vm, pos, argc); break;
#     case BE_CLOSURE: do_closure(vm, pos, argc); break;
#     case BE_NTVCLOS: do_ntvclos(vm, pos, argc); break;
#     case BE_NTVFUNC: do_ntvfunc(vm, pos, argc); break;
#     case BE_CTYPE_FUNC: do_cfunc(vm, pos, argc); break;
#     default: call_error(vm, v);
#     }
# }
def be_dofunc(vm, v_idx, argc):
    """Execute a function value at stack index v_idx with argc arguments."""
    pos = v_idx - vm.reg_idx
    v = vm.stack[v_idx]
    vt = var_type(v)
    if vt == BE_CLASS:
        do_class(vm, pos, argc)
    elif vt == BE_CLOSURE:
        do_closure(vm, pos, argc)
    elif vt == BE_NTVCLOS:
        do_ntvclos(vm, pos, argc)
    elif vt == BE_NTVFUNC:
        do_ntvfunc(vm, pos, argc)
    elif vt == BE_CTYPE_FUNC:
        do_cfunc(vm, pos, argc)
    else:
        call_error(vm, v)


# ============================================================================
# be_default_init_native_function
# ============================================================================

# int be_default_init_native_function(bvm *vm)
# {
#     int argc = be_top(vm);
#     if (argc >= 1) {
#         be_pushvalue(vm, 1);
#     } else {
#         be_pushnil(vm);
#     }
#     be_return(vm);
# }
def be_default_init_native_function(vm):
    """Default empty constructor — returns self (arg 1) or nil."""
    # be_top equivalent: vm.top_idx - vm.reg_idx
    argc = vm.top_idx - vm.reg_idx
    if argc >= 1:
        # be_pushvalue(vm, 1) — push copy of arg 1
        var_setval(vm.stack[vm.top_idx], vm.stack[vm.reg_idx])
        vm.top_idx += 1
    else:
        var_setnil(vm.stack[vm.top_idx])
        vm.top_idx += 1
    # be_return(vm) — set return value and return
    ret_idx = vm.cf.func
    var_setval(vm.stack[ret_idx], vm.stack[vm.top_idx - 1])


# ============================================================================
# Observability hooks
# ============================================================================

# BERRY_API void be_set_obs_hook(bvm *vm, bobshook hook)
def be_set_obs_hook(vm, hook):
    vm.obshook = hook

# BERRY_API void be_set_ctype_func_hanlder(bvm *vm, bctypefunc handler)
def be_set_ctype_func_hanlder(vm, handler):
    vm.ctypefunc = handler

# BERRY_API bctypefunc be_get_ctype_func_hanlder(bvm *vm)
def be_get_ctype_func_hanlder(vm):
    return vm.ctypefunc


# ============================================================================
# be_indexof — resolve stack index (positive = absolute, negative = relative)
# ============================================================================

# bvalue* be_indexof(bvm *vm, int idx)
# {
#     if (idx > 0) {
#         return vm->reg + idx - 1;
#     }
#     return vm->top + idx;
# }
def be_indexof(vm, idx):
    """Resolve a stack index to an absolute index into vm.stack."""
    if idx > 0:
        return vm.reg_idx + idx - 1
    return vm.top_idx + idx
