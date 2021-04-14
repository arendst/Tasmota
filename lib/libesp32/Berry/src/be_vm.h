/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#ifndef BE_VM_H
#define BE_VM_H

#include "be_object.h"

typedef struct {
    struct {
        bmap *vtab; /* global variable index table */
        bvector vlist; /* global variable list */
    } global;
    struct {
        bmap *vtab; /* built-in variable index table */
        bvector vlist; /* built-in variable list */
    } builtin;
} bglobaldesc;

typedef struct {
    bvalue *func; /* function register pointer */
    bvalue *top; /* top register pointer */
    bvalue *reg; /* base register pointer */
    binstruction *ip; /* instruction pointer (only berry-function) */
#if BE_USE_DEBUG_HOOK
    blineinfo *lineinfo;
#endif
    int status;
} bcallframe;

struct bgc {
    bgcobject *list; /* the GC-object list */
    bgcobject *gray; /* the gray object list */
    bgcobject *fixed; /* the fixed objecct list  */
    size_t usage; /* the count of bytes currently allocated */
    size_t threshold; /* he threshold of allocation for the next GC */
    bbyte steprate; /* the rate of increase in the distribution between two GCs (percentage) */
    bbyte status;
};

struct bstringtable {
    bstring **table;
    int count; /* string count */
    int size;
};

struct bmoduledesc {
    bmap *loaded; /* loaded module map */
    blist *path; /* module load path list */
};

typedef struct {
    bvalue func;
    binstruction *ip;
} bcallsnapshot;

struct bupval {
    bvalue* value;
    union {
        bvalue value;
        struct bupval* next;
    } u;
    int refcnt;
};

struct bvm {
    bglobaldesc gbldesc; /* global description */
    bvalue *stack; /* stack space */
    bvalue *stacktop; /* stack top register */
    bupval *upvalist; /* open upvalue list */
    bstack callstack; /* function call stack */
    bstack exceptstack; /* exception stack */
    bcallframe *cf; /* function call frame */
    bvalue *reg; /* function base register */
    bvalue *top; /* function top register */
    binstruction *ip; /* function instruction pointer */
    struct blongjmp *errjmp; /* error jump point */
    bstack refstack; /* object reference stack */
    struct bmoduledesc module; /* module description */
    struct bstringtable strtab; /* short string table */
    bstack tracestack; /* call state trace-stack */
    bmap *ntvclass; /* native class table */
    blist *registry; /* registry list */
    struct bgc gc;
#if BE_USE_OBSERVABILITY_HOOK
    bobshook obshook;
#endif
#if BE_USE_DEBUG_HOOK
    bvalue hook;
    bbyte hookmask;
#endif
};

#define NONE_FLAG           0
#define BASE_FRAME          (1 << 0)
#define PRIM_FUNC           (1 << 1)

void be_dofunc(bvm *vm, bvalue *v, int argc);
bbool be_value2bool(bvm *vm, bvalue *v);
bbool be_vm_iseq(bvm *vm, bvalue *a, bvalue *b);
bbool be_vm_isneq(bvm *vm, bvalue *a, bvalue *b);
bbool be_vm_islt(bvm *vm, bvalue *a, bvalue *b);
bbool be_vm_isle(bvm *vm, bvalue *a, bvalue *b);
bbool be_vm_isgt(bvm *vm, bvalue *a, bvalue *b);
bbool be_vm_isge(bvm *vm, bvalue *a, bvalue *b);

#endif
