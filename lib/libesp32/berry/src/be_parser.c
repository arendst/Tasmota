/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#include "be_parser.h"
#include "be_lexer.h"
#include "be_vector.h"
#include "be_mem.h"
#include "be_vm.h"
#include "be_map.h"
#include "be_list.h"
#include "be_var.h"
#include "be_code.h"
#include "be_string.h"
#include "be_func.h"
#include "be_class.h"
#include "be_decoder.h"
#include "be_exec.h"
#include <limits.h>

#define OP_NOT_BINARY           TokenNone
#define OP_NOT_UNARY            TokenNone
#define OP_NOT_ASSIGN           TokenNone
#define UNARY_OP_PRIO           3
#define ASSIGN_OP_PRIO          16

#define FUNC_METHOD             1
#define FUNC_ANONYMOUS          2
#define FUNC_STATIC             4

#if BE_INTGER_TYPE == 0 /* int */
  #define M_IMAX    INT_MAX
  #define M_IMIN    INT_MIN
#elif BE_INTGER_TYPE == 1 /* long */
  #define M_IMAX    LONG_MAX
  #define M_IMIN    LONG_MIN
#else /* int64_t (long long) */
  #define M_IMAX    LLONG_MAX
  #define M_IMIN    LLONG_MIN
#endif

/* get binary operator priority */
#define binary_op_prio(op)      (binary_op_prio_tab[cast_int(op) - OptAdd])

#define scan_next_token(parser) (be_lexer_scan_next(&(parser)->lexer))
#define next_token(parser)      ((parser)->lexer.token)
#define next_type(parser)       (next_token(parser).type)
#define max(a, b)               ((a) > (b) ? (a) : (b))
#define token2str(parser)       be_token2str((parser)->vm, &next_token(parser))
#define funcname(parser)        ((parser)->islocal ? "loader" : "main")

#define upval_index(v)          ((v) & 0xFF)
#define upval_target(v)         ((bbyte)(((v) >> 8) & 0xFF))
#define upval_instack(v)        ((bbyte)(((v) >> 16) != 0))
#define upval_desc(i, t, s)     (((i) & 0xFF) | (((t) & 0xFF) << 8) \
                                | (((s) != 0) << 16))

#define match_id(parser, s)     ((s) = _match_id(parser))

#define parser_newstr(p, str)   be_lexer_newstr(&(p)->lexer, (str))

#define parser_error(p, msg)    be_lexerror(&(p)->lexer, msg)

#define push_error(parser, ...) \
    parser_error(parser, be_pushfstring(parser->vm, __VA_ARGS__))

typedef struct {
    blexer lexer;
    bvm *vm;
    bfuncinfo *finfo;
    bclosure *cl;
    bbyte islocal;
} bparser;

#if BE_USE_SCRIPT_COMPILER

static void stmtlist(bparser *parser);
static void block(bparser *parser, int type);
static void expr(bparser *parser, bexpdesc *e);
static void walrus_expr(bparser *parser, bexpdesc *e);
static void sub_expr(bparser *parser, bexpdesc *e, int prio);

static const bbyte binary_op_prio_tab[] = {
    5, 5, 4, 4, 4, /* + - * / % */
    11, 11, 12, 12, 11, 11, /* < <= == != > >= */
    7, 9, 8, 6, 6, 10, 13, 14 /*  & | ^ << >> .. && || */
};

static void match_token(bparser *parser, btokentype type)
{
    if (next_type(parser) != type) { /* error when token is no match */
        btoken *token = &next_token(parser);
        const char *s1 = be_tokentype2str(type);
        const char *s2 = be_token2str(parser->vm, token);
        push_error(parser, "expected '%s' before '%s'", s1, s2);
    }
    scan_next_token(parser); /* skip this token */
}

/* Check that the next token is not of type `type` */
/* or raise an exception */
static void match_notoken(bparser *parser, btokentype type)
{
    if (next_type(parser) == type) { /* error when token is match */
        push_error(parser,
            "expected statement before '%s'", token2str(parser));
    }
}

/* check that if the expdesc is a symbol, it is a valid one or raise an exception */
static void check_symbol(bparser *parser, bexpdesc *e)
{
    if (e->type == ETVOID && e->v.s == NULL) { /* error when token is not a symbol */
        push_error(parser,
            "unexpected symbol near '%s'", token2str(parser));
    }
}

/* check that the value in `e` is valid for a variable, i.e. contains a value or a valid symbol */
static void check_var(bparser *parser, bexpdesc *e)
{
    check_symbol(parser, e); /* check the token is a symbol */
    if (e->type == ETVOID) { /* error when symbol is undefined */
        int line = parser->lexer.linenumber;
        parser->lexer.linenumber = parser->lexer.lastline;
        push_error(parser,
            "'%s' undeclared (first use in this function)", str(e->v.s));
        parser->lexer.linenumber = line;
    }
}

static int match_skip(bparser *parser, btokentype type)
{
    if (next_type(parser) == type) { /* match */
        scan_next_token(parser); /* skip this token */
        return btrue;
    }
    return bfalse;
}

static bstring* _match_id(bparser *parser)
{
    if (next_type(parser) == TokenId) {
        bstring *str = next_token(parser).u.s;
        scan_next_token(parser); /* skip ID */
        return str;
    }
    return NULL;
}

#if BE_DEBUG_VAR_INFO

void begin_varinfo(bparser *parser, bstring *name)
{
    bvarinfo *var;
    bfuncinfo *finfo = parser->finfo;
    be_vector_push_c(parser->vm, &finfo->varvec, NULL);
    var = be_vector_end(&finfo->varvec);
    var->name = name;
    var->beginpc = finfo->pc;
    var->endpc = 0; /*  */
    finfo->proto->varinfo = be_vector_data(&finfo->varvec);
    finfo->proto->nvarinfo = be_vector_capacity(&finfo->varvec);
}

void end_varinfo(bparser *parser, int beginpc)
{
    bfuncinfo *finfo = parser->finfo;
    bblockinfo *binfo = finfo->binfo;
    bvarinfo *it = be_vector_data(&finfo->varvec);
    bvarinfo *end = be_vector_end(&finfo->varvec);
    if (beginpc == -1) /* use block->beginpc by default */
        beginpc = binfo->beginpc;
    /* skip the variable of the previous blocks */
    for (; (it <= end) && (it->beginpc < beginpc); ++it);
    for (; it <= end; ++it) {
        if (!it->endpc) /* write to endpc only once */
            it->endpc = finfo->pc;
    }
}

#else

#define begin_varinfo(parser, name)
#define end_varinfo(parser, beginpc) (void)(beginpc)

#endif

/* Initialize bblockinfo structure */
static void begin_block(bfuncinfo *finfo, bblockinfo *binfo, int type)
{
    binfo->prev = finfo->binfo; /* save previous block */
    finfo->binfo = binfo; /* tell parser this is the current block */
    binfo->type = (bbyte)type;
    binfo->hasupval = 0;
    binfo->sideeffect = 0;
    binfo->lastjmp = 0;
    binfo->beginpc = finfo->pc; /* set starting pc for this block */
    binfo->nactlocals = (bbyte)be_list_count(finfo->local); /* count number of local variables in previous block */
    if (type & BLOCK_LOOP) {
        binfo->breaklist = NO_JUMP;
        binfo->continuelist = NO_JUMP;
    }
}

static void end_block_ex(bparser *parser, int beginpc)
{
    bfuncinfo *finfo = parser->finfo;
    bblockinfo *binfo = finfo->binfo;
    be_code_close(finfo, 0); /* close upvalues */
    if (binfo->type & BLOCK_LOOP) {
        be_code_jumpto(finfo, binfo->beginpc);
        be_code_patchjump(finfo, binfo->breaklist);
        be_code_patchlist(finfo, binfo->continuelist, binfo->beginpc);
    }
    end_varinfo(parser, beginpc);
    be_list_resize(parser->vm, finfo->local, binfo->nactlocals); /* remove local variables from this block, they are now out of scope */
    finfo->freereg = binfo->nactlocals; /* adjust first free register accordingly */
    finfo->binfo = binfo->prev; /* restore previous block */
}

static void end_block(bparser *parser)
{
    end_block_ex(parser, -1);
}

#if BE_DEBUG_SOURCE_FILE
/* Return the name of the source for this parser, could be `string`,
   `stdin` or the name of the current function */
static bstring* parser_source(bparser *parser)
{
    if (parser->finfo) {
        return parser->finfo->proto->source;
    }
    return be_newstr(parser->vm, parser->lexer.fname);
}
#endif

