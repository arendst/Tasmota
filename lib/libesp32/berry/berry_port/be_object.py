"""
Berry core type definitions.
Mirrors: src/be_object.h, with supporting types from be_vm.h, be_class.h,
         be_map.h, be_list.h, be_func.h, be_module.h, be_string.h, be_gc.h,
         be_exec.h

This module defines all Berry value types, GC object structures, and the
var_* helper functions that mirror the C macros in be_object.h.
Original C code is included as comments for each definition.
"""

from berry_port.berry_conf import (
    BE_DEBUG_SOURCE_FILE,
    BE_DEBUG_RUNTIME_INFO,
    BE_DEBUG_VAR_INFO,
    BE_USE_STR_HASH_CACHE,
    BE_USE_PERF_COUNTERS,
    BE_USE_DEBUG_HOOK,
    BE_USE_PREPROCESSOR,
)

# ============================================================================
# Basic type constants (from be_object.h)
# ============================================================================

# /* basic types, do not change value */
# #define BE_NONE        -256
# #define BE_NIL          0
# #define BE_INT          1
# #define BE_REAL         2
# #define BE_BOOL         3
# #define BE_COMPTR       4
# #define BE_INDEX        5
# #define BE_FUNCTION     6
BE_NONE = -256
BE_NIL = 0
BE_INT = 1
BE_REAL = 2
BE_BOOL = 3
BE_COMPTR = 4
BE_INDEX = 5
BE_FUNCTION = 6

# #define BE_GCOBJECT     16
# #define BE_GCOBJECT_MAX (3<<5)   /* 96 */
BE_GCOBJECT = 16
BE_GCOBJECT_MAX = 3 << 5  # 96

# #define BE_STRING       16
# #define BE_CLASS        17
# #define BE_INSTANCE     18
# #define BE_PROTO        19
# #define BE_LIST         20
# #define BE_MAP          21
# #define BE_MODULE       22
# #define BE_COMOBJ       23
BE_STRING = 16
BE_CLASS = 17
BE_INSTANCE = 18
BE_PROTO = 19
BE_LIST = 20
BE_MAP = 21
BE_MODULE = 22
BE_COMOBJ = 23

# #define BE_NTVFUNC      ((0 << 5) | BE_FUNCTION)    /* 6 */
# #define BE_CLOSURE      ((1 << 5) | BE_FUNCTION)    /* 38 */
# #define BE_NTVCLOS      ((2 << 5) | BE_FUNCTION)    /* 70 */
# #define BE_CTYPE_FUNC   ((3 << 5) | BE_FUNCTION)    /* 102 */
# #define BE_STATIC       (1 << 7)                    /* 128 */
BE_NTVFUNC = (0 << 5) | BE_FUNCTION       # 6
BE_CLOSURE = (1 << 5) | BE_FUNCTION       # 38
BE_NTVCLOS = (2 << 5) | BE_FUNCTION       # 70
BE_CTYPE_FUNC = (3 << 5) | BE_FUNCTION    # 102
BE_STATIC = 1 << 7                        # 128

# ============================================================================
# Varg flags (from be_object.h)
# ============================================================================

# /* values for bproto.varg */
# #define BE_VA_VARARG            (1 << 0)
# #define BE_VA_METHOD            (1 << 1)
# #define BE_VA_STATICMETHOD      (1 << 2)
# #define BE_VA_SHARED_KTAB       (1 << 3)
# #define BE_VA_NOCOMPACT         (1 << 4)
BE_VA_VARARG = 1 << 0
BE_VA_METHOD = 1 << 1
BE_VA_STATICMETHOD = 1 << 2
BE_VA_SHARED_KTAB = 1 << 3
BE_VA_NOCOMPACT = 1 << 4

# ============================================================================
# GC mark constants (from be_gc.h)
# ============================================================================

# typedef enum {
#     GC_WHITE = 0x00,
#     GC_GRAY = 0x01,
#     GC_DARK = 0x02,
#     GC_FIXED = 0x04,
#     GC_CONST = 0x08
# } bgcmark;
GC_WHITE = 0x00
GC_GRAY = 0x01
GC_DARK = 0x02
GC_FIXED = 0x04
GC_CONST = 0x08

# ============================================================================
# Error codes (from berry.h)
# ============================================================================

# enum berrorcode {
#     BE_OK = 0, BE_EXIT, BE_MALLOC_FAIL, BE_EXCEPTION,
#     BE_SYNTAX_ERROR, BE_EXEC_ERROR, BE_IO_ERROR
# };
BE_OK = 0
BE_EXIT = 1
BE_MALLOC_FAIL = 2
BE_EXCEPTION = 3
BE_SYNTAX_ERROR = 4
BE_EXEC_ERROR = 5
BE_IO_ERROR = 6

# ============================================================================
# Native module member type specifiers (from berry.h)
# ============================================================================

# #define BE_CNIL        0
# #define BE_CINT        1
# #define BE_CREAL       2
# #define BE_CBOOL       3
# #define BE_CFUNCTION   4
# #define BE_CSTRING     5
# #define BE_CMODULE     6
BE_CNIL = 0
BE_CINT = 1
BE_CREAL = 2
BE_CBOOL = 3
BE_CFUNCTION = 4
BE_CSTRING = 5
BE_CMODULE = 6

# ============================================================================
# Module name flag (from be_module.h)
# ============================================================================

# #define BE_MODULE_NAME    1
BE_MODULE_NAME = 1

# ============================================================================
# Call frame status flags (from be_vm.h)
# ============================================================================

