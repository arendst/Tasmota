"""
Berry class and instance system.
Mirrors: src/be_class.c / src/be_class.h

Implements class creation, member binding (variables, methods, native methods,
closure methods), instance creation with single-inheritance super/sub chains,
and member lookup/set with virtual member support.
"""

# /********************************************************************
# ** Copyright (c) 2018-2020 Guan Wenliang
# ** This file is part of the Berry default interpreter.
# ** skiars@qq.com, https://github.com/Skiars/berry
# ** See Copyright Notice in the LICENSE file or at
# ** https://github.com/Skiars/berry/blob/master/LICENSE
# ********************************************************************/

from berry_port.be_object import (
    BE_CLASS, BE_INSTANCE, BE_NONE, BE_NIL, BE_BOOL, BE_FUNCTION, BE_MODULE,
    MT_VARIABLE, MT_METHOD, MT_PRIMMETHOD,
    bclass, binstance, bvalue, bclosure,
    var_type, var_basetype, var_primetype,
    var_istype, var_isproto, var_isclosure, var_isfunction,
    var_setnil, var_setval, var_setclass, var_setinstance, var_setstr,
    var_setclosure, var_setntvfunc,
    var_toint, var_toobj, var_tobool,
    var_markstatic, var_clearstatic,
    cast_class, cast_instance,
    gc_isconst,
    basetype,
    be_map_iter,
)
from berry_port.be_map import (
    be_map_new, be_map_findstr, be_map_insertstr, be_map_next, be_map_compact,
)
from berry_port.be_string import be_newstrn


# ============================================================================
# Internal helpers
# ============================================================================

# #define check_members(vm, c)
# if (!(c)->members) {
#     (c)->members = be_map_new(vm);
# }
def _check_members(vm, c):
    """Ensure the class has a members map, creating one if needed."""
    if c.members is None:
        c.members = be_map_new(vm)


# ============================================================================
# be_newclass
# ============================================================================

# bclass* be_newclass(bvm *vm, bstring *name, bclass *super)
# {
#     bgcobject *gco = be_gcnew(vm, BE_CLASS, bclass);
#     bclass *obj = cast_class(gco);
#     bvalue *buf = be_incrtop(vm); /* protect new objects from GC */
#     var_setclass(buf, obj);
#     if (obj) {
#         obj->super = super;
#         obj->members = NULL; /* gc protection */
#         obj->nvar = 0;
#         obj->name = name;
#     }
#     be_stackpop(vm, 1);
#     return obj;
# }
def be_newclass(vm, name, super_=None):
    """Create a new class with the given name and optional superclass.

    In C, the new class is temporarily pushed onto the VM stack to protect
    it from GC. In Python we register it on the GC list directly.
    """
    obj = bclass()
    obj.type = BE_CLASS

    obj.super = super_
    obj.members = None  # gc protection
    obj.nvar = 0
    obj.name = name
    return obj


# ============================================================================
# be_class_compress
# ============================================================================

# void be_class_compress(bvm *vm, bclass *c)
# {
#     if (!gc_isconst(c) && c->members) {
#         be_map_compact(vm, c->members); /* clear space */
#     }
# }
def be_class_compress(vm, c):
    """Compact the class members map to remove unused slots."""
    if not gc_isconst(c) and c.members is not None:
        be_map_compact(vm, c.members)


# ============================================================================
# be_class_attribute
# ============================================================================

# /* Return the type of the class attribute, only used to check if the attribute already exists */
# int be_class_attribute(bvm *vm, bclass *c, bstring *attr)
# {
#     for (; c; c = c->super) {
#         if (c->members) {
#             bvalue *v = be_map_findstr(vm, c->members, attr);
#             if (v) {
#                 return var_type(v);
#             }
#         }
#     }
#     return BE_NONE;
# }
def be_class_attribute(vm, c, attr):
    """Return the type of a class attribute, walking the superclass chain.

    Returns BE_NONE if the attribute is not found in any class in the
    hierarchy. Used to check if an attribute already exists.
    """
    while c is not None:
        if c.members is not None:
            v = be_map_findstr(vm, c.members, attr)
            if v is not None:
                return var_type(v)
        c = c.super
    return BE_NONE


