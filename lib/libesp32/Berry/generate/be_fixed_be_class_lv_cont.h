#include "be_constobj.h"

static be_define_const_map_slots(be_class_lv_cont_map) {
    { be_const_key(tostring, -1), be_const_func(lvx_tostring) },
    { be_const_key(get_layout, -1), be_const_func(lvbe_cont_get_layout) },
    { be_const_key(set_fit2, -1), be_const_func(lvbe_cont_set_fit2) },
    { be_const_key(set_fit, -1), be_const_func(lvbe_cont_set_fit) },
    { be_const_key(get_fit_top, -1), be_const_func(lvbe_cont_get_fit_top) },
    { be_const_key(get_fit_bottom, -1), be_const_func(lvbe_cont_get_fit_bottom) },
    { be_const_key(init, 9), be_const_func(lvbe_cont_create) },
    { be_const_key(create, -1), be_const_func(lvbe_cont_create) },
    { be_const_key(get_fit_right, 1), be_const_func(lvbe_cont_get_fit_right) },
    { be_const_key(get_fit_left, -1), be_const_func(lvbe_cont_get_fit_left) },
    { be_const_key(set_layout, -1), be_const_func(lvbe_cont_set_layout) },
    { be_const_key(dot_p, 7), be_const_int(0) },
    { be_const_key(set_fit4, -1), be_const_func(lvbe_cont_set_fit4) },
};

static be_define_const_map(
    be_class_lv_cont_map,
    13
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_lv_cont,
    1,
    (bclass *)&be_class_lv_obj,
    lv_cont
);
