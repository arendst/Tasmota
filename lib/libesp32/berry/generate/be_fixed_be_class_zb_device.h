#include "be_constobj.h"

static be_define_const_map_slots(be_class_zb_device_map) {
    { be_const_key_weak(init, -1), be_const_ctype_func(zd_init) },
    { be_const_key_weak(tostring, -1), be_const_closure(zb_device_tostring_closure) },
    { be_const_key_weak(_p, -1), be_const_var(0) },
    { be_const_key_weak(member, 0), be_const_func(zd_member) },
};

static be_define_const_map(
    be_class_zb_device_map,
    4
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_zb_device,
    1,
    NULL,
    zb_device
);
