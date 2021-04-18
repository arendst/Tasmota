#include "be_constobj.h"

static be_define_const_map_slots(be_class_lv_img_map) {
    { be_const_key(get_offset_y, -1), be_const_func(lvbe_img_get_offset_y) },
    { be_const_key(get_zoom, 2), be_const_func(lvbe_img_get_zoom) },
    { be_const_key(set_tasmota_logo, -1), be_const_func(lvbe_img_set_tasmota_logo) },
    { be_const_key(tostring, -1), be_const_func(lvx_tostring) },
    { be_const_key(get_pivot, 15), be_const_func(lvbe_img_get_pivot) },
    { be_const_key(get_auto_size, 1), be_const_func(lvbe_img_get_auto_size) },
    { be_const_key(get_offset_x, 4), be_const_func(lvbe_img_get_offset_x) },
    { be_const_key(set_angle, 0), be_const_func(lvbe_img_set_angle) },
    { be_const_key(get_src, -1), be_const_func(lvbe_img_get_src) },
    { be_const_key(set_zoom, 12), be_const_func(lvbe_img_set_zoom) },
    { be_const_key(dot_p, -1), be_const_int(0) },
    { be_const_key(create, -1), be_const_func(lvbe_img_create) },
    { be_const_key(init, -1), be_const_func(lvbe_img_create) },
    { be_const_key(get_angle, -1), be_const_func(lvbe_img_get_angle) },
    { be_const_key(set_src, -1), be_const_func(lvbe_img_set_src) },
    { be_const_key(set_pivot, 17), be_const_func(lvbe_img_set_pivot) },
    { be_const_key(set_antialias, -1), be_const_func(lvbe_img_set_antialias) },
    { be_const_key(get_antialias, -1), be_const_func(lvbe_img_get_antialias) },
    { be_const_key(set_offset_x, -1), be_const_func(lvbe_img_set_offset_x) },
    { be_const_key(set_offset_y, 10), be_const_func(lvbe_img_set_offset_y) },
    { be_const_key(get_file_name, -1), be_const_func(lvbe_img_get_file_name) },
    { be_const_key(set_auto_size, -1), be_const_func(lvbe_img_set_auto_size) },
};

static be_define_const_map(
    be_class_lv_img_map,
    22
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_lv_img,
    1,
    (bclass *)&be_class_lv_obj,
    lv_img
);
