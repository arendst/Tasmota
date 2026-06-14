/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#ifndef BE_CONSTOBJ_H
#define BE_CONSTOBJ_H

#ifdef __cplusplus
extern "C" {
#endif

#include "be_object.h"
#include "be_gc.h"
#include "be_map.h"
#include "be_list.h"
#include "be_class.h"
#include "be_string.h"
#include "be_module.h"
#include "be_byteslib.h"

#ifndef __cplusplus

#define be_const_header(_t)                                     \
    .next = NULL,                                               \
    .type = (_t),                                               \
    .marked = GC_CONST

#define be_define_const_bytes(_name, ...)                               \
    const binstance_arg3 be_const_instance_##_name = {                  \
        be_const_header(BE_INSTANCE),                                   \
        .super = NULL,                                                  \
        .sub = NULL,                                                    \
        ._class = (bclass*) &be_class_bytes,                            \
        .members = {                                                    \
            {.v.c = (const void*) & (const uint8_t[]) { __VA_ARGS__ },  \
            .type = BE_COMPTR },                                        \
            be_const_int(sizeof(#_name) / 2),                           \
            be_const_int(BYTES_SIZE_SOLIDIFIED)                         \
        }                                                               \
    }

/* special version to define a default empty bytes */
#define be_define_const_bytes_empty()                                   \
    const binstance_arg3 be_const_instance_ = {                         \
        be_const_header(BE_INSTANCE),                                   \
        .super = NULL,                                                  \
        .sub = NULL,                                                    \
        ._class = (bclass*) &be_class_bytes,                            \
        .members = {                                                    \
            {.v.c = (const void*) & (const uint8_t[]) { 0x00 },         \
            .type = BE_COMPTR },                                        \
            be_const_int(0),                                            \
            be_const_int(BYTES_SIZE_SOLIDIFIED)                         \
        }                                                               \
    }

#define be_const_bytes_instance(_bytes) {                               \
    .v.c = &be_const_instance_##_bytes,                                 \
    .type = BE_INSTANCE                                                 \
}

#define be_define_const_str_weak(_name, _s, _len)               \
    const bcstring be_const_str_##_name = {                     \
        .next = NULL,                                           \
        .type = BE_STRING,                                      \
        .marked = GC_CONST,                                     \
        .extra = 0,                                             \
        .slen = _len,                                           \
        .hash = 0,                                              \
        .s = _s                                                 \
    }

#define be_const_key(_str, _next) {                             \
    .v.c = &be_const_str_##_str,                                \
    .type = BE_STRING,                                          \
    .next = (uint32_t)(_next) & 0xFFFFFF                        \
}

/* try to use the predefined string in strtab, but don't create an instance if none is present */
/* the behavior is exactly the same as `be_const_key()` but it not detected by coc */
#define be_const_key_weak(_str, _next) {                        \
    .v.c = &be_const_str_##_str,                                \
    .type = BE_STRING,                                          \
    .next = (uint32_t)(_next) & 0xFFFFFF                        \
}

#define be_const_key_literal(_str, _next) {                     \
    .v.c = be_str_literal(#_str),                                \
    .type = BE_STRING,                                          \
    .next = (uint32_t)(_next) & 0xFFFFFF                        \
}

#define be_const_key_int(_i, _next) {                           \
    .v.i = _i,                                                  \
    .type = BE_INT,                                             \
    .next = (uint32_t)(_next) & 0xFFFFFF                        \
}

/* Compact map node (BE_USE_COMPACT_MAP): the node is
 * { key_v, key_type, next, val_type, val_v }. The be_ckey_* macros emit the
 * first three elements (key payload, key type, chain link masked to 16 bits);
 * the node template then appends the value type byte and the value payload.
 * be_ckey / be_ckey_weak are recognized by coc (strong / weak) like
 * be_const_key / be_const_key_weak. */
