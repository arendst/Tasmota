"""
Berry constant object support.
Mirrors: src/be_constobj.h

This module provides Python equivalents for the C macros used to define
constant (compile-time) Berry objects. These are used by solidification
output and precompiled objects. In C, these macros create static const
struct initializers; in Python, they create pre-configured object instances
with GC_CONST marking.

Original C macro code is included as comments for each definition.
"""

from berry_port.be_object import (
    BE_NIL, BE_INT, BE_REAL, BE_BOOL, BE_COMPTR, BE_INDEX,
    BE_STRING, BE_CLASS, BE_INSTANCE, BE_PROTO, BE_LIST, BE_MAP,
    BE_MODULE, BE_CLOSURE, BE_NTVFUNC, BE_STATIC,
    GC_CONST,
    bgcobject, bvalue, bstring, bcstring, bproto, bclosure,
    bclass, binstance, blist, bmap, bmapnode, bmapkey, bmodule,
    bvector,
    var_settype, var_setobj,
)


# ============================================================================
# be_const_header — set GC header fields for a constant object
# ============================================================================

# #define be_const_header(_t)
#     .next = NULL,
#     .type = (_t),
#     .marked = GC_CONST

def be_const_header(obj, type_):
    """Apply constant GC header to an object.

    In C this is a designated-initializer fragment; in Python we set
    the three bcommon_header fields on an already-allocated object.
    """
    obj.next = None
    obj.type = type_
    obj.marked = GC_CONST


# ============================================================================
# Constant bvalue constructors
# ============================================================================

# #define be_const_nil() {
#     .v.i = 0,
#     .type = BE_NIL
# }

def be_const_nil():
    """Create a constant nil bvalue."""
    v = bvalue()
    v.type = BE_NIL
    v.v = None
    return v


# #define be_const_int(_val) {
#     .v.i = (bint)(_val),
#     .type = BE_INT
# }

def be_const_int(val):
    """Create a constant integer bvalue."""
    v = bvalue()
    v.type = BE_INT
    v.v = int(val)
    return v


# #define be_const_real(_val) {
#     .v.r = (breal)(_val),
#     .type = BE_REAL
# }

def be_const_real(val):
    """Create a constant real bvalue."""
    v = bvalue()
    v.type = BE_REAL
    v.v = float(val)
    return v


# #define be_const_real_hex(_val) {
#     .v.p = (void*)(_val),
#     .type = BE_REAL
# }

def be_const_real_hex(val):
    """Create a constant real bvalue from a hex-encoded integer representation."""
    import struct
    v = bvalue()
    v.type = BE_REAL
    # Reinterpret integer bits as double
    v.v = struct.unpack('d', struct.pack('Q', val & 0xFFFFFFFFFFFFFFFF))[0]
    return v


# #define be_const_bool(_val) {
#     .v.b = (bbool)(_val),
#     .type = BE_BOOL
# }

def be_const_bool(val):
    """Create a constant boolean bvalue."""
    v = bvalue()
    v.type = BE_BOOL
    v.v = bool(val)
    return v


# #define be_const_str(_str) {
#     .v.s = (bstring*)(_str),
#     .type = BE_STRING
# }

def be_const_str(s):
    """Create a constant string bvalue referencing a bstring object."""
    v = bvalue()
    v.type = BE_STRING
    v.v = s
    return v


# #define be_const_var(_val) {
#     .v.i = (bint)(_val),
#     .type = BE_INDEX
# }

def be_const_var(val):
    """Create a constant index (instance variable slot) bvalue."""
    v = bvalue()
    v.type = BE_INDEX
    v.v = int(val)
    return v


# #define be_const_func(_func) {
#     .v.nf = (_func),
#     .type = BE_NTVFUNC
# }

def be_const_func(func):
    """Create a constant native function bvalue."""
    v = bvalue()
    v.type = BE_NTVFUNC
    v.v = func
    return v


# #define be_const_static_func(_func) {
#     .v.nf = (_func),
#     .type = BE_NTVFUNC | BE_STATIC
# }

def be_const_static_func(func):
    """Create a constant static native function bvalue."""
    v = bvalue()
    v.type = BE_NTVFUNC | BE_STATIC
    v.v = func
    return v


# #define be_const_comptr(_val) {
#     .v.c = (const void*)(_val),
#     .type = BE_COMPTR
# }

def be_const_comptr(val):
    """Create a constant comptr (C pointer) bvalue."""
    v = bvalue()
    v.type = BE_COMPTR
    v.v = val
    return v


