#include "be_constobj.h"

static be_define_const_map_slots(m_liblight_map) {
    { be_const_key(gamma8, 1), be_const_func(l_gamma8) },
    { be_const_key(get, -1), be_const_func(l_getlight) },
    { be_const_key(set, 4), be_const_func(l_setlight) },
    { be_const_key(gamma10, -1), be_const_func(l_gamma10) },
    { be_const_key(reverse_gamma10, -1), be_const_func(l_rev_gamma10) },
};

static be_define_const_map(
    m_liblight_map,
    5
);

static be_define_const_module(
    m_liblight,
    "light"
);

BE_EXPORT_VARIABLE be_define_const_native_module(light);
