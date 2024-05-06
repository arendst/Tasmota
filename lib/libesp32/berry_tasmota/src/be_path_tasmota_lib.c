/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/

/********************************************************************
 * Berry module `path`
 * 
 * Minimal version of `import path`
 * 
 *******************************************************************/
#include "be_object.h"
#include "be_strlib.h"
#include "be_mem.h"
#include "be_sys.h"
#include <time.h>

// provides MPATH_ constants
#include "be_port.h"

// from be_port.cpp, becasue it uses c++
extern int _m_path_action(bvm *vm, int8_t action);

static int m_path_listdir(bvm *vm){
    return _m_path_action(vm, MPATH_LISTDIR);
}
static int m_path_isdir(bvm *vm){
    return _m_path_action(vm, MPATH_ISDIR);
}
static int m_path_mkdir(bvm *vm) {
    return _m_path_action(vm, MPATH_MKDIR);
}
static int m_path_rmdir(bvm *vm) {
    return _m_path_action(vm, MPATH_RMDIR);
}
static int m_path_rename(bvm *vm) {
    return _m_path_action(vm, MPATH_RENAME);
}
static int m_path_exists(bvm *vm) {
    return _m_path_action(vm, MPATH_EXISTS);
}
static int m_path_last_modified(bvm *vm){
    return _m_path_action(vm, MPATH_MODIFIED);
}
static int m_path_remove(bvm *vm){
    return _m_path_action(vm, MPATH_REMOVE);
}

extern int be_format_fs(void);
static int m_path_format(bvm *vm)
{
    if (be_top(vm) >= 1 && be_isbool(vm, 1)) {
        if (be_tobool(vm, 1)) {
            be_pushbool(vm, be_format_fs());
            be_return(vm);
        }
    }
    be_pushbool(vm, bfalse);
    be_return(vm);
}

/* @const_object_info_begin
module path (scope: global, file: tasmota_path) {
    exists, func(m_path_exists)
    last_modified, func(m_path_last_modified)
    listdir, func(m_path_listdir)
    isdir, func(m_path_isdir)
    remove, func(m_path_remove)
    format, func(m_path_format)
    mkdir, func(m_path_mkdir)
    rmdir, func(m_path_rmdir)
    rename, func(m_path_rename)
}
@const_object_info_end */
#include "be_fixed_tasmota_path.h"
