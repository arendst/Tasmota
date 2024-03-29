
/********************************************************************
 * Generated code, don't edit
  *******************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "be_ctypes.h"
#include "be_mapping.h"

/* `lv_style` methods */
const be_ntv_func_def_t lv_style_func[] = {
  { "is_const", { (const void*) &lv_style_is_const, "b", "(lv.style)" } },
  { "is_empty", { (const void*) &lv_style_is_empty, "b", "(lv.style)" } },
  { "remove_prop", { (const void*) &lv_style_remove_prop, "b", "(lv.style)i" } },
  { "reset", { (const void*) &lv_style_reset, "", "(lv.style)" } },
  { "set_align", { (const void*) &lv_style_set_align, "", "(lv.style)i" } },
  { "set_anim", { (const void*) &lv_style_set_anim, "", "(lv.style)(lv.anim)" } },
  { "set_anim_duration", { (const void*) &lv_style_set_anim_duration, "", "(lv.style)i" } },
  { "set_anim_time", { (const void*) &lv_style_set_anim_duration, "", "(lv.style)i" } },
  { "set_arc_color", { (const void*) &lv_style_set_arc_color, "", "(lv.style)(lv.color)" } },
  { "set_arc_image_src", { (const void*) &lv_style_set_arc_image_src, "", "(lv.style)." } },
  { "set_arc_opa", { (const void*) &lv_style_set_arc_opa, "", "(lv.style)i" } },
  { "set_arc_rounded", { (const void*) &lv_style_set_arc_rounded, "", "(lv.style)b" } },
  { "set_arc_width", { (const void*) &lv_style_set_arc_width, "", "(lv.style)i" } },
  { "set_base_dir", { (const void*) &lv_style_set_base_dir, "", "(lv.style)i" } },
  { "set_bg_color", { (const void*) &lv_style_set_bg_color, "", "(lv.style)(lv.color)" } },
  { "set_bg_grad", { (const void*) &lv_style_set_bg_grad, "", "(lv.style)(lv.grad_dsc)" } },
  { "set_bg_grad_color", { (const void*) &lv_style_set_bg_grad_color, "", "(lv.style)(lv.color)" } },
  { "set_bg_grad_dir", { (const void*) &lv_style_set_bg_grad_dir, "", "(lv.style)i" } },
  { "set_bg_grad_opa", { (const void*) &lv_style_set_bg_grad_opa, "", "(lv.style)i" } },
  { "set_bg_grad_stop", { (const void*) &lv_style_set_bg_grad_stop, "", "(lv.style)i" } },
  { "set_bg_image_opa", { (const void*) &lv_style_set_bg_image_opa, "", "(lv.style)i" } },
  { "set_bg_image_recolor", { (const void*) &lv_style_set_bg_image_recolor, "", "(lv.style)(lv.color)" } },
  { "set_bg_image_recolor_opa", { (const void*) &lv_style_set_bg_image_recolor_opa, "", "(lv.style)i" } },
  { "set_bg_image_src", { (const void*) &lv_style_set_bg_image_src, "", "(lv.style)." } },
  { "set_bg_image_tiled", { (const void*) &lv_style_set_bg_image_tiled, "", "(lv.style)b" } },
  { "set_bg_img_opa", { (const void*) &lv_style_set_bg_image_opa, "", "(lv.style)i" } },
  { "set_bg_img_recolor", { (const void*) &lv_style_set_bg_image_recolor, "", "(lv.style)(lv.color)" } },
  { "set_bg_img_recolor_opa", { (const void*) &lv_style_set_bg_image_recolor_opa, "", "(lv.style)i" } },
  { "set_bg_img_src", { (const void*) &lv_style_set_bg_image_src, "", "(lv.style)." } },
  { "set_bg_img_tiled", { (const void*) &lv_style_set_bg_image_tiled, "", "(lv.style)b" } },
  { "set_bg_main_opa", { (const void*) &lv_style_set_bg_main_opa, "", "(lv.style)i" } },
  { "set_bg_main_stop", { (const void*) &lv_style_set_bg_main_stop, "", "(lv.style)i" } },
  { "set_bg_opa", { (const void*) &lv_style_set_bg_opa, "", "(lv.style)i" } },
  { "set_bitmap_mask_src", { (const void*) &lv_style_set_bitmap_mask_src, "", "(lv.style)(lv.image_dsc)" } },
  { "set_blend_mode", { (const void*) &lv_style_set_blend_mode, "", "(lv.style)i" } },
  { "set_border_color", { (const void*) &lv_style_set_border_color, "", "(lv.style)(lv.color)" } },
  { "set_border_opa", { (const void*) &lv_style_set_border_opa, "", "(lv.style)i" } },
  { "set_border_post", { (const void*) &lv_style_set_border_post, "", "(lv.style)b" } },
  { "set_border_side", { (const void*) &lv_style_set_border_side, "", "(lv.style)i" } },
  { "set_border_width", { (const void*) &lv_style_set_border_width, "", "(lv.style)i" } },
  { "set_clip_corner", { (const void*) &lv_style_set_clip_corner, "", "(lv.style)b" } },
  { "set_color_filter_dsc", { (const void*) &lv_style_set_color_filter_dsc, "", "(lv.style)(lv.color_filter_dsc)" } },
  { "set_color_filter_opa", { (const void*) &lv_style_set_color_filter_opa, "", "(lv.style)i" } },
  { "set_flex_cross_place", { (const void*) &lv_style_set_flex_cross_place, "", "(lv.style)i" } },
  { "set_flex_flow", { (const void*) &lv_style_set_flex_flow, "", "(lv.style)i" } },
  { "set_flex_grow", { (const void*) &lv_style_set_flex_grow, "", "(lv.style)i" } },
  { "set_flex_main_place", { (const void*) &lv_style_set_flex_main_place, "", "(lv.style)i" } },
  { "set_flex_track_place", { (const void*) &lv_style_set_flex_track_place, "", "(lv.style)i" } },
  { "set_grid_cell_column_pos", { (const void*) &lv_style_set_grid_cell_column_pos, "", "(lv.style)i" } },
  { "set_grid_cell_column_span", { (const void*) &lv_style_set_grid_cell_column_span, "", "(lv.style)i" } },
  { "set_grid_cell_row_pos", { (const void*) &lv_style_set_grid_cell_row_pos, "", "(lv.style)i" } },
  { "set_grid_cell_row_span", { (const void*) &lv_style_set_grid_cell_row_span, "", "(lv.style)i" } },
  { "set_grid_cell_x_align", { (const void*) &lv_style_set_grid_cell_x_align, "", "(lv.style)i" } },
  { "set_grid_cell_y_align", { (const void*) &lv_style_set_grid_cell_y_align, "", "(lv.style)i" } },
  { "set_grid_column_align", { (const void*) &lv_style_set_grid_column_align, "", "(lv.style)i" } },
  { "set_grid_column_dsc_array", { (const void*) &lv_style_set_grid_column_dsc_array, "", "(lv.style)(lv.int_arr)" } },
  { "set_grid_row_align", { (const void*) &lv_style_set_grid_row_align, "", "(lv.style)i" } },
  { "set_grid_row_dsc_array", { (const void*) &lv_style_set_grid_row_dsc_array, "", "(lv.style)(lv.int_arr)" } },
  { "set_height", { (const void*) &lv_style_set_height, "", "(lv.style)i" } },
  { "set_image_opa", { (const void*) &lv_style_set_image_opa, "", "(lv.style)i" } },
  { "set_image_recolor", { (const void*) &lv_style_set_image_recolor, "", "(lv.style)(lv.color)" } },
  { "set_image_recolor_opa", { (const void*) &lv_style_set_image_recolor_opa, "", "(lv.style)i" } },
  { "set_img_opa", { (const void*) &lv_style_set_image_opa, "", "(lv.style)i" } },
  { "set_img_recolor", { (const void*) &lv_style_set_image_recolor, "", "(lv.style)(lv.color)" } },
  { "set_img_recolor_opa", { (const void*) &lv_style_set_image_recolor_opa, "", "(lv.style)i" } },
  { "set_layout", { (const void*) &lv_style_set_layout, "", "(lv.style)i" } },
  { "set_length", { (const void*) &lv_style_set_length, "", "(lv.style)i" } },
  { "set_line_color", { (const void*) &lv_style_set_line_color, "", "(lv.style)(lv.color)" } },
  { "set_line_dash_gap", { (const void*) &lv_style_set_line_dash_gap, "", "(lv.style)i" } },
  { "set_line_dash_width", { (const void*) &lv_style_set_line_dash_width, "", "(lv.style)i" } },
  { "set_line_opa", { (const void*) &lv_style_set_line_opa, "", "(lv.style)i" } },
  { "set_line_rounded", { (const void*) &lv_style_set_line_rounded, "", "(lv.style)b" } },
  { "set_line_width", { (const void*) &lv_style_set_line_width, "", "(lv.style)i" } },
  { "set_margin_bottom", { (const void*) &lv_style_set_margin_bottom, "", "(lv.style)i" } },
  { "set_margin_left", { (const void*) &lv_style_set_margin_left, "", "(lv.style)i" } },
  { "set_margin_right", { (const void*) &lv_style_set_margin_right, "", "(lv.style)i" } },
  { "set_margin_top", { (const void*) &lv_style_set_margin_top, "", "(lv.style)i" } },
  { "set_max_height", { (const void*) &lv_style_set_max_height, "", "(lv.style)i" } },
  { "set_max_width", { (const void*) &lv_style_set_max_width, "", "(lv.style)i" } },
  { "set_min_height", { (const void*) &lv_style_set_min_height, "", "(lv.style)i" } },
  { "set_min_width", { (const void*) &lv_style_set_min_width, "", "(lv.style)i" } },
  { "set_opa", { (const void*) &lv_style_set_opa, "", "(lv.style)i" } },
  { "set_opa_layered", { (const void*) &lv_style_set_opa_layered, "", "(lv.style)i" } },
  { "set_outline_color", { (const void*) &lv_style_set_outline_color, "", "(lv.style)(lv.color)" } },
  { "set_outline_opa", { (const void*) &lv_style_set_outline_opa, "", "(lv.style)i" } },
  { "set_outline_pad", { (const void*) &lv_style_set_outline_pad, "", "(lv.style)i" } },
  { "set_outline_width", { (const void*) &lv_style_set_outline_width, "", "(lv.style)i" } },
  { "set_pad_all", { (const void*) &lv_style_set_pad_all, "", "(lv.style)i" } },
  { "set_pad_bottom", { (const void*) &lv_style_set_pad_bottom, "", "(lv.style)i" } },
  { "set_pad_column", { (const void*) &lv_style_set_pad_column, "", "(lv.style)i" } },
  { "set_pad_gap", { (const void*) &lv_style_set_pad_gap, "", "(lv.style)i" } },
  { "set_pad_hor", { (const void*) &lv_style_set_pad_hor, "", "(lv.style)i" } },
  { "set_pad_left", { (const void*) &lv_style_set_pad_left, "", "(lv.style)i" } },
  { "set_pad_right", { (const void*) &lv_style_set_pad_right, "", "(lv.style)i" } },
  { "set_pad_row", { (const void*) &lv_style_set_pad_row, "", "(lv.style)i" } },
  { "set_pad_top", { (const void*) &lv_style_set_pad_top, "", "(lv.style)i" } },
  { "set_pad_ver", { (const void*) &lv_style_set_pad_ver, "", "(lv.style)i" } },
  { "set_prop", { (const void*) &lv_style_set_prop, "", "(lv.style)ii" } },
  { "set_radius", { (const void*) &lv_style_set_radius, "", "(lv.style)i" } },
  { "set_rotary_sensitivity", { (const void*) &lv_style_set_rotary_sensitivity, "", "(lv.style)i" } },
  { "set_shadow_color", { (const void*) &lv_style_set_shadow_color, "", "(lv.style)(lv.color)" } },
  { "set_shadow_offset_x", { (const void*) &lv_style_set_shadow_offset_x, "", "(lv.style)i" } },
  { "set_shadow_offset_y", { (const void*) &lv_style_set_shadow_offset_y, "", "(lv.style)i" } },
  { "set_shadow_ofs_x", { (const void*) &lv_style_set_shadow_offset_x, "", "(lv.style)i" } },
  { "set_shadow_ofs_y", { (const void*) &lv_style_set_shadow_offset_y, "", "(lv.style)i" } },
  { "set_shadow_opa", { (const void*) &lv_style_set_shadow_opa, "", "(lv.style)i" } },
  { "set_shadow_spread", { (const void*) &lv_style_set_shadow_spread, "", "(lv.style)i" } },
  { "set_shadow_width", { (const void*) &lv_style_set_shadow_width, "", "(lv.style)i" } },
  { "set_size", { (const void*) &lv_style_set_size, "", "(lv.style)ii" } },
  { "set_text_align", { (const void*) &lv_style_set_text_align, "", "(lv.style)i" } },
  { "set_text_color", { (const void*) &lv_style_set_text_color, "", "(lv.style)(lv.color)" } },
  { "set_text_decor", { (const void*) &lv_style_set_text_decor, "", "(lv.style)i" } },
  { "set_text_font", { (const void*) &lv_style_set_text_font, "", "(lv.style)(lv.font)" } },
  { "set_text_letter_space", { (const void*) &lv_style_set_text_letter_space, "", "(lv.style)i" } },
  { "set_text_line_space", { (const void*) &lv_style_set_text_line_space, "", "(lv.style)i" } },
  { "set_text_opa", { (const void*) &lv_style_set_text_opa, "", "(lv.style)i" } },
  { "set_transform_angle", { (const void*) &lv_style_set_transform_rotation, "", "(lv.style)i" } },
  { "set_transform_height", { (const void*) &lv_style_set_transform_height, "", "(lv.style)i" } },
  { "set_transform_pivot_x", { (const void*) &lv_style_set_transform_pivot_x, "", "(lv.style)i" } },
  { "set_transform_pivot_y", { (const void*) &lv_style_set_transform_pivot_y, "", "(lv.style)i" } },
  { "set_transform_rotation", { (const void*) &lv_style_set_transform_rotation, "", "(lv.style)i" } },
  { "set_transform_scale", { (const void*) &lv_style_set_transform_scale, "", "(lv.style)i" } },
  { "set_transform_scale_x", { (const void*) &lv_style_set_transform_scale_x, "", "(lv.style)i" } },
  { "set_transform_scale_y", { (const void*) &lv_style_set_transform_scale_y, "", "(lv.style)i" } },
  { "set_transform_skew_x", { (const void*) &lv_style_set_transform_skew_x, "", "(lv.style)i" } },
  { "set_transform_skew_y", { (const void*) &lv_style_set_transform_skew_y, "", "(lv.style)i" } },
  { "set_transform_width", { (const void*) &lv_style_set_transform_width, "", "(lv.style)i" } },
  { "set_transform_zoom", { (const void*) &lv_style_set_transform_scale, "", "(lv.style)i" } },
  { "set_transition", { (const void*) &lv_style_set_transition, "", "(lv.style)(lv.style_transition_dsc)" } },
  { "set_translate_x", { (const void*) &lv_style_set_translate_x, "", "(lv.style)i" } },
  { "set_translate_y", { (const void*) &lv_style_set_translate_y, "", "(lv.style)i" } },
  { "set_width", { (const void*) &lv_style_set_width, "", "(lv.style)i" } },
  { "set_x", { (const void*) &lv_style_set_x, "", "(lv.style)i" } },
  { "set_y", { (const void*) &lv_style_set_y, "", "(lv.style)i" } },
};

/* `lv_font` methods */
const be_ntv_func_def_t lv_font_func[] = {
};

/* `lv_color` methods */
const be_ntv_func_def_t lv_color_func[] = {
};

/* `lv_theme` methods */
const be_ntv_func_def_t lv_theme_func[] = {
};

/* `lv_group` methods */
const be_ntv_func_def_t lv_group_func[] = {
  { "add_obj", { (const void*) &lv_group_add_obj, "", "(lv.group)(lv.obj)" } },
  { "del", { (const void*) &lv_group_delete, "", "(lv.group)" } },
  { "delete", { (const void*) &lv_group_delete, "", "(lv.group)" } },
  { "focus_freeze", { (const void*) &lv_group_focus_freeze, "", "(lv.group)b" } },
  { "focus_next", { (const void*) &lv_group_focus_next, "", "(lv.group)" } },
  { "focus_obj", { (const void*) &lv_group_focus_obj, "", "(lv.obj)" } },
  { "focus_prev", { (const void*) &lv_group_focus_prev, "", "(lv.group)" } },
  { "get_edge_cb", { (const void*) &lv_group_get_edge_cb, "C", "(lv.group)" } },
  { "get_editing", { (const void*) &lv_group_get_editing, "b", "(lv.group)" } },
  { "get_focus_cb", { (const void*) &lv_group_get_focus_cb, "lv.group_focus_cb", "(lv.group)" } },
  { "get_focused", { (const void*) &lv_group_get_focused, "lv.obj", "(lv.group)" } },
  { "get_obj_count", { (const void*) &lv_group_get_obj_count, "i", "(lv.group)" } },
  { "get_wrap", { (const void*) &lv_group_get_wrap, "b", "(lv.group)" } },
  { "remove", { (const void*) &lv_group_delete, "", "(lv.group)" } },
  { "remove_all_objs", { (const void*) &lv_group_remove_all_objs, "", "(lv.group)" } },
  { "remove_obj", { (const void*) &lv_group_remove_obj, "", "(lv.obj)" } },
  { "send_data", { (const void*) &lv_group_send_data, "i", "(lv.group)i" } },
  { "set_default", { (const void*) &lv_group_set_default, "", "(lv.group)" } },
  { "set_editing", { (const void*) &lv_group_set_editing, "", "(lv.group)b" } },
  { "set_focus_cb", { (const void*) &lv_group_set_focus_cb, "", "(lv.group)^lv_group_focus_cb^" } },
  { "set_refocus_policy", { (const void*) &lv_group_set_refocus_policy, "", "(lv.group)i" } },
  { "set_wrap", { (const void*) &lv_group_set_wrap, "", "(lv.group)b" } },
  { "swap_obj", { (const void*) &lv_group_swap_obj, "", "(lv.obj)(lv.obj)" } },
};

