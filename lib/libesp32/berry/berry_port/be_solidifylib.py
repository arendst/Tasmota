"""
Berry solidification module: serialize Berry objects to C source code.
Mirrors: src/be_solidifylib.c

The solidification module traverses Berry closures, classes, and modules
and emits C source code using Berry's constant object macros. This is the
critical path for Tasmota integration — output must be character-for-character
identical to the C implementation.

Original C code is included as comments for each function.
"""

# /********************************************************************
# ** Copyright (c) 2018-2020 Guan Wenliang
# ** This file is part of the Berry default interpreter.
# ** skiars@qq.com, https://github.com/Skiars/berry
# ** See Copyright Notice in the LICENSE file or at
# ** https://github.com/Skiars/berry/blob/master/LICENSE
# ********************************************************************/

import struct
import sys

from berry_port.be_object import (
    BE_NIL, BE_INT, BE_REAL, BE_BOOL, BE_COMPTR, BE_INDEX,
    BE_STRING, BE_CLASS, BE_INSTANCE, BE_CLOSURE, BE_NTVFUNC,
    BE_LIST, BE_MAP, BE_MODULE, BE_STATIC,
    BE_VA_STATICMETHOD, BE_VA_SHARED_KTAB, BE_VA_NOCOMPACT,
    bvalue,
    var_primetype, var_type, var_tobool, var_toint, var_toreal,
    var_tostr, var_toobj, var_isstatic,
    var_isstr, var_isclosure, var_isclass, var_ismodule,
    gc_isconst,
)
from berry_port.be_decoder import (
    IGET_OP, IGET_RA, IGET_RKB, IGET_RKC, IGET_Bx,
    ISET_RKB, ISET_RKC, ISET_Bx,
    isK, setK, isKB, isKC, KR_MASK,
    IRKB_MASK, IRKC_MASK, IBx_MASK,
    OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD,
    OP_LT, OP_LE, OP_EQ, OP_NE, OP_GT, OP_GE,
    OP_CONNECT, OP_GETMBR, OP_SETMBR, OP_GETMET,
    OP_GETIDX, OP_SETIDX, OP_AND, OP_OR, OP_XOR, OP_SHL, OP_SHR,
    OP_RAISE, OP_MOVE, OP_SETSUPER, OP_NEG, OP_FLIP, OP_IMPORT,
    OP_GETNGBL, OP_SETNGBL,
    OP_CLASS, OP_LDCONST, OP_RET,
    OP_GETGBL, OP_SETGBL,
)
from berry_port.berry_conf import BE_INTGER_TYPE, BE_USE_SINGLE_FLOAT


# ============================================================================
# Lazy imports to avoid circular dependencies
# ============================================================================

def _lazy_be_string():
    import berry_port.be_string as mod
    return mod

def _lazy_be_map():
    import berry_port.be_map as mod
    return mod

def _lazy_be_debug():
    import berry_port.be_debug as mod
    return mod

def _lazy_be_api():
    import berry_port.be_api as mod
    return mod

def _lazy_be_module():
    import berry_port.be_module as mod
    return mod

def _lazy_be_vector():
    import berry_port.be_vector as mod
    return mod

def _lazy_be_byteslib():
    import berry_port.be_byteslib as mod
    return mod

def _lazy_be_mem():
    import berry_port.be_mem as mod
    return mod


# ============================================================================
# Output helpers (mirrors logfmt / lognofmt macros)
# ============================================================================

# #define logfmt(...)
# #define lognofmt(__s)
def _logfmt(fout, fmt, *args):
    """Format and write a string. Mirrors the C logfmt macro."""
    if args:
        s = fmt % args
    else:
        s = fmt
    if fout is not None:
        fout.write(s)
    else:
        sys.stdout.write(s)

def _lognofmt(fout, s):
    """Write a string without formatting. Mirrors the C lognofmt macro."""
    if fout is not None:
        fout.write(s)
    else:
        sys.stdout.write(s)


# ============================================================================
# be_builtin_count (from be_solidifylib.c)
# ============================================================================

# #define be_builtin_count(vm)
#     be_vector_count(&(vm)->gbldesc.builtin.vlist)
def _be_builtin_count(vm):
    be_vector = _lazy_be_vector()
    return be_vector.be_vector_count(vm.gbldesc.builtin.vlist)


# ============================================================================
# String-to-identifier encoding (from be_solidifylib.c)
# ============================================================================

# /********************************************************************\
#  * Encode string to identifiers
#  *
#  * `_X` is used as an escape marker
# \********************************************************************/
# static unsigned toidentifier_length(const char *s)
# {
#     unsigned len = 1;
#     const char * p = s;
#     while (*p) {
#         if (p[0] == '_' && p[1] != '\0' && p[1] == 'X') {
#             len += 3;
#             p += 2;
#         } else if (isalnum(p[0]) || p[0] == '_') {
#             p++;
#             len++;
#         } else {        // escape
#             p++;
#             len += 4;
#         }
#     }
#     return len;
# }
def toidentifier_length(s):
    """Compute the length of the identifier-encoded version of string s.
    Returns the length including the null terminator (matching C behavior).
    Operates on raw bytes to match C behavior.
    """
    # Convert to raw bytes using latin-1 (transparent byte mapping used
    # internally by the Python port to represent C byte strings)
    if isinstance(s, str):
        s = s.encode('latin-1')
    length = 1  # for null terminator
    i = 0
    while i < len(s):
        if i + 1 < len(s) and s[i] == ord('_') and s[i + 1] == ord('X'):
            length += 3  # _X -> _X_
            i += 2
        elif (chr(s[i]).isalnum() if s[i] < 128 else False) or s[i] == ord('_'):
            i += 1
            length += 1
        else:  # escape
            i += 1
            length += 4  # _XHH
    return length


# inline static char hexdigit(int v)
# {
#     v = v & 0xF;
#     if (v >= 10)    return v - 10 + 'A';
#     return v + '0';
# }
def _hexdigit(v):
    """Convert a nibble (0-15) to a hex digit character (uppercase)."""
    v = v & 0xF
    if v >= 10:
        return chr(v - 10 + ord('A'))
    return chr(v + ord('0'))


