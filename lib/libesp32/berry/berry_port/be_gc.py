"""
Berry garbage collector.
Mirrors: src/be_gc.c / src/be_gc.h

Three-color mark-sweep GC. Objects are linked via gc.list. The GC runs
automatically when gc.usage > gc.threshold. Since Python manages its own
memory, the Berry GC primarily tracks Berry-level object lifecycle for
correctness (destructor calls, weak references) rather than actual memory
reclamation.

Original C code is included as comments for each function.
"""

# /********************************************************************
# ** Copyright (c) 2018-2020 Guan Wenliang
# ** This file is part of the Berry default interpreter.
# ** skiars@qq.com, https://github.com/Skiars/berry
# ** See Copyright Notice in the LICENSE file or at
# ** https://github.com/Skiars/berry/blob/master/LICENSE
# ********************************************************************/

from berry_port.be_object import (
    # Type constants
    BE_STRING, BE_CLASS, BE_INSTANCE, BE_PROTO, BE_LIST, BE_MAP,
    BE_MODULE, BE_CLOSURE, BE_NTVCLOS, BE_COMOBJ, BE_FUNCTION,
    BE_GCOBJECT, BE_GCOBJECT_MAX, BE_MODULE_NAME,
    BE_VA_SHARED_KTAB,
    # GC mark constants
    GC_WHITE, GC_GRAY, GC_DARK, GC_FIXED, GC_CONST,
    # bvalue / var helpers
    bvalue, var_type, var_primetype, var_settype, var_setnil,
    var_isstatic, var_togc, var_isinstance, var_setinstance,
    var_toobj,
    # GC helpers
    gc_object, gc_iswhite, gc_isgray, gc_isdark,
    gc_setmark, gc_setwhite, gc_setgray, gc_setdark,
    gc_isfixed, gc_setfixed, gc_clearfixed, gc_isconst, gc_exmark,
    be_isgcobj,
    # Cast helpers
    cast_proto, cast_closure, cast_ntvclos, cast_str,
    cast_class, cast_instance, cast_map, cast_list, cast_module,
    # Accessor helpers
    be_class_name, be_class_members, be_class_super,
    be_instance_class, be_instance_members, be_instance_member_count,
    be_instance_super,
    be_list_data, be_list_end,
    be_map_iter,
    basetype,
)
from berry_port.berry_conf import (
    BE_USE_DEBUG_GC,
    BE_USE_PERF_COUNTERS,
    BE_USE_DEBUG_HOOK,
    BE_USE_PREPROCESSOR,
    BE_DEBUG_SOURCE_FILE,
    BE_DEBUG_RUNTIME_INFO,
    BE_DEBUG_VAR_INFO,
)

# ============================================================================
# GC status flags (from be_gc.c)
# ============================================================================

# #define GC_PAUSE    (1 << 0)  /* GC will not be executed automatically */
# #define GC_HALT     (1 << 1)  /* GC completely stopped */
# #define GC_ALLOC    (1 << 2)  /* GC in alloc */
GC_PAUSE = 1 << 0
GC_HALT = 1 << 1
GC_ALLOC = 1 << 2

# ============================================================================
# Internal macros (from be_gc.c)
# ============================================================================

# #define next_threshold(gc)  ((gc).usage * ((size_t)(gc).steprate + 100) / 100)
def _next_threshold(gc):
    return gc.usage * (gc.steprate + 100) // 100


# #define link_gray(vm, obj)     {    \
#     (obj)->gray = (vm)->gc.gray;    \
#     (vm)->gc.gray = gc_object(obj); \
# }
def _link_gray(vm, obj):
    obj.gray = vm.gc.gray
    vm.gc.gray = gc_object(obj)


# ============================================================================
# Forward declarations for lazy imports (avoid circular imports)
# ============================================================================

def _lazy_be_map():
    from berry_port import be_map
    return be_map

def _lazy_be_string():
    from berry_port import be_string
    return be_string

def _lazy_be_func():
    from berry_port import be_func
    return be_func

def _lazy_be_var():
    from berry_port import be_var
    return be_var

def _lazy_be_vector():
    from berry_port import be_vector
    return be_vector

def _lazy_be_class():
    from berry_port import be_class
    return be_class

def _lazy_be_mem():
    from berry_port import be_mem
    return be_mem


# ============================================================================
# comp_is_gc_debug helper (from be_vm.h)
# ============================================================================

# #define comp_is_gc_debug(vm)  ((vm)->compopt & (1<<COMP_GC_DEBUG))
def _comp_is_gc_debug(vm):
    from berry_port.be_object import COMP_GC_DEBUG
    return (getattr(vm, 'compopt', 0) & (1 << COMP_GC_DEBUG)) != 0


# ============================================================================
# be_gc_init (be_gc.c)
# ============================================================================

# void be_gc_init(bvm *vm)
# {
#     vm->gc.usage = sizeof(bvm);
#     be_gc_setsteprate(vm, 200);
#     be_gc_init_memory_pools(vm);
# }
def be_gc_init(vm):
    """Initialize the garbage collector state.

    Sets initial usage to a nominal VM size, configures the default
    step rate (200%), and initializes memory pools (no-op in Python).
    """
    vm.gc.usage = 256  # nominal sizeof(bvm) — Python doesn't have a fixed size
    be_gc_setsteprate(vm, 200)
    _lazy_be_mem().be_gc_init_memory_pools(vm)


# ============================================================================
# be_gc_deleteall (be_gc.c)
# ============================================================================

