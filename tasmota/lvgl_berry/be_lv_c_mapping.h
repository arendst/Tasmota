
/********************************************************************
 * Generated code, don't edit
  *******************************************************************/

#ifdef __cplusplus
extern "C" {
#endif



/* `lv_img` methods */
#if BE_LV_WIDGET_IMG
const lvbe_call_c_t lv_img_func[] = {
  { "create", (void*) &lv_img_create, "+lv_img", "(lv_obj)(lv_obj)" },
  { "get_angle", (void*) &lv_img_get_angle, "i", "(lv_obj)" },
  { "get_antialias", (void*) &lv_img_get_antialias, "b", "(lv_obj)" },
  { "get_auto_size", (void*) &lv_img_get_auto_size, "b", "(lv_obj)" },
  { "get_file_name", (void*) &lv_img_get_file_name, "s", "(lv_obj)" },
  { "get_offset_x", (void*) &lv_img_get_offset_x, "i", "(lv_obj)" },
  { "get_offset_y", (void*) &lv_img_get_offset_y, "i", "(lv_obj)" },
  { "get_pivot", (void*) &lv_img_get_pivot, "", "(lv_obj)(lv_point)" },
  { "get_src", (void*) &lv_img_get_src, ".", "(lv_obj)" },
  { "get_zoom", (void*) &lv_img_get_zoom, "i", "(lv_obj)" },
  { "set_angle", (void*) &lv_img_set_angle, "", "(lv_obj)i" },
  { "set_antialias", (void*) &lv_img_set_antialias, "", "(lv_obj)b" },
  { "set_auto_size", (void*) &lv_img_set_auto_size, "", "(lv_obj)b" },
  { "set_offset_x", (void*) &lv_img_set_offset_x, "", "(lv_obj)i" },
  { "set_offset_y", (void*) &lv_img_set_offset_y, "", "(lv_obj)i" },
  { "set_pivot", (void*) &lv_img_set_pivot, "", "(lv_obj)ii" },
  { "set_src", (void*) &lv_img_set_src, "", "(lv_obj)." },
  { "set_tasmota_logo", (void*) &lv_img_set_tasmota_logo, "", "(lv_obj)" },
  { "set_zoom", (void*) &lv_img_set_zoom, "", "(lv_obj)i" },
};
#endif // BE_LV_WIDGET_IMG

/* `lv_style` methods */
const lvbe_call_c_t lv_style_func[] = {
  { "copy", (void*) &lv_style_copy, "", "(lv_style)(lv_style)" },
  { "list_copy", (void*) &lv_style_list_copy, "", "(lv_style_list)(lv_style_list)" },
  { "list_get_local_style", (void*) &lv_style_list_get_local_style, "lv_style", "(lv_style_list)" },
  { "list_get_style", (void*) &lv_style_list_get_style, "lv_style", "(lv_style_list)i" },
  { "list_init", (void*) &lv_style_list_init, "", "(lv_style_list)" },
  { "remove_prop", (void*) &lv_style_remove_prop, "b", "(lv_style)(lv_style_property)" },
  { "reset", (void*) &lv_style_reset, "", "(lv_style)" },
  { "set_bg_blend_mode", (void*) &lv_style_set_bg_blend_mode, "", "(lv_style)ii" },
  { "set_bg_color", (void*) &lv_style_set_bg_color, "", "(lv_style)i(lv_color)" },
  { "set_bg_grad_color", (void*) &lv_style_set_bg_grad_color, "", "(lv_style)i(lv_color)" },
  { "set_bg_grad_dir", (void*) &lv_style_set_bg_grad_dir, "", "(lv_style)ii" },
  { "set_bg_grad_stop", (void*) &lv_style_set_bg_grad_stop, "", "(lv_style)ii" },
  { "set_bg_main_stop", (void*) &lv_style_set_bg_main_stop, "", "(lv_style)ii" },
  { "set_bg_opa", (void*) &lv_style_set_bg_opa, "", "(lv_style)ii" },
  { "set_border_blend_mode", (void*) &lv_style_set_border_blend_mode, "", "(lv_style)ii" },
  { "set_border_color", (void*) &lv_style_set_border_color, "", "(lv_style)i(lv_color)" },
  { "set_border_opa", (void*) &lv_style_set_border_opa, "", "(lv_style)ii" },
  { "set_border_post", (void*) &lv_style_set_border_post, "", "(lv_style)ib" },
  { "set_border_side", (void*) &lv_style_set_border_side, "", "(lv_style)ii" },
  { "set_border_width", (void*) &lv_style_set_border_width, "", "(lv_style)ii" },
  { "set_clip_corner", (void*) &lv_style_set_clip_corner, "", "(lv_style)ib" },
  { "set_image_blend_mode", (void*) &lv_style_set_image_blend_mode, "", "(lv_style)ii" },
  { "set_image_opa", (void*) &lv_style_set_image_opa, "", "(lv_style)ii" },
  { "set_image_recolor", (void*) &lv_style_set_image_recolor, "", "(lv_style)i(lv_color)" },
  { "set_image_recolor_opa", (void*) &lv_style_set_image_recolor_opa, "", "(lv_style)ii" },
  { "set_line_blend_mode", (void*) &lv_style_set_line_blend_mode, "", "(lv_style)ii" },
  { "set_line_color", (void*) &lv_style_set_line_color, "", "(lv_style)i(lv_color)" },
  { "set_line_dash_gap", (void*) &lv_style_set_line_dash_gap, "", "(lv_style)ii" },
  { "set_line_dash_width", (void*) &lv_style_set_line_dash_width, "", "(lv_style)ii" },
  { "set_line_opa", (void*) &lv_style_set_line_opa, "", "(lv_style)ii" },
  { "set_line_rounded", (void*) &lv_style_set_line_rounded, "", "(lv_style)ib" },
  { "set_line_width", (void*) &lv_style_set_line_width, "", "(lv_style)ii" },
  { "set_margin_bottom", (void*) &lv_style_set_margin_bottom, "", "(lv_style)ii" },
  { "set_margin_left", (void*) &lv_style_set_margin_left, "", "(lv_style)ii" },
  { "set_margin_right", (void*) &lv_style_set_margin_right, "", "(lv_style)ii" },
  { "set_margin_top", (void*) &lv_style_set_margin_top, "", "(lv_style)ii" },
  { "set_opa_scale", (void*) &lv_style_set_opa_scale, "", "(lv_style)ii" },
  { "set_outline_blend_mode", (void*) &lv_style_set_outline_blend_mode, "", "(lv_style)ii" },
  { "set_outline_color", (void*) &lv_style_set_outline_color, "", "(lv_style)i(lv_color)" },
  { "set_outline_opa", (void*) &lv_style_set_outline_opa, "", "(lv_style)ii" },
  { "set_outline_pad", (void*) &lv_style_set_outline_pad, "", "(lv_style)ii" },
  { "set_outline_width", (void*) &lv_style_set_outline_width, "", "(lv_style)ii" },
  { "set_pad_bottom", (void*) &lv_style_set_pad_bottom, "", "(lv_style)ii" },
  { "set_pad_inner", (void*) &lv_style_set_pad_inner, "", "(lv_style)ii" },
  { "set_pad_left", (void*) &lv_style_set_pad_left, "", "(lv_style)ii" },
  { "set_pad_right", (void*) &lv_style_set_pad_right, "", "(lv_style)ii" },
  { "set_pad_top", (void*) &lv_style_set_pad_top, "", "(lv_style)ii" },
  { "set_pattern_blend_mode", (void*) &lv_style_set_pattern_blend_mode, "", "(lv_style)ii" },
  { "set_pattern_image", (void*) &lv_style_set_pattern_image, "", "(lv_style)i." },
  { "set_pattern_opa", (void*) &lv_style_set_pattern_opa, "", "(lv_style)ii" },
  { "set_pattern_recolor", (void*) &lv_style_set_pattern_recolor, "", "(lv_style)i(lv_color)" },
  { "set_pattern_recolor_opa", (void*) &lv_style_set_pattern_recolor_opa, "", "(lv_style)ii" },
  { "set_pattern_repeat", (void*) &lv_style_set_pattern_repeat, "", "(lv_style)ib" },
  { "set_radius", (void*) &lv_style_set_radius, "", "(lv_style)ii" },
  { "set_scale_border_width", (void*) &lv_style_set_scale_border_width, "", "(lv_style)ii" },
  { "set_scale_end_border_width", (void*) &lv_style_set_scale_end_border_width, "", "(lv_style)ii" },
  { "set_scale_end_color", (void*) &lv_style_set_scale_end_color, "", "(lv_style)i(lv_color)" },
  { "set_scale_end_line_width", (void*) &lv_style_set_scale_end_line_width, "", "(lv_style)ii" },
  { "set_scale_grad_color", (void*) &lv_style_set_scale_grad_color, "", "(lv_style)i(lv_color)" },
  { "set_scale_width", (void*) &lv_style_set_scale_width, "", "(lv_style)ii" },
  { "set_shadow_blend_mode", (void*) &lv_style_set_shadow_blend_mode, "", "(lv_style)ii" },
  { "set_shadow_color", (void*) &lv_style_set_shadow_color, "", "(lv_style)i(lv_color)" },
  { "set_shadow_ofs_x", (void*) &lv_style_set_shadow_ofs_x, "", "(lv_style)ii" },
  { "set_shadow_ofs_y", (void*) &lv_style_set_shadow_ofs_y, "", "(lv_style)ii" },
  { "set_shadow_opa", (void*) &lv_style_set_shadow_opa, "", "(lv_style)ii" },
  { "set_shadow_spread", (void*) &lv_style_set_shadow_spread, "", "(lv_style)ii" },
  { "set_shadow_width", (void*) &lv_style_set_shadow_width, "", "(lv_style)ii" },
  { "set_size", (void*) &lv_style_set_size, "", "(lv_style)ii" },
  { "set_text_blend_mode", (void*) &lv_style_set_text_blend_mode, "", "(lv_style)ii" },
  { "set_text_color", (void*) &lv_style_set_text_color, "", "(lv_style)i(lv_color)" },
  { "set_text_decor", (void*) &lv_style_set_text_decor, "", "(lv_style)i(lv_text_decor)" },
  { "set_text_font", (void*) &lv_style_set_text_font, "", "(lv_style)i(lv_font)" },
  { "set_text_letter_space", (void*) &lv_style_set_text_letter_space, "", "(lv_style)ii" },
  { "set_text_line_space", (void*) &lv_style_set_text_line_space, "", "(lv_style)ii" },
  { "set_text_opa", (void*) &lv_style_set_text_opa, "", "(lv_style)ii" },
  { "set_text_sel_bg_color", (void*) &lv_style_set_text_sel_bg_color, "", "(lv_style)i(lv_color)" },
  { "set_text_sel_color", (void*) &lv_style_set_text_sel_color, "", "(lv_style)i(lv_color)" },
  { "set_transform_angle", (void*) &lv_style_set_transform_angle, "", "(lv_style)ii" },
  { "set_transform_height", (void*) &lv_style_set_transform_height, "", "(lv_style)ii" },
  { "set_transform_width", (void*) &lv_style_set_transform_width, "", "(lv_style)ii" },
  { "set_transform_zoom", (void*) &lv_style_set_transform_zoom, "", "(lv_style)ii" },
  { "set_transition_delay", (void*) &lv_style_set_transition_delay, "", "(lv_style)ii" },
  { "set_transition_path", (void*) &lv_style_set_transition_path, "", "(lv_style)i(lv_anim_path)" },
  { "set_transition_prop_1", (void*) &lv_style_set_transition_prop_1, "", "(lv_style)ii" },
  { "set_transition_prop_2", (void*) &lv_style_set_transition_prop_2, "", "(lv_style)ii" },
  { "set_transition_prop_3", (void*) &lv_style_set_transition_prop_3, "", "(lv_style)ii" },
  { "set_transition_prop_4", (void*) &lv_style_set_transition_prop_4, "", "(lv_style)ii" },
  { "set_transition_prop_5", (void*) &lv_style_set_transition_prop_5, "", "(lv_style)ii" },
  { "set_transition_prop_6", (void*) &lv_style_set_transition_prop_6, "", "(lv_style)ii" },
  { "set_transition_time", (void*) &lv_style_set_transition_time, "", "(lv_style)ii" },
  { "set_value_align", (void*) &lv_style_set_value_align, "", "(lv_style)ii" },
  { "set_value_blend_mode", (void*) &lv_style_set_value_blend_mode, "", "(lv_style)ii" },
  { "set_value_color", (void*) &lv_style_set_value_color, "", "(lv_style)i(lv_color)" },
  { "set_value_font", (void*) &lv_style_set_value_font, "", "(lv_style)i(lv_font)" },
  { "set_value_letter_space", (void*) &lv_style_set_value_letter_space, "", "(lv_style)ii" },
  { "set_value_line_space", (void*) &lv_style_set_value_line_space, "", "(lv_style)ii" },
  { "set_value_ofs_x", (void*) &lv_style_set_value_ofs_x, "", "(lv_style)ii" },
  { "set_value_ofs_y", (void*) &lv_style_set_value_ofs_y, "", "(lv_style)ii" },
  { "set_value_opa", (void*) &lv_style_set_value_opa, "", "(lv_style)ii" },
  { "set_value_str", (void*) &lv_style_set_value_str, "", "(lv_style)is" },
};

/* `lv_group` methods */
const lvbe_call_c_t lv_group_func[] = {
  { "add_obj", (void*) &lv_group_add_obj, "", "(lv_group)(lv_obj)" },
  { "create", (void*) &lv_group_create, "+lv_group", "" },
  { "del", (void*) &lv_group_del, "", "(lv_group)" },
  { "focus_freeze", (void*) &lv_group_focus_freeze, "", "(lv_group)b" },
  { "focus_next", (void*) &lv_group_focus_next, "", "(lv_group)" },
  { "focus_obj", (void*) &lv_group_focus_obj, "", "(lv_obj)" },
  { "focus_prev", (void*) &lv_group_focus_prev, "", "(lv_group)" },
  { "get_click_focus", (void*) &lv_group_get_click_focus, "b", "(lv_group)" },
  { "get_editing", (void*) &lv_group_get_editing, "b", "(lv_group)" },
  { "get_focused", (void*) &lv_group_get_focused, "lv_obj", "(lv_group)" },
  { "get_wrap", (void*) &lv_group_get_wrap, "b", "(lv_group)" },
  { "remove_all_objs", (void*) &lv_group_remove_all_objs, "", "(lv_group)" },
  { "remove_obj", (void*) &lv_group_remove_obj, "", "(lv_obj)" },
  { "send_data", (void*) &lv_group_send_data, "i", "(lv_group)i" },
  { "set_click_focus", (void*) &lv_group_set_click_focus, "", "(lv_group)b" },
  { "set_editing", (void*) &lv_group_set_editing, "", "(lv_group)b" },
  { "set_focus_cb", (void*) &lv_group_set_focus_cb, "", "(lv_group)&0" },
  { "set_refocus_policy", (void*) &lv_group_set_refocus_policy, "", "(lv_group)(lv_group_refocus_policy)" },
  { "set_wrap", (void*) &lv_group_set_wrap, "", "(lv_group)b" },
};

/* `lv_indev` methods */
const lvbe_call_c_t lv_indev_func[] = {
  { "enable", (void*) &lv_indev_enable, "", "(lv_indev)b" },
  { "get_obj_act", (void*) &lv_indev_get_obj_act, "lv_obj", "" },
  { "get_type", (void*) &lv_indev_get_type, "i", "(lv_indev)" },
  { "search_obj", (void*) &lv_indev_search_obj, "lv_obj", "(lv_obj)(lv_point)" },
  { "set_group", (void*) &lv_indev_set_group, "", "(lv_indev)(lv_group)" },
};

/* `lv_obj` methods */
const lvbe_call_c_t lv_obj_func[] = {
  { "add_protect", (void*) &lv_obj_add_protect, "", "(lv_obj)i" },
  { "add_state", (void*) &lv_obj_add_state, "", "(lv_obj)i" },
  { "add_style", (void*) &lv_obj_add_style, "", "(lv_obj)i(lv_style)" },
  { "align", (void*) &lv_obj_align, "", "(lv_obj)(lv_obj)iii" },
  { "align_mid", (void*) &lv_obj_align_mid, "", "(lv_obj)(lv_obj)iii" },
  { "align_mid_x", (void*) &lv_obj_align_mid_x, "", "(lv_obj)(lv_obj)ii" },
  { "align_mid_y", (void*) &lv_obj_align_mid_y, "", "(lv_obj)(lv_obj)ii" },
  { "align_x", (void*) &lv_obj_align_x, "", "(lv_obj)(lv_obj)ii" },
  { "align_y", (void*) &lv_obj_align_y, "", "(lv_obj)(lv_obj)ii" },
  { "allocate_ext_attr", (void*) &lv_obj_allocate_ext_attr, ".", "(lv_obj)i" },
  { "area_is_visible", (void*) &lv_obj_area_is_visible, "b", "(lv_obj)(lv_area)" },
  { "clean", (void*) &lv_obj_clean, "", "(lv_obj)" },
  { "clean_style_list", (void*) &lv_obj_clean_style_list, "", "(lv_obj)i" },
  { "clear_protect", (void*) &lv_obj_clear_protect, "", "(lv_obj)i" },
  { "clear_state", (void*) &lv_obj_clear_state, "", "(lv_obj)i" },
  { "count_children", (void*) &lv_obj_count_children, "i", "(lv_obj)" },
  { "count_children_recursive", (void*) &lv_obj_count_children_recursive, "i", "(lv_obj)" },
  { "create", (void*) &lv_obj_create, "+lv_obj", "(lv_obj)(lv_obj)" },
  { "del", (void*) &lv_obj_del, "i", "(lv_obj)" },
  { "del_anim_ready_cb", (void*) &lv_obj_del_anim_ready_cb, "", "(lv_anim)" },
  { "del_async", (void*) &lv_obj_del_async, "", "(_lv_obj)" },
  { "fade_in", (void*) &lv_obj_fade_in, "", "(lv_obj)ii" },
  { "fade_out", (void*) &lv_obj_fade_out, "", "(lv_obj)ii" },
  { "finish_transitions", (void*) &lv_obj_finish_transitions, "", "(lv_obj)i" },
  { "get_adv_hittest", (void*) &lv_obj_get_adv_hittest, "b", "(lv_obj)" },
  { "get_auto_realign", (void*) &lv_obj_get_auto_realign, "b", "(lv_obj)" },
  { "get_base_dir", (void*) &lv_obj_get_base_dir, "i", "(lv_obj)" },
  { "get_child", (void*) &lv_obj_get_child, "lv_obj", "(lv_obj)(lv_obj)" },
  { "get_child_back", (void*) &lv_obj_get_child_back, "lv_obj", "(lv_obj)(lv_obj)" },
  { "get_click", (void*) &lv_obj_get_click, "b", "(lv_obj)" },
  { "get_coords", (void*) &lv_obj_get_coords, "", "(lv_obj)(lv_area)" },
  { "get_drag", (void*) &lv_obj_get_drag, "b", "(lv_obj)" },
  { "get_drag_dir", (void*) &lv_obj_get_drag_dir, "i", "(lv_obj)" },
  { "get_drag_parent", (void*) &lv_obj_get_drag_parent, "b", "(lv_obj)" },
  { "get_drag_throw", (void*) &lv_obj_get_drag_throw, "b", "(lv_obj)" },
  { "get_draw_rect_ext_pad_size", (void*) &lv_obj_get_draw_rect_ext_pad_size, "i", "(lv_obj)i" },
  { "get_ext_attr", (void*) &lv_obj_get_ext_attr, ".", "(lv_obj)" },
  { "get_ext_click_pad_bottom", (void*) &lv_obj_get_ext_click_pad_bottom, "i", "(lv_obj)" },
  { "get_ext_click_pad_left", (void*) &lv_obj_get_ext_click_pad_left, "i", "(lv_obj)" },
  { "get_ext_click_pad_right", (void*) &lv_obj_get_ext_click_pad_right, "i", "(lv_obj)" },
  { "get_ext_click_pad_top", (void*) &lv_obj_get_ext_click_pad_top, "i", "(lv_obj)" },
  { "get_ext_draw_pad", (void*) &lv_obj_get_ext_draw_pad, "i", "(lv_obj)" },
  { "get_focus_parent", (void*) &lv_obj_get_focus_parent, "b", "(lv_obj)" },
  { "get_focused_obj", (void*) &lv_obj_get_focused_obj, "lv_obj", "(lv_obj)" },
  { "get_gesture_parent", (void*) &lv_obj_get_gesture_parent, "b", "(lv_obj)" },
  { "get_group", (void*) &lv_obj_get_group, ".", "(lv_obj)" },
  { "get_height", (void*) &lv_obj_get_height, "i", "(lv_obj)" },
  { "get_height_fit", (void*) &lv_obj_get_height_fit, "i", "(lv_obj)" },
  { "get_height_grid", (void*) &lv_obj_get_height_grid, "i", "(lv_obj)ii" },
  { "get_height_margin", (void*) &lv_obj_get_height_margin, "i", "(lv_obj)" },
  { "get_hidden", (void*) &lv_obj_get_hidden, "b", "(lv_obj)" },
  { "get_inner_coords", (void*) &lv_obj_get_inner_coords, "", "(lv_obj)(lv_area)" },
  { "get_local_style", (void*) &lv_obj_get_local_style, "lv_style", "(lv_obj)i" },
  { "get_parent", (void*) &lv_obj_get_parent, "lv_obj", "(lv_obj)" },
  { "get_parent_event", (void*) &lv_obj_get_parent_event, "b", "(lv_obj)" },
  { "get_protect", (void*) &lv_obj_get_protect, "i", "(lv_obj)" },
  { "get_screen", (void*) &lv_obj_get_screen, "lv_obj", "(lv_obj)" },
  { "get_state", (void*) &lv_obj_get_state, "i", "(lv_obj)i" },
  { "get_style_bg_blend_mode", (void*) &lv_obj_get_style_bg_blend_mode, "i", "(lv_obj)i" },
  { "get_style_bg_color", (void*) &lv_obj_get_style_bg_color, "lv_color", "(lv_obj)i" },
  { "get_style_bg_grad_color", (void*) &lv_obj_get_style_bg_grad_color, "lv_color", "(lv_obj)i" },
  { "get_style_bg_grad_dir", (void*) &lv_obj_get_style_bg_grad_dir, "i", "(lv_obj)i" },
  { "get_style_bg_grad_stop", (void*) &lv_obj_get_style_bg_grad_stop, "i", "(lv_obj)i" },
  { "get_style_bg_main_stop", (void*) &lv_obj_get_style_bg_main_stop, "i", "(lv_obj)i" },
  { "get_style_bg_opa", (void*) &lv_obj_get_style_bg_opa, "i", "(lv_obj)i" },
  { "get_style_border_blend_mode", (void*) &lv_obj_get_style_border_blend_mode, "i", "(lv_obj)i" },
  { "get_style_border_color", (void*) &lv_obj_get_style_border_color, "lv_color", "(lv_obj)i" },
  { "get_style_border_opa", (void*) &lv_obj_get_style_border_opa, "i", "(lv_obj)i" },
  { "get_style_border_post", (void*) &lv_obj_get_style_border_post, "b", "(lv_obj)i" },
  { "get_style_border_side", (void*) &lv_obj_get_style_border_side, "i", "(lv_obj)i" },
  { "get_style_border_width", (void*) &lv_obj_get_style_border_width, "i", "(lv_obj)i" },
  { "get_style_clip_corner", (void*) &lv_obj_get_style_clip_corner, "b", "(lv_obj)i" },
  { "get_style_image_blend_mode", (void*) &lv_obj_get_style_image_blend_mode, "i", "(lv_obj)i" },
  { "get_style_image_opa", (void*) &lv_obj_get_style_image_opa, "i", "(lv_obj)i" },
  { "get_style_image_recolor", (void*) &lv_obj_get_style_image_recolor, "lv_color", "(lv_obj)i" },
  { "get_style_image_recolor_opa", (void*) &lv_obj_get_style_image_recolor_opa, "i", "(lv_obj)i" },
  { "get_style_line_blend_mode", (void*) &lv_obj_get_style_line_blend_mode, "i", "(lv_obj)i" },
  { "get_style_line_color", (void*) &lv_obj_get_style_line_color, "lv_color", "(lv_obj)i" },
  { "get_style_line_dash_gap", (void*) &lv_obj_get_style_line_dash_gap, "i", "(lv_obj)i" },
  { "get_style_line_dash_width", (void*) &lv_obj_get_style_line_dash_width, "i", "(lv_obj)i" },
  { "get_style_line_opa", (void*) &lv_obj_get_style_line_opa, "i", "(lv_obj)i" },
  { "get_style_line_rounded", (void*) &lv_obj_get_style_line_rounded, "b", "(lv_obj)i" },
  { "get_style_line_width", (void*) &lv_obj_get_style_line_width, "i", "(lv_obj)i" },
  { "get_style_margin_bottom", (void*) &lv_obj_get_style_margin_bottom, "i", "(lv_obj)i" },
  { "get_style_margin_left", (void*) &lv_obj_get_style_margin_left, "i", "(lv_obj)i" },
  { "get_style_margin_right", (void*) &lv_obj_get_style_margin_right, "i", "(lv_obj)i" },
  { "get_style_margin_top", (void*) &lv_obj_get_style_margin_top, "i", "(lv_obj)i" },
  { "get_style_opa_scale", (void*) &lv_obj_get_style_opa_scale, "i", "(lv_obj)i" },
  { "get_style_outline_blend_mode", (void*) &lv_obj_get_style_outline_blend_mode, "i", "(lv_obj)i" },
  { "get_style_outline_color", (void*) &lv_obj_get_style_outline_color, "lv_color", "(lv_obj)i" },
  { "get_style_outline_opa", (void*) &lv_obj_get_style_outline_opa, "i", "(lv_obj)i" },
  { "get_style_outline_pad", (void*) &lv_obj_get_style_outline_pad, "i", "(lv_obj)i" },
  { "get_style_outline_width", (void*) &lv_obj_get_style_outline_width, "i", "(lv_obj)i" },
  { "get_style_pad_bottom", (void*) &lv_obj_get_style_pad_bottom, "i", "(lv_obj)i" },
  { "get_style_pad_inner", (void*) &lv_obj_get_style_pad_inner, "i", "(lv_obj)i" },
  { "get_style_pad_left", (void*) &lv_obj_get_style_pad_left, "i", "(lv_obj)i" },
  { "get_style_pad_right", (void*) &lv_obj_get_style_pad_right, "i", "(lv_obj)i" },
  { "get_style_pad_top", (void*) &lv_obj_get_style_pad_top, "i", "(lv_obj)i" },
  { "get_style_pattern_blend_mode", (void*) &lv_obj_get_style_pattern_blend_mode, "i", "(lv_obj)i" },
  { "get_style_pattern_image", (void*) &lv_obj_get_style_pattern_image, ".", "(lv_obj)i" },
  { "get_style_pattern_opa", (void*) &lv_obj_get_style_pattern_opa, "i", "(lv_obj)i" },
  { "get_style_pattern_recolor", (void*) &lv_obj_get_style_pattern_recolor, "lv_color", "(lv_obj)i" },
  { "get_style_pattern_recolor_opa", (void*) &lv_obj_get_style_pattern_recolor_opa, "i", "(lv_obj)i" },
  { "get_style_pattern_repeat", (void*) &lv_obj_get_style_pattern_repeat, "b", "(lv_obj)i" },
  { "get_style_radius", (void*) &lv_obj_get_style_radius, "i", "(lv_obj)i" },
  { "get_style_scale_border_width", (void*) &lv_obj_get_style_scale_border_width, "i", "(lv_obj)i" },
  { "get_style_scale_end_border_width", (void*) &lv_obj_get_style_scale_end_border_width, "i", "(lv_obj)i" },
  { "get_style_scale_end_color", (void*) &lv_obj_get_style_scale_end_color, "lv_color", "(lv_obj)i" },
  { "get_style_scale_end_line_width", (void*) &lv_obj_get_style_scale_end_line_width, "i", "(lv_obj)i" },
  { "get_style_scale_grad_color", (void*) &lv_obj_get_style_scale_grad_color, "lv_color", "(lv_obj)i" },
  { "get_style_scale_width", (void*) &lv_obj_get_style_scale_width, "i", "(lv_obj)i" },
  { "get_style_shadow_blend_mode", (void*) &lv_obj_get_style_shadow_blend_mode, "i", "(lv_obj)i" },
  { "get_style_shadow_color", (void*) &lv_obj_get_style_shadow_color, "lv_color", "(lv_obj)i" },
  { "get_style_shadow_ofs_x", (void*) &lv_obj_get_style_shadow_ofs_x, "i", "(lv_obj)i" },
  { "get_style_shadow_ofs_y", (void*) &lv_obj_get_style_shadow_ofs_y, "i", "(lv_obj)i" },
  { "get_style_shadow_opa", (void*) &lv_obj_get_style_shadow_opa, "i", "(lv_obj)i" },
  { "get_style_shadow_spread", (void*) &lv_obj_get_style_shadow_spread, "i", "(lv_obj)i" },
  { "get_style_shadow_width", (void*) &lv_obj_get_style_shadow_width, "i", "(lv_obj)i" },
  { "get_style_size", (void*) &lv_obj_get_style_size, "i", "(lv_obj)i" },
  { "get_style_text_blend_mode", (void*) &lv_obj_get_style_text_blend_mode, "i", "(lv_obj)i" },
  { "get_style_text_color", (void*) &lv_obj_get_style_text_color, "lv_color", "(lv_obj)i" },
  { "get_style_text_letter_space", (void*) &lv_obj_get_style_text_letter_space, "i", "(lv_obj)i" },
  { "get_style_text_line_space", (void*) &lv_obj_get_style_text_line_space, "i", "(lv_obj)i" },
  { "get_style_text_opa", (void*) &lv_obj_get_style_text_opa, "i", "(lv_obj)i" },
  { "get_style_text_sel_bg_color", (void*) &lv_obj_get_style_text_sel_bg_color, "lv_color", "(lv_obj)i" },
  { "get_style_text_sel_color", (void*) &lv_obj_get_style_text_sel_color, "lv_color", "(lv_obj)i" },
  { "get_style_transform_angle", (void*) &lv_obj_get_style_transform_angle, "i", "(lv_obj)i" },
  { "get_style_transform_height", (void*) &lv_obj_get_style_transform_height, "i", "(lv_obj)i" },
  { "get_style_transform_width", (void*) &lv_obj_get_style_transform_width, "i", "(lv_obj)i" },
  { "get_style_transform_zoom", (void*) &lv_obj_get_style_transform_zoom, "i", "(lv_obj)i" },
  { "get_style_transition_delay", (void*) &lv_obj_get_style_transition_delay, "i", "(lv_obj)i" },
  { "get_style_transition_prop_1", (void*) &lv_obj_get_style_transition_prop_1, "i", "(lv_obj)i" },
  { "get_style_transition_prop_2", (void*) &lv_obj_get_style_transition_prop_2, "i", "(lv_obj)i" },
  { "get_style_transition_prop_3", (void*) &lv_obj_get_style_transition_prop_3, "i", "(lv_obj)i" },
  { "get_style_transition_prop_4", (void*) &lv_obj_get_style_transition_prop_4, "i", "(lv_obj)i" },
  { "get_style_transition_prop_5", (void*) &lv_obj_get_style_transition_prop_5, "i", "(lv_obj)i" },
  { "get_style_transition_prop_6", (void*) &lv_obj_get_style_transition_prop_6, "i", "(lv_obj)i" },
  { "get_style_transition_time", (void*) &lv_obj_get_style_transition_time, "i", "(lv_obj)i" },
  { "get_style_value_align", (void*) &lv_obj_get_style_value_align, "i", "(lv_obj)i" },
  { "get_style_value_blend_mode", (void*) &lv_obj_get_style_value_blend_mode, "i", "(lv_obj)i" },
  { "get_style_value_color", (void*) &lv_obj_get_style_value_color, "lv_color", "(lv_obj)i" },
  { "get_style_value_letter_space", (void*) &lv_obj_get_style_value_letter_space, "i", "(lv_obj)i" },
  { "get_style_value_line_space", (void*) &lv_obj_get_style_value_line_space, "i", "(lv_obj)i" },
  { "get_style_value_ofs_x", (void*) &lv_obj_get_style_value_ofs_x, "i", "(lv_obj)i" },
  { "get_style_value_ofs_y", (void*) &lv_obj_get_style_value_ofs_y, "i", "(lv_obj)i" },
  { "get_style_value_opa", (void*) &lv_obj_get_style_value_opa, "i", "(lv_obj)i" },
  { "get_style_value_str", (void*) &lv_obj_get_style_value_str, "s", "(lv_obj)i" },
  { "get_top", (void*) &lv_obj_get_top, "b", "(lv_obj)" },
  { "get_type", (void*) &lv_obj_get_type, "", "(lv_obj)(lv_obj_type)" },
  { "get_user_data", (void*) &lv_obj_get_user_data, "i", "(lv_obj)" },
  { "get_width", (void*) &lv_obj_get_width, "i", "(lv_obj)" },
  { "get_width_fit", (void*) &lv_obj_get_width_fit, "i", "(lv_obj)" },
  { "get_width_grid", (void*) &lv_obj_get_width_grid, "i", "(lv_obj)ii" },
  { "get_width_margin", (void*) &lv_obj_get_width_margin, "i", "(lv_obj)" },
  { "get_x", (void*) &lv_obj_get_x, "i", "(lv_obj)" },
  { "get_y", (void*) &lv_obj_get_y, "i", "(lv_obj)" },
  { "handle_get_type_signal", (void*) &lv_obj_handle_get_type_signal, "i", "(lv_obj_type)s" },
  { "hittest", (void*) &lv_obj_hittest, "b", "(lv_obj)(lv_point)" },
  { "init_draw_img_dsc", (void*) &lv_obj_init_draw_img_dsc, "", "(lv_obj)i(lv_draw_img_dsc)" },
  { "init_draw_label_dsc", (void*) &lv_obj_init_draw_label_dsc, "", "(lv_obj)i(lv_draw_label_dsc)" },
  { "init_draw_line_dsc", (void*) &lv_obj_init_draw_line_dsc, "", "(lv_obj)i(lv_draw_line_dsc)" },
  { "init_draw_rect_dsc", (void*) &lv_obj_init_draw_rect_dsc, "", "(lv_obj)i(lv_draw_rect_dsc)" },
  { "invalidate", (void*) &lv_obj_invalidate, "", "(lv_obj)" },
  { "invalidate_area", (void*) &lv_obj_invalidate_area, "", "(lv_obj)(lv_area)" },
  { "is_focused", (void*) &lv_obj_is_focused, "b", "(lv_obj)" },
  { "is_point_on_coords", (void*) &lv_obj_is_point_on_coords, "b", "(lv_obj)(lv_point)" },
  { "is_protected", (void*) &lv_obj_is_protected, "b", "(lv_obj)i" },
  { "is_visible", (void*) &lv_obj_is_visible, "b", "(lv_obj)" },
  { "move_background", (void*) &lv_obj_move_background, "", "(lv_obj)" },
  { "move_foreground", (void*) &lv_obj_move_foreground, "", "(lv_obj)" },
  { "realign", (void*) &lv_obj_realign, "", "(lv_obj)" },
  { "refresh_ext_draw_pad", (void*) &lv_obj_refresh_ext_draw_pad, "", "(lv_obj)" },
  { "refresh_style", (void*) &lv_obj_refresh_style, "", "(lv_obj)i(lv_style_property)" },
  { "remove_style", (void*) &lv_obj_remove_style, "", "(lv_obj)i(lv_style)" },
  { "remove_style_local_prop", (void*) &lv_obj_remove_style_local_prop, "b", "(lv_obj)i(lv_style_property)" },
  { "report_style_mod", (void*) &lv_obj_report_style_mod, "", "(lv_style)" },
  { "reset_style_list", (void*) &lv_obj_reset_style_list, "", "(lv_obj)i" },
  { "set_adv_hittest", (void*) &lv_obj_set_adv_hittest, "", "(lv_obj)b" },
  { "set_auto_realign", (void*) &lv_obj_set_auto_realign, "", "(lv_obj)b" },
  { "set_base_dir", (void*) &lv_obj_set_base_dir, "", "(lv_obj)i" },
  { "set_click", (void*) &lv_obj_set_click, "", "(lv_obj)b" },
  { "set_design_cb", (void*) &lv_obj_set_design_cb, "", "(lv_obj)&3" },
  { "set_drag", (void*) &lv_obj_set_drag, "", "(lv_obj)b" },
  { "set_drag_dir", (void*) &lv_obj_set_drag_dir, "", "(lv_obj)i" },
  { "set_drag_parent", (void*) &lv_obj_set_drag_parent, "", "(lv_obj)b" },
  { "set_drag_throw", (void*) &lv_obj_set_drag_throw, "", "(lv_obj)b" },
  { "set_event_cb", (void*) &lv_obj_set_event_cb, "", "(lv_obj)&1" },
  { "set_ext_click_area", (void*) &lv_obj_set_ext_click_area, "", "(lv_obj)iiii" },
  { "set_focus_parent", (void*) &lv_obj_set_focus_parent, "", "(lv_obj)b" },
  { "set_gesture_parent", (void*) &lv_obj_set_gesture_parent, "", "(lv_obj)b" },
  { "set_height", (void*) &lv_obj_set_height, "", "(lv_obj)i" },
  { "set_height_fit", (void*) &lv_obj_set_height_fit, "", "(lv_obj)i" },
  { "set_height_margin", (void*) &lv_obj_set_height_margin, "", "(lv_obj)i" },
  { "set_hidden", (void*) &lv_obj_set_hidden, "", "(lv_obj)b" },
  { "set_parent", (void*) &lv_obj_set_parent, "", "(lv_obj)(lv_obj)" },
  { "set_parent_event", (void*) &lv_obj_set_parent_event, "", "(lv_obj)b" },
  { "set_pos", (void*) &lv_obj_set_pos, "", "(lv_obj)ii" },
  { "set_signal_cb", (void*) &lv_obj_set_signal_cb, "", "(lv_obj)&2" },
  { "set_size", (void*) &lv_obj_set_size, "", "(lv_obj)ii" },
  { "set_state", (void*) &lv_obj_set_state, "", "(lv_obj)i" },
  { "set_style_local_bg_blend_mode", (void*) &lv_obj_set_style_local_bg_blend_mode, "", "(lv_obj)iii" },
  { "set_style_local_bg_color", (void*) &lv_obj_set_style_local_bg_color, "", "(lv_obj)ii(lv_color)" },
  { "set_style_local_bg_grad_color", (void*) &lv_obj_set_style_local_bg_grad_color, "", "(lv_obj)ii(lv_color)" },
  { "set_style_local_bg_grad_dir", (void*) &lv_obj_set_style_local_bg_grad_dir, "", "(lv_obj)iii" },
  { "set_style_local_bg_grad_stop", (void*) &lv_obj_set_style_local_bg_grad_stop, "", "(lv_obj)iii" },
  { "set_style_local_bg_main_stop", (void*) &lv_obj_set_style_local_bg_main_stop, "", "(lv_obj)iii" },
  { "set_style_local_bg_opa", (void*) &lv_obj_set_style_local_bg_opa, "", "(lv_obj)iii" },
  { "set_style_local_border_blend_mode", (void*) &lv_obj_set_style_local_border_blend_mode, "", "(lv_obj)iii" },
  { "set_style_local_border_color", (void*) &lv_obj_set_style_local_border_color, "", "(lv_obj)ii(lv_color)" },
  { "set_style_local_border_opa", (void*) &lv_obj_set_style_local_border_opa, "", "(lv_obj)iii" },
  { "set_style_local_border_post", (void*) &lv_obj_set_style_local_border_post, "", "(lv_obj)iib" },
  { "set_style_local_border_side", (void*) &lv_obj_set_style_local_border_side, "", "(lv_obj)iii" },
  { "set_style_local_border_width", (void*) &lv_obj_set_style_local_border_width, "", "(lv_obj)iii" },
  { "set_style_local_clip_corner", (void*) &lv_obj_set_style_local_clip_corner, "", "(lv_obj)iib" },
  { "set_style_local_image_blend_mode", (void*) &lv_obj_set_style_local_image_blend_mode, "", "(lv_obj)iii" },
  { "set_style_local_image_opa", (void*) &lv_obj_set_style_local_image_opa, "", "(lv_obj)iii" },
  { "set_style_local_image_recolor", (void*) &lv_obj_set_style_local_image_recolor, "", "(lv_obj)ii(lv_color)" },
  { "set_style_local_image_recolor_opa", (void*) &lv_obj_set_style_local_image_recolor_opa, "", "(lv_obj)iii" },
  { "set_style_local_line_blend_mode", (void*) &lv_obj_set_style_local_line_blend_mode, "", "(lv_obj)iii" },
  { "set_style_local_line_color", (void*) &lv_obj_set_style_local_line_color, "", "(lv_obj)ii(lv_color)" },
  { "set_style_local_line_dash_gap", (void*) &lv_obj_set_style_local_line_dash_gap, "", "(lv_obj)iii" },
  { "set_style_local_line_dash_width", (void*) &lv_obj_set_style_local_line_dash_width, "", "(lv_obj)iii" },
  { "set_style_local_line_opa", (void*) &lv_obj_set_style_local_line_opa, "", "(lv_obj)iii" },
  { "set_style_local_line_rounded", (void*) &lv_obj_set_style_local_line_rounded, "", "(lv_obj)iib" },
  { "set_style_local_line_width", (void*) &lv_obj_set_style_local_line_width, "", "(lv_obj)iii" },
  { "set_style_local_margin_bottom", (void*) &lv_obj_set_style_local_margin_bottom, "", "(lv_obj)iii" },
  { "set_style_local_margin_left", (void*) &lv_obj_set_style_local_margin_left, "", "(lv_obj)iii" },
  { "set_style_local_margin_right", (void*) &lv_obj_set_style_local_margin_right, "", "(lv_obj)iii" },
  { "set_style_local_margin_top", (void*) &lv_obj_set_style_local_margin_top, "", "(lv_obj)iii" },
  { "set_style_local_opa_scale", (void*) &lv_obj_set_style_local_opa_scale, "", "(lv_obj)iii" },
  { "set_style_local_outline_blend_mode", (void*) &lv_obj_set_style_local_outline_blend_mode, "", "(lv_obj)iii" },
  { "set_style_local_outline_color", (void*) &lv_obj_set_style_local_outline_color, "", "(lv_obj)ii(lv_color)" },
  { "set_style_local_outline_opa", (void*) &lv_obj_set_style_local_outline_opa, "", "(lv_obj)iii" },
  { "set_style_local_outline_pad", (void*) &lv_obj_set_style_local_outline_pad, "", "(lv_obj)iii" },
  { "set_style_local_outline_width", (void*) &lv_obj_set_style_local_outline_width, "", "(lv_obj)iii" },
  { "set_style_local_pad_bottom", (void*) &lv_obj_set_style_local_pad_bottom, "", "(lv_obj)iii" },
  { "set_style_local_pad_inner", (void*) &lv_obj_set_style_local_pad_inner, "", "(lv_obj)iii" },
  { "set_style_local_pad_left", (void*) &lv_obj_set_style_local_pad_left, "", "(lv_obj)iii" },
  { "set_style_local_pad_right", (void*) &lv_obj_set_style_local_pad_right, "", "(lv_obj)iii" },
  { "set_style_local_pad_top", (void*) &lv_obj_set_style_local_pad_top, "", "(lv_obj)iii" },
  { "set_style_local_pattern_blend_mode", (void*) &lv_obj_set_style_local_pattern_blend_mode, "", "(lv_obj)iii" },
  { "set_style_local_pattern_image", (void*) &lv_obj_set_style_local_pattern_image, "", "(lv_obj)ii." },
  { "set_style_local_pattern_opa", (void*) &lv_obj_set_style_local_pattern_opa, "", "(lv_obj)iii" },
  { "set_style_local_pattern_recolor", (void*) &lv_obj_set_style_local_pattern_recolor, "", "(lv_obj)ii(lv_color)" },
  { "set_style_local_pattern_recolor_opa", (void*) &lv_obj_set_style_local_pattern_recolor_opa, "", "(lv_obj)iii" },
  { "set_style_local_pattern_repeat", (void*) &lv_obj_set_style_local_pattern_repeat, "", "(lv_obj)iib" },
  { "set_style_local_radius", (void*) &lv_obj_set_style_local_radius, "", "(lv_obj)iii" },
  { "set_style_local_scale_border_width", (void*) &lv_obj_set_style_local_scale_border_width, "", "(lv_obj)iii" },
  { "set_style_local_scale_end_border_width", (void*) &lv_obj_set_style_local_scale_end_border_width, "", "(lv_obj)iii" },
  { "set_style_local_scale_end_color", (void*) &lv_obj_set_style_local_scale_end_color, "", "(lv_obj)ii(lv_color)" },
  { "set_style_local_scale_end_line_width", (void*) &lv_obj_set_style_local_scale_end_line_width, "", "(lv_obj)iii" },
  { "set_style_local_scale_grad_color", (void*) &lv_obj_set_style_local_scale_grad_color, "", "(lv_obj)ii(lv_color)" },
  { "set_style_local_scale_width", (void*) &lv_obj_set_style_local_scale_width, "", "(lv_obj)iii" },
  { "set_style_local_shadow_blend_mode", (void*) &lv_obj_set_style_local_shadow_blend_mode, "", "(lv_obj)iii" },
  { "set_style_local_shadow_color", (void*) &lv_obj_set_style_local_shadow_color, "", "(lv_obj)ii(lv_color)" },
  { "set_style_local_shadow_ofs_x", (void*) &lv_obj_set_style_local_shadow_ofs_x, "", "(lv_obj)iii" },
  { "set_style_local_shadow_ofs_y", (void*) &lv_obj_set_style_local_shadow_ofs_y, "", "(lv_obj)iii" },
  { "set_style_local_shadow_opa", (void*) &lv_obj_set_style_local_shadow_opa, "", "(lv_obj)iii" },
  { "set_style_local_shadow_spread", (void*) &lv_obj_set_style_local_shadow_spread, "", "(lv_obj)iii" },
  { "set_style_local_shadow_width", (void*) &lv_obj_set_style_local_shadow_width, "", "(lv_obj)iii" },
  { "set_style_local_size", (void*) &lv_obj_set_style_local_size, "", "(lv_obj)iii" },
  { "set_style_local_text_blend_mode", (void*) &lv_obj_set_style_local_text_blend_mode, "", "(lv_obj)iii" },
  { "set_style_local_text_color", (void*) &lv_obj_set_style_local_text_color, "", "(lv_obj)ii(lv_color)" },
  { "set_style_local_text_decor", (void*) &lv_obj_set_style_local_text_decor, "", "(lv_obj)ii(lv_text_decor)" },
  { "set_style_local_text_font", (void*) &lv_obj_set_style_local_text_font, "", "(lv_obj)ii(lv_font)" },
  { "set_style_local_text_letter_space", (void*) &lv_obj_set_style_local_text_letter_space, "", "(lv_obj)iii" },
  { "set_style_local_text_line_space", (void*) &lv_obj_set_style_local_text_line_space, "", "(lv_obj)iii" },
  { "set_style_local_text_opa", (void*) &lv_obj_set_style_local_text_opa, "", "(lv_obj)iii" },
  { "set_style_local_text_sel_bg_color", (void*) &lv_obj_set_style_local_text_sel_bg_color, "", "(lv_obj)ii(lv_color)" },
  { "set_style_local_text_sel_color", (void*) &lv_obj_set_style_local_text_sel_color, "", "(lv_obj)ii(lv_color)" },
  { "set_style_local_transform_angle", (void*) &lv_obj_set_style_local_transform_angle, "", "(lv_obj)iii" },
  { "set_style_local_transform_height", (void*) &lv_obj_set_style_local_transform_height, "", "(lv_obj)iii" },
  { "set_style_local_transform_width", (void*) &lv_obj_set_style_local_transform_width, "", "(lv_obj)iii" },
  { "set_style_local_transform_zoom", (void*) &lv_obj_set_style_local_transform_zoom, "", "(lv_obj)iii" },
  { "set_style_local_transition_delay", (void*) &lv_obj_set_style_local_transition_delay, "", "(lv_obj)iii" },
  { "set_style_local_transition_path", (void*) &lv_obj_set_style_local_transition_path, "", "(lv_obj)ii(lv_anim_path)" },
  { "set_style_local_transition_prop_1", (void*) &lv_obj_set_style_local_transition_prop_1, "", "(lv_obj)iii" },
  { "set_style_local_transition_prop_2", (void*) &lv_obj_set_style_local_transition_prop_2, "", "(lv_obj)iii" },
  { "set_style_local_transition_prop_3", (void*) &lv_obj_set_style_local_transition_prop_3, "", "(lv_obj)iii" },
  { "set_style_local_transition_prop_4", (void*) &lv_obj_set_style_local_transition_prop_4, "", "(lv_obj)iii" },
  { "set_style_local_transition_prop_5", (void*) &lv_obj_set_style_local_transition_prop_5, "", "(lv_obj)iii" },
  { "set_style_local_transition_prop_6", (void*) &lv_obj_set_style_local_transition_prop_6, "", "(lv_obj)iii" },
  { "set_style_local_transition_time", (void*) &lv_obj_set_style_local_transition_time, "", "(lv_obj)iii" },
  { "set_style_local_value_align", (void*) &lv_obj_set_style_local_value_align, "", "(lv_obj)iii" },
  { "set_style_local_value_blend_mode", (void*) &lv_obj_set_style_local_value_blend_mode, "", "(lv_obj)iii" },
  { "set_style_local_value_color", (void*) &lv_obj_set_style_local_value_color, "", "(lv_obj)ii(lv_color)" },
  { "set_style_local_value_font", (void*) &lv_obj_set_style_local_value_font, "", "(lv_obj)ii(lv_font)" },
  { "set_style_local_value_letter_space", (void*) &lv_obj_set_style_local_value_letter_space, "", "(lv_obj)iii" },
  { "set_style_local_value_line_space", (void*) &lv_obj_set_style_local_value_line_space, "", "(lv_obj)iii" },
  { "set_style_local_value_ofs_x", (void*) &lv_obj_set_style_local_value_ofs_x, "", "(lv_obj)iii" },
  { "set_style_local_value_ofs_y", (void*) &lv_obj_set_style_local_value_ofs_y, "", "(lv_obj)iii" },
  { "set_style_local_value_opa", (void*) &lv_obj_set_style_local_value_opa, "", "(lv_obj)iii" },
  { "set_style_local_value_str", (void*) &lv_obj_set_style_local_value_str, "", "(lv_obj)iis" },
  { "set_top", (void*) &lv_obj_set_top, "", "(lv_obj)b" },
  { "set_user_data", (void*) &lv_obj_set_user_data, "", "(lv_obj)i" },
  { "set_width", (void*) &lv_obj_set_width, "", "(lv_obj)i" },
  { "set_width_fit", (void*) &lv_obj_set_width_fit, "", "(lv_obj)i" },
  { "set_width_margin", (void*) &lv_obj_set_width_margin, "", "(lv_obj)i" },
  { "set_x", (void*) &lv_obj_set_x, "", "(lv_obj)i" },
  { "set_y", (void*) &lv_obj_set_y, "", "(lv_obj)i" },
};

/* `lv_arc` methods */
#if BE_LV_WIDGET_ARC
const lvbe_call_c_t lv_arc_func[] = {
  { "create", (void*) &lv_arc_create, "+lv_arc", "(lv_obj)(lv_obj)" },
  { "get_adjustable", (void*) &lv_arc_get_adjustable, "b", "(lv_obj)" },
  { "get_angle_end", (void*) &lv_arc_get_angle_end, "i", "(lv_obj)" },
  { "get_angle_start", (void*) &lv_arc_get_angle_start, "i", "(lv_obj)" },
  { "get_bg_angle_end", (void*) &lv_arc_get_bg_angle_end, "i", "(lv_obj)" },
  { "get_bg_angle_start", (void*) &lv_arc_get_bg_angle_start, "i", "(lv_obj)" },
  { "get_max_value", (void*) &lv_arc_get_max_value, "i", "(lv_obj)" },
  { "get_min_value", (void*) &lv_arc_get_min_value, "i", "(lv_obj)" },
  { "get_type", (void*) &lv_arc_get_type, "i", "(lv_obj)" },
  { "get_value", (void*) &lv_arc_get_value, "i", "(lv_obj)" },
  { "is_dragged", (void*) &lv_arc_is_dragged, "b", "(lv_obj)" },
  { "set_adjustable", (void*) &lv_arc_set_adjustable, "", "(lv_obj)b" },
  { "set_angles", (void*) &lv_arc_set_angles, "", "(lv_obj)ii" },
  { "set_bg_angles", (void*) &lv_arc_set_bg_angles, "", "(lv_obj)ii" },
  { "set_bg_end_angle", (void*) &lv_arc_set_bg_end_angle, "", "(lv_obj)i" },
  { "set_bg_start_angle", (void*) &lv_arc_set_bg_start_angle, "", "(lv_obj)i" },
  { "set_chg_rate", (void*) &lv_arc_set_chg_rate, "", "(lv_obj)i" },
  { "set_end_angle", (void*) &lv_arc_set_end_angle, "", "(lv_obj)i" },
  { "set_range", (void*) &lv_arc_set_range, "", "(lv_obj)ii" },
  { "set_rotation", (void*) &lv_arc_set_rotation, "", "(lv_obj)i" },
  { "set_start_angle", (void*) &lv_arc_set_start_angle, "", "(lv_obj)i" },
  { "set_type", (void*) &lv_arc_set_type, "", "(lv_obj)i" },
  { "set_value", (void*) &lv_arc_set_value, "", "(lv_obj)i" },
};
#endif // BE_LV_WIDGET_ARC

/* `lv_bar` methods */
#if BE_LV_WIDGET_BAR
const lvbe_call_c_t lv_bar_func[] = {
  { "create", (void*) &lv_bar_create, "+lv_bar", "(lv_obj)(lv_obj)" },
  { "get_anim_time", (void*) &lv_bar_get_anim_time, "i", "(lv_obj)" },
  { "get_max_value", (void*) &lv_bar_get_max_value, "i", "(lv_obj)" },
  { "get_min_value", (void*) &lv_bar_get_min_value, "i", "(lv_obj)" },
  { "get_start_value", (void*) &lv_bar_get_start_value, "i", "(lv_obj)" },
  { "get_type", (void*) &lv_bar_get_type, "i", "(lv_obj)" },
  { "get_value", (void*) &lv_bar_get_value, "i", "(lv_obj)" },
  { "set_anim_time", (void*) &lv_bar_set_anim_time, "", "(lv_obj)i" },
  { "set_range", (void*) &lv_bar_set_range, "", "(lv_obj)ii" },
  { "set_start_value", (void*) &lv_bar_set_start_value, "", "(lv_obj)i(lv_anim_enable)" },
  { "set_type", (void*) &lv_bar_set_type, "", "(lv_obj)i" },
  { "set_value", (void*) &lv_bar_set_value, "", "(lv_obj)i(lv_anim_enable)" },
};
#endif // BE_LV_WIDGET_BAR

/* `lv_btn` methods */
#if BE_LV_WIDGET_BTN
const lvbe_call_c_t lv_btn_func[] = {
  { "create", (void*) &lv_btn_create, "+lv_btn", "(lv_obj)(lv_obj)" },
  { "get_checkable", (void*) &lv_btn_get_checkable, "b", "(lv_obj)" },
  { "get_fit_bottom", (void*) &lv_btn_get_fit_bottom, "i", "(lv_obj)" },
  { "get_fit_left", (void*) &lv_btn_get_fit_left, "i", "(lv_obj)" },
  { "get_fit_right", (void*) &lv_btn_get_fit_right, "i", "(lv_obj)" },
  { "get_fit_top", (void*) &lv_btn_get_fit_top, "i", "(lv_obj)" },
  { "get_layout", (void*) &lv_btn_get_layout, "i", "(lv_obj)" },
  { "get_state", (void*) &lv_btn_get_state, "i", "(lv_obj)" },
  { "set_checkable", (void*) &lv_btn_set_checkable, "", "(lv_obj)b" },
  { "set_fit", (void*) &lv_btn_set_fit, "", "(lv_obj)i" },
  { "set_fit2", (void*) &lv_btn_set_fit2, "", "(lv_obj)ii" },
  { "set_fit4", (void*) &lv_btn_set_fit4, "", "(lv_obj)iiii" },
  { "set_layout", (void*) &lv_btn_set_layout, "", "(lv_obj)i" },
  { "set_state", (void*) &lv_btn_set_state, "", "(lv_obj)i" },
  { "toggle", (void*) &lv_btn_toggle, "", "(lv_obj)" },
};
#endif // BE_LV_WIDGET_BTN

/* `lv_btnmatrix` methods */
#if BE_LV_WIDGET_BTNMATRIX
const lvbe_call_c_t lv_btnmatrix_func[] = {
  { "clear_btn_ctrl", (void*) &lv_btnmatrix_clear_btn_ctrl, "", "(lv_obj)i(lv_btnmatrix_ctrl)" },
  { "clear_btn_ctrl_all", (void*) &lv_btnmatrix_clear_btn_ctrl_all, "", "(lv_obj)(lv_btnmatrix_ctrl)" },
  { "create", (void*) &lv_btnmatrix_create, "+lv_btnmatrix", "(lv_obj)(lv_obj)" },
  { "get_active_btn", (void*) &lv_btnmatrix_get_active_btn, "i", "(lv_obj)" },
  { "get_active_btn_text", (void*) &lv_btnmatrix_get_active_btn_text, "s", "(lv_obj)" },
  { "get_align", (void*) &lv_btnmatrix_get_align, "i", "(lv_obj)" },
  { "get_btn_ctrl", (void*) &lv_btnmatrix_get_btn_ctrl, "b", "(lv_obj)i(lv_btnmatrix_ctrl)" },
  { "get_btn_text", (void*) &lv_btnmatrix_get_btn_text, "s", "(lv_obj)i" },
  { "get_focused_btn", (void*) &lv_btnmatrix_get_focused_btn, "i", "(lv_obj)" },
  { "get_one_check", (void*) &lv_btnmatrix_get_one_check, "b", "(lv_obj)" },
  { "get_recolor", (void*) &lv_btnmatrix_get_recolor, "b", "(lv_obj)" },
  { "set_align", (void*) &lv_btnmatrix_set_align, "", "(lv_obj)i" },
  { "set_btn_ctrl", (void*) &lv_btnmatrix_set_btn_ctrl, "", "(lv_obj)i(lv_btnmatrix_ctrl)" },
  { "set_btn_ctrl_all", (void*) &lv_btnmatrix_set_btn_ctrl_all, "", "(lv_obj)(lv_btnmatrix_ctrl)" },
  { "set_btn_width", (void*) &lv_btnmatrix_set_btn_width, "", "(lv_obj)ii" },
  { "set_ctrl_map", (void*) &lv_btnmatrix_set_ctrl_map, "", "(lv_obj)(lv_btnmatrix_ctrl)" },
  { "set_focused_btn", (void*) &lv_btnmatrix_set_focused_btn, "", "(lv_obj)i" },
  { "set_map", (void*) &lv_btnmatrix_set_map, "", "(lv_obj)s" },
  { "set_one_check", (void*) &lv_btnmatrix_set_one_check, "", "(lv_obj)b" },
  { "set_recolor", (void*) &lv_btnmatrix_set_recolor, "", "(lv_obj)b" },
};
#endif // BE_LV_WIDGET_BTNMATRIX

/* `lv_calendar` methods */
#if BE_LV_WIDGET_CALENDAR
const lvbe_call_c_t lv_calendar_func[] = {
  { "create", (void*) &lv_calendar_create, "+lv_calendar", "(lv_obj)(lv_obj)" },
  { "get_day_of_week", (void*) &lv_calendar_get_day_of_week, "i", "iii" },
  { "get_highlighted_dates_num", (void*) &lv_calendar_get_highlighted_dates_num, "i", "(lv_obj)" },
  { "set_day_names", (void*) &lv_calendar_set_day_names, "", "(lv_obj)(char)" },
  { "set_highlighted_dates", (void*) &lv_calendar_set_highlighted_dates, "", "(lv_obj)(lv_calendar_date)i" },
  { "set_month_names", (void*) &lv_calendar_set_month_names, "", "(lv_obj)(char)" },
  { "set_showed_date", (void*) &lv_calendar_set_showed_date, "", "(lv_obj)(lv_calendar_date)" },
  { "set_today_date", (void*) &lv_calendar_set_today_date, "", "(lv_obj)(lv_calendar_date)" },
};
#endif // BE_LV_WIDGET_CALENDAR

/* `lv_canvas` methods */
#if BE_LV_WIDGET_CANVAS
const lvbe_call_c_t lv_canvas_func[] = {
  { "blur_hor", (void*) &lv_canvas_blur_hor, "", "(lv_obj)(lv_area)i" },
  { "blur_ver", (void*) &lv_canvas_blur_ver, "", "(lv_obj)(lv_area)i" },
  { "copy_buf", (void*) &lv_canvas_copy_buf, "", "(lv_obj).iiii" },
  { "create", (void*) &lv_canvas_create, "+lv_canvas", "(lv_obj)(lv_obj)" },
  { "draw_arc", (void*) &lv_canvas_draw_arc, "", "(lv_obj)iiiii(lv_draw_line_dsc)" },
  { "draw_img", (void*) &lv_canvas_draw_img, "", "(lv_obj)ii.(lv_draw_img_dsc)" },
  { "draw_line", (void*) &lv_canvas_draw_line, "", "(lv_obj)ii(lv_draw_line_dsc)" },
  { "draw_polygon", (void*) &lv_canvas_draw_polygon, "", "(lv_obj)ii(lv_draw_rect_dsc)" },
  { "draw_rect", (void*) &lv_canvas_draw_rect, "", "(lv_obj)iiii(lv_draw_rect_dsc)" },
  { "draw_text", (void*) &lv_canvas_draw_text, "", "(lv_obj)iii(lv_draw_label_dsc)si" },
  { "fill_bg", (void*) &lv_canvas_fill_bg, "", "(lv_obj)(lv_color)i" },
  { "get_px", (void*) &lv_canvas_get_px, "lv_color", "(lv_obj)ii" },
  { "set_buffer", (void*) &lv_canvas_set_buffer, "", "(lv_obj).ii(lv_img_cf)" },
  { "set_palette", (void*) &lv_canvas_set_palette, "", "(lv_obj)i(lv_color)" },
  { "set_px", (void*) &lv_canvas_set_px, "", "(lv_obj)ii(lv_color)" },
  { "transform", (void*) &lv_canvas_transform, "", "(lv_obj)(lv_img_dsc)iiiiiib" },
};
#endif // BE_LV_WIDGET_CANVAS

/* `lv_chart` methods */
#if BE_LV_WIDGET_CHART
const lvbe_call_c_t lv_chart_func[] = {
  { "clear_series", (void*) &lv_chart_clear_series, "", "(lv_obj)(lv_chart_series)" },
  { "create", (void*) &lv_chart_create, "+lv_chart", "(lv_obj)(lv_obj)" },
  { "get_cursor_point", (void*) &lv_chart_get_cursor_point, "i", "(lv_obj)(lv_chart_cursor)" },
  { "get_nearest_index_from_coord", (void*) &lv_chart_get_nearest_index_from_coord, "i", "(lv_obj)i" },
  { "get_point_count", (void*) &lv_chart_get_point_count, "i", "(lv_obj)" },
  { "get_point_id", (void*) &lv_chart_get_point_id, "i", "(lv_obj)(lv_chart_series)i" },
  { "get_series_area", (void*) &lv_chart_get_series_area, "", "(lv_obj)(lv_area)" },
  { "get_series_axis", (void*) &lv_chart_get_series_axis, "i", "(lv_obj)(lv_chart_series)" },
  { "get_type", (void*) &lv_chart_get_type, "i", "(lv_obj)" },
  { "get_x_from_index", (void*) &lv_chart_get_x_from_index, "i", "(lv_obj)(lv_chart_series)i" },
  { "get_x_start_point", (void*) &lv_chart_get_x_start_point, "i", "(lv_chart_series)" },
  { "get_y_from_index", (void*) &lv_chart_get_y_from_index, "i", "(lv_obj)(lv_chart_series)i" },
  { "hide_series", (void*) &lv_chart_hide_series, "", "(lv_obj)(lv_chart_series)b" },
  { "init_points", (void*) &lv_chart_init_points, "", "(lv_obj)(lv_chart_series)i" },
  { "refresh", (void*) &lv_chart_refresh, "", "(lv_obj)" },
  { "remove_series", (void*) &lv_chart_remove_series, "", "(lv_obj)(lv_chart_series)" },
  { "set_cursor_point", (void*) &lv_chart_set_cursor_point, "", "(lv_obj)(lv_chart_cursor)(lv_point)" },
  { "set_div_line_count", (void*) &lv_chart_set_div_line_count, "", "(lv_obj)ii" },
  { "set_ext_array", (void*) &lv_chart_set_ext_array, "", "(lv_obj)(lv_chart_series)ii" },
  { "set_next", (void*) &lv_chart_set_next, "", "(lv_obj)(lv_chart_series)i" },
  { "set_point_count", (void*) &lv_chart_set_point_count, "", "(lv_obj)i" },
  { "set_point_id", (void*) &lv_chart_set_point_id, "", "(lv_obj)(lv_chart_series)ii" },
  { "set_points", (void*) &lv_chart_set_points, "", "(lv_obj)(lv_chart_series)i" },
  { "set_secondary_y_tick_length", (void*) &lv_chart_set_secondary_y_tick_length, "", "(lv_obj)ii" },
  { "set_secondary_y_tick_texts", (void*) &lv_chart_set_secondary_y_tick_texts, "", "(lv_obj)si(lv_chart_axis_options)" },
  { "set_series_axis", (void*) &lv_chart_set_series_axis, "", "(lv_obj)(lv_chart_series)i" },
  { "set_type", (void*) &lv_chart_set_type, "", "(lv_obj)i" },
  { "set_update_mode", (void*) &lv_chart_set_update_mode, "", "(lv_obj)(lv_chart_update_mode)" },
  { "set_x_start_point", (void*) &lv_chart_set_x_start_point, "", "(lv_obj)(lv_chart_series)i" },
  { "set_x_tick_length", (void*) &lv_chart_set_x_tick_length, "", "(lv_obj)ii" },
  { "set_x_tick_texts", (void*) &lv_chart_set_x_tick_texts, "", "(lv_obj)si(lv_chart_axis_options)" },
  { "set_y_range", (void*) &lv_chart_set_y_range, "", "(lv_obj)iii" },
  { "set_y_tick_length", (void*) &lv_chart_set_y_tick_length, "", "(lv_obj)ii" },
  { "set_y_tick_texts", (void*) &lv_chart_set_y_tick_texts, "", "(lv_obj)si(lv_chart_axis_options)" },
};
#endif // BE_LV_WIDGET_CHART

/* `lv_checkbox` methods */
#if BE_LV_WIDGET_CHECKBOX
const lvbe_call_c_t lv_checkbox_func[] = {
  { "create", (void*) &lv_checkbox_create, "+lv_checkbox", "(lv_obj)(lv_obj)" },
  { "get_state", (void*) &lv_checkbox_get_state, "i", "(lv_obj)" },
  { "get_text", (void*) &lv_checkbox_get_text, "s", "(lv_obj)" },
  { "is_checked", (void*) &lv_checkbox_is_checked, "b", "(lv_obj)" },
  { "is_inactive", (void*) &lv_checkbox_is_inactive, "b", "(lv_obj)" },
  { "set_checked", (void*) &lv_checkbox_set_checked, "", "(lv_obj)b" },
  { "set_disabled", (void*) &lv_checkbox_set_disabled, "", "(lv_obj)" },
  { "set_state", (void*) &lv_checkbox_set_state, "", "(lv_obj)i" },
  { "set_text", (void*) &lv_checkbox_set_text, "", "(lv_obj)s" },
  { "set_text_static", (void*) &lv_checkbox_set_text_static, "", "(lv_obj)s" },
};
#endif // BE_LV_WIDGET_CHECKBOX

/* `lv_cont` methods */
#if BE_LV_WIDGET_CONT
const lvbe_call_c_t lv_cont_func[] = {
  { "create", (void*) &lv_cont_create, "+lv_cont", "(lv_obj)(lv_obj)" },
  { "get_fit_bottom", (void*) &lv_cont_get_fit_bottom, "i", "(lv_obj)" },
  { "get_fit_left", (void*) &lv_cont_get_fit_left, "i", "(lv_obj)" },
  { "get_fit_right", (void*) &lv_cont_get_fit_right, "i", "(lv_obj)" },
  { "get_fit_top", (void*) &lv_cont_get_fit_top, "i", "(lv_obj)" },
  { "get_layout", (void*) &lv_cont_get_layout, "i", "(lv_obj)" },
  { "set_fit", (void*) &lv_cont_set_fit, "", "(lv_obj)i" },
  { "set_fit2", (void*) &lv_cont_set_fit2, "", "(lv_obj)ii" },
  { "set_fit4", (void*) &lv_cont_set_fit4, "", "(lv_obj)iiii" },
  { "set_layout", (void*) &lv_cont_set_layout, "", "(lv_obj)i" },
};
#endif // BE_LV_WIDGET_CONT

/* `lv_cpicker` methods */
#if BE_LV_WIDGET_CPICKER
const lvbe_call_c_t lv_cpicker_func[] = {
  { "create", (void*) &lv_cpicker_create, "+lv_cpicker", "(lv_obj)(lv_obj)" },
  { "get_color", (void*) &lv_cpicker_get_color, "lv_color", "(lv_obj)" },
  { "get_color_mode", (void*) &lv_cpicker_get_color_mode, "i", "(lv_obj)" },
  { "get_color_mode_fixed", (void*) &lv_cpicker_get_color_mode_fixed, "b", "(lv_obj)" },
  { "get_hsv", (void*) &lv_cpicker_get_hsv, "i", "(lv_obj)" },
  { "get_hue", (void*) &lv_cpicker_get_hue, "i", "(lv_obj)" },
  { "get_knob_colored", (void*) &lv_cpicker_get_knob_colored, "b", "(lv_obj)" },
  { "get_saturation", (void*) &lv_cpicker_get_saturation, "i", "(lv_obj)" },
  { "get_value", (void*) &lv_cpicker_get_value, "i", "(lv_obj)" },
  { "set_color", (void*) &lv_cpicker_set_color, "b", "(lv_obj)(lv_color)" },
  { "set_color_mode", (void*) &lv_cpicker_set_color_mode, "", "(lv_obj)i" },
  { "set_color_mode_fixed", (void*) &lv_cpicker_set_color_mode_fixed, "", "(lv_obj)b" },
  { "set_hsv", (void*) &lv_cpicker_set_hsv, "b", "(lv_obj)i" },
  { "set_hue", (void*) &lv_cpicker_set_hue, "b", "(lv_obj)i" },
  { "set_knob_colored", (void*) &lv_cpicker_set_knob_colored, "", "(lv_obj)b" },
  { "set_saturation", (void*) &lv_cpicker_set_saturation, "b", "(lv_obj)i" },
  { "set_type", (void*) &lv_cpicker_set_type, "", "(lv_obj)(lv_cpicker_type)" },
  { "set_value", (void*) &lv_cpicker_set_value, "b", "(lv_obj)i" },
};
#endif // BE_LV_WIDGET_CPICKER

/* `lv_dropdown` methods */
#if BE_LV_WIDGET_DROPDOWN
const lvbe_call_c_t lv_dropdown_func[] = {
  { "add_option", (void*) &lv_dropdown_add_option, "", "(lv_obj)si" },
  { "clear_options", (void*) &lv_dropdown_clear_options, "", "(lv_obj)" },
  { "close", (void*) &lv_dropdown_close, "", "(lv_obj)" },
  { "create", (void*) &lv_dropdown_create, "+lv_dropdown", "(lv_obj)(lv_obj)" },
  { "get_dir", (void*) &lv_dropdown_get_dir, "i", "(lv_obj)" },
  { "get_max_height", (void*) &lv_dropdown_get_max_height, "i", "(lv_obj)" },
  { "get_option_cnt", (void*) &lv_dropdown_get_option_cnt, "i", "(lv_obj)" },
  { "get_options", (void*) &lv_dropdown_get_options, "s", "(lv_obj)" },
  { "get_selected", (void*) &lv_dropdown_get_selected, "i", "(lv_obj)" },
  { "get_selected_str", (void*) &lv_dropdown_get_selected_str, "", "(lv_obj)si" },
  { "get_show_selected", (void*) &lv_dropdown_get_show_selected, "b", "(lv_obj)" },
  { "get_symbol", (void*) &lv_dropdown_get_symbol, "s", "(lv_obj)" },
  { "get_text", (void*) &lv_dropdown_get_text, "s", "(lv_obj)" },
  { "open", (void*) &lv_dropdown_open, "", "(lv_obj)" },
  { "set_dir", (void*) &lv_dropdown_set_dir, "", "(lv_obj)i" },
  { "set_max_height", (void*) &lv_dropdown_set_max_height, "", "(lv_obj)i" },
  { "set_options", (void*) &lv_dropdown_set_options, "", "(lv_obj)s" },
  { "set_options_static", (void*) &lv_dropdown_set_options_static, "", "(lv_obj)s" },
  { "set_selected", (void*) &lv_dropdown_set_selected, "", "(lv_obj)i" },
  { "set_show_selected", (void*) &lv_dropdown_set_show_selected, "", "(lv_obj)b" },
  { "set_symbol", (void*) &lv_dropdown_set_symbol, "", "(lv_obj)s" },
  { "set_text", (void*) &lv_dropdown_set_text, "", "(lv_obj)s" },
};
#endif // BE_LV_WIDGET_DROPDOWN

/* `lv_gauge` methods */
#if BE_LV_WIDGET_GAUGE
const lvbe_call_c_t lv_gauge_func[] = {
  { "create", (void*) &lv_gauge_create, "+lv_gauge", "(lv_obj)(lv_obj)" },
  { "get_angle_offset", (void*) &lv_gauge_get_angle_offset, "i", "(lv_obj)" },
  { "get_critical_value", (void*) &lv_gauge_get_critical_value, "i", "(lv_obj)" },
  { "get_label_count", (void*) &lv_gauge_get_label_count, "i", "(lv_obj)" },
  { "get_line_count", (void*) &lv_gauge_get_line_count, "i", "(lv_obj)" },
  { "get_max_value", (void*) &lv_gauge_get_max_value, "i", "(lv_obj)" },
  { "get_min_value", (void*) &lv_gauge_get_min_value, "i", "(lv_obj)" },
  { "get_needle_count", (void*) &lv_gauge_get_needle_count, "i", "(lv_obj)" },
  { "get_needle_img", (void*) &lv_gauge_get_needle_img, ".", "(lv_obj)" },
  { "get_needle_img_pivot_x", (void*) &lv_gauge_get_needle_img_pivot_x, "i", "(lv_obj)" },
  { "get_needle_img_pivot_y", (void*) &lv_gauge_get_needle_img_pivot_y, "i", "(lv_obj)" },
  { "get_scale_angle", (void*) &lv_gauge_get_scale_angle, "i", "(lv_obj)" },
  { "get_value", (void*) &lv_gauge_get_value, "i", "(lv_obj)i" },
  { "set_angle_offset", (void*) &lv_gauge_set_angle_offset, "", "(lv_obj)i" },
  { "set_critical_value", (void*) &lv_gauge_set_critical_value, "", "(lv_obj)i" },
  { "set_formatter_cb", (void*) &lv_gauge_set_formatter_cb, "", "(lv_obj)&4" },
  { "set_needle_count", (void*) &lv_gauge_set_needle_count, "", "(lv_obj)i(lv_color)" },
  { "set_needle_img", (void*) &lv_gauge_set_needle_img, "", "(lv_obj).ii" },
  { "set_range", (void*) &lv_gauge_set_range, "", "(lv_obj)ii" },
  { "set_scale", (void*) &lv_gauge_set_scale, "", "(lv_obj)iii" },
  { "set_value", (void*) &lv_gauge_set_value, "", "(lv_obj)ii" },
};
#endif // BE_LV_WIDGET_GAUGE

/* `lv_imgbtn` methods */
#if BE_LV_WIDGET_IMGBTN
const lvbe_call_c_t lv_imgbtn_func[] = {
  { "create", (void*) &lv_imgbtn_create, "+lv_imgbtn", "(lv_obj)(lv_obj)" },
  { "get_src", (void*) &lv_imgbtn_get_src, ".", "(lv_obj)i" },
  { "get_state", (void*) &lv_imgbtn_get_state, "i", "(lv_obj)" },
  { "set_checkable", (void*) &lv_imgbtn_set_checkable, "", "(lv_obj)b" },
  { "set_src", (void*) &lv_imgbtn_set_src, "", "(lv_obj)i." },
  { "set_state", (void*) &lv_imgbtn_set_state, "", "(lv_obj)i" },
  { "toggle", (void*) &lv_imgbtn_toggle, "", "(lv_obj)" },
};
#endif // BE_LV_WIDGET_IMGBTN

/* `lv_keyboard` methods */
#if BE_LV_WIDGET_KEYBOARD
const lvbe_call_c_t lv_keyboard_func[] = {
  { "create", (void*) &lv_keyboard_create, "+lv_keyboard", "(lv_obj)(lv_obj)" },
  { "def_event_cb", (void*) &lv_keyboard_def_event_cb, "", "(lv_obj)(lv_event)" },
  { "get_cursor_manage", (void*) &lv_keyboard_get_cursor_manage, "b", "(lv_obj)" },
  { "get_mode", (void*) &lv_keyboard_get_mode, "i", "(lv_obj)" },
  { "get_textarea", (void*) &lv_keyboard_get_textarea, "lv_obj", "(lv_obj)" },
  { "set_ctrl_map", (void*) &lv_keyboard_set_ctrl_map, "", "(lv_obj)i(lv_btnmatrix_ctrl)" },
  { "set_cursor_manage", (void*) &lv_keyboard_set_cursor_manage, "", "(lv_obj)b" },
  { "set_map", (void*) &lv_keyboard_set_map, "", "(lv_obj)is" },
  { "set_mode", (void*) &lv_keyboard_set_mode, "", "(lv_obj)i" },
  { "set_textarea", (void*) &lv_keyboard_set_textarea, "", "(lv_obj)(lv_obj)" },
};
#endif // BE_LV_WIDGET_KEYBOARD

/* `lv_label` methods */
#if BE_LV_WIDGET_LABEL
const lvbe_call_c_t lv_label_func[] = {
  { "create", (void*) &lv_label_create, "+lv_label", "(lv_obj)(lv_obj)" },
  { "cut_text", (void*) &lv_label_cut_text, "", "(lv_obj)ii" },
  { "get_align", (void*) &lv_label_get_align, "i", "(lv_obj)" },
  { "get_anim_speed", (void*) &lv_label_get_anim_speed, "i", "(lv_obj)" },
  { "get_letter_on", (void*) &lv_label_get_letter_on, "i", "(lv_obj)(lv_point)" },
  { "get_letter_pos", (void*) &lv_label_get_letter_pos, "", "(lv_obj)i(lv_point)" },
  { "get_long_mode", (void*) &lv_label_get_long_mode, "i", "(lv_obj)" },
  { "get_recolor", (void*) &lv_label_get_recolor, "b", "(lv_obj)" },
  { "get_text", (void*) &lv_label_get_text, "s", "(lv_obj)" },
  { "get_text_sel_end", (void*) &lv_label_get_text_sel_end, "i", "(lv_obj)" },
  { "get_text_sel_start", (void*) &lv_label_get_text_sel_start, "i", "(lv_obj)" },
  { "ins_text", (void*) &lv_label_ins_text, "", "(lv_obj)is" },
  { "is_char_under_pos", (void*) &lv_label_is_char_under_pos, "b", "(lv_obj)(lv_point)" },
  { "refr_text", (void*) &lv_label_refr_text, "", "(lv_obj)" },
  { "set_align", (void*) &lv_label_set_align, "", "(lv_obj)i" },
  { "set_anim_speed", (void*) &lv_label_set_anim_speed, "", "(lv_obj)i" },
  { "set_long_mode", (void*) &lv_label_set_long_mode, "", "(lv_obj)i" },
  { "set_recolor", (void*) &lv_label_set_recolor, "", "(lv_obj)b" },
  { "set_text", (void*) &lv_label_set_text, "", "(lv_obj)s" },
  { "set_text_fmt", (void*) &lv_label_set_text_fmt, "", "(lv_obj)s" },
  { "set_text_sel_end", (void*) &lv_label_set_text_sel_end, "", "(lv_obj)i" },
  { "set_text_sel_start", (void*) &lv_label_set_text_sel_start, "", "(lv_obj)i" },
  { "set_text_static", (void*) &lv_label_set_text_static, "", "(lv_obj)s" },
};
#endif // BE_LV_WIDGET_LABEL

/* `lv_led` methods */
#if BE_LV_WIDGET_LED
const lvbe_call_c_t lv_led_func[] = {
  { "create", (void*) &lv_led_create, "+lv_led", "(lv_obj)(lv_obj)" },
  { "get_bright", (void*) &lv_led_get_bright, "i", "(lv_obj)" },
  { "off", (void*) &lv_led_off, "", "(lv_obj)" },
  { "on", (void*) &lv_led_on, "", "(lv_obj)" },
  { "set_bright", (void*) &lv_led_set_bright, "", "(lv_obj)i" },
  { "toggle", (void*) &lv_led_toggle, "", "(lv_obj)" },
};
#endif // BE_LV_WIDGET_LED

/* `lv_line` methods */
#if BE_LV_WIDGET_LINE
const lvbe_call_c_t lv_line_func[] = {
  { "create", (void*) &lv_line_create, "+lv_line", "(lv_obj)(lv_obj)" },
  { "get_auto_size", (void*) &lv_line_get_auto_size, "b", "(lv_obj)" },
  { "get_y_invert", (void*) &lv_line_get_y_invert, "b", "(lv_obj)" },
  { "set_auto_size", (void*) &lv_line_set_auto_size, "", "(lv_obj)b" },
  { "set_points", (void*) &lv_line_set_points, "", "(lv_obj)ii" },
  { "set_y_invert", (void*) &lv_line_set_y_invert, "", "(lv_obj)b" },
};
#endif // BE_LV_WIDGET_LINE

/* `lv_linemeter` methods */
#if BE_LV_WIDGET_LINEMETER
const lvbe_call_c_t lv_linemeter_func[] = {
  { "create", (void*) &lv_linemeter_create, "+lv_linemeter", "(lv_obj)(lv_obj)" },
  { "draw_scale", (void*) &lv_linemeter_draw_scale, "", "(lv_obj)(lv_area)i" },
  { "get_angle_offset", (void*) &lv_linemeter_get_angle_offset, "i", "(lv_obj)" },
  { "get_line_count", (void*) &lv_linemeter_get_line_count, "i", "(lv_obj)" },
  { "get_max_value", (void*) &lv_linemeter_get_max_value, "i", "(lv_obj)" },
  { "get_min_value", (void*) &lv_linemeter_get_min_value, "i", "(lv_obj)" },
  { "get_mirror", (void*) &lv_linemeter_get_mirror, "b", "(lv_obj)" },
  { "get_scale_angle", (void*) &lv_linemeter_get_scale_angle, "i", "(lv_obj)" },
  { "get_value", (void*) &lv_linemeter_get_value, "i", "(lv_obj)" },
  { "set_angle_offset", (void*) &lv_linemeter_set_angle_offset, "", "(lv_obj)i" },
  { "set_mirror", (void*) &lv_linemeter_set_mirror, "", "(lv_obj)b" },
  { "set_range", (void*) &lv_linemeter_set_range, "", "(lv_obj)ii" },
  { "set_scale", (void*) &lv_linemeter_set_scale, "", "(lv_obj)ii" },
  { "set_value", (void*) &lv_linemeter_set_value, "", "(lv_obj)i" },
};
#endif // BE_LV_WIDGET_LINEMETER

/* `lv_list` methods */
#if BE_LV_WIDGET_LIST
const lvbe_call_c_t lv_list_func[] = {
  { "add_btn", (void*) &lv_list_add_btn, "lv_obj", "(lv_obj).s" },
  { "clean", (void*) &lv_list_clean, "", "(lv_obj)" },
  { "create", (void*) &lv_list_create, "+lv_list", "(lv_obj)(lv_obj)" },
  { "down", (void*) &lv_list_down, "", "(lv_obj)" },
  { "focus", (void*) &lv_list_focus, "", "(lv_obj)(lv_anim_enable)" },
  { "focus_btn", (void*) &lv_list_focus_btn, "", "(lv_obj)(lv_obj)" },
  { "get_anim_time", (void*) &lv_list_get_anim_time, "i", "(lv_obj)" },
  { "get_btn_img", (void*) &lv_list_get_btn_img, "lv_obj", "(lv_obj)" },
  { "get_btn_index", (void*) &lv_list_get_btn_index, "i", "(lv_obj)(lv_obj)" },
  { "get_btn_label", (void*) &lv_list_get_btn_label, "lv_obj", "(lv_obj)" },
  { "get_btn_selected", (void*) &lv_list_get_btn_selected, "lv_obj", "(lv_obj)" },
  { "get_btn_text", (void*) &lv_list_get_btn_text, "s", "(lv_obj)" },
  { "get_edge_flash", (void*) &lv_list_get_edge_flash, "b", "(lv_obj)" },
  { "get_layout", (void*) &lv_list_get_layout, "i", "(lv_obj)" },
  { "get_next_btn", (void*) &lv_list_get_next_btn, "lv_obj", "(lv_obj)(lv_obj)" },
  { "get_prev_btn", (void*) &lv_list_get_prev_btn, "lv_obj", "(lv_obj)(lv_obj)" },
  { "get_scroll_propagation", (void*) &lv_list_get_scroll_propagation, "b", "(lv_obj)" },
  { "get_scrollbar_mode", (void*) &lv_list_get_scrollbar_mode, "i", "(lv_obj)" },
  { "get_size", (void*) &lv_list_get_size, "i", "(lv_obj)" },
  { "remove", (void*) &lv_list_remove, "b", "(lv_obj)i" },
  { "set_anim_time", (void*) &lv_list_set_anim_time, "", "(lv_obj)i" },
  { "set_edge_flash", (void*) &lv_list_set_edge_flash, "", "(lv_obj)b" },
  { "set_layout", (void*) &lv_list_set_layout, "", "(lv_obj)i" },
  { "set_scroll_propagation", (void*) &lv_list_set_scroll_propagation, "", "(lv_obj)b" },
  { "set_scrollbar_mode", (void*) &lv_list_set_scrollbar_mode, "", "(lv_obj)i" },
  { "up", (void*) &lv_list_up, "", "(lv_obj)" },
};
#endif // BE_LV_WIDGET_LIST

/* `lv_msgbox` methods */
#if BE_LV_WIDGET_MSGBOX
const lvbe_call_c_t lv_msgbox_func[] = {
  { "add_btns", (void*) &lv_msgbox_add_btns, "", "(lv_obj)s" },
  { "create", (void*) &lv_msgbox_create, "+lv_msgbox", "(lv_obj)(lv_obj)" },
  { "get_active_btn", (void*) &lv_msgbox_get_active_btn, "i", "(lv_obj)" },
  { "get_active_btn_text", (void*) &lv_msgbox_get_active_btn_text, "s", "(lv_obj)" },
  { "get_anim_time", (void*) &lv_msgbox_get_anim_time, "i", "(lv_obj)" },
  { "get_btnmatrix", (void*) &lv_msgbox_get_btnmatrix, "lv_obj", "(lv_obj)" },
  { "get_recolor", (void*) &lv_msgbox_get_recolor, "b", "(lv_obj)" },
  { "get_text", (void*) &lv_msgbox_get_text, "s", "(lv_obj)" },
  { "set_anim_time", (void*) &lv_msgbox_set_anim_time, "", "(lv_obj)i" },
  { "set_recolor", (void*) &lv_msgbox_set_recolor, "", "(lv_obj)b" },
  { "set_text", (void*) &lv_msgbox_set_text, "", "(lv_obj)s" },
  { "set_text_fmt", (void*) &lv_msgbox_set_text_fmt, "", "(lv_obj)s" },
  { "start_auto_close", (void*) &lv_msgbox_start_auto_close, "", "(lv_obj)i" },
  { "stop_auto_close", (void*) &lv_msgbox_stop_auto_close, "", "(lv_obj)" },
};
#endif // BE_LV_WIDGET_MSGBOX

/* `lv_objmask` methods */
#if BE_LV_WIDGET_OBJMASK
const lvbe_call_c_t lv_objmask_func[] = {
  { "create", (void*) &lv_objmask_create, "+lv_objmask", "(lv_obj)(lv_obj)" },
  { "remove_mask", (void*) &lv_objmask_remove_mask, "", "(lv_obj)(lv_objmask_mask)" },
  { "update_mask", (void*) &lv_objmask_update_mask, "", "(lv_obj)(lv_objmask_mask)." },
};
#endif // BE_LV_WIDGET_OBJMASK

/* `lv_page` methods */
#if BE_LV_WIDGET_PAGE
const lvbe_call_c_t lv_page_func[] = {
  { "clean", (void*) &lv_page_clean, "", "(lv_obj)" },
  { "create", (void*) &lv_page_create, "+lv_page", "(lv_obj)(lv_obj)" },
  { "focus", (void*) &lv_page_focus, "", "(lv_obj)(lv_obj)(lv_anim_enable)" },
  { "get_anim_time", (void*) &lv_page_get_anim_time, "i", "(lv_obj)" },
  { "get_edge_flash", (void*) &lv_page_get_edge_flash, "b", "(lv_obj)" },
  { "get_height_fit", (void*) &lv_page_get_height_fit, "i", "(lv_obj)" },
  { "get_height_grid", (void*) &lv_page_get_height_grid, "i", "(lv_obj)ii" },
  { "get_scrl_fit_bottom", (void*) &lv_page_get_scrl_fit_bottom, "i", "(lv_obj)" },
  { "get_scrl_fit_left", (void*) &lv_page_get_scrl_fit_left, "i", "(lv_obj)" },
  { "get_scrl_fit_right", (void*) &lv_page_get_scrl_fit_right, "i", "(lv_obj)" },
  { "get_scrl_fit_top", (void*) &lv_page_get_scrl_fit_top, "i", "(lv_obj)" },
  { "get_scrl_height", (void*) &lv_page_get_scrl_height, "i", "(lv_obj)" },
  { "get_scrl_layout", (void*) &lv_page_get_scrl_layout, "i", "(lv_obj)" },
  { "get_scrl_width", (void*) &lv_page_get_scrl_width, "i", "(lv_obj)" },
  { "get_scroll_propagation", (void*) &lv_page_get_scroll_propagation, "b", "(lv_obj)" },
  { "get_scrollable", (void*) &lv_page_get_scrollable, "lv_obj", "(lv_obj)" },
  { "get_scrollbar_mode", (void*) &lv_page_get_scrollbar_mode, "i", "(lv_obj)" },
  { "get_width_fit", (void*) &lv_page_get_width_fit, "i", "(lv_obj)" },
  { "get_width_grid", (void*) &lv_page_get_width_grid, "i", "(lv_obj)ii" },
  { "glue_obj", (void*) &lv_page_glue_obj, "", "(lv_obj)b" },
  { "on_edge", (void*) &lv_page_on_edge, "b", "(lv_obj)(lv_page_edge)" },
  { "scroll_hor", (void*) &lv_page_scroll_hor, "", "(lv_obj)i" },
  { "scroll_ver", (void*) &lv_page_scroll_ver, "", "(lv_obj)i" },
  { "set_anim_time", (void*) &lv_page_set_anim_time, "", "(lv_obj)i" },
  { "set_edge_flash", (void*) &lv_page_set_edge_flash, "", "(lv_obj)b" },
  { "set_scrl_height", (void*) &lv_page_set_scrl_height, "", "(lv_obj)i" },
  { "set_scrl_layout", (void*) &lv_page_set_scrl_layout, "", "(lv_obj)i" },
  { "set_scrl_width", (void*) &lv_page_set_scrl_width, "", "(lv_obj)i" },
  { "set_scroll_propagation", (void*) &lv_page_set_scroll_propagation, "", "(lv_obj)b" },
  { "set_scrollable_fit", (void*) &lv_page_set_scrollable_fit, "", "(lv_obj)i" },
  { "set_scrollable_fit2", (void*) &lv_page_set_scrollable_fit2, "", "(lv_obj)ii" },
  { "set_scrollbar_mode", (void*) &lv_page_set_scrollbar_mode, "", "(lv_obj)i" },
  { "start_edge_flash", (void*) &lv_page_start_edge_flash, "", "(lv_obj)(lv_page_edge)" },
};
#endif // BE_LV_WIDGET_PAGE

/* `lv_roller` methods */
#if BE_LV_WIDGET_ROLLER
const lvbe_call_c_t lv_roller_func[] = {
  { "create", (void*) &lv_roller_create, "+lv_roller", "(lv_obj)(lv_obj)" },
  { "get_align", (void*) &lv_roller_get_align, "i", "(lv_obj)" },
  { "get_anim_time", (void*) &lv_roller_get_anim_time, "i", "(lv_obj)" },
  { "get_auto_fit", (void*) &lv_roller_get_auto_fit, "b", "(lv_obj)" },
  { "get_option_cnt", (void*) &lv_roller_get_option_cnt, "i", "(lv_obj)" },
  { "get_options", (void*) &lv_roller_get_options, "s", "(lv_obj)" },
  { "get_selected", (void*) &lv_roller_get_selected, "i", "(lv_obj)" },
  { "get_selected_str", (void*) &lv_roller_get_selected_str, "", "(lv_obj)si" },
  { "set_align", (void*) &lv_roller_set_align, "", "(lv_obj)i" },
  { "set_anim_time", (void*) &lv_roller_set_anim_time, "", "(lv_obj)i" },
  { "set_auto_fit", (void*) &lv_roller_set_auto_fit, "", "(lv_obj)b" },
  { "set_options", (void*) &lv_roller_set_options, "", "(lv_obj)s(lv_roller_mode)" },
  { "set_selected", (void*) &lv_roller_set_selected, "", "(lv_obj)i(lv_anim_enable)" },
  { "set_visible_row_count", (void*) &lv_roller_set_visible_row_count, "", "(lv_obj)i" },
};
#endif // BE_LV_WIDGET_ROLLER

/* `lv_slider` methods */
#if BE_LV_WIDGET_SLIDER
const lvbe_call_c_t lv_slider_func[] = {
  { "create", (void*) &lv_slider_create, "+lv_slider", "(lv_obj)(lv_obj)" },
  { "get_anim_time", (void*) &lv_slider_get_anim_time, "i", "(lv_obj)" },
  { "get_left_value", (void*) &lv_slider_get_left_value, "i", "(lv_obj)" },
  { "get_max_value", (void*) &lv_slider_get_max_value, "i", "(lv_obj)" },
  { "get_min_value", (void*) &lv_slider_get_min_value, "i", "(lv_obj)" },
  { "get_type", (void*) &lv_slider_get_type, "i", "(lv_obj)" },
  { "get_value", (void*) &lv_slider_get_value, "i", "(lv_obj)" },
  { "is_dragged", (void*) &lv_slider_is_dragged, "b", "(lv_obj)" },
  { "set_anim_time", (void*) &lv_slider_set_anim_time, "", "(lv_obj)i" },
  { "set_left_value", (void*) &lv_slider_set_left_value, "", "(lv_obj)i(lv_anim_enable)" },
  { "set_range", (void*) &lv_slider_set_range, "", "(lv_obj)ii" },
  { "set_type", (void*) &lv_slider_set_type, "", "(lv_obj)i" },
  { "set_value", (void*) &lv_slider_set_value, "", "(lv_obj)i(lv_anim_enable)" },
};
#endif // BE_LV_WIDGET_SLIDER

/* `lv_spinbox` methods */
#if BE_LV_WIDGET_SPINBOX
const lvbe_call_c_t lv_spinbox_func[] = {
  { "create", (void*) &lv_spinbox_create, "+lv_spinbox", "(lv_obj)(lv_obj)" },
  { "decrement", (void*) &lv_spinbox_decrement, "", "(lv_obj)" },
  { "get_rollover", (void*) &lv_spinbox_get_rollover, "b", "(lv_obj)" },
  { "get_step", (void*) &lv_spinbox_get_step, "i", "(lv_obj)" },
  { "get_value", (void*) &lv_spinbox_get_value, "i", "(lv_obj)" },
  { "increment", (void*) &lv_spinbox_increment, "", "(lv_obj)" },
  { "set_digit_format", (void*) &lv_spinbox_set_digit_format, "", "(lv_obj)ii" },
  { "set_padding_left", (void*) &lv_spinbox_set_padding_left, "", "(lv_obj)i" },
  { "set_range", (void*) &lv_spinbox_set_range, "", "(lv_obj)ii" },
  { "set_rollover", (void*) &lv_spinbox_set_rollover, "", "(lv_obj)b" },
  { "set_step", (void*) &lv_spinbox_set_step, "", "(lv_obj)i" },
  { "set_value", (void*) &lv_spinbox_set_value, "", "(lv_obj)i" },
  { "step_next", (void*) &lv_spinbox_step_next, "", "(lv_obj)" },
  { "step_prev", (void*) &lv_spinbox_step_prev, "", "(lv_obj)" },
};
#endif // BE_LV_WIDGET_SPINBOX

/* `lv_spinner` methods */
#if BE_LV_WIDGET_SPINNER
const lvbe_call_c_t lv_spinner_func[] = {
  { "anim_cb", (void*) &lv_spinner_anim_cb, "", ".i" },
  { "create", (void*) &lv_spinner_create, "+lv_spinner", "(lv_obj)(lv_obj)" },
  { "get_arc_length", (void*) &lv_spinner_get_arc_length, "i", "(lv_obj)" },
  { "get_dir", (void*) &lv_spinner_get_dir, "i", "(lv_obj)" },
  { "get_spin_time", (void*) &lv_spinner_get_spin_time, "i", "(lv_obj)" },
  { "get_type", (void*) &lv_spinner_get_type, "i", "(lv_obj)" },
  { "set_arc_length", (void*) &lv_spinner_set_arc_length, "", "(lv_obj)i" },
  { "set_dir", (void*) &lv_spinner_set_dir, "", "(lv_obj)i" },
  { "set_spin_time", (void*) &lv_spinner_set_spin_time, "", "(lv_obj)i" },
  { "set_type", (void*) &lv_spinner_set_type, "", "(lv_obj)i" },
};
#endif // BE_LV_WIDGET_SPINNER

/* `lv_switch` methods */
#if BE_LV_WIDGET_SWITCH
const lvbe_call_c_t lv_switch_func[] = {
  { "create", (void*) &lv_switch_create, "+lv_switch", "(lv_obj)(lv_obj)" },
  { "get_anim_time", (void*) &lv_switch_get_anim_time, "i", "(lv_obj)" },
  { "get_state", (void*) &lv_switch_get_state, "b", "(lv_obj)" },
  { "off", (void*) &lv_switch_off, "", "(lv_obj)(lv_anim_enable)" },
  { "on", (void*) &lv_switch_on, "", "(lv_obj)(lv_anim_enable)" },
  { "set_anim_time", (void*) &lv_switch_set_anim_time, "", "(lv_obj)i" },
  { "toggle", (void*) &lv_switch_toggle, "b", "(lv_obj)(lv_anim_enable)" },
};
#endif // BE_LV_WIDGET_SWITCH

/* `lv_table` methods */
#if BE_LV_WIDGET_TABLE
const lvbe_call_c_t lv_table_func[] = {
  { "create", (void*) &lv_table_create, "+lv_table", "(lv_obj)(lv_obj)" },
  { "get_cell_align", (void*) &lv_table_get_cell_align, "i", "(lv_obj)ii" },
  { "get_cell_crop", (void*) &lv_table_get_cell_crop, "i", "(lv_obj)ii" },
  { "get_cell_merge_right", (void*) &lv_table_get_cell_merge_right, "b", "(lv_obj)ii" },
  { "get_cell_type", (void*) &lv_table_get_cell_type, "i", "(lv_obj)ii" },
  { "get_cell_value", (void*) &lv_table_get_cell_value, "s", "(lv_obj)ii" },
  { "get_col_cnt", (void*) &lv_table_get_col_cnt, "i", "(lv_obj)" },
  { "get_col_width", (void*) &lv_table_get_col_width, "i", "(lv_obj)i" },
  { "get_pressed_cell", (void*) &lv_table_get_pressed_cell, "i", "(lv_obj)(uint16)(uint16)" },
  { "get_row_cnt", (void*) &lv_table_get_row_cnt, "i", "(lv_obj)" },
  { "set_cell_align", (void*) &lv_table_set_cell_align, "", "(lv_obj)iii" },
  { "set_cell_crop", (void*) &lv_table_set_cell_crop, "", "(lv_obj)iib" },
  { "set_cell_merge_right", (void*) &lv_table_set_cell_merge_right, "", "(lv_obj)iib" },
  { "set_cell_type", (void*) &lv_table_set_cell_type, "", "(lv_obj)iii" },
  { "set_cell_value", (void*) &lv_table_set_cell_value, "", "(lv_obj)iis" },
  { "set_cell_value_fmt", (void*) &lv_table_set_cell_value_fmt, "", "(lv_obj)iis" },
  { "set_col_cnt", (void*) &lv_table_set_col_cnt, "", "(lv_obj)i" },
  { "set_col_width", (void*) &lv_table_set_col_width, "", "(lv_obj)ii" },
  { "set_row_cnt", (void*) &lv_table_set_row_cnt, "", "(lv_obj)i" },
};
#endif // BE_LV_WIDGET_TABLE

/* `lv_tabview` methods */
#if BE_LV_WIDGET_TABVIEW
const lvbe_call_c_t lv_tabview_func[] = {
  { "add_tab", (void*) &lv_tabview_add_tab, "lv_obj", "(lv_obj)s" },
  { "clean_tab", (void*) &lv_tabview_clean_tab, "", "(lv_obj)" },
  { "create", (void*) &lv_tabview_create, "+lv_tabview", "(lv_obj)(lv_obj)" },
  { "get_anim_time", (void*) &lv_tabview_get_anim_time, "i", "(lv_obj)" },
  { "get_btns_pos", (void*) &lv_tabview_get_btns_pos, "i", "(lv_obj)" },
  { "get_tab", (void*) &lv_tabview_get_tab, "lv_obj", "(lv_obj)i" },
  { "get_tab_act", (void*) &lv_tabview_get_tab_act, "i", "(lv_obj)" },
  { "get_tab_count", (void*) &lv_tabview_get_tab_count, "i", "(lv_obj)" },
  { "set_anim_time", (void*) &lv_tabview_set_anim_time, "", "(lv_obj)i" },
  { "set_btns_pos", (void*) &lv_tabview_set_btns_pos, "", "(lv_obj)i" },
  { "set_tab_act", (void*) &lv_tabview_set_tab_act, "", "(lv_obj)i(lv_anim_enable)" },
  { "set_tab_name", (void*) &lv_tabview_set_tab_name, "", "(lv_obj)is" },
};
#endif // BE_LV_WIDGET_TABVIEW

/* `lv_textarea` methods */
#if BE_LV_WIDGET_TEXTAREA
const lvbe_call_c_t lv_textarea_func[] = {
  { "add_char", (void*) &lv_textarea_add_char, "", "(lv_obj)i" },
  { "add_text", (void*) &lv_textarea_add_text, "", "(lv_obj)s" },
  { "clear_selection", (void*) &lv_textarea_clear_selection, "", "(lv_obj)" },
  { "create", (void*) &lv_textarea_create, "+lv_textarea", "(lv_obj)(lv_obj)" },
  { "cursor_down", (void*) &lv_textarea_cursor_down, "", "(lv_obj)" },
  { "cursor_left", (void*) &lv_textarea_cursor_left, "", "(lv_obj)" },
  { "cursor_right", (void*) &lv_textarea_cursor_right, "", "(lv_obj)" },
  { "cursor_up", (void*) &lv_textarea_cursor_up, "", "(lv_obj)" },
  { "del_char", (void*) &lv_textarea_del_char, "", "(lv_obj)" },
  { "del_char_forward", (void*) &lv_textarea_del_char_forward, "", "(lv_obj)" },
  { "get_accepted_chars", (void*) &lv_textarea_get_accepted_chars, "s", "(lv_obj)" },
  { "get_cursor_blink_time", (void*) &lv_textarea_get_cursor_blink_time, "i", "(lv_obj)" },
  { "get_cursor_click_pos", (void*) &lv_textarea_get_cursor_click_pos, "b", "(lv_obj)" },
  { "get_cursor_hidden", (void*) &lv_textarea_get_cursor_hidden, "b", "(lv_obj)" },
  { "get_cursor_pos", (void*) &lv_textarea_get_cursor_pos, "i", "(lv_obj)" },
  { "get_edge_flash", (void*) &lv_textarea_get_edge_flash, "b", "(lv_obj)" },
  { "get_label", (void*) &lv_textarea_get_label, "lv_obj", "(lv_obj)" },
  { "get_max_length", (void*) &lv_textarea_get_max_length, "i", "(lv_obj)" },
  { "get_one_line", (void*) &lv_textarea_get_one_line, "b", "(lv_obj)" },
  { "get_placeholder_text", (void*) &lv_textarea_get_placeholder_text, "s", "(lv_obj)" },
  { "get_pwd_mode", (void*) &lv_textarea_get_pwd_mode, "b", "(lv_obj)" },
  { "get_pwd_show_time", (void*) &lv_textarea_get_pwd_show_time, "i", "(lv_obj)" },
  { "get_scroll_propagation", (void*) &lv_textarea_get_scroll_propagation, "b", "(lv_obj)" },
  { "get_scrollbar_mode", (void*) &lv_textarea_get_scrollbar_mode, "i", "(lv_obj)" },
  { "get_text", (void*) &lv_textarea_get_text, "s", "(lv_obj)" },
  { "get_text_sel_en", (void*) &lv_textarea_get_text_sel_en, "b", "(lv_obj)" },
  { "set_accepted_chars", (void*) &lv_textarea_set_accepted_chars, "", "(lv_obj)s" },
  { "set_cursor_blink_time", (void*) &lv_textarea_set_cursor_blink_time, "", "(lv_obj)i" },
  { "set_cursor_click_pos", (void*) &lv_textarea_set_cursor_click_pos, "", "(lv_obj)b" },
  { "set_cursor_hidden", (void*) &lv_textarea_set_cursor_hidden, "", "(lv_obj)b" },
  { "set_cursor_pos", (void*) &lv_textarea_set_cursor_pos, "", "(lv_obj)i" },
  { "set_edge_flash", (void*) &lv_textarea_set_edge_flash, "", "(lv_obj)b" },
  { "set_insert_replace", (void*) &lv_textarea_set_insert_replace, "", "(lv_obj)s" },
  { "set_max_length", (void*) &lv_textarea_set_max_length, "", "(lv_obj)i" },
  { "set_one_line", (void*) &lv_textarea_set_one_line, "", "(lv_obj)b" },
  { "set_placeholder_text", (void*) &lv_textarea_set_placeholder_text, "", "(lv_obj)s" },
  { "set_pwd_mode", (void*) &lv_textarea_set_pwd_mode, "", "(lv_obj)b" },
  { "set_pwd_show_time", (void*) &lv_textarea_set_pwd_show_time, "", "(lv_obj)i" },
  { "set_scroll_propagation", (void*) &lv_textarea_set_scroll_propagation, "", "(lv_obj)b" },
  { "set_scrollbar_mode", (void*) &lv_textarea_set_scrollbar_mode, "", "(lv_obj)i" },
  { "set_text", (void*) &lv_textarea_set_text, "", "(lv_obj)s" },
  { "set_text_align", (void*) &lv_textarea_set_text_align, "", "(lv_obj)i" },
  { "set_text_sel", (void*) &lv_textarea_set_text_sel, "", "(lv_obj)b" },
  { "text_is_selected", (void*) &lv_textarea_text_is_selected, "b", "(lv_obj)" },
};
#endif // BE_LV_WIDGET_TEXTAREA

/* `lv_tileview` methods */
#if BE_LV_WIDGET_TILEVIEW
const lvbe_call_c_t lv_tileview_func[] = {
  { "add_element", (void*) &lv_tileview_add_element, "", "(lv_obj)(lv_obj)" },
  { "create", (void*) &lv_tileview_create, "+lv_tileview", "(lv_obj)(lv_obj)" },
  { "get_anim_time", (void*) &lv_tileview_get_anim_time, "i", "(lv_obj)" },
  { "get_edge_flash", (void*) &lv_tileview_get_edge_flash, "b", "(lv_obj)" },
  { "get_tile_act", (void*) &lv_tileview_get_tile_act, "", "(lv_obj)(lv_coord)(lv_coord)" },
  { "set_anim_time", (void*) &lv_tileview_set_anim_time, "", "(lv_obj)i" },
  { "set_edge_flash", (void*) &lv_tileview_set_edge_flash, "", "(lv_obj)b" },
  { "set_tile_act", (void*) &lv_tileview_set_tile_act, "", "(lv_obj)ii(lv_anim_enable)" },
  { "set_valid_positions", (void*) &lv_tileview_set_valid_positions, "", "(lv_obj)ii" },
};
#endif // BE_LV_WIDGET_TILEVIEW

/* `lv_win` methods */
#if BE_LV_WIDGET_WIN
const lvbe_call_c_t lv_win_func[] = {
  { "add_btn_left", (void*) &lv_win_add_btn_left, "lv_obj", "(lv_obj)." },
  { "add_btn_right", (void*) &lv_win_add_btn_right, "lv_obj", "(lv_obj)." },
  { "clean", (void*) &lv_win_clean, "", "(lv_obj)" },
  { "close_event_cb", (void*) &lv_win_close_event_cb, "", "(lv_obj)(lv_event)" },
  { "create", (void*) &lv_win_create, "+lv_win", "(lv_obj)(lv_obj)" },
  { "focus", (void*) &lv_win_focus, "", "(lv_obj)(lv_obj)(lv_anim_enable)" },
  { "get_anim_time", (void*) &lv_win_get_anim_time, "i", "(lv_obj)" },
  { "get_btn_width", (void*) &lv_win_get_btn_width, "i", "(lv_obj)" },
  { "get_content", (void*) &lv_win_get_content, "lv_obj", "(lv_obj)" },
  { "get_drag", (void*) &lv_win_get_drag, "b", "(lv_obj)" },
  { "get_from_btn", (void*) &lv_win_get_from_btn, "lv_obj", "(lv_obj)" },
  { "get_header_height", (void*) &lv_win_get_header_height, "i", "(lv_obj)" },
  { "get_layout", (void*) &lv_win_get_layout, "i", "(lv_obj)" },
  { "get_sb_mode", (void*) &lv_win_get_sb_mode, "i", "(lv_obj)" },
  { "get_title", (void*) &lv_win_get_title, "s", "(lv_obj)" },
  { "get_width", (void*) &lv_win_get_width, "i", "(lv_obj)" },
  { "scroll_hor", (void*) &lv_win_scroll_hor, "", "(lv_obj)i" },
  { "scroll_ver", (void*) &lv_win_scroll_ver, "", "(lv_obj)i" },
  { "set_anim_time", (void*) &lv_win_set_anim_time, "", "(lv_obj)i" },
  { "set_btn_width", (void*) &lv_win_set_btn_width, "", "(lv_obj)i" },
  { "set_content_size", (void*) &lv_win_set_content_size, "", "(lv_obj)ii" },
  { "set_drag", (void*) &lv_win_set_drag, "", "(lv_obj)b" },
  { "set_header_height", (void*) &lv_win_set_header_height, "", "(lv_obj)i" },
  { "set_layout", (void*) &lv_win_set_layout, "", "(lv_obj)i" },
  { "set_scrollbar_mode", (void*) &lv_win_set_scrollbar_mode, "", "(lv_obj)i" },
  { "set_title", (void*) &lv_win_set_title, "", "(lv_obj)s" },
  { "title_get_alignment", (void*) &lv_win_title_get_alignment, "i", "(lv_obj)" },
  { "title_set_alignment", (void*) &lv_win_title_set_alignment, "", "(lv_obj)i" },
};
#endif // BE_LV_WIDGET_WIN


// map of clases
const lvbe_call_c_classes_t lv_classes[] = {
#if BE_LV_WIDGET_ARC
  { "lv_arc", lv_arc_func, sizeof(lv_arc_func) / sizeof(lv_arc_func[0]) },
#endif // BE_LV_WIDGET_ARC
#if BE_LV_WIDGET_BAR
  { "lv_bar", lv_bar_func, sizeof(lv_bar_func) / sizeof(lv_bar_func[0]) },
#endif // BE_LV_WIDGET_BAR
#if BE_LV_WIDGET_BTN
  { "lv_btn", lv_btn_func, sizeof(lv_btn_func) / sizeof(lv_btn_func[0]) },
#endif // BE_LV_WIDGET_BTN
#if BE_LV_WIDGET_BTNMATRIX
  { "lv_btnmatrix", lv_btnmatrix_func, sizeof(lv_btnmatrix_func) / sizeof(lv_btnmatrix_func[0]) },
#endif // BE_LV_WIDGET_BTNMATRIX
#if BE_LV_WIDGET_CALENDAR
  { "lv_calendar", lv_calendar_func, sizeof(lv_calendar_func) / sizeof(lv_calendar_func[0]) },
#endif // BE_LV_WIDGET_CALENDAR
#if BE_LV_WIDGET_CANVAS
  { "lv_canvas", lv_canvas_func, sizeof(lv_canvas_func) / sizeof(lv_canvas_func[0]) },
#endif // BE_LV_WIDGET_CANVAS
#if BE_LV_WIDGET_CHART
  { "lv_chart", lv_chart_func, sizeof(lv_chart_func) / sizeof(lv_chart_func[0]) },
#endif // BE_LV_WIDGET_CHART
#if BE_LV_WIDGET_CHECKBOX
  { "lv_checkbox", lv_checkbox_func, sizeof(lv_checkbox_func) / sizeof(lv_checkbox_func[0]) },
#endif // BE_LV_WIDGET_CHECKBOX
#if BE_LV_WIDGET_CONT
  { "lv_cont", lv_cont_func, sizeof(lv_cont_func) / sizeof(lv_cont_func[0]) },
#endif // BE_LV_WIDGET_CONT
#if BE_LV_WIDGET_CPICKER
  { "lv_cpicker", lv_cpicker_func, sizeof(lv_cpicker_func) / sizeof(lv_cpicker_func[0]) },
#endif // BE_LV_WIDGET_CPICKER
#if BE_LV_WIDGET_DROPDOWN
  { "lv_dropdown", lv_dropdown_func, sizeof(lv_dropdown_func) / sizeof(lv_dropdown_func[0]) },
#endif // BE_LV_WIDGET_DROPDOWN
#if BE_LV_WIDGET_GAUGE
  { "lv_gauge", lv_gauge_func, sizeof(lv_gauge_func) / sizeof(lv_gauge_func[0]) },
#endif // BE_LV_WIDGET_GAUGE
  { "lv_group", lv_group_func, sizeof(lv_group_func) / sizeof(lv_group_func[0]) },
#if BE_LV_WIDGET_IMG
  { "lv_img", lv_img_func, sizeof(lv_img_func) / sizeof(lv_img_func[0]) },
#endif // BE_LV_WIDGET_IMG
#if BE_LV_WIDGET_IMGBTN
  { "lv_imgbtn", lv_imgbtn_func, sizeof(lv_imgbtn_func) / sizeof(lv_imgbtn_func[0]) },
#endif // BE_LV_WIDGET_IMGBTN
  { "lv_indev", lv_indev_func, sizeof(lv_indev_func) / sizeof(lv_indev_func[0]) },
#if BE_LV_WIDGET_KEYBOARD
  { "lv_keyboard", lv_keyboard_func, sizeof(lv_keyboard_func) / sizeof(lv_keyboard_func[0]) },
#endif // BE_LV_WIDGET_KEYBOARD
#if BE_LV_WIDGET_LABEL
  { "lv_label", lv_label_func, sizeof(lv_label_func) / sizeof(lv_label_func[0]) },
#endif // BE_LV_WIDGET_LABEL
#if BE_LV_WIDGET_LED
  { "lv_led", lv_led_func, sizeof(lv_led_func) / sizeof(lv_led_func[0]) },
#endif // BE_LV_WIDGET_LED
#if BE_LV_WIDGET_LINE
  { "lv_line", lv_line_func, sizeof(lv_line_func) / sizeof(lv_line_func[0]) },
#endif // BE_LV_WIDGET_LINE
#if BE_LV_WIDGET_LINEMETER
  { "lv_linemeter", lv_linemeter_func, sizeof(lv_linemeter_func) / sizeof(lv_linemeter_func[0]) },
#endif // BE_LV_WIDGET_LINEMETER
#if BE_LV_WIDGET_LIST
  { "lv_list", lv_list_func, sizeof(lv_list_func) / sizeof(lv_list_func[0]) },
#endif // BE_LV_WIDGET_LIST
#if BE_LV_WIDGET_MSGBOX
  { "lv_msgbox", lv_msgbox_func, sizeof(lv_msgbox_func) / sizeof(lv_msgbox_func[0]) },
#endif // BE_LV_WIDGET_MSGBOX
  { "lv_obj", lv_obj_func, sizeof(lv_obj_func) / sizeof(lv_obj_func[0]) },
#if BE_LV_WIDGET_OBJMASK
  { "lv_objmask", lv_objmask_func, sizeof(lv_objmask_func) / sizeof(lv_objmask_func[0]) },
#endif // BE_LV_WIDGET_OBJMASK
#if BE_LV_WIDGET_PAGE
  { "lv_page", lv_page_func, sizeof(lv_page_func) / sizeof(lv_page_func[0]) },
#endif // BE_LV_WIDGET_PAGE
#if BE_LV_WIDGET_ROLLER
  { "lv_roller", lv_roller_func, sizeof(lv_roller_func) / sizeof(lv_roller_func[0]) },
#endif // BE_LV_WIDGET_ROLLER
#if BE_LV_WIDGET_SLIDER
  { "lv_slider", lv_slider_func, sizeof(lv_slider_func) / sizeof(lv_slider_func[0]) },
#endif // BE_LV_WIDGET_SLIDER
#if BE_LV_WIDGET_SPINBOX
  { "lv_spinbox", lv_spinbox_func, sizeof(lv_spinbox_func) / sizeof(lv_spinbox_func[0]) },
#endif // BE_LV_WIDGET_SPINBOX
#if BE_LV_WIDGET_SPINNER
  { "lv_spinner", lv_spinner_func, sizeof(lv_spinner_func) / sizeof(lv_spinner_func[0]) },
#endif // BE_LV_WIDGET_SPINNER
  { "lv_style", lv_style_func, sizeof(lv_style_func) / sizeof(lv_style_func[0]) },
#if BE_LV_WIDGET_SWITCH
  { "lv_switch", lv_switch_func, sizeof(lv_switch_func) / sizeof(lv_switch_func[0]) },
#endif // BE_LV_WIDGET_SWITCH
#if BE_LV_WIDGET_TABLE
  { "lv_table", lv_table_func, sizeof(lv_table_func) / sizeof(lv_table_func[0]) },
#endif // BE_LV_WIDGET_TABLE
#if BE_LV_WIDGET_TABVIEW
  { "lv_tabview", lv_tabview_func, sizeof(lv_tabview_func) / sizeof(lv_tabview_func[0]) },
#endif // BE_LV_WIDGET_TABVIEW
#if BE_LV_WIDGET_TEXTAREA
  { "lv_textarea", lv_textarea_func, sizeof(lv_textarea_func) / sizeof(lv_textarea_func[0]) },
#endif // BE_LV_WIDGET_TEXTAREA
#if BE_LV_WIDGET_TILEVIEW
  { "lv_tileview", lv_tileview_func, sizeof(lv_tileview_func) / sizeof(lv_tileview_func[0]) },
#endif // BE_LV_WIDGET_TILEVIEW
#if BE_LV_WIDGET_WIN
  { "lv_win", lv_win_func, sizeof(lv_win_func) / sizeof(lv_win_func[0]) },
#endif // BE_LV_WIDGET_WIN
};
const size_t lv_classes_size = sizeof(lv_classes) / sizeof(lv_classes[0]);

  /* `lv_img` methods */
#if BE_LV_WIDGET_IMG
  int lvbe_img_create(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_img_create, "+lv_img", "(lv_obj)(lv_obj)"); }
