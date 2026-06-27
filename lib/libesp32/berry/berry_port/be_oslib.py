"""
Berry os module.
Mirrors: src/be_oslib.c

This module implements the os module functions for the Berry interpreter:
getcwd, chdir, mkdir, remove, listdir, system, exit, and the os.path
sub-module (isdir, isfile, exists, split, splitext, join).

Uses Python's os module internally instead of the C be_sys.h platform layer.

Original C code is included as comments for each function.
"""

# /********************************************************************
# ** Copyright (c) 2018-2020 Guan Wenliang
# ** This file is part of the Berry default interpreter.
# ** skiars@qq.com, https://github.com/Skiars/berry
# ** See Copyright Notice in the LICENSE file or at
# ** https://github.com/Skiars/berry/blob/master/LICENSE
# ********************************************************************/

import os
import sys

from berry_port import be_api
from berry_port.be_strlib import be_splitpath, be_splitname


# ============================================================================
# os module functions
# ============================================================================

# static int m_getcwd(bvm *vm)
# {
#     char *buf = be_malloc(vm, FNAME_BUF_SIZE);
#     if (be_getcwd(buf, FNAME_BUF_SIZE)) {
#         be_pushstring(vm, buf);
#     } else {
#         be_pushstring(vm, "");
#     }
#     be_free(vm, buf, FNAME_BUF_SIZE);
#     be_return(vm);
# }
def m_getcwd(vm):
    try:
        cwd = os.getcwd()
        be_api.be_pushstring(vm, cwd)
    except OSError:
        be_api.be_pushstring(vm, "")
    return be_api.be_returnvalue(vm)


# static int m_chdir(bvm *vm)
# {
#     if (be_top(vm) >= 1 && be_isstring(vm, 1)) {
#         int res = be_chdir(be_tostring(vm, 1));
#         be_pushbool(vm, res == 0);
#     }
#     be_return(vm);
# }
def m_chdir(vm):
    if be_api.be_top(vm) >= 1 and be_api.be_isstring(vm, 1):
        path = be_api.be_tostring(vm, 1)
        try:
            os.chdir(path)
            be_api.be_pushbool(vm, True)
        except OSError:
            be_api.be_pushbool(vm, False)
    return be_api.be_returnvalue(vm)


# static int m_mkdir(bvm *vm)
# {
#     int res = 1;
#     if (be_top(vm) >= 1 && be_isstring(vm, 1)) {
#         res = be_mkdir(be_tostring(vm, 1));
#     }
#     be_pushbool(vm, res == 0);
#     be_return(vm);
# }
def m_mkdir(vm):
    res = 1
    if be_api.be_top(vm) >= 1 and be_api.be_isstring(vm, 1):
        path = be_api.be_tostring(vm, 1)
        try:
            os.mkdir(path)
            res = 0
        except OSError:
            res = 1
    be_api.be_pushbool(vm, res == 0)
    return be_api.be_returnvalue(vm)


# static int m_remove(bvm *vm)
# {
#     int res = 1;
#     if (be_top(vm) >= 1 && be_isstring(vm, 1)) {
#         res = be_unlink(be_tostring(vm, 1));
#     }
#     be_pushbool(vm, res == 0);
#     be_return(vm);
# }
def m_remove(vm):
    res = 1
    if be_api.be_top(vm) >= 1 and be_api.be_isstring(vm, 1):
        path = be_api.be_tostring(vm, 1)
        try:
            os.remove(path)
            res = 0
        except OSError:
            res = 1
    be_api.be_pushbool(vm, res == 0)
    return be_api.be_returnvalue(vm)


# static int m_listdir(bvm *vm)
# {
#     int res;
#     bdirinfo info;
#     if (be_top(vm) >= 1 && be_isstring(vm, 1)) {
#         res = be_dirfirst(&info, be_tostring(vm, 1));
#     } else {
#         res = be_dirfirst(&info, ".");
#     }
#     be_newobject(vm, "list");
#     while (res == 0) {
#         const char *fn = info.name;
#         if (strcmp(fn, ".") && strcmp(fn, "..")) {
#             be_pushstring(vm, fn);
#             be_data_push(vm, -2);
#             be_pop(vm, 1);
#         }
#         res = be_dirnext(&info);
#     }
#     be_dirclose(&info);
#     be_pop(vm, 1);
#     be_return(vm);
# }
def m_listdir(vm):
    if be_api.be_top(vm) >= 1 and be_api.be_isstring(vm, 1):
        dirpath = be_api.be_tostring(vm, 1)
    else:
        dirpath = "."
    be_api.be_newobject(vm, "list")
    try:
        entries = os.listdir(dirpath)
        for fn in entries:
            if fn != "." and fn != "..":
                be_api.be_pushstring(vm, fn)
                be_api.be_data_push(vm, -2)
                be_api.be_pop(vm, 1)
    except OSError:
        pass  # return empty list on error
    be_api.be_pop(vm, 1)
    return be_api.be_returnvalue(vm)


