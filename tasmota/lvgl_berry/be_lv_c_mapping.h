
/********************************************************************
 * Generated code, don't edit
  *******************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "be_ctypes.h"

/* `lv_style` methods */
const lvbe_call_c_t lv_style_func[] = {
  { "set_align", (void*) &lv_style_set_align, "", "(lv.lv_style)i" },
  { "set_anim_speed", (void*) &lv_style_set_anim_speed, "", "(lv.lv_style)i" },
  { "set_anim_time", (void*) &lv_style_set_anim_time, "", "(lv.lv_style)i" },
  { "set_arc_color", (void*) &lv_style_set_arc_color, "", "(lv.lv_style)(lv.lv_color)" },
  { "set_arc_color_filtered", (void*) &lv_style_set_arc_color_filtered, "", "(lv.lv_style)(lv.lv_color)" },
  { "set_arc_img_src", (void*) &lv_style_set_arc_img_src, "", "(lv.lv_style)." },
  { "set_arc_opa", (void*) &lv_style_set_arc_opa, "", "(lv.lv_style)i" },
  { "set_arc_rounded", (void*) &lv_style_set_arc_rounded, "", "(lv.lv_style)i" },
  { "set_arc_width", (void*) &lv_style_set_arc_width, "", "(lv.lv_style)i" },
  { "set_base_dir", (void*) &lv_style_set_base_dir, "", "(lv.lv_style)i" },
  { "set_bg_color", (void*) &lv_style_set_bg_color, "", "(lv.lv_style)(lv.lv_color)" },
  { "set_bg_color_filtered", (void*) &lv_style_set_bg_color_filtered, "", "(lv.lv_style)(lv.lv_color)" },
  { "set_bg_grad_color", (void*) &lv_style_set_bg_grad_color, "", "(lv.lv_style)(lv.lv_color)" },
  { "set_bg_grad_color_filtered", (void*) &lv_style_set_bg_grad_color_filtered, "", "(lv.lv_style)(lv.lv_color)" },
  { "set_bg_grad_dir", (void*) &lv_style_set_bg_grad_dir, "", "(lv.lv_style)i" },
  { "set_bg_grad_stop", (void*) &lv_style_set_bg_grad_stop, "", "(lv.lv_style)i" },
  { "set_bg_img_opa", (void*) &lv_style_set_bg_img_opa, "", "(lv.lv_style)i" },
  { "set_bg_img_recolor", (void*) &lv_style_set_bg_img_recolor, "", "(lv.lv_style)(lv.lv_color)" },
  { "set_bg_img_recolor_filtered", (void*) &lv_style_set_bg_img_recolor_filtered, "", "(lv.lv_style)(lv.lv_color)" },
  { "set_bg_img_recolor_opa", (void*) &lv_style_set_bg_img_recolor_opa, "", "(lv.lv_style)i" },
  { "set_bg_img_src", (void*) &lv_style_set_bg_img_src, "", "(lv.lv_style)." },
  { "set_bg_img_tiled", (void*) &lv_style_set_bg_img_tiled, "", "(lv.lv_style)b" },
  { "set_bg_main_stop", (void*) &lv_style_set_bg_main_stop, "", "(lv.lv_style)i" },
  { "set_bg_opa", (void*) &lv_style_set_bg_opa, "", "(lv.lv_style)i" },
  { "set_blend_mode", (void*) &lv_style_set_blend_mode, "", "(lv.lv_style)i" },
  { "set_border_color", (void*) &lv_style_set_border_color, "", "(lv.lv_style)(lv.lv_color)" },
  { "set_border_color_filtered", (void*) &lv_style_set_border_color_filtered, "", "(lv.lv_style)(lv.lv_color)" },
  { "set_border_opa", (void*) &lv_style_set_border_opa, "", "(lv.lv_style)i" },
  { "set_border_post", (void*) &lv_style_set_border_post, "", "(lv.lv_style)b" },
  { "set_border_side", (void*) &lv_style_set_border_side, "", "(lv.lv_style)i" },
  { "set_border_width", (void*) &lv_style_set_border_width, "", "(lv.lv_style)i" },
  { "set_clip_corner", (void*) &lv_style_set_clip_corner, "", "(lv.lv_style)b" },
  { "set_color_filter_dsc", (void*) &lv_style_set_color_filter_dsc, "", "(lv.lv_style)(lv.lv_color_filter_dsc)" },
  { "set_color_filter_opa", (void*) &lv_style_set_color_filter_opa, "", "(lv.lv_style)i" },
  { "set_height", (void*) &lv_style_set_height, "", "(lv.lv_style)i" },
  { "set_img_opa", (void*) &lv_style_set_img_opa, "", "(lv.lv_style)i" },
  { "set_img_recolor", (void*) &lv_style_set_img_recolor, "", "(lv.lv_style)(lv.lv_color)" },
  { "set_img_recolor_filtered", (void*) &lv_style_set_img_recolor_filtered, "", "(lv.lv_style)(lv.lv_color)" },
  { "set_img_recolor_opa", (void*) &lv_style_set_img_recolor_opa, "", "(lv.lv_style)i" },
  { "set_layout", (void*) &lv_style_set_layout, "", "(lv.lv_style)i" },
  { "set_line_color", (void*) &lv_style_set_line_color, "", "(lv.lv_style)(lv.lv_color)" },
  { "set_line_color_filtered", (void*) &lv_style_set_line_color_filtered, "", "(lv.lv_style)(lv.lv_color)" },
  { "set_line_dash_gap", (void*) &lv_style_set_line_dash_gap, "", "(lv.lv_style)i" },
  { "set_line_dash_width", (void*) &lv_style_set_line_dash_width, "", "(lv.lv_style)i" },
  { "set_line_opa", (void*) &lv_style_set_line_opa, "", "(lv.lv_style)i" },
  { "set_line_rounded", (void*) &lv_style_set_line_rounded, "", "(lv.lv_style)i" },
  { "set_line_width", (void*) &lv_style_set_line_width, "", "(lv.lv_style)i" },
  { "set_max_height", (void*) &lv_style_set_max_height, "", "(lv.lv_style)i" },
  { "set_max_width", (void*) &lv_style_set_max_width, "", "(lv.lv_style)i" },
  { "set_min_height", (void*) &lv_style_set_min_height, "", "(lv.lv_style)i" },
  { "set_min_width", (void*) &lv_style_set_min_width, "", "(lv.lv_style)i" },
  { "set_opa", (void*) &lv_style_set_opa, "", "(lv.lv_style)i" },
  { "set_outline_color", (void*) &lv_style_set_outline_color, "", "(lv.lv_style)(lv.lv_color)" },
  { "set_outline_color_filtered", (void*) &lv_style_set_outline_color_filtered, "", "(lv.lv_style)(lv.lv_color)" },
  { "set_outline_opa", (void*) &lv_style_set_outline_opa, "", "(lv.lv_style)i" },
  { "set_outline_pad", (void*) &lv_style_set_outline_pad, "", "(lv.lv_style)i" },
  { "set_outline_width", (void*) &lv_style_set_outline_width, "", "(lv.lv_style)i" },
  { "set_pad_bottom", (void*) &lv_style_set_pad_bottom, "", "(lv.lv_style)i" },
  { "set_pad_column", (void*) &lv_style_set_pad_column, "", "(lv.lv_style)i" },
  { "set_pad_left", (void*) &lv_style_set_pad_left, "", "(lv.lv_style)i" },
  { "set_pad_right", (void*) &lv_style_set_pad_right, "", "(lv.lv_style)i" },
  { "set_pad_row", (void*) &lv_style_set_pad_row, "", "(lv.lv_style)i" },
  { "set_pad_top", (void*) &lv_style_set_pad_top, "", "(lv.lv_style)i" },
  { "set_radius", (void*) &lv_style_set_radius, "", "(lv.lv_style)i" },
  { "set_shadow_color", (void*) &lv_style_set_shadow_color, "", "(lv.lv_style)(lv.lv_color)" },
  { "set_shadow_color_filtered", (void*) &lv_style_set_shadow_color_filtered, "", "(lv.lv_style)(lv.lv_color)" },
  { "set_shadow_ofs_x", (void*) &lv_style_set_shadow_ofs_x, "", "(lv.lv_style)i" },
  { "set_shadow_ofs_y", (void*) &lv_style_set_shadow_ofs_y, "", "(lv.lv_style)i" },
  { "set_shadow_opa", (void*) &lv_style_set_shadow_opa, "", "(lv.lv_style)i" },
  { "set_shadow_spread", (void*) &lv_style_set_shadow_spread, "", "(lv.lv_style)i" },
  { "set_shadow_width", (void*) &lv_style_set_shadow_width, "", "(lv.lv_style)i" },
  { "set_text_align", (void*) &lv_style_set_text_align, "", "(lv.lv_style)i" },
  { "set_text_color", (void*) &lv_style_set_text_color, "", "(lv.lv_style)(lv.lv_color)" },
  { "set_text_color_filtered", (void*) &lv_style_set_text_color_filtered, "", "(lv.lv_style)(lv.lv_color)" },
  { "set_text_decor", (void*) &lv_style_set_text_decor, "", "(lv.lv_style)i" },
  { "set_text_font", (void*) &lv_style_set_text_font, "", "(lv.lv_style)(lv.lv_font)" },
  { "set_text_letter_space", (void*) &lv_style_set_text_letter_space, "", "(lv.lv_style)i" },
  { "set_text_line_space", (void*) &lv_style_set_text_line_space, "", "(lv.lv_style)i" },
  { "set_text_opa", (void*) &lv_style_set_text_opa, "", "(lv.lv_style)i" },
  { "set_transform_angle", (void*) &lv_style_set_transform_angle, "", "(lv.lv_style)i" },
  { "set_transform_height", (void*) &lv_style_set_transform_height, "", "(lv.lv_style)i" },
  { "set_transform_width", (void*) &lv_style_set_transform_width, "", "(lv.lv_style)i" },
  { "set_transform_zoom", (void*) &lv_style_set_transform_zoom, "", "(lv.lv_style)i" },
  { "set_transition", (void*) &lv_style_set_transition, "", "(lv.lv_style)(lv.lv_style_transition_dsc)" },
  { "set_translate_x", (void*) &lv_style_set_translate_x, "", "(lv.lv_style)i" },
  { "set_translate_y", (void*) &lv_style_set_translate_y, "", "(lv.lv_style)i" },
  { "set_width", (void*) &lv_style_set_width, "", "(lv.lv_style)i" },
  { "set_x", (void*) &lv_style_set_x, "", "(lv.lv_style)i" },
  { "set_y", (void*) &lv_style_set_y, "", "(lv.lv_style)i" },
};

/* `lv_font` methods */
const lvbe_call_c_t lv_font_func[] = {
};

/* `lv_color` methods */
const lvbe_call_c_t lv_color_func[] = {
};

/* `lv_theme` methods */
const lvbe_call_c_t lv_theme_func[] = {
};

/* `lv_img` methods */
#ifdef BE_LV_WIDGET_IMG
const lvbe_call_c_t lv_img_func[] = {
  { "get_angle", (void*) &lv_img_get_angle, "i", "(lv.lv_obj)" },
  { "get_antialias", (void*) &lv_img_get_antialias, "b", "(lv.lv_obj)" },
  { "get_offset_x", (void*) &lv_img_get_offset_x, "i", "(lv.lv_obj)" },
  { "get_offset_y", (void*) &lv_img_get_offset_y, "i", "(lv.lv_obj)" },
  { "get_pivot", (void*) &lv_img_get_pivot, "", "(lv.lv_obj)(lv.lv_point)" },
  { "get_src", (void*) &lv_img_get_src, ".", "(lv.lv_obj)" },
  { "get_zoom", (void*) &lv_img_get_zoom, "i", "(lv.lv_obj)" },
  { "set_angle", (void*) &lv_img_set_angle, "", "(lv.lv_obj)i" },
  { "set_antialias", (void*) &lv_img_set_antialias, "", "(lv.lv_obj)b" },
  { "set_offset_x", (void*) &lv_img_set_offset_x, "", "(lv.lv_obj)i" },
  { "set_offset_y", (void*) &lv_img_set_offset_y, "", "(lv.lv_obj)i" },
  { "set_pivot", (void*) &lv_img_set_pivot, "", "(lv.lv_obj)ii" },
  { "set_src", (void*) &lv_img_set_src, "", "(lv.lv_obj)." },
  { "set_tasmota_logo", (void*) &lv_img_set_tasmota_logo, "", "(lv.lv_obj)" },
  { "set_zoom", (void*) &lv_img_set_zoom, "", "(lv.lv_obj)i" },
};
#endif // BE_LV_WIDGET_IMG