#endif // BE_LV_WIDGET_IMG

  /* `lv_style` methods */

  /* `lv_group` methods */
  int lvbe_group_create(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_group_create, "+lv_group", ""); }

  /* `lv_indev` methods */

  /* `lv_obj` methods */
  int lvbe_obj_create(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_create, "+lv_obj", "(lv_obj)(lv_obj)"); }

  /* `lv_arc` methods */
#if BE_LV_WIDGET_ARC
  int lvbe_arc_create(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_arc_create, "+lv_arc", "(lv_obj)(lv_obj)"); }
#endif // BE_LV_WIDGET_ARC

  /* `lv_bar` methods */
#if BE_LV_WIDGET_BAR
  int lvbe_bar_create(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_bar_create, "+lv_bar", "(lv_obj)(lv_obj)"); }
#endif // BE_LV_WIDGET_BAR

  /* `lv_btn` methods */
#if BE_LV_WIDGET_BTN
  int lvbe_btn_create(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_btn_create, "+lv_btn", "(lv_obj)(lv_obj)"); }
#endif // BE_LV_WIDGET_BTN

  /* `lv_btnmatrix` methods */
#if BE_LV_WIDGET_BTNMATRIX
  int lvbe_btnmatrix_create(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_btnmatrix_create, "+lv_btnmatrix", "(lv_obj)(lv_obj)"); }
