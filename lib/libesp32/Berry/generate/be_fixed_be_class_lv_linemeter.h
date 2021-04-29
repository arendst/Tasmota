#include "be_constobj.h"

static be_define_const_map_slots(be_class_lv_linemeter_map) {
    { be_const_key(set_mirror, 9), be_const_func(lvbe_linemeter_set_mirror) },
    { be_const_key(get_max_value, 10), be_const_func(lvbe_linemeter_get_max_value) },
    { be_const_key(tostring, 12), be_const_func(lvx_tostring) },
    { be_const_key(set_value, -1), be_const_func(lvbe_linemeter_set_value) },
    { be_const_key(set_angle_offset, -1), be_const_func(lvbe_linemeter_set_angle_offset) },
    { be_const_key(draw_scale, -1), be_const_func(lvbe_linemeter_draw_scale) },
    { be_const_key(get_mirror, 11), be_const_func(lvbe_linemeter_get_mirror) },
    { be_const_key(get_line_count, -1), be_const_func(lvbe_linemeter_get_line_count) },
    { be_const_key(set_scale, -1), be_const_func(lvbe_linemeter_set_scale) },
    { be_const_key(get_value, -1), be_const_func(lvbe_linemeter_get_value) },
    { be_const_key(get_scale_angle, -1), be_const_func(lvbe_linemeter_get_scale_angle) },
    { be_const_key(get_angle_offset, -1), be_const_func(lvbe_linemeter_get_angle_offset) },
    { be_const_key(set_range, -1), be_const_func(lvbe_linemeter_set_range) },
    { be_const_key(dot_p, 5), be_const_int(0) },
    { be_const_key(get_min_value, -1), be_const_func(lvbe_linemeter_get_min_value) },
    { be_const_key(init, -1), be_const_func(lvbe_linemeter_create) },
    { be_const_key(create, -1), be_const_func(lvbe_linemeter_create) },
};

static be_define_const_map(
    be_class_lv_linemeter_map,
    17
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_lv_linemeter,
    1,
    (bclass *)&be_class_lv_obj,
    lv_linemeter
);
