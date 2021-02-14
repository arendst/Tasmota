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
#include "be_debug.h"
#include "be_exec.h"

#define OP_NOT_BINARY           TokenNone
#define OP_NOT_UNARY            TokenNone
#define OP_NOT_ASSIGN           TokenNone
#define UNARY_OP_PRIO           3
#define ASSIGN_OP_PRIO          16

#define FUNC_METHOD             1
#define FUNC_ANONYMOUS          2

/* get binary operator priority */
#define binary_op_prio(op)      (pgm_read_byte(&binary_op_prio_tab[cast_int(op) - OptAdd]))

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

static void match_notoken(bparser *parser, btokentype type)
{
    if (next_type(parser) == type) { /* error when token is match */
        push_error(parser,
            "expected statement before '%s'", token2str(parser));
    }
}

static void check_symbol(bparser *parser, bexpdesc *e)
{
    if (e->type == ETVOID && e->v.s == NULL) { /* error when token is not a symbol */
        push_error(parser,
            "unexpected symbol near '%s'", token2str(parser));
    }
}

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
    for (; it->beginpc < beginpc; ++it);
    for (; it <= end; ++it) {
        if (!it->endpc) /* write to endpc only once */
            it->endpc = finfo->pc;
    }
}

#else

#define begin_varinfo(parser, name)
#define end_varinfo(parser, beginpc) (void)(beginpc)

#endif

static void begin_block(bfuncinfo *finfo, bblockinfo *binfo, int type)
{
    binfo->prev = finfo->binfo;
    finfo->binfo = binfo;
    binfo->type = (bbyte)type;
    binfo->hasupval = 0;
    binfo->beginpc = finfo->pc;
    binfo->nactlocals = (bbyte)be_list_count(finfo->local);
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
    be_list_resize(parser->vm, finfo->local, binfo->nactlocals);
    finfo->freereg = binfo->nactlocals;
    finfo->binfo = binfo->prev;
}

static void end_block(bparser *parser)
{
    end_block_ex(parser, -1);
}

static bstring* parser_source(bparser *parser)
{
    if (parser->finfo) {
        return parser->finfo->proto->source;
    }
    return be_newstr(parser->vm, parser->lexer.fname);
}

