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

extern int m_path_listdir(bvm *vm);

static int m_path_exists(bvm *vm)
{
    const char *path = NULL;
    if (be_top(vm) >= 1 && be_isstring(vm, 1)) {
        path = be_tostring(vm, 1);
        be_pushbool(vm, be_isexist(path));
    } else {
        be_pushbool(vm, bfalse);
    }
    be_return(vm);
}
extern time_t be_last_modified(void *hfile);

static int m_path_last_modified(bvm *vm)
{
    if (be_top(vm) >= 1 && be_isstring(vm, 1)) {
        const char *path = be_tostring(vm, 1);
        void * f = be_fopen(path, "r");
        if (f) {
            be_pushint(vm, be_last_modified(f));
            be_fclose(f);
            be_return(vm);
        }
    }
    be_return_nil(vm);
}

static int m_path_remove(bvm *vm)
{
    const char *path = NULL;
    if (be_top(vm) >= 1 && be_isstring(vm, 1)) {
        path = be_tostring(vm, 1);
        be_pushbool(vm, be_unlink(path));
    } else {
        be_pushbool(vm, bfalse);
    }
    be_return(vm);
}

/* @const_object_info_begin
module path (scope: global, file: tasmota_path) {
    exists, func(m_path_exists)
    last_modified, func(m_path_last_modified)
    listdir, func(m_path_listdir)
    remove, func(m_path_remove)
}
@const_object_info_end */
#include "be_fixed_tasmota_path.h"