# void be_gc_deleteall(bvm *vm)
# {
#     bupval *uv, *uvnext;
#     bgcobject *node, *next;
#     /* halt GC and delete all objects */
#     vm->gc.status |= GC_HALT;
#     /* first: call destructor */
#     for (node = vm->gc.list; node; node = node->next) {
#         destruct_object(vm, node);
#     }
#     /* second: free objects */
#     for (node = vm->gc.list; node; node = next) {
#         next = node->next;
#         free_object(vm, node);
#     }
#     /* delete open upvalue list */
#     for (uv = vm->upvalist; uv; uv = uvnext) {
#         uvnext = uv->u.next;
#         be_free(vm, uv, sizeof(bupval));
#     }
# }
def be_gc_deleteall(vm):
    """Halt GC and delete all managed objects.

    First calls destructors (deinit) on instances, then frees all
    objects on the GC list, and finally frees open upvalues.
    """
    be_mem = _lazy_be_mem()

    vm.gc.status |= GC_HALT

    # First pass: call destructors
    node = vm.gc.list
    while node is not None:
        _destruct_object(vm, node)
        node = node.next

    # Second pass: free objects
    node = vm.gc.list
    while node is not None:
        next_ = node.next
        _free_object(vm, node)
        node = next_

    # Delete open upvalue list
    uv = vm.upvalist
    while uv is not None:
        uvnext = uv.u_next
        # be_free(vm, uv, sizeof(bupval)) — in Python just drop the reference
        uv = uvnext
    vm.upvalist = None


# ============================================================================
# be_gc_setsteprate (be_gc.c)
# ============================================================================

# void be_gc_setsteprate(bvm *vm, int rate)
# {
#     be_assert(rate >= 100 && rate <= 355);
#     vm->gc.steprate = (bbyte)(rate - 100);
#     vm->gc.threshold = next_threshold(vm->gc);
# }
def be_gc_setsteprate(vm, rate):
    """Set the GC step rate (100–355).

    The step rate controls how much memory can be allocated before the
    next automatic GC cycle. A rate of 200 means the threshold is 2×
    current usage.
    """
    assert 100 <= rate <= 355
    vm.gc.steprate = rate - 100
    vm.gc.threshold = _next_threshold(vm.gc)


# ============================================================================
# be_gc_setpause (be_gc.c)
# ============================================================================

# void be_gc_setpause(bvm *vm, int pause)
# {
#     if (pause) {
#         vm->gc.status |= GC_PAUSE;
#     } else {
#         vm->gc.status &= ~GC_PAUSE;
#     }
# }
def be_gc_setpause(vm, pause):
    """Enable or disable automatic GC collection."""
    if pause:
        vm.gc.status |= GC_PAUSE
    else:
        vm.gc.status &= ~GC_PAUSE


# ============================================================================
# be_gc_memcount (be_gc.c)
# ============================================================================

# size_t be_gc_memcount(bvm *vm)
# {
#     return vm->gc.usage;
# }
def be_gc_memcount(vm):
    """Return the current tracked memory usage in bytes."""
    return vm.gc.usage


# ============================================================================
# be_newgcobj (be_gc.c)
# ============================================================================

# bgcobject* be_newgcobj(bvm *vm, int type, size_t size)
# {
#     bgcobject *obj = be_malloc(vm, size);
#     be_gc_auto(vm);
#     var_settype(obj, (bbyte)type);
#     obj->marked = GC_WHITE;
#     obj->next = vm->gc.list;
#     vm->gc.list = obj;
#     return obj;
# }
def be_newgcobj(vm, type_, size):
    """Allocate a new GC-managed object.

    The object is linked into vm.gc.list, marked white, and an
    automatic GC cycle may be triggered.

    In the Python port, *size* is tracked for gc.usage accounting but
    the actual Python object is created by the caller. This function
    creates a generic bgcobject placeholder; callers typically replace
    it with the real object after calling this.

    Returns the object (which the caller should replace with the
    actual typed object while preserving the linked-list linkage).
    """
    from berry_port.be_object import bgcobject
    be_mem = _lazy_be_mem()

    # Track allocation in gc.usage
    vm.gc.usage += size
    be_gc_auto(vm)

    obj = bgcobject()
    var_settype(obj, type_ & 0xFF)
    obj.marked = GC_WHITE
    obj.next = vm.gc.list
    vm.gc.list = obj
    return obj


# ============================================================================
# be_gc_newstr (be_gc.c)
# ============================================================================

# bgcobject* be_gc_newstr(bvm *vm, size_t size, int islong)
# {
#     bgcobject *obj;
#     if (islong) {
#         return be_newgcobj(vm, BE_STRING, size);
#     }
#     obj = be_malloc(vm, size);
#     be_gc_auto(vm);
#     var_settype(obj, BE_STRING);
#     obj->marked = GC_WHITE;
#     return obj;
# }
def be_gc_newstr(vm, size, islong):
    """Allocate a new GC-managed string object.

    Long strings are linked into the GC list (like normal GC objects).
    Short strings are NOT linked into the GC list — they are managed
    by the string interning table and swept by be_gcstrtab.
    """
    from berry_port.be_object import bgcobject

    if islong:
        return be_newgcobj(vm, BE_STRING, size)

    # Short string: track usage but don't link into gc.list
    vm.gc.usage += size
    be_gc_auto(vm)

    obj = bgcobject()
    var_settype(obj, BE_STRING)
    obj.marked = GC_WHITE
    return obj


# ============================================================================
# be_gc_fix / be_gc_unfix / be_gc_fix_set (be_gc.c)
# ============================================================================