static void begin_func(bparser *parser, bfuncinfo *finfo, bblockinfo *binfo)
{
    bvm *vm = parser->vm;
    bproto *proto = be_newproto(vm);
    var_setproto(vm->top, proto);
    be_stackpush(vm);
    be_vector_init(vm, &finfo->code, sizeof(binstruction));
    proto->code = be_vector_data(&finfo->code);
    proto->codesize = be_vector_capacity(&finfo->code);
    be_vector_init(vm, &finfo->kvec, sizeof(bvalue));
    proto->ktab = be_vector_data(&finfo->kvec);
    proto->nconst = be_vector_capacity(&finfo->kvec);
    be_vector_init(vm, &finfo->pvec, sizeof(bproto*));
    proto->ptab = be_vector_data(&finfo->pvec);
    proto->nproto = be_vector_capacity(&finfo->pvec);
    proto->source = parser_source(parser);
    finfo->local = be_list_new(vm);
    var_setlist(vm->top, finfo->local);
    be_stackpush(vm);
    finfo->upval = be_map_new(vm);
    var_setmap(vm->top, finfo->upval);
    be_stackpush(vm);
    finfo->prev = parser->finfo;
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

static void end_func(bparser *parser)
{
    bvm *vm = parser->vm;
    bfuncinfo *finfo = parser->finfo;
    bproto *proto = finfo->proto;

    be_code_ret(finfo, NULL); /* append a return to last code */
    end_block(parser);
    setupvals(finfo);
    proto->code = be_vector_release(vm, &finfo->code);
    proto->codesize = finfo->pc;
    proto->ktab = be_vector_release(vm, &finfo->kvec);
    proto->nconst = be_vector_count(&finfo->kvec);
    proto->ptab = be_vector_release(vm, &finfo->pvec);
    proto->nproto = be_vector_count(&finfo->pvec);
#if BE_DEBUG_RUNTIME_INFO
    proto->lineinfo = be_vector_release(vm, &finfo->linevec);
    proto->nlineinfo = be_vector_count(&finfo->linevec);
#endif
#if BE_DEBUG_VAR_INFO
    proto->varinfo = be_vector_release(vm, &finfo->varvec);
    proto->nvarinfo = be_vector_count(&finfo->varvec);
#endif
    parser->finfo = parser->finfo->prev;
    be_stackpop(vm, 2); /* pop upval and local */
}

static btokentype get_binop(bparser *parser)
{
    btokentype op = next_type(parser);
    if (op >= OptAdd && op <= OptOr) {
        return op;
    }
    return OP_NOT_BINARY;
}

static btokentype get_unary_op(bparser *parser)
{
    btokentype op = next_type(parser);
    if (op == OptSub || op == OptNot || op == OptFlip) {
        return op; /* operator 'negative' 'not' and 'flip' */
    }
    return OP_NOT_UNARY;
}

static btokentype get_assign_op(bparser *parser)
{
    btokentype op = next_type(parser);
    if (op >= OptAssign && op <= OptRsfAssign) {
        return op;
    }
    return OP_NOT_ASSIGN;
}

static void init_exp(bexpdesc *e, exptype_t type, bint i)
{
    e->type = (bbyte)type;
    e->t = NO_JUMP;
    e->f = NO_JUMP;
    e->not = 0;
    e->v.s = NULL;
    e->v.i = i;
}

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

static int new_localvar(bparser *parser, bstring *name)
{
    bfuncinfo *finfo = parser->finfo;
    int reg = find_localvar(finfo, name, finfo->binfo->nactlocals);
    if (reg == -1) {
        bvalue *var;
        reg = be_list_count(finfo->local); /* new local index */
        var = be_list_push(parser->vm, finfo->local, NULL);
        var_setstr(var, name);
        if (reg >= finfo->freereg) {
            be_code_allocregs(finfo, 1); /* use a register */
        }
        begin_varinfo(parser, name);
    }
    return reg;
}

static int find_upval(bfuncinfo *finfo, bstring *s)
{
    bvm *vm = finfo->lexer->vm;
    bvalue *desc = be_map_findstr(vm, finfo->upval, s);
    if (desc) {
        return upval_index(desc->v.i);
    }
    return -1;
}

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

static void new_var(bparser *parser, bstring *name, bexpdesc *var)
{
    bfuncinfo *finfo = parser->finfo;
    if (finfo->prev || finfo->binfo->prev || parser->islocal) {
        init_exp(var, ETLOCAL, 0);
        var->v.idx = new_localvar(parser, name);
    } else {
        init_exp(var, ETGLOBAL, 0);
        var->v.idx = be_global_new(parser->vm, name);
        if (var->v.idx > (int)IBx_MASK) {
            push_error(parser,
                "too many global variables (in '%s')", str(name));
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
                } else if (be_global_find(vm, s) >= 0) {
                    return ETGLOBAL; /* global variable */
                } else {
                    return ETVOID; /* unknow (new variable or error) */
                }
            }
            init_exp(var, ETUPVAL, idx);
            return ETUPVAL;
        }
    }
}

static void singlevar(bparser *parser, bexpdesc *var)
{
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
    default:
        break;
    }
}

static void func_varlist(bparser *parser)
{
    bexpdesc v;
    bstring *str;
    /* '(' [ID {',' ID}] ')' */
    match_token(parser, OptLBK); /* skip '(' */
    if (match_id(parser, str) != NULL) {
        new_var(parser, str, &v); /* new variable */
        while (match_skip(parser, OptComma)) { /* ',' */
            str = next_token(parser).u.s;
            match_token(parser, TokenId); /* match and skip ID */
            /* new local variable */
            if (find_localvar(parser->finfo, str, 0) == -1) {
                new_var(parser, str, &v);
            } else {
                push_error(parser, "redefinition of '%s'", str(str));
            }
        }
    }
    match_token(parser, OptRBK); /* skip ')' */
    parser->finfo->proto->argc = parser->finfo->freereg;
}

static bproto* funcbody(bparser *parser, bstring *name, int type)
{
    bfuncinfo finfo;
    bblockinfo binfo;

    /* '(' varlist ')' block 'end' */
    begin_func(parser, &finfo, &binfo);
    finfo.proto->name = name;
    if (type & FUNC_METHOD) {
        new_localvar(parser, parser_newstr(parser, "self"));
    }
    func_varlist(parser);
    stmtlist(parser);
    end_func(parser);
    match_token(parser, KeyEnd); /* skip 'end' */
    return finfo.proto;
}

