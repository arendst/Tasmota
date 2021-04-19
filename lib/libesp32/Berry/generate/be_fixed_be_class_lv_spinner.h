#include "be_constobj.h"

static be_define_const_map_slots(be_class_lv_spinner_map) {
    { be_const_key(tostring, 8), be_const_func(lvx_tostring) },
    { be_const_key(get_dir, -1), be_const_func(lvbe_spinner_get_dir) },
    { be_const_key(get_arc_length, -1), be_const_func(lvbe_spinner_get_arc_length) },
    { be_const_key(dot_p, 4), be_const_int(0) },
    { be_const_key(create, -1), be_const_func(lvbe_spinner_create) },
    { be_const_key(get_spin_time, -1), be_const_func(lvbe_spinner_get_spin_time) },
    { be_const_key(init, 1), be_const_func(lvbe_spinner_create) },
    { be_const_key(anim_cb, -1), be_const_func(lvbe_spinner_anim_cb) },
    { be_const_key(set_spin_time, 10), be_const_func(lvbe_spinner_set_spin_time) },
    { be_const_key(set_arc_length, -1), be_const_func(lvbe_spinner_set_arc_length) },
    { be_const_key(set_type, 7), be_const_func(lvbe_spinner_set_type) },
    { be_const_key(get_type, 3), be_const_func(lvbe_spinner_get_type) },
    { be_const_key(set_dir, 5), be_const_func(lvbe_spinner_set_dir) },
};

static be_define_const_map(
    be_class_lv_spinner_map,
    13
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_lv_spinner,
    1,
    (bclass *)&be_class_lv_obj,
    lv_spinner
);
