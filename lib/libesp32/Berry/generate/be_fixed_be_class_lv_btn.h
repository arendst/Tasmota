#include "be_constobj.h"

static be_define_const_map_slots(be_class_lv_btn_map) {
    { be_const_key(get_state, -1), be_const_func(lvbe_btn_get_state) },
    { be_const_key(get_fit_top, 10), be_const_func(lvbe_btn_get_fit_top) },
    { be_const_key(get_fit_bottom, 0), be_const_func(lvbe_btn_get_fit_bottom) },
    { be_const_key(set_fit4, -1), be_const_func(lvbe_btn_set_fit4) },
    { be_const_key(get_fit_right, -1), be_const_func(lvbe_btn_get_fit_right) },
    { be_const_key(create, -1), be_const_func(lvbe_btn_create) },
    { be_const_key(set_fit, -1), be_const_func(lvbe_btn_set_fit) },
    { be_const_key(tostring, 3), be_const_func(lvx_tostring) },
    { be_const_key(set_layout, -1), be_const_func(lvbe_btn_set_layout) },
    { be_const_key(get_fit_left, -1), be_const_func(lvbe_btn_get_fit_left) },
    { be_const_key(dot_p, 13), be_const_int(0) },
    { be_const_key(set_state, 6), be_const_func(lvbe_btn_set_state) },
    { be_const_key(set_checkable, 14), be_const_func(lvbe_btn_set_checkable) },
    { be_const_key(toggle, -1), be_const_func(lvbe_btn_toggle) },
    { be_const_key(get_layout, 16), be_const_func(lvbe_btn_get_layout) },
    { be_const_key(init, 9), be_const_func(lvbe_btn_create) },
    { be_const_key(get_checkable, -1), be_const_func(lvbe_btn_get_checkable) },
    { be_const_key(set_fit2, 2), be_const_func(lvbe_btn_set_fit2) },
};

static be_define_const_map(
    be_class_lv_btn_map,
    18
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_lv_btn,
    1,
    (bclass *)&be_class_lv_obj,
    lv_btn
);
