"""
Berry recursive descent parser.
Mirrors: src/be_parser.c / src/be_parser.h

One-pass recursive descent parser that consumes tokens from the lexer and
drives the code generator to emit bytecode. Produces a bclosure wrapping
a bproto. Implements all expression and statement parsers, variable
resolution (local, global, upvalue, named global), and block management.

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
    bvalue, bproto, bclosure, bclass, binstance, blist, bmap,
    bupvaldesc, blineinfo, bvarinfo,
    BE_NIL, BE_INT, BE_REAL, BE_BOOL, BE_STRING, BE_FUNCTION,
    BE_CLASS, BE_NONE,
    var_setnil, var_setval, var_setint, var_setstr, var_setlist, var_setmap,
    var_setclosure, var_setproto,
    var_type, var_basetype, var_primetype,
    var_istype, var_isproto, var_isclosure,
    var_toint, var_tostr, var_toobj,
    be_list_data, be_list_count, be_list_at,
    be_map_iter, be_map_count,
    cast_int,
)
from berry_port.be_lexer import (
    blexer,
    be_lexer_init, be_lexer_deinit, be_lexer_scan_next,
    be_lexer_newstr, be_token2str, be_tokentype2str,
    be_lexerror,
    TokenNone, TokenEOS, TokenId, TokenInteger, TokenReal, TokenString,
    OptAssign, OptAddAssign, OptSubAssign, OptMulAssign, OptDivAssign,
    OptModAssign, OptAndAssign, OptOrAssign, OptXorAssign,
    OptLsfAssign, OptRsfAssign,
    OptAdd, OptSub, OptMul, OptDiv, OptMod,
    OptLT, OptLE, OptEQ, OptNE, OptGT, OptGE,
    OptBitAnd, OptBitOr, OptBitXor, OptShiftL, OptShiftR,
    OptConnect, OptAnd, OptOr,
    OptNot, OptFlip,
    OptSpaceLBK, OptCallLBK, OptRBK, OptLSB, OptRSB, OptLBR, OptRBR,
    OptDot, OptComma, OptSemic, OptColon, OptQuestion, OptArrow,
    OptWalrus,
    KeyIf, KeyElif, KeyElse, KeyWhile, KeyFor, KeyDef, KeyEnd,
    KeyClass, KeyBreak, KeyContinue, KeyReturn, KeyTrue, KeyFalse,
    KeyNil, KeyVar, KeyDo, KeyImport, KeyAs, KeyTry, KeyExcept,
    KeyRaise, KeyStatic,
)
from berry_port.be_vector import (
    be_vector_init, be_vector_push_c, be_vector_release,
    be_vector_data, be_vector_end, be_vector_count, be_vector_capacity,
)
from berry_port.be_mem import be_malloc, be_free
from berry_port.be_string import be_newstr, be_newstrn, be_str2cstr, be_eqstr
from berry_port.be_list import (
    be_list_new, be_list_push, be_list_resize,
)
from berry_port.be_map import (
    be_map_new, be_map_findstr, be_map_insertstr, be_map_next,
)
from berry_port.be_object import be_map_count as _be_map_count
from berry_port.be_func import be_newproto, be_newclosure
from berry_port.be_class import (
    be_newclass, be_class_compress, be_class_attribute,
    be_class_member_bind, be_class_method_bind,
)
from berry_port.be_decoder import NO_JUMP, IBx_MASK
from berry_port.berry_conf import (
    BE_USE_SCRIPT_COMPILER,
    BE_DEBUG_RUNTIME_INFO, BE_DEBUG_VAR_INFO,
    BE_INTGER_TYPE,
)


# ============================================================================
# Integer limits (mirrors C limits.h)
# ============================================================================

# #if BE_INTGER_TYPE == 0
#   #define M_IMAX    INT_MAX
#   #define M_IMIN    INT_MIN
# #elif BE_INTGER_TYPE == 1
#   #define M_IMAX    LONG_MAX
#   #define M_IMIN    LONG_MIN
# #else
#   #define M_IMAX    LLONG_MAX
#   #define M_IMIN    LLONG_MIN
# #endif
if BE_INTGER_TYPE == 0:
    M_IMAX = 2**31 - 1
    M_IMIN = -(2**31)
elif BE_INTGER_TYPE == 1:
    M_IMAX = 2**63 - 1   # long on 64-bit
    M_IMIN = -(2**63)
else:  # int64_t (long long)
    M_IMAX = 2**63 - 1
    M_IMIN = -(2**63)


# ============================================================================
# Expression type enum (from be_parser.h exptype_t)
# ============================================================================

# typedef enum {
#     ETVOID,     /* unknown (new variable or error) */
#     ETNIL,
#     ETBOOL,
#     ETREAL,
#     ETINT,
#     ETSTRING,
#     ETPROTO,
#     ETCONST,
#     ETLOCAL,    /* local variable */
#     ETGLOBAL,   /* global by index number */
#     ETUPVAL,
#     ETMEMBER,   /* member accessor (by name) */
#     ETINDEX,    /* index accessor */
#     ETREG,      /* temporary register */
#     ETNGLOBAL   /* named global */
# } exptype_t;
ETVOID    = 0
ETNIL     = 1
ETBOOL    = 2
ETREAL    = 3
ETINT     = 4
ETSTRING  = 5
ETPROTO   = 6
ETCONST   = 7
ETLOCAL   = 8
ETGLOBAL  = 9
ETUPVAL   = 10
ETMEMBER  = 11
ETINDEX   = 12
ETREG     = 13
ETNGLOBAL = 14


# ============================================================================
# Block type definitions (from be_parser.h)
# ============================================================================

# #define BLOCK_LOOP      1
# #define BLOCK_EXCEPT    2
BLOCK_LOOP   = 1
BLOCK_EXCEPT = 2


# ============================================================================
# Function type flags
# ============================================================================

# #define FUNC_METHOD     1
# #define FUNC_ANONYMOUS  2
# #define FUNC_STATIC     4
FUNC_METHOD    = 1
FUNC_ANONYMOUS = 2
FUNC_STATIC    = 4


# ============================================================================
# Parser constants
# ============================================================================

# #define OP_NOT_BINARY   TokenNone
# #define OP_NOT_UNARY    TokenNone
# #define OP_NOT_ASSIGN   TokenNone
# #define UNARY_OP_PRIO   3
# #define ASSIGN_OP_PRIO  16
OP_NOT_BINARY  = TokenNone
OP_NOT_UNARY   = TokenNone
OP_NOT_ASSIGN  = TokenNone
UNARY_OP_PRIO  = 3
ASSIGN_OP_PRIO = 16


# ============================================================================
# Compiler option helpers (from be_vm.h)
# ============================================================================

# typedef enum {
#     COMP_NAMED_GBL = 0x00,
#     COMP_STRICT = 0x01,
#     COMP_GC_DEBUG = 0x02,
# } compoptmask;
COMP_NAMED_GBL = 0x00
COMP_STRICT    = 0x01

# #define comp_is_named_gbl(vm)   ((vm)->compopt & (1<<COMP_NAMED_GBL))
# #define comp_is_strict(vm)      ((vm)->compopt & (1<<COMP_STRICT))
def comp_is_named_gbl(vm):
    return (vm.compopt & (1 << COMP_NAMED_GBL)) != 0

def comp_is_strict(vm):
    return (vm.compopt & (1 << COMP_STRICT)) != 0


# ============================================================================
# Upvalue descriptor helpers (from be_parser.c)
# ============================================================================

# #define upval_index(v)      ((v) & 0xFF)
# #define upval_target(v)     ((bbyte)(((v) >> 8) & 0xFF))
# #define upval_instack(v)    ((bbyte)(((v) >> 16) != 0))
# #define upval_desc(i, t, s) (((i) & 0xFF) | (((t) & 0xFF) << 8) | (((s) != 0) << 16))
def upval_index(v):
    return v & 0xFF

def upval_target(v):
    return (v >> 8) & 0xFF

def upval_instack(v):
    return ((v >> 16) != 0)

def upval_desc(i, t, s):
    return (i & 0xFF) | ((t & 0xFF) << 8) | ((1 if s else 0) << 16)


# ============================================================================
# Binary operator priority table (from be_parser.c)
# ============================================================================

# static const bbyte binary_op_prio_tab[] = {
#     5, 5, 4, 4, 4,             /* + - * / % */
#     11, 11, 12, 12, 11, 11,    /* < <= == != > >= */
#     7, 9, 8, 6, 6, 10, 13, 14  /* & | ^ << >> .. && || */
# };
binary_op_prio_tab = [
    5, 5, 4, 4, 4,             # + - * / %
    11, 11, 12, 12, 11, 11,    # < <= == != > >=
    7, 9, 8, 6, 6, 10, 13, 14  # & | ^ << >> .. && ||
]

# #define binary_op_prio(op)  (binary_op_prio_tab[cast_int(op) - OptAdd])
def binary_op_prio(op):
    return binary_op_prio_tab[cast_int(op) - OptAdd]


# ============================================================================
# bexpdesc — expression descriptor (from be_parser.h)
# ============================================================================

# typedef struct {
#     union {
#         struct { unsigned int idx:9; unsigned int obj:9; unsigned int tt:5; } ss;
#         breal r;
#         bint i;
#         bstring *s;
#         bproto *p;
#         int idx;
#     } v;
#     int t;
#     int f;
#     bbyte not;
#     exptype_t type;
# } bexpdesc;
class bexpdesc:
    """Expression descriptor — mirrors C bexpdesc struct.

    The `v` field is a Python object whose interpretation depends on `type`:
      - ETINT: v.i (int)
      - ETREAL: v.r (float)
      - ETSTRING: v.s (bstring)
      - ETPROTO: v.p (bproto)
      - ETLOCAL/ETGLOBAL/ETUPVAL/ETREG/ETNGLOBAL: v.idx (int)
      - ETMEMBER/ETINDEX: v.ss (suffix struct with idx, obj, tt)

    For simplicity, we store all union variants as direct attributes.
    """
    __slots__ = ('type', 't', 'f', 'not_', 'v')

    def __init__(self):
        self.type = ETVOID
        self.t = NO_JUMP
        self.f = NO_JUMP
        self.not_ = 0
        self.v = _expdesc_v()


class _expdesc_v:
    """Union-like value holder for bexpdesc.v"""
    __slots__ = ('i', 'r', 's', 'p', 'idx', 'ss')

    def __init__(self):
        self.i = 0
        self.r = 0.0
        self.s = None
        self.p = None
        self.idx = 0
        self.ss = _suffix_desc()


class _suffix_desc:
    """Suffix descriptor for ETMEMBER/ETINDEX — mirrors C ss struct."""
    __slots__ = ('idx', 'obj', 'tt')

    def __init__(self):
        self.idx = 0
        self.obj = 0
        self.tt = 0


def _copy_expdesc(dst, src):
    """Deep-copy an expdesc (used for `bexpdesc e = *l` in C)."""
    dst.type = src.type
    dst.t = src.t
    dst.f = src.f
    dst.not_ = src.not_
    dst.v.i = src.v.i
    dst.v.r = src.v.r
    dst.v.s = src.v.s
    dst.v.p = src.v.p
    dst.v.idx = src.v.idx
    dst.v.ss.idx = src.v.ss.idx
    dst.v.ss.obj = src.v.ss.obj
    dst.v.ss.tt = src.v.ss.tt


def _clone_expdesc(src):
    """Return a new expdesc that is a copy of src."""
    e = bexpdesc()
    _copy_expdesc(e, src)
    return e


# ============================================================================
# bblockinfo — block information (from be_parser.h)
# ============================================================================

# typedef struct bblockinfo {
#     struct bblockinfo *prev;
#     bbyte nactlocals;
#     bbyte type;
#     bbyte hasupval;
#     bbyte sideeffect;
#     int lastjmp;
#     int breaklist;
#     int beginpc;
#     int continuelist;
# } bblockinfo;
class bblockinfo:
    """Block information — mirrors C bblockinfo struct."""
    __slots__ = ('prev', 'nactlocals', 'type', 'hasupval', 'sideeffect',
                 'lastjmp', 'breaklist', 'beginpc', 'continuelist')

    def __init__(self):
        self.prev = None
        self.nactlocals = 0
        self.type = 0
        self.hasupval = 0
        self.sideeffect = 0
        self.lastjmp = 0
        self.breaklist = NO_JUMP
        self.beginpc = 0
        self.continuelist = NO_JUMP


# ============================================================================
# bfuncinfo — function compilation info (from be_parser.h)
# ============================================================================

# typedef struct bfuncinfo {
#     struct bfuncinfo *prev;
#     bproto *proto;
#     bblockinfo *binfo;
#     struct blexer *lexer;
#     blist *local;
#     bmap *upval;
#     bvector code;
#     bvector kvec;
#     bvector pvec;
#     bvector linevec;   /* if BE_DEBUG_RUNTIME_INFO */
#     bvector varvec;    /* if BE_DEBUG_VAR_INFO */
#     int pc;
#     bbyte freereg;
#     bbyte flags;
# } bfuncinfo;
class bfuncinfo:
    """Function compilation info — mirrors C bfuncinfo struct."""
    __slots__ = ('prev', 'proto', 'binfo', 'lexer', 'local', 'upval',
                 'code', 'kvec', 'pvec', 'linevec', 'varvec',
                 'pc', 'freereg', 'flags')

    def __init__(self):
        self.prev = None
        self.proto = None
        self.binfo = None
        self.lexer = None
        self.local = None
        self.upval = None
        self.code = None   # bvector
        self.kvec = None   # bvector
        self.pvec = None   # bvector
        self.linevec = None  # bvector (debug)
        self.varvec = None   # bvector (debug)
        self.pc = 0
        self.freereg = 0
        self.flags = 0


# ============================================================================
# bparser — parser state (from be_parser.c)
# ============================================================================

# typedef struct {
#     blexer lexer;
#     bvm *vm;
#     bfuncinfo *finfo;
#     bclosure *cl;
#     bbyte islocal;
# } bparser;
class bparser:
    """Parser state — mirrors C bparser struct."""
    __slots__ = ('lexer', 'vm', 'finfo', 'cl', 'islocal')

    def __init__(self):
        self.lexer = blexer()
        self.vm = None
        self.finfo = None
        self.cl = None
        self.islocal = 0


# ============================================================================
# Convenience macros (from be_parser.c)
# ============================================================================

# #define scan_next_token(parser) (be_lexer_scan_next(&(parser)->lexer))
def scan_next_token(parser):
    return be_lexer_scan_next(parser.lexer)

# #define next_token(parser)      ((parser)->lexer.token)
def next_token(parser):
    return parser.lexer.token

# #define next_type(parser)       (next_token(parser).type)
def next_type(parser):
    return parser.lexer.token.type

# #define token2str(parser)       be_token2str((parser)->vm, &next_token(parser))
def token2str(parser):
    return be_token2str(parser.vm, parser.lexer.token)

# #define funcname(parser)        ((parser)->islocal ? "loader" : "main")
def funcname(parser):
    return "loader" if parser.islocal else "main"

# #define parser_newstr(p, str)   be_lexer_newstr(&(p)->lexer, (str))
def parser_newstr(parser, s):
    return be_lexer_newstr(parser.lexer, s)

# #define parser_error(p, msg)    be_lexerror(&(p)->lexer, msg)
def parser_error(parser, msg):
    be_lexerror(parser.lexer, msg)

# #define push_error(parser, ...)
#     parser_error(parser, be_pushfstring(parser->vm, __VA_ARGS__))
def push_error(parser, fmt, *args):
    if args:
        msg = fmt % args
    else:
        msg = fmt
    parser_error(parser, msg)


# ============================================================================
# Stub imports for modules not yet ported (be_code, be_var, be_exec)
#
# These are forward-declared here and will be resolved at call time via
# lazy imports. This avoids circular import issues and allows the parser
# to be loaded before be_code.py and be_var.py are implemented.
# ============================================================================

def _lazy_be_code():
    """Lazy import for be_code module (task 5.4)."""
    import berry_port.be_code as mod
    return mod

def _lazy_be_var():
    """Lazy import for be_var module (task 5.5)."""
    import berry_port.be_var as mod
    return mod

def _lazy_be_exec():
    """Lazy import for be_exec module (task 7.2)."""
    import berry_port.be_exec as mod
    return mod


# ============================================================================
# Forward-declared code generator wrappers
# These delegate to be_code.py functions via lazy import.
# ============================================================================

def be_code_allocregs(finfo, count):
    return _lazy_be_code().be_code_allocregs(finfo, count)

def be_code_freeregs(finfo, n):
    # #define be_code_freeregs(f, n)  ((f)->freereg -= (bbyte)(n))
    finfo.freereg -= n

def be_code_prebinop(finfo, op, e):
    return _lazy_be_code().be_code_prebinop(finfo, op, e)

def be_code_binop(finfo, op, e1, e2, dst):
    return _lazy_be_code().be_code_binop(finfo, op, e1, e2, dst)

def be_code_unop(finfo, op, e):
    return _lazy_be_code().be_code_unop(finfo, op, e)

def be_code_setvar(finfo, e1, e2, keep_reg):
    return _lazy_be_code().be_code_setvar(finfo, e1, e2, keep_reg)

def be_code_nextreg(finfo, e):
    return _lazy_be_code().be_code_nextreg(finfo, e)

def be_code_jump(finfo):
    return _lazy_be_code().be_code_jump(finfo)

def be_code_jumpto(finfo, dst):
    return _lazy_be_code().be_code_jumpto(finfo, dst)

def be_code_jumpbool(finfo, e, jumptrue):
    return _lazy_be_code().be_code_jumpbool(finfo, e, jumptrue)

def be_code_conjump(finfo, list_ref, jmp):
    # Handle both int and list-ref (mutable int pointer) callers
    if isinstance(list_ref, list):
        list_ref[0] = _lazy_be_code().be_code_conjump(finfo, list_ref[0], jmp)
        return list_ref[0]
    return _lazy_be_code().be_code_conjump(finfo, list_ref, jmp)

def be_code_patchlist(finfo, list_, dst):
    return _lazy_be_code().be_code_patchlist(finfo, list_, dst)

def be_code_patchjump(finfo, jmp):
    return _lazy_be_code().be_code_patchjump(finfo, jmp)

def be_code_getmethod(finfo, e):
    return _lazy_be_code().be_code_getmethod(finfo, e)

def be_code_call(finfo, base, argc):
    return _lazy_be_code().be_code_call(finfo, base, argc)

def be_code_proto(finfo, proto):
    return _lazy_be_code().be_code_proto(finfo, proto)

def be_code_closure(finfo, e, idx):
    return _lazy_be_code().be_code_closure(finfo, e, idx)

def be_code_close(finfo, isret):
    return _lazy_be_code().be_code_close(finfo, isret)

def be_code_class(finfo, dst, c):
    return _lazy_be_code().be_code_class(finfo, dst, c)

def be_code_ret(finfo, e):
    return _lazy_be_code().be_code_ret(finfo, e)

def be_code_resolve(finfo, k):
    return _lazy_be_code().be_code_resolve(finfo, k)

def be_code_member(finfo, e1, e2):
    return _lazy_be_code().be_code_member(finfo, e1, e2)

def be_code_index(finfo, c, k):
    return _lazy_be_code().be_code_index(finfo, c, k)

def be_code_setsuper(finfo, c, s):
    return _lazy_be_code().be_code_setsuper(finfo, c, s)

def be_code_import(finfo, m, v):
    return _lazy_be_code().be_code_import(finfo, m, v)

def be_code_exblk(finfo, depth):
    return _lazy_be_code().be_code_exblk(finfo, depth)

def be_code_catch(finfo, base, ecnt, vcnt, jmp):
    return _lazy_be_code().be_code_catch(finfo, base, ecnt, vcnt, jmp)

def be_code_raise(finfo, e1, e2):
    return _lazy_be_code().be_code_raise(finfo, e1, e2)

def be_code_implicit_class(finfo, e, c):
    return _lazy_be_code().be_code_implicit_class(finfo, e, c)


# Forward-declared variable resolution wrappers
def be_global_find(vm, name):
    return _lazy_be_var().be_global_find(vm, name)

def be_global_new(vm, name):
    return _lazy_be_var().be_global_new(vm, name)

def be_builtin_find(vm, name):
    return _lazy_be_var().be_builtin_find(vm, name)

def be_builtin_count(vm):
    return _lazy_be_var().be_builtin_count(vm)

def be_builtin_name(vm, index):
    return _lazy_be_var().be_builtin_name(vm, index)

def be_global_release_space(vm):
    return _lazy_be_var().be_global_release_space(vm)


# Forward-declared exec wrappers
def be_stackpush(vm):
    """Push one slot onto the VM stack."""
    # Inline implementation matching be_exec.py / be_lexer.py stub
    if vm.top_idx >= len(vm.stack):
        vm.stack.append(bvalue())
    vm.top_idx += 1

def be_stackpop(vm, n):
    """Pop n slots from the VM stack."""
    vm.top_idx -= n


# ============================================================================
# init_exp — Initialize bexpdesc (from be_parser.c)
# ============================================================================

# static void init_exp(bexpdesc *e, exptype_t type, bint i)
# {
#     e->type = (bbyte)type;
#     e->t = NO_JUMP;
#     e->f = NO_JUMP;
#     e->not = 0;
#     e->v.s = NULL;
#     e->v.i = i;
# }
def init_exp(e, type_, i):
    """Initialize an expression descriptor with type and integer value."""
    e.type = type_
    e.t = NO_JUMP
    e.f = NO_JUMP
    e.not_ = 0
    e.v.s = None
    e.v.i = i
    e.v.idx = i  # idx shares storage with i in the C union


# ============================================================================
# Token matching helpers (from be_parser.c)
# ============================================================================

# static void match_token(bparser *parser, btokentype type)
# {
#     if (next_type(parser) != type) {
#         btoken *token = &next_token(parser);
#         const char *s1 = be_tokentype2str(type);
#         const char *s2 = be_token2str(parser->vm, token);
#         push_error(parser, "expected '%s' before '%s'", s1, s2);
#     }
#     scan_next_token(parser);
# }
def match_token(parser, type_):
    """Match and skip the expected token type, or raise an error."""
    if next_type(parser) != type_:
        s1 = be_tokentype2str(type_)
        s2 = token2str(parser)
        push_error(parser, "expected '%s' before '%s'", s1, s2)
    scan_next_token(parser)


# static void match_notoken(bparser *parser, btokentype type)
# {
#     if (next_type(parser) == type) {
#         push_error(parser,
#             "expected statement before '%s'", token2str(parser));
#     }
# }
def match_notoken(parser, type_):
    """Check that the next token is NOT of the given type, or raise an error."""
    if next_type(parser) == type_:
        push_error(parser, "expected statement before '%s'", token2str(parser))


# static void check_symbol(bparser *parser, bexpdesc *e)
# {
#     if (e->type == ETVOID && e->v.s == NULL) {
#         push_error(parser,
#             "unexpected symbol near '%s'", token2str(parser));
#     }
# }
def check_symbol(parser, e):
    """Check that the expdesc is a valid symbol, or raise an error."""
    if e.type == ETVOID and e.v.s is None:
        push_error(parser, "unexpected symbol near '%s'", token2str(parser))


# static void check_var(bparser *parser, bexpdesc *e)
# {
#     check_symbol(parser, e);
#     if (e->type == ETVOID) {
#         int line = parser->lexer.linenumber;
#         parser->lexer.linenumber = parser->lexer.lastline;
#         push_error(parser,
#             "'%s' undeclared (first use in this function)", str(e->v.s));
#         parser->lexer.linenumber = line;
#     }
# }
def check_var(parser, e):
    """Check that the value in e is valid for a variable."""
    check_symbol(parser, e)
    if e.type == ETVOID:
        line = parser.lexer.linenumber
        parser.lexer.linenumber = parser.lexer.lastline
        name_s = be_str2cstr(e.v.s) if e.v.s is not None else "?"
        push_error(parser,
            "'%s' undeclared (first use in this function)", name_s)
        parser.lexer.linenumber = line


# static int match_skip(bparser *parser, btokentype type)
# {
#     if (next_type(parser) == type) {
#         scan_next_token(parser);
#         return btrue;
#     }
#     return bfalse;
# }
def match_skip(parser, type_):
    """If the next token matches, skip it and return True."""
    if next_type(parser) == type_:
        scan_next_token(parser)
        return True
    return False


# static bstring* _match_id(bparser *parser)
# {
#     if (next_type(parser) == TokenId) {
#         bstring *str = next_token(parser).u.s;
#         scan_next_token(parser);
#         return str;
#     }
#     return NULL;
# }
def _match_id(parser):
    """If the next token is an identifier, consume it and return the string."""
    if next_type(parser) == TokenId:
        s = next_token(parser).u.s
        scan_next_token(parser)
        return s
    return None


# ============================================================================
# Debug variable info (from be_parser.c)
# ============================================================================

if BE_DEBUG_VAR_INFO:
    # void begin_varinfo(bparser *parser, bstring *name)
    # {
    #     bvarinfo *var;
    #     bfuncinfo *finfo = parser->finfo;
    #     be_vector_push_c(parser->vm, &finfo->varvec, NULL);
    #     var = be_vector_end(&finfo->varvec);
    #     var->name = name;
    #     var->beginpc = finfo->pc;
    #     var->endpc = 0;
    #     finfo->proto->varinfo = be_vector_data(&finfo->varvec);
    #     finfo->proto->nvarinfo = be_vector_capacity(&finfo->varvec);
    # }
    def begin_varinfo(parser, name):
        finfo = parser.finfo
        be_vector_push_c(parser.vm, finfo.varvec, None)
        var = be_vector_end(finfo.varvec)
        if var is not None:
            var.name = name
            var.beginpc = finfo.pc
            var.endpc = 0
        else:
            vi = bvarinfo()
            vi.name = name
            vi.beginpc = finfo.pc
            vi.endpc = 0
            finfo.varvec.data[-1] = vi
        finfo.proto.varinfo = be_vector_data(finfo.varvec)
        finfo.proto.nvarinfo = be_vector_capacity(finfo.varvec)

    # void end_varinfo(bparser *parser, int beginpc)
    def end_varinfo(parser, beginpc):
        finfo = parser.finfo
        binfo = finfo.binfo
        if beginpc == -1:
            beginpc = binfo.beginpc
        data = be_vector_data(finfo.varvec)
        if data is None:
            return
        for it in data:
            if it is None:
                continue
            if it.beginpc < beginpc:
                continue
            if not it.endpc:
                it.endpc = finfo.pc
else:
    def begin_varinfo(parser, name):
        pass

    def end_varinfo(parser, beginpc):
        pass


# ============================================================================
# Block management (from be_parser.c)
# ============================================================================

# static void begin_block(bfuncinfo *finfo, bblockinfo *binfo, int type)
# {
#     binfo->prev = finfo->binfo;
#     finfo->binfo = binfo;
#     binfo->type = (bbyte)type;
#     binfo->hasupval = 0;
#     binfo->sideeffect = 0;
#     binfo->lastjmp = 0;
#     binfo->beginpc = finfo->pc;
#     binfo->nactlocals = (bbyte)be_list_count(finfo->local);
#     if (type & BLOCK_LOOP) {
#         binfo->breaklist = NO_JUMP;
#         binfo->continuelist = NO_JUMP;
#     }
# }
def begin_block(finfo, binfo, type_):
    """Initialize a block info structure and push it onto the block stack."""
    binfo.prev = finfo.binfo
    finfo.binfo = binfo
    binfo.type = type_
    binfo.hasupval = 0
    binfo.sideeffect = 0
    binfo.lastjmp = 0
    binfo.beginpc = finfo.pc
    binfo.nactlocals = be_list_count(finfo.local)
    if type_ & BLOCK_LOOP:
        binfo.breaklist = NO_JUMP
        binfo.continuelist = NO_JUMP


# static void end_block_ex(bparser *parser, int beginpc)
# {
#     bfuncinfo *finfo = parser->finfo;
#     bblockinfo *binfo = finfo->binfo;
#     be_code_close(finfo, 0);
#     if (binfo->type & BLOCK_LOOP) {
#         be_code_jumpto(finfo, binfo->beginpc);
#         be_code_patchjump(finfo, binfo->breaklist);
#         be_code_patchlist(finfo, binfo->continuelist, binfo->beginpc);
#     }
#     end_varinfo(parser, beginpc);
#     be_list_resize(parser->vm, finfo->local, binfo->nactlocals);
#     finfo->freereg = binfo->nactlocals;
#     finfo->binfo = binfo->prev;
# }
def end_block_ex(parser, beginpc):
    """End a block, closing upvalues and patching jumps."""
    finfo = parser.finfo
    binfo = finfo.binfo
    be_code_close(finfo, 0)
    if binfo.type & BLOCK_LOOP:
        be_code_jumpto(finfo, binfo.beginpc)
        be_code_patchjump(finfo, binfo.breaklist)
        be_code_patchlist(finfo, binfo.continuelist, binfo.beginpc)
    end_varinfo(parser, beginpc)
    be_list_resize(parser.vm, finfo.local, binfo.nactlocals)
    finfo.freereg = binfo.nactlocals
    finfo.binfo = binfo.prev


# static void end_block(bparser *parser)
# {
#     end_block_ex(parser, -1);
# }
def end_block(parser):
    """End a block using default beginpc."""
    end_block_ex(parser, -1)


# ============================================================================
# Function management (from be_parser.c)
# ============================================================================

# static bstring* parser_source(bparser *parser)
# {
#     if (parser->finfo) {
#         return parser->finfo->proto->source;
#     }
#     return be_newstr(parser->vm, parser->lexer.fname);
# }
def parser_source(parser):
    """Return the source name for this parser."""
    if parser.finfo:
        return parser.finfo.proto.source
    return be_newstr(parser.vm, parser.lexer.fname)


# static void begin_func(bparser *parser, bfuncinfo *finfo, bblockinfo *binfo)
def begin_func(parser, finfo, binfo):
    """Initialize a function block and create a new bproto."""
    vm = parser.vm
    proto = be_newproto(vm)
    # var_setproto(vm->top, proto); be_stackpush(vm);
    if vm.top_idx >= len(vm.stack):
        vm.stack.append(bvalue())
    var_setproto(vm.stack[vm.top_idx], proto)
    be_stackpush(vm)

    from berry_port.be_vector import bvector as _bvector_cls
    # Initialize code vector
    finfo.code = _bvector_cls()
    be_vector_init(vm, finfo.code, 4)  # sizeof(binstruction) = 4
    proto.code = be_vector_data(finfo.code)
    proto.codesize = be_vector_capacity(finfo.code)

    # Initialize constants vector
    finfo.kvec = _bvector_cls()
    be_vector_init(vm, finfo.kvec, 1)  # sizeof(bvalue) — element size placeholder
    proto.ktab = be_vector_data(finfo.kvec)
    proto.nconst = be_vector_capacity(finfo.kvec)

    # Initialize sub-proto vector
    finfo.pvec = _bvector_cls()
    be_vector_init(vm, finfo.pvec, 1)  # sizeof(bproto*)
    proto.ptab = be_vector_data(finfo.pvec)
    proto.nproto = be_vector_capacity(finfo.pvec)

    # Source file tracking
    if BE_DEBUG_RUNTIME_INFO:
        proto.source = parser_source(parser)

    # Local variable list
    finfo.local = be_list_new(vm)
    if vm.top_idx >= len(vm.stack):
        vm.stack.append(bvalue())
    var_setlist(vm.stack[vm.top_idx], finfo.local)
    be_stackpush(vm)

    # Upvalue map
    finfo.upval = be_map_new(vm)
    if vm.top_idx >= len(vm.stack):
        vm.stack.append(bvalue())
    var_setmap(vm.stack[vm.top_idx], finfo.upval)
    be_stackpush(vm)

    # Link to parser
    finfo.prev = parser.finfo
    finfo.lexer = parser.lexer
    finfo.proto = proto
    finfo.freereg = 0
    finfo.binfo = None
    finfo.pc = 0
    finfo.flags = 0
    parser.finfo = finfo

    # Debug info vectors
    if BE_DEBUG_RUNTIME_INFO:
        finfo.linevec = _bvector_cls()
        be_vector_init(vm, finfo.linevec, 1)
        proto.lineinfo = be_vector_data(finfo.linevec)
        proto.nlineinfo = be_vector_capacity(finfo.linevec)

    if BE_DEBUG_VAR_INFO:
        finfo.varvec = _bvector_cls()
        be_vector_init(vm, finfo.varvec, 1)
        proto.varinfo = be_vector_data(finfo.varvec)
        proto.nvarinfo = be_vector_capacity(finfo.varvec)

    begin_block(finfo, binfo, 0)


# static void setupvals(bfuncinfo *finfo)
# {
#     bproto *proto = finfo->proto;
#     int nupvals = be_map_count(finfo->upval);
#     if (nupvals) {
#         bmapnode *node;
#         bmap *map = finfo->upval;
#         bmapiter iter = be_map_iter();
#         bupvaldesc *upvals = be_malloc(...);
#         while ((node = be_map_next(map, &iter)) != NULL) {
#             uint32_t v = (uint32_t)var_toint(&node->value);
#             bupvaldesc *uv = upvals + upval_index(v);
#             uv->idx = upval_target(v);
#             uv->instack = upval_instack(v);
#         }
#         proto->upvals = upvals;
#         proto->nupvals = (bbyte)nupvals;
#     }
# }
def setupvals(finfo):
    """Compute the upvalue descriptor array from the upval map."""
    proto = finfo.proto
    nupvals = _be_map_count(finfo.upval)
    if nupvals:
        upvals = [bupvaldesc() for _ in range(nupvals)]
        map_ = finfo.upval
        iter_ref = [be_map_iter()]
        while True:
            node, iter_ref = be_map_next(map_, iter_ref)
            if node is None:
                break
            v = var_toint(node.value) & 0xFFFFFFFF
            uv = upvals[upval_index(v)]
            uv.idx = upval_target(v)
            uv.instack = 1 if upval_instack(v) else 0
            if BE_DEBUG_VAR_INFO:
                uv.name = var_tostr(node.key) if hasattr(node, 'key') and hasattr(node.key, 'v') else None
        proto.upvals = upvals
        proto.nupvals = nupvals


# static void end_func(bparser *parser)
def end_func(parser):
    """Finalize the current function: append return, close block, compact vectors."""
    vm = parser.vm
    finfo = parser.finfo
    proto = finfo.proto

    be_code_ret(finfo, None)  # append a return to last code
    end_block(parser)         # close block
    setupvals(finfo)          # close upvals

    proto.code = be_vector_release(vm, finfo.code)
    proto.codesize = finfo.pc
    proto.ktab = be_vector_release(vm, finfo.kvec)
    proto.nconst = be_vector_count(finfo.kvec)
    proto.ptab = be_vector_release(vm, finfo.pvec)
    proto.nproto = be_vector_count(finfo.pvec)

    if BE_DEBUG_RUNTIME_INFO:
        proto.lineinfo = be_vector_release(vm, finfo.linevec)
        proto.nlineinfo = be_vector_count(finfo.linevec)

    if BE_DEBUG_VAR_INFO:
        proto.varinfo = be_vector_release(vm, finfo.varvec)
        proto.nvarinfo = be_vector_count(finfo.varvec)

    parser.finfo = parser.finfo.prev  # restore previous finfo
    be_stackpop(vm, 2)  # pop upval and local


# ============================================================================
# Operator detection helpers (from be_parser.c)
# ============================================================================

# static btokentype get_binop(bparser *parser)
# {
#     btokentype op = next_type(parser);
#     if (op >= OptAdd && op <= OptOr) { return op; }
#     return OP_NOT_BINARY;
# }
def get_binop(parser):
    """Return the binary operator token type, or OP_NOT_BINARY."""
    op = next_type(parser)
    if OptAdd <= op <= OptOr:
        return op
    return OP_NOT_BINARY


# static btokentype get_unary_op(bparser *parser)
# {
#     btokentype op = next_type(parser);
#     if (op == OptSub || op == OptNot || op == OptFlip) { return op; }
#     return OP_NOT_UNARY;
# }
def get_unary_op(parser):
    """Return the unary operator token type, or OP_NOT_UNARY."""
    op = next_type(parser)
    if op == OptSub or op == OptNot or op == OptFlip:
        return op
    return OP_NOT_UNARY


# static btokentype get_assign_op(bparser *parser)
# {
#     btokentype op = next_type(parser);
#     if ((op >= OptAssign && op <= OptRsfAssign) || op == OptWalrus) { return op; }
#     return OP_NOT_ASSIGN;
# }
def get_assign_op(parser):
    """Return the assignment operator token type, or OP_NOT_ASSIGN."""
    op = next_type(parser)
    if (OptAssign <= op <= OptRsfAssign) or op == OptWalrus:
        return op
    return OP_NOT_ASSIGN


# ============================================================================
# Variable resolution (from be_parser.c)
# ============================================================================

# static int find_localvar(bfuncinfo *finfo, bstring *s, int begin)
# {
#     int i, count = be_list_count(finfo->local);
#     bvalue *var = be_list_data(finfo->local);
#     for (i = count - 1; i >= begin; --i) {
#         if (be_eqstr(var[i].v.s, s)) { return i; }
#     }
#     return -1;
# }
def find_localvar(finfo, s, begin):
    """Find a local variable by name, searching backwards from end to begin."""
    count = be_list_count(finfo.local)
    data = be_list_data(finfo.local)
    for i in range(count - 1, begin - 1, -1):
        if be_eqstr(data[i].v, s):
            return i
    return -1


# static int new_localvar(bparser *parser, bstring *name)
def new_localvar(parser, name):
    """Create a new local variable, or error if redefined in same scope."""
    finfo = parser.finfo
    reg = find_localvar(finfo, name, finfo.binfo.nactlocals)
    if reg == -1:
        if comp_is_strict(parser.vm):
            name_s = be_str2cstr(name)
            if find_localvar(finfo, name, 0) >= 0 and name_s[0:1] != '.':
                push_error(parser,
                    "strict: redefinition of '%s' from outer scope", name_s)
        reg = be_list_count(finfo.local)
        var = be_list_push(parser.vm, finfo.local, None)
        var_setstr(var, name)
        if reg >= finfo.freereg:
            be_code_allocregs(finfo, 1)
        begin_varinfo(parser, name)
    else:
        name_s = be_str2cstr(name)
        push_error(parser, "redefinition of '%s'", name_s)
    return reg


# static int find_upval(bfuncinfo *finfo, bstring *s)
# {
#     bvm *vm = finfo->lexer->vm;
#     bvalue *desc = be_map_findstr(vm, finfo->upval, s);
#     if (desc) { return upval_index(desc->v.i); }
#     return -1;
# }
def find_upval(finfo, s):
    """Find an upvalue by name. Returns its index or -1."""
    vm = finfo.lexer.vm
    desc = be_map_findstr(vm, finfo.upval, s)
    if desc is not None:
        return upval_index(var_toint(desc))
    return -1


# static void mark_upval(bfuncinfo *finfo, int level)
# {
#     bblockinfo *binfo = finfo->prev->binfo;
#     while (binfo->nactlocals > level) {
#         binfo = binfo->prev;
#     }
#     binfo->hasupval = 1;
# }
def mark_upval(finfo, level):
    """Mark the block containing the upvalue's stack slot."""
    binfo = finfo.prev.binfo
    while binfo.nactlocals > level:
        binfo = binfo.prev
    binfo.hasupval = 1