# #define be_const_class(_class) {
#     .v.c = &(_class),
#     .type = BE_CLASS
# }

def be_const_class(cls):
    """Create a constant class bvalue referencing a bclass object."""
    v = bvalue()
    v.type = BE_CLASS
    v.v = cls
    return v


# #define be_const_closure(_closure) {
#     .v.c = &(_closure),
#     .type = BE_CLOSURE
# }

def be_const_closure(closure):
    """Create a constant closure bvalue referencing a bclosure object."""
    v = bvalue()
    v.type = BE_CLOSURE
    v.v = closure
    return v


# #define be_const_static_closure(_closure) {
#     .v.c = &(_closure),
#     .type = BE_CLOSURE | BE_STATIC
# }

def be_const_static_closure(closure):
    """Create a constant static closure bvalue."""
    v = bvalue()
    v.type = BE_CLOSURE | BE_STATIC
    v.v = closure
    return v


# #define be_const_module(_module) {
#     .v.c = &(_module),
#     .type = BE_MODULE
# }

def be_const_module(module):
    """Create a constant module bvalue referencing a bmodule object."""
    v = bvalue()
    v.type = BE_MODULE
    v.v = module
    return v


# #define be_const_simple_instance(_instance) {
#     .v.c = (_instance),
#     .type = BE_INSTANCE
# }

def be_const_simple_instance(instance):
    """Create a constant instance bvalue referencing a binstance object."""
    v = bvalue()
    v.type = BE_INSTANCE
    v.v = instance
    return v


# #define be_const_map(_map) {
#     .v.c = &(_map),
#     .type = BE_MAP
# }

def be_const_map(m):
    """Create a constant map bvalue referencing a bmap object."""
    v = bvalue()
    v.type = BE_MAP
    v.v = m
    return v


# #define be_const_list(_list) {
#     .v.c = &(_list),
#     .type = BE_LIST
# }

def be_const_list(lst):
    """Create a constant list bvalue referencing a blist object."""
    v = bvalue()
    v.type = BE_LIST
    v.v = lst
    return v


# ============================================================================
# Constant map key constructors
# ============================================================================

# #define be_const_key(_str, _next) {
#     .v.c = &be_const_str_##_str,
#     .type = BE_STRING,
#     .next = (uint32_t)(_next) & 0xFFFFFF
# }

def be_const_key(str_obj, next_idx):
    """Create a constant map key referencing a string with a chain pointer.

    str_obj: a bstring/bcstring object
    next_idx: next-node index in the map chain (24-bit, -1 means none)
    """
    k = bmapkey()
    k.v = str_obj
    k.type = BE_STRING
    k.next = next_idx & 0xFFFFFF
    return k


# #define be_const_key_weak(_str, _next) {
#     .v.c = &be_const_str_##_str,
#     .type = BE_STRING,
#     .next = (uint32_t)(_next) & 0xFFFFFF
# }

def be_const_key_weak(str_obj, next_idx):
    """Create a constant map key (weak variant, same behavior as be_const_key).

    The 'weak' variant is identical in behavior but is not detected by the
    coc (Compile-time Object Constructor) tool.
    """
    return be_const_key(str_obj, next_idx)


# #define be_const_key_int(_i, _next) {
#     .v.i = _i,
#     .type = BE_INT,
#     .next = (uint32_t)(_next) & 0xFFFFFF
# }

def be_const_key_int(i, next_idx):
    """Create a constant map key with an integer key."""
    k = bmapkey()
    k.v = int(i)
    k.type = BE_INT
    k.next = next_idx & 0xFFFFFF
    return k


# ============================================================================
# Constant string constructors
# ============================================================================

# #define be_define_const_str_weak(_name, _s, _len)
#     const bcstring be_const_str_##_name = {
#         .next = NULL,
#         .type = BE_STRING,
#         .marked = GC_CONST,
#         .extra = 0,
#         .slen = _len,
#         .hash = 0,
#         .s = _s
#     }

def be_define_const_str_weak(s, slen=None):
    """Create a constant weak string (bcstring).

    Returns a bcstring with GC_CONST marking and hash=0 (lazy-computed).
    If slen is not provided, it is computed from the string length,
    capped at 255 for long strings.
    """
    if slen is None:
        slen = min(len(s), 255)
    cs = bcstring(s=s, hash_val=0, slen=slen)
    cs.next = None
    cs.type = BE_STRING
    cs.marked = GC_CONST
    cs.extra = 0
    return cs


# #define be_str_weak(_str)
#     (bstring*) &be_const_str_##_str

