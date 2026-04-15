"""Berry bytecode save/load — Python port of be_bytecode.c.

Implements bytecode file format serialization and deserialization:
- Header: magic bytes 0xBE 0xCD 0xFE, version=4, sizeinfo
- Global info: builtin count, global count, global names
- Proto serialization: recursive save/load of function prototypes

Original C code is included as comments throughout.
"""

# /********************************************************************
# ** Copyright (c) 2018-2020 Guan Wenliang
# ** This file is part of the Berry default interpreter.
# ** skiars@qq.com, https://github.com/Skiars/berry
# ** See Copyright Notice in the LICENSE file or at
# ** https://github.com/Skiars/berry/blob/master/LICENSE
# ********************************************************************/

import struct

from berry_port.berry_conf import (
    BE_INTGER_TYPE,
    BE_USE_SINGLE_FLOAT,
    BE_DEBUG_SOURCE_FILE,
    BE_USE_BYTECODE_SAVER,
    BE_USE_BYTECODE_LOADER,
    BE_USE_SCRIPT_COMPILER,
)
from berry_port.be_object import (
    BE_NIL, BE_INT, BE_REAL, BE_BOOL, BE_STRING, BE_CLASS,
    BE_PROTO, BE_CLOSURE, BE_INDEX,
    BE_VA_METHOD, BE_VA_STATICMETHOD,
    bvalue, bproto, bclosure, bupvaldesc,
    var_type, var_primetype, var_basetype,
    var_isnil, var_isstr, var_isindex, var_isclosure, var_isproto, var_isclass,
    var_toint, var_toreal, var_tostr, var_toobj, var_toidx,
    var_setnil, var_setint, var_setreal, var_setstr, var_setclass,
    var_setclosure, var_setproto, var_setindex,
    be_map_iter, be_map_count, be_list_at,
    str_len,
)
from berry_port.be_decoder import (
    IGET_OP, IGET_Bx, ISET_Bx, IBx_MASK,
    OP_GETGBL, OP_SETGBL,
)

# #define MAGIC_NUMBER1       0xBE
# #define MAGIC_NUMBER2       0xCD
# #define MAGIC_NUMBER3       0xFE
# #define BYTECODE_VERSION    4
MAGIC_NUMBER1 = 0xBE
MAGIC_NUMBER2 = 0xCD
MAGIC_NUMBER3 = 0xFE
BYTECODE_VERSION = 4

# #define USE_64BIT_INT       (BE_INTGER_TYPE == 2 ...)
USE_64BIT_INT = (BE_INTGER_TYPE == 2)


# ============================================================================
# Lazy imports to avoid circular dependencies
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

def _lazy_be_exec():
    import berry_port.be_exec as m
    return m

def _lazy_be_var():
    import berry_port.be_var as m
    return m

def _lazy_be_map():
    import berry_port.be_map as m
    return m

def _lazy_be_list():
    import berry_port.be_list as m
    return m

def _lazy_be_mem():
    import berry_port.be_mem as m
    return m

def _lazy_be_vm():
    import berry_port.be_vm as m
    return m


# ============================================================================
# Helper: bytecode_error
# ============================================================================

# static void bytecode_error(bvm *vm, const char *msg)
# {
#     be_raise(vm, "io_error", msg);
# }
def bytecode_error(vm, msg):
    be_vm = _lazy_be_vm()
    be_vm.be_raise(vm, "io_error", msg)


# ============================================================================
# Helper: vm_sizeinfo
# ============================================================================

# static uint8_t vm_sizeinfo(void)
# {
#     uint8_t res = sizeof(bint) == 8;
#     res |= (sizeof(breal) == 8) << 1;
#     return res;
# }
def vm_sizeinfo():
    res = 1 if USE_64BIT_INT else 0  # sizeof(bint) == 8
    res |= (0 if BE_USE_SINGLE_FLOAT else 1) << 1  # sizeof(breal) == 8
    return res


# ============================================================================
# Bytecode Saver
# ============================================================================

# static void save_byte(void *fp, uint8_t value)
# {
#     be_fwrite(fp, &value, 1);
# }
def save_byte(fp, value):
    fp.write(struct.pack('<B', value & 0xFF))


# static void save_word(void *fp, uint16_t value)
# {
#     uint8_t buffer[2];
#     buffer[0] = value & 0xff;
#     buffer[1] = value >> 8;
#     be_fwrite(fp, buffer, sizeof(buffer));
# }
def save_word(fp, value):
    fp.write(struct.pack('<H', value & 0xFFFF))


# static void save_long(void *fp, uint32_t value)
# {
#     uint8_t buffer[4];
#     buffer[0] = value & 0xff;
#     buffer[1] = (value >> 8) & 0xff;
#     buffer[2] = (value >> 16) & 0xff;
#     buffer[3] = (value >> 24) & 0xff;
#     be_fwrite(fp, buffer, sizeof(buffer));
# }
def save_long(fp, value):
    fp.write(struct.pack('<I', value & 0xFFFFFFFF))


