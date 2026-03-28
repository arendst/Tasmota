"""
berry_port/be_baselib.py — Port of src/be_baselib.c

Base library functions: assert, print, input, super, type, classname,
classof, number, int, real, str, size, module, compile, issubclass,
isinstance, __iterator__, call, bool, format.

Original C code is included as comments.
"""

# ============================================================================
# Imports — lazy to avoid circular dependencies
# ============================================================================

import sys

def _lazy_be_api():
    import berry_port.be_api as m; return m

def _lazy_be_object():
    import berry_port.be_object as m; return m

def _lazy_be_vm():
    import berry_port.be_vm as m; return m

def _lazy_be_class():
    import berry_port.be_class as m; return m

def _lazy_be_map():
    import berry_port.be_map as m; return m

def _lazy_be_vector():
    import berry_port.be_vector as m; return m

def _lazy_be_strlib():
    import berry_port.be_strlib as m; return m

def _lazy_be_exec():
    import berry_port.be_exec as m; return m

def _lazy_be_mem():
    import berry_port.be_mem as m; return m

def _lazy_be_string():
    import berry_port.be_string as m; return m

READLINE_STEP = 100

# ============================================================================
# be_baselib_assert
# ============================================================================

# int be_baselib_assert(bvm *vm)
# {
#     int argc = be_top(vm);
#     /* assertion fails when there is no argument
#      * or the first argument is nil or false. */
#     if (!argc || !be_tobool(vm, 1)) {
#         const char *msg = "assert failed!";
#         if (argc >= 2 && be_isstring(vm, 2)) {
#             msg = be_tostring(vm, 2);
#         }
#         be_raise(vm, "assert_failed", msg);
#     }
#     be_return_nil(vm);
# }
def be_baselib_assert(vm):
    be_api = _lazy_be_api()
    argc = be_api.be_top(vm)
    if not argc or not be_api.be_tobool(vm, 1):
        msg = "assert failed!"
        if argc >= 2 and be_api.be_isstring(vm, 2):
            msg = be_api.be_tostring(vm, 2)
        be_api.be_raise(vm, "assert_failed", msg)
    return be_api.be_returnnilvalue(vm)

# ============================================================================
# be_baselib_print
# ============================================================================

# int be_baselib_print(bvm *vm)
# {
#     int i, argc = be_top(vm);
#     for (i = 1; i <= argc; ++i) {
#         const char *str = be_tostring(vm, i);
#         size_t len = be_strlen(vm, i);
#         be_writebuffer(str, len);
#         if (i < argc) {
#             be_writebuffer(" ", 1);
#         }
#     }
#     be_writenewline();
#     be_return_nil(vm);
# }
def be_baselib_print(vm):
    be_api = _lazy_be_api()
    argc = be_api.be_top(vm)
    for i in range(1, argc + 1):
        s = be_api.be_tostring(vm, i)
        be_api.be_writestring(s)
        if i < argc:
            be_api.be_writestring(" ")
    be_api.be_writenewline()
    return be_api.be_returnnilvalue(vm)


# ============================================================================
# m_readline  (static helper)
# ============================================================================

# static int m_readline(bvm *vm)
# {
#     size_t pos = 0, size = READLINE_STEP;
#     char *buffer = be_malloc(vm, size);
#     char *res = be_readstring(buffer, (int)size);
#     while (res) {
#         pos += strlen(buffer + pos) - 1;
#         if (!pos || buffer[pos] == '\n') {
#             buffer[pos] = '\0'; /* trim \n */
#             break;
#         }
#         buffer = be_realloc(vm, buffer, size, size + READLINE_STEP);
#         res = be_readstring(buffer + pos + 1, READLINE_STEP);
#         size += READLINE_STEP;
#     }
#     be_pushstring(vm, buffer);
#     be_free(vm, buffer, size);
#     be_return(vm);
# }
def _m_readline(vm):
    """Read a line from stdin. Simplified from C version since Python handles
    line buffering natively."""
    be_api = _lazy_be_api()
    try:
        line = sys.stdin.readline()
        if line.endswith('\n'):
            line = line[:-1]
    except EOFError:
        line = ""
    be_api.be_pushstring(vm, line)
    return be_api.be_returnvalue(vm)

# ============================================================================
# be_baselib_input
# ============================================================================

# int be_baselib_input(bvm *vm)
# {
#     if (be_top(vm) && be_isstring(vm, 1)) { /* echo prompt */
#         be_writestring(be_tostring(vm, 1));
#     }
#     return m_readline(vm);
# }
def be_baselib_input(vm):
    be_api = _lazy_be_api()
    if be_api.be_top(vm) and be_api.be_isstring(vm, 1):
        be_api.be_writestring(be_api.be_tostring(vm, 1))
    return _m_readline(vm)