#define be_ckey(_str, _next)                                    \
    { .c = &be_const_str_##_str }, BE_STRING, ((uint32_t)(_next) & 0xFFFF)

#define be_ckey_weak(_str, _next)                               \
    { .c = &be_const_str_##_str }, BE_STRING, ((uint32_t)(_next) & 0xFFFF)

#define be_ckey_int(_i, _next)                                  \
    { .i = (bint)(_i) }, BE_INT, ((uint32_t)(_next) & 0xFFFF)

#define be_const_func(_func) {                                  \
    .v.nf = (_func),                                            \
    .type = BE_NTVFUNC                                          \
}

#define be_const_static_func(_func) {                           \
    .v.nf = (_func),                                            \
    .type = BE_NTVFUNC | BE_STATIC                              \
}

#define be_const_nil() {                                        \
    .v.i = 0,                                                   \
    .type = BE_NIL                                              \
}

#define be_const_int(_val) {                                    \
    .v.i = (bint)(_val),                                        \
    .type = BE_INT                                              \
}

#define be_const_var(_val) {                                  \
    .v.i = (bint)(_val),                                        \
    .type = BE_INDEX                                            \
}

#define be_const_real(_val) {                                   \
    .v.r = (breal)(_val),                                       \
    .type = BE_REAL                                             \
}

#define be_const_real_hex(_val) {                               \
    .v.p = (void*)(_val),                                       \
    .type = BE_REAL                                             \
}

#define be_const_bool(_val) {                                   \
    .v.b = (bbool)(_val),                                       \
    .type = BE_BOOL                                             \
}

#define be_const_str(_str) {                                    \
    .v.s = (bstring*)(_str),                                    \
    .type = BE_STRING                                           \
}

#define be_const_comptr(_val) {                                 \
    .v.c = (const void*)(_val),                                 \
    .type = BE_COMPTR                                           \
}

#define be_const_class(_class) {                                \
    .v.c = &(_class),                                           \
    .type = BE_CLASS                                            \
}

#define be_const_closure(_closure) {                            \
    .v.c = &(_closure),                                         \
    .type = BE_CLOSURE                                          \
}

#define be_const_static_closure(_closure) {                     \
    .v.c = &(_closure),                                         \
    .type = BE_CLOSURE | BE_STATIC                              \
}

#define be_const_module(_module) {                              \
    .v.c = &(_module),                                          \
    .type = BE_MODULE                                           \
}

#define be_const_simple_instance(_instance) {                   \
    .v.c = (_instance),                                         \
    .type = BE_INSTANCE                                         \
}

#define be_const_map(_map) {                                    \
    .v.c = &(_map),                                             \
    .type = BE_MAP                                              \
}

#define be_const_list(_list) {                                  \
    .v.c = &(_list),                                            \
    .type = BE_LIST                                             \
}

#if BE_USE_COMPACT_MAP
#define be_define_const_map_slots(_name)                        \
const bmapnodec _name##_slots[] =
#else
#define be_define_const_map_slots(_name)                        \
const bmapnode _name##_slots[] =
#endif

#define be_define_const_map(_name, _size)                       \
const bmap _name = {                                            \
    be_const_header(BE_MAP),                                    \
    .slots = (bmapnode*)_name##_slots,                          \
    .lastfree = NULL,                                           \
    .size = _size,                                              \
    .count = _size                                              \
}

#define be_define_const_class(_name, _nvar, _super, _name_)     \
const bclass _name = {                                          \
    be_const_header(BE_CLASS),                                  \
    .nvar = _nvar,                                              \
    .super = _super,                                            \
    .members = (bmap*)&_name##_map,                             \
    .name = (bstring*)&be_const_str_##_name_                    \
}

#define be_define_const_empty_class(_name, _super, _name_)      \
const bclass _name = {                                          \
    be_const_header(BE_CLASS),                                  \
    .nvar = 0,                                                  \
    .super = (bclass*)_super,                                   \
    .members = NULL,                                            \
    .name = (bstring*)&be_const_str_##_name_                    \
}

#define be_define_const_module(_name, _name_)                   \
const bmodule _name = {                                         \
    be_const_header(BE_MODULE),                                 \
    .table = (bmap*)&_name##_map,                               \
    .info.name = _name_,                                        \
}

#define be_define_const_vector(_name, _data, _size)             \
const bvector _name = {                                         \
    .capacity = _size,                                          \
    .size = sizeof(bvalue),                                     \
    .count = _size,                                             \
    .data = (void*)_data,                                       \
    .end = (void*)(_data + (_size) - 1)                         \
}

