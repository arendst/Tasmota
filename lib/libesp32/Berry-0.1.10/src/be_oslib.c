/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#include "be_object.h"
#include "be_strlib.h"
#include "be_mem.h"
#include "be_sys.h"
#include <string.h>
#include <stdlib.h>

#define FNAME_BUF_SIZE 512

#if BE_USE_OS_MODULE

#if !BE_USE_FILE_SYSTEM
  #error the dependent macro BE_USE_FILE_SYSTEM must be enabled
#endif

static int m_getcwd(bvm *vm)
{
    char *buf = be_malloc(vm, FNAME_BUF_SIZE);
    if (be_getcwd(buf, FNAME_BUF_SIZE)) {
        be_pushstring(vm, buf);
    } else {
        be_pushstring(vm, "");
    }
    be_free(vm, buf, FNAME_BUF_SIZE);
    be_return(vm);
}

static int m_chdir(bvm *vm)
{
    if (be_top(vm) >= 1 && be_isstring(vm, 1)) {
        int res = be_chdir(be_tostring(vm, 1));
        be_pushbool(vm, res == 0);
    }
    be_return(vm);
}

static int m_mkdir(bvm *vm)
{
    int res = 1;
    if (be_top(vm) >= 1 && be_isstring(vm, 1)) {
        res = be_mkdir(be_tostring(vm, 1));
    }
    be_pushbool(vm, res == 0);
    be_return(vm);
}

static int m_remove(bvm *vm)
{
    int res = 1;
    if (be_top(vm) >= 1 && be_isstring(vm, 1)) {
        res = be_unlink(be_tostring(vm, 1));
    }
    be_pushbool(vm, res == 0);
    be_return(vm);
}

static int m_listdir(bvm *vm)
{
    int res;
    bdirinfo info;
    if (be_top(vm) >= 1 && be_isstring(vm, 1)) {
        res = be_dirfirst(&info, be_tostring(vm, 1));
    } else {
        res = be_dirfirst(&info, ".");
    }
    be_newobject(vm, "list");
    while (res == 0) {
        const char *fn = info.name;
        if (strcmp(fn, ".") && strcmp(fn, "..")) {
            be_pushstring(vm, fn);
            be_data_push(vm, -2);
            be_pop(vm, 1);
        }
        res = be_dirnext(&info);
    }
    be_dirclose(&info);
    be_pop(vm, 1);
    be_return(vm);
}

static int m_system(bvm *vm)
{
    int res = -1, i, argc = be_top(vm);
    if (argc > 0) {
        be_tostring(vm, 1);
        be_pushstring(vm, " ");
        for (i = 2; i <= argc; ++i) {
            be_strconcat(vm, 1); /* add " " */
            be_tostring(vm, i);
            be_pushvalue(vm, i);
            be_strconcat(vm, 1); /* concat arg */
            be_pop(vm, 1);
        }
        be_pop(vm, argc);
        res = system(be_tostring(vm, 1));
    }
    be_pushint(vm, res);
    be_return(vm);
}

static int m_exit(bvm *vm)
{
    int status = 0;
    if (be_top(vm)) {
        if (be_isint(vm, 1)) {
            status = be_toindex(vm, 1); /* get the exit code */
        } else if (be_isbool(vm, 1)) {
            status = be_tobool(vm, 1) - 1; /* true: 0, false: -1 */
        } else {
            status = -1;
        }
    }
    be_exit(vm, status);
    be_return_nil(vm);
}

static int m_path_isdir(bvm *vm)
{
    const char *path = NULL;
    if (be_top(vm) >= 1 && be_isstring(vm, 1)) {
        path = be_tostring(vm, 1);
    }
    be_pushbool(vm, be_isdir(path));
    be_return(vm);
}

static int m_path_isfile(bvm *vm)
{
    const char *path = NULL;
    if (be_top(vm) >= 1 && be_isstring(vm, 1)) {
        path = be_tostring(vm, 1);
    }
    be_pushbool(vm, be_isfile(path));
    be_return(vm);
}