def be_str_weak(str_obj):
    """Return a reference to a constant string object.

    In C this casts a bcstring pointer to bstring*. In Python we just
    return the object directly since there is no pointer casting needed.
    """
    return str_obj


# #define be_str_literal(_str)
#     be_nested_const_str(_str, 0, sizeof(_str)-1)

def be_str_literal(s):
    """Create a constant string from a literal.

    Equivalent to be_nested_const_str with hash=0 and len computed from
    the string content.
    """
    return be_nested_const_str(s, 0, len(s))


# ============================================================================
# Nested constant string (used inside bvalue initializers)
# ============================================================================

# #define be_nested_string(_str, _hash, _len)
#     {
#         { .s=(be_nested_const_str(_str, _hash, _len )) },
#         BE_STRING
#     }

def be_nested_string(s, hash_val, slen):
    """Create a bvalue wrapping a nested constant string."""
    v = bvalue()
    v.type = BE_STRING
    v.v = be_nested_const_str(s, hash_val, slen)
    return v


# be_nested_const_str is referenced by several macros but defined elsewhere
# in the C codebase as an inline struct literal. We provide the Python equivalent.

def be_nested_const_str(s, hash_val, slen):
    """Create a bcstring for use in nested constant object definitions.

    This is the Python equivalent of the C compound literal:
        (bstring*) & (const bcstring) { ... }
    """
    cs = bcstring(s=s, hash_val=hash_val, slen=min(slen, 255))
    cs.next = None
    cs.type = BE_STRING
    cs.marked = GC_CONST
    cs.extra = 0
    return cs


# #define be_nested_str(_name_)
#     {
#         { .s=((bstring*)&be_const_str_##_name_) },
#         BE_STRING
#     }

def be_nested_str(str_obj):
    """Create a bvalue wrapping a reference to a named constant string."""
    v = bvalue()
    v.type = BE_STRING
    v.v = str_obj
    return v


# #define be_nested_str_weak(_name_)
#     {
#         { .s=((bstring*)&be_const_str_##_name_) },
#         BE_STRING
#     }

def be_nested_str_weak(str_obj):
    """Create a bvalue wrapping a weak constant string reference.

    Identical to be_nested_str but not detected by coc.
    """
    return be_nested_str(str_obj)


# #define be_nested_str_long(_name_)
#     {
#         { .s=((bstring*)&be_const_str_##_name_) },
#         BE_STRING
#     }

def be_nested_str_long(str_obj):
    """Create a bvalue wrapping a long constant string reference.

    Identical to be_nested_str but for long strings.
    """
    return be_nested_str(str_obj)


# ============================================================================
# Nested constant key (for map node keys with hash and chain)
# ============================================================================

# #define be_nested_key(_str, _hash, _len, _next)
#     {
#         { .s=(be_nested_const_str(_str, _hash, _len )) },
#         BE_STRING,
#         (uint32_t)(_next) & 0xFFFFFF
#     }

def be_nested_key(s, hash_val, slen, next_idx):
    """Create a constant map key with an inline constant string."""
    k = bmapkey()
    k.v = be_nested_const_str(s, hash_val, slen)
    k.type = BE_STRING
    k.next = next_idx & 0xFFFFFF
    return k


# ============================================================================
# Constant composite object constructors
# ============================================================================

# #define be_nested_map(_size, _slots)
#     & (const bmap) {
#         be_const_header(BE_MAP),
#         .slots = _slots,
#         .lastfree = NULL,
#         .size = _size,
#         .count = _size
#     }

def be_nested_map(size, slots):
    """Create a constant map with pre-built slots.

    size: number of slots (also used as count for constant maps)
    slots: list of bmapnode objects
    """
    m = bmap()
    be_const_header(m, BE_MAP)
    m.slots = slots
    m.lastfree = None
    m.size = size
    m.count = size
    return m


# #define be_nested_list(_size, _items)
#     & (const blist) {
#         be_const_header(BE_LIST),
#         .count = _size,
#         .capacity = _size,
#         .data = _items
#     }

def be_nested_list(size, items):
    """Create a constant list with pre-built items.

    size: number of items
    items: list of bvalue objects
    """
    lst = blist()
    be_const_header(lst, BE_LIST)
    lst.count = size
    lst.capacity = size
    lst.data = items
    return lst


# #define be_nested_simple_instance(_class_ptr, _members)
#     & (const binstance) {
#         be_const_header(BE_INSTANCE),
#         .super = NULL,
#         .sub = NULL,
#         ._class = (bclass*) _class_ptr,
#         .members = _members
#     }