# static void toidentifier(char *to, const char *p)
# {
#     while (*p) {
#         if (p[0] == '_' && p[1] == 'X') {
#             to[0] = '_';
#             to[1] = 'X';
#             to[2] = '_';
#             p += 2;
#             to += 3;
#         } else if (isalnum(p[0]) || p[0] == '_') {
#             *to = *p;
#             to++;
#             p++;
#         } else {        // escape
#             to[0] = '_';
#             to[1] = 'X';
#             to[2] = hexdigit((*p & 0xF0) >> 4);
#             to[3] = hexdigit(*p & 0x0F);
#             p++;
#             to += 4;
#         }
#     }
#     *to = 0;      // final NULL
# }
def toidentifier(s):
    """Encode a string to a C-identifier-safe string using _X escape scheme.
    Operates on raw bytes to match C behavior.
    Returns the encoded string.
    """
    # Convert to raw bytes using latin-1 (transparent byte mapping used
    # internally by the Python port to represent C byte strings)
    if isinstance(s, str):
        s = s.encode('latin-1')
    parts = []
    i = 0
    while i < len(s):
        if i + 1 < len(s) and s[i] == ord('_') and s[i + 1] == ord('X'):
            parts.append('_X_')
            i += 2
        elif (chr(s[i]).isalnum() if s[i] < 128 else False) or s[i] == ord('_'):
            parts.append(chr(s[i]))
            i += 1
        else:  # escape
            b = s[i]
            parts.append('_X')
            parts.append(_hexdigit((b & 0xF0) >> 4))
            parts.append(_hexdigit(b & 0x0F))
            i += 1
    return ''.join(parts)



# ============================================================================
# m_solidify_map (from be_solidifylib.c)
# ============================================================================

# static void m_solidify_map(bvm *vm, bbool str_literal, bmap * map,
#                            const char *prefix_name, void* fout)
# {
#     // compact first
#     be_map_compact(vm, map);
#
#     logfmt("    be_nested_map(%i,\n", map->count);
#     logfmt("    ( (struct bmapnode*) &(const bmapnode[]) {\n");
#     for (int i = 0; i < map->size; i++) {
#         bmapnode * node = &map->slots[i];
#         if (node->key.type == BE_NIL) {
#             continue;   /* key not used */
#         }
#         int key_next = node->key.next;
#         if (0xFFFFFF == key_next) {
#             key_next = -1;      /* more readable */
#         }
#         if (node->key.type == BE_STRING) {
#             const char * key = str(node->key.v.s);
#             size_t id_len = toidentifier_length(key);
#             char id_buf[id_len];
#             toidentifier(id_buf, key);
#             if (!str_literal) {
#                 logfmt("        { be_const_key(%s, %i), ", id_buf, key_next);
#             } else {
#                 logfmt("        { be_const_key_weak(%s, %i), ", id_buf, key_next);
#             }
#             m_solidify_bvalue(vm, str_literal, &node->value, prefix_name,
#                               str(node->key.v.s), fout);
#         } else if (node->key.type == BE_INT) {
#             logfmt("        { be_const_key_int(%lli, %i), ",
#                    node->key.v.i, key_next);
#             m_solidify_bvalue(vm, str_literal, &node->value, prefix_name,
#                               NULL, fout);
#         } else {
#             char error[64];
#             snprintf(error, sizeof(error),
#                      "Unsupported type in key: %i", node->key.type);
#             be_raise(vm, "internal_error", error);
#         }
#         logfmt(" },\n");
#     }
#     logfmt("    }))");
# }
def m_solidify_map(vm, str_literal, map_, prefix_name, fout):
    """Serialize a Berry map to C source using be_nested_map macro format."""
    be_map = _lazy_be_map()
    be_string = _lazy_be_string()
    be_api = _lazy_be_api()

    # compact first
    be_map.be_map_compact(vm, map_)

    _logfmt(fout, "    be_nested_map(%i,\n", map_.count)
    _logfmt(fout, "    ( (struct bmapnode*) &(const bmapnode[]) {\n")

    for i in range(map_.size):
        node = map_.slots[i]
        if node.key.type == BE_NIL:
            continue  # key not used

        key_next = node.key.next
        if key_next == 0xFFFFFF:
            key_next = -1  # more readable

        if node.key.type == BE_STRING:
            # convert the string literal to identifier
            key = be_string.be_str2cstr(node.key.v)
            id_buf = toidentifier(key)
            if not str_literal:
                _logfmt(fout, "        { be_const_key(%s, %i), ", id_buf, key_next)
            else:
                _logfmt(fout, "        { be_const_key_weak(%s, %i), ", id_buf, key_next)
            m_solidify_bvalue(vm, str_literal, node.value, prefix_name,
                              be_string.be_str2cstr(node.key.v), fout)
        elif node.key.type == BE_INT:
            _logfmt(fout, "        { be_const_key_int(%i, %i), ",
                    node.key.v, key_next)
            m_solidify_bvalue(vm, str_literal, node.value, prefix_name,
                              None, fout)
        else:
            error = "Unsupported type in key: %i" % node.key.type
            be_api.be_raise(vm, "internal_error", error)

        _logfmt(fout, " },\n")

    _lognofmt(fout, "    }))")


# ============================================================================
# m_solidify_list (from be_solidifylib.c)
# ============================================================================

# static void m_solidify_list(bvm *vm, bbool str_literal, const blist * list,
#                             const char *prefix_name, void* fout)
# {
#     logfmt("    be_nested_list(%i,\n", list->count);
#     logfmt("    ( (struct bvalue*) &(const bvalue[]) {\n");
#     for (int i = 0; i < list->count; i++) {
#         logfmt("        ");
#         m_solidify_bvalue(vm, str_literal, &list->data[i], prefix_name,
#                           "", fout);
#         logfmt(",\n");
#     }
#     logfmt("    }))");
# }
def m_solidify_list(vm, str_literal, list_, prefix_name, fout):
    """Serialize a Berry list to C source using be_nested_list macro format."""
    _logfmt(fout, "    be_nested_list(%i,\n", list_.count)
    _logfmt(fout, "    ( (struct bvalue*) &(const bvalue[]) {\n")

    for i in range(list_.count):
        _lognofmt(fout, "        ")
        m_solidify_bvalue(vm, str_literal, list_.data[i], prefix_name, "", fout)
        _lognofmt(fout, ",\n")

    _lognofmt(fout, "    }))")



# ============================================================================
# m_solidify_bvalue (from be_solidifylib.c)
# ============================================================================