/* Initialize a function block and create a new `bprotoˋ */
static void begin_func(bparser *parser, bfuncinfo *finfo, bblockinfo *binfo)
{
    bvm *vm = parser->vm;
    bproto *proto = be_newproto(vm);
    var_setproto(vm->top, proto);
    be_stackpush(vm);
    be_vector_init(vm, &finfo->code, sizeof(binstruction)); /* vector for code, vectors are not gced */
    proto->code = be_vector_data(&finfo->code);
    proto->codesize = be_vector_capacity(&finfo->code);
    be_vector_init(vm, &finfo->kvec, sizeof(bvalue)); /* vector for constants */
    proto->ktab = be_vector_data(&finfo->kvec);
    proto->nconst = be_vector_capacity(&finfo->kvec);
    be_vector_init(vm, &finfo->pvec, sizeof(bproto*)); /* vector for subprotos */
    proto->ptab = be_vector_data(&finfo->pvec);
    proto->nproto = be_vector_capacity(&finfo->pvec);
#if BE_DEBUG_SOURCE_FILE
    proto->source = parser_source(parser); /* keep a copy of source for function */
#endif
    finfo->local = be_list_new(vm); /* list for local variables */
    var_setlist(vm->top, finfo->local); /* push list of local variables on the stack (avoid gc) */
    be_stackpush(vm);
    finfo->upval = be_map_new(vm); /* push a map for upvals on stack */
    var_setmap(vm->top, finfo->upval);
    be_stackpush(vm);
    finfo->prev = parser->finfo; /* init finfo */
    finfo->lexer = &parser->lexer;
    finfo->proto = proto;
    finfo->freereg = 0;
    finfo->binfo = NULL;
    finfo->pc = 0;
    finfo->flags = 0;
    parser->finfo = finfo;
#if BE_DEBUG_RUNTIME_INFO
    be_vector_init(vm, &finfo->linevec, sizeof(blineinfo));
    proto->lineinfo = be_vector_data(&finfo->linevec);
    proto->nlineinfo = be_vector_capacity(&finfo->linevec);
#endif
#if BE_DEBUG_VAR_INFO
    be_vector_init(vm, &finfo->varvec, sizeof(bvarinfo));
    proto->varinfo = be_vector_data(&finfo->varvec);
    proto->nvarinfo = be_vector_capacity(&finfo->varvec);
#endif
    begin_block(finfo, binfo, 0);
}

/* compute the upval structure */
static void setupvals(bfuncinfo *finfo)
{
    bproto *proto = finfo->proto;
    int nupvals = be_map_count(finfo->upval);
    if (nupvals) {
        bmapnode *node;
        bmap *map = finfo->upval;
        bmapiter iter = be_map_iter();
        bupvaldesc *upvals = be_malloc(
                finfo->lexer->vm, sizeof(bupvaldesc) * nupvals);
        while ((node = be_map_next(map, &iter)) != NULL) {
            uint32_t v = (uint32_t)var_toint(&node->value);
            bupvaldesc *uv = upvals + upval_index(v);
            uv->idx = upval_target(v);
            uv->instack = upval_instack(v);
#if BE_DEBUG_VAR_INFO
            uv->name = var_tostr(&node->key);
#endif
        }
        proto->upvals = upvals;
        proto->nupvals = (bbyte)nupvals;
    }
}

/* Function is complete, finalize bproto */
static void end_func(bparser *parser)
{
    bvm *vm = parser->vm;
    bfuncinfo *finfo = parser->finfo;
    bproto *proto = finfo->proto;

    be_code_ret(finfo, NULL); /* append a return to last code */
    end_block(parser); /* close block */
    setupvals(finfo); /* close upvals */
    proto->code = be_vector_release(vm, &finfo->code); /* compact all vectors and return NULL if empty */
    proto->codesize = finfo->pc;
    proto->ktab = be_vector_release(vm, &finfo->kvec);
    proto->nconst = be_vector_count(&finfo->kvec);
    proto->nproto = be_vector_count(&finfo->pvec);
    proto->ptab = be_vector_release(vm, &finfo->pvec);
#if BE_USE_MEM_ALIGNED
    proto->code = be_move_to_aligned(vm, proto->code, proto->codesize * sizeof(binstruction));     /* move `code` to 4-bytes aligned memory region */
    proto->ktab = be_move_to_aligned(vm, proto->ktab, proto->nconst * sizeof(bvalue));     /* move `ktab` to 4-bytes aligned memory region */
#endif /* BE_USE_MEM_ALIGNED */
#if BE_DEBUG_RUNTIME_INFO
    proto->lineinfo = be_vector_release(vm, &finfo->linevec);     /* move `lineinfo` to 4-bytes aligned memory region */
    proto->nlineinfo = be_vector_count(&finfo->linevec);
#if BE_USE_MEM_ALIGNED
    proto->lineinfo = be_move_to_aligned(vm, proto->lineinfo, proto->nlineinfo * sizeof(blineinfo));
#endif /* BE_USE_MEM_ALIGNED */
#endif /* BE_DEBUG_RUNTIME_INFO */
#if BE_DEBUG_VAR_INFO
    proto->varinfo = be_vector_release(vm, &finfo->varvec);
    proto->nvarinfo = be_vector_count(&finfo->varvec);
#endif
    parser->finfo = parser->finfo->prev; /* restore previous `finfo` */
    be_stackpop(vm, 2); /* pop upval and local */
}

/* is the next token a binary operator? If yes return the operator or `OP_NOT_BINARY` */
static btokentype get_binop(bparser *parser)
{
    btokentype op = next_type(parser);
    if (op >= OptAdd && op <= OptOr) {
        return op;
    }
    return OP_NOT_BINARY;
}

/* is the next token a unary operator? If yes return the operator or `OP_NOT_BINARY` */
/* operator 'negative' 'not' and 'flip' */
static btokentype get_unary_op(bparser *parser)
{
    btokentype op = next_type(parser);
    if (op == OptSub || op == OptNot || op == OptFlip) {
        return op; /* operator 'negative' 'not' and 'flip' */
    }
    return OP_NOT_UNARY;
}

/* is the next token an assignment operator? If yes return the operator or `OP_NOT_BINARY` */
/* `=`, `+=`, ... `>>=` */
static btokentype get_assign_op(bparser *parser)
{
    btokentype op = next_type(parser);
    if ((op >= OptAssign && op <= OptRsfAssign) || op == OptWalrus) {
        return op;
    }
    return OP_NOT_ASSIGN;
}

/* Initialize bexpdesc structure with specific type and value as int */
static void init_exp(bexpdesc *e, exptype_t type, bint i)
{
    e->type = (bbyte)type;
    e->t = NO_JUMP;
    e->f = NO_JUMP;
    e->not = 0;
    e->v.s = NULL;
    e->v.i = i;
}

/* find local variable by name, starting at index `begin` */
/* linear search, returns -1 if not found */
static int find_localvar(bfuncinfo *finfo, bstring *s, int begin)
{
    int i, count = be_list_count(finfo->local);
    bvalue *var = be_list_data(finfo->local);
    for (i = count - 1; i >= begin; --i) {
        if (be_eqstr(var[i].v.s, s)) {
            return i;
        }
    }
    return -1; /* not found */
}

/* create a new local variable by name, or return the current register if already exists */
/* returns the Reg number for the variable */
/* If STRICT, we don't allow a var to hide a var from outer scope */
/* We don't allow the same var to be defined twice in same scope */
static int new_localvar(bparser *parser, bstring *name)
{
    bfuncinfo *finfo = parser->finfo;
    int reg = find_localvar(finfo, name, finfo->binfo->nactlocals); /* look if already exists skipping the local variables from upper blocks */
    /* 'strict': raise an exception if the local variable shadows another local variable */
    if (reg == -1) {
        bvalue *var;
        if (comp_is_strict(parser->vm)) {
            if (find_localvar(finfo, name, 0) >= 0 && str(name)[0] != '.') {  /* we do accept nested redifinition of internal variables starting with '.' */
                push_error(parser, "strict: redefinition of '%s' from outer scope", str(name));
            }
        }
        reg = be_list_count(finfo->local); /* new local index */
        var = be_list_push(parser->vm, finfo->local, NULL);
        var_setstr(var, name);
        if (reg >= finfo->freereg) {
            be_code_allocregs(finfo, 1); /* use a register */
        }
        begin_varinfo(parser, name);
    } else {
        push_error(parser, "redefinition of '%s'", str(name));
    }
    return reg;
}

