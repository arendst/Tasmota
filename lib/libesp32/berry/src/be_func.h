/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#ifndef BE_FUNC_H
#define BE_FUNC_H

#include "be_object.h"

#define be_newntvclos(vm, cf) \
    be_newntvclosure(vm, cf, 0)

#define be_ntvclos_upval(cc, n) \
    (((bupval**)((size_t)cc + sizeof(bntvclos)))[n])

void be_initupvals(bvm *vm, bclosure *cl);
void be_upvals_close(bvm *vm, bvalue *level);
void be_release_upvalues(bvm *vm, bclosure *cl);
bproto* be_newproto(bvm *vm);
bclosure* be_newclosure(bvm *vm, int nupval);
bntvclos* be_newntvclosure(bvm *vm, bntvfunc cf, int nupvals);
bstring* be_func_varname(bproto *proto, int index, int pc);

#endif