/* `lv_disp` methods */
const lvbe_call_c_t lv_disp_func[] = {
  { "clean_dcache", (void*) &lv_disp_clean_dcache, "", "(lv.lv_disp)" },
  { "dpx", (void*) &lv_disp_dpx, "i", "(lv.lv_disp)i" },
  { "get_inactive_time", (void*) &lv_disp_get_inactive_time, "i", "(lv.lv_disp)" },
  { "get_layer_sys", (void*) &lv_disp_get_layer_sys, "lv.lv_obj", "(lv.lv_disp)" },
  { "get_layer_top", (void*) &lv_disp_get_layer_top, "lv.lv_obj", "(lv.lv_disp)" },
  { "get_scr_act", (void*) &lv_disp_get_scr_act, "lv.lv_obj", "(lv.lv_disp)" },
  { "get_scr_prev", (void*) &lv_disp_get_scr_prev, "lv.lv_obj", "(lv.lv_disp)" },
  { "get_theme", (void*) &lv_disp_get_theme, "lv.lv_theme", "(lv.lv_disp)" },
  { "load_scr", (void*) &lv_disp_load_scr, "", "(lv.lv_obj)" },
  { "set_bg_color", (void*) &lv_disp_set_bg_color, "", "(lv.lv_disp)(lv.lv_color)" },
  { "set_bg_image", (void*) &lv_disp_set_bg_image, "", "(lv.lv_disp)." },
  { "set_bg_opa", (void*) &lv_disp_set_bg_opa, "", "(lv.lv_disp)i" },
  { "set_theme", (void*) &lv_disp_set_theme, "", "(lv.lv_disp)(lv.lv_theme)" },
  { "trig_activity", (void*) &lv_disp_trig_activity, "", "(lv.lv_disp)" },
};

