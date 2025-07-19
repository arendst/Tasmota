/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#ifndef BE_EXEC_H
#define BE_EXEC_H

#include "be_object.h"
#include <setjmp.h>

/* protected-call function */
typedef void (*bpfunc)(bvm *vm, void *data);

#if BE_DEBUG
bvalue* be_incrtop(bvm *vm);
#else
/* increase top register */
#define be_incrtop(vm)          ((vm)->top++)
#endif

#define be_stackpop(vm, n)      ((vm)->top -= (n))

/* in MinGW-w64, setjmp / longjmp may be broken,
 * so here is replaced by __builtin version. */
#if defined(__GNUC__) && defined(__MINGW32__)
  #define be_setjmp(env)        __builtin_setjmp(env)
  #define be_longjmp(env, v)    __builtin_longjmp(env, v)
#else
  #define be_setjmp(env)        setjmp(env)
  #define be_longjmp(env, v)    longjmp(env, v)
#endif

typedef jmp_buf bjmpbuf;

struct blongjmp {
    bjmpbuf b;
    struct blongjmp *prev;
    volatile int status; /* error code */
};

struct bexecptframe {
    struct blongjmp errjmp; /* long jump information */
    int depth; /* function call stack depth */
    binstruction *ip; /* instruction pointer */
    int refcount; /* save object reference stack */
};

void be_throw(bvm *vm, int errorcode);
int be_execprotected(bvm *vm, bpfunc f, void *data);
int be_protectedparser(bvm *vm, const char *fname,
    breader reader, void *data, bbool islocal);
int be_protectedcall(bvm *vm, bvalue *v, int argc);
void be_stackpush(bvm *vm);
void be_stack_expansion(bvm *vm, int n);
void be_except_block_setup(bvm *vm);
void be_except_block_resume(bvm *vm);
void be_except_block_close(bvm *vm, int count);
void be_save_stacktrace(bvm *vm);

#endif
