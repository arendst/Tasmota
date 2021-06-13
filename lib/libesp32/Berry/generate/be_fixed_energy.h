#include "be_constobj.h"

static be_define_const_map_slots(m_libenergy_map) {
    { be_const_key(read, -1), be_const_func(b_nrg_read) },
};

static be_define_const_map(
    m_libenergy_map,
    1
);

static be_define_const_module(
    m_libenergy,
    "energy"
);

BE_EXPORT_VARIABLE be_define_const_native_module(energy, NULL);
