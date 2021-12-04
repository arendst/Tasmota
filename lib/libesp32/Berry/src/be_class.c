/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#include "be_class.h"
#include "be_string.h"
#include "be_vector.h"
#include "be_map.h"
#include "be_exec.h"
#include "be_gc.h"
#include "be_vm.h"
#include "be_func.h"
#include "be_var.h"
#include <string.h>

#define check_members(vm, c)            \
    if (!(c)->members) {                \
        (c)->members = be_map_new(vm);  \
    }

bclass* be_newclass(bvm *vm, bstring *name, bclass *super)
{
    bgcobject *gco = be_gcnew(vm, BE_CLASS, bclass);
    bclass *obj = cast_class(gco);
    bvalue *buf = be_incrtop(vm); /* protect new objects from GC */
    var_setclass(buf, obj);
    if (obj) {
        obj->super = super;
        obj->members = NULL; /* gc protection */
        obj->nvar = 0;
        obj->name = name;
    }
    be_stackpop(vm, 1);
    return obj;
}

void be_class_compress(bvm *vm, bclass *c)
{
    if (!gc_isconst(c) && c->members) {
        be_map_compact(vm, c->members); /* clear space */
    }
}

int be_class_attribute(bvm *vm, bclass *c, bstring *attr)
{
    for (; c; c = c->super) {
        if (c->members) {
            bvalue *v = be_map_findstr(vm, c->members, attr);
            if (v) {
                return var_type(v);
            }
        }
    }
    return BE_NONE;
}

void be_member_bind(bvm *vm, bclass *c, bstring *name, bbool var)
{
    bvalue *attr;
    set_fixed(name);
    check_members(vm, c);
    attr = be_map_insertstr(vm, c->members, name, NULL);
    restore_fixed(name);
    if (var) {
        /* this is an instance variable so we set it as MT_VARIABLE */
        attr->v.i = c->nvar++;
        attr->type = MT_VARIABLE;
    } else {
        /* this is a static class constant, leave it as BE_NIL */
        attr->v.i = 0;
        attr->type = BE_NIL;
    }
}

void be_method_bind(bvm *vm, bclass *c, bstring *name, bproto *p, bbool is_static)
{
    bclosure *cl;
    bvalue *attr;
    set_fixed(name);
    check_members(vm, c);
    attr = be_map_insertstr(vm, c->members, name, NULL);
    restore_fixed(name);
    var_setnil(attr);
    cl = be_newclosure(vm, p->nupvals);
    cl->proto = p;
    var_setclosure(attr, cl);
    if (is_static) {
        func_setstatic(attr);
    }
}

void be_prim_method_bind(bvm *vm, bclass *c, bstring *name, bntvfunc f)
{
    bvalue *attr;
    set_fixed(name);
    check_members(vm, c);
    attr = be_map_insertstr(vm, c->members, name, NULL);
    restore_fixed(name);
    attr->v.nf = f;
    attr->type = MT_PRIMMETHOD;
}

void be_closure_method_bind(bvm *vm, bclass *c, bstring *name, bclosure *cl)
{
    bvalue *attr;
    check_members(vm, c);
    attr = be_map_insertstr(vm, c->members, name, NULL);
    attr->v.gc = (bgcobject*) cl;
    attr->type = MT_METHOD;
}

/* get the closure method count that need upvalues */
int be_class_closure_count(bclass *c)
{
    int count = 0;
    if (c->members) {
        bmapnode *node;
        bmap *members = c->members;
        bmapiter iter = be_map_iter();
        while ((node = be_map_next(members, &iter)) != NULL) {
            if (var_isproto(&node->value)) {
                ++count;
            }
        }
    }
    return count;
}

static binstance* instance_member(bvm *vm,
    binstance *obj, bstring *name, bvalue *dst)
{
    for (; obj; obj = obj->super) {
        bmap *members = obj->_class->members;
        if (members) {
            bvalue *v = be_map_findstr(vm, members, name);
            if (v) {
                *dst = *v;
                return obj;
            }
        }
    }
    var_setnil(dst);
    return NULL;
}