/* `lv_obj` methods */
#ifdef BE_LV_WIDGET_OBJ
const be_ntv_func_def_t lv_obj_func[] = {
  { "add_event_cb", { (const void*) &lv_obj_add_event_cb, "lv.event_dsc", "(lv.obj)^lv_event_cb^i." } },
  { "add_flag", { (const void*) &lv_obj_add_flag, "", "(lv.obj)i" } },
  { "add_state", { (const void*) &lv_obj_add_state, "", "(lv.obj)i" } },
  { "add_style", { (const void*) &lv_obj_add_style, "", "(lv.obj)(lv.style)i" } },
  { "align", { (const void*) &lv_obj_align, "", "(lv.obj)iii" } },
  { "align_to", { (const void*) &lv_obj_align_to, "", "(lv.obj)(lv.obj)iii" } },
  { "allocate_spec_attr", { (const void*) &lv_obj_allocate_spec_attr, "", "(lv.obj)" } },
  { "area_is_visible", { (const void*) &lv_obj_area_is_visible, "b", "(lv.obj)(lv.area)" } },
  { "calculate_ext_draw_size", { (const void*) &lv_obj_calculate_ext_draw_size, "i", "(lv.obj)i" } },
  { "calculate_style_text_align", { (const void*) &lv_obj_calculate_style_text_align, "i", "(lv.obj)is" } },
  { "center", { (const void*) &lv_obj_center, "", "(lv.obj)" } },
  { "check_type", { (const void*) &lv_obj_check_type, "b", "(lv.obj)(lv.obj_class)" } },
  { "class_init_obj", { (const void*) &lv_obj_class_init_obj, "", "(lv.obj)" } },
  { "clean", { (const void*) &lv_obj_clean, "", "(lv.obj)" } },
  { "clear_flag", { (const void*) &lv_obj_remove_flag, "", "(lv.obj)i" } },
  { "clear_state", { (const void*) &lv_obj_remove_state, "", "(lv.obj)i" } },
  { "del", { (const void*) &lv_obj_delete, "", "(lv.obj)" } },
  { "del_async", { (const void*) &lv_obj_delete_async, "", "(lv.obj)" } },
  { "delete", { (const void*) &lv_obj_delete, "", "(lv.obj)" } },
  { "delete_async", { (const void*) &lv_obj_delete_async, "", "(lv.obj)" } },
  { "delete_delayed", { (const void*) &lv_obj_delete_delayed, "", "(lv.obj)i" } },
  { "dump_tree", { (const void*) &lv_obj_dump_tree, "", "(lv.obj)" } },
  { "fade_in", { (const void*) &lv_obj_fade_in, "", "(lv.obj)ii" } },
  { "fade_out", { (const void*) &lv_obj_fade_out, "", "(lv.obj)ii" } },
  { "free_id", { (const void*) &lv_obj_free_id, "", "(lv.obj)" } },
  { "get_child", { (const void*) &lv_obj_get_child, "lv.obj", "(lv.obj)i" } },
  { "get_child_by_type", { (const void*) &lv_obj_get_child_by_type, "lv.obj", "(lv.obj)i(lv.obj_class)" } },
  { "get_child_cnt", { (const void*) &lv_obj_get_child_count, "i", "(lv.obj)" } },
  { "get_child_count", { (const void*) &lv_obj_get_child_count, "i", "(lv.obj)" } },
  { "get_child_count_by_type", { (const void*) &lv_obj_get_child_count_by_type, "i", "(lv.obj)(lv.obj_class)" } },
  { "get_class", { (const void*) &lv_obj_get_class, "lv.obj_class", "(lv.obj)" } },
  { "get_click_area", { (const void*) &lv_obj_get_click_area, "", "(lv.obj)(lv.area)" } },
  { "get_content_coords", { (const void*) &lv_obj_get_content_coords, "", "(lv.obj)(lv.area)" } },
  { "get_content_height", { (const void*) &lv_obj_get_content_height, "i", "(lv.obj)" } },
  { "get_content_width", { (const void*) &lv_obj_get_content_width, "i", "(lv.obj)" } },
  { "get_coords", { (const void*) &lv_obj_get_coords, "", "(lv.obj)(lv.area)" } },
  { "get_disp", { (const void*) &lv_obj_get_display, "lv.display", "(lv.obj)" } },
  { "get_display", { (const void*) &lv_obj_get_display, "lv.display", "(lv.obj)" } },
  { "get_event_count", { (const void*) &lv_obj_get_event_count, "i", "(lv.obj)" } },
  { "get_event_dsc", { (const void*) &lv_obj_get_event_dsc, "lv.event_dsc", "(lv.obj)i" } },
  { "get_group", { (const void*) &lv_obj_get_group, "lv.group", "(lv.obj)" } },
  { "get_height", { (const void*) &lv_obj_get_height, "i", "(lv.obj)" } },
  { "get_index", { (const void*) &lv_obj_get_index, "i", "(lv.obj)" } },
  { "get_index_by_type", { (const void*) &lv_obj_get_index_by_type, "i", "(lv.obj)(lv.obj_class)" } },
  { "get_parent", { (const void*) &lv_obj_get_parent, "lv.obj", "(lv.obj)" } },
  { "get_screen", { (const void*) &lv_obj_get_screen, "lv.obj", "(lv.obj)" } },
  { "get_scroll_bottom", { (const void*) &lv_obj_get_scroll_bottom, "i", "(lv.obj)" } },
  { "get_scroll_dir", { (const void*) &lv_obj_get_scroll_dir, "i", "(lv.obj)" } },
  { "get_scroll_end", { (const void*) &lv_obj_get_scroll_end, "", "(lv.obj)c" } },
  { "get_scroll_left", { (const void*) &lv_obj_get_scroll_left, "i", "(lv.obj)" } },
  { "get_scroll_right", { (const void*) &lv_obj_get_scroll_right, "i", "(lv.obj)" } },
  { "get_scroll_snap_x", { (const void*) &lv_obj_get_scroll_snap_x, "i", "(lv.obj)" } },
  { "get_scroll_snap_y", { (const void*) &lv_obj_get_scroll_snap_y, "i", "(lv.obj)" } },
  { "get_scroll_top", { (const void*) &lv_obj_get_scroll_top, "i", "(lv.obj)" } },
  { "get_scroll_x", { (const void*) &lv_obj_get_scroll_x, "i", "(lv.obj)" } },
  { "get_scroll_y", { (const void*) &lv_obj_get_scroll_y, "i", "(lv.obj)" } },
  { "get_scrollbar_area", { (const void*) &lv_obj_get_scrollbar_area, "", "(lv.obj)(lv.area)(lv.area)" } },
  { "get_scrollbar_mode", { (const void*) &lv_obj_get_scrollbar_mode, "i", "(lv.obj)" } },
  { "get_self_height", { (const void*) &lv_obj_get_self_height, "i", "(lv.obj)" } },
  { "get_self_width", { (const void*) &lv_obj_get_self_width, "i", "(lv.obj)" } },
  { "get_sibling", { (const void*) &lv_obj_get_sibling, "lv.obj", "(lv.obj)i" } },
  { "get_sibling_by_type", { (const void*) &lv_obj_get_sibling_by_type, "lv.obj", "(lv.obj)i(lv.obj_class)" } },
  { "get_state", { (const void*) &lv_obj_get_state, "i", "(lv.obj)" } },
  { "get_style_align", { (const void*) &lv_obj_get_style_align, "i", "(lv.obj)i" } },
  { "get_style_anim", { (const void*) &lv_obj_get_style_anim, "lv.anim", "(lv.obj)i" } },
  { "get_style_anim_duration", { (const void*) &lv_obj_get_style_anim_duration, "i", "(lv.obj)i" } },
  { "get_style_anim_time", { (const void*) &lv_obj_get_style_anim_duration, "i", "(lv.obj)i" } },
  { "get_style_arc_color", { (const void*) &lv_obj_get_style_arc_color, "lv.color", "(lv.obj)i" } },
  { "get_style_arc_color_filtered", { (const void*) &lv_obj_get_style_arc_color_filtered, "lv.color", "(lv.obj)i" } },
  { "get_style_arc_image_src", { (const void*) &lv_obj_get_style_arc_image_src, "c", "(lv.obj)i" } },
  { "get_style_arc_opa", { (const void*) &lv_obj_get_style_arc_opa, "i", "(lv.obj)i" } },
  { "get_style_arc_rounded", { (const void*) &lv_obj_get_style_arc_rounded, "b", "(lv.obj)i" } },
  { "get_style_arc_width", { (const void*) &lv_obj_get_style_arc_width, "i", "(lv.obj)i" } },
  { "get_style_base_dir", { (const void*) &lv_obj_get_style_base_dir, "i", "(lv.obj)i" } },
  { "get_style_bg_color", { (const void*) &lv_obj_get_style_bg_color, "lv.color", "(lv.obj)i" } },
  { "get_style_bg_color_filtered", { (const void*) &lv_obj_get_style_bg_color_filtered, "lv.color", "(lv.obj)i" } },
  { "get_style_bg_grad", { (const void*) &lv_obj_get_style_bg_grad, "lv.grad_dsc", "(lv.obj)i" } },
  { "get_style_bg_grad_color", { (const void*) &lv_obj_get_style_bg_grad_color, "lv.color", "(lv.obj)i" } },
  { "get_style_bg_grad_color_filtered", { (const void*) &lv_obj_get_style_bg_grad_color_filtered, "lv.color", "(lv.obj)i" } },
  { "get_style_bg_grad_dir", { (const void*) &lv_obj_get_style_bg_grad_dir, "i", "(lv.obj)i" } },
  { "get_style_bg_grad_opa", { (const void*) &lv_obj_get_style_bg_grad_opa, "i", "(lv.obj)i" } },
  { "get_style_bg_grad_stop", { (const void*) &lv_obj_get_style_bg_grad_stop, "i", "(lv.obj)i" } },
  { "get_style_bg_image_opa", { (const void*) &lv_obj_get_style_bg_image_opa, "i", "(lv.obj)i" } },
  { "get_style_bg_image_recolor", { (const void*) &lv_obj_get_style_bg_image_recolor, "lv.color", "(lv.obj)i" } },
  { "get_style_bg_image_recolor_filtered", { (const void*) &lv_obj_get_style_bg_image_recolor_filtered, "lv.color", "(lv.obj)i" } },
  { "get_style_bg_image_recolor_opa", { (const void*) &lv_obj_get_style_bg_image_recolor_opa, "i", "(lv.obj)i" } },
  { "get_style_bg_image_src", { (const void*) &lv_obj_get_style_bg_image_src, "c", "(lv.obj)i" } },
  { "get_style_bg_image_tiled", { (const void*) &lv_obj_get_style_bg_image_tiled, "b", "(lv.obj)i" } },
  { "get_style_bg_main_opa", { (const void*) &lv_obj_get_style_bg_main_opa, "i", "(lv.obj)i" } },
  { "get_style_bg_main_stop", { (const void*) &lv_obj_get_style_bg_main_stop, "i", "(lv.obj)i" } },
  { "get_style_bg_opa", { (const void*) &lv_obj_get_style_bg_opa, "i", "(lv.obj)i" } },
  { "get_style_bitmap_mask_src", { (const void*) &lv_obj_get_style_bitmap_mask_src, "lv.image_dsc", "(lv.obj)i" } },
  { "get_style_blend_mode", { (const void*) &lv_obj_get_style_blend_mode, "i", "(lv.obj)i" } },
  { "get_style_border_color", { (const void*) &lv_obj_get_style_border_color, "lv.color", "(lv.obj)i" } },
  { "get_style_border_color_filtered", { (const void*) &lv_obj_get_style_border_color_filtered, "lv.color", "(lv.obj)i" } },
  { "get_style_border_opa", { (const void*) &lv_obj_get_style_border_opa, "i", "(lv.obj)i" } },
  { "get_style_border_post", { (const void*) &lv_obj_get_style_border_post, "b", "(lv.obj)i" } },
  { "get_style_border_side", { (const void*) &lv_obj_get_style_border_side, "i", "(lv.obj)i" } },
  { "get_style_border_width", { (const void*) &lv_obj_get_style_border_width, "i", "(lv.obj)i" } },
  { "get_style_clip_corner", { (const void*) &lv_obj_get_style_clip_corner, "b", "(lv.obj)i" } },
  { "get_style_color_filter_dsc", { (const void*) &lv_obj_get_style_color_filter_dsc, "lv.color_filter_dsc", "(lv.obj)i" } },
  { "get_style_color_filter_opa", { (const void*) &lv_obj_get_style_color_filter_opa, "i", "(lv.obj)i" } },
  { "get_style_flex_cross_place", { (const void*) &lv_obj_get_style_flex_cross_place, "i", "(lv.obj)i" } },
  { "get_style_flex_flow", { (const void*) &lv_obj_get_style_flex_flow, "i", "(lv.obj)i" } },
  { "get_style_flex_grow", { (const void*) &lv_obj_get_style_flex_grow, "i", "(lv.obj)i" } },
  { "get_style_flex_main_place", { (const void*) &lv_obj_get_style_flex_main_place, "i", "(lv.obj)i" } },
  { "get_style_flex_track_place", { (const void*) &lv_obj_get_style_flex_track_place, "i", "(lv.obj)i" } },
  { "get_style_grid_cell_column_pos", { (const void*) &lv_obj_get_style_grid_cell_column_pos, "i", "(lv.obj)i" } },
  { "get_style_grid_cell_column_span", { (const void*) &lv_obj_get_style_grid_cell_column_span, "i", "(lv.obj)i" } },
  { "get_style_grid_cell_row_pos", { (const void*) &lv_obj_get_style_grid_cell_row_pos, "i", "(lv.obj)i" } },
  { "get_style_grid_cell_row_span", { (const void*) &lv_obj_get_style_grid_cell_row_span, "i", "(lv.obj)i" } },
  { "get_style_grid_cell_x_align", { (const void*) &lv_obj_get_style_grid_cell_x_align, "i", "(lv.obj)i" } },
  { "get_style_grid_cell_y_align", { (const void*) &lv_obj_get_style_grid_cell_y_align, "i", "(lv.obj)i" } },
  { "get_style_grid_column_align", { (const void*) &lv_obj_get_style_grid_column_align, "i", "(lv.obj)i" } },
  { "get_style_grid_column_dsc_array", { (const void*) &lv_obj_get_style_grid_column_dsc_array, "lv.int_arr", "(lv.obj)i" } },
  { "get_style_grid_row_align", { (const void*) &lv_obj_get_style_grid_row_align, "i", "(lv.obj)i" } },
  { "get_style_grid_row_dsc_array", { (const void*) &lv_obj_get_style_grid_row_dsc_array, "lv.int_arr", "(lv.obj)i" } },
  { "get_style_height", { (const void*) &lv_obj_get_style_height, "i", "(lv.obj)i" } },
  { "get_style_image_opa", { (const void*) &lv_obj_get_style_image_opa, "i", "(lv.obj)i" } },
  { "get_style_image_recolor", { (const void*) &lv_obj_get_style_image_recolor, "lv.color", "(lv.obj)i" } },
  { "get_style_image_recolor_filtered", { (const void*) &lv_obj_get_style_image_recolor_filtered, "lv.color", "(lv.obj)i" } },
  { "get_style_image_recolor_opa", { (const void*) &lv_obj_get_style_image_recolor_opa, "i", "(lv.obj)i" } },
  { "get_style_img_opa", { (const void*) &lv_obj_get_style_image_opa, "i", "(lv.obj)i" } },
  { "get_style_img_recolor", { (const void*) &lv_obj_get_style_image_recolor, "lv.color", "(lv.obj)i" } },
  { "get_style_img_recolor_filtered", { (const void*) &lv_obj_get_style_image_recolor_filtered, "lv.color", "(lv.obj)i" } },
  { "get_style_img_recolor_opa", { (const void*) &lv_obj_get_style_image_recolor_opa, "i", "(lv.obj)i" } },
  { "get_style_layout", { (const void*) &lv_obj_get_style_layout, "i", "(lv.obj)i" } },
  { "get_style_length", { (const void*) &lv_obj_get_style_length, "i", "(lv.obj)i" } },
  { "get_style_line_color", { (const void*) &lv_obj_get_style_line_color, "lv.color", "(lv.obj)i" } },
  { "get_style_line_color_filtered", { (const void*) &lv_obj_get_style_line_color_filtered, "lv.color", "(lv.obj)i" } },
  { "get_style_line_dash_gap", { (const void*) &lv_obj_get_style_line_dash_gap, "i", "(lv.obj)i" } },
  { "get_style_line_dash_width", { (const void*) &lv_obj_get_style_line_dash_width, "i", "(lv.obj)i" } },
  { "get_style_line_opa", { (const void*) &lv_obj_get_style_line_opa, "i", "(lv.obj)i" } },
  { "get_style_line_rounded", { (const void*) &lv_obj_get_style_line_rounded, "b", "(lv.obj)i" } },
  { "get_style_line_width", { (const void*) &lv_obj_get_style_line_width, "i", "(lv.obj)i" } },
  { "get_style_margin_bottom", { (const void*) &lv_obj_get_style_margin_bottom, "i", "(lv.obj)i" } },
  { "get_style_margin_left", { (const void*) &lv_obj_get_style_margin_left, "i", "(lv.obj)i" } },
  { "get_style_margin_right", { (const void*) &lv_obj_get_style_margin_right, "i", "(lv.obj)i" } },
  { "get_style_margin_top", { (const void*) &lv_obj_get_style_margin_top, "i", "(lv.obj)i" } },
  { "get_style_max_height", { (const void*) &lv_obj_get_style_max_height, "i", "(lv.obj)i" } },
  { "get_style_max_width", { (const void*) &lv_obj_get_style_max_width, "i", "(lv.obj)i" } },
  { "get_style_min_height", { (const void*) &lv_obj_get_style_min_height, "i", "(lv.obj)i" } },
  { "get_style_min_width", { (const void*) &lv_obj_get_style_min_width, "i", "(lv.obj)i" } },
  { "get_style_opa", { (const void*) &lv_obj_get_style_opa, "i", "(lv.obj)i" } },
  { "get_style_opa_layered", { (const void*) &lv_obj_get_style_opa_layered, "i", "(lv.obj)i" } },
  { "get_style_opa_recursive", { (const void*) &lv_obj_get_style_opa_recursive, "i", "(lv.obj)i" } },
  { "get_style_outline_color", { (const void*) &lv_obj_get_style_outline_color, "lv.color", "(lv.obj)i" } },
  { "get_style_outline_color_filtered", { (const void*) &lv_obj_get_style_outline_color_filtered, "lv.color", "(lv.obj)i" } },
  { "get_style_outline_opa", { (const void*) &lv_obj_get_style_outline_opa, "i", "(lv.obj)i" } },
  { "get_style_outline_pad", { (const void*) &lv_obj_get_style_outline_pad, "i", "(lv.obj)i" } },
  { "get_style_outline_width", { (const void*) &lv_obj_get_style_outline_width, "i", "(lv.obj)i" } },
  { "get_style_pad_bottom", { (const void*) &lv_obj_get_style_pad_bottom, "i", "(lv.obj)i" } },
  { "get_style_pad_column", { (const void*) &lv_obj_get_style_pad_column, "i", "(lv.obj)i" } },
  { "get_style_pad_left", { (const void*) &lv_obj_get_style_pad_left, "i", "(lv.obj)i" } },
  { "get_style_pad_right", { (const void*) &lv_obj_get_style_pad_right, "i", "(lv.obj)i" } },
  { "get_style_pad_row", { (const void*) &lv_obj_get_style_pad_row, "i", "(lv.obj)i" } },
  { "get_style_pad_top", { (const void*) &lv_obj_get_style_pad_top, "i", "(lv.obj)i" } },
  { "get_style_prop", { (const void*) &lv_obj_get_style_prop, "i", "(lv.obj)ii" } },
  { "get_style_radius", { (const void*) &lv_obj_get_style_radius, "i", "(lv.obj)i" } },
  { "get_style_rotary_sensitivity", { (const void*) &lv_obj_get_style_rotary_sensitivity, "i", "(lv.obj)i" } },
  { "get_style_shadow_color", { (const void*) &lv_obj_get_style_shadow_color, "lv.color", "(lv.obj)i" } },
  { "get_style_shadow_color_filtered", { (const void*) &lv_obj_get_style_shadow_color_filtered, "lv.color", "(lv.obj)i" } },
  { "get_style_shadow_offset_x", { (const void*) &lv_obj_get_style_shadow_offset_x, "i", "(lv.obj)i" } },
  { "get_style_shadow_offset_y", { (const void*) &lv_obj_get_style_shadow_offset_y, "i", "(lv.obj)i" } },
  { "get_style_shadow_ofs_x", { (const void*) &lv_obj_get_style_shadow_offset_x, "i", "(lv.obj)i" } },
  { "get_style_shadow_ofs_y", { (const void*) &lv_obj_get_style_shadow_offset_y, "i", "(lv.obj)i" } },
  { "get_style_shadow_opa", { (const void*) &lv_obj_get_style_shadow_opa, "i", "(lv.obj)i" } },
  { "get_style_shadow_spread", { (const void*) &lv_obj_get_style_shadow_spread, "i", "(lv.obj)i" } },
  { "get_style_shadow_width", { (const void*) &lv_obj_get_style_shadow_width, "i", "(lv.obj)i" } },
  { "get_style_space_bottom", { (const void*) &lv_obj_get_style_space_bottom, "i", "(lv.obj)i" } },
  { "get_style_space_left", { (const void*) &lv_obj_get_style_space_left, "i", "(lv.obj)i" } },
  { "get_style_space_right", { (const void*) &lv_obj_get_style_space_right, "i", "(lv.obj)i" } },
  { "get_style_space_top", { (const void*) &lv_obj_get_style_space_top, "i", "(lv.obj)i" } },
  { "get_style_text_align", { (const void*) &lv_obj_get_style_text_align, "i", "(lv.obj)i" } },
  { "get_style_text_color", { (const void*) &lv_obj_get_style_text_color, "lv.color", "(lv.obj)i" } },
  { "get_style_text_color_filtered", { (const void*) &lv_obj_get_style_text_color_filtered, "lv.color", "(lv.obj)i" } },
  { "get_style_text_decor", { (const void*) &lv_obj_get_style_text_decor, "i", "(lv.obj)i" } },
  { "get_style_text_font", { (const void*) &lv_obj_get_style_text_font, "lv.font", "(lv.obj)i" } },
  { "get_style_text_letter_space", { (const void*) &lv_obj_get_style_text_letter_space, "i", "(lv.obj)i" } },
  { "get_style_text_line_space", { (const void*) &lv_obj_get_style_text_line_space, "i", "(lv.obj)i" } },
  { "get_style_text_opa", { (const void*) &lv_obj_get_style_text_opa, "i", "(lv.obj)i" } },
  { "get_style_transform_angle", { (const void*) &lv_obj_get_style_transform_rotation, "i", "(lv.obj)i" } },
  { "get_style_transform_height", { (const void*) &lv_obj_get_style_transform_height, "i", "(lv.obj)i" } },
  { "get_style_transform_pivot_x", { (const void*) &lv_obj_get_style_transform_pivot_x, "i", "(lv.obj)i" } },
  { "get_style_transform_pivot_y", { (const void*) &lv_obj_get_style_transform_pivot_y, "i", "(lv.obj)i" } },
  { "get_style_transform_rotation", { (const void*) &lv_obj_get_style_transform_rotation, "i", "(lv.obj)i" } },
  { "get_style_transform_scale_x", { (const void*) &lv_obj_get_style_transform_scale_x, "i", "(lv.obj)i" } },
  { "get_style_transform_scale_x_safe", { (const void*) &lv_obj_get_style_transform_scale_x_safe, "i", "(lv.obj)i" } },
  { "get_style_transform_scale_y", { (const void*) &lv_obj_get_style_transform_scale_y, "i", "(lv.obj)i" } },
  { "get_style_transform_scale_y_safe", { (const void*) &lv_obj_get_style_transform_scale_y_safe, "i", "(lv.obj)i" } },
  { "get_style_transform_skew_x", { (const void*) &lv_obj_get_style_transform_skew_x, "i", "(lv.obj)i" } },
  { "get_style_transform_skew_y", { (const void*) &lv_obj_get_style_transform_skew_y, "i", "(lv.obj)i" } },
  { "get_style_transform_width", { (const void*) &lv_obj_get_style_transform_width, "i", "(lv.obj)i" } },
  { "get_style_transition", { (const void*) &lv_obj_get_style_transition, "lv.style_transition_dsc", "(lv.obj)i" } },
  { "get_style_translate_x", { (const void*) &lv_obj_get_style_translate_x, "i", "(lv.obj)i" } },
  { "get_style_translate_y", { (const void*) &lv_obj_get_style_translate_y, "i", "(lv.obj)i" } },
  { "get_style_width", { (const void*) &lv_obj_get_style_width, "i", "(lv.obj)i" } },
  { "get_style_x", { (const void*) &lv_obj_get_style_x, "i", "(lv.obj)i" } },
  { "get_style_y", { (const void*) &lv_obj_get_style_y, "i", "(lv.obj)i" } },
  { "get_transformed_area", { (const void*) &lv_obj_get_transformed_area, "", "(lv.obj)(lv.area)i" } },
  { "get_user_data", { (const void*) &lv_obj_get_user_data, "c", "(lv.obj)" } },
  { "get_width", { (const void*) &lv_obj_get_width, "i", "(lv.obj)" } },
  { "get_x", { (const void*) &lv_obj_get_x, "i", "(lv.obj)" } },
  { "get_x2", { (const void*) &lv_obj_get_x2, "i", "(lv.obj)" } },
  { "get_x_aligned", { (const void*) &lv_obj_get_x_aligned, "i", "(lv.obj)" } },
  { "get_y", { (const void*) &lv_obj_get_y, "i", "(lv.obj)" } },
  { "get_y2", { (const void*) &lv_obj_get_y2, "i", "(lv.obj)" } },
  { "get_y_aligned", { (const void*) &lv_obj_get_y_aligned, "i", "(lv.obj)" } },
  { "has_class", { (const void*) &lv_obj_has_class, "b", "(lv.obj)(lv.obj_class)" } },
  { "has_flag", { (const void*) &lv_obj_has_flag, "b", "(lv.obj)i" } },
  { "has_flag_any", { (const void*) &lv_obj_has_flag_any, "b", "(lv.obj)i" } },
  { "has_state", { (const void*) &lv_obj_has_state, "b", "(lv.obj)i" } },
  { "has_style_prop", { (const void*) &lv_obj_has_style_prop, "b", "(lv.obj)ii" } },
  { "hit_test", { (const void*) &lv_obj_hit_test, "b", "(lv.obj)c" } },
  { "init_draw_arc_dsc", { (const void*) &lv_obj_init_draw_arc_dsc, "", "(lv.obj)i(lv.draw_arc_dsc)" } },
  { "init_draw_image_dsc", { (const void*) &lv_obj_init_draw_image_dsc, "", "(lv.obj)i(lv.draw_image_dsc)" } },
  { "init_draw_label_dsc", { (const void*) &lv_obj_init_draw_label_dsc, "", "(lv.obj)i(lv.draw_label_dsc)" } },
  { "init_draw_line_dsc", { (const void*) &lv_obj_init_draw_line_dsc, "", "(lv.obj)i(lv.draw_line_dsc)" } },
  { "init_draw_rect_dsc", { (const void*) &lv_obj_init_draw_rect_dsc, "", "(lv.obj)i(lv.draw_rect_dsc)" } },
  { "invalidate", { (const void*) &lv_obj_invalidate, "", "(lv.obj)" } },
  { "invalidate_area", { (const void*) &lv_obj_invalidate_area, "", "(lv.obj)(lv.area)" } },
  { "is_editable", { (const void*) &lv_obj_is_editable, "b", "(lv.obj)" } },
  { "is_group_def", { (const void*) &lv_obj_is_group_def, "b", "(lv.obj)" } },
  { "is_layout_positioned", { (const void*) &lv_obj_is_layout_positioned, "b", "(lv.obj)" } },
  { "is_scrolling", { (const void*) &lv_obj_is_scrolling, "b", "(lv.obj)" } },
  { "is_valid", { (const void*) &lv_obj_is_valid, "b", "(lv.obj)" } },
  { "is_visible", { (const void*) &lv_obj_is_visible, "b", "(lv.obj)" } },
  { "mark_layout_as_dirty", { (const void*) &lv_obj_mark_layout_as_dirty, "", "(lv.obj)" } },
  { "move_background", { (const void*) &lv_obj_move_background, "", "(lv.obj)" } },
  { "move_children_by", { (const void*) &lv_obj_move_children_by, "", "(lv.obj)iib" } },
  { "move_foreground", { (const void*) &lv_obj_move_foreground, "", "(lv.obj)" } },
  { "move_to", { (const void*) &lv_obj_move_to, "", "(lv.obj)ii" } },
  { "move_to_index", { (const void*) &lv_obj_move_to_index, "", "(lv.obj)i" } },
  { "readjust_scroll", { (const void*) &lv_obj_readjust_scroll, "", "(lv.obj)i" } },
  { "refr_pos", { (const void*) &lv_obj_refr_pos, "", "(lv.obj)" } },
  { "refr_size", { (const void*) &lv_obj_refr_size, "b", "(lv.obj)" } },
  { "refresh_ext_draw_size", { (const void*) &lv_obj_refresh_ext_draw_size, "", "(lv.obj)" } },
  { "refresh_self_size", { (const void*) &lv_obj_refresh_self_size, "b", "(lv.obj)" } },
  { "refresh_style", { (const void*) &lv_obj_refresh_style, "", "(lv.obj)ii" } },
  { "remove", { (const void*) &lv_obj_delete, "", "(lv.obj)" } },
  { "remove_event", { (const void*) &lv_obj_remove_event, "b", "(lv.obj)i" } },
  { "remove_event_cb", { (const void*) &lv_obj_remove_event_cb, "b", "(lv.obj)." } },
  { "remove_event_cb_with_user_data", { (const void*) &lv_obj_remove_event_cb_with_user_data, "i", "(lv.obj).." } },
  { "remove_event_dsc", { (const void*) &lv_obj_remove_event_dsc, "b", "(lv.obj)(lv.event_dsc)" } },
  { "remove_flag", { (const void*) &lv_obj_remove_flag, "", "(lv.obj)i" } },
  { "remove_local_style_prop", { (const void*) &lv_obj_remove_local_style_prop, "b", "(lv.obj)ii" } },
  { "remove_state", { (const void*) &lv_obj_remove_state, "", "(lv.obj)i" } },
  { "remove_style", { (const void*) &lv_obj_remove_style, "", "(lv.obj)(lv.style)i" } },
  { "remove_style_all", { (const void*) &lv_obj_remove_style_all, "", "(lv.obj)" } },
  { "replace_style", { (const void*) &lv_obj_replace_style, "b", "(lv.obj)(lv.style)(lv.style)i" } },
  { "scroll_by", { (const void*) &lv_obj_scroll_by, "", "(lv.obj)iii" } },
  { "scroll_by_bounded", { (const void*) &lv_obj_scroll_by_bounded, "", "(lv.obj)iii" } },
  { "scroll_to", { (const void*) &lv_obj_scroll_to, "", "(lv.obj)iii" } },
  { "scroll_to_view", { (const void*) &lv_obj_scroll_to_view, "", "(lv.obj)i" } },
  { "scroll_to_view_recursive", { (const void*) &lv_obj_scroll_to_view_recursive, "", "(lv.obj)i" } },
  { "scroll_to_x", { (const void*) &lv_obj_scroll_to_x, "", "(lv.obj)ii" } },
  { "scroll_to_y", { (const void*) &lv_obj_scroll_to_y, "", "(lv.obj)ii" } },
  { "scrollbar_invalidate", { (const void*) &lv_obj_scrollbar_invalidate, "", "(lv.obj)" } },
  { "send_event", { (const void*) &lv_obj_send_event, "i", "(lv.obj)i." } },
  { "set_align", { (const void*) &lv_obj_set_align, "", "(lv.obj)i" } },
  { "set_content_height", { (const void*) &lv_obj_set_content_height, "", "(lv.obj)i" } },
  { "set_content_width", { (const void*) &lv_obj_set_content_width, "", "(lv.obj)i" } },
  { "set_ext_click_area", { (const void*) &lv_obj_set_ext_click_area, "", "(lv.obj)i" } },
  { "set_flex_align", { (const void*) &lv_obj_set_flex_align, "", "(lv.obj)iii" } },
  { "set_flex_flow", { (const void*) &lv_obj_set_flex_flow, "", "(lv.obj)i" } },
  { "set_flex_grow", { (const void*) &lv_obj_set_flex_grow, "", "(lv.obj)i" } },
  { "set_grid_align", { (const void*) &lv_obj_set_grid_align, "", "(lv.obj)ii" } },
  { "set_grid_cell", { (const void*) &lv_obj_set_grid_cell, "", "(lv.obj)iiiiii" } },
  { "set_grid_dsc_array", { (const void*) &lv_obj_set_grid_dsc_array, "", "(lv.obj)(lv.int_arr)(lv.int_arr)" } },
  { "set_height", { (const void*) &lv_obj_set_height, "", "(lv.obj)i" } },
  { "set_layout", { (const void*) &lv_obj_set_layout, "", "(lv.obj)i" } },
  { "set_local_style_prop", { (const void*) &lv_obj_set_local_style_prop, "", "(lv.obj)iii" } },
  { "set_parent", { (const void*) &lv_obj_set_parent, "", "(lv.obj)(lv.obj)" } },
  { "set_pos", { (const void*) &lv_obj_set_pos, "", "(lv.obj)ii" } },
  { "set_scroll_dir", { (const void*) &lv_obj_set_scroll_dir, "", "(lv.obj)i" } },
  { "set_scroll_snap_x", { (const void*) &lv_obj_set_scroll_snap_x, "", "(lv.obj)i" } },
  { "set_scroll_snap_y", { (const void*) &lv_obj_set_scroll_snap_y, "", "(lv.obj)i" } },
  { "set_scrollbar_mode", { (const void*) &lv_obj_set_scrollbar_mode, "", "(lv.obj)i" } },
  { "set_size", { (const void*) &lv_obj_set_size, "", "(lv.obj)ii" } },
  { "set_state", { (const void*) &lv_obj_set_state, "", "(lv.obj)ib" } },
  { "set_style_align", { (const void*) &lv_obj_set_style_align, "", "(lv.obj)ii" } },
  { "set_style_anim", { (const void*) &lv_obj_set_style_anim, "", "(lv.obj)(lv.anim)i" } },
  { "set_style_anim_duration", { (const void*) &lv_obj_set_style_anim_duration, "", "(lv.obj)ii" } },
  { "set_style_anim_time", { (const void*) &lv_obj_set_style_anim_duration, "", "(lv.obj)ii" } },
  { "set_style_arc_color", { (const void*) &lv_obj_set_style_arc_color, "", "(lv.obj)(lv.color)i" } },
  { "set_style_arc_image_src", { (const void*) &lv_obj_set_style_arc_image_src, "", "(lv.obj).i" } },
  { "set_style_arc_opa", { (const void*) &lv_obj_set_style_arc_opa, "", "(lv.obj)ii" } },
  { "set_style_arc_rounded", { (const void*) &lv_obj_set_style_arc_rounded, "", "(lv.obj)bi" } },
  { "set_style_arc_width", { (const void*) &lv_obj_set_style_arc_width, "", "(lv.obj)ii" } },
  { "set_style_base_dir", { (const void*) &lv_obj_set_style_base_dir, "", "(lv.obj)ii" } },
  { "set_style_bg_color", { (const void*) &lv_obj_set_style_bg_color, "", "(lv.obj)(lv.color)i" } },
  { "set_style_bg_grad", { (const void*) &lv_obj_set_style_bg_grad, "", "(lv.obj)(lv.grad_dsc)i" } },
  { "set_style_bg_grad_color", { (const void*) &lv_obj_set_style_bg_grad_color, "", "(lv.obj)(lv.color)i" } },
  { "set_style_bg_grad_dir", { (const void*) &lv_obj_set_style_bg_grad_dir, "", "(lv.obj)ii" } },
  { "set_style_bg_grad_opa", { (const void*) &lv_obj_set_style_bg_grad_opa, "", "(lv.obj)ii" } },
  { "set_style_bg_grad_stop", { (const void*) &lv_obj_set_style_bg_grad_stop, "", "(lv.obj)ii" } },
  { "set_style_bg_image_opa", { (const void*) &lv_obj_set_style_bg_image_opa, "", "(lv.obj)ii" } },
  { "set_style_bg_image_recolor", { (const void*) &lv_obj_set_style_bg_image_recolor, "", "(lv.obj)(lv.color)i" } },
  { "set_style_bg_image_recolor_opa", { (const void*) &lv_obj_set_style_bg_image_recolor_opa, "", "(lv.obj)ii" } },
  { "set_style_bg_image_src", { (const void*) &lv_obj_set_style_bg_image_src, "", "(lv.obj).i" } },
  { "set_style_bg_image_tiled", { (const void*) &lv_obj_set_style_bg_image_tiled, "", "(lv.obj)bi" } },
  { "set_style_bg_main_opa", { (const void*) &lv_obj_set_style_bg_main_opa, "", "(lv.obj)ii" } },
  { "set_style_bg_main_stop", { (const void*) &lv_obj_set_style_bg_main_stop, "", "(lv.obj)ii" } },
  { "set_style_bg_opa", { (const void*) &lv_obj_set_style_bg_opa, "", "(lv.obj)ii" } },
  { "set_style_bitmap_mask_src", { (const void*) &lv_obj_set_style_bitmap_mask_src, "", "(lv.obj)(lv.image_dsc)i" } },
  { "set_style_blend_mode", { (const void*) &lv_obj_set_style_blend_mode, "", "(lv.obj)ii" } },
  { "set_style_border_color", { (const void*) &lv_obj_set_style_border_color, "", "(lv.obj)(lv.color)i" } },
  { "set_style_border_opa", { (const void*) &lv_obj_set_style_border_opa, "", "(lv.obj)ii" } },
  { "set_style_border_post", { (const void*) &lv_obj_set_style_border_post, "", "(lv.obj)bi" } },
  { "set_style_border_side", { (const void*) &lv_obj_set_style_border_side, "", "(lv.obj)ii" } },
  { "set_style_border_width", { (const void*) &lv_obj_set_style_border_width, "", "(lv.obj)ii" } },
  { "set_style_clip_corner", { (const void*) &lv_obj_set_style_clip_corner, "", "(lv.obj)bi" } },
  { "set_style_color_filter_dsc", { (const void*) &lv_obj_set_style_color_filter_dsc, "", "(lv.obj)(lv.color_filter_dsc)i" } },
  { "set_style_color_filter_opa", { (const void*) &lv_obj_set_style_color_filter_opa, "", "(lv.obj)ii" } },
  { "set_style_flex_cross_place", { (const void*) &lv_obj_set_style_flex_cross_place, "", "(lv.obj)ii" } },
  { "set_style_flex_flow", { (const void*) &lv_obj_set_style_flex_flow, "", "(lv.obj)ii" } },
  { "set_style_flex_grow", { (const void*) &lv_obj_set_style_flex_grow, "", "(lv.obj)ii" } },
  { "set_style_flex_main_place", { (const void*) &lv_obj_set_style_flex_main_place, "", "(lv.obj)ii" } },
  { "set_style_flex_track_place", { (const void*) &lv_obj_set_style_flex_track_place, "", "(lv.obj)ii" } },
  { "set_style_grid_cell_column_pos", { (const void*) &lv_obj_set_style_grid_cell_column_pos, "", "(lv.obj)ii" } },
  { "set_style_grid_cell_column_span", { (const void*) &lv_obj_set_style_grid_cell_column_span, "", "(lv.obj)ii" } },
  { "set_style_grid_cell_row_pos", { (const void*) &lv_obj_set_style_grid_cell_row_pos, "", "(lv.obj)ii" } },
  { "set_style_grid_cell_row_span", { (const void*) &lv_obj_set_style_grid_cell_row_span, "", "(lv.obj)ii" } },
  { "set_style_grid_cell_x_align", { (const void*) &lv_obj_set_style_grid_cell_x_align, "", "(lv.obj)ii" } },
  { "set_style_grid_cell_y_align", { (const void*) &lv_obj_set_style_grid_cell_y_align, "", "(lv.obj)ii" } },
  { "set_style_grid_column_align", { (const void*) &lv_obj_set_style_grid_column_align, "", "(lv.obj)ii" } },
  { "set_style_grid_column_dsc_array", { (const void*) &lv_obj_set_style_grid_column_dsc_array, "", "(lv.obj)(lv.int_arr)i" } },
  { "set_style_grid_row_align", { (const void*) &lv_obj_set_style_grid_row_align, "", "(lv.obj)ii" } },
  { "set_style_grid_row_dsc_array", { (const void*) &lv_obj_set_style_grid_row_dsc_array, "", "(lv.obj)(lv.int_arr)i" } },
  { "set_style_height", { (const void*) &lv_obj_set_style_height, "", "(lv.obj)ii" } },
  { "set_style_image_opa", { (const void*) &lv_obj_set_style_image_opa, "", "(lv.obj)ii" } },
  { "set_style_image_recolor", { (const void*) &lv_obj_set_style_image_recolor, "", "(lv.obj)(lv.color)i" } },
  { "set_style_image_recolor_opa", { (const void*) &lv_obj_set_style_image_recolor_opa, "", "(lv.obj)ii" } },
  { "set_style_img_opa", { (const void*) &lv_obj_set_style_image_opa, "", "(lv.obj)ii" } },
  { "set_style_img_recolor", { (const void*) &lv_obj_set_style_image_recolor, "", "(lv.obj)(lv.color)i" } },
  { "set_style_img_recolor_opa", { (const void*) &lv_obj_set_style_image_recolor_opa, "", "(lv.obj)ii" } },
  { "set_style_layout", { (const void*) &lv_obj_set_style_layout, "", "(lv.obj)ii" } },
  { "set_style_length", { (const void*) &lv_obj_set_style_length, "", "(lv.obj)ii" } },
  { "set_style_line_color", { (const void*) &lv_obj_set_style_line_color, "", "(lv.obj)(lv.color)i" } },
  { "set_style_line_dash_gap", { (const void*) &lv_obj_set_style_line_dash_gap, "", "(lv.obj)ii" } },
  { "set_style_line_dash_width", { (const void*) &lv_obj_set_style_line_dash_width, "", "(lv.obj)ii" } },
  { "set_style_line_opa", { (const void*) &lv_obj_set_style_line_opa, "", "(lv.obj)ii" } },
  { "set_style_line_rounded", { (const void*) &lv_obj_set_style_line_rounded, "", "(lv.obj)bi" } },
  { "set_style_line_width", { (const void*) &lv_obj_set_style_line_width, "", "(lv.obj)ii" } },
  { "set_style_margin_all", { (const void*) &lv_obj_set_style_margin_all, "", "(lv.obj)ii" } },
  { "set_style_margin_bottom", { (const void*) &lv_obj_set_style_margin_bottom, "", "(lv.obj)ii" } },
  { "set_style_margin_hor", { (const void*) &lv_obj_set_style_margin_hor, "", "(lv.obj)ii" } },
  { "set_style_margin_left", { (const void*) &lv_obj_set_style_margin_left, "", "(lv.obj)ii" } },
  { "set_style_margin_right", { (const void*) &lv_obj_set_style_margin_right, "", "(lv.obj)ii" } },
  { "set_style_margin_top", { (const void*) &lv_obj_set_style_margin_top, "", "(lv.obj)ii" } },
  { "set_style_margin_ver", { (const void*) &lv_obj_set_style_margin_ver, "", "(lv.obj)ii" } },
  { "set_style_max_height", { (const void*) &lv_obj_set_style_max_height, "", "(lv.obj)ii" } },
  { "set_style_max_width", { (const void*) &lv_obj_set_style_max_width, "", "(lv.obj)ii" } },
  { "set_style_min_height", { (const void*) &lv_obj_set_style_min_height, "", "(lv.obj)ii" } },
  { "set_style_min_width", { (const void*) &lv_obj_set_style_min_width, "", "(lv.obj)ii" } },
  { "set_style_opa", { (const void*) &lv_obj_set_style_opa, "", "(lv.obj)ii" } },
  { "set_style_opa_layered", { (const void*) &lv_obj_set_style_opa_layered, "", "(lv.obj)ii" } },
  { "set_style_outline_color", { (const void*) &lv_obj_set_style_outline_color, "", "(lv.obj)(lv.color)i" } },
  { "set_style_outline_opa", { (const void*) &lv_obj_set_style_outline_opa, "", "(lv.obj)ii" } },
  { "set_style_outline_pad", { (const void*) &lv_obj_set_style_outline_pad, "", "(lv.obj)ii" } },
  { "set_style_outline_width", { (const void*) &lv_obj_set_style_outline_width, "", "(lv.obj)ii" } },
  { "set_style_pad_all", { (const void*) &lv_obj_set_style_pad_all, "", "(lv.obj)ii" } },
  { "set_style_pad_bottom", { (const void*) &lv_obj_set_style_pad_bottom, "", "(lv.obj)ii" } },
  { "set_style_pad_column", { (const void*) &lv_obj_set_style_pad_column, "", "(lv.obj)ii" } },
  { "set_style_pad_gap", { (const void*) &lv_obj_set_style_pad_gap, "", "(lv.obj)ii" } },
  { "set_style_pad_hor", { (const void*) &lv_obj_set_style_pad_hor, "", "(lv.obj)ii" } },
  { "set_style_pad_left", { (const void*) &lv_obj_set_style_pad_left, "", "(lv.obj)ii" } },
  { "set_style_pad_right", { (const void*) &lv_obj_set_style_pad_right, "", "(lv.obj)ii" } },
  { "set_style_pad_row", { (const void*) &lv_obj_set_style_pad_row, "", "(lv.obj)ii" } },
  { "set_style_pad_top", { (const void*) &lv_obj_set_style_pad_top, "", "(lv.obj)ii" } },
  { "set_style_pad_ver", { (const void*) &lv_obj_set_style_pad_ver, "", "(lv.obj)ii" } },
  { "set_style_radius", { (const void*) &lv_obj_set_style_radius, "", "(lv.obj)ii" } },
  { "set_style_rotary_sensitivity", { (const void*) &lv_obj_set_style_rotary_sensitivity, "", "(lv.obj)ii" } },
  { "set_style_shadow_color", { (const void*) &lv_obj_set_style_shadow_color, "", "(lv.obj)(lv.color)i" } },
  { "set_style_shadow_offset_x", { (const void*) &lv_obj_set_style_shadow_offset_x, "", "(lv.obj)ii" } },
  { "set_style_shadow_offset_y", { (const void*) &lv_obj_set_style_shadow_offset_y, "", "(lv.obj)ii" } },
  { "set_style_shadow_ofs_x", { (const void*) &lv_obj_set_style_shadow_offset_x, "", "(lv.obj)ii" } },
  { "set_style_shadow_ofs_y", { (const void*) &lv_obj_set_style_shadow_offset_y, "", "(lv.obj)ii" } },
  { "set_style_shadow_opa", { (const void*) &lv_obj_set_style_shadow_opa, "", "(lv.obj)ii" } },
  { "set_style_shadow_spread", { (const void*) &lv_obj_set_style_shadow_spread, "", "(lv.obj)ii" } },
  { "set_style_shadow_width", { (const void*) &lv_obj_set_style_shadow_width, "", "(lv.obj)ii" } },
  { "set_style_size", { (const void*) &lv_obj_set_style_size, "", "(lv.obj)iii" } },
  { "set_style_text_align", { (const void*) &lv_obj_set_style_text_align, "", "(lv.obj)ii" } },
  { "set_style_text_color", { (const void*) &lv_obj_set_style_text_color, "", "(lv.obj)(lv.color)i" } },
  { "set_style_text_decor", { (const void*) &lv_obj_set_style_text_decor, "", "(lv.obj)ii" } },
  { "set_style_text_font", { (const void*) &lv_obj_set_style_text_font, "", "(lv.obj)(lv.font)i" } },
  { "set_style_text_letter_space", { (const void*) &lv_obj_set_style_text_letter_space, "", "(lv.obj)ii" } },
  { "set_style_text_line_space", { (const void*) &lv_obj_set_style_text_line_space, "", "(lv.obj)ii" } },
  { "set_style_text_opa", { (const void*) &lv_obj_set_style_text_opa, "", "(lv.obj)ii" } },
  { "set_style_transform_angle", { (const void*) &lv_obj_set_style_transform_rotation, "", "(lv.obj)ii" } },
  { "set_style_transform_height", { (const void*) &lv_obj_set_style_transform_height, "", "(lv.obj)ii" } },
  { "set_style_transform_pivot_x", { (const void*) &lv_obj_set_style_transform_pivot_x, "", "(lv.obj)ii" } },
  { "set_style_transform_pivot_y", { (const void*) &lv_obj_set_style_transform_pivot_y, "", "(lv.obj)ii" } },
  { "set_style_transform_rotation", { (const void*) &lv_obj_set_style_transform_rotation, "", "(lv.obj)ii" } },
  { "set_style_transform_scale", { (const void*) &lv_obj_set_style_transform_scale, "", "(lv.obj)ii" } },
  { "set_style_transform_scale_x", { (const void*) &lv_obj_set_style_transform_scale_x, "", "(lv.obj)ii" } },
  { "set_style_transform_scale_y", { (const void*) &lv_obj_set_style_transform_scale_y, "", "(lv.obj)ii" } },
  { "set_style_transform_skew_x", { (const void*) &lv_obj_set_style_transform_skew_x, "", "(lv.obj)ii" } },
  { "set_style_transform_skew_y", { (const void*) &lv_obj_set_style_transform_skew_y, "", "(lv.obj)ii" } },
  { "set_style_transform_width", { (const void*) &lv_obj_set_style_transform_width, "", "(lv.obj)ii" } },
  { "set_style_transform_zoom", { (const void*) &lv_obj_set_style_transform_scale, "", "(lv.obj)ii" } },
  { "set_style_transition", { (const void*) &lv_obj_set_style_transition, "", "(lv.obj)(lv.style_transition_dsc)i" } },
  { "set_style_translate_x", { (const void*) &lv_obj_set_style_translate_x, "", "(lv.obj)ii" } },
  { "set_style_translate_y", { (const void*) &lv_obj_set_style_translate_y, "", "(lv.obj)ii" } },
  { "set_style_width", { (const void*) &lv_obj_set_style_width, "", "(lv.obj)ii" } },
  { "set_style_x", { (const void*) &lv_obj_set_style_x, "", "(lv.obj)ii" } },
  { "set_style_y", { (const void*) &lv_obj_set_style_y, "", "(lv.obj)ii" } },
  { "set_user_data", { (const void*) &lv_obj_set_user_data, "", "(lv.obj)." } },
  { "set_width", { (const void*) &lv_obj_set_width, "", "(lv.obj)i" } },
  { "set_x", { (const void*) &lv_obj_set_x, "", "(lv.obj)i" } },
  { "set_y", { (const void*) &lv_obj_set_y, "", "(lv.obj)i" } },
  { "stringify_id", { (const void*) &lv_obj_stringify_id, "s", "(lv.obj)ci" } },
  { "swap", { (const void*) &lv_obj_swap, "", "(lv.obj)(lv.obj)" } },
  { "transform_point", { (const void*) &lv_obj_transform_point, "", "(lv.obj)ci" } },
  { "transform_point_array", { (const void*) &lv_obj_transform_point_array, "", "(lv.obj)(lv.point_arr)ii" } },
  { "update_flag", { (const void*) &lv_obj_update_flag, "", "(lv.obj)ib" } },
  { "update_layout", { (const void*) &lv_obj_update_layout, "", "(lv.obj)" } },
  { "update_snap", { (const void*) &lv_obj_update_snap, "", "(lv.obj)i" } },
};
#endif // BE_LV_WIDGET_OBJ