# ============================================================================
# find_class_closure  (static helper)
# ============================================================================

# /* Look in the current class and all super classes for a method
#    corresponding to a specific closure pointer */
# static bclass *find_class_closure(bclass *cl, bclosure *needle)
# {
#     while (cl) {
#         bmapnode *node;  /* iterate on members of the class */
#         bmap *members = be_class_members(cl);
#         if (members) {  /* only iterate if there are members */
#             bmapiter iter = be_map_iter();
#             while ((node = be_map_next(members, &iter)) != NULL) {
#                 if (var_primetype(&node->value) == BE_CLOSURE) {
#                     bclosure *clos_iter = var_toobj(&node->value);
#                     if (clos_iter == needle) {
#                         return cl;
#                     }
#                 }
#             }
#         }
#         cl = be_class_super(cl);  /* move to super class */
#     }
#     return NULL;  /* not found */
# }
def _find_class_closure(cl, needle):
    be_obj = _lazy_be_object()
    be_cls = _lazy_be_class()
    be_map = _lazy_be_map()
    while cl is not None:
        members = be_obj.be_class_members(cl)
        if members is not None:
            iter_ref = [be_obj.be_map_iter()]
            while True:
                node, iter_ref = be_map.be_map_next(members, iter_ref)
                if node is None:
                    break
                if be_obj.var_primetype(node.value) == be_obj.BE_CLOSURE:
                    clos_iter = be_obj.var_toobj(node.value)
                    if clos_iter is needle:
                        return cl
        cl = be_obj.be_class_super(cl)
    return None

# ============================================================================
# obj2int  (static helper)
# ============================================================================

# static bbool obj2int(bvm *vm, bvalue *var, bint *val)
# {
#     binstance *obj = var_toobj(var);
#     bstring *toint = str_literal(vm, "toint");
#     int type = be_instance_member(vm, obj, toint, vm->top);
#     if (type != BE_NONE && type != BE_NIL) {
#         vm->top[1] = *var; /* move self to argv[0] */
#         be_dofunc(vm, vm->top, 1); /* call method 'toint' */
#         if (var_isint(vm->top)) {
#             *val = var_toint(vm->top);
#             return btrue;
#         }
#     }
#     return bfalse;
# }
def _obj2int(vm, var):
    """Try to call 'toint' method on an instance. Returns (True, int_val) or (False, 0)."""
    be_obj = _lazy_be_object()
    be_cls = _lazy_be_class()
    be_vm_mod = _lazy_be_vm()
    obj = be_obj.var_toobj(var)
    toint_str = be_vm_mod.str_literal(vm, "toint")
    # be_instance_member writes result to vm.stack[vm.top_idx]
    type_ = be_cls.be_instance_member(vm, obj, toint_str, vm.stack[vm.top_idx] if vm.top_idx < len(vm.stack) else None)
    if type_ != be_obj.BE_NONE and type_ != be_obj.BE_NIL:
        # move self to argv[0]: vm->top[1] = *var
        if vm.top_idx + 1 >= len(vm.stack):
            vm.stack.append(be_obj.bvalue())
        be_obj.var_setval(vm.stack[vm.top_idx + 1], var)
        be_vm_mod.be_dofunc(vm, vm.top_idx, 1)
        top_val = vm.stack[vm.top_idx]
        if be_obj.var_isint(top_val):
            return (True, be_obj.var_toint(top_val))
    return (False, 0)


# ============================================================================
# be_baselib_super
# ============================================================================

