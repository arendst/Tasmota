#include "be_constobj.h"

static be_define_const_map_slots(be_class_lv_slider_map) {
    { be_const_key(get_type, -1), be_const_func(lvbe_slider_get_type) },
    { be_const_key(get_anim_time, -1), be_const_func(lvbe_slider_get_anim_time) },
    { be_const_key(dot_p, -1), be_const_int(0) },
    { be_const_key(init, 10), be_const_func(lvbe_slider_create) },
    { be_const_key(is_dragged, -1), be_const_func(lvbe_slider_is_dragged) },
    { be_const_key(tostring, -1), be_const_func(lvx_tostring) },
    { be_const_key(create, -1), be_const_func(lvbe_slider_create) },
    { be_const_key(get_value, -1), be_const_func(lvbe_slider_get_value) },
    { be_const_key(get_max_value, -1), be_const_func(lvbe_slider_get_max_value) },
    { be_const_key(set_range, 7), be_const_func(lvbe_slider_set_range) },
    { be_const_key(set_anim_time, 2), be_const_func(lvbe_slider_set_anim_time) },
    { be_const_key(get_left_value, -1), be_const_func(lvbe_slider_get_left_value) },
    { be_const_key(set_type, -1), be_const_func(lvbe_slider_set_type) },
    { be_const_key(set_value, 6), be_const_func(lvbe_slider_set_value) },
    { be_const_key(get_min_value, -1), be_const_func(lvbe_slider_get_min_value) },
    { be_const_key(set_left_value, 1), be_const_func(lvbe_slider_set_left_value) },
};

static be_define_const_map(
    be_class_lv_slider_map,
    16
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_lv_slider,
    1,
    (bclass *)&be_class_lv_obj,
    lv_slider
);
