#include "be_constobj.h"

static be_define_const_map_slots(be_class_light_state_map) {
    { be_const_key(set_huesat, -1), be_const_ctype_func(ls_set_huesat) },
    { be_const_key(RGBCT, -1), be_const_int(5) },
    { be_const_key(set_mode_rgb, -1), be_const_ctype_func(ls_set_mode_rgb) },
    { be_const_key(set_xy, -1), be_const_ctype_func(ls_set_xy) },
    { be_const_key(set_power, -1), be_const_ctype_func(ls_set_power) },
    { be_const_key(gamma8, -1), be_const_ctype_func(ls_gamma8) },
    { be_const_key(_p, -1), be_const_var(0) },
    { be_const_key(gamma10, 8), be_const_ctype_func(ls_gamma10) },
    { be_const_key(reverse_gamma10, -1), be_const_ctype_func(ls_rev_gamma10) },
    { be_const_key(RGB, -1), be_const_int(3) },
    { be_const_key(get, 17), be_const_func(light_state_get) },
    { be_const_key(member, -1), be_const_func(light_state_member) },
    { be_const_key(set_mode_ct, 0), be_const_ctype_func(ls_set_mode_ct) },
    { be_const_key(init, -1), be_const_ctype_func(ls_init) },
    { be_const_key(CT, 4), be_const_int(2) },
    { be_const_key(DIMMER, 10), be_const_int(1) },
    { be_const_key(set_ct, 1), be_const_ctype_func(ls_set_ct) },
    { be_const_key(RGBW, -1), be_const_int(4) },
    { be_const_key(set_rgb, -1), be_const_ctype_func(ls_set_rgb) },
};

static be_define_const_map(
    be_class_light_state_map,
    19
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_light_state,
    1,
    NULL,
    light_state
);