# int be_baselib_super(bvm *vm)
# {
#     int argc = be_top(vm);
#     if (argc == 0 || be_isnil(vm, 1)) {
#         be_return_nil(vm);
#     }
#     if (be_isclass(vm, 1)) {
#         be_getsuper(vm, 1);
#         be_return(vm);
#     }
#     if (be_isinstance(vm, 1)) {
#         binstance *o = var_toobj(be_indexof(vm, 1));
#         bclass *target_class = NULL;
#         bclass *base_class = NULL;
#         if (argc >= 2) {
#             if (be_isclass(vm, 2)) {
#                 target_class = var_toobj(be_indexof(vm, 2));
#             } else if (be_isnil(vm, 2)) {
#                 // ignore
#             } else {
#                 be_raise(vm, "type_error", "leveled super() requires 'instance' and 'class' arguments");
#             }
#         }
#         if (argc == 1) {
#             int size = be_stack_count(&vm->callstack);
#             if (size >= 2) {
#                 bcallframe *caller = be_vector_at(&vm->callstack, size - 2);
#                 bvalue *func = caller->func;
#                 if (var_primetype(func) == BE_CLOSURE) {
#                     bclosure *clos_ctx = var_toobj(func);
#                     base_class = find_class_closure(o->_class, clos_ctx);
#                 }
#             }
#         }
#         if (base_class || target_class) {
#             if (base_class) {
#                 target_class = base_class->super;
#                 if (!target_class) be_return_nil(vm);
#             }
#             if (o) {
#                 o = be_instance_super(o);
#             }
#             while (o) {
#                 bclass *c = be_instance_class(o);
#                 if (c == target_class) break;
#                 o = be_instance_super(o);
#             }
#             bvalue *top = be_incrtop(vm);
#             if (o) {
#                 var_setinstance(top, o);
#             } else {
#                 var_setnil(top);
#             }
#             be_return(vm);
#         } else {
#             be_getsuper(vm, 1);
#             be_return(vm);
#         }
#     }
#     be_return_nil(vm);
# }
def be_baselib_super(vm):
    be_api = _lazy_be_api()
    be_obj = _lazy_be_object()
    be_vec = _lazy_be_vector()

    argc = be_api.be_top(vm)

    # if no argument, or arg 1 is nil, return nil
    if argc == 0 or be_api.be_isnil(vm, 1):
        return be_api.be_returnnilvalue(vm)

    # if arg 1 is a class, simply return super
    if be_api.be_isclass(vm, 1):
        be_api.be_getsuper(vm, 1)
        return be_api.be_returnvalue(vm)

    # arg 1 is an instance
    if be_api.be_isinstance(vm, 1):
        o = be_obj.var_toobj(vm.stack[be_api.be_indexof(vm, 1)])
        target_class = None
        base_class = None

        # if arg 2 is present, it must be a class
        if argc >= 2:
            if be_api.be_isclass(vm, 2):
                target_class = be_obj.var_toobj(vm.stack[be_api.be_indexof(vm, 2)])
            elif be_api.be_isnil(vm, 2):
                pass  # ignore
            else:
                be_api.be_raise(vm, "type_error",
                    "leveled super() requires 'instance' and 'class' arguments")

        # if instance is the sole argument, try to find caller's class
        if argc == 1:
            size = be_vec.be_stack_count(vm.callstack)
            if size >= 2:
                caller = be_vec.be_vector_at(vm.callstack, size - 2)
                func = caller.func if isinstance(caller.func, be_obj.bvalue) else vm.stack[caller.func]
                if be_obj.var_primetype(func) == be_obj.BE_CLOSURE:
                    clos_ctx = be_obj.var_toobj(func)
                    base_class = _find_class_closure(o._class, clos_ctx)

        if base_class is not None or target_class is not None:
            if base_class is not None:
                target_class = base_class.super
                if target_class is None:
                    return be_api.be_returnnilvalue(vm)

            # leveled super: skip current and walk to target
            if o is not None:
                o = be_obj.be_instance_super(o)
            while o is not None:
                c = be_obj.be_instance_class(o)
                if c is target_class:
                    break
                o = be_obj.be_instance_super(o)

            top_idx = be_api.be_incrtop(vm)
            top = vm.stack[top_idx]
            if o is not None:
                be_obj.var_setinstance(top, o)
            else:
                be_obj.var_setnil(top)
            return be_api.be_returnvalue(vm)
        else:
            be_api.be_getsuper(vm, 1)
            return be_api.be_returnvalue(vm)

    # fall through
    return be_api.be_returnnilvalue(vm)


# ============================================================================
# be_baselib_type
# ============================================================================

# int be_baselib_type(bvm *vm)
# {
#     if (be_top(vm)) {
#         be_pushstring(vm, be_typename(vm, 1));
#         be_return(vm);
#     }
#     be_return_nil(vm);
# }
def be_baselib_type(vm):
    be_api = _lazy_be_api()
    if be_api.be_top(vm):
        be_api.be_pushstring(vm, be_api.be_typename(vm, 1))
        return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)

# ============================================================================
# be_baselib_classname
# ============================================================================

# int be_baselib_classname(bvm *vm)
# {
#     if (be_top(vm)) {
#         const char *t = be_classname(vm, 1);
#         if (t) {
#             be_pushstring(vm, t);
#             be_return(vm);
#         }
#     }
#     be_return_nil(vm);
# }
def be_baselib_classname(vm):
    be_api = _lazy_be_api()
    if be_api.be_top(vm):
        t = be_api.be_classname(vm, 1)
        if t is not None:
            be_api.be_pushstring(vm, t)
            return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)

# ============================================================================
# be_baselib_classof
# ============================================================================

