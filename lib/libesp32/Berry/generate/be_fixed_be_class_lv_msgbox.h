#include "be_constobj.h"

static be_define_const_map_slots(be_class_lv_msgbox_map) {
    { be_const_key(get_text, 12), be_const_func(lvbe_msgbox_get_text) },
    { be_const_key(set_anim_time, 3), be_const_func(lvbe_msgbox_set_anim_time) },
    { be_const_key(tostring, 0), be_const_func(lvx_tostring) },
    { be_const_key(add_btns, 4), be_const_func(lvbe_msgbox_add_btns) },
    { be_const_key(get_active_btn, -1), be_const_func(lvbe_msgbox_get_active_btn) },
    { be_const_key(stop_auto_close, -1), be_const_func(lvbe_msgbox_stop_auto_close) },
    { be_const_key(start_auto_close, -1), be_const_func(lvbe_msgbox_start_auto_close) },
    { be_const_key(init, -1), be_const_func(lvbe_msgbox_create) },
    { be_const_key(get_btnmatrix, -1), be_const_func(lvbe_msgbox_get_btnmatrix) },
    { be_const_key(get_anim_time, 6), be_const_func(lvbe_msgbox_get_anim_time) },
    { be_const_key(set_text_fmt, -1), be_const_func(lvbe_msgbox_set_text_fmt) },
    { be_const_key(set_recolor, 10), be_const_func(lvbe_msgbox_set_recolor) },
    { be_const_key(get_active_btn_text, -1), be_const_func(lvbe_msgbox_get_active_btn_text) },
    { be_const_key(dot_p, 8), be_const_int(0) },
    { be_const_key(get_recolor, -1), be_const_func(lvbe_msgbox_get_recolor) },
    { be_const_key(set_text, 7), be_const_func(lvbe_msgbox_set_text) },
    { be_const_key(create, -1), be_const_func(lvbe_msgbox_create) },
};

static be_define_const_map(
    be_class_lv_msgbox_map,
    17
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_lv_msgbox,
    1,
    (bclass *)&be_class_lv_obj,
    lv_msgbox
);
