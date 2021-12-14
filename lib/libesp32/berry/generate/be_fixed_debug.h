#include "be_constobj.h"

static be_define_const_map_slots(m_libdebug_map) {
    { be_const_key(counters, 3), be_const_func(m_counters) },
    { be_const_key(traceback, -1), be_const_func(m_traceback) },
    { be_const_key(attrdump, 0), be_const_func(m_attrdump) },
    { be_const_key(calldepth, 5), be_const_func(m_calldepth) },
    { be_const_key(top, -1), be_const_func(m_top) },
    { be_const_key(codedump, -1), be_const_func(m_codedump) },
};

static be_define_const_map(
    m_libdebug_map,
    6
);

static be_define_const_module(
    m_libdebug,
    "debug"
);

BE_EXPORT_VARIABLE be_define_const_native_module(debug);