# int be_baselib_classof(bvm *vm)
# {
#     if (be_top(vm) && be_classof(vm, 1)) {
#         be_return(vm);
#     }
#     be_return_nil(vm);
# }
def be_baselib_classof(vm):
    be_api = _lazy_be_api()
    if be_api.be_top(vm) and be_api.be_classof(vm, 1):
        return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)

# ============================================================================
# be_baselib_number
# ============================================================================

# int be_baselib_number(bvm *vm)
# {
#     if (be_top(vm)) {
#         if (be_isstring(vm, 1)) {
#             be_str2num(vm, be_tostring(vm, 1));
#             be_return(vm);
#         } else if (be_isnumber(vm, 1)) {
#             be_pushvalue(vm, 1);
#             be_return(vm);
#         }
#     }
#     be_return_nil(vm);
# }
def be_baselib_number(vm):
    be_api = _lazy_be_api()
    be_strlib = _lazy_be_strlib()
    if be_api.be_top(vm):
        if be_api.be_isstring(vm, 1):
            be_strlib.be_str2num(vm, be_api.be_tostring(vm, 1))
            return be_api.be_returnvalue(vm)
        elif be_api.be_isnumber(vm, 1):
            be_api.be_pushvalue(vm, 1)
            return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)


# ============================================================================
# be_baselib_int
# ============================================================================

# int be_baselib_int(bvm *vm)
# {
#     if (be_top(vm)) {
#         if (be_isstring(vm, 1)) {
#             const char *s = be_tostring(vm, 1);
#             be_pushint(vm, be_str2int(s, NULL));
#         } else if (be_isreal(vm, 1)) {
#             be_pushint(vm, (bint)be_toreal(vm, 1));
#         } else if (be_isint(vm, 1)) {
#             be_pushvalue(vm, 1);
#         } else if (be_isbool(vm, 1)) {
#             be_pushint(vm, be_tobool(vm, 1) ? 1 : 0);
#         } else if (be_iscomptr(vm, 1)) {
#             intptr_t p = (intptr_t) be_tocomptr(vm, 1);
#             be_pushint(vm, (int) p);
#         } else if (be_isinstance(vm, 1)) {
#             bvalue *v = be_indexof(vm, 1);
#             bint val;
#             if (obj2int(vm, v, &val)) {
#                 be_pushint(vm, val);
#             } else {
#                 be_return_nil(vm);
#             }
#         } else {
#             be_return_nil(vm);
#         }
#         be_return(vm);
#     }
#     be_return_nil(vm);
# }
def be_baselib_int(vm):
    be_api = _lazy_be_api()
    be_strlib = _lazy_be_strlib()
    if be_api.be_top(vm):
        if be_api.be_isstring(vm, 1):
            s = be_api.be_tostring(vm, 1)
            be_api.be_pushint(vm, be_strlib.be_str2int(s))
        elif be_api.be_isreal(vm, 1):
            be_api.be_pushint(vm, int(be_api.be_toreal(vm, 1)))
        elif be_api.be_isint(vm, 1):
            be_api.be_pushvalue(vm, 1)
        elif be_api.be_isbool(vm, 1):
            be_api.be_pushint(vm, 1 if be_api.be_tobool(vm, 1) else 0)
        elif be_api.be_iscomptr(vm, 1):
            p = be_api.be_tocomptr(vm, 1)
            be_api.be_pushint(vm, int(p) if p is not None else 0)
        elif be_api.be_isinstance(vm, 1):
            v = vm.stack[be_api.be_indexof(vm, 1)]
            ok, val = _obj2int(vm, v)
            if ok:
                be_api.be_pushint(vm, val)
            else:
                return be_api.be_returnnilvalue(vm)
        else:
            return be_api.be_returnnilvalue(vm)
        return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)

# ============================================================================
# be_baselib_real
# ============================================================================

# int be_baselib_real(bvm *vm)
# {
#     if (be_top(vm)) {
#         if (be_isstring(vm, 1)) {
#             const char *s = be_tostring(vm, 1);
#             be_pushreal(vm, be_str2real(s, NULL));
#         } else if (be_isint(vm, 1)) {
#             be_pushreal(vm, (breal)be_toint(vm, 1));
#         } else if (be_isreal(vm, 1)) {
#             be_pushvalue(vm, 1);
#         } else {
#             be_return_nil(vm);
#         }
#         be_return(vm);
#     }
#     be_return_nil(vm);
# }
def be_baselib_real(vm):
    be_api = _lazy_be_api()
    be_strlib = _lazy_be_strlib()
    if be_api.be_top(vm):
        if be_api.be_isstring(vm, 1):
            s = be_api.be_tostring(vm, 1)
            be_api.be_pushreal(vm, be_strlib.be_str2real(s))
        elif be_api.be_isint(vm, 1):
            be_api.be_pushreal(vm, float(be_api.be_toint(vm, 1)))
        elif be_api.be_isreal(vm, 1):
            be_api.be_pushvalue(vm, 1)
        else:
            return be_api.be_returnnilvalue(vm)
        return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)

