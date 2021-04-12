#include "be_constobj.h"

static be_define_const_map_slots(m_libsolidify_map) {
    { be_const_key(dump, -1), be_const_func(m_dump) },
};

static be_define_const_map(
    m_libsolidify_map,
    1
);

static be_define_const_module(
    m_libsolidify,
    "solidify"
);

BE_EXPORT_VARIABLE be_define_const_native_module(solidify, NULL);
