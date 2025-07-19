/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#include "be_object.h"
#include "be_gc.h"

#if BE_USE_GC_MODULE

static int m_allocated(bvm *vm)
{
    size_t count = be_gc_memcount(vm);
    if (count < 0x80000000) {
        be_pushint(vm, (bint)count);
    } else {
        be_pushreal(vm, (breal)count);
    }
    be_return(vm);
}

static int m_collect(bvm *vm)
{
    be_gc_collect(vm);
    be_return_nil(vm);
}

#if !BE_USE_PRECOMPILED_OBJECT
be_native_module_attr_table(gc){
    be_native_module_function("allocated", m_allocated),
    be_native_module_function("collect", m_collect)
};

be_define_native_module(gc, NULL);
#else
/* @const_object_info_begin
module gc (scope: global, depend: BE_USE_GC_MODULE) {
    allocated, func(m_allocated)
    collect, func(m_collect)
}
@const_object_info_end */
#include "../generate/be_fixed_gc.h"
#endif

#endif /* BE_USE_SYS_MODULE */
