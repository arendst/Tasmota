#include "be_constobj.h"

static be_define_const_map_slots(be_class_lv_switch_map) {
    { be_const_key(get_anim_time, 1), be_const_func(lvbe_switch_get_anim_time) },
    { be_const_key(init, -1), be_const_func(lvbe_switch_create) },
    { be_const_key(off, -1), be_const_func(lvbe_switch_off) },
    { be_const_key(toggle, -1), be_const_func(lvbe_switch_toggle) },
    { be_const_key(on, -1), be_const_func(lvbe_switch_on) },
    { be_const_key(tostring, 0), be_const_func(lvx_tostring) },
    { be_const_key(get_state, -1), be_const_func(lvbe_switch_get_state) },
    { be_const_key(set_anim_time, 8), be_const_func(lvbe_switch_set_anim_time) },
    { be_const_key(create, -1), be_const_func(lvbe_switch_create) },
    { be_const_key(dot_p, 6), be_const_int(0) },
};

static be_define_const_map(
    be_class_lv_switch_map,
    10
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_lv_switch,
    1,
    (bclass *)&be_class_lv_obj,
    lv_switch
);
