#include "be_constobj.h"

static be_define_const_map_slots(be_class_lv_arc_map) {
    { be_const_key(set_type, -1), be_const_func(lvbe_arc_set_type) },
    { be_const_key(set_adjustable, -1), be_const_func(lvbe_arc_set_adjustable) },
    { be_const_key(set_bg_start_angle, -1), be_const_func(lvbe_arc_set_bg_start_angle) },
    { be_const_key(set_chg_rate, -1), be_const_func(lvbe_arc_set_chg_rate) },
    { be_const_key(set_start_angle, 18), be_const_func(lvbe_arc_set_start_angle) },
    { be_const_key(get_min_value, -1), be_const_func(lvbe_arc_get_min_value) },
    { be_const_key(create, 7), be_const_func(lvbe_arc_create) },
    { be_const_key(dot_p, 16), be_const_int(0) },
    { be_const_key(get_bg_angle_start, 5), be_const_func(lvbe_arc_get_bg_angle_start) },
    { be_const_key(is_dragged, -1), be_const_func(lvbe_arc_is_dragged) },
    { be_const_key(set_range, 21), be_const_func(lvbe_arc_set_range) },
    { be_const_key(get_angle_end, 6), be_const_func(lvbe_arc_get_angle_end) },
    { be_const_key(get_max_value, 9), be_const_func(lvbe_arc_get_max_value) },
    { be_const_key(tostring, -1), be_const_func(lvx_tostring) },
    { be_const_key(set_bg_angles, -1), be_const_func(lvbe_arc_set_bg_angles) },
    { be_const_key(set_end_angle, -1), be_const_func(lvbe_arc_set_end_angle) },
    { be_const_key(set_bg_end_angle, 23), be_const_func(lvbe_arc_set_bg_end_angle) },
    { be_const_key(get_adjustable, -1), be_const_func(lvbe_arc_get_adjustable) },
    { be_const_key(set_rotation, -1), be_const_func(lvbe_arc_set_rotation) },
    { be_const_key(set_value, 10), be_const_func(lvbe_arc_set_value) },
    { be_const_key(set_angles, -1), be_const_func(lvbe_arc_set_angles) },
    { be_const_key(init, -1), be_const_func(lvbe_arc_create) },
    { be_const_key(get_angle_start, -1), be_const_func(lvbe_arc_get_angle_start) },
    { be_const_key(get_bg_angle_end, -1), be_const_func(lvbe_arc_get_bg_angle_end) },
    { be_const_key(get_type, 4), be_const_func(lvbe_arc_get_type) },
    { be_const_key(get_value, 15), be_const_func(lvbe_arc_get_value) },
};

static be_define_const_map(
    be_class_lv_arc_map,
    26
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_lv_arc,
    1,
    (bclass *)&be_class_lv_obj,
    lv_arc
);