# // pass key name in case of class, or NULL if none
# static void m_solidify_bvalue(bvm *vm, bbool str_literal,
#                               const bvalue * value,
#                               const char *prefix_name,
#                               const char *key, void* fout)
# {
#     int type = var_primetype(value);
#     switch (type) {
#     case BE_NIL:
#         logfmt("be_const_nil()");
#         break;
#     case BE_BOOL:
#         logfmt("be_const_bool(%i)", var_tobool(value));
#         break;
#     case BE_INT:
#         logfmt("be_const_int(%lli)", var_toint(value));
#         break;
#     case BE_INDEX:
#         logfmt("be_const_var(%lli)", var_toint(value));
#         break;
#     case BE_REAL:
#         logfmt("be_const_real_hex(0x%016" PRIx64 ")",
#                (uint64_t)var_toobj(value));
#         break;
#     case BE_STRING:
#         { ... }
#         break;
#     case BE_CLOSURE:
#         { ... }
#         break;
#     case BE_CLASS:
#         logfmt("be_const_class(be_class_%s)",
#                str(((bclass*) var_toobj(value))->name));
#         break;
#     case BE_COMPTR:
#         logfmt("be_const_comptr(&be_ntv_%s_%s)",
#                prefix_name ? prefix_name : "unknown",
#                key ? key : "unknown");
#         break;
#     case BE_NTVFUNC:
#         logfmt("be_const_%sfunc(be_ntv_%s_%s)",
#                var_isstatic(value) ? "static_" : "",
#                prefix_name ? prefix_name : "unknown",
#                key ? key : "unknown");
#         break;
#     case BE_INSTANCE:
#         { ... }
#         break;
#     case BE_MAP:
#         m_solidify_map(vm, str_literal, (bmap *) var_toobj(value),
#                        prefix_name, fout);
#         break;
#     case BE_LIST:
#         m_solidify_list(vm, str_literal, (blist *) var_toobj(value),
#                         prefix_name, fout);
#         break;
#     default:
#         { ... }
#     }
# }
def m_solidify_bvalue(vm, str_literal, value, prefix_name, key, fout):
    """Serialize any bvalue to C source code."""
    be_string = _lazy_be_string()
    be_api = _lazy_be_api()

    type_ = var_primetype(value)

    if type_ == BE_NIL:
        _lognofmt(fout, "be_const_nil()")

    elif type_ == BE_BOOL:
        _logfmt(fout, "be_const_bool(%i)", 1 if var_tobool(value) else 0)

    elif type_ == BE_INT:
        _logfmt(fout, "be_const_int(%i)", var_toint(value))

    elif type_ == BE_INDEX:
        _logfmt(fout, "be_const_var(%i)", var_toint(value))

    elif type_ == BE_REAL:
        # In C: logfmt("be_const_real_hex(0x%016" PRIx64 ")",
        #              (uint64_t)var_toobj(value));
        # The real value is stored as its raw bit pattern.
        # For double (BE_USE_SINGLE_FLOAT==0): 64-bit hex
        # For float (BE_USE_SINGLE_FLOAT==1): 32-bit hex
        real_val = var_toreal(value)
        if BE_USE_SINGLE_FLOAT:
            # Pack as 32-bit float, interpret as uint32
            raw = struct.unpack('>I', struct.pack('>f', real_val))[0]
            _logfmt(fout, "be_const_real_hex(0x%08X)", raw)
        else:
            # Pack as 64-bit double, interpret as uint64
            raw = struct.unpack('<Q', struct.pack('<d', real_val))[0]
            _logfmt(fout, "be_const_real_hex(0x%016x)", raw)

    elif type_ == BE_STRING:
        s = be_string.be_str2cstr(var_tostr(value))
        slen = len(s)
        id_buf = toidentifier(s)
        if slen >= 255:
            # decompose to avoid any size limit
            _lognofmt(fout, "be_nested_str_long(")
            _lognofmt(fout, id_buf)
            _lognofmt(fout, ")")
        elif not str_literal:
            _logfmt(fout, "be_nested_str(%s)", id_buf)
        else:
            _logfmt(fout, "be_nested_str_weak(%s)", id_buf)

    elif type_ == BE_CLOSURE:
        clo = var_toobj(value)
        func_name = be_string.be_str2cstr(clo.proto.name)
        func_name_id = toidentifier(func_name)
        static_prefix = "static_" if var_isstatic(value) else ""
        prefix = (prefix_name + "_") if prefix_name else ""
        _logfmt(fout, "be_const_%sclosure(%s%s_closure)",
                static_prefix, prefix, func_name_id)

    elif type_ == BE_CLASS:
        cl = var_toobj(value)
        _logfmt(fout, "be_const_class(be_class_%s)",
                be_string.be_str2cstr(cl.name))

    elif type_ == BE_COMPTR:
        _logfmt(fout, "be_const_comptr(&be_ntv_%s_%s)",
                prefix_name if prefix_name else "unknown",
                key if key else "unknown")

    elif type_ == BE_NTVFUNC:
        static_prefix = "static_" if var_isstatic(value) else ""
        _logfmt(fout, "be_const_%sfunc(be_ntv_%s_%s)",
                static_prefix,
                prefix_name if prefix_name else "unknown",
                key if key else "unknown")

    elif type_ == BE_INSTANCE:
        _solidify_instance(vm, str_literal, value, prefix_name, key, fout)

    elif type_ == BE_MAP:
        m_solidify_map(vm, str_literal, var_toobj(value), prefix_name, fout)

    elif type_ == BE_LIST:
        m_solidify_list(vm, str_literal, var_toobj(value), prefix_name, fout)

    else:
        error = "Unsupported type in function constants: %i" % type_
        be_api.be_raise(vm, "internal_error", error)


def _solidify_instance(vm, str_literal, value, prefix_name, key, fout):
    """Handle BE_INSTANCE case for m_solidify_bvalue."""
    be_string = _lazy_be_string()
    be_api = _lazy_be_api()
    be_byteslib = _lazy_be_byteslib()

    ins = var_toobj(value)
    cl = ins._class

    # Check if it's a bytes instance
    # In C: if (cl == &be_class_bytes)
    if be_string.be_str2cstr(cl.name) == "bytes":
        bufptr = var_toobj(ins.members[0])
        length = var_toint(ins.members[1])
        if bufptr is not None and length > 0:
            hex_out = be_byteslib.be_bytes_tohex(bufptr, length)
        else:
            hex_out = ""
        _lognofmt(fout, "be_const_bytes_instance(")
        _lognofmt(fout, hex_out)
        _lognofmt(fout, ")")
    elif ins.super is not None or ins.sub is not None:
        be_api.be_raise(vm, "internal_error",
                        "instance must not have a super/sub class")
    else:
        cl_name = be_string.be_str2cstr(cl.name)
        if cl_name == "map":
            cl_ptr = "map"
        elif cl_name == "list":
            cl_ptr = "list"
        else:
            be_api.be_raise(vm, "internal_error", "unsupported class")
            return

        _logfmt(fout,
                "be_const_simple_instance(be_nested_simple_instance("
                "&be_class_%s, {\n", cl_ptr)
        if cl_ptr == "map":
            _lognofmt(fout, "        be_const_map( * ")
        else:
            _lognofmt(fout, "        be_const_list( * ")
        m_solidify_bvalue(vm, str_literal, ins.members[0],
                          prefix_name, key, fout)
        _lognofmt(fout, "    ) } ))")



# ============================================================================
# m_solidify_closure_inner_class (from be_solidifylib.c)
# ============================================================================