# #define NONE_FLAG           0
# #define BASE_FRAME          (1 << 0)
# #define PRIM_FUNC           (1 << 1)
NONE_FLAG = 0
BASE_FRAME = 1 << 0
PRIM_FUNC = 1 << 1

# ============================================================================
# Compilation option masks (from be_vm.h)
# ============================================================================

# typedef enum {
#     COMP_NAMED_GBL = 0x00,
#     COMP_STRICT = 0x01,
#     COMP_GC_DEBUG = 0x02,
# } compoptmask;
COMP_NAMED_GBL = 0x00
COMP_STRICT = 0x01
COMP_GC_DEBUG = 0x02

# ============================================================================
# Class/instance macros (from be_class.h)
# ============================================================================

# #define MT_VARIABLE     BE_INDEX
# #define MT_METHOD       BE_CLOSURE
# #define MT_PRIMMETHOD   BE_NTVFUNC
MT_VARIABLE = BE_INDEX
MT_METHOD = BE_CLOSURE
MT_PRIMMETHOD = BE_NTVFUNC

# ============================================================================
# Short string max length (from be_string.h)
# ============================================================================

# #define SHORT_STR_MAX_LEN   64
SHORT_STR_MAX_LEN = 64


# ============================================================================
# GC object base class (from be_object.h)
# ============================================================================

# typedef struct bgcobject {
#     bcommon_header;   /* next, type, marked */
# } bgcobject;
class bgcobject:
    """Base GC object with bcommon_header: next, type, marked."""
    __slots__ = ('next', 'type', 'marked')

    def __init__(self):
        self.next = None       # bgcobject* next
        self.type = 0          # bbyte type
        self.marked = 0        # bbyte marked


# ============================================================================
# String structures (from be_object.h, be_string.h)
# ============================================================================

# typedef struct bstring {
#     bstring_header;   /* next, type, marked, extra, slen */
# } bstring;
class bstring(bgcobject):
    """Berry string header: bcommon_header + extra + slen."""
    __slots__ = ('extra', 'slen', '_s', '_hash', 'llen')

    def __init__(self):
        super().__init__()
        self.type = BE_STRING
        self.extra = 0         # bbyte extra
        self.slen = 0          # bbyte slen (255 means long string)
        self._s = ""           # the actual string data
        self._hash = 0         # cached hash value
        self.llen = 0          # long string length (used when slen == 255)


# typedef struct {
#     bstring_header;
#     uint32_t hash;
#     /* char s[]; */
# } bsstring;
# (Short string — interned, slen <= 64)
# Represented by bstring with slen <= 64.

# typedef struct {
#     bstring str;
#     int llen;
#     /* char s[]; */
# } blstring;
# (Long string — slen == 255, actual length in llen)
# Represented by bstring with slen == 255 and llen set.

# typedef struct {
#     bstring_header;
#     uint32_t hash;
#     const char *s;
# } bcstring;
class bcstring(bgcobject):
    """Const string with external pointer (for solidified constant strings)."""
    __slots__ = ('extra', 'slen', 'hash', 's')

    def __init__(self, s="", hash_val=0, slen=0):
        super().__init__()
        self.type = BE_STRING
        self.marked = GC_CONST
        self.extra = 0         # bbyte extra
        self.slen = slen       # bbyte slen
        self.hash = hash_val   # uint32_t hash
        self.s = s             # const char *s


# ============================================================================
# Value data union and bvalue (from be_object.h)
# ============================================================================

# union bvaldata {
#     bbool b;
#     breal r;
#     bint i;
#     void *p;
#     const void *c;
#     bstring *s;
#     bgcobject *gc;
#     bntvfunc nf;
# };
#
# typedef struct bvalue {
#     union bvaldata v;
#     int type;
# } bvalue;
class bvalue:
    """Berry value: tagged union with type and value fields.

    The 'v' attribute holds the actual value (Python object).
    The 'type' attribute holds the Berry type tag.
    """
    __slots__ = ('type', 'v')

    def __init__(self):
        self.type = BE_NIL
        self.v = None


# ============================================================================
# Upvalue descriptor (from be_object.h)
# ============================================================================

# typedef struct {
# #if BE_DEBUG_VAR_INFO
#     bstring *name;
# #endif
#     bbyte instack;
#     bbyte idx;
# } bupvaldesc;
class bupvaldesc:
    """Upvalue descriptor used during compilation."""
    __slots__ = ('name', 'instack', 'idx') if BE_DEBUG_VAR_INFO else ('instack', 'idx')

    def __init__(self):
        if BE_DEBUG_VAR_INFO:
            self.name = None   # bstring* name
        self.instack = 0       # bbyte instack
        self.idx = 0           # bbyte idx


# ============================================================================
# Line info (from be_object.h)
# ============================================================================

# typedef struct {
# #if BE_DEBUG_RUNTIME_INFO > 1
#     uint16_t linenumber;
#     uint16_t endpc;
# #else
#     int linenumber;
#     int endpc;
# #endif
# } blineinfo;
class blineinfo:
    """Debug line information for bytecode ranges."""
    __slots__ = ('linenumber', 'endpc')

    def __init__(self):
        self.linenumber = 0    # int or uint16_t linenumber
        self.endpc = 0         # int or uint16_t endpc


# ============================================================================
# Variable info (from be_object.h)
# ============================================================================

