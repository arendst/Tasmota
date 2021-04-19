#include "be_constobj.h"

static be_define_const_map_slots(be_class_lv_bar_map) {
    { be_const_key(set_value, 8), be_const_func(lvbe_bar_set_value) },
    { be_const_key(get_anim_time, -1), be_const_func(lvbe_bar_get_anim_time) },
    { be_const_key(set_anim_time, 7), be_const_func(lvbe_bar_set_anim_time) },
    { be_const_key(set_range, -1), be_const_func(lvbe_bar_set_range) },
    { be_const_key(get_type, 11), be_const_func(lvbe_bar_get_type) },
    { be_const_key(get_start_value, -1), be_const_func(lvbe_bar_get_start_value) },
    { be_const_key(set_type, -1), be_const_func(lvbe_bar_set_type) },
    { be_const_key(create, -1), be_const_func(lvbe_bar_create) },
    { be_const_key(init, -1), be_const_func(lvbe_bar_create) },
    { be_const_key(get_min_value, -1), be_const_func(lvbe_bar_get_min_value) },
    { be_const_key(tostring, 1), be_const_func(lvx_tostring) },
    { be_const_key(set_start_value, 13), be_const_func(lvbe_bar_set_start_value) },
    { be_const_key(get_value, -1), be_const_func(lvbe_bar_get_value) },
    { be_const_key(dot_p, -1), be_const_int(0) },
    { be_const_key(get_max_value, -1), be_const_func(lvbe_bar_get_max_value) },
};

static be_define_const_map(
    be_class_lv_bar_map,
    15
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_lv_bar,
    1,
    (bclass *)&be_class_lv_obj,
    lv_bar
);