#endif // BE_LV_WIDGET_BTNMATRIX

  /* `lv_calendar` methods */
#if BE_LV_WIDGET_CALENDAR
  int lvbe_calendar_create(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_calendar_create, "+lv_calendar", "(lv_obj)(lv_obj)"); }
#endif // BE_LV_WIDGET_CALENDAR

  /* `lv_canvas` methods */
#if BE_LV_WIDGET_CANVAS
  int lvbe_canvas_create(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_canvas_create, "+lv_canvas", "(lv_obj)(lv_obj)"); }
#endif // BE_LV_WIDGET_CANVAS

  /* `lv_chart` methods */
#if BE_LV_WIDGET_CHART
  int lvbe_chart_create(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_chart_create, "+lv_chart", "(lv_obj)(lv_obj)"); }
#endif // BE_LV_WIDGET_CHART

  /* `lv_checkbox` methods */
#if BE_LV_WIDGET_CHECKBOX
  int lvbe_checkbox_create(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_checkbox_create, "+lv_checkbox", "(lv_obj)(lv_obj)"); }
#endif // BE_LV_WIDGET_CHECKBOX

  /* `lv_cont` methods */
#if BE_LV_WIDGET_CONT
  int lvbe_cont_create(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_cont_create, "+lv_cont", "(lv_obj)(lv_obj)"); }
