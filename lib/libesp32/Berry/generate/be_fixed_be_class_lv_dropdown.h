#include "be_constobj.h"

static be_define_const_map_slots(be_class_lv_dropdown_map) {
    { be_const_key(tostring, 3), be_const_func(lvx_tostring) },
    { be_const_key(member, -1), be_const_func(lvx_member) },
    { be_const_key(_p, -1), be_const_var(0) },
    { be_const_key(init, 4), be_const_func(lvbe_dropdown_create) },
    { be_const_key(_class, -1), be_const_int(&lv_dropdown_class) },
};

static be_define_const_map(
    be_class_lv_dropdown_map,
    5
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_lv_dropdown,
    1,
    (bclass *)&be_class_lv_obj,
    lv_dropdown
);
