"""
Berry closure and upvalue management.
Mirrors: src/be_func.c / src/be_func.h

Implements closure creation, upvalue initialization and lifecycle (open/close),
prototype allocation, and debug variable name lookup. The open upvalue list
(vm.upvalist) is a singly-linked list sorted by stack level, enabling
efficient sharing of upvalues across closures that capture the same variable.
"""

# /********************************************************************
# ** Copyright (c) 2018-2020 Guan Wenliang
# ** This file is part of the Berry default interpreter.
# ** skiars@qq.com, https://github.com/Skiars/berry
# ** See Copyright Notice in the LICENSE file or at
# ** https://github.com/Skiars/berry/blob/master/LICENSE
# ********************************************************************/

from berry_port.be_object import (
    BE_PROTO, BE_CLOSURE, BE_NTVCLOS,
    bproto, bclosure, bntvclos, bupval, bvalue,
    var_setnil, var_setntvclos, var_toobj,
    cast_closure,
)
from berry_port.be_mem import be_malloc, be_free
from berry_port.berry_conf import (
    BE_DEBUG_SOURCE_FILE,
    BE_DEBUG_RUNTIME_INFO,
    BE_DEBUG_VAR_INFO,
)


# ============================================================================
# Macros from be_func.h
# ============================================================================

# #define clousersize(n) \
#     (sizeof(bclosure) + sizeof(bupval*) * ((size_t)(n) - 1))
# (Not needed in Python — bclosure.upvals is a Python list.)

# #define be_ntvclos_upval(cc, n) \
#     (((bupval**)((size_t)cc + sizeof(bntvclos)))[n])
def be_ntvclos_upval(cc, n):
    """Access the n-th upvalue pointer of a native closure.

    In C this is pointer arithmetic past the bntvclos struct.
    In Python, upvals is a list on the bntvclos object.
    """
    # #define be_ntvclos_upval(cc, n) \
    #     (((bupval**)((size_t)cc + sizeof(bntvclos)))[n])
    return cc.upvals[n]


# ============================================================================
# Internal: findupval (static in C)
# ============================================================================

# static bupval* findupval(bvm *vm, bvalue *level)
# {
#     bupval *node = vm->upvalist;
#     while (node != NULL && node->value > level) {
#         node = node->u.next;
#     }
#     if (!node || node->value != level) {
#         /* not found */
#         node = be_malloc(vm, sizeof(bupval));
#         node->value = level;
#         node->refcnt = 0;
#         /* insert to list head */
#         node->u.next = vm->upvalist;
#         vm->upvalist = node;
#     }
#     return node;
# }
def _findupval(vm, level):
    """Find or create an open upvalue for the stack slot at *level*.

    The open upvalue list (vm.upvalist) is a singly-linked list.
    In the C implementation, upvalues are sorted by stack pointer
    (descending) and the search walks until node->value <= level.

    In the Python port, *level* is an integer index into vm.stack.
    The list is walked via u_next links. If no matching upvalue is
    found, a new one is created and inserted at the list head.
    """
    node = vm.upvalist
    while node is not None and node.value > level:
        node = node.u_next

    if node is None or node.value != level:
        # Not found — create a new upvalue
        new_uv = bupval()
        new_uv.value = level
        new_uv.refcnt = 0
        # Insert at list head
        new_uv.u_next = vm.upvalist
        vm.upvalist = new_uv
        node = new_uv

    return node


# ============================================================================
# be_initupvals
# ============================================================================