# ============================================================================
# check_method  (static helper)
# ============================================================================

# static int check_method(bvm *vm, const char *attr)
# {
#     return be_top(vm) &&
#         be_isinstance(vm, 1) && be_getmethod(vm, 1, attr);
# }
def _check_method(vm, attr):
    be_api = _lazy_be_api()
    return (be_api.be_top(vm) and
            be_api.be_isinstance(vm, 1) and
            be_api.be_getmethod(vm, 1, attr))

# ============================================================================
# be_baselib_iterator
# ============================================================================

# int be_baselib_iterator(bvm *vm)
# {
#     if (be_top(vm) && be_isfunction(vm, 1)) {
#         be_return(vm); /* return the argument[0]::function */
#     }
#     if (check_method(vm, "iter")) {
#         be_pushvalue(vm, 1);
#         be_call(vm, 1);
#         be_pop(vm, 1);
#         be_return(vm);
#     }
#     be_return_nil(vm);
# }
def be_baselib_iterator(vm):
    be_api = _lazy_be_api()
    if be_api.be_top(vm) and be_api.be_isfunction(vm, 1):
        return be_api.be_returnvalue(vm)
    if _check_method(vm, "iter"):
        be_api.be_pushvalue(vm, 1)
        be_api.be_call(vm, 1)
        be_api.be_pop(vm, 1)
        return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)


# ============================================================================
# l_call
# ============================================================================

# /* call a function with variable number of arguments */
# /* first argument is a callable object (function, closure, native function, native closure) */
# /* then all subsequent arguments are pushed except the last one */
# /* If the last argument is a 'list', then all elements are pushed as arguments */
# /* otherwise the last argument is pushed as well */
# static int l_call(bvm *vm)
# {
#     int top = be_top(vm);
#     if (top >= 1 && (be_isfunction(vm, 1) || be_isclass(vm, 1))) {
#         size_t arg_count = top - 1;
#         if (top > 1 && be_isinstance(vm, top) && be_getmember(vm, top, ".p") && be_islist(vm, top + 1)) {
#             int32_t list_size = be_data_size(vm, top + 1);
#             if (list_size > 0) {
#                 be_stack_require(vm, list_size + 3);
#                 for (int i = 0; i < list_size; i++) {
#                     be_pushnil(vm);
#                 }
#                 be_moveto(vm, top + 1, top + 1 + list_size);
#                 be_moveto(vm, top, top + list_size);
#                 be_pushiter(vm, -1);
#                 while (be_iter_hasnext(vm, -2)) {
#                     be_iter_next(vm, -2);
#                     be_moveto(vm, -1, top);
#                     top++;
#                     be_pop(vm, 1);
#                 }
#                 be_pop(vm, 1);  /* remove iterator */
#             }
#             be_pop(vm, 2);
#             arg_count = arg_count - 1 + list_size;
#         }
#         be_call(vm, arg_count);
#         be_pop(vm, arg_count);
#         be_return(vm);
#     }
#     be_raise(vm, "value_error", "first argument must be a function or a class");
#     be_return_nil(vm);
# }
def l_call(vm):
    be_api = _lazy_be_api()
    top = be_api.be_top(vm)
    if top >= 1 and (be_api.be_isfunction(vm, 1) or be_api.be_isclass(vm, 1)):
        arg_count = top - 1
        # test if last argument is a list
        if (top > 1 and be_api.be_isinstance(vm, top) and
                be_api.be_getmember(vm, top, ".p") and
                be_api.be_islist(vm, top + 1)):
            list_size = be_api.be_data_size(vm, top + 1)
            if list_size > 0:
                be_api.be_stack_require(vm, list_size + 3)
                for _i in range(list_size):
                    be_api.be_pushnil(vm)
                be_api.be_moveto(vm, top + 1, top + 1 + list_size)
                be_api.be_moveto(vm, top, top + list_size)
                be_api.be_pushiter(vm, -1)
                while be_api.be_iter_hasnext(vm, -2):
                    be_api.be_iter_next(vm, -2)
                    be_api.be_moveto(vm, -1, top)
                    top += 1
                    be_api.be_pop(vm, 1)
                be_api.be_pop(vm, 1)  # remove iterator
            be_api.be_pop(vm, 2)
            arg_count = arg_count - 1 + list_size
        # actual call
        be_api.be_call(vm, arg_count)
        # remove args
        be_api.be_pop(vm, arg_count)
        return be_api.be_returnvalue(vm)
    be_api.be_raise(vm, "value_error", "first argument must be a function or a class")
    return be_api.be_returnnilvalue(vm)