# static void save_header(void *fp)
# {
#     uint8_t buffer[8] = { 0 };
#     buffer[0] = MAGIC_NUMBER1;
#     buffer[1] = MAGIC_NUMBER2;
#     buffer[2] = MAGIC_NUMBER3;
#     buffer[3] = BYTECODE_VERSION;
#     buffer[4] = vm_sizeinfo();
#     be_fwrite(fp, buffer, sizeof(buffer));
# }
def save_header(fp):
    buf = bytearray(8)
    buf[0] = MAGIC_NUMBER1
    buf[1] = MAGIC_NUMBER2
    buf[2] = MAGIC_NUMBER3
    buf[3] = BYTECODE_VERSION
    buf[4] = vm_sizeinfo()
    fp.write(bytes(buf))


# static void save_int(void *fp, bint i)
# {
# #if USE_64BIT_INT
#     save_long(fp, i & 0xffffffff);
#     save_long(fp, (i >> 32) & 0xffffffff);
# #else
#     save_long(fp, (uint32_t)i);
# #endif
# }
def save_int(fp, i):
    if USE_64BIT_INT:
        # Convert to unsigned 64-bit representation for negative numbers
        if i < 0:
            i = i & 0xFFFFFFFFFFFFFFFF
        save_long(fp, i & 0xFFFFFFFF)
        save_long(fp, (i >> 32) & 0xFFFFFFFF)
    else:
        save_long(fp, i & 0xFFFFFFFF)


# static void save_real(void *fp, breal r)
# {
# #if BE_USE_SINGLE_FLOAT
#     union { breal r; uint32_t i; } u;
#     u.r = r;
#     save_long(fp, u.i);
# #else
#     union { breal r; uint64_t i; } u;
#     u.r = r;
#     save_long(fp, u.i & 0xffffffff);
#     save_long(fp, (u.i >> 32) & 0xffffffff);
# #endif
# }
def save_real(fp, r):
    if BE_USE_SINGLE_FLOAT:
        raw = struct.pack('<f', r)
        fp.write(raw)
    else:
        raw = struct.pack('<d', r)
        fp.write(raw)


# static void save_string(void *fp, bstring *s)
# {
#     if (s) {
#         uint16_t length = (uint16_t)str_len(s);
#         const char *data = str(s);
#         save_word(fp, length);
#         be_fwrite(fp, data, length);
#     } else {
#         save_word(fp, 0);
#     }
# }
def save_string(fp, s):
    be_string = _lazy_be_string()
    if s is not None:
        length = str_len(s)
        data = be_string.be_str2cstr(s)
        save_word(fp, length)
        if isinstance(data, str):
            fp.write(data.encode('utf-8')[:length])
        else:
            fp.write(data[:length])
    else:
        save_word(fp, 0)


# static bstring** save_members(bvm *vm, void *fp, bclass *c, int nvar)
# {
#     bmapnode *node;
#     bstring **vars = NULL;
#     bmap *members = c->members;
#     bmapiter iter = be_map_iter();
#     if (nvar) {
#         vars = be_malloc(vm, sizeof(bstring *) * nvar);
#     }
#     while ((node = be_map_next(members, &iter)) != NULL) {
#         be_assert(var_isstr(&node->key));
#         if (var_isindex(&node->value)) {
#             if (vars == NULL) { return NULL; }
#             vars[var_toidx(&node->value)] = var_tostr(&node->key);
#         } else {
#             bproto *proto;
#             bvalue *value = &node->value;
#             be_assert(var_isclosure(value) || var_isproto(value) || var_isnil(value));
#             save_string(fp, var_tostr(&node->key));
#             if (var_isproto(value)) {
#                 proto = var_toobj(value);
#                 save_proto(vm, fp, proto);
#             } else if (var_isclosure(value)) {
#                 proto = cast(bclosure *, var_toobj(value))->proto;
#                 save_proto(vm, fp, proto);
#             } else if (var_isnil(value)) {
#                 save_word(fp, 0);
#             } else {
#                 be_raise(vm, "internal_error", "unsupported member in class");
#                 return NULL;
#             }
#         }
#     }
#     return vars;
# }
def save_members(vm, fp, c, nvar):
    be_map = _lazy_be_map()
    vars_ = [None] * nvar if nvar else None
    members = c.members
    iter_ref = [be_map_iter()]
    while True:
        node, iter_ref = be_map.be_map_next(members, iter_ref)
        if node is None:
            break
        assert var_isstr(node.key), "member key must be a string"
        if var_isindex(node.value):
            # Cache member-variable name
            if vars_ is None:
                return None  # should never be executed
            vars_[var_toidx(node.value)] = var_tostr(node.key)
        else:
            # Save method's name and function
            value = node.value
            assert var_isclosure(value) or var_isproto(value) or var_isnil(value)
            save_string(fp, var_tostr(node.key))  # save method name
            if var_isproto(value):
                proto = var_toobj(value)
                save_proto(vm, fp, proto)
            elif var_isclosure(value):
                proto = var_toobj(value).proto
                save_proto(vm, fp, proto)
            elif var_isnil(value):
                # Static member (nil default) — store zero-length name marker
                save_word(fp, 0)
            else:
                be_vm = _lazy_be_vm()
                be_vm.be_raise(vm, "internal_error", "unsupported member in class")
                return None
    return vars_


