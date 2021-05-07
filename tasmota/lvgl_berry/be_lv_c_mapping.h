
/********************************************************************
 * Generated code, don't edit
  *******************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

  /* `lv_img` methods */
  int lvbe_img_set_tasmota_logo(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_img_set_tasmota_logo, "", "(lv_obj)"); }
  int lvbe_img_create(bvm *vm)       { return lvx_init_2(vm, (void*) &lv_img_create, "lv_img", "(lv_obj)(lv_obj)"); }
  int lvbe_img_set_src(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_img_set_src, "", "(lv_obj)i"); }
  int lvbe_img_set_auto_size(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_img_set_auto_size, "", "(lv_obj)b"); }
  int lvbe_img_set_offset_x(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_img_set_offset_x, "", "(lv_obj)i"); }
  int lvbe_img_set_offset_y(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_img_set_offset_y, "", "(lv_obj)i"); }
  int lvbe_img_set_pivot(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_img_set_pivot, "", "(lv_obj)ii"); }
  int lvbe_img_set_angle(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_img_set_angle, "", "(lv_obj)i"); }
  int lvbe_img_set_zoom(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_img_set_zoom, "", "(lv_obj)i"); }
  int lvbe_img_set_antialias(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_img_set_antialias, "", "(lv_obj)b"); }
  int lvbe_img_get_src(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_img_get_src, "i", "(lv_obj)"); }
  int lvbe_img_get_file_name(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_img_get_file_name, "s", "(lv_obj)"); }
  int lvbe_img_get_auto_size(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_img_get_auto_size, "b", "(lv_obj)"); }
  int lvbe_img_get_offset_x(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_img_get_offset_x, "i", "(lv_obj)"); }
  int lvbe_img_get_offset_y(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_img_get_offset_y, "i", "(lv_obj)"); }
  int lvbe_img_get_angle(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_img_get_angle, "i", "(lv_obj)"); }
  int lvbe_img_get_pivot(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_img_get_pivot, "", "(lv_obj)(lv_point)"); }
  int lvbe_img_get_zoom(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_img_get_zoom, "i", "(lv_obj)"); }
  int lvbe_img_get_antialias(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_img_get_antialias, "b", "(lv_obj)"); }

  /* `lv_style` methods */
  int lvbe_style_copy(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_copy, "", "(lv_style)(lv_style)"); }
  int lvbe_style_list_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_list_init, "", "(lv_style_list)"); }
  int lvbe_style_list_copy(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_list_copy, "", "(lv_style_list)(lv_style_list)"); }
  int lvbe_style_list_get_style(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_list_get_style, "lv_style", "(lv_style_list)i"); }
  int lvbe_style_reset(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_reset, "", "(lv_style)"); }
  int lvbe_style_remove_prop(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_remove_prop, "b", "(lv_style)(lv_style_property)"); }
  int lvbe_style_list_get_local_style(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_list_get_local_style, "lv_style", "(lv_style_list)"); }
  int lvbe_style_set_radius(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_radius, "", "(lv_style)ii"); }
  int lvbe_style_set_clip_corner(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_clip_corner, "", "(lv_style)ib"); }
  int lvbe_style_set_size(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_size, "", "(lv_style)ii"); }
  int lvbe_style_set_transform_width(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_transform_width, "", "(lv_style)ii"); }
  int lvbe_style_set_transform_height(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_transform_height, "", "(lv_style)ii"); }
  int lvbe_style_set_transform_angle(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_transform_angle, "", "(lv_style)ii"); }
  int lvbe_style_set_transform_zoom(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_transform_zoom, "", "(lv_style)ii"); }
  int lvbe_style_set_opa_scale(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_opa_scale, "", "(lv_style)ii"); }
  int lvbe_style_set_pad_top(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_pad_top, "", "(lv_style)ii"); }
  int lvbe_style_set_pad_bottom(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_pad_bottom, "", "(lv_style)ii"); }
  int lvbe_style_set_pad_left(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_pad_left, "", "(lv_style)ii"); }
  int lvbe_style_set_pad_right(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_pad_right, "", "(lv_style)ii"); }
  int lvbe_style_set_pad_inner(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_pad_inner, "", "(lv_style)ii"); }
  int lvbe_style_set_margin_top(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_margin_top, "", "(lv_style)ii"); }
  int lvbe_style_set_margin_bottom(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_margin_bottom, "", "(lv_style)ii"); }
  int lvbe_style_set_margin_left(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_margin_left, "", "(lv_style)ii"); }
  int lvbe_style_set_margin_right(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_margin_right, "", "(lv_style)ii"); }
  int lvbe_style_set_bg_blend_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_bg_blend_mode, "", "(lv_style)ii"); }
  int lvbe_style_set_bg_main_stop(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_bg_main_stop, "", "(lv_style)ii"); }
  int lvbe_style_set_bg_grad_stop(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_bg_grad_stop, "", "(lv_style)ii"); }
  int lvbe_style_set_bg_grad_dir(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_bg_grad_dir, "", "(lv_style)ii"); }
  int lvbe_style_set_bg_color(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_bg_color, "", "(lv_style)i(lv_color)"); }
  int lvbe_style_set_bg_grad_color(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_bg_grad_color, "", "(lv_style)i(lv_color)"); }
  int lvbe_style_set_bg_opa(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_bg_opa, "", "(lv_style)ii"); }
  int lvbe_style_set_border_width(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_border_width, "", "(lv_style)ii"); }
  int lvbe_style_set_border_side(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_border_side, "", "(lv_style)ii"); }
  int lvbe_style_set_border_blend_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_border_blend_mode, "", "(lv_style)ii"); }
  int lvbe_style_set_border_post(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_border_post, "", "(lv_style)ib"); }
  int lvbe_style_set_border_color(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_border_color, "", "(lv_style)i(lv_color)"); }
  int lvbe_style_set_border_opa(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_border_opa, "", "(lv_style)ii"); }
  int lvbe_style_set_outline_width(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_outline_width, "", "(lv_style)ii"); }
  int lvbe_style_set_outline_pad(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_outline_pad, "", "(lv_style)ii"); }
  int lvbe_style_set_outline_blend_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_outline_blend_mode, "", "(lv_style)ii"); }
  int lvbe_style_set_outline_color(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_outline_color, "", "(lv_style)i(lv_color)"); }
  int lvbe_style_set_outline_opa(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_outline_opa, "", "(lv_style)ii"); }
  int lvbe_style_set_shadow_width(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_shadow_width, "", "(lv_style)ii"); }
  int lvbe_style_set_shadow_ofs_x(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_shadow_ofs_x, "", "(lv_style)ii"); }
  int lvbe_style_set_shadow_ofs_y(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_shadow_ofs_y, "", "(lv_style)ii"); }
  int lvbe_style_set_shadow_spread(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_shadow_spread, "", "(lv_style)ii"); }
  int lvbe_style_set_shadow_blend_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_shadow_blend_mode, "", "(lv_style)ii"); }
  int lvbe_style_set_shadow_color(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_shadow_color, "", "(lv_style)i(lv_color)"); }
  int lvbe_style_set_shadow_opa(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_shadow_opa, "", "(lv_style)ii"); }
  int lvbe_style_set_pattern_repeat(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_pattern_repeat, "", "(lv_style)ib"); }
  int lvbe_style_set_pattern_blend_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_pattern_blend_mode, "", "(lv_style)ii"); }
  int lvbe_style_set_pattern_recolor(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_pattern_recolor, "", "(lv_style)i(lv_color)"); }
  int lvbe_style_set_pattern_opa(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_pattern_opa, "", "(lv_style)ii"); }
  int lvbe_style_set_pattern_recolor_opa(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_pattern_recolor_opa, "", "(lv_style)ii"); }
  int lvbe_style_set_pattern_image(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_pattern_image, "", "(lv_style)ii"); }
  int lvbe_style_set_value_letter_space(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_value_letter_space, "", "(lv_style)ii"); }
  int lvbe_style_set_value_line_space(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_value_line_space, "", "(lv_style)ii"); }
  int lvbe_style_set_value_blend_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_value_blend_mode, "", "(lv_style)ii"); }
  int lvbe_style_set_value_ofs_x(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_value_ofs_x, "", "(lv_style)ii"); }
  int lvbe_style_set_value_ofs_y(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_value_ofs_y, "", "(lv_style)ii"); }
  int lvbe_style_set_value_align(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_value_align, "", "(lv_style)ii"); }
  int lvbe_style_set_value_color(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_value_color, "", "(lv_style)i(lv_color)"); }
  int lvbe_style_set_value_opa(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_value_opa, "", "(lv_style)ii"); }
  int lvbe_style_set_value_font(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_value_font, "", "(lv_style)i(lv_font)"); }
  int lvbe_style_set_value_str(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_value_str, "", "(lv_style)is"); }
  int lvbe_style_set_text_letter_space(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_text_letter_space, "", "(lv_style)ii"); }
  int lvbe_style_set_text_line_space(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_text_line_space, "", "(lv_style)ii"); }
  int lvbe_style_set_text_decor(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_text_decor, "", "(lv_style)i(lv_text_decor)"); }
  int lvbe_style_set_text_blend_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_text_blend_mode, "", "(lv_style)ii"); }
  int lvbe_style_set_text_color(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_text_color, "", "(lv_style)i(lv_color)"); }
  int lvbe_style_set_text_sel_color(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_text_sel_color, "", "(lv_style)i(lv_color)"); }
  int lvbe_style_set_text_sel_bg_color(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_text_sel_bg_color, "", "(lv_style)i(lv_color)"); }
  int lvbe_style_set_text_opa(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_text_opa, "", "(lv_style)ii"); }
  int lvbe_style_set_text_font(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_text_font, "", "(lv_style)i(lv_font)"); }
  int lvbe_style_set_line_width(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_line_width, "", "(lv_style)ii"); }
  int lvbe_style_set_line_blend_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_line_blend_mode, "", "(lv_style)ii"); }
  int lvbe_style_set_line_dash_width(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_line_dash_width, "", "(lv_style)ii"); }
  int lvbe_style_set_line_dash_gap(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_line_dash_gap, "", "(lv_style)ii"); }
  int lvbe_style_set_line_rounded(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_line_rounded, "", "(lv_style)ib"); }
  int lvbe_style_set_line_color(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_line_color, "", "(lv_style)i(lv_color)"); }
  int lvbe_style_set_line_opa(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_line_opa, "", "(lv_style)ii"); }
  int lvbe_style_set_image_blend_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_image_blend_mode, "", "(lv_style)ii"); }
  int lvbe_style_set_image_recolor(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_image_recolor, "", "(lv_style)i(lv_color)"); }
  int lvbe_style_set_image_opa(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_image_opa, "", "(lv_style)ii"); }
  int lvbe_style_set_image_recolor_opa(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_image_recolor_opa, "", "(lv_style)ii"); }
  int lvbe_style_set_transition_time(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_transition_time, "", "(lv_style)ii"); }
  int lvbe_style_set_transition_delay(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_transition_delay, "", "(lv_style)ii"); }
  int lvbe_style_set_transition_prop_1(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_transition_prop_1, "", "(lv_style)ii"); }
  int lvbe_style_set_transition_prop_2(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_transition_prop_2, "", "(lv_style)ii"); }
  int lvbe_style_set_transition_prop_3(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_transition_prop_3, "", "(lv_style)ii"); }
  int lvbe_style_set_transition_prop_4(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_transition_prop_4, "", "(lv_style)ii"); }
  int lvbe_style_set_transition_prop_5(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_transition_prop_5, "", "(lv_style)ii"); }
  int lvbe_style_set_transition_prop_6(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_transition_prop_6, "", "(lv_style)ii"); }
  int lvbe_style_set_transition_path(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_transition_path, "", "(lv_style)i(lv_anim_path)"); }
  int lvbe_style_set_scale_width(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_scale_width, "", "(lv_style)ii"); }
  int lvbe_style_set_scale_border_width(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_scale_border_width, "", "(lv_style)ii"); }
  int lvbe_style_set_scale_end_border_width(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_scale_end_border_width, "", "(lv_style)ii"); }
  int lvbe_style_set_scale_end_line_width(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_scale_end_line_width, "", "(lv_style)ii"); }
  int lvbe_style_set_scale_grad_color(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_scale_grad_color, "", "(lv_style)i(lv_color)"); }
  int lvbe_style_set_scale_end_color(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_style_set_scale_end_color, "", "(lv_style)i(lv_color)"); }

  /* `lv_group` methods */
  int lvbe_group_create(bvm *vm)       { return lvx_init_2(vm, (void*) &lv_group_create, "lv_group", ""); }
  int lvbe_group_del(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_group_del, "", "(lv_group)"); }
  int lvbe_group_add_obj(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_group_add_obj, "", "(lv_group)(lv_obj)"); }
  int lvbe_group_remove_obj(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_group_remove_obj, "", "(lv_obj)"); }
  int lvbe_group_remove_all_objs(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_group_remove_all_objs, "", "(lv_group)"); }
  int lvbe_group_focus_obj(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_group_focus_obj, "", "(lv_obj)"); }
  int lvbe_group_focus_next(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_group_focus_next, "", "(lv_group)"); }
  int lvbe_group_focus_prev(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_group_focus_prev, "", "(lv_group)"); }
  int lvbe_group_focus_freeze(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_group_focus_freeze, "", "(lv_group)b"); }
  int lvbe_group_send_data(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_group_send_data, "i", "(lv_group)i"); }
  int lvbe_group_set_focus_cb(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_group_set_focus_cb, "", "(lv_group)&0"); }
  int lvbe_group_set_refocus_policy(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_group_set_refocus_policy, "", "(lv_group)(lv_group_refocus_policy)"); }
  int lvbe_group_set_editing(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_group_set_editing, "", "(lv_group)b"); }
  int lvbe_group_set_click_focus(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_group_set_click_focus, "", "(lv_group)b"); }
  int lvbe_group_set_wrap(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_group_set_wrap, "", "(lv_group)b"); }
  int lvbe_group_get_focused(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_group_get_focused, "lv_obj", "(lv_group)"); }
  int lvbe_group_get_editing(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_group_get_editing, "b", "(lv_group)"); }
  int lvbe_group_get_click_focus(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_group_get_click_focus, "b", "(lv_group)"); }
  int lvbe_group_get_wrap(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_group_get_wrap, "b", "(lv_group)"); }

  /* `lv_indev` methods */
  int lvbe_indev_get_type(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_indev_get_type, "i", "(lv_indev)"); }
  int lvbe_indev_enable(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_indev_enable, "", "(lv_indev)b"); }
  int lvbe_indev_set_group(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_indev_set_group, "", "(lv_indev)(lv_group)"); }
  int lvbe_indev_get_obj_act(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_indev_get_obj_act, "lv_obj", ""); }
  int lvbe_indev_search_obj(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_indev_search_obj, "lv_obj", "(lv_obj)(lv_point)"); }

  /* `lv_obj` methods */
  int lvbe_obj_create(bvm *vm)       { return lvx_init_2(vm, (void*) &lv_obj_create, "lv_obj", "(lv_obj)(lv_obj)"); }
  int lvbe_obj_del(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_del, "i", "(lv_obj)"); }
  int lvbe_obj_del_anim_ready_cb(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_del_anim_ready_cb, "", "(lv_anim)"); }
  int lvbe_obj_del_async(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_del_async, "", "(_lv_obj)"); }
  int lvbe_obj_clean(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_clean, "", "(lv_obj)"); }
  int lvbe_obj_invalidate_area(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_invalidate_area, "", "(lv_obj)(lv_area)"); }
  int lvbe_obj_invalidate(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_invalidate, "", "(lv_obj)"); }
  int lvbe_obj_area_is_visible(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_area_is_visible, "b", "(lv_obj)(lv_area)"); }
  int lvbe_obj_is_visible(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_is_visible, "b", "(lv_obj)"); }
  int lvbe_obj_set_parent(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_parent, "", "(lv_obj)(lv_obj)"); }
  int lvbe_obj_move_foreground(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_move_foreground, "", "(lv_obj)"); }
  int lvbe_obj_move_background(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_move_background, "", "(lv_obj)"); }
  int lvbe_obj_set_pos(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_pos, "", "(lv_obj)ii"); }
  int lvbe_obj_set_x(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_x, "", "(lv_obj)i"); }
  int lvbe_obj_set_y(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_y, "", "(lv_obj)i"); }
  int lvbe_obj_set_size(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_size, "", "(lv_obj)ii"); }
  int lvbe_obj_set_width(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_width, "", "(lv_obj)i"); }
  int lvbe_obj_set_height(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_height, "", "(lv_obj)i"); }
  int lvbe_obj_set_width_fit(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_width_fit, "", "(lv_obj)i"); }
  int lvbe_obj_set_height_fit(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_height_fit, "", "(lv_obj)i"); }
  int lvbe_obj_set_width_margin(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_width_margin, "", "(lv_obj)i"); }
  int lvbe_obj_set_height_margin(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_height_margin, "", "(lv_obj)i"); }
  int lvbe_obj_align(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_align, "", "(lv_obj)(lv_obj)iii"); }
  int lvbe_obj_align_x(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_align_x, "", "(lv_obj)(lv_obj)ii"); }
  int lvbe_obj_align_y(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_align_y, "", "(lv_obj)(lv_obj)ii"); }
  int lvbe_obj_align_mid(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_align_mid, "", "(lv_obj)(lv_obj)iii"); }
  int lvbe_obj_align_mid_x(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_align_mid_x, "", "(lv_obj)(lv_obj)ii"); }
  int lvbe_obj_align_mid_y(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_align_mid_y, "", "(lv_obj)(lv_obj)ii"); }
  int lvbe_obj_realign(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_realign, "", "(lv_obj)"); }
  int lvbe_obj_set_auto_realign(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_auto_realign, "", "(lv_obj)b"); }
  int lvbe_obj_set_ext_click_area(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_ext_click_area, "", "(lv_obj)iiii"); }
  int lvbe_obj_add_style(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_add_style, "", "(lv_obj)i(lv_style)"); }
  int lvbe_obj_remove_style(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_remove_style, "", "(lv_obj)i(lv_style)"); }
  int lvbe_obj_clean_style_list(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_clean_style_list, "", "(lv_obj)i"); }
  int lvbe_obj_reset_style_list(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_reset_style_list, "", "(lv_obj)i"); }
  int lvbe_obj_refresh_style(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_refresh_style, "", "(lv_obj)i(lv_style_property)"); }
  int lvbe_obj_report_style_mod(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_report_style_mod, "", "(lv_style)"); }
  int lvbe_obj_remove_style_local_prop(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_remove_style_local_prop, "b", "(lv_obj)i(lv_style_property)"); }
  int lvbe_obj_set_hidden(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_hidden, "", "(lv_obj)b"); }
  int lvbe_obj_set_adv_hittest(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_adv_hittest, "", "(lv_obj)b"); }
  int lvbe_obj_set_click(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_click, "", "(lv_obj)b"); }
  int lvbe_obj_set_top(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_top, "", "(lv_obj)b"); }
  int lvbe_obj_set_drag(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_drag, "", "(lv_obj)b"); }
  int lvbe_obj_set_drag_dir(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_drag_dir, "", "(lv_obj)i"); }
  int lvbe_obj_set_drag_throw(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_drag_throw, "", "(lv_obj)b"); }
  int lvbe_obj_set_drag_parent(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_drag_parent, "", "(lv_obj)b"); }
  int lvbe_obj_set_focus_parent(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_focus_parent, "", "(lv_obj)b"); }
  int lvbe_obj_set_gesture_parent(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_gesture_parent, "", "(lv_obj)b"); }
  int lvbe_obj_set_parent_event(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_parent_event, "", "(lv_obj)b"); }
  int lvbe_obj_set_base_dir(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_base_dir, "", "(lv_obj)i"); }
  int lvbe_obj_add_protect(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_add_protect, "", "(lv_obj)i"); }
  int lvbe_obj_clear_protect(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_clear_protect, "", "(lv_obj)i"); }
  int lvbe_obj_set_state(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_state, "", "(lv_obj)i"); }
  int lvbe_obj_add_state(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_add_state, "", "(lv_obj)i"); }
  int lvbe_obj_clear_state(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_clear_state, "", "(lv_obj)i"); }
  int lvbe_obj_finish_transitions(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_finish_transitions, "", "(lv_obj)i"); }
  int lvbe_obj_set_event_cb(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_event_cb, "", "(lv_obj)&1"); }
  int lvbe_obj_set_signal_cb(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_signal_cb, "", "(lv_obj)&2"); }
  int lvbe_obj_set_design_cb(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_design_cb, "", "(lv_obj)&3"); }
  int lvbe_obj_allocate_ext_attr(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_allocate_ext_attr, "i", "(lv_obj)i"); }
  int lvbe_obj_refresh_ext_draw_pad(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_refresh_ext_draw_pad, "", "(lv_obj)"); }
  int lvbe_obj_get_screen(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_screen, "lv_obj", "(lv_obj)"); }
  int lvbe_obj_get_parent(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_parent, "lv_obj", "(lv_obj)"); }
  int lvbe_obj_get_child(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_child, "lv_obj", "(lv_obj)(lv_obj)"); }
  int lvbe_obj_get_child_back(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_child_back, "lv_obj", "(lv_obj)(lv_obj)"); }
  int lvbe_obj_count_children(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_count_children, "i", "(lv_obj)"); }
  int lvbe_obj_count_children_recursive(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_count_children_recursive, "i", "(lv_obj)"); }
  int lvbe_obj_get_coords(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_coords, "", "(lv_obj)(lv_area)"); }
  int lvbe_obj_get_inner_coords(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_inner_coords, "", "(lv_obj)(lv_area)"); }
  int lvbe_obj_get_x(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_x, "i", "(lv_obj)"); }
  int lvbe_obj_get_y(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_y, "i", "(lv_obj)"); }
  int lvbe_obj_get_width(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_width, "i", "(lv_obj)"); }
  int lvbe_obj_get_height(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_height, "i", "(lv_obj)"); }
  int lvbe_obj_get_width_fit(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_width_fit, "i", "(lv_obj)"); }
  int lvbe_obj_get_height_fit(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_height_fit, "i", "(lv_obj)"); }
  int lvbe_obj_get_height_margin(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_height_margin, "i", "(lv_obj)"); }
  int lvbe_obj_get_width_margin(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_width_margin, "i", "(lv_obj)"); }
  int lvbe_obj_get_width_grid(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_width_grid, "i", "(lv_obj)ii"); }
  int lvbe_obj_get_height_grid(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_height_grid, "i", "(lv_obj)ii"); }
  int lvbe_obj_get_auto_realign(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_auto_realign, "b", "(lv_obj)"); }
  int lvbe_obj_get_ext_click_pad_left(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_ext_click_pad_left, "i", "(lv_obj)"); }
  int lvbe_obj_get_ext_click_pad_right(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_ext_click_pad_right, "i", "(lv_obj)"); }
  int lvbe_obj_get_ext_click_pad_top(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_ext_click_pad_top, "i", "(lv_obj)"); }
  int lvbe_obj_get_ext_click_pad_bottom(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_ext_click_pad_bottom, "i", "(lv_obj)"); }
  int lvbe_obj_get_ext_draw_pad(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_ext_draw_pad, "i", "(lv_obj)"); }
  int lvbe_obj_get_local_style(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_local_style, "lv_style", "(lv_obj)i"); }
  int lvbe_obj_get_hidden(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_hidden, "b", "(lv_obj)"); }
  int lvbe_obj_get_adv_hittest(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_adv_hittest, "b", "(lv_obj)"); }
  int lvbe_obj_get_click(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_click, "b", "(lv_obj)"); }
  int lvbe_obj_get_top(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_top, "b", "(lv_obj)"); }
  int lvbe_obj_get_drag(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_drag, "b", "(lv_obj)"); }
  int lvbe_obj_get_drag_dir(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_drag_dir, "i", "(lv_obj)"); }
  int lvbe_obj_get_drag_throw(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_drag_throw, "b", "(lv_obj)"); }
  int lvbe_obj_get_drag_parent(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_drag_parent, "b", "(lv_obj)"); }
  int lvbe_obj_get_focus_parent(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_focus_parent, "b", "(lv_obj)"); }
  int lvbe_obj_get_parent_event(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_parent_event, "b", "(lv_obj)"); }
  int lvbe_obj_get_gesture_parent(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_gesture_parent, "b", "(lv_obj)"); }
  int lvbe_obj_get_base_dir(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_base_dir, "i", "(lv_obj)"); }
  int lvbe_obj_get_protect(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_protect, "i", "(lv_obj)"); }
  int lvbe_obj_is_protected(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_is_protected, "b", "(lv_obj)i"); }
  int lvbe_obj_get_state(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_state, "i", "(lv_obj)i"); }
  int lvbe_obj_is_point_on_coords(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_is_point_on_coords, "b", "(lv_obj)(lv_point)"); }
  int lvbe_obj_hittest(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_hittest, "b", "(lv_obj)(lv_point)"); }
  int lvbe_obj_get_ext_attr(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_ext_attr, "i", "(lv_obj)"); }
  int lvbe_obj_get_type(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_type, "", "(lv_obj)(lv_obj_type)"); }
  int lvbe_obj_get_user_data(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_user_data, "i", "(lv_obj)"); }
  int lvbe_obj_set_user_data(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_user_data, "", "(lv_obj)i"); }
  int lvbe_obj_get_group(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_group, "i", "(lv_obj)"); }
  int lvbe_obj_is_focused(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_is_focused, "b", "(lv_obj)"); }
  int lvbe_obj_get_focused_obj(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_focused_obj, "lv_obj", "(lv_obj)"); }
  int lvbe_obj_handle_get_type_signal(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_handle_get_type_signal, "i", "(lv_obj_type)s"); }
  int lvbe_obj_init_draw_rect_dsc(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_init_draw_rect_dsc, "", "(lv_obj)i(lv_draw_rect_dsc)"); }
  int lvbe_obj_init_draw_label_dsc(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_init_draw_label_dsc, "", "(lv_obj)i(lv_draw_label_dsc)"); }
  int lvbe_obj_init_draw_img_dsc(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_init_draw_img_dsc, "", "(lv_obj)i(lv_draw_img_dsc)"); }
  int lvbe_obj_init_draw_line_dsc(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_init_draw_line_dsc, "", "(lv_obj)i(lv_draw_line_dsc)"); }
  int lvbe_obj_get_draw_rect_ext_pad_size(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_draw_rect_ext_pad_size, "i", "(lv_obj)i"); }
  int lvbe_obj_fade_in(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_fade_in, "", "(lv_obj)ii"); }
  int lvbe_obj_fade_out(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_fade_out, "", "(lv_obj)ii"); }
  int lvbe_obj_get_style_radius(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_radius, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_radius(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_radius, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_clip_corner(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_clip_corner, "b", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_clip_corner(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_clip_corner, "", "(lv_obj)iib"); }
  int lvbe_obj_get_style_size(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_size, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_size(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_size, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_transform_width(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_transform_width, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_transform_width(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_transform_width, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_transform_height(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_transform_height, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_transform_height(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_transform_height, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_transform_angle(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_transform_angle, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_transform_angle(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_transform_angle, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_transform_zoom(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_transform_zoom, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_transform_zoom(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_transform_zoom, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_opa_scale(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_opa_scale, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_opa_scale(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_opa_scale, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_pad_top(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_pad_top, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_pad_top(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_pad_top, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_pad_bottom(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_pad_bottom, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_pad_bottom(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_pad_bottom, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_pad_left(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_pad_left, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_pad_left(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_pad_left, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_pad_right(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_pad_right, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_pad_right(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_pad_right, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_pad_inner(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_pad_inner, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_pad_inner(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_pad_inner, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_margin_top(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_margin_top, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_margin_top(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_margin_top, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_margin_bottom(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_margin_bottom, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_margin_bottom(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_margin_bottom, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_margin_left(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_margin_left, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_margin_left(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_margin_left, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_margin_right(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_margin_right, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_margin_right(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_margin_right, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_bg_blend_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_bg_blend_mode, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_bg_blend_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_bg_blend_mode, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_bg_main_stop(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_bg_main_stop, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_bg_main_stop(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_bg_main_stop, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_bg_grad_stop(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_bg_grad_stop, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_bg_grad_stop(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_bg_grad_stop, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_bg_grad_dir(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_bg_grad_dir, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_bg_grad_dir(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_bg_grad_dir, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_bg_color(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_bg_color, "lv_color", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_bg_color(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_bg_color, "", "(lv_obj)ii(lv_color)"); }
  int lvbe_obj_get_style_bg_grad_color(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_bg_grad_color, "lv_color", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_bg_grad_color(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_bg_grad_color, "", "(lv_obj)ii(lv_color)"); }
  int lvbe_obj_get_style_bg_opa(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_bg_opa, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_bg_opa(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_bg_opa, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_border_width(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_border_width, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_border_width(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_border_width, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_border_side(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_border_side, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_border_side(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_border_side, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_border_blend_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_border_blend_mode, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_border_blend_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_border_blend_mode, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_border_post(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_border_post, "b", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_border_post(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_border_post, "", "(lv_obj)iib"); }
  int lvbe_obj_get_style_border_color(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_border_color, "lv_color", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_border_color(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_border_color, "", "(lv_obj)ii(lv_color)"); }
  int lvbe_obj_get_style_border_opa(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_border_opa, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_border_opa(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_border_opa, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_outline_width(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_outline_width, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_outline_width(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_outline_width, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_outline_pad(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_outline_pad, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_outline_pad(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_outline_pad, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_outline_blend_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_outline_blend_mode, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_outline_blend_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_outline_blend_mode, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_outline_color(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_outline_color, "lv_color", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_outline_color(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_outline_color, "", "(lv_obj)ii(lv_color)"); }
  int lvbe_obj_get_style_outline_opa(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_outline_opa, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_outline_opa(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_outline_opa, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_shadow_width(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_shadow_width, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_shadow_width(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_shadow_width, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_shadow_ofs_x(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_shadow_ofs_x, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_shadow_ofs_x(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_shadow_ofs_x, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_shadow_ofs_y(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_shadow_ofs_y, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_shadow_ofs_y(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_shadow_ofs_y, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_shadow_spread(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_shadow_spread, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_shadow_spread(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_shadow_spread, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_shadow_blend_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_shadow_blend_mode, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_shadow_blend_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_shadow_blend_mode, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_shadow_color(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_shadow_color, "lv_color", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_shadow_color(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_shadow_color, "", "(lv_obj)ii(lv_color)"); }
  int lvbe_obj_get_style_shadow_opa(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_shadow_opa, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_shadow_opa(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_shadow_opa, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_pattern_repeat(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_pattern_repeat, "b", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_pattern_repeat(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_pattern_repeat, "", "(lv_obj)iib"); }
  int lvbe_obj_get_style_pattern_blend_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_pattern_blend_mode, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_pattern_blend_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_pattern_blend_mode, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_pattern_recolor(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_pattern_recolor, "lv_color", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_pattern_recolor(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_pattern_recolor, "", "(lv_obj)ii(lv_color)"); }
  int lvbe_obj_get_style_pattern_opa(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_pattern_opa, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_pattern_opa(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_pattern_opa, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_pattern_recolor_opa(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_pattern_recolor_opa, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_pattern_recolor_opa(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_pattern_recolor_opa, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_pattern_image(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_pattern_image, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_pattern_image(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_pattern_image, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_value_letter_space(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_value_letter_space, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_value_letter_space(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_value_letter_space, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_value_line_space(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_value_line_space, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_value_line_space(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_value_line_space, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_value_blend_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_value_blend_mode, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_value_blend_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_value_blend_mode, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_value_ofs_x(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_value_ofs_x, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_value_ofs_x(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_value_ofs_x, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_value_ofs_y(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_value_ofs_y, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_value_ofs_y(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_value_ofs_y, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_value_align(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_value_align, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_value_align(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_value_align, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_value_color(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_value_color, "lv_color", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_value_color(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_value_color, "", "(lv_obj)ii(lv_color)"); }
  int lvbe_obj_get_style_value_opa(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_value_opa, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_value_opa(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_value_opa, "", "(lv_obj)iii"); }
  int lvbe_obj_set_style_local_value_font(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_value_font, "", "(lv_obj)ii(lv_font)"); }
  int lvbe_obj_get_style_value_str(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_value_str, "s", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_value_str(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_value_str, "", "(lv_obj)iis"); }
  int lvbe_obj_get_style_text_letter_space(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_text_letter_space, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_text_letter_space(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_text_letter_space, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_text_line_space(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_text_line_space, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_text_line_space(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_text_line_space, "", "(lv_obj)iii"); }
  int lvbe_obj_set_style_local_text_decor(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_text_decor, "", "(lv_obj)ii(lv_text_decor)"); }
  int lvbe_obj_get_style_text_blend_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_text_blend_mode, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_text_blend_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_text_blend_mode, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_text_color(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_text_color, "lv_color", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_text_color(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_text_color, "", "(lv_obj)ii(lv_color)"); }
  int lvbe_obj_get_style_text_sel_color(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_text_sel_color, "lv_color", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_text_sel_color(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_text_sel_color, "", "(lv_obj)ii(lv_color)"); }
  int lvbe_obj_get_style_text_sel_bg_color(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_text_sel_bg_color, "lv_color", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_text_sel_bg_color(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_text_sel_bg_color, "", "(lv_obj)ii(lv_color)"); }
  int lvbe_obj_get_style_text_opa(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_text_opa, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_text_opa(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_text_opa, "", "(lv_obj)iii"); }
  int lvbe_obj_set_style_local_text_font(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_text_font, "", "(lv_obj)ii(lv_font)"); }
  int lvbe_obj_get_style_line_width(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_line_width, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_line_width(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_line_width, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_line_blend_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_line_blend_mode, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_line_blend_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_line_blend_mode, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_line_dash_width(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_line_dash_width, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_line_dash_width(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_line_dash_width, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_line_dash_gap(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_line_dash_gap, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_line_dash_gap(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_line_dash_gap, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_line_rounded(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_line_rounded, "b", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_line_rounded(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_line_rounded, "", "(lv_obj)iib"); }
  int lvbe_obj_get_style_line_color(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_line_color, "lv_color", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_line_color(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_line_color, "", "(lv_obj)ii(lv_color)"); }
  int lvbe_obj_get_style_line_opa(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_line_opa, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_line_opa(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_line_opa, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_image_blend_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_image_blend_mode, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_image_blend_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_image_blend_mode, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_image_recolor(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_image_recolor, "lv_color", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_image_recolor(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_image_recolor, "", "(lv_obj)ii(lv_color)"); }
  int lvbe_obj_get_style_image_opa(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_image_opa, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_image_opa(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_image_opa, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_image_recolor_opa(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_image_recolor_opa, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_image_recolor_opa(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_image_recolor_opa, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_transition_time(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_transition_time, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_transition_time(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_transition_time, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_transition_delay(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_transition_delay, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_transition_delay(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_transition_delay, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_transition_prop_1(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_transition_prop_1, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_transition_prop_1(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_transition_prop_1, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_transition_prop_2(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_transition_prop_2, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_transition_prop_2(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_transition_prop_2, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_transition_prop_3(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_transition_prop_3, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_transition_prop_3(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_transition_prop_3, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_transition_prop_4(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_transition_prop_4, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_transition_prop_4(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_transition_prop_4, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_transition_prop_5(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_transition_prop_5, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_transition_prop_5(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_transition_prop_5, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_transition_prop_6(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_transition_prop_6, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_transition_prop_6(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_transition_prop_6, "", "(lv_obj)iii"); }
  int lvbe_obj_set_style_local_transition_path(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_transition_path, "", "(lv_obj)ii(lv_anim_path)"); }
  int lvbe_obj_get_style_scale_width(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_scale_width, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_scale_width(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_scale_width, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_scale_border_width(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_scale_border_width, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_scale_border_width(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_scale_border_width, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_scale_end_border_width(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_scale_end_border_width, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_scale_end_border_width(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_scale_end_border_width, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_scale_end_line_width(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_scale_end_line_width, "i", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_scale_end_line_width(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_scale_end_line_width, "", "(lv_obj)iii"); }
  int lvbe_obj_get_style_scale_grad_color(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_scale_grad_color, "lv_color", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_scale_grad_color(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_scale_grad_color, "", "(lv_obj)ii(lv_color)"); }
  int lvbe_obj_get_style_scale_end_color(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_get_style_scale_end_color, "lv_color", "(lv_obj)i"); }
  int lvbe_obj_set_style_local_scale_end_color(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_set_style_local_scale_end_color, "", "(lv_obj)ii(lv_color)"); }

  /* `lv_arc` methods */
  int lvbe_arc_create(bvm *vm)       { return lvx_init_2(vm, (void*) &lv_arc_create, "lv_arc", "(lv_obj)(lv_obj)"); }
  int lvbe_arc_set_start_angle(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_arc_set_start_angle, "", "(lv_obj)i"); }
  int lvbe_arc_set_end_angle(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_arc_set_end_angle, "", "(lv_obj)i"); }
  int lvbe_arc_set_angles(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_arc_set_angles, "", "(lv_obj)ii"); }
  int lvbe_arc_set_bg_start_angle(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_arc_set_bg_start_angle, "", "(lv_obj)i"); }
  int lvbe_arc_set_bg_end_angle(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_arc_set_bg_end_angle, "", "(lv_obj)i"); }
  int lvbe_arc_set_bg_angles(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_arc_set_bg_angles, "", "(lv_obj)ii"); }
  int lvbe_arc_set_rotation(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_arc_set_rotation, "", "(lv_obj)i"); }
  int lvbe_arc_set_type(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_arc_set_type, "", "(lv_obj)i"); }
  int lvbe_arc_set_value(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_arc_set_value, "", "(lv_obj)i"); }
  int lvbe_arc_set_range(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_arc_set_range, "", "(lv_obj)ii"); }
  int lvbe_arc_set_chg_rate(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_arc_set_chg_rate, "", "(lv_obj)i"); }
  int lvbe_arc_set_adjustable(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_arc_set_adjustable, "", "(lv_obj)b"); }
  int lvbe_arc_get_angle_start(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_arc_get_angle_start, "i", "(lv_obj)"); }
  int lvbe_arc_get_angle_end(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_arc_get_angle_end, "i", "(lv_obj)"); }
  int lvbe_arc_get_bg_angle_start(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_arc_get_bg_angle_start, "i", "(lv_obj)"); }
  int lvbe_arc_get_bg_angle_end(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_arc_get_bg_angle_end, "i", "(lv_obj)"); }
  int lvbe_arc_get_type(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_arc_get_type, "i", "(lv_obj)"); }
  int lvbe_arc_get_value(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_arc_get_value, "i", "(lv_obj)"); }
  int lvbe_arc_get_min_value(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_arc_get_min_value, "i", "(lv_obj)"); }
  int lvbe_arc_get_max_value(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_arc_get_max_value, "i", "(lv_obj)"); }
  int lvbe_arc_is_dragged(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_arc_is_dragged, "b", "(lv_obj)"); }
  int lvbe_arc_get_adjustable(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_arc_get_adjustable, "b", "(lv_obj)"); }

  /* `lv_bar` methods */
  int lvbe_bar_create(bvm *vm)       { return lvx_init_2(vm, (void*) &lv_bar_create, "lv_bar", "(lv_obj)(lv_obj)"); }
  int lvbe_bar_set_value(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_bar_set_value, "", "(lv_obj)i(lv_anim_enable)"); }
  int lvbe_bar_set_start_value(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_bar_set_start_value, "", "(lv_obj)i(lv_anim_enable)"); }
  int lvbe_bar_set_range(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_bar_set_range, "", "(lv_obj)ii"); }
  int lvbe_bar_set_type(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_bar_set_type, "", "(lv_obj)i"); }
  int lvbe_bar_set_anim_time(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_bar_set_anim_time, "", "(lv_obj)i"); }
  int lvbe_bar_get_value(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_bar_get_value, "i", "(lv_obj)"); }
  int lvbe_bar_get_start_value(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_bar_get_start_value, "i", "(lv_obj)"); }
  int lvbe_bar_get_min_value(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_bar_get_min_value, "i", "(lv_obj)"); }
  int lvbe_bar_get_max_value(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_bar_get_max_value, "i", "(lv_obj)"); }
  int lvbe_bar_get_type(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_bar_get_type, "i", "(lv_obj)"); }
  int lvbe_bar_get_anim_time(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_bar_get_anim_time, "i", "(lv_obj)"); }

  /* `lv_btn` methods */
  int lvbe_btn_create(bvm *vm)       { return lvx_init_2(vm, (void*) &lv_btn_create, "lv_btn", "(lv_obj)(lv_obj)"); }
  int lvbe_btn_set_checkable(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_btn_set_checkable, "", "(lv_obj)b"); }
  int lvbe_btn_set_state(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_btn_set_state, "", "(lv_obj)i"); }
  int lvbe_btn_toggle(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_btn_toggle, "", "(lv_obj)"); }
  int lvbe_btn_set_layout(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_btn_set_layout, "", "(lv_obj)i"); }
  int lvbe_btn_set_fit4(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_btn_set_fit4, "", "(lv_obj)iiii"); }
  int lvbe_btn_set_fit2(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_btn_set_fit2, "", "(lv_obj)ii"); }
  int lvbe_btn_set_fit(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_btn_set_fit, "", "(lv_obj)i"); }
  int lvbe_btn_get_state(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_btn_get_state, "i", "(lv_obj)"); }
  int lvbe_btn_get_checkable(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_btn_get_checkable, "b", "(lv_obj)"); }
  int lvbe_btn_get_layout(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_btn_get_layout, "i", "(lv_obj)"); }
  int lvbe_btn_get_fit_left(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_btn_get_fit_left, "i", "(lv_obj)"); }
  int lvbe_btn_get_fit_right(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_btn_get_fit_right, "i", "(lv_obj)"); }
  int lvbe_btn_get_fit_top(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_btn_get_fit_top, "i", "(lv_obj)"); }
  int lvbe_btn_get_fit_bottom(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_btn_get_fit_bottom, "i", "(lv_obj)"); }

  /* `lv_btnmatrix` methods */
  int lvbe_btnmatrix_create(bvm *vm)       { return lvx_init_2(vm, (void*) &lv_btnmatrix_create, "lv_btnmatrix", "(lv_obj)(lv_obj)"); }
  int lvbe_btnmatrix_set_map(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_btnmatrix_set_map, "", "(lv_obj)s"); }
  int lvbe_btnmatrix_set_ctrl_map(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_btnmatrix_set_ctrl_map, "", "(lv_obj)(lv_btnmatrix_ctrl)"); }
  int lvbe_btnmatrix_set_focused_btn(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_btnmatrix_set_focused_btn, "", "(lv_obj)i"); }
  int lvbe_btnmatrix_set_recolor(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_btnmatrix_set_recolor, "", "(lv_obj)b"); }
  int lvbe_btnmatrix_set_btn_ctrl(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_btnmatrix_set_btn_ctrl, "", "(lv_obj)i(lv_btnmatrix_ctrl)"); }
  int lvbe_btnmatrix_clear_btn_ctrl(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_btnmatrix_clear_btn_ctrl, "", "(lv_obj)i(lv_btnmatrix_ctrl)"); }
  int lvbe_btnmatrix_set_btn_ctrl_all(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_btnmatrix_set_btn_ctrl_all, "", "(lv_obj)(lv_btnmatrix_ctrl)"); }
  int lvbe_btnmatrix_clear_btn_ctrl_all(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_btnmatrix_clear_btn_ctrl_all, "", "(lv_obj)(lv_btnmatrix_ctrl)"); }
  int lvbe_btnmatrix_set_btn_width(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_btnmatrix_set_btn_width, "", "(lv_obj)ii"); }
  int lvbe_btnmatrix_set_one_check(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_btnmatrix_set_one_check, "", "(lv_obj)b"); }
  int lvbe_btnmatrix_set_align(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_btnmatrix_set_align, "", "(lv_obj)i"); }
  int lvbe_btnmatrix_get_recolor(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_btnmatrix_get_recolor, "b", "(lv_obj)"); }
  int lvbe_btnmatrix_get_active_btn(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_btnmatrix_get_active_btn, "i", "(lv_obj)"); }
  int lvbe_btnmatrix_get_active_btn_text(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_btnmatrix_get_active_btn_text, "s", "(lv_obj)"); }
  int lvbe_btnmatrix_get_focused_btn(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_btnmatrix_get_focused_btn, "i", "(lv_obj)"); }
  int lvbe_btnmatrix_get_btn_text(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_btnmatrix_get_btn_text, "s", "(lv_obj)i"); }
  int lvbe_btnmatrix_get_btn_ctrl(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_btnmatrix_get_btn_ctrl, "b", "(lv_obj)i(lv_btnmatrix_ctrl)"); }
  int lvbe_btnmatrix_get_one_check(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_btnmatrix_get_one_check, "b", "(lv_obj)"); }
  int lvbe_btnmatrix_get_align(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_btnmatrix_get_align, "i", "(lv_obj)"); }

  /* `lv_calendar` methods */
  int lvbe_calendar_create(bvm *vm)       { return lvx_init_2(vm, (void*) &lv_calendar_create, "lv_calendar", "(lv_obj)(lv_obj)"); }
  int lvbe_calendar_set_today_date(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_calendar_set_today_date, "", "(lv_obj)(lv_calendar_date)"); }
  int lvbe_calendar_set_showed_date(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_calendar_set_showed_date, "", "(lv_obj)(lv_calendar_date)"); }
  int lvbe_calendar_set_highlighted_dates(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_calendar_set_highlighted_dates, "", "(lv_obj)(lv_calendar_date)i"); }
  int lvbe_calendar_set_day_names(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_calendar_set_day_names, "", "(lv_obj)(char)"); }
  int lvbe_calendar_set_month_names(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_calendar_set_month_names, "", "(lv_obj)(char)"); }
  int lvbe_calendar_get_highlighted_dates_num(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_calendar_get_highlighted_dates_num, "i", "(lv_obj)"); }
  int lvbe_calendar_get_day_of_week(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_calendar_get_day_of_week, "i", "iii"); }

  /* `lv_canvas` methods */
  int lvbe_canvas_create(bvm *vm)       { return lvx_init_2(vm, (void*) &lv_canvas_create, "lv_canvas", "(lv_obj)(lv_obj)"); }
  int lvbe_canvas_set_buffer(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_canvas_set_buffer, "", "(lv_obj)iii(lv_img_cf)"); }
  int lvbe_canvas_set_px(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_canvas_set_px, "", "(lv_obj)ii(lv_color)"); }
  int lvbe_canvas_set_palette(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_canvas_set_palette, "", "(lv_obj)i(lv_color)"); }
  int lvbe_canvas_get_px(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_canvas_get_px, "lv_color", "(lv_obj)ii"); }
  int lvbe_canvas_copy_buf(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_canvas_copy_buf, "", "(lv_obj)iiiii"); }
  int lvbe_canvas_transform(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_canvas_transform, "", "(lv_obj)(lv_img_dsc)iiiiiib"); }
  int lvbe_canvas_blur_hor(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_canvas_blur_hor, "", "(lv_obj)(lv_area)i"); }
  int lvbe_canvas_blur_ver(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_canvas_blur_ver, "", "(lv_obj)(lv_area)i"); }
  int lvbe_canvas_fill_bg(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_canvas_fill_bg, "", "(lv_obj)(lv_color)i"); }
  int lvbe_canvas_draw_rect(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_canvas_draw_rect, "", "(lv_obj)iiii(lv_draw_rect_dsc)"); }
  int lvbe_canvas_draw_text(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_canvas_draw_text, "", "(lv_obj)iii(lv_draw_label_dsc)si"); }
  int lvbe_canvas_draw_img(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_canvas_draw_img, "", "(lv_obj)iii(lv_draw_img_dsc)"); }
  int lvbe_canvas_draw_line(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_canvas_draw_line, "", "(lv_obj)ii(lv_draw_line_dsc)"); }
  int lvbe_canvas_draw_polygon(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_canvas_draw_polygon, "", "(lv_obj)ii(lv_draw_rect_dsc)"); }
  int lvbe_canvas_draw_arc(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_canvas_draw_arc, "", "(lv_obj)iiiii(lv_draw_line_dsc)"); }

  /* `lv_chart` methods */
  int lvbe_chart_create(bvm *vm)       { return lvx_init_2(vm, (void*) &lv_chart_create, "lv_chart", "(lv_obj)(lv_obj)"); }
  int lvbe_chart_remove_series(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_chart_remove_series, "", "(lv_obj)(lv_chart_series)"); }
  int lvbe_chart_clear_series(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_chart_clear_series, "", "(lv_obj)(lv_chart_series)"); }
  int lvbe_chart_hide_series(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_chart_hide_series, "", "(lv_obj)(lv_chart_series)b"); }
  int lvbe_chart_set_div_line_count(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_chart_set_div_line_count, "", "(lv_obj)ii"); }
  int lvbe_chart_set_y_range(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_chart_set_y_range, "", "(lv_obj)iii"); }
  int lvbe_chart_set_type(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_chart_set_type, "", "(lv_obj)i"); }
  int lvbe_chart_set_point_count(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_chart_set_point_count, "", "(lv_obj)i"); }
  int lvbe_chart_init_points(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_chart_init_points, "", "(lv_obj)(lv_chart_series)i"); }
  int lvbe_chart_set_points(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_chart_set_points, "", "(lv_obj)(lv_chart_series)i"); }
  int lvbe_chart_set_next(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_chart_set_next, "", "(lv_obj)(lv_chart_series)i"); }
  int lvbe_chart_set_update_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_chart_set_update_mode, "", "(lv_obj)(lv_chart_update_mode)"); }
  int lvbe_chart_set_x_tick_length(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_chart_set_x_tick_length, "", "(lv_obj)ii"); }
  int lvbe_chart_set_y_tick_length(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_chart_set_y_tick_length, "", "(lv_obj)ii"); }
  int lvbe_chart_set_secondary_y_tick_length(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_chart_set_secondary_y_tick_length, "", "(lv_obj)ii"); }
  int lvbe_chart_set_x_tick_texts(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_chart_set_x_tick_texts, "", "(lv_obj)si(lv_chart_axis_options)"); }
  int lvbe_chart_set_secondary_y_tick_texts(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_chart_set_secondary_y_tick_texts, "", "(lv_obj)si(lv_chart_axis_options)"); }
  int lvbe_chart_set_y_tick_texts(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_chart_set_y_tick_texts, "", "(lv_obj)si(lv_chart_axis_options)"); }
  int lvbe_chart_set_x_start_point(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_chart_set_x_start_point, "", "(lv_obj)(lv_chart_series)i"); }
  int lvbe_chart_set_ext_array(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_chart_set_ext_array, "", "(lv_obj)(lv_chart_series)ii"); }
  int lvbe_chart_set_point_id(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_chart_set_point_id, "", "(lv_obj)(lv_chart_series)ii"); }
  int lvbe_chart_set_series_axis(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_chart_set_series_axis, "", "(lv_obj)(lv_chart_series)i"); }
  int lvbe_chart_set_cursor_point(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_chart_set_cursor_point, "", "(lv_obj)(lv_chart_cursor)(lv_point)"); }
  int lvbe_chart_get_type(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_chart_get_type, "i", "(lv_obj)"); }
  int lvbe_chart_get_point_count(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_chart_get_point_count, "i", "(lv_obj)"); }
  int lvbe_chart_get_x_start_point(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_chart_get_x_start_point, "i", "(lv_chart_series)"); }
  int lvbe_chart_get_point_id(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_chart_get_point_id, "i", "(lv_obj)(lv_chart_series)i"); }
  int lvbe_chart_get_series_axis(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_chart_get_series_axis, "i", "(lv_obj)(lv_chart_series)"); }
  int lvbe_chart_get_series_area(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_chart_get_series_area, "", "(lv_obj)(lv_area)"); }
  int lvbe_chart_get_cursor_point(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_chart_get_cursor_point, "i", "(lv_obj)(lv_chart_cursor)"); }
  int lvbe_chart_get_nearest_index_from_coord(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_chart_get_nearest_index_from_coord, "i", "(lv_obj)i"); }
  int lvbe_chart_get_x_from_index(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_chart_get_x_from_index, "i", "(lv_obj)(lv_chart_series)i"); }
  int lvbe_chart_get_y_from_index(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_chart_get_y_from_index, "i", "(lv_obj)(lv_chart_series)i"); }
  int lvbe_chart_refresh(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_chart_refresh, "", "(lv_obj)"); }

  /* `lv_checkbox` methods */
  int lvbe_checkbox_create(bvm *vm)       { return lvx_init_2(vm, (void*) &lv_checkbox_create, "lv_checkbox", "(lv_obj)(lv_obj)"); }
  int lvbe_checkbox_set_text(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_checkbox_set_text, "", "(lv_obj)s"); }
  int lvbe_checkbox_set_text_static(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_checkbox_set_text_static, "", "(lv_obj)s"); }
  int lvbe_checkbox_set_checked(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_checkbox_set_checked, "", "(lv_obj)b"); }
  int lvbe_checkbox_set_disabled(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_checkbox_set_disabled, "", "(lv_obj)"); }
  int lvbe_checkbox_set_state(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_checkbox_set_state, "", "(lv_obj)i"); }
  int lvbe_checkbox_get_text(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_checkbox_get_text, "s", "(lv_obj)"); }
  int lvbe_checkbox_is_checked(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_checkbox_is_checked, "b", "(lv_obj)"); }
  int lvbe_checkbox_is_inactive(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_checkbox_is_inactive, "b", "(lv_obj)"); }
  int lvbe_checkbox_get_state(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_checkbox_get_state, "i", "(lv_obj)"); }

  /* `lv_cont` methods */
  int lvbe_cont_create(bvm *vm)       { return lvx_init_2(vm, (void*) &lv_cont_create, "lv_cont", "(lv_obj)(lv_obj)"); }
  int lvbe_cont_set_layout(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_cont_set_layout, "", "(lv_obj)i"); }
  int lvbe_cont_set_fit4(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_cont_set_fit4, "", "(lv_obj)iiii"); }
  int lvbe_cont_set_fit2(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_cont_set_fit2, "", "(lv_obj)ii"); }
  int lvbe_cont_set_fit(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_cont_set_fit, "", "(lv_obj)i"); }
  int lvbe_cont_get_layout(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_cont_get_layout, "i", "(lv_obj)"); }
  int lvbe_cont_get_fit_left(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_cont_get_fit_left, "i", "(lv_obj)"); }
  int lvbe_cont_get_fit_right(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_cont_get_fit_right, "i", "(lv_obj)"); }
  int lvbe_cont_get_fit_top(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_cont_get_fit_top, "i", "(lv_obj)"); }
  int lvbe_cont_get_fit_bottom(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_cont_get_fit_bottom, "i", "(lv_obj)"); }

  /* `lv_cpicker` methods */
  int lvbe_cpicker_create(bvm *vm)       { return lvx_init_2(vm, (void*) &lv_cpicker_create, "lv_cpicker", "(lv_obj)(lv_obj)"); }
  int lvbe_cpicker_set_type(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_cpicker_set_type, "", "(lv_obj)(lv_cpicker_type)"); }
  int lvbe_cpicker_set_hue(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_cpicker_set_hue, "b", "(lv_obj)i"); }
  int lvbe_cpicker_set_saturation(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_cpicker_set_saturation, "b", "(lv_obj)i"); }
  int lvbe_cpicker_set_value(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_cpicker_set_value, "b", "(lv_obj)i"); }
  int lvbe_cpicker_set_hsv(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_cpicker_set_hsv, "b", "(lv_obj)i"); }
  int lvbe_cpicker_set_color(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_cpicker_set_color, "b", "(lv_obj)(lv_color)"); }
  int lvbe_cpicker_set_color_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_cpicker_set_color_mode, "", "(lv_obj)i"); }
  int lvbe_cpicker_set_color_mode_fixed(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_cpicker_set_color_mode_fixed, "", "(lv_obj)b"); }
  int lvbe_cpicker_set_knob_colored(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_cpicker_set_knob_colored, "", "(lv_obj)b"); }
  int lvbe_cpicker_get_color_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_cpicker_get_color_mode, "i", "(lv_obj)"); }
  int lvbe_cpicker_get_color_mode_fixed(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_cpicker_get_color_mode_fixed, "b", "(lv_obj)"); }
  int lvbe_cpicker_get_hue(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_cpicker_get_hue, "i", "(lv_obj)"); }
  int lvbe_cpicker_get_saturation(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_cpicker_get_saturation, "i", "(lv_obj)"); }
  int lvbe_cpicker_get_value(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_cpicker_get_value, "i", "(lv_obj)"); }
  int lvbe_cpicker_get_hsv(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_cpicker_get_hsv, "i", "(lv_obj)"); }
  int lvbe_cpicker_get_color(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_cpicker_get_color, "lv_color", "(lv_obj)"); }
  int lvbe_cpicker_get_knob_colored(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_cpicker_get_knob_colored, "b", "(lv_obj)"); }

  /* `lv_dropdown` methods */
  int lvbe_dropdown_create(bvm *vm)       { return lvx_init_2(vm, (void*) &lv_dropdown_create, "lv_dropdown", "(lv_obj)(lv_obj)"); }
  int lvbe_dropdown_set_text(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_dropdown_set_text, "", "(lv_obj)s"); }
  int lvbe_dropdown_clear_options(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_dropdown_clear_options, "", "(lv_obj)"); }
  int lvbe_dropdown_set_options(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_dropdown_set_options, "", "(lv_obj)s"); }
  int lvbe_dropdown_set_options_static(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_dropdown_set_options_static, "", "(lv_obj)s"); }
  int lvbe_dropdown_add_option(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_dropdown_add_option, "", "(lv_obj)si"); }
  int lvbe_dropdown_set_selected(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_dropdown_set_selected, "", "(lv_obj)i"); }
  int lvbe_dropdown_set_dir(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_dropdown_set_dir, "", "(lv_obj)i"); }
  int lvbe_dropdown_set_max_height(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_dropdown_set_max_height, "", "(lv_obj)i"); }
  int lvbe_dropdown_set_symbol(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_dropdown_set_symbol, "", "(lv_obj)s"); }
  int lvbe_dropdown_set_show_selected(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_dropdown_set_show_selected, "", "(lv_obj)b"); }
  int lvbe_dropdown_get_text(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_dropdown_get_text, "s", "(lv_obj)"); }
  int lvbe_dropdown_get_options(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_dropdown_get_options, "s", "(lv_obj)"); }
  int lvbe_dropdown_get_selected(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_dropdown_get_selected, "i", "(lv_obj)"); }
  int lvbe_dropdown_get_option_cnt(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_dropdown_get_option_cnt, "i", "(lv_obj)"); }
  int lvbe_dropdown_get_selected_str(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_dropdown_get_selected_str, "", "(lv_obj)si"); }
  int lvbe_dropdown_get_max_height(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_dropdown_get_max_height, "i", "(lv_obj)"); }
  int lvbe_dropdown_get_symbol(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_dropdown_get_symbol, "s", "(lv_obj)"); }
  int lvbe_dropdown_get_dir(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_dropdown_get_dir, "i", "(lv_obj)"); }
  int lvbe_dropdown_get_show_selected(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_dropdown_get_show_selected, "b", "(lv_obj)"); }
  int lvbe_dropdown_open(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_dropdown_open, "", "(lv_obj)"); }
  int lvbe_dropdown_close(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_dropdown_close, "", "(lv_obj)"); }

  /* `lv_gauge` methods */
  int lvbe_gauge_create(bvm *vm)       { return lvx_init_2(vm, (void*) &lv_gauge_create, "lv_gauge", "(lv_obj)(lv_obj)"); }
  int lvbe_gauge_set_needle_count(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_gauge_set_needle_count, "", "(lv_obj)i(lv_color)"); }
  int lvbe_gauge_set_value(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_gauge_set_value, "", "(lv_obj)ii"); }
  int lvbe_gauge_set_range(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_gauge_set_range, "", "(lv_obj)ii"); }
  int lvbe_gauge_set_critical_value(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_gauge_set_critical_value, "", "(lv_obj)i"); }
  int lvbe_gauge_set_scale(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_gauge_set_scale, "", "(lv_obj)iii"); }
  int lvbe_gauge_set_angle_offset(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_gauge_set_angle_offset, "", "(lv_obj)i"); }
  int lvbe_gauge_set_needle_img(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_gauge_set_needle_img, "", "(lv_obj)iii"); }
  int lvbe_gauge_set_formatter_cb(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_gauge_set_formatter_cb, "", "(lv_obj)&4"); }
  int lvbe_gauge_get_value(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_gauge_get_value, "i", "(lv_obj)i"); }
  int lvbe_gauge_get_needle_count(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_gauge_get_needle_count, "i", "(lv_obj)"); }
  int lvbe_gauge_get_min_value(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_gauge_get_min_value, "i", "(lv_obj)"); }
  int lvbe_gauge_get_max_value(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_gauge_get_max_value, "i", "(lv_obj)"); }
  int lvbe_gauge_get_critical_value(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_gauge_get_critical_value, "i", "(lv_obj)"); }
  int lvbe_gauge_get_label_count(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_gauge_get_label_count, "i", "(lv_obj)"); }
  int lvbe_gauge_get_line_count(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_gauge_get_line_count, "i", "(lv_obj)"); }
  int lvbe_gauge_get_scale_angle(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_gauge_get_scale_angle, "i", "(lv_obj)"); }
  int lvbe_gauge_get_angle_offset(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_gauge_get_angle_offset, "i", "(lv_obj)"); }
  int lvbe_gauge_get_needle_img(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_gauge_get_needle_img, "i", "(lv_obj)"); }
  int lvbe_gauge_get_needle_img_pivot_x(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_gauge_get_needle_img_pivot_x, "i", "(lv_obj)"); }
  int lvbe_gauge_get_needle_img_pivot_y(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_gauge_get_needle_img_pivot_y, "i", "(lv_obj)"); }

  /* `lv_imgbtn` methods */
  int lvbe_imgbtn_create(bvm *vm)       { return lvx_init_2(vm, (void*) &lv_imgbtn_create, "lv_imgbtn", "(lv_obj)(lv_obj)"); }
  int lvbe_imgbtn_set_src(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_imgbtn_set_src, "", "(lv_obj)ii"); }
  int lvbe_imgbtn_set_state(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_imgbtn_set_state, "", "(lv_obj)i"); }
  int lvbe_imgbtn_toggle(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_imgbtn_toggle, "", "(lv_obj)"); }
  int lvbe_imgbtn_set_checkable(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_imgbtn_set_checkable, "", "(lv_obj)b"); }
  int lvbe_imgbtn_get_src(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_imgbtn_get_src, "i", "(lv_obj)i"); }
  int lvbe_imgbtn_get_state(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_imgbtn_get_state, "i", "(lv_obj)"); }

  /* `lv_keyboard` methods */
  int lvbe_keyboard_create(bvm *vm)       { return lvx_init_2(vm, (void*) &lv_keyboard_create, "lv_keyboard", "(lv_obj)(lv_obj)"); }
  int lvbe_keyboard_set_textarea(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_keyboard_set_textarea, "", "(lv_obj)(lv_obj)"); }
  int lvbe_keyboard_set_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_keyboard_set_mode, "", "(lv_obj)i"); }
  int lvbe_keyboard_set_cursor_manage(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_keyboard_set_cursor_manage, "", "(lv_obj)b"); }
  int lvbe_keyboard_set_map(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_keyboard_set_map, "", "(lv_obj)is"); }
  int lvbe_keyboard_set_ctrl_map(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_keyboard_set_ctrl_map, "", "(lv_obj)i(lv_btnmatrix_ctrl)"); }
  int lvbe_keyboard_get_textarea(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_keyboard_get_textarea, "lv_obj", "(lv_obj)"); }
  int lvbe_keyboard_get_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_keyboard_get_mode, "i", "(lv_obj)"); }
  int lvbe_keyboard_get_cursor_manage(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_keyboard_get_cursor_manage, "b", "(lv_obj)"); }
  int lvbe_keyboard_def_event_cb(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_keyboard_def_event_cb, "", "(lv_obj)(lv_event)"); }

  /* `lv_label` methods */
  int lvbe_label_create(bvm *vm)       { return lvx_init_2(vm, (void*) &lv_label_create, "lv_label", "(lv_obj)(lv_obj)"); }
  int lvbe_label_set_text(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_label_set_text, "", "(lv_obj)s"); }
  int lvbe_label_set_text_fmt(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_label_set_text_fmt, "", "(lv_obj)s"); }
  int lvbe_label_set_text_static(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_label_set_text_static, "", "(lv_obj)s"); }
  int lvbe_label_set_long_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_label_set_long_mode, "", "(lv_obj)i"); }
  int lvbe_label_set_align(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_label_set_align, "", "(lv_obj)i"); }
  int lvbe_label_set_recolor(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_label_set_recolor, "", "(lv_obj)b"); }
  int lvbe_label_set_anim_speed(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_label_set_anim_speed, "", "(lv_obj)i"); }
  int lvbe_label_set_text_sel_start(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_label_set_text_sel_start, "", "(lv_obj)i"); }
  int lvbe_label_set_text_sel_end(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_label_set_text_sel_end, "", "(lv_obj)i"); }
  int lvbe_label_get_text(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_label_get_text, "s", "(lv_obj)"); }
  int lvbe_label_get_long_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_label_get_long_mode, "i", "(lv_obj)"); }
  int lvbe_label_get_align(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_label_get_align, "i", "(lv_obj)"); }
  int lvbe_label_get_recolor(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_label_get_recolor, "b", "(lv_obj)"); }
  int lvbe_label_get_anim_speed(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_label_get_anim_speed, "i", "(lv_obj)"); }
  int lvbe_label_get_letter_pos(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_label_get_letter_pos, "", "(lv_obj)i(lv_point)"); }
  int lvbe_label_get_letter_on(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_label_get_letter_on, "i", "(lv_obj)(lv_point)"); }
  int lvbe_label_is_char_under_pos(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_label_is_char_under_pos, "b", "(lv_obj)(lv_point)"); }
  int lvbe_label_get_text_sel_start(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_label_get_text_sel_start, "i", "(lv_obj)"); }
  int lvbe_label_get_text_sel_end(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_label_get_text_sel_end, "i", "(lv_obj)"); }
  int lvbe_label_ins_text(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_label_ins_text, "", "(lv_obj)is"); }
  int lvbe_label_cut_text(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_label_cut_text, "", "(lv_obj)ii"); }
  int lvbe_label_refr_text(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_label_refr_text, "", "(lv_obj)"); }

  /* `lv_led` methods */
  int lvbe_led_create(bvm *vm)       { return lvx_init_2(vm, (void*) &lv_led_create, "lv_led", "(lv_obj)(lv_obj)"); }
  int lvbe_led_set_bright(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_led_set_bright, "", "(lv_obj)i"); }
  int lvbe_led_on(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_led_on, "", "(lv_obj)"); }
  int lvbe_led_off(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_led_off, "", "(lv_obj)"); }
  int lvbe_led_toggle(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_led_toggle, "", "(lv_obj)"); }
  int lvbe_led_get_bright(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_led_get_bright, "i", "(lv_obj)"); }

  /* `lv_line` methods */
  int lvbe_line_create(bvm *vm)       { return lvx_init_2(vm, (void*) &lv_line_create, "lv_line", "(lv_obj)(lv_obj)"); }
  int lvbe_line_set_points(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_line_set_points, "", "(lv_obj)ii"); }
  int lvbe_line_set_auto_size(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_line_set_auto_size, "", "(lv_obj)b"); }
  int lvbe_line_set_y_invert(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_line_set_y_invert, "", "(lv_obj)b"); }
  int lvbe_line_get_auto_size(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_line_get_auto_size, "b", "(lv_obj)"); }
  int lvbe_line_get_y_invert(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_line_get_y_invert, "b", "(lv_obj)"); }

  /* `lv_linemeter` methods */
  int lvbe_linemeter_create(bvm *vm)       { return lvx_init_2(vm, (void*) &lv_linemeter_create, "lv_linemeter", "(lv_obj)(lv_obj)"); }
  int lvbe_linemeter_set_value(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_linemeter_set_value, "", "(lv_obj)i"); }
  int lvbe_linemeter_set_range(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_linemeter_set_range, "", "(lv_obj)ii"); }
  int lvbe_linemeter_set_scale(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_linemeter_set_scale, "", "(lv_obj)ii"); }
  int lvbe_linemeter_set_angle_offset(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_linemeter_set_angle_offset, "", "(lv_obj)i"); }
  int lvbe_linemeter_set_mirror(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_linemeter_set_mirror, "", "(lv_obj)b"); }
  int lvbe_linemeter_get_value(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_linemeter_get_value, "i", "(lv_obj)"); }
  int lvbe_linemeter_get_min_value(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_linemeter_get_min_value, "i", "(lv_obj)"); }
  int lvbe_linemeter_get_max_value(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_linemeter_get_max_value, "i", "(lv_obj)"); }
  int lvbe_linemeter_get_line_count(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_linemeter_get_line_count, "i", "(lv_obj)"); }
  int lvbe_linemeter_get_scale_angle(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_linemeter_get_scale_angle, "i", "(lv_obj)"); }
  int lvbe_linemeter_get_angle_offset(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_linemeter_get_angle_offset, "i", "(lv_obj)"); }
  int lvbe_linemeter_draw_scale(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_linemeter_draw_scale, "", "(lv_obj)(lv_area)i"); }
  int lvbe_linemeter_get_mirror(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_linemeter_get_mirror, "b", "(lv_obj)"); }

  /* `lv_list` methods */
  int lvbe_list_create(bvm *vm)       { return lvx_init_2(vm, (void*) &lv_list_create, "lv_list", "(lv_obj)(lv_obj)"); }
  int lvbe_list_clean(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_list_clean, "", "(lv_obj)"); }
  int lvbe_list_add_btn(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_list_add_btn, "lv_obj", "(lv_obj)is"); }
  int lvbe_list_remove(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_list_remove, "b", "(lv_obj)i"); }
  int lvbe_list_focus_btn(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_list_focus_btn, "", "(lv_obj)(lv_obj)"); }
  int lvbe_list_set_scrollbar_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_list_set_scrollbar_mode, "", "(lv_obj)i"); }
  int lvbe_list_set_scroll_propagation(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_list_set_scroll_propagation, "", "(lv_obj)b"); }
  int lvbe_list_set_edge_flash(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_list_set_edge_flash, "", "(lv_obj)b"); }
  int lvbe_list_set_anim_time(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_list_set_anim_time, "", "(lv_obj)i"); }
  int lvbe_list_set_layout(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_list_set_layout, "", "(lv_obj)i"); }
  int lvbe_list_get_btn_text(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_list_get_btn_text, "s", "(lv_obj)"); }
  int lvbe_list_get_btn_label(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_list_get_btn_label, "lv_obj", "(lv_obj)"); }
  int lvbe_list_get_btn_img(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_list_get_btn_img, "lv_obj", "(lv_obj)"); }
  int lvbe_list_get_prev_btn(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_list_get_prev_btn, "lv_obj", "(lv_obj)(lv_obj)"); }
  int lvbe_list_get_next_btn(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_list_get_next_btn, "lv_obj", "(lv_obj)(lv_obj)"); }
  int lvbe_list_get_btn_index(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_list_get_btn_index, "i", "(lv_obj)(lv_obj)"); }
  int lvbe_list_get_size(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_list_get_size, "i", "(lv_obj)"); }
  int lvbe_list_get_btn_selected(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_list_get_btn_selected, "lv_obj", "(lv_obj)"); }
  int lvbe_list_get_layout(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_list_get_layout, "i", "(lv_obj)"); }
  int lvbe_list_get_scrollbar_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_list_get_scrollbar_mode, "i", "(lv_obj)"); }
  int lvbe_list_get_scroll_propagation(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_list_get_scroll_propagation, "b", "(lv_obj)"); }
  int lvbe_list_get_edge_flash(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_list_get_edge_flash, "b", "(lv_obj)"); }
  int lvbe_list_get_anim_time(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_list_get_anim_time, "i", "(lv_obj)"); }
  int lvbe_list_up(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_list_up, "", "(lv_obj)"); }
  int lvbe_list_down(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_list_down, "", "(lv_obj)"); }
  int lvbe_list_focus(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_list_focus, "", "(lv_obj)(lv_anim_enable)"); }

  /* `lv_msgbox` methods */
  int lvbe_msgbox_create(bvm *vm)       { return lvx_init_2(vm, (void*) &lv_msgbox_create, "lv_msgbox", "(lv_obj)(lv_obj)"); }
  int lvbe_msgbox_add_btns(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_msgbox_add_btns, "", "(lv_obj)s"); }
  int lvbe_msgbox_set_text(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_msgbox_set_text, "", "(lv_obj)s"); }
  int lvbe_msgbox_set_text_fmt(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_msgbox_set_text_fmt, "", "(lv_obj)s"); }
  int lvbe_msgbox_set_anim_time(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_msgbox_set_anim_time, "", "(lv_obj)i"); }
  int lvbe_msgbox_start_auto_close(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_msgbox_start_auto_close, "", "(lv_obj)i"); }
  int lvbe_msgbox_stop_auto_close(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_msgbox_stop_auto_close, "", "(lv_obj)"); }
  int lvbe_msgbox_set_recolor(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_msgbox_set_recolor, "", "(lv_obj)b"); }
  int lvbe_msgbox_get_text(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_msgbox_get_text, "s", "(lv_obj)"); }
  int lvbe_msgbox_get_active_btn(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_msgbox_get_active_btn, "i", "(lv_obj)"); }
  int lvbe_msgbox_get_active_btn_text(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_msgbox_get_active_btn_text, "s", "(lv_obj)"); }
  int lvbe_msgbox_get_anim_time(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_msgbox_get_anim_time, "i", "(lv_obj)"); }
  int lvbe_msgbox_get_recolor(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_msgbox_get_recolor, "b", "(lv_obj)"); }
  int lvbe_msgbox_get_btnmatrix(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_msgbox_get_btnmatrix, "lv_obj", "(lv_obj)"); }

  /* `lv_objmask` methods */
  int lvbe_objmask_create(bvm *vm)       { return lvx_init_2(vm, (void*) &lv_objmask_create, "lv_objmask", "(lv_obj)(lv_obj)"); }
  int lvbe_objmask_update_mask(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_objmask_update_mask, "", "(lv_obj)(lv_objmask_mask)i"); }
  int lvbe_objmask_remove_mask(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_objmask_remove_mask, "", "(lv_obj)(lv_objmask_mask)"); }

  /* `lv_page` methods */
  int lvbe_page_create(bvm *vm)       { return lvx_init_2(vm, (void*) &lv_page_create, "lv_page", "(lv_obj)(lv_obj)"); }
  int lvbe_page_clean(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_page_clean, "", "(lv_obj)"); }
  int lvbe_page_get_scrollable(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_page_get_scrollable, "lv_obj", "(lv_obj)"); }
  int lvbe_page_get_anim_time(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_page_get_anim_time, "i", "(lv_obj)"); }
  int lvbe_page_set_scrollbar_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_page_set_scrollbar_mode, "", "(lv_obj)i"); }
  int lvbe_page_set_anim_time(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_page_set_anim_time, "", "(lv_obj)i"); }
  int lvbe_page_set_scroll_propagation(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_page_set_scroll_propagation, "", "(lv_obj)b"); }
  int lvbe_page_set_edge_flash(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_page_set_edge_flash, "", "(lv_obj)b"); }
  int lvbe_page_set_scrollable_fit2(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_page_set_scrollable_fit2, "", "(lv_obj)ii"); }
  int lvbe_page_set_scrollable_fit(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_page_set_scrollable_fit, "", "(lv_obj)i"); }
  int lvbe_page_set_scrl_width(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_page_set_scrl_width, "", "(lv_obj)i"); }
  int lvbe_page_set_scrl_height(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_page_set_scrl_height, "", "(lv_obj)i"); }
  int lvbe_page_set_scrl_layout(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_page_set_scrl_layout, "", "(lv_obj)i"); }
  int lvbe_page_get_scrollbar_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_page_get_scrollbar_mode, "i", "(lv_obj)"); }
  int lvbe_page_get_scroll_propagation(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_page_get_scroll_propagation, "b", "(lv_obj)"); }
  int lvbe_page_get_edge_flash(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_page_get_edge_flash, "b", "(lv_obj)"); }
  int lvbe_page_get_width_fit(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_page_get_width_fit, "i", "(lv_obj)"); }
  int lvbe_page_get_height_fit(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_page_get_height_fit, "i", "(lv_obj)"); }
  int lvbe_page_get_width_grid(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_page_get_width_grid, "i", "(lv_obj)ii"); }
  int lvbe_page_get_height_grid(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_page_get_height_grid, "i", "(lv_obj)ii"); }
  int lvbe_page_get_scrl_width(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_page_get_scrl_width, "i", "(lv_obj)"); }
  int lvbe_page_get_scrl_height(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_page_get_scrl_height, "i", "(lv_obj)"); }
  int lvbe_page_get_scrl_layout(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_page_get_scrl_layout, "i", "(lv_obj)"); }
  int lvbe_page_get_scrl_fit_left(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_page_get_scrl_fit_left, "i", "(lv_obj)"); }
  int lvbe_page_get_scrl_fit_right(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_page_get_scrl_fit_right, "i", "(lv_obj)"); }
  int lvbe_page_get_scrl_fit_top(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_page_get_scrl_fit_top, "i", "(lv_obj)"); }
  int lvbe_page_get_scrl_fit_bottom(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_page_get_scrl_fit_bottom, "i", "(lv_obj)"); }
  int lvbe_page_on_edge(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_page_on_edge, "b", "(lv_obj)(lv_page_edge)"); }
  int lvbe_page_glue_obj(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_page_glue_obj, "", "(lv_obj)b"); }
  int lvbe_page_focus(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_page_focus, "", "(lv_obj)(lv_obj)(lv_anim_enable)"); }
  int lvbe_page_scroll_hor(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_page_scroll_hor, "", "(lv_obj)i"); }
  int lvbe_page_scroll_ver(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_page_scroll_ver, "", "(lv_obj)i"); }
  int lvbe_page_start_edge_flash(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_page_start_edge_flash, "", "(lv_obj)(lv_page_edge)"); }

  /* `lv_roller` methods */
  int lvbe_roller_create(bvm *vm)       { return lvx_init_2(vm, (void*) &lv_roller_create, "lv_roller", "(lv_obj)(lv_obj)"); }
  int lvbe_roller_set_options(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_roller_set_options, "", "(lv_obj)s(lv_roller_mode)"); }
  int lvbe_roller_set_align(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_roller_set_align, "", "(lv_obj)i"); }
  int lvbe_roller_set_selected(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_roller_set_selected, "", "(lv_obj)i(lv_anim_enable)"); }
  int lvbe_roller_set_visible_row_count(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_roller_set_visible_row_count, "", "(lv_obj)i"); }
  int lvbe_roller_set_auto_fit(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_roller_set_auto_fit, "", "(lv_obj)b"); }
  int lvbe_roller_set_anim_time(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_roller_set_anim_time, "", "(lv_obj)i"); }
  int lvbe_roller_get_selected(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_roller_get_selected, "i", "(lv_obj)"); }
  int lvbe_roller_get_option_cnt(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_roller_get_option_cnt, "i", "(lv_obj)"); }
  int lvbe_roller_get_selected_str(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_roller_get_selected_str, "", "(lv_obj)si"); }
  int lvbe_roller_get_align(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_roller_get_align, "i", "(lv_obj)"); }
  int lvbe_roller_get_auto_fit(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_roller_get_auto_fit, "b", "(lv_obj)"); }
  int lvbe_roller_get_options(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_roller_get_options, "s", "(lv_obj)"); }
  int lvbe_roller_get_anim_time(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_roller_get_anim_time, "i", "(lv_obj)"); }

  /* `lv_slider` methods */
  int lvbe_slider_create(bvm *vm)       { return lvx_init_2(vm, (void*) &lv_slider_create, "lv_slider", "(lv_obj)(lv_obj)"); }
  int lvbe_slider_set_value(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_slider_set_value, "", "(lv_obj)i(lv_anim_enable)"); }
  int lvbe_slider_set_left_value(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_slider_set_left_value, "", "(lv_obj)i(lv_anim_enable)"); }
  int lvbe_slider_set_range(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_slider_set_range, "", "(lv_obj)ii"); }
  int lvbe_slider_set_anim_time(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_slider_set_anim_time, "", "(lv_obj)i"); }
  int lvbe_slider_set_type(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_slider_set_type, "", "(lv_obj)i"); }
  int lvbe_slider_get_value(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_slider_get_value, "i", "(lv_obj)"); }
  int lvbe_slider_get_left_value(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_slider_get_left_value, "i", "(lv_obj)"); }
  int lvbe_slider_get_min_value(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_slider_get_min_value, "i", "(lv_obj)"); }
  int lvbe_slider_get_max_value(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_slider_get_max_value, "i", "(lv_obj)"); }
  int lvbe_slider_is_dragged(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_slider_is_dragged, "b", "(lv_obj)"); }
  int lvbe_slider_get_anim_time(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_slider_get_anim_time, "i", "(lv_obj)"); }
  int lvbe_slider_get_type(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_slider_get_type, "i", "(lv_obj)"); }

  /* `lv_spinbox` methods */
  int lvbe_spinbox_create(bvm *vm)       { return lvx_init_2(vm, (void*) &lv_spinbox_create, "lv_spinbox", "(lv_obj)(lv_obj)"); }
  int lvbe_spinbox_set_rollover(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_spinbox_set_rollover, "", "(lv_obj)b"); }
  int lvbe_spinbox_set_value(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_spinbox_set_value, "", "(lv_obj)i"); }
  int lvbe_spinbox_set_digit_format(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_spinbox_set_digit_format, "", "(lv_obj)ii"); }
  int lvbe_spinbox_set_step(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_spinbox_set_step, "", "(lv_obj)i"); }
  int lvbe_spinbox_set_range(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_spinbox_set_range, "", "(lv_obj)ii"); }
  int lvbe_spinbox_set_padding_left(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_spinbox_set_padding_left, "", "(lv_obj)i"); }
  int lvbe_spinbox_get_rollover(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_spinbox_get_rollover, "b", "(lv_obj)"); }
  int lvbe_spinbox_get_value(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_spinbox_get_value, "i", "(lv_obj)"); }
  int lvbe_spinbox_get_step(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_spinbox_get_step, "i", "(lv_obj)"); }
  int lvbe_spinbox_step_next(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_spinbox_step_next, "", "(lv_obj)"); }
  int lvbe_spinbox_step_prev(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_spinbox_step_prev, "", "(lv_obj)"); }
  int lvbe_spinbox_increment(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_spinbox_increment, "", "(lv_obj)"); }
  int lvbe_spinbox_decrement(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_spinbox_decrement, "", "(lv_obj)"); }

  /* `lv_spinner` methods */
  int lvbe_spinner_create(bvm *vm)       { return lvx_init_2(vm, (void*) &lv_spinner_create, "lv_spinner", "(lv_obj)(lv_obj)"); }
  int lvbe_spinner_set_arc_length(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_spinner_set_arc_length, "", "(lv_obj)i"); }
  int lvbe_spinner_set_spin_time(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_spinner_set_spin_time, "", "(lv_obj)i"); }
  int lvbe_spinner_set_type(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_spinner_set_type, "", "(lv_obj)i"); }
  int lvbe_spinner_set_dir(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_spinner_set_dir, "", "(lv_obj)i"); }
  int lvbe_spinner_get_arc_length(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_spinner_get_arc_length, "i", "(lv_obj)"); }
  int lvbe_spinner_get_spin_time(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_spinner_get_spin_time, "i", "(lv_obj)"); }
  int lvbe_spinner_get_type(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_spinner_get_type, "i", "(lv_obj)"); }
  int lvbe_spinner_get_dir(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_spinner_get_dir, "i", "(lv_obj)"); }
  int lvbe_spinner_anim_cb(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_spinner_anim_cb, "", "ii"); }

  /* `lv_switch` methods */
  int lvbe_switch_create(bvm *vm)       { return lvx_init_2(vm, (void*) &lv_switch_create, "lv_switch", "(lv_obj)(lv_obj)"); }
  int lvbe_switch_on(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_switch_on, "", "(lv_obj)(lv_anim_enable)"); }
  int lvbe_switch_off(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_switch_off, "", "(lv_obj)(lv_anim_enable)"); }
  int lvbe_switch_toggle(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_switch_toggle, "b", "(lv_obj)(lv_anim_enable)"); }
  int lvbe_switch_set_anim_time(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_switch_set_anim_time, "", "(lv_obj)i"); }
  int lvbe_switch_get_state(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_switch_get_state, "b", "(lv_obj)"); }
  int lvbe_switch_get_anim_time(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_switch_get_anim_time, "i", "(lv_obj)"); }

  /* `lv_table` methods */
  int lvbe_table_create(bvm *vm)       { return lvx_init_2(vm, (void*) &lv_table_create, "lv_table", "(lv_obj)(lv_obj)"); }
  int lvbe_table_set_cell_value(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_table_set_cell_value, "", "(lv_obj)iis"); }
  int lvbe_table_set_cell_value_fmt(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_table_set_cell_value_fmt, "", "(lv_obj)iis"); }
  int lvbe_table_set_row_cnt(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_table_set_row_cnt, "", "(lv_obj)i"); }
  int lvbe_table_set_col_cnt(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_table_set_col_cnt, "", "(lv_obj)i"); }
  int lvbe_table_set_col_width(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_table_set_col_width, "", "(lv_obj)ii"); }
  int lvbe_table_set_cell_align(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_table_set_cell_align, "", "(lv_obj)iii"); }
  int lvbe_table_set_cell_type(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_table_set_cell_type, "", "(lv_obj)iii"); }
  int lvbe_table_set_cell_crop(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_table_set_cell_crop, "", "(lv_obj)iib"); }
  int lvbe_table_set_cell_merge_right(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_table_set_cell_merge_right, "", "(lv_obj)iib"); }
  int lvbe_table_get_cell_value(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_table_get_cell_value, "s", "(lv_obj)ii"); }
  int lvbe_table_get_row_cnt(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_table_get_row_cnt, "i", "(lv_obj)"); }
  int lvbe_table_get_col_cnt(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_table_get_col_cnt, "i", "(lv_obj)"); }
  int lvbe_table_get_col_width(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_table_get_col_width, "i", "(lv_obj)i"); }
  int lvbe_table_get_cell_align(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_table_get_cell_align, "i", "(lv_obj)ii"); }
  int lvbe_table_get_cell_type(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_table_get_cell_type, "i", "(lv_obj)ii"); }
  int lvbe_table_get_cell_crop(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_table_get_cell_crop, "i", "(lv_obj)ii"); }
  int lvbe_table_get_cell_merge_right(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_table_get_cell_merge_right, "b", "(lv_obj)ii"); }
  int lvbe_table_get_pressed_cell(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_table_get_pressed_cell, "i", "(lv_obj)(uint16)(uint16)"); }

  /* `lv_tabview` methods */
  int lvbe_tabview_create(bvm *vm)       { return lvx_init_2(vm, (void*) &lv_tabview_create, "lv_tabview", "(lv_obj)(lv_obj)"); }
  int lvbe_tabview_add_tab(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_tabview_add_tab, "lv_obj", "(lv_obj)s"); }
  int lvbe_tabview_clean_tab(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_tabview_clean_tab, "", "(lv_obj)"); }
  int lvbe_tabview_set_tab_act(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_tabview_set_tab_act, "", "(lv_obj)i(lv_anim_enable)"); }
  int lvbe_tabview_set_tab_name(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_tabview_set_tab_name, "", "(lv_obj)is"); }
  int lvbe_tabview_set_anim_time(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_tabview_set_anim_time, "", "(lv_obj)i"); }
  int lvbe_tabview_set_btns_pos(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_tabview_set_btns_pos, "", "(lv_obj)i"); }
  int lvbe_tabview_get_tab_act(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_tabview_get_tab_act, "i", "(lv_obj)"); }
  int lvbe_tabview_get_tab_count(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_tabview_get_tab_count, "i", "(lv_obj)"); }
  int lvbe_tabview_get_tab(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_tabview_get_tab, "lv_obj", "(lv_obj)i"); }
  int lvbe_tabview_get_anim_time(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_tabview_get_anim_time, "i", "(lv_obj)"); }
  int lvbe_tabview_get_btns_pos(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_tabview_get_btns_pos, "i", "(lv_obj)"); }

  /* `lv_textarea` methods */
  int lvbe_textarea_create(bvm *vm)       { return lvx_init_2(vm, (void*) &lv_textarea_create, "lv_textarea", "(lv_obj)(lv_obj)"); }
  int lvbe_textarea_add_char(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_add_char, "", "(lv_obj)i"); }
  int lvbe_textarea_add_text(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_add_text, "", "(lv_obj)s"); }
  int lvbe_textarea_del_char(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_del_char, "", "(lv_obj)"); }
  int lvbe_textarea_del_char_forward(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_del_char_forward, "", "(lv_obj)"); }
  int lvbe_textarea_set_text(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_set_text, "", "(lv_obj)s"); }
  int lvbe_textarea_set_placeholder_text(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_set_placeholder_text, "", "(lv_obj)s"); }
  int lvbe_textarea_set_cursor_pos(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_set_cursor_pos, "", "(lv_obj)i"); }
  int lvbe_textarea_set_cursor_hidden(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_set_cursor_hidden, "", "(lv_obj)b"); }
  int lvbe_textarea_set_cursor_click_pos(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_set_cursor_click_pos, "", "(lv_obj)b"); }
  int lvbe_textarea_set_pwd_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_set_pwd_mode, "", "(lv_obj)b"); }
  int lvbe_textarea_set_one_line(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_set_one_line, "", "(lv_obj)b"); }
  int lvbe_textarea_set_text_align(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_set_text_align, "", "(lv_obj)i"); }
  int lvbe_textarea_set_accepted_chars(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_set_accepted_chars, "", "(lv_obj)s"); }
  int lvbe_textarea_set_max_length(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_set_max_length, "", "(lv_obj)i"); }
  int lvbe_textarea_set_insert_replace(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_set_insert_replace, "", "(lv_obj)s"); }
  int lvbe_textarea_set_scrollbar_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_set_scrollbar_mode, "", "(lv_obj)i"); }
  int lvbe_textarea_set_scroll_propagation(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_set_scroll_propagation, "", "(lv_obj)b"); }
  int lvbe_textarea_set_edge_flash(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_set_edge_flash, "", "(lv_obj)b"); }
  int lvbe_textarea_set_text_sel(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_set_text_sel, "", "(lv_obj)b"); }
  int lvbe_textarea_set_pwd_show_time(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_set_pwd_show_time, "", "(lv_obj)i"); }
  int lvbe_textarea_set_cursor_blink_time(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_set_cursor_blink_time, "", "(lv_obj)i"); }
  int lvbe_textarea_get_text(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_get_text, "s", "(lv_obj)"); }
  int lvbe_textarea_get_placeholder_text(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_get_placeholder_text, "s", "(lv_obj)"); }
  int lvbe_textarea_get_label(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_get_label, "lv_obj", "(lv_obj)"); }
  int lvbe_textarea_get_cursor_pos(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_get_cursor_pos, "i", "(lv_obj)"); }
  int lvbe_textarea_get_cursor_hidden(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_get_cursor_hidden, "b", "(lv_obj)"); }
  int lvbe_textarea_get_cursor_click_pos(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_get_cursor_click_pos, "b", "(lv_obj)"); }
  int lvbe_textarea_get_pwd_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_get_pwd_mode, "b", "(lv_obj)"); }
  int lvbe_textarea_get_one_line(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_get_one_line, "b", "(lv_obj)"); }
  int lvbe_textarea_get_accepted_chars(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_get_accepted_chars, "s", "(lv_obj)"); }
  int lvbe_textarea_get_max_length(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_get_max_length, "i", "(lv_obj)"); }
  int lvbe_textarea_get_scrollbar_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_get_scrollbar_mode, "i", "(lv_obj)"); }
  int lvbe_textarea_get_scroll_propagation(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_get_scroll_propagation, "b", "(lv_obj)"); }
  int lvbe_textarea_get_edge_flash(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_get_edge_flash, "b", "(lv_obj)"); }
  int lvbe_textarea_text_is_selected(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_text_is_selected, "b", "(lv_obj)"); }
  int lvbe_textarea_get_text_sel_en(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_get_text_sel_en, "b", "(lv_obj)"); }
  int lvbe_textarea_get_pwd_show_time(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_get_pwd_show_time, "i", "(lv_obj)"); }
  int lvbe_textarea_get_cursor_blink_time(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_get_cursor_blink_time, "i", "(lv_obj)"); }
  int lvbe_textarea_clear_selection(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_clear_selection, "", "(lv_obj)"); }
  int lvbe_textarea_cursor_right(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_cursor_right, "", "(lv_obj)"); }
  int lvbe_textarea_cursor_left(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_cursor_left, "", "(lv_obj)"); }
  int lvbe_textarea_cursor_down(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_cursor_down, "", "(lv_obj)"); }
  int lvbe_textarea_cursor_up(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_textarea_cursor_up, "", "(lv_obj)"); }

  /* `lv_tileview` methods */
  int lvbe_tileview_create(bvm *vm)       { return lvx_init_2(vm, (void*) &lv_tileview_create, "lv_tileview", "(lv_obj)(lv_obj)"); }
  int lvbe_tileview_add_element(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_tileview_add_element, "", "(lv_obj)(lv_obj)"); }
  int lvbe_tileview_set_valid_positions(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_tileview_set_valid_positions, "", "(lv_obj)ii"); }
  int lvbe_tileview_set_tile_act(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_tileview_set_tile_act, "", "(lv_obj)ii(lv_anim_enable)"); }
  int lvbe_tileview_set_edge_flash(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_tileview_set_edge_flash, "", "(lv_obj)b"); }
  int lvbe_tileview_set_anim_time(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_tileview_set_anim_time, "", "(lv_obj)i"); }
  int lvbe_tileview_get_tile_act(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_tileview_get_tile_act, "", "(lv_obj)(lv_coord)(lv_coord)"); }
  int lvbe_tileview_get_edge_flash(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_tileview_get_edge_flash, "b", "(lv_obj)"); }
  int lvbe_tileview_get_anim_time(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_tileview_get_anim_time, "i", "(lv_obj)"); }

  /* `lv_win` methods */
  int lvbe_win_create(bvm *vm)       { return lvx_init_2(vm, (void*) &lv_win_create, "lv_win", "(lv_obj)(lv_obj)"); }
  int lvbe_win_clean(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_win_clean, "", "(lv_obj)"); }
  int lvbe_win_add_btn_right(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_win_add_btn_right, "lv_obj", "(lv_obj)i"); }
  int lvbe_win_add_btn_left(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_win_add_btn_left, "lv_obj", "(lv_obj)i"); }
  int lvbe_win_close_event_cb(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_win_close_event_cb, "", "(lv_obj)(lv_event)"); }
  int lvbe_win_set_title(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_win_set_title, "", "(lv_obj)s"); }
  int lvbe_win_set_header_height(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_win_set_header_height, "", "(lv_obj)i"); }
  int lvbe_win_set_btn_width(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_win_set_btn_width, "", "(lv_obj)i"); }
  int lvbe_win_set_content_size(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_win_set_content_size, "", "(lv_obj)ii"); }
  int lvbe_win_set_layout(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_win_set_layout, "", "(lv_obj)i"); }
  int lvbe_win_set_scrollbar_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_win_set_scrollbar_mode, "", "(lv_obj)i"); }
  int lvbe_win_set_anim_time(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_win_set_anim_time, "", "(lv_obj)i"); }
  int lvbe_win_set_drag(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_win_set_drag, "", "(lv_obj)b"); }
  int lvbe_win_title_set_alignment(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_win_title_set_alignment, "", "(lv_obj)i"); }
  int lvbe_win_get_title(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_win_get_title, "s", "(lv_obj)"); }
  int lvbe_win_get_content(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_win_get_content, "lv_obj", "(lv_obj)"); }
  int lvbe_win_get_header_height(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_win_get_header_height, "i", "(lv_obj)"); }
  int lvbe_win_get_btn_width(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_win_get_btn_width, "i", "(lv_obj)"); }
  int lvbe_win_get_from_btn(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_win_get_from_btn, "lv_obj", "(lv_obj)"); }
  int lvbe_win_get_layout(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_win_get_layout, "i", "(lv_obj)"); }
  int lvbe_win_get_sb_mode(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_win_get_sb_mode, "i", "(lv_obj)"); }
  int lvbe_win_get_anim_time(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_win_get_anim_time, "i", "(lv_obj)"); }
  int lvbe_win_get_width(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_win_get_width, "i", "(lv_obj)"); }
  int lvbe_win_get_drag(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_win_get_drag, "b", "(lv_obj)"); }
  int lvbe_win_title_get_alignment(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_win_title_get_alignment, "i", "(lv_obj)"); }
  int lvbe_win_focus(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_win_focus, "", "(lv_obj)(lv_obj)(lv_anim_enable)"); }
  int lvbe_win_scroll_hor(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_win_scroll_hor, "", "(lv_obj)i"); }
  int lvbe_win_scroll_ver(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_win_scroll_ver, "", "(lv_obj)i"); }


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
