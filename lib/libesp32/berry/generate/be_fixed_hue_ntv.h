#include "be_constobj.h"

static be_define_const_map_slots(m_libhue_ntv_map) {
    { be_const_key(full_state, -1), be_const_ctype_func(be_hue_light_full_state) },
    { be_const_key(light_state, -1), be_const_ctype_func(be_hue_light_state) },
};

static be_define_const_map(
    m_libhue_ntv_map,
    2
);

static be_define_const_module(
    m_libhue_ntv,
    "hue_ntv"
);

BE_EXPORT_VARIABLE be_define_const_native_module(hue_ntv);
