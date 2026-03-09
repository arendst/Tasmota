/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#ifndef BE_MODULE_H
#define BE_MODULE_H

#include "be_object.h"

#define BE_MODULE_NAME    1

typedef struct bmodule {
    bcommon_header;
    bmap *table;
    union infodata {
        const bntvmodule_t *native;
        const char *name;
        const bstring *sname;
#ifdef __cplusplus
        BE_CONSTEXPR infodata(const char *name) : name(name) {}
#endif
    } info;
    bgcobject *gray; /* for gc gray list */
#ifdef __cplusplus
    BE_CONSTEXPR bmodule(bmap *tab, const char *name) :
        next(0), type(BE_MODULE), marked(GC_CONST),
        table(tab), info(infodata(name)), gray(0) {}
#endif
} bmodule;

bmodule* be_module_new(bvm *vm);
void be_module_delete(bvm *vm, bmodule *module);
int be_module_load(bvm *vm, bstring *path);
int be_module_load_nocache(bvm *vm, bstring *path, bbool cache);
void be_cache_module(bvm *vm, bstring *name);
int be_module_attr(bvm *vm, bmodule *module, bstring *attr, bvalue *dst);
bbool be_module_setmember(bvm *vm, bmodule *module, bstring *attr, bvalue *src);
const char* be_module_name(const bmodule *module);
bbool be_module_setname(bmodule *module, bstring *name);

#endif
