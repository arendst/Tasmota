"""
Berry lexical analyzer.
Mirrors: src/be_lexer.c / src/be_lexer.h

The lexer tokenizes Berry source code into a stream of tokens. It handles
identifiers, keywords, numbers (decimal and hex), strings (with escape
sequences), operators, comments (line and block), f-strings, and
preprocessor directives (#if/#elif/#else/#endif/#define/#undef).

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
    bvalue, bstring, bmap,
    var_setstr, var_setmap, var_setnil, var_setint,
    var_isint, var_isstr, var_toint, var_tostr,
    gc_object,
    str_extra,
)
from berry_port.be_string import (
    be_newstr, be_newstrn, be_str2cstr,
    be_strhash, be_str_setextra,
)
from berry_port.be_mem import be_realloc, be_malloc, be_free
from berry_port.be_map import (
    be_map_new, be_map_findstr, be_map_insertstr,
    be_map_removestr, be_map_val2node,
)
from berry_port.berry_conf import (
    BE_USE_SCRIPT_COMPILER,
    BE_USE_PREPROCESSOR,
    BE_PREPROC_MAX_DEPTH,
    BE_USE_SINGLE_FLOAT,
    BE_INTGER_TYPE,
)
import struct


# ============================================================================
# Constants
# ============================================================================

SHORT_STR_LEN = 32
EOS = '\0'  # end of source


# ============================================================================
# Token type enum (from be_lexer.h btokentype)
# ============================================================================

# typedef enum {
#     TokenNone = 0,
#     TokenEOS,       /* end of source */
#     TokenId,        /* identifier */
#     TokenInteger,
#     TokenReal,
#     TokenString,
#     /* operator, don't change order */
#     /* assign operator */
#     OptAssign,      /* operator, = */
#     ...
#     KeyStatic,      /* keyword static */
# } btokentype;

TokenNone       = 0
TokenEOS        = 1
TokenId         = 2
TokenInteger    = 3
TokenReal       = 4
TokenString     = 5
# assign operators
OptAssign       = 6
OptAddAssign    = 7
OptSubAssign    = 8
OptMulAssign    = 9
OptDivAssign    = 10
OptModAssign    = 11
OptAndAssign    = 12
OptOrAssign     = 13
OptXorAssign    = 14
OptLsfAssign    = 15
OptRsfAssign    = 16
# binary operators
OptAdd          = 17
OptSub          = 18
OptMul          = 19
OptDiv          = 20
OptMod          = 21
OptLT           = 22
OptLE           = 23
OptEQ           = 24
OptNE           = 25
OptGT           = 26
OptGE           = 27
OptBitAnd       = 28
OptBitOr        = 29
OptBitXor       = 30
OptShiftL       = 31
OptShiftR       = 32
OptConnect      = 33
OptAnd          = 34
OptOr           = 35
# unary operators
OptNot          = 36
OptFlip         = 37
# postfix / brackets
OptSpaceLBK     = 38
OptCallLBK      = 39
OptRBK          = 40
OptLSB          = 41
OptRSB          = 42
OptLBR          = 43
OptRBR          = 44
OptDot          = 45
# other symbols
OptComma        = 46
OptSemic        = 47
OptColon        = 48
OptQuestion     = 49
OptArrow        = 50
OptWalrus       = 51
# keywords
KeyIf           = 52
KeyElif         = 53
KeyElse         = 54
KeyWhile        = 55
KeyFor          = 56
KeyDef          = 57
KeyEnd          = 58
KeyClass        = 59
KeyBreak        = 60
KeyContinue     = 61
KeyReturn       = 62
KeyTrue         = 63
KeyFalse        = 64
KeyNil          = 65
KeyVar          = 66
KeyDo           = 67
KeyImport       = 68
KeyAs           = 69
KeyTry          = 70
KeyExcept       = 71
KeyRaise        = 72
KeyStatic       = 73


# /* IMPORTANT: This must follow the enum found in be_lexer.h !!! */
# static const char* const token_strings[] = { ... };
token_strings = [
    "NONE", "EOS", "ID", "INT", "REAL", "STR",
    "=", "+=", "-=", "*=", "/=", "%=", "&=", "|=",
    "^=", "<<=", ">>=", "+", "-", "*", "/", "%",
    "<", "<=", "==", "!=", ">", ">=", "&", "|",
    "^", "<<", ">>", "..", "&&", "||", "!", "~",
    "(", "(", ")", "[", "]", "{", "}", ".", ",", ";",
    ":", "?", "->", ":=", "if", "elif", "else", "while",
    "for", "def", "end", "class", "break", "continue",
    "return", "true", "false", "nil", "var", "do",
    "import", "as", "try", "except", "raise", "static",
]


def _type_count():
    return len(token_strings)


# ============================================================================
# Structures (from be_lexer.h)
# ============================================================================

class blexerreader:
    """Mirrors struct blexerreader."""
    # struct blexerreader {
    #     const char *s;
    #     size_t len;
    #     void *data;
    #     breader readf;
    #     int cursor;
    # };
    __slots__ = ('s', 'len', 'data', 'readf', 'cursor')
    def __init__(self):
        self.s = None       # current buffer (string)
        self.len = 0        # remaining length in current buffer
        self.data = None    # user data for reader callback
        self.readf = None   # reader callback: (lexer, data, size_ref) -> str or None
        self.cursor = 0     # current character (int ordinal or 0 for EOS)


class blexerbuf:
    """Mirrors struct blexerbuf."""
    # struct blexerbuf {
    #     char *s;
    #     size_t len, size;
    # };
    __slots__ = ('s', 'len', 'size')
    def __init__(self):
        self.s = None       # buffer (bytearray in Python)
        self.len = 0        # current content length
        self.size = 0       # allocated size


class btoken:
    """Mirrors struct btoken."""
    # typedef struct btoken {
    #     btokentype type;
    #     union { bstring *s; bint i; breal r; } u;
    # } btoken;
    __slots__ = ('type', 's', 'i', 'r')
    def __init__(self):
        self.type = TokenNone
        self.s = None       # string value (bstring)
        self.i = 0          # integer value
        self.r = 0.0        # real value

    @property
    def u(self):
        """C union access: token.u.s / token.u.i / token.u.r → self."""
        return self


class bppstate:
    """Mirrors bppstate struct for preprocessor conditional stack."""
    # typedef struct {
    #     uint8_t active : 1;
    #     uint8_t matched : 1;
    #     int line;
    # } bppstate;
    __slots__ = ('active', 'matched', 'line')
    def __init__(self):
        self.active = 0     # 1 = emitting tokens, 0 = skipping
        self.matched = 0    # 1 = some branch already matched
        self.line = 0       # line number where #if started


class blexer:
    """Mirrors struct blexer."""
    # typedef struct blexer {
    #     const char *fname;
    #     btoken token;
    #     int linenumber;
    #     int lastline;
    #     btokentype cacheType;
    #     struct blexerbuf buf;
    #     struct blexerreader reader;
    #     bmap *strtab;
    #     bvm *vm;
    #     int had_whitespace;
    #     /* preprocessor fields */
    #     bppstate ppstack[BE_PREPROC_MAX_DEPTH];
    #     int ppdepth;
    #     bbool pp_at_line_start;
    #     bbool pp_translatable_ready;
    #     bbool pp_hash_consumed;
    # } blexer;
    __slots__ = (
        'fname', 'token', 'linenumber', 'lastline', 'cacheType',
        'buf', 'reader', 'strtab', 'vm', 'had_whitespace',
        'ppstack', 'ppdepth', 'pp_at_line_start',
        'pp_translatable_ready', 'pp_hash_consumed',
    )
    def __init__(self):
        self.fname = None
        self.token = btoken()
        self.linenumber = 1
        self.lastline = 1
        self.cacheType = TokenNone
        self.buf = blexerbuf()
        self.reader = blexerreader()
        self.strtab = None      # bmap for string caching
        self.vm = None
        self.had_whitespace = 0
        # preprocessor state
        self.ppstack = [bppstate() for _ in range(BE_PREPROC_MAX_DEPTH)]
        self.ppdepth = -1
        self.pp_at_line_start = True
        self.pp_translatable_ready = False
        self.pp_hash_consumed = False


# ============================================================================
# Inline helper macros (from be_lexer.c)
# ============================================================================

# #define lexbuf(lex)         ((lex)->buf.s)
def _lexbuf(lex):
    return lex.buf.s

# #define lgetc(lex)          ((lex)->reader.cursor)
def _lgetc(lex):
    return lex.reader.cursor

# #define setstr(lex, v)      ((lex)->token.u.s = (v))
def _setstr(lex, v):
    lex.token.s = v

# #define setint(lex, v)      ((lex)->token.u.i = (v))
def _setint(lex, v):
    lex.token.i = v

# #define setreal(lex, v)     ((lex)->token.u.r = (v))
def _setreal(lex, v):
    lex.token.r = v


# ============================================================================
# Stub functions for dependencies not yet ported
# ============================================================================

def _ensure_stack_slot(vm):
    """Ensure vm.stack[vm.top_idx] exists so callers can write to it."""
    from berry_port.be_object import bvalue
    while len(vm.stack) <= vm.top_idx:
        vm.stack.append(bvalue())

def _be_stackpush(vm):
    """Push one slot onto the VM value stack, growing it if needed."""
    if not hasattr(vm, 'stack') or vm.stack is None:
        return
    _ensure_stack_slot(vm)
    vm.top_idx += 1
    _ensure_stack_slot(vm)

def _be_stackpop(vm, n):
    """Stub for be_stackpop (be_exec.py not yet ported)."""
    vm.top_idx -= n

def _be_gc_fix(vm, obj):
    """Stub for be_gc_fix."""
    if hasattr(obj, 'marked'):
        from berry_port.be_object import GC_FIXED
        obj.marked |= GC_FIXED

def _be_gc_unfix(vm, obj):
    """Stub for be_gc_unfix."""
    if hasattr(obj, 'marked'):
        from berry_port.be_object import GC_FIXED
        obj.marked &= ~GC_FIXED

def _be_raise(vm, etype, msg):
    """Call the real be_raise from be_api to properly push exception
    values onto the Berry stack and throw via BerryException."""
    from berry_port.be_api import be_raise
    be_raise(vm, etype, msg)

def _be_pushfstring(vm, fmt, *args):
    """Call the real be_pushfstring from be_api."""
    from berry_port.be_api import be_pushfstring
    return be_pushfstring(vm, fmt, *args)


# ============================================================================
# Number parsing helpers (from be_strlib.c, needed by lexer)
# ============================================================================

# int be_char2hex(int c)
# {
#     if (c >= '0' && c <= '9') return c - '0';
#     else if (c >= 'a' && c <= 'f') return c - 'a' + 0x0A;
#     else if (c >= 'A' && c <= 'F') return c - 'A' + 0x0A;
#     return -1;
# }
def _wrap_bint(v):
    """Wrap a Python arbitrary-precision int to the signed range of bint.

    In C, ``bint`` is a fixed-width signed type whose size depends on
    ``BE_INTGER_TYPE`` (0 → int/32-bit, 1 → long/32-bit, 2 → long long/64-bit).
    Python integers have unlimited precision, so we must manually truncate
    and sign-extend to match C overflow semantics.
    """
    if BE_INTGER_TYPE <= 1:          # 32-bit signed
        v = v & 0xFFFFFFFF
        if v >= 0x80000000:
            v -= 0x100000000
    else:                            # 64-bit signed
        v = v & 0xFFFFFFFFFFFFFFFF
        if v >= 0x8000000000000000:
            v -= 0x10000000000000000
    return v


def be_char2hex(c):
    """Convert a hex character to its integer value, or -1 if invalid."""
    if isinstance(c, str):
        c = ord(c) if c else -1
    if ord('0') <= c <= ord('9'):
        return c - ord('0')
    elif ord('a') <= c <= ord('f'):
        return c - ord('a') + 0x0A
    elif ord('A') <= c <= ord('F'):
        return c - ord('A') + 0x0A
    return -1


def be_str2int(s, endstr=None):
    """Parse a string to a Berry integer (bint).

    Supports decimal and hexadecimal (0x/0X prefix).
    Mirrors the C be_str2int from be_strlib.c.
    """
    # BERRY_API bint be_str2int(const char *str, const char **endstr)
    if isinstance(s, (bytearray, bytes)):
        s = s.decode('latin-1')
    s = s.lstrip()
    if not s:
        return 0

    idx = 0
    # Check for hex
    if len(s) >= 2 and s[0] == '0' and s[1] in ('x', 'X'):
        idx = 2
        result = 0
        while idx < len(s):
            dig = be_char2hex(ord(s[idx]))
            if dig < 0:
                break
            result = result * 16 + dig
            idx += 1
        return _wrap_bint(result)

    # Decimal
    sign = 1
    if idx < len(s) and s[idx] in ('+', '-'):
        if s[idx] == '-':
            sign = -1
        idx += 1

    result = 0
    while idx < len(s) and s[idx].isdigit():
        result = result * 10 + (ord(s[idx]) - ord('0'))
        idx += 1

    return _wrap_bint(sign * result)


def _f32(x):
    """Round a Python float through float32, matching C single-precision."""
    return struct.unpack('<f', struct.pack('<f', x))[0]


def be_str2real(s, endstr=None):
    """Parse a string to a Berry real (breal).

    Mirrors the C be_str2real from be_strlib.c.
    When BE_USE_SINGLE_FLOAT is enabled, all intermediate arithmetic is
    rounded through float32 at each step to match C's single-precision
    accumulation behaviour.
    """
    if isinstance(s, (bytearray, bytes)):
        s = s.decode('latin-1')
    s = s.lstrip()
    if not s:
        return 0.0

    f = _f32 if BE_USE_SINGLE_FLOAT else lambda x: x

    pos = 0
    sign_char = s[pos] if pos < len(s) else '\0'
    c = sign_char
    if pos < len(s):
        pos += 1
    if c in ('+', '-'):
        c = s[pos] if pos < len(s) else '\0'
        if pos < len(s):
            pos += 1

    sum_val = f(0.0)

    # Integer part
    while c.isdigit():
        sum_val = f(f(sum_val * 10) + (ord(c) - ord('0')))
        c = s[pos] if pos < len(s) else '\0'
        if pos < len(s):
            pos += 1

    # Fractional part
    if c == '.':
        c = s[pos] if pos < len(s) else '\0'
        if pos < len(s):
            pos += 1
        point = f(0.1)
        deci = f(0.0)
        while c.isdigit():
            digit = f((ord(c) - ord('0')) * point)
            new_deci = f(deci + digit)
            if new_deci < deci:
                break  # precision limit
            deci = new_deci
            point = f(point * f(0.1))
            c = s[pos] if pos < len(s) else '\0'
            if pos < len(s):
                pos += 1
        sum_val = f(sum_val + deci)

    # Scientific notation
    if c in ('e', 'E'):
        c = s[pos] if pos < len(s) else '\0'
        if pos < len(s):
            pos += 1
        esign = 1
        if c == '-':
            esign = -1
            c = s[pos] if pos < len(s) else '\0'
            if pos < len(s):
                pos += 1
        elif c == '+':
            c = s[pos] if pos < len(s) else '\0'
            if pos < len(s):
                pos += 1
        e = 0
        while c.isdigit():
            e = e * 10 + (ord(c) - ord('0'))
            c = s[pos] if pos < len(s) else '\0'
            if pos < len(s):
                pos += 1
        e *= esign
        ratio = f(0.1) if e < 0 else f(10.0)
        if e < 0:
            e = -e
        while e > 0:
            e -= 1
            sum_val = f(sum_val * ratio)

    return -sum_val if sign_char == '-' else sum_val


# ============================================================================
# Core lexer functions (from be_lexer.c)
# ============================================================================

# void be_lexerror(blexer *lexer, const char *msg)
# {
#     bvm *vm = lexer->vm;
#     const char *error = be_pushfstring(vm,
#         "%s:%d: %s", lexer->fname, lexer->linenumber, msg);
#     be_lexer_deinit(lexer);
#     be_raise(vm, "syntax_error", error);
# }
def be_lexerror(lexer, msg):
    """Report a lexer error with file and line info, then raise."""
    vm = lexer.vm
    # C code: const char *error = be_pushfstring(vm, "%s:%d: %s", ...);
    error = _be_pushfstring(vm, "%s:%d: %s", lexer.fname, lexer.linenumber, msg)
    be_lexer_deinit(lexer)
    _be_raise(vm, "syntax_error", error)


# static void keyword_registe(bvm *vm)
# {
#     int i;
#     for (i = KeyIf; i < type_count(); ++i) {
#         bstring *s = be_newstr(vm, token_strings[i]);
#         be_gc_fix(vm, gc_object(s));
#         be_str_setextra(s, i);
#     }
# }
def _keyword_register(vm):
    """Register all keyword strings in the string table with their token type."""
    for i in range(KeyIf, _type_count()):
        s = be_newstr(vm, token_strings[i])
        _be_gc_fix(vm, gc_object(s))
        be_str_setextra(s, i)


# static void keyword_unregiste(bvm *vm)
# {
#     int i;
#     for (i = KeyIf; i < type_count(); ++i) {
#         bstring *s = be_newstr(vm, token_strings[i]);
#         be_gc_unfix(vm, gc_object(s));
#     }
# }
def _keyword_unregister(vm):
    """Unregister keyword strings (unfix from GC)."""
    for i in range(KeyIf, _type_count()):
        s = be_newstr(vm, token_strings[i])
        _be_gc_unfix(vm, gc_object(s))


# static bstring* cache_string(blexer *lexer, bstring *s)
# {
#     bvalue *res;
#     bvm *vm = lexer->vm;
#     var_setstr(vm->top, s);
#     be_stackpush(vm);
#     res = be_map_findstr(lexer->vm, lexer->strtab, s);
#     if (res) {
#         s = var_tostr(&be_map_val2node(res)->key);
#     } else {
#         res = be_map_insertstr(vm, lexer->strtab, s, NULL);
#         var_setnil(res);
#     }
#     be_stackpop(vm, 1);
#     return s;
# }
def _cache_string(lexer, s):
    """Cache a string in the lexer's string table for deduplication.

    In C, cache_string uses be_map_val2node to recover the node from
    the found value and extracts the key string. In the Python port,
    be_map_findstr already returns the value slot of an existing entry,
    and the key in that entry is the same interned string we searched
    for. Since short strings are interned (identity-equal), the original
    string `s` is already the canonical copy. We just ensure it's in
    the map for GC reachability.
    """
    vm = lexer.vm
    _ensure_stack_slot(vm)
    var_setstr(vm.stack[vm.top_idx], s)
    _be_stackpush(vm)
    res = be_map_findstr(vm, lexer.strtab, s)
    if res is None:
        res = be_map_insertstr(vm, lexer.strtab, s, None)
        if res is not None:
            var_setnil(res)
    # s is already the interned string — no need to extract from node
    _be_stackpop(vm, 1)
    return s


# static bstring* lexer_newstrn(blexer *lexer, const char *str, size_t len)
# {
#     return cache_string(lexer, be_newstrn(lexer->vm, str, len));
# }
def _lexer_newstrn(lexer, s_data, length):
    """Create or find an interned string of given length via the lexer cache."""
    return _cache_string(lexer, be_newstrn(lexer.vm, s_data, length))


# bstring* be_lexer_newstr(blexer *lexer, const char *str)
# {
#     return cache_string(lexer, be_newstr(lexer->vm, str));
# }
def be_lexer_newstr(lexer, s_data):
    """Create or find an interned string via the lexer cache (public API)."""
    return _cache_string(lexer, be_newstr(lexer.vm, s_data))


# ============================================================================
# Character reading (from be_lexer.c)
# ============================================================================

# static int next(blexer *lexer)
# {
#     struct blexerreader *lr = &lexer->reader;
#     if (!(lr->len--)) {
#         static const char eos = EOS;
#         const char *s = lr->readf(lexer, lr->data, &lr->len);
#         lr->s = s ? s : &eos;
#         --lr->len;
#     }
#     lexer->reader.cursor = *lr->s++;
#     return lexer->reader.cursor;
# }
def _next(lexer):
    """Read the next character from the source stream.

    When the current buffer is exhausted, calls the reader callback
    to get the next chunk. Returns the character as an integer ordinal.
    """
    lr = lexer.reader
    if lr.len <= 0:
        # Buffer exhausted — request more data
        size_ref = [0]
        s = lr.readf(lexer, lr.data, size_ref)
        if s is not None:
            lr.s = s
            lr.len = size_ref[0]
        else:
            lr.s = EOS
            lr.len = 1
        lr.len -= 1  # account for the character we're about to read
    else:
        lr.len -= 1

    if lr.s is None or len(lr.s) == 0:
        lr.cursor = 0  # EOS
    else:
        ch = lr.s[0]
        lr.s = lr.s[1:]
        lr.cursor = ord(ch) if isinstance(ch, str) else ch
    return lr.cursor


# static void clear_buf(blexer *lexer)
# {
#     lexer->buf.len = 0;
# }
def _clear_buf(lexer):
    """Reset the lexer buffer length to zero."""
    lexer.buf.len = 0


# static void save_char(blexer *lexer, int ch)
# {
#     struct blexerbuf *buf = &lexer->buf;
#     if (buf->len >= buf->size) {
#         size_t size = buf->size << 1;
#         buf->s = be_realloc(lexer->vm, buf->s, buf->size, size);
#         buf->size = size;
#     }
#     buf->s[buf->len++] = (char)ch;
# }
def _save_char(lexer, ch):
    """Append a character to the lexer buffer, growing if needed."""
    buf = lexer.buf
    if buf.len >= buf.size:
        new_size = buf.size << 1
        new_s = bytearray(new_size)
        new_s[:buf.size] = buf.s[:buf.size]
        # Track memory delta
        lexer.vm.gc.usage += new_size - buf.size
        buf.s = new_s
        buf.size = new_size
    buf.s[buf.len] = ch & 0xFF
    buf.len += 1


# static int save(blexer *lexer)
# {
#     int ch = lgetc(lexer);
#     save_char(lexer, ch);
#     return next(lexer);
# }
def _save(lexer):
    """Save the current character to the buffer and advance to the next.

    Characters with ordinals 0x00-0xFF are saved directly as single bytes.
    These represent raw byte values in the Latin-1 byte mapping used
    internally by the Python port.

    Characters with ordinals > 0xFF are real Unicode codepoints (from
    Python source input that hasn't been normalized yet) and are encoded
    as UTF-8 byte sequences to match C Berry's behavior.
    """
    ch = _lgetc(lexer)
    if ch > 0xFF:
        # Real Unicode codepoint — encode as UTF-8 bytes
        for b in chr(ch).encode('utf-8'):
            _save_char(lexer, b)
    else:
        _save_char(lexer, ch)
    return _next(lexer)


# static bstring* buf_tostr(blexer *lexer)
# {
#     struct blexerbuf *buf = &lexer->buf;
#     return lexer_newstrn(lexer, buf->s, buf->len);
# }
def _buf_tostr(lexer):
    """Convert the current buffer contents to an interned Berry string."""
    buf = lexer.buf
    s_data = buf.s[:buf.len].decode('latin-1')
    return _lexer_newstrn(lexer, s_data, buf.len)


# ============================================================================
# Character classification (from be_lexer.c)
# ============================================================================

# static int is_newline(int c)
def _is_newline(c):
    return c == ord('\n') or c == ord('\r')

# static int is_digit(int c)
def _is_digit(c):
    return ord('0') <= c <= ord('9')

# static int is_octal(int c)
def _is_octal(c):
    return ord('0') <= c <= ord('7')

# static int is_ident_start(int c)
def _is_ident_start(c):
    return (ord('a') <= c <= ord('z')) or (ord('A') <= c <= ord('Z')) or (c == ord('_'))

# static int is_word(int c)
def _is_word(c):
    return _is_ident_start(c) or _is_digit(c)


# static int check_next(blexer *lexer, int c)
# {
#     if (lgetc(lexer) == c) {
#         next(lexer);
#         return 1;
#     }
#     return 0;
# }
def _check_next(lexer, c):
    """If the current character matches c, consume it and return 1."""
    if _lgetc(lexer) == ord(c) if isinstance(c, str) else c:
        _next(lexer)
        return 1
    return 0


# static int check2hex(blexer *lexer, int c)
def _check2hex(lexer, c):
    """Convert c to hex digit value, raising lexer error if invalid."""
    result = be_char2hex(c)
    if result < 0:
        be_lexerror(lexer, "invalid hexadecimal number")
    return result


# static int read_hex(blexer *lexer, const char *src)
def _read_hex(lexer, src, offset):
    """Read a 2-digit hex escape from buffer at offset."""
    c = _check2hex(lexer, src[offset])
    return (c << 4) + _check2hex(lexer, src[offset + 1])


# static int read_oct(blexer *lexer, const char *src)
def _read_oct(lexer, src, offset):
    """Read a 3-digit octal escape from buffer at offset."""
    c = 0
    end = offset + 3
    i = offset
    while i < end and i < len(src) and _is_octal(src[i]):
        c = 8 * c + src[i] - ord('0')
        i += 1
    if i < end:
        be_lexerror(lexer, "invalid octal number")
    return c


# ============================================================================
# Unicode loading (from be_lexer.c)
# ============================================================================

# char* be_load_unicode(char *dst, const char *src)
# {
#     int ucode = 0, i = 4;
#     while (i--) {
#         int ch = *src++;
#         if (ch >= '0' && ch <= '9') ucode = (ucode << 4) | (ch - '0');
#         else if (ch >= 'A' && ch <= 'F') ucode = (ucode << 4) | (ch - 'A' + 0x0A);
#         else if (ch >= 'a' && ch <= 'f') ucode = (ucode << 4) | (ch - 'a' + 0x0A);
#         else return NULL;
#     }
#     /* convert unicode to utf8 */
#     if (ucode < 0x007F):
#         *dst++ = (char)(ucode & 0x7F);
#     elif (ucode < 0x7FF):
#         *dst++ = (char)(((ucode >> 6) & 0x1F) | 0xC0);
#         *dst++ = (char)((ucode & 0x3F) | 0x80);
#     else:
#         *dst++ = (char)(((ucode >> 12) & 0x0F) | 0xE0);
#         *dst++ = (char)(((ucode >> 6) & 0x03F) | 0x80);
#         *dst++ = (char)((ucode & 0x3F) | 0x80);
#     return dst;
# }
def be_load_unicode(src, offset):
    """Parse a 4-digit unicode escape and return UTF-8 bytes.

    Args:
        src: bytearray or bytes containing the hex digits.
        offset: starting index of the 4 hex digits.

    Returns:
        A list of byte values (UTF-8 encoded), or None on error.
    """
    ucode = 0
    for i in range(4):
        if offset + i >= len(src):
            return None
        ch = src[offset + i]
        if ord('0') <= ch <= ord('9'):
            ucode = (ucode << 4) | (ch - ord('0'))
        elif ord('A') <= ch <= ord('F'):
            ucode = (ucode << 4) | (ch - ord('A') + 0x0A)
        elif ord('a') <= ch <= ord('f'):
            ucode = (ucode << 4) | (ch - ord('a') + 0x0A)
        else:
            return None

    # Convert unicode codepoint to UTF-8
    result = []
    if ucode < 0x0080:
        result.append(ucode & 0x7F)
    elif ucode < 0x0800:
        result.append(((ucode >> 6) & 0x1F) | 0xC0)
        result.append((ucode & 0x3F) | 0x80)
    else:
        result.append(((ucode >> 12) & 0x0F) | 0xE0)
        result.append(((ucode >> 6) & 0x3F) | 0x80)
        result.append((ucode & 0x3F) | 0x80)
    return result


# ============================================================================
# String escape processing (from be_lexer.c)
# ============================================================================

# static void tr_string(blexer *lexer)
def _tr_string(lexer):
    """Process escape sequences in the lexer buffer in-place.

    Handles: \\a \\b \\f \\n \\r \\t \\v \\\\ \\' \\" \\? \\xHH \\OOO \\uXXXX
    Raises lexer error on unfinished strings or invalid escapes.
    """
    src = lexer.buf.s
    src_len = lexer.buf.len
    dst = bytearray()
    i = 0
    while i < src_len:
        c = src[i]
        i += 1
        if c == ord('\n') or c == ord('\r'):
            be_lexerror(lexer, "unfinished string")
        elif c == ord('\\'):
            if i >= src_len:
                be_lexerror(lexer, "unfinished string")
            nc = src[i]
            if nc != ord('u'):
                escape_map = {
                    ord('a'): ord('\a'), ord('b'): ord('\b'),
                    ord('f'): ord('\f'), ord('n'): ord('\n'),
                    ord('r'): ord('\r'), ord('t'): ord('\t'),
                    ord('v'): ord('\v'), ord('\\'): ord('\\'),
                    ord("'"): ord("'"), ord('"'): ord('"'),
                    ord('?'): ord('?'),
                }
                if nc in escape_map:
                    dst.append(escape_map[nc])
                    i += 1
                elif nc == ord('x'):
                    i += 1  # skip 'x'
                    c = _read_hex(lexer, src, i)
                    dst.append(c & 0xFF)
                    i += 2
                else:
                    # Octal escape
                    c = _read_oct(lexer, src, i)
                    if c != 0:
                        i += 2
                    dst.append(c & 0xFF)
                    i += 1
            else:
                # Unicode escape \uXXXX
                utf8_bytes = be_load_unicode(src, i + 1)
                if utf8_bytes is None:
                    be_lexerror(lexer, "incorrect '\\u' encoding")
                for b in utf8_bytes:
                    dst.append(b)
                i += 5  # skip 'u' + 4 hex digits
        else:
            dst.append(c)

    # Copy result back into buffer
    for j in range(len(dst)):
        lexer.buf.s[j] = dst[j]
    lexer.buf.len = len(dst)


# ============================================================================
# Newline and comment handling (from be_lexer.c)
# ============================================================================

# static int skip_newline(blexer *lexer)
# {
#     int lc = lgetc(lexer);
#     next(lexer);
#     if (is_newline(lgetc(lexer)) && lgetc(lexer) != lc) {
#         next(lexer);
#     }
#     lexer->linenumber++;
#     lexer->pp_at_line_start = btrue;
#     return lexer->reader.cursor;
# }
def _skip_newline(lexer):
    """Skip a newline sequence (\\n, \\r, \\r\\n, or \\n\\r) and increment line number."""
    lc = _lgetc(lexer)
    _next(lexer)
    cur = _lgetc(lexer)
    if _is_newline(cur) and cur != lc:
        _next(lexer)
    lexer.linenumber += 1
    if BE_USE_PREPROCESSOR:
        lexer.pp_at_line_start = True
    return lexer.reader.cursor


# static void skip_comment(blexer *lexer)
# {
#     next(lexer); /* skip '#' */
#     if (lgetc(lexer) == '-') { /* mult-line comment */
#         int lno = lexer->linenumber;
#         int mark, c = 'x';
#         do {
#             mark = c == '-';
#             if (is_newline(c)) { c = skip_newline(lexer); continue; }
#             c = next(lexer);
#         } while (!(mark && c == '#') && c != EOS);
#         if (c == EOS) {
#             be_lexerror(lexer, be_pushfstring(lexer->vm,
#                 "unterminated comment block started in line %d", lno));
#         }
#         next(lexer); /* skip '#' */
#     } else { /* line comment */
#         while (!is_newline(lgetc(lexer)) && lgetc(lexer)):
#             next(lexer);
#     }
# }
def _skip_comment(lexer):
    """Skip a comment: line (#...) or block (#- ... -#)."""
    _next(lexer)  # skip '#'
    if _lgetc(lexer) == ord('-'):
        # Block comment #- ... -#
        lno = lexer.linenumber
        c = ord('x')
        while True:
            mark = (c == ord('-'))
            if _is_newline(c):
                c = _skip_newline(lexer)
                continue
            c = _next(lexer)
            if (mark and c == ord('#')) or c == 0:
                break
        if c == 0:
            be_lexerror(lexer, "unterminated comment block started in line %d" % lno)
        _next(lexer)  # skip trailing '#'
    else:
        # Line comment
        while not _is_newline(_lgetc(lexer)) and _lgetc(lexer) != 0:
            _next(lexer)


# static void skip_comment_body(blexer *lexer)
# /* Like skip_comment but '#' has already been consumed. */
def _skip_comment_body(lexer):
    """Skip comment body when '#' has already been consumed."""
    if _lgetc(lexer) == ord('-'):
        # Block comment
        lno = lexer.linenumber
        c = ord('x')
        while True:
            mark = (c == ord('-'))
            if _is_newline(c):
                c = _skip_newline(lexer)
                continue
            c = _next(lexer)
            if (mark and c == ord('#')) or c == 0:
                break
        if c == 0:
            be_lexerror(lexer, "unterminated comment block started in line %d" % lno)
        _next(lexer)  # skip trailing '#'
    else:
        # Line comment
        while not _is_newline(_lgetc(lexer)) and _lgetc(lexer) != 0:
            _next(lexer)


# ============================================================================
# Number scanning (from be_lexer.c)
# ============================================================================

# static bbool scan_realexp(blexer *lexer)
def _scan_realexp(lexer):
    """Scan an optional exponent part (e/E followed by optional sign and digits)."""
    c = _lgetc(lexer)
    if c == ord('e') or c == ord('E'):
        c = _save(lexer)
        if c == ord('+') or c == ord('-'):
            c = _save(lexer)
        if not _is_digit(c):
            be_lexerror(lexer, "malformed number")
        while _is_digit(_lgetc(lexer)):
            _save(lexer)
        return True
    return False


# static btokentype scan_dot_real(blexer *lexer)
def _scan_dot_real(lexer):
    """Scan a token starting with '.': could be '.', '..', or a real number."""
    if _save(lexer) == ord('.'):
        _next(lexer)
        return OptConnect
    if _is_digit(_lgetc(lexer)):
        while _is_digit(_lgetc(lexer)):
            _save(lexer)
        _scan_realexp(lexer)
        buf_data = lexer.buf.s[:lexer.buf.len].decode('latin-1')
        _setreal(lexer, be_str2real(buf_data))
        return TokenReal
    return OptDot


# static bbool decimal_dots(blexer *lexer)
def _decimal_dots(lexer):
    """Check if a dot after digits is a decimal point or '..' operator."""
    if _lgetc(lexer) == ord('.'):
        if _save(lexer) != ord('.'):
            while _is_digit(_lgetc(lexer)):
                _save(lexer)
            return True
        # Token '..'
        _next(lexer)
        lexer.cacheType = OptConnect
    return False


# static bint scan_hexadecimal(blexer *lexer)
def _scan_hexadecimal(lexer):
    """Scan hexadecimal digits after 0x/0X prefix."""
    res = 0
    num = 0
    while True:
        dig = be_char2hex(_lgetc(lexer))
        if dig < 0:
            break
        res = (res << 4) + dig
        _next(lexer)
        num += 1
    if num == 0:
        be_lexerror(lexer, "invalid hexadecimal number")
    return _wrap_bint(res)


# static btokentype scan_decimal(blexer *lexer)
def _scan_decimal(lexer):
    """Scan a decimal number (integer or real)."""
    tok_type = TokenInteger
    while _is_digit(_lgetc(lexer)):
        _save(lexer)
    has_dots = _decimal_dots(lexer)
    has_exp = _scan_realexp(lexer)
    if has_dots or has_exp:
        tok_type = TokenReal
    _save_char(lexer, 0)  # null terminator
    buf_data = lexer.buf.s[:lexer.buf.len - 1].decode('latin-1')  # exclude null
    if tok_type == TokenReal:
        _setreal(lexer, be_str2real(buf_data))
    else:
        _setint(lexer, be_str2int(buf_data))
    return tok_type


# static btokentype scan_numeral(blexer *lexer)
def _scan_numeral(lexer):
    """Scan a numeric literal (decimal or hexadecimal)."""
    tok_type = TokenInteger
    c0 = _lgetc(lexer)
    c1 = _save(lexer)
    # hex: 0[xX][0-9a-fA-F]+
    if c0 == ord('0') and (c1 == ord('x') or c1 == ord('X')):
        _next(lexer)
        _setint(lexer, _scan_hexadecimal(lexer))
    else:
        tok_type = _scan_decimal(lexer)
    # Check for invalid trailing characters
    if lexer.cacheType == TokenNone:
        if _is_ident_start(_lgetc(lexer)) or _decimal_dots(lexer):
            be_lexerror(lexer, "malformed number")
    return tok_type


# ============================================================================
# String scanning (from be_lexer.c)
# ============================================================================

# static btokentype scan_string(blexer *lexer)
def _scan_string(lexer):
    """Scan a string literal (single or double quoted), handling concatenation."""
    while True:
        end = _lgetc(lexer)  # string delimiter
        _next(lexer)  # skip opening quote
        while True:
            c = _lgetc(lexer)
            if c == 0 or c == end:
                break
            _save(lexer)
            if c == ord('\\'):
                _save(lexer)
        if c == 0:
            be_lexerror(lexer, "unfinished string")
        c = _next(lexer)  # skip closing quote

        if BE_USE_PREPROCESSOR:
            if c == ord('#'):
                _next(lexer)  # consume '#'
                if _is_ident_start(_lgetc(lexer)):
                    lexer.pp_translatable_ready = True
                    break
                # Not translatable — treat as comment
                _skip_comment_body(lexer)
                _skip_delimiter(lexer)
                c = _lgetc(lexer)
                if c != ord('"') and c != ord("'"):
                    break
                continue

        # Check for adjacent string literal (concatenation)
        _skip_delimiter(lexer)
        c = _lgetc(lexer)
        if c != ord('"') and c != ord("'"):
            break

    _tr_string(lexer)
    _setstr(lexer, _buf_tostr(lexer))
    return TokenString


# ============================================================================
# F-string scanning (from be_lexer.c)
# ============================================================================

# static void scan_f_string(blexer *lexer)
def _scan_f_string(lexer):
    """Scan an f-string and transpile it to format(...) syntax.

    The f-string content is parsed and converted into a call to the
    format() function, which is then fed back into the lexer as if
    it were source code.
    """
    _clear_buf(lexer)
    _scan_string(lexer)  # scan the entire string into lexer.buf

    # Save original reader state
    saved_reader_s = lexer.reader.s
    saved_reader_len = lexer.reader.len
    saved_reader_data = lexer.reader.data
    saved_reader_readf = lexer.reader.readf
    saved_reader_cursor = lexer.reader.cursor

    # Save the unparsed f-string buffer
    unparsed_s = lexer.buf.s[:lexer.buf.len]
    unparsed_len = lexer.buf.len

    # Prepare a new buffer for the transpiled output
    new_size = unparsed_len + 20
    lexer.buf.s = bytearray(new_size)
    lexer.buf.size = new_size
    lexer.buf.len = 0

    # First pass: build format string
    _save_char(lexer, ord('('))
    _save_char(lexer, ord('"'))

    i = 0
    while i < unparsed_len:
        ch = unparsed_s[i]
        if ch == ord('%'):
            _save_char(lexer, ord('%'))
            _save_char(lexer, ord('%'))
        elif ch == ord('\\'):
            _save_char(lexer, ord('\\'))
            _save_char(lexer, ord('\\'))
        elif ch == ord('"'):
            _save_char(lexer, ord('\\'))
            _save_char(lexer, ord('"'))
        elif ch == ord('}'):
            if i + 1 < unparsed_len and unparsed_s[i + 1] == ord('}'):
                i += 1
            _save_char(lexer, ord('}'))
        elif ch == ord('\n'):
            _save_char(lexer, ord('\\'))
            _save_char(lexer, ord('n'))
        elif ch == ord('\r'):
            _save_char(lexer, ord('\\'))
            _save_char(lexer, ord('r'))
        elif ch == ord('{'):
            i += 1
            if i < unparsed_len and unparsed_s[i] == ord('{'):
                _save_char(lexer, ord('{'))
            else:
                # Expression inside {}
                rollback = lexer.buf.len
                while i < unparsed_len:
                    ch = unparsed_s[i]
                    # stop parsing if single ':' or '}'
                    if ch == ord('}') or (ch == ord(':') and unparsed_s[i + 1] != ord(':')):
                        break
                    _save_char(lexer, ch)
                    if ch == ord('='):
                        break
                    # if '::' then encode the second ':' but don't parse it again in next iteration
                    if ch == ord(':'):
                        _save_char(lexer, ch)
                        i += 1
                    i += 1
                if i >= unparsed_len:
                    _be_raise(lexer.vm, "syntax_error", "'}' expected")

                if ch == ord('='):
                    i += 1
                    if i >= unparsed_len:
                        _be_raise(lexer.vm, "syntax_error", "'}' expected")
                    ch = unparsed_s[i]
                    if ch != ord(':') and ch != ord('}'):
                        _be_raise(lexer.vm, "syntax_error", "':' or '}' expected after '='")
                else:
                    lexer.buf.len = rollback

                _save_char(lexer, ord('%'))
                if ch == ord(':'):
                    i += 1
                    if i < unparsed_len and unparsed_s[i] == ord('%'):
                        i += 1
                    while i < unparsed_len:
                        ch = unparsed_s[i]
                        if ch == ord('}'):
                            break
                        _save_char(lexer, ch)
                        i += 1
                    if i >= unparsed_len:
                        _be_raise(lexer.vm, "syntax_error", "'}' expected")
                else:
                    _save_char(lexer, ord('s'))
        else:
            _save_char(lexer, ch)
        i += 1

    _save_char(lexer, ord('"'))

    # Second pass: add arguments
    i = 0
    while i < unparsed_len:
        if unparsed_s[i] == ord('{'):
            i += 1
            if i < unparsed_len and unparsed_s[i] == ord('{'):
                i += 1
                continue
            _save_char(lexer, ord(','))
            while i < unparsed_len:
                ch = unparsed_s[i]
                if ch == ord(':') and i + 1 < unparsed_len and unparsed_s[i + 1] == ord(':'):
                    _save_char(lexer, ch)
                    i += 1  # skip second ':'
                elif ch == ord('=') or ch == ord(':') or ch == ord('}'):
                    break
                else:
                    _save_char(lexer, ch)
                i += 1
            if ch == ord('=') or ch == ord(':'):
                i += 1
                while i < unparsed_len:
                    ch = unparsed_s[i]
                    if ch == ord('}'):
                        break
                    i += 1
        i += 1

    _save_char(lexer, ord(')'))

    # Set up a temporary reader from the transpiled buffer
    parsed_s = lexer.buf.s[:lexer.buf.len].decode('latin-1')
    parsed_len = lexer.buf.len

    # Restore the original buffer (the unparsed f-string)
    lexer.buf.s = bytearray(max(SHORT_STR_LEN, len(unparsed_s)))
    lexer.buf.s[:len(unparsed_s)] = unparsed_s
    lexer.buf.len = unparsed_len
    lexer.buf.size = len(lexer.buf.s)

    # Set up temporary reader for the transpiled string
    _fstr_remaining = [parsed_s]
    _fstr_saved_cursor = [saved_reader_cursor]
    _fstr_restored = [False]

    def _fstr_reader(lex, data, size_ref):
        if _fstr_remaining[0] is not None:
            s = _fstr_remaining[0]
            _fstr_remaining[0] = None
            size_ref[0] = len(s)
            return s
        elif not _fstr_restored[0]:
            _fstr_restored[0] = True
            # Restore original reader
            lex.reader.s = saved_reader_s
            lex.reader.len = saved_reader_len
            lex.reader.data = saved_reader_data
            lex.reader.readf = saved_reader_readf
            # Build a single chunk: saved cursor char + remaining buffer.
            # We must return everything in one call because _next will
            # overwrite lr.s/lr.len with whatever we return here, so any
            # data not included would be lost.
            prefix = ""
            if _fstr_saved_cursor[0] >= 0:
                prefix = chr(_fstr_saved_cursor[0])
                _fstr_saved_cursor[0] = -1
            remaining = ""
            if saved_reader_s and saved_reader_len > 0:
                remaining = saved_reader_s[:saved_reader_len]
            combined = prefix + remaining
            if combined:
                size_ref[0] = len(combined)
                return combined
            return lex.reader.readf(lex, lex.reader.data, size_ref)
        else:
            return lex.reader.readf(lex, lex.reader.data, size_ref)

    lexer.reader.readf = _fstr_reader
    lexer.reader.data = None
    lexer.reader.s = ""
    lexer.reader.len = 0

    _next(lexer)  # start parsing the transpiled f-string (always '(')

    # Replace 'f' identifier with 'format'
    lexer.buf.len = 6
    lexer.buf.s[:6] = b'format'


# ============================================================================
# Identifier scanning (from be_lexer.c)
# ============================================================================

# static btokentype scan_identifier(blexer *lexer)
def _scan_identifier(lexer):
    """Scan an identifier or keyword. Handles f-string detection."""
    _save(lexer)
    while _is_word(_lgetc(lexer)):
        _save(lexer)

    # Check for f-string: f"..." or f'...'
    ch = _lgetc(lexer)
    if (lexer.buf.len == 1 and lexer.buf.s[0] == ord('f')
            and (ch == ord('"') or ch == ord("'"))):
        _scan_f_string(lexer)

    s = _buf_tostr(lexer)
    tok_type = str_extra(s)
    if tok_type >= KeyIf and tok_type < _type_count():
        lexer.token.type = tok_type
        return lexer.token.type
    _setstr(lexer, s)
    return TokenId


# ============================================================================
# Operator scanning (from be_lexer.c)
# ============================================================================

# static btokentype scan_assign(blexer *lexer, btokentype is, btokentype not)
def _scan_assign(lexer, is_tok, not_tok):
    """Scan an operator that may be followed by '=' for compound assignment."""
    _next(lexer)
    return is_tok if _check_next(lexer, '=') else not_tok


# static btokentype scan_sub(blexer *lexer)
def _scan_sub(lexer):
    """Scan '-', '->', or '-='."""
    c = _next(lexer)
    if c == ord('>'):
        op = OptArrow
    elif c == ord('='):
        op = OptSubAssign
    else:
        return OptSub
    _next(lexer)
    return op


# static btokentype scan_and(blexer *lexer)
def _scan_and(lexer):
    """Scan '&', '&&', or '&='."""
    c = _next(lexer)
    if c == ord('&'):
        op = OptAnd
    elif c == ord('='):
        op = OptAndAssign
    else:
        return OptBitAnd
    _next(lexer)
    return op


# static btokentype scan_or(blexer *lexer)
def _scan_or(lexer):
    """Scan '|', '||', or '|='."""
    c = _next(lexer)
    if c == ord('|'):
        op = OptOr
    elif c == ord('='):
        op = OptOrAssign
    else:
        return OptBitOr
    _next(lexer)
    return op


# static btokentype scan_le(blexer *lexer)
def _scan_le(lexer):
    """Scan '<', '<=', '<<', or '<<='."""
    c = _next(lexer)
    if c == ord('='):
        _next(lexer)
        return OptLE
    elif c == ord('<'):
        _next(lexer)
        return OptLsfAssign if _check_next(lexer, '=') else OptShiftL
    return OptLT


# static btokentype scan_ge(blexer *lexer)
def _scan_ge(lexer):
    """Scan '>', '>=', '>>', or '>>='."""
    c = _next(lexer)
    if c == ord('='):
        _next(lexer)
        return OptGE
    elif c == ord('>'):
        _next(lexer)
        return OptRsfAssign if _check_next(lexer, '=') else OptShiftR
    return OptGT


# ============================================================================
# Delimiter skipping (from be_lexer.c)
# ============================================================================

# static int skip_delimiter(blexer *lexer)
def _skip_delimiter(lexer):
    """Skip whitespace, newlines, and comments. Returns 1 if any were found."""
    c = _lgetc(lexer)
    delimiter_present = 0
    while True:
        if c == ord('#'):
            if BE_USE_PREPROCESSOR and lexer.pp_at_line_start:
                nc = _next(lexer)  # consume '#', peek next
                if ord('a') <= nc <= ord('z'):
                    lexer.pp_hash_consumed = True
                    break
                # Not a directive — treat as comment
                _skip_comment_body(lexer)
                c = _lgetc(lexer)
                delimiter_present = 1
                continue
            _skip_comment(lexer)
        elif c == ord('\r') or c == ord('\n'):
            _skip_newline(lexer)
        elif c == ord(' ') or c == ord('\t') or c == ord('\f') or c == ord('\v'):
            _next(lexer)
        else:
            break
        c = _lgetc(lexer)
        delimiter_present = 1
    lexer.had_whitespace = delimiter_present
    return delimiter_present


# ============================================================================
# Preprocessor functions (from be_lexer.c, guarded by BE_USE_PREPROCESSOR)
# ============================================================================

# static bbool pp_skipping(blexer *lexer)
def _pp_skipping(lexer):
    """Check if currently inside a false conditional block."""
    return (lexer.ppdepth >= 0
            and not lexer.ppstack[lexer.ppdepth].active)


# static void pp_skip_to_eol(blexer *lexer)
def _pp_skip_to_eol(lexer):
    """Consume characters until newline or EOF."""
    while not _is_newline(_lgetc(lexer)) and _lgetc(lexer) != 0:
        _next(lexer)


# static int pp_read_ident(blexer *lexer)
def _pp_read_ident(lexer):
    """Read an identifier from current position into lexer buffer.
    Returns the length of the identifier read (0 if none)."""
    length = 0
    _clear_buf(lexer)
    if _is_ident_start(_lgetc(lexer)):
        while True:
            _save(lexer)
            length += 1
            if not _is_word(_lgetc(lexer)):
                break
    return length


# static bvalue* pp_find_macro(blexer *lexer, const char *name, int len)
def _pp_find_macro(lexer, name, length):
    """Look up a macro name in the VM's preprocessor map.
    Returns the bvalue if found, None otherwise."""
    vm = lexer.vm
    pp_map = getattr(vm, 'preprocessor', None)
    if pp_map is not None:
        key = be_newstrn(vm, name, length)
        return be_map_findstr(vm, pp_map, key)
    return None


# static bbool pp_macro_truthy(blexer *lexer, const char *name, int len)
def _pp_macro_truthy(lexer, name, length):
    """Evaluate truthiness of a macro for #if evaluation."""
    val = _pp_find_macro(lexer, name, length)
    if val is None:
        return False
    if var_isint(val) and var_toint(val) == 0:
        return False
    return True


# static bmap* pp_ensure_map(blexer *lexer)
def _pp_ensure_map(lexer):
    """Ensure the VM's preprocessor map exists."""
    vm = lexer.vm
    if not hasattr(vm, 'preprocessor') or vm.preprocessor is None:
        vm.preprocessor = be_map_new(vm)
    return vm.preprocessor


# static bbool pp_parse_int(const char *s, int len, bint *out)
def _pp_parse_int(s, length):
    """Try to parse a string as an integer. Returns (True, value) or (False, 0)."""
    if length == 0:
        return False, 0
    i = 0
    neg = False
    if s[0] == '-':
        neg = True
        i = 1
    elif s[0] == '+':
        i = 1
    if i >= length:
        return False, 0
    val = 0
    while i < length:
        if s[i] < '0' or s[i] > '9':
            return False, 0
        val = val * 10 + (ord(s[i]) - ord('0'))
        i += 1
    return True, (-val if neg else val)


# static void pp_define(blexer *lexer)
def _pp_define(lexer):
    """Handle #define MACRO_NAME [value]."""
    vm = lexer.vm

    # Skip whitespace after #define
    while _lgetc(lexer) == ord(' ') or _lgetc(lexer) == ord('\t'):
        _next(lexer)

    # Read macro name
    namelen = _pp_read_ident(lexer)
    if namelen == 0:
        be_lexerror(lexer, "macro name expected after '#define'")
        return
    name = lexer.buf.s[:namelen].decode('latin-1')

    # Intern the macro name
    pp_map = _pp_ensure_map(lexer)
    key = be_newstrn(vm, name, namelen)
    _ensure_stack_slot(vm)
    var_setstr(vm.stack[vm.top_idx], key)
    _be_stackpush(vm)

    # Skip whitespace between name and value
    while _lgetc(lexer) == ord(' ') or _lgetc(lexer) == ord('\t'):
        _next(lexer)

    # Check if there's a value
    if _is_newline(_lgetc(lexer)) or _lgetc(lexer) == 0:
        val = bvalue()
        var_setint(val, 1)
    else:
        # Read value: collect characters until end of line
        _clear_buf(lexer)
        while not _is_newline(_lgetc(lexer)) and _lgetc(lexer) != 0:
            _save(lexer)
        vstart = lexer.buf.s[:lexer.buf.len].decode('latin-1')
        vlen = len(vstart)

        # Trim trailing whitespace
        while vlen > 0 and vstart[vlen - 1] in (' ', '\t'):
            vlen -= 1

        val = bvalue()
        if vlen == 0:
            var_setint(val, 1)
        else:
            ok, ival = _pp_parse_int(vstart, vlen)
            if ok:
                var_setint(val, ival)
            else:
                s = be_newstrn(vm, vstart, vlen)
                var_setstr(val, s)

    be_map_insertstr(vm, pp_map, key, val)
    _be_stackpop(vm, 1)


# static void pp_undef(blexer *lexer)
def _pp_undef(lexer):
    """Handle #undef MACRO_NAME."""
    vm = lexer.vm

    while _lgetc(lexer) == ord(' ') or _lgetc(lexer) == ord('\t'):
        _next(lexer)

    namelen = _pp_read_ident(lexer)
    if namelen == 0:
        be_lexerror(lexer, "macro name expected after '#undef'")
        return

    pp_map = getattr(vm, 'preprocessor', None)
    if pp_map is not None:
        name = lexer.buf.s[:namelen].decode('latin-1')
        key = be_newstrn(vm, name, namelen)
        be_map_removestr(vm, pp_map, key)

    _pp_skip_to_eol(lexer)


# static void pp_if(blexer *lexer)
def _pp_if(lexer):
    """Handle #if MACRO_NAME or #if !MACRO_NAME."""
    negate = False

    if lexer.ppdepth + 1 >= BE_PREPROC_MAX_DEPTH:
        be_lexerror(lexer, "preprocessor conditional nesting too deep")
        return

    # If already skipping, push dummy inactive entry
    if _pp_skipping(lexer):
        lexer.ppdepth += 1
        lexer.ppstack[lexer.ppdepth].active = 0
        lexer.ppstack[lexer.ppdepth].matched = 1
        lexer.ppstack[lexer.ppdepth].line = lexer.linenumber
        _pp_skip_to_eol(lexer)
        return

    while _lgetc(lexer) == ord(' ') or _lgetc(lexer) == ord('\t'):
        _next(lexer)

    if _lgetc(lexer) == ord('!'):
        negate = True
        _next(lexer)
        while _lgetc(lexer) == ord(' ') or _lgetc(lexer) == ord('\t'):
            _next(lexer)

    namelen = _pp_read_ident(lexer)
    if namelen == 0:
        be_lexerror(lexer, "macro name expected after '#if'")
        return

    name = lexer.buf.s[:namelen].decode('latin-1')
    truthy = _pp_macro_truthy(lexer, name, namelen)
    if negate:
        truthy = not truthy

    lexer.ppdepth += 1
    lexer.ppstack[lexer.ppdepth].active = 1 if truthy else 0
    lexer.ppstack[lexer.ppdepth].matched = 1 if truthy else 0
    lexer.ppstack[lexer.ppdepth].line = lexer.linenumber

    _pp_skip_to_eol(lexer)


# static void pp_elif(blexer *lexer)
def _pp_elif(lexer):
    """Handle #elif MACRO_NAME or #elif !MACRO_NAME."""
    if lexer.ppdepth < 0:
        be_lexerror(lexer, "'#elif' without matching '#if'")
        return

    if lexer.ppstack[lexer.ppdepth].matched:
        lexer.ppstack[lexer.ppdepth].active = 0
        _pp_skip_to_eol(lexer)
        return

    negate = False
    while _lgetc(lexer) == ord(' ') or _lgetc(lexer) == ord('\t'):
        _next(lexer)

    if _lgetc(lexer) == ord('!'):
        negate = True
        _next(lexer)
        while _lgetc(lexer) == ord(' ') or _lgetc(lexer) == ord('\t'):
            _next(lexer)

    namelen = _pp_read_ident(lexer)
    if namelen == 0:
        be_lexerror(lexer, "macro name expected after '#elif'")
        return

    name = lexer.buf.s[:namelen].decode('latin-1')
    truthy = _pp_macro_truthy(lexer, name, namelen)
    if negate:
        truthy = not truthy

    lexer.ppstack[lexer.ppdepth].active = 1 if truthy else 0
    if truthy:
        lexer.ppstack[lexer.ppdepth].matched = 1

    _pp_skip_to_eol(lexer)


# static void pp_else(blexer *lexer)
def _pp_else(lexer):
    """Handle #else."""
    if lexer.ppdepth < 0:
        be_lexerror(lexer, "'#else' without matching '#if'")
        return

    if lexer.ppstack[lexer.ppdepth].matched:
        lexer.ppstack[lexer.ppdepth].active = 0
    else:
        lexer.ppstack[lexer.ppdepth].active = 1
        lexer.ppstack[lexer.ppdepth].matched = 1

    _pp_skip_to_eol(lexer)


# static void pp_endif(blexer *lexer)
def _pp_endif(lexer):
    """Handle #endif."""
    if lexer.ppdepth < 0:
        be_lexerror(lexer, "'#endif' without matching '#if'")
        return
    lexer.ppdepth -= 1
    _pp_skip_to_eol(lexer)


# static bbool pp_process_directive(blexer *lexer)
def _pp_process_directive(lexer):
    """Process a directive line after '#' has been consumed.

    The cursor is on the first character after '#'.
    Returns True if a valid directive was processed, False if '#'
    should be treated as a comment.
    """
    c = _lgetc(lexer)
    if c < ord('a') or c > ord('z'):
        return False

    length = _pp_read_ident(lexer)
    if length == 0:
        return False

    kw = lexer.buf.s[:length].decode('latin-1')

    if kw == "if":
        _pp_if(lexer)
    elif kw == "elif":
        _pp_elif(lexer)
    elif kw == "else":
        _pp_else(lexer)
    elif kw == "endif":
        _pp_endif(lexer)
    elif kw == "define":
        if not _pp_skipping(lexer):
            _pp_define(lexer)
        else:
            _pp_skip_to_eol(lexer)
    elif kw == "undef":
        if not _pp_skipping(lexer):
            _pp_undef(lexer)
        else:
            _pp_skip_to_eol(lexer)
    else:
        return False

    return True


# static void pp_scan_translatable(blexer *lexer)
def _pp_scan_translatable(lexer):
    """After scan_string, check for translatable string ("text"#MACRO).

    If pp_translatable_ready is set, '#' was already consumed and lgetc
    is the first character of the macro identifier.
    """
    if not lexer.pp_translatable_ready:
        return
    lexer.pp_translatable_ready = False

    length = _pp_read_ident(lexer)
    name = lexer.buf.s[:length].decode('latin-1')
    val = _pp_find_macro(lexer, name, length)
    if val is not None and var_isstr(val):
        _setstr(lexer, var_tostr(val))


# ============================================================================
# Main lexer dispatch loop (from be_lexer.c)
# ============================================================================

# static btokentype lexer_next(blexer *lexer)
def _lexer_next(lexer):
    """Main lexer dispatch loop. Scans and returns the next token type.

    Handles whitespace/comment skipping, preprocessor directives (when
    BE_USE_PREPROCESSOR is enabled), and dispatches to the appropriate
    scanner for each token type.
    """
    while True:
        # --- Preprocessor skipping mode ---
        if BE_USE_PREPROCESSOR:
            while _pp_skipping(lexer):
                c = _lgetc(lexer)
                if c == 0:
                    break
                if _is_newline(c):
                    _skip_newline(lexer)
                    # After newline, check for '#' directive
                    while _lgetc(lexer) == ord(' ') or _lgetc(lexer) == ord('\t'):
                        _next(lexer)
                    if _lgetc(lexer) == ord('#'):
                        _next(lexer)  # consume '#'
                        if not _pp_process_directive(lexer):
                            _pp_skip_to_eol(lexer)
                        _clear_buf(lexer)
                    continue
                _next(lexer)

        # --- Check if skip_delimiter already consumed '#' for a directive ---
        if BE_USE_PREPROCESSOR and lexer.pp_hash_consumed:
            lexer.pp_hash_consumed = False
            c = _lgetc(lexer)
            if c == ord('-'):
                # Block comment #- ... -#
                lno = lexer.linenumber
                c2 = ord('x')
                while True:
                    mark = (c2 == ord('-'))
                    if _is_newline(c2):
                        c2 = _skip_newline(lexer)
                        continue
                    c2 = _next(lexer)
                    if (mark and c2 == ord('#')) or c2 == 0:
                        break
                if c2 == 0:
                    be_lexerror(lexer, "unterminated comment block started in line %d" % lno)
                _next(lexer)
            elif _pp_process_directive(lexer):
                lexer.pp_at_line_start = False
                _clear_buf(lexer)
            else:
                _pp_skip_to_eol(lexer)
                _clear_buf(lexer)
            lexer.had_whitespace = 1
            continue

        c = _lgetc(lexer)

        # --- Whitespace and comments ---
        if c == ord('\r') or c == ord('\n'):
            _skip_newline(lexer)
            lexer.had_whitespace = 1
            continue
        if c == ord(' ') or c == ord('\t') or c == ord('\f') or c == ord('\v'):
            _next(lexer)
            lexer.had_whitespace = 1
            continue
        if c == ord('#'):
            if BE_USE_PREPROCESSOR and lexer.pp_at_line_start:
                _next(lexer)  # consume '#'
                if _lgetc(lexer) == ord('-'):
                    # Block comment
                    lno = lexer.linenumber
                    c2 = ord('x')
                    while True:
                        mark = (c2 == ord('-'))
                        if _is_newline(c2):
                            c2 = _skip_newline(lexer)
                            continue
                        c2 = _next(lexer)
                        if (mark and c2 == ord('#')) or c2 == 0:
                            break
                    if c2 == 0:
                        be_lexerror(lexer, "unterminated comment block started in line %d" % lno)
                    _next(lexer)
                elif _pp_process_directive(lexer):
                    lexer.pp_at_line_start = False
                    _clear_buf(lexer)
                else:
                    _pp_skip_to_eol(lexer)
                    _clear_buf(lexer)
                lexer.had_whitespace = 1
                continue
            _skip_comment(lexer)
            lexer.had_whitespace = 1
            continue

        # --- End of source ---
        if c == 0:
            if BE_USE_PREPROCESSOR and lexer.ppdepth >= 0:
                be_lexerror(lexer,
                    "unfinished conditional block started in line %d"
                    % lexer.ppstack[lexer.ppdepth].line)
            return TokenEOS

        # --- Operators ---
        if c == ord('+'):
            return _scan_assign(lexer, OptAddAssign, OptAdd)
        if c == ord('-'):
            return _scan_sub(lexer)
        if c == ord('*'):
            return _scan_assign(lexer, OptMulAssign, OptMul)
        if c == ord('/'):
            return _scan_assign(lexer, OptDivAssign, OptDiv)
        if c == ord('%'):
            return _scan_assign(lexer, OptModAssign, OptMod)
        if c == ord('('):
            _next(lexer)
            return OptSpaceLBK if lexer.had_whitespace else OptCallLBK
        if c == ord(')'):
            _next(lexer)
            return OptRBK
        if c == ord('['):
            _next(lexer)
            return OptLSB
        if c == ord(']'):
            _next(lexer)
            return OptRSB
        if c == ord('{'):
            _next(lexer)
            return OptLBR
        if c == ord('}'):
            _next(lexer)
            return OptRBR
        if c == ord(','):
            _next(lexer)
            return OptComma
        if c == ord(';'):
            _next(lexer)
            return OptSemic
        if c == ord(':'):
            _next(lexer)
            return OptWalrus if _check_next(lexer, '=') else OptColon
        if c == ord('?'):
            _next(lexer)
            return OptQuestion
        if c == ord('^'):
            return _scan_assign(lexer, OptXorAssign, OptBitXor)
        if c == ord('~'):
            _next(lexer)
            return OptFlip
        if c == ord('&'):
            return _scan_and(lexer)
        if c == ord('|'):
            return _scan_or(lexer)
        if c == ord('<'):
            return _scan_le(lexer)
        if c == ord('>'):
            return _scan_ge(lexer)
        if c == ord('='):
            _next(lexer)
            return OptEQ if _check_next(lexer, '=') else OptAssign
        if c == ord('!'):
            _next(lexer)
            return OptNE if _check_next(lexer, '=') else OptNot

        # --- Strings ---
        if c == ord("'") or c == ord('"'):
            tok_type = _scan_string(lexer)
            if BE_USE_PREPROCESSOR:
                _pp_scan_translatable(lexer)
            return tok_type

        # --- Dot / connect / real starting with dot ---
        if c == ord('.'):
            return _scan_dot_real(lexer)

        # --- Numbers ---
        if ord('0') <= c <= ord('9'):
            return _scan_numeral(lexer)

        # --- Identifiers and keywords ---
        if _is_ident_start(c):
            return _scan_identifier(lexer)

        # --- Stray character ---
        be_lexerror(lexer, "stray '\\%d' in program" % (c & 0xFF))
        return TokenNone


# ============================================================================
# Public API (from be_lexer.c)
# ============================================================================

# static void lexerbuf_init(blexer *lexer)
# {
#     lexer->buf.size = SHORT_STR_LEN;
#     lexer->buf.s = be_malloc(lexer->vm, SHORT_STR_LEN);
#     lexer->buf.len = 0;
# }
def _lexerbuf_init(lexer):
    """Initialize the lexer's internal character buffer."""
    lexer.buf.size = SHORT_STR_LEN
    lexer.buf.s = bytearray(SHORT_STR_LEN)
    lexer.vm.gc.usage += SHORT_STR_LEN
    lexer.buf.len = 0


# void be_lexer_init(blexer *lexer, bvm *vm,
#     const char *fname, breader reader, void *data)
# {
#     lexer->vm = vm;
#     lexer->cacheType = TokenNone;
#     lexer->fname = fname;
#     lexer->linenumber = 1;
#     lexer->lastline = 1;
#     lexer->reader.readf = reader;
#     lexer->reader.data = data;
#     lexer->reader.len = 0;
#     lexer->had_whitespace = 1;
#     lexer->ppdepth = -1;
#     lexer->pp_at_line_start = btrue;
#     lexer->pp_translatable_ready = bfalse;
#     lexer->pp_hash_consumed = bfalse;
#     lexerbuf_init(lexer);
#     keyword_registe(vm);
#     lexer->strtab = be_map_new(vm);
#     var_setmap(vm->top, lexer->strtab);
#     be_stackpush(vm);
#     next(lexer);
# }
def be_lexer_init(lexer, vm, fname, reader, data):
    """Initialize the lexer with a source reader.

    Args:
        lexer: blexer instance to initialize.
        vm: Berry VM instance.
        fname: Source file name (for error messages).
        reader: Reader callback with signature (lexer, data, size_ref) -> str or None.
        data: User data passed to the reader callback.
    """
    lexer.vm = vm
    lexer.cacheType = TokenNone
    lexer.fname = fname
    lexer.linenumber = 1
    lexer.lastline = 1
    lexer.reader.readf = reader
    lexer.reader.data = data
    lexer.reader.len = 0
    lexer.had_whitespace = 1
    if BE_USE_PREPROCESSOR:
        lexer.ppdepth = -1
        lexer.pp_at_line_start = True
        lexer.pp_translatable_ready = False
        lexer.pp_hash_consumed = False
    _lexerbuf_init(lexer)
    _keyword_register(vm)
    lexer.strtab = be_map_new(vm)
    _ensure_stack_slot(vm)
    var_setmap(vm.stack[vm.top_idx], lexer.strtab)
    _be_stackpush(vm)
    _next(lexer)  # read the first character


# void be_lexer_deinit(blexer *lexer)
# {
#     be_free(lexer->vm, lexer->buf.s, lexer->buf.size);
#     keyword_unregiste(lexer->vm);
# }
def be_lexer_deinit(lexer):
    """Deinitialize the lexer, freeing the buffer and unregistering keywords."""
    if lexer.buf.s is not None:
        lexer.vm.gc.usage -= lexer.buf.size
        lexer.buf.s = None
    _keyword_unregister(lexer.vm)


# int be_lexer_scan_next(blexer *lexer)
# {
#     btokentype type;
#     if (lexer->cacheType != TokenNone) {
#         lexer->token.type = lexer->cacheType;
#         lexer->cacheType = TokenNone;
#         return 1;
#     }
#     if (lgetc(lexer) == EOS) {
#         lexer->token.type = TokenEOS;
#         return 0;
#     }
#     lexer->lastline = lexer->linenumber;
#     lexer->had_whitespace = 0;
#     type = lexer_next(lexer);
#     clear_buf(lexer);
#     if (type != TokenNone) {
#         lexer->token.type = type;
#         if BE_USE_PREPROCESSOR:
#             if (type != TokenEOS &&
#                 !(lexer->pp_at_line_start &&
#                   (lgetc(lexer) == '#' || lexer->pp_hash_consumed))):
#                 lexer->pp_at_line_start = bfalse;
#     } else {
#         lexer->token.type = TokenEOS;
#         return 0;
#     }
#     return 1;
# }
def be_lexer_scan_next(lexer):
    """Scan the next token. Returns 1 if a token was produced, 0 at EOS."""
    if lexer.cacheType != TokenNone:
        lexer.token.type = lexer.cacheType
        lexer.cacheType = TokenNone
        return 1

    if _lgetc(lexer) == 0:
        lexer.token.type = TokenEOS
        return 0

    lexer.lastline = lexer.linenumber
    lexer.had_whitespace = 0
    tok_type = _lexer_next(lexer)
    _clear_buf(lexer)

    if tok_type != TokenNone and tok_type != TokenEOS:
        lexer.token.type = tok_type
        if BE_USE_PREPROCESSOR:
            if not (lexer.pp_at_line_start
                    and (_lgetc(lexer) == ord('#')
                         or lexer.pp_hash_consumed)):
                lexer.pp_at_line_start = False
        return 1
    else:
        lexer.token.type = TokenEOS
        return 0


# const char* be_token2str(bvm *vm, btoken *token)
# {
#     switch (token->type) {
#     case TokenString:
#     case TokenId:
#         return str(token->u.s);
#     case TokenInteger:
#         return be_pushfstring(vm, "%d", token->u.i);
#     case TokenReal:
#         return be_pushfstring(vm, "%g", token->u.r);
#     default:
#         return token_strings[token->type];
#     }
# }
def be_token2str(vm, token):
    """Convert a token to its string representation."""
    if token.type == TokenString or token.type == TokenId:
        return be_str2cstr(token.s)
    elif token.type == TokenInteger:
        return str(token.i)
    elif token.type == TokenReal:
        return "%g" % token.r
    else:
        if 0 <= token.type < len(token_strings):
            return token_strings[token.type]
        return "?"


# const char* be_tokentype2str(btokentype type)
# {
#     return token_strings[type];
# }
def be_tokentype2str(tok_type):
    """Convert a token type to its string name."""
    if 0 <= tok_type < len(token_strings):
        return token_strings[tok_type]
    return "?"