# ============================================================================
# be_baselib_str
# ============================================================================

# int be_baselib_str(bvm *vm)
# {
#     if (be_top(vm)) {
#         be_tostring(vm, 1);
#     } else {
#         be_pushstring(vm, "");
#     }
#     be_return(vm);
# }
def be_baselib_str(vm):
    be_api = _lazy_be_api()
    if be_api.be_top(vm):
        be_api.be_tostring(vm, 1)
    else:
        be_api.be_pushstring(vm, "")
    return be_api.be_returnvalue(vm)

# ============================================================================
# l_bool
# ============================================================================

# static int l_bool(bvm *vm)
# {
#     if (be_top(vm)) {
#         be_pushbool(vm, be_tobool(vm, 1));
#     } else {
#         be_pushbool(vm, bfalse);
#     }
#     be_return(vm);
# }
def l_bool(vm):
    be_api = _lazy_be_api()
    if be_api.be_top(vm):
        be_api.be_pushbool(vm, be_api.be_tobool(vm, 1))
    else:
        be_api.be_pushbool(vm, False)
    return be_api.be_returnvalue(vm)

# ============================================================================
# be_baselib_size
# ============================================================================

# int be_baselib_size(bvm *vm)
# {
#     if (be_top(vm) && be_isstring(vm, 1)) {
#         be_pushint(vm, be_strlen(vm, 1));
#         be_return(vm);
#     }
#     if (check_method(vm, "size")) {
#         be_pushvalue(vm, 1);
#         be_call(vm, 1);
#         be_pop(vm, 1);
#         be_return(vm);
#     }
#     be_return_nil(vm);
# }
def be_baselib_size(vm):
    be_api = _lazy_be_api()
    if be_api.be_top(vm) and be_api.be_isstring(vm, 1):
        be_api.be_pushint(vm, be_api.be_strlen(vm, 1))
        return be_api.be_returnvalue(vm)
    if _check_method(vm, "size"):
        be_api.be_pushvalue(vm, 1)
        be_api.be_call(vm, 1)
        be_api.be_pop(vm, 1)
        return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)

# ============================================================================
# be_baselib_module
# ============================================================================

# int be_baselib_module(bvm *vm)
# {
#     int argc = be_top(vm);
#     be_newmodule(vm);
#     if (argc > 0 && be_isstring(vm, 1)) {
#         be_setname(vm, -1, be_tostring(vm, 1));
#     }
#     be_return(vm);
# }
def be_baselib_module(vm):
    be_api = _lazy_be_api()
    argc = be_api.be_top(vm)
    be_api.be_newmodule(vm)
    if argc > 0 and be_api.be_isstring(vm, 1):
        be_api.be_setname(vm, -1, be_api.be_tostring(vm, 1))
    return be_api.be_returnvalue(vm)


# ============================================================================
# compile helpers
# ============================================================================

# #if BE_USE_SCRIPT_COMPILER
# static int raise_compile_error(bvm *vm)
# {
#     be_pop(vm, 2); /* pop the exception value and message */
#     be_throw(vm, BE_EXCEPTION);
#     return 0;
# }
def _raise_compile_error(vm):
    be_api = _lazy_be_api()
    be_exec = _lazy_be_exec()
    be_obj = _lazy_be_object()
    be_api.be_pop(vm, 2)
    be_exec.be_throw(vm, be_obj.BE_EXCEPTION)
    return 0

# static int m_compile_str(bvm *vm)
# {
#     int len = be_strlen(vm, 1);
#     const char *src = be_tostring(vm, 1);
#     int res = be_loadbuffer(vm, "string", src, len);
#     if (res == BE_OK) {
#         be_return(vm);
#     }
#     return raise_compile_error(vm);
# }
def _m_compile_str(vm):
    be_api = _lazy_be_api()
    be_obj = _lazy_be_object()
    length = be_api.be_strlen(vm, 1)
    src = be_api.be_tostring(vm, 1)
    res = be_api.be_loadbuffer(vm, "string", src, length)
    if res == be_obj.BE_OK:
        return be_api.be_returnvalue(vm)
    return _raise_compile_error(vm)