/* `lv_event` methods */
const be_ntv_func_def_t lv_event_func[] = {
  { "get_code", { (const void*) &lv_event_get_code, "i", "(lv.event)" } },
  { "get_cover_area", { (const void*) &lv_event_get_cover_area, "lv.area", "(lv.event)" } },
  { "get_current_target", { (const void*) &lv_event_get_current_target, "c", "(lv.event)" } },
  { "get_current_target_obj", { (const void*) &lv_event_get_current_target_obj, "lv.obj", "(lv.event)" } },
  { "get_hit_test_info", { (const void*) &lv_event_get_hit_test_info, "c", "(lv.event)" } },
  { "get_indev", { (const void*) &lv_event_get_indev, "lv.indev", "(lv.event)" } },
  { "get_key", { (const void*) &lv_event_get_key, "i", "(lv.event)" } },
  { "get_layer", { (const void*) &lv_event_get_layer, "lv.layer", "(lv.event)" } },
  { "get_old_size", { (const void*) &lv_event_get_old_size, "lv.area", "(lv.event)" } },
  { "get_param", { (const void*) &lv_event_get_param, "c", "(lv.event)" } },
  { "get_rotary_diff", { (const void*) &lv_event_get_rotary_diff, "i", "(lv.event)" } },
  { "get_scroll_anim", { (const void*) &lv_event_get_scroll_anim, "lv.anim", "(lv.event)" } },
  { "get_self_size_info", { (const void*) &lv_event_get_self_size_info, "c", "(lv.event)" } },
  { "get_target", { (const void*) &lv_event_get_target, "c", "(lv.event)" } },
  { "get_target_obj", { (const void*) &lv_event_get_target_obj, "lv.obj", "(lv.event)" } },
  { "get_user_data", { (const void*) &lv_event_get_user_data, "c", "(lv.event)" } },
  { "set_cover_res", { (const void*) &lv_event_set_cover_res, "", "(lv.event)i" } },
  { "set_ext_draw_size", { (const void*) &lv_event_set_ext_draw_size, "", "(lv.event)i" } },
  { "stop_bubbling", { (const void*) &lv_event_stop_bubbling, "", "(lv.event)" } },
  { "stop_processing", { (const void*) &lv_event_stop_processing, "", "(lv.event)" } },
};