# ============================================================================
# be_class_member_bind
# ============================================================================

# void be_class_member_bind(bvm *vm, bclass *c, bstring *name, bbool var)
# {
#     bvalue *attr;
#     set_fixed(name);
#     check_members(vm, c);
#     attr = be_map_insertstr(vm, c->members, name, NULL);
#     restore_fixed(name);
#     if (var) {
#         /* this is an instance variable so we set it as MT_VARIABLE */
#         attr->v.i = c->nvar++;
#         attr->type = MT_VARIABLE;
#     } else {
#         /* this is a static class constant, leave it as BE_NIL */
#         attr->v.i = 0;
#         attr->type = BE_NIL;
#     }
# }
def be_class_member_bind(vm, c, name, var):
    """Bind a member variable or static nil slot to a class.

    If *var* is true, the member is an instance variable (MT_VARIABLE)
    and its value is the variable index (c.nvar, auto-incremented).
    If *var* is false, the member is a static class constant initialized
    to BE_NIL.
    """
    # set_fixed / restore_fixed — no-op in Python port
    _check_members(vm, c)
    attr = be_map_insertstr(vm, c.members, name, None)
    if var:
        attr.v = c.nvar
        attr.type = MT_VARIABLE
        c.nvar += 1
    else:
        attr.v = 0
        attr.type = BE_NIL


# ============================================================================
# be_class_method_bind
# ============================================================================

# void be_class_method_bind(bvm *vm, bclass *c, bstring *name, bproto *p, bbool is_static)
# {
#     bclosure *cl;
#     bvalue *attr;
#     set_fixed(name);
#     check_members(vm, c);
#     attr = be_map_insertstr(vm, c->members, name, NULL);
#     restore_fixed(name);
#     var_setnil(attr);
#     cl = be_newclosure(vm, p->nupvals);
#     cl->proto = p;
#     var_setclosure(attr, cl);
#     if (is_static) {
#         var_markstatic(attr);
#     }
# }
def be_class_method_bind(vm, c, name, p, is_static):
    """Bind a Berry closure method to a class from a prototype.

    Creates a new bclosure wrapping the prototype *p* and inserts it
    into the class members map. Marks the method as static if requested.

    Note: be_newclosure is not yet available (be_func.py task 3.9),
    so we create the closure object inline for now.
    """
    # set_fixed / restore_fixed — no-op in Python port
    _check_members(vm, c)
    attr = be_map_insertstr(vm, c.members, name, None)
    var_setnil(attr)
    # Create closure (inline until be_func.py is available)
    cl = bclosure()
    cl.type = (1 << 5) | 6  # BE_CLOSURE
    cl.nupvals = p.nupvals
    cl.proto = p
    cl.upvals = [None] * p.nupvals
    var_setclosure(attr, cl)
    if is_static:
        var_markstatic(attr)


# ============================================================================
# be_class_native_method_bind
# ============================================================================

# void be_class_native_method_bind(bvm *vm, bclass *c, bstring *name, bntvfunc f)
# {
#     bvalue *attr;
#     set_fixed(name);
#     check_members(vm, c);
#     attr = be_map_insertstr(vm, c->members, name, NULL);
#     restore_fixed(name);
#     attr->v.nf = f;
#     attr->type = MT_PRIMMETHOD;
# }
def be_class_native_method_bind(vm, c, name, f):
    """Bind a native (Python callable) method to a class."""
    # set_fixed / restore_fixed — no-op in Python port
    _check_members(vm, c)
    attr = be_map_insertstr(vm, c.members, name, None)
    attr.v = f
    attr.type = MT_PRIMMETHOD


# ============================================================================
# be_class_closure_method_bind
# ============================================================================

# void be_class_closure_method_bind(bvm *vm, bclass *c, bstring *name, bclosure *cl)
# {
#     bvalue *attr;
#     check_members(vm, c);
#     attr = be_map_insertstr(vm, c->members, name, NULL);
#     attr->v.gc = (bgcobject*) cl;
#     attr->type = MT_METHOD;
# }
def be_class_closure_method_bind(vm, c, name, cl):
    """Bind an existing closure object as a method on a class."""
    _check_members(vm, c)
    attr = be_map_insertstr(vm, c.members, name, None)
    attr.v = cl
    attr.type = MT_METHOD


