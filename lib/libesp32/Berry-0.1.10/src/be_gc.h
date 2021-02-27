/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#ifndef BE_GC_H
#define BE_GC_H

#include "be_object.h"

#define BE_GCOBJECT         BE_STRING

#define gc_object(o)        cast(bgcobject*, o)
#define gc_cast(o, t, T)    ((o) && pgm_read_byte(&(o)->type) == (t) ? (T*)(o) : NULL)
#define cast_proto(o)       gc_cast(o, BE_PROTO, bproto)
#define cast_closure(o)     gc_cast(o, BE_CLOSURE, bclosure)
#define cast_ntvclos(o)     gc_cast(o, BE_NTVCLOS, bntvclos)
#define cast_str(o)         gc_cast(o, BE_STRING, bstring)
#define cast_class(o)       gc_cast(o, BE_CLASS, bclass)
#define cast_instance(o)    gc_cast(o, BE_INSTANCE, binstance)
#define cast_map(o)         gc_cast(o, BE_MAP, bmap)
#define cast_list(o)        gc_cast(o, BE_LIST, blist)
#define cast_module(o)      gc_cast(o, BE_MODULE, bmodule)

#define gc_ismark(o, m)     (((o)->marked & 0x03) == m)
#define gc_iswhite(o)       gc_ismark((o), GC_WHITE)
#define gc_isgray(o)        gc_ismark((o), GC_GRAY)
#define gc_isdark(o)        gc_ismark((o), GC_DARK)

#define gc_setmark(o, m) \
if (!gc_isconst(o)) { \
    (o)->marked &= ~0x03; \
    (o)->marked |= (m) & 0x03; \
}

#define gc_setwhite(o)      gc_setmark((o), GC_WHITE)
#define gc_setgray(o)       gc_setmark((o), GC_GRAY)
#define gc_setdark(o)       gc_setmark((o), GC_DARK)
#define gc_isfixed(o)       ((pgm_read_byte(&(o)->marked) & GC_FIXED) != 0)
#define gc_setfixed(o)      ((o)->marked |= GC_FIXED)
#define gc_clearfixed(o)    ((o)->marked &= ~GC_FIXED)
#define gc_isconst(o)       ((pgm_read_byte(&(o)->marked) & GC_CONST) != 0)
#define gc_exmark(o)        ((pgm_read_byte(&(o)->marked) >> 4) & 0x0F)
#define gc_setexmark(o, k)  ((o)->marked |= (k) << 4)

#define be_isgctype(t)      ((t) >= BE_GCOBJECT)
#define be_isgcobj(o)       be_isgctype(var_type(o))
#define be_gcnew(v, t, s)   be_newgcobj((v), (t), sizeof(s))

/* the GC mark uses bit4:0 of the `object->marked` field,
 * so other bits can be used for special flags (ex-mark). */
typedef enum {
    GC_WHITE = 0x00, /* unreachable object */
    GC_GRAY = 0x01,  /* unscanned object */
    GC_DARK = 0x02,  /* scanned object */
    GC_FIXED = 0x04, /* disable collection mark */
    GC_CONST = 0x08  /* constant object mark */
} bgcmark;

void be_gc_init(bvm *vm);
void be_gc_deleteall(bvm *vm);
void be_gc_setsteprate(bvm *vm, int rate);
void be_gc_setpause(bvm *vm, int pause);
size_t be_gc_memcount(bvm *vm);
bgcobject *be_newgcobj(bvm *vm, int type, size_t size);
bgcobject* be_gc_newstr(bvm *vm, size_t size, int islong);
void be_gc_fix(bvm *vm, bgcobject *obj);
void be_gc_unfix(bvm *vm, bgcobject *obj);
void be_gc_collect(bvm *vm);
void be_gc_auto(bvm *vm);

#endif