# /* solidify any inner class */
# static void m_solidify_closure_inner_class(bvm *vm, bbool str_literal,
#                                            const bclosure *clo, void* fout)
# {
#     // parse any class in constants to output it first
#     bproto *pr = clo->proto;
#     if ((!gc_isconst(clo)) && (pr->nconst > 0) &&
#         (!(pr->varg & BE_VA_SHARED_KTAB)) &&
#         (!(pr->varg & BE_VA_NOCOMPACT))) {
#         for (int k = 0; k < pr->nconst; k++) {
#             if (var_type(&pr->ktab[k]) == BE_CLASS) {
#                 if ((k == 0) && (pr->varg & BE_VA_STATICMETHOD)) {
#                     // it is the implicit '_class' variable from a static
#                     // method, don't dump the class
#                 } else {
#                     // output the class
#                     m_solidify_subclass(vm, str_literal,
#                         (bclass*) var_toobj(&pr->ktab[k]), fout);
#                 }
#             }
#         }
#     }
# }
def m_solidify_closure_inner_class(vm, str_literal, clo, fout):
    """Pre-declare and solidify any inner classes found in closure constants."""
    pr = clo.proto
    if ((not gc_isconst(clo)) and (pr.nconst > 0) and
            (not (pr.varg & BE_VA_SHARED_KTAB)) and
            (not (pr.varg & BE_VA_NOCOMPACT))):
        for k in range(pr.nconst):
            if var_type(pr.ktab[k]) == BE_CLASS:
                if (k == 0) and (pr.varg & BE_VA_STATICMETHOD):
                    # implicit '_class' variable from a static method — skip
                    pass
                else:
                    # output the class
                    m_solidify_subclass(vm, str_literal,
                                        var_toobj(pr.ktab[k]), fout)


# ============================================================================
# m_solidify_proto (from be_solidifylib.c)
# ============================================================================

# static void m_solidify_proto(bvm *vm, bbool str_literal, const bproto *pr,
#                              const char * func_name, int indent,
#                              const char * prefix_name, void* fout)
# {
#     logfmt("%*sbe_nested_proto(\n", indent, "");
#     indent += 2;
#     logfmt("%*s%d,                          /* nstack */\n",
#            indent, "", pr->nstack);
#     logfmt("%*s%d,                          /* argc */\n",
#            indent, "", pr->argc);
#     logfmt("%*s%d,                          /* varg */\n",
#            indent, "", pr->varg);
#     logfmt("%*s%d,                          /* has upvals */\n",
#            indent, "", (pr->nupvals > 0) ? 1 : 0);
#     ... (upvals, sub protos, constants, name, source, code) ...
# }
def m_solidify_proto(vm, str_literal, pr, func_name, indent, prefix_name, fout):
    """Serialize a function prototype to C source using be_nested_proto format."""
    be_string = _lazy_be_string()
    be_debug = _lazy_be_debug()
    be_api = _lazy_be_api()

    _logfmt(fout, "%*sbe_nested_proto(\n", indent, "")
    indent += 2

    _logfmt(fout, "%*s%d,                          /* nstack */\n",
            indent, "", pr.nstack)
    _logfmt(fout, "%*s%d,                          /* argc */\n",
            indent, "", pr.argc)
    _logfmt(fout, "%*s%d,                          /* varg */\n",
            indent, "", pr.varg)
    _logfmt(fout, "%*s%d,                          /* has upvals */\n",
            indent, "", 1 if (pr.nupvals > 0) else 0)

    # upvals
    if pr.nupvals > 0:
        _logfmt(fout, "%*s( &(const bupvaldesc[%2d]) {  /* upvals */\n",
                indent, "", pr.nupvals)
        for i in range(pr.nupvals):
            _logfmt(fout, "%*s  be_local_const_upval(%i, %i),\n",
                    indent, "", pr.upvals[i].instack, pr.upvals[i].idx)
        _logfmt(fout, "%*s}),\n", indent, "")
    else:
        _logfmt(fout, "%*sNULL,                       /* no upvals */\n",
                indent, "")

    # sub protos
    _logfmt(fout, "%*s%d,                          /* has sup protos */\n",
            indent, "", 1 if (pr.nproto > 0) else 0)
    if pr.nproto > 0:
        _logfmt(fout, "%*s( &(const struct bproto*[%2d]) {\n",
                indent, "", pr.nproto)
        for i in range(pr.nproto):
            sub_name = "%s_%d" % (func_name, i)
            m_solidify_proto(vm, str_literal, pr.ptab[i], sub_name,
                             indent + 2, prefix_name, fout)
            _lognofmt(fout, ",\n")
        _logfmt(fout, "%*s}),\n", indent, "")
    else:
        _logfmt(fout, "%*sNULL,                       /* no sub protos */\n",
                indent, "")

    # constants
    _logfmt(fout, "%*s%d,                          /* has constants */\n",
            indent, "", 1 if (pr.nconst > 0) else 0)
    if pr.nconst > 0:
        if pr.varg & BE_VA_SHARED_KTAB:
            _logfmt(fout, "%*s&be_ktab_%s,     /* shared constants */\n",
                    indent, "", prefix_name)
        else:
            _logfmt(fout, "%*s( &(const bvalue[%2d]) {     /* constants */\n",
                    indent, "", pr.nconst)
            for k in range(pr.nconst):
                _logfmt(fout, "%*s/* K%-3d */  ", indent, "", k)
                m_solidify_bvalue(vm, str_literal, pr.ktab[k],
                                  None, None, fout)
                _lognofmt(fout, ",\n")
            _logfmt(fout, "%*s}),\n", indent, "")
    else:
        _logfmt(fout, "%*sNULL,                       /* no const */\n",
                indent, "")

    # function name
    name_str = be_string.be_str2cstr(pr.name)
    id_buf = toidentifier(name_str)
    if not str_literal:
        _logfmt(fout, "%*s&be_const_str_%s,\n", indent, "", id_buf)
    else:
        _logfmt(fout, "%*sbe_str_weak(%s),\n", indent, "", id_buf)

    # source — hard-code as "solidified"
    _logfmt(fout, "%*s&be_const_str_solidified,\n", indent, "")

    # code
    _logfmt(fout, "%*s( &(const binstruction[%2d]) {  /* code */\n",
            indent, "", pr.codesize)
    for pc in range(pr.codesize):
        ins = pr.code[pc]
        _logfmt(fout, "%*s  0x%08X,  //", indent, "", ins & 0xFFFFFFFF)
        be_debug.be_print_inst(ins, pc, fout)
        op = IGET_OP(ins)
        if op == OP_GETGBL or op == OP_SETGBL:
            # check if the global is in built-ins
            glb = IGET_Bx(ins)
            if glb > _be_builtin_count(vm):
                _logfmt(fout, "\n===== unsupported global G%d\n", glb)
                be_api.be_raise(vm, "internal_error",
                                "Unsupported access to non-builtin global")
    _logfmt(fout, "%*s})\n", indent, "")

    indent -= 2
    _logfmt(fout, "%*s)", indent, "")