# ============================================================================
# be_class_closure_count
# ============================================================================

# /* get the closure method count that need upvalues */
# int be_class_closure_count(bclass *c)
# {
#     int count = 0;
#     if (c->members) {
#         bmapnode *node;
#         bmap *members = c->members;
#         bmapiter iter = be_map_iter();
#         while ((node = be_map_next(members, &iter)) != NULL) {
#             if (var_isproto(&node->value)) {
#                 ++count;
#             }
#         }
#     }
#     return count;
# }
def be_class_closure_count(c):
    """Count the number of proto (not yet wrapped as closure) methods in a class.

    This counts members whose value type is BE_PROTO, which are methods
    that still need to be wrapped in closures with upvalue initialization.
    """
    count = 0
    if c.members is not None:
        members = c.members
        iter_ref = [be_map_iter()]
        while True:
            node, iter_ref = be_map_next(members, iter_ref)
            if node is None:
                break
            if var_isproto(node.value):
                count += 1
    return count


# ============================================================================
# Internal: instance_member (static in C)
# ============================================================================

# static binstance* instance_member(bvm *vm,
#     binstance *obj, bstring *name, bvalue *dst)
# {
#     for (; obj; obj = obj->super) {
#         bmap *members = obj->_class->members;
#         if (members) {
#             bvalue *v = be_map_findstr(vm, members, name);
#             if (v) {
#                 *dst = *v;
#                 return obj;
#             }
#         }
#     }
#     var_setnil(dst);
#     return NULL;
# }
def _instance_member(vm, obj, name, dst):
    """Walk the instance super chain looking for a member by name.

    Copies the raw member value (which may be MT_VARIABLE index) into *dst*.
    Returns the instance that owns the member, or None if not found.
    """
    while obj is not None:
        members = obj._class.members
        if members is not None:
            v = be_map_findstr(vm, members, name)
            if v is not None:
                dst.type = v.type
                dst.v = v.v
                return obj
        obj = obj.super
    var_setnil(dst)
    return None


# ============================================================================
# Internal: class_member (static in C)
# ============================================================================

# static bclass* class_member(bvm *vm,
#     bclass *obj, bstring *name, bvalue *dst)
# {
#     for (; obj; obj = obj->super) {
#         bmap *members = obj->members;
#         if (members) {
#             bvalue *v = be_map_findstr(vm, members, name);
#             if (v) {
#                 *dst = *v;
#                 return obj;
#             }
#         }
#     }
#     var_setnil(dst);
#     return NULL;
# }
def _class_member(vm, obj, name, dst):
    """Walk the class super chain looking for a member by name.

    Copies the raw member value into *dst*.
    Returns the class that owns the member, or None if not found.
    """
    while obj is not None:
        members = obj.members
        if members is not None:
            v = be_map_findstr(vm, members, name)
            if v is not None:
                dst.type = v.type
                dst.v = v.v
                return obj
        obj = obj.super
    var_setnil(dst)
    return None


# ============================================================================
# be_class_upvalue_init
# ============================================================================

# void be_class_upvalue_init(bvm *vm, bclass *c)
# {
#     bmap *mbr = c->members;
#     if (mbr != NULL) {
#         bmapnode *node;
#         bmapiter iter = be_map_iter();
#         while ((node = be_map_next(mbr, &iter)) != NULL) {
#             if (var_isclosure(&node->value)) {
#                 bclosure *cl = var_toobj(&node->value);
#                 if (cl->proto->nupvals) {
#                     /* initialize the closure's upvalues */
#                     be_release_upvalues(vm, cl);
#                     be_initupvals(vm, cl);
#                 }
#             }
#         }
#     }
# }
def be_class_upvalue_init(vm, c):
    """Initialize upvalues for all closure methods in a class.

    Iterates over the class members map and for each closure with
    upvalues, releases any existing upvalues and re-initializes them.

    Note: be_release_upvalues and be_initupvals are in be_func.py
    (task 3.9). This function will call them once available; for now
    it performs the iteration and delegates to the func module.
    """
    mbr = c.members
    if mbr is not None:
        iter_ref = [be_map_iter()]
        while True:
            node, iter_ref = be_map_next(mbr, iter_ref)
            if node is None:
                break
            if var_isclosure(node.value):
                cl = var_toobj(node.value)
                if cl.proto.nupvals:
                    # Defer to be_func.py when available
                    try:
                        from berry_port.be_func import be_release_upvalues, be_initupvals
                        be_release_upvalues(vm, cl)
                        be_initupvals(vm, cl)
                    except ImportError:
                        pass  # be_func.py not yet implemented