# static void save_class(bvm *vm, void *fp, bclass *c)
# {
#     bstring **vars;
#     int i, count = c->members ? be_map_count(c->members) : 0;
#     int nvar = c->nvar - be_class_closure_count(c);
#     save_string(fp, c->name);
#     save_long(fp, nvar);
#     save_long(fp, count - nvar);
#     if (count > 0) {
#         vars = save_members(vm, fp, c, nvar);
#         if (vars != NULL) {
#             for (i = 0; i < nvar; ++i) {
#                 save_string(fp, vars[i]);
#             }
#             be_free(vm, vars, sizeof(bstring *) * nvar);
#         }
#     }
# }
def save_class(vm, fp, c):
    be_class = _lazy_be_class()
    count = be_map_count(c.members) if c.members is not None else 0
    nvar = c.nvar - be_class.be_class_closure_count(c)
    save_string(fp, c.name)
    save_long(fp, nvar)
    save_long(fp, count - nvar)
    if count > 0:
        vars_ = save_members(vm, fp, c, nvar)
        if vars_ is not None:
            for i in range(nvar):
                save_string(fp, vars_[i])


# static void save_value(bvm *vm, void *fp, bvalue *v)
# {
#     save_byte(fp, (uint8_t)var_primetype(v));
#     switch (var_primetype(v)) {
#     case BE_INT: save_int(fp, var_toint(v)); break;
#     case BE_REAL: save_real(fp, var_toreal(v)); break;
#     case BE_STRING: save_string(fp, var_tostr(v)); break;
#     case BE_CLASS: save_class(vm, fp, var_toobj(v)); break;
#     default: break;
#     }
# }
def save_value(vm, fp, v):
    ptype = var_primetype(v)
    save_byte(fp, ptype)
    if ptype == BE_INT:
        save_int(fp, var_toint(v))
    elif ptype == BE_REAL:
        save_real(fp, var_toreal(v))
    elif ptype == BE_STRING:
        save_string(fp, var_tostr(v))
    elif ptype == BE_CLASS:
        save_class(vm, fp, var_toobj(v))


# static void save_bytecode(bvm *vm, void *fp, bproto *proto)
# {
#     int forbid_gbl = comp_is_named_gbl(vm);
#     binstruction *code = proto->code, *end;
#     save_long(fp, (uint32_t)proto->codesize);
#     for (end = code + proto->codesize; code < end; ++code) {
#         save_long(fp, (uint32_t)*code);
#         if (forbid_gbl) {
#             if ((uint32_t)*code == OP_GETGBL || (uint32_t)*code == OP_SETGBL) {
#                 be_raise(vm, "internal_error", "GETGBL/SETGBL found when saving with named globals");
#             }
#         }
#     }
# }
def save_bytecode(vm, fp, proto):
    be_var = _lazy_be_var()
    forbid_gbl = be_var.comp_is_named_gbl(vm)
    save_long(fp, proto.codesize & 0xFFFFFFFF)
    if proto.code is not None:
        for i in range(proto.codesize):
            ins = proto.code[i] & 0xFFFFFFFF
            save_long(fp, ins)
            if forbid_gbl:
                op = IGET_OP(ins)
                if op == OP_GETGBL or op == OP_SETGBL:
                    be_vm = _lazy_be_vm()
                    be_vm.be_raise(vm, "internal_error",
                                   "GETGBL/SETGBL found when saving with named globals")


# static void save_constants(bvm *vm, void *fp, bproto *proto)
# {
#     bvalue *v = proto->ktab, *end;
#     save_long(fp, proto->nconst);
#     for (end = v + proto->nconst; v < end; ++v) {
#         if ((v == proto->ktab) && (proto->varg & BE_VA_STATICMETHOD) && (v->type == BE_CLASS)) {
#             bvalue v_nil;
#             v_nil.v.i = 0;
#             v_nil.type = BE_NIL;
#             save_value(vm, fp, &v_nil);
#         } else {
#             save_value(vm, fp, v);
#         }
#     }
# }
def save_constants(vm, fp, proto):
    save_long(fp, proto.nconst)
    if proto.ktab is not None:
        for i in range(proto.nconst):
            v = proto.ktab[i]
            if (i == 0) and (proto.varg & BE_VA_STATICMETHOD) and (v.type == BE_CLASS):
                # Implicit `_class` parameter — output nil
                v_nil = bvalue()
                v_nil.v = 0
                v_nil.type = BE_NIL
                save_value(vm, fp, v_nil)
            else:
                save_value(vm, fp, v)


# static void save_proto_table(bvm *vm, void *fp, bproto *proto)
# {
#     bproto **p = proto->ptab, **end;
#     save_long(fp, proto->nproto);
#     if (p) {
#         for (end = p + proto->nproto; p < end; ++p) {
#             save_proto(vm, fp, *p);
#         }
#     }
# }
def save_proto_table(vm, fp, proto):
    save_long(fp, proto.nproto)
    if proto.ptab is not None:
        for i in range(proto.nproto):
            save_proto(vm, fp, proto.ptab[i])