# ============================================================================
# m_solidify_closure (from be_solidifylib.c)
# ============================================================================

# static void m_solidify_closure(bvm *vm, bbool str_literal,
#                                const bclosure *clo,
#                                const char * prefix_name, void* fout)
# {
#     bproto *pr = clo->proto;
#     const char * func_name = str(pr->name);
#     if (clo->nupvals > 0) {
#         const char *name = str(clo->proto->name);
#         logfmt("--> Unsupported upvals in closure in '%s' <---",
#                name ? name : "<unkown>");
#     }
#     int indent = 2;
#     m_solidify_closure_inner_class(vm, str_literal, clo, fout);
#     logfmt("\n");
#     logfmt("/***************************************************...\n");
#     logfmt("** Solidified function: %s\n", func_name);
#     logfmt("***************************************************...\n");
#     {
#         size_t id_len = toidentifier_length(func_name);
#         char func_name_id[id_len];
#         toidentifier(func_name_id, func_name);
#         logfmt("be_local_closure(%s%s%s,   /* name */\n",
#                prefix_name ? prefix_name : "",
#                prefix_name ? "_" : "",
#                func_name_id);
#     }
#     m_solidify_proto(vm, str_literal, pr, func_name, indent,
#                      prefix_name, fout);
#     logfmt("\n");
#     logfmt(");\n");
#     logfmt("/***************************************************...\n\n");
# }
def m_solidify_closure(vm, str_literal, clo, prefix_name, fout):
    """Serialize a closure to C source using be_local_closure macro format."""
    be_string = _lazy_be_string()

    pr = clo.proto
    func_name = be_string.be_str2cstr(pr.name)

    if clo.nupvals > 0:
        name = be_string.be_str2cstr(clo.proto.name) if clo.proto.name else "<unkown>"
        _logfmt(fout, "--> Unsupported upvals in closure in '%s' <---", name)

    indent = 2

    m_solidify_closure_inner_class(vm, str_literal, clo, fout)

    _lognofmt(fout, "\n")
    _lognofmt(fout, "/********************************************************************\n")
    _logfmt(fout, "** Solidified function: %s\n", func_name)
    _lognofmt(fout, "********************************************************************/\n")

    func_name_id = toidentifier(func_name)
    prefix = prefix_name if prefix_name else ""
    sep = "_" if prefix_name else ""
    _logfmt(fout, "be_local_closure(%s%s%s,   /* name */\n",
            prefix, sep, func_name_id)

    m_solidify_proto(vm, str_literal, pr, func_name, indent, prefix_name, fout)
    _lognofmt(fout, "\n")

    # closure
    _lognofmt(fout, ");\n")
    _lognofmt(fout, "/*******************************************************************/\n\n")


# ============================================================================
# m_solidify_subclass (from be_solidifylib.c)
# ============================================================================

# static void m_solidify_subclass(bvm *vm, bbool str_literal,
#                                 const bclass *cla, void* fout)
# {
#     const char * classname = str(cla->name);
#     m_compact_class(vm, str_literal, cla, fout);
#     char prefix_name[strlen(classname) + 10];
#     snprintf(prefix_name, sizeof(prefix_name), "class_%s", classname);
#     logfmt("\nextern const bclass be_class_%s;\n", classname);
#     if (cla->members) {
#         bmapnode *node;
#         bmapiter iter = be_map_iter();
#         while ((node = be_map_next(cla->members, &iter)) != NULL) {
#             if (var_isstr(&node->key) && var_isclosure(&node->value)) {
#                 bclosure *f = var_toobj(&node->value);
#                 m_solidify_closure(vm, str_literal, f, prefix_name, fout);
#             }
#         }
#     }
#     logfmt("\n");
#     logfmt("/***************************************************...\n");
#     logfmt("** Solidified class: %s\n", classname);
#     logfmt("***************************************************...\n");
#     if (cla->super) {
#         logfmt("extern const bclass be_class_%s;\n",
#                str(cla->super->name));
#     }
#     logfmt("be_local_class(%s,\n", classname);
#     logfmt("    %i,\n", cla->nvar);
#     if (cla->super) {
#         logfmt("    &be_class_%s,\n", str(cla->super->name));
#     } else {
#         logfmt("    NULL,\n");
#     }
#     if (cla->members) {
#         m_solidify_map(vm, str_literal, cla->members, prefix_name, fout);
#         logfmt(",\n");
#     } else {
#         logfmt("    NULL,\n");
#     }
#     ... name ...
#     logfmt(");\n");
# }
def m_solidify_subclass(vm, str_literal, cla, fout):
    """Serialize a class (and its methods) to C source."""
    be_string = _lazy_be_string()
    be_map = _lazy_be_map()

    classname = be_string.be_str2cstr(cla.name)

    # compact class constant tables
    m_compact_class(vm, str_literal, cla, fout)

    prefix_name = "class_%s" % classname

    # pre-declare class to support '_class' implicit variable
    _logfmt(fout, "\nextern const bclass be_class_%s;\n", classname)

    # iterate on members to dump closures
    if cla.members is not None:
        it = [None]
        node, it = be_map.be_map_next(cla.members, it)
        while node is not None:
            if var_isstr(node.key) and var_isclosure(node.value):
                f = var_toobj(node.value)
                m_solidify_closure(vm, str_literal, f, prefix_name, fout)
            node, it = be_map.be_map_next(cla.members, it)

    _lognofmt(fout, "\n")
    _lognofmt(fout, "/********************************************************************\n")
    _logfmt(fout, "** Solidified class: %s\n", classname)
    _lognofmt(fout, "********************************************************************/\n")

    if cla.super is not None:
        _logfmt(fout, "extern const bclass be_class_%s;\n",
                be_string.be_str2cstr(cla.super.name))

    _logfmt(fout, "be_local_class(%s,\n", classname)
    _logfmt(fout, "    %i,\n", cla.nvar)

    if cla.super is not None:
        _logfmt(fout, "    &be_class_%s,\n",
                be_string.be_str2cstr(cla.super.name))
    else:
        _lognofmt(fout, "    NULL,\n")

    if cla.members is not None:
        m_solidify_map(vm, str_literal, cla.members, prefix_name, fout)
        _lognofmt(fout, ",\n")
    else:
        _lognofmt(fout, "    NULL,\n")

    id_buf = toidentifier(classname)
    if not str_literal:
        _logfmt(fout, "    (bstring*) &be_const_str_%s\n", id_buf)
    else:
        _logfmt(fout, "    be_str_weak(%s)\n", id_buf)

    _lognofmt(fout, ");\n")