#define be_define_const_native_module(_module)                  \
const bntvmodule_t be_native_module(_module) = {                  \
    .name = #_module,                                           \
    .attrs = NULL,                                              \
    .size = 0,                                                  \
    .module = (bmodule*)&(m_lib##_module)                       \
}

/* defines needed for solidified classes */
#define be_local_class(_name, _nvar, _super, _map, _cname)      \
  const bclass be_class_##_name = {                             \
    be_const_header(BE_CLASS),                                  \
    .nvar = _nvar,                                              \
    .super = (bclass*)_super,                                   \
    .members = (bmap*)_map,                                     \
    .name = _cname                                              \
}

/* defines needed for solidified modules */
#define be_local_module(_c_name, _module_name, _map)            \
  static const bmodule m_lib##_c_name = {                       \
    be_const_header(BE_MODULE),                                 \
    .table = (bmap*)_map,                                       \
    .info.name = _module_name                                   \
}

/* only instances with no super and no sub instance are supported */
/* primarily for `list` and `map`*/
#define be_nested_simple_instance(_class_ptr, _members)         \
  & (const binstance)  {                                        \
    be_const_header(BE_INSTANCE),                               \
    .super = NULL,                                              \
    .sub = NULL,                                                \
    ._class = (bclass*) _class_ptr,                             \
    .members = _members                                         \
  }

#define be_nested_simple_instance_1_arg(_class_ptr, arg0) \
  & (const binstance)  {                                        \
    be_const_header(BE_INSTANCE),                               \
    .super = NULL,                                              \
    .sub = NULL,                                                \
    ._class = (bclass*) _class_ptr,                             \
    .members = { arg0 }                                         \
  }


/* only instances with no super and no sub instance are supported */
/* primarily for `list` and `map`*/
#define be_nested_simple_instance_3_args(_class_ptr, arg0, arg1, arg2) \
  & (const binstance_arg3)  {                                   \
    be_const_header(BE_INSTANCE),                               \
    .super = NULL,                                              \
    .sub = NULL,                                                \
    ._class = (bclass*) _class_ptr,                             \
    .members = { arg0, arg1, arg2 }                             \
  }

#define be_nested_map(_size, _slots)                            \
  & (const bmap) {                                              \
    be_const_header(BE_MAP),                                    \
    .slots = _slots,                                            \
    .lastfree = NULL,                                           \
    .size = _size,                                              \
    .count = _size                                              \
  }

#define be_nested_list(_size, _items)                           \
  & (const blist) {                                             \
    be_const_header(BE_LIST),                                   \
    .count = _size,                                             \
    .capacity = _size,                                          \
    .data = _items                                              \
  }