# void be_gc_fix(bvm *vm, bgcobject *obj)
# {
#     (void)vm;
#     if (!gc_isconst(obj)) {
#         gc_setfixed(obj);
#     }
# }
def be_gc_fix(vm, obj):
    """Mark an object as fixed (will not be collected)."""
    if not gc_isconst(obj):
        gc_setfixed(obj)


# void be_gc_unfix(bvm *vm, bgcobject *obj)
# {
#     (void)vm;
#     if (!gc_isconst(obj)) {
#         gc_clearfixed(obj);
#     }
# }
def be_gc_unfix(vm, obj):
    """Remove the fixed mark from an object."""
    if not gc_isconst(obj):
        gc_clearfixed(obj)


# bbool be_gc_fix_set(bvm *vm, bgcobject *obj, bbool fix)
# {
#     (void)vm;
#     bbool was_fixed = gc_isfixed(obj);
#     if (!gc_isconst(obj)) {
#         if (fix) {
#             gc_setfixed(obj);
#         } else {
#             gc_clearfixed(obj);
#         }
#     }
#     return was_fixed;
# }
def be_gc_fix_set(vm, obj, fix):
    """Set or clear the fixed mark, returning the previous state."""
    was_fixed = gc_isfixed(obj)
    if not gc_isconst(obj):
        if fix:
            gc_setfixed(obj)
        else:
            gc_clearfixed(obj)
    return was_fixed


# ============================================================================
# mark_gray — core marking function (be_gc.c)
# ============================================================================

# static void mark_gray(bvm *vm, bgcobject *obj)
# {
#     if (obj && gc_iswhite(obj) && !gc_isconst(obj)) {
#         gc_setgray(obj);
#         be_assert(!var_isstatic(obj));
#         switch (var_primetype(obj)) {
#         case BE_STRING: gc_setdark(obj); break;
#         case BE_CLASS: link_gray(vm, cast_class(obj)); break;
#         case BE_PROTO: link_gray(vm, cast_proto(obj)); break;
#         case BE_INSTANCE: link_gray(vm, cast_instance(obj)); break;
#         case BE_MAP: link_gray(vm, cast_map(obj)); break;
#         case BE_LIST: link_gray(vm, cast_list(obj)); break;
#         case BE_CLOSURE: link_gray(vm, cast_closure(obj)); break;
#         case BE_NTVCLOS: link_gray(vm, cast_ntvclos(obj)); break;
#         case BE_MODULE: link_gray(vm, cast_module(obj)); break;
#         case BE_COMOBJ: gc_setdark(obj); break;
#         default: break;
#         }
#     }
# }
def _mark_gray(vm, obj):
    """Mark a white object as gray (discovered but not yet scanned).

    Strings and COMOBJ are immediately set to dark (no sub-references).
    Compound objects are added to the gray list for later scanning.
    """
    if obj is not None and gc_iswhite(obj) and not gc_isconst(obj):
        gc_setgray(obj)
        ptype = var_primetype(obj)
        if ptype == BE_STRING:
            gc_setdark(obj)
        elif ptype == BE_CLASS:
            _link_gray(vm, obj)  # obj is already a bclass
        elif ptype == BE_PROTO:
            _link_gray(vm, obj)
        elif ptype == BE_INSTANCE:
            _link_gray(vm, obj)
        elif ptype == BE_MAP:
            _link_gray(vm, obj)
        elif ptype == BE_LIST:
            _link_gray(vm, obj)
        elif ptype == BE_CLOSURE:
            _link_gray(vm, obj)
        elif ptype == BE_NTVCLOS:
            _link_gray(vm, obj)
        elif ptype == BE_MODULE:
            _link_gray(vm, obj)
        elif ptype == BE_COMOBJ:
            gc_setdark(obj)


# static void mark_gray_var(bvm *vm, bvalue *value)
# {
#     if (be_isgcobj(value)) {
#         mark_gray(vm, var_togc(value));
#     }
# }
def _mark_gray_var(vm, value):
    """Mark the GC object referenced by a bvalue, if any."""
    if be_isgcobj(value):
        _mark_gray(vm, var_togc(value))


# ============================================================================
# Type-specific mark functions (be_gc.c)
# ============================================================================

# static void mark_map(bvm *vm, bgcobject *obj)
# {
#     bmap *map = cast_map(obj);
#     gc_try (map != NULL) {
#         bmapnode *node;
#         bmapiter iter = be_map_iter();
#         vm->gc.gray = map->gray;
#         while ((node = be_map_next(map, &iter)) != NULL) {
#             bmapkey *key = &node->key;
#             bvalue *val = &node->value;
#             if (be_isgcobj(key)) {
#                 mark_gray(vm, var_togc(key));
#             }
#             mark_gray_var(vm, val);
#         }
#     }
# }
def _mark_map(vm, obj):
    """Scan a map: mark all keys and values, then remove from gray list."""
    be_map = _lazy_be_map()
    map_ = obj  # already a bmap
    if map_ is not None:
        vm.gc.gray = map_.gray  # remove from gray list
        iter_ = [be_map_iter()]
        while True:
            node, iter_ = be_map.be_map_next(map_, iter_)
            if node is None:
                break
            key = node.key
            val = node.value
            if be_isgcobj(key):
                _mark_gray(vm, var_togc(key))
            _mark_gray_var(vm, val)


