#include "be_constobj.h"

static be_define_const_map_slots(m_libglobal_map) {
    { be_const_key(member, -1), be_const_func(m_findglobal) },
    { be_const_key(setmember, 0), be_const_func(m_setglobal) },
    { be_const_key(opt_call, -1), be_const_func(m_globals) },
};

static be_define_const_map(
    m_libglobal_map,
    3
);

static be_define_const_module(
    m_libglobal,
    "global"
);

BE_EXPORT_VARIABLE be_define_const_native_module(global);