# static void save_upvals(void *fp, bproto *proto)
# {
#     bupvaldesc *uv = proto->upvals, *end;
#     save_byte(fp, proto->nupvals);
#     if (uv) {
#         for (end = uv + proto->nupvals; uv < end; ++uv) {
#             save_byte(fp, uv->instack);
#             save_byte(fp, uv->idx);
#         }
#     }
# }
def save_upvals(fp, proto):
    save_byte(fp, proto.nupvals)
    if proto.upvals is not None:
        for i in range(proto.nupvals):
            uv = proto.upvals[i]
            save_byte(fp, uv.instack)
            save_byte(fp, uv.idx)


# static void save_proto(bvm *vm, void *fp, bproto *proto)
# {
#     if (proto) {
#         save_string(fp, proto->name);
# #if BE_DEBUG_SOURCE_FILE
#         save_string(fp, proto->source);
# #else
#         save_string(fp, NULL);
# #endif
#         save_byte(fp, proto->argc);
#         save_byte(fp, proto->nstack);
#         save_byte(fp, proto->varg);
#         save_byte(fp, 0x00);
#         save_bytecode(vm, fp, proto);
#         save_constants(vm, fp, proto);
#         save_proto_table(vm, fp, proto);
#         save_upvals(fp, proto);
#     }
# }
def save_proto(vm, fp, proto):
    if proto is not None:
        save_string(fp, proto.name)
        if BE_DEBUG_SOURCE_FILE:
            save_string(fp, proto.source)
        else:
            save_string(fp, None)
        save_byte(fp, proto.argc)
        save_byte(fp, proto.nstack)
        save_byte(fp, proto.varg)
        save_byte(fp, 0x00)  # reserved
        save_bytecode(vm, fp, proto)
        save_constants(vm, fp, proto)
        save_proto_table(vm, fp, proto)
        save_upvals(fp, proto)


# static void save_globals(bvm *vm, void *fp)
# {
#     bmapnode *node;
#     bmapiter iter = be_map_iter();
#     bmap *map = vm->gbldesc.global.vtab;
#     int i, count = be_global_count(vm);
#     bstring **list = be_malloc(vm, sizeof(bstring*) * count);
#     while ((node = be_map_next(map, &iter)) != NULL) {
#         if (var_isstr(&node->key)) {
#             int idx = var_toidx(&node->value);
#             be_assert(idx < count);
#             list[idx] = var_tostr(&node->key);
#         }
#     }
#     for (i = 0; i < count; ++i) {
#         save_string(fp, list[i]);
#     }
#     be_free(vm, list, sizeof(bstring*) * count);
# }
def save_globals(vm, fp):
    be_var = _lazy_be_var()
    be_map = _lazy_be_map()
    map_ = vm.gbldesc.global_.vtab
    count = be_var.be_global_count(vm)
    name_list = [None] * count
    iter_ref = [be_map_iter()]
    while True:
        node, iter_ref = be_map.be_map_next(map_, iter_ref)
        if node is None:
            break
        if var_isstr(node.key):
            idx = var_toidx(node.value)
            assert idx < count
            name_list[idx] = var_tostr(node.key)
    for i in range(count):
        save_string(fp, name_list[i])


# static void save_global_info(bvm *vm, void *fp)
# {
#     save_long(fp, be_builtin_count(vm));
#     if (comp_is_named_gbl(vm)) {
#         save_long(fp, 0);
#     } else {
#         save_long(fp, be_global_count(vm));
#         save_globals(vm, fp);
#     }
# }
def save_global_info(vm, fp):
    be_var = _lazy_be_var()
    save_long(fp, be_var.be_builtin_count(vm))
    if be_var.comp_is_named_gbl(vm):
        # Named globals only — no need to save current globals
        save_long(fp, 0)
    else:
        save_long(fp, be_var.be_global_count(vm))
        save_globals(vm, fp)


# void be_bytecode_save_to_fs(bvm *vm, void *fp, bproto *proto)
# {
#     save_header(fp);
#     save_global_info(vm, fp);
#     save_proto(vm, fp, proto);
# }
def be_bytecode_save_to_fs(vm, fp, proto):
    save_header(fp)
    save_global_info(vm, fp)
    save_proto(vm, fp, proto)


# void be_bytecode_save(bvm *vm, const char *filename, bproto *proto)
# {
#     void *fp = be_fopen(filename, "wb");
#     if (fp == NULL) {
#         bytecode_error(vm, be_pushfstring(vm,
#             "can not open file '%s'.", filename));
#     } else {
#         be_bytecode_save_to_fs(vm, fp, proto);
#         be_fclose(fp);
#     }
# }
def be_bytecode_save(vm, filename, proto):
    try:
        fp = open(filename, 'wb')
    except OSError:
        be_vm = _lazy_be_vm()
        bytecode_error(vm, be_vm.be_pushfstring(vm,
            "can not open file '%s'.", filename))
        return
    try:
        be_bytecode_save_to_fs(vm, fp, proto)
    finally:
        fp.close()


