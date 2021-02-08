#include "be_constobj.h"

static be_define_const_map_slots(m_libdebug_map) {
    { be_const_key(attrdump, -1), be_const_func(m_attrdump) },
    { be_const_key(upvname, -1), be_const_func(m_upvname) },
    { be_const_key(codedump, -1), be_const_func(m_codedump) },
    { be_const_key(top, -1), be_const_func(m_top) },
    { be_const_key(sethook, 3), be_const_func(m_sethook) },
    { be_const_key(varname, 7), be_const_func(m_varname) },
    { be_const_key(calldepth, -1), be_const_func(m_calldepth) },
    { be_const_key(traceback, -1), be_const_func(m_traceback) },
};

static be_define_const_map(
    m_libdebug_map,
    8
);

static be_define_const_module(
    m_libdebug,
    "debug"
);

BE_EXPORT_VARIABLE be_define_const_native_module(debug, NULL);