#define be_nested_str(_name_)                                   \
  {                                                             \
    { .s=((bstring*)&be_const_str_##_name_) },                  \
    BE_STRING                                                   \
  }

/* variant that does not trigger strtab */
#define be_nested_str_weak(_name_)                              \
  {                                                             \
    { .s=((bstring*)&be_const_str_##_name_) },                  \
    BE_STRING                                                   \
  }

/* variant for long strings that does not trigger strtab */
#define be_nested_str_long(_name_)                              \
  {                                                             \
    { .s=((bstring*)&be_const_str_##_name_) },                  \
    BE_STRING                                                   \
  }

#define be_nested_str_literal(_name_)                           \
  {                                                             \
    { .s=(be_nested_const_str(_name_, _hash, sizeof(_name_)-1 ))\
    },                                                          \
    BE_STRING                                                   \
  }

#define be_str_literal(_str)                                    \
  be_nested_const_str(_str, 0, sizeof(_str)-1 )

#define be_str_weak(_str)                                       \
  (bstring*) &be_const_str_##_str

#define be_nested_string(_str, _hash, _len)                     \
  {                                                             \
    { .s=(be_nested_const_str(_str, _hash, _len ))              \
    },                                                          \
    BE_STRING                                                   \
  }

#define be_nested_key(_str, _hash, _len, _next)                 \
  {                                                             \
    { .s=(be_nested_const_str(_str, _hash, _len )) },           \
    BE_STRING,                                                  \
    (uint32_t)(_next) & 0xFFFFFF                                \
  }

/* Compact-map (BE_USE_COMPACT_MAP) literal-string key: emits the first three
 * elements of a bmapnodec initializer (key payload, key type, chain link masked
 * to 16 bits). Compact sibling of be_nested_key, for hand-written const maps. */
#define be_ckey_nested(_str, _hash, _len, _next)                \
    { .s=(be_nested_const_str(_str, _hash, _len )) },           \
    BE_STRING,                                                  \
    (uint32_t)(_next) & 0xFFFF

/* Compact constant table (BE_USE_COMPACT_KTAB): payload-word initializers.
 * These produce a `union bvaldata` (no type field) used in the split
 * constant arrays emitted by solidify. The type lives in the parallel
 * `bbyte` array. Mirrors the readable be_const_* style.
 * be_kv_str / be_kv_str_weak / be_kv_str_long are recognized by the `coc`
 * tool (strong / weak / long-bclstring respectively) so the referenced
 * string is registered in the string table — keep them in lockstep with
 * tools/coc/coc_parser.py. */
#define be_kv_nil()             { .i = 0 }
#define be_kv_int(_v)           { .i = (bint)(_v) }
#define be_kv_bool(_v)          { .b = (bbool)(_v) }
#define be_kv_real(_hex)        { .p = (void*)(_hex) }
#define be_kv_str(_name)        { .s = (bstring*)&be_const_str_##_name }
#define be_kv_str_weak(_name)   { .s = (bstring*)&be_const_str_##_name }
#define be_kv_str_long(_name)   { .s = (bstring*)&be_const_str_##_name }
#define be_kv_class(_class)     { .c = &(_class) }
#define be_kv_closure(_closure) { .c = &(_closure) }
#define be_kv_comptr(_ptr)      { .c = (const void*)(_ptr) }
#define be_kv_func(_func)       { .nf = (_func) }

/* Compact map value payloads (BE_USE_COMPACT_MAP) for the types that do not
 * occur in a function ktab: a generic pointer payload (nested map / list /
 * simple instance), and a bytes instance. be_kv_bytes_instance is recognized
 * by coc so the referenced bytes literal is registered. */
#define be_kv_ptr(_expr)            { .c = (const void*)(_expr) }
#define be_kv_bytes_instance(_bytes) { .c = &be_const_instance_##_bytes }

/* be_ckv_*: a full compact-map-node value = "<type byte>, <payload>" (two
 * positional elements of bmapnodec: val_type then val_v). These mirror the
 * be_const_* family and are produced by the coc generator (block_builder.py)
 * from `be_const_<x>(...)` -> `be_ckv_<x>(...)`. Keep in lockstep with the
 * be_const_* macros above. */
#define be_ckv_nil()                BE_NIL, { .i = 0 }
#define be_ckv_int(_v)              BE_INT, { .i = (bint)(_v) }
#define be_ckv_var(_v)              BE_INDEX, { .i = (bint)(_v) }
#define be_ckv_real(_v)             BE_REAL, { .r = (breal)(_v) }
#define be_ckv_real_hex(_v)         BE_REAL, { .p = (void*)(_v) }
#define be_ckv_bool(_v)             BE_BOOL, { .b = (bbool)(_v) }
#define be_ckv_str(_s)              BE_STRING, { .s = (bstring*)(_s) }
#define be_ckv_comptr(_v)           BE_COMPTR, { .c = (const void*)(_v) }
#define be_ckv_class(_c)            BE_CLASS, { .c = &(_c) }
#define be_ckv_closure(_c)          BE_CLOSURE, { .c = &(_c) }
#define be_ckv_static_closure(_c)   BE_CLOSURE | BE_STATIC, { .c = &(_c) }
#define be_ckv_func(_f)             BE_NTVFUNC, { .nf = (_f) }
#define be_ckv_static_func(_f)      BE_NTVFUNC | BE_STATIC, { .nf = (_f) }
#define be_ckv_module(_m)           BE_MODULE, { .c = &(_m) }
#define be_ckv_simple_instance(_i)  BE_INSTANCE, { .c = (_i) }
#define be_ckv_map(_m)              BE_MAP, { .c = &(_m) }
#define be_ckv_list(_l)             BE_LIST, { .c = &(_l) }
#define be_ckv_bytes_instance(_b)   BE_INSTANCE, { .c = &be_const_instance_##_b }

#else

#define be_define_const_str_weak(_name, _s, _len)               \
const bcstring be_const_str_##_name = {                         \
    NULL,                                                       \
    BE_STRING,                                                  \
    GC_CONST,                                                   \
    0,                                                          \
    _len,                                                       \
    0,                                                          \
    _s                                                          \
}

#define be_const_key(_str, _next) {                             \
    bvaldata(&be_const_str_##_str),                             \
        BE_STRING,                                              \
        uint32_t((_next)&0xFFFFFF)                              \
}

/* try to use the predefined string in strtab, but don't create an instance if none is present */
/* the behavior is exactly the same as `be_const_key()` but it not detected by coc */
#define be_const_key_weak(_str, _next) {                        \
    bvaldata(&be_const_str_##_str),                             \
        BE_STRING,                                              \
        uint32_t((_next)&0xFFFFFF)                              \
}

#define be_const_key_int(_i, _next) {                           \
    bvaldata(i),                                                \
        BE_INT,                                                 \
        uint32_t((_next)&0xFFFFFF)                              \
}

/* Compact map node (BE_USE_COMPACT_MAP), C++ variant. */
#define be_ckey(_str, _next)                                    \
    bvaldata((const void*)&be_const_str_##_str), BE_STRING, uint32_t((_next)&0xFFFF)

#define be_ckey_weak(_str, _next)                               \
    bvaldata((const void*)&be_const_str_##_str), BE_STRING, uint32_t((_next)&0xFFFF)

#define be_ckey_int(_i, _next)                                  \
    bvaldata(bint(_i)), BE_INT, uint32_t((_next)&0xFFFF)

#define be_const_func(_func) {                                  \
    bvaldata(_func),                                            \
    BE_NTVFUNC                                                  \
}

#define be_const_static_func(_func) {                           \
    bvaldata(_func),                                            \
    BE_NTVFUNC | BE_STATIC                                      \
}

#define be_const_nil() {                                        \
    bvaldata(0),                                                \
    BE_NIL                                                      \
}

#define be_const_int(_val) {                                    \
    bvaldata(bint(_val)),                                       \
    BE_INT                                                      \
}

#define be_const_bool(_val) {                                   \
    bvaldata(bbool(_val)),                                      \
    BE_BOOL                                                     \
}

#define be_const_var(_val) {                                  \
    bvaldata(bint(_val)),                                       \
    BE_INDEX                                                    \
}