# ============================================================================
# Bytecode Loader
# ============================================================================

# static uint8_t load_byte(void *fp)
# {
#     uint8_t buffer[1];
#     if (be_fread(fp, buffer, sizeof(buffer)) == sizeof(buffer)) {
#         return buffer[0];
#     }
#     return 0;
# }
def load_byte(fp):
    data = fp.read(1)
    if len(data) == 1:
        return data[0]
    return 0


# static uint16_t load_word(void *fp)
# {
#     uint8_t buffer[2];
#     if (be_fread(fp, buffer, sizeof(buffer)) == sizeof(buffer)) {
#         return ((uint16_t)buffer[1] << 8) | buffer[0];
#     }
#     return 0;
# }
def load_word(fp):
    data = fp.read(2)
    if len(data) == 2:
        return (data[1] << 8) | data[0]
    return 0


# static uint32_t load_long(void *fp)
# {
#     uint8_t buffer[4];
#     if (be_fread(fp, buffer, sizeof(buffer)) == sizeof(buffer)) {
#         return ((uint32_t)buffer[3] << 24)
#             | ((uint32_t)buffer[2] << 16)
#             | ((uint32_t)buffer[1] << 8)
#             | buffer[0];
#     }
#     return 0;
# }
def load_long(fp):
    data = fp.read(4)
    if len(data) == 4:
        return ((data[3] << 24) | (data[2] << 16) |
                (data[1] << 8) | data[0])
    return 0


# static int load_head(void *fp)
# {
#     int res;
#     uint8_t buffer[8] = { 0 };
#     be_fread(fp, buffer, sizeof(buffer));
#     res = buffer[0] == MAGIC_NUMBER1 &&
#           buffer[1] == MAGIC_NUMBER2 &&
#           buffer[2] == MAGIC_NUMBER3 &&
#           buffer[4] == vm_sizeinfo();
#     if (res) {
#         return buffer[3];
#     } else {
#         return 0;
#     }
# }
def load_head(fp):
    data = fp.read(8)
    if len(data) < 8:
        return 0
    buf = bytearray(data)
    res = (buf[0] == MAGIC_NUMBER1 and
           buf[1] == MAGIC_NUMBER2 and
           buf[2] == MAGIC_NUMBER3 and
           buf[4] == vm_sizeinfo())
    if res:
        return buf[3]  # version
    else:
        return 0


# bbool be_bytecode_check(const char *path)
# {
#     void *fp = be_fopen(path, "r");
#     if (fp) {
#         uint8_t buffer[3], rb;
#         rb = (uint8_t)be_fread(fp, buffer, 3);
#         be_fclose(fp);
#         return rb == 3 &&
#             buffer[0] == MAGIC_NUMBER1 &&
#             buffer[1] == MAGIC_NUMBER2 &&
#             buffer[2] == MAGIC_NUMBER3;
#     }
#     return bfalse;
# }
def be_bytecode_check(path):
    try:
        with open(path, 'rb') as fp:
            data = fp.read(3)
            return (len(data) == 3 and
                    data[0] == MAGIC_NUMBER1 and
                    data[1] == MAGIC_NUMBER2 and
                    data[2] == MAGIC_NUMBER3)
    except OSError:
        return False


# static bint load_int(void *fp)
# {
# #if USE_64BIT_INT
#     bint i;
#     i = load_long(fp);
#     i |= (bint)load_long(fp) << 32;
#     return i;
# #else
#     return load_long(fp);
# #endif
# }
def load_int(fp):
    if USE_64BIT_INT:
        lo = load_long(fp)
        hi = load_long(fp)
        val = lo | (hi << 32)
        # Sign-extend from 64-bit
        if val >= (1 << 63):
            val -= (1 << 64)
        return val
    else:
        val = load_long(fp)
        # Sign-extend from 32-bit
        if val >= (1 << 31):
            val -= (1 << 32)
        return val


# static breal load_real(void *fp)
# {
# #if BE_USE_SINGLE_FLOAT
#     union { breal r; uint32_t i; } u;
#     u.i = load_long(fp);
#     return u.r;
# #else
#     union { breal r; uint64_t i; } u;
#     u.i = load_long(fp);
#     u.i |= (uint64_t)load_long(fp) << 32;
#     return u.r;
# #endif
# }
def load_real(fp):
    if BE_USE_SINGLE_FLOAT:
        raw = fp.read(4)
        if len(raw) == 4:
            return struct.unpack('<f', raw)[0]
        return 0.0
    else:
        raw = fp.read(8)
        if len(raw) == 8:
            return struct.unpack('<d', raw)[0]
        return 0.0