# typedef struct {
#     bstring *name;
# #if BE_DEBUG_RUNTIME_INFO > 1
#     uint16_t beginpc;
#     uint16_t endpc;
# #else
#     int beginpc;
#     int endpc;
# #endif
# } bvarinfo;
class bvarinfo:
    """Debug variable information for scope tracking."""
    __slots__ = ('name', 'beginpc', 'endpc')

    def __init__(self):
        self.name = None       # bstring* name
        self.beginpc = 0       # int or uint16_t beginpc
        self.endpc = 0         # int or uint16_t endpc


# ============================================================================
# Function prototype (from be_object.h)
# ============================================================================

# typedef struct bproto {
#     bcommon_header;
#     bbyte nstack;
#     bbyte nupvals;
#     bbyte argc;
#     bbyte varg;
#     int16_t codesize;
#     int16_t nconst;
#     int16_t nproto;
#     bgcobject *gray;
#     bupvaldesc *upvals;
#     bvalue *ktab;
#     struct bproto **ptab;
#     binstruction *code;
#     bstring *name;
# #if BE_DEBUG_SOURCE_FILE
#     bstring *source;
# #endif
# #if BE_DEBUG_RUNTIME_INFO
#     blineinfo *lineinfo;
#     int nlineinfo;
# #endif
# #if BE_DEBUG_VAR_INFO
#     bvarinfo *varinfo;
#     int nvarinfo;
# #endif
# } bproto;
class bproto(bgcobject):
    """Function prototype: bytecode, constants, upvalue descriptors, sub-protos."""
    __slots__ = (
        'nstack', 'nupvals', 'argc', 'varg',
        'codesize', 'nconst', 'nproto',
        'upvals', 'ktab', 'ptab', 'code', 'name',
        'source', 'lineinfo', 'nlineinfo', 'varinfo', 'nvarinfo',
    )

    def __init__(self):
        super().__init__()
        self.type = BE_PROTO
        self.nstack = 0        # bbyte nstack
        self.nupvals = 0       # bbyte nupvals
        self.argc = 0          # bbyte argc
        self.varg = 0          # bbyte varg
        self.codesize = 0      # int16_t codesize
        self.nconst = 0        # int16_t nconst
        self.nproto = 0        # int16_t nproto
        self.upvals = None     # bupvaldesc* upvals (list of bupvaldesc)
        self.ktab = None       # bvalue* ktab (list of bvalue)
        self.ptab = None       # bproto** ptab (list of bproto)
        self.code = None       # binstruction* code (list of uint32)
        self.name = None       # bstring* name
        # conditional fields — always present in Python, guarded at usage
        self.source = None     # bstring* source (BE_DEBUG_SOURCE_FILE)
        self.lineinfo = None   # blineinfo* lineinfo (BE_DEBUG_RUNTIME_INFO)
        self.nlineinfo = 0     # int nlineinfo
        self.varinfo = None    # bvarinfo* varinfo (BE_DEBUG_VAR_INFO)
        self.nvarinfo = 0      # int nvarinfo


# ============================================================================
# Berry closure (from be_object.h)
# ============================================================================

# struct bclosure {
#     bcommon_header;
#     bbyte nupvals;
#     bgcobject *gray;
#     bproto *proto;
#     bupval *upvals[1];
# };
class bclosure(bgcobject):
    """Berry closure: pairs a bproto with captured upvalues."""
    __slots__ = ('nupvals', 'proto', 'upvals')

    def __init__(self):
        super().__init__()
        self.type = BE_CLOSURE
        self.nupvals = 0       # bbyte nupvals
        self.proto = None      # bproto* proto
        self.upvals = []       # bupval*[] (list of bupval)


# ============================================================================
# Native closure (from be_object.h)
# ============================================================================

# struct bntvclos {
#     bcommon_header;
#     bbyte nupvals;
#     bgcobject *gray;
#     bntvfunc f;
# };
class bntvclos(bgcobject):
    """C native function or closure."""
    __slots__ = ('nupvals', 'f', 'upvals')

    def __init__(self):
        super().__init__()
        self.type = BE_NTVCLOS
        self.nupvals = 0       # bbyte nupvals
        self.f = None          # bntvfunc f (Python callable)
        self.upvals = []       # bupval*[] (accessed via be_ntvclos_upval)


# ============================================================================
# Common object (from be_object.h)
# ============================================================================

# typedef struct {
#     bcommon_header;
#     void *data;
#     bntvfunc destroy;
# } bcommomobj;
class bcommomobj(bgcobject):
    """Common object with data pointer and destroy callback."""
    __slots__ = ('data', 'destroy')

    def __init__(self):
        super().__init__()
        self.type = BE_COMOBJ
        self.data = None       # void* data
        self.destroy = None    # bntvfunc destroy


# ============================================================================
# Map key and node (from be_map.h)
# ============================================================================

# typedef struct bmapkey {
#     union bvaldata v;
#     uint32_t type:8;
#     uint32_t next:24;
# } bmapkey;
class bmapkey:
    """Map key: value union + packed type (8 bits) and next (24 bits)."""
    __slots__ = ('v', 'type', 'next')

    def __init__(self):
        self.v = None          # union bvaldata v
        self.type = BE_NIL     # uint32_t type:8
        self.next = 0          # uint32_t next:24 (0xFFFFFF = no next)


# typedef struct bmapnode {
#     bmapkey key;
#     bvalue value;
# } bmapnode;
class bmapnode:
    """Map node: key-value pair in the hash table."""
    __slots__ = ('key', 'value')

    def __init__(self):
        self.key = bmapkey()
        self.value = bvalue()


# ============================================================================
# Map (from be_map.h)
# ============================================================================