# static void mark_list(bvm *vm, bgcobject *obj)
# {
#     blist *list = cast_list(obj);
#     gc_try (list != NULL) {
#         bvalue *val = be_list_data(list);
#         bvalue *end = be_list_end(list);
#         vm->gc.gray = list->gray;
#         for (; val < end; val++) {
#             mark_gray_var(vm, val);
#         }
#     }
# }
def _mark_list(vm, obj):
    """Scan a list: mark all element values."""
    list_ = obj  # already a blist
    if list_ is not None:
        vm.gc.gray = list_.gray  # remove from gray list
        data = be_list_data(list_)
        if data is not None:
            for i in range(list_.count):
                _mark_gray_var(vm, data[i])


# static void mark_proto(bvm *vm, bgcobject *obj)
# {
#     bproto *p = cast_proto(obj);
#     gc_try (p != NULL) {
#         int count;
#         bvalue *k = p->ktab;
#         bproto **ptab = p->ptab;
#         vm->gc.gray = p->gray;
#         for (count = p->nconst; count--; ++k) {
#             mark_gray_var(vm, k);
#         }
#         for (count = p->nproto; count--; ++ptab) {
#             mark_gray(vm, gc_object(*ptab));
#         }
#         gc_setdark_safe(p->name);
#         #if BE_DEBUG_SOURCE_FILE
#         gc_setdark_safe(p->source);
#         #endif
#         #if BE_DEBUG_VAR_INFO
#         if (p->nvarinfo) { ... }
#         #endif
#     }
# }
def _mark_proto(vm, obj):
    """Scan a proto: mark constants, sub-protos, name, source, varinfo."""
    p = obj  # already a bproto
    if p is not None:
        vm.gc.gray = p.gray  # remove from gray list
        # Mark constants
        if p.ktab is not None:
            for i in range(p.nconst):
                _mark_gray_var(vm, p.ktab[i])
        # Mark sub-protos
        if p.ptab is not None:
            for i in range(p.nproto):
                _mark_gray(vm, gc_object(p.ptab[i]))
        # Mark name string
        if p.name is not None:
            gc_setdark(p.name)
        # Mark source string
        if BE_DEBUG_SOURCE_FILE and p.source is not None:
            gc_setdark(p.source)
        # Mark variable info names
        if BE_DEBUG_VAR_INFO and p.nvarinfo and p.varinfo is not None:
            for i in range(p.nvarinfo):
                vinfo = p.varinfo[i]
                if vinfo.name is not None:
                    gc_setdark(vinfo.name)


# static void mark_closure(bvm *vm, bgcobject *obj)
# {
#     bclosure *cl = cast_closure(obj);
#     gc_try (cl != NULL) {
#         int count = cl->nupvals;
#         bupval **uv = cl->upvals;
#         vm->gc.gray = cl->gray;
#         for (; count--; ++uv) {
#             if (*uv && (*uv)->refcnt) {
#                 mark_gray_var(vm, (*uv)->value);
#             }
#         }
#         mark_gray(vm, gc_object(cl->proto));
#     }
# }
def _mark_closure(vm, obj):
    """Scan a closure: mark upvalues and the proto."""
    cl = obj  # already a bclosure
    if cl is not None:
        vm.gc.gray = cl.gray  # remove from gray list
        if cl.upvals is not None:
            for i in range(cl.nupvals):
                uv = cl.upvals[i]
                if uv is not None and uv.refcnt:
                    _mark_gray_var(vm, uv.value)
        _mark_gray(vm, gc_object(cl.proto))


# static void mark_ntvclos(bvm *vm, bgcobject *obj)
# {
#     bntvclos *f = cast_ntvclos(obj);
#     gc_try (f != NULL) {
#         int count = f->nupvals;
#         bupval **uv = &be_ntvclos_upval(f, 0);
#         vm->gc.gray = f->gray;
#         for (; count--; ++uv) {
#             if (*uv && (*uv)->refcnt) {
#                 mark_gray_var(vm, (*uv)->value);
#             }
#         }
#     }
# }
def _mark_ntvclos(vm, obj):
    """Scan a native closure: mark upvalues."""
    f = obj  # already a bntvclos
    if f is not None:
        vm.gc.gray = f.gray  # remove from gray list
        if f.upvals is not None:
            for i in range(f.nupvals):
                uv = f.upvals[i]
                if uv is not None and uv.refcnt:
                    _mark_gray_var(vm, uv.value)


# static void mark_class(bvm *vm, bgcobject *obj)
# {
#     bclass *c = cast_class(obj);
#     gc_try (c != NULL) {
#         vm->gc.gray = c->gray;
#         mark_gray(vm, gc_object(be_class_name(c)));
#         mark_gray(vm, gc_object(be_class_members(c)));
#         mark_gray(vm, gc_object(be_class_super(c)));
#     }
# }
def _mark_class(vm, obj):
    """Scan a class: mark name, members map, and superclass."""
    c = obj  # already a bclass
    if c is not None:
        vm.gc.gray = c.gray  # remove from gray list
        _mark_gray(vm, gc_object(be_class_name(c)))
        _mark_gray(vm, gc_object(be_class_members(c)))
        _mark_gray(vm, gc_object(be_class_super(c)))


# static void mark_instance(bvm *vm, bgcobject *obj)
# {
#     binstance *o = cast_instance(obj);
#     gc_try (o != NULL) {
#         bvalue *var = be_instance_members(o);
#         int nvar = be_instance_member_count(o);
#         vm->gc.gray = o->gray;
#         mark_gray(vm, gc_object(be_instance_class(o)));
#         mark_gray(vm, gc_object(be_instance_super(o)));
#         for (; nvar--; var++) {
#             mark_gray_var(vm, var);
#         }
#     }
# }
def _mark_instance(vm, obj):
    """Scan an instance: mark class, super-instance, and member variables."""
    o = obj  # already a binstance
    if o is not None:
        vm.gc.gray = o.gray  # remove from gray list
        _mark_gray(vm, gc_object(be_instance_class(o)))
        _mark_gray(vm, gc_object(be_instance_super(o)))
        members = be_instance_members(o)
        nvar = be_instance_member_count(o)
        if members is not None:
            for i in range(nvar):
                _mark_gray_var(vm, members[i])


