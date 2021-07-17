#include "be_constobj.h"

static be_define_const_map_slots(be_lv_gauge_format_cb_map) {
    { be_const_key(call, -1), be_const_func(lv_gauge_format_cb_call) },
};

static be_define_const_map(
    be_lv_gauge_format_cb_map,
    1
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_lv_gauge_format_cb,
    0,
    (bclass *)&be_lvgl_cb,
    lv_gauge_format_cb
);