# struct bmap {
#     bcommon_header;
#     bgcobject *gray;
#     bmapnode *slots;
#     bmapnode *lastfree;
#     int size;
#     int count;
# };
class bmap(bgcobject):
    """Berry map (hash table) with open addressing and chaining."""
    __slots__ = ('slots', 'lastfree', 'size', 'count')

    def __init__(self):
        super().__init__()
        self.type = BE_MAP
        self.slots = None      # bmapnode* slots (list of bmapnode)
        self.lastfree = 0      # int index (replaces bmapnode* lastfree)
        self.size = 0          # int size
        self.count = 0         # int count


# ============================================================================
# List (from be_list.h)
# ============================================================================

# struct blist {
#     bcommon_header;
#     bgcobject *gray;
#     int count, capacity;
#     bvalue *data;
# };
class blist(bgcobject):
    """Berry list container."""
    __slots__ = ('count', 'capacity', 'data')

    def __init__(self):
        super().__init__()
        self.type = BE_LIST
        self.count = 0         # int count
        self.capacity = 0      # int capacity
        self.data = None       # bvalue* data (list of bvalue)


# ============================================================================
# Class (from be_class.h)
# ============================================================================

# struct bclass {
#     bcommon_header;
#     uint16_t nvar;
#     struct bclass *super;
#     bmap *members;
#     bstring *name;
#     bgcobject *gray;
# };
class bclass(bgcobject):
    """Berry class with single inheritance."""
    __slots__ = ('nvar', 'super', 'members', 'name')

    def __init__(self):
        super().__init__()
        self.type = BE_CLASS
        self.nvar = 0          # uint16_t nvar
        self.super = None      # bclass* super
        self.members = None    # bmap* members
        self.name = None       # bstring* name


# ============================================================================
# Instance (from be_class.h)
# ============================================================================

# struct binstance {
#     bcommon_header;
#     struct binstance *super;
#     struct binstance *sub;
#     bclass *_class;
#     bgcobject *gray;
#     bvalue members[1];
# };
class binstance(bgcobject):
    """Berry class instance with variable-length member array."""
    __slots__ = ('super', 'sub', '_class', 'members')

    def __init__(self):
        super().__init__()
        self.type = BE_INSTANCE
        self.super = None      # binstance* super
        self.sub = None        # binstance* sub
        self._class = None     # bclass* _class
        self.members = []      # bvalue[] members


# ============================================================================
# Module (from be_module.h)
# ============================================================================

# typedef struct bmodule {
#     bcommon_header;
#     bmap *table;
#     union infodata {
#         const bntvmodule_t *native;
#         const char *name;
#         const bstring *sname;
#     } info;
#     bgcobject *gray;
# } bmodule;
class bmodule(bgcobject):
    """Berry module with attribute table."""
    __slots__ = ('table', 'info')

    def __init__(self):
        super().__init__()
        self.type = BE_MODULE
        self.table = None      # bmap* table
        self.info = None       # union infodata (native/name/sname)


# ============================================================================
# Upvalue (from be_vm.h)
# ============================================================================

# struct bupval {
#     bvalue* value;
#     union {
#         bvalue value;
#         struct bupval* next;
#     } u;
#     int refcnt;
# };
class bupval:
    """Upvalue: captures a variable from an enclosing scope.

    When open, 'value' points to a stack slot (by index).
    When closed, 'value' points to u_value (internal storage).
    """
    __slots__ = ('value', 'u_value', 'u_next', 'refcnt')

    def __init__(self):
        self.value = None      # bvalue* value (reference to stack slot or u_value)
        self.u_value = bvalue()  # union { bvalue value; ... } u
        self.u_next = None     # union { ...; bupval* next; } u
        self.refcnt = 0        # int refcnt


# ============================================================================
# Vector / Stack (from be_object.h)
# ============================================================================

# typedef struct bvector {
#     int capacity;
#     int size;
#     int count;
#     void *data;
#     void *end;
# } bvector, bstack;
class bvector:
    """Dynamic array used throughout the interpreter. Also aliased as bstack."""
    __slots__ = ('capacity', 'size', 'count', 'data', 'end', '_mem')

    def __init__(self):
        self.capacity = 0      # int capacity
        self.size = 0          # int size (element byte size — informational in Python)
        self.count = 0         # int count
        self.data = []         # void* data (Python list)
        self.end = 0           # void* end (index of last element)
        self._mem = None       # backing memory for gc.usage tracking

# bstack is an alias for bvector
bstack = bvector


# ============================================================================
# GC state (from be_vm.h)
# ============================================================================

# struct bgc {
#     bgcobject *list;
#     bgcobject *gray;
#     bgcobject *fixed;
#     struct gc16_t* pool16;
#     struct gc32_t* pool32;
#     size_t usage;
#     size_t threshold;
#     bbyte steprate;
#     bbyte status;
# };
class bgc:
    """Minimal GC state — only tracks memory usage."""
    __slots__ = ('usage',)

    def __init__(self):
        self.usage = 0         # size_t usage


# ============================================================================
# String table (from be_vm.h)
# ============================================================================

# struct bstringtable {
#     bstring **table;
#     int count;
#     int size;
# };
class bstringtable:
    """Hash table for short string interning."""
    __slots__ = ('table', 'count', 'size')

    def __init__(self):
        self.table = None      # bstring** table (list of bstring or None)
        self.count = 0         # int count
        self.size = 0          # int size


# ============================================================================
# Module descriptor (from be_vm.h)
# ============================================================================