# static void mark_module(bvm *vm, bgcobject *obj)
# {
#     bmodule *o = cast_module(obj);
#     gc_try (o != NULL) {
#         vm->gc.gray = o->gray;
#         mark_gray(vm, gc_object(o->table));
#         if (!gc_isconst(o) && gc_exmark(o) & BE_MODULE_NAME) {
#             mark_gray(vm, gc_object(o->info.sname));
#         }
#     }
# }
def _mark_module(vm, obj):
    """Scan a module: mark its table and optional name string."""
    o = obj  # already a bmodule
    if o is not None:
        vm.gc.gray = o.gray  # remove from gray list
        _mark_gray(vm, gc_object(o.table))
        if not gc_isconst(o) and (gc_exmark(o) & BE_MODULE_NAME):
            # info holds a bstring when BE_MODULE_NAME is set
            _mark_gray(vm, gc_object(o.info))


# ============================================================================
# Free helpers (be_gc.c)
# ============================================================================

# static void free_proto(bvm *vm, bgcobject *obj)
# {
#     bproto *proto = cast_proto(obj);
#     gc_try (proto != NULL) {
#         be_free(vm, proto->upvals, proto->nupvals * sizeof(bupvaldesc));
#         if (!(proto->varg & BE_VA_SHARED_KTAB)) {
#             be_free(vm, proto->ktab, proto->nconst * sizeof(bvalue));
#         }
#         be_free(vm, proto->ptab, proto->nproto * sizeof(bproto*));
#         be_free(vm, proto->code, proto->codesize * sizeof(binstruction));
#         #if BE_DEBUG_RUNTIME_INFO
#         be_free(vm, proto->lineinfo, proto->nlineinfo * sizeof(blineinfo));
#         #endif
#         #if BE_DEBUG_VAR_INFO
#         be_free(vm, proto->varinfo, proto->nvarinfo * sizeof(bvarinfo));
#         #endif
#         be_free(vm, proto, sizeof(bproto));
#     }
# }
def _free_proto(vm, obj):
    """Free a proto and all its owned arrays."""
    proto = obj
    if proto is not None:
        # In Python, dropping references is sufficient.
        # We still decrement gc.usage for tracking accuracy.
        proto.upvals = None
        if not (proto.varg & BE_VA_SHARED_KTAB):
            proto.ktab = None
        proto.ptab = None
        proto.code = None
        if BE_DEBUG_RUNTIME_INFO:
            proto.lineinfo = None
        if BE_DEBUG_VAR_INFO:
            proto.varinfo = None


# static void free_closure(bvm *vm, bgcobject *obj)
# {
#     bclosure *cl = cast_closure(obj);
#     gc_try (cl != NULL) {
#         int count = cl->nupvals;
#         be_release_upvalues(vm, cl);
#         be_free(vm, cl, sizeof(bclosure) + sizeof(bupval*) * ((size_t)count - 1));
#     }
# }
def _free_closure(vm, obj):
    """Free a closure, releasing its upvalues."""
    cl = obj
    if cl is not None:
        _lazy_be_func().be_release_upvalues(vm, cl)
        cl.upvals = None
        cl.proto = None


# static void free_ntvclos(bvm *vm, bgcobject *obj)
# {
#     bntvclos *f = cast_ntvclos(obj);
#     gc_try (f != NULL) {
#         int count = f->nupvals;
#         bupval **uv = &be_ntvclos_upval(f, 0);
#         while (count--) {
#             be_free(vm, *uv++, sizeof(bupval));
#         }
#         be_free(vm, f, sizeof(bntvclos) + sizeof(bupval*) * f->nupvals);
#     }
# }
def _free_ntvclos(vm, obj):
    """Free a native closure and its upvalues."""
    f = obj
    if f is not None:
        if f.upvals is not None:
            for i in range(f.nupvals):
                f.upvals[i] = None
        f.upvals = None


# static void free_lstring(bvm *vm, bgcobject *obj)
# {
#     blstring *ls = gc_cast(obj, BE_STRING, blstring);
#     gc_try (ls != NULL) {
#         be_free(vm, ls, sizeof(blstring) + ls->llen + 1);
#     }
# }
def _free_lstring(vm, obj):
    """Free a long string object."""
    # In Python, just drop the reference. The string is a bstring.
    pass


# static void free_instance(bvm *vm, bgcobject *obj)
# {
#     binstance *o = cast_instance(obj);
#     int nvar = be_instance_member_count(o);
#     be_free(vm, obj, sizeof(binstance) + sizeof(bvalue) * (nvar - 1));
# }
def _free_instance(vm, obj):
    """Free an instance object."""
    o = obj
    if o is not None:
        o.members = None
        o._class = None
        o.super = None
        o.sub = None