/* Find upval by name, if found return its index number, or -1 */
static int find_upval(bfuncinfo *finfo, bstring *s)
{
    bvm *vm = finfo->lexer->vm;
    bvalue *desc = be_map_findstr(vm, finfo->upval, s);
    if (desc) {
        return upval_index(desc->v.i);
    }
    return -1;
}

/* Recursively search for upper blocks that are referenced in upvals */
/* and mark them with `hasupval` */
static void mark_upval(bfuncinfo *finfo, int level)
{
    bblockinfo *binfo = finfo->prev->binfo;
    while (binfo->nactlocals > level) {
        binfo = binfo->prev;
    }
    binfo->hasupval = 1;
}

static int new_upval(bvm *vm, bfuncinfo *finfo, bstring *name, bexpdesc *var)
{
    int index;
    bvalue *desc;
    int target = var->v.idx;
    int instack = var->type == ETLOCAL;
    if (instack) { /* mark upvalue's block */
        mark_upval(finfo, target);
    }
    index = be_map_count(finfo->upval);
    if (index >= 255) {
        be_lexerror(finfo->lexer, be_pushfstring(vm,
            "too many upvalues (in '%s')", str(name)));
    }
    desc = be_map_insertstr(vm, finfo->upval, name, NULL);
    var_setint(desc, upval_desc(index, target, instack));
    return index;
}

/* create a new variable in currenr context as name, and create expdesc for it */
/* If within a block, create as local, otherwise as global */
static void new_var(bparser *parser, bstring *name, bexpdesc *var)
{
    bfuncinfo *finfo = parser->finfo;
    if (comp_is_strict(parser->vm)) {
        /* check if we are masking a builtin */
        if (be_builtin_find(parser->vm, name) >= 0) {
            push_error(parser, "strict: redefinition of builtin '%s'", str(name));
        }
    }
    if (finfo->prev || finfo->binfo->prev || parser->islocal) {
        init_exp(var, ETLOCAL, 0);
        var->v.idx = new_localvar(parser, name); /* if local, contains the index in current local var list */
    } else {
        init_exp(var, ETGLOBAL, 0);
        var->v.idx = be_global_new(parser->vm, name);
        if (var->v.idx > (int)IBx_MASK) {
            push_error(parser,
                "too many global variables (in '%s')", str(name));
        }
        if (comp_is_named_gbl(parser->vm)) {
            /* change to ETNGLBAL */
            bexpdesc key;
            init_exp(&key, ETSTRING, 0);
            key.v.s = name;
            init_exp(var, ETNGLOBAL, 0);
            var->v.idx = be_code_nglobal(parser->finfo, &key);
        }
    }
}

static int singlevaraux(bvm *vm, bfuncinfo *finfo, bstring *s, bexpdesc *var)
{
    if (finfo == NULL) {
        return ETVOID;
    } else {
        int idx = find_localvar(finfo, s, 0);
        if (idx >= 0) { /* local variable */
            init_exp(var, ETLOCAL, 0);
            var->v.idx = idx;
            return ETLOCAL;
        } else {
            idx = find_upval(finfo, s);
            if (idx < 0) {
                /* find the previous scope  */
                int res = singlevaraux(vm, finfo->prev, s, var);
                if (res == ETUPVAL || res == ETLOCAL) {
                    idx = new_upval(vm, finfo, s, var); /* new upvalue */
                } else {
                    idx = be_global_find(vm, s);
                    if (idx >= 0) {
                        if (idx < be_builtin_count(vm)) {
                            return ETGLOBAL; /* global variable */
                        } else {
                            return comp_is_named_gbl(vm) ? ETNGLOBAL : ETGLOBAL; /* global variable */
                        }
                    } else {
                        return ETVOID; /* unknow (new variable or error) */
                    }
                }
            }
            init_exp(var, ETUPVAL, idx);
            return ETUPVAL;
        }
    }
}

/* get variable from next toden as name */
/* and create an expdesc from it */
/* can be new, global, named global, upval */
static void singlevar(bparser *parser, bexpdesc *var)
{
    bexpdesc key;
    bstring *varname = next_token(parser).u.s;
    int type = singlevaraux(parser->vm, parser->finfo, varname, var);
    switch (type) {
    case ETVOID:
        init_exp(var, ETVOID, 0);
        var->v.s = varname;
        break;
    case ETGLOBAL:
        init_exp(var, ETGLOBAL, 0);
        var->v.idx = be_global_find(parser->vm, varname);
        break;
    case ETNGLOBAL:
        init_exp(&key, ETSTRING, 0);
        key.v.s = varname;
        init_exp(var, ETNGLOBAL, 0);
        var->v.idx = be_code_nglobal(parser->finfo, &key);
        break;
    default:
        break;
    }
}

/* parse a vararg argument in the form `def f(a, *b) end` */
/* Munch the '*', read the token, create variable and declare the function as vararg */
static void func_vararg(bparser *parser) {
    bexpdesc v;
    bstring *str;
    match_token(parser, OptMul); /* skip '*' */
    str = next_token(parser).u.s;
    match_token(parser, TokenId); /* match and skip ID */
    new_var(parser, str, &v); /* new variable */
    parser->finfo->proto->varg |= BE_VA_VARARG;   /* set varg flag */
}

/* Parse function or method definition variable list */
/* Create an implicit local variable for each argument starting at R0 */
/* Update function proto argc to the expected number or arguments */
/* Raise an exception if multiple arguments have the same name */
/* New: vararg support */
static void func_varlist(bparser *parser)
{
    bexpdesc v;
    bstring *str;
    /* '(' [ ID {',' ID}] ')' or */
    /* '(' '*' ID ')' or */
    /* '(' [ ID {',' ID}] ',' '*' ID ')' */
    match_token(parser, OptLBK); /* skip '(' */
    if (next_type(parser) == OptMul) {
        func_vararg(parser);
    } else if (match_id(parser, str) != NULL) {
        new_var(parser, str, &v); /* new variable */
        while (match_skip(parser, OptComma)) { /* ',' */
            if (next_type(parser) == OptMul) {
                func_vararg(parser);
                break;
            } else {
                str = next_token(parser).u.s;
                match_token(parser, TokenId); /* match and skip ID */
                /* new local variable */
                new_var(parser, str, &v);
            }
        }
    }
    match_token(parser, OptRBK); /* skip ')' */
    parser->finfo->proto->argc = parser->finfo->freereg;
}

/* Parse a function includind arg list and body */
/* Given name and type (function or method) */
/* Returns `bproto` object */
static bproto* funcbody(bparser *parser, bstring *name, bclass *c, int type)
{
    bfuncinfo finfo;
    bblockinfo binfo;

    /* '(' varlist ')' block 'end' */
    begin_func(parser, &finfo, &binfo); /* init new function context */
    finfo.proto->name = name;
    if (type & FUNC_METHOD) { /* If method, add an implicit first argument `self` */
        new_localvar(parser, parser_newstr(parser, "self"));
        finfo.proto->varg |= BE_VA_METHOD;
    }
    func_varlist(parser); /* parse arg list */
    if ((type & FUNC_STATIC) && (c != NULL)) { /* If static method, add an implicit local variable `_class` */
        bexpdesc e1, e2;
        new_var(parser, parser_newstr(parser, "_class"), &e1); /* new implicit variable '_class' */
        init_exp(&e2, ETCONST, 0);
        be_code_implicit_class(parser->finfo, &e2, c);
        be_code_setvar(parser->finfo, &e1, &e2, bfalse);
        finfo.proto->varg |= BE_VA_STATICMETHOD;
    }
    stmtlist(parser); /* parse statement without final `end` */
    end_func(parser); /* close function context */
    match_token(parser, KeyEnd); /* skip 'end' */
    return finfo.proto; /* return fully constructed `bproto` */
}

/* anonymous function, build `bproto` object with name `_anonymous_` */
/* and build a expdesc for the bproto */
static void anon_func(bparser *parser, bexpdesc *e)
{
    bproto *proto;
    bstring *name = parser_newstr(parser, "_anonymous_");
    /* 'def' ID '(' varlist ')' block 'end' */
    scan_next_token(parser); /* skip 'def' */
    proto = funcbody(parser, name, NULL, FUNC_ANONYMOUS);
    init_exp(e, ETPROTO, be_code_proto(parser->finfo, proto));
    be_stackpop(parser->vm, 1);
}