# struct bmoduledesc {
#     bmap *loaded;
#     blist *path;
# };
class bmoduledesc:
    """Module system state: loaded modules and search paths."""
    __slots__ = ('loaded', 'path')

    def __init__(self):
        self.loaded = None     # bmap* loaded
        self.path = None       # blist* path


# ============================================================================
# Global descriptor (from be_vm.h)
# ============================================================================

# typedef struct {
#     struct {
#         bmap *vtab;
#         bvector vlist;
#     } global;
#     struct {
#         bmap *vtab;
#         bvector vlist;
#     } builtin;
# } bglobaldesc;
class _bglobaldesc_scope:
    """Inner scope for global/builtin variable tables."""
    __slots__ = ('vtab', 'vlist')

    def __init__(self):
        self.vtab = None       # bmap* vtab
        self.vlist = bvector() # bvector vlist


class bglobaldesc:
    """Global and built-in variable descriptor."""
    __slots__ = ('global_', 'builtin')

    def __init__(self):
        self.global_ = _bglobaldesc_scope()   # global scope
        self.builtin = _bglobaldesc_scope()   # builtin scope


# ============================================================================
# Call frame (from be_vm.h)
# ============================================================================

# typedef struct {
#     bvalue *func;
#     bvalue *top;
#     bvalue *reg;
#     binstruction *ip;
# #if BE_USE_DEBUG_HOOK
#     blineinfo *lineinfo;
# #endif
#     int status;
# } bcallframe;
class bcallframe:
    """Function call frame: tracks registers and instruction pointer."""
    __slots__ = (
        'func', 'top', 'reg', 'ip', 'ip_idx', 'status', 'lineinfo',
    )

    def __init__(self):
        self.func = 0          # int index into stack (replaces bvalue* func)
        self.top = 0           # int index into stack (replaces bvalue* top)
        self.reg = 0           # int index into stack (replaces bvalue* reg)
        self.ip = 0            # code list reference (replaces binstruction* ip)
        self.ip_idx = 0        # int index into code list
        self.status = 0        # int status
        self.lineinfo = None   # blineinfo* lineinfo (BE_USE_DEBUG_HOOK)


# ============================================================================
# Call snapshot (from be_vm.h)
# ============================================================================

# typedef struct {
#     bvalue func;
#     binstruction *ip;
# } bcallsnapshot;
class bcallsnapshot:
    """Saved call state for stack tracing."""
    __slots__ = ('func', 'ip')

    def __init__(self):
        self.func = bvalue()   # bvalue func
        self.ip = 0            # int index (replaces binstruction* ip)


# ============================================================================
# Exception frame (from be_exec.h)
# ============================================================================

# struct bexecptframe {
#     struct blongjmp errjmp;
#     int depth;
#     binstruction *ip;
#     int refcount;
# };
class bexecptframe:
    """Exception frame for Berry try/except blocks.

    In the Python port, errjmp is replaced by Python exception handling.
    """
    __slots__ = ('errjmp', 'depth', 'ip', 'ip_code', 'refcount')

    def __init__(self):
        self.errjmp = None     # replaced by Python exception mechanism
        self.depth = 0         # int depth (call stack depth)
        self.ip = 0            # int index (replaces binstruction* ip)
        self.ip_code = None    # code array reference (for resume)
        self.refcount = 0      # int refcount


# ============================================================================
# Native module object (from berry.h)
# ============================================================================

# typedef struct bntvmodobj {
#     const char *name;
#     int type;
#     union value { bint i; breal r; bbool b; bntvfunc f; const char *s; const void *o; } u;
# } bntvmodobj_t;
class bntvmodobj:
    """Native module attribute entry."""
    __slots__ = ('name', 'type', 'u')

    def __init__(self, name=None, type_=BE_CNIL, u=None):
        self.name = name       # const char* name
        self.type = type_      # int type
        self.u = u             # union value


# typedef struct bntvmodule {
#     const char *name;
#     const bntvmodobj_t *attrs;
#     size_t size;
#     const struct bmodule *module;
# } bntvmodule_t;
class bntvmodule:
    """Native module descriptor."""
    __slots__ = ('name', 'attrs', 'size', 'module')

    def __init__(self, name=None, attrs=None, size=0, module=None):
        self.name = name       # const char* name
        self.attrs = attrs     # bntvmodobj_t* attrs (list of bntvmodobj)
        self.size = size       # size_t size
        self.module = module   # const bmodule* module


# ============================================================================
# Native function info (from berry.h)
# ============================================================================

# typedef struct {
#     const char *name;
#     bntvfunc function;
# } bnfuncinfo;
class bnfuncinfo:
    """Native function registration entry."""
    __slots__ = ('name', 'function')

    def __init__(self, name=None, function=None):
        self.name = name       # const char* name
        self.function = function  # bntvfunc function


# ============================================================================
# Helper macros as Python functions (from be_object.h)
# ============================================================================

# #define cast(_T, _v)            ((_T)(_v))
# #define cast_int(_v)            cast(int, _v)
# #define cast_bool(_v)           cast(bbool, _v)
def cast_int(v):
    """cast(int, v)"""
    return int(v)

def cast_bool(v):
    """cast(bbool, v)"""
    return bool(v)

# #define basetype(_t)            ((_t) & 0x1F)
def basetype(t):
    """basetype(t) -> t & 0x1F"""
    return t & 0x1F

# ---------------------------------------------------------------------------
# var_type / var_basetype / var_primetype / var_isstatic / var_istype
# ---------------------------------------------------------------------------

