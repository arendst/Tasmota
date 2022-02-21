#include "be_constobj.h"

static be_define_const_map_slots(be_class_ccronexpr_map) {
    { be_const_key(deinit, 2), be_const_ctype_func(ccronexpr_init) },
    { be_const_key(_X2Ep, -1), be_const_var(0) },
    { be_const_key(next, -1), be_const_ctype_func(ccronexpr_next) },
    { be_const_key(init, 1), be_const_ctype_func(ccronexpr_init) },
};

static be_define_const_map(
    be_class_ccronexpr_map,
    4
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_ccronexpr,
    1,
    NULL,
    ccronexpr
);