# static bstring* load_string(bvm *vm, void *fp)
# {
#     uint16_t len = load_word(fp);
#     if (len > 0) {
#         bstring *str;
#         char *buf = be_malloc(vm, len);
#         be_fread(fp, buf, len);
#         str = be_newstrn(vm, buf, len);
#         be_free(vm, buf, len);
#         return str;
#     }
#     return str_literal(vm, "");
# }
def load_string(vm, fp):
    be_string = _lazy_be_string()
    be_vm = _lazy_be_vm()
    length = load_word(fp)
    if length > 0:
        buf = fp.read(length)
        # Decode bytes to Python string for be_newstrn
        try:
            s_data = buf.decode('utf-8')
        except UnicodeDecodeError:
            s_data = buf.decode('latin-1')
        return be_string.be_newstrn(vm, s_data, length)
    return be_vm.str_literal(vm, "")


# static bstring* cache_string(bvm *vm, void *fp)
# {
#     bstring *str = load_string(vm, fp);
#     var_setstr(vm->top, str);
#     be_incrtop(vm);
#     return str;
# }
def cache_string(vm, fp):
    s = load_string(vm, fp)
    # Push string onto stack to protect from GC
    if vm.top_idx >= len(vm.stack):
        vm.stack.append(bvalue())
    var_setstr(vm.stack[vm.top_idx], s)
    vm.top_idx += 1
    return s


# static void load_class(bvm *vm, void *fp, bvalue *v, int version)
# {
#     int nvar, count;
#     bclass *c = be_newclass(vm, NULL, NULL);
#     var_setclass(v, c);
#     c->name = load_string(vm, fp);
#     nvar = load_long(fp);
#     count = load_long(fp);
#     while (count--) {
#         bvalue *value;
#         bstring *name = cache_string(vm, fp);
#         value = vm->top;
#         var_setproto(value, NULL);
#         be_incrtop(vm);
#         if (load_proto(vm, fp, (bproto**)&var_toobj(value), -3, version)) {
#             bproto *proto = (bproto*)var_toobj(value);
#             bbool is_method = proto->varg & BE_VA_METHOD;
#             if (!is_method) {
#                 if ((proto->nconst > 0) && (proto->ktab->type == BE_NIL)) {
#                     proto->ktab->type = BE_CLASS;
#                     proto->ktab->v.p = c;
#                 }
#             }
#             be_class_method_bind(vm, c, name, var_toobj(value), !is_method);
#         } else {
#             be_class_member_bind(vm, c, name, bfalse);
#         }
#         be_stackpop(vm, 2);
#     }
#     for (count = 0; count < nvar; ++count) {
#         bstring *name = cache_string(vm, fp);
#         be_class_member_bind(vm, c, name, btrue);
#         be_stackpop(vm, 1);
#     }
# }
def load_class(vm, fp, v, version):
    be_class = _lazy_be_class()
    c = be_class.be_newclass(vm, None, None)
    var_setclass(v, c)
    c.name = load_string(vm, fp)
    nvar = load_long(fp)
    count = load_long(fp)
    while count > 0:
        count -= 1
        name = cache_string(vm, fp)
        # Push a proto slot onto the stack
        if vm.top_idx >= len(vm.stack):
            vm.stack.append(bvalue())
        value = vm.stack[vm.top_idx]
        var_setproto(value, None)
        vm.top_idx += 1
        # load_proto returns (success, proto_obj) via a mutable list
        proto_holder = [None]
        success = load_proto(vm, fp, proto_holder, -3, version)
        if success:
            proto = proto_holder[0]
            # Update the stack slot with the loaded proto
            var_setproto(value, proto)
            is_method = (proto.varg & BE_VA_METHOD) != 0
            if not is_method:
                if proto.nconst > 0 and proto.ktab is not None and proto.ktab[0].type == BE_NIL:
                    # Replace implicit _class parameter with the class
                    proto.ktab[0].type = BE_CLASS
                    proto.ktab[0].v = c
            be_class.be_class_method_bind(vm, c, name, proto, not is_method)
        else:
            # No proto — static member set to nil
            be_class.be_class_member_bind(vm, c, name, False)
        vm.top_idx -= 2  # pop cached string and proto
    for _ in range(nvar):
        name = cache_string(vm, fp)
        be_class.be_class_member_bind(vm, c, name, True)
        vm.top_idx -= 1  # pop cached string


# static void load_value(bvm *vm, void *fp, bvalue *v, int version)
# {
#     switch (load_byte(fp)) {
#     case BE_INT: var_setint(v, load_int(fp)); break;
#     case BE_REAL: var_setreal(v, load_real(fp)); break;
#     case BE_STRING: var_setstr(v, load_string(vm, fp)); break;
#     case BE_CLASS: load_class(vm, fp, v, version); break;
#     default: break;
#     }
# }
def load_value(vm, fp, v, version):
    type_byte = load_byte(fp)
    if type_byte == BE_INT:
        var_setint(v, load_int(fp))
    elif type_byte == BE_REAL:
        var_setreal(v, load_real(fp))
    elif type_byte == BE_STRING:
        var_setstr(v, load_string(vm, fp))
    elif type_byte == BE_CLASS:
        load_class(vm, fp, v, version)
    # default: leave v as-is (nil)