# void be_initupvals(bvm *vm, bclosure *cl)
# {
#     int count = cl->proto->nupvals;
#     bupvaldesc *desc = cl->proto->upvals;
#     bvalue *stack = vm->reg;
#     bupval **uv = cl->upvals;
#     bupval **superuv = cast(bclosure*, var_toobj(vm->cf->func))->upvals;
#     for (; count--; desc++, uv++) {
#         if (desc->instack) {
#             bvalue *ref = stack + desc->idx;
#             *uv = findupval(vm, ref);
#         } else {
#             *uv = superuv[desc->idx];
#         }
#         (*uv)->refcnt++;
#     }
# }
def be_initupvals(vm, cl):
    """Initialize upvalues for a newly created closure.

    For each upvalue descriptor in the closure's prototype:
    - If instack: the upvalue captures a local in the current frame,
      so find/create an open upvalue pointing to that stack slot.
    - Otherwise: the upvalue is inherited from the enclosing closure's
      upvalue at the given index.

    Each resolved upvalue gets its refcnt incremented.
    """
    count = cl.proto.nupvals
    descs = cl.proto.upvals       # list of bupvaldesc
    reg_idx = vm.reg_idx          # base register index for current frame
    # The enclosing closure is the function value of the current call frame
    super_cl = cast_closure(var_toobj(vm.stack[vm.cf.func]))
    superuv = super_cl.upvals

    # Ensure cl.upvals has enough slots
    while len(cl.upvals) < count:
        cl.upvals.append(None)

    for i in range(count):
        desc = descs[i]
        if desc.instack:
            # Capture a local variable from the current stack frame
            ref = reg_idx + desc.idx
            cl.upvals[i] = _findupval(vm, ref)
        else:
            # Inherit from enclosing closure
            cl.upvals[i] = superuv[desc.idx]
        cl.upvals[i].refcnt += 1


# ============================================================================
# be_upvals_close
# ============================================================================

# void be_upvals_close(bvm *vm, bvalue *level)
# {
#     bupval *node = vm->upvalist, *next;
#     bupval **prev = &vm->upvalist;
#     while (node) {
#         next = node->u.next;
#         if (node->value >= level) {
#             if (!node->refcnt) {
#                 be_free(vm, node, sizeof(bupval));
#             } else {
#                 node->u.value = *node->value; /* move value to upvalue slot */
#                 node->value = &node->u.value;
#             }
#             *prev = next;   /* remove from linked list */
#         } else {
#             prev = &node->u.next;
#         }
#         node = next;
#     }
# }
def be_upvals_close(vm, level):
    """Close all open upvalues at or above *level*.

    *level* is an integer stack index. For each upvalue whose stack
    reference >= level:
    - If refcnt == 0: the upvalue is unreferenced, discard it.
    - Otherwise: copy the stack value into the upvalue's internal
      storage (u_value) and redirect value to point to u_value
      (i.e., mark it as "closed").

    The upvalue is removed from the open list in either case.
    """
    # We rebuild the linked list, skipping closed/freed nodes
    node = vm.upvalist
    # Use a sentinel approach: track the "previous node" for relinking
    # Since Python doesn't have pointer-to-pointer, we handle head specially
    new_head = None
    tail = None

    while node is not None:
        next_node = node.u_next
        if node.value >= level:
            # This upvalue is at or above the closing level
            if not node.refcnt:
                # No references — just discard
                pass
            else:
                # Close: copy stack value into internal storage, redirect
                node.u_value = bvalue()
                # Copy the actual value from the stack
                src = vm.stack[node.value]
                node.u_value.type = src.type
                node.u_value.v = src.v
                # Mark as closed: set value to None (no longer a stack index)
                node.value = None
            # Remove from linked list (don't link into new list)
            # But if closed (refcnt > 0), the upvalue still exists,
            # just not in the open list
        else:
            # Keep in the open list
            if new_head is None:
                new_head = node
                tail = node
            else:
                tail.u_next = node
                tail = node
            tail.u_next = None
        node = next_node

    vm.upvalist = new_head


# ============================================================================
# be_release_upvalues
# ============================================================================