/* `lv_obj` methods */
const lvbe_call_c_t lv_obj_func[] = {
  { "add_event_cb", (void*) &lv_obj_add_event_cb, "i", "(lv.lv_obj)^lv_event_cb^i." },
  { "add_flag", (void*) &lv_obj_add_flag, "", "(lv.lv_obj)i" },
  { "add_state", (void*) &lv_obj_add_state, "", "(lv.lv_obj)i" },
  { "add_style", (void*) &lv_obj_add_style, "", "(lv.lv_obj)(lv.lv_style)i" },
  { "align", (void*) &lv_obj_align, "", "(lv.lv_obj)iii" },
  { "align_to", (void*) &lv_obj_align_to, "", "(lv.lv_obj)(lv.lv_obj)iii" },
  { "allocate_spec_attr", (void*) &lv_obj_allocate_spec_attr, "", "(lv.lv_obj)" },
  { "area_is_visible", (void*) &lv_obj_area_is_visible, "b", "(lv.lv_obj)(lv.lv_area)" },
  { "calculate_ext_draw_size", (void*) &lv_obj_calculate_ext_draw_size, "i", "(lv.lv_obj)i" },
  { "center", (void*) &lv_obj_center, "", "(lv.lv_obj)" },
  { "check_type", (void*) &lv_obj_check_type, "b", "(lv.lv_obj)(lv.lv_obj_class)" },
  { "class_init_obj", (void*) &lv_obj_class_init_obj, "", "(lv.lv_obj)" },
  { "clean", (void*) &lv_obj_clean, "", "(lv.lv_obj)" },
  { "clear_flag", (void*) &lv_obj_clear_flag, "", "(lv.lv_obj)i" },
  { "clear_state", (void*) &lv_obj_clear_state, "", "(lv.lv_obj)i" },
  { "del", (void*) &lv_obj_del, "", "(lv.lv_obj)" },
  { "del_async", (void*) &lv_obj_del_async, "", "(lv.lv_obj)" },
  { "dpx", (void*) &lv_obj_dpx, "i", "(lv.lv_obj)i" },
  { "fade_in", (void*) &lv_obj_fade_in, "", "(lv.lv_obj)ii" },
  { "fade_out", (void*) &lv_obj_fade_out, "", "(lv.lv_obj)ii" },
  { "get_child", (void*) &lv_obj_get_child, "lv.lv_obj", "(lv.lv_obj)i" },
  { "get_child_cnt", (void*) &lv_obj_get_child_cnt, "i", "(lv.lv_obj)" },
  { "get_child_id", (void*) &lv_obj_get_child_id, "i", "(lv.lv_obj)" },
  { "get_class", (void*) &lv_obj_get_class, "lv.lv_obj_class", "(lv.lv_obj)" },
  { "get_click_area", (void*) &lv_obj_get_click_area, "", "(lv.lv_obj)(lv.lv_area)" },
  { "get_content_coords", (void*) &lv_obj_get_content_coords, "", "(lv.lv_obj)(lv.lv_area)" },
  { "get_content_height", (void*) &lv_obj_get_content_height, "i", "(lv.lv_obj)" },
  { "get_content_width", (void*) &lv_obj_get_content_width, "i", "(lv.lv_obj)" },
  { "get_coords", (void*) &lv_obj_get_coords, "", "(lv.lv_obj)(lv.lv_area)" },
  { "get_disp", (void*) &lv_obj_get_disp, "lv.lv_disp", "(lv.lv_obj)" },
  { "get_group", (void*) &lv_obj_get_group, ".", "(lv.lv_obj)" },
  { "get_height", (void*) &lv_obj_get_height, "i", "(lv.lv_obj)" },
  { "get_local_style_prop", (void*) &lv_obj_get_local_style_prop, "i", "(lv.lv_obj)(lv.lv_style_prop)(lv.lv_style_value)i" },
  { "get_parent", (void*) &lv_obj_get_parent, "lv.lv_obj", "(lv.lv_obj)" },
  { "get_screen", (void*) &lv_obj_get_screen, "lv.lv_obj", "(lv.lv_obj)" },
  { "get_scroll_bottom", (void*) &lv_obj_get_scroll_bottom, "i", "(lv.lv_obj)" },
  { "get_scroll_dir", (void*) &lv_obj_get_scroll_dir, "i", "(lv.lv_obj)" },
  { "get_scroll_end", (void*) &lv_obj_get_scroll_end, "", "(lv.lv_obj)(lv.lv_point)" },
  { "get_scroll_left", (void*) &lv_obj_get_scroll_left, "i", "(lv.lv_obj)" },
  { "get_scroll_right", (void*) &lv_obj_get_scroll_right, "i", "(lv.lv_obj)" },
  { "get_scroll_snap_x", (void*) &lv_obj_get_scroll_snap_x, "i", "(lv.lv_obj)" },
  { "get_scroll_snap_y", (void*) &lv_obj_get_scroll_snap_y, "i", "(lv.lv_obj)" },
  { "get_scroll_top", (void*) &lv_obj_get_scroll_top, "i", "(lv.lv_obj)" },
  { "get_scroll_x", (void*) &lv_obj_get_scroll_x, "i", "(lv.lv_obj)" },
  { "get_scroll_y", (void*) &lv_obj_get_scroll_y, "i", "(lv.lv_obj)" },
  { "get_scrollbar_area", (void*) &lv_obj_get_scrollbar_area, "", "(lv.lv_obj)(lv.lv_area)(lv.lv_area)" },
  { "get_scrollbar_mode", (void*) &lv_obj_get_scrollbar_mode, "i", "(lv.lv_obj)" },
  { "get_self_height", (void*) &lv_obj_get_self_height, "i", "(lv.lv_obj)" },
  { "get_self_width", (void*) &lv_obj_get_self_width, "i", "(lv.lv_obj)" },
  { "get_state", (void*) &lv_obj_get_state, "i", "(lv.lv_obj)" },
  { "get_style_align", (void*) &lv_obj_get_style_align, "i", "(lv.lv_obj)i" },
  { "get_style_anim_speed", (void*) &lv_obj_get_style_anim_speed, "i", "(lv.lv_obj)i" },
  { "get_style_anim_time", (void*) &lv_obj_get_style_anim_time, "i", "(lv.lv_obj)i" },
  { "get_style_arc_color", (void*) &lv_obj_get_style_arc_color, "lv.lv_color", "(lv.lv_obj)i" },
  { "get_style_arc_color_filtered", (void*) &lv_obj_get_style_arc_color_filtered, "lv.lv_color", "(lv.lv_obj)i" },
  { "get_style_arc_img_src", (void*) &lv_obj_get_style_arc_img_src, ".", "(lv.lv_obj)i" },
  { "get_style_arc_opa", (void*) &lv_obj_get_style_arc_opa, "i", "(lv.lv_obj)i" },
  { "get_style_arc_rounded", (void*) &lv_obj_get_style_arc_rounded, "i", "(lv.lv_obj)i" },
  { "get_style_arc_width", (void*) &lv_obj_get_style_arc_width, "i", "(lv.lv_obj)i" },
  { "get_style_base_dir", (void*) &lv_obj_get_style_base_dir, "i", "(lv.lv_obj)i" },
  { "get_style_bg_color", (void*) &lv_obj_get_style_bg_color, "lv.lv_color", "(lv.lv_obj)i" },
  { "get_style_bg_color_filtered", (void*) &lv_obj_get_style_bg_color_filtered, "lv.lv_color", "(lv.lv_obj)i" },
  { "get_style_bg_grad_color", (void*) &lv_obj_get_style_bg_grad_color, "lv.lv_color", "(lv.lv_obj)i" },
  { "get_style_bg_grad_color_filtered", (void*) &lv_obj_get_style_bg_grad_color_filtered, "lv.lv_color", "(lv.lv_obj)i" },
  { "get_style_bg_grad_dir", (void*) &lv_obj_get_style_bg_grad_dir, "i", "(lv.lv_obj)i" },
  { "get_style_bg_grad_stop", (void*) &lv_obj_get_style_bg_grad_stop, "i", "(lv.lv_obj)i" },
  { "get_style_bg_img_opa", (void*) &lv_obj_get_style_bg_img_opa, "i", "(lv.lv_obj)i" },
  { "get_style_bg_img_recolor", (void*) &lv_obj_get_style_bg_img_recolor, "lv.lv_color", "(lv.lv_obj)i" },
  { "get_style_bg_img_recolor_filtered", (void*) &lv_obj_get_style_bg_img_recolor_filtered, "lv.lv_color", "(lv.lv_obj)i" },
  { "get_style_bg_img_recolor_opa", (void*) &lv_obj_get_style_bg_img_recolor_opa, "i", "(lv.lv_obj)i" },
  { "get_style_bg_img_src", (void*) &lv_obj_get_style_bg_img_src, ".", "(lv.lv_obj)i" },
  { "get_style_bg_img_tiled", (void*) &lv_obj_get_style_bg_img_tiled, "b", "(lv.lv_obj)i" },
  { "get_style_bg_main_stop", (void*) &lv_obj_get_style_bg_main_stop, "i", "(lv.lv_obj)i" },
  { "get_style_bg_opa", (void*) &lv_obj_get_style_bg_opa, "i", "(lv.lv_obj)i" },
  { "get_style_blend_mode", (void*) &lv_obj_get_style_blend_mode, "i", "(lv.lv_obj)i" },
  { "get_style_border_color", (void*) &lv_obj_get_style_border_color, "lv.lv_color", "(lv.lv_obj)i" },
  { "get_style_border_color_filtered", (void*) &lv_obj_get_style_border_color_filtered, "lv.lv_color", "(lv.lv_obj)i" },
  { "get_style_border_opa", (void*) &lv_obj_get_style_border_opa, "i", "(lv.lv_obj)i" },
  { "get_style_border_post", (void*) &lv_obj_get_style_border_post, "b", "(lv.lv_obj)i" },
  { "get_style_border_side", (void*) &lv_obj_get_style_border_side, "i", "(lv.lv_obj)i" },
  { "get_style_border_width", (void*) &lv_obj_get_style_border_width, "i", "(lv.lv_obj)i" },
  { "get_style_clip_corner", (void*) &lv_obj_get_style_clip_corner, "b", "(lv.lv_obj)i" },
  { "get_style_color_filter_opa", (void*) &lv_obj_get_style_color_filter_opa, "i", "(lv.lv_obj)i" },
  { "get_style_height", (void*) &lv_obj_get_style_height, "i", "(lv.lv_obj)i" },
  { "get_style_img_opa", (void*) &lv_obj_get_style_img_opa, "i", "(lv.lv_obj)i" },
  { "get_style_img_recolor", (void*) &lv_obj_get_style_img_recolor, "lv.lv_color", "(lv.lv_obj)i" },
  { "get_style_img_recolor_filtered", (void*) &lv_obj_get_style_img_recolor_filtered, "lv.lv_color", "(lv.lv_obj)i" },
  { "get_style_img_recolor_opa", (void*) &lv_obj_get_style_img_recolor_opa, "i", "(lv.lv_obj)i" },
  { "get_style_layout", (void*) &lv_obj_get_style_layout, "i", "(lv.lv_obj)i" },
  { "get_style_line_color", (void*) &lv_obj_get_style_line_color, "lv.lv_color", "(lv.lv_obj)i" },
  { "get_style_line_color_filtered", (void*) &lv_obj_get_style_line_color_filtered, "lv.lv_color", "(lv.lv_obj)i" },
  { "get_style_line_dash_gap", (void*) &lv_obj_get_style_line_dash_gap, "i", "(lv.lv_obj)i" },
  { "get_style_line_dash_width", (void*) &lv_obj_get_style_line_dash_width, "i", "(lv.lv_obj)i" },
  { "get_style_line_opa", (void*) &lv_obj_get_style_line_opa, "i", "(lv.lv_obj)i" },
  { "get_style_line_rounded", (void*) &lv_obj_get_style_line_rounded, "i", "(lv.lv_obj)i" },
  { "get_style_line_width", (void*) &lv_obj_get_style_line_width, "i", "(lv.lv_obj)i" },
  { "get_style_max_height", (void*) &lv_obj_get_style_max_height, "i", "(lv.lv_obj)i" },
  { "get_style_max_width", (void*) &lv_obj_get_style_max_width, "i", "(lv.lv_obj)i" },
  { "get_style_min_height", (void*) &lv_obj_get_style_min_height, "i", "(lv.lv_obj)i" },
  { "get_style_min_width", (void*) &lv_obj_get_style_min_width, "i", "(lv.lv_obj)i" },
  { "get_style_opa", (void*) &lv_obj_get_style_opa, "i", "(lv.lv_obj)i" },
  { "get_style_outline_color", (void*) &lv_obj_get_style_outline_color, "lv.lv_color", "(lv.lv_obj)i" },
  { "get_style_outline_color_filtered", (void*) &lv_obj_get_style_outline_color_filtered, "lv.lv_color", "(lv.lv_obj)i" },
  { "get_style_outline_opa", (void*) &lv_obj_get_style_outline_opa, "i", "(lv.lv_obj)i" },
  { "get_style_outline_pad", (void*) &lv_obj_get_style_outline_pad, "i", "(lv.lv_obj)i" },
  { "get_style_outline_width", (void*) &lv_obj_get_style_outline_width, "i", "(lv.lv_obj)i" },
  { "get_style_pad_bottom", (void*) &lv_obj_get_style_pad_bottom, "i", "(lv.lv_obj)i" },
  { "get_style_pad_column", (void*) &lv_obj_get_style_pad_column, "i", "(lv.lv_obj)i" },
  { "get_style_pad_left", (void*) &lv_obj_get_style_pad_left, "i", "(lv.lv_obj)i" },
  { "get_style_pad_right", (void*) &lv_obj_get_style_pad_right, "i", "(lv.lv_obj)i" },
  { "get_style_pad_row", (void*) &lv_obj_get_style_pad_row, "i", "(lv.lv_obj)i" },
  { "get_style_pad_top", (void*) &lv_obj_get_style_pad_top, "i", "(lv.lv_obj)i" },
  { "get_style_prop", (void*) &lv_obj_get_style_prop, "i", "(lv.lv_obj)i(lv.lv_style_prop)" },
  { "get_style_radius", (void*) &lv_obj_get_style_radius, "i", "(lv.lv_obj)i" },
  { "get_style_shadow_color", (void*) &lv_obj_get_style_shadow_color, "lv.lv_color", "(lv.lv_obj)i" },
  { "get_style_shadow_color_filtered", (void*) &lv_obj_get_style_shadow_color_filtered, "lv.lv_color", "(lv.lv_obj)i" },
  { "get_style_shadow_ofs_x", (void*) &lv_obj_get_style_shadow_ofs_x, "i", "(lv.lv_obj)i" },
  { "get_style_shadow_ofs_y", (void*) &lv_obj_get_style_shadow_ofs_y, "i", "(lv.lv_obj)i" },
  { "get_style_shadow_opa", (void*) &lv_obj_get_style_shadow_opa, "i", "(lv.lv_obj)i" },
  { "get_style_shadow_spread", (void*) &lv_obj_get_style_shadow_spread, "i", "(lv.lv_obj)i" },
  { "get_style_shadow_width", (void*) &lv_obj_get_style_shadow_width, "i", "(lv.lv_obj)i" },
  { "get_style_text_align", (void*) &lv_obj_get_style_text_align, "i", "(lv.lv_obj)i" },
  { "get_style_text_color", (void*) &lv_obj_get_style_text_color, "lv.lv_color", "(lv.lv_obj)i" },
  { "get_style_text_color_filtered", (void*) &lv_obj_get_style_text_color_filtered, "lv.lv_color", "(lv.lv_obj)i" },
  { "get_style_text_decor", (void*) &lv_obj_get_style_text_decor, "i", "(lv.lv_obj)i" },
  { "get_style_text_font", (void*) &lv_obj_get_style_text_font, "lv.lv_font", "(lv.lv_obj)i" },
  { "get_style_text_letter_space", (void*) &lv_obj_get_style_text_letter_space, "i", "(lv.lv_obj)i" },
  { "get_style_text_line_space", (void*) &lv_obj_get_style_text_line_space, "i", "(lv.lv_obj)i" },
  { "get_style_text_opa", (void*) &lv_obj_get_style_text_opa, "i", "(lv.lv_obj)i" },
  { "get_style_transform_angle", (void*) &lv_obj_get_style_transform_angle, "i", "(lv.lv_obj)i" },
  { "get_style_transform_height", (void*) &lv_obj_get_style_transform_height, "i", "(lv.lv_obj)i" },
  { "get_style_transform_width", (void*) &lv_obj_get_style_transform_width, "i", "(lv.lv_obj)i" },
  { "get_style_transform_zoom", (void*) &lv_obj_get_style_transform_zoom, "i", "(lv.lv_obj)i" },
  { "get_style_translate_x", (void*) &lv_obj_get_style_translate_x, "i", "(lv.lv_obj)i" },
  { "get_style_translate_y", (void*) &lv_obj_get_style_translate_y, "i", "(lv.lv_obj)i" },
  { "get_style_width", (void*) &lv_obj_get_style_width, "i", "(lv.lv_obj)i" },
  { "get_style_x", (void*) &lv_obj_get_style_x, "i", "(lv.lv_obj)i" },
  { "get_style_y", (void*) &lv_obj_get_style_y, "i", "(lv.lv_obj)i" },
  { "get_user_data", (void*) &lv_obj_get_user_data, ".", "(lv.lv_obj)" },
  { "get_width", (void*) &lv_obj_get_width, "i", "(lv.lv_obj)" },
  { "get_x", (void*) &lv_obj_get_x, "i", "(lv.lv_obj)" },
  { "get_x2", (void*) &lv_obj_get_x2, "i", "(lv.lv_obj)" },
  { "get_y", (void*) &lv_obj_get_y, "i", "(lv.lv_obj)" },
  { "get_y2", (void*) &lv_obj_get_y2, "i", "(lv.lv_obj)" },
  { "has_class", (void*) &lv_obj_has_class, "b", "(lv.lv_obj)(lv.lv_obj_class)" },
  { "has_flag", (void*) &lv_obj_has_flag, "b", "(lv.lv_obj)i" },
  { "has_flag_any", (void*) &lv_obj_has_flag_any, "b", "(lv.lv_obj)i" },
  { "has_state", (void*) &lv_obj_has_state, "b", "(lv.lv_obj)i" },
  { "hit_test", (void*) &lv_obj_hit_test, "b", "(lv.lv_obj)(lv.lv_point)" },
  { "init_draw_arc_dsc", (void*) &lv_obj_init_draw_arc_dsc, "", "(lv.lv_obj)i(lv.lv_draw_arc_dsc)" },
  { "init_draw_img_dsc", (void*) &lv_obj_init_draw_img_dsc, "", "(lv.lv_obj)i(lv.lv_draw_img_dsc)" },
  { "init_draw_label_dsc", (void*) &lv_obj_init_draw_label_dsc, "", "(lv.lv_obj)i(lv.lv_draw_label_dsc)" },
  { "init_draw_line_dsc", (void*) &lv_obj_init_draw_line_dsc, "", "(lv.lv_obj)i(lv.lv_draw_line_dsc)" },
  { "init_draw_rect_dsc", (void*) &lv_obj_init_draw_rect_dsc, "", "(lv.lv_obj)i(lv.lv_draw_rect_dsc)" },
  { "invalidate", (void*) &lv_obj_invalidate, "", "(lv.lv_obj)" },
  { "invalidate_area", (void*) &lv_obj_invalidate_area, "", "(lv.lv_obj)(lv.lv_area)" },
  { "is_editable", (void*) &lv_obj_is_editable, "b", "(lv.lv_obj)" },
  { "is_group_def", (void*) &lv_obj_is_group_def, "b", "(lv.lv_obj)" },
  { "is_layout_positioned", (void*) &lv_obj_is_layout_positioned, "b", "(lv.lv_obj)" },
  { "is_scrolling", (void*) &lv_obj_is_scrolling, "b", "(lv.lv_obj)" },
  { "is_valid", (void*) &lv_obj_is_valid, "b", "(lv.lv_obj)" },
  { "is_visible", (void*) &lv_obj_is_visible, "b", "(lv.lv_obj)" },
  { "mark_layout_as_dirty", (void*) &lv_obj_mark_layout_as_dirty, "", "(lv.lv_obj)" },
  { "move_background", (void*) &lv_obj_move_background, "", "(lv.lv_obj)" },
  { "move_children_by", (void*) &lv_obj_move_children_by, "", "(lv.lv_obj)iib" },
  { "move_foreground", (void*) &lv_obj_move_foreground, "", "(lv.lv_obj)" },
  { "move_to", (void*) &lv_obj_move_to, "", "(lv.lv_obj)ii" },
  { "readjust_scroll", (void*) &lv_obj_readjust_scroll, "", "(lv.lv_obj)(lv.lv_anim_enable)" },
  { "refr_pos", (void*) &lv_obj_refr_pos, "", "(lv.lv_obj)" },
  { "refr_size", (void*) &lv_obj_refr_size, "b", "(lv.lv_obj)" },
  { "refresh_ext_draw_size", (void*) &lv_obj_refresh_ext_draw_size, "", "(lv.lv_obj)" },
  { "refresh_self_size", (void*) &lv_obj_refresh_self_size, "b", "(lv.lv_obj)" },
  { "refresh_style", (void*) &lv_obj_refresh_style, "", "(lv.lv_obj)i(lv.lv_style_prop)" },
  { "remove_event_cb", (void*) &lv_obj_remove_event_cb, "b", "(lv.lv_obj)^lv_event_cb^" },
  { "remove_event_dsc", (void*) &lv_obj_remove_event_dsc, "b", "(lv.lv_obj)i" },
  { "remove_local_style_prop", (void*) &lv_obj_remove_local_style_prop, "b", "(lv.lv_obj)(lv.lv_style_prop)i" },
  { "remove_style", (void*) &lv_obj_remove_style, "", "(lv.lv_obj)(lv.lv_style)i" },
  { "remove_style_all", (void*) &lv_obj_remove_style_all, "", "(lv.lv_obj)" },
  { "scroll_by", (void*) &lv_obj_scroll_by, "", "(lv.lv_obj)ii(lv.lv_anim_enable)" },
  { "scroll_to", (void*) &lv_obj_scroll_to, "", "(lv.lv_obj)ii(lv.lv_anim_enable)" },
  { "scroll_to_view", (void*) &lv_obj_scroll_to_view, "", "(lv.lv_obj)(lv.lv_anim_enable)" },
  { "scroll_to_view_recursive", (void*) &lv_obj_scroll_to_view_recursive, "", "(lv.lv_obj)(lv.lv_anim_enable)" },
  { "scroll_to_x", (void*) &lv_obj_scroll_to_x, "", "(lv.lv_obj)i(lv.lv_anim_enable)" },
  { "scroll_to_y", (void*) &lv_obj_scroll_to_y, "", "(lv.lv_obj)i(lv.lv_anim_enable)" },
  { "scrollbar_invalidate", (void*) &lv_obj_scrollbar_invalidate, "", "(lv.lv_obj)" },
  { "set_align", (void*) &lv_obj_set_align, "", "(lv.lv_obj)i" },
  { "set_content_height", (void*) &lv_obj_set_content_height, "", "(lv.lv_obj)i" },
  { "set_content_width", (void*) &lv_obj_set_content_width, "", "(lv.lv_obj)i" },
  { "set_ext_click_area", (void*) &lv_obj_set_ext_click_area, "", "(lv.lv_obj)i" },
  { "set_height", (void*) &lv_obj_set_height, "", "(lv.lv_obj)i" },
  { "set_layout", (void*) &lv_obj_set_layout, "", "(lv.lv_obj)i" },
  { "set_local_style_prop", (void*) &lv_obj_set_local_style_prop, "", "(lv.lv_obj)(lv.lv_style_prop)ii" },
  { "set_parent", (void*) &lv_obj_set_parent, "", "(lv.lv_obj)(lv.lv_obj)" },
  { "set_pos", (void*) &lv_obj_set_pos, "", "(lv.lv_obj)ii" },
  { "set_scroll_dir", (void*) &lv_obj_set_scroll_dir, "", "(lv.lv_obj)i" },
  { "set_scroll_snap_x", (void*) &lv_obj_set_scroll_snap_x, "", "(lv.lv_obj)i" },
  { "set_scroll_snap_y", (void*) &lv_obj_set_scroll_snap_y, "", "(lv.lv_obj)i" },
  { "set_scrollbar_mode", (void*) &lv_obj_set_scrollbar_mode, "", "(lv.lv_obj)i" },
  { "set_size", (void*) &lv_obj_set_size, "", "(lv.lv_obj)ii" },
  { "set_style_align", (void*) &lv_obj_set_style_align, "", "(lv.lv_obj)ii" },
  { "set_style_anim_speed", (void*) &lv_obj_set_style_anim_speed, "", "(lv.lv_obj)ii" },
  { "set_style_anim_time", (void*) &lv_obj_set_style_anim_time, "", "(lv.lv_obj)ii" },
  { "set_style_arc_color", (void*) &lv_obj_set_style_arc_color, "", "(lv.lv_obj)(lv.lv_color)i" },
  { "set_style_arc_color_filtered", (void*) &lv_obj_set_style_arc_color_filtered, "", "(lv.lv_obj)(lv.lv_color)i" },
  { "set_style_arc_img_src", (void*) &lv_obj_set_style_arc_img_src, "", "(lv.lv_obj).i" },
  { "set_style_arc_opa", (void*) &lv_obj_set_style_arc_opa, "", "(lv.lv_obj)ii" },
  { "set_style_arc_rounded", (void*) &lv_obj_set_style_arc_rounded, "", "(lv.lv_obj)ii" },
  { "set_style_arc_width", (void*) &lv_obj_set_style_arc_width, "", "(lv.lv_obj)ii" },
  { "set_style_base_dir", (void*) &lv_obj_set_style_base_dir, "", "(lv.lv_obj)ii" },
  { "set_style_bg_color", (void*) &lv_obj_set_style_bg_color, "", "(lv.lv_obj)(lv.lv_color)i" },
  { "set_style_bg_color_filtered", (void*) &lv_obj_set_style_bg_color_filtered, "", "(lv.lv_obj)(lv.lv_color)i" },
  { "set_style_bg_grad_color", (void*) &lv_obj_set_style_bg_grad_color, "", "(lv.lv_obj)(lv.lv_color)i" },
  { "set_style_bg_grad_color_filtered", (void*) &lv_obj_set_style_bg_grad_color_filtered, "", "(lv.lv_obj)(lv.lv_color)i" },
  { "set_style_bg_grad_dir", (void*) &lv_obj_set_style_bg_grad_dir, "", "(lv.lv_obj)ii" },
  { "set_style_bg_grad_stop", (void*) &lv_obj_set_style_bg_grad_stop, "", "(lv.lv_obj)ii" },
  { "set_style_bg_img_opa", (void*) &lv_obj_set_style_bg_img_opa, "", "(lv.lv_obj)ii" },
  { "set_style_bg_img_recolor", (void*) &lv_obj_set_style_bg_img_recolor, "", "(lv.lv_obj)(lv.lv_color)i" },
  { "set_style_bg_img_recolor_filtered", (void*) &lv_obj_set_style_bg_img_recolor_filtered, "", "(lv.lv_obj)(lv.lv_color)i" },
  { "set_style_bg_img_recolor_opa", (void*) &lv_obj_set_style_bg_img_recolor_opa, "", "(lv.lv_obj)ii" },
  { "set_style_bg_img_src", (void*) &lv_obj_set_style_bg_img_src, "", "(lv.lv_obj).i" },
  { "set_style_bg_img_tiled", (void*) &lv_obj_set_style_bg_img_tiled, "", "(lv.lv_obj)bi" },
  { "set_style_bg_main_stop", (void*) &lv_obj_set_style_bg_main_stop, "", "(lv.lv_obj)ii" },
  { "set_style_bg_opa", (void*) &lv_obj_set_style_bg_opa, "", "(lv.lv_obj)ii" },
  { "set_style_blend_mode", (void*) &lv_obj_set_style_blend_mode, "", "(lv.lv_obj)ii" },
  { "set_style_border_color", (void*) &lv_obj_set_style_border_color, "", "(lv.lv_obj)(lv.lv_color)i" },
  { "set_style_border_color_filtered", (void*) &lv_obj_set_style_border_color_filtered, "", "(lv.lv_obj)(lv.lv_color)i" },
  { "set_style_border_opa", (void*) &lv_obj_set_style_border_opa, "", "(lv.lv_obj)ii" },
  { "set_style_border_post", (void*) &lv_obj_set_style_border_post, "", "(lv.lv_obj)bi" },
  { "set_style_border_side", (void*) &lv_obj_set_style_border_side, "", "(lv.lv_obj)ii" },
  { "set_style_border_width", (void*) &lv_obj_set_style_border_width, "", "(lv.lv_obj)ii" },
  { "set_style_clip_corner", (void*) &lv_obj_set_style_clip_corner, "", "(lv.lv_obj)bi" },
  { "set_style_color_filter_dsc", (void*) &lv_obj_set_style_color_filter_dsc, "", "(lv.lv_obj)(lv.lv_color_filter_dsc)i" },
  { "set_style_color_filter_opa", (void*) &lv_obj_set_style_color_filter_opa, "", "(lv.lv_obj)ii" },
  { "set_style_height", (void*) &lv_obj_set_style_height, "", "(lv.lv_obj)ii" },
  { "set_style_img_opa", (void*) &lv_obj_set_style_img_opa, "", "(lv.lv_obj)ii" },
  { "set_style_img_recolor", (void*) &lv_obj_set_style_img_recolor, "", "(lv.lv_obj)(lv.lv_color)i" },
  { "set_style_img_recolor_filtered", (void*) &lv_obj_set_style_img_recolor_filtered, "", "(lv.lv_obj)(lv.lv_color)i" },
  { "set_style_img_recolor_opa", (void*) &lv_obj_set_style_img_recolor_opa, "", "(lv.lv_obj)ii" },
  { "set_style_layout", (void*) &lv_obj_set_style_layout, "", "(lv.lv_obj)ii" },
  { "set_style_line_color", (void*) &lv_obj_set_style_line_color, "", "(lv.lv_obj)(lv.lv_color)i" },
  { "set_style_line_color_filtered", (void*) &lv_obj_set_style_line_color_filtered, "", "(lv.lv_obj)(lv.lv_color)i" },
  { "set_style_line_dash_gap", (void*) &lv_obj_set_style_line_dash_gap, "", "(lv.lv_obj)ii" },
  { "set_style_line_dash_width", (void*) &lv_obj_set_style_line_dash_width, "", "(lv.lv_obj)ii" },
  { "set_style_line_opa", (void*) &lv_obj_set_style_line_opa, "", "(lv.lv_obj)ii" },
  { "set_style_line_rounded", (void*) &lv_obj_set_style_line_rounded, "", "(lv.lv_obj)ii" },
  { "set_style_line_width", (void*) &lv_obj_set_style_line_width, "", "(lv.lv_obj)ii" },
  { "set_style_max_height", (void*) &lv_obj_set_style_max_height, "", "(lv.lv_obj)ii" },
  { "set_style_max_width", (void*) &lv_obj_set_style_max_width, "", "(lv.lv_obj)ii" },
  { "set_style_min_height", (void*) &lv_obj_set_style_min_height, "", "(lv.lv_obj)ii" },
  { "set_style_min_width", (void*) &lv_obj_set_style_min_width, "", "(lv.lv_obj)ii" },
  { "set_style_opa", (void*) &lv_obj_set_style_opa, "", "(lv.lv_obj)ii" },
  { "set_style_outline_color", (void*) &lv_obj_set_style_outline_color, "", "(lv.lv_obj)(lv.lv_color)i" },
  { "set_style_outline_color_filtered", (void*) &lv_obj_set_style_outline_color_filtered, "", "(lv.lv_obj)(lv.lv_color)i" },
  { "set_style_outline_opa", (void*) &lv_obj_set_style_outline_opa, "", "(lv.lv_obj)ii" },
  { "set_style_outline_pad", (void*) &lv_obj_set_style_outline_pad, "", "(lv.lv_obj)ii" },
  { "set_style_outline_width", (void*) &lv_obj_set_style_outline_width, "", "(lv.lv_obj)ii" },
  { "set_style_pad_all", (void*) &lv_obj_set_style_pad_all, "", "(lv.lv_obj)ii" },
  { "set_style_pad_bottom", (void*) &lv_obj_set_style_pad_bottom, "", "(lv.lv_obj)ii" },
  { "set_style_pad_column", (void*) &lv_obj_set_style_pad_column, "", "(lv.lv_obj)ii" },
  { "set_style_pad_gap", (void*) &lv_obj_set_style_pad_gap, "", "(lv.lv_obj)ii" },
  { "set_style_pad_hor", (void*) &lv_obj_set_style_pad_hor, "", "(lv.lv_obj)ii" },
  { "set_style_pad_left", (void*) &lv_obj_set_style_pad_left, "", "(lv.lv_obj)ii" },
  { "set_style_pad_right", (void*) &lv_obj_set_style_pad_right, "", "(lv.lv_obj)ii" },
  { "set_style_pad_row", (void*) &lv_obj_set_style_pad_row, "", "(lv.lv_obj)ii" },
  { "set_style_pad_top", (void*) &lv_obj_set_style_pad_top, "", "(lv.lv_obj)ii" },
  { "set_style_pad_ver", (void*) &lv_obj_set_style_pad_ver, "", "(lv.lv_obj)ii" },
  { "set_style_radius", (void*) &lv_obj_set_style_radius, "", "(lv.lv_obj)ii" },
  { "set_style_shadow_color", (void*) &lv_obj_set_style_shadow_color, "", "(lv.lv_obj)(lv.lv_color)i" },
  { "set_style_shadow_color_filtered", (void*) &lv_obj_set_style_shadow_color_filtered, "", "(lv.lv_obj)(lv.lv_color)i" },
  { "set_style_shadow_ofs_x", (void*) &lv_obj_set_style_shadow_ofs_x, "", "(lv.lv_obj)ii" },
  { "set_style_shadow_ofs_y", (void*) &lv_obj_set_style_shadow_ofs_y, "", "(lv.lv_obj)ii" },
  { "set_style_shadow_opa", (void*) &lv_obj_set_style_shadow_opa, "", "(lv.lv_obj)ii" },
  { "set_style_shadow_spread", (void*) &lv_obj_set_style_shadow_spread, "", "(lv.lv_obj)ii" },
  { "set_style_shadow_width", (void*) &lv_obj_set_style_shadow_width, "", "(lv.lv_obj)ii" },
  { "set_style_size", (void*) &lv_obj_set_style_size, "", "(lv.lv_obj)ii" },
  { "set_style_text_align", (void*) &lv_obj_set_style_text_align, "", "(lv.lv_obj)ii" },
  { "set_style_text_color", (void*) &lv_obj_set_style_text_color, "", "(lv.lv_obj)(lv.lv_color)i" },
  { "set_style_text_color_filtered", (void*) &lv_obj_set_style_text_color_filtered, "", "(lv.lv_obj)(lv.lv_color)i" },
  { "set_style_text_decor", (void*) &lv_obj_set_style_text_decor, "", "(lv.lv_obj)ii" },
  { "set_style_text_font", (void*) &lv_obj_set_style_text_font, "", "(lv.lv_obj)(lv.lv_font)i" },
  { "set_style_text_letter_space", (void*) &lv_obj_set_style_text_letter_space, "", "(lv.lv_obj)ii" },
  { "set_style_text_line_space", (void*) &lv_obj_set_style_text_line_space, "", "(lv.lv_obj)ii" },
  { "set_style_text_opa", (void*) &lv_obj_set_style_text_opa, "", "(lv.lv_obj)ii" },
  { "set_style_transform_angle", (void*) &lv_obj_set_style_transform_angle, "", "(lv.lv_obj)ii" },
  { "set_style_transform_height", (void*) &lv_obj_set_style_transform_height, "", "(lv.lv_obj)ii" },
  { "set_style_transform_width", (void*) &lv_obj_set_style_transform_width, "", "(lv.lv_obj)ii" },
  { "set_style_transform_zoom", (void*) &lv_obj_set_style_transform_zoom, "", "(lv.lv_obj)ii" },
  { "set_style_transition", (void*) &lv_obj_set_style_transition, "", "(lv.lv_obj)(lv.lv_style_transition_dsc)i" },
  { "set_style_translate_x", (void*) &lv_obj_set_style_translate_x, "", "(lv.lv_obj)ii" },
  { "set_style_translate_y", (void*) &lv_obj_set_style_translate_y, "", "(lv.lv_obj)ii" },
  { "set_style_width", (void*) &lv_obj_set_style_width, "", "(lv.lv_obj)ii" },
  { "set_style_x", (void*) &lv_obj_set_style_x, "", "(lv.lv_obj)ii" },
  { "set_style_y", (void*) &lv_obj_set_style_y, "", "(lv.lv_obj)ii" },
  { "set_user_data", (void*) &lv_obj_set_user_data, "", "(lv.lv_obj)." },
  { "set_width", (void*) &lv_obj_set_width, "", "(lv.lv_obj)i" },
  { "set_x", (void*) &lv_obj_set_x, "", "(lv.lv_obj)i" },
  { "set_y", (void*) &lv_obj_set_y, "", "(lv.lv_obj)i" },
  { "tree_walk", (void*) &lv_obj_tree_walk, "", "(lv.lv_obj)^lv_obj_tree_walk_cb^." },
  { "update_layout", (void*) &lv_obj_update_layout, "", "(lv.lv_obj)" },
  { "update_snap", (void*) &lv_obj_update_snap, "", "(lv.lv_obj)(lv.lv_anim_enable)" },
};

