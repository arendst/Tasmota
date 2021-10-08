/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#include "be_object.h"
#include "be_exec.h"
#include "be_mem.h"
#include "be_gc.h"
#include "be_vm.h"

#define cast_comobj(o)      gc_cast(o, BE_COMOBJ, bcommomobj)

const char* be_vtype2str(bvalue *v)
{
    switch(var_type(v)) {
    case BE_NIL: return "nil";
    case BE_INT: return "int";
    case BE_REAL: return "real";
    case BE_BOOL: return "bool";
    case BE_CLOSURE: case BE_NTVCLOS:
    case BE_NTVFUNC: return "function";
    case BE_PROTO: return "proto";
    case BE_CLASS: return "class";
    case BE_STRING: return "string";
    case BE_LIST: return "list";
    case BE_MAP: return "map";
    case BE_INSTANCE: return "instance";
    case BE_MODULE: return "module";
    case BE_INDEX: return "var";
    case BE_COMPTR: return "ptr";
    default: return "invalid type";
    }
}

bvalue* be_indexof(bvm *vm, int idx)
{
    if (idx > 0) { /* absolute index */
        be_assert(vm->reg + idx <= vm->top);
        return vm->reg + idx - 1;
    }
    /* relative index */
    be_assert(vm->top + idx >= vm->reg);
    return vm->top + idx;
}

BERRY_API void be_newcomobj(bvm *vm, void *data, bntvfunc destory)
{
    bcommomobj *obj;
    bgcobject *gco = be_gcnew(vm, BE_COMOBJ, bcommomobj);
    if ((obj = cast_comobj(gco)) != NULL) {
        bvalue* top = be_incrtop(vm);
        obj->data = data;
        obj->destory = destory;
        var_setobj(top, BE_COMOBJ, obj);
    }
}

void be_commonobj_delete(bvm *vm, bgcobject *obj)
{
    bcommomobj *co = cast_comobj(obj);
    if (co) {
        if (co->destory && co->data) {
            be_pushntvfunction(vm, co->destory);
            be_pushcomptr(vm, co->data);
            be_call(vm, 1);
            be_pop(vm, 2);
        }
        be_free(vm, co, sizeof(bcommomobj));
    }
}
