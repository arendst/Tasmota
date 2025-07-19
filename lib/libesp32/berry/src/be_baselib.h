/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#ifndef __BE_BASELIB_H
#define __BE_BASELIB_H

#include "be_object.h"

int be_baselib_assert(bvm *vm);
int be_baselib_print(bvm *vm);
int be_baselib_input(bvm *vm);
int be_baselib_super(bvm *vm);
int be_baselib_type(bvm *vm);
int be_baselib_classname(bvm *vm);
int be_baselib_classof(bvm *vm);
int be_baselib_number(bvm *vm);
int be_baselib_str(bvm *vm);
int be_baselib_int(bvm *vm);
int be_baselib_real(bvm *vm);
int be_baselib_module(bvm *vm);
int be_baselib_size(bvm *vm);
int be_baselib_compile(bvm *vm);
int be_baselib_issubclass(bvm *vm);
int be_baselib_isinstance(bvm *vm);
int be_baselib_iterator(bvm *vm);

#endif