# void be_release_upvalues(bvm *vm, bclosure *cl)
# {
#     int i, count = cl->nupvals;
#     for (i = 0; i < count; ++i) {
#         bupval *uv = cl->upvals[i];
#         if (uv) {
#             if (uv->refcnt) {
#                 --uv->refcnt;
#             }
#             /* delete non-referenced closed upvalue */
#             if (uv->value == &uv->u.value && !uv->refcnt) {
#                 be_free(vm, uv, sizeof(bupval));
#             }
#         }
#     }
# }
def be_release_upvalues(vm, cl):
    """Decrement refcnt on all upvalues held by closure *cl*.

    If an upvalue is closed (value points to internal storage) and its
    refcnt drops to zero, it is freed.
    """
    count = cl.nupvals
    for i in range(count):
        if i < len(cl.upvals):
            uv = cl.upvals[i]
        else:
            uv = None
        if uv is not None:
            if uv.refcnt:
                uv.refcnt -= 1
            # Check if closed (self-reference sentinel) and unreferenced
            if _is_closed(uv) and not uv.refcnt:
                # In Python, just let GC reclaim it
                cl.upvals[i] = None


def _is_closed(uv):
    """Check whether an upvalue is closed.

    In C: uv->value == &uv->u.value (pointer to internal storage).
    In Python: closed upvalues have value set to None (no longer a stack index).
    """
    return uv.value is None


def upval_value(uv):
    """Read the current value of an upvalue.

    If open, reads from the VM stack (caller must provide vm).
    If closed, reads from the internal u_value storage.

    This is a convenience for code that needs to dereference an upvalue.
    For open upvalues, the caller should use vm.stack[uv.value] directly.
    For closed upvalues, use uv.u_value.
    """
    if _is_closed(uv):
        return uv.u_value
    # For open upvalues, value is a stack index — caller needs vm.stack
    return None  # caller must use vm.stack[uv.value]


# ============================================================================
# be_newproto
# ============================================================================

# bproto* be_newproto(bvm *vm)
# {
#     bgcobject *gco = be_gcnew(vm, BE_PROTO, bproto);
#     bproto *p = cast_proto(gco);
#     if (p) {
#         p->upvals = NULL;
#         p->ktab = NULL;
#         p->ptab = NULL;
#         p->code = NULL;
#         p->name = NULL;
#         p->gray = NULL;
#         p->codesize = 0;
#         p->nupvals = 0;
#         p->nproto = 0;
#         p->nconst = 0;
#         p->nstack = 0;
#         p->codesize = 0;
#         p->argc = 0;
#         p->varg = 0;
# #if BE_DEBUG_SOURCE_FILE
#         p->source = NULL;
# #endif
# #if BE_DEBUG_RUNTIME_INFO
#         p->lineinfo = NULL;
#         p->nlineinfo = 0;
# #endif
# #if BE_DEBUG_VAR_INFO
#         p->varinfo = NULL;
#         p->nvarinfo = 0;
# #endif
#     }
#     return p;
# }
def be_newproto(vm):
    """Allocate and initialize a new function prototype.

    All fields are zeroed/nulled. The proto is registered on the GC list.
    """
    p = bproto()
    p.type = BE_PROTO

    # Register with GC
    p.next = vm.gc.list
    vm.gc.list = p

    p.upvals = None
    p.ktab = None
    p.ptab = None
    p.code = None
    p.name = None
    p.gray = None
    p.codesize = 0
    p.nupvals = 0
    p.nproto = 0
    p.nconst = 0
    p.nstack = 0
    p.argc = 0
    p.varg = 0
    # Conditional debug fields (always present in Python)
    p.source = None
    p.lineinfo = None
    p.nlineinfo = 0
    p.varinfo = None
    p.nvarinfo = 0
    return p


# ============================================================================
# be_newclosure
# ============================================================================

# bclosure* be_newclosure(bvm *vm, int nupval)
# {
#     bgcobject *gco = be_newgcobj(vm, BE_CLOSURE, clousersize(nupval));
#     bclosure *cl = cast_closure(gco);
#     if (cl) {
#         cl->proto = NULL;
#         cl->nupvals = (bbyte)nupval;
#         while (nupval--) {
#             cl->upvals[nupval] = NULL;
#         }
#     }
#     return cl;
# }
def be_newclosure(vm, nupval):
    """Allocate a new Berry closure with *nupval* upvalue slots.

    The closure is registered on the GC list. All upvalue slots are
    initialized to None, and proto is set to None.
    """
    cl = bclosure()
    cl.type = BE_CLOSURE

    # Register with GC
    cl.next = vm.gc.list
    vm.gc.list = cl

    cl.proto = None
    cl.nupvals = nupval
    cl.upvals = [None] * nupval
    return cl


