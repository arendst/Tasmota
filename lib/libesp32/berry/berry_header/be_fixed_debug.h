#include "be_constobj.h"

static be_define_const_map_slots(m_libdebug_map) {
    { be_const_key(attrdump, 4), be_const_func(m_attrdump) },
    { be_const_key(counters, 10), be_const_func(m_counters) },
    { be_const_key(calldepth, -1), be_const_func(m_calldepth) },
    { be_const_key(traceback, -1), be_const_func(m_traceback) },
    { be_const_key(gcdebug, -1), be_const_func(m_gcdebug) },
    { be_const_key(top, -1), be_const_func(m_top) },
    { be_const_key(reallocs, 3), be_const_func(m_reallocs) },
    { be_const_key(frees, -1), be_const_func(m_frees) },
    { be_const_key(caller, -1), be_const_func(m_caller) },
    { be_const_key(allocs, -1), be_const_func(m_allocs) },
    { be_const_key(codedump, -1), be_const_func(m_codedump) },
};

static be_define_const_map(
    m_libdebug_map,
    11
);

static be_define_const_module(
    m_libdebug,
    "debug"
);

BE_EXPORT_VARIABLE be_define_const_native_module(debug);
