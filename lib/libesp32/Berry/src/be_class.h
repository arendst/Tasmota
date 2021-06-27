/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#ifndef BE_CLASS_H
#define BE_CLASS_H

#include "be_object.h"

#define MT_VARIABLE                     BE_INT
#define MT_METHOD                       BE_CLOSURE
#define MT_PRIMMETHOD                   BE_NTVFUNC

#define be_class_name(cl)               ((cl)->name)
#define be_class_members(cl)            ((cl)->members)
#define be_class_super(cl)              ((cl)->super)
#define be_class_setsuper(self, sup)    ((self)->super = (sup))
#define be_class_setsub(self, sub)      ((self)->sub = (sub))
#define be_instance_name(obj)           ((obj)->_class->name)
#define be_instance_class(obj)          ((obj)->_class)
#define be_instance_members(obj)        ((obj)->members)
#define be_instance_member_count(obj)   ((obj)->_class->nvar)
#define be_instance_super(obj)          ((obj)->super)
#define be_instance_sub(obj)            ((obj)->sub)

struct bclass {
    bcommon_header;
    uint16_t nvar; /* members variable data field count */
    struct bclass *super;
    bmap *members;
    bstring *name;
    bgcobject *gray; /* for gc gray list */
#ifdef __cplusplus
    BE_CONSTEXPR bclass(uint16_t nv, bclass *sup, bmap *mem, bstring *s) :
        next(0), type(BE_CLASS), marked(GC_CONST), nvar(nv),
        super(sup), members(mem), name(s), gray(0) {}
#endif
};

struct binstance {
    bcommon_header;
    struct binstance *super;
    struct binstance *sub;
    bclass *_class;
    bgcobject *gray; /* for gc gray list */
    bvalue members[1]; /* members variable data field */
};

bclass* be_newclass(bvm *vm, bstring *name, bclass *super);
void be_class_compress(bvm *vm, bclass *c);
int be_class_attribute(bvm *vm, bclass *c, bstring *attr);
void be_member_bind(bvm *vm, bclass *c, bstring *name);
void be_method_bind(bvm *vm, bclass *c, bstring *name, bproto *p);
void be_prim_method_bind(bvm *vm, bclass *c, bstring *name, bntvfunc f);
void be_closure_method_bind(bvm *vm, bclass *c, bstring *name, bclosure *cl);
int be_class_closure_count(bclass *c);
void be_class_upvalue_init(bvm *vm, bclass *c);
bbool be_class_newobj(bvm *vm, bclass *c, bvalue *argv, int argc, int mode);
int be_instance_member(bvm *vm, binstance *obj, bstring *name, bvalue *dst);
bbool be_instance_setmember(bvm *vm, binstance *obj, bstring *name, bvalue *src);

#endif