#endif // BE_LV_WIDGET_CONT

  /* `lv_cpicker` methods */
#if BE_LV_WIDGET_CPICKER
  int lvbe_cpicker_create(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_cpicker_create, "+lv_cpicker", "(lv_obj)(lv_obj)"); }
#endif // BE_LV_WIDGET_CPICKER

  /* `lv_dropdown` methods */
#if BE_LV_WIDGET_DROPDOWN
  int lvbe_dropdown_create(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_dropdown_create, "+lv_dropdown", "(lv_obj)(lv_obj)"); }
#endif // BE_LV_WIDGET_DROPDOWN

  /* `lv_gauge` methods */
#if BE_LV_WIDGET_GAUGE
  int lvbe_gauge_create(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_gauge_create, "+lv_gauge", "(lv_obj)(lv_obj)"); }
#endif // BE_LV_WIDGET_GAUGE

  /* `lv_imgbtn` methods */
#if BE_LV_WIDGET_IMGBTN
  int lvbe_imgbtn_create(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_imgbtn_create, "+lv_imgbtn", "(lv_obj)(lv_obj)"); }
#endif // BE_LV_WIDGET_IMGBTN

  /* `lv_keyboard` methods */
#if BE_LV_WIDGET_KEYBOARD
  int lvbe_keyboard_create(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_keyboard_create, "+lv_keyboard", "(lv_obj)(lv_obj)"); }
