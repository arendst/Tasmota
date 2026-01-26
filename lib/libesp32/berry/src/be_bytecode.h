/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#ifndef __BE_BYTECODE_H
#define __BE_BYTECODE_H

#include "be_object.h"

void be_bytecode_save(bvm *vm, const char *filename, bproto *proto);
void be_bytecode_save_to_fs(bvm *vm, void *fp, bproto *proto);
bclosure* be_bytecode_load(bvm *vm, const char *filename);
bclosure* be_bytecode_load_from_fs(bvm *vm, void *fp);
bbool be_bytecode_check(const char *path);

#endif
