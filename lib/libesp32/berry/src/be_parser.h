/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#ifndef BE_PARSER_H
#define BE_PARSER_H

#include "be_object.h"
#include "be_string.h"

typedef enum {
    ETVOID,    /* unknown (new variable or error) */
    ETNIL,
    ETBOOL,
    ETREAL,
    ETINT,
    ETSTRING,
    ETPROTO,
    ETCONST,
    ETLOCAL,    /* local variable, allocated until end of scope */
    ETGLOBAL,   /* global by index number */
    ETUPVAL,
    ETMEMBER,   /* member accessor (by name) */
    ETINDEX,    /* index accessor (ex array index) */
    ETREG,      /* temporary register, can be freed if top of stack */
    ETNGLOBAL   /* named global */
} exptype_t;

typedef struct {
    union {
        struct { /* for suffix */
            unsigned int idx:9; /* suffix RK index */
            unsigned int obj:9; /* object RK index */
            unsigned int tt:5; /* object type */
        } ss;
        breal r;     /* for ETREAL */
        bint i;      /* for ETINT */
        bstring *s;  /* for ETSTRING */
        bproto *p;  /* for ETPROTO */
        int idx;     /* variable index */
    } v;
    int t; /* patch list of 'exit when true' */
    int f; /* patch list of 'exit when false' */
    bbyte not; /* not mark */
    bbyte type;
} bexpdesc;

typedef struct bblockinfo {
    struct bblockinfo *prev;
    bbyte nactlocals; /* number of active local variables */
    bbyte type;       /* block type mask */
    bbyte hasupval;   /* has upvalue mark */
    bbyte sideeffect; /* did the last expr/statement had a side effect */
    int lastjmp;      /* pc for the last jump, prevents false register optimizations */ 
    int breaklist;    /* break list */
    int beginpc;      /* begin pc */
    int continuelist; /* continue list */
} bblockinfo;

typedef struct bfuncinfo {
    struct bfuncinfo *prev; /* outer function */
    bproto *proto; /* the function prototype */
    bblockinfo *binfo; /* block information */
    struct blexer *lexer; /* the lexer pointer */
    blist *local; /* local variable */
    bmap *upval; /* upvalue variable */
    bvector code; /* code vector */
    bvector kvec; /* constants table */
    bvector pvec; /* proto table */
#if BE_DEBUG_RUNTIME_INFO /* debug information */
    bvector linevec;
#endif
#if BE_DEBUG_VAR_INFO
    bvector varvec;
#endif
    int pc; /* program count */
    bbyte freereg; /* first free register */
    bbyte flags; /* some flages */
} bfuncinfo;

/* code block type definitions */
#define BLOCK_LOOP      1
#define BLOCK_EXCEPT    2

bclosure *be_parser_source(bvm *vm,
    const char *fname, breader reader, void *data, bbool islocal);

#endif
