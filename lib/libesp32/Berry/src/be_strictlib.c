/********************************************************************
** Copyright (c) 2018-2021 Guan Wenliang & Stephan Hadinger
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

#if BE_USE_STRICT_MODULE

static int m_init(bvm *vm)
{
    comp_set_strict(vm);    /* enable compiler strict mode */
    be_return_nil(vm);
}

#if !BE_USE_PRECOMPILED_OBJECT
be_native_module_attr_table(strict) {
    be_native_module_function("init", m_init),
};

be_define_native_module(strict, NULL);
#else
/* @const_object_info_begin
module strict (scope: strict, depend: BE_USE_STRICT_MODULE) {
    init, func(m_init)
}
@const_object_info_end */
#include "../generate/be_fixed_strict.h"
#endif

#endif /* BE_USE_STRICT_MODULE */
