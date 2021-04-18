#include "be_constobj.h"

static be_define_const_map_slots(be_class_lv_cpicker_map) {
    { be_const_key(init, 2), be_const_func(lvbe_cpicker_create) },
    { be_const_key(get_hue, -1), be_const_func(lvbe_cpicker_get_hue) },
    { be_const_key(get_value, -1), be_const_func(lvbe_cpicker_get_value) },
    { be_const_key(set_color, 13), be_const_func(lvbe_cpicker_set_color) },
    { be_const_key(set_color_mode, -1), be_const_func(lvbe_cpicker_set_color_mode) },
    { be_const_key(get_knob_colored, 15), be_const_func(lvbe_cpicker_get_knob_colored) },
    { be_const_key(get_color_mode, -1), be_const_func(lvbe_cpicker_get_color_mode) },
    { be_const_key(set_hsv, 18), be_const_func(lvbe_cpicker_set_hsv) },
    { be_const_key(set_saturation, -1), be_const_func(lvbe_cpicker_set_saturation) },
    { be_const_key(set_knob_colored, -1), be_const_func(lvbe_cpicker_set_knob_colored) },
    { be_const_key(set_hue, 17), be_const_func(lvbe_cpicker_set_hue) },
    { be_const_key(set_color_mode_fixed, -1), be_const_func(lvbe_cpicker_set_color_mode_fixed) },
    { be_const_key(set_value, 0), be_const_func(lvbe_cpicker_set_value) },
    { be_const_key(set_type, -1), be_const_func(lvbe_cpicker_set_type) },
    { be_const_key(create, -1), be_const_func(lvbe_cpicker_create) },
    { be_const_key(get_color, 16), be_const_func(lvbe_cpicker_get_color) },
    { be_const_key(get_color_mode_fixed, -1), be_const_func(lvbe_cpicker_get_color_mode_fixed) },
    { be_const_key(tostring, -1), be_const_func(lvx_tostring) },
    { be_const_key(dot_p, -1), be_const_int(0) },
    { be_const_key(get_saturation, -1), be_const_func(lvbe_cpicker_get_saturation) },
    { be_const_key(get_hsv, -1), be_const_func(lvbe_cpicker_get_hsv) },
};

static be_define_const_map(
    be_class_lv_cpicker_map,
    21
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_lv_cpicker,
    1,
    (bclass *)&be_class_lv_obj,
    lv_cpicker
);