/* `lv_display` methods */
const be_ntv_func_def_t lv_display_func[] = {
  { "add_event_cb", { (const void*) &lv_display_add_event_cb, "", "(lv.display)^lv_event_cb^i." } },
  { "del", { (const void*) &lv_display_delete, "", "(lv.display)" } },
  { "delete", { (const void*) &lv_display_delete, "", "(lv.display)" } },
  { "delete_event", { (const void*) &lv_display_delete_event, "b", "(lv.display)i" } },
  { "delete_refr_timer", { (const void*) &lv_display_delete_refr_timer, "", "(lv.display)" } },
  { "dpx", { (const void*) &lv_display_dpx, "i", "(lv.display)i" } },
  { "enable_invalidation", { (const void*) &lv_display_enable_invalidation, "", "(lv.display)b" } },
  { "get_angle", { (const void*) &lv_display_get_rotation, "i", "(lv.display)" } },
  { "get_antialiasing", { (const void*) &lv_display_get_antialiasing, "b", "(lv.display)" } },
  { "get_color_format", { (const void*) &lv_display_get_color_format, "i", "(lv.display)" } },
  { "get_dpi", { (const void*) &lv_display_get_dpi, "i", "(lv.display)" } },
  { "get_driver_data", { (const void*) &lv_display_get_driver_data, "c", "(lv.display)" } },
  { "get_event_count", { (const void*) &lv_display_get_event_count, "i", "(lv.display)" } },
  { "get_event_dsc", { (const void*) &lv_display_get_event_dsc, "lv.event_dsc", "(lv.display)i" } },
  { "get_hor_res", { (const void*) &lv_display_get_horizontal_resolution, "i", "(lv.display)" } },
  { "get_horizontal_resolution", { (const void*) &lv_display_get_horizontal_resolution, "i", "(lv.display)" } },
  { "get_inactive_time", { (const void*) &lv_display_get_inactive_time, "i", "(lv.display)" } },
  { "get_layer_bottom", { (const void*) &lv_display_get_layer_bottom, "lv.obj", "(lv.display)" } },
  { "get_layer_sys", { (const void*) &lv_display_get_layer_sys, "lv.obj", "(lv.display)" } },
  { "get_layer_top", { (const void*) &lv_display_get_layer_top, "lv.obj", "(lv.display)" } },
  { "get_next", { (const void*) &lv_display_get_next, "lv.display", "(lv.display)" } },
  { "get_offset_x", { (const void*) &lv_display_get_offset_x, "i", "(lv.display)" } },
  { "get_offset_y", { (const void*) &lv_display_get_offset_y, "i", "(lv.display)" } },
  { "get_physical_hor_res", { (const void*) &lv_display_get_physical_horizontal_resolution, "i", "(lv.display)" } },
  { "get_physical_horizontal_resolution", { (const void*) &lv_display_get_physical_horizontal_resolution, "i", "(lv.display)" } },
  { "get_physical_ver_res", { (const void*) &lv_display_get_physical_vertical_resolution, "i", "(lv.display)" } },
  { "get_physical_vertical_resolution", { (const void*) &lv_display_get_physical_vertical_resolution, "i", "(lv.display)" } },
  { "get_refr_timer", { (const void*) &lv_display_get_refr_timer, "lv.timer", "(lv.display)" } },
  { "get_rotation", { (const void*) &lv_display_get_rotation, "i", "(lv.display)" } },
  { "get_scr_act", { (const void*) &lv_display_get_screen_active, "lv.obj", "(lv.display)" } },
  { "get_scr_prev", { (const void*) &lv_display_get_screen_prev, "lv.obj", "(lv.display)" } },
  { "get_screen_active", { (const void*) &lv_display_get_screen_active, "lv.obj", "(lv.display)" } },
  { "get_screen_prev", { (const void*) &lv_display_get_screen_prev, "lv.obj", "(lv.display)" } },
  { "get_theme", { (const void*) &lv_display_get_theme, "lv.theme", "(lv.display)" } },
  { "get_user_data", { (const void*) &lv_display_get_user_data, "c", "(lv.display)" } },
  { "get_ver_res", { (const void*) &lv_display_get_vertical_resolution, "i", "(lv.display)" } },
  { "get_vertical_resolution", { (const void*) &lv_display_get_vertical_resolution, "i", "(lv.display)" } },
  { "is_double_buffered", { (const void*) &lv_display_is_double_buffered, "b", "(lv.display)" } },
  { "is_invalidation_enabled", { (const void*) &lv_display_is_invalidation_enabled, "b", "(lv.display)" } },
  { "remove", { (const void*) &lv_display_delete, "", "(lv.display)" } },
  { "remove_event_cb_with_user_data", { (const void*) &lv_display_remove_event_cb_with_user_data, "i", "(lv.display).." } },
  { "send_event", { (const void*) &lv_display_send_event, "i", "(lv.display)i." } },
  { "set_angle", { (const void*) &lv_display_set_rotation, "", "(lv.display)i" } },
  { "set_antialiasing", { (const void*) &lv_display_set_antialiasing, "", "(lv.display)b" } },
  { "set_buffers", { (const void*) &lv_display_set_buffers, "", "(lv.display)..ii" } },
  { "set_color_format", { (const void*) &lv_display_set_color_format, "", "(lv.display)i" } },
  { "set_default", { (const void*) &lv_display_set_default, "", "(lv.display)" } },
  { "set_dpi", { (const void*) &lv_display_set_dpi, "", "(lv.display)i" } },
  { "set_driver_data", { (const void*) &lv_display_set_driver_data, "", "(lv.display)." } },
  { "set_offset", { (const void*) &lv_display_set_offset, "", "(lv.display)ii" } },
  { "set_physical_resolution", { (const void*) &lv_display_set_physical_resolution, "", "(lv.display)ii" } },
  { "set_render_mode", { (const void*) &lv_display_set_render_mode, "", "(lv.display)i" } },
  { "set_resolution", { (const void*) &lv_display_set_resolution, "", "(lv.display)ii" } },
  { "set_rotation", { (const void*) &lv_display_set_rotation, "", "(lv.display)i" } },
  { "set_theme", { (const void*) &lv_display_set_theme, "", "(lv.display)(lv.theme)" } },
  { "set_user_data", { (const void*) &lv_display_set_user_data, "", "(lv.display)." } },
  { "trig_activity", { (const void*) &lv_display_trigger_activity, "", "(lv.display)" } },
  { "trigger_activity", { (const void*) &lv_display_trigger_activity, "", "(lv.display)" } },
};