/* `lv_group` methods */
const lvbe_call_c_t lv_group_func[] = {
  { "add_obj", (void*) &lv_group_add_obj, "", "(lv.lv_group)(lv.lv_obj)" },
  { "del", (void*) &lv_group_del, "", "(lv.lv_group)" },
  { "focus_freeze", (void*) &lv_group_focus_freeze, "", "(lv.lv_group)b" },
  { "focus_next", (void*) &lv_group_focus_next, "", "(lv.lv_group)" },
  { "focus_obj", (void*) &lv_group_focus_obj, "", "(lv.lv_obj)" },
  { "focus_prev", (void*) &lv_group_focus_prev, "", "(lv.lv_group)" },
  { "get_editing", (void*) &lv_group_get_editing, "b", "(lv.lv_group)" },
  { "get_focus_cb", (void*) &lv_group_get_focus_cb, "lv.lv_group_focus_cb", "(lv.lv_group)" },
  { "get_focused", (void*) &lv_group_get_focused, "lv.lv_obj", "(lv.lv_group)" },
  { "get_obj_count", (void*) &lv_group_get_obj_count, "i", "(lv.lv_group)" },
  { "get_wrap", (void*) &lv_group_get_wrap, "b", "(lv.lv_group)" },
  { "remove_all_objs", (void*) &lv_group_remove_all_objs, "", "(lv.lv_group)" },
  { "remove_obj", (void*) &lv_group_remove_obj, "", "(lv.lv_obj)" },
  { "send_data", (void*) &lv_group_send_data, "i", "(lv.lv_group)i" },
  { "set_default", (void*) &lv_group_set_default, "", "(lv.lv_group)" },
  { "set_editing", (void*) &lv_group_set_editing, "", "(lv.lv_group)b" },
  { "set_focus_cb", (void*) &lv_group_set_focus_cb, "", "(lv.lv_group)^lv_group_focus_cb^" },
  { "set_refocus_policy", (void*) &lv_group_set_refocus_policy, "", "(lv.lv_group)(lv.lv_group_refocus_policy)" },
  { "set_wrap", (void*) &lv_group_set_wrap, "", "(lv.lv_group)b" },
};

