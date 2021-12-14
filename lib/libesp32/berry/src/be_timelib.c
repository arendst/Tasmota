/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#include "berry.h"
#include <time.h>

#if BE_USE_TIME_MODULE

static int m_time(bvm *vm)
{
    be_pushint(vm, (bint)time(NULL));
    be_return(vm);
}

static void time_insert(bvm *vm, const char *key, int value)
{
    be_pushstring(vm, key);
    be_pushint(vm, value);
    be_data_insert(vm, -3);
    be_pop(vm, 2);
}

static int m_dump(bvm *vm)
{
    if (be_top(vm) >= 1 && be_isint(vm, 1)) {
        time_t ts = be_toint(vm, 1);
        struct tm *t = localtime(&ts);
        be_newobject(vm, "map");
        time_insert(vm, "year", t->tm_year + 1900);
        time_insert(vm, "month", t->tm_mon + 1);
        time_insert(vm, "day", t->tm_mday);
        time_insert(vm, "hour", t->tm_hour);
        time_insert(vm, "min", t->tm_min);
        time_insert(vm, "sec", t->tm_sec);
        time_insert(vm, "weekday", t->tm_wday);
        be_pop(vm, 1);
        be_return(vm);
    }
    be_return_nil(vm);
}

static int m_clock(bvm *vm)
{
    be_pushreal(vm, clock() / (breal)CLOCKS_PER_SEC);
    be_return(vm);
}

#if !BE_USE_PRECOMPILED_OBJECT
be_native_module_attr_table(time) {
    be_native_module_function("time", m_time),
    be_native_module_function("dump", m_dump),
    be_native_module_function("clock", m_clock)
};

be_define_native_module(time, NULL);
#else
/* @const_object_info_begin
module time (scope: global, depend: BE_USE_TIME_MODULE) {
    time, func(m_time)
    dump, func(m_dump)
    clock, func(m_clock)
}
@const_object_info_end */
#include "../generate/be_fixed_time.h"
#endif

#endif /* BE_USE_TIME_MODULE */
