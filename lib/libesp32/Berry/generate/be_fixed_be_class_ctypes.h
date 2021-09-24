#include "be_constobj.h"

static be_define_const_map_slots(be_class_ctypes_map) {
    { be_const_key(_def, 5), be_const_nil() },
    { be_const_key(setmember, 0), be_const_func(be_ctypes_setmember) },
    { be_const_key(copy, -1), be_const_func(be_ctypes_copy) },
    { be_const_key(init, -1), be_const_func(be_ctypes_init) },
    { be_const_key(tomap, -1), be_const_func(be_ctypes_tomap) },
    { be_const_key(member, -1), be_const_func(be_ctypes_member) },
};

static be_define_const_map(
    be_class_ctypes_map,
    6
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_ctypes,
    0,
    (bclass *)&be_class_bytes,
    ctypes_bytes
);