/* `lv_indev` methods */
const lvbe_call_c_t lv_indev_func[] = {
  { "enable", (void*) &lv_indev_enable, "", "(lv.lv_indev)b" },
  { "get_gesture_dir", (void*) &lv_indev_get_gesture_dir, "i", "(lv.lv_indev)" },
  { "get_key", (void*) &lv_indev_get_key, "i", "(lv.lv_indev)" },
  { "get_point", (void*) &lv_indev_get_point, "", "(lv.lv_indev)(lv.lv_point)" },
  { "get_scroll_dir", (void*) &lv_indev_get_scroll_dir, "i", "(lv.lv_indev)" },
  { "get_scroll_obj", (void*) &lv_indev_get_scroll_obj, "lv.lv_obj", "(lv.lv_indev)" },
  { "get_type", (void*) &lv_indev_get_type, "i", "(lv.lv_indev)" },
  { "get_vect", (void*) &lv_indev_get_vect, "", "(lv.lv_indev)(lv.lv_point)" },
  { "reset", (void*) &lv_indev_reset, "", "(lv.lv_indev)(lv.lv_obj)" },
  { "reset_long_press", (void*) &lv_indev_reset_long_press, "", "(lv.lv_indev)" },
  { "search_obj", (void*) &lv_indev_search_obj, "lv.lv_obj", "(lv.lv_obj)(lv.lv_point)" },
  { "set_button_points", (void*) &lv_indev_set_button_points, "", "(lv.lv_indev)i" },
  { "set_cursor", (void*) &lv_indev_set_cursor, "", "(lv.lv_indev)(lv.lv_obj)" },
  { "set_group", (void*) &lv_indev_set_group, "", "(lv.lv_indev)(lv.lv_group)" },
  { "wait_release", (void*) &lv_indev_wait_release, "", "(lv.lv_indev)" },
};

/* `lv_chart` methods */
#ifdef BE_LV_WIDGET_CHART
const lvbe_call_c_t lv_chart_func[] = {
  { "get_cursor_point", (void*) &lv_chart_get_cursor_point, "i", "(lv.lv_obj)(lv.lv_chart_cursor)" },
  { "get_point_count", (void*) &lv_chart_get_point_count, "i", "(lv.lv_obj)" },
  { "get_point_pos_by_id", (void*) &lv_chart_get_point_pos_by_id, "", "(lv.lv_obj)(lv.lv_chart_series)i(lv.lv_point)" },
  { "get_pressed_point", (void*) &lv_chart_get_pressed_point, "i", "(lv.lv_obj)" },
  { "get_type", (void*) &lv_chart_get_type, "i", "(lv.lv_obj)" },
  { "get_x_start_point", (void*) &lv_chart_get_x_start_point, "i", "(lv.lv_obj)(lv.lv_chart_series)" },
  { "get_zoom_x", (void*) &lv_chart_get_zoom_x, "i", "(lv.lv_obj)" },
  { "get_zoom_y", (void*) &lv_chart_get_zoom_y, "i", "(lv.lv_obj)" },
  { "hide_series", (void*) &lv_chart_hide_series, "", "(lv.lv_obj)(lv.lv_chart_series)b" },
  { "refresh", (void*) &lv_chart_refresh, "", "(lv.lv_obj)" },
  { "remove_series", (void*) &lv_chart_remove_series, "", "(lv.lv_obj)(lv.lv_chart_series)" },
  { "set_all_value", (void*) &lv_chart_set_all_value, "", "(lv.lv_obj)(lv.lv_chart_series)i" },
  { "set_axis_tick", (void*) &lv_chart_set_axis_tick, "", "(lv.lv_obj)iiiiibi" },
  { "set_cursor_point", (void*) &lv_chart_set_cursor_point, "", "(lv.lv_obj)(lv.lv_chart_cursor)(lv.lv_chart_series)i" },
  { "set_cursor_pos", (void*) &lv_chart_set_cursor_pos, "", "(lv.lv_obj)(lv.lv_chart_cursor)(lv.lv_point)" },
  { "set_div_line_count", (void*) &lv_chart_set_div_line_count, "", "(lv.lv_obj)ii" },
  { "set_ext_x_array", (void*) &lv_chart_set_ext_x_array, "", "(lv.lv_obj)(lv.lv_chart_series)i" },
  { "set_ext_y_array", (void*) &lv_chart_set_ext_y_array, "", "(lv.lv_obj)(lv.lv_chart_series)i" },
  { "set_next_value", (void*) &lv_chart_set_next_value, "", "(lv.lv_obj)(lv.lv_chart_series)i" },
  { "set_next_value2", (void*) &lv_chart_set_next_value2, "", "(lv.lv_obj)(lv.lv_chart_series)ii" },
  { "set_point_count", (void*) &lv_chart_set_point_count, "", "(lv.lv_obj)i" },
  { "set_range", (void*) &lv_chart_set_range, "", "(lv.lv_obj)iii" },
  { "set_series_color", (void*) &lv_chart_set_series_color, "", "(lv.lv_obj)(lv.lv_chart_series)(lv.lv_color)" },
  { "set_type", (void*) &lv_chart_set_type, "", "(lv.lv_obj)i" },
  { "set_update_mode", (void*) &lv_chart_set_update_mode, "", "(lv.lv_obj)(lv.lv_chart_update_mode)" },
  { "set_value_by_id", (void*) &lv_chart_set_value_by_id, "", "(lv.lv_obj)(lv.lv_chart_series)ii" },
  { "set_value_by_id2", (void*) &lv_chart_set_value_by_id2, "", "(lv.lv_obj)(lv.lv_chart_series)iii" },
  { "set_x_start_point", (void*) &lv_chart_set_x_start_point, "", "(lv.lv_obj)(lv.lv_chart_series)i" },
  { "set_zoom_x", (void*) &lv_chart_set_zoom_x, "", "(lv.lv_obj)i" },
  { "set_zoom_y", (void*) &lv_chart_set_zoom_y, "", "(lv.lv_obj)i" },
};
#endif // BE_LV_WIDGET_CHART

/* `lv_colorwheel` methods */
#ifdef BE_LV_WIDGET_COLORWHEEL
const lvbe_call_c_t lv_colorwheel_func[] = {
  { "get_color_mode", (void*) &lv_colorwheel_get_color_mode, "i", "(lv.lv_obj)" },
  { "get_color_mode_fixed", (void*) &lv_colorwheel_get_color_mode_fixed, "b", "(lv.lv_obj)" },
  { "get_hsv", (void*) &lv_colorwheel_get_hsv, "i", "(lv.lv_obj)" },
  { "get_rgb", (void*) &lv_colorwheel_get_rgb, "lv.lv_color", "(lv.lv_obj)" },
  { "set_hsv", (void*) &lv_colorwheel_set_hsv, "b", "(lv.lv_obj)i" },
  { "set_mode", (void*) &lv_colorwheel_set_mode, "", "(lv.lv_obj)i" },
  { "set_mode_fixed", (void*) &lv_colorwheel_set_mode_fixed, "", "(lv.lv_obj)b" },
  { "set_rgb", (void*) &lv_colorwheel_set_rgb, "b", "(lv.lv_obj)(lv.lv_color)" },
};
#endif // BE_LV_WIDGET_COLORWHEEL

/* `lv_imgbtn` methods */
#ifdef BE_LV_WIDGET_IMGBTN
const lvbe_call_c_t lv_imgbtn_func[] = {
  { "set_src", (void*) &lv_imgbtn_set_src, "", "(lv.lv_obj)(lv.lv_imgbtn_state)..." },
};
#endif // BE_LV_WIDGET_IMGBTN

/* `lv_led` methods */
#ifdef BE_LV_WIDGET_LED
const lvbe_call_c_t lv_led_func[] = {
  { "get_brightness", (void*) &lv_led_get_brightness, "i", "(lv.lv_obj)" },
  { "off", (void*) &lv_led_off, "", "(lv.lv_obj)" },
  { "on", (void*) &lv_led_on, "", "(lv.lv_obj)" },
  { "set_brightness", (void*) &lv_led_set_brightness, "", "(lv.lv_obj)i" },
  { "set_color", (void*) &lv_led_set_color, "", "(lv.lv_obj)(lv.lv_color)" },
  { "toggle", (void*) &lv_led_toggle, "", "(lv.lv_obj)" },
};
#endif // BE_LV_WIDGET_LED

/* `lv_meter` methods */
#ifdef BE_LV_WIDGET_METER
const lvbe_call_c_t lv_meter_func[] = {
  { "add_arc", (void*) &lv_meter_add_arc, "lv.lv_meter_indicator", "(lv.lv_obj)(lv.lv_meter_scale)i(lv.lv_color)i" },
  { "add_needle_img", (void*) &lv_meter_add_needle_img, "lv.lv_meter_indicator", "(lv.lv_obj)(lv.lv_meter_scale).ii" },
  { "add_needle_line", (void*) &lv_meter_add_needle_line, "lv.lv_meter_indicator", "(lv.lv_obj)(lv.lv_meter_scale)i(lv.lv_color)i" },
  { "add_scale", (void*) &lv_meter_add_scale, "lv.lv_meter_scale", "(lv.lv_obj)" },
  { "add_scale_lines", (void*) &lv_meter_add_scale_lines, "lv.lv_meter_indicator", "(lv.lv_obj)(lv.lv_meter_scale)(lv.lv_color)(lv.lv_color)bi" },
  { "set_indicator_end_value", (void*) &lv_meter_set_indicator_end_value, "", "(lv.lv_obj)(lv.lv_meter_indicator)i" },
  { "set_indicator_start_value", (void*) &lv_meter_set_indicator_start_value, "", "(lv.lv_obj)(lv.lv_meter_indicator)i" },
  { "set_indicator_value", (void*) &lv_meter_set_indicator_value, "", "(lv.lv_obj)(lv.lv_meter_indicator)i" },
  { "set_scale_major_ticks", (void*) &lv_meter_set_scale_major_ticks, "", "(lv.lv_obj)(lv.lv_meter_scale)iii(lv.lv_color)i" },
  { "set_scale_range", (void*) &lv_meter_set_scale_range, "", "(lv.lv_obj)(lv.lv_meter_scale)iiii" },
  { "set_scale_ticks", (void*) &lv_meter_set_scale_ticks, "", "(lv.lv_obj)(lv.lv_meter_scale)iii(lv.lv_color)" },
};
#endif // BE_LV_WIDGET_METER

