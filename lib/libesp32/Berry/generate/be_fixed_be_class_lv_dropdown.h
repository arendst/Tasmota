#include "be_constobj.h"

static be_define_const_map_slots(be_class_lv_dropdown_map) {
    { be_const_key(get_selected, -1), be_const_func(lvbe_dropdown_get_selected) },
    { be_const_key(get_show_selected, -1), be_const_func(lvbe_dropdown_get_show_selected) },
    { be_const_key(init, -1), be_const_func(lvbe_dropdown_create) },
    { be_const_key(set_symbol, 18), be_const_func(lvbe_dropdown_set_symbol) },
    { be_const_key(get_text, -1), be_const_func(lvbe_dropdown_get_text) },
    { be_const_key(set_text, 2), be_const_func(lvbe_dropdown_set_text) },
    { be_const_key(set_dir, 8), be_const_func(lvbe_dropdown_set_dir) },
    { be_const_key(get_dir, -1), be_const_func(lvbe_dropdown_get_dir) },
    { be_const_key(get_option_cnt, -1), be_const_func(lvbe_dropdown_get_option_cnt) },
    { be_const_key(create, -1), be_const_func(lvbe_dropdown_create) },
    { be_const_key(set_selected, -1), be_const_func(lvbe_dropdown_set_selected) },
    { be_const_key(dot_p, 24), be_const_int(0) },
    { be_const_key(open, 7), be_const_func(lvbe_dropdown_open) },
    { be_const_key(set_max_height, -1), be_const_func(lvbe_dropdown_set_max_height) },
    { be_const_key(add_option, -1), be_const_func(lvbe_dropdown_add_option) },
    { be_const_key(set_show_selected, 16), be_const_func(lvbe_dropdown_set_show_selected) },
    { be_const_key(tostring, -1), be_const_func(lvx_tostring) },
    { be_const_key(set_options_static, 9), be_const_func(lvbe_dropdown_set_options_static) },
    { be_const_key(clear_options, -1), be_const_func(lvbe_dropdown_clear_options) },
    { be_const_key(get_max_height, 11), be_const_func(lvbe_dropdown_get_max_height) },
    { be_const_key(set_options, 15), be_const_func(lvbe_dropdown_set_options) },
    { be_const_key(close, -1), be_const_func(lvbe_dropdown_close) },
    { be_const_key(get_selected_str, -1), be_const_func(lvbe_dropdown_get_selected_str) },
    { be_const_key(get_symbol, 13), be_const_func(lvbe_dropdown_get_symbol) },
    { be_const_key(get_options, -1), be_const_func(lvbe_dropdown_get_options) },
};

static be_define_const_map(
    be_class_lv_dropdown_map,
    25
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_lv_dropdown,
    1,
    (bclass *)&be_class_lv_obj,
    lv_dropdown
);