# static int new_upval(bvm *vm, bfuncinfo *finfo, bstring *name, bexpdesc *var)
def new_upval(vm, finfo, name, var):
    """Create a new upvalue entry in the function's upval map."""
    target = var.v.idx
    instack = (var.type == ETLOCAL)
    if instack:
        mark_upval(finfo, target)
    index = _be_map_count(finfo.upval)
    if index >= 255:
        name_s = be_str2cstr(name)
        be_lexerror(finfo.lexer,
            "too many upvalues (in '%s')" % name_s)
    desc = be_map_insertstr(vm, finfo.upval, name, None)
    var_setint(desc, upval_desc(index, target, instack))
    return index


# static void new_var(bparser *parser, bstring *name, bexpdesc *var)
def new_var(parser, name, var):
    """Create a new variable in the current context."""
    finfo = parser.finfo
    if comp_is_strict(parser.vm):
        if be_builtin_find(parser.vm, name) >= 0:
            name_s = be_str2cstr(name)
            push_error(parser,
                "strict: redefinition of builtin '%s'", name_s)
    if finfo.prev or finfo.binfo.prev or parser.islocal:
        init_exp(var, ETLOCAL, 0)
        var.v.idx = new_localvar(parser, name)
    else:
        init_exp(var, ETGLOBAL, 0)
        var.v.idx = be_global_find(parser.vm, name)
        if var.v.idx < 0:
            var.v.idx = be_global_new(parser.vm, name)
        if var.v.idx > int(IBx_MASK):
            name_s = be_str2cstr(name)
            push_error(parser,
                "too many global variables (in '%s')", name_s)
        if comp_is_named_gbl(parser.vm):
            key = bexpdesc()
            init_exp(key, ETSTRING, 0)
            key.v.s = name
            init_exp(var, ETNGLOBAL, 0)
            idx = be_code_resolve(parser.finfo, key)
            var.v.idx = idx
            # C union aliasing: v.idx and v.ss.idx share memory
            var.v.ss.idx = idx