/* `lv_msgbox` methods */
#ifdef BE_LV_WIDGET_MSGBOX
const lvbe_call_c_t lv_msgbox_func[] = {
  { "close", (void*) &lv_msgbox_close, "", "(lv.lv_obj)" },
  { "get_active_btn_text", (void*) &lv_msgbox_get_active_btn_text, "s", "(lv.lv_obj)" },
  { "get_btns", (void*) &lv_msgbox_get_btns, "lv.lv_obj", "(lv.lv_obj)" },
  { "get_close_btn", (void*) &lv_msgbox_get_close_btn, "lv.lv_obj", "(lv.lv_obj)" },
  { "get_text", (void*) &lv_msgbox_get_text, "lv.lv_obj", "(lv.lv_obj)" },
  { "get_title", (void*) &lv_msgbox_get_title, "lv.lv_obj", "(lv.lv_obj)" },
};
#endif // BE_LV_WIDGET_MSGBOX

/* `lv_spinbox` methods */
#ifdef BE_LV_WIDGET_SPINBOX
const lvbe_call_c_t lv_spinbox_func[] = {
  { "decrement", (void*) &lv_spinbox_decrement, "", "(lv.lv_obj)" },
  { "get_rollover", (void*) &lv_spinbox_get_rollover, "b", "(lv.lv_obj)" },
  { "get_step", (void*) &lv_spinbox_get_step, "i", "(lv.lv_obj)" },
  { "get_value", (void*) &lv_spinbox_get_value, "i", "(lv.lv_obj)" },
  { "increment", (void*) &lv_spinbox_increment, "", "(lv.lv_obj)" },
  { "set_digit_format", (void*) &lv_spinbox_set_digit_format, "", "(lv.lv_obj)ii" },
  { "set_range", (void*) &lv_spinbox_set_range, "", "(lv.lv_obj)ii" },
  { "set_rollover", (void*) &lv_spinbox_set_rollover, "", "(lv.lv_obj)b" },
  { "set_step", (void*) &lv_spinbox_set_step, "", "(lv.lv_obj)i" },
  { "set_value", (void*) &lv_spinbox_set_value, "", "(lv.lv_obj)i" },
  { "step_next", (void*) &lv_spinbox_step_next, "", "(lv.lv_obj)" },
  { "step_prev", (void*) &lv_spinbox_step_prev, "", "(lv.lv_obj)" },
};
#endif // BE_LV_WIDGET_SPINBOX

/* `lv_spinner` methods */
#ifdef BE_LV_WIDGET_SPINNER
const lvbe_call_c_t lv_spinner_func[] = {
};
#endif // BE_LV_WIDGET_SPINNER

/* `lv_arc` methods */
#ifdef BE_LV_WIDGET_ARC
const lvbe_call_c_t lv_arc_func[] = {
  { "get_angle_end", (void*) &lv_arc_get_angle_end, "i", "(lv.lv_obj)" },
  { "get_angle_start", (void*) &lv_arc_get_angle_start, "i", "(lv.lv_obj)" },
  { "get_bg_angle_end", (void*) &lv_arc_get_bg_angle_end, "i", "(lv.lv_obj)" },
  { "get_bg_angle_start", (void*) &lv_arc_get_bg_angle_start, "i", "(lv.lv_obj)" },
  { "get_max_value", (void*) &lv_arc_get_max_value, "i", "(lv.lv_obj)" },
  { "get_min_value", (void*) &lv_arc_get_min_value, "i", "(lv.lv_obj)" },
  { "get_mode", (void*) &lv_arc_get_mode, "i", "(lv.lv_obj)" },
  { "get_value", (void*) &lv_arc_get_value, "i", "(lv.lv_obj)" },
  { "set_angles", (void*) &lv_arc_set_angles, "", "(lv.lv_obj)ii" },
  { "set_bg_angles", (void*) &lv_arc_set_bg_angles, "", "(lv.lv_obj)ii" },
  { "set_bg_end_angle", (void*) &lv_arc_set_bg_end_angle, "", "(lv.lv_obj)i" },
  { "set_bg_start_angle", (void*) &lv_arc_set_bg_start_angle, "", "(lv.lv_obj)i" },
  { "set_change_rate", (void*) &lv_arc_set_change_rate, "", "(lv.lv_obj)i" },
  { "set_end_angle", (void*) &lv_arc_set_end_angle, "", "(lv.lv_obj)i" },
  { "set_mode", (void*) &lv_arc_set_mode, "", "(lv.lv_obj)i" },
  { "set_range", (void*) &lv_arc_set_range, "", "(lv.lv_obj)ii" },
  { "set_rotation", (void*) &lv_arc_set_rotation, "", "(lv.lv_obj)i" },
  { "set_start_angle", (void*) &lv_arc_set_start_angle, "", "(lv.lv_obj)i" },
  { "set_value", (void*) &lv_arc_set_value, "", "(lv.lv_obj)i" },
};
#endif // BE_LV_WIDGET_ARC

/* `lv_bar` methods */
#ifdef BE_LV_WIDGET_BAR
const lvbe_call_c_t lv_bar_func[] = {
  { "get_max_value", (void*) &lv_bar_get_max_value, "i", "(lv.lv_obj)" },
  { "get_min_value", (void*) &lv_bar_get_min_value, "i", "(lv.lv_obj)" },
  { "get_mode", (void*) &lv_bar_get_mode, "i", "(lv.lv_obj)" },
  { "get_start_value", (void*) &lv_bar_get_start_value, "i", "(lv.lv_obj)" },
  { "get_value", (void*) &lv_bar_get_value, "i", "(lv.lv_obj)" },
  { "set_mode", (void*) &lv_bar_set_mode, "", "(lv.lv_obj)i" },
  { "set_range", (void*) &lv_bar_set_range, "", "(lv.lv_obj)ii" },
  { "set_start_value", (void*) &lv_bar_set_start_value, "", "(lv.lv_obj)i(lv.lv_anim_enable)" },
  { "set_value", (void*) &lv_bar_set_value, "", "(lv.lv_obj)i(lv.lv_anim_enable)" },
};
#endif // BE_LV_WIDGET_BAR

/* `lv_btn` methods */
#ifdef BE_LV_WIDGET_BTN
const lvbe_call_c_t lv_btn_func[] = {
};
#endif // BE_LV_WIDGET_BTN

/* `lv_btnmatrix` methods */
#ifdef BE_LV_WIDGET_BTNMATRIX
const lvbe_call_c_t lv_btnmatrix_func[] = {
  { "clear_btn_ctrl", (void*) &lv_btnmatrix_clear_btn_ctrl, "", "(lv.lv_obj)i(lv.lv_btnmatrix_ctrl)" },
  { "clear_btn_ctrl_all", (void*) &lv_btnmatrix_clear_btn_ctrl_all, "", "(lv.lv_obj)(lv.lv_btnmatrix_ctrl)" },
  { "get_btn_text", (void*) &lv_btnmatrix_get_btn_text, "s", "(lv.lv_obj)i" },
  { "get_one_checked", (void*) &lv_btnmatrix_get_one_checked, "b", "(lv.lv_obj)" },
  { "get_selected_btn", (void*) &lv_btnmatrix_get_selected_btn, "i", "(lv.lv_obj)" },
  { "has_btn_ctrl", (void*) &lv_btnmatrix_has_btn_ctrl, "b", "(lv.lv_obj)i(lv.lv_btnmatrix_ctrl)" },
  { "set_btn_ctrl", (void*) &lv_btnmatrix_set_btn_ctrl, "", "(lv.lv_obj)i(lv.lv_btnmatrix_ctrl)" },
  { "set_btn_ctrl_all", (void*) &lv_btnmatrix_set_btn_ctrl_all, "", "(lv.lv_obj)(lv.lv_btnmatrix_ctrl)" },
  { "set_btn_width", (void*) &lv_btnmatrix_set_btn_width, "", "(lv.lv_obj)ii" },
  { "set_ctrl_map", (void*) &lv_btnmatrix_set_ctrl_map, "", "(lv.lv_obj)(lv.lv_btnmatrix_ctrl)" },
  { "set_map", (void*) &lv_btnmatrix_set_map, "", "(lv.lv_obj)s" },
  { "set_one_checked", (void*) &lv_btnmatrix_set_one_checked, "", "(lv.lv_obj)b" },
  { "set_selected_btn", (void*) &lv_btnmatrix_set_selected_btn, "", "(lv.lv_obj)i" },
};
#endif // BE_LV_WIDGET_BTNMATRIX

/* `lv_canvas` methods */
#ifdef BE_LV_WIDGET_CANVAS
const lvbe_call_c_t lv_canvas_func[] = {
  { "blur_hor", (void*) &lv_canvas_blur_hor, "", "(lv.lv_obj)(lv.lv_area)i" },
  { "blur_ver", (void*) &lv_canvas_blur_ver, "", "(lv.lv_obj)(lv.lv_area)i" },
  { "copy_buf", (void*) &lv_canvas_copy_buf, "", "(lv.lv_obj).iiii" },
  { "draw_arc", (void*) &lv_canvas_draw_arc, "", "(lv.lv_obj)iiiii(lv.lv_draw_arc_dsc)" },
  { "draw_img", (void*) &lv_canvas_draw_img, "", "(lv.lv_obj)ii.(lv.lv_draw_img_dsc)" },
  { "draw_line", (void*) &lv_canvas_draw_line, "", "(lv.lv_obj)ii(lv.lv_draw_line_dsc)" },
  { "draw_polygon", (void*) &lv_canvas_draw_polygon, "", "(lv.lv_obj)ii(lv.lv_draw_rect_dsc)" },
  { "draw_rect", (void*) &lv_canvas_draw_rect, "", "(lv.lv_obj)iiii(lv.lv_draw_rect_dsc)" },
  { "draw_text", (void*) &lv_canvas_draw_text, "", "(lv.lv_obj)iii(lv.lv_draw_label_dsc)s" },
  { "fill_bg", (void*) &lv_canvas_fill_bg, "", "(lv.lv_obj)(lv.lv_color)i" },
  { "get_px", (void*) &lv_canvas_get_px, "lv.lv_color", "(lv.lv_obj)ii" },
  { "set_buffer", (void*) &lv_canvas_set_buffer, "", "(lv.lv_obj).iii" },
  { "set_palette", (void*) &lv_canvas_set_palette, "", "(lv.lv_obj)i(lv.lv_color)" },
  { "set_px", (void*) &lv_canvas_set_px, "", "(lv.lv_obj)ii(lv.lv_color)" },
  { "transform", (void*) &lv_canvas_transform, "", "(lv.lv_obj)(lv.lv_img_dsc)iiiiiib" },
};
#endif // BE_LV_WIDGET_CANVAS

/* `lv_checkbox` methods */
#ifdef BE_LV_WIDGET_CHECKBOX
const lvbe_call_c_t lv_checkbox_func[] = {
  { "get_text", (void*) &lv_checkbox_get_text, "s", "(lv.lv_obj)" },
  { "set_text", (void*) &lv_checkbox_set_text, "", "(lv.lv_obj)s" },
  { "set_text_static", (void*) &lv_checkbox_set_text_static, "", "(lv.lv_obj)s" },
};
#endif // BE_LV_WIDGET_CHECKBOX

/* `lv_dropdown` methods */
#ifdef BE_LV_WIDGET_DROPDOWN
const lvbe_call_c_t lv_dropdown_func[] = {
  { "add_option", (void*) &lv_dropdown_add_option, "", "(lv.lv_obj)si" },
  { "clear_options", (void*) &lv_dropdown_clear_options, "", "(lv.lv_obj)" },
  { "close", (void*) &lv_dropdown_close, "", "(lv.lv_obj)" },
  { "get_dir", (void*) &lv_dropdown_get_dir, "i", "(lv.lv_obj)" },
  { "get_list", (void*) &lv_dropdown_get_list, "lv.lv_obj", "(lv.lv_obj)" },
  { "get_option_cnt", (void*) &lv_dropdown_get_option_cnt, "i", "(lv.lv_obj)" },
  { "get_options", (void*) &lv_dropdown_get_options, "s", "(lv.lv_obj)" },
  { "get_selected", (void*) &lv_dropdown_get_selected, "i", "(lv.lv_obj)" },
  { "get_selected_highlight", (void*) &lv_dropdown_get_selected_highlight, "b", "(lv.lv_obj)" },
  { "get_selected_str", (void*) &lv_dropdown_get_selected_str, "", "(lv.lv_obj)si" },
  { "get_symbol", (void*) &lv_dropdown_get_symbol, "s", "(lv.lv_obj)" },
  { "get_text", (void*) &lv_dropdown_get_text, "s", "(lv.lv_obj)" },
  { "open", (void*) &lv_dropdown_open, "", "(lv.lv_obj)" },
  { "set_dir", (void*) &lv_dropdown_set_dir, "", "(lv.lv_obj)i" },
  { "set_options", (void*) &lv_dropdown_set_options, "", "(lv.lv_obj)s" },
  { "set_options_static", (void*) &lv_dropdown_set_options_static, "", "(lv.lv_obj)s" },
  { "set_selected", (void*) &lv_dropdown_set_selected, "", "(lv.lv_obj)i" },
  { "set_selected_highlight", (void*) &lv_dropdown_set_selected_highlight, "", "(lv.lv_obj)b" },
  { "set_symbol", (void*) &lv_dropdown_set_symbol, "", "(lv.lv_obj)." },
  { "set_text", (void*) &lv_dropdown_set_text, "", "(lv.lv_obj)s" },
};
#endif // BE_LV_WIDGET_DROPDOWN