# ============================================================================
# m_solidify_class (from be_solidifylib.c)
# ============================================================================

# static void m_solidify_class(bvm *vm, bbool str_literal, bclass *cl,
#                              void* fout)
# {
#     m_solidify_subclass(vm, str_literal, cl, fout);
# }
def m_solidify_class(vm, str_literal, cl, fout):
    """Serialize a top-level class to C source."""
    m_solidify_subclass(vm, str_literal, cl, fout)



# ============================================================================
# m_solidify_module (from be_solidifylib.c)
# ============================================================================

# static void m_solidify_module(bvm *vm, bbool str_literal, bmodule *ml,
#                               void* fout)
# {
#     const char * modulename = be_module_name(ml);
#     if (!modulename) { modulename = ""; }
#     if (ml->table) {
#         bmapnode *node;
#         bmapiter iter = be_map_iter();
#         while ((node = be_map_next(ml->table, &iter)) != NULL) {
#             if (var_isstr(&node->key) && var_isclosure(&node->value)) {
#                 bclosure *f = var_toobj(&node->value);
#                 m_solidify_closure(vm, str_literal, f, NULL, fout);
#             }
#             if (var_isstr(&node->key) && var_isclass(&node->value)) {
#                 bclass *cl = var_toobj(&node->value);
#                 m_solidify_subclass(vm, str_literal, cl, fout);
#             }
#         }
#     }
#     logfmt("\n");
#     logfmt("/***************************************************...\n");
#     logfmt("** Solidified module: %s\n", modulename);
#     logfmt("***************************************************...\n");
#     logfmt("be_local_module(%s,\n", modulename);
#     logfmt("    \"%s\",\n", modulename);
#     if (ml->table) {
#         m_solidify_map(vm, str_literal, ml->table, NULL, fout);
#         logfmt("\n");
#     } else {
#         logfmt("    NULL,\n");
#     }
#     logfmt(");\n");
#     logfmt("BE_EXPORT_VARIABLE be_define_const_native_module(%s);\n",
#            modulename);
#     logfmt("/***************************************************...\n");
# }
def m_solidify_module(vm, str_literal, ml, fout):
    """Serialize a module to C source using be_local_module macro format."""
    be_module = _lazy_be_module()
    be_map = _lazy_be_map()

    modulename = be_module.be_module_name(ml)
    if modulename is None:
        modulename = ""

    # iterate on members to dump closures and classes
    if ml.table is not None:
        it = [None]
        node, it = be_map.be_map_next(ml.table, it)
        while node is not None:
            if var_isstr(node.key) and var_isclosure(node.value):
                f = var_toobj(node.value)
                m_solidify_closure(vm, str_literal, f, None, fout)
            if var_isstr(node.key) and var_isclass(node.value):
                cl = var_toobj(node.value)
                m_solidify_subclass(vm, str_literal, cl, fout)
            node, it = be_map.be_map_next(ml.table, it)

    _lognofmt(fout, "\n")
    _lognofmt(fout, "/********************************************************************\n")
    _logfmt(fout, "** Solidified module: %s\n", modulename)
    _lognofmt(fout, "********************************************************************/\n")

    _logfmt(fout, "be_local_module(%s,\n", modulename)
    _logfmt(fout, "    \"%s\",\n", modulename)

    if ml.table is not None:
        m_solidify_map(vm, str_literal, ml.table, None, fout)
        _lognofmt(fout, "\n")
    else:
        _lognofmt(fout, "    NULL,\n")

    _lognofmt(fout, ");\n")
    _logfmt(fout, "BE_EXPORT_VARIABLE be_define_const_native_module(%s);\n",
            modulename)
    _lognofmt(fout, "/********************************************************************/\n")



# ============================================================================
# m_compact_class (from be_solidifylib.c)
# ============================================================================