# static int singlevaraux(bvm *vm, bfuncinfo *finfo, bstring *s, bexpdesc *var)
def singlevaraux(vm, finfo, s, var):
    """Recursively resolve a variable through local, upvalue, and global scopes."""
    if finfo is None:
        return ETVOID
    else:
        idx = find_localvar(finfo, s, 0)
        if idx >= 0:
            init_exp(var, ETLOCAL, 0)
            var.v.idx = idx
            return ETLOCAL
        else:
            idx = find_upval(finfo, s)
            if idx < 0:
                res = singlevaraux(vm, finfo.prev, s, var)
                if res == ETUPVAL or res == ETLOCAL:
                    idx = new_upval(vm, finfo, s, var)
                else:
                    idx = be_global_find(vm, s)
                    if idx >= 0:
                        if idx < be_builtin_count(vm):
                            return ETGLOBAL
                        else:
                            return ETNGLOBAL if comp_is_named_gbl(vm) else ETGLOBAL
                    else:
                        return ETVOID
            init_exp(var, ETUPVAL, idx)
            return ETUPVAL


# static void singlevar(bparser *parser, bexpdesc *var)
def singlevar(parser, var):
    """Resolve a single variable from the current token."""
    varname = next_token(parser).u.s
    type_ = singlevaraux(parser.vm, parser.finfo, varname, var)
    if type_ == ETVOID:
        init_exp(var, ETVOID, 0)
        var.v.s = varname
    elif type_ == ETGLOBAL:
        init_exp(var, ETGLOBAL, 0)
        var.v.idx = be_global_find(parser.vm, varname)
    elif type_ == ETNGLOBAL:
        key = bexpdesc()
        init_exp(key, ETSTRING, 0)
        key.v.s = varname
        init_exp(var, ETNGLOBAL, 0)
        idx = be_code_resolve(parser.finfo, key)
        var.v.idx = idx
        # In C, v.idx and v.ss.idx share memory via a union.
        # The code generator reads e.v.ss.idx for OP_GETNGBL/OP_SETNGBL,
        # so we must keep both in sync.
        var.v.ss.idx = idx
    # else: ETLOCAL or ETUPVAL — already set by singlevaraux


