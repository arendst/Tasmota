/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang, Stephan Hadinger
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#include "be_object.h"
#include "be_module.h"
#include "be_string.h"
#include "be_vector.h"
#include "be_class.h"
#include "be_debug.h"
#include "be_map.h"
#include "be_vm.h"
#include "be_exec.h"
#include "be_gc.h"
#include <string.h>


#if !BE_USE_PRECOMPILED_OBJECT
be_native_module_attr_table(undefined) {
    be_native_module_nil(".p"),         /* not needed but can't be empty */
};

be_define_native_module(undefined, NULL);
#else
/* @const_object_info_begin
module undefined (scope: global) {
    .p, nil()
}
@const_object_info_end */
#include "../generate/be_fixed_undefined.h"
#endif