# ============================================================================
# Internal: newobjself (static in C)
# ============================================================================

# /* (internal) Instanciate an instance for a single class and initialize variables to nil */
# static binstance* newobjself(bvm *vm, bclass *c)
# {
#     size_t size = sizeof(binstance) + sizeof(bvalue) * (c->nvar - 1);
#     bgcobject *gco = be_newgcobj(vm, BE_INSTANCE, size);
#     binstance *obj = cast_instance(gco);
#     be_assert(obj != NULL);
#     if (obj) { /* initialize members */
#         bvalue *v = obj->members, *end = v + c->nvar;
#         while (v < end) { var_setnil(v); ++v; }
#         obj->_class = c;
#         obj->super = NULL;
#         obj->sub = NULL;
#     }
#     return obj;
# }
def _newobjself(vm, c):
    """Create a single instance for class *c* with all member variables set to nil."""
    obj = binstance()
    obj.type = BE_INSTANCE

    # Initialize member variable slots to nil
    obj.members = [bvalue() for _ in range(c.nvar)]
    obj._class = c
    obj.super = None
    obj.sub = None
    return obj


# ============================================================================
# Internal: newobject (static in C)
# ============================================================================

# /* (internal) Instanciate the whole chain of instances when there is a class hierarchy */
# /* All variables set to nil, constructors are not called here */
# static binstance* newobject(bvm *vm, bclass *c)
# {
#     binstance *obj, *prev;
#     be_assert(c != NULL);
#     obj = prev = newobjself(vm, c);
#     var_setinstance(vm->top, obj);
#     be_incrtop(vm); /* protect new objects from GC */
#     for (c = c->super; c; c = c->super) {
#         prev->super = newobjself(vm, c);
#         prev->super->sub = prev;
#         prev = prev->super;
#     }
#     be_stackpop(vm, 1);
#     return obj;
# }
def _newobject(vm, c):
    """Create the full instance chain for a class hierarchy.

    Creates one binstance per class in the inheritance chain, linked
    via super/sub pointers. All member variables are initialized to nil.
    Constructors are NOT called here.
    """
    assert c is not None
    obj = _newobjself(vm, c)
    prev = obj
    c = c.super
    while c is not None:
        prev.super = _newobjself(vm, c)
        prev.super.sub = prev
        prev = prev.super
        c = c.super
    return obj


# ============================================================================
# be_class_newobj
# ============================================================================

# /* Instanciate new instance from stack with argc parameters */
# /* Pushes the constructor on the stack to be executed if a constructor is found */
# /* Returns true if a constructor is found */
# bbool be_class_newobj(bvm *vm, bclass *c, int pos, int argc, int mode)
# {
#     bvalue init;
#     binstance *obj = newobject(vm, c);
#     var_setinstance(vm->reg + pos, obj);
#     var_setinstance(vm->reg + pos - mode, obj);
#     /* find constructor */
#     obj = instance_member(vm, obj, str_literal(vm, "init"), &init);
#     if (obj && var_type(&init) != MT_VARIABLE) {
#         /* copy argv */
#         bvalue * reg;
#         for (reg = vm->reg + pos + 1; argc > 0; --argc) {
#             reg[argc] = reg[argc - 2];
#         }
#         *reg = init; /* set constructor */
#         return btrue;
#     }
#     return bfalse;
# }
def be_class_newobj(vm, c, pos, argc, mode):
    """Instantiate a new object from class *c* and set up the constructor call.

    Creates the instance hierarchy, writes it into the stack at the
    appropriate positions, and looks up the 'init' constructor. If found,
    shifts the arguments and places the constructor on the stack so the
    caller can execute it.

    Returns True if a constructor was found, False otherwise.
    """
    init = bvalue()
    obj = _newobject(vm, c)

    # reg = vm->reg + pos
    reg_idx = vm.reg_idx + pos
    var_setinstance(vm.stack[reg_idx], obj)
    var_setinstance(vm.stack[reg_idx - mode], obj)

    # Find constructor 'init'
    init_name = be_newstrn(vm, "init", 4)
    obj = _instance_member(vm, obj, init_name, init)
    if obj is not None and var_type(init) != MT_VARIABLE:
        # Copy argv: shift arguments up by 2 to make room for constructor
        base_idx = reg_idx + 1
        for i in range(argc, 0, -1):
            var_setval(vm.stack[base_idx + i], vm.stack[base_idx + i - 2])
        # Set constructor at base
        var_setval(vm.stack[base_idx], init)
        return True
    return False