# static void m_compact_class(bvm *vm, bbool str_literal, const bclass *cla,
#                             void* fout)
# {
#     const char * classname = str(cla->name);
#     const int MAX_KTAB_SIZE = 256;
#     bvalue ktab[MAX_KTAB_SIZE];
#     int ktab_size = 0;
#     int ktab_total = 0;
#
#     if (cla->members) {
#         bmapnode *node;
#         bmapiter iter = be_map_iter();
#         /* first iteration to build the global ktab */
#         while ((node = be_map_next(cla->members, &iter)) != NULL) {
#             if (var_isstr(&node->key) && var_isclosure(&node->value)) {
#                 bclosure *cl = var_toobj(&node->value);
#                 bproto *pr = cl->proto;
#                 if ((gc_isconst(cl)) || (pr->varg & BE_VA_SHARED_KTAB) ||
#                     (pr->varg & BE_VA_NOCOMPACT)) { continue; }
#                 for (int i = 0; i < pr->nconst; i++) {
#                     int found = 0;
#                     for (int j = 0; j < ktab_size; j++) {
#                         if ((pr->ktab[i].type == ktab[j].type) &&
#                             (pr->ktab[i].v.i == ktab[j].v.i) &&
#                             (pr->ktab[i].v.c == ktab[j].v.c)) {
#                             found = 1;
#                             break;
#                         }
#                     }
#                     if (!found) {
#                         ktab[ktab_size++] = pr->ktab[i];
#                     }
#                     if (ktab_size >= MAX_KTAB_SIZE) {
#                         logfmt("// ktab too big for class '%s' - skipping\n",
#                                classname);
#                         return;
#                     }
#                 }
#                 ktab_total += pr->nconst;
#             }
#         }
#         if (ktab_size == ktab_total) {
#             return;  /* nothing to optimize */
#         }
#         /* allocate a proper ktab */
#         bvalue *new_ktab = be_malloc(vm, sizeof(bvalue) * ktab_size);
#         memmove(new_ktab, ktab, sizeof(bvalue) * ktab_size);
#         /* second iteration to replace ktab and patch code */
#         iter = be_map_iter();
#         while ((node = be_map_next(cla->members, &iter)) != NULL) {
#             ... (patch instructions) ...
#         }
#     }
#     ... (output extern statements, sub-classes, shared ktab) ...
# }
def m_compact_class(vm, str_literal, cla, fout):
    """Merge per-method constant tables into a shared ktab for a class.

    Builds a unified constant table, patches instruction indices in each
    method's bytecode, and sets the BE_VA_SHARED_KTAB flag.
    """
    be_string = _lazy_be_string()
    be_map = _lazy_be_map()

    classname = be_string.be_str2cstr(cla.name)

    MAX_KTAB_SIZE = 256
    ktab = []  # list of bvalue
    ktab_size = 0
    ktab_total = 0

    if cla.members is not None:
        # first iteration to build the global ktab
        it = [None]
        node, it = be_map.be_map_next(cla.members, it)
        while node is not None:
            if var_isstr(node.key) and var_isclosure(node.value):
                cl = var_toobj(node.value)
                pr = cl.proto

                if (gc_isconst(cl) or (pr.varg & BE_VA_SHARED_KTAB) or
                        (pr.varg & BE_VA_NOCOMPACT)):
                    node, it = be_map.be_map_next(cla.members, it)
                    continue

                # iterate on each bvalue in ktab
                for i in range(pr.nconst):
                    # look if the bvalue pair is already in ktab
                    found = False
                    for j in range(ktab_size):
                        if _bvalue_eq(pr.ktab[i], ktab[j]):
                            found = True
                            break
                    # if not already there, add it
                    if not found:
                        ktab.append(pr.ktab[i])
                        ktab_size += 1
                    if ktab_size >= MAX_KTAB_SIZE:
                        _logfmt(fout,
                                "// ktab too big for class '%s' - skipping\n",
                                classname)
                        return
                ktab_total += pr.nconst

            node, it = be_map.be_map_next(cla.members, it)

        if ktab_size == ktab_total:
            return  # nothing to optimize, 0 or 1 method

        # allocate a proper ktab (in Python, just use the list)
        new_ktab = list(ktab)

        # second iteration to replace ktab and patch code
        it = [None]
        node, it = be_map.be_map_next(cla.members, it)
        while node is not None:
            if var_isstr(node.key) and var_isclosure(node.value):
                cl = var_toobj(node.value)
                pr = cl.proto

                if (gc_isconst(cl) or (pr.varg & BE_VA_SHARED_KTAB) or
                        (pr.varg & BE_VA_NOCOMPACT)):
                    node, it = be_map.be_map_next(cla.members, it)
                    continue

                # build mapping array: old index -> new index
                mapping_array = [0] * MAX_KTAB_SIZE
                for i in range(pr.nconst):
                    for j in range(ktab_size):
                        if _bvalue_eq(pr.ktab[i], ktab[j]):
                            mapping_array[i] = j
                            break

                # replace ktab
                pr.ktab = new_ktab
                pr.nconst = ktab_size
                # flag as shared ktab
                pr.varg |= BE_VA_SHARED_KTAB

                # parse code to replace any K<x> reference
                _patch_code(pr, mapping_array, ktab_size, vm)

            node, it = be_map.be_map_next(cla.members, it)

    # scan classes and generate extern statements
    for k in range(ktab_size):
        if var_isclass(ktab[k]):
            cl = var_toobj(ktab[k])
            _logfmt(fout, "extern const bclass be_class_%s;\n",
                    be_string.be_str2cstr(cl.name))

    # scan again to export all sub-classes
    for k in range(ktab_size):
        if var_isclass(ktab[k]):
            cl = var_toobj(ktab[k])
            if cl is not cla:
                m_solidify_subclass(vm, str_literal, cl, fout)

    # output shared ktab
    indent = 0
    _logfmt(fout,
            "// compact class '%s' ktab size: %d, total: %d (saved %i bytes)\n",
            classname, ktab_size, ktab_total,
            (ktab_total - ktab_size) * 8)
    _logfmt(fout, "static const bvalue be_ktab_class_%s[%i] = {\n",
            classname, ktab_size)
    for k in range(ktab_size):
        _logfmt(fout, "%*s/* K%-3d */  ", indent + 2, "", k)
        m_solidify_bvalue(vm, str_literal, ktab[k], None, None, fout)
        _lognofmt(fout, ",\n")
    _logfmt(fout, "%*s};\n", indent, "")
    _lognofmt(fout, "\n")



# ============================================================================
# Helper: bvalue equality for ktab deduplication
# ============================================================================

def _bvalue_eq(a, b):
    """Compare two bvalues for equality (type + value).
    Mirrors the C comparison: type, v.i, and v.c fields.
    In Python, we compare type and the v field directly.
    For strings, we must compare the actual string content because
    long strings (>64 chars) are not interned and may be distinct objects
    with identical content.
    """
    if a.type != b.type:
        return False
    # For strings, compare actual content (long strings are not interned)
    if var_isstr(a):
        be_string = _lazy_be_string()
        return be_string.be_str2cstr(a.v) == be_string.be_str2cstr(b.v)
    # For most types, comparing v directly works
    # For objects (closures, classes, etc.), identity comparison is correct
    return a.v == b.v


# ============================================================================
# Helper: patch bytecode instructions for shared ktab
# ============================================================================

# Extracted from m_compact_class second iteration inner loop
def _patch_code(pr, mapping_array, ktab_size, vm):
    """Patch instruction constant indices to reference the shared ktab."""
    be_api = _lazy_be_api()

    for pc in range(pr.codesize):
        ins = pr.code[pc]
        op = IGET_OP(ins)

        if op in (OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD,
                  OP_LT, OP_LE, OP_EQ, OP_NE, OP_GT, OP_GE,
                  OP_CONNECT, OP_GETMBR, OP_SETMBR, OP_GETMET,
                  OP_GETIDX, OP_SETIDX, OP_AND, OP_OR, OP_XOR,
                  OP_SHL, OP_SHR, OP_RAISE):
            # B and C might contain 'K' constant
            if isKB(ins):
                kidx = IGET_RKB(ins) & KR_MASK
                if kidx >= ktab_size:
                    be_api.be_raise(vm, "value_error",
                                    "invalid ktab index")
                ins = (ins & ~IRKB_MASK) | ISET_RKB(setK(mapping_array[kidx]))
            if isKC(ins):
                kidx = IGET_RKC(ins) & KR_MASK
                if kidx >= ktab_size:
                    be_api.be_raise(vm, "value_error",
                                    "invalid ktab index")
                ins = (ins & ~IRKC_MASK) | ISET_RKC(setK(mapping_array[kidx]))
            pr.code[pc] = ins

        elif op in (OP_MOVE, OP_SETSUPER, OP_NEG, OP_FLIP, OP_IMPORT,
                    OP_GETNGBL, OP_SETNGBL):
            # Only B might contain 'K' constant
            if isKB(ins):
                kidx = IGET_RKB(ins) & KR_MASK
                if kidx >= ktab_size:
                    be_api.be_raise(vm, "value_error",
                                    "invalid ktab index")
                ins = (ins & ~IRKB_MASK) | ISET_RKB(setK(mapping_array[kidx]))
            pr.code[pc] = ins

        elif op in (OP_CLASS, OP_LDCONST):
            # Bx contains the K
            kidx = IGET_Bx(ins)
            if kidx >= ktab_size:
                be_api.be_raise(vm, "value_error",
                                "invalid ktab index")
            ins = (ins & ~IBx_MASK) | ISET_Bx(mapping_array[kidx])
            pr.code[pc] = ins

        elif op == OP_RET:
            if IGET_RA(ins):
                # Only B might contain 'K' constant
                if isKB(ins):
                    kidx = IGET_RKB(ins) & KR_MASK
                    if kidx >= ktab_size:
                        be_api.be_raise(vm, "value_error",
                                        "invalid ktab index")
                    ins = (ins & ~IRKB_MASK) | ISET_RKB(
                        setK(mapping_array[kidx]))
            pr.code[pc] = ins

        # default: no patching needed for other opcodes


