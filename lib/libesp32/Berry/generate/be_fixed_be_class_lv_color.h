#include "be_constobj.h"

static be_define_const_map_slots(be_class_lv_color_map) {
    { be_const_key(init, -1), be_const_func(lco_init) },
    { be_const_key(tostring, 2), be_const_func(lco_tostring) },
    { be_const_key(dot_p, -1), be_const_int(0) },
};

static be_define_const_map(
    be_class_lv_color_map,
    3
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_lv_color,
    1,
    NULL,
    lv_color
);