# ============================================================================
# be_instance_member_simple
# ============================================================================

# /* Find instance member by name and copy value to `dst` */
# /* Do not look into virtual members */
# int be_instance_member_simple(bvm *vm, binstance *instance, bstring *name, bvalue *dst)
# {
#     int type;
#     be_assert(name != NULL);
#     binstance * obj = instance_member(vm, instance, name, dst);
#     if (obj && var_type(dst) == MT_VARIABLE) {
#         *dst = obj->members[dst->v.i];
#     }
#     type = var_type(dst);
#     var_clearstatic(dst);
#     return type;
# }
def be_instance_member_simple(vm, instance, name, dst):
    """Find an instance member by name (no virtual member lookup).

    If the member is an instance variable (MT_VARIABLE), resolves it
    to the actual value from the instance's members array.
    Returns the type of the resolved member, or BE_NONE via nil.
    """
    assert name is not None
    obj = _instance_member(vm, instance, name, dst)
    if obj is not None and var_type(dst) == MT_VARIABLE:
        idx = var_toint(dst)
        src = obj.members[idx]
        dst.type = src.type
        dst.v = src.v
    type_ = var_type(dst)
    var_clearstatic(dst)
    return type_


# ============================================================================
# be_instance_member
# ============================================================================

# /* Find instance member by name and copy value to `dst` */
# /* Input: none of `obj`, `name` and `dst` may not be NULL */
# /* Returns the type of the member or BE_NONE if member not found */
# int be_instance_member(bvm *vm, binstance *instance, bstring *name, bvalue *dst)
# {
#     int type;
#     be_assert(name != NULL);
#     binstance *obj = instance_member(vm, instance, name, dst);
#     if (obj && var_type(dst) == MT_VARIABLE) {
#         *dst = obj->members[dst->v.i];
#     }
#     type = var_type(dst);
#     if (obj) {
#         var_clearstatic(dst);
#         return type;
#     } else {  /* if no method found, try virtual */
#         /* if 'init' does not exist, create a virtual empty constructor */
#         if (strcmp(str(name), "init") == 0) {
#             var_setntvfunc(dst, be_default_init_native_function);
#             return var_primetype(dst);
#         } else {
#             /* get method 'member' */
#             obj = instance_member(vm, instance, str_literal(vm, "member"), vm->top);
#             if (obj && basetype(var_type(vm->top)) == BE_FUNCTION) {
#                 bvalue *top = vm->top;
#                 var_setinstance(&top[1], instance);
#                 var_setstr(&top[2], name);
#                 vm->top += 3;   /* prevent gc collection results */
#                 be_dofunc(vm, top, 2); /* call method 'member' */
#                 vm->top -= 3;
#                 *dst = *vm->top;   /* copy result to R(A) */
#                 if (obj && var_type(dst) == MT_VARIABLE) {
#                     *dst = obj->members[dst->v.i];
#                 }
#                 type = var_type(dst);
#                 if (type == BE_MODULE) {
#                     bmodule *mod = var_toobj(dst);
#                     if (strcmp(be_module_name(mod), "undefined") == 0) {
#                         return BE_NONE;
#                     }
#                 }
#                     var_clearstatic(dst);
#                     return type;
#             }
#         }
#     }
#     return BE_NONE;
# }
def be_instance_member(vm, instance, name, dst):
    """Find an instance member by name, with virtual member support.

    Lookup order:
    1. Direct member lookup through the instance super chain.
    2. If not found and name is 'init', return a default empty constructor.
    3. If not found, try calling the 'member' virtual method on the instance.
    4. If the virtual method returns module 'undefined', treat as not found.

    Returns the type of the resolved member, or BE_NONE if not found.
    """
    assert name is not None
    obj = _instance_member(vm, instance, name, dst)
    if obj is not None and var_type(dst) == MT_VARIABLE:
        idx = var_toint(dst)
        src = obj.members[idx]
        dst.type = src.type
        dst.v = src.v
    type_ = var_type(dst)
    if obj is not None:
        var_clearstatic(dst)
        return type_
    else:
        # No direct member found — try virtual
        # If 'init' does not exist, create a virtual empty constructor
        name_str = name.s if hasattr(name, 's') else ""
        if name_str == "init":
            var_setntvfunc(dst, _be_default_init_native_function)
            return var_primetype(dst)
        else:
            # Get method 'member' for virtual dispatch
            member_name = be_newstrn(vm, "member", 6)
            top_val = bvalue()
            obj = _instance_member(vm, instance, member_name, top_val)
            if obj is not None and basetype(var_type(top_val)) == BE_FUNCTION:
                # Virtual member dispatch requires be_dofunc (be_vm.py, task 7.4)
                # For now, we defer to a lazy import
                try:
                    from berry_port.be_vm import be_dofunc
                    # Set up the call: top[0]=method, top[1]=instance, top[2]=name
                    top_idx = vm.top_idx
                    vm.stack[top_idx].type = top_val.type
                    vm.stack[top_idx].v = top_val.v
                    var_setinstance(vm.stack[top_idx + 1], instance)
                    var_setstr(vm.stack[top_idx + 2], name)
                    vm.top_idx += 3
                    be_dofunc(vm, top_idx, 2)
                    vm.top_idx -= 3
                    # Copy result
                    result = vm.stack[vm.top_idx]
                    dst.type = result.type
                    dst.v = result.v
                    if obj is not None and var_type(dst) == MT_VARIABLE:
                        idx = var_toint(dst)
                        src_val = obj.members[idx]
                        dst.type = src_val.type
                        dst.v = src_val.v
                    type_ = var_type(dst)
                    if type_ == BE_MODULE:
                        mod = var_toobj(dst)
                        try:
                            from berry_port.be_module import be_module_name
                            if be_module_name(mod) == "undefined":
                                return BE_NONE
                        except ImportError:
                            pass
                    var_clearstatic(dst)
                    return type_
                except ImportError:
                    pass  # be_vm.py not yet implemented
    return BE_NONE


