"""
berry_port/be_timelib.py — Port of src/be_timelib.c

time module: wall-clock time, local-time decomposition, and a CPU clock.

Uses Python's native `time` module internally instead of the C <time.h> layer.

Original C code is included as comments for each function.
"""

# /********************************************************************
# ** Copyright (c) 2018-2020 Guan Wenliang
# ** This file is part of the Berry default interpreter.
# ** skiars@qq.com, https://github.com/Skiars/berry
# ** See Copyright Notice in the LICENSE file or at
# ** https://github.com/Skiars/berry/blob/master/LICENSE
# ********************************************************************/

import time as _time

from berry_port import be_api


# ============================================================================
# time module functions
# ============================================================================

# static int m_time(bvm *vm)
# {
#     be_pushint(vm, (bint)time(NULL));
#     be_return(vm);
# }
def m_time(vm):
    """time() -> int — seconds since the Unix epoch (integer truncation)."""
    be_api.be_pushint(vm, int(_time.time()))
    return be_api.be_returnvalue(vm)


# static void time_insert(bvm *vm, const char *key, int value)
# {
#     be_pushstring(vm, key);
#     be_pushint(vm, value);
#     be_data_insert(vm, -3);
#     be_pop(vm, 2);
# }
def _time_insert(vm, key, value):
    """Insert a (string, int) entry into the map at stack[-3]."""
    be_api.be_pushstring(vm, key)
    be_api.be_pushint(vm, value)
    be_api.be_data_insert(vm, -3)
    be_api.be_pop(vm, 2)


# static int m_dump(bvm *vm)
# {
#     if (be_top(vm) >= 1 && be_isint(vm, 1)) {
#         time_t ts = be_toint(vm, 1);
#         struct tm *t = localtime(&ts);
#         be_newobject(vm, "map");
#         time_insert(vm, "year", t->tm_year + 1900);
#         time_insert(vm, "month", t->tm_mon + 1);
#         time_insert(vm, "day", t->tm_mday);
#         time_insert(vm, "hour", t->tm_hour);
#         time_insert(vm, "min", t->tm_min);
#         time_insert(vm, "sec", t->tm_sec);
#         time_insert(vm, "weekday", t->tm_wday);
#         time_insert(vm, "epoch", ts);
#         be_pop(vm, 1);
#         be_return(vm);
#     }
#     be_return_nil(vm);
# }
def m_dump(vm):
    """dump(ts) -> map — decompose epoch seconds into local-time fields."""
    if be_api.be_top(vm) >= 1 and be_api.be_isint(vm, 1):
        ts = be_api.be_toint(vm, 1)
        t = _time.localtime(ts)
        be_api.be_newobject(vm, "map")
        _time_insert(vm, "year", t.tm_year)
        _time_insert(vm, "month", t.tm_mon)
        _time_insert(vm, "day", t.tm_mday)
        _time_insert(vm, "hour", t.tm_hour)
        _time_insert(vm, "min", t.tm_min)
        _time_insert(vm, "sec", t.tm_sec)
        # C tm_wday: 0=Sunday..6=Saturday; Python tm_wday: 0=Monday..6=Sunday
        c_wday = (t.tm_wday + 1) % 7
        _time_insert(vm, "weekday", c_wday)
        _time_insert(vm, "epoch", ts)
        be_api.be_pop(vm, 1)
        return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)


# static int m_clock(bvm *vm)
# {
#     be_pushreal(vm, clock() / (breal)CLOCKS_PER_SEC);
#     be_return(vm);
# }
def m_clock(vm):
    """clock() -> real — CPU time in seconds since process start."""
    be_api.be_pushreal(vm, _time.process_time())
    return be_api.be_returnvalue(vm)


# /* @const_object_info_begin
# module time (scope: global, depend: BE_USE_TIME_MODULE) {
#     time, func(m_time)
#     dump, func(m_dump)
#     clock, func(m_clock)
# }
# @const_object_info_end */

def be_time_module_table():
    """Return the native module attribute table for the time module."""
    return [
        ("time", m_time),
        ("dump", m_dump),
        ("clock", m_clock),
    ]
