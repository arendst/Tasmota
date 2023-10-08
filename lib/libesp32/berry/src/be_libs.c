/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#include "be_libs.h"

extern void be_load_baselib(bvm *vm);
extern void be_load_baselib_next(bvm *vm);
extern void be_load_listlib(bvm *vm);
extern void be_load_maplib(bvm *vm);
extern void be_load_rangelib(bvm *vm);
extern void be_load_filelib(bvm *vm);
extern void be_load_byteslib(bvm *vm);

void be_loadlibs(bvm *vm)
{
    be_load_baselib(vm);
#if !BE_USE_PRECOMPILED_OBJECT
    be_load_listlib(vm);
    be_load_maplib(vm);
    be_load_rangelib(vm);
    be_load_filelib(vm);
    be_load_byteslib(vm);
    be_load_baselib_next(vm);
#endif
}
