#include "be_constobj.h"

static be_define_const_map_slots(be_class_lv_line_map) {
    { be_const_key(get_y_invert, -1), be_const_func(lvbe_line_get_y_invert) },
    { be_const_key(dot_p, -1), be_const_int(0) },
    { be_const_key(get_auto_size, -1), be_const_func(lvbe_line_get_auto_size) },
    { be_const_key(init, -1), be_const_func(lvbe_line_create) },
    { be_const_key(set_auto_size, -1), be_const_func(lvbe_line_set_auto_size) },
    { be_const_key(set_points, 8), be_const_func(lvbe_line_set_points) },
    { be_const_key(set_y_invert, 3), be_const_func(lvbe_line_set_y_invert) },
    { be_const_key(tostring, -1), be_const_func(lvx_tostring) },
    { be_const_key(create, -1), be_const_func(lvbe_line_create) },
};

static be_define_const_map(
    be_class_lv_line_map,
    9
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_lv_line,
    1,
    (bclass *)&be_class_lv_obj,
    lv_line
);