# ============================================================================
# Function definition helpers (from be_parser.c)
# ============================================================================

# static void func_vararg(bparser *parser)
def func_vararg(parser):
    """Parse a vararg parameter: '*' ID."""
    from berry_port.be_object import BE_VA_VARARG
    v = bexpdesc()
    match_token(parser, OptMul)  # skip '*'
    s = next_token(parser).u.s
    match_token(parser, TokenId)  # match and skip ID
    new_var(parser, s, v)
    parser.finfo.proto.varg |= BE_VA_VARARG


# static void func_varlist(bparser *parser)
def func_varlist(parser):
    """Parse function parameter list: '(' [ID {',' ID}] [, '*' ID] ')'."""
    v = bexpdesc()
    type_lbk = next_type(parser)
    if type_lbk == OptSpaceLBK or type_lbk == OptCallLBK:
        match_token(parser, type_lbk)  # skip '('
    else:
        match_token(parser, OptCallLBK)  # raise error

    if next_type(parser) == OptMul:
        func_vararg(parser)
    else:
        s = _match_id(parser)
        if s is not None:
            new_var(parser, s, v)
            while match_skip(parser, OptComma):
                if next_type(parser) == OptMul:
                    func_vararg(parser)
                    break
                else:
                    s = next_token(parser).u.s
                    match_token(parser, TokenId)
                    new_var(parser, s, v)

    match_token(parser, OptRBK)  # skip ')'
    parser.finfo.proto.argc = parser.finfo.freereg


