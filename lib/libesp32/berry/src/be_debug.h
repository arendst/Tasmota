/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#ifndef BE_DEBUG_H
#define BE_DEBUG_H

#include "be_object.h"

struct bhookblock {
    void *data;
    bntvhook hook;
};

void be_dumpclosure(bclosure *cl);
void be_tracestack(bvm *vm);
void be_callhook(bvm *vm, int mask);
bbool be_debug_varname(bvm *vm, int level, int index);
bbool be_debug_upvname(bvm *vm, int level, int index);

#if BE_USE_DEBUG_MODULE
void be_print_inst(binstruction ins, int pc, void* fout);
#endif

#endif