/* anonymous function */
static void anon_func(bparser *parser, bexpdesc *e)
{
    bproto *proto;
    bstring *name = parser_newstr(parser, "<anonymous>");
    /* 'def' ID '(' varlist ')' block 'end' */
    scan_next_token(parser); /* skip 'def' */
    proto = funcbody(parser, name, FUNC_ANONYMOUS);
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
            if (find_localvar(parser->finfo, str, 0) == -1) {
                new_var(parser, str, &v);
            } else {
                push_error(parser, "redefinition of '%s'", str(str));
            }
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
    e->type = ETLOCAL;
}

static void list_nextmember(bparser *parser, bexpdesc *l)
{
    bexpdesc e, v = *l;
    bfuncinfo *finfo = parser->finfo;
    expr(parser, &e); /* value */
    check_var(parser, &e);
    be_code_binop(finfo, OptConnect, &v, &e);
    be_code_freeregs(finfo, 1);
}

static void map_nextmember(bparser *parser, bexpdesc *l)
{
    bexpdesc e, v = *l;
    bfuncinfo *finfo = parser->finfo;
    expr(parser, &e); /* key */
    check_var(parser, &e);
    be_code_index(finfo, &v, &e);
    match_token(parser, OptColon); /* ':' */
    expr(parser, &e); /* value */
    check_var(parser, &e);
    be_code_setvar(finfo, &v, &e);
}

static void list_expr(bparser *parser, bexpdesc *e)
{
    /* '[' {expr ','} [expr] ']' */
    new_primtype(parser, "list", e); /* new list */
    while (next_type(parser) != OptRSB) {
        list_nextmember(parser, e);
        if (!match_skip(parser, OptComma)) { /* ',' */
            break;
        }
    }
    e->type = ETREG;
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

static int exprlist(bparser *parser, bexpdesc *e)
{
    bfuncinfo *finfo = parser->finfo;
    int n = 1;
    /* expr { ',' expr } */
    expr(parser, e);
    check_var(parser, e);
    be_code_nextreg(finfo, e);
    while (match_skip(parser, OptComma)) { /* ',' */
        expr(parser, e);
        check_var(parser, e);
        be_code_nextreg(finfo, e);
        ++n;
    }
    return n;
}

static void call_expr(bparser *parser, bexpdesc *e)
{
    bexpdesc args;
    bfuncinfo *finfo = parser->finfo;
    int argc = 0, base;
    int ismember = e->type == ETMEMBER;

    /* func '(' [exprlist] ')' */
    check_var(parser, e);
    /* code function index to next register */
    if (ismember) {
        base = be_code_getmethod(finfo, e);
    } else {
        base = be_code_nextreg(finfo, e);
    }
    scan_next_token(parser); /* skip '(' */
    if (next_type(parser) != OptRBK) {
        argc = exprlist(parser, &args);
    }
    match_token(parser, OptRBK); /* skip ')' */
    argc += ismember;
    be_code_call(finfo, base, argc);
    if (e->type != ETREG) {
        e->type = ETREG;
        e->v.idx = base;
    }
}

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
        /* sub_expr() is more efficient because there is no need to initialize e. */
        sub_expr(parser, e, ASSIGN_OP_PRIO);
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
    bbool suffix = l->type == ETINDEX || l->type == ETMEMBER;
    /* in the suffix expression, if the object is a temporary
     * variable (l->v.ss.tt == ETREG), it needs to be cached. */
    if (suffix && l->v.ss.tt == ETREG) {
        be_code_allocregs(finfo, 1);
    }
}

/* compound assignment */
static void compound_assign(bparser *parser, int op, bexpdesc *l, bexpdesc *r)
{
    if (op != OptAssign) { /* check left variable */
        check_var(parser, l);
        /* cache the register of the object when continuously assigning */
        suffix_alloc_reg(parser, l);
    }
    expr(parser, r); /* right expression */
    check_var(parser, r);
    if (op != OptAssign) { /* compound assignment */
        bexpdesc e = *l;
        op = op < OptAndAssign ? op - OptAddAssign + OptAdd
                : op - OptAndAssign + OptBitAnd;
        be_code_binop(parser->finfo, op, &e, r); /* coding operation */
        *r = e;
    }
}

static int check_newvar(bparser *parser, bexpdesc *e)
{
    if (e->type == ETGLOBAL) {
        if (e->v.idx < be_builtin_count(parser->vm)) {
            e->v.s = be_builtin_name(parser->vm, e->v.idx);
            return btrue;
        }
        return bfalse;
    }
    return e->type == ETVOID;
}

