#include "be_constobj.h"

static be_define_const_map_slots(be_class_lv_spinbox_map) {
    { be_const_key(get_value, 6), be_const_func(lvbe_spinbox_get_value) },
    { be_const_key(decrement, -1), be_const_func(lvbe_spinbox_decrement) },
    { be_const_key(tostring, 4), be_const_func(lvx_tostring) },
    { be_const_key(set_value, -1), be_const_func(lvbe_spinbox_set_value) },
    { be_const_key(step_next, 11), be_const_func(lvbe_spinbox_step_next) },
    { be_const_key(init, -1), be_const_func(lvbe_spinbox_create) },
    { be_const_key(set_rollover, -1), be_const_func(lvbe_spinbox_set_rollover) },
    { be_const_key(get_step, -1), be_const_func(lvbe_spinbox_get_step) },
    { be_const_key(set_padding_left, -1), be_const_func(lvbe_spinbox_set_padding_left) },
    { be_const_key(set_digit_format, -1), be_const_func(lvbe_spinbox_set_digit_format) },
    { be_const_key(increment, -1), be_const_func(lvbe_spinbox_increment) },
    { be_const_key(set_range, -1), be_const_func(lvbe_spinbox_set_range) },
    { be_const_key(get_rollover, -1), be_const_func(lvbe_spinbox_get_rollover) },
    { be_const_key(dot_p, -1), be_const_int(0) },
    { be_const_key(set_step, -1), be_const_func(lvbe_spinbox_set_step) },
    { be_const_key(step_prev, 5), be_const_func(lvbe_spinbox_step_prev) },
    { be_const_key(create, -1), be_const_func(lvbe_spinbox_create) },
};

static be_define_const_map(
    be_class_lv_spinbox_map,
    17
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_lv_spinbox,
    1,
    (bclass *)&be_class_lv_obj,
    lv_spinbox
);