#endif // BE_LV_WIDGET_KEYBOARD

  /* `lv_label` methods */
#if BE_LV_WIDGET_LABEL
  int lvbe_label_create(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_label_create, "+lv_label", "(lv_obj)(lv_obj)"); }
#endif // BE_LV_WIDGET_LABEL

  /* `lv_led` methods */
#if BE_LV_WIDGET_LED
  int lvbe_led_create(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_led_create, "+lv_led", "(lv_obj)(lv_obj)"); }
#endif // BE_LV_WIDGET_LED

  /* `lv_line` methods */
#if BE_LV_WIDGET_LINE
  int lvbe_line_create(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_line_create, "+lv_line", "(lv_obj)(lv_obj)"); }
#endif // BE_LV_WIDGET_LINE

  /* `lv_linemeter` methods */
#if BE_LV_WIDGET_LINEMETER
  int lvbe_linemeter_create(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_linemeter_create, "+lv_linemeter", "(lv_obj)(lv_obj)"); }
#endif // BE_LV_WIDGET_LINEMETER

  /* `lv_list` methods */
#if BE_LV_WIDGET_LIST
  int lvbe_list_create(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_list_create, "+lv_list", "(lv_obj)(lv_obj)"); }
#endif // BE_LV_WIDGET_LIST

  /* `lv_msgbox` methods */