# ============================================================================
# m_dump — entry point (from be_solidifylib.c)
# ============================================================================

# static int m_dump(bvm *vm)
# {
#     int top = be_top(vm);
#     if (top >= 1) {
#         bvalue *v = be_indexof(vm, 1);
#         bbool str_literal = bfalse;
#         if (top >= 2) {
#             str_literal = be_tobool(vm, 2);
#         }
#         void* fout = NULL;
#         if (top >= 3 && be_isinstance(vm, 3)) {
#             be_getmember(vm, 3, ".p");
#             if (be_iscomptr(vm, -1)) {
#                 fout = be_tocomptr(vm, -1);
#             }
#             be_pop(vm, 1);
#         }
#         const char *prefix_name = NULL;
#         if (top >= 4 && be_isstring(vm, 4)) {
#             prefix_name = be_tostring(vm, 4);
#         }
#         if (var_isclosure(v)) {
#             m_solidify_closure(vm, str_literal, var_toobj(v),
#                                prefix_name, fout);
#         } else if (var_isclass(v)) {
#             m_solidify_class(vm, str_literal, var_toobj(v), fout);
#         } else if (var_ismodule(v)) {
#             m_solidify_module(vm, str_literal, var_toobj(v), fout);
#         } else {
#             be_raise(vm, "value_error", "unsupported type");
#         }
#     }
#     be_return_nil(vm);
# }
def m_dump(vm):
    """Entry point for solidify.dump().

    Arguments on Berry stack:
      1: closure, class, or module to solidify
      2: (optional) str_literal mode (bool)
      3: (optional) file object for output
      4: (optional) explicit prefix name (string)
    """
    be_api = _lazy_be_api()

    top = be_api.be_top(vm)
    if top >= 1:
        v_idx = be_api.be_indexof(vm, 1)
        v = vm.stack[v_idx]
        str_literal = False
        if top >= 2:
            str_literal = be_api.be_tobool(vm, 2)

        fout = None
        if top >= 3 and be_api.be_isinstance(vm, 3):
            be_api.be_getmember(vm, 3, ".p")
            if be_api.be_iscomptr(vm, -1):
                fout = be_api.be_tocomptr(vm, -1)
            be_api.be_pop(vm, 1)

        prefix_name = None
        if top >= 4 and be_api.be_isstring(vm, 4):
            prefix_name = be_api.be_tostring(vm, 4)

        if var_isclosure(v):
            m_solidify_closure(vm, str_literal, var_toobj(v),
                               prefix_name, fout)
        elif var_isclass(v):
            m_solidify_class(vm, str_literal, var_toobj(v), fout)
        elif var_ismodule(v):
            m_solidify_module(vm, str_literal, var_toobj(v), fout)
        else:
            be_api.be_raise(vm, "value_error", "unsupported type")

    be_api.be_returnnilvalue(vm)


# ============================================================================
# m_compact — entry point (from be_solidifylib.c)
# ============================================================================

# static int m_compact(bvm *vm)
# {
#     int top = be_top(vm);
#     if (top >= 1) {
#         bvalue *v = be_indexof(vm, 1);
#         bbool str_literal = bfalse;
#         if (top >= 2) {
#             str_literal = be_tobool(vm, 2);
#         }
#         void* fout = NULL;
#         if (top >= 3 && be_isinstance(vm, 3)) {
#             be_getmember(vm, 3, ".p");
#             if (be_iscomptr(vm, -1)) {
#                 fout = be_tocomptr(vm, -1);
#             }
#             be_pop(vm, 1);
#         }
#         if (var_isclass(v)) {
#             m_compact_class(vm, str_literal, var_toobj(v), fout);
#         } else if (var_ismodule(v)) {
#             // TODO
#         } else {
#             be_raise(vm, "value_error", "unsupported type");
#         }
#     }
#     be_return_nil(vm);
# }
def m_compact(vm):
    """Entry point for solidify.compact().

    Arguments on Berry stack:
      1: class or module to compact
      2: (optional) str_literal mode (bool)
      3: (optional) file object for output
    """
    be_api = _lazy_be_api()

    top = be_api.be_top(vm)
    if top >= 1:
        v_idx = be_api.be_indexof(vm, 1)
        v = vm.stack[v_idx]
        str_literal = False
        if top >= 2:
            str_literal = be_api.be_tobool(vm, 2)

        fout = None
        if top >= 3 and be_api.be_isinstance(vm, 3):
            be_api.be_getmember(vm, 3, ".p")
            if be_api.be_iscomptr(vm, -1):
                fout = be_api.be_tocomptr(vm, -1)
            be_api.be_pop(vm, 1)

        if var_isclass(v):
            m_compact_class(vm, str_literal, var_toobj(v), fout)
        elif var_ismodule(v):
            pass  # TODO
        else:
            be_api.be_raise(vm, "value_error", "unsupported type")

    be_api.be_returnnilvalue(vm)


# ============================================================================
# m_nocompact (from be_solidifylib.c)
# ============================================================================

# static int m_nocompact(bvm *vm)
# {
#     int top = be_top(vm);
#     if (top >= 1) {
#         bvalue *v = be_indexof(vm, 1);
#         if (var_isclosure(v)) {
#             bclosure *cl = var_toobj(v);
#             bproto *pr = cl->proto;
#             pr->varg |= BE_VA_NOCOMPACT;
#         } else {
#             be_raise(vm, "value_error", "unsupported type");
#         }
#     }
#     be_return_nil(vm);
# }
def m_nocompact(vm):
    """Entry point for solidify.nocompact().

    Marks a closure's proto as non-compactable.
    """
    be_api = _lazy_be_api()

    top = be_api.be_top(vm)
    if top >= 1:
        v_idx = be_api.be_indexof(vm, 1)
        v = vm.stack[v_idx]
        if var_isclosure(v):
            cl = var_toobj(v)
            pr = cl.proto
            pr.varg |= BE_VA_NOCOMPACT
        else:
            be_api.be_raise(vm, "value_error", "unsupported type")

    be_api.be_returnnilvalue(vm)