# #define var_type(_v)            ((_v)->type)
def var_type(v):
    """var_type(v) -> v.type"""
    return v.type

# #define var_basetype(_v)        basetype((_v)->type)
def var_basetype(v):
    """var_basetype(v) -> basetype(v.type)"""
    return basetype(v.type)

# #define var_primetype(_v)       (var_type(_v) & ~BE_STATIC)
def var_primetype(v):
    """var_primetype(v) -> v.type & ~BE_STATIC"""
    return v.type & ~BE_STATIC

# #define var_isstatic(_v)        ((var_type(_v) & BE_STATIC) == BE_STATIC)
def var_isstatic(v):
    """var_isstatic(v) -> (v.type & BE_STATIC) == BE_STATIC"""
    return (v.type & BE_STATIC) == BE_STATIC

# #define var_istype(_v, _t)      (var_primetype(_v) == _t)
def var_istype(v, t):
    """var_istype(v, t) -> var_primetype(v) == t"""
    return var_primetype(v) == t

# #define var_settype(_v, _t)     ((_v)->type = _t)
def var_settype(v, t):
    """var_settype(v, t) -> v.type = t"""
    v.type = t

# #define var_markstatic(_v)      var_settype(_v, var_type(_v) | BE_STATIC)
def var_markstatic(v):
    """var_markstatic(v) -> v.type |= BE_STATIC"""
    v.type = v.type | BE_STATIC

# #define var_clearstatic(_v)     var_settype(_v, var_type(_v) & ~BE_STATIC)
def var_clearstatic(v):
    """var_clearstatic(v) -> v.type &= ~BE_STATIC"""
    v.type = v.type & ~BE_STATIC

# #define var_setobj(_v, _t, _o)  { (_v)->v.p = _o; var_settype(_v, _t); }
def var_setobj(v, t, o):
    """var_setobj(v, t, o) -> v.v = o; v.type = t"""
    v.v = o
    v.type = t


# ---------------------------------------------------------------------------
# Type check helpers (var_is*)
# ---------------------------------------------------------------------------

# #define var_isnil(_v)           var_istype(_v, BE_NIL)
def var_isnil(v):
    return var_istype(v, BE_NIL)

# #define var_isbool(_v)          var_istype(_v, BE_BOOL)
def var_isbool(v):
    return var_istype(v, BE_BOOL)

# #define var_isint(_v)           var_istype(_v, BE_INT)
def var_isint(v):
    return var_istype(v, BE_INT)

# #define var_isreal(_v)          var_istype(_v, BE_REAL)
def var_isreal(v):
    return var_istype(v, BE_REAL)

# #define var_isstr(_v)           var_istype(_v, BE_STRING)
def var_isstr(v):
    return var_istype(v, BE_STRING)

# #define var_isclosure(_v)       var_istype(_v, BE_CLOSURE)
def var_isclosure(v):
    return var_istype(v, BE_CLOSURE)

# #define var_isntvclos(_v)       var_istype(_v, BE_NTVCLOS)
def var_isntvclos(v):
    return var_istype(v, BE_NTVCLOS)

# #define var_isntvfunc(_v)       var_istype(_v, BE_NTVFUNC)
def var_isntvfunc(v):
    return var_istype(v, BE_NTVFUNC)

# #define var_isfunction(_v)      (var_basetype(_v) == BE_FUNCTION)
def var_isfunction(v):
    return var_basetype(v) == BE_FUNCTION

# #define var_isproto(_v)         var_istype(_v, BE_PROTO)
def var_isproto(v):
    return var_istype(v, BE_PROTO)

# #define var_isclass(_v)         var_istype(_v, BE_CLASS)
def var_isclass(v):
    return var_istype(v, BE_CLASS)

# #define var_isinstance(_v)      var_istype(_v, BE_INSTANCE)
def var_isinstance(v):
    return var_istype(v, BE_INSTANCE)

# #define var_islist(_v)          var_istype(_v, BE_LIST)
def var_islist(v):
    return var_istype(v, BE_LIST)

# #define var_ismap(_v)           var_istype(_v, BE_MAP)
def var_ismap(v):
    return var_istype(v, BE_MAP)

# #define var_ismodule(_v)        var_istype(_v, BE_MODULE)
def var_ismodule(v):
    return var_istype(v, BE_MODULE)

# #define var_isindex(_v)         var_istype(_v, BE_INDEX)
def var_isindex(v):
    return var_istype(v, BE_INDEX)

# #define var_iscomptr(_v)        var_istype(_v, BE_COMPTR)
def var_iscomptr(v):
    return var_istype(v, BE_COMPTR)

# #define var_isnumber(_v)        (var_isint(_v) || var_isreal(_v))
def var_isnumber(v):
    return var_isint(v) or var_isreal(v)

# ---------------------------------------------------------------------------
# Value setters (var_set*)
# ---------------------------------------------------------------------------

# #define var_setnil(_v)          var_settype(_v, BE_NIL)
def var_setnil(v):
    v.type = BE_NIL
    v.v = None

# #define var_setval(_v, _s)      (*(_v) = *(_s))
def var_setval(v, s):
    """Copy value from s to v."""
    v.type = s.type
    v.v = s.v

# #define var_setbool(_v, _b)     { var_settype(_v, BE_BOOL); (_v)->v.b = (bbool)(_b); }
def var_setbool(v, b):
    v.type = BE_BOOL
    v.v = bool(b)

# #define var_setint(_v, _i)      { var_settype(_v, BE_INT); (_v)->v.i = (_i); }
def var_setint(v, i):
    v.type = BE_INT
    v.v = i

