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

#define comp_is_strict(vm)          ((vm)->compopt & (1<<COMP_STRICT))
#define comp_set_strict(vm)         ((vm)->compopt |= (1<<COMP_STRICT))
#define comp_clear_strict(vm)       ((vm)->compopt &= ~(1<<COMP_STRICT))

#define comp_is_gc_debug(vm)       ((vm)->compopt & (1<<COMP_GC_DEBUG))
#define comp_set_gc_debug(vm)      ((vm)->compopt |= (1<<COMP_GC_DEBUG))
#define comp_clear_gc_debug(vm)    ((vm)->compopt &= ~(1<<COMP_GC_DEBUG))

/* Compilation options */
typedef enum {
    COMP_NAMED_GBL = 0x00,  /* compile with named globals */
    COMP_STRICT = 0x01,     /* compile with named globals */
    COMP_GC_DEBUG = 0x02,   /* compile with gc debug */
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

struct gc16_t;           /* memory pool for 0-16 bytes or less objects */
struct gc32_t;           /* memory pool for 17-32 bytes */
struct bgc {
    bgcobject *list; /* the GC-object list */
    bgcobject *gray; /* the gray object list */
    bgcobject *fixed; /* the fixed objecct list  */
    struct gc16_t* pool16;
    struct gc32_t* pool32;
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
    struct bgc gc;
    bctypefunc ctypefunc; /* handler to ctype_func */
    bbyte compopt; /* compilation options */
    int32_t bytesmaxsize; /* max allowed size for bytes() object, default 32kb but can be increased */
    bobshook obshook;
    bmicrosfnct microsfnct; /* fucntion to get time as a microsecond resolution */
#if BE_USE_PERF_COUNTERS
    uint32_t counter_ins; /* instructions counter */
    uint32_t counter_enter; /* counter for times the VM was entered */
    uint32_t counter_call; /* counter for calls, VM or native */
    uint32_t counter_get; /* counter for GETMBR or GETMET */
    uint32_t counter_set; /* counter for SETMBR */
    uint32_t counter_get_global; /* counter for GETNBGL */
    uint32_t counter_try; /* counter for `try` statement */
    uint32_t counter_exc; /* counter for raised exceptions */
    uint32_t counter_gc_kept; /* counter for objects scanned by last gc */
    uint32_t counter_gc_freed; /* counter for objects freed by last gc */
    uint32_t counter_mem_alloc; /* counter for memory allocations */
    uint32_t counter_mem_free; /* counter for memory frees */
    uint32_t counter_mem_realloc; /* counter for memory reallocations */

    uint32_t micros_gc0;
    uint32_t micros_gc1;
    uint32_t micros_gc2;
    uint32_t micros_gc3;
    uint32_t micros_gc4;
    uint32_t micros_gc5;

    uint32_t gc_mark_string;
    uint32_t gc_mark_class;
    uint32_t gc_mark_proto;
    uint32_t gc_mark_instance;
    uint32_t gc_mark_map;
    uint32_t gc_mark_list;
    uint32_t gc_mark_closure;
    uint32_t gc_mark_ntvclos;
    uint32_t gc_mark_module;
    uint32_t gc_mark_comobj;
#endif
#if BE_USE_DEBUG_HOOK
    bvalue hook;
    bbyte hookmask;
#endif
};

#define NONE_FLAG           0
#define BASE_FRAME          (1 << 0)
#define PRIM_FUNC           (1 << 1)

int be_default_init_native_function(bvm *vm);
void be_dofunc(bvm *vm, bvalue *v, int argc);
bbool be_value2bool(bvm *vm, bvalue *v);
bbool be_vm_iseq(bvm *vm, bvalue *a, bvalue *b);
bbool be_vm_isneq(bvm *vm, bvalue *a, bvalue *b);
bbool be_vm_islt(bvm *vm, bvalue *a, bvalue *b);
bbool be_vm_isle(bvm *vm, bvalue *a, bvalue *b);
bbool be_vm_isgt(bvm *vm, bvalue *a, bvalue *b);
bbool be_vm_isge(bvm *vm, bvalue *a, bvalue *b);

#endif
