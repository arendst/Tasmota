#include "be_constobj.h"

static be_define_const_map_slots(be_class_lv_objmask_map) {
    { be_const_key(dot_p, -1), be_const_int(0) },
    { be_const_key(tostring, 0), be_const_func(lvx_tostring) },
    { be_const_key(remove_mask, -1), be_const_func(lvbe_objmask_remove_mask) },
    { be_const_key(init, -1), be_const_func(lvbe_objmask_create) },
    { be_const_key(create, -1), be_const_func(lvbe_objmask_create) },
    { be_const_key(update_mask, 4), be_const_func(lvbe_objmask_update_mask) },
};

static be_define_const_map(
    be_class_lv_objmask_map,
    6
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_lv_objmask,
    1,
    (bclass *)&be_class_lv_obj,
    lv_objmask
);