#if BE_LV_WIDGET_MSGBOX
  int lvbe_msgbox_create(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_msgbox_create, "+lv_msgbox", "(lv_obj)(lv_obj)"); }
#endif // BE_LV_WIDGET_MSGBOX

  /* `lv_objmask` methods */
#if BE_LV_WIDGET_OBJMASK
  int lvbe_objmask_create(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_objmask_create, "+lv_objmask", "(lv_obj)(lv_obj)"); }
#endif // BE_LV_WIDGET_OBJMASK

  /* `lv_page` methods */
#if BE_LV_WIDGET_PAGE
  int lvbe_page_create(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_page_create, "+lv_page", "(lv_obj)(lv_obj)"); }
#endif // BE_LV_WIDGET_PAGE

  /* `lv_roller` methods */
#if BE_LV_WIDGET_ROLLER
  int lvbe_roller_create(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_roller_create, "+lv_roller", "(lv_obj)(lv_obj)"); }
#endif // BE_LV_WIDGET_ROLLER

  /* `lv_slider` methods */
#if BE_LV_WIDGET_SLIDER
  int lvbe_slider_create(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_slider_create, "+lv_slider", "(lv_obj)(lv_obj)"); }
#endif // BE_LV_WIDGET_SLIDER

  /* `lv_spinbox` methods */
