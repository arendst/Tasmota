#include "be_constobj.h"

static be_define_const_map_slots(be_lvgl_cb_map) {
    { be_const_key(dot_p, -1), be_const_var(0) },
    { be_const_key(tostring, 2), be_const_func(lvx_tostring) },
    { be_const_key(call, -1), be_const_func(lv_cb_call) },
    { be_const_key(init, 0), be_const_func(lv0_init) },
};

static be_define_const_map(
    be_lvgl_cb_map,
    4
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_lvgl_cb,
    1,
    NULL,
    lv_cb
);