#define be_const_real(_val) {                                   \
    bvaldata(breal(_val)),                                      \
    BE_REAL                                                     \
}

#define be_const_real_hex(_val) {                               \
    bvaldata((void*)(_val)),                                    \
    BE_REAL                                                     \
}

#define be_const_comptr(_val) {                                 \
    bvaldata((void*)(_val)),                                    \
    BE_COMPTR                                                   \
}

#define be_const_str(_string) {                                 \
    bvaldata(bstring(_string)),                                 \
    BE_STRING                                                   \
}

#define be_const_class(_class) {                                \
    bvaldata(&(_class)),                                        \
    BE_CLASS                                                    \
}

#define be_const_closure(_closure) {                            \
    bvaldata(&(_closure)),                                      \
    BE_CLOSURE                                                  \
}

#define be_const_static_closure(_closure) {                     \
    bvaldata(&(_closure)),                                      \
    BE_CLOSURE | BE_STATIC                                      \
}

#define be_const_module(_module) {                              \
    bvaldata(&(_module)),                                       \
    BE_MODULE                                                   \
}

#if BE_USE_COMPACT_MAP
#define be_define_const_map_slots(_name)                        \
const bmapnodec _name##_slots[] =
#else
#define be_define_const_map_slots(_name)                        \
const bmapnode _name##_slots[] =
#endif

#define be_define_const_map(_name, _size)                       \
const bmap _name(                                               \
    (bmapnode*)_name##_slots, _size                             \
)

#define be_define_const_class(_name, _nvar, _super, _name_)     \
const bclass _name(                                             \
    _nvar, (bclass*)_super, (bmap*)&_name##_map,                         \
    (bstring*)&be_const_str_##_name_                            \
)