static void lambda_varlist(bparser *parser)
{
    bexpdesc v;
    bstring *str;
    /* [ID {',' ID}] | {ID}] */
    if (match_id(parser, str) != NULL) {
        bbool comma;
        new_var(parser, str, &v); /* new variable */
        comma = next_type(parser) == OptComma;
        while (next_type(parser) != OptArrow) {
            if (comma) {
                match_token(parser, OptComma); /* match and skip ',' */
            }
            str = next_token(parser).u.s;
            match_token(parser, TokenId); /* match and skip ID */
            /* new local variable */
            new_var(parser, str, &v);
        }
    }
    match_token(parser, OptArrow); /* skip '->' */
    parser->finfo->proto->argc = parser->finfo->freereg;
}

/* lambda expression */
static void lambda_expr(bparser *parser, bexpdesc *e)
{
    bexpdesc e1;
    bfuncinfo finfo;
    bblockinfo binfo;
    bstring *name = parser_newstr(parser, "<lambda>");
    /* '/' ID {[',' ID]} '->' expr */
    scan_next_token(parser); /* skip '/' */
    begin_func(parser, &finfo, &binfo);
    finfo.proto->name = name;
    lambda_varlist(parser);
    expr(parser, &e1);
    check_var(parser, &e1);
    be_code_ret(parser->finfo, &e1);
    end_func(parser);
    init_exp(e, ETPROTO, be_code_proto(parser->finfo, finfo.proto));
    be_stackpop(parser->vm, 1);
}

/* Instanciate a builtin type by name */
/* Allocates a new register for the value, and call empty constructor */
/* Is allocated as LOCAL and must be changed to REG when completed */
static void new_primtype(bparser *parser, const char *type, bexpdesc *e)
{
    int idx;
    bvm *vm = parser->vm;
    bfuncinfo *finfo = parser->finfo;

    scan_next_token(parser);
    idx = be_builtin_find(vm, parser_newstr(parser, type));
    init_exp(e, ETGLOBAL, idx);
    idx = be_code_nextreg(finfo, e);
    be_code_call(finfo, idx, 0);
    e->type = ETLOCAL;  /* declare as local, will be changed to ETREG when completely initialized */
}

/* Parse next member within a list */
/* `l` contains the current list. The expr is evaluated and added to the list */
static void list_nextmember(bparser *parser, bexpdesc *l)
{
    bexpdesc e, v = *l;
    bfuncinfo *finfo = parser->finfo;
    expr(parser, &e); /* value */
    check_var(parser, &e); /* check that we don´t have an unknown symbol */
    be_code_binop(finfo, OptConnect, &v, &e, -1); /* add it to list with CONNECT */
    be_code_freeregs(finfo, 1);  /* since left arg is LOCAL, an ETREG was allocated. Free it */
}

static void map_nextmember(bparser *parser, bexpdesc *l)
{
    bexpdesc e, v = *l;
    bfuncinfo *finfo = parser->finfo;
    expr(parser, &e); /* key */
    check_var(parser, &e);  /* check if value is valid */
    be_code_index(finfo, &v, &e);  /* package as `v` as INDEX suffix for value `e` */
    match_token(parser, OptColon); /* ':' */
    expr(parser, &e); /* value in `e` */
    check_var(parser, &e);  /* check if value is correct */
    be_code_setvar(finfo, &v, &e, bfalse);  /* set suffi  INDEX value to e */
}

static void list_expr(bparser *parser, bexpdesc *e)
{
    /* '[' {expr ','} [expr] ']' */
    new_primtype(parser, "list", e); /* new list, created as LOCAL first */
    while (next_type(parser) != OptRSB) {
        list_nextmember(parser, e);
        if (!match_skip(parser, OptComma)) { /* ',' */
            break;
        }
    }
    e->type = ETREG; /* then turned to REG */
    match_token(parser, OptRSB); /* skip ']' */
}

static void map_expr(bparser *parser, bexpdesc *e)
{
    /* '{' {expr ':' expr ','} [expr ':' expr] '}' */
    new_primtype(parser, "map", e); /* new map */
    while (next_type(parser) != OptRBR) {
        map_nextmember(parser, e);
        if (!match_skip(parser, OptComma)) { /* ',' */
            break;
        }
    }
    e->type = ETREG;
    match_token(parser, OptRBR); /* skip '}' */
}

/* push each argument as new reg and return number of args */
/* TODO `e` is ignored by caller */
static int exprlist(bparser *parser, bexpdesc *e)
{
    bfuncinfo *finfo = parser->finfo;
    int n = 1;
    /* expr { ',' expr } */
    expr(parser, e);  /* parse expr */
    check_var(parser, e);  /* check if valid */
    be_code_nextreg(finfo, e);  /* move result to next reg */
    while (match_skip(parser, OptComma)) { /* ',' */
        expr(parser, e);
        check_var(parser, e);
        be_code_nextreg(finfo, e);
        ++n;
    }
    return n;
}

/* parse call to method or function */
/* `e` can be a member (method) or a register */
/* On return, `e` is ETREG to the result of the call */
static void call_expr(bparser *parser, bexpdesc *e)
{
    bexpdesc args;
    bfuncinfo *finfo = parser->finfo;
    int argc = 0, base;
    int ismember = e->type == ETMEMBER;

    parser->finfo->binfo->sideeffect = 1;   /* has side effect */
    /* func '(' [exprlist] ')' */
    check_var(parser, e);
    /* code function index to next register */
    if (ismember) {
        base = be_code_getmethod(finfo, e);
    } else {
        base = be_code_nextreg(finfo, e); /* allocate a new base reg if not at top already */
    }
    /* base is always taken at top of freereg and allocates 1 reg for function and 2 regs for method */
    scan_next_token(parser); /* skip '(' */
    if (next_type(parser) != OptRBK) {  /* if arg list is not empty */
        argc = exprlist(parser, &args);  /* push each argument as new reg and return number of args */
    }
    match_token(parser, OptRBK); /* skip ')' */
    argc += ismember;   /* if method there is an additional implicit arg */
    be_code_call(finfo, base, argc);
    if (e->type != ETREG) {
        e->type = ETREG;
        e->v.idx = base;
    }
}

/* Parse member expression */
/* Generates an ETMEMBER object that is materialized later into GETMBR, GETMET or SETMBR */
static void member_expr(bparser *parser, bexpdesc *e)
{
    bstring *str;
    /* . ID */
    check_var(parser, e);
    scan_next_token(parser); /* skip '.' */
    if (match_id(parser, str) != NULL) {
        bexpdesc key;
        init_exp(&key, ETSTRING, 0);
        key.v.s = str;
        be_code_member(parser->finfo, e, &key);
    } else if (next_type(parser) == OptLBK) {
        scan_next_token(parser); /* skip '(' */
        bexpdesc key;
        expr(parser, &key);
        check_var(parser, &key);
        match_token(parser, OptRBK); /* skip ')' */
        be_code_member(parser->finfo, e, &key);
    } else {
        push_error(parser, "invalid syntax near '%s'",
            be_token2str(parser->vm, &next_token(parser)));
    }
}

static void index_expr(bparser *parser, bexpdesc *e)
{
    bexpdesc e1;
    /* [expr] */
    check_var(parser, e);
    scan_next_token(parser); /* skip '[' */
    expr(parser, &e1);
    check_var(parser, &e1);
    be_code_index(parser->finfo, e, &e1);
    match_token(parser, OptRSB); /* skip ']' */
}

static void simple_expr(bparser *parser, bexpdesc *e)
{
    switch (next_type(parser)) {
    case TokenInteger:
        init_exp(e, ETINT, next_token(parser).u.i);
        break;
    case TokenReal:
        init_exp(e, ETREAL, 0);
        e->v.r = next_token(parser).u.r;
        break;
    case TokenString:
        init_exp(e, ETSTRING, 0);
        e->v.s = next_token(parser).u.s;
        break;
    case TokenId:
        singlevar(parser, e);
        break;
    case KeyTrue:
        init_exp(e, ETBOOL, 1);
        break;
    case KeyFalse:
        init_exp(e, ETBOOL, 0);
        break;
    case KeyNil:
        init_exp(e, ETNIL, 0);
        break;
    default: /* unknow expr */
        return;
    }
    scan_next_token(parser);
}

static void primary_expr(bparser *parser, bexpdesc *e)
{
    switch (next_type(parser)) {
    case OptLBK: /* '(' expr ')' */
        scan_next_token(parser); /* skip '(' */
        expr(parser, e);
        check_var(parser, e);
        match_token(parser, OptRBK); /* skip ')' */
        break;
    case OptLSB: /* list */
        list_expr(parser, e);
        break;
    case OptLBR: /* map */
        map_expr(parser, e);
        break;
    case KeyDef: /* anonymous function */
        anon_func(parser, e);
        break;
    case OptDiv: /* lambda expression */
        lambda_expr(parser, e);
        break;
    default: /* simple expr */
        simple_expr(parser, e);
        break;
    }
}