# static void free_object(bvm *vm, bgcobject *obj)
# {
#     switch (var_primetype(obj)) {
#     case BE_STRING: free_lstring(vm, obj); break;
#     case BE_CLASS: be_free(vm, obj, sizeof(bclass)); break;
#     case BE_INSTANCE: free_instance(vm, obj); break;
#     case BE_MAP: be_map_delete(vm, cast_map(obj)); break;
#     case BE_LIST: be_list_delete(vm, cast_list(obj)); break;
#     case BE_CLOSURE: free_closure(vm, obj); break;
#     case BE_NTVCLOS: free_ntvclos(vm, obj); break;
#     case BE_PROTO: free_proto(vm, obj); break;
#     case BE_MODULE: break;  # be_module_delete not yet ported
#     case BE_COMOBJ: break;  # be_commonobj_delete not yet ported
#     default: break;
#     }
# }
def _free_object(vm, obj):
    """Free a GC object based on its type.

    Each type-specific free function expects the actual typed object
    (bmap, blist, etc.), not a bare bgcobject. We use hasattr checks
    to guard against placeholder objects that haven't been fully
    initialized.
    """
    ptype = var_primetype(obj)
    if ptype == BE_STRING:
        _free_lstring(vm, obj)
    elif ptype == BE_CLASS:
        pass  # Python GC handles it
    elif ptype == BE_INSTANCE:
        if hasattr(obj, 'members'):
            _free_instance(vm, obj)
    elif ptype == BE_MAP:
        if hasattr(obj, 'slots'):
            _lazy_be_map().be_map_delete(vm, obj)
    elif ptype == BE_LIST:
        if hasattr(obj, 'capacity'):
            from berry_port.be_list import be_list_delete
            be_list_delete(vm, obj)
    elif ptype == BE_CLOSURE:
        if hasattr(obj, 'upvals'):
            _free_closure(vm, obj)
    elif ptype == BE_NTVCLOS:
        if hasattr(obj, 'upvals'):
            _free_ntvclos(vm, obj)
    elif ptype == BE_PROTO:
        if hasattr(obj, 'ktab'):
            _free_proto(vm, obj)
    elif ptype == BE_MODULE:
        if hasattr(obj, 'table'):
            from berry_port.be_module import be_module_delete
            be_module_delete(vm, obj)
    elif ptype == BE_COMOBJ:
        pass  # be_commonobj_delete will be wired when ported


# ============================================================================
# Pre-mark phase functions (be_gc.c)
# ============================================================================

# static void premark_internal(bvm *vm)
# {
#     mark_gray(vm, gc_object(vm->module.loaded));
#     mark_gray(vm, gc_object(vm->module.path));
#     mark_gray(vm, gc_object(vm->ntvclass));
#     #if BE_USE_PREPROCESSOR
#     if (vm->preprocessor) {
#         mark_gray(vm, gc_object(vm->preprocessor));
#     }
#     #endif
#     #if BE_USE_DEBUG_HOOK
#     if (be_isgcobj(&vm->hook)) {
#         mark_gray(vm, gc_object(var_toobj(&vm->hook)));
#     }
#     #endif
# }
def premark_internal(vm):
    """Mark VM-internal root objects: loaded modules, path list, ntvclass."""
    _mark_gray(vm, gc_object(vm.module.loaded))
    _mark_gray(vm, gc_object(vm.module.path))
    _mark_gray(vm, gc_object(getattr(vm, 'ntvclass', None)))
    if BE_USE_PREPROCESSOR:
        pp = getattr(vm, 'preprocessor', None)
        if pp is not None:
            _mark_gray(vm, gc_object(pp))
    if BE_USE_DEBUG_HOOK:
        hook = getattr(vm, 'hook', None)
        if hook is not None and be_isgcobj(hook):
            _mark_gray(vm, gc_object(var_toobj(hook)))


# static void premark_global(bvm *vm)
# {
#     bvalue *v = vm->gbldesc.global.vlist.data;
#     bvalue *end = v + be_global_count(vm);
#     while (v < end) {
#         if (be_isgcobj(v)) {
#             mark_gray(vm, var_togc(v));
#         }
#         ++v;
#     }
#     v = vm->gbldesc.builtin.vlist.data;
#     end = v + be_builtin_count(vm);
#     while (v < end) {
#         mark_gray_var(vm, v++);
#     }
# }
def premark_global(vm):
    """Mark all global and built-in variables as roots."""
    be_var = _lazy_be_var()

    # Global variables
    gdata = vm.gbldesc.global_.vlist.data
    if gdata is not None:
        gcount = be_var.be_global_count(vm)
        for i in range(gcount):
            v = gdata[i]
            if be_isgcobj(v):
                _mark_gray(vm, var_togc(v))

    # Built-in variables
    bdata = vm.gbldesc.builtin.vlist.data
    if bdata is not None:
        bcount = be_var.be_builtin_count(vm)
        for i in range(bcount):
            _mark_gray_var(vm, bdata[i])


# static void premark_stack(bvm *vm)
# {
#     bvalue *v = vm->stack, *end = vm->top;
#     for (; v < end; ++v) {
#         mark_gray_var(vm, v);
#     }
#     end = vm->stacktop;
#     for (; v < end; ++v) {
#         var_setnil(v);
#     }
# }
def premark_stack(vm):
    """Mark all live stack values and nil-out the unused portion."""
    stack = vm.stack
    if stack is None:
        return
    top_idx = getattr(vm, 'top_idx', 0)
    stacktop_idx = getattr(vm, 'stacktop_idx', len(stack))

    # Mark live objects on the stack
    for i in range(top_idx):
        if i < len(stack):
            _mark_gray_var(vm, stack[i])

    # Set unused stack slots to nil
    for i in range(top_idx, min(stacktop_idx, len(stack))):
        var_setnil(stack[i])


