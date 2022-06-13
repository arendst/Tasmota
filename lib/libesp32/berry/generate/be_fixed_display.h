#include "be_constobj.h"

static be_define_const_map_slots(m_libdisplay_map) {
    { be_const_key_weak(start, -1), be_const_func(be_ntv_display_start) },
    { be_const_key_weak(started, 0), be_const_ctype_func(be_ntv_display_started) },
    { be_const_key_weak(driver_name, -1), be_const_ctype_func(be_ntv_display_driver_name) },
    { be_const_key_weak(touch_update, 2), be_const_ctype_func(be_ntv_display_touch_update) },
    { be_const_key_weak(dimmer, -1), be_const_func(be_ntv_display_dimmer) },
};

static be_define_const_map(
    m_libdisplay_map,
    5
);

static be_define_const_module(
    m_libdisplay,
    "display"
);

BE_EXPORT_VARIABLE be_define_const_native_module(display);