#define be_define_const_empty_class(_name, _super, _name_)      \
const bclass _name(                                             \
    0, (bclass*)_super, NULL,                                            \
    (bstring*)&be_const_str_##_name_                            \
)

#define be_define_const_module(_name, _name_)                   \
const bmodule _name((bmap*)&_name##_map, _name_)

#define be_define_const_vector(_name, _data, _size)             \
const bvector _name = {                                         \
    _size, sizeof(bvalue), _size,                               \
    (void*)_data, (void*)(_data + (_size) - 1)                  \
}

#define be_define_const_native_module(_module)                  \
const bntvmodule_t be_native_module_##_module = {               \
    #_module,                                                   \
    0, 0,                                                       \
    (bmodule*)&(m_lib##_module)                                 \
}

/* Compact constant table (BE_USE_COMPACT_KTAB): payload-word initializers,
 * C++ variant using the union bvaldata constructors. */
#define be_kv_nil()             bvaldata(bint(0))
#define be_kv_int(_v)           bvaldata(bint(_v))
#define be_kv_bool(_v)          bvaldata(bbool(_v))
#define be_kv_real(_hex)        bvaldata((void*)(_hex))
#define be_kv_str(_name)        bvaldata((const void*)&be_const_str_##_name)
#define be_kv_str_weak(_name)   bvaldata((const void*)&be_const_str_##_name)
#define be_kv_str_long(_name)   bvaldata((const void*)&be_const_str_##_name)
#define be_kv_class(_class)     bvaldata((const void*)&(_class))
#define be_kv_closure(_closure) bvaldata((const void*)&(_closure))
#define be_kv_comptr(_ptr)      bvaldata((const void*)(_ptr))
#define be_kv_func(_func)       bvaldata((bntvfunc)(_func))
#define be_kv_ptr(_expr)        bvaldata((const void*)(_expr))
#define be_kv_bytes_instance(_bytes) bvaldata((const void*)&be_const_instance_##_bytes)

/* be_ckv_*: full compact-map-node value "<type byte>, <payload>", C++ variant.
 * Mirrors the be_const_* C++ payloads. */
#define be_ckv_nil()                BE_NIL, bvaldata(bint(0))
#define be_ckv_int(_v)              BE_INT, bvaldata(bint(_v))
#define be_ckv_var(_v)              BE_INDEX, bvaldata(bint(_v))
#define be_ckv_real(_v)             BE_REAL, bvaldata(breal(_v))
#define be_ckv_real_hex(_v)         BE_REAL, bvaldata((void*)(_v))
#define be_ckv_bool(_v)             BE_BOOL, bvaldata(bbool(_v))
#define be_ckv_str(_s)              BE_STRING, bvaldata(bstring(_s))
#define be_ckv_comptr(_v)           BE_COMPTR, bvaldata((void*)(_v))
#define be_ckv_class(_c)            BE_CLASS, bvaldata(&(_c))
#define be_ckv_closure(_c)          BE_CLOSURE, bvaldata(&(_c))
#define be_ckv_static_closure(_c)   BE_CLOSURE | BE_STATIC, bvaldata(&(_c))
#define be_ckv_func(_f)             BE_NTVFUNC, bvaldata((bntvfunc)(_f))
#define be_ckv_static_func(_f)      BE_NTVFUNC | BE_STATIC, bvaldata((bntvfunc)(_f))
#define be_ckv_module(_m)           BE_MODULE, bvaldata(&(_m))
#define be_ckv_simple_instance(_i)  BE_INSTANCE, bvaldata((const void*)(_i))
#define be_ckv_map(_m)              BE_MAP, bvaldata((const void*)&(_m))
#define be_ckv_list(_l)             BE_LIST, bvaldata((const void*)&(_l))
#define be_ckv_bytes_instance(_b)   BE_INSTANCE, bvaldata((const void*)&be_const_instance_##_b)

#endif

/* provide pointers to map and list classes for solidified code */
extern const bclass be_class_list;
extern const bclass be_class_map;
extern const bclass be_class_bytes;

#ifdef __cplusplus
}
#endif

#endif
