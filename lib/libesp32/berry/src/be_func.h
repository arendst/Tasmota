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
#if BE_USE_COMPACT_KTAB
/* Build a proto's compact constant table (kval+ktype, in a single
 * allocation) from a temporary `bvalue` array of `nconst` entries.
 * Sets proto->kval, proto->ktype and proto->nconst. Does not free `src`;
 * the caller owns it. `src` may be NULL when nconst == 0. */
void be_proto_set_ktab(bvm *vm, bproto *proto, const bvalue *src, int nconst);
/* Size in bytes of a runtime-allocated compact ktab block of `nconst`. */
#define be_proto_ktab_size(_nconst) \
    ((size_t)(_nconst) * (sizeof(union bvaldata) + sizeof(bbyte)))
#endif
bclosure* be_newclosure(bvm *vm, int nupval);
bntvclos* be_newntvclosure(bvm *vm, bntvfunc cf, int nupvals);
bstring* be_func_varname(bproto *proto, int index, int pc);

#endif