#if BE_LV_WIDGET_SPINBOX
  int lvbe_spinbox_create(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_spinbox_create, "+lv_spinbox", "(lv_obj)(lv_obj)"); }
#endif // BE_LV_WIDGET_SPINBOX

  /* `lv_spinner` methods */
#if BE_LV_WIDGET_SPINNER
  int lvbe_spinner_create(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_spinner_create, "+lv_spinner", "(lv_obj)(lv_obj)"); }
#endif // BE_LV_WIDGET_SPINNER

  /* `lv_switch` methods */
#if BE_LV_WIDGET_SWITCH
  int lvbe_switch_create(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_switch_create, "+lv_switch", "(lv_obj)(lv_obj)"); }
#endif // BE_LV_WIDGET_SWITCH

  /* `lv_table` methods */
#if BE_LV_WIDGET_TABLE
  int lvbe_table_create(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_table_create, "+lv_table", "(lv_obj)(lv_obj)"); }
#endif // BE_LV_WIDGET_TABLE

  /* `lv_tabview` methods */
#if BE_LV_WIDGET_TABVIEW
  int lvbe_tabview_create(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_tabview_create, "+lv_tabview", "(lv_obj)(lv_obj)"); }
#endif // BE_LV_WIDGET_TABVIEW

  /* `lv_textarea` methods */
