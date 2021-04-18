#include "be_constobj.h"

static be_define_const_map_slots(be_class_lv_gauge_map) {
    { be_const_key(get_needle_img_pivot_y, -1), be_const_func(lvbe_gauge_get_needle_img_pivot_y) },
    { be_const_key(get_needle_img_pivot_x, -1), be_const_func(lvbe_gauge_get_needle_img_pivot_x) },
    { be_const_key(get_line_count, -1), be_const_func(lvbe_gauge_get_line_count) },
    { be_const_key(init, -1), be_const_func(lvbe_gauge_create) },
    { be_const_key(dot_p, 20), be_const_int(0) },
    { be_const_key(create, -1), be_const_func(lvbe_gauge_create) },
    { be_const_key(get_min_value, 2), be_const_func(lvbe_gauge_get_min_value) },
    { be_const_key(set_range, -1), be_const_func(lvbe_gauge_set_range) },
    { be_const_key(get_max_value, -1), be_const_func(lvbe_gauge_get_max_value) },
    { be_const_key(get_value, 7), be_const_func(lvbe_gauge_get_value) },
    { be_const_key(get_label_count, -1), be_const_func(lvbe_gauge_get_label_count) },
    { be_const_key(set_needle_img, 1), be_const_func(lvbe_gauge_set_needle_img) },
    { be_const_key(get_needle_count, -1), be_const_func(lvbe_gauge_get_needle_count) },
    { be_const_key(tostring, -1), be_const_func(lvx_tostring) },
    { be_const_key(set_scale, -1), be_const_func(lvbe_gauge_set_scale) },
    { be_const_key(set_critical_value, 16), be_const_func(lvbe_gauge_set_critical_value) },
    { be_const_key(get_angle_offset, -1), be_const_func(lvbe_gauge_get_angle_offset) },
    { be_const_key(get_critical_value, -1), be_const_func(lvbe_gauge_get_critical_value) },
    { be_const_key(set_formatter_cb, -1), be_const_func(lvbe_gauge_set_formatter_cb) },
    { be_const_key(set_needle_count, 4), be_const_func(lvbe_gauge_set_needle_count) },
    { be_const_key(set_angle_offset, 17), be_const_func(lvbe_gauge_set_angle_offset) },
    { be_const_key(set_value, -1), be_const_func(lvbe_gauge_set_value) },
    { be_const_key(get_scale_angle, -1), be_const_func(lvbe_gauge_get_scale_angle) },
    { be_const_key(get_needle_img, 12), be_const_func(lvbe_gauge_get_needle_img) },
};

static be_define_const_map(
    be_class_lv_gauge_map,
    24
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_lv_gauge,
    1,
    (bclass *)&be_class_lv_obj,
    lv_gauge
);