static bclass* class_member(bvm *vm,
    bclass *obj, bstring *name, bvalue *dst)
{
    for (; obj; obj = obj->super) {
        bmap *members = obj->members;
        if (members) {
            bvalue *v = be_map_findstr(vm, members, name);
            if (v) {
                *dst = *v;
                return obj;
            }
        }
    }
    var_setnil(dst);
    return NULL;
}

void be_class_upvalue_init(bvm *vm, bclass *c)
{
    bmap *mbr = c->members;
    if (mbr != NULL) {
        bmapnode *node;
        bmapiter iter = be_map_iter();
        while ((node = be_map_next(mbr, &iter)) != NULL) {
            if (var_isclosure(&node->value)) {
                bclosure *cl = var_toobj(&node->value);
                if (cl->proto->nupvals) {
                    /* initialize the closure's upvalues */
                    be_release_upvalues(vm, cl);
                    be_initupvals(vm, cl);
                }
            }
        }
    }
}

/* (internal) Instanciate an instance for a single class and initialize variables to nil */
static binstance* newobjself(bvm *vm, bclass *c)
{
    size_t size = sizeof(binstance) + sizeof(bvalue) * (c->nvar - 1);
    bgcobject *gco = be_newgcobj(vm, BE_INSTANCE, size);
    binstance *obj = cast_instance(gco);
    be_assert(obj != NULL);
    if (obj) { /* initialize members */
        bvalue *v = obj->members, *end = v + c->nvar;  /* instance variables is a simple array of pointers at obj->members of size c->nvar */
        while (v < end) { var_setnil(v); ++v; }  /* Initialize all instance variables to `nil` */
        obj->_class = c;  /* set its class object */
        obj->super = NULL;  /* no super class instance for now */
        obj->sub = NULL;  /* no subclass instance for now */
    }
    return obj;
}

/* (internal) Instanciate the whole chain of instances when there is a class hierarchy */
/* All variables set to nil, constructors are not called here */
static binstance* newobject(bvm *vm, bclass *c)
{
    binstance *obj, *prev;
    be_assert(c != NULL);
    obj = prev = newobjself(vm, c);
    var_setinstance(vm->top, obj);
    be_incrtop(vm); /* protect new objects from GC */
    for (c = c->super; c; c = c->super) {  /* initialize one instance object per class and per superclass */
        prev->super = newobjself(vm, c);
        prev->super->sub = prev;  /* link the super/sub classes instances */
        prev = prev->super;
    }
    be_stackpop(vm, 1);
    return obj;
}

/* Instanciate new instance from stack with argc parameters */
/* Pushes the constructor on the stack to be executed if a construtor is found */
/* Returns true if a constructor is found */
bbool be_class_newobj(bvm *vm, bclass *c, int pos, int argc, int mode)
{
    bvalue init;
    binstance *obj = newobject(vm, c);  /* create empty object hierarchy from class hierarchy */
    // reg = vm->reg + pos - mode; /* the stack may have changed, mode=1 when class is instanciated from module #104 */
    var_setinstance(vm->reg + pos, obj);
    var_setinstance(vm->reg + pos - mode, obj);  /* copy to reg and reg+1 if mode==1 */
    /* find constructor */
    obj = instance_member(vm, obj, str_literal(vm, "init"), &init);
    if (obj && var_type(&init) != MT_VARIABLE) {
        /* copy argv */
        bvalue * reg;
        for (reg = vm->reg + pos + 1; argc > 0; --argc) {
            reg[argc] = reg[argc - 2];
        }
        *reg = init; /* set constructor */
        return btrue;
    }
    return bfalse;
}

/* Default empty constructor */
static int default_init_native_method(bvm *vm) {
    be_return_nil(vm);
}

