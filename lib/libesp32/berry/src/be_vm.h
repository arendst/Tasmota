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

#define comp_is_named_gbl(vm)       ((vm)->compopt & (1<<COMP_NAMED_GBL))
#define comp_set_named_gbl(vm)      ((vm)->compopt |= (1<<COMP_NAMED_GBL))
#define comp_clear_named_gbl(vm)    ((vm)->compopt &= ~(1<<COMP_NAMED_GBL))

#define comp_is_strict(vm)       ((vm)->compopt & (1<<COMP_STRICT))
#define comp_set_strict(vm)      ((vm)->compopt |= (1<<COMP_STRICT))
#define comp_clear_strict(vm)    ((vm)->compopt &= ~(1<<COMP_STRICT))

/* Compilation options */
typedef enum {
    COMP_NAMED_GBL = 0x00, /* compile with named globals */
    COMP_STRICT = 0x01, /* compile with named globals */
} compoptmask;

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
    bctypefunc ctypefunc; /* handler to ctype_func */
    bbyte compopt; /* compilation options */
    bobshook obshook;
#if BE_USE_PERF_COUNTERS
    uint32_t counter_ins; /* instructions counter */
    uint32_t counter_enter; /* counter for times the VM was entered */
    uint32_t counter_call; /* counter for calls, VM or native */
    uint32_t counter_get; /* counter for GETMBR or GETMET */
    uint32_t counter_set; /* counter for SETMBR */
    uint32_t counter_try; /* counter for `try` statement */
    uint32_t counter_exc; /* counter for raised exceptions */
    uint32_t counter_gc_kept; /* counter for objects scanned by last gc */
    uint32_t counter_gc_freed; /* counter for objects freed by last gc */
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