/* `lv_indev` methods */
const be_ntv_func_def_t lv_indev_func[] = {
  { "add_event_cb", { (const void*) &lv_indev_add_event_cb, "", "(lv.indev)^lv_event_cb^i." } },
  { "del", { (const void*) &lv_indev_delete, "", "(lv.indev)" } },
  { "delete", { (const void*) &lv_indev_delete, "", "(lv.indev)" } },
  { "enable", { (const void*) &lv_indev_enable, "", "(lv.indev)b" } },
  { "get_disp", { (const void*) &lv_indev_get_display, "lv.display", "(lv.indev)" } },
  { "get_display", { (const void*) &lv_indev_get_display, "lv.display", "(lv.indev)" } },
  { "get_driver_data", { (const void*) &lv_indev_get_driver_data, "c", "(lv.indev)" } },
  { "get_event_count", { (const void*) &lv_indev_get_event_count, "i", "(lv.indev)" } },
  { "get_event_dsc", { (const void*) &lv_indev_get_event_dsc, "lv.event_dsc", "(lv.indev)i" } },
  { "get_gesture_dir", { (const void*) &lv_indev_get_gesture_dir, "i", "(lv.indev)" } },
  { "get_group", { (const void*) &lv_indev_get_group, "lv.group", "(lv.indev)" } },
  { "get_key", { (const void*) &lv_indev_get_key, "i", "(lv.indev)" } },
  { "get_mode", { (const void*) &lv_indev_get_mode, "i", "(lv.indev)" } },
  { "get_next", { (const void*) &lv_indev_get_next, "lv.indev", "(lv.indev)" } },
  { "get_point", { (const void*) &lv_indev_get_point, "", "(lv.indev)c" } },
  { "get_read_timer", { (const void*) &lv_indev_get_read_timer, "lv.timer", "(lv.indev)" } },
  { "get_scroll_dir", { (const void*) &lv_indev_get_scroll_dir, "i", "(lv.indev)" } },
  { "get_scroll_obj", { (const void*) &lv_indev_get_scroll_obj, "lv.obj", "(lv.indev)" } },
  { "get_state", { (const void*) &lv_indev_get_state, "i", "(lv.indev)" } },
  { "get_type", { (const void*) &lv_indev_get_type, "i", "(lv.indev)" } },
  { "get_user_data", { (const void*) &lv_indev_get_user_data, "c", "(lv.indev)" } },
  { "get_vect", { (const void*) &lv_indev_get_vect, "", "(lv.indev)c" } },
  { "read", { (const void*) &lv_indev_read, "", "(lv.indev)" } },
  { "remove", { (const void*) &lv_indev_delete, "", "(lv.indev)" } },
  { "remove_event", { (const void*) &lv_indev_remove_event, "b", "(lv.indev)i" } },
  { "remove_event_cb_with_user_data", { (const void*) &lv_indev_remove_event_cb_with_user_data, "i", "(lv.indev).." } },
  { "reset", { (const void*) &lv_indev_reset, "", "(lv.indev)(lv.obj)" } },
  { "reset_long_press", { (const void*) &lv_indev_reset_long_press, "", "(lv.indev)" } },
  { "search_obj", { (const void*) &lv_indev_search_obj, "lv.obj", "(lv.obj)c" } },
  { "send_event", { (const void*) &lv_indev_send_event, "i", "(lv.indev)i." } },
  { "set_button_points", { (const void*) &lv_indev_set_button_points, "", "(lv.indev)(lv.point_arr)" } },
  { "set_cursor", { (const void*) &lv_indev_set_cursor, "", "(lv.indev)(lv.obj)" } },
  { "set_disp", { (const void*) &lv_indev_set_display, "", "(lv.indev)(lv.display)" } },
  { "set_display", { (const void*) &lv_indev_set_display, "", "(lv.indev)(lv.display)" } },
  { "set_driver_data", { (const void*) &lv_indev_set_driver_data, "", "(lv.indev)." } },
  { "set_group", { (const void*) &lv_indev_set_group, "", "(lv.indev)(lv.group)" } },
  { "set_mode", { (const void*) &lv_indev_set_mode, "", "(lv.indev)i" } },
  { "set_type", { (const void*) &lv_indev_set_type, "", "(lv.indev)i" } },
  { "set_user_data", { (const void*) &lv_indev_set_user_data, "", "(lv.indev)." } },
  { "wait_release", { (const void*) &lv_indev_wait_release, "", "(lv.indev)" } },
};

/* `lv_qrcode` methods */
#ifdef BE_LV_WIDGET_QRCODE
const be_ntv_func_def_t lv_qrcode_func[] = {
  { "set_dark_color", { (const void*) &lv_qrcode_set_dark_color, "", "(lv.obj)(lv.color)" } },
  { "set_light_color", { (const void*) &lv_qrcode_set_light_color, "", "(lv.obj)(lv.color)" } },
  { "set_size", { (const void*) &lv_qrcode_set_size, "", "(lv.obj)i" } },
  { "update", { (const void*) &lv_qrcode_update, "i", "(lv.obj).i" } },
};
#endif // BE_LV_WIDGET_QRCODE

/* `lv_anim` methods */
const be_ntv_func_def_t lv_anim_func[] = {
  { "custom_delete", { (const void*) &lv_anim_custom_delete, "b", "(lv.anim)c" } },
  { "custom_get", { (const void*) &lv_anim_custom_get, "lv.anim", "(lv.anim)c" } },
  { "get_delay", { (const void*) &lv_anim_get_delay, "i", "(lv.anim)" } },
  { "get_playtime", { (const void*) &lv_anim_get_playtime, "i", "(lv.anim)" } },
  { "get_repeat_count", { (const void*) &lv_anim_get_repeat_count, "i", "(lv.anim)" } },
  { "get_time", { (const void*) &lv_anim_get_time, "i", "(lv.anim)" } },
  { "get_user_data", { (const void*) &lv_anim_get_user_data, "c", "(lv.anim)" } },
  { "init", { (const void*) &lv_anim_init, "", "(lv.anim)" } },
  { "set_bezier3_param", { (const void*) &lv_anim_set_bezier3_param, "", "(lv.anim)iiii" } },
  { "set_completed_cb", { (const void*) &lv_anim_set_completed_cb, "", "(lv.anim)c" } },
  { "set_custom_exec_cb", { (const void*) &lv_anim_set_custom_exec_cb, "", "(lv.anim)c" } },
  { "set_delay", { (const void*) &lv_anim_set_delay, "", "(lv.anim)i" } },
  { "set_duration", { (const void*) &lv_anim_set_duration, "", "(lv.anim)i" } },
  { "set_early_apply", { (const void*) &lv_anim_set_early_apply, "", "(lv.anim)b" } },
  { "set_exec_cb", { (const void*) &lv_anim_set_exec_cb, "", "(lv.anim)c" } },
  { "set_get_value_cb", { (const void*) &lv_anim_set_get_value_cb, "", "(lv.anim)c" } },
  { "set_path_cb", { (const void*) &lv_anim_set_path_cb, "", "(lv.anim)c" } },
  { "set_playback_delay", { (const void*) &lv_anim_set_playback_delay, "", "(lv.anim)i" } },
  { "set_playback_duration", { (const void*) &lv_anim_set_playback_duration, "", "(lv.anim)i" } },
  { "set_playback_time", { (const void*) &lv_anim_set_playback_time, "", "(lv.anim)i" } },
  { "set_ready_cb", { (const void*) &lv_anim_set_completed_cb, "", "(lv.anim)c" } },
  { "set_repeat_count", { (const void*) &lv_anim_set_repeat_count, "", "(lv.anim)i" } },
  { "set_repeat_delay", { (const void*) &lv_anim_set_repeat_delay, "", "(lv.anim)i" } },
  { "set_start_cb", { (const void*) &lv_anim_set_start_cb, "", "(lv.anim)c" } },
  { "set_time", { (const void*) &lv_anim_set_time, "", "(lv.anim)i" } },
  { "set_user_data", { (const void*) &lv_anim_set_user_data, "", "(lv.anim)." } },
  { "set_values", { (const void*) &lv_anim_set_values, "", "(lv.anim)ii" } },
  { "set_var", { (const void*) &lv_anim_set_var, "", "(lv.anim)." } },
  { "start", { (const void*) &lv_anim_start, "lv.anim", "(lv.anim)" } },
};

/* `lv_timer` methods */
const be_ntv_func_def_t lv_timer_func[] = {
  { "del", { (const void*) &lv_timer_delete, "", "(lv.timer)" } },
  { "delete", { (const void*) &lv_timer_delete, "", "(lv.timer)" } },
  { "get_next", { (const void*) &lv_timer_get_next, "lv.timer", "(lv.timer)" } },
  { "get_paused", { (const void*) &lv_timer_get_paused, "b", "(lv.timer)" } },
  { "get_user_data", { (const void*) &lv_timer_get_user_data, "c", "(lv.timer)" } },
  { "pause", { (const void*) &lv_timer_pause, "", "(lv.timer)" } },
  { "ready", { (const void*) &lv_timer_ready, "", "(lv.timer)" } },
  { "remove", { (const void*) &lv_timer_delete, "", "(lv.timer)" } },
  { "reset", { (const void*) &lv_timer_reset, "", "(lv.timer)" } },
  { "resume", { (const void*) &lv_timer_resume, "", "(lv.timer)" } },
  { "set_auto_delete", { (const void*) &lv_timer_set_auto_delete, "", "(lv.timer)b" } },
  { "set_cb", { (const void*) &lv_timer_set_cb, "", "(lv.timer)^lv_timer_cb^" } },
  { "set_period", { (const void*) &lv_timer_set_period, "", "(lv.timer)i" } },
  { "set_repeat_count", { (const void*) &lv_timer_set_repeat_count, "", "(lv.timer)i" } },
  { "set_user_data", { (const void*) &lv_timer_set_user_data, "", "(lv.timer)." } },
};

/* `lv_animimg` methods */
#ifdef BE_LV_WIDGET_ANIMIMG
const be_ntv_func_def_t lv_animimg_func[] = {
  { "get_duration", { (const void*) &lv_animimg_get_duration, "i", "(lv.obj)" } },
  { "get_repeat_count", { (const void*) &lv_animimg_get_repeat_count, "i", "(lv.obj)" } },
  { "get_src_count", { (const void*) &lv_animimg_get_src_count, "i", "(lv.obj)" } },
  { "set_duration", { (const void*) &lv_animimg_set_duration, "", "(lv.obj)i" } },
  { "set_repeat_count", { (const void*) &lv_animimg_set_repeat_count, "", "(lv.obj)i" } },
  { "set_src", { (const void*) &lv_animimg_set_src, "", "(lv.obj)ci" } },
  { "start", { (const void*) &lv_animimg_start, "", "(lv.obj)" } },
};
#endif // BE_LV_WIDGET_ANIMIMG

/* `lv_arc` methods */
#ifdef BE_LV_WIDGET_ARC
const be_ntv_func_def_t lv_arc_func[] = {
  { "align_obj_to_angle", { (const void*) &lv_arc_align_obj_to_angle, "", "(lv.obj)(lv.obj)i" } },
  { "get_angle", { (const void*) &lv_arc_get_rotation, "i", "(lv.obj)" } },
  { "get_angle_end", { (const void*) &lv_arc_get_angle_end, "i", "(lv.obj)" } },
  { "get_angle_start", { (const void*) &lv_arc_get_angle_start, "i", "(lv.obj)" } },
  { "get_bg_angle_end", { (const void*) &lv_arc_get_bg_angle_end, "i", "(lv.obj)" } },
  { "get_bg_angle_start", { (const void*) &lv_arc_get_bg_angle_start, "i", "(lv.obj)" } },
  { "get_knob_offset", { (const void*) &lv_arc_get_knob_offset, "i", "(lv.obj)" } },
  { "get_max_value", { (const void*) &lv_arc_get_max_value, "i", "(lv.obj)" } },
  { "get_min_value", { (const void*) &lv_arc_get_min_value, "i", "(lv.obj)" } },
  { "get_mode", { (const void*) &lv_arc_get_mode, "i", "(lv.obj)" } },
  { "get_rotation", { (const void*) &lv_arc_get_rotation, "i", "(lv.obj)" } },
  { "get_value", { (const void*) &lv_arc_get_value, "i", "(lv.obj)" } },
  { "rotate_obj_to_angle", { (const void*) &lv_arc_rotate_obj_to_angle, "", "(lv.obj)(lv.obj)i" } },
  { "set_angle", { (const void*) &lv_arc_set_rotation, "", "(lv.obj)i" } },
  { "set_angles", { (const void*) &lv_arc_set_angles, "", "(lv.obj)ii" } },
  { "set_bg_angles", { (const void*) &lv_arc_set_bg_angles, "", "(lv.obj)ii" } },
  { "set_bg_end_angle", { (const void*) &lv_arc_set_bg_end_angle, "", "(lv.obj)i" } },
  { "set_bg_start_angle", { (const void*) &lv_arc_set_bg_start_angle, "", "(lv.obj)i" } },
  { "set_change_rate", { (const void*) &lv_arc_set_change_rate, "", "(lv.obj)i" } },
  { "set_end_angle", { (const void*) &lv_arc_set_end_angle, "", "(lv.obj)i" } },
  { "set_knob_offset", { (const void*) &lv_arc_set_knob_offset, "", "(lv.obj)i" } },
  { "set_mode", { (const void*) &lv_arc_set_mode, "", "(lv.obj)i" } },
  { "set_range", { (const void*) &lv_arc_set_range, "", "(lv.obj)ii" } },
  { "set_rotation", { (const void*) &lv_arc_set_rotation, "", "(lv.obj)i" } },
  { "set_start_angle", { (const void*) &lv_arc_set_start_angle, "", "(lv.obj)i" } },
  { "set_value", { (const void*) &lv_arc_set_value, "", "(lv.obj)i" } },
};
#endif // BE_LV_WIDGET_ARC

/* `lv_bar` methods */
#ifdef BE_LV_WIDGET_BAR
const be_ntv_func_def_t lv_bar_func[] = {
  { "get_indic_area", { (const void*) &lv_bar_get_indic_area, "lv.area", "(lv.obj)" } },
  { "get_max_value", { (const void*) &lv_bar_get_max_value, "i", "(lv.obj)" } },
  { "get_min_value", { (const void*) &lv_bar_get_min_value, "i", "(lv.obj)" } },
  { "get_mode", { (const void*) &lv_bar_get_mode, "i", "(lv.obj)" } },
  { "get_start_value", { (const void*) &lv_bar_get_start_value, "i", "(lv.obj)" } },
  { "get_value", { (const void*) &lv_bar_get_value, "i", "(lv.obj)" } },
  { "is_symmetrical", { (const void*) &lv_bar_is_symmetrical, "b", "(lv.obj)" } },
  { "set_mode", { (const void*) &lv_bar_set_mode, "", "(lv.obj)i" } },
  { "set_range", { (const void*) &lv_bar_set_range, "", "(lv.obj)ii" } },
  { "set_start_value", { (const void*) &lv_bar_set_start_value, "", "(lv.obj)ii" } },
  { "set_value", { (const void*) &lv_bar_set_value, "", "(lv.obj)ii" } },
};
#endif // BE_LV_WIDGET_BAR

/* `lv_button` methods */
#ifdef BE_LV_WIDGET_BUTTON
const be_ntv_func_def_t lv_button_func[] = {
};
#endif // BE_LV_WIDGET_BUTTON

/* `lv_buttonmatrix` methods */
#ifdef BE_LV_WIDGET_BUTTONMATRIX
const be_ntv_func_def_t lv_buttonmatrix_func[] = {
  { "_btn_text", { (const void*) &lv_buttonmatrix_get_button_text, "s", "(lv.obj)i" } },
  { "clear_button_ctrl", { (const void*) &lv_buttonmatrix_clear_button_ctrl, "", "(lv.obj)ii" } },
  { "clear_button_ctrl_all", { (const void*) &lv_buttonmatrix_clear_button_ctrl_all, "", "(lv.obj)i" } },
  { "get_button_text", { (const void*) &lv_buttonmatrix_get_button_text, "s", "(lv.obj)i" } },
  { "get_map", { (const void*) &lv_buttonmatrix_get_map, "c", "(lv.obj)" } },
  { "get_one_checked", { (const void*) &lv_buttonmatrix_get_one_checked, "b", "(lv.obj)" } },
  { "get_popovers", { (const void*) &lv_buttonmatrix_get_popovers, "b", "(lv.obj)" } },
  { "get_selected_button", { (const void*) &lv_buttonmatrix_get_selected_button, "i", "(lv.obj)" } },
  { "has_button_ctrl", { (const void*) &lv_buttonmatrix_has_button_ctrl, "b", "(lv.obj)ii" } },
  { "set_button_ctrl", { (const void*) &lv_buttonmatrix_set_button_ctrl, "", "(lv.obj)ii" } },
  { "set_button_ctrl_all", { (const void*) &lv_buttonmatrix_set_button_ctrl_all, "", "(lv.obj)i" } },
  { "set_button_width", { (const void*) &lv_buttonmatrix_set_button_width, "", "(lv.obj)ii" } },
  { "set_map", { (const void*) &lv_buttonmatrix_set_map, "", "(lv.obj)(lv.str_arr)" } },
  { "set_one_checked", { (const void*) &lv_buttonmatrix_set_one_checked, "", "(lv.obj)b" } },
  { "set_selected_button", { (const void*) &lv_buttonmatrix_set_selected_button, "", "(lv.obj)i" } },
};
#endif // BE_LV_WIDGET_BUTTONMATRIX

/* `lv_calendar` methods */
#ifdef BE_LV_WIDGET_CALENDAR
const be_ntv_func_def_t lv_calendar_func[] = {
  { "get_btnmatrix", { (const void*) &lv_calendar_get_btnmatrix, "lv.obj", "(lv.obj)" } },
  { "get_highlighted_dates_num", { (const void*) &lv_calendar_get_highlighted_dates_num, "i", "(lv.obj)" } },
  { "header_dropdown_set_year_list", { (const void*) &lv_calendar_header_dropdown_set_year_list, "", "(lv.obj)s" } },
  { "set_day_names", { (const void*) &lv_calendar_set_day_names, "", "(lv.obj)c" } },
  { "set_showed_date", { (const void*) &lv_calendar_set_showed_date, "", "(lv.obj)ii" } },
  { "set_today_date", { (const void*) &lv_calendar_set_today_date, "", "(lv.obj)iii" } },
};
#endif // BE_LV_WIDGET_CALENDAR