# static bproto* funcbody(bparser *parser, bstring *name, bclass *c, int type)
def funcbody(parser, name, c, type_):
    """Parse a function body including arg list and body. Returns bproto."""
    from berry_port.be_object import BE_VA_METHOD, BE_VA_STATICMETHOD
    finfo = bfuncinfo()
    binfo = bblockinfo()

    begin_func(parser, finfo, binfo)
    finfo.proto.name = name

    if type_ & FUNC_METHOD:
        new_localvar(parser, parser_newstr(parser, "self"))
        finfo.proto.varg |= BE_VA_METHOD

    func_varlist(parser)

    if (type_ & FUNC_STATIC) and (c is not None):
        e1 = bexpdesc()
        e2 = bexpdesc()
        new_var(parser, parser_newstr(parser, "_class"), e1)
        init_exp(e2, ETCONST, 0)
        be_code_implicit_class(parser.finfo, e2, c)
        be_code_setvar(parser.finfo, e1, e2, False)
        finfo.proto.varg |= BE_VA_STATICMETHOD

    stmtlist(parser)
    end_func(parser)
    match_token(parser, KeyEnd)  # skip 'end'
    return finfo.proto


# static void anon_func(bparser *parser, bexpdesc *e)
def anon_func(parser, e):
    """Parse an anonymous function definition."""
    name = parser_newstr(parser, "_anonymous_")
    scan_next_token(parser)  # skip 'def'
    proto = funcbody(parser, name, None, FUNC_ANONYMOUS)
    init_exp(e, ETPROTO, be_code_proto(parser.finfo, proto))
    be_stackpop(parser.vm, 1)


# static void lambda_varlist(bparser *parser)
def lambda_varlist(parser):
    """Parse lambda parameter list: [ID {',' ID}] '->'."""
    v = bexpdesc()
    s = _match_id(parser)
    if s is not None:
        comma = (next_type(parser) == OptComma)
        new_var(parser, s, v)
        while next_type(parser) != OptArrow:
            if comma:
                match_token(parser, OptComma)
            s = next_token(parser).u.s
            match_token(parser, TokenId)
            new_var(parser, s, v)
    match_token(parser, OptArrow)  # skip '->'
    parser.finfo.proto.argc = parser.finfo.freereg


# static void lambda_expr(bparser *parser, bexpdesc *e)
def lambda_expr(parser, e):
    """Parse a lambda expression: '/' params '->' expr."""
    finfo = bfuncinfo()
    binfo = bblockinfo()
    name = parser_newstr(parser, "<lambda>")
    scan_next_token(parser)  # skip '/'
    begin_func(parser, finfo, binfo)
    finfo.proto.name = name
    lambda_varlist(parser)
    e1 = bexpdesc()
    expr(parser, e1)
    check_var(parser, e1)
    be_code_ret(parser.finfo, e1)
    end_func(parser)
    init_exp(e, ETPROTO, be_code_proto(parser.finfo, finfo.proto))
    be_stackpop(parser.vm, 1)


# ============================================================================
# Primitive type instantiation (from be_parser.c)
# ============================================================================

# static void new_primtype(bparser *parser, const char *type, bexpdesc *e)
def new_primtype(parser, type_name, e):
    """Instantiate a builtin type by name (list or map)."""
    vm = parser.vm
    finfo = parser.finfo
    scan_next_token(parser)
    idx = be_builtin_find(vm, parser_newstr(parser, type_name))
    init_exp(e, ETGLOBAL, idx)
    idx = be_code_nextreg(finfo, e)
    be_code_call(finfo, idx, 0)
    e.type = ETLOCAL


# ============================================================================
# List and map expression helpers (from be_parser.c)
# ============================================================================

# static void list_nextmember(bparser *parser, bexpdesc *l)
def list_nextmember(parser, l):
    """Parse and add the next member to a list literal."""
    e = bexpdesc()
    v = _clone_expdesc(l)
    finfo = parser.finfo
    expr(parser, e)
    check_var(parser, e)
    be_code_binop(finfo, OptConnect, v, e, -1)
    be_code_freeregs(finfo, 1)


# static void map_nextmember(bparser *parser, bexpdesc *l)
def map_nextmember(parser, l):
    """Parse and add the next key:value pair to a map literal."""
    e = bexpdesc()
    v = _clone_expdesc(l)
    finfo = parser.finfo
    expr(parser, e)       # key
    check_var(parser, e)
    be_code_index(finfo, v, e)
    match_token(parser, OptColon)  # ':'
    expr(parser, e)       # value
    check_var(parser, e)
    be_code_setvar(finfo, v, e, False)


# static void list_expr(bparser *parser, bexpdesc *e)
def list_expr(parser, e):
    """Parse a list literal: '[' {expr ','} [expr] ']'."""
    new_primtype(parser, "list", e)
    while next_type(parser) != OptRSB:
        list_nextmember(parser, e)
        if not match_skip(parser, OptComma):
            break
    e.type = ETREG
    match_token(parser, OptRSB)


# static void map_expr(bparser *parser, bexpdesc *e)
def map_expr(parser, e):
    """Parse a map literal: '{' {expr ':' expr ','} [expr ':' expr] '}'."""
    new_primtype(parser, "map", e)
    while next_type(parser) != OptRBR:
        map_nextmember(parser, e)
        if not match_skip(parser, OptComma):
            break
    e.type = ETREG
    match_token(parser, OptRBR)


# ============================================================================
# Expression list (from be_parser.c)
# ============================================================================

# static int exprlist(bparser *parser, bexpdesc *e)
def exprlist(parser, e):
    """Parse a comma-separated expression list. Returns count."""
    finfo = parser.finfo
    n = 1
    expr(parser, e)
    check_var(parser, e)
    be_code_nextreg(finfo, e)
    while match_skip(parser, OptComma):
        expr(parser, e)
        check_var(parser, e)
        be_code_nextreg(finfo, e)
        n += 1
    return n


# ============================================================================
# Suffix expressions: call, member, index (from be_parser.c)
# ============================================================================

# static void call_expr(bparser *parser, bexpdesc *e)
def call_expr(parser, e):
    """Parse a function/method call expression."""
    args = bexpdesc()
    finfo = parser.finfo
    argc = 0
    ismember = (e.type == ETMEMBER)

    parser.finfo.binfo.sideeffect = 1
    check_var(parser, e)

    if ismember:
        base = be_code_getmethod(finfo, e)
    else:
        base = be_code_nextreg(finfo, e)

    scan_next_token(parser)  # skip '('
    if next_type(parser) != OptRBK:
        argc = exprlist(parser, args)
    match_token(parser, OptRBK)  # skip ')'
    argc += ismember
    be_code_call(finfo, base, argc)
    if e.type != ETREG:
        e.type = ETREG
        e.v.idx = base


# static void member_expr(bparser *parser, bexpdesc *e)
def member_expr(parser, e):
    """Parse a member access expression: '.' ID or '.' '(' expr ')'."""
    check_var(parser, e)
    scan_next_token(parser)  # skip '.'
    s = _match_id(parser)
    if s is not None:
        key = bexpdesc()
        init_exp(key, ETSTRING, 0)
        key.v.s = s
        be_code_member(parser.finfo, e, key)
    elif next_type(parser) == OptCallLBK:
        scan_next_token(parser)  # skip '(' — must be no space before
        key = bexpdesc()
        expr(parser, key)
        check_var(parser, key)
        match_token(parser, OptRBK)  # skip ')'
        be_code_member(parser.finfo, e, key)
    else:
        push_error(parser, "invalid syntax near '%s'",
            be_token2str(parser.vm, next_token(parser)))


# static void index_expr(bparser *parser, bexpdesc *e)
def index_expr(parser, e):
    """Parse an index expression: '[' expr ']'."""
    e1 = bexpdesc()
    check_var(parser, e)
    scan_next_token(parser)  # skip '['
    expr(parser, e1)
    check_var(parser, e1)
    be_code_index(parser.finfo, e, e1)
    match_token(parser, OptRSB)  # skip ']'


# ============================================================================
# Primary and suffix expression parsing (from be_parser.c)
# ============================================================================

# static void simple_expr(bparser *parser, bexpdesc *e)
def simple_expr(parser, e):
    """Parse a simple (atomic) expression."""
    tt = next_type(parser)
    if tt == TokenInteger:
        init_exp(e, ETINT, next_token(parser).u.i)
    elif tt == TokenReal:
        init_exp(e, ETREAL, 0)
        e.v.r = next_token(parser).u.r
    elif tt == TokenString:
        init_exp(e, ETSTRING, 0)
        e.v.s = next_token(parser).u.s
    elif tt == TokenId:
        singlevar(parser, e)
    elif tt == KeyTrue:
        init_exp(e, ETBOOL, 1)
    elif tt == KeyFalse:
        init_exp(e, ETBOOL, 0)
    elif tt == KeyNil:
        init_exp(e, ETNIL, 0)
    else:
        return  # unknown expr
    scan_next_token(parser)


# static void primary_expr(bparser *parser, bexpdesc *e)
def primary_expr(parser, e):
    """Parse a primary expression (grouping, list, map, anon func, lambda, or simple)."""
    tt = next_type(parser)
    if tt == OptSpaceLBK or tt == OptCallLBK:
        scan_next_token(parser)  # skip '('
        expr(parser, e)
        check_var(parser, e)
        match_token(parser, OptRBK)  # skip ')'
    elif tt == OptLSB:
        list_expr(parser, e)
    elif tt == OptLBR:
        map_expr(parser, e)
    elif tt == KeyDef:
        anon_func(parser, e)
    elif tt == OptDiv:
        lambda_expr(parser, e)
    else:
        simple_expr(parser, e)