# static int m_compile_file(bvm *vm)
# {
#     const char *fname = be_tostring(vm, 1);
#     int res = be_loadfile(vm, fname);
#     if (res == BE_OK) {
#         be_return(vm);
#     } else if (res == BE_IO_ERROR) {
#         be_pushstring(vm, "io_error");
#         be_pushvalue(vm, -2);
#     }
#     return raise_compile_error(vm);
# }
def _m_compile_file(vm):
    be_api = _lazy_be_api()
    be_obj = _lazy_be_object()
    be_exec = _lazy_be_exec()
    fname = be_api.be_tostring(vm, 1)
    res = be_exec.be_loadmode(vm, fname, False)
    if res == be_obj.BE_OK:
        return be_api.be_returnvalue(vm)
    elif res == be_obj.BE_IO_ERROR:
        be_api.be_pushstring(vm, "io_error")
        be_api.be_pushvalue(vm, -2)
    return _raise_compile_error(vm)

# ============================================================================
# be_baselib_compile
# ============================================================================

# int be_baselib_compile(bvm *vm)
# {
# #if BE_USE_SCRIPT_COMPILER
#     if (be_top(vm) && be_isstring(vm, 1)) {
#         if (be_top(vm) >= 2 && be_isstring(vm, 2)) {
#             const char *s = be_tostring(vm, 2);
#             if (!strcmp(s, "string")) {
#                 return m_compile_str(vm);
#             }
#             if (!strcmp(s, "file")) {
#                 return m_compile_file(vm);
#             }
#         } else {
#             return m_compile_str(vm);
#         }
#     }
# #endif
#     be_return_nil(vm);
# }
def be_baselib_compile(vm):
    be_api = _lazy_be_api()
    from berry_port.berry_conf import BE_USE_SCRIPT_COMPILER
    if BE_USE_SCRIPT_COMPILER:
        if be_api.be_top(vm) and be_api.be_isstring(vm, 1):
            if be_api.be_top(vm) >= 2 and be_api.be_isstring(vm, 2):
                s = be_api.be_tostring(vm, 2)
                if s == "string":
                    return _m_compile_str(vm)
                if s == "file":
                    return _m_compile_file(vm)
            else:
                return _m_compile_str(vm)
    return be_api.be_returnnilvalue(vm)

# ============================================================================
# _issubv  (static helper)
# ============================================================================

# static int _issubv(bvm *vm, bbool (*filter)(bvm*, int))
# {
#     bbool status = bfalse;
#     if (be_top(vm) >= 2 && filter(vm, 1)) {
#         be_pushvalue(vm, 2);
#         status = be_isderived(vm, 1);
#     }
#     be_pushbool(vm, status);
#     be_return(vm);
# }
def _issubv(vm, filter_fn):
    be_api = _lazy_be_api()
    status = False
    if be_api.be_top(vm) >= 2 and filter_fn(vm, 1):
        be_api.be_pushvalue(vm, 2)
        status = be_api.be_isderived(vm, 1)
    be_api.be_pushbool(vm, status)
    return be_api.be_returnvalue(vm)

# ============================================================================
# be_baselib_issubclass
# ============================================================================

# int be_baselib_issubclass(bvm *vm)
# {
#     return _issubv(vm, be_isclass);
# }
def be_baselib_issubclass(vm):
    be_api = _lazy_be_api()
    return _issubv(vm, be_api.be_isclass)

# ============================================================================
# be_baselib_isinstance
# ============================================================================

# int be_baselib_isinstance(bvm *vm)
# {
#     return _issubv(vm, be_isinstance);
# }
def be_baselib_isinstance(vm):
    be_api = _lazy_be_api()
    return _issubv(vm, be_api.be_isinstance)


# ============================================================================
# be_load_baselib  /  be_load_baselib_next
# ============================================================================