# static int m_system(bvm *vm)
# {
#     int res = -1, i, argc = be_top(vm);
#     if (argc > 0) {
#         be_tostring(vm, 1);
#         be_pushstring(vm, " ");
#         for (i = 2; i <= argc; ++i) {
#             be_strconcat(vm, 1); /* add " " */
#             be_tostring(vm, i);
#             be_pushvalue(vm, i);
#             be_strconcat(vm, 1); /* concat arg */
#             be_pop(vm, 1);
#         }
#         be_pop(vm, argc);
#         res = system(be_tostring(vm, 1));
#     }
#     be_pushint(vm, res);
#     be_return(vm);
# }
def m_system(vm):
    res = -1
    argc = be_api.be_top(vm)
    if argc > 0:
        be_api.be_tostring(vm, 1)
        be_api.be_pushstring(vm, " ")
        for i in range(2, argc + 1):
            be_api.be_strconcat(vm, 1)  # add " "
            be_api.be_tostring(vm, i)
            be_api.be_pushvalue(vm, i)
            be_api.be_strconcat(vm, 1)  # concat arg
            be_api.be_pop(vm, 1)
        be_api.be_pop(vm, argc)
        res = os.system(be_api.be_tostring(vm, 1))
    be_api.be_pushint(vm, res)
    return be_api.be_returnvalue(vm)


# static int m_exit(bvm *vm)
# {
#     int status = 0;
#     if (be_top(vm)) {
#         if (be_isint(vm, 1)) {
#             status = be_toindex(vm, 1); /* get the exit code */
#         } else if (be_isbool(vm, 1)) {
#             status = be_tobool(vm, 1) - 1; /* true: 0, false: -1 */
#         } else {
#             status = -1;
#         }
#     }
#     be_exit(vm, status);
#     be_return_nil(vm);
# }
def m_exit(vm):
    from berry_port.be_exec import be_exit
    status = 0
    if be_api.be_top(vm):
        if be_api.be_isint(vm, 1):
            status = be_api.be_toindex(vm, 1)
        elif be_api.be_isbool(vm, 1):
            status = (1 if be_api.be_tobool(vm, 1) else 0) - 1
        else:
            status = -1
    be_exit(vm, status)
    return be_api.be_returnnilvalue(vm)


# ============================================================================
# os.path sub-module functions
# ============================================================================

# static int m_path_isdir(bvm *vm)
# {
#     const char *path = NULL;
#     if (be_top(vm) >= 1 && be_isstring(vm, 1)) {
#         path = be_tostring(vm, 1);
#     }
#     be_pushbool(vm, be_isdir(path));
#     be_return(vm);
# }
def m_path_isdir(vm):
    path = None
    if be_api.be_top(vm) >= 1 and be_api.be_isstring(vm, 1):
        path = be_api.be_tostring(vm, 1)
    be_api.be_pushbool(vm, path is not None and os.path.isdir(path))
    return be_api.be_returnvalue(vm)


# static int m_path_isfile(bvm *vm)
# {
#     const char *path = NULL;
#     if (be_top(vm) >= 1 && be_isstring(vm, 1)) {
#         path = be_tostring(vm, 1);
#     }
#     be_pushbool(vm, be_isfile(path));
#     be_return(vm);
# }
def m_path_isfile(vm):
    path = None
    if be_api.be_top(vm) >= 1 and be_api.be_isstring(vm, 1):
        path = be_api.be_tostring(vm, 1)
    be_api.be_pushbool(vm, path is not None and os.path.isfile(path))
    return be_api.be_returnvalue(vm)


# static int m_path_exists(bvm *vm)
# {
#     const char *path = NULL;
#     if (be_top(vm) >= 1 && be_isstring(vm, 1)) {
#         path = be_tostring(vm, 1);
#     }
#     be_pushbool(vm, be_isexist(path));
#     be_return(vm);
# }
def m_path_exists(vm):
    path = None
    if be_api.be_top(vm) >= 1 and be_api.be_isstring(vm, 1):
        path = be_api.be_tostring(vm, 1)
    be_api.be_pushbool(vm, path is not None and os.path.exists(path))
    return be_api.be_returnvalue(vm)


# static int m_path_split(bvm *vm)
# {
#     if (be_top(vm) >= 1 && be_isstring(vm, 1)) {
#         const char *path = be_tostring(vm, 1);
#         const char *split = be_splitpath(path);
#         size_t len = split - path;
#         if (split > path + 1 && split[-1] == '/') {
#             const char *p = split - 1;
#             for (; p >= path && *p == '/'; --p);
#             if (p >= path) {
#                 len = p - path + 1;
#             }
#         }
#         be_getbuiltin(vm, "list");
#         be_pushnstring(vm, path, len);
#         be_pushstring(vm, split);
#         be_call(vm, 2);
#         be_return(vm);
#     }
#     be_return_nil(vm);
# }
def m_path_split(vm):
    if be_api.be_top(vm) >= 1 and be_api.be_isstring(vm, 1):
        path = be_api.be_tostring(vm, 1)
        # be_splitpath returns the filename portion (after last '/')
        filename = be_splitpath(path)
        # len = offset of filename in path
        split_pos = len(path) - len(filename)
        length = split_pos
        # Strip trailing slashes from directory part (but keep root "/")
        if split_pos > 1 and path[split_pos - 1] == '/':
            p = split_pos - 1
            while p >= 0 and path[p] == '/':
                p -= 1
            if p >= 0:
                length = p + 1
        dir_part = path[:length]
        be_api.be_getbuiltin(vm, "list")
        be_api.be_pushstring(vm, dir_part)
        be_api.be_pushstring(vm, filename)
        be_api.be_call(vm, 2)
        return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)