# static void suffix_expr(bparser *parser, bexpdesc *e)
def suffix_expr(parser, e):
    """Parse a suffix expression chain: primary { call | member | index }."""
    primary_expr(parser, e)
    while True:
        tt = next_type(parser)
        if tt == OptCallLBK:
            call_expr(parser, e)
        elif tt == OptDot:
            member_expr(parser, e)
        elif tt == OptLSB:
            index_expr(parser, e)
        else:
            return


# static void suffix_alloc_reg(bparser *parser, bexpdesc *l)
def suffix_alloc_reg(parser, l):
    """Allocate a register for suffix expression caching if needed."""
    finfo = parser.finfo
    is_suffix = (l.type == ETINDEX or l.type == ETMEMBER)
    is_suffix_reg = l.v.ss.tt in (ETREG, ETLOCAL, ETGLOBAL, ETNGLOBAL)
    is_global = (l.type == ETGLOBAL or l.type == ETNGLOBAL)
    is_upval = (l.type == ETUPVAL)
    if is_global or is_upval or (is_suffix and is_suffix_reg):
        be_code_allocregs(finfo, 1)


# ============================================================================
# Compound assignment (from be_parser.c)
# ============================================================================

# static void compound_assign(bparser *parser, int op, bexpdesc *l, bexpdesc *r)
def compound_assign(parser, op, l, r):
    """Handle compound assignment (+=, -=, etc.)."""
    dst = -1
    if op != OptAssign:
        check_var(parser, l)
        dst = parser.finfo.freereg
        suffix_alloc_reg(parser, l)
    expr(parser, r)
    check_var(parser, r)
    if op != OptAssign:
        e = _clone_expdesc(l)
        if op < OptAndAssign:
            op = op - OptAddAssign + OptAdd
        else:
            op = op - OptAndAssign + OptBitAnd
        be_code_binop(parser.finfo, op, e, r, dst)
        _copy_expdesc(r, e)


# ============================================================================
# New variable check (from be_parser.c)
# ============================================================================

# static int check_newvar(bparser *parser, bexpdesc *e)
def check_newvar(parser, e):
    """Check if we need to create a new local variable for assignment."""
    if e.type == ETGLOBAL:
        if e.v.idx < be_builtin_count(parser.vm):
            e.v.s = be_builtin_name(parser.vm, e.v.idx)
            if comp_is_strict(parser.vm):
                name_s = be_str2cstr(e.v.s)
                push_error(parser,
                    "strict: redefinition of builtin '%s'", name_s)
            return True
        return False
    if comp_is_strict(parser.vm):
        finfo = parser.finfo
        if (e.type == ETVOID) and (finfo.prev or finfo.binfo.prev or parser.islocal):
            name_s = be_str2cstr(e.v.s)
            push_error(parser,
                "strict: no global '%s', did you mean 'var %s'?",
                name_s, name_s)
    return e.type == ETVOID


# ============================================================================
# Expression parsing (from be_parser.c)
# ============================================================================

# static void assign_expr(bparser *parser)
def assign_expr(parser):
    """Parse an assignment expression or plain expression statement."""
    e = bexpdesc()
    line = parser.lexer.linenumber
    parser.finfo.binfo.sideeffect = 0
    expr(parser, e)
    check_symbol(parser, e)
    op = get_assign_op(parser)
    if op != OP_NOT_ASSIGN:
        e1 = bexpdesc()
        parser.finfo.binfo.sideeffect = 1
        scan_next_token(parser)
        compound_assign(parser, op, e, e1)
        if check_newvar(parser, e):
            new_var(parser, e.v.s, e)
        if be_code_setvar(parser.finfo, e, e1, False):
            parser.lexer.linenumber = line
            parser_error(parser, "try to assign constant expressions.")
    else:
        be_code_resolve(parser.finfo, e)
        if e.type >= ETMEMBER:
            finfo = parser.finfo
            finfo.freereg = be_list_count(finfo.local)
        elif e.type == ETVOID:
            parser.lexer.linenumber = line
            check_var(parser, e)


# static void cond_expr(bparser *parser, bexpdesc *e)
def cond_expr(parser, e):
    """Parse a conditional (ternary) expression: expr '?' expr ':' expr."""
    if next_type(parser) == OptQuestion:
        jl = NO_JUMP
        finfo = parser.finfo
        check_var(parser, e)
        scan_next_token(parser)  # skip '?'
        be_code_jumpbool(finfo, e, False)
        jf = e.f
        expr(parser, e)
        check_var(parser, e)
        be_code_nextreg(finfo, e)
        be_code_freeregs(finfo, 1)
        # be_code_conjump modifies jl in-place; we use a list wrapper
        jl_ref = [jl]
        be_code_conjump(finfo, jl_ref, be_code_jump(finfo))
        jl = jl_ref[0]
        be_code_patchjump(finfo, jf)
        match_token(parser, OptColon)  # skip ':'
        expr(parser, e)
        check_var(parser, e)
        e.v.idx = be_code_nextreg(finfo, e)
        be_code_patchjump(finfo, jl)
        e.type = ETREG


# static void sub_expr(bparser *parser, bexpdesc *e, int prio)
def sub_expr(parser, e, prio):
    """Parse a sub-expression with operator precedence."""
    finfo = parser.finfo
    op = get_unary_op(parser)
    if op != OP_NOT_UNARY:
        scan_next_token(parser)
        line = parser.lexer.linenumber
        sub_expr(parser, e, UNARY_OP_PRIO)
        check_var(parser, e)
        res = be_code_unop(finfo, op, e)
        if res:
            parser.lexer.linenumber = line
            push_error(parser, "wrong type argument to unary '%s'",
                "negative" if res == 1 else "bit-flip")
    else:
        suffix_expr(parser, e)

    op = get_binop(parser)
    while op != OP_NOT_BINARY and prio > binary_op_prio(op):
        e2 = bexpdesc()
        check_var(parser, e)
        scan_next_token(parser)
        be_code_prebinop(finfo, op, e)
        if op == OptConnect:
            parser.finfo.binfo.sideeffect = 1
        init_exp(e2, ETVOID, 0)
        sub_expr(parser, e2, binary_op_prio(op))
        if (op == OptConnect) and (e2.type == ETVOID) and (e2.v.s is None):
            init_exp(e2, ETINT, M_IMAX)
        else:
            check_var(parser, e2)
        be_code_binop(finfo, op, e, e2, -1)
        op = get_binop(parser)

    if prio == ASSIGN_OP_PRIO:
        cond_expr(parser, e)


# static void walrus_expr(bparser *parser, bexpdesc *e)
def walrus_expr(parser, e):
    """Parse a walrus expression: sub_expr [':=' expr]."""
    line = parser.lexer.linenumber
    sub_expr(parser, e, ASSIGN_OP_PRIO)
    op = next_type(parser)
    if op == OptWalrus:
        check_symbol(parser, e)
        e1 = _clone_expdesc(e)
        parser.finfo.binfo.sideeffect = 1
        scan_next_token(parser)  # skip ':='
        expr(parser, e)
        check_var(parser, e)
        if check_newvar(parser, e1):
            new_var(parser, e1.v.s, e1)
        if be_code_setvar(parser.finfo, e1, e, True):
            parser.lexer.linenumber = line
            parser_error(parser, "try to assign constant expressions.")


# static void expr(bparser *parser, bexpdesc *e)
def expr(parser, e):
    """Parse a full expression."""
    init_exp(e, ETVOID, 0)
    walrus_expr(parser, e)


# static void expr_stmt(bparser *parser)
def expr_stmt(parser):
    """Parse an expression statement."""
    assign_expr(parser)


# ============================================================================
# Block follow check (from be_parser.c)
# ============================================================================

# static int block_follow(bparser *parser)
def block_follow(parser):
    """Return True if the current token allows more statements in a block."""
    tt = next_type(parser)
    if tt in (KeyElse, KeyElif, KeyEnd, KeyExcept, TokenEOS):
        return False
    return True


# ============================================================================
# Condition helpers (from be_parser.c)
# ============================================================================

# static int cond_stmt(bparser *parser)
def cond_stmt(parser):
    """Parse a condition expression and emit a conditional jump."""
    e = bexpdesc()
    match_notoken(parser, OptRBK)
    expr(parser, e)
    check_var(parser, e)
    be_code_jumpbool(parser.finfo, e, False)
    return e.f


# static void condition_block(bparser *parser, int *jmp)
def condition_block(parser, jmp_ref):
    """Parse a condition followed by a block, patching jumps."""
    finfo = parser.finfo
    br = cond_stmt(parser)
    block(parser, 0)
    if next_type(parser) == KeyElif or next_type(parser) == KeyElse:
        jmp_ref[0] = be_code_conjump(finfo, jmp_ref[0], be_code_jump(finfo))
    be_code_patchjump(finfo, br)


# ============================================================================
# Statement parsers (from be_parser.c)
# ============================================================================

# static void if_stmt(bparser *parser)
def if_stmt(parser):
    """Parse an if/elif/else statement."""
    jl_ref = [NO_JUMP]
    scan_next_token(parser)  # skip 'if'
    condition_block(parser, jl_ref)
    while match_skip(parser, KeyElif):
        condition_block(parser, jl_ref)
    if match_skip(parser, KeyElse):
        block(parser, 0)
    match_token(parser, KeyEnd)
    be_code_patchjump(parser.finfo, jl_ref[0])


# static void do_stmt(bparser *parser)
def do_stmt(parser):
    """Parse a do/end block."""
    scan_next_token(parser)  # skip 'do'
    block(parser, 0)
    match_token(parser, KeyEnd)


# static void while_stmt(bparser *parser)
def while_stmt(parser):
    """Parse a while loop."""
    finfo = parser.finfo
    scan_next_token(parser)  # skip 'while'
    binfo = bblockinfo()
    begin_block(parser.finfo, binfo, BLOCK_LOOP)
    brk = cond_stmt(parser)
    stmtlist(parser)
    end_block(parser)
    be_code_patchjump(finfo, brk)
    match_token(parser, KeyEnd)


# static bstring* for_itvar(bparser *parser)
def for_itvar(parser):
    """Parse the iteration variable in a for loop."""
    s = _match_id(parser)
    if s is None:
        push_error(parser,
            "missing iteration variable before '%s'", token2str(parser))
    return s


# static void for_init(bparser *parser, bexpdesc *v)
def for_init(parser, v):
    """Initialize the for loop iterator: .it = __iterator__(expr)."""
    e = bexpdesc()
    finfo = parser.finfo
    s = parser_newstr(parser, "__iterator__")
    init_exp(e, ETGLOBAL, be_builtin_find(parser.vm, s))
    be_code_nextreg(finfo, e)
    expr(parser, v)
    check_var(parser, v)
    be_code_nextreg(finfo, v)
    be_code_call(finfo, e.v.idx, 1)
    be_code_freeregs(finfo, 1)
    s = parser_newstr(parser, ".it")
    init_exp(v, ETLOCAL, new_localvar(parser, s))