static void suffix_expr(bparser *parser, bexpdesc *e)
{
    primary_expr(parser, e);
    for (;;) {
        switch (next_type(parser)) {
        case OptLBK: /* '(' function call */
            call_expr(parser, e);
            break;
        case OptDot: /* '.' member */
            member_expr(parser, e);
            break;
        case OptLSB: /* '[' index */
            index_expr(parser, e);
            break;
        default:
            return;
        }
    }
}

static void suffix_alloc_reg(bparser *parser, bexpdesc *l)
{
    bfuncinfo *finfo = parser->finfo;
    bbool is_suffix = l->type == ETINDEX || l->type == ETMEMBER;   /* is suffix */
    bbool is_suffix_reg = l->v.ss.tt == ETREG || l->v.ss.tt == ETLOCAL || l->v.ss.tt == ETGLOBAL || l->v.ss.tt == ETNGLOBAL;   /* if suffix, does it need a register */
    bbool is_global = l->type == ETGLOBAL || l->type == ETNGLOBAL;
    bbool is_upval = l->type == ETUPVAL;
    /* in the suffix expression, if the object is a temporary
     * variable (l->v.ss.tt == ETREG), it needs to be cached. */
    if (is_global || is_upval || (is_suffix && is_suffix_reg)) {
        be_code_allocregs(finfo, 1);
    }
}

/* compound assignment */
static void compound_assign(bparser *parser, int op, bexpdesc *l, bexpdesc *r)
{
    int dst = -1;  /* destination register in case of compound assignment */
    if (op != OptAssign) { /* check left variable */
        check_var(parser, l);
        /* cache the register of the object when continuously assigning */
        dst = parser->finfo->freereg;
        suffix_alloc_reg(parser, l);
    }
    expr(parser, r); /* right expression */
    check_var(parser, r);
    if (op != OptAssign) { /* compound assignment */
        bexpdesc e = *l;
        op = op < OptAndAssign ? op - OptAddAssign + OptAdd
                : op - OptAndAssign + OptBitAnd;
        be_code_binop(parser->finfo, op, &e, r, dst); /* coding operation */
        *r = e;
    }
}

/* check if we need to create a new local variable with this name to be assigned to */
/* Returns true if it´s a new local variable */
/* A new implicit local variable is created if no global has the same name (excluding builtins) */
/* This means that you can override a builtin silently */
/* This also means that a function cannot create a global, they must preexist or create with `global` module */
static int check_newvar(bparser *parser, bexpdesc *e)
{
    if (e->type == ETGLOBAL) {
        if (e->v.idx < be_builtin_count(parser->vm)) {
            e->v.s = be_builtin_name(parser->vm, e->v.idx);
            if (comp_is_strict(parser->vm)) {
                push_error(parser, "strict: redefinition of builtin '%s'",
                    str(e->v.s));
            }
            return btrue;
        }
        return bfalse;
    }
    if (comp_is_strict(parser->vm)) {
        bfuncinfo *finfo = parser->finfo;
        if ((e->type == ETVOID) && (finfo->prev || finfo->binfo->prev || parser->islocal)) {
            push_error(parser, "strict: no global '%s', did you mean 'var %s'?",
                str(e->v.s), str(e->v.s));
        }
    }
    return e->type == ETVOID;
}

static void assign_expr(bparser *parser)
{
    bexpdesc e;
    btokentype op;
    int line = parser->lexer.linenumber;
    parser->finfo->binfo->sideeffect = 0;      /* reinit side effect marker */
    expr(parser, &e); /* left expression */
    check_symbol(parser, &e);
    op = get_assign_op(parser);
    if (op != OP_NOT_ASSIGN) { /* assign operator */
        bexpdesc e1;
        parser->finfo->binfo->sideeffect = 1;
        scan_next_token(parser);
        compound_assign(parser, op, &e, &e1);
        if (check_newvar(parser, &e)) { /* new variable */
            new_var(parser, e.v.s, &e);
        }
        if (be_code_setvar(parser->finfo, &e, &e1, bfalse)) {
            parser->lexer.linenumber = line;
            parser_error(parser,
                "try to assign constant expressions.");
        }
    } else if (e.type >= ETMEMBER) {
        bfuncinfo *finfo = parser->finfo;
        /* these expressions occupy a register and need to be freed */
        finfo->freereg = (bbyte)be_list_count(finfo->local);
    } else if (e.type == ETVOID) { /* not assign expression */
        /* undeclared symbol */
        parser->lexer.linenumber = line;
        check_var(parser, &e);
    } 
}

/* conditional expression */
static void cond_expr(bparser *parser, bexpdesc *e)
{
    /* expr '?' expr ':' expr */
    if (next_type(parser) == OptQuestion) {
        int jf, jl = NO_JUMP; /* jump list */
        bfuncinfo *finfo = parser->finfo;
        check_var(parser, e);  /* check if valid */
        scan_next_token(parser); /* skip '?' */
        be_code_jumpbool(finfo, e, bfalse); /* go if true */
        jf = e->f;
        expr(parser, e);
        check_var(parser, e);
        be_code_nextreg(finfo, e);
        be_code_freeregs(finfo, 1);
        be_code_conjump(finfo, &jl, be_code_jump(finfo)); /* connect jump */
        be_code_patchjump(finfo, jf);
        match_token(parser, OptColon); /* match and skip ':' */
        expr(parser, e);
        check_var(parser, e);
        e->v.idx = be_code_nextreg(finfo, e);
        be_code_patchjump(finfo, jl);
        e->type = ETREG;
    }
}

/* binary operator: + - * / % && || < <= == != > >=
 * unary operator: + - !
 */
static void sub_expr(bparser *parser, bexpdesc *e, int prio)
{
    bfuncinfo *finfo = parser->finfo;
    btokentype op = get_unary_op(parser);  /* check if first token in unary op */
    if (op != OP_NOT_UNARY) {  /* unary op found */
        int line, res;
        scan_next_token(parser);  /* move to next token */
        line = parser->lexer.linenumber;  /* remember line number for error reporting */
        sub_expr(parser, e, UNARY_OP_PRIO);  /* parse subexpr with new prio */
        check_var(parser, e);  /* check that the value is ok */
        res = be_code_unop(finfo, op, e);  /* apply unary op with optimizations if the token is a value */
        if (res) { /* encode unary op */
            parser->lexer.linenumber = line;
            push_error(parser, "wrong type argument to unary '%s'",
                res == 1 ? "negative" : "bit-flip");
        }
    } else {
        suffix_expr(parser, e);  /* parse left part of binop */
    }
    op = get_binop(parser);  /* check if binop */
    while (op != OP_NOT_BINARY && prio > binary_op_prio(op)) {  /* is binop applicable */
        bexpdesc e2;
        check_var(parser, e);  /* check that left part is valid */
        scan_next_token(parser);  /* move to next token */
        be_code_prebinop(finfo, op, e); /* and or */
        if (op == OptConnect) {
            parser->finfo->binfo->sideeffect = 1;
        }
        init_exp(&e2, ETVOID, 0);
        sub_expr(parser, &e2, binary_op_prio(op));  /* parse right side */
        if ((op == OptConnect) && (e2.type == ETVOID) && (e2.v.s == NULL)) {    /* 'e2.v.s == NULL' checks that it's not an undefined variable */
            init_exp(&e2, ETINT, M_IMAX);
        } else {
            check_var(parser, &e2);  /* check if valid */
        }
        be_code_binop(finfo, op, e, &e2, -1); /* encode binary op */
        op = get_binop(parser);  /* is there a following binop? */
    }
    if (prio == ASSIGN_OP_PRIO) {
        cond_expr(parser, e);
    }
}

static void walrus_expr(bparser *parser, bexpdesc *e)
{
    int line = parser->lexer.linenumber;
    sub_expr(parser, e, ASSIGN_OP_PRIO);    /* left expression */
    btokentype op = next_type(parser);
    if (op == OptWalrus) {
        check_symbol(parser, e);
        bexpdesc e1 = *e;           /* copy var to e1, e will get the result of expression */
        parser->finfo->binfo->sideeffect = 1;   /* has side effect */
        scan_next_token(parser);    /* skip ':=' */
        expr(parser, e);
        check_var(parser, e);
        if (check_newvar(parser, &e1)) { /* new variable */
            new_var(parser, e1.v.s, &e1);
        }
        if (be_code_setvar(parser->finfo, &e1, e, btrue /* do not release register */ )) {
            parser->lexer.linenumber = line;
            parser_error(parser,
                "try to assign constant expressions.");
        }
    }
}