/* `lv_label` methods */
#ifdef BE_LV_WIDGET_LABEL
const lvbe_call_c_t lv_label_func[] = {
  { "cut_text", (void*) &lv_label_cut_text, "", "(lv.lv_obj)ii" },
  { "get_letter_on", (void*) &lv_label_get_letter_on, "i", "(lv.lv_obj)(lv.lv_point)" },
  { "get_letter_pos", (void*) &lv_label_get_letter_pos, "", "(lv.lv_obj)i(lv.lv_point)" },
  { "get_long_mode", (void*) &lv_label_get_long_mode, "i", "(lv.lv_obj)" },
  { "get_recolor", (void*) &lv_label_get_recolor, "b", "(lv.lv_obj)" },
  { "get_text", (void*) &lv_label_get_text, "s", "(lv.lv_obj)" },
  { "get_text_selection_end", (void*) &lv_label_get_text_selection_end, "i", "(lv.lv_obj)" },
  { "get_text_selection_start", (void*) &lv_label_get_text_selection_start, "i", "(lv.lv_obj)" },
  { "ins_text", (void*) &lv_label_ins_text, "", "(lv.lv_obj)is" },
  { "is_char_under_pos", (void*) &lv_label_is_char_under_pos, "b", "(lv.lv_obj)(lv.lv_point)" },
  { "set_long_mode", (void*) &lv_label_set_long_mode, "", "(lv.lv_obj)i" },
  { "set_recolor", (void*) &lv_label_set_recolor, "", "(lv.lv_obj)b" },
  { "set_text", (void*) &lv_label_set_text, "", "(lv.lv_obj)s" },
  { "set_text_fmt", (void*) &lv_label_set_text_fmt, "", "(lv.lv_obj)s" },
  { "set_text_sel_end", (void*) &lv_label_set_text_sel_end, "", "(lv.lv_obj)i" },
  { "set_text_sel_start", (void*) &lv_label_set_text_sel_start, "", "(lv.lv_obj)i" },
  { "set_text_static", (void*) &lv_label_set_text_static, "", "(lv.lv_obj)s" },
};
#endif // BE_LV_WIDGET_LABEL

/* `lv_line` methods */
#ifdef BE_LV_WIDGET_LINE
const lvbe_call_c_t lv_line_func[] = {
  { "get_y_invert", (void*) &lv_line_get_y_invert, "b", "(lv.lv_obj)" },
  { "set_points", (void*) &lv_line_set_points, "", "(lv.lv_obj)ii" },
  { "set_y_invert", (void*) &lv_line_set_y_invert, "", "(lv.lv_obj)b" },
};
#endif // BE_LV_WIDGET_LINE

/* `lv_roller` methods */
#ifdef BE_LV_WIDGET_ROLLER
const lvbe_call_c_t lv_roller_func[] = {
  { "get_option_cnt", (void*) &lv_roller_get_option_cnt, "i", "(lv.lv_obj)" },
  { "get_options", (void*) &lv_roller_get_options, "s", "(lv.lv_obj)" },
  { "get_selected", (void*) &lv_roller_get_selected, "i", "(lv.lv_obj)" },
  { "get_selected_str", (void*) &lv_roller_get_selected_str, "", "(lv.lv_obj)si" },
  { "set_options", (void*) &lv_roller_set_options, "", "(lv.lv_obj)s(lv.lv_roller_mode)" },
  { "set_selected", (void*) &lv_roller_set_selected, "", "(lv.lv_obj)i(lv.lv_anim_enable)" },
  { "set_visible_row_count", (void*) &lv_roller_set_visible_row_count, "", "(lv.lv_obj)i" },
};
#endif // BE_LV_WIDGET_ROLLER

/* `lv_slider` methods */
#ifdef BE_LV_WIDGET_SLIDER
const lvbe_call_c_t lv_slider_func[] = {
  { "get_left_value", (void*) &lv_slider_get_left_value, "i", "(lv.lv_obj)" },
  { "get_max_value", (void*) &lv_slider_get_max_value, "i", "(lv.lv_obj)" },
  { "get_min_value", (void*) &lv_slider_get_min_value, "i", "(lv.lv_obj)" },
  { "get_mode", (void*) &lv_slider_get_mode, "i", "(lv.lv_obj)" },
  { "get_value", (void*) &lv_slider_get_value, "i", "(lv.lv_obj)" },
  { "is_dragged", (void*) &lv_slider_is_dragged, "b", "(lv.lv_obj)" },
  { "set_left_value", (void*) &lv_slider_set_left_value, "", "(lv.lv_obj)i(lv.lv_anim_enable)" },
  { "set_mode", (void*) &lv_slider_set_mode, "", "(lv.lv_obj)i" },
  { "set_range", (void*) &lv_slider_set_range, "", "(lv.lv_obj)ii" },
  { "set_value", (void*) &lv_slider_set_value, "", "(lv.lv_obj)i(lv.lv_anim_enable)" },
};
#endif // BE_LV_WIDGET_SLIDER

/* `lv_switch` methods */
#ifdef BE_LV_WIDGET_SWITCH
const lvbe_call_c_t lv_switch_func[] = {
};
#endif // BE_LV_WIDGET_SWITCH

/* `lv_table` methods */
#ifdef BE_LV_WIDGET_TABLE
const lvbe_call_c_t lv_table_func[] = {
  { "add_cell_ctrl", (void*) &lv_table_add_cell_ctrl, "", "(lv.lv_obj)ii(lv.lv_table_cell_ctrl)" },
  { "clear_cell_ctrl", (void*) &lv_table_clear_cell_ctrl, "", "(lv.lv_obj)ii(lv.lv_table_cell_ctrl)" },
  { "get_cell_value", (void*) &lv_table_get_cell_value, "s", "(lv.lv_obj)ii" },
  { "get_col_cnt", (void*) &lv_table_get_col_cnt, "i", "(lv.lv_obj)" },
  { "get_col_width", (void*) &lv_table_get_col_width, "i", "(lv.lv_obj)i" },
  { "get_row_cnt", (void*) &lv_table_get_row_cnt, "i", "(lv.lv_obj)" },
  { "get_selected_cell", (void*) &lv_table_get_selected_cell, "", "(lv.lv_obj)(lv.uint16)(lv.uint16)" },
  { "has_cell_ctrl", (void*) &lv_table_has_cell_ctrl, "b", "(lv.lv_obj)ii(lv.lv_table_cell_ctrl)" },
  { "set_cell_value", (void*) &lv_table_set_cell_value, "", "(lv.lv_obj)iis" },
  { "set_cell_value_fmt", (void*) &lv_table_set_cell_value_fmt, "", "(lv.lv_obj)iis" },
  { "set_col_cnt", (void*) &lv_table_set_col_cnt, "", "(lv.lv_obj)i" },
  { "set_col_width", (void*) &lv_table_set_col_width, "", "(lv.lv_obj)ii" },
  { "set_row_cnt", (void*) &lv_table_set_row_cnt, "", "(lv.lv_obj)i" },
};
#endif // BE_LV_WIDGET_TABLE

/* `lv_textarea` methods */
#ifdef BE_LV_WIDGET_TEXTAREA
const lvbe_call_c_t lv_textarea_func[] = {
  { "add_char", (void*) &lv_textarea_add_char, "", "(lv.lv_obj)i" },
  { "add_text", (void*) &lv_textarea_add_text, "", "(lv.lv_obj)s" },
  { "clear_selection", (void*) &lv_textarea_clear_selection, "", "(lv.lv_obj)" },
  { "cursor_down", (void*) &lv_textarea_cursor_down, "", "(lv.lv_obj)" },
  { "cursor_left", (void*) &lv_textarea_cursor_left, "", "(lv.lv_obj)" },
  { "cursor_right", (void*) &lv_textarea_cursor_right, "", "(lv.lv_obj)" },
  { "cursor_up", (void*) &lv_textarea_cursor_up, "", "(lv.lv_obj)" },
  { "del_char", (void*) &lv_textarea_del_char, "", "(lv.lv_obj)" },
  { "del_char_forward", (void*) &lv_textarea_del_char_forward, "", "(lv.lv_obj)" },
  { "get_accepted_chars", (void*) &lv_textarea_get_accepted_chars, "s", "(lv.lv_obj)" },
  { "get_cursor_click_pos", (void*) &lv_textarea_get_cursor_click_pos, "b", "(lv.lv_obj)" },
  { "get_cursor_pos", (void*) &lv_textarea_get_cursor_pos, "i", "(lv.lv_obj)" },
  { "get_label", (void*) &lv_textarea_get_label, "lv.lv_obj", "(lv.lv_obj)" },
  { "get_max_length", (void*) &lv_textarea_get_max_length, "i", "(lv.lv_obj)" },
  { "get_one_line", (void*) &lv_textarea_get_one_line, "b", "(lv.lv_obj)" },
  { "get_password_mode", (void*) &lv_textarea_get_password_mode, "b", "(lv.lv_obj)" },
  { "get_password_show_time", (void*) &lv_textarea_get_password_show_time, "i", "(lv.lv_obj)" },
  { "get_placeholder_text", (void*) &lv_textarea_get_placeholder_text, "s", "(lv.lv_obj)" },
  { "get_text", (void*) &lv_textarea_get_text, "s", "(lv.lv_obj)" },
  { "get_text_selection", (void*) &lv_textarea_get_text_selection, "b", "(lv.lv_obj)" },
  { "set_accepted_chars", (void*) &lv_textarea_set_accepted_chars, "", "(lv.lv_obj)s" },
  { "set_align", (void*) &lv_textarea_set_align, "", "(lv.lv_obj)i" },
  { "set_cursor_click_pos", (void*) &lv_textarea_set_cursor_click_pos, "", "(lv.lv_obj)b" },
  { "set_cursor_pos", (void*) &lv_textarea_set_cursor_pos, "", "(lv.lv_obj)i" },
  { "set_insert_replace", (void*) &lv_textarea_set_insert_replace, "", "(lv.lv_obj)s" },
  { "set_max_length", (void*) &lv_textarea_set_max_length, "", "(lv.lv_obj)i" },
  { "set_one_line", (void*) &lv_textarea_set_one_line, "", "(lv.lv_obj)b" },
  { "set_password_mode", (void*) &lv_textarea_set_password_mode, "", "(lv.lv_obj)b" },
  { "set_password_show_time", (void*) &lv_textarea_set_password_show_time, "", "(lv.lv_obj)i" },
  { "set_placeholder_text", (void*) &lv_textarea_set_placeholder_text, "", "(lv.lv_obj)s" },
  { "set_text", (void*) &lv_textarea_set_text, "", "(lv.lv_obj)s" },
  { "set_text_selection", (void*) &lv_textarea_set_text_selection, "", "(lv.lv_obj)b" },
  { "text_is_selected", (void*) &lv_textarea_text_is_selected, "b", "(lv.lv_obj)" },
};
#endif // BE_LV_WIDGET_TEXTAREA

extern const bclass be_class_lv_arc;
extern const bclass be_class_lv_bar;
extern const bclass be_class_lv_btn;
extern const bclass be_class_lv_btnmatrix;
extern const bclass be_class_lv_canvas;
extern const bclass be_class_lv_chart;
extern const bclass be_class_lv_checkbox;
extern const bclass be_class_lv_color;
extern const bclass be_class_lv_colorwheel;
extern const bclass be_class_lv_disp;
extern const bclass be_class_lv_dropdown;
extern const bclass be_class_lv_font;
extern const bclass be_class_lv_group;
extern const bclass be_class_lv_img;
extern const bclass be_class_lv_imgbtn;
extern const bclass be_class_lv_indev;
extern const bclass be_class_lv_label;
extern const bclass be_class_lv_led;
extern const bclass be_class_lv_line;
extern const bclass be_class_lv_meter;
extern const bclass be_class_lv_msgbox;
extern const bclass be_class_lv_obj;
extern const bclass be_class_lv_roller;
extern const bclass be_class_lv_slider;
extern const bclass be_class_lv_spinbox;
extern const bclass be_class_lv_spinner;
extern const bclass be_class_lv_style;
extern const bclass be_class_lv_switch;
extern const bclass be_class_lv_table;
extern const bclass be_class_lv_textarea;
extern const bclass be_class_lv_theme;


