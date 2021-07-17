#include "be_constobj.h"

static be_define_const_map_slots(be_class_lv_ctypes_map) {
    { be_const_key(dot_def, -1), be_const_int(0) },
    { be_const_key(setmember, -1), be_const_func(be_ctypes_setmember) },
    { be_const_key(init, -1), be_const_func(be_ctypes_init) },
    { be_const_key(member, 2), be_const_func(be_ctypes_member) },
};

static be_define_const_map(
    be_class_lv_ctypes_map,
    4
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_lv_ctypes,
    1,
    (bclass *)&be_class_bytes,
    ctypes_bytes
);
