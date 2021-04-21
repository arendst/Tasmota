#include "be_constobj.h"

static be_define_const_map_slots(be_class_lv_page_map) {
    { be_const_key(on_edge, -1), be_const_func(lvbe_page_on_edge) },
    { be_const_key(glue_obj, -1), be_const_func(lvbe_page_glue_obj) },
    { be_const_key(scroll_ver, -1), be_const_func(lvbe_page_scroll_ver) },
    { be_const_key(get_height_fit, -1), be_const_func(lvbe_page_get_height_fit) },
    { be_const_key(get_scrollbar_mode, 16), be_const_func(lvbe_page_get_scrollbar_mode) },
    { be_const_key(create, 29), be_const_func(lvbe_page_create) },
    { be_const_key(get_scrl_height, -1), be_const_func(lvbe_page_get_scrl_height) },
    { be_const_key(set_scrl_layout, -1), be_const_func(lvbe_page_set_scrl_layout) },
    { be_const_key(set_edge_flash, -1), be_const_func(lvbe_page_set_edge_flash) },
    { be_const_key(get_height_grid, -1), be_const_func(lvbe_page_get_height_grid) },
    { be_const_key(get_scrl_fit_bottom, 1), be_const_func(lvbe_page_get_scrl_fit_bottom) },
    { be_const_key(get_scrl_layout, 18), be_const_func(lvbe_page_get_scrl_layout) },
    { be_const_key(get_scrl_fit_left, 33), be_const_func(lvbe_page_get_scrl_fit_left) },
    { be_const_key(get_width_grid, -1), be_const_func(lvbe_page_get_width_grid) },
    { be_const_key(set_scrl_height, 6), be_const_func(lvbe_page_set_scrl_height) },
    { be_const_key(init, -1), be_const_func(lvbe_page_create) },
    { be_const_key(get_edge_flash, -1), be_const_func(lvbe_page_get_edge_flash) },
    { be_const_key(focus, -1), be_const_func(lvbe_page_focus) },
    { be_const_key(set_anim_time, -1), be_const_func(lvbe_page_set_anim_time) },
    { be_const_key(dot_p, -1), be_const_int(0) },
    { be_const_key(get_width_fit, 22), be_const_func(lvbe_page_get_width_fit) },
    { be_const_key(get_scrollable, 34), be_const_func(lvbe_page_get_scrollable) },
    { be_const_key(get_scroll_propagation, -1), be_const_func(lvbe_page_get_scroll_propagation) },
    { be_const_key(get_scrl_width, -1), be_const_func(lvbe_page_get_scrl_width) },
    { be_const_key(set_scrollbar_mode, 9), be_const_func(lvbe_page_set_scrollbar_mode) },
    { be_const_key(tostring, 27), be_const_func(lvx_tostring) },
    { be_const_key(clean, -1), be_const_func(lvbe_page_clean) },
    { be_const_key(set_scrollable_fit2, -1), be_const_func(lvbe_page_set_scrollable_fit2) },
    { be_const_key(set_scroll_propagation, 32), be_const_func(lvbe_page_set_scroll_propagation) },
    { be_const_key(set_scrollable_fit, -1), be_const_func(lvbe_page_set_scrollable_fit) },
    { be_const_key(scroll_hor, -1), be_const_func(lvbe_page_scroll_hor) },
    { be_const_key(get_anim_time, 17), be_const_func(lvbe_page_get_anim_time) },
    { be_const_key(start_edge_flash, -1), be_const_func(lvbe_page_start_edge_flash) },
    { be_const_key(get_scrl_fit_top, -1), be_const_func(lvbe_page_get_scrl_fit_top) },
    { be_const_key(get_scrl_fit_right, -1), be_const_func(lvbe_page_get_scrl_fit_right) },
    { be_const_key(set_scrl_width, -1), be_const_func(lvbe_page_set_scrl_width) },
};

static be_define_const_map(
    be_class_lv_page_map,
    36
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_lv_page,
    1,
    (bclass *)&be_class_lv_obj,
    lv_page
);