# static void for_iter(bparser *parser, bstring *var, bexpdesc *it)
def for_iter(parser, var, it):
    """Parse the for loop body: itvar = .it()."""
    e = bexpdesc()
    finfo = parser.finfo
    finfo.binfo.beginpc = finfo.pc
    init_exp(e, ETLOCAL, new_localvar(parser, var))
    be_code_setvar(finfo, e, it, False)
    be_code_call(finfo, e.v.idx, 0)
    stmtlist(parser)


# static void for_leave(bparser *parser, int jcatch, int beginpc)
def for_leave(parser, jcatch, beginpc):
    """Emit the for loop exit code with stop_iteration exception handling."""
    e = bexpdesc()
    finfo = parser.finfo
    jbrk = finfo.binfo.breaklist
    init_exp(e, ETSTRING, 0)
    e.v.s = parser_newstr(parser, "stop_iteration")
    end_block_ex(parser, beginpc)
    if jbrk != NO_JUMP:
        be_code_exblk(finfo, 1)
        jbrk = be_code_jump(finfo)
    jcatch_ref = [jcatch]
    be_code_conjump(finfo, jcatch_ref, finfo.pc)
    jcatch = jcatch_ref[0]
    be_code_catch(finfo, be_code_nextreg(finfo, e), 1, 0, None)
    be_code_raise(finfo, None, None)
    jbrk_ref = [jbrk]
    be_code_conjump(finfo, jbrk_ref, finfo.pc)
    be_code_freeregs(finfo, 1)


# static void for_stmt(bparser *parser)
def for_stmt(parser):
    """Parse a for loop: for ID : expr block end."""
    finfo = parser.finfo
    beginpc = finfo.pc
    scan_next_token(parser)  # skip 'for'
    binfo = bblockinfo()
    begin_block(finfo, binfo, BLOCK_EXCEPT | BLOCK_LOOP)
    var = for_itvar(parser)
    match_token(parser, OptColon)  # skip ':'
    iter_ = bexpdesc()
    for_init(parser, iter_)
    jcatch = be_code_exblk(finfo, 0)
    for_iter(parser, var, iter_)
    for_leave(parser, jcatch, beginpc)
    match_token(parser, KeyEnd)


# static bblockinfo* break_block(bparser *parser)
def break_block(parser):
    """Find the enclosing loop block for break/continue, leaving exception blocks."""
    try_depth = 0
    binfo = parser.finfo.binfo
    scan_next_token(parser)  # skip 'break' or 'continue'
    while binfo and not (binfo.type & BLOCK_LOOP):
        if binfo.type & BLOCK_EXCEPT:
            try_depth += 1
        binfo = binfo.prev
    if binfo and try_depth:
        be_code_exblk(parser.finfo, try_depth)
    return binfo


# static void break_stmt(bparser *parser)
def break_stmt(parser):
    """Parse a break statement."""
    f = parser.finfo
    binfo = break_block(parser)
    if binfo is not None:
        binfo_ref = [binfo.breaklist]
        be_code_conjump(f, binfo_ref, be_code_jump(f))
        binfo.breaklist = binfo_ref[0]
    else:
        parser_error(parser, "break not loop")


# static void continue_stmt(bparser *parser)
def continue_stmt(parser):
    """Parse a continue statement."""
    f = parser.finfo
    b = break_block(parser)
    if b is not None:
        b_ref = [b.continuelist]
        be_code_conjump(f, b_ref, be_code_jump(f))
        b.continuelist = b_ref[0]
    else:
        parser_error(parser, "continue not loop")


# static void return_stmt(bparser *parser)
def return_stmt(parser):
    """Parse a return statement."""
    e = bexpdesc()
    scan_next_token(parser)  # skip 'return'
    expr(parser, e)
    if e.v.s:
        check_var(parser, e)
    be_code_ret(parser.finfo, e)


# static void def_stmt(bparser *parser)
def def_stmt(parser):
    """Parse a function definition statement."""
    e = bexpdesc()
    finfo = parser.finfo
    scan_next_token(parser)  # skip 'def'
    proto = funcbody(parser, func_name(parser, e, 0), None, 0)
    be_code_closure(finfo, e, be_code_proto(finfo, proto))
    be_stackpop(parser.vm, 1)


# ============================================================================
# Class statement helpers (from be_parser.c)
# ============================================================================

# static bbool isoverloadable(btokentype type)
def isoverloadable(type_):
    """Check if a token type is an overloadable operator."""
    return (OptAdd <= type_ <= OptConnect) or type_ == OptFlip or type_ == OptSpaceLBK


# static bstring* func_name(bparser* parser, bexpdesc* e, int ismethod)
def func_name(parser, e, ismethod):
    """Parse a function name (identifier or overloadable operator for methods)."""
    type_ = next_type(parser)
    if type_ == TokenId:
        name = next_token(parser).u.s
        if not ismethod:
            new_var(parser, name, e)
        scan_next_token(parser)
        return name
    elif ismethod and isoverloadable(type_):
        scan_next_token(parser)
        # '-*' negative operator
        if type_ == OptSub and next_type(parser) == OptMul:
            scan_next_token(parser)
            return parser_newstr(parser, "-*")
        # '()' call operator
        if type_ == OptSpaceLBK and next_type(parser) == OptRBK:
            scan_next_token(parser)
            return parser_newstr(parser, "()")
        return parser_newstr(parser, be_tokentype2str(type_))
    push_error(parser,
        "the token '%s' is not a valid function name.", token2str(parser))
    return None


# static void check_class_attr(bparser *parser, bclass *c, bstring *attr)
def check_class_attr(parser, c, attr):
    """Check that a class attribute is not already defined."""
    if be_class_attribute(parser.vm, c, attr) != BE_NONE:
        attr_s = be_str2cstr(attr)
        push_error(parser,
            "redefinition of the attribute '%s'", attr_s)


# static void classvar_stmt(bparser *parser, bclass *c)
def classvar_stmt(parser, c):
    """Parse class variable declarations: 'var' ID {',' ID}."""
    scan_next_token(parser)  # skip 'var'
    name = _match_id(parser)
    if name is not None:
        check_class_attr(parser, c, name)
        be_class_member_bind(parser.vm, c, name, True)
        while match_skip(parser, OptComma):
            name = _match_id(parser)
            if name is not None:
                check_class_attr(parser, c, name)
                be_class_member_bind(parser.vm, c, name, True)
            else:
                parser_error(parser, "class var error")
    else:
        parser_error(parser, "class var error")


# static void class_static_assignment_expr(bparser *parser, bexpdesc *e, bstring *name)
def class_static_assignment_expr(parser, e, name):
    """Parse optional static member assignment: '=' expr."""
    if match_skip(parser, OptAssign):
        e2 = bexpdesc()
        expr(parser, e2)
        e1 = _clone_expdesc(e)
        key = bexpdesc()
        init_exp(key, ETSTRING, 0)
        key.v.s = name
        be_code_member(parser.finfo, e1, key)
        be_code_setvar(parser.finfo, e1, e2, False)


# static void classdef_stmt(bparser *parser, bclass *c, bbool is_static)
def classdef_stmt(parser, c, is_static):
    """Parse a method definition inside a class."""
    e = bexpdesc()
    scan_next_token(parser)  # skip 'def'
    name = func_name(parser, e, 1)
    check_class_attr(parser, c, name)
    proto = funcbody(parser, name, c, FUNC_STATIC if is_static else FUNC_METHOD)
    be_class_method_bind(parser.vm, c, proto.name, proto, is_static)
    be_stackpop(parser.vm, 1)


# static void classstatic_stmt(bparser *parser, bclass *c, bexpdesc *e)
def classstatic_stmt(parser, c, e):
    """Parse a static declaration inside a class."""
    scan_next_token(parser)  # skip 'static'
    if next_type(parser) == KeyDef:
        classdef_stmt(parser, c, True)
    elif next_type(parser) == KeyClass:
        classstaticclass_stmt(parser, c, e)
    else:
        if next_type(parser) == KeyVar:
            scan_next_token(parser)  # skip 'var' if any
        name = _match_id(parser)
        if name is not None:
            check_class_attr(parser, c, name)
            be_class_member_bind(parser.vm, c, name, False)
            class_static_assignment_expr(parser, e, name)
            while match_skip(parser, OptComma):
                name = _match_id(parser)
                if name is not None:
                    check_class_attr(parser, c, name)
                    be_class_member_bind(parser.vm, c, name, False)
                    class_static_assignment_expr(parser, e, name)
                else:
                    parser_error(parser, "class static error")
        else:
            parser_error(parser, "class static error")


# static void class_inherit(bparser *parser, bexpdesc *e)
def class_inherit(parser, e):
    """Parse class inheritance: ':' expr."""
    if next_type(parser) == OptColon:
        ec = _clone_expdesc(e)
        e1 = bexpdesc()
        scan_next_token(parser)  # skip ':'
        expr(parser, e1)
        check_var(parser, e1)
        be_code_setsuper(parser.finfo, ec, e1)


# static void class_block(bparser *parser, bclass *c, bexpdesc *e)
def class_block(parser, c, e):
    """Parse the body of a class definition."""
    while block_follow(parser):
        tt = next_type(parser)
        if tt == KeyVar:
            classvar_stmt(parser, c)
        elif tt == KeyStatic:
            classstatic_stmt(parser, c, e)
        elif tt == KeyDef:
            classdef_stmt(parser, c, False)
        elif tt == OptSemic:
            scan_next_token(parser)
        else:
            push_error(parser,
                "unexpected token '%s'", token2str(parser))


# static void class_stmt(bparser *parser)
def class_stmt(parser):
    """Parse a class definition statement."""
    scan_next_token(parser)  # skip 'class'
    name = _match_id(parser)
    if name is not None:
        e = bexpdesc()
        c = be_newclass(parser.vm, name, None)
        new_var(parser, name, e)
        be_code_class(parser.finfo, e, c)
        class_inherit(parser, e)

        binfo = bblockinfo()
        begin_block(parser.finfo, binfo, 0)

        class_str = parser_newstr(parser, "_class")
        e1 = bexpdesc()
        init_exp(e1, ETLOCAL, 0)
        e1.v.idx = new_localvar(parser, class_str)
        be_code_setvar(parser.finfo, e1, e, True)
        begin_varinfo(parser, class_str)

        class_block(parser, c, e)
        end_block(parser)

        be_class_compress(parser.vm, c)
        match_token(parser, KeyEnd)
    else:
        parser_error(parser, "class name error")