# static void load_bytecode(bvm *vm, void *fp, bproto *proto, int info)
# {
#     int size = (int)load_long(fp);
#     if (size) {
#         binstruction *code, *end;
#         int bcnt = be_builtin_count(vm);
#         blist *list = var_toobj(be_indexof(vm, info));
#         be_assert(be_islist(vm, info));
#         proto->code = be_malloc(vm, sizeof(binstruction) * size);
#         proto->codesize = size;
#         code = proto->code;
#         for (end = code + size; code < end; ++code) {
#             binstruction ins = (binstruction)load_long(fp);
#             binstruction op = IGET_OP(ins);
#             if (op == OP_GETGBL || op == OP_SETGBL) {
#                 int idx = IGET_Bx(ins);
#                 if (idx >= bcnt) {
#                     bvalue *name = be_list_at(list, idx - bcnt);
#                     idx = be_global_find(vm, var_tostr(name));
#                     ins = (ins & ~IBx_MASK) | ISET_Bx(idx);
#                 }
#             }
#             *code = ins;
#         }
#     }
# }
def load_bytecode(vm, fp, proto, info):
    be_var = _lazy_be_var()
    be_vm = _lazy_be_vm()
    size = load_long(fp)
    if size:
        bcnt = be_var.be_builtin_count(vm)
        # Resolve the global name list from the stack
        list_idx = be_vm.be_indexof(vm, info)
        list_obj = var_toobj(vm.stack[list_idx])
        proto.code = [0] * size
        proto.codesize = size
        for i in range(size):
            ins = load_long(fp)
            op = IGET_OP(ins)
            # Fix global variable index
            if op == OP_GETGBL or op == OP_SETGBL:
                idx = IGET_Bx(ins)
                if idx >= bcnt:  # does not fix builtin index
                    name_val = be_list_at(list_obj, idx - bcnt)
                    idx = be_var.be_global_find(vm, var_tostr(name_val))
                    ins = (ins & ~IBx_MASK) | ISET_Bx(idx)
            proto.code[i] = ins


# static void load_constant(bvm *vm, void *fp, bproto *proto, int version)
# {
#     int size = (int)load_long(fp);
#     if (size) {
#         bvalue *end, *v = be_malloc(vm, sizeof(bvalue) * size);
#         memset(v, 0, sizeof(bvalue) * size);
#         proto->ktab = v;
#         proto->nconst = size;
#         for (end = v + size; v < end; ++v) {
#             load_value(vm, fp, v, version);
#         }
#     }
# }
def load_constant(vm, fp, proto, version):
    size = load_long(fp)
    if size:
        ktab = [bvalue() for _ in range(size)]
        proto.ktab = ktab
        proto.nconst = size
        for i in range(size):
            load_value(vm, fp, ktab[i], version)


# static void load_proto_table(bvm *vm, void *fp, bproto *proto, int info, int version)
# {
#     int size = (int)load_long(fp);
#     if (size) {
#         bproto **p = be_malloc(vm, sizeof(bproto *) * size);
#         memset(p, 0, sizeof(bproto *) * size);
#         proto->ptab = p;
#         proto->nproto = size;
#         while (size--) {
#             load_proto(vm, fp, p++, info, version);
#         }
#     }
# }
def load_proto_table(vm, fp, proto, info, version):
    size = load_long(fp)
    if size:
        ptab = [None] * size
        proto.ptab = ptab
        proto.nproto = size
        for i in range(size):
            holder = [None]
            load_proto(vm, fp, holder, info, version)
            ptab[i] = holder[0]


# static void load_upvals(bvm *vm, void *fp, bproto *proto)
# {
#     int size = (int)load_byte(fp);
#     if (size) {
#         bupvaldesc *uv, *end;
#         proto->upvals = be_malloc(vm, sizeof(bupvaldesc) * size);
#         proto->nupvals = (bbyte)size;
#         uv = proto->upvals;
#         for (end = uv + size; uv < end; ++uv) {
#             uv->instack = load_byte(fp);
#             uv->idx = load_byte(fp);
#         }
#     }
# }
def load_upvals(vm, fp, proto):
    size = load_byte(fp)
    if size:
        upvals = []
        for _ in range(size):
            uv = bupvaldesc()
            uv.instack = load_byte(fp)
            uv.idx = load_byte(fp)
            upvals.append(uv)
        proto.upvals = upvals
        proto.nupvals = size


