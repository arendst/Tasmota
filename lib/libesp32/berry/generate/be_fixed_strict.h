#include "be_constobj.h"

static be_define_const_map_slots(m_libstrict_map) {
    { be_const_key(init, -1), be_const_func(m_init) },
};

static be_define_const_map(
    m_libstrict_map,
    1
);

static be_define_const_module(
    m_libstrict,
    "strict"
);

BE_EXPORT_VARIABLE be_define_const_native_module(strict);
