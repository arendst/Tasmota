#include "be_constobj.h"

static be_define_const_map_slots(be_class_lv_roller_map) {
    { be_const_key(set_options, -1), be_const_func(lvbe_roller_set_options) },
    { be_const_key(get_align, 6), be_const_func(lvbe_roller_get_align) },
    { be_const_key(set_visible_row_count, 12), be_const_func(lvbe_roller_set_visible_row_count) },
    { be_const_key(get_selected_str, -1), be_const_func(lvbe_roller_get_selected_str) },
    { be_const_key(get_selected, -1), be_const_func(lvbe_roller_get_selected) },
    { be_const_key(set_selected, -1), be_const_func(lvbe_roller_set_selected) },
    { be_const_key(set_anim_time, 0), be_const_func(lvbe_roller_set_anim_time) },
    { be_const_key(set_align, -1), be_const_func(lvbe_roller_set_align) },
    { be_const_key(get_auto_fit, -1), be_const_func(lvbe_roller_get_auto_fit) },
    { be_const_key(set_auto_fit, 10), be_const_func(lvbe_roller_set_auto_fit) },
    { be_const_key(get_anim_time, -1), be_const_func(lvbe_roller_get_anim_time) },
    { be_const_key(get_option_cnt, 5), be_const_func(lvbe_roller_get_option_cnt) },
    { be_const_key(tostring, -1), be_const_func(lvx_tostring) },
    { be_const_key(get_options, 14), be_const_func(lvbe_roller_get_options) },
    { be_const_key(dot_p, -1), be_const_int(0) },
    { be_const_key(init, 7), be_const_func(lvbe_roller_create) },
    { be_const_key(create, -1), be_const_func(lvbe_roller_create) },
};

static be_define_const_map(
    be_class_lv_roller_map,
    17
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_lv_roller,
    1,
    (bclass *)&be_class_lv_obj,
    lv_roller
);