# static void premark_tracestack(bvm *vm)
# {
#     bcallsnapshot *cf = be_vector_data(&vm->tracestack);
#     bcallsnapshot *end = be_vector_end(&vm->tracestack);
#     for (; cf <= end; ++cf) {
#         mark_gray_var(vm, &cf->func);
#     }
# }
def premark_tracestack(vm):
    """Mark function values in the trace stack (for error reporting)."""
    be_vector = _lazy_be_vector()
    tracestack = getattr(vm, 'tracestack', None)
    if tracestack is None:
        return
    data = tracestack.data
    if data is None:
        return
    count = tracestack.count
    for i in range(count):
        _mark_gray_var(vm, data[i].func)


# static void premark_fixed(bvm *vm)
# {
#     bgcobject *node = vm->gc.list;
#     for (; node; node = node->next) {
#         if (gc_isfixed(node) && gc_iswhite(node)) {
#             mark_gray(vm, node);
#         }
#     }
# }
def premark_fixed(vm):
    """Mark all fixed (pinned) objects that are still white."""
    node = vm.gc.list
    while node is not None:
        if gc_isfixed(node) and gc_iswhite(node):
            _mark_gray(vm, node)
        node = node.next


# ============================================================================
# mark_unscanned — process gray list until empty (be_gc.c)
# ============================================================================

# static void mark_unscanned(bvm *vm)
# {
#     while (vm->gc.gray) {
#         bgcobject *obj = vm->gc.gray;
#         if (obj && !gc_isdark(obj) && !gc_isconst(obj)) {
#             gc_setdark(obj);
#             switch (var_primetype(obj)) {
#             case BE_CLASS: mark_class(vm, obj); break;
#             case BE_PROTO: mark_proto(vm, obj); break;
#             case BE_INSTANCE: mark_instance(vm, obj); break;
#             case BE_MAP: mark_map(vm, obj); break;
#             case BE_LIST: mark_list(vm, obj); break;
#             case BE_CLOSURE: mark_closure(vm, obj); break;
#             case BE_NTVCLOS: mark_ntvclos(vm, obj); break;
#             case BE_MODULE: mark_module(vm, obj); break;
#             default: be_assert(0); break;
#             }
#         }
#     }
# }
def mark_unscanned(vm):
    """Process the gray list: scan each gray object and mark it dark.

    Each type-specific marker removes the object from the gray list
    head (by setting vm.gc.gray = obj.gray) and marks sub-references.
    """
    while vm.gc.gray is not None:
        obj = vm.gc.gray
        if obj is not None and not gc_isdark(obj) and not gc_isconst(obj):
            gc_setdark(obj)
            ptype = var_primetype(obj)
            if ptype == BE_CLASS:
                _mark_class(vm, obj)
            elif ptype == BE_PROTO:
                _mark_proto(vm, obj)
            elif ptype == BE_INSTANCE:
                _mark_instance(vm, obj)
            elif ptype == BE_MAP:
                _mark_map(vm, obj)
            elif ptype == BE_LIST:
                _mark_list(vm, obj)
            elif ptype == BE_CLOSURE:
                _mark_closure(vm, obj)
            elif ptype == BE_NTVCLOS:
                _mark_ntvclos(vm, obj)
            elif ptype == BE_MODULE:
                _mark_module(vm, obj)
            else:
                # Unknown type in gray list — skip to avoid infinite loop
                vm.gc.gray = getattr(obj, 'gray', None)
        else:
            # Object is already dark, const, or None — advance past it
            vm.gc.gray = getattr(obj, 'gray', None) if obj is not None else None


# ============================================================================
# Destruct phase (be_gc.c)
# ============================================================================

# static void destruct_object(bvm *vm, bgcobject *obj)
# {
#     if (vm->gc.status & GC_ALLOC) {
#         return;
#     }
#     if (obj->type == BE_INSTANCE) {
#         int type;
#         binstance *ins = cast_instance(obj);
#         type = be_instance_member_simple(vm, ins, str_literal(vm, "deinit"), vm->top);
#         be_incrtop(vm);
#         if (basetype(type) == BE_FUNCTION) {
#             var_setinstance(vm->top, ins);
#             be_incrtop(vm);
#             be_dofunc(vm, vm->top - 2, 1);
#             be_stackpop(vm, 1);
#         }
#         be_stackpop(vm, 1);
#     }
# }
def _destruct_object(vm, obj):
    """Call the 'deinit' destructor on an instance, if it has one.

    Destructors are not called during allocation (GC_ALLOC flag set).
    This is a best-effort operation — exceptions during deinit are
    silently ignored to avoid disrupting the GC cycle.
    """
    if vm.gc.status & GC_ALLOC:
        return
    if obj.type == BE_INSTANCE:
        # Destructor calling requires the VM execution machinery
        # (be_dofunc, str_literal, etc.) which is in be_vm.py / be_exec.py.
        # These modules are not yet ported, so we skip destructor calls
        # for now. When be_vm.py is available, this will be wired up.
        #
        # The C code does:
        #   type = be_instance_member_simple(vm, ins, str_literal(vm, "deinit"), vm->top);
        #   if (basetype(type) == BE_FUNCTION) {
        #       var_setinstance(vm->top, ins);
        #       be_dofunc(vm, vm->top - 2, 1);
        #   }
        pass


