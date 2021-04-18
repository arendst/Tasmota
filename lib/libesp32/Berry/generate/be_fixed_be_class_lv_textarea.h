#include "be_constobj.h"

static be_define_const_map_slots(be_class_lv_textarea_map) {
    { be_const_key(get_text, -1), be_const_func(lvbe_textarea_get_text) },
    { be_const_key(dot_p, -1), be_const_int(0) },
    { be_const_key(get_pwd_mode, 10), be_const_func(lvbe_textarea_get_pwd_mode) },
    { be_const_key(get_text_sel_en, 27), be_const_func(lvbe_textarea_get_text_sel_en) },
    { be_const_key(set_scrollbar_mode, 41), be_const_func(lvbe_textarea_set_scrollbar_mode) },
    { be_const_key(set_cursor_click_pos, -1), be_const_func(lvbe_textarea_set_cursor_click_pos) },
    { be_const_key(set_text, 9), be_const_func(lvbe_textarea_set_text) },
    { be_const_key(del_char, 39), be_const_func(lvbe_textarea_del_char) },
    { be_const_key(cursor_right, 42), be_const_func(lvbe_textarea_cursor_right) },
    { be_const_key(cursor_up, -1), be_const_func(lvbe_textarea_cursor_up) },
    { be_const_key(cursor_down, -1), be_const_func(lvbe_textarea_cursor_down) },
    { be_const_key(set_text_align, 45), be_const_func(lvbe_textarea_set_text_align) },
    { be_const_key(set_pwd_mode, -1), be_const_func(lvbe_textarea_set_pwd_mode) },
    { be_const_key(del_char_forward, -1), be_const_func(lvbe_textarea_del_char_forward) },
    { be_const_key(text_is_selected, 38), be_const_func(lvbe_textarea_text_is_selected) },
    { be_const_key(get_cursor_hidden, -1), be_const_func(lvbe_textarea_get_cursor_hidden) },
    { be_const_key(set_text_sel, -1), be_const_func(lvbe_textarea_set_text_sel) },
    { be_const_key(set_insert_replace, -1), be_const_func(lvbe_textarea_set_insert_replace) },
    { be_const_key(get_max_length, 34), be_const_func(lvbe_textarea_get_max_length) },
    { be_const_key(set_cursor_blink_time, 25), be_const_func(lvbe_textarea_set_cursor_blink_time) },
    { be_const_key(add_char, 19), be_const_func(lvbe_textarea_add_char) },
    { be_const_key(init, -1), be_const_func(lvbe_textarea_create) },
    { be_const_key(get_edge_flash, -1), be_const_func(lvbe_textarea_get_edge_flash) },
    { be_const_key(set_cursor_hidden, 0), be_const_func(lvbe_textarea_set_cursor_hidden) },
    { be_const_key(get_one_line, -1), be_const_func(lvbe_textarea_get_one_line) },
    { be_const_key(get_placeholder_text, -1), be_const_func(lvbe_textarea_get_placeholder_text) },
    { be_const_key(set_cursor_pos, -1), be_const_func(lvbe_textarea_set_cursor_pos) },
    { be_const_key(get_scrollbar_mode, -1), be_const_func(lvbe_textarea_get_scrollbar_mode) },
    { be_const_key(get_cursor_blink_time, 17), be_const_func(lvbe_textarea_get_cursor_blink_time) },
    { be_const_key(get_scroll_propagation, 28), be_const_func(lvbe_textarea_get_scroll_propagation) },
    { be_const_key(get_label, 18), be_const_func(lvbe_textarea_get_label) },
    { be_const_key(cursor_left, -1), be_const_func(lvbe_textarea_cursor_left) },
    { be_const_key(set_accepted_chars, -1), be_const_func(lvbe_textarea_set_accepted_chars) },
    { be_const_key(set_placeholder_text, 31), be_const_func(lvbe_textarea_set_placeholder_text) },
    { be_const_key(set_max_length, -1), be_const_func(lvbe_textarea_set_max_length) },
    { be_const_key(get_accepted_chars, 37), be_const_func(lvbe_textarea_get_accepted_chars) },
    { be_const_key(get_cursor_click_pos, -1), be_const_func(lvbe_textarea_get_cursor_click_pos) },
    { be_const_key(get_cursor_pos, -1), be_const_func(lvbe_textarea_get_cursor_pos) },
    { be_const_key(set_pwd_show_time, -1), be_const_func(lvbe_textarea_set_pwd_show_time) },
    { be_const_key(set_edge_flash, -1), be_const_func(lvbe_textarea_set_edge_flash) },
    { be_const_key(clear_selection, 22), be_const_func(lvbe_textarea_clear_selection) },
    { be_const_key(get_pwd_show_time, -1), be_const_func(lvbe_textarea_get_pwd_show_time) },
    { be_const_key(tostring, -1), be_const_func(lvx_tostring) },
    { be_const_key(set_one_line, -1), be_const_func(lvbe_textarea_set_one_line) },
    { be_const_key(add_text, 7), be_const_func(lvbe_textarea_add_text) },
    { be_const_key(set_scroll_propagation, -1), be_const_func(lvbe_textarea_set_scroll_propagation) },
    { be_const_key(create, -1), be_const_func(lvbe_textarea_create) },
};

static be_define_const_map(
    be_class_lv_textarea_map,
    47
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_lv_textarea,
    1,
    (bclass *)&be_class_lv_obj,
    lv_textarea
);
