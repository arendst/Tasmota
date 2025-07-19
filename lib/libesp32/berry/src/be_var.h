/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#ifndef BE_VAR_H
#define BE_VAR_H

#include "be_object.h"

#define be_global_count(vm) \
    be_vector_count(&(vm)->gbldesc.global.vlist)

#define be_builtin_count(vm) \
    be_vector_count(&(vm)->gbldesc.builtin.vlist)

void be_globalvar_init(bvm *vm);
void be_globalvar_deinit(bvm *vm);
int be_global_find(bvm *vm, bstring *name);
int be_global_new(bvm *vm, bstring *name);
bbool be_global_undef(bvm *vm, bstring *name);
bvalue* be_global_var(bvm *vm, int index);
void be_global_release_space(bvm *vm);
int be_builtin_find(bvm *vm, bstring *name);
bstring* be_builtin_name(bvm *vm, int index);
int be_builtin_new(bvm *vm, bstring *name);
void be_bulitin_release_space(bvm *vm);
void be_const_builtin_set(bvm *vm, const bmap *map, const bvector *vec);

#endif