/* `lv_canvas` methods */
#ifdef BE_LV_WIDGET_CANVAS
const be_ntv_func_def_t lv_canvas_func[] = {
  { "fill_bg", { (const void*) &lv_canvas_fill_bg, "", "(lv.obj)(lv.color)i" } },
  { "finish_layer", { (const void*) &lv_canvas_finish_layer, "", "(lv.obj)(lv.layer)" } },
  { "get_buf", { (const void*) &lv_canvas_get_buf, "c", "(lv.obj)" } },
  { "get_image", { (const void*) &lv_canvas_get_image, "lv.image_dsc", "(lv.obj)" } },
  { "get_px", { (const void*) &lv_canvas_get_px, "i", "(lv.obj)ii" } },
  { "init_layer", { (const void*) &lv_canvas_init_layer, "", "(lv.obj)(lv.layer)" } },
  { "set_buffer", { (const void*) &lv_canvas_set_buffer, "", "(lv.obj).iii" } },
  { "set_palette", { (const void*) &lv_canvas_set_palette, "", "(lv.obj)ii" } },
  { "set_px", { (const void*) &lv_canvas_set_px, "", "(lv.obj)ii(lv.color)i" } },
};
#endif // BE_LV_WIDGET_CANVAS

/* `lv_chart` methods */
#ifdef BE_LV_WIDGET_CHART
const be_ntv_func_def_t lv_chart_func[] = {
  { "add_cursor", { (const void*) &lv_chart_add_cursor, "lv.chart_cursor", "(lv.obj)(lv.color)i" } },
  { "add_series", { (const void*) &lv_chart_add_series, "lv.chart_series", "(lv.obj)(lv.color)i" } },
  { "get_cursor_point", { (const void*) &lv_chart_get_cursor_point, "i", "(lv.obj)(lv.chart_cursor)" } },
  { "get_first_point_center_offset", { (const void*) &lv_chart_get_first_point_center_offset, "i", "(lv.obj)" } },
  { "get_point_count", { (const void*) &lv_chart_get_point_count, "i", "(lv.obj)" } },
  { "get_point_pos_by_id", { (const void*) &lv_chart_get_point_pos_by_id, "", "(lv.obj)(lv.chart_series)ic" } },
  { "get_pressed_point", { (const void*) &lv_chart_get_pressed_point, "i", "(lv.obj)" } },
  { "get_series_next", { (const void*) &lv_chart_get_series_next, "lv.chart_series", "(lv.obj)(lv.chart_series)" } },
  { "get_type", { (const void*) &lv_chart_get_type, "i", "(lv.obj)" } },
  { "get_x_array", { (const void*) &lv_chart_get_x_array, "lv.int_arr", "(lv.obj)(lv.chart_series)" } },
  { "get_x_start_point", { (const void*) &lv_chart_get_x_start_point, "i", "(lv.obj)(lv.chart_series)" } },
  { "get_y_array", { (const void*) &lv_chart_get_y_array, "lv.int_arr", "(lv.obj)(lv.chart_series)" } },
  { "hide_series", { (const void*) &lv_chart_hide_series, "", "(lv.obj)(lv.chart_series)b" } },
  { "refresh", { (const void*) &lv_chart_refresh, "", "(lv.obj)" } },
  { "remove_series", { (const void*) &lv_chart_remove_series, "", "(lv.obj)(lv.chart_series)" } },
  { "set_all_value", { (const void*) &lv_chart_set_all_value, "", "(lv.obj)(lv.chart_series)i" } },
  { "set_cursor_point", { (const void*) &lv_chart_set_cursor_point, "", "(lv.obj)(lv.chart_cursor)(lv.chart_series)i" } },
  { "set_cursor_pos", { (const void*) &lv_chart_set_cursor_pos, "", "(lv.obj)(lv.chart_cursor)c" } },
  { "set_div_line_count", { (const void*) &lv_chart_set_div_line_count, "", "(lv.obj)ii" } },
  { "set_ext_x_array", { (const void*) &lv_chart_set_ext_x_array, "", "(lv.obj)(lv.chart_series)(lv.int_arr)" } },
  { "set_ext_y_array", { (const void*) &lv_chart_set_ext_y_array, "", "(lv.obj)(lv.chart_series)(lv.int_arr)" } },
  { "set_next_value", { (const void*) &lv_chart_set_next_value, "", "(lv.obj)(lv.chart_series)i" } },
  { "set_next_value2", { (const void*) &lv_chart_set_next_value2, "", "(lv.obj)(lv.chart_series)ii" } },
  { "set_point_count", { (const void*) &lv_chart_set_point_count, "", "(lv.obj)i" } },
  { "set_range", { (const void*) &lv_chart_set_range, "", "(lv.obj)iii" } },
  { "set_series_color", { (const void*) &lv_chart_set_series_color, "", "(lv.obj)(lv.chart_series)(lv.color)" } },
  { "set_type", { (const void*) &lv_chart_set_type, "", "(lv.obj)i" } },
  { "set_update_mode", { (const void*) &lv_chart_set_update_mode, "", "(lv.obj)i" } },
  { "set_value_by_id", { (const void*) &lv_chart_set_value_by_id, "", "(lv.obj)(lv.chart_series)ii" } },
  { "set_value_by_id2", { (const void*) &lv_chart_set_value_by_id2, "", "(lv.obj)(lv.chart_series)iii" } },
  { "set_x_start_point", { (const void*) &lv_chart_set_x_start_point, "", "(lv.obj)(lv.chart_series)i" } },
};
#endif // BE_LV_WIDGET_CHART

/* `lv_checkbox` methods */
#ifdef BE_LV_WIDGET_CHECKBOX
const be_ntv_func_def_t lv_checkbox_func[] = {
  { "get_text", { (const void*) &lv_checkbox_get_text, "s", "(lv.obj)" } },
  { "set_text", { (const void*) &lv_checkbox_set_text, "", "(lv.obj)s" } },
  { "set_text_static", { (const void*) &lv_checkbox_set_text_static, "", "(lv.obj)s" } },
};
#endif // BE_LV_WIDGET_CHECKBOX

/* `lv_dropdown` methods */
#ifdef BE_LV_WIDGET_DROPDOWN
const be_ntv_func_def_t lv_dropdown_func[] = {
  { "add_option", { (const void*) &lv_dropdown_add_option, "", "(lv.obj)si" } },
  { "clear_options", { (const void*) &lv_dropdown_clear_options, "", "(lv.obj)" } },
  { "close", { (const void*) &lv_dropdown_close, "", "(lv.obj)" } },
  { "get_dir", { (const void*) &lv_dropdown_get_dir, "i", "(lv.obj)" } },
  { "get_list", { (const void*) &lv_dropdown_get_list, "lv.obj", "(lv.obj)" } },
  { "get_option_cnt", { (const void*) &lv_dropdown_get_option_count, "i", "(lv.obj)" } },
  { "get_option_count", { (const void*) &lv_dropdown_get_option_count, "i", "(lv.obj)" } },
  { "get_option_index", { (const void*) &lv_dropdown_get_option_index, "i", "(lv.obj)s" } },
  { "get_options", { (const void*) &lv_dropdown_get_options, "s", "(lv.obj)" } },
  { "get_selected", { (const void*) &lv_dropdown_get_selected, "i", "(lv.obj)" } },
  { "get_selected_highlight", { (const void*) &lv_dropdown_get_selected_highlight, "b", "(lv.obj)" } },
  { "get_selected_str", { (const void*) &lv_dropdown_get_selected_str, "", "(lv.obj)ci" } },
  { "get_symbol", { (const void*) &lv_dropdown_get_symbol, "s", "(lv.obj)" } },
  { "get_text", { (const void*) &lv_dropdown_get_text, "s", "(lv.obj)" } },
  { "is_open", { (const void*) &lv_dropdown_is_open, "b", "(lv.obj)" } },
  { "open", { (const void*) &lv_dropdown_open, "", "(lv.obj)" } },
  { "set_dir", { (const void*) &lv_dropdown_set_dir, "", "(lv.obj)i" } },
  { "set_options", { (const void*) &lv_dropdown_set_options, "", "(lv.obj)s" } },
  { "set_options_static", { (const void*) &lv_dropdown_set_options_static, "", "(lv.obj)s" } },
  { "set_selected", { (const void*) &lv_dropdown_set_selected, "", "(lv.obj)i" } },
  { "set_selected_highlight", { (const void*) &lv_dropdown_set_selected_highlight, "", "(lv.obj)b" } },
  { "set_symbol", { (const void*) &lv_dropdown_set_symbol, "", "(lv.obj)." } },
  { "set_text", { (const void*) &lv_dropdown_set_text, "", "(lv.obj)s" } },
};
#endif // BE_LV_WIDGET_DROPDOWN

/* `lv_image` methods */
#ifdef BE_LV_WIDGET_IMAGE
const be_ntv_func_def_t lv_image_func[] = {
  { "get_angle", { (const void*) &lv_image_get_rotation, "i", "(lv.obj)" } },
  { "get_antialias", { (const void*) &lv_image_get_antialias, "b", "(lv.obj)" } },
  { "get_bitmap_map_src", { (const void*) &lv_image_get_bitmap_map_src, "lv.image_dsc", "(lv.obj)" } },
  { "get_blend_mode", { (const void*) &lv_image_get_blend_mode, "i", "(lv.obj)" } },
  { "get_inner_align", { (const void*) &lv_image_get_inner_align, "i", "(lv.obj)" } },
  { "get_offset_x", { (const void*) &lv_image_get_offset_x, "i", "(lv.obj)" } },
  { "get_offset_y", { (const void*) &lv_image_get_offset_y, "i", "(lv.obj)" } },
  { "get_pivot", { (const void*) &lv_image_get_pivot, "", "(lv.obj)c" } },
  { "get_rotation", { (const void*) &lv_image_get_rotation, "i", "(lv.obj)" } },
  { "get_scale", { (const void*) &lv_image_get_scale, "i", "(lv.obj)" } },
  { "get_scale_x", { (const void*) &lv_image_get_scale_x, "i", "(lv.obj)" } },
  { "get_scale_y", { (const void*) &lv_image_get_scale_y, "i", "(lv.obj)" } },
  { "get_src", { (const void*) &lv_image_get_src, "c", "(lv.obj)" } },
  { "get_zoom", { (const void*) &lv_image_get_scale, "i", "(lv.obj)" } },
  { "set_angle", { (const void*) &lv_image_set_rotation, "", "(lv.obj)i" } },
  { "set_antialias", { (const void*) &lv_image_set_antialias, "", "(lv.obj)b" } },
  { "set_bitmap_map_src", { (const void*) &lv_image_set_bitmap_map_src, "", "(lv.obj)(lv.image_dsc)" } },
  { "set_blend_mode", { (const void*) &lv_image_set_blend_mode, "", "(lv.obj)i" } },
  { "set_inner_align", { (const void*) &lv_image_set_inner_align, "", "(lv.obj)i" } },
  { "set_offset_x", { (const void*) &lv_image_set_offset_x, "", "(lv.obj)i" } },
  { "set_offset_y", { (const void*) &lv_image_set_offset_y, "", "(lv.obj)i" } },
  { "set_pivot", { (const void*) &lv_image_set_pivot, "", "(lv.obj)ii" } },
  { "set_rotation", { (const void*) &lv_image_set_rotation, "", "(lv.obj)i" } },
  { "set_scale", { (const void*) &lv_image_set_scale, "", "(lv.obj)i" } },
  { "set_scale_x", { (const void*) &lv_image_set_scale_x, "", "(lv.obj)i" } },
  { "set_scale_y", { (const void*) &lv_image_set_scale_y, "", "(lv.obj)i" } },
  { "set_src", { (const void*) &lv_image_set_src, "", "(lv.obj)." } },
  { "set_tasmota_logo", { (const void*) &lv_image_set_tasmota_logo, "", "(lv.obj)" } },
  { "set_zoom", { (const void*) &lv_image_set_scale, "", "(lv.obj)i" } },
};
#endif // BE_LV_WIDGET_IMAGE

/* `lv_imagebutton` methods */
#ifdef BE_LV_WIDGET_IMAGEBUTTON
const be_ntv_func_def_t lv_imagebutton_func[] = {
  { "set_src", { (const void*) &lv_imagebutton_set_src, "", "(lv.obj)i..." } },
  { "set_state", { (const void*) &lv_imagebutton_set_state, "", "(lv.obj)i" } },
};
#endif // BE_LV_WIDGET_IMAGEBUTTON

/* `lv_keyboard` methods */
#ifdef BE_LV_WIDGET_KEYBOARD
const be_ntv_func_def_t lv_keyboard_func[] = {
  { "_btn_text", { (const void*) &lv_keyboard_get_button_text, "s", "(lv.obj)i" } },
  { "get_button_text", { (const void*) &lv_keyboard_get_button_text, "s", "(lv.obj)i" } },
  { "get_map_array", { (const void*) &lv_keyboard_get_map_array, "c", "(lv.obj)" } },
  { "get_mode", { (const void*) &lv_keyboard_get_mode, "i", "(lv.obj)" } },
  { "get_selected_button", { (const void*) &lv_keyboard_get_selected_button, "i", "(lv.obj)" } },
  { "get_textarea", { (const void*) &lv_keyboard_get_textarea, "lv.obj", "(lv.obj)" } },
  { "set_mode", { (const void*) &lv_keyboard_set_mode, "", "(lv.obj)i" } },
  { "set_popovers", { (const void*) &lv_keyboard_set_popovers, "", "(lv.obj)b" } },
  { "set_textarea", { (const void*) &lv_keyboard_set_textarea, "", "(lv.obj)(lv.obj)" } },
};
#endif // BE_LV_WIDGET_KEYBOARD

/* `lv_label` methods */
#ifdef BE_LV_WIDGET_LABEL
const be_ntv_func_def_t lv_label_func[] = {
  { "cut_text", { (const void*) &lv_label_cut_text, "", "(lv.obj)ii" } },
  { "get_letter_on", { (const void*) &lv_label_get_letter_on, "i", "(lv.obj)cb" } },
  { "get_letter_pos", { (const void*) &lv_label_get_letter_pos, "", "(lv.obj)ic" } },
  { "get_long_mode", { (const void*) &lv_label_get_long_mode, "i", "(lv.obj)" } },
  { "get_text", { (const void*) &lv_label_get_text, "s", "(lv.obj)" } },
  { "get_text_selection_end", { (const void*) &lv_label_get_text_selection_end, "i", "(lv.obj)" } },
  { "get_text_selection_start", { (const void*) &lv_label_get_text_selection_start, "i", "(lv.obj)" } },
  { "ins_text", { (const void*) &lv_label_ins_text, "", "(lv.obj)is" } },
  { "is_char_under_pos", { (const void*) &lv_label_is_char_under_pos, "b", "(lv.obj)c" } },
  { "set_long_mode", { (const void*) &lv_label_set_long_mode, "", "(lv.obj)i" } },
  { "set_text", { (const void*) &lv_label_set_text, "", "(lv.obj)s" } },
  { "set_text_fmt", { (const void*) &lv_label_set_text_fmt, "", "(lv.obj)s[......]" } },
  { "set_text_selection_end", { (const void*) &lv_label_set_text_selection_end, "", "(lv.obj)i" } },
  { "set_text_selection_start", { (const void*) &lv_label_set_text_selection_start, "", "(lv.obj)i" } },
  { "set_text_static", { (const void*) &lv_label_set_text_static, "", "(lv.obj)s" } },
};
#endif // BE_LV_WIDGET_LABEL

/* `lv_led` methods */
#ifdef BE_LV_WIDGET_LED
const be_ntv_func_def_t lv_led_func[] = {
  { "get_brightness", { (const void*) &lv_led_get_brightness, "i", "(lv.obj)" } },
  { "off", { (const void*) &lv_led_off, "", "(lv.obj)" } },
  { "on", { (const void*) &lv_led_on, "", "(lv.obj)" } },
  { "set_brightness", { (const void*) &lv_led_set_brightness, "", "(lv.obj)i" } },
  { "set_color", { (const void*) &lv_led_set_color, "", "(lv.obj)(lv.color)" } },
  { "toggle", { (const void*) &lv_led_toggle, "", "(lv.obj)" } },
};
#endif // BE_LV_WIDGET_LED

/* `lv_line` methods */
#ifdef BE_LV_WIDGET_LINE
const be_ntv_func_def_t lv_line_func[] = {
  { "get_points", { (const void*) &lv_line_get_points, "c", "(lv.obj)" } },
  { "get_points_num", { (const void*) &lv_line_get_points_num, "i", "(lv.obj)" } },
  { "get_y_invert", { (const void*) &lv_line_get_y_invert, "b", "(lv.obj)" } },
  { "set_points", { (const void*) &lv_line_set_points, "", "(lv.obj)(lv.point_arr)i" } },
  { "set_y_invert", { (const void*) &lv_line_set_y_invert, "", "(lv.obj)b" } },
};
#endif // BE_LV_WIDGET_LINE

/* `lv_list` methods */
#ifdef BE_LV_WIDGET_LIST
const be_ntv_func_def_t lv_list_func[] = {
  { "_btn_text", { (const void*) &lv_list_get_button_text, "s", "(lv.obj)(lv.obj)" } },
  { "add_btn", { (const void*) &lv_list_add_button, "lv.obj", "(lv.obj).s" } },
  { "add_button", { (const void*) &lv_list_add_button, "lv.obj", "(lv.obj).s" } },
  { "add_text", { (const void*) &lv_list_add_text, "lv.obj", "(lv.obj)s" } },
  { "get_button_text", { (const void*) &lv_list_get_button_text, "s", "(lv.obj)(lv.obj)" } },
  { "set_btn_text", { (const void*) &lv_list_set_button_text, "", "(lv.obj)(lv.obj)s" } },
  { "set_button_text", { (const void*) &lv_list_set_button_text, "", "(lv.obj)(lv.obj)s" } },
};
#endif // BE_LV_WIDGET_LIST

/* `lv_menu` methods */
#ifdef BE_LV_WIDGET_MENU
const be_ntv_func_def_t lv_menu_func[] = {
  { "back_button_is_root", { (const void*) &lv_menu_back_button_is_root, "b", "(lv.obj)(lv.obj)" } },
  { "clear_history", { (const void*) &lv_menu_clear_history, "", "(lv.obj)" } },
  { "get_cur_main_page", { (const void*) &lv_menu_get_cur_main_page, "lv.obj", "(lv.obj)" } },
  { "get_cur_sidebar_page", { (const void*) &lv_menu_get_cur_sidebar_page, "lv.obj", "(lv.obj)" } },
  { "get_main_header", { (const void*) &lv_menu_get_main_header, "lv.obj", "(lv.obj)" } },
  { "get_main_header_back_button", { (const void*) &lv_menu_get_main_header_back_button, "lv.obj", "(lv.obj)" } },
  { "get_sidebar_header", { (const void*) &lv_menu_get_sidebar_header, "lv.obj", "(lv.obj)" } },
  { "get_sidebar_header_back_button", { (const void*) &lv_menu_get_sidebar_header_back_button, "lv.obj", "(lv.obj)" } },
  { "set_load_page_event", { (const void*) &lv_menu_set_load_page_event, "", "(lv.obj)(lv.obj)(lv.obj)" } },
  { "set_mode_header", { (const void*) &lv_menu_set_mode_header, "", "(lv.obj)i" } },
  { "set_mode_root_back_button", { (const void*) &lv_menu_set_mode_root_back_button, "", "(lv.obj)i" } },
  { "set_page", { (const void*) &lv_menu_set_page, "", "(lv.obj)(lv.obj)" } },
  { "set_page_title", { (const void*) &lv_menu_set_page_title, "", "(lv.obj)c" } },
  { "set_page_title_static", { (const void*) &lv_menu_set_page_title_static, "", "(lv.obj)c" } },
  { "set_sidebar_page", { (const void*) &lv_menu_set_sidebar_page, "", "(lv.obj)(lv.obj)" } },
};
#endif // BE_LV_WIDGET_MENU