# void be_load_baselib(bvm *vm)
# {
#     be_regfunc(vm, "assert", be_baselib_assert);
#     be_regfunc(vm, "print", be_baselib_print);
#     be_regfunc(vm, "input", be_baselib_input);
#     be_regfunc(vm, "super", be_baselib_super);
#     be_regfunc(vm, "type", be_baselib_type);
#     be_regfunc(vm, "classname", be_baselib_classname);
#     be_regfunc(vm, "classof", be_baselib_classof);
#     be_regfunc(vm, "number", be_baselib_number);
#     be_regfunc(vm, "str", be_baselib_str);
#     be_regfunc(vm, "int", be_baselib_int);
#     be_regfunc(vm, "real", be_baselib_real);
#     be_regfunc(vm, "module", be_baselib_module);
#     be_regfunc(vm, "size", be_baselib_size);
#     be_regfunc(vm, "compile", be_baselib_compile);
#     be_regfunc(vm, "issubclass", be_baselib_issubclass);
#     be_regfunc(vm, "isinstance", be_baselib_isinstance);
#     be_regfunc(vm, "__iterator__", be_baselib_iterator);
# }
def be_load_baselib(vm):
    """Register all base library functions as builtins.

    We first build the builtin name→index map and value vector (equivalent
    to the C precompiled m_builtin_map / m_builtin_vector), then call
    be_const_builtin_set.  After that,
    be_regfunc / be_regclass can locate each name in the builtin range and
    fill in the actual function / class value.
    """
    from berry_port import be_var as _be_var
    from berry_port import be_string as _be_str
    from berry_port import be_map as _be_map
    from berry_port.be_object import bvalue, bvector as bvector_cls, var_setnil, var_setint

    # -- Ordered builtin names (must match the C __vlist_array order) --------
    _BUILTIN_NAMES = [
        "assert",        # 0
        "print",         # 1
        "input",         # 2
        "super",         # 3
        "type",          # 4
        "classname",     # 5
        "classof",       # 6
        "number",        # 7
        "str",           # 8
        "int",           # 9
        "real",          # 10
        "module",        # 11
        "size",          # 12
        "compile",       # 13
        "issubclass",    # 14
        "isinstance",    # 15
        "__iterator__",  # 16
        "open",          # 17  (file lib — placeholder)
        "list",          # 18
        "map",           # 19
        "range",         # 20
        "bytes",         # 21
        "call",          # 22
        "bool",          # 23
        "format",        # 24
    ]

    # -- Build the builtin map (name → index) --------------------------------
    blt_map = _be_map.be_map_new(vm)
    for idx, name in enumerate(_BUILTIN_NAMES):
        s = _be_str.be_newstr(vm, name)
        desc = _be_map.be_map_insertstr(vm, blt_map, s, None)
        var_setint(desc, idx)

    # -- Build the builtin vector (values, initially nil) --------------------
    blt_vec = bvector_cls()
    blt_vec.data = [bvalue() for _ in range(len(_BUILTIN_NAMES))]
    for v in blt_vec.data:
        var_setnil(v)
    blt_vec.count = len(_BUILTIN_NAMES)
    blt_vec.capacity = len(_BUILTIN_NAMES)
    blt_vec.end = len(_BUILTIN_NAMES)
    blt_vec.size = 1

    # -- Install into the VM -------------------------------------------------
    # Fix the builtin map so GC won't collect it.
    # In C this is a compile-time const object; in Python we must pin it.
    from berry_port import be_gc as _be_gc
    from berry_port.be_object import gc_object
    _be_gc.be_gc_fix(vm, gc_object(blt_map))
    _be_var.be_const_builtin_set(vm, blt_map, blt_vec)

    # -- Now register the actual functions (be_regfunc will find them) --------
    be_api = _lazy_be_api()
    be_api.be_regfunc(vm, "assert", be_baselib_assert)
    be_api.be_regfunc(vm, "print", be_baselib_print)
    be_api.be_regfunc(vm, "input", be_baselib_input)
    be_api.be_regfunc(vm, "super", be_baselib_super)
    be_api.be_regfunc(vm, "type", be_baselib_type)
    be_api.be_regfunc(vm, "classname", be_baselib_classname)
    be_api.be_regfunc(vm, "classof", be_baselib_classof)
    be_api.be_regfunc(vm, "number", be_baselib_number)
    be_api.be_regfunc(vm, "str", be_baselib_str)
    be_api.be_regfunc(vm, "int", be_baselib_int)
    be_api.be_regfunc(vm, "real", be_baselib_real)
    be_api.be_regfunc(vm, "module", be_baselib_module)
    be_api.be_regfunc(vm, "size", be_baselib_size)
    be_api.be_regfunc(vm, "compile", be_baselib_compile)
    be_api.be_regfunc(vm, "issubclass", be_baselib_issubclass)
    be_api.be_regfunc(vm, "isinstance", be_baselib_isinstance)
    be_api.be_regfunc(vm, "__iterator__", be_baselib_iterator)

# /* call must be added later to respect order of builtins */
# void be_load_baselib_next(bvm *vm)
# {
#     be_regfunc(vm, "call", l_call);
#     be_regfunc(vm, "bool", l_bool);
#     be_regfunc(vm, "format", be_str_format);
# }
def be_load_baselib_next(vm):
    """Register call, bool, format — must be added after classes to respect
    builtin ordering."""
    be_api = _lazy_be_api()
    be_strlib = _lazy_be_strlib()
    be_api.be_regfunc(vm, "call", l_call)
    be_api.be_regfunc(vm, "bool", l_bool)
    be_api.be_regfunc(vm, "format", be_strlib.be_str_format)