/* Parse new expression and return value in `e` (overwritten) */
/* Initializes an empty expdes  and parse subexpr */
/* Always allocates a new temp register at top of freereg */
static void expr(bparser *parser, bexpdesc *e)
{
    init_exp(e, ETVOID, 0);
    walrus_expr(parser, e);
}

static void expr_stmt(bparser *parser)
{
    assign_expr(parser);
}

static int block_follow(bparser *parser)
{
    switch (next_type(parser)) {
    case KeyElse: case KeyElif:
    case KeyEnd: case KeyExcept:
    case TokenEOS:
        return 0;
    default:
        return 1;
    }
}

static int cond_stmt(bparser *parser)
{
    bexpdesc e;
    /* expr */
    match_notoken(parser, OptRBK);
    expr(parser, &e);
    check_var(parser, &e);
    be_code_jumpbool(parser->finfo, &e, bfalse); /* go if true */
    return e.f;
}

static void condition_block(bparser *parser, int *jmp)
{
    bfuncinfo *finfo = parser->finfo;
    int br = cond_stmt(parser);
    block(parser, 0);
    if (next_type(parser) == KeyElif
            || next_type(parser) == KeyElse) {
        be_code_conjump(finfo, jmp, be_code_jump(finfo)); /* connect jump */
    }
    be_code_patchjump(finfo, br);
}

static void if_stmt(bparser *parser)
{
    int jl = NO_JUMP; /* jump list */
    /* IF expr block {ELSEIF expr block}, [ELSE block], end */
    scan_next_token(parser); /* skip 'if' */
    condition_block(parser, &jl);
    while (match_skip(parser, KeyElif)) { /* 'elif' */
        condition_block(parser, &jl);
    }
    if (match_skip(parser, KeyElse)) { /* 'else' */
        block(parser, 0);
    }
    match_token(parser, KeyEnd); /* skip end */
    be_code_patchjump(parser->finfo, jl);
}

static void do_stmt(bparser *parser)
{
    /* DO block END */
    scan_next_token(parser); /* skip 'do' */
    block(parser, 0);
    match_token(parser, KeyEnd); /* skip 'end' */
}

static void while_stmt(bparser *parser)
{
    int brk;
    bblockinfo binfo;
    bfuncinfo *finfo = parser->finfo;
    /* WHILE expr block END */
    scan_next_token(parser); /* skip 'while' */
    begin_block(parser->finfo, &binfo, BLOCK_LOOP);
    brk = cond_stmt(parser);
    stmtlist(parser);
    end_block(parser);
    be_code_patchjump(finfo, brk);
    match_token(parser, KeyEnd); /* skip 'end' */
}

static bstring* for_itvar(bparser *parser)
{
    bstring *str;
    if (match_id(parser, str) == NULL) {
        push_error(parser,
            "missing iteration variable before '%s'",
            token2str(parser));
    }
    return str;
}

static void for_init(bparser *parser, bexpdesc *v)
{
    bexpdesc e;
    bstring *s;
    bfuncinfo *finfo = parser->finfo;
    /* .it = __iterator__(expr) */
    s = parser_newstr(parser, "__iterator__");
    init_exp(&e, ETGLOBAL, be_builtin_find(parser->vm, s));
    be_code_nextreg(finfo, &e); /* code function '__iterator__' */
    expr(parser, v);
    check_var(parser, v);
    be_code_nextreg(finfo, v);
    be_code_call(finfo, e.v.idx, 1); /* call __iterator__(expr) */
    be_code_freeregs(finfo, 1); /* free register of __iterator__ */
    s = parser_newstr(parser, ".it");
    init_exp(v, ETLOCAL, new_localvar(parser, s));
}

static void for_iter(bparser *parser, bstring *var, bexpdesc *it)
{
    bexpdesc e;
    bfuncinfo *finfo = parser->finfo;
    /* reset the jump head PC of the for loop body */
    finfo->binfo->beginpc = finfo->pc;
    /* itvar = .it() */
    init_exp(&e, ETLOCAL, new_localvar(parser, var)); /* new itvar */
    be_code_setvar(finfo, &e, it, bfalse); /* code function to variable '.it' */
    be_code_call(finfo, e.v.idx, 0); /* itvar <- call .it() */
    stmtlist(parser);
}

static void for_leave(bparser *parser, int jcatch, int beginpc)
{
    bexpdesc e;
    bfuncinfo *finfo = parser->finfo;
    int jbrk = finfo->binfo->breaklist;
    init_exp(&e, ETSTRING, 0);
    e.v.s = parser_newstr(parser, "stop_iteration");
    end_block_ex(parser, beginpc); /* leave except & loop block */
    if (jbrk != NO_JUMP) { /* has `break` statement in iteration block */
        be_code_exblk(finfo, 1);
        jbrk = be_code_jump(finfo);
    }
    be_code_conjump(finfo, &jcatch, finfo->pc);
    be_code_catch(finfo, be_code_nextreg(finfo, &e), 1, 0, NULL);
    be_code_raise(finfo, NULL, NULL);
    be_code_conjump(finfo, &jbrk, finfo->pc);
    be_code_freeregs(finfo, 1);
}

/* approximate equivalent script code:
 * .it = __iter__(expr)
 * try
 *     while (1)
 *         itvar = .it()
 *         stmtlist
 *     end
 * except ('stop_iteration')
 * end
 * */
static void for_stmt(bparser *parser)
{
    bstring *var;
    bexpdesc iter;
    bblockinfo binfo;
    int jcatch, beginpc = parser->finfo->pc;
    /* FOR ID : expr block END */
    scan_next_token(parser); /* skip 'for' */
    begin_block(parser->finfo, &binfo, BLOCK_EXCEPT | BLOCK_LOOP);
    var = for_itvar(parser);
    match_token(parser, OptColon); /* skip ':' */
    for_init(parser, &iter);
    jcatch = be_code_exblk(parser->finfo, 0);
    for_iter(parser, var, &iter);
    for_leave(parser, jcatch, beginpc);
    match_token(parser, KeyEnd); /* skip 'end' */
}

static bblockinfo* break_block(bparser *parser)
{
    int try_depth = 0; /* count of exception catch blocks */
    bblockinfo *binfo = parser->finfo->binfo;
    /* BREAK | CONTINUE */
    scan_next_token(parser); /* skip 'break' or 'continue' */
    while (binfo && !(binfo->type & BLOCK_LOOP)) {
        if (binfo->type & BLOCK_EXCEPT) {
            ++try_depth; /* leave the exception catch block */
        }
        binfo = binfo->prev;
    }
    if (binfo && try_depth) { /* exception catch blocks that needs to leave */
        be_code_exblk(parser->finfo, try_depth);
    }
    return binfo;
}

static void break_stmt(bparser *parser)
{
    bfuncinfo *f = parser->finfo;
    bblockinfo *binfo = break_block(parser);
    if (binfo != NULL) { /* connect jump */
        be_code_conjump(f, &binfo->breaklist, be_code_jump(f));
    } else {
        parser_error(parser, "break not loop");
    }
}

static void continue_stmt(bparser *parser)
{
    bfuncinfo *f = parser->finfo;
    bblockinfo *b = break_block(parser);
    if (b != NULL) { /* connect jump */
        be_code_conjump(f, &b->continuelist, be_code_jump(f));
    } else {
        parser_error(parser, "continue not loop");
    }
}

static bbool isoverloadable(btokentype type)
{
    return (type >= OptAdd && type <= OptConnect) /* overloaded binary operator */
        || type == OptFlip || type == OptLBK;     /* '~' and '()' operator */
}

static bstring* func_name(bparser* parser, bexpdesc* e, int ismethod)
{
    btokentype type = next_type(parser);
    if (type == TokenId) {
        bstring *name = next_token(parser).u.s;
        if (!ismethod) {
            new_var(parser, name, e); /* new variable */
        }
        scan_next_token(parser); /* skip name */
        return name;
    } else if (ismethod && isoverloadable(type)) {
        scan_next_token(parser); /* skip token */
        /* '-*' negative operator */
        if (type == OptSub && next_type(parser) == OptMul) {
            scan_next_token(parser); /* skip '*' */
            return parser_newstr(parser, "-*");
        }
        /* '()' call operator */
        if (type == OptLBK && next_type(parser) == OptRBK) {
            scan_next_token(parser); /* skip ')' */
            return parser_newstr(parser, "()");
        }
        return parser_newstr(parser, be_tokentype2str(type));
    }
    push_error(parser,
        "the token '%s' is not a valid function name.",
        token2str(parser));
    return NULL;
}