/* `lv_msgbox` methods */
#ifdef BE_LV_WIDGET_MSGBOX
const be_ntv_func_def_t lv_msgbox_func[] = {
  { "add_close_button", { (const void*) &lv_msgbox_add_close_button, "lv.obj", "(lv.obj)" } },
  { "add_footer_button", { (const void*) &lv_msgbox_add_footer_button, "lv.obj", "(lv.obj)s" } },
  { "add_header_button", { (const void*) &lv_msgbox_add_header_button, "lv.obj", "(lv.obj)." } },
  { "add_text", { (const void*) &lv_msgbox_add_text, "lv.obj", "(lv.obj)s" } },
  { "add_title", { (const void*) &lv_msgbox_add_title, "lv.obj", "(lv.obj)s" } },
  { "close", { (const void*) &lv_msgbox_close, "", "(lv.obj)" } },
  { "close_async", { (const void*) &lv_msgbox_close_async, "", "(lv.obj)" } },
  { "get_content", { (const void*) &lv_msgbox_get_content, "lv.obj", "(lv.obj)" } },
  { "get_footer", { (const void*) &lv_msgbox_get_footer, "lv.obj", "(lv.obj)" } },
  { "get_header", { (const void*) &lv_msgbox_get_header, "lv.obj", "(lv.obj)" } },
  { "get_title", { (const void*) &lv_msgbox_get_title, "lv.obj", "(lv.obj)" } },
};
#endif // BE_LV_WIDGET_MSGBOX

/* `lv_roller` methods */
#ifdef BE_LV_WIDGET_ROLLER
const be_ntv_func_def_t lv_roller_func[] = {
  { "get_option_cnt", { (const void*) &lv_roller_get_option_count, "i", "(lv.obj)" } },
  { "get_option_count", { (const void*) &lv_roller_get_option_count, "i", "(lv.obj)" } },
  { "get_options", { (const void*) &lv_roller_get_options, "s", "(lv.obj)" } },
  { "get_selected", { (const void*) &lv_roller_get_selected, "i", "(lv.obj)" } },
  { "get_selected_str", { (const void*) &lv_roller_get_selected_str, "", "(lv.obj)ci" } },
  { "set_options", { (const void*) &lv_roller_set_options, "", "(lv.obj)si" } },
  { "set_selected", { (const void*) &lv_roller_set_selected, "", "(lv.obj)ii" } },
  { "set_visible_row_cnt", { (const void*) &lv_roller_set_visible_row_count, "", "(lv.obj)i" } },
  { "set_visible_row_count", { (const void*) &lv_roller_set_visible_row_count, "", "(lv.obj)i" } },
};
#endif // BE_LV_WIDGET_ROLLER

/* `lv_scale` methods */
#ifdef BE_LV_WIDGET_SCALE
const be_ntv_func_def_t lv_scale_func[] = {
  { "add_section", { (const void*) &lv_scale_add_section, "lv.scale_section", "(lv.obj)" } },
  { "get_angle_range", { (const void*) &lv_scale_get_angle_range, "i", "(lv.obj)" } },
  { "get_label_show", { (const void*) &lv_scale_get_label_show, "b", "(lv.obj)" } },
  { "get_major_tick_every", { (const void*) &lv_scale_get_major_tick_every, "i", "(lv.obj)" } },
  { "get_mode", { (const void*) &lv_scale_get_mode, "i", "(lv.obj)" } },
  { "get_range_max_value", { (const void*) &lv_scale_get_range_max_value, "i", "(lv.obj)" } },
  { "get_range_min_value", { (const void*) &lv_scale_get_range_min_value, "i", "(lv.obj)" } },
  { "get_total_tick_count", { (const void*) &lv_scale_get_total_tick_count, "i", "(lv.obj)" } },
  { "set_angle", { (const void*) &lv_scale_set_rotation, "", "(lv.obj)i" } },
  { "set_angle_range", { (const void*) &lv_scale_set_angle_range, "", "(lv.obj)i" } },
  { "set_image_needle_value", { (const void*) &lv_scale_set_image_needle_value, "", "(lv.obj)(lv.obj)i" } },
  { "set_label_show", { (const void*) &lv_scale_set_label_show, "", "(lv.obj)b" } },
  { "set_line_needle_value", { (const void*) &lv_scale_set_line_needle_value, "", "(lv.obj)(lv.obj)ii" } },
  { "set_major_tick_every", { (const void*) &lv_scale_set_major_tick_every, "", "(lv.obj)i" } },
  { "set_mode", { (const void*) &lv_scale_set_mode, "", "(lv.obj)i" } },
  { "set_post_draw", { (const void*) &lv_scale_set_post_draw, "", "(lv.obj)b" } },
  { "set_range", { (const void*) &lv_scale_set_range, "", "(lv.obj)ii" } },
  { "set_rotation", { (const void*) &lv_scale_set_rotation, "", "(lv.obj)i" } },
  { "set_text_src", { (const void*) &lv_scale_set_text_src, "", "(lv.obj)(lv.str_arr)" } },
  { "set_total_tick_count", { (const void*) &lv_scale_set_total_tick_count, "", "(lv.obj)i" } },
};
#endif // BE_LV_WIDGET_SCALE

/* `lv_scale_section` methods */
#ifdef BE_LV_WIDGET_SCALE_SECTION
const be_ntv_func_def_t lv_scale_section_func[] = {
  { "set_range", { (const void*) &lv_scale_section_set_range, "", "(lv.scale_section)ii" } },
  { "set_style", { (const void*) &lv_scale_section_set_style, "", "(lv.scale_section)i(lv.style)" } },
};
#endif // BE_LV_WIDGET_SCALE_SECTION

/* `lv_slider` methods */
#ifdef BE_LV_WIDGET_SLIDER
const be_ntv_func_def_t lv_slider_func[] = {
  { "get_left_value", { (const void*) &lv_slider_get_left_value, "i", "(lv.obj)" } },
  { "get_max_value", { (const void*) &lv_slider_get_max_value, "i", "(lv.obj)" } },
  { "get_min_value", { (const void*) &lv_slider_get_min_value, "i", "(lv.obj)" } },
  { "get_mode", { (const void*) &lv_slider_get_mode, "i", "(lv.obj)" } },
  { "get_value", { (const void*) &lv_slider_get_value, "i", "(lv.obj)" } },
  { "is_dragged", { (const void*) &lv_slider_is_dragged, "b", "(lv.obj)" } },
  { "is_symmetrical", { (const void*) &lv_slider_is_symmetrical, "b", "(lv.obj)" } },
  { "set_left_value", { (const void*) &lv_slider_set_left_value, "", "(lv.obj)ii" } },
  { "set_mode", { (const void*) &lv_slider_set_mode, "", "(lv.obj)i" } },
  { "set_range", { (const void*) &lv_slider_set_range, "", "(lv.obj)ii" } },
  { "set_value", { (const void*) &lv_slider_set_value, "", "(lv.obj)ii" } },
};
#endif // BE_LV_WIDGET_SLIDER

/* `lv_span` methods */
#ifdef BE_LV_WIDGET_SPAN
const be_ntv_func_def_t lv_span_func[] = {
  { "get_style", { (const void*) &lv_span_get_style, "lv.style", "(lv.span)" } },
  { "set_text", { (const void*) &lv_span_set_text, "", "(lv.span)s" } },
  { "set_text_static", { (const void*) &lv_span_set_text_static, "", "(lv.span)s" } },
};
#endif // BE_LV_WIDGET_SPAN

/* `lv_spangroup` methods */
#ifdef BE_LV_WIDGET_SPANGROUP
const be_ntv_func_def_t lv_spangroup_func[] = {
  { "delete_span", { (const void*) &lv_spangroup_delete_span, "", "(lv.obj)(lv.span)" } },
  { "get_align", { (const void*) &lv_spangroup_get_align, "i", "(lv.obj)" } },
  { "get_child", { (const void*) &lv_spangroup_get_child, "lv.span", "(lv.obj)i" } },
  { "get_expand_height", { (const void*) &lv_spangroup_get_expand_height, "i", "(lv.obj)i" } },
  { "get_expand_width", { (const void*) &lv_spangroup_get_expand_width, "i", "(lv.obj)i" } },
  { "get_indent", { (const void*) &lv_spangroup_get_indent, "i", "(lv.obj)" } },
  { "get_max_line_height", { (const void*) &lv_spangroup_get_max_line_height, "i", "(lv.obj)" } },
  { "get_max_lines", { (const void*) &lv_spangroup_get_max_lines, "i", "(lv.obj)" } },
  { "get_mode", { (const void*) &lv_spangroup_get_mode, "i", "(lv.obj)" } },
  { "get_overflow", { (const void*) &lv_spangroup_get_overflow, "i", "(lv.obj)" } },
  { "get_span_count", { (const void*) &lv_spangroup_get_span_count, "i", "(lv.obj)" } },
  { "new_span", { (const void*) &lv_spangroup_new_span, "lv.span", "(lv.obj)" } },
  { "refr_mode", { (const void*) &lv_spangroup_refr_mode, "", "(lv.obj)" } },
  { "set_align", { (const void*) &lv_spangroup_set_align, "", "(lv.obj)i" } },
  { "set_indent", { (const void*) &lv_spangroup_set_indent, "", "(lv.obj)i" } },
  { "set_max_lines", { (const void*) &lv_spangroup_set_max_lines, "", "(lv.obj)i" } },
  { "set_mode", { (const void*) &lv_spangroup_set_mode, "", "(lv.obj)i" } },
  { "set_overflow", { (const void*) &lv_spangroup_set_overflow, "", "(lv.obj)i" } },
};
#endif // BE_LV_WIDGET_SPANGROUP

/* `lv_spinbox` methods */
#ifdef BE_LV_WIDGET_SPINBOX
const be_ntv_func_def_t lv_spinbox_func[] = {
  { "decrement", { (const void*) &lv_spinbox_decrement, "", "(lv.obj)" } },
  { "get_rollover", { (const void*) &lv_spinbox_get_rollover, "b", "(lv.obj)" } },
  { "get_step", { (const void*) &lv_spinbox_get_step, "i", "(lv.obj)" } },
  { "get_value", { (const void*) &lv_spinbox_get_value, "i", "(lv.obj)" } },
  { "increment", { (const void*) &lv_spinbox_increment, "", "(lv.obj)" } },
  { "set_cursor_pos", { (const void*) &lv_spinbox_set_cursor_pos, "", "(lv.obj)i" } },
  { "set_digit_format", { (const void*) &lv_spinbox_set_digit_format, "", "(lv.obj)ii" } },
  { "set_digit_step_direction", { (const void*) &lv_spinbox_set_digit_step_direction, "", "(lv.obj)i" } },
  { "set_range", { (const void*) &lv_spinbox_set_range, "", "(lv.obj)ii" } },
  { "set_rollover", { (const void*) &lv_spinbox_set_rollover, "", "(lv.obj)b" } },
  { "set_step", { (const void*) &lv_spinbox_set_step, "", "(lv.obj)i" } },
  { "set_value", { (const void*) &lv_spinbox_set_value, "", "(lv.obj)i" } },
  { "step_next", { (const void*) &lv_spinbox_step_next, "", "(lv.obj)" } },
  { "step_prev", { (const void*) &lv_spinbox_step_prev, "", "(lv.obj)" } },
};
#endif // BE_LV_WIDGET_SPINBOX

/* `lv_spinner` methods */
#ifdef BE_LV_WIDGET_SPINNER
const be_ntv_func_def_t lv_spinner_func[] = {
  { "set_anim_params", { (const void*) &lv_spinner_set_anim_params, "", "(lv.obj)ii" } },
};
#endif // BE_LV_WIDGET_SPINNER

/* `lv_switch` methods */
#ifdef BE_LV_WIDGET_SWITCH
const be_ntv_func_def_t lv_switch_func[] = {
};
#endif // BE_LV_WIDGET_SWITCH

/* `lv_table` methods */
#ifdef BE_LV_WIDGET_TABLE
const be_ntv_func_def_t lv_table_func[] = {
  { "add_cell_ctrl", { (const void*) &lv_table_add_cell_ctrl, "", "(lv.obj)iii" } },
  { "clear_cell_ctrl", { (const void*) &lv_table_clear_cell_ctrl, "", "(lv.obj)iii" } },
  { "get_cell_user_data", { (const void*) &lv_table_get_cell_user_data, "c", "(lv.obj)ii" } },
  { "get_cell_value", { (const void*) &lv_table_get_cell_value, "s", "(lv.obj)ii" } },
  { "get_col_cnt", { (const void*) &lv_table_get_column_count, "i", "(lv.obj)" } },
  { "get_col_width", { (const void*) &lv_table_get_column_width, "i", "(lv.obj)i" } },
  { "get_column_count", { (const void*) &lv_table_get_column_count, "i", "(lv.obj)" } },
  { "get_column_width", { (const void*) &lv_table_get_column_width, "i", "(lv.obj)i" } },
  { "get_row_cnt", { (const void*) &lv_table_get_row_count, "i", "(lv.obj)" } },
  { "get_row_count", { (const void*) &lv_table_get_row_count, "i", "(lv.obj)" } },
  { "get_selected_cell", { (const void*) &lv_table_get_selected_cell, "", "(lv.obj)(lv.int_arr)(lv.int_arr)" } },
  { "has_cell_ctrl", { (const void*) &lv_table_has_cell_ctrl, "b", "(lv.obj)iii" } },
  { "set_cell_user_data", { (const void*) &lv_table_set_cell_user_data, "", "(lv.obj)ii." } },
  { "set_cell_value", { (const void*) &lv_table_set_cell_value, "", "(lv.obj)iis" } },
  { "set_cell_value_fmt", { (const void*) &lv_table_set_cell_value_fmt, "", "(lv.obj)iis[......]" } },
  { "set_col_cnt", { (const void*) &lv_table_set_column_count, "", "(lv.obj)i" } },
  { "set_col_width", { (const void*) &lv_table_set_column_width, "", "(lv.obj)ii" } },
  { "set_column_count", { (const void*) &lv_table_set_column_count, "", "(lv.obj)i" } },
  { "set_column_width", { (const void*) &lv_table_set_column_width, "", "(lv.obj)ii" } },
  { "set_row_cnt", { (const void*) &lv_table_set_row_count, "", "(lv.obj)i" } },
  { "set_row_count", { (const void*) &lv_table_set_row_count, "", "(lv.obj)i" } },
};
#endif // BE_LV_WIDGET_TABLE

/* `lv_tabview` methods */
#ifdef BE_LV_WIDGET_TABVIEW
const be_ntv_func_def_t lv_tabview_func[] = {
  { "add_tab", { (const void*) &lv_tabview_add_tab, "lv.obj", "(lv.obj)s" } },
  { "get_content", { (const void*) &lv_tabview_get_content, "lv.obj", "(lv.obj)" } },
  { "get_tab_act", { (const void*) &lv_tabview_get_tab_active, "i", "(lv.obj)" } },
  { "get_tab_active", { (const void*) &lv_tabview_get_tab_active, "i", "(lv.obj)" } },
  { "get_tab_bar", { (const void*) &lv_tabview_get_tab_bar, "lv.obj", "(lv.obj)" } },
  { "get_tab_btns", { (const void*) &lv_tabview_get_tab_bar, "lv.obj", "(lv.obj)" } },
  { "get_tab_count", { (const void*) &lv_tabview_get_tab_count, "i", "(lv.obj)" } },
  { "rename_tab", { (const void*) &lv_tabview_rename_tab, "", "(lv.obj)is" } },
  { "set_act", { (const void*) &lv_tabview_set_active, "", "(lv.obj)ii" } },
  { "set_active", { (const void*) &lv_tabview_set_active, "", "(lv.obj)ii" } },
  { "set_tab_bar_position", { (const void*) &lv_tabview_set_tab_bar_position, "", "(lv.obj)i" } },
  { "set_tab_bar_size", { (const void*) &lv_tabview_set_tab_bar_size, "", "(lv.obj)i" } },
};
#endif // BE_LV_WIDGET_TABVIEW

/* `lv_textarea` methods */
#ifdef BE_LV_WIDGET_TEXTAREA
const be_ntv_func_def_t lv_textarea_func[] = {
  { "add_char", { (const void*) &lv_textarea_add_char, "", "(lv.obj)i" } },
  { "add_text", { (const void*) &lv_textarea_add_text, "", "(lv.obj)s" } },
  { "clear_selection", { (const void*) &lv_textarea_clear_selection, "", "(lv.obj)" } },
  { "cursor_down", { (const void*) &lv_textarea_cursor_down, "", "(lv.obj)" } },
  { "cursor_left", { (const void*) &lv_textarea_cursor_left, "", "(lv.obj)" } },
  { "cursor_right", { (const void*) &lv_textarea_cursor_right, "", "(lv.obj)" } },
  { "cursor_up", { (const void*) &lv_textarea_cursor_up, "", "(lv.obj)" } },
  { "delete_char", { (const void*) &lv_textarea_delete_char, "", "(lv.obj)" } },
  { "delete_char_forward", { (const void*) &lv_textarea_delete_char_forward, "", "(lv.obj)" } },
  { "get_accepted_chars", { (const void*) &lv_textarea_get_accepted_chars, "s", "(lv.obj)" } },
  { "get_current_char", { (const void*) &lv_textarea_get_current_char, "i", "(lv.obj)" } },
  { "get_cursor_click_pos", { (const void*) &lv_textarea_get_cursor_click_pos, "b", "(lv.obj)" } },
  { "get_cursor_pos", { (const void*) &lv_textarea_get_cursor_pos, "i", "(lv.obj)" } },
  { "get_label", { (const void*) &lv_textarea_get_label, "lv.obj", "(lv.obj)" } },
  { "get_max_length", { (const void*) &lv_textarea_get_max_length, "i", "(lv.obj)" } },
  { "get_one_line", { (const void*) &lv_textarea_get_one_line, "b", "(lv.obj)" } },
  { "get_password_bullet", { (const void*) &lv_textarea_get_password_bullet, "s", "(lv.obj)" } },
  { "get_password_mode", { (const void*) &lv_textarea_get_password_mode, "b", "(lv.obj)" } },
  { "get_password_show_time", { (const void*) &lv_textarea_get_password_show_time, "i", "(lv.obj)" } },
  { "get_placeholder_text", { (const void*) &lv_textarea_get_placeholder_text, "s", "(lv.obj)" } },
  { "get_text", { (const void*) &lv_textarea_get_text, "s", "(lv.obj)" } },
  { "get_text_selection", { (const void*) &lv_textarea_get_text_selection, "b", "(lv.obj)" } },
  { "set_accepted_chars", { (const void*) &lv_textarea_set_accepted_chars, "", "(lv.obj)s" } },
  { "set_align", { (const void*) &lv_textarea_set_align, "", "(lv.obj)i" } },
  { "set_cursor_click_pos", { (const void*) &lv_textarea_set_cursor_click_pos, "", "(lv.obj)b" } },
  { "set_cursor_pos", { (const void*) &lv_textarea_set_cursor_pos, "", "(lv.obj)i" } },
  { "set_insert_replace", { (const void*) &lv_textarea_set_insert_replace, "", "(lv.obj)s" } },
  { "set_max_length", { (const void*) &lv_textarea_set_max_length, "", "(lv.obj)i" } },
  { "set_one_line", { (const void*) &lv_textarea_set_one_line, "", "(lv.obj)b" } },
  { "set_password_bullet", { (const void*) &lv_textarea_set_password_bullet, "", "(lv.obj)s" } },
  { "set_password_mode", { (const void*) &lv_textarea_set_password_mode, "", "(lv.obj)b" } },
  { "set_password_show_time", { (const void*) &lv_textarea_set_password_show_time, "", "(lv.obj)i" } },
  { "set_placeholder_text", { (const void*) &lv_textarea_set_placeholder_text, "", "(lv.obj)s" } },
  { "set_text", { (const void*) &lv_textarea_set_text, "", "(lv.obj)s" } },
  { "set_text_selection", { (const void*) &lv_textarea_set_text_selection, "", "(lv.obj)b" } },
  { "text_is_selected", { (const void*) &lv_textarea_text_is_selected, "b", "(lv.obj)" } },
};
#endif // BE_LV_WIDGET_TEXTAREA

