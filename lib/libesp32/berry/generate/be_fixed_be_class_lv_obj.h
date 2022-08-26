#include "be_constobj.h"

static be_define_const_map_slots(be_class_lv_obj_map) {
    { be_const_key_weak(tostring, 3), be_const_func(lv_x_tostring) },
    { be_const_key_weak(member, -1), be_const_func(lv_x_member) },
    { be_const_key_weak(_p, -1), be_const_var(0) },
    { be_const_key_weak(init, 4), be_const_func(be_ntv_lv_obj_init) },
    { be_const_key_weak(_class, -1), be_const_comptr(&lv_obj_class) },
};

static be_define_const_map(
    be_class_lv_obj_map,
    5
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_lv_obj,
    1,
    NULL,
    lv_obj
);