# Default empty constructor (mirrors be_default_init_native_function in be_vm.c)
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
def _be_default_init_native_function(vm):
    """Default empty constructor for classes without an explicit init method.

    Returns the first argument (self) if present, otherwise nil.
    This is a placeholder; the real implementation lives in be_vm.py.
    """
    # This will be replaced by be_vm.be_default_init_native_function
    # once the VM module is available. For now, return 0 (BE_OK).
    return 0


# ============================================================================
# be_class_member
# ============================================================================

# int be_class_member(bvm *vm, bclass *obj, bstring *name, bvalue *dst)
# {
#     int type;
#     be_assert(name != NULL);
#     obj = class_member(vm, obj, name, dst);
#     type = var_type(dst);
#     var_clearstatic(dst);
#     return obj ? type : BE_NONE;
# }
def be_class_member(vm, obj, name, dst):
    """Find a class member by name, walking the superclass chain.

    Returns the type of the member, or BE_NONE if not found.
    """
    assert name is not None
    found = _class_member(vm, obj, name, dst)
    type_ = var_type(dst)
    var_clearstatic(dst)
    return type_ if found is not None else BE_NONE


# ============================================================================
# be_instance_setmember
# ============================================================================

# bbool be_instance_setmember(bvm *vm, binstance *o, bstring *name, bvalue *src)
# {
#     bvalue v;
#     be_assert(name != NULL);
#     binstance * obj = instance_member(vm, o, name, &v);
#     if (obj && var_istype(&v, MT_VARIABLE)) {
#         obj->members[var_toint(&v)] = *src;
#         return btrue;
#     } else {
#         obj = instance_member(vm, o, str_literal(vm, "setmember"), &v);
#         if (obj && var_type(&v) == MT_VARIABLE) {
#             v = obj->members[v.v.i];
#         }
#         if (var_basetype(&v) == BE_FUNCTION) {
#             if (var_isfunction(src)) {
#                 var_clearstatic(src);
#             }
#             bvalue *top = vm->top;
#             var_setval(top, &v);
#             var_setinstance(top + 1, o);
#             var_setstr(top + 2, name);
#             var_setval(top + 3, src);
#             vm->top += 4;
#             be_dofunc(vm, top, 3);
#             vm->top -= 4;
#             int type = var_type(vm->top);
#             if (type == BE_BOOL) {
#                 bbool ret = var_tobool(vm->top);
#                 if (!ret) {
#                     return bfalse;
#                 }
#             } else if (type == BE_MODULE) {
#                 bmodule *mod = var_toobj(vm->top);
#                 if (strcmp(be_module_name(mod), "undefined") == 0) {
#                     return bfalse;
#                 }
#             }
#             return btrue;
#         }
#     }
#     return bfalse;
# }
def be_instance_setmember(vm, o, name, src):
    """Set an instance member by name.

    If the member is a direct instance variable (MT_VARIABLE), sets it
    directly. Otherwise, tries the 'setmember' virtual method.

    Returns True if the member was set, False otherwise.
    """
    assert name is not None
    v = bvalue()
    obj = _instance_member(vm, o, name, v)
    if obj is not None and var_istype(v, MT_VARIABLE):
        idx = var_toint(v)
        obj.members[idx].type = src.type
        obj.members[idx].v = src.v
        return True
    else:
        # Try 'setmember' virtual method
        setmember_name = be_newstrn(vm, "setmember", 9)
        obj = _instance_member(vm, o, setmember_name, v)
        if obj is not None and var_type(v) == MT_VARIABLE:
            idx = v.v
            src_val = obj.members[idx]
            v.type = src_val.type
            v.v = src_val.v
        if basetype(var_type(v)) == BE_FUNCTION:
            if var_isfunction(src):
                var_clearstatic(src)
            # Virtual setmember dispatch requires be_dofunc (be_vm.py, task 7.4)
            try:
                from berry_port.be_vm import be_dofunc
                top_idx = vm.top_idx
                var_setval(vm.stack[top_idx], v)
                var_setinstance(vm.stack[top_idx + 1], o)
                var_setstr(vm.stack[top_idx + 2], name)
                var_setval(vm.stack[top_idx + 3], src)
                vm.top_idx += 4
                be_dofunc(vm, top_idx, 3)
                vm.top_idx -= 4
                result = vm.stack[vm.top_idx]
                type_ = var_type(result)
                if type_ == BE_BOOL:
                    if not var_tobool(result):
                        return False
                elif type_ == BE_MODULE:
                    mod = var_toobj(result)
                    try:
                        from berry_port.be_module import be_module_name
                        if be_module_name(mod) == "undefined":
                            return False
                    except ImportError:
                        pass
                return True
            except ImportError:
                pass  # be_vm.py not yet implemented
    return False


# ============================================================================
# be_class_setmember
# ============================================================================

# bbool be_class_setmember(bvm *vm, bclass *o, bstring *name, bvalue *src)
# {
#     bvalue v;
#     be_assert(name != NULL);
#     if (!gc_isconst(o)) {
#         bclass * obj = class_member(vm, o, name, &v);
#         if (obj && !var_istype(&v, MT_VARIABLE)) {
#             be_map_insertstr(vm, obj->members, name, src);
#             return btrue;
#         }
#     }
#     return bfalse;
# }
def be_class_setmember(vm, o, name, src):
    """Set a class-level member by name.

    Only works on non-const classes and only for non-variable members
    (i.e., methods and static members, not instance variable slots).

    Returns True if the member was set, False otherwise.
    """
    assert name is not None
    if not gc_isconst(o):
        v = bvalue()
        obj = _class_member(vm, o, name, v)
        if obj is not None and not var_istype(v, MT_VARIABLE):
            be_map_insertstr(vm, obj.members, name, src)
            return True
    return False
