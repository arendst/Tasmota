#include "be_constobj.h"

static be_define_const_map_slots(m_libdisplay_map) {
    { be_const_key(dimmer, -1), be_const_func(be_ntv_display_dimmer) },
    { be_const_key(start, 0), be_const_func(be_ntv_display_start) },
};

static be_define_const_map(
    m_libdisplay_map,
    2
);

static be_define_const_module(
    m_libdisplay,
    "display"
);

BE_EXPORT_VARIABLE be_define_const_native_module(display);