# static void classstaticclass_stmt(bparser *parser, bclass *c_out, bexpdesc *e_out)
def classstaticclass_stmt(parser, c_out, e_out):
    """Parse a static nested class definition."""
    scan_next_token(parser)  # skip 'class'
    name = _match_id(parser)
    if name is not None:
        e_class = bexpdesc()
        check_class_attr(parser, c_out, name)
        be_class_member_bind(parser.vm, c_out, name, False)
        c = be_newclass(parser.vm, name, None)
        new_var(parser, name, e_class)
        be_code_class(parser.finfo, e_class, c)
        class_inherit(parser, e_class)
        class_block(parser, c, e_class)
        be_class_compress(parser.vm, c)
        match_token(parser, KeyEnd)
        # Assign the class to the static member
        e1 = _clone_expdesc(e_out)
        key = bexpdesc()
        init_exp(key, ETSTRING, 0)
        key.v.s = name
        be_code_member(parser.finfo, e1, key)
        be_code_setvar(parser.finfo, e1, e_class, False)
    else:
        parser_error(parser, "class name error")


# ============================================================================
# Import statement (from be_parser.c)
# ============================================================================

# static void import_stmt(bparser *parser)
def import_stmt(parser):
    """Parse an import statement: 'import' (ID ['as' ID] | STRING 'as' ID)."""
    m = bexpdesc()
    v = bexpdesc()
    scan_next_token(parser)  # skip 'import'
    init_exp(m, ETSTRING, 0)
    m.v.s = next_token(parser).u.s
    name = m.v.s

    if next_type(parser) == TokenString:
        # STRING 'as' ID
        scan_next_token(parser)  # skip the module path
        match_token(parser, KeyAs)  # match and skip 'as'
        name = next_token(parser).u.s
        match_token(parser, TokenId)  # match and skip ID
    else:
        # ID ['as' ID] | ID {',' ID}
        match_token(parser, TokenId)  # match and skip ID
        if match_skip(parser, KeyAs):
            name = next_token(parser).u.s
            match_token(parser, TokenId)
        else:
            # {',' ID}
            while match_skip(parser, OptComma):
                new_var(parser, name, v)
                be_code_import(parser.finfo, m, v)
                init_exp(m, ETSTRING, 0)
                m.v.s = next_token(parser).u.s
                name = m.v.s
                match_token(parser, TokenId)

    new_var(parser, name, v)
    be_code_import(parser.finfo, m, v)


# ============================================================================
# Var statement (from be_parser.c)
# ============================================================================

# static void var_field(bparser *parser)
def var_field(parser):
    """Parse a single variable declaration: ID ['=' expr]."""
    e1 = bexpdesc()
    e2 = bexpdesc()
    name = next_token(parser).u.s
    match_token(parser, TokenId)
    if match_skip(parser, OptAssign):
        expr(parser, e2)
        check_var(parser, e2)
    else:
        init_exp(e2, ETNIL, 0)
    new_var(parser, name, e1)
    be_code_setvar(parser.finfo, e1, e2, False)


# static void var_stmt(bparser *parser)
def var_stmt(parser):
    """Parse a var statement: 'var' ID ['=' expr] {',' ID ['=' expr]}."""
    scan_next_token(parser)  # skip 'var'
    var_field(parser)
    while match_skip(parser, OptComma):
        var_field(parser)


# ============================================================================
# Exception handling (from be_parser.c)
# ============================================================================

# static int except_case_list(bparser *parser, int *base)
def except_case_list(parser, base_ref):
    """Parse exception case list: expr {',' expr} | '..'."""
    finfo = parser.finfo
    if match_skip(parser, OptConnect):  # '..'
        base_ref[0] = finfo.freereg
        return 0
    e = bexpdesc()
    expr(parser, e)
    check_var(parser, e)
    base_ref[0] = be_code_nextreg(finfo, e)
    idx = base_ref[0]
    while match_skip(parser, OptComma):
        expr(parser, e)
        check_var(parser, e)
        idx = be_code_nextreg(finfo, e)
    count = idx - base_ref[0] + 1
    be_code_freeregs(finfo, count)
    return count


# static int except_var_list(bparser *parser, int base)
def except_var_list(parser, base):
    """Parse exception variable list: ['as' ID [',' ID]]."""
    v = bexpdesc()
    if match_skip(parser, KeyAs):
        name = next_token(parser).u.s
        match_token(parser, TokenId)
        new_var(parser, name, v)
        # be_assert(v.type == ETLOCAL && v.v.idx == base)
        if match_skip(parser, OptComma):
            name = next_token(parser).u.s
            match_token(parser, TokenId)
            new_var(parser, name, v)
            # be_assert(v.type == ETLOCAL && v.v.idx == base + 1)
            return 2
        return 1
    return 0


# static void except_block(bparser *parser, int *jmp, int *jbrk)
def except_block(parser, jmp_ref, jbrk_ref):
    """Parse an except block."""
    base_ref = [0]
    binfo = bblockinfo()
    finfo = parser.finfo
    match_token(parser, KeyExcept)  # skip 'except'
    begin_block(finfo, binfo, 0)
    be_code_conjump(finfo, jmp_ref, finfo.pc)
    ecnt = except_case_list(parser, base_ref)
    vcnt = except_var_list(parser, base_ref[0])
    be_code_catch(finfo, base_ref[0], ecnt, vcnt, jmp_ref)
    stmtlist(parser)
    be_code_conjump(finfo, jbrk_ref, be_code_jump(finfo))
    end_block(parser)


# static void try_stmt(bparser *parser)
def try_stmt(parser):
    """Parse a try/except statement."""
    scan_next_token(parser)  # skip 'try'
    jcatch = be_code_exblk(parser.finfo, 0)
    block(parser, BLOCK_EXCEPT)
    be_code_exblk(parser.finfo, 1)
    jbrk = be_code_jump(parser.finfo)
    jcatch_ref = [jcatch]
    jbrk_ref = [jbrk]
    except_block(parser, jcatch_ref, jbrk_ref)
    while next_type(parser) == KeyExcept:
        except_block(parser, jcatch_ref, jbrk_ref)
    be_code_patchjump(parser.finfo, jcatch_ref[0])
    be_code_raise(parser.finfo, None, None)
    be_code_patchjump(parser.finfo, jbrk_ref[0])
    match_token(parser, KeyEnd)


# static void throw_stmt(bparser *parser)
def throw_stmt(parser):
    """Parse a raise statement: 'raise' expr [',' expr]."""
    e1 = bexpdesc()
    e2 = bexpdesc()
    scan_next_token(parser)  # skip 'raise'
    expr(parser, e1)
    check_var(parser, e1)
    if match_skip(parser, OptComma):
        expr(parser, e2)
        check_var(parser, e2)
        be_code_raise(parser.finfo, e1, e2)
    else:
        be_code_raise(parser.finfo, e1, None)


# ============================================================================
# Statement dispatcher (from be_parser.c)
# ============================================================================

# static void statement(bparser *parser)
def statement(parser):
    """Parse a single statement."""
    sideeffect = parser.finfo.binfo.sideeffect
    parser.finfo.binfo.sideeffect = 1  # default: has side effect
    tt = next_type(parser)
    if tt == KeyIf:
        if_stmt(parser)
    elif tt == KeyWhile:
        while_stmt(parser)
    elif tt == KeyFor:
        for_stmt(parser)
    elif tt == KeyDo:
        do_stmt(parser)
    elif tt == KeyBreak:
        break_stmt(parser)
    elif tt == KeyContinue:
        continue_stmt(parser)
    elif tt == KeyDef:
        def_stmt(parser)
    elif tt == KeyClass:
        class_stmt(parser)
    elif tt == KeyReturn:
        return_stmt(parser)
    elif tt == KeyImport:
        import_stmt(parser)
    elif tt == KeyVar:
        var_stmt(parser)
    elif tt == KeyTry:
        try_stmt(parser)
    elif tt == KeyRaise:
        throw_stmt(parser)
    elif tt == OptSemic:
        parser.finfo.binfo.sideeffect = sideeffect
        scan_next_token(parser)
    else:
        parser.finfo.binfo.sideeffect = sideeffect
        expr_stmt(parser)
        if comp_is_strict(parser.vm) and parser.finfo.binfo.sideeffect == 0:
            push_error(parser, "strict: expression without side effect detected")
    # be_assert(parser->finfo->freereg >= be_list_count(parser->finfo->local))


# static void stmtlist(bparser *parser)
def stmtlist(parser):
    """Parse a list of statements until a block-ending token."""
    while block_follow(parser):
        statement(parser)


# static void block(bparser *parser, int type)
def block(parser, type_):
    """Parse a block: begin_block, stmtlist, end_block."""
    binfo = bblockinfo()
    begin_block(parser.finfo, binfo, type_)
    stmtlist(parser)
    end_block(parser)


# ============================================================================
# Main function and entry point (from be_parser.c)
# ============================================================================

# static void mainfunc(bparser *parser, bclosure *cl)
def mainfunc(parser, cl):
    """Parse the top-level function (main or loader)."""
    binfo = bblockinfo()
    finfo = bfuncinfo()
    begin_func(parser, finfo, binfo)
    finfo.proto.argc = 0
    finfo.proto.name = be_newstr(parser.vm, funcname(parser))
    cl.proto = finfo.proto
    # be_remove(parser->vm, -3) — remove proto from stack
    from berry_port.be_api import be_remove as _be_remove
    _be_remove(parser.vm, -3)
    stmtlist(parser)
    end_func(parser)
    match_token(parser, TokenEOS)


# bclosure* be_parser_source(bvm *vm,
#     const char *fname, breader reader, void *data, bbool islocal)
# {
#     bparser parser;
#     bclosure *cl = be_newclosure(vm, 0);
#     parser.vm = vm;
#     parser.finfo = NULL;
#     parser.cl = cl;
#     parser.islocal = (bbyte)islocal;
#     var_setclosure(vm->top, cl);
#     be_stackpush(vm);
#     be_lexer_init(&parser.lexer, vm, fname, reader, data);
#     scan_next_token(&parser);
#     mainfunc(&parser, cl);
#     be_lexer_deinit(&parser.lexer);
#     be_global_release_space(vm);
#     be_stackpop(vm, 2);
#     scan_next_token(&parser);
#     return cl;
# }
def be_parser_source(vm, fname, reader, data, islocal):
    """Parse Berry source code and return a bclosure.

    This is the main entry point for the parser. It initializes the lexer,
    parses the entire source, and returns a closure wrapping the compiled
    prototype.

    Args:
        vm: The Berry VM instance.
        fname: Source file name (for error messages).
        reader: Reader callback function(data, size_ref) -> str or None.
        data: Opaque data passed to the reader.
        islocal: True if this is a local (non-global) compilation.

    Returns:
        A bclosure containing the compiled bproto.
    """
    parser = bparser()
    cl = be_newclosure(vm, 0)
    parser.vm = vm
    parser.finfo = None
    parser.cl = cl
    parser.islocal = 1 if islocal else 0

    # Push closure on stack for GC protection
    if vm.top_idx >= len(vm.stack):
        vm.stack.append(bvalue())
    var_setclosure(vm.stack[vm.top_idx], cl)
    be_stackpush(vm)

    be_lexer_init(parser.lexer, vm, fname, reader, data)
    scan_next_token(parser)  # scan first token
    mainfunc(parser, cl)
    be_lexer_deinit(parser.lexer)
    be_global_release_space(vm)
    be_stackpop(vm, 2)  # pop strtab
    scan_next_token(parser)  # clear lexer
    return cl