# ============================================================================
# Internal: init_upvals for native closures (static in C)
# ============================================================================

# static void init_upvals(bvm *vm, bntvclos *f)
# {
#     int count = f->nupvals;
#     bupval **upvals = &be_ntvclos_upval(f, 0);
#     while (count--) {
#         bupval *uv = be_malloc(vm, sizeof(bupval)); /* was closed */
#         uv->value = &uv->u.value;
#         uv->refcnt = 1;
#         var_setnil(uv->value);
#         *upvals++ = uv;
#     }
# }
def _init_upvals_ntv(vm, f):
    """Initialize upvalues for a native closure.

    Each upvalue is created in the "closed" state (value points to
    internal storage) with refcnt=1 and a nil value.
    """
    f.upvals = []
    for _ in range(f.nupvals):
        uv = bupval()
        # Mark as closed: value points to internal storage (u_value)
        uv.refcnt = 1
        uv.u_value = bvalue()
        var_setnil(uv.u_value)
        uv.value = uv.u_value  # closed: points to own storage
        f.upvals.append(uv)


# ============================================================================
# be_newntvclosure
# ============================================================================

# bntvclos* be_newntvclosure(bvm *vm, bntvfunc cf, int nupvals)
# {
#     size_t size = sizeof(bntvclos) + sizeof(bupval*) * nupvals;
#     bgcobject *gco = be_newgcobj(vm, BE_NTVCLOS, size);
#     bntvclos *f = cast_ntvclos(gco);
#     if (f) {
#         f->f = cf;
#         f->nupvals = (bbyte)nupvals;
#         if (nupvals) {
#             var_setntvclos(vm->top, f);
#             be_incrtop(vm);
#             init_upvals(vm, f); /* may be GC */
#             be_stackpop(vm, 1);
#         }
#     }
#     return f;
# }
def be_newntvclosure(vm, cf, nupvals):
    """Allocate a new native closure wrapping callable *cf* with *nupvals* upvalue slots.

    In C, the native closure is temporarily pushed onto the stack to
    protect it from GC during upvalue initialization. In Python we
    register on the GC list directly.
    """
    f = bntvclos()
    f.type = BE_NTVCLOS

    # Register with GC
    f.next = vm.gc.list
    vm.gc.list = f

    f.f = cf
    f.nupvals = nupvals
    if nupvals:
        _init_upvals_ntv(vm, f)
    else:
        f.upvals = []
    return f


# ============================================================================
# be_func_varname
# ============================================================================

# #if BE_DEBUG_VAR_INFO
# bstring* be_func_varname(bproto *proto, int index, int pc)
# {
#     int i, nvarinfo = proto->nvarinfo;
#     bvarinfo *varinfo = proto->varinfo;
#     for (i = 0; i < nvarinfo && varinfo[i].beginpc <= pc; ++i) {
#         if (pc <= varinfo[i].endpc && index-- == 0) {
#             return varinfo[i].name;
#         }
#     }
#     return NULL;
# }
# #endif
def be_func_varname(proto, index, pc):
    """Look up the name of a local variable by register index at a given PC.

    Walks the varinfo table (available when BE_DEBUG_VAR_INFO is enabled)
    to find the variable whose scope covers *pc* and whose positional
    index matches. Returns the bstring name, or None if not found.
    """
    if not BE_DEBUG_VAR_INFO:
        return None
    if proto.varinfo is None:
        return None

    nvarinfo = proto.nvarinfo
    varinfo = proto.varinfo
    idx = index
    for i in range(nvarinfo):
        vi = varinfo[i]
        if vi.beginpc > pc:
            break
        if pc <= vi.endpc:
            if idx == 0:
                return vi.name
            idx -= 1
    return None
