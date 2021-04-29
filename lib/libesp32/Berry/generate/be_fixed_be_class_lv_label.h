#include "be_constobj.h"

static be_define_const_map_slots(be_class_lv_label_map) {
    { be_const_key(get_letter_on, -1), be_const_func(lvbe_label_get_letter_on) },
    { be_const_key(create, 14), be_const_func(lvbe_label_create) },
    { be_const_key(is_char_under_pos, -1), be_const_func(lvbe_label_is_char_under_pos) },
    { be_const_key(ins_text, -1), be_const_func(lvbe_label_ins_text) },
    { be_const_key(set_text_static, -1), be_const_func(lvbe_label_set_text_static) },
    { be_const_key(set_anim_speed, -1), be_const_func(lvbe_label_set_anim_speed) },
    { be_const_key(init, -1), be_const_func(lvbe_label_create) },
    { be_const_key(get_text_sel_start, -1), be_const_func(lvbe_label_get_text_sel_start) },
    { be_const_key(set_recolor, 15), be_const_func(lvbe_label_set_recolor) },
    { be_const_key(get_letter_pos, 23), be_const_func(lvbe_label_get_letter_pos) },
    { be_const_key(get_text_sel_end, -1), be_const_func(lvbe_label_get_text_sel_end) },
    { be_const_key(set_text_sel_start, 1), be_const_func(lvbe_label_set_text_sel_start) },
    { be_const_key(refr_text, 4), be_const_func(lvbe_label_refr_text) },
    { be_const_key(cut_text, 9), be_const_func(lvbe_label_cut_text) },
    { be_const_key(get_text, 16), be_const_func(lvbe_label_get_text) },
    { be_const_key(set_long_mode, -1), be_const_func(lvbe_label_set_long_mode) },
    { be_const_key(set_text, 17), be_const_func(lvbe_label_set_text) },
    { be_const_key(dot_p, -1), be_const_int(0) },
    { be_const_key(get_long_mode, -1), be_const_func(lvbe_label_get_long_mode) },
    { be_const_key(get_align, 6), be_const_func(lvbe_label_get_align) },
    { be_const_key(set_text_sel_end, 2), be_const_func(lvbe_label_set_text_sel_end) },
    { be_const_key(set_text_fmt, -1), be_const_func(lvbe_label_set_text_fmt) },
    { be_const_key(get_recolor, -1), be_const_func(lvbe_label_get_recolor) },
    { be_const_key(tostring, 24), be_const_func(lvx_tostring) },
    { be_const_key(get_anim_speed, -1), be_const_func(lvbe_label_get_anim_speed) },
    { be_const_key(set_align, -1), be_const_func(lvbe_label_set_align) },
};

static be_define_const_map(
    be_class_lv_label_map,
    26
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_lv_label,
    1,
    (bclass *)&be_class_lv_obj,
    lv_label
);