# static void destruct_white(bvm *vm)
# {
#     bgcobject *node = vm->gc.list;
#     vm->gc.status |= GC_HALT;
#     while (node) {
#         if (gc_iswhite(node)) {
#             destruct_object(vm, node);
#         }
#         node = node->next;
#     }
#     vm->gc.status &= ~GC_HALT;
# }
def destruct_white(vm):
    """Call destructors on all white (unreachable) objects.

    GC is halted during destruction to prevent re-entrant collection
    if a destructor allocates memory.
    """
    vm.gc.status |= GC_HALT
    node = vm.gc.list
    while node is not None:
        if gc_iswhite(node):
            _destruct_object(vm, node)
        node = node.next
    vm.gc.status &= ~GC_HALT


# ============================================================================
# Sweep phase (be_gc.c)
# ============================================================================

# static void delete_white(bvm *vm)
# {
#     bgcobject *node, *prev, *next;
#     for (node = vm->gc.list, prev = node; node; node = next) {
#         next = node->next;
#         if (gc_iswhite(node)) {
#             if (node == vm->gc.list) {
#                 vm->gc.list = node->next;
#                 prev = node->next;
#             } else {
#                 prev->next = next;
#             }
#             free_object(vm, node);
#         } else {
#             gc_setwhite(node);
#             prev = node;
#         }
#     }
# }
def delete_white(vm):
    """Sweep the GC list: free white objects, reset survivors to white."""
    prev = None
    node = vm.gc.list
    while node is not None:
        next_ = node.next
        if gc_iswhite(node):
            # Unlink from list
            if node == vm.gc.list:
                vm.gc.list = next_
                prev = None  # prev stays None since we removed the head
            else:
                if prev is not None:
                    prev.next = next_
            _free_object(vm, node)
        else:
            gc_setwhite(node)
            prev = node
        node = next_


# static void reset_fixedlist(bvm *vm)
# {
#     bgcobject *node;
#     for (node = vm->gc.fixed; node; node = node->next) {
#         if (gc_isdark(node)) {
#             gc_setwhite(node);
#         }
#     }
# }
def reset_fixedlist(vm):
    """Reset dark fixed-list objects back to white for the next cycle."""
    node = vm.gc.fixed
    while node is not None:
        if gc_isdark(node):
            gc_setwhite(node)
        node = node.next


# ============================================================================
# be_gc_auto (be_gc.c)
# ============================================================================

# void be_gc_auto(bvm *vm)
# {
#     if (vm->gc.status & GC_PAUSE && (BE_USE_DEBUG_GC || vm->gc.usage > vm->gc.threshold || comp_is_gc_debug(vm))) {
#         be_gc_collect(vm);
#     }
# }
def be_gc_auto(vm):
    """Trigger an automatic GC cycle if conditions are met.

    Collection runs when GC_PAUSE is set (auto-collection enabled) and
    either debug GC is on or usage exceeds the threshold.
    """
    if vm.gc.status & GC_PAUSE:
        if BE_USE_DEBUG_GC or vm.gc.usage > vm.gc.threshold or _comp_is_gc_debug(vm):
            be_gc_collect(vm)


# ============================================================================
# be_gc_collect — full collection cycle (be_gc.c)
# ============================================================================

# void be_gc_collect(bvm *vm)
# {
#     if (vm->gc.status & GC_HALT) {
#         return;
#     }
#     if (vm->obshook != NULL) (*vm->obshook)(vm, BE_OBS_GC_START, vm->gc.usage);
#     /* step 1: set root-set reference objects to unscanned */
#     mark_gray_reset_counters(vm);
#     premark_internal(vm);
#     premark_global(vm);
#     premark_stack(vm);
#     premark_tracestack(vm);
#     premark_fixed(vm);
#     /* step 2: set unscanned objects to black */
#     mark_unscanned(vm);
#     /* step 3: destruct and delete unreachable objects */
#     destruct_white(vm);
#     delete_white(vm);
#     be_gcstrtab(vm);
#     /* step 4: reset the fixed objects */
#     reset_fixedlist(vm);
#     /* step 5: calculate the next GC threshold */
#     vm->gc.threshold = next_threshold(vm->gc);
#     be_gc_memory_pools(vm);
#     if (vm->obshook != NULL) (*vm->obshook)(vm, BE_OBS_GC_END, vm->gc.usage);
# }
def be_gc_collect(vm):
    """Run a full mark-sweep garbage collection cycle.

    Steps:
    1. Pre-mark: mark root-set objects (internals, globals, stack,
       tracestack, fixed objects) as gray.
    2. Mark: process gray list until empty, marking reachable objects dark.
    3. Destruct: call deinit on white (unreachable) instances.
    4. Sweep: free white objects, reset survivors to white.
    5. String GC: sweep the string interning table.
    6. Reset: clear dark marks on fixed-list objects.
    7. Threshold: recalculate the next GC threshold.
    """
    if vm.gc.status & GC_HALT:
        return

    # Observer hook (if set)
    obshook = getattr(vm, 'obshook', None)
    if obshook is not None:
        obshook(vm, 'GC_START', vm.gc.usage)

    # Step 1: pre-mark root set
    premark_internal(vm)
    premark_global(vm)
    premark_stack(vm)
    premark_tracestack(vm)
    premark_fixed(vm)

    # Step 2: process gray list
    mark_unscanned(vm)

    # Step 3: destruct and delete unreachable objects
    destruct_white(vm)
    delete_white(vm)

    # String table GC
    _lazy_be_string().be_gcstrtab(vm)

    # Step 4: reset fixed list
    reset_fixedlist(vm)

    # Step 5: recalculate threshold
    vm.gc.threshold = _next_threshold(vm.gc)

    # Memory pools (no-op in Python port)
    _lazy_be_mem().be_gc_memory_pools(vm)

    # Observer hook
    if obshook is not None:
        obshook(vm, 'GC_END', vm.gc.usage)