# static int m_path_splitext(bvm *vm)
# {
#     if (be_top(vm) >= 1 && be_isstring(vm, 1)) {
#         const char *path = be_tostring(vm, 1);
#         const char *split = be_splitname(path);
#         be_getbuiltin(vm, "list");
#         be_pushnstring(vm, path, split - path);
#         be_pushstring(vm, split);
#         be_call(vm, 2);
#         be_return(vm);
#     }
#     be_return_nil(vm);
# }
def m_path_splitext(vm):
    if be_api.be_top(vm) >= 1 and be_api.be_isstring(vm, 1):
        path = be_api.be_tostring(vm, 1)
        # be_splitname returns the index where the extension starts
        ext_pos = be_splitname(path)
        # ext_pos is an int index into path
        if isinstance(ext_pos, int):
            stem = path[:ext_pos]
            ext = path[ext_pos:]
        else:
            # be_splitname returned a string (shouldn't happen with current impl)
            stem = path
            ext = ""
        be_api.be_getbuiltin(vm, "list")
        be_api.be_pushstring(vm, stem)
        be_api.be_pushstring(vm, ext)
        be_api.be_call(vm, 2)
        return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)


# static int m_path_join(bvm *vm)
# {
#     char *buf, *p;
#     int i, len = 0, argc = be_top(vm);
#     for (i = 1; i <= argc; ++i) {
#         if (be_isstring(vm, i)) {
#             len += be_strlen(vm, i) + 1;
#         } else {
#             be_raise(vm, "type_error", "arguments must be string");
#         }
#     }
#     buf = p = be_malloc(vm, (size_t)len + 1);
#     for (i = 1; i <= argc; ++i) {
#         int l = be_strlen(vm, i);
#         const char *s = be_tostring(vm, i);
#         if (s[0] == '/') {
#             p = buf;
#         }
#         strcpy(p, s);
#         p += l;
#         if (l && s[l - 1] != '/' && i != argc) {
#             *p++ = '/';
#         }
#     }
#     be_pushnstring(vm, buf, p - buf);
#     be_free(vm, buf, (size_t)len + 1);
#     be_return(vm);
# }
def m_path_join(vm):
    argc = be_api.be_top(vm)
    for i in range(1, argc + 1):
        if not be_api.be_isstring(vm, i):
            be_api.be_raise(vm, "type_error", "arguments must be string")
    parts = []
    result = ""
    for i in range(1, argc + 1):
        s = be_api.be_tostring(vm, i)
        if s.startswith('/'):
            result = ""
        result += s
        if s and not s.endswith('/') and i != argc:
            result += '/'
    be_api.be_pushstring(vm, result)
    return be_api.be_returnvalue(vm)


# ============================================================================
# Module tables
# ============================================================================

# /* @const_object_info_begin
# module path (scope: local, file: os_path, depend: BE_USE_OS_MODULE) {
#     isdir, func(m_path_isdir)
#     isfile, func(m_path_isfile)
#     exists, func(m_path_exists)
#     split, func(m_path_split)
#     splitext, func(m_path_splitext)
#     join, func(m_path_join)
# }
# @const_object_info_end */

def be_os_path_module_table():
    """Return the native module attribute table for the os.path sub-module."""
    return [
        ("isdir", m_path_isdir),
        ("isfile", m_path_isfile),
        ("exists", m_path_exists),
        ("split", m_path_split),
        ("splitext", m_path_splitext),
        ("join", m_path_join),
    ]


# /* @const_object_info_begin
# module os (scope: global, depend: BE_USE_OS_MODULE) {
#     getcwd, func(m_getcwd)
#     chdir, func(m_chdir)
#     mkdir, func(m_mkdir)
#     remove, func(m_remove)
#     listdir, func(m_listdir)
#     system, func(m_system)
#     exit, func(m_exit)
#     path, module(m_libpath)
# }
# @const_object_info_end */

def be_os_module_table():
    """Return the native module attribute table for the os module.

    The 'path' entry is a sub-module (bntvmodule), not a function.
    It is handled specially by the registration code in be_libs.py.
    """
    return [
        ("getcwd", m_getcwd),
        ("chdir", m_chdir),
        ("mkdir", m_mkdir),
        ("remove", m_remove),
        ("listdir", m_listdir),
        ("system", m_system),
        ("exit", m_exit),
        # "path" sub-module is added separately via be_os_path_module_table()
    ]
