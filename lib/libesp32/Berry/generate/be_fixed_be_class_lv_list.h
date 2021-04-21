#include "be_constobj.h"

static be_define_const_map_slots(be_class_lv_list_map) {
    { be_const_key(remove, -1), be_const_func(lvbe_list_remove) },
    { be_const_key(get_edge_flash, 16), be_const_func(lvbe_list_get_edge_flash) },
    { be_const_key(get_anim_time, -1), be_const_func(lvbe_list_get_anim_time) },
    { be_const_key(tostring, -1), be_const_func(lvx_tostring) },
    { be_const_key(get_btn_label, 24), be_const_func(lvbe_list_get_btn_label) },
    { be_const_key(up, -1), be_const_func(lvbe_list_up) },
    { be_const_key(get_scrollbar_mode, -1), be_const_func(lvbe_list_get_scrollbar_mode) },
    { be_const_key(get_prev_btn, 28), be_const_func(lvbe_list_get_prev_btn) },
    { be_const_key(get_scroll_propagation, 14), be_const_func(lvbe_list_get_scroll_propagation) },
    { be_const_key(focus, 13), be_const_func(lvbe_list_focus) },
    { be_const_key(get_layout, 4), be_const_func(lvbe_list_get_layout) },
    { be_const_key(down, -1), be_const_func(lvbe_list_down) },
    { be_const_key(get_btn_index, -1), be_const_func(lvbe_list_get_btn_index) },
    { be_const_key(clean, -1), be_const_func(lvbe_list_clean) },
    { be_const_key(create, -1), be_const_func(lvbe_list_create) },
    { be_const_key(get_next_btn, -1), be_const_func(lvbe_list_get_next_btn) },
    { be_const_key(get_btn_img, -1), be_const_func(lvbe_list_get_btn_img) },
    { be_const_key(get_btn_selected, -1), be_const_func(lvbe_list_get_btn_selected) },
    { be_const_key(set_edge_flash, -1), be_const_func(lvbe_list_set_edge_flash) },
    { be_const_key(get_btn_text, -1), be_const_func(lvbe_list_get_btn_text) },
    { be_const_key(set_layout, -1), be_const_func(lvbe_list_set_layout) },
    { be_const_key(dot_p, -1), be_const_int(0) },
    { be_const_key(set_scrollbar_mode, -1), be_const_func(lvbe_list_set_scrollbar_mode) },
    { be_const_key(focus_btn, -1), be_const_func(lvbe_list_focus_btn) },
    { be_const_key(init, -1), be_const_func(lvbe_list_create) },
    { be_const_key(set_scroll_propagation, 27), be_const_func(lvbe_list_set_scroll_propagation) },
    { be_const_key(get_size, 20), be_const_func(lvbe_list_get_size) },
    { be_const_key(set_anim_time, -1), be_const_func(lvbe_list_set_anim_time) },
    { be_const_key(add_btn, -1), be_const_func(lvbe_list_add_btn) },
};

static be_define_const_map(
    be_class_lv_list_map,
    29
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_lv_list,
    1,
    (bclass *)&be_class_lv_obj,
    lv_list
);