static void def_stmt(bparser *parser)
{
    bexpdesc e;
    bproto *proto;
    bfuncinfo *finfo = parser->finfo;
    /* 'def' ID '(' varlist ')' block 'end' */
    scan_next_token(parser); /* skip 'def' */
    proto = funcbody(parser, func_name(parser, &e, 0), NULL, 0);
    be_code_closure(finfo, &e, be_code_proto(finfo, proto));
    be_stackpop(parser->vm, 1);
}

static void return_stmt(bparser *parser)
{
    bexpdesc e;
    /* 'return' expr */
    scan_next_token(parser); /* skip 'return' */
    expr(parser, &e);
    if (e.v.s) { /* expression is not empty */
        check_var(parser, &e);
    }
    be_code_ret(parser->finfo, &e);
}

static void check_class_attr(bparser *parser, bclass *c, bstring *attr)
{
    if (be_class_attribute(parser->vm, c, attr) != BE_NONE) {
        push_error(parser,
            "redefinition of the attribute '%s'", str(attr));
    }
}

static void classvar_stmt(bparser *parser, bclass *c)
{
    bstring *name;
    /* 'var' ID {',' ID} */
    scan_next_token(parser); /* skip 'var' */
    if (match_id(parser, name) != NULL) {
        check_class_attr(parser, c, name);
        be_class_member_bind(parser->vm, c, name, btrue);
        while (match_skip(parser, OptComma)) { /* ',' */
            if (match_id(parser, name) != NULL) {
                check_class_attr(parser, c, name);
                be_class_member_bind(parser->vm, c, name, btrue);
            } else {
                parser_error(parser, "class var error");
            }
        }
    } else {
        parser_error(parser, "class var error");
    }
}

static void class_static_assignment_expr(bparser *parser, bexpdesc *e, bstring *name)
{
    if (match_skip(parser, OptAssign)) { /* '=' */
        bexpdesc e1, e2;
        /* parse the right expression */
        expr(parser, &e2);

        e1 = *e;        /* copy the class description */
        bexpdesc key;   /* build the member key */
        init_exp(&key, ETSTRING, 0);
        key.v.s = name;

        be_code_member(parser->finfo, &e1, &key);   /* compute member accessor */
        be_code_setvar(parser->finfo, &e1, &e2, bfalse);    /* set member */
    }
}

static void classdef_stmt(bparser *parser, bclass *c, bbool is_static)
{
    bexpdesc e;
    bstring *name;
    bproto *proto;
    /* 'def' ID '(' varlist ')' block 'end' */
    scan_next_token(parser); /* skip 'def' */
    name = func_name(parser, &e, 1);
    check_class_attr(parser, c, name);
    proto = funcbody(parser, name, c, is_static ? FUNC_STATIC : FUNC_METHOD);
    be_class_method_bind(parser->vm, c, proto->name, proto, is_static);
    be_stackpop(parser->vm, 1);
}

static void classstaticclass_stmt(bparser *parser, bclass *c_out, bexpdesc *e_out);

static void classstatic_stmt(bparser *parser, bclass *c, bexpdesc *e)
{
    bstring *name;
    /* 'static' ['var'] ID ['=' expr] {',' ID ['=' expr] } */
    /* 'static' 'def' ID '(' varlist ')' block 'end' */
    scan_next_token(parser); /* skip 'static' */
    if (next_type(parser) == KeyDef) {  /* 'static' 'def' ... */
        classdef_stmt(parser, c, btrue);
    } else if (next_type(parser) == KeyClass) {  /* 'static' 'class' ... */
        classstaticclass_stmt(parser, c, e);
    } else {
        if (next_type(parser) == KeyVar) {
            scan_next_token(parser); /* skip 'var' if any */
        }
        if (match_id(parser, name) != NULL) {
            check_class_attr(parser, c, name);
            be_class_member_bind(parser->vm, c, name, bfalse);
            class_static_assignment_expr(parser, e, name);

            while (match_skip(parser, OptComma)) { /* ',' */
                if (match_id(parser, name) != NULL) {
                    check_class_attr(parser, c, name);
                    be_class_member_bind(parser->vm, c, name, bfalse);
                    class_static_assignment_expr(parser, e, name);
                } else {
                    parser_error(parser, "class static error");
                }
            }
        } else {
            parser_error(parser, "class static error");
        }
    }
}

static void class_inherit(bparser *parser, bexpdesc *e)
{
    if (next_type(parser) == OptColon) { /* ':' */
        bexpdesc ec = *e;    /* work on a copy because we preserve original class */
        bexpdesc e1;
        scan_next_token(parser); /* skip ':' */
        expr(parser, &e1);
        check_var(parser, &e1);
        be_code_setsuper(parser->finfo, &ec, &e1);
    }
}

static void class_block(bparser *parser, bclass *c, bexpdesc *e)
{
    /* { [;] } */
    while (block_follow(parser)) {
        switch (next_type(parser)) {
        case KeyVar: classvar_stmt(parser, c); break;
        case KeyStatic: classstatic_stmt(parser, c, e); break;
        case KeyDef: classdef_stmt(parser, c, bfalse); break;
        case OptSemic: scan_next_token(parser); break;
        default: push_error(parser,
                "unexpected token '%s'", token2str(parser));
        }
    }
}

static void class_stmt(bparser *parser)
{
    bstring *name;
    /* 'class' ID [':' ID] class_block 'end' */
    scan_next_token(parser); /* skip 'class' */
    if (match_id(parser, name) != NULL) {
        bexpdesc e;
        bclass *c = be_newclass(parser->vm, name, NULL);
        new_var(parser, name, &e);
        be_code_class(parser->finfo, &e, c);
        class_inherit(parser, &e);

        bblockinfo binfo;
        begin_block(parser->finfo, &binfo, 0);

        bstring *class_str = parser_newstr(parser, "_class");   /* we always define `_class` local variable */
        bexpdesc e1;                        /* if inline class, we add a second local variable for _class */
        init_exp(&e1, ETLOCAL, 0);
        e1.v.idx = new_localvar(parser, class_str);
        be_code_setvar(parser->finfo, &e1, &e, btrue);

        begin_varinfo(parser, class_str);

        class_block(parser, c, &e);
        end_block(parser);
        
        be_class_compress(parser->vm, c); /* compress class size */
        match_token(parser, KeyEnd); /* skip 'end' */
    } else {
        parser_error(parser, "class name error");
    }
}

static void classstaticclass_stmt(bparser *parser, bclass *c_out, bexpdesc *e_out)
{
    bstring *name;
    /* [preceding 'static'] 'class' ID [':' ID] class_block 'end' */
    scan_next_token(parser); /* skip 'class' */
    if (match_id(parser, name) != NULL) {
        bexpdesc e_class;         /* new class object */
        check_class_attr(parser, c_out, name);      /* check that the class names does not collide with another member */
        be_class_member_bind(parser->vm, c_out, name, bfalse);  /* add the member slot as static */
        /* create the class object */
        bclass *c = be_newclass(parser->vm, name, NULL);
        new_var(parser, name, &e_class);    /* add a local var to the static initialization code for static members */
        be_code_class(parser->finfo, &e_class, c);
        class_inherit(parser, &e_class);
        class_block(parser, c, &e_class);
        be_class_compress(parser->vm, c); /* compress class size */
        match_token(parser, KeyEnd); /* skip 'end' */
        /* add the code to copy the class object to the static member */
        bexpdesc e1 = *e_out;        /* copy the class description */
        bexpdesc key;   /* build the member key */
        init_exp(&key, ETSTRING, 0);
        key.v.s = name;
        /* assign the class to the static member */
        be_code_member(parser->finfo, &e1, &key);   /* compute member accessor */
        be_code_setvar(parser->finfo, &e1, &e_class, bfalse);    /* set member */
    } else {
        parser_error(parser, "class name error");
    }
}

