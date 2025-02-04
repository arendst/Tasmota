#include "be_constobj.h"

static be_define_const_map_slots(m_libsys_map) {
    { be_const_key(path, -1), be_const_func(m_path) },
};

static be_define_const_map(
    m_libsys_map,
    1
);

static be_define_const_module(
    m_libsys,
    "sys"
);

BE_EXPORT_VARIABLE be_define_const_native_module(sys);