/* Find instance member by name and copy value to `dst` */
/* Do not look into virtual members */
int be_instance_member_simple(bvm *vm, binstance *instance, bstring *name, bvalue *dst)
{
    int type;
    be_assert(name != NULL);
    binstance * obj = instance_member(vm, instance, name, dst);
    type = var_type(dst);
    if (obj && type == MT_VARIABLE) {
        *dst = obj->members[dst->v.i];
    }
    return type;
}

/* Find instance member by name and copy value to `dst` */
/* Input: none of `obj`, `name` and `dst` may not be NULL */
/* Returns the type of the member or BE_NONE if member not found */
/* TODO need to support synthetic members */
int be_instance_member(bvm *vm, binstance *instance, bstring *name, bvalue *dst)
{
    int type;
    be_assert(name != NULL);
    binstance * obj = instance_member(vm, instance, name, dst);
    type = var_type(dst);
    if (obj && type == MT_VARIABLE) {
        *dst = obj->members[dst->v.i];
    }
    if (obj) {
        return type;
    } else {  /* if no method found, try virtual */
        /* if 'init' does not exist, create a virtual empty constructor */
        if (strcmp(str(name), "init") == 0) {
            var_setntvfunc(dst, default_init_native_method);
            return var_type(dst);
        } else {
            /* get method 'member' */
            obj = instance_member(vm, instance, str_literal(vm, "member"), vm->top);
            if (obj && basetype(var_type(vm->top)) == BE_FUNCTION) {
                bvalue *top = vm->top;
                var_setinstance(&top[1], instance);
                var_setstr(&top[2], name);
                vm->top += 3;   /* prevent gc collection results */
                be_dofunc(vm, top, 2); /* call method 'member' */
                vm->top -= 3;
                *dst = *vm->top;   /* copy result to R(A) */
                if (obj && var_type(dst) == MT_VARIABLE) {
                    *dst = obj->members[dst->v.i];
                }
                type = var_type(dst);
                if (type != BE_NIL) {
                    return type;
                }
            }
        }
    }
    return BE_NONE;
}

int be_class_member(bvm *vm, bclass *obj, bstring *name, bvalue *dst)
{
    int type;
    be_assert(name != NULL);
    obj = class_member(vm, obj, name, dst);
    type = var_type(dst);
    if (obj) {
        return type;
    } else {
        return BE_NONE;
    }
}

bbool be_instance_setmember(bvm *vm, binstance *o, bstring *name, bvalue *src)
{
    bvalue v;
    be_assert(name != NULL);
    binstance * obj = instance_member(vm, o, name, &v);
    if (obj && var_istype(&v, MT_VARIABLE)) {
        obj->members[var_toint(&v)] = *src;
        return btrue;
    } else {
        obj = instance_member(vm, o, str_literal(vm, "setmember"), &v);
        if (obj && var_type(&v) == MT_VARIABLE) {
            v = obj->members[v.v.i];
        }
        if (var_basetype(&v) == BE_FUNCTION) {
            bvalue *top = vm->top;
            var_setval(top, &v);
            var_setinstance(top + 1, o); /* move instance to argv[0] */
            var_setstr(top + 2, name); /* move method name to argv[1] */
            var_setval(top + 3, src); /* move method name to argv[1] */
            vm->top += 4;   /* prevent collection results */
            be_dofunc(vm, top, 3); /* call method 'member' */
            vm->top -= 4;
            return btrue;
        }
    }
    return bfalse;
}

bbool be_class_setmember(bvm *vm, bclass *o, bstring *name, bvalue *src)
{
    bvalue v;
    be_assert(name != NULL);
    if (!gc_isconst(o)) {
        bclass * obj = class_member(vm, o, name, &v);
        if (obj && !var_istype(&v, MT_VARIABLE)) {
            be_map_insertstr(vm, obj->members, name, src);
            return btrue;
        }
    }
    return bfalse;
}