static void import_stmt(bparser *parser)
{
    bstring *name; /* variable name */
    bexpdesc m, v;
    /* 'import' (ID (['as' ID] | {',' ID}) | STRING 'as' ID ) */
    scan_next_token(parser); /* skip 'import' */
    init_exp(&m, ETSTRING, 0);
    m.v.s = name = next_token(parser).u.s;
    if (next_type(parser) == TokenString) { /* STRING 'as' ID */
        scan_next_token(parser); /* skip the module path */
        match_token(parser, KeyAs); /* match and skip 'as' */
        name = next_token(parser).u.s;
        match_token(parser, TokenId); /* match and skip ID */
    } else { /* ID (['as' ID] | {',' ID}) */
        match_token(parser, TokenId); /* match and skip ID */
        if (match_skip(parser, KeyAs)) { /* 'as' */
            name = next_token(parser).u.s;
            match_token(parser, TokenId); /* match and skip ID */
        } else { /* {',' ID} */
            while (match_skip(parser, OptComma)) { /* ',' */
                new_var(parser, name, &v);
                be_code_import(parser->finfo, &m, &v); /* code import */
                init_exp(&m, ETSTRING, 0); /* scanning for next node */
                m.v.s = name = next_token(parser).u.s;
                match_token(parser, TokenId); /* match and skip ID */
            }
        }
    }
    new_var(parser, name, &v);
    be_code_import(parser->finfo, &m, &v);
}

static void var_field(bparser *parser)
{
    /* ID ['=' expr] */
    bexpdesc e1, e2;
    bstring *name;
    name = next_token(parser).u.s;
    match_token(parser, TokenId); /* match and skip ID */
    if (match_skip(parser, OptAssign)) { /* '=' */
        expr(parser, &e2);
        check_var(parser, &e2);
    } else {
        init_exp(&e2, ETNIL, 0);
    }
    new_var(parser, name, &e1); /* new variable */
    be_code_setvar(parser->finfo, &e1, &e2, bfalse);
}

static void var_stmt(bparser *parser)
{
    /* 'var' ID ['=' expr] {',' ID ['=' expr]} */
    scan_next_token(parser); /* skip 'var' */
    var_field(parser);
    while (match_skip(parser, OptComma)) { /* ',' */
        var_field(parser);
    }
}

static int except_case_list(bparser *parser, int *base)
{
    int idx;
    bexpdesc e;
    bfuncinfo *finfo = parser->finfo;
    /* expr {',' expr} | '..' */
    if (match_skip(parser, OptConnect)) { /* '..' */
        *base = finfo->freereg;
        return 0;
    }
    expr(parser, &e); /* first exception expression */
    check_var(parser, &e);
    *base = idx = be_code_nextreg(finfo, &e);
    while (match_skip(parser, OptComma)) { /* ',' */
        expr(parser, &e);
        check_var(parser, &e);
        idx = be_code_nextreg(finfo, &e);
    }
    idx = idx - *base + 1; /* count of exception expression */
    be_code_freeregs(finfo, idx);
    return idx;
}

static int except_var_list(bparser *parser, int base)
{
    bexpdesc v;
    (void)base; /* unused variable (no debugging) */
    /* [as ID [, ID]] */
    if (match_skip(parser, KeyAs)) { /* 'as' */
        bstring *name = next_token(parser).u.s;
        match_token(parser, TokenId); /* match and skip ID */
        new_var(parser, name, &v); /* new local variable */
        be_assert(v.type == ETLOCAL && v.v.idx == base);
        if (match_skip(parser, OptComma)) { /* match and skip ',' */
            name = next_token(parser).u.s;
            match_token(parser, TokenId); /* match and skip ID */
            new_var(parser, name, &v); /* new local variable */
            be_assert(v.type == ETLOCAL && v.v.idx == base + 1);
            return 2;
        }
        return 1;
    }
    return 0;
}

static void except_block(bparser *parser, int *jmp, int *jbrk)
{
    int base = 0; /* the first register of the catch opcode */
    int ecnt = 0; /* exception cases count */
    int vcnt = 0; /* exception variable count */
    bblockinfo binfo;
    bfuncinfo *finfo = parser->finfo;
    /* 'except' (expr {',' expr} | '..') ['as' ID [',' ID]] */
    match_token(parser, KeyExcept); /* skip 'except' */
    begin_block(finfo, &binfo, 0); /* begin catch block */
    /* link from the previous except failure point */
    be_code_conjump(finfo, jmp, finfo->pc);
    /* (expr {',' expr} | '..') ['as' ID [',' ID]] */
    ecnt = except_case_list(parser, &base);
    vcnt = except_var_list(parser, base);
    be_code_catch(finfo, base, ecnt, vcnt, jmp);
    stmtlist(parser);
    be_code_conjump(finfo, jbrk, be_code_jump(finfo));
    end_block(parser); /* leave catch block */
}

static void try_stmt(bparser *parser)
{
    int jcatch, jbrk;
    /* 'try' block 'except' except_stmt block 'end' */
    scan_next_token(parser); /* skip 'try' */
    jcatch = be_code_exblk(parser->finfo, 0);
    block(parser, BLOCK_EXCEPT);
    be_code_exblk(parser->finfo, 1);
    jbrk = be_code_jump(parser->finfo);
    except_block(parser, &jcatch, &jbrk);
    while (next_type(parser) == KeyExcept) {
        except_block(parser, &jcatch, &jbrk);
    }
    be_code_patchjump(parser->finfo, jcatch);
    be_code_raise(parser->finfo, NULL, NULL);
    be_code_patchjump(parser->finfo, jbrk);
    match_token(parser, KeyEnd); /* skip 'end' */
}

static void throw_stmt(bparser *parser)
{
    bexpdesc e1, e2;
    /* 'raise' expr */
    scan_next_token(parser); /* skip 'raise' */
    expr(parser, &e1);
    check_var(parser, &e1);
    if (match_skip(parser, OptComma)) {
        expr(parser, &e2);
        check_var(parser, &e2);
        be_code_raise(parser->finfo, &e1, &e2);
    } else {
        be_code_raise(parser->finfo, &e1, NULL);
    }
}

static void statement(bparser *parser)
{
    /* save value of sideeffect */
    bbyte sideeffect = parser->finfo->binfo->sideeffect;
    parser->finfo->binfo->sideeffect = 1;   /* by default declare side effect */
    switch (next_type(parser)) {
    case KeyIf: if_stmt(parser); break;
    case KeyWhile: while_stmt(parser); break;
    case KeyFor: for_stmt(parser); break;
    case KeyDo: do_stmt(parser); break;
    case KeyBreak: break_stmt(parser); break;
    case KeyContinue: continue_stmt(parser); break;
    case KeyDef: def_stmt(parser); break;
    case KeyClass: class_stmt(parser); break;
    case KeyReturn: return_stmt(parser); break;
    case KeyImport: import_stmt(parser); break;
    case KeyVar: var_stmt(parser); break;
    case KeyTry: try_stmt(parser); break;
    case KeyRaise: throw_stmt(parser); break;
    case OptSemic:
        parser->finfo->binfo->sideeffect = sideeffect;      /* restore sideeffect */
        scan_next_token(parser); break; /* empty statement */
    default:
        parser->finfo->binfo->sideeffect = sideeffect;      /* restore sideeffect */
        expr_stmt(parser);
        if (comp_is_strict(parser->vm) && parser->finfo->binfo->sideeffect == 0) {
            push_error(parser, "strict: expression without side effect detected");
        }
        break;
    }
    be_assert(parser->finfo->freereg >= be_list_count(parser->finfo->local));
}

static void stmtlist(bparser *parser)
{
    while (block_follow(parser)) {
        statement(parser);
    }
}

static void block(bparser *parser, int type)
{
    bblockinfo binfo;
    begin_block(parser->finfo, &binfo, type);
    stmtlist(parser);
    end_block(parser);
}

static void mainfunc(bparser *parser, bclosure *cl)
{
    bblockinfo binfo;
    bfuncinfo finfo;
    begin_func(parser, &finfo, &binfo);
    finfo.proto->argc = 0; /* args */
    finfo.proto->name = be_newstr(parser->vm, funcname(parser));
    cl->proto = finfo.proto;
    be_remove(parser->vm, -3);  /* pop proto from stack */
    stmtlist(parser);
    end_func(parser);
    match_token(parser, TokenEOS); /* skip EOS */
}

bclosure* be_parser_source(bvm *vm,
    const char *fname, breader reader, void *data, bbool islocal)
{
    bparser parser;
    bclosure *cl = be_newclosure(vm, 0);
    parser.vm = vm;
    parser.finfo = NULL;
    parser.cl = cl;
    parser.islocal = (bbyte)islocal;
    var_setclosure(vm->top, cl);
    be_stackpush(vm);
    be_lexer_init(&parser.lexer, vm, fname, reader, data);
    scan_next_token(&parser); /* scan first token */
    mainfunc(&parser, cl);
    be_lexer_deinit(&parser.lexer);
    be_global_release_space(vm); /* clear global space */
    be_stackpop(vm, 2); /* pop strtab */
    scan_next_token(&parser); /* clear lexer */
    return cl;
}

#endif