def be_nested_simple_instance(class_ptr, members):
    """Create a constant simple instance (no super/sub chain).

    Primarily used for solidified list and map instances.
    """
    inst = binstance()
    be_const_header(inst, BE_INSTANCE)
    inst.super = None
    inst.sub = None
    inst._class = class_ptr
    inst.members = members
    return inst


# ============================================================================
# Constant proto (used by solidification output)
# ============================================================================

# #define be_nested_proto(_nstack, _argc, _varg, _has_upvals,
#                         _upvals, _ktab, _subprotos, _code,
#                         _name, _source, _codesize, _nconst,
#                         _nproto, _extra)
# In C this is a compound literal creating a bproto. In Python we build
# a bproto object with the same fields.

def be_nested_proto(nstack, argc, varg, has_upvals,
                    upvals, ktab, subprotos, code,
                    name, source, codesize, nconst,
                    nproto):
    """Create a constant function prototype for solidified code.

    Parameters mirror the C macro fields:
        nstack:    number of stack slots needed
        argc:      number of arguments
        varg:      vararg/method/static flags (BE_VA_*)
        has_upvals: whether this proto has upvalues
        upvals:    list of bupvaldesc
        ktab:      list of bvalue constants
        subprotos: list of bproto sub-prototypes
        code:      list of binstruction (uint32) values
        name:      bstring for function name
        source:    bstring for source filename
        codesize:  number of instructions
        nconst:    number of constants
        nproto:    number of sub-prototypes
    """
    p = bproto()
    be_const_header(p, BE_PROTO)
    p.nstack = nstack
    p.argc = argc
    p.varg = varg
    p.nupvals = len(upvals) if upvals else 0
    p.upvals = upvals if upvals else []
    p.ktab = ktab if ktab else []
    p.ptab = subprotos if subprotos else []
    p.code = code if code else []
    p.name = name
    p.source = source
    p.codesize = codesize
    p.nconst = nconst
    p.nproto = nproto
    # Debug info not populated for constant protos
    p.lineinfo = None
    p.nlineinfo = 0
    p.varinfo = None
    p.nvarinfo = 0
    return p


# ============================================================================
# Constant closure (wraps a constant proto)
# ============================================================================

# #define be_local_closure(_name, _proto)
# In C this creates a static const bclosure. In Python we build a bclosure.

def be_local_closure(proto):
    """Create a constant closure wrapping a constant proto.

    In C: be_local_closure(name, proto_ptr) creates a static const bclosure.
    In Python we just wrap the proto in a bclosure with GC_CONST marking.
    """
    cl = bclosure()
    be_const_header(cl, BE_CLOSURE)
    cl.proto = proto
    cl.nupvals = proto.nupvals if proto else 0
    cl.upvals = [None] * cl.nupvals
    return cl


# ============================================================================
# Constant class
# ============================================================================

# #define be_local_class(_name, _nvar, _super, _map, _cname)
#     const bclass be_class_##_name = {
#         be_const_header(BE_CLASS),
#         .nvar = _nvar,
#         .super = (bclass*)_super,
#         .members = (bmap*)_map,
#         .name = _cname
#     }

def be_local_class(nvar, super_class, members_map, name):
    """Create a constant class definition.

    nvar:        number of instance variables
    super_class: superclass (bclass) or None
    members_map: members map (bmap) or None
    name:        class name (bstring)
    """
    cl = bclass()
    be_const_header(cl, BE_CLASS)
    cl.nvar = nvar
    cl.super = super_class
    cl.members = members_map
    cl.name = name
    return cl


# ============================================================================
# Constant module
# ============================================================================

# #define be_local_module(_c_name, _module_name, _map)
#     static const bmodule m_lib##_c_name = {
#         be_const_header(BE_MODULE),
#         .table = (bmap*)_map,
#         .info.name = _module_name
#     }

def be_local_module(module_name, members_map):
    """Create a constant module definition.

    module_name: module name string
    members_map: members map (bmap)
    """
    m = bmodule()
    be_const_header(m, BE_MODULE)
    m.table = members_map
    m.info = module_name  # info.name in C — stores the module name string
    return m


# ============================================================================
# Constant map with slots
# ============================================================================

# #define be_define_const_map_slots(_name)
#     const bmapnode _name##_slots[] =
#
# #define be_define_const_map(_name, _size)
#     const bmap _name = {
#         be_const_header(BE_MAP),
#         .slots = (bmapnode*)_name##_slots,
#         .lastfree = NULL,
#         .size = _size,
#         .count = _size
#     }