# #define var_setreal(_v, _r)     { var_settype(_v, BE_REAL); (_v)->v.r = (_r); }
def var_setreal(v, r):
    v.type = BE_REAL
    v.v = float(r)

# #define var_setstr(_v, _s)      var_setobj(_v, BE_STRING, _s)
def var_setstr(v, s):
    var_setobj(v, BE_STRING, s)

# #define var_setinstance(_v, _o) var_setobj(_v, BE_INSTANCE, _o)
def var_setinstance(v, o):
    var_setobj(v, BE_INSTANCE, o)

# #define var_setclass(_v, _o)    var_setobj(_v, BE_CLASS, _o)
def var_setclass(v, o):
    var_setobj(v, BE_CLASS, o)

# #define var_setclosure(_v, _o)  var_setobj(_v, BE_CLOSURE, _o)
def var_setclosure(v, o):
    var_setobj(v, BE_CLOSURE, o)

# #define var_setntvclos(_v, _o)  var_setobj(_v, BE_NTVCLOS, _o)
def var_setntvclos(v, o):
    var_setobj(v, BE_NTVCLOS, o)

# #define var_setntvfunc(_v, _o)  { (_v)->v.nf = (_o); var_settype(_v, BE_NTVFUNC); }
def var_setntvfunc(v, o):
    v.v = o
    v.type = BE_NTVFUNC

# #define var_setlist(_v, _o)     var_setobj(_v, BE_LIST, _o)
def var_setlist(v, o):
    var_setobj(v, BE_LIST, o)

# #define var_setmap(_v, _o)      var_setobj(_v, BE_MAP, _o)
def var_setmap(v, o):
    var_setobj(v, BE_MAP, o)

# #define var_setmodule(_v, _o)   var_setobj(_v, BE_MODULE, _o)
def var_setmodule(v, o):
    var_setobj(v, BE_MODULE, o)

# #define var_setindex(_v, _i)    { var_settype(_v, BE_INDEX); (_v)->v.i = (_i); }
def var_setindex(v, i):
    v.type = BE_INDEX
    v.v = i

# #define var_setproto(_v, _o)    var_setobj(_v, BE_PROTO, _o)
def var_setproto(v, o):
    var_setobj(v, BE_PROTO, o)

# #define var_setcomptr(_v, _o)   var_setobj(_v, BE_COMPTR, _o)
def var_setcomptr(v, o):
    var_setobj(v, BE_COMPTR, o)


# ---------------------------------------------------------------------------
# Value getters (var_to*)
# ---------------------------------------------------------------------------

# #define var_tobool(_v)          ((_v)->v.b)
def var_tobool(v):
    return v.v

# #define var_toint(_v)           ((_v)->v.i)
def var_toint(v):
    return v.v

# #define var_toreal(_v)          ((_v)->v.r)
def var_toreal(v):
    return v.v

# #define var_tostr(_v)           ((_v)->v.s)
def var_tostr(v):
    return v.v

# #define var_togc(_v)            ((_v)->v.gc)
def var_togc(v):
    return v.v

# #define var_toobj(_v)           ((_v)->v.p)
def var_toobj(v):
    return v.v

# #define var_tontvfunc(_v)       ((_v)->v.nf)
def var_tontvfunc(v):
    return v.v

# #define var_toidx(_v)           cast_int(var_toint(_v))
def var_toidx(v):
    return int(v.v)

# ============================================================================
# GC helper macros as Python functions (from be_gc.h)
# ============================================================================

# #define gc_object(o)        cast(bgcobject*, o)
def gc_object(o):
    """Cast to bgcobject."""
    return o

# #define gc_cast(o, t, T)    ((o) && (o)->type == (t) ? (T*)(o) : NULL)
def gc_cast(o, t):
    """Return o if o is not None and o.type == t, else None."""
    return o if o is not None and o.type == t else None

# #define cast_proto(o)       gc_cast(o, BE_PROTO, bproto)
def cast_proto(o):
    return gc_cast(o, BE_PROTO)

# #define cast_closure(o)     gc_cast(o, BE_CLOSURE, bclosure)
def cast_closure(o):
    return gc_cast(o, BE_CLOSURE)

# #define cast_ntvclos(o)     gc_cast(o, BE_NTVCLOS, bntvclos)
def cast_ntvclos(o):
    return gc_cast(o, BE_NTVCLOS)

# #define cast_str(o)         gc_cast(o, BE_STRING, bstring)
def cast_str(o):
    return gc_cast(o, BE_STRING)

# #define cast_class(o)       gc_cast(o, BE_CLASS, bclass)
def cast_class(o):
    return gc_cast(o, BE_CLASS)

# #define cast_instance(o)    gc_cast(o, BE_INSTANCE, binstance)
def cast_instance(o):
    return gc_cast(o, BE_INSTANCE)

# #define cast_map(o)         gc_cast(o, BE_MAP, bmap)
def cast_map(o):
    return gc_cast(o, BE_MAP)

# #define cast_list(o)        gc_cast(o, BE_LIST, blist)
def cast_list(o):
    return gc_cast(o, BE_LIST)

# #define cast_module(o)      gc_cast(o, BE_MODULE, bmodule)
def cast_module(o):
    return gc_cast(o, BE_MODULE)

# ---------------------------------------------------------------------------
# GC mark helpers (from be_gc.h)
# ---------------------------------------------------------------------------

# #define gc_ismark(o, m)     (((o)->marked & 0x03) == m)
def gc_ismark(o, m):
    return (o.marked & 0x03) == m