/* `lv_tileview` methods */
#ifdef BE_LV_WIDGET_TILEVIEW
const be_ntv_func_def_t lv_tileview_func[] = {
  { "add_tile", { (const void*) &lv_tileview_add_tile, "lv.obj", "(lv.obj)iii" } },
  { "get_tile_act", { (const void*) &lv_tileview_get_tile_active, "lv.obj", "(lv.obj)" } },
  { "get_tile_active", { (const void*) &lv_tileview_get_tile_active, "lv.obj", "(lv.obj)" } },
  { "set_tile", { (const void*) &lv_tileview_set_tile, "", "(lv.obj)(lv.obj)i" } },
  { "set_tile_by_index", { (const void*) &lv_tileview_set_tile_by_index, "", "(lv.obj)iii" } },
  { "set_tile_id", { (const void*) &lv_tileview_set_tile_by_index, "", "(lv.obj)iii" } },
};
#endif // BE_LV_WIDGET_TILEVIEW

extern const bclass be_class_lv_anim;
extern const bclass be_class_lv_animimg;
extern const bclass be_class_lv_arc;
extern const bclass be_class_lv_bar;
extern const bclass be_class_lv_button;
extern const bclass be_class_lv_buttonmatrix;
extern const bclass be_class_lv_calendar;
extern const bclass be_class_lv_canvas;
extern const bclass be_class_lv_chart;
extern const bclass be_class_lv_checkbox;
extern const bclass be_class_lv_color;
extern const bclass be_class_lv_display;
extern const bclass be_class_lv_dropdown;
extern const bclass be_class_lv_event;
extern const bclass be_class_lv_font;
extern const bclass be_class_lv_group;
extern const bclass be_class_lv_image;
extern const bclass be_class_lv_imagebutton;
extern const bclass be_class_lv_indev;
extern const bclass be_class_lv_keyboard;
extern const bclass be_class_lv_label;
extern const bclass be_class_lv_led;
extern const bclass be_class_lv_line;
extern const bclass be_class_lv_list;
extern const bclass be_class_lv_menu;
extern const bclass be_class_lv_msgbox;
extern const bclass be_class_lv_obj;
extern const bclass be_class_lv_qrcode;
extern const bclass be_class_lv_roller;
extern const bclass be_class_lv_scale;
extern const bclass be_class_lv_scale_section;
extern const bclass be_class_lv_slider;
extern const bclass be_class_lv_span;
extern const bclass be_class_lv_spangroup;
extern const bclass be_class_lv_spinbox;
extern const bclass be_class_lv_spinner;
extern const bclass be_class_lv_style;
extern const bclass be_class_lv_switch;
extern const bclass be_class_lv_table;
extern const bclass be_class_lv_tabview;
extern const bclass be_class_lv_textarea;
extern const bclass be_class_lv_theme;
extern const bclass be_class_lv_tileview;
extern const bclass be_class_lv_timer;


// map of clases
const be_ntv_class_def_t lv_classes[] = {
  { "lv_anim", &be_class_lv_anim, lv_anim_func, sizeof(lv_anim_func) / sizeof(lv_anim_func[0]) },
#ifdef BE_LV_WIDGET_ANIMIMG
  { "lv_animimg", &be_class_lv_animimg, lv_animimg_func, sizeof(lv_animimg_func) / sizeof(lv_animimg_func[0]) },
#endif // BE_LV_WIDGET_ANIMIMG
#ifdef BE_LV_WIDGET_ARC
  { "lv_arc", &be_class_lv_arc, lv_arc_func, sizeof(lv_arc_func) / sizeof(lv_arc_func[0]) },
#endif // BE_LV_WIDGET_ARC
#ifdef BE_LV_WIDGET_BAR
  { "lv_bar", &be_class_lv_bar, lv_bar_func, sizeof(lv_bar_func) / sizeof(lv_bar_func[0]) },
#endif // BE_LV_WIDGET_BAR
#ifdef BE_LV_WIDGET_BUTTON
  { "lv_button", &be_class_lv_button, lv_button_func, sizeof(lv_button_func) / sizeof(lv_button_func[0]) },
#endif // BE_LV_WIDGET_BUTTON
#ifdef BE_LV_WIDGET_BUTTONMATRIX
  { "lv_buttonmatrix", &be_class_lv_buttonmatrix, lv_buttonmatrix_func, sizeof(lv_buttonmatrix_func) / sizeof(lv_buttonmatrix_func[0]) },
#endif // BE_LV_WIDGET_BUTTONMATRIX
#ifdef BE_LV_WIDGET_CALENDAR
  { "lv_calendar", &be_class_lv_calendar, lv_calendar_func, sizeof(lv_calendar_func) / sizeof(lv_calendar_func[0]) },
#endif // BE_LV_WIDGET_CALENDAR
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
  { "lv_display", &be_class_lv_display, lv_display_func, sizeof(lv_display_func) / sizeof(lv_display_func[0]) },
#ifdef BE_LV_WIDGET_DROPDOWN
  { "lv_dropdown", &be_class_lv_dropdown, lv_dropdown_func, sizeof(lv_dropdown_func) / sizeof(lv_dropdown_func[0]) },
#endif // BE_LV_WIDGET_DROPDOWN
  { "lv_event", &be_class_lv_event, lv_event_func, sizeof(lv_event_func) / sizeof(lv_event_func[0]) },
  { "lv_font", &be_class_lv_font, lv_font_func, sizeof(lv_font_func) / sizeof(lv_font_func[0]) },
  { "lv_group", &be_class_lv_group, lv_group_func, sizeof(lv_group_func) / sizeof(lv_group_func[0]) },
#ifdef BE_LV_WIDGET_IMAGE
  { "lv_image", &be_class_lv_image, lv_image_func, sizeof(lv_image_func) / sizeof(lv_image_func[0]) },
#endif // BE_LV_WIDGET_IMAGE
#ifdef BE_LV_WIDGET_IMAGEBUTTON
  { "lv_imagebutton", &be_class_lv_imagebutton, lv_imagebutton_func, sizeof(lv_imagebutton_func) / sizeof(lv_imagebutton_func[0]) },
#endif // BE_LV_WIDGET_IMAGEBUTTON
  { "lv_indev", &be_class_lv_indev, lv_indev_func, sizeof(lv_indev_func) / sizeof(lv_indev_func[0]) },
#ifdef BE_LV_WIDGET_KEYBOARD
  { "lv_keyboard", &be_class_lv_keyboard, lv_keyboard_func, sizeof(lv_keyboard_func) / sizeof(lv_keyboard_func[0]) },
#endif // BE_LV_WIDGET_KEYBOARD
#ifdef BE_LV_WIDGET_LABEL
  { "lv_label", &be_class_lv_label, lv_label_func, sizeof(lv_label_func) / sizeof(lv_label_func[0]) },
#endif // BE_LV_WIDGET_LABEL
#ifdef BE_LV_WIDGET_LED
  { "lv_led", &be_class_lv_led, lv_led_func, sizeof(lv_led_func) / sizeof(lv_led_func[0]) },
#endif // BE_LV_WIDGET_LED
#ifdef BE_LV_WIDGET_LINE
  { "lv_line", &be_class_lv_line, lv_line_func, sizeof(lv_line_func) / sizeof(lv_line_func[0]) },
#endif // BE_LV_WIDGET_LINE
#ifdef BE_LV_WIDGET_LIST
  { "lv_list", &be_class_lv_list, lv_list_func, sizeof(lv_list_func) / sizeof(lv_list_func[0]) },
#endif // BE_LV_WIDGET_LIST
#ifdef BE_LV_WIDGET_MENU
  { "lv_menu", &be_class_lv_menu, lv_menu_func, sizeof(lv_menu_func) / sizeof(lv_menu_func[0]) },
#endif // BE_LV_WIDGET_MENU
#ifdef BE_LV_WIDGET_MSGBOX
  { "lv_msgbox", &be_class_lv_msgbox, lv_msgbox_func, sizeof(lv_msgbox_func) / sizeof(lv_msgbox_func[0]) },
#endif // BE_LV_WIDGET_MSGBOX
#ifdef BE_LV_WIDGET_OBJ
  { "lv_obj", &be_class_lv_obj, lv_obj_func, sizeof(lv_obj_func) / sizeof(lv_obj_func[0]) },
#endif // BE_LV_WIDGET_OBJ
#ifdef BE_LV_WIDGET_QRCODE
  { "lv_qrcode", &be_class_lv_qrcode, lv_qrcode_func, sizeof(lv_qrcode_func) / sizeof(lv_qrcode_func[0]) },
#endif // BE_LV_WIDGET_QRCODE
#ifdef BE_LV_WIDGET_ROLLER
  { "lv_roller", &be_class_lv_roller, lv_roller_func, sizeof(lv_roller_func) / sizeof(lv_roller_func[0]) },
#endif // BE_LV_WIDGET_ROLLER
#ifdef BE_LV_WIDGET_SCALE
  { "lv_scale", &be_class_lv_scale, lv_scale_func, sizeof(lv_scale_func) / sizeof(lv_scale_func[0]) },
#endif // BE_LV_WIDGET_SCALE
#ifdef BE_LV_WIDGET_SCALE_SECTION
  { "lv_scale_section", &be_class_lv_scale_section, lv_scale_section_func, sizeof(lv_scale_section_func) / sizeof(lv_scale_section_func[0]) },
#endif // BE_LV_WIDGET_SCALE_SECTION
#ifdef BE_LV_WIDGET_SLIDER
  { "lv_slider", &be_class_lv_slider, lv_slider_func, sizeof(lv_slider_func) / sizeof(lv_slider_func[0]) },
#endif // BE_LV_WIDGET_SLIDER
#ifdef BE_LV_WIDGET_SPAN
  { "lv_span", &be_class_lv_span, lv_span_func, sizeof(lv_span_func) / sizeof(lv_span_func[0]) },
#endif // BE_LV_WIDGET_SPAN
#ifdef BE_LV_WIDGET_SPANGROUP
  { "lv_spangroup", &be_class_lv_spangroup, lv_spangroup_func, sizeof(lv_spangroup_func) / sizeof(lv_spangroup_func[0]) },
#endif // BE_LV_WIDGET_SPANGROUP
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
#ifdef BE_LV_WIDGET_TABVIEW
  { "lv_tabview", &be_class_lv_tabview, lv_tabview_func, sizeof(lv_tabview_func) / sizeof(lv_tabview_func[0]) },
#endif // BE_LV_WIDGET_TABVIEW
#ifdef BE_LV_WIDGET_TEXTAREA
  { "lv_textarea", &be_class_lv_textarea, lv_textarea_func, sizeof(lv_textarea_func) / sizeof(lv_textarea_func[0]) },
#endif // BE_LV_WIDGET_TEXTAREA
  { "lv_theme", &be_class_lv_theme, lv_theme_func, sizeof(lv_theme_func) / sizeof(lv_theme_func[0]) },
#ifdef BE_LV_WIDGET_TILEVIEW
  { "lv_tileview", &be_class_lv_tileview, lv_tileview_func, sizeof(lv_tileview_func) / sizeof(lv_tileview_func[0]) },
#endif // BE_LV_WIDGET_TILEVIEW
  { "lv_timer", &be_class_lv_timer, lv_timer_func, sizeof(lv_timer_func) / sizeof(lv_timer_func[0]) },
};
const size_t lv_classes_size = sizeof(lv_classes) / sizeof(lv_classes[0]);

  /* `lv_style` methods */
  /* `lv_font` methods */
  /* `lv_color` methods */
  /* `lv_theme` methods */
  /* `lv_group` methods */
  int be_ntv_lv_group_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_group_create, "+_p", ""); }
  /* `lv_obj` methods */
#ifdef BE_LV_WIDGET_OBJ
  int be_ntv_lv_obj_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_OBJ
  /* `lv_event` methods */
  /* `lv_display` methods */
  /* `lv_indev` methods */
  /* `lv_qrcode` methods */
#ifdef BE_LV_WIDGET_QRCODE
  int be_ntv_lv_qrcode_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_qrcode_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_QRCODE
  /* `lv_anim` methods */
  /* `lv_timer` methods */
  /* `lv_animimg` methods */
#ifdef BE_LV_WIDGET_ANIMIMG
  int be_ntv_lv_animimg_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_animimg_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_ANIMIMG
  /* `lv_arc` methods */
#ifdef BE_LV_WIDGET_ARC
  int be_ntv_lv_arc_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_arc_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_ARC
  /* `lv_bar` methods */
#ifdef BE_LV_WIDGET_BAR
  int be_ntv_lv_bar_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_bar_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_BAR
  /* `lv_button` methods */
#ifdef BE_LV_WIDGET_BUTTON
  int be_ntv_lv_button_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_button_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_BUTTON
  /* `lv_buttonmatrix` methods */
#ifdef BE_LV_WIDGET_BUTTONMATRIX
  int be_ntv_lv_buttonmatrix_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_buttonmatrix_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_BUTTONMATRIX
  /* `lv_calendar` methods */
#ifdef BE_LV_WIDGET_CALENDAR
  int be_ntv_lv_calendar_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_calendar_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_CALENDAR
#ifdef BE_LV_WIDGET_CALENDAR
  int be_ntv_lv_calendar_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_calendar_header_arrow_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_CALENDAR
#ifdef BE_LV_WIDGET_CALENDAR
  int be_ntv_lv_calendar_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_calendar_header_dropdown_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_CALENDAR
  /* `lv_canvas` methods */
#ifdef BE_LV_WIDGET_CANVAS
  int be_ntv_lv_canvas_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_canvas_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_CANVAS
  /* `lv_chart` methods */
#ifdef BE_LV_WIDGET_CHART
  int be_ntv_lv_chart_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_chart_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_CHART
  /* `lv_checkbox` methods */
#ifdef BE_LV_WIDGET_CHECKBOX
  int be_ntv_lv_checkbox_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_checkbox_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_CHECKBOX
  /* `lv_dropdown` methods */
#ifdef BE_LV_WIDGET_DROPDOWN
  int be_ntv_lv_dropdown_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_dropdown_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_DROPDOWN
  /* `lv_image` methods */
#ifdef BE_LV_WIDGET_IMAGE
  int be_ntv_lv_image_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_image_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_IMAGE
  /* `lv_imagebutton` methods */
#ifdef BE_LV_WIDGET_IMAGEBUTTON
  int be_ntv_lv_imagebutton_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_imagebutton_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_IMAGEBUTTON
  /* `lv_keyboard` methods */
#ifdef BE_LV_WIDGET_KEYBOARD
  int be_ntv_lv_keyboard_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_keyboard_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_KEYBOARD
  /* `lv_label` methods */
#ifdef BE_LV_WIDGET_LABEL
  int be_ntv_lv_label_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_label_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_LABEL
  /* `lv_led` methods */
#ifdef BE_LV_WIDGET_LED
  int be_ntv_lv_led_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_led_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_LED
  /* `lv_line` methods */
#ifdef BE_LV_WIDGET_LINE
  int be_ntv_lv_line_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_line_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_LINE
  /* `lv_list` methods */
#ifdef BE_LV_WIDGET_LIST
  int be_ntv_lv_list_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_list_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_LIST
  /* `lv_menu` methods */
#ifdef BE_LV_WIDGET_MENU
  int be_ntv_lv_menu_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_menu_cont_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_MENU
#ifdef BE_LV_WIDGET_MENU
  int be_ntv_lv_menu_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_menu_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_MENU
#ifdef BE_LV_WIDGET_MENU
  int be_ntv_lv_menu_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_menu_page_create, "+_p", "(lv.obj)c"); }
#endif // BE_LV_WIDGET_MENU
#ifdef BE_LV_WIDGET_MENU
  int be_ntv_lv_menu_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_menu_section_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_MENU
#ifdef BE_LV_WIDGET_MENU
  int be_ntv_lv_menu_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_menu_separator_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_MENU
  /* `lv_msgbox` methods */
#ifdef BE_LV_WIDGET_MSGBOX
  int be_ntv_lv_msgbox_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_msgbox_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_MSGBOX
  /* `lv_roller` methods */
#ifdef BE_LV_WIDGET_ROLLER
  int be_ntv_lv_roller_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_roller_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_ROLLER
  /* `lv_scale` methods */
#ifdef BE_LV_WIDGET_SCALE
  int be_ntv_lv_scale_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_scale_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_SCALE
  /* `lv_scale_section` methods */
#ifdef BE_LV_WIDGET_SCALE_SECTION
  int be_ntv_lv_scale_section_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_scale_section_set_style, "+_p", "(lv.scale_section)i(lv.style)"); }
#endif // BE_LV_WIDGET_SCALE_SECTION
  /* `lv_slider` methods */
#ifdef BE_LV_WIDGET_SLIDER
  int be_ntv_lv_slider_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_slider_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_SLIDER
  /* `lv_span` methods */
#ifdef BE_LV_WIDGET_SPAN
  int be_ntv_lv_span_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_span_set_text_static, "+_p", "(lv.span)s"); }
#endif // BE_LV_WIDGET_SPAN
  /* `lv_spangroup` methods */
#ifdef BE_LV_WIDGET_SPANGROUP
  int be_ntv_lv_spangroup_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_spangroup_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_SPANGROUP
  /* `lv_spinbox` methods */
#ifdef BE_LV_WIDGET_SPINBOX
  int be_ntv_lv_spinbox_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_spinbox_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_SPINBOX
  /* `lv_spinner` methods */
#ifdef BE_LV_WIDGET_SPINNER
  int be_ntv_lv_spinner_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_spinner_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_SPINNER
  /* `lv_switch` methods */
#ifdef BE_LV_WIDGET_SWITCH
  int be_ntv_lv_switch_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_switch_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_SWITCH
  /* `lv_table` methods */
#ifdef BE_LV_WIDGET_TABLE
  int be_ntv_lv_table_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_table_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_TABLE
  /* `lv_tabview` methods */
#ifdef BE_LV_WIDGET_TABVIEW
  int be_ntv_lv_tabview_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_tabview_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_TABVIEW
  /* `lv_textarea` methods */
#ifdef BE_LV_WIDGET_TEXTAREA
  int be_ntv_lv_textarea_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_TEXTAREA
  /* `lv_tileview` methods */
#ifdef BE_LV_WIDGET_TILEVIEW
  int be_ntv_lv_tileview_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_tileview_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_TILEVIEW

// create font either empty or from parameter on stack
int lvbe_font_create(bvm *vm)       { return be_call_c_func(vm, NULL, "+_p", ""); }
int lvbe_theme_create(bvm *vm)       { return be_call_c_func(vm, NULL, "+_p", ""); }


#ifdef __cplusplus
} /* extern "C" */
#endif
/********************************************************************/
