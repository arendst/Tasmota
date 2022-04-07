#include "be_constobj.h"

static be_define_const_map_slots(be_class_ccronexpr_map) {
    { be_const_key(init, -1), be_const_ctype_func(ccronexpr_init) },
    { be_const_key(now, -1), be_const_static_ctype_func(ccronexpr_now) },
    { be_const_key(next, -1), be_const_ctype_func(ccronexpr_next) },
    { be_const_key(time_reached, 1), be_const_static_ctype_func(ccronexpr_time_reached) },
    { be_const_key(_X2Ep, -1), be_const_var(0) },
};

static be_define_const_map(
    be_class_ccronexpr_map,
    5
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_ccronexpr,
    1,
    NULL,
    ccronexpr
);