def be_define_const_map(slots, size):
    """Create a constant map from pre-defined slots.

    slots: list of bmapnode objects
    size:  number of slots (= count for constant maps)
    """
    return be_nested_map(size, slots)


# ============================================================================
# Constant class (define variant)
# ============================================================================

# #define be_define_const_class(_name, _nvar, _super, _name_)
#     const bclass _name = {
#         be_const_header(BE_CLASS),
#         .nvar = _nvar,
#         .super = _super,
#         .members = (bmap*)&_name##_map,
#         .name = (bstring*)&be_const_str_##_name_
#     }

def be_define_const_class(nvar, super_class, members_map, name):
    """Create a constant class (define variant).

    Equivalent to be_local_class.
    """
    return be_local_class(nvar, super_class, members_map, name)


# ============================================================================
# Constant module (define variant)
# ============================================================================

# #define be_define_const_module(_name, _name_)
#     const bmodule _name = {
#         be_const_header(BE_MODULE),
#         .table = (bmap*)&_name##_map,
#         .info.name = _name_,
#     }

def be_define_const_module(module_name, members_map):
    """Create a constant module (define variant).

    Equivalent to be_local_module.
    """
    return be_local_module(module_name, members_map)


# ============================================================================
# Constant vector
# ============================================================================

# #define be_define_const_vector(_name, _data, _size)
#     const bvector _name = {
#         .capacity = _size,
#         .size = sizeof(bvalue),
#         .count = _size,
#         .data = (void*)_data,
#         .end = (void*)(_data + (_size) - 1)
#     }

def be_define_const_vector(data, size):
    """Create a constant vector with pre-built data.

    data: list of elements
    size: number of elements
    """
    vec = bvector()
    vec.capacity = size
    vec.size = 1  # element size placeholder (not byte-level in Python)
    vec.count = size
    vec.data = data
    vec.end = size - 1 if size > 0 else 0
    return vec


# ============================================================================
# Constant native module
# ============================================================================

# #define be_define_const_native_module(_module)
#     const bntvmodule_t be_native_module(_module) = {
#         .name = #_module,
#         .attrs = NULL,
#         .size = 0,
#         .module = (bmodule*)&(m_lib##_module)
#     }

def be_define_const_native_module(name, module):
    """Create a constant native module registration entry.

    name:   module name string
    module: bmodule object
    """
    from berry_port.be_object import bntvmodule
    nm = bntvmodule(name=name, attrs=None, size=0, module=module)
    return nm


# ============================================================================
# Constant bytes instance
# ============================================================================

# #define be_define_const_bytes(_name, ...)
#     const binstance_arg3 be_const_instance_##_name = {
#         be_const_header(BE_INSTANCE),
#         .super = NULL,
#         .sub = NULL,
#         ._class = (bclass*) &be_class_bytes,
#         .members = {
#             {.v.c = (const void*) & (const uint8_t[]) { __VA_ARGS__ },
#             .type = BE_COMPTR },
#             be_const_int(sizeof(#_name) / 2),
#             be_const_int(BYTES_SIZE_SOLIDIFIED)
#         }
#     }

BYTES_SIZE_SOLIDIFIED = -1  # sentinel value for solidified bytes

def be_define_const_bytes(name, data, bytes_class=None):
    """Create a constant bytes instance.

    name:        name string (used to compute length in C via sizeof)
    data:        bytes or bytearray with the raw data
    bytes_class: reference to the bytes bclass (be_class_bytes)
    """
    inst = binstance()
    be_const_header(inst, BE_INSTANCE)
    inst.super = None
    inst.sub = None
    inst._class = bytes_class
    inst.members = [
        be_const_comptr(data),
        be_const_int(len(data)),
        be_const_int(BYTES_SIZE_SOLIDIFIED),
    ]
    return inst


# #define be_define_const_bytes_empty()
#     const binstance_arg3 be_const_instance_ = { ... }

def be_define_const_bytes_empty(bytes_class=None):
    """Create a constant empty bytes instance."""
    inst = binstance()
    be_const_header(inst, BE_INSTANCE)
    inst.super = None
    inst.sub = None
    inst._class = bytes_class
    inst.members = [
        be_const_comptr(b'\x00'),
        be_const_int(0),
        be_const_int(BYTES_SIZE_SOLIDIFIED),
    ]
    return inst


# #define be_const_bytes_instance(_bytes) {
#     .v.c = &be_const_instance_##_bytes,
#     .type = BE_INSTANCE
# }

def be_const_bytes_instance(bytes_inst):
    """Create a bvalue referencing a constant bytes instance."""
    return be_const_simple_instance(bytes_inst)
