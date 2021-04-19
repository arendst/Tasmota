#include "be_constobj.h"

static be_define_const_map_slots(be_class_lv_imgbtn_map) {
    { be_const_key(dot_p, -1), be_const_int(0) },
    { be_const_key(set_state, -1), be_const_func(lvbe_imgbtn_set_state) },
    { be_const_key(set_checkable, -1), be_const_func(lvbe_imgbtn_set_checkable) },
    { be_const_key(toggle, 1), be_const_func(lvbe_imgbtn_toggle) },
    { be_const_key(get_src, -1), be_const_func(lvbe_imgbtn_get_src) },
    { be_const_key(init, 6), be_const_func(lvbe_imgbtn_create) },
    { be_const_key(tostring, -1), be_const_func(lvx_tostring) },
    { be_const_key(create, -1), be_const_func(lvbe_imgbtn_create) },
    { be_const_key(set_src, -1), be_const_func(lvbe_imgbtn_set_src) },
    { be_const_key(get_state, 0), be_const_func(lvbe_imgbtn_get_state) },
};

static be_define_const_map(
    be_class_lv_imgbtn_map,
    10
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_lv_imgbtn,
    1,
    (bclass *)&be_class_lv_obj,
    lv_imgbtn
);