static void assign_expr(bparser *parser)
{
    bexpdesc e;
    btokentype op;
    int line = parser->lexer.linenumber;
    expr(parser, &e); /* left expression */
    check_symbol(parser, &e);
    op = get_assign_op(parser);
    if (op != OP_NOT_ASSIGN) { /* assign operator */
        bexpdesc e1;
        scan_next_token(parser);
        compound_assign(parser, op, &e, &e1);
        if (check_newvar(parser, &e)) { /* new variable */
            new_var(parser, e.v.s, &e);
        }
        if (be_code_setvar(parser->finfo, &e, &e1)) {
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
    btokentype op = get_unary_op(parser);
    if (op != OP_NOT_UNARY) {
        int line, res;
        scan_next_token(parser);
        line = parser->lexer.linenumber;
        sub_expr(parser, e, UNARY_OP_PRIO);
        check_var(parser, e);
        res = be_code_unop(finfo, op, e);
        if (res) { /* encode unary op */
            parser->lexer.linenumber = line;
            push_error(parser, "wrong type argument to unary '%s'",
                res == 1 ? "negative" : "bit-flip");
        }
    } else {
        suffix_expr(parser, e);
    }
    op = get_binop(parser);
    while (op != OP_NOT_BINARY && prio > binary_op_prio(op)) {
        bexpdesc e2;
        check_var(parser, e);
        scan_next_token(parser);
        be_code_prebinop(finfo, op, e); /* and or */
        init_exp(&e2, ETVOID, 0);
        sub_expr(parser, &e2, binary_op_prio(op));
        check_var(parser, &e2);
        be_code_binop(finfo, op, e, &e2); /* encode binary op */
        op = get_binop(parser);
    }
    if (prio == ASSIGN_OP_PRIO) {
        cond_expr(parser, e);
    }
}

static void expr(bparser *parser, bexpdesc *e)
{
    init_exp(e, ETVOID, 0);
    sub_expr(parser, e, ASSIGN_OP_PRIO);
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
    be_code_setvar(finfo, &e, it); /* code function to variable '.it' */
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
    proto = funcbody(parser, func_name(parser, &e, 0), 0);
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
    if (be_class_attribute(parser->vm, c, attr) != BE_NIL) {
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
        be_member_bind(parser->vm, c, name);
        while (match_skip(parser, OptComma)) { /* ',' */
            if (match_id(parser, name) != NULL) {
                check_class_attr(parser, c, name);
                be_member_bind(parser->vm, c, name);
            } else {
                parser_error(parser, "class var error");
            }
        }
    } else {
        parser_error(parser, "class var error");
    }
}

static void classdef_stmt(bparser *parser, bclass *c)
{
    bexpdesc e;
    bstring *name;
    bproto *proto;
    /* 'def' ID '(' varlist ')' block 'end' */
    scan_next_token(parser); /* skip 'def' */
    name = func_name(parser, &e, 1);
    check_class_attr(parser, c, name);
    proto = funcbody(parser, name, FUNC_METHOD);
    be_method_bind(parser->vm, c, proto->name, proto);
    be_stackpop(parser->vm, 1);
}

static void class_inherit(bparser *parser, bexpdesc *e)
{
    if (next_type(parser) == OptColon) { /* ':' */
        bexpdesc e1;
        scan_next_token(parser); /* skip ':' */
        expr(parser, &e1);
        check_var(parser, &e1);
        be_code_setsuper(parser->finfo, e, &e1);
    }
}

static void class_block(bparser *parser, bclass *c)
{
    /* { [;] } */
    while (block_follow(parser)) {
        switch (next_type(parser)) {
        case KeyVar: classvar_stmt(parser, c); break;
        case KeyDef: classdef_stmt(parser, c); break;
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
        class_block(parser, c);
        be_class_compress(parser->vm, c); /* compress class size */
        match_token(parser, KeyEnd); /* skip 'end' */
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
    be_code_setvar(parser->finfo, &e1, &e2);
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
    case OptSemic: scan_next_token(parser); break; /* empty statement */
    default: expr_stmt(parser); break;
    }
    be_assert(parser->finfo->freereg == be_list_count(parser->finfo->local));
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
    be_stackpop(vm, 1);
    scan_next_token(&parser); /* clear lexer */
    return cl;
}

#endif