// map of clases
const lvbe_call_c_classes_t lv_classes[] = {
#ifdef BE_LV_WIDGET_ARC
  { "lv_arc", &be_class_lv_arc, lv_arc_func, sizeof(lv_arc_func) / sizeof(lv_arc_func[0]) },
#endif // BE_LV_WIDGET_ARC
#ifdef BE_LV_WIDGET_BAR
  { "lv_bar", &be_class_lv_bar, lv_bar_func, sizeof(lv_bar_func) / sizeof(lv_bar_func[0]) },
#endif // BE_LV_WIDGET_BAR
#ifdef BE_LV_WIDGET_BTN
  { "lv_btn", &be_class_lv_btn, lv_btn_func, sizeof(lv_btn_func) / sizeof(lv_btn_func[0]) },
#endif // BE_LV_WIDGET_BTN
#ifdef BE_LV_WIDGET_BTNMATRIX
  { "lv_btnmatrix", &be_class_lv_btnmatrix, lv_btnmatrix_func, sizeof(lv_btnmatrix_func) / sizeof(lv_btnmatrix_func[0]) },
#endif // BE_LV_WIDGET_BTNMATRIX
#ifdef BE_LV_WIDGET_CANVAS
  { "lv_canvas", &be_class_lv_canvas, lv_canvas_func, sizeof(lv_canvas_func) / sizeof(lv_canvas_func[0]) },
#endif // BE_LV_WIDGET_CANVAS
#ifdef BE_LV_WIDGET_CHART
  { "lv_chart", &be_class_lv_chart, lv_chart_func, sizeof(lv_chart_func) / sizeof(lv_chart_func[0]) },
#endif // BE_LV_WIDGET_CHART
#ifdef BE_LV_WIDGET_CHECKBOX
  { "lv_checkbox", &be_class_lv_checkbox, lv_checkbox_func, sizeof(lv_checkbox_func) / sizeof(lv_checkbox_func[0]) },
#endif // BE_LV_WIDGET_CHECKBOX
  { "lv_color", &be_class_lv_color, lv_color_func, sizeof(lv_color_func) / sizeof(lv_color_func[0]) },
#ifdef BE_LV_WIDGET_COLORWHEEL
  { "lv_colorwheel", &be_class_lv_colorwheel, lv_colorwheel_func, sizeof(lv_colorwheel_func) / sizeof(lv_colorwheel_func[0]) },
#endif // BE_LV_WIDGET_COLORWHEEL
  { "lv_disp", &be_class_lv_disp, lv_disp_func, sizeof(lv_disp_func) / sizeof(lv_disp_func[0]) },
#ifdef BE_LV_WIDGET_DROPDOWN
  { "lv_dropdown", &be_class_lv_dropdown, lv_dropdown_func, sizeof(lv_dropdown_func) / sizeof(lv_dropdown_func[0]) },
#endif // BE_LV_WIDGET_DROPDOWN
  { "lv_font", &be_class_lv_font, lv_font_func, sizeof(lv_font_func) / sizeof(lv_font_func[0]) },
  { "lv_group", &be_class_lv_group, lv_group_func, sizeof(lv_group_func) / sizeof(lv_group_func[0]) },
#ifdef BE_LV_WIDGET_IMG
  { "lv_img", &be_class_lv_img, lv_img_func, sizeof(lv_img_func) / sizeof(lv_img_func[0]) },
#endif // BE_LV_WIDGET_IMG
#ifdef BE_LV_WIDGET_IMGBTN
  { "lv_imgbtn", &be_class_lv_imgbtn, lv_imgbtn_func, sizeof(lv_imgbtn_func) / sizeof(lv_imgbtn_func[0]) },
#endif // BE_LV_WIDGET_IMGBTN
  { "lv_indev", &be_class_lv_indev, lv_indev_func, sizeof(lv_indev_func) / sizeof(lv_indev_func[0]) },
#ifdef BE_LV_WIDGET_LABEL
  { "lv_label", &be_class_lv_label, lv_label_func, sizeof(lv_label_func) / sizeof(lv_label_func[0]) },
#endif // BE_LV_WIDGET_LABEL
#ifdef BE_LV_WIDGET_LED
  { "lv_led", &be_class_lv_led, lv_led_func, sizeof(lv_led_func) / sizeof(lv_led_func[0]) },
#endif // BE_LV_WIDGET_LED
#ifdef BE_LV_WIDGET_LINE
  { "lv_line", &be_class_lv_line, lv_line_func, sizeof(lv_line_func) / sizeof(lv_line_func[0]) },
#endif // BE_LV_WIDGET_LINE
#ifdef BE_LV_WIDGET_METER
  { "lv_meter", &be_class_lv_meter, lv_meter_func, sizeof(lv_meter_func) / sizeof(lv_meter_func[0]) },
#endif // BE_LV_WIDGET_METER
#ifdef BE_LV_WIDGET_MSGBOX
  { "lv_msgbox", &be_class_lv_msgbox, lv_msgbox_func, sizeof(lv_msgbox_func) / sizeof(lv_msgbox_func[0]) },
#endif // BE_LV_WIDGET_MSGBOX
  { "lv_obj", &be_class_lv_obj, lv_obj_func, sizeof(lv_obj_func) / sizeof(lv_obj_func[0]) },
#ifdef BE_LV_WIDGET_ROLLER
  { "lv_roller", &be_class_lv_roller, lv_roller_func, sizeof(lv_roller_func) / sizeof(lv_roller_func[0]) },
#endif // BE_LV_WIDGET_ROLLER
#ifdef BE_LV_WIDGET_SLIDER
  { "lv_slider", &be_class_lv_slider, lv_slider_func, sizeof(lv_slider_func) / sizeof(lv_slider_func[0]) },
#endif // BE_LV_WIDGET_SLIDER
#ifdef BE_LV_WIDGET_SPINBOX
  { "lv_spinbox", &be_class_lv_spinbox, lv_spinbox_func, sizeof(lv_spinbox_func) / sizeof(lv_spinbox_func[0]) },
#endif // BE_LV_WIDGET_SPINBOX
#ifdef BE_LV_WIDGET_SPINNER
  { "lv_spinner", &be_class_lv_spinner, lv_spinner_func, sizeof(lv_spinner_func) / sizeof(lv_spinner_func[0]) },
#endif // BE_LV_WIDGET_SPINNER
  { "lv_style", &be_class_lv_style, lv_style_func, sizeof(lv_style_func) / sizeof(lv_style_func[0]) },
#ifdef BE_LV_WIDGET_SWITCH
  { "lv_switch", &be_class_lv_switch, lv_switch_func, sizeof(lv_switch_func) / sizeof(lv_switch_func[0]) },
#endif // BE_LV_WIDGET_SWITCH
#ifdef BE_LV_WIDGET_TABLE
  { "lv_table", &be_class_lv_table, lv_table_func, sizeof(lv_table_func) / sizeof(lv_table_func[0]) },
#endif // BE_LV_WIDGET_TABLE
#ifdef BE_LV_WIDGET_TEXTAREA
  { "lv_textarea", &be_class_lv_textarea, lv_textarea_func, sizeof(lv_textarea_func) / sizeof(lv_textarea_func[0]) },
#endif // BE_LV_WIDGET_TEXTAREA
  { "lv_theme", &be_class_lv_theme, lv_theme_func, sizeof(lv_theme_func) / sizeof(lv_theme_func[0]) },
};
const size_t lv_classes_size = sizeof(lv_classes) / sizeof(lv_classes[0]);

  /* `lv_style` methods */
  /* `lv_font` methods */
  /* `lv_color` methods */
  /* `lv_theme` methods */
  /* `lv_img` methods */
#ifdef BE_LV_WIDGET_IMG
  int be_ntv_lv_img_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_img_create, "+", "(lv.lv_obj)"); }
#endif // BE_LV_WIDGET_IMG
  /* `lv_disp` methods */
  /* `lv_obj` methods */
  int be_ntv_lv_obj_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_create, "+", "(lv.lv_obj)"); }
  /* `lv_group` methods */
  int be_ntv_lv_group_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_group_create, "+", ""); }
  /* `lv_indev` methods */
  /* `lv_chart` methods */
#ifdef BE_LV_WIDGET_CHART
  int be_ntv_lv_chart_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_chart_create, "+", "(lv.lv_obj)"); }
#endif // BE_LV_WIDGET_CHART
  /* `lv_colorwheel` methods */
#ifdef BE_LV_WIDGET_COLORWHEEL
  int be_ntv_lv_colorwheel_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_colorwheel_create, "+", "(lv.lv_obj)b"); }
#endif // BE_LV_WIDGET_COLORWHEEL
  /* `lv_imgbtn` methods */
#ifdef BE_LV_WIDGET_IMGBTN
  int be_ntv_lv_imgbtn_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_imgbtn_create, "+", "(lv.lv_obj)"); }
#endif // BE_LV_WIDGET_IMGBTN
  /* `lv_led` methods */
#ifdef BE_LV_WIDGET_LED
  int be_ntv_lv_led_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_led_create, "+", "(lv.lv_obj)"); }
#endif // BE_LV_WIDGET_LED
  /* `lv_meter` methods */
#ifdef BE_LV_WIDGET_METER
  int be_ntv_lv_meter_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_meter_create, "+", "(lv.lv_obj)"); }
#endif // BE_LV_WIDGET_METER
  /* `lv_msgbox` methods */
#ifdef BE_LV_WIDGET_MSGBOX
  int be_ntv_lv_msgbox_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_msgbox_create, "+", "(lv.lv_obj)sssb"); }
#endif // BE_LV_WIDGET_MSGBOX
  /* `lv_spinbox` methods */
#ifdef BE_LV_WIDGET_SPINBOX
  int be_ntv_lv_spinbox_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_spinbox_create, "+", "(lv.lv_obj)"); }
#endif // BE_LV_WIDGET_SPINBOX
  /* `lv_spinner` methods */
#ifdef BE_LV_WIDGET_SPINNER
  int be_ntv_lv_spinner_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_spinner_create, "+", "(lv.lv_obj)ii"); }
#endif // BE_LV_WIDGET_SPINNER
  /* `lv_arc` methods */
#ifdef BE_LV_WIDGET_ARC
  int be_ntv_lv_arc_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_arc_create, "+", "(lv.lv_obj)"); }
#endif // BE_LV_WIDGET_ARC
  /* `lv_bar` methods */
#ifdef BE_LV_WIDGET_BAR
  int be_ntv_lv_bar_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_bar_create, "+", "(lv.lv_obj)"); }
#endif // BE_LV_WIDGET_BAR
  /* `lv_btn` methods */
#ifdef BE_LV_WIDGET_BTN
  int be_ntv_lv_btn_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_btn_create, "+", "(lv.lv_obj)"); }
#endif // BE_LV_WIDGET_BTN
  /* `lv_btnmatrix` methods */
#ifdef BE_LV_WIDGET_BTNMATRIX
  int be_ntv_lv_btnmatrix_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_btnmatrix_create, "+", "(lv.lv_obj)"); }
#endif // BE_LV_WIDGET_BTNMATRIX
  /* `lv_canvas` methods */
#ifdef BE_LV_WIDGET_CANVAS
  int be_ntv_lv_canvas_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_canvas_create, "+", "(lv.lv_obj)"); }
#endif // BE_LV_WIDGET_CANVAS
  /* `lv_checkbox` methods */
#ifdef BE_LV_WIDGET_CHECKBOX
  int be_ntv_lv_checkbox_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_checkbox_create, "+", "(lv.lv_obj)"); }
#endif // BE_LV_WIDGET_CHECKBOX
  /* `lv_dropdown` methods */
#ifdef BE_LV_WIDGET_DROPDOWN
  int be_ntv_lv_dropdown_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_dropdown_create, "+", "(lv.lv_obj)"); }
#endif // BE_LV_WIDGET_DROPDOWN
  /* `lv_label` methods */
#ifdef BE_LV_WIDGET_LABEL
  int be_ntv_lv_label_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_label_create, "+", "(lv.lv_obj)"); }
#endif // BE_LV_WIDGET_LABEL
  /* `lv_line` methods */
#ifdef BE_LV_WIDGET_LINE
  int be_ntv_lv_line_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_line_create, "+", "(lv.lv_obj)"); }
#endif // BE_LV_WIDGET_LINE
  /* `lv_roller` methods */
#ifdef BE_LV_WIDGET_ROLLER
  int be_ntv_lv_roller_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_roller_create, "+", "(lv.lv_obj)"); }
#endif // BE_LV_WIDGET_ROLLER
  /* `lv_slider` methods */
#ifdef BE_LV_WIDGET_SLIDER
  int be_ntv_lv_slider_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_slider_create, "+", "(lv.lv_obj)"); }
#endif // BE_LV_WIDGET_SLIDER
  /* `lv_switch` methods */
#ifdef BE_LV_WIDGET_SWITCH
  int be_ntv_lv_switch_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_switch_create, "+", "(lv.lv_obj)"); }
#endif // BE_LV_WIDGET_SWITCH
  /* `lv_table` methods */
#ifdef BE_LV_WIDGET_TABLE
  int be_ntv_lv_table_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_table_create, "+", "(lv.lv_obj)"); }
#endif // BE_LV_WIDGET_TABLE
  /* `lv_textarea` methods */
#ifdef BE_LV_WIDGET_TEXTAREA
  int be_ntv_lv_textarea_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_create, "+", "(lv.lv_obj)"); }
#endif // BE_LV_WIDGET_TEXTAREA

// create font either empty or from parameter on stack
int lvbe_font_create(bvm *vm)       { return be_call_c_func(vm, NULL, "+lv_font", ""); }
int lvbe_theme_create(bvm *vm)       { return be_call_c_func(vm, NULL, "+lv_theme", ""); }


#ifdef __cplusplus
} /* extern "C" */
#endif
/********************************************************************/
