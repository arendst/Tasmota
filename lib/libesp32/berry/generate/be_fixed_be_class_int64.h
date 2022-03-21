#include "be_constobj.h"

static be_define_const_map_slots(be_class_int64_map) {
    { be_const_key(_X3C, -1), be_const_ctype_func(int64_lt) },
    { be_const_key(_X2F, -1), be_const_ctype_func(int64_div) },
    { be_const_key(_p, -1), be_const_var(0) },
    { be_const_key(_X2A, -1), be_const_ctype_func(int64_mul) },
    { be_const_key(_X25, -1), be_const_ctype_func(int64_mod) },
    { be_const_key(tostring, -1), be_const_ctype_func(int64_tostring) },
    { be_const_key(_X3C_X3D, -1), be_const_ctype_func(int64_lte) },
    { be_const_key(set, -1), be_const_ctype_func(int64_set) },
    { be_const_key(frombytes, -1), be_const_ctype_func(int64_frombytes) },
    { be_const_key(toint, 3), be_const_ctype_func(int64_toint) },
    { be_const_key(_X2B, -1), be_const_ctype_func(int64_add) },
    { be_const_key(_X21_X3D, -1), be_const_ctype_func(int64_nequals) },
    { be_const_key(deinit, -1), be_const_ctype_func(int64_deinit) },
    { be_const_key(_X2D, -1), be_const_ctype_func(int64_sub) },
    { be_const_key(_X2D_X2A, 1), be_const_ctype_func(int64_neg) },
    { be_const_key(_X3D_X3D, 18), be_const_ctype_func(int64_equals) },
    { be_const_key(_X3E_X3D, 13), be_const_ctype_func(int64_gte) },
    { be_const_key(_X3E, -1), be_const_ctype_func(int64_gt) },
    { be_const_key(init, -1), be_const_ctype_func(int64_init) },
    { be_const_key(tobytes, 0), be_const_ctype_func(int64_tobytes) },
};

static be_define_const_map(
    be_class_int64_map,
    20
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_int64,
    1,
    NULL,
    int64
);
