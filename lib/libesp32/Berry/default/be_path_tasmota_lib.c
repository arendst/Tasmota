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

static int m_path_exists(bvm *vm)
{
    const char *path = NULL;
    if (be_top(vm) >= 1 && be_isstring(vm, 1)) {
        path = be_tostring(vm, 1);
    }
    be_pushbool(vm, be_isexist(path));
    be_return(vm);
}

#if !BE_USE_PRECOMPILED_OBJECT
be_native_module_attr_table(path) {
    be_native_module_function("exists", m_path_exists),
};

static be_define_native_module(path, NULL);

#else
/* @const_object_info_begin
module path (scope: global, file: tasmota_path) {
    exists, func(m_path_exists)
}
@const_object_info_end */
#include "../generate/be_fixed_tasmota_path.h"

#endif