#if BE_LV_WIDGET_TEXTAREA
  int lvbe_textarea_create(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_create, "+lv_textarea", "(lv_obj)(lv_obj)"); }
#endif // BE_LV_WIDGET_TEXTAREA

  /* `lv_tileview` methods */
#if BE_LV_WIDGET_TILEVIEW
  int lvbe_tileview_create(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_tileview_create, "+lv_tileview", "(lv_obj)(lv_obj)"); }
#endif // BE_LV_WIDGET_TILEVIEW

  /* `lv_win` methods */
#if BE_LV_WIDGET_WIN
  int lvbe_win_create(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_win_create, "+lv_win", "(lv_obj)(lv_obj)"); }
#endif // BE_LV_WIDGET_WIN


  extern void be_load_lv_img_lib(bvm *vm);
  extern void be_load_lv_style_lib(bvm *vm);
  extern void be_load_lv_group_lib(bvm *vm);
  extern void be_load_lv_indev_lib(bvm *vm);
  extern void be_load_lv_obj_lib(bvm *vm);
  extern void be_load_lv_arc_lib(bvm *vm);
  extern void be_load_lv_bar_lib(bvm *vm);
  extern void be_load_lv_btn_lib(bvm *vm);
  extern void be_load_lv_btnmatrix_lib(bvm *vm);
  extern void be_load_lv_calendar_lib(bvm *vm);
  extern void be_load_lv_canvas_lib(bvm *vm);
  extern void be_load_lv_chart_lib(bvm *vm);
  extern void be_load_lv_checkbox_lib(bvm *vm);
  extern void be_load_lv_cont_lib(bvm *vm);
  extern void be_load_lv_cpicker_lib(bvm *vm);
  extern void be_load_lv_dropdown_lib(bvm *vm);
  extern void be_load_lv_gauge_lib(bvm *vm);
  extern void be_load_lv_imgbtn_lib(bvm *vm);
  extern void be_load_lv_keyboard_lib(bvm *vm);
  extern void be_load_lv_label_lib(bvm *vm);
  extern void be_load_lv_led_lib(bvm *vm);
  extern void be_load_lv_line_lib(bvm *vm);
  extern void be_load_lv_linemeter_lib(bvm *vm);
  extern void be_load_lv_list_lib(bvm *vm);
  extern void be_load_lv_msgbox_lib(bvm *vm);
  extern void be_load_lv_objmask_lib(bvm *vm);
  extern void be_load_lv_page_lib(bvm *vm);
  extern void be_load_lv_roller_lib(bvm *vm);
  extern void be_load_lv_slider_lib(bvm *vm);
  extern void be_load_lv_spinbox_lib(bvm *vm);
  extern void be_load_lv_spinner_lib(bvm *vm);
  extern void be_load_lv_switch_lib(bvm *vm);
  extern void be_load_lv_table_lib(bvm *vm);
  extern void be_load_lv_tabview_lib(bvm *vm);
  extern void be_load_lv_textarea_lib(bvm *vm);
  extern void be_load_lv_tileview_lib(bvm *vm);
  extern void be_load_lv_win_lib(bvm *vm);

  void be_load_lv_all_lib(bvm *vm) {
#if BE_LV_WIDGET_IMG
    be_load_lv_img_lib(vm);
#endif
    be_load_lv_style_lib(vm);
    be_load_lv_group_lib(vm);
    be_load_lv_indev_lib(vm);
    be_load_lv_obj_lib(vm);
#if BE_LV_WIDGET_ARC
    be_load_lv_arc_lib(vm);
#endif
#if BE_LV_WIDGET_BAR
    be_load_lv_bar_lib(vm);
#endif
#if BE_LV_WIDGET_BTN
    be_load_lv_btn_lib(vm);
#endif
#if BE_LV_WIDGET_BTNMATRIX
    be_load_lv_btnmatrix_lib(vm);
#endif
#if BE_LV_WIDGET_CALENDAR
    be_load_lv_calendar_lib(vm);
#endif
#if BE_LV_WIDGET_CANVAS
    be_load_lv_canvas_lib(vm);
#endif
#if BE_LV_WIDGET_CHART
    be_load_lv_chart_lib(vm);
#endif
#if BE_LV_WIDGET_CHECKBOX
    be_load_lv_checkbox_lib(vm);
#endif
#if BE_LV_WIDGET_CONT
    be_load_lv_cont_lib(vm);
#endif
#if BE_LV_WIDGET_CPICKER
    be_load_lv_cpicker_lib(vm);
#endif
#if BE_LV_WIDGET_DROPDOWN
    be_load_lv_dropdown_lib(vm);
#endif
#if BE_LV_WIDGET_GAUGE
    be_load_lv_gauge_lib(vm);
#endif
#if BE_LV_WIDGET_IMGBTN
    be_load_lv_imgbtn_lib(vm);
#endif
#if BE_LV_WIDGET_KEYBOARD
    be_load_lv_keyboard_lib(vm);
#endif
#if BE_LV_WIDGET_LABEL
    be_load_lv_label_lib(vm);
#endif
#if BE_LV_WIDGET_LED
    be_load_lv_led_lib(vm);
#endif
#if BE_LV_WIDGET_LINE
    be_load_lv_line_lib(vm);
#endif
#if BE_LV_WIDGET_LINEMETER
    be_load_lv_linemeter_lib(vm);
#endif
#if BE_LV_WIDGET_LIST
    be_load_lv_list_lib(vm);
#endif
#if BE_LV_WIDGET_MSGBOX
    be_load_lv_msgbox_lib(vm);
#endif
#if BE_LV_WIDGET_OBJMASK
    be_load_lv_objmask_lib(vm);
#endif
#if BE_LV_WIDGET_PAGE
    be_load_lv_page_lib(vm);
#endif
#if BE_LV_WIDGET_ROLLER
    be_load_lv_roller_lib(vm);
#endif
#if BE_LV_WIDGET_SLIDER
    be_load_lv_slider_lib(vm);
#endif
#if BE_LV_WIDGET_SPINBOX
    be_load_lv_spinbox_lib(vm);
#endif
#if BE_LV_WIDGET_SPINNER
    be_load_lv_spinner_lib(vm);
#endif
#if BE_LV_WIDGET_SWITCH
    be_load_lv_switch_lib(vm);
#endif
#if BE_LV_WIDGET_TABLE
    be_load_lv_table_lib(vm);
#endif
#if BE_LV_WIDGET_TABVIEW
    be_load_lv_tabview_lib(vm);
#endif
#if BE_LV_WIDGET_TEXTAREA
    be_load_lv_textarea_lib(vm);
#endif
#if BE_LV_WIDGET_TILEVIEW
    be_load_lv_tileview_lib(vm);
#endif
#if BE_LV_WIDGET_WIN
    be_load_lv_win_lib(vm);
#endif
  };
#ifdef __cplusplus
} /* extern "C" */
#endif
/********************************************************************/
