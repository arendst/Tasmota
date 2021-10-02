#include "be_constobj.h"

static be_define_const_map_slots(be_class_ctypes_dyn_map) {
    { be_const_key(_def, -1), be_const_var(0) },
    { be_const_key(init, 0), be_const_func(be_ctypes_dyn_init) },
};

static be_define_const_map(
    be_class_ctypes_dyn_map,
    2
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_ctypes_dyn,
    1,
    (bclass *)&be_class_ctypes,
    ctypes_bytes_dyn
);
