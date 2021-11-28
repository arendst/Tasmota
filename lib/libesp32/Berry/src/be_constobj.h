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

#ifndef __cplusplus

#define be_const_header(_t)                                     \
    .next = NULL,                                               \
    .type = (_t),                                               \
    .marked = GC_CONST

#define be_const_key(_str, _next) {                             \
    .v.c = &be_const_str_##_str,                                \
    .type = BE_STRING,                                          \
    .next = (uint32_t)(_next) & 0xFFFFFF                        \
}

#define be_const_func(_func) {                                  \
    .v.nf = (_func),                                            \
    .type = BE_FUNCTION                                         \
}

#define be_const_static_func(_func) {                           \
    .v.nf = (_func),                                            \
    .type = BE_FUNCTION | BE_FUNC_STATIC                        \
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
    .v.c = (const void*)(_val),                                       \
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
    .type = BE_CLOSURE | BE_FUNC_STATIC                         \
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

#define be_define_const_map_slots(_name)                        \
const bmapnode _name##_slots[] =

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
const bntvmodule be_native_module(_module) = {                  \
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

// #define be_local_instance(_name, _class_ptr, _members)          \
//   static const binstance i_##_name = {                          \
//     be_const_header(BE_INSTANCE),                               \
//     .super = NULL,                                              \
//     .sub = NULL,                                                \
//     ._class = (bclass*) _class_ptr,                             \
//     .members = _members                                         \
//   }

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

#define be_nested_str_literal(_str)                             \
  {                                                             \
    { .s=(be_nested_const_str(_str, 0, sizeof(_str)-1 ))        \
    },                                                          \
    BE_STRING                                                   \
  }

#define be_str_literal(_str)                                    \
  be_nested_const_str(_str, 0, sizeof(_str)-1 )

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

#else

#define be_const_key(_str, _next) {                             \
    bvaldata(&be_const_str_##_str),                             \
        BE_STRING,                                              \
        uint32_t((_next)&0xFFFFFF)                              \
}

#define be_const_func(_func) {                                  \
    bvaldata(_func),                                            \
    BE_FUNCTION                                                 \
}

#define be_const_static_func(_func) {                           \
    bvaldata(_func),                                            \
    BE_FUNCTION | BE_FUNC_STATIC                                \
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
    BE_CLOSURE | BE_FUNC_STATIC                                 \
}

#define be_const_module(_module) {                              \
    bvaldata(&(_module)),                                       \
    BE_MODULE                                                   \
}

#define be_define_const_map_slots(_name)                        \
const bmapnode _name##_slots[] =

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
const bntvmodule be_native_module(_module) = {                  \
    #_module,                                                   \
    0, 0,                                                       \
    (bmodule*)&(m_lib##_module)                                 \
}

#endif

/* provide pointers to map and list classes for solidified code */
extern const bclass be_class_list;
extern const bclass be_class_map;

#ifdef __cplusplus
}
#endif

#endif
