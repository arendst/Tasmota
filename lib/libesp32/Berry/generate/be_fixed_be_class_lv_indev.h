#include "be_constobj.h"

static be_define_const_map_slots(be_class_lv_indev_map) {
    { be_const_key(get_type, -1), be_const_func(lvbe_indev_get_type) },
    { be_const_key(search_obj, -1), be_const_func(lvbe_indev_search_obj) },
    { be_const_key(get_obj_act, -1), be_const_func(lvbe_indev_get_obj_act) },
    { be_const_key(init, 4), be_const_func(lv0_init) },
    { be_const_key(dot_p, -1), be_const_int(0) },
    { be_const_key(tostring, -1), be_const_func(lvx_tostring) },
    { be_const_key(enable, -1), be_const_func(lvbe_indev_enable) },
    { be_const_key(set_group, 1), be_const_func(lvbe_indev_set_group) },
};

static be_define_const_map(
    be_class_lv_indev_map,
    8
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_lv_indev,
    1,
    (bclass *)&be_class_lv_obj,
    lv_indev
);