# static bbool load_proto(bvm *vm, void *fp, bproto **proto, int info, int version)
# {
#     bstring *name = load_string(vm, fp);
#     if (str_len(name)) {
#         *proto = be_newproto(vm);
#         (*proto)->name = name;
# #if BE_DEBUG_SOURCE_FILE
#         (*proto)->source = load_string(vm, fp);
# #else
#         load_string(vm, fp);
# #endif
#         (*proto)->argc = load_byte(fp);
#         (*proto)->nstack = load_byte(fp);
#         if (version > 1) {
#             (*proto)->varg = load_byte(fp);
#             load_byte(fp);
#         }
#         load_bytecode(vm, fp, *proto, info);
#         load_constant(vm, fp, *proto, version);
#         load_proto_table(vm, fp, *proto, info, version);
#         load_upvals(vm, fp, *proto);
#         return btrue;
#     }
#     return bfalse;
# }
def load_proto(vm, fp, proto_holder, info, version):
    """Load a proto from the bytecode stream.

    proto_holder is a mutable list [proto_or_None]. On success, proto_holder[0]
    is set to the loaded bproto and True is returned. On failure (empty name,
    indicating a static member), returns False.
    """
    be_func = _lazy_be_func()
    name = load_string(vm, fp)
    if str_len(name) > 0:
        proto = be_func.be_newproto(vm)
        proto_holder[0] = proto
        proto.name = name
        if BE_DEBUG_SOURCE_FILE:
            proto.source = load_string(vm, fp)
        else:
            load_string(vm, fp)  # discard source name
        proto.argc = load_byte(fp)
        proto.nstack = load_byte(fp)
        if version > 1:
            proto.varg = load_byte(fp)
            load_byte(fp)  # discard reserved byte
        load_bytecode(vm, fp, proto, info)
        load_constant(vm, fp, proto, version)
        load_proto_table(vm, fp, proto, info, version)
        load_upvals(vm, fp, proto)
        return True
    return False  # no proto read


# void load_global_info(bvm *vm, void *fp)
# {
#     int i;
#     int bcnt = (int)load_long(fp);
#     int gcnt = (int)load_long(fp);
#     if (bcnt > be_builtin_count(vm)) {
#         bytecode_error(vm, be_pushfstring(vm,
#             "inconsistent number of builtin objects."));
#     }
#     be_newlist(vm);
#     for (i = 0; i < gcnt; ++i) {
#         bstring *name = cache_string(vm, fp);
#         be_global_new(vm, name);
#         be_data_push(vm, -2);
#         be_stackpop(vm, 1);
#     }
#     be_global_release_space(vm);
# }
def load_global_info(vm, fp):
    be_var = _lazy_be_var()
    be_list_mod = _lazy_be_list()
    be_vm = _lazy_be_vm()
    bcnt = load_long(fp)
    gcnt = load_long(fp)
    if bcnt > be_var.be_builtin_count(vm):
        bytecode_error(vm, be_vm.be_pushfstring(vm,
            "inconsistent number of builtin objects."))
    # Create a new list on the stack (equivalent to be_newlist(vm))
    lst = be_list_mod.be_list_new(vm)
    if vm.top_idx >= len(vm.stack):
        vm.stack.append(bvalue())
    from berry_port.be_object import var_setlist
    var_setlist(vm.stack[vm.top_idx], lst)
    vm.top_idx += 1
    for _ in range(gcnt):
        name = cache_string(vm, fp)
        be_var.be_global_new(vm, name)
        # Push the variable name to the list
        name_val = vm.stack[vm.top_idx - 1]
        be_list_mod.be_list_push(vm, lst, name_val)
        vm.top_idx -= 1  # pop the cached string
    be_var.be_global_release_space(vm)


# bclosure* be_bytecode_load_from_fs(bvm *vm, void *fp)
# {
#     int version = load_head(fp);
#     if (version == BYTECODE_VERSION) {
#         bclosure *cl = be_newclosure(vm, 0);
#         var_setclosure(vm->top, cl);
#         be_stackpush(vm);
#         load_global_info(vm, fp);
#         load_proto(vm, fp, &cl->proto, -1, version);
#         be_stackpop(vm, 2);
#         be_fclose(fp);
#         return cl;
#     }
#     bytecode_error(vm, be_pushfstring(vm,
#         "invalid bytecode version."));
#     return NULL;
# }
def be_bytecode_load_from_fs(vm, fp):
    be_func = _lazy_be_func()
    be_vm = _lazy_be_vm()
    version = load_head(fp)
    if version == BYTECODE_VERSION:
        cl = be_func.be_newclosure(vm, 0)
        # Push closure onto stack to protect from GC
        if vm.top_idx >= len(vm.stack):
            vm.stack.append(bvalue())
        var_setclosure(vm.stack[vm.top_idx], cl)
        vm.top_idx += 1
        load_global_info(vm, fp)
        # Load the top-level proto into the closure
        proto_holder = [None]
        load_proto(vm, fp, proto_holder, -1, version)
        cl.proto = proto_holder[0]
        vm.top_idx -= 2  # pop the closure and list
        fp.close()
        return cl
    bytecode_error(vm, be_vm.be_pushfstring(vm,
        "invalid bytecode version."))
    return None


# bclosure* be_bytecode_load(bvm *vm, const char *filename)
# {
#     void *fp = be_fopen(filename, "rb");
#     if (fp == NULL) {
#         bytecode_error(vm, be_pushfstring(vm,
#             "can not open file '%s'.", filename));
#     } else {
#         return be_bytecode_load_from_fs(vm, fp);
#     }
#     return NULL;
# }
def be_bytecode_load(vm, filename):
    be_vm = _lazy_be_vm()
    try:
        fp = open(filename, 'rb')
    except OSError:
        bytecode_error(vm, be_vm.be_pushfstring(vm,
            "can not open file '%s'.", filename))
        return None
    return be_bytecode_load_from_fs(vm, fp)