# #define gc_iswhite(o)       gc_ismark((o), GC_WHITE)
def gc_iswhite(o):
    return gc_ismark(o, GC_WHITE)

# #define gc_isgray(o)        gc_ismark((o), GC_GRAY)
def gc_isgray(o):
    return gc_ismark(o, GC_GRAY)

# #define gc_isdark(o)        gc_ismark((o), GC_DARK)
def gc_isdark(o):
    return gc_ismark(o, GC_DARK)

# #define gc_setmark(o, m)
# if (!gc_isconst(o)) {
#     (o)->marked &= ~0x03;
#     (o)->marked |= (m) & 0x03;
# }
def gc_setmark(o, m):
    if not gc_isconst(o):
        o.marked = (o.marked & ~0x03) | (m & 0x03)

# #define gc_setwhite(o)      gc_setmark((o), GC_WHITE)
def gc_setwhite(o):
    gc_setmark(o, GC_WHITE)

# #define gc_setgray(o)       gc_setmark((o), GC_GRAY)
def gc_setgray(o):
    gc_setmark(o, GC_GRAY)

# #define gc_setdark(o)       gc_setmark((o), GC_DARK)
def gc_setdark(o):
    gc_setmark(o, GC_DARK)

# #define gc_isfixed(o)       (((o)->marked & GC_FIXED) != 0)
def gc_isfixed(o):
    return (o.marked & GC_FIXED) != 0

# #define gc_setfixed(o)      ((o)->marked |= GC_FIXED)
def gc_setfixed(o):
    o.marked |= GC_FIXED

# #define gc_clearfixed(o)    ((o)->marked &= ~GC_FIXED)
def gc_clearfixed(o):
    o.marked &= ~GC_FIXED

# #define gc_isconst(o)       (((o)->marked & GC_CONST) != 0)
def gc_isconst(o):
    return (o.marked & GC_CONST) != 0

# #define gc_exmark(o)        (((o)->marked >> 4) & 0x0F)
def gc_exmark(o):
    return (o.marked >> 4) & 0x0F

# #define gc_setexmark(o, k)  ((o)->marked |= (k) << 4)
def gc_setexmark(o, k):
    o.marked |= (k << 4)

# #define be_isgcobj(o)       (var_primetype(o) >= BE_GCOBJECT && var_primetype(o) < BE_GCOBJECT_MAX)
def be_isgcobj(v):
    """Check if a bvalue holds a GC-managed object."""
    pt = var_primetype(v)
    return pt >= BE_GCOBJECT and pt < BE_GCOBJECT_MAX

# ============================================================================
# Class/instance accessor macros (from be_class.h)
# ============================================================================

# #define be_class_name(cl)               ((cl)->name)
def be_class_name(cl):
    return cl.name

# #define be_class_members(cl)            ((cl)->members)
def be_class_members(cl):
    return cl.members

# #define be_class_super(cl)              ((cl)->super)
def be_class_super(cl):
    return cl.super

# #define be_class_setsuper(self, sup)    ((self)->super = (sup))
def be_class_setsuper(self_, sup):
    self_.super = sup

# #define be_class_setsub(self, sub)      ((self)->sub = (sub))
def be_class_setsub(self_, sub):
    self_.sub = sub

# #define be_instance_name(obj)           ((obj)->_class->name)
def be_instance_name(obj):
    return obj._class.name

# #define be_instance_class(obj)          ((obj)->_class)
def be_instance_class(obj):
    return obj._class

# #define be_instance_members(obj)        ((obj)->members)
def be_instance_members(obj):
    return obj.members

# #define be_instance_member_count(obj)   ((obj)->_class->nvar)
def be_instance_member_count(obj):
    return obj._class.nvar

# #define be_instance_super(obj)          ((obj)->super)
def be_instance_super(obj):
    return obj.super

# #define be_instance_sub(obj)            ((obj)->sub)
def be_instance_sub(obj):
    return obj.sub

# ============================================================================
# List accessor macros (from be_list.h)
# ============================================================================

# #define be_list_data(list)          ((list)->data)
def be_list_data(lst):
    return lst.data

# #define be_list_count(list)         ((list)->count)
def be_list_count(lst):
    return lst.count

# #define be_list_at(list, index)     ((list)->data + index)
def be_list_at(lst, index):
    return lst.data[index]

# #define be_list_end(list)           ((list)->data + (list)->count)
# Returns the index one past the last element.
def be_list_end(lst):
    return lst.count

# ============================================================================
# Map accessor macros (from be_map.h)
# ============================================================================

# #define be_map_iter()       NULL
def be_map_iter():
    return None

# #define be_map_count(map)   ((map)->count)
def be_map_count(m):
    return m.count

# #define be_map_size(map)    (map->size)
def be_map_size(m):
    return m.size

# #define be_map_key2value(dst, node) do {
#     (dst)->type = (node)->key.type;
#     (dst)->v = (node)->key.v;
# } while (0);
def be_map_key2value(dst, node):
    """Copy map key into a bvalue."""
    dst.type = node.key.type
    dst.v = node.key.v

# ============================================================================
# String accessor macros (from be_string.h)
# ============================================================================

# #define str_len(_s)
#     ((_s)->slen == 255 ? cast(blstring*, _s)->llen : (_s)->slen)
def str_len(s):
    """Get the length of a bstring."""
    return s.llen if s.slen == 255 else s.slen

# #define str_extra(_s)           ((_s)->extra)
def str_extra(s):
    return s.extra
