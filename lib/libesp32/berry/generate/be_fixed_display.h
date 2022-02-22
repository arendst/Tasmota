#include "be_constobj.h"

static be_define_const_map_slots(m_libdisplay_map) {
    { be_const_key(touch_update, -1), be_const_ctype_func(be_ntv_display_touch_update) },
    { be_const_key(dimmer, -1), be_const_func(be_ntv_display_dimmer) },
    { be_const_key(start, -1), be_const_func(be_ntv_display_start) },
};

static be_define_const_map(
    m_libdisplay_map,
    3
);

static be_define_const_module(
    m_libdisplay,
    "display"
);

BE_EXPORT_VARIABLE be_define_const_native_module(display);
