/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#ifndef BE_CODE_H
#define BE_CODE_H

#include "be_parser.h"

#define be_code_freeregs(f, n)      ((f)->freereg -= (bbyte)(n))

int be_code_allocregs(bfuncinfo *finfo, int count);
void be_code_prebinop(bfuncinfo *finfo, int op, bexpdesc *e);
void be_code_binop(bfuncinfo *finfo, int op, bexpdesc *e1, bexpdesc *e2, int dst);
int be_code_unop(bfuncinfo *finfo, int op, bexpdesc *e);
int be_code_setvar(bfuncinfo *finfo, bexpdesc *e1, bexpdesc *e2, bbool keep_reg);
int be_code_nextreg(bfuncinfo *finfo, bexpdesc *e);
int be_code_jump(bfuncinfo *finfo);
void be_code_jumpto(bfuncinfo *finfo, int dst);
void be_code_jumpbool(bfuncinfo *finfo, bexpdesc *e, int jumptrue);
void be_code_conjump(bfuncinfo *finfo, int *list, int jmp);
void be_code_patchlist(bfuncinfo *finfo, int list, int dst);
void be_code_patchjump(bfuncinfo *finfo, int jmp);
int be_code_getmethod(bfuncinfo *finfo, bexpdesc *e);
void be_code_call(bfuncinfo *finfo, int base, int argc);
int be_code_proto(bfuncinfo *finfo, bproto *proto);
void be_code_closure(bfuncinfo *finfo, bexpdesc *e, int idx);
void be_code_close(bfuncinfo *finfo, int isret);
void be_code_class(bfuncinfo *finfo, bexpdesc *dst, bclass *c);
void be_code_ret(bfuncinfo *finfo, bexpdesc *e);
int be_code_nglobal(bfuncinfo *finfo, bexpdesc *k);
void be_code_member(bfuncinfo *finfo, bexpdesc *e1, bexpdesc *e2);
void be_code_index(bfuncinfo *finfo, bexpdesc *c, bexpdesc *k);
void be_code_setsuper(bfuncinfo *finfo, bexpdesc *c, bexpdesc *s);
void be_code_import(bfuncinfo *finfo, bexpdesc *m, bexpdesc *v);
int be_code_exblk(bfuncinfo *finfo, int depth);
void be_code_catch(bfuncinfo *finfo, int base, int ecnt, int vcnt, int *jmp);
void be_code_raise(bfuncinfo *finfo, bexpdesc *e1, bexpdesc *e2);
void be_code_implicit_class(bfuncinfo *finfo, bexpdesc *e, bclass *c);

#endif