static int m_path_split(bvm *vm)
{
    if (be_top(vm) >= 1 && be_isstring(vm, 1)) {
        const char *path = be_tostring(vm, 1);
        const char *split = be_splitpath(path);
        size_t len = split - path;
        if (split > path + 1 && split[-1] == '/') {
            const char *p = split - 1;
            for (; p >= path && *p == '/'; --p);
            if (p >= path) {
                len = p - path + 1;
            }
        }
        be_getbuiltin(vm, "list");
        be_pushnstring(vm, path, len);
        be_pushstring(vm, split);
        be_call(vm, 2);
        be_return(vm);
    }
    be_return_nil(vm);
}

static int m_path_splitext(bvm *vm)
{
    if (be_top(vm) >= 1 && be_isstring(vm, 1)) {
        const char *path = be_tostring(vm, 1);
        const char *split = be_splitname(path);
        be_getbuiltin(vm, "list");
        be_pushnstring(vm, path, split - path);
        be_pushstring(vm, split);
        be_call(vm, 2);
        be_return(vm);
    }
    be_return_nil(vm);
}

static int m_path_exists(bvm *vm)
{
    const char *path = NULL;
    if (be_top(vm) >= 1 && be_isstring(vm, 1)) {
        path = be_tostring(vm, 1);
    }
    be_pushbool(vm, be_isexist(path));
    be_return(vm);
}

static int m_path_join(bvm *vm)
{
    char *buf, *p;
    int i, len = 0, argc = be_top(vm);
    for (i = 1; i <= argc; ++i) {
        if (be_isstring(vm, i)) {
            len += be_strlen(vm, i) + 1;
        } else {
            be_raise(vm, "type_error", "arguments must be string");
        }
    }
    buf = p = be_malloc(vm, (size_t)len + 1);
    for (i = 1; i <= argc; ++i) {
        int l = be_strlen(vm, i);
        const char *s = be_tostring(vm, i);
        if (s[0] == '/') {
            p = buf;
        }
        strcpy(p, s);
        p += l;
        if (l && s[l - 1] != '/' && i != argc) {
            *p++ = '/';
        }
    }
    be_pushnstring(vm, buf, p - buf);
    be_free(vm, buf, (size_t)len + 1);
    be_return(vm);
}

#if !BE_USE_PRECOMPILED_OBJECT
be_native_module_attr_table(path) {
    be_native_module_function("isdir", m_path_isdir),
    be_native_module_function("isfile", m_path_isfile),
    be_native_module_function("exists", m_path_exists),
    be_native_module_function("split", m_path_split),
    be_native_module_function("splitext", m_path_splitext),
    be_native_module_function("join", m_path_join)
};

static be_define_native_module(path, NULL);

be_native_module_attr_table(os) {
    be_native_module_function("getcwd", m_getcwd),
    be_native_module_function("chdir", m_chdir),
    be_native_module_function("mkdir", m_mkdir),
    be_native_module_function("remove", m_remove),
    be_native_module_function("listdir", m_listdir),
    be_native_module_function("system", m_system),
    be_native_module_function("exit", m_exit),
    be_native_module_module("path", be_native_module(path))
};

be_define_native_module(os, NULL);
#else
/* @const_object_info_begin
module path (scope: local, file: os_path, depend: BE_USE_OS_MODULE) {
    isdir, func(m_path_isdir)
    isfile, func(m_path_isfile)
    exists, func(m_path_exists)
    split, func(m_path_split)
    splitext, func(m_path_splitext)
    join, func(m_path_join)
}
@const_object_info_end */
#include "../generate/be_fixed_os_path.h"

/* @const_object_info_begin
module os (scope: global, depend: BE_USE_OS_MODULE) {
    getcwd, func(m_getcwd)
    chdir, func(m_chdir)
    mkdir, func(m_mkdir)
    remove, func(m_remove)
    listdir, func(m_listdir)
    system, func(m_system)
    exit, func(m_exit)
    path, module(m_libpath)
}
@const_object_info_end */
#include "../generate/be_fixed_os.h"
#endif

#endif /* BE_USE_OS_MODULE */
