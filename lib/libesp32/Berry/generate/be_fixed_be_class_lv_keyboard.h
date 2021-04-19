#include "be_constobj.h"

static be_define_const_map_slots(be_class_lv_keyboard_map) {
    { be_const_key(set_ctrl_map, -1), be_const_func(lvbe_keyboard_set_ctrl_map) },
    { be_const_key(def_event_cb, 10), be_const_func(lvbe_keyboard_def_event_cb) },
    { be_const_key(get_textarea, 6), be_const_func(lvbe_keyboard_get_textarea) },
    { be_const_key(init, -1), be_const_func(lvbe_keyboard_create) },
    { be_const_key(get_cursor_manage, -1), be_const_func(lvbe_keyboard_get_cursor_manage) },
    { be_const_key(create, -1), be_const_func(lvbe_keyboard_create) },
    { be_const_key(set_textarea, 9), be_const_func(lvbe_keyboard_set_textarea) },
    { be_const_key(dot_p, -1), be_const_int(0) },
    { be_const_key(set_cursor_manage, -1), be_const_func(lvbe_keyboard_set_cursor_manage) },
    { be_const_key(set_map, -1), be_const_func(lvbe_keyboard_set_map) },
    { be_const_key(tostring, -1), be_const_func(lvx_tostring) },
    { be_const_key(set_mode, -1), be_const_func(lvbe_keyboard_set_mode) },
};

static be_define_const_map(
    be_class_lv_keyboard_map,
    12
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_lv_keyboard,
    1,
    (bclass *)&be_class_lv_obj,
    lv_keyboard
);
