#include "be_constobj.h"

static be_define_const_map_slots(be_class_lv_led_map) {
    { be_const_key(get_bright, -1), be_const_func(lvbe_led_get_bright) },
    { be_const_key(toggle, 4), be_const_func(lvbe_led_toggle) },
    { be_const_key(on, -1), be_const_func(lvbe_led_on) },
    { be_const_key(set_bright, -1), be_const_func(lvbe_led_set_bright) },
    { be_const_key(dot_p, -1), be_const_int(0) },
    { be_const_key(off, 8), be_const_func(lvbe_led_off) },
    { be_const_key(init, 3), be_const_func(lvbe_led_create) },
    { be_const_key(tostring, -1), be_const_func(lvx_tostring) },
    { be_const_key(create, -1), be_const_func(lvbe_led_create) },
};

static be_define_const_map(
    be_class_lv_led_map,
    9
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_lv_led,
    1,
    (bclass *)&be_class_lv_obj,
    lv_led
);
