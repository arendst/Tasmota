#include "be_constobj.h"

static be_define_const_map_slots(be_class_lv_win_map) {
    { be_const_key(set_content_size, 10), be_const_func(lvbe_win_set_content_size) },
    { be_const_key(set_layout, -1), be_const_func(lvbe_win_set_layout) },
    { be_const_key(dot_p, -1), be_const_int(0) },
    { be_const_key(get_title, -1), be_const_func(lvbe_win_get_title) },
    { be_const_key(clean, 8), be_const_func(lvbe_win_clean) },
    { be_const_key(get_content, -1), be_const_func(lvbe_win_get_content) },
    { be_const_key(set_btn_width, -1), be_const_func(lvbe_win_set_btn_width) },
    { be_const_key(title_get_alignment, -1), be_const_func(lvbe_win_title_get_alignment) },
    { be_const_key(scroll_hor, -1), be_const_func(lvbe_win_scroll_hor) },
    { be_const_key(add_btn_right, -1), be_const_func(lvbe_win_add_btn_right) },
    { be_const_key(get_anim_time, 13), be_const_func(lvbe_win_get_anim_time) },
    { be_const_key(init, -1), be_const_func(lvbe_win_create) },
    { be_const_key(get_from_btn, 15), be_const_func(lvbe_win_get_from_btn) },
    { be_const_key(add_btn_left, -1), be_const_func(lvbe_win_add_btn_left) },
    { be_const_key(title_set_alignment, -1), be_const_func(lvbe_win_title_set_alignment) },
    { be_const_key(scroll_ver, -1), be_const_func(lvbe_win_scroll_ver) },
    { be_const_key(set_header_height, -1), be_const_func(lvbe_win_set_header_height) },
    { be_const_key(focus, 19), be_const_func(lvbe_win_focus) },
    { be_const_key(set_anim_time, 5), be_const_func(lvbe_win_set_anim_time) },
    { be_const_key(close_event_cb, -1), be_const_func(lvbe_win_close_event_cb) },
    { be_const_key(create, -1), be_const_func(lvbe_win_create) },
    { be_const_key(get_sb_mode, 7), be_const_func(lvbe_win_get_sb_mode) },
    { be_const_key(get_btn_width, -1), be_const_func(lvbe_win_get_btn_width) },
    { be_const_key(get_width, -1), be_const_func(lvbe_win_get_width) },
    { be_const_key(get_layout, -1), be_const_func(lvbe_win_get_layout) },
    { be_const_key(set_drag, 2), be_const_func(lvbe_win_set_drag) },
    { be_const_key(tostring, 3), be_const_func(lvx_tostring) },
    { be_const_key(set_scrollbar_mode, -1), be_const_func(lvbe_win_set_scrollbar_mode) },
    { be_const_key(set_title, 24), be_const_func(lvbe_win_set_title) },
    { be_const_key(get_drag, 11), be_const_func(lvbe_win_get_drag) },
    { be_const_key(get_header_height, -1), be_const_func(lvbe_win_get_header_height) },
};

static be_define_const_map(
    be_class_lv_win_map,
    31
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_lv_win,
    1,
    (bclass *)&be_class_lv_obj,
    lv_win
);
