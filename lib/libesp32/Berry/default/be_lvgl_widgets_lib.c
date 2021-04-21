
/********************************************************************
 * Generated code, don't edit
 *******************************************************************/
 
 /********************************************************************
 * Tasmota LVGL classes for widgets
 *******************************************************************/
#include "be_object.h"
#include "be_string.h"

#ifdef USE_LVGL

#include "lvgl.h"

extern int lv0_start(bvm *vm);

extern int lv0_scr_act(bvm *vm);
extern int lv0_layer_top(bvm *vm);
extern int lv0_layer_sys(bvm *vm);
extern int lv0_get_hor_res(bvm *vm);
extern int lv0_get_ver_res(bvm *vm);
extern int lv0_screenshot(bvm *vm);

extern int lco_init(bvm *vm);
extern int lco_tostring(bvm *vm);

extern int lvx_init_2(bvm *vm);           // generic function
extern int lvx_tostring(bvm *vm);       // generic function

extern int lvs_init(bvm *vm);
extern int lvs_tostring(bvm *vm);

BE_EXPORT_VARIABLE extern const bclass be_class_lv_obj;

/* `lv_img` external functions definitions */
extern int lvbe_img_set_tasmota_logo(bvm *vm);
extern int lvbe_img_create(bvm *vm);
extern int lvbe_img_set_src(bvm *vm);
extern int lvbe_img_set_auto_size(bvm *vm);
extern int lvbe_img_set_offset_x(bvm *vm);
extern int lvbe_img_set_offset_y(bvm *vm);
extern int lvbe_img_set_pivot(bvm *vm);
extern int lvbe_img_set_angle(bvm *vm);
extern int lvbe_img_set_zoom(bvm *vm);
extern int lvbe_img_set_antialias(bvm *vm);
extern int lvbe_img_get_src(bvm *vm);
extern int lvbe_img_get_file_name(bvm *vm);
extern int lvbe_img_get_auto_size(bvm *vm);
extern int lvbe_img_get_offset_x(bvm *vm);
extern int lvbe_img_get_offset_y(bvm *vm);
extern int lvbe_img_get_angle(bvm *vm);
extern int lvbe_img_get_pivot(bvm *vm);
extern int lvbe_img_get_zoom(bvm *vm);
extern int lvbe_img_get_antialias(bvm *vm);

/* `lv_style` external functions definitions */
extern int lvbe_style_copy(bvm *vm);
extern int lvbe_style_list_init(bvm *vm);
extern int lvbe_style_list_copy(bvm *vm);
extern int lvbe_style_list_get_style(bvm *vm);
extern int lvbe_style_reset(bvm *vm);
extern int lvbe_style_remove_prop(bvm *vm);
extern int lvbe_style_list_get_local_style(bvm *vm);
extern int lvbe_style_set_radius(bvm *vm);
extern int lvbe_style_set_clip_corner(bvm *vm);
extern int lvbe_style_set_size(bvm *vm);
extern int lvbe_style_set_transform_width(bvm *vm);
extern int lvbe_style_set_transform_height(bvm *vm);
extern int lvbe_style_set_transform_angle(bvm *vm);
extern int lvbe_style_set_transform_zoom(bvm *vm);
extern int lvbe_style_set_opa_scale(bvm *vm);
extern int lvbe_style_set_pad_top(bvm *vm);
extern int lvbe_style_set_pad_bottom(bvm *vm);
extern int lvbe_style_set_pad_left(bvm *vm);
extern int lvbe_style_set_pad_right(bvm *vm);
extern int lvbe_style_set_pad_inner(bvm *vm);
extern int lvbe_style_set_margin_top(bvm *vm);
extern int lvbe_style_set_margin_bottom(bvm *vm);
extern int lvbe_style_set_margin_left(bvm *vm);
extern int lvbe_style_set_margin_right(bvm *vm);
extern int lvbe_style_set_bg_blend_mode(bvm *vm);
extern int lvbe_style_set_bg_main_stop(bvm *vm);
extern int lvbe_style_set_bg_grad_stop(bvm *vm);
extern int lvbe_style_set_bg_grad_dir(bvm *vm);
extern int lvbe_style_set_bg_color(bvm *vm);
extern int lvbe_style_set_bg_grad_color(bvm *vm);
extern int lvbe_style_set_bg_opa(bvm *vm);
extern int lvbe_style_set_border_width(bvm *vm);
extern int lvbe_style_set_border_side(bvm *vm);
extern int lvbe_style_set_border_blend_mode(bvm *vm);
extern int lvbe_style_set_border_post(bvm *vm);
extern int lvbe_style_set_border_color(bvm *vm);
extern int lvbe_style_set_border_opa(bvm *vm);
extern int lvbe_style_set_outline_width(bvm *vm);
extern int lvbe_style_set_outline_pad(bvm *vm);
extern int lvbe_style_set_outline_blend_mode(bvm *vm);
extern int lvbe_style_set_outline_color(bvm *vm);
extern int lvbe_style_set_outline_opa(bvm *vm);
extern int lvbe_style_set_shadow_width(bvm *vm);
extern int lvbe_style_set_shadow_ofs_x(bvm *vm);
extern int lvbe_style_set_shadow_ofs_y(bvm *vm);
extern int lvbe_style_set_shadow_spread(bvm *vm);
extern int lvbe_style_set_shadow_blend_mode(bvm *vm);
extern int lvbe_style_set_shadow_color(bvm *vm);
extern int lvbe_style_set_shadow_opa(bvm *vm);
extern int lvbe_style_set_pattern_repeat(bvm *vm);
extern int lvbe_style_set_pattern_blend_mode(bvm *vm);
extern int lvbe_style_set_pattern_recolor(bvm *vm);
extern int lvbe_style_set_pattern_opa(bvm *vm);
extern int lvbe_style_set_pattern_recolor_opa(bvm *vm);
extern int lvbe_style_set_pattern_image(bvm *vm);
extern int lvbe_style_set_value_letter_space(bvm *vm);
extern int lvbe_style_set_value_line_space(bvm *vm);
extern int lvbe_style_set_value_blend_mode(bvm *vm);
extern int lvbe_style_set_value_ofs_x(bvm *vm);
extern int lvbe_style_set_value_ofs_y(bvm *vm);
extern int lvbe_style_set_value_align(bvm *vm);
extern int lvbe_style_set_value_color(bvm *vm);
extern int lvbe_style_set_value_opa(bvm *vm);
extern int lvbe_style_set_value_font(bvm *vm);
extern int lvbe_style_set_value_str(bvm *vm);
extern int lvbe_style_set_text_letter_space(bvm *vm);
extern int lvbe_style_set_text_line_space(bvm *vm);
extern int lvbe_style_set_text_decor(bvm *vm);
extern int lvbe_style_set_text_blend_mode(bvm *vm);
extern int lvbe_style_set_text_color(bvm *vm);
extern int lvbe_style_set_text_sel_color(bvm *vm);
extern int lvbe_style_set_text_sel_bg_color(bvm *vm);
extern int lvbe_style_set_text_opa(bvm *vm);
extern int lvbe_style_set_text_font(bvm *vm);
extern int lvbe_style_set_line_width(bvm *vm);
extern int lvbe_style_set_line_blend_mode(bvm *vm);
extern int lvbe_style_set_line_dash_width(bvm *vm);
extern int lvbe_style_set_line_dash_gap(bvm *vm);
extern int lvbe_style_set_line_rounded(bvm *vm);
extern int lvbe_style_set_line_color(bvm *vm);
extern int lvbe_style_set_line_opa(bvm *vm);
extern int lvbe_style_set_image_blend_mode(bvm *vm);
extern int lvbe_style_set_image_recolor(bvm *vm);
extern int lvbe_style_set_image_opa(bvm *vm);
extern int lvbe_style_set_image_recolor_opa(bvm *vm);
extern int lvbe_style_set_transition_time(bvm *vm);
extern int lvbe_style_set_transition_delay(bvm *vm);
extern int lvbe_style_set_transition_prop_1(bvm *vm);
extern int lvbe_style_set_transition_prop_2(bvm *vm);
extern int lvbe_style_set_transition_prop_3(bvm *vm);
extern int lvbe_style_set_transition_prop_4(bvm *vm);
extern int lvbe_style_set_transition_prop_5(bvm *vm);
extern int lvbe_style_set_transition_prop_6(bvm *vm);
extern int lvbe_style_set_transition_path(bvm *vm);
extern int lvbe_style_set_scale_width(bvm *vm);
extern int lvbe_style_set_scale_border_width(bvm *vm);
extern int lvbe_style_set_scale_end_border_width(bvm *vm);
extern int lvbe_style_set_scale_end_line_width(bvm *vm);
extern int lvbe_style_set_scale_grad_color(bvm *vm);
extern int lvbe_style_set_scale_end_color(bvm *vm);

/* `lv_group` external functions definitions */
extern int lvbe_group_create(bvm *vm);
extern int lvbe_group_del(bvm *vm);
extern int lvbe_group_add_obj(bvm *vm);
extern int lvbe_group_remove_obj(bvm *vm);
extern int lvbe_group_remove_all_objs(bvm *vm);
extern int lvbe_group_focus_obj(bvm *vm);
extern int lvbe_group_focus_next(bvm *vm);
extern int lvbe_group_focus_prev(bvm *vm);
extern int lvbe_group_focus_freeze(bvm *vm);
extern int lvbe_group_send_data(bvm *vm);
extern int lvbe_group_set_focus_cb(bvm *vm);
extern int lvbe_group_set_refocus_policy(bvm *vm);
extern int lvbe_group_set_editing(bvm *vm);
extern int lvbe_group_set_click_focus(bvm *vm);
extern int lvbe_group_set_wrap(bvm *vm);
extern int lvbe_group_get_focused(bvm *vm);
extern int lvbe_group_get_focus_cb(bvm *vm);
extern int lvbe_group_get_editing(bvm *vm);
extern int lvbe_group_get_click_focus(bvm *vm);
extern int lvbe_group_get_wrap(bvm *vm);

/* `lv_obj` external functions definitions */
extern int lvbe_obj_create(bvm *vm);
extern int lvbe_obj_del(bvm *vm);
extern int lvbe_obj_del_anim_ready_cb(bvm *vm);
extern int lvbe_obj_del_async(bvm *vm);
extern int lvbe_obj_clean(bvm *vm);
extern int lvbe_obj_invalidate_area(bvm *vm);
extern int lvbe_obj_invalidate(bvm *vm);
extern int lvbe_obj_area_is_visible(bvm *vm);
extern int lvbe_obj_is_visible(bvm *vm);
extern int lvbe_obj_set_parent(bvm *vm);
extern int lvbe_obj_move_foreground(bvm *vm);
extern int lvbe_obj_move_background(bvm *vm);
extern int lvbe_obj_set_pos(bvm *vm);
extern int lvbe_obj_set_x(bvm *vm);
extern int lvbe_obj_set_y(bvm *vm);
extern int lvbe_obj_set_size(bvm *vm);
extern int lvbe_obj_set_width(bvm *vm);
extern int lvbe_obj_set_height(bvm *vm);
extern int lvbe_obj_set_width_fit(bvm *vm);
extern int lvbe_obj_set_height_fit(bvm *vm);
extern int lvbe_obj_set_width_margin(bvm *vm);
extern int lvbe_obj_set_height_margin(bvm *vm);
extern int lvbe_obj_align(bvm *vm);
extern int lvbe_obj_align_x(bvm *vm);
extern int lvbe_obj_align_y(bvm *vm);
extern int lvbe_obj_align_mid(bvm *vm);
extern int lvbe_obj_align_mid_x(bvm *vm);
extern int lvbe_obj_align_mid_y(bvm *vm);
extern int lvbe_obj_realign(bvm *vm);
extern int lvbe_obj_set_auto_realign(bvm *vm);
extern int lvbe_obj_set_ext_click_area(bvm *vm);
extern int lvbe_obj_add_style(bvm *vm);
extern int lvbe_obj_remove_style(bvm *vm);
extern int lvbe_obj_clean_style_list(bvm *vm);
extern int lvbe_obj_reset_style_list(bvm *vm);
extern int lvbe_obj_refresh_style(bvm *vm);
extern int lvbe_obj_report_style_mod(bvm *vm);
extern int lvbe_obj_remove_style_local_prop(bvm *vm);
extern int lvbe_obj_set_hidden(bvm *vm);
extern int lvbe_obj_set_adv_hittest(bvm *vm);
extern int lvbe_obj_set_click(bvm *vm);
extern int lvbe_obj_set_top(bvm *vm);
extern int lvbe_obj_set_drag(bvm *vm);
extern int lvbe_obj_set_drag_dir(bvm *vm);
extern int lvbe_obj_set_drag_throw(bvm *vm);
extern int lvbe_obj_set_drag_parent(bvm *vm);
extern int lvbe_obj_set_focus_parent(bvm *vm);
extern int lvbe_obj_set_gesture_parent(bvm *vm);
extern int lvbe_obj_set_parent_event(bvm *vm);
extern int lvbe_obj_set_base_dir(bvm *vm);
extern int lvbe_obj_add_protect(bvm *vm);
extern int lvbe_obj_clear_protect(bvm *vm);
extern int lvbe_obj_set_state(bvm *vm);
extern int lvbe_obj_add_state(bvm *vm);
extern int lvbe_obj_clear_state(bvm *vm);
extern int lvbe_obj_finish_transitions(bvm *vm);
extern int lvbe_obj_set_event_cb(bvm *vm);
extern int lvbe_obj_set_signal_cb(bvm *vm);
extern int lvbe_obj_set_design_cb(bvm *vm);
extern int lvbe_obj_allocate_ext_attr(bvm *vm);
extern int lvbe_obj_refresh_ext_draw_pad(bvm *vm);
extern int lvbe_obj_get_screen(bvm *vm);
extern int lvbe_obj_get_parent(bvm *vm);
extern int lvbe_obj_get_child(bvm *vm);
extern int lvbe_obj_get_child_back(bvm *vm);
extern int lvbe_obj_count_children(bvm *vm);
extern int lvbe_obj_count_children_recursive(bvm *vm);
extern int lvbe_obj_get_coords(bvm *vm);
extern int lvbe_obj_get_inner_coords(bvm *vm);
extern int lvbe_obj_get_x(bvm *vm);
extern int lvbe_obj_get_y(bvm *vm);
extern int lvbe_obj_get_width(bvm *vm);
extern int lvbe_obj_get_height(bvm *vm);
extern int lvbe_obj_get_width_fit(bvm *vm);
extern int lvbe_obj_get_height_fit(bvm *vm);
extern int lvbe_obj_get_height_margin(bvm *vm);
extern int lvbe_obj_get_width_margin(bvm *vm);
extern int lvbe_obj_get_width_grid(bvm *vm);
extern int lvbe_obj_get_height_grid(bvm *vm);
extern int lvbe_obj_get_auto_realign(bvm *vm);
extern int lvbe_obj_get_ext_click_pad_left(bvm *vm);
extern int lvbe_obj_get_ext_click_pad_right(bvm *vm);
extern int lvbe_obj_get_ext_click_pad_top(bvm *vm);
extern int lvbe_obj_get_ext_click_pad_bottom(bvm *vm);
extern int lvbe_obj_get_ext_draw_pad(bvm *vm);
extern int lvbe_obj_get_local_style(bvm *vm);
extern int lvbe_obj_get_hidden(bvm *vm);
extern int lvbe_obj_get_adv_hittest(bvm *vm);
extern int lvbe_obj_get_click(bvm *vm);
extern int lvbe_obj_get_top(bvm *vm);
extern int lvbe_obj_get_drag(bvm *vm);
extern int lvbe_obj_get_drag_dir(bvm *vm);
extern int lvbe_obj_get_drag_throw(bvm *vm);
extern int lvbe_obj_get_drag_parent(bvm *vm);
extern int lvbe_obj_get_focus_parent(bvm *vm);
extern int lvbe_obj_get_parent_event(bvm *vm);
extern int lvbe_obj_get_gesture_parent(bvm *vm);
extern int lvbe_obj_get_base_dir(bvm *vm);
extern int lvbe_obj_get_protect(bvm *vm);
extern int lvbe_obj_is_protected(bvm *vm);
extern int lvbe_obj_get_state(bvm *vm);
extern int lvbe_obj_is_point_on_coords(bvm *vm);
extern int lvbe_obj_hittest(bvm *vm);
extern int lvbe_obj_get_ext_attr(bvm *vm);
extern int lvbe_obj_get_type(bvm *vm);
extern int lvbe_obj_get_user_data(bvm *vm);
extern int lvbe_obj_set_user_data(bvm *vm);
extern int lvbe_obj_get_group(bvm *vm);
extern int lvbe_obj_is_focused(bvm *vm);
extern int lvbe_obj_get_focused_obj(bvm *vm);
extern int lvbe_obj_handle_get_type_signal(bvm *vm);
extern int lvbe_obj_init_draw_rect_dsc(bvm *vm);
extern int lvbe_obj_init_draw_label_dsc(bvm *vm);
extern int lvbe_obj_init_draw_img_dsc(bvm *vm);
extern int lvbe_obj_init_draw_line_dsc(bvm *vm);
extern int lvbe_obj_get_draw_rect_ext_pad_size(bvm *vm);
extern int lvbe_obj_fade_in(bvm *vm);
extern int lvbe_obj_fade_out(bvm *vm);
extern int lvbe_obj_get_style_radius(bvm *vm);
extern int lvbe_obj_set_style_local_radius(bvm *vm);
extern int lvbe_obj_get_style_clip_corner(bvm *vm);
extern int lvbe_obj_set_style_local_clip_corner(bvm *vm);
extern int lvbe_obj_get_style_size(bvm *vm);
extern int lvbe_obj_set_style_local_size(bvm *vm);
extern int lvbe_obj_get_style_transform_width(bvm *vm);
extern int lvbe_obj_set_style_local_transform_width(bvm *vm);
extern int lvbe_obj_get_style_transform_height(bvm *vm);
extern int lvbe_obj_set_style_local_transform_height(bvm *vm);
extern int lvbe_obj_get_style_transform_angle(bvm *vm);
extern int lvbe_obj_set_style_local_transform_angle(bvm *vm);
extern int lvbe_obj_get_style_transform_zoom(bvm *vm);
extern int lvbe_obj_set_style_local_transform_zoom(bvm *vm);
extern int lvbe_obj_get_style_opa_scale(bvm *vm);
extern int lvbe_obj_set_style_local_opa_scale(bvm *vm);
extern int lvbe_obj_get_style_pad_top(bvm *vm);
extern int lvbe_obj_set_style_local_pad_top(bvm *vm);
extern int lvbe_obj_get_style_pad_bottom(bvm *vm);
extern int lvbe_obj_set_style_local_pad_bottom(bvm *vm);
extern int lvbe_obj_get_style_pad_left(bvm *vm);
extern int lvbe_obj_set_style_local_pad_left(bvm *vm);
extern int lvbe_obj_get_style_pad_right(bvm *vm);
extern int lvbe_obj_set_style_local_pad_right(bvm *vm);
extern int lvbe_obj_get_style_pad_inner(bvm *vm);
extern int lvbe_obj_set_style_local_pad_inner(bvm *vm);
extern int lvbe_obj_get_style_margin_top(bvm *vm);
extern int lvbe_obj_set_style_local_margin_top(bvm *vm);
extern int lvbe_obj_get_style_margin_bottom(bvm *vm);
extern int lvbe_obj_set_style_local_margin_bottom(bvm *vm);
extern int lvbe_obj_get_style_margin_left(bvm *vm);
extern int lvbe_obj_set_style_local_margin_left(bvm *vm);
extern int lvbe_obj_get_style_margin_right(bvm *vm);
extern int lvbe_obj_set_style_local_margin_right(bvm *vm);
extern int lvbe_obj_set_style_local_bg_blend_mode(bvm *vm);
extern int lvbe_obj_get_style_bg_main_stop(bvm *vm);
extern int lvbe_obj_set_style_local_bg_main_stop(bvm *vm);
extern int lvbe_obj_get_style_bg_grad_stop(bvm *vm);
extern int lvbe_obj_set_style_local_bg_grad_stop(bvm *vm);
extern int lvbe_obj_set_style_local_bg_grad_dir(bvm *vm);
extern int lvbe_obj_get_style_bg_color(bvm *vm);
extern int lvbe_obj_set_style_local_bg_color(bvm *vm);
extern int lvbe_obj_get_style_bg_grad_color(bvm *vm);
extern int lvbe_obj_set_style_local_bg_grad_color(bvm *vm);
extern int lvbe_obj_get_style_bg_opa(bvm *vm);
extern int lvbe_obj_set_style_local_bg_opa(bvm *vm);
extern int lvbe_obj_get_style_border_width(bvm *vm);
extern int lvbe_obj_set_style_local_border_width(bvm *vm);
extern int lvbe_obj_set_style_local_border_side(bvm *vm);
extern int lvbe_obj_set_style_local_border_blend_mode(bvm *vm);
extern int lvbe_obj_get_style_border_post(bvm *vm);
extern int lvbe_obj_set_style_local_border_post(bvm *vm);
extern int lvbe_obj_get_style_border_color(bvm *vm);
extern int lvbe_obj_set_style_local_border_color(bvm *vm);
extern int lvbe_obj_get_style_border_opa(bvm *vm);
extern int lvbe_obj_set_style_local_border_opa(bvm *vm);
extern int lvbe_obj_get_style_outline_width(bvm *vm);
extern int lvbe_obj_set_style_local_outline_width(bvm *vm);
extern int lvbe_obj_get_style_outline_pad(bvm *vm);
extern int lvbe_obj_set_style_local_outline_pad(bvm *vm);
extern int lvbe_obj_set_style_local_outline_blend_mode(bvm *vm);
extern int lvbe_obj_get_style_outline_color(bvm *vm);
extern int lvbe_obj_set_style_local_outline_color(bvm *vm);
extern int lvbe_obj_get_style_outline_opa(bvm *vm);
extern int lvbe_obj_set_style_local_outline_opa(bvm *vm);
extern int lvbe_obj_get_style_shadow_width(bvm *vm);
extern int lvbe_obj_set_style_local_shadow_width(bvm *vm);
extern int lvbe_obj_get_style_shadow_ofs_x(bvm *vm);
extern int lvbe_obj_set_style_local_shadow_ofs_x(bvm *vm);
extern int lvbe_obj_get_style_shadow_ofs_y(bvm *vm);
extern int lvbe_obj_set_style_local_shadow_ofs_y(bvm *vm);
extern int lvbe_obj_get_style_shadow_spread(bvm *vm);
extern int lvbe_obj_set_style_local_shadow_spread(bvm *vm);
extern int lvbe_obj_set_style_local_shadow_blend_mode(bvm *vm);
extern int lvbe_obj_get_style_shadow_color(bvm *vm);
extern int lvbe_obj_set_style_local_shadow_color(bvm *vm);
extern int lvbe_obj_get_style_shadow_opa(bvm *vm);
extern int lvbe_obj_set_style_local_shadow_opa(bvm *vm);
extern int lvbe_obj_get_style_pattern_repeat(bvm *vm);
extern int lvbe_obj_set_style_local_pattern_repeat(bvm *vm);
extern int lvbe_obj_set_style_local_pattern_blend_mode(bvm *vm);
extern int lvbe_obj_get_style_pattern_recolor(bvm *vm);
extern int lvbe_obj_set_style_local_pattern_recolor(bvm *vm);
extern int lvbe_obj_get_style_pattern_opa(bvm *vm);
extern int lvbe_obj_set_style_local_pattern_opa(bvm *vm);
extern int lvbe_obj_get_style_pattern_recolor_opa(bvm *vm);
extern int lvbe_obj_set_style_local_pattern_recolor_opa(bvm *vm);
extern int lvbe_obj_get_style_pattern_image(bvm *vm);
extern int lvbe_obj_set_style_local_pattern_image(bvm *vm);
extern int lvbe_obj_get_style_value_letter_space(bvm *vm);
extern int lvbe_obj_set_style_local_value_letter_space(bvm *vm);
extern int lvbe_obj_get_style_value_line_space(bvm *vm);
extern int lvbe_obj_set_style_local_value_line_space(bvm *vm);
extern int lvbe_obj_set_style_local_value_blend_mode(bvm *vm);
extern int lvbe_obj_get_style_value_ofs_x(bvm *vm);
extern int lvbe_obj_set_style_local_value_ofs_x(bvm *vm);
extern int lvbe_obj_get_style_value_ofs_y(bvm *vm);
extern int lvbe_obj_set_style_local_value_ofs_y(bvm *vm);
extern int lvbe_obj_set_style_local_value_align(bvm *vm);
extern int lvbe_obj_get_style_value_color(bvm *vm);
extern int lvbe_obj_set_style_local_value_color(bvm *vm);
extern int lvbe_obj_get_style_value_opa(bvm *vm);
extern int lvbe_obj_set_style_local_value_opa(bvm *vm);
extern int lvbe_obj_set_style_local_value_font(bvm *vm);
extern int lvbe_obj_get_style_value_str(bvm *vm);
extern int lvbe_obj_set_style_local_value_str(bvm *vm);
extern int lvbe_obj_get_style_text_letter_space(bvm *vm);
extern int lvbe_obj_set_style_local_text_letter_space(bvm *vm);
extern int lvbe_obj_get_style_text_line_space(bvm *vm);
extern int lvbe_obj_set_style_local_text_line_space(bvm *vm);
extern int lvbe_obj_set_style_local_text_decor(bvm *vm);
extern int lvbe_obj_set_style_local_text_blend_mode(bvm *vm);
extern int lvbe_obj_get_style_text_color(bvm *vm);
extern int lvbe_obj_set_style_local_text_color(bvm *vm);
extern int lvbe_obj_get_style_text_sel_color(bvm *vm);
extern int lvbe_obj_set_style_local_text_sel_color(bvm *vm);
extern int lvbe_obj_get_style_text_sel_bg_color(bvm *vm);
extern int lvbe_obj_set_style_local_text_sel_bg_color(bvm *vm);
extern int lvbe_obj_get_style_text_opa(bvm *vm);
extern int lvbe_obj_set_style_local_text_opa(bvm *vm);
extern int lvbe_obj_set_style_local_text_font(bvm *vm);
extern int lvbe_obj_get_style_line_width(bvm *vm);
extern int lvbe_obj_set_style_local_line_width(bvm *vm);
extern int lvbe_obj_set_style_local_line_blend_mode(bvm *vm);
extern int lvbe_obj_get_style_line_dash_width(bvm *vm);
extern int lvbe_obj_set_style_local_line_dash_width(bvm *vm);
extern int lvbe_obj_get_style_line_dash_gap(bvm *vm);
extern int lvbe_obj_set_style_local_line_dash_gap(bvm *vm);
extern int lvbe_obj_get_style_line_rounded(bvm *vm);
extern int lvbe_obj_set_style_local_line_rounded(bvm *vm);
extern int lvbe_obj_get_style_line_color(bvm *vm);
extern int lvbe_obj_set_style_local_line_color(bvm *vm);
extern int lvbe_obj_get_style_line_opa(bvm *vm);
extern int lvbe_obj_set_style_local_line_opa(bvm *vm);
extern int lvbe_obj_set_style_local_image_blend_mode(bvm *vm);
extern int lvbe_obj_get_style_image_recolor(bvm *vm);
extern int lvbe_obj_set_style_local_image_recolor(bvm *vm);
extern int lvbe_obj_get_style_image_opa(bvm *vm);
extern int lvbe_obj_set_style_local_image_opa(bvm *vm);
extern int lvbe_obj_get_style_image_recolor_opa(bvm *vm);
extern int lvbe_obj_set_style_local_image_recolor_opa(bvm *vm);
extern int lvbe_obj_get_style_transition_time(bvm *vm);
extern int lvbe_obj_set_style_local_transition_time(bvm *vm);
extern int lvbe_obj_get_style_transition_delay(bvm *vm);
extern int lvbe_obj_set_style_local_transition_delay(bvm *vm);
extern int lvbe_obj_get_style_transition_prop_1(bvm *vm);
extern int lvbe_obj_set_style_local_transition_prop_1(bvm *vm);
extern int lvbe_obj_get_style_transition_prop_2(bvm *vm);
extern int lvbe_obj_set_style_local_transition_prop_2(bvm *vm);
extern int lvbe_obj_get_style_transition_prop_3(bvm *vm);
extern int lvbe_obj_set_style_local_transition_prop_3(bvm *vm);
extern int lvbe_obj_get_style_transition_prop_4(bvm *vm);
extern int lvbe_obj_set_style_local_transition_prop_4(bvm *vm);
extern int lvbe_obj_get_style_transition_prop_5(bvm *vm);
extern int lvbe_obj_set_style_local_transition_prop_5(bvm *vm);
extern int lvbe_obj_get_style_transition_prop_6(bvm *vm);
extern int lvbe_obj_set_style_local_transition_prop_6(bvm *vm);
extern int lvbe_obj_set_style_local_transition_path(bvm *vm);
extern int lvbe_obj_get_style_scale_width(bvm *vm);
extern int lvbe_obj_set_style_local_scale_width(bvm *vm);
extern int lvbe_obj_get_style_scale_border_width(bvm *vm);
extern int lvbe_obj_set_style_local_scale_border_width(bvm *vm);
extern int lvbe_obj_get_style_scale_end_border_width(bvm *vm);
extern int lvbe_obj_set_style_local_scale_end_border_width(bvm *vm);
extern int lvbe_obj_get_style_scale_end_line_width(bvm *vm);
extern int lvbe_obj_set_style_local_scale_end_line_width(bvm *vm);
extern int lvbe_obj_get_style_scale_grad_color(bvm *vm);
extern int lvbe_obj_set_style_local_scale_grad_color(bvm *vm);
extern int lvbe_obj_get_style_scale_end_color(bvm *vm);
extern int lvbe_obj_set_style_local_scale_end_color(bvm *vm);

/* `lv_arc` external functions definitions */
extern int lvbe_arc_create(bvm *vm);
extern int lvbe_arc_set_start_angle(bvm *vm);
extern int lvbe_arc_set_end_angle(bvm *vm);
extern int lvbe_arc_set_angles(bvm *vm);
extern int lvbe_arc_set_bg_start_angle(bvm *vm);
extern int lvbe_arc_set_bg_end_angle(bvm *vm);
extern int lvbe_arc_set_bg_angles(bvm *vm);
extern int lvbe_arc_set_rotation(bvm *vm);
extern int lvbe_arc_set_type(bvm *vm);
extern int lvbe_arc_set_value(bvm *vm);
extern int lvbe_arc_set_range(bvm *vm);
extern int lvbe_arc_set_chg_rate(bvm *vm);
extern int lvbe_arc_set_adjustable(bvm *vm);
extern int lvbe_arc_get_angle_start(bvm *vm);
extern int lvbe_arc_get_angle_end(bvm *vm);
extern int lvbe_arc_get_bg_angle_start(bvm *vm);
extern int lvbe_arc_get_bg_angle_end(bvm *vm);
extern int lvbe_arc_get_type(bvm *vm);
extern int lvbe_arc_get_value(bvm *vm);
extern int lvbe_arc_get_min_value(bvm *vm);
extern int lvbe_arc_get_max_value(bvm *vm);
extern int lvbe_arc_is_dragged(bvm *vm);
extern int lvbe_arc_get_adjustable(bvm *vm);

/* `lv_bar` external functions definitions */
extern int lvbe_bar_create(bvm *vm);
extern int lvbe_bar_set_value(bvm *vm);
extern int lvbe_bar_set_start_value(bvm *vm);
extern int lvbe_bar_set_range(bvm *vm);
extern int lvbe_bar_set_type(bvm *vm);
extern int lvbe_bar_set_anim_time(bvm *vm);
extern int lvbe_bar_get_value(bvm *vm);
extern int lvbe_bar_get_start_value(bvm *vm);
extern int lvbe_bar_get_min_value(bvm *vm);
extern int lvbe_bar_get_max_value(bvm *vm);
extern int lvbe_bar_get_type(bvm *vm);
extern int lvbe_bar_get_anim_time(bvm *vm);

/* `lv_btn` external functions definitions */
extern int lvbe_btn_create(bvm *vm);
extern int lvbe_btn_set_checkable(bvm *vm);
extern int lvbe_btn_set_state(bvm *vm);
extern int lvbe_btn_toggle(bvm *vm);
extern int lvbe_btn_set_layout(bvm *vm);
extern int lvbe_btn_set_fit4(bvm *vm);
extern int lvbe_btn_set_fit2(bvm *vm);
extern int lvbe_btn_set_fit(bvm *vm);
extern int lvbe_btn_get_state(bvm *vm);
extern int lvbe_btn_get_checkable(bvm *vm);
extern int lvbe_btn_get_layout(bvm *vm);
extern int lvbe_btn_get_fit_left(bvm *vm);
extern int lvbe_btn_get_fit_right(bvm *vm);
extern int lvbe_btn_get_fit_top(bvm *vm);
extern int lvbe_btn_get_fit_bottom(bvm *vm);

/* `lv_btnmatrix` external functions definitions */
extern int lvbe_btnmatrix_create(bvm *vm);
extern int lvbe_btnmatrix_set_map(bvm *vm);
extern int lvbe_btnmatrix_set_ctrl_map(bvm *vm);
extern int lvbe_btnmatrix_set_focused_btn(bvm *vm);
extern int lvbe_btnmatrix_set_recolor(bvm *vm);
extern int lvbe_btnmatrix_set_btn_ctrl(bvm *vm);
extern int lvbe_btnmatrix_clear_btn_ctrl(bvm *vm);
extern int lvbe_btnmatrix_set_btn_ctrl_all(bvm *vm);
extern int lvbe_btnmatrix_clear_btn_ctrl_all(bvm *vm);
extern int lvbe_btnmatrix_set_btn_width(bvm *vm);
extern int lvbe_btnmatrix_set_one_check(bvm *vm);
extern int lvbe_btnmatrix_set_align(bvm *vm);
extern int lvbe_btnmatrix_get_recolor(bvm *vm);
extern int lvbe_btnmatrix_get_active_btn(bvm *vm);
extern int lvbe_btnmatrix_get_active_btn_text(bvm *vm);
extern int lvbe_btnmatrix_get_focused_btn(bvm *vm);
extern int lvbe_btnmatrix_get_btn_text(bvm *vm);
extern int lvbe_btnmatrix_get_btn_ctrl(bvm *vm);
extern int lvbe_btnmatrix_get_one_check(bvm *vm);
extern int lvbe_btnmatrix_get_align(bvm *vm);

/* `lv_calendar` external functions definitions */
extern int lvbe_calendar_create(bvm *vm);
extern int lvbe_calendar_set_today_date(bvm *vm);
extern int lvbe_calendar_set_showed_date(bvm *vm);
extern int lvbe_calendar_set_highlighted_dates(bvm *vm);
extern int lvbe_calendar_set_day_names(bvm *vm);
extern int lvbe_calendar_set_month_names(bvm *vm);
extern int lvbe_calendar_get_highlighted_dates_num(bvm *vm);
extern int lvbe_calendar_get_day_of_week(bvm *vm);

/* `lv_canvas` external functions definitions */
extern int lvbe_canvas_create(bvm *vm);
extern int lvbe_canvas_set_buffer(bvm *vm);
extern int lvbe_canvas_set_px(bvm *vm);
extern int lvbe_canvas_set_palette(bvm *vm);
extern int lvbe_canvas_get_px(bvm *vm);
extern int lvbe_canvas_copy_buf(bvm *vm);
extern int lvbe_canvas_transform(bvm *vm);
extern int lvbe_canvas_blur_hor(bvm *vm);
extern int lvbe_canvas_blur_ver(bvm *vm);
extern int lvbe_canvas_fill_bg(bvm *vm);
extern int lvbe_canvas_draw_rect(bvm *vm);
extern int lvbe_canvas_draw_text(bvm *vm);
extern int lvbe_canvas_draw_img(bvm *vm);
extern int lvbe_canvas_draw_line(bvm *vm);
extern int lvbe_canvas_draw_polygon(bvm *vm);
extern int lvbe_canvas_draw_arc(bvm *vm);

/* `lv_chart` external functions definitions */
extern int lvbe_chart_create(bvm *vm);
extern int lvbe_chart_remove_series(bvm *vm);
extern int lvbe_chart_clear_series(bvm *vm);
extern int lvbe_chart_hide_series(bvm *vm);
extern int lvbe_chart_set_div_line_count(bvm *vm);
extern int lvbe_chart_set_y_range(bvm *vm);
extern int lvbe_chart_set_type(bvm *vm);
extern int lvbe_chart_set_point_count(bvm *vm);
extern int lvbe_chart_init_points(bvm *vm);
extern int lvbe_chart_set_points(bvm *vm);
extern int lvbe_chart_set_next(bvm *vm);
extern int lvbe_chart_set_update_mode(bvm *vm);
extern int lvbe_chart_set_x_tick_length(bvm *vm);
extern int lvbe_chart_set_y_tick_length(bvm *vm);
extern int lvbe_chart_set_secondary_y_tick_length(bvm *vm);
extern int lvbe_chart_set_x_tick_texts(bvm *vm);
extern int lvbe_chart_set_secondary_y_tick_texts(bvm *vm);
extern int lvbe_chart_set_y_tick_texts(bvm *vm);
extern int lvbe_chart_set_x_start_point(bvm *vm);
extern int lvbe_chart_set_ext_array(bvm *vm);
extern int lvbe_chart_set_point_id(bvm *vm);
extern int lvbe_chart_set_series_axis(bvm *vm);
extern int lvbe_chart_set_cursor_point(bvm *vm);
extern int lvbe_chart_get_type(bvm *vm);
extern int lvbe_chart_get_point_count(bvm *vm);
extern int lvbe_chart_get_x_start_point(bvm *vm);
extern int lvbe_chart_get_point_id(bvm *vm);
extern int lvbe_chart_get_series_axis(bvm *vm);
extern int lvbe_chart_get_series_area(bvm *vm);
extern int lvbe_chart_get_cursor_point(bvm *vm);
extern int lvbe_chart_get_nearest_index_from_coord(bvm *vm);
extern int lvbe_chart_get_x_from_index(bvm *vm);
extern int lvbe_chart_get_y_from_index(bvm *vm);
extern int lvbe_chart_refresh(bvm *vm);

/* `lv_checkbox` external functions definitions */
extern int lvbe_checkbox_create(bvm *vm);
extern int lvbe_checkbox_set_text(bvm *vm);
extern int lvbe_checkbox_set_text_static(bvm *vm);
extern int lvbe_checkbox_set_checked(bvm *vm);
extern int lvbe_checkbox_set_disabled(bvm *vm);
extern int lvbe_checkbox_set_state(bvm *vm);
extern int lvbe_checkbox_get_text(bvm *vm);
extern int lvbe_checkbox_is_checked(bvm *vm);
extern int lvbe_checkbox_is_inactive(bvm *vm);
extern int lvbe_checkbox_get_state(bvm *vm);

/* `lv_cont` external functions definitions */
extern int lvbe_cont_create(bvm *vm);
extern int lvbe_cont_set_layout(bvm *vm);
extern int lvbe_cont_set_fit4(bvm *vm);
extern int lvbe_cont_set_fit2(bvm *vm);
extern int lvbe_cont_set_fit(bvm *vm);
extern int lvbe_cont_get_layout(bvm *vm);
extern int lvbe_cont_get_fit_left(bvm *vm);
extern int lvbe_cont_get_fit_right(bvm *vm);
extern int lvbe_cont_get_fit_top(bvm *vm);
extern int lvbe_cont_get_fit_bottom(bvm *vm);

/* `lv_cpicker` external functions definitions */
extern int lvbe_cpicker_create(bvm *vm);
extern int lvbe_cpicker_set_type(bvm *vm);
extern int lvbe_cpicker_set_hue(bvm *vm);
extern int lvbe_cpicker_set_saturation(bvm *vm);
extern int lvbe_cpicker_set_value(bvm *vm);
extern int lvbe_cpicker_set_hsv(bvm *vm);
extern int lvbe_cpicker_set_color(bvm *vm);
extern int lvbe_cpicker_set_color_mode(bvm *vm);
extern int lvbe_cpicker_set_color_mode_fixed(bvm *vm);
extern int lvbe_cpicker_set_knob_colored(bvm *vm);
extern int lvbe_cpicker_get_color_mode(bvm *vm);
extern int lvbe_cpicker_get_color_mode_fixed(bvm *vm);
extern int lvbe_cpicker_get_hue(bvm *vm);
extern int lvbe_cpicker_get_saturation(bvm *vm);
extern int lvbe_cpicker_get_value(bvm *vm);
extern int lvbe_cpicker_get_hsv(bvm *vm);
extern int lvbe_cpicker_get_color(bvm *vm);
extern int lvbe_cpicker_get_knob_colored(bvm *vm);

/* `lv_dropdown` external functions definitions */
extern int lvbe_dropdown_create(bvm *vm);
extern int lvbe_dropdown_set_text(bvm *vm);
extern int lvbe_dropdown_clear_options(bvm *vm);
extern int lvbe_dropdown_set_options(bvm *vm);
extern int lvbe_dropdown_set_options_static(bvm *vm);
extern int lvbe_dropdown_add_option(bvm *vm);
extern int lvbe_dropdown_set_selected(bvm *vm);
extern int lvbe_dropdown_set_dir(bvm *vm);
extern int lvbe_dropdown_set_max_height(bvm *vm);
extern int lvbe_dropdown_set_symbol(bvm *vm);
extern int lvbe_dropdown_set_show_selected(bvm *vm);
extern int lvbe_dropdown_get_text(bvm *vm);
extern int lvbe_dropdown_get_options(bvm *vm);
extern int lvbe_dropdown_get_selected(bvm *vm);
extern int lvbe_dropdown_get_option_cnt(bvm *vm);
extern int lvbe_dropdown_get_selected_str(bvm *vm);
extern int lvbe_dropdown_get_max_height(bvm *vm);
extern int lvbe_dropdown_get_symbol(bvm *vm);
extern int lvbe_dropdown_get_dir(bvm *vm);
extern int lvbe_dropdown_get_show_selected(bvm *vm);
extern int lvbe_dropdown_open(bvm *vm);
extern int lvbe_dropdown_close(bvm *vm);

/* `lv_gauge` external functions definitions */
extern int lvbe_gauge_create(bvm *vm);
extern int lvbe_gauge_set_needle_count(bvm *vm);
extern int lvbe_gauge_set_value(bvm *vm);
extern int lvbe_gauge_set_range(bvm *vm);
extern int lvbe_gauge_set_critical_value(bvm *vm);
extern int lvbe_gauge_set_scale(bvm *vm);
extern int lvbe_gauge_set_angle_offset(bvm *vm);
extern int lvbe_gauge_set_needle_img(bvm *vm);
extern int lvbe_gauge_set_formatter_cb(bvm *vm);
extern int lvbe_gauge_get_value(bvm *vm);
extern int lvbe_gauge_get_needle_count(bvm *vm);
extern int lvbe_gauge_get_min_value(bvm *vm);
extern int lvbe_gauge_get_max_value(bvm *vm);
extern int lvbe_gauge_get_critical_value(bvm *vm);
extern int lvbe_gauge_get_label_count(bvm *vm);
extern int lvbe_gauge_get_line_count(bvm *vm);
extern int lvbe_gauge_get_scale_angle(bvm *vm);
extern int lvbe_gauge_get_angle_offset(bvm *vm);
extern int lvbe_gauge_get_needle_img(bvm *vm);
extern int lvbe_gauge_get_needle_img_pivot_x(bvm *vm);
extern int lvbe_gauge_get_needle_img_pivot_y(bvm *vm);

/* `lv_imgbtn` external functions definitions */
extern int lvbe_imgbtn_create(bvm *vm);
extern int lvbe_imgbtn_set_src(bvm *vm);
extern int lvbe_imgbtn_set_state(bvm *vm);
extern int lvbe_imgbtn_toggle(bvm *vm);
extern int lvbe_imgbtn_set_checkable(bvm *vm);
extern int lvbe_imgbtn_get_src(bvm *vm);
extern int lvbe_imgbtn_get_state(bvm *vm);

/* `lv_keyboard` external functions definitions */
extern int lvbe_keyboard_create(bvm *vm);
extern int lvbe_keyboard_set_textarea(bvm *vm);
extern int lvbe_keyboard_set_mode(bvm *vm);
extern int lvbe_keyboard_set_cursor_manage(bvm *vm);
extern int lvbe_keyboard_set_map(bvm *vm);
extern int lvbe_keyboard_set_ctrl_map(bvm *vm);
extern int lvbe_keyboard_get_textarea(bvm *vm);
extern int lvbe_keyboard_get_cursor_manage(bvm *vm);
extern int lvbe_keyboard_def_event_cb(bvm *vm);

/* `lv_label` external functions definitions */
extern int lvbe_label_create(bvm *vm);
extern int lvbe_label_set_text(bvm *vm);
extern int lvbe_label_set_text_fmt(bvm *vm);
extern int lvbe_label_set_text_static(bvm *vm);
extern int lvbe_label_set_long_mode(bvm *vm);
extern int lvbe_label_set_align(bvm *vm);
extern int lvbe_label_set_recolor(bvm *vm);
extern int lvbe_label_set_anim_speed(bvm *vm);
extern int lvbe_label_set_text_sel_start(bvm *vm);
extern int lvbe_label_set_text_sel_end(bvm *vm);
extern int lvbe_label_get_text(bvm *vm);
extern int lvbe_label_get_long_mode(bvm *vm);
extern int lvbe_label_get_align(bvm *vm);
extern int lvbe_label_get_recolor(bvm *vm);
extern int lvbe_label_get_anim_speed(bvm *vm);
extern int lvbe_label_get_letter_pos(bvm *vm);
extern int lvbe_label_get_letter_on(bvm *vm);
extern int lvbe_label_is_char_under_pos(bvm *vm);
extern int lvbe_label_get_text_sel_start(bvm *vm);
extern int lvbe_label_get_text_sel_end(bvm *vm);
extern int lvbe_label_ins_text(bvm *vm);
extern int lvbe_label_cut_text(bvm *vm);
extern int lvbe_label_refr_text(bvm *vm);

/* `lv_led` external functions definitions */
extern int lvbe_led_create(bvm *vm);
extern int lvbe_led_set_bright(bvm *vm);
extern int lvbe_led_on(bvm *vm);
extern int lvbe_led_off(bvm *vm);
extern int lvbe_led_toggle(bvm *vm);
extern int lvbe_led_get_bright(bvm *vm);

/* `lv_line` external functions definitions */
extern int lvbe_line_create(bvm *vm);
extern int lvbe_line_set_points(bvm *vm);
extern int lvbe_line_set_auto_size(bvm *vm);
extern int lvbe_line_set_y_invert(bvm *vm);
extern int lvbe_line_get_auto_size(bvm *vm);
extern int lvbe_line_get_y_invert(bvm *vm);

/* `lv_linemeter` external functions definitions */
extern int lvbe_linemeter_create(bvm *vm);
extern int lvbe_linemeter_set_value(bvm *vm);
extern int lvbe_linemeter_set_range(bvm *vm);
extern int lvbe_linemeter_set_scale(bvm *vm);
extern int lvbe_linemeter_set_angle_offset(bvm *vm);
extern int lvbe_linemeter_set_mirror(bvm *vm);
extern int lvbe_linemeter_get_value(bvm *vm);
extern int lvbe_linemeter_get_min_value(bvm *vm);
extern int lvbe_linemeter_get_max_value(bvm *vm);
extern int lvbe_linemeter_get_line_count(bvm *vm);
extern int lvbe_linemeter_get_scale_angle(bvm *vm);
extern int lvbe_linemeter_get_angle_offset(bvm *vm);
extern int lvbe_linemeter_draw_scale(bvm *vm);
extern int lvbe_linemeter_get_mirror(bvm *vm);

/* `lv_list` external functions definitions */
extern int lvbe_list_create(bvm *vm);
extern int lvbe_list_clean(bvm *vm);
extern int lvbe_list_add_btn(bvm *vm);
extern int lvbe_list_remove(bvm *vm);
extern int lvbe_list_focus_btn(bvm *vm);
extern int lvbe_list_set_scrollbar_mode(bvm *vm);
extern int lvbe_list_set_scroll_propagation(bvm *vm);
extern int lvbe_list_set_edge_flash(bvm *vm);
extern int lvbe_list_set_anim_time(bvm *vm);
extern int lvbe_list_set_layout(bvm *vm);
extern int lvbe_list_get_btn_text(bvm *vm);
extern int lvbe_list_get_btn_label(bvm *vm);
extern int lvbe_list_get_btn_img(bvm *vm);
extern int lvbe_list_get_prev_btn(bvm *vm);
extern int lvbe_list_get_next_btn(bvm *vm);
extern int lvbe_list_get_btn_index(bvm *vm);
extern int lvbe_list_get_size(bvm *vm);
extern int lvbe_list_get_btn_selected(bvm *vm);
extern int lvbe_list_get_layout(bvm *vm);
extern int lvbe_list_get_scrollbar_mode(bvm *vm);
extern int lvbe_list_get_scroll_propagation(bvm *vm);
extern int lvbe_list_get_edge_flash(bvm *vm);
extern int lvbe_list_get_anim_time(bvm *vm);
extern int lvbe_list_up(bvm *vm);
extern int lvbe_list_down(bvm *vm);
extern int lvbe_list_focus(bvm *vm);

/* `lv_msgbox` external functions definitions */
extern int lvbe_msgbox_create(bvm *vm);
extern int lvbe_msgbox_add_btns(bvm *vm);
extern int lvbe_msgbox_set_text(bvm *vm);
extern int lvbe_msgbox_set_text_fmt(bvm *vm);
extern int lvbe_msgbox_set_anim_time(bvm *vm);
extern int lvbe_msgbox_start_auto_close(bvm *vm);
extern int lvbe_msgbox_stop_auto_close(bvm *vm);
extern int lvbe_msgbox_set_recolor(bvm *vm);
extern int lvbe_msgbox_get_text(bvm *vm);
extern int lvbe_msgbox_get_active_btn(bvm *vm);
extern int lvbe_msgbox_get_active_btn_text(bvm *vm);
extern int lvbe_msgbox_get_anim_time(bvm *vm);
extern int lvbe_msgbox_get_recolor(bvm *vm);
extern int lvbe_msgbox_get_btnmatrix(bvm *vm);

/* `lv_objmask` external functions definitions */
extern int lvbe_objmask_create(bvm *vm);
extern int lvbe_objmask_update_mask(bvm *vm);
extern int lvbe_objmask_remove_mask(bvm *vm);

/* `lv_page` external functions definitions */
extern int lvbe_page_create(bvm *vm);
extern int lvbe_page_clean(bvm *vm);
extern int lvbe_page_get_scrollable(bvm *vm);
extern int lvbe_page_get_anim_time(bvm *vm);
extern int lvbe_page_set_scrollbar_mode(bvm *vm);
extern int lvbe_page_set_anim_time(bvm *vm);
extern int lvbe_page_set_scroll_propagation(bvm *vm);
extern int lvbe_page_set_edge_flash(bvm *vm);
extern int lvbe_page_set_scrollable_fit2(bvm *vm);
extern int lvbe_page_set_scrollable_fit(bvm *vm);
extern int lvbe_page_set_scrl_width(bvm *vm);
extern int lvbe_page_set_scrl_height(bvm *vm);
extern int lvbe_page_set_scrl_layout(bvm *vm);
extern int lvbe_page_get_scrollbar_mode(bvm *vm);
extern int lvbe_page_get_scroll_propagation(bvm *vm);
extern int lvbe_page_get_edge_flash(bvm *vm);
extern int lvbe_page_get_width_fit(bvm *vm);
extern int lvbe_page_get_height_fit(bvm *vm);
extern int lvbe_page_get_width_grid(bvm *vm);
extern int lvbe_page_get_height_grid(bvm *vm);
extern int lvbe_page_get_scrl_width(bvm *vm);
extern int lvbe_page_get_scrl_height(bvm *vm);
extern int lvbe_page_get_scrl_layout(bvm *vm);
extern int lvbe_page_get_scrl_fit_left(bvm *vm);
extern int lvbe_page_get_scrl_fit_right(bvm *vm);
extern int lvbe_page_get_scrl_fit_top(bvm *vm);
extern int lvbe_page_get_scrl_fit_bottom(bvm *vm);
extern int lvbe_page_on_edge(bvm *vm);
extern int lvbe_page_glue_obj(bvm *vm);
extern int lvbe_page_focus(bvm *vm);
extern int lvbe_page_scroll_hor(bvm *vm);
extern int lvbe_page_scroll_ver(bvm *vm);
extern int lvbe_page_start_edge_flash(bvm *vm);

/* `lv_roller` external functions definitions */
extern int lvbe_roller_create(bvm *vm);
extern int lvbe_roller_set_options(bvm *vm);
extern int lvbe_roller_set_align(bvm *vm);
extern int lvbe_roller_set_selected(bvm *vm);
extern int lvbe_roller_set_visible_row_count(bvm *vm);
extern int lvbe_roller_set_auto_fit(bvm *vm);
extern int lvbe_roller_set_anim_time(bvm *vm);
extern int lvbe_roller_get_selected(bvm *vm);
extern int lvbe_roller_get_option_cnt(bvm *vm);
extern int lvbe_roller_get_selected_str(bvm *vm);
extern int lvbe_roller_get_align(bvm *vm);
extern int lvbe_roller_get_auto_fit(bvm *vm);
extern int lvbe_roller_get_options(bvm *vm);
extern int lvbe_roller_get_anim_time(bvm *vm);

/* `lv_slider` external functions definitions */
extern int lvbe_slider_create(bvm *vm);
extern int lvbe_slider_set_value(bvm *vm);
extern int lvbe_slider_set_left_value(bvm *vm);
extern int lvbe_slider_set_range(bvm *vm);
extern int lvbe_slider_set_anim_time(bvm *vm);
extern int lvbe_slider_set_type(bvm *vm);
extern int lvbe_slider_get_value(bvm *vm);
extern int lvbe_slider_get_left_value(bvm *vm);
extern int lvbe_slider_get_min_value(bvm *vm);
extern int lvbe_slider_get_max_value(bvm *vm);
extern int lvbe_slider_is_dragged(bvm *vm);
extern int lvbe_slider_get_anim_time(bvm *vm);
extern int lvbe_slider_get_type(bvm *vm);

/* `lv_spinbox` external functions definitions */
extern int lvbe_spinbox_create(bvm *vm);
extern int lvbe_spinbox_set_rollover(bvm *vm);
extern int lvbe_spinbox_set_value(bvm *vm);
extern int lvbe_spinbox_set_digit_format(bvm *vm);
extern int lvbe_spinbox_set_step(bvm *vm);
extern int lvbe_spinbox_set_range(bvm *vm);
extern int lvbe_spinbox_set_padding_left(bvm *vm);
extern int lvbe_spinbox_get_rollover(bvm *vm);
extern int lvbe_spinbox_get_value(bvm *vm);
extern int lvbe_spinbox_get_step(bvm *vm);
extern int lvbe_spinbox_step_next(bvm *vm);
extern int lvbe_spinbox_step_prev(bvm *vm);
extern int lvbe_spinbox_increment(bvm *vm);
extern int lvbe_spinbox_decrement(bvm *vm);

/* `lv_spinner` external functions definitions */
extern int lvbe_spinner_create(bvm *vm);
extern int lvbe_spinner_set_arc_length(bvm *vm);
extern int lvbe_spinner_set_spin_time(bvm *vm);
extern int lvbe_spinner_set_type(bvm *vm);
extern int lvbe_spinner_set_dir(bvm *vm);
extern int lvbe_spinner_get_arc_length(bvm *vm);
extern int lvbe_spinner_get_spin_time(bvm *vm);
extern int lvbe_spinner_get_type(bvm *vm);
extern int lvbe_spinner_get_dir(bvm *vm);
extern int lvbe_spinner_anim_cb(bvm *vm);

/* `lv_switch` external functions definitions */
extern int lvbe_switch_create(bvm *vm);
extern int lvbe_switch_on(bvm *vm);
extern int lvbe_switch_off(bvm *vm);
extern int lvbe_switch_toggle(bvm *vm);
extern int lvbe_switch_set_anim_time(bvm *vm);
extern int lvbe_switch_get_state(bvm *vm);
extern int lvbe_switch_get_anim_time(bvm *vm);

/* `lv_table` external functions definitions */
extern int lvbe_table_create(bvm *vm);
extern int lvbe_table_set_cell_value(bvm *vm);
extern int lvbe_table_set_cell_value_fmt(bvm *vm);
extern int lvbe_table_set_row_cnt(bvm *vm);
extern int lvbe_table_set_col_cnt(bvm *vm);
extern int lvbe_table_set_col_width(bvm *vm);
extern int lvbe_table_set_cell_align(bvm *vm);
extern int lvbe_table_set_cell_type(bvm *vm);
extern int lvbe_table_set_cell_crop(bvm *vm);
extern int lvbe_table_set_cell_merge_right(bvm *vm);
extern int lvbe_table_get_cell_value(bvm *vm);
extern int lvbe_table_get_row_cnt(bvm *vm);
extern int lvbe_table_get_col_cnt(bvm *vm);
extern int lvbe_table_get_col_width(bvm *vm);
extern int lvbe_table_get_cell_align(bvm *vm);
extern int lvbe_table_get_cell_type(bvm *vm);
extern int lvbe_table_get_cell_crop(bvm *vm);
extern int lvbe_table_get_cell_merge_right(bvm *vm);
extern int lvbe_table_get_pressed_cell(bvm *vm);

/* `lv_tabview` external functions definitions */
extern int lvbe_tabview_create(bvm *vm);
extern int lvbe_tabview_add_tab(bvm *vm);
extern int lvbe_tabview_clean_tab(bvm *vm);
extern int lvbe_tabview_set_tab_act(bvm *vm);
extern int lvbe_tabview_set_tab_name(bvm *vm);
extern int lvbe_tabview_set_anim_time(bvm *vm);
extern int lvbe_tabview_set_btns_pos(bvm *vm);
extern int lvbe_tabview_get_tab_act(bvm *vm);
extern int lvbe_tabview_get_tab_count(bvm *vm);
extern int lvbe_tabview_get_tab(bvm *vm);
extern int lvbe_tabview_get_anim_time(bvm *vm);
extern int lvbe_tabview_get_btns_pos(bvm *vm);

/* `lv_textarea` external functions definitions */
extern int lvbe_textarea_create(bvm *vm);
extern int lvbe_textarea_add_char(bvm *vm);
extern int lvbe_textarea_add_text(bvm *vm);
extern int lvbe_textarea_del_char(bvm *vm);
extern int lvbe_textarea_del_char_forward(bvm *vm);
extern int lvbe_textarea_set_text(bvm *vm);
extern int lvbe_textarea_set_placeholder_text(bvm *vm);
extern int lvbe_textarea_set_cursor_pos(bvm *vm);
extern int lvbe_textarea_set_cursor_hidden(bvm *vm);
extern int lvbe_textarea_set_cursor_click_pos(bvm *vm);
extern int lvbe_textarea_set_pwd_mode(bvm *vm);
extern int lvbe_textarea_set_one_line(bvm *vm);
extern int lvbe_textarea_set_text_align(bvm *vm);
extern int lvbe_textarea_set_accepted_chars(bvm *vm);
extern int lvbe_textarea_set_max_length(bvm *vm);
extern int lvbe_textarea_set_insert_replace(bvm *vm);
extern int lvbe_textarea_set_scrollbar_mode(bvm *vm);
extern int lvbe_textarea_set_scroll_propagation(bvm *vm);
extern int lvbe_textarea_set_edge_flash(bvm *vm);
extern int lvbe_textarea_set_text_sel(bvm *vm);
extern int lvbe_textarea_set_pwd_show_time(bvm *vm);
extern int lvbe_textarea_set_cursor_blink_time(bvm *vm);
extern int lvbe_textarea_get_text(bvm *vm);
extern int lvbe_textarea_get_placeholder_text(bvm *vm);
extern int lvbe_textarea_get_label(bvm *vm);
extern int lvbe_textarea_get_cursor_pos(bvm *vm);
extern int lvbe_textarea_get_cursor_hidden(bvm *vm);
extern int lvbe_textarea_get_cursor_click_pos(bvm *vm);
extern int lvbe_textarea_get_pwd_mode(bvm *vm);
extern int lvbe_textarea_get_one_line(bvm *vm);
extern int lvbe_textarea_get_accepted_chars(bvm *vm);
extern int lvbe_textarea_get_max_length(bvm *vm);
extern int lvbe_textarea_get_scrollbar_mode(bvm *vm);
extern int lvbe_textarea_get_scroll_propagation(bvm *vm);
extern int lvbe_textarea_get_edge_flash(bvm *vm);
extern int lvbe_textarea_text_is_selected(bvm *vm);
extern int lvbe_textarea_get_text_sel_en(bvm *vm);
extern int lvbe_textarea_get_pwd_show_time(bvm *vm);
extern int lvbe_textarea_get_cursor_blink_time(bvm *vm);
extern int lvbe_textarea_clear_selection(bvm *vm);
extern int lvbe_textarea_cursor_right(bvm *vm);
extern int lvbe_textarea_cursor_left(bvm *vm);
extern int lvbe_textarea_cursor_down(bvm *vm);
extern int lvbe_textarea_cursor_up(bvm *vm);

/* `lv_tileview` external functions definitions */
extern int lvbe_tileview_create(bvm *vm);
extern int lvbe_tileview_add_element(bvm *vm);
extern int lvbe_tileview_set_valid_positions(bvm *vm);
extern int lvbe_tileview_set_tile_act(bvm *vm);
extern int lvbe_tileview_set_edge_flash(bvm *vm);
extern int lvbe_tileview_set_anim_time(bvm *vm);
extern int lvbe_tileview_get_tile_act(bvm *vm);
extern int lvbe_tileview_get_edge_flash(bvm *vm);
extern int lvbe_tileview_get_anim_time(bvm *vm);

/* `lv_win` external functions definitions */
extern int lvbe_win_create(bvm *vm);
extern int lvbe_win_clean(bvm *vm);
extern int lvbe_win_add_btn_right(bvm *vm);
extern int lvbe_win_add_btn_left(bvm *vm);
extern int lvbe_win_close_event_cb(bvm *vm);
extern int lvbe_win_set_title(bvm *vm);
extern int lvbe_win_set_header_height(bvm *vm);
extern int lvbe_win_set_btn_width(bvm *vm);
extern int lvbe_win_set_content_size(bvm *vm);
extern int lvbe_win_set_layout(bvm *vm);
extern int lvbe_win_set_scrollbar_mode(bvm *vm);
extern int lvbe_win_set_anim_time(bvm *vm);
extern int lvbe_win_set_drag(bvm *vm);
extern int lvbe_win_title_set_alignment(bvm *vm);
extern int lvbe_win_get_title(bvm *vm);
extern int lvbe_win_get_content(bvm *vm);
extern int lvbe_win_get_header_height(bvm *vm);
extern int lvbe_win_get_btn_width(bvm *vm);
extern int lvbe_win_get_from_btn(bvm *vm);
extern int lvbe_win_get_layout(bvm *vm);
extern int lvbe_win_get_sb_mode(bvm *vm);
extern int lvbe_win_get_anim_time(bvm *vm);
extern int lvbe_win_get_width(bvm *vm);
extern int lvbe_win_get_drag(bvm *vm);
extern int lvbe_win_title_get_alignment(bvm *vm);
extern int lvbe_win_focus(bvm *vm);
extern int lvbe_win_scroll_hor(bvm *vm);
extern int lvbe_win_scroll_ver(bvm *vm);


#if BE_USE_PRECOMPILED_OBJECT

#include "../generate/be_fixed_be_class_lv_img.h"
#include "../generate/be_fixed_be_class_lv_style.h"
#include "../generate/be_fixed_be_class_lv_group.h"
#include "../generate/be_fixed_be_class_lv_obj.h"
#include "../generate/be_fixed_be_class_lv_arc.h"
#include "../generate/be_fixed_be_class_lv_bar.h"
#include "../generate/be_fixed_be_class_lv_btn.h"
#include "../generate/be_fixed_be_class_lv_btnmatrix.h"
#include "../generate/be_fixed_be_class_lv_calendar.h"
#include "../generate/be_fixed_be_class_lv_canvas.h"
#include "../generate/be_fixed_be_class_lv_chart.h"
#include "../generate/be_fixed_be_class_lv_checkbox.h"
#include "../generate/be_fixed_be_class_lv_cont.h"
#include "../generate/be_fixed_be_class_lv_cpicker.h"
#include "../generate/be_fixed_be_class_lv_dropdown.h"
#include "../generate/be_fixed_be_class_lv_gauge.h"
#include "../generate/be_fixed_be_class_lv_imgbtn.h"
#include "../generate/be_fixed_be_class_lv_keyboard.h"
#include "../generate/be_fixed_be_class_lv_label.h"
#include "../generate/be_fixed_be_class_lv_led.h"
#include "../generate/be_fixed_be_class_lv_line.h"
#include "../generate/be_fixed_be_class_lv_linemeter.h"
#include "../generate/be_fixed_be_class_lv_list.h"
#include "../generate/be_fixed_be_class_lv_msgbox.h"
#include "../generate/be_fixed_be_class_lv_objmask.h"
#include "../generate/be_fixed_be_class_lv_page.h"
#include "../generate/be_fixed_be_class_lv_roller.h"
#include "../generate/be_fixed_be_class_lv_slider.h"
#include "../generate/be_fixed_be_class_lv_spinbox.h"
#include "../generate/be_fixed_be_class_lv_spinner.h"
#include "../generate/be_fixed_be_class_lv_switch.h"
#include "../generate/be_fixed_be_class_lv_table.h"
#include "../generate/be_fixed_be_class_lv_tabview.h"
#include "../generate/be_fixed_be_class_lv_textarea.h"
#include "../generate/be_fixed_be_class_lv_tileview.h"
#include "../generate/be_fixed_be_class_lv_win.h"

#endif


void be_load_lv_img_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_img_create },
    { "tostring", lvx_tostring },

    { "set_tasmota_logo", lvbe_img_set_tasmota_logo },
    { "create", lvbe_img_create },
    { "set_src", lvbe_img_set_src },
    { "set_auto_size", lvbe_img_set_auto_size },
    { "set_offset_x", lvbe_img_set_offset_x },
    { "set_offset_y", lvbe_img_set_offset_y },
    { "set_pivot", lvbe_img_set_pivot },
    { "set_angle", lvbe_img_set_angle },
    { "set_zoom", lvbe_img_set_zoom },
    { "set_antialias", lvbe_img_set_antialias },
    { "get_src", lvbe_img_get_src },
    { "get_file_name", lvbe_img_get_file_name },
    { "get_auto_size", lvbe_img_get_auto_size },
    { "get_offset_x", lvbe_img_get_offset_x },
    { "get_offset_y", lvbe_img_get_offset_y },
    { "get_angle", lvbe_img_get_angle },
    { "get_pivot", lvbe_img_get_pivot },
    { "get_zoom", lvbe_img_get_zoom },
    { "get_antialias", lvbe_img_get_antialias },

    // { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */

    { NULL, NULL }
  };
  be_regclass(vm, "lv_img", members);

  be_getglobal(vm, "lv_img");
  be_getglobal(vm, "lv_obj");
  be_setsuper(vm, -2);
  be_pop(vm, 2);
#else
    be_pushntvclass(vm, &be_class_lv_img);
    be_setglobal(vm, "lv_img");
    be_pop(vm, 1);
#endif
};

/* @const_object_info_begin
class be_class_lv_img (scope: global, name: lv_img, super: be_class_lv_obj) {
    .p, var
    init, func(lvbe_img_create)
    tostring, func(lvx_tostring)
    set_tasmota_logo, func(lvbe_img_set_tasmota_logo)
    create, func(lvbe_img_create)
    set_src, func(lvbe_img_set_src)
    set_auto_size, func(lvbe_img_set_auto_size)
    set_offset_x, func(lvbe_img_set_offset_x)
    set_offset_y, func(lvbe_img_set_offset_y)
    set_pivot, func(lvbe_img_set_pivot)
    set_angle, func(lvbe_img_set_angle)
    set_zoom, func(lvbe_img_set_zoom)
    set_antialias, func(lvbe_img_set_antialias)
    get_src, func(lvbe_img_get_src)
    get_file_name, func(lvbe_img_get_file_name)
    get_auto_size, func(lvbe_img_get_auto_size)
    get_offset_x, func(lvbe_img_get_offset_x)
    get_offset_y, func(lvbe_img_get_offset_y)
    get_angle, func(lvbe_img_get_angle)
    get_pivot, func(lvbe_img_get_pivot)
    get_zoom, func(lvbe_img_get_zoom)
    get_antialias, func(lvbe_img_get_antialias)
}
@const_object_info_end */

void be_load_lv_style_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvs_init },
    { "tostring", lvs_tostring },

    { "copy", lvbe_style_copy },
    { "list_init", lvbe_style_list_init },
    { "list_copy", lvbe_style_list_copy },
    { "list_get_style", lvbe_style_list_get_style },
    { "reset", lvbe_style_reset },
    { "remove_prop", lvbe_style_remove_prop },
    { "list_get_local_style", lvbe_style_list_get_local_style },
    { "set_radius", lvbe_style_set_radius },
    { "set_clip_corner", lvbe_style_set_clip_corner },
    { "set_size", lvbe_style_set_size },
    { "set_transform_width", lvbe_style_set_transform_width },
    { "set_transform_height", lvbe_style_set_transform_height },
    { "set_transform_angle", lvbe_style_set_transform_angle },
    { "set_transform_zoom", lvbe_style_set_transform_zoom },
    { "set_opa_scale", lvbe_style_set_opa_scale },
    { "set_pad_top", lvbe_style_set_pad_top },
    { "set_pad_bottom", lvbe_style_set_pad_bottom },
    { "set_pad_left", lvbe_style_set_pad_left },
    { "set_pad_right", lvbe_style_set_pad_right },
    { "set_pad_inner", lvbe_style_set_pad_inner },
    { "set_margin_top", lvbe_style_set_margin_top },
    { "set_margin_bottom", lvbe_style_set_margin_bottom },
    { "set_margin_left", lvbe_style_set_margin_left },
    { "set_margin_right", lvbe_style_set_margin_right },
    { "set_bg_blend_mode", lvbe_style_set_bg_blend_mode },
    { "set_bg_main_stop", lvbe_style_set_bg_main_stop },
    { "set_bg_grad_stop", lvbe_style_set_bg_grad_stop },
    { "set_bg_grad_dir", lvbe_style_set_bg_grad_dir },
    { "set_bg_color", lvbe_style_set_bg_color },
    { "set_bg_grad_color", lvbe_style_set_bg_grad_color },
    { "set_bg_opa", lvbe_style_set_bg_opa },
    { "set_border_width", lvbe_style_set_border_width },
    { "set_border_side", lvbe_style_set_border_side },
    { "set_border_blend_mode", lvbe_style_set_border_blend_mode },
    { "set_border_post", lvbe_style_set_border_post },
    { "set_border_color", lvbe_style_set_border_color },
    { "set_border_opa", lvbe_style_set_border_opa },
    { "set_outline_width", lvbe_style_set_outline_width },
    { "set_outline_pad", lvbe_style_set_outline_pad },
    { "set_outline_blend_mode", lvbe_style_set_outline_blend_mode },
    { "set_outline_color", lvbe_style_set_outline_color },
    { "set_outline_opa", lvbe_style_set_outline_opa },
    { "set_shadow_width", lvbe_style_set_shadow_width },
    { "set_shadow_ofs_x", lvbe_style_set_shadow_ofs_x },
    { "set_shadow_ofs_y", lvbe_style_set_shadow_ofs_y },
    { "set_shadow_spread", lvbe_style_set_shadow_spread },
    { "set_shadow_blend_mode", lvbe_style_set_shadow_blend_mode },
    { "set_shadow_color", lvbe_style_set_shadow_color },
    { "set_shadow_opa", lvbe_style_set_shadow_opa },
    { "set_pattern_repeat", lvbe_style_set_pattern_repeat },
    { "set_pattern_blend_mode", lvbe_style_set_pattern_blend_mode },
    { "set_pattern_recolor", lvbe_style_set_pattern_recolor },
    { "set_pattern_opa", lvbe_style_set_pattern_opa },
    { "set_pattern_recolor_opa", lvbe_style_set_pattern_recolor_opa },
    { "set_pattern_image", lvbe_style_set_pattern_image },
    { "set_value_letter_space", lvbe_style_set_value_letter_space },
    { "set_value_line_space", lvbe_style_set_value_line_space },
    { "set_value_blend_mode", lvbe_style_set_value_blend_mode },
    { "set_value_ofs_x", lvbe_style_set_value_ofs_x },
    { "set_value_ofs_y", lvbe_style_set_value_ofs_y },
    { "set_value_align", lvbe_style_set_value_align },
    { "set_value_color", lvbe_style_set_value_color },
    { "set_value_opa", lvbe_style_set_value_opa },
    { "set_value_font", lvbe_style_set_value_font },
    { "set_value_str", lvbe_style_set_value_str },
    { "set_text_letter_space", lvbe_style_set_text_letter_space },
    { "set_text_line_space", lvbe_style_set_text_line_space },
    { "set_text_decor", lvbe_style_set_text_decor },
    { "set_text_blend_mode", lvbe_style_set_text_blend_mode },
    { "set_text_color", lvbe_style_set_text_color },
    { "set_text_sel_color", lvbe_style_set_text_sel_color },
    { "set_text_sel_bg_color", lvbe_style_set_text_sel_bg_color },
    { "set_text_opa", lvbe_style_set_text_opa },
    { "set_text_font", lvbe_style_set_text_font },
    { "set_line_width", lvbe_style_set_line_width },
    { "set_line_blend_mode", lvbe_style_set_line_blend_mode },
    { "set_line_dash_width", lvbe_style_set_line_dash_width },
    { "set_line_dash_gap", lvbe_style_set_line_dash_gap },
    { "set_line_rounded", lvbe_style_set_line_rounded },
    { "set_line_color", lvbe_style_set_line_color },
    { "set_line_opa", lvbe_style_set_line_opa },
    { "set_image_blend_mode", lvbe_style_set_image_blend_mode },
    { "set_image_recolor", lvbe_style_set_image_recolor },
    { "set_image_opa", lvbe_style_set_image_opa },
    { "set_image_recolor_opa", lvbe_style_set_image_recolor_opa },
    { "set_transition_time", lvbe_style_set_transition_time },
    { "set_transition_delay", lvbe_style_set_transition_delay },
    { "set_transition_prop_1", lvbe_style_set_transition_prop_1 },
    { "set_transition_prop_2", lvbe_style_set_transition_prop_2 },
    { "set_transition_prop_3", lvbe_style_set_transition_prop_3 },
    { "set_transition_prop_4", lvbe_style_set_transition_prop_4 },
    { "set_transition_prop_5", lvbe_style_set_transition_prop_5 },
    { "set_transition_prop_6", lvbe_style_set_transition_prop_6 },
    { "set_transition_path", lvbe_style_set_transition_path },
    { "set_scale_width", lvbe_style_set_scale_width },
    { "set_scale_border_width", lvbe_style_set_scale_border_width },
    { "set_scale_end_border_width", lvbe_style_set_scale_end_border_width },
    { "set_scale_end_line_width", lvbe_style_set_scale_end_line_width },
    { "set_scale_grad_color", lvbe_style_set_scale_grad_color },
    { "set_scale_end_color", lvbe_style_set_scale_end_color },

    // { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */

    { NULL, NULL }
  };
  be_regclass(vm, "lv_style", members);

  be_getglobal(vm, "lv_style");
  be_getglobal(vm, "lv_obj");
  be_setsuper(vm, -2);
  be_pop(vm, 2);
#else
    be_pushntvclass(vm, &be_class_lv_style);
    be_setglobal(vm, "lv_style");
    be_pop(vm, 1);
#endif
};

/* @const_object_info_begin
class be_class_lv_style (scope: global, name: lv_style, super: be_class_lv_obj) {
    .p, var
    init, func(lvs_init)
    tostring, func(lvs_tostring)
    copy, func(lvbe_style_copy)
    list_init, func(lvbe_style_list_init)
    list_copy, func(lvbe_style_list_copy)
    list_get_style, func(lvbe_style_list_get_style)
    reset, func(lvbe_style_reset)
    remove_prop, func(lvbe_style_remove_prop)
    list_get_local_style, func(lvbe_style_list_get_local_style)
    set_radius, func(lvbe_style_set_radius)
    set_clip_corner, func(lvbe_style_set_clip_corner)
    set_size, func(lvbe_style_set_size)
    set_transform_width, func(lvbe_style_set_transform_width)
    set_transform_height, func(lvbe_style_set_transform_height)
    set_transform_angle, func(lvbe_style_set_transform_angle)
    set_transform_zoom, func(lvbe_style_set_transform_zoom)
    set_opa_scale, func(lvbe_style_set_opa_scale)
    set_pad_top, func(lvbe_style_set_pad_top)
    set_pad_bottom, func(lvbe_style_set_pad_bottom)
    set_pad_left, func(lvbe_style_set_pad_left)
    set_pad_right, func(lvbe_style_set_pad_right)
    set_pad_inner, func(lvbe_style_set_pad_inner)
    set_margin_top, func(lvbe_style_set_margin_top)
    set_margin_bottom, func(lvbe_style_set_margin_bottom)
    set_margin_left, func(lvbe_style_set_margin_left)
    set_margin_right, func(lvbe_style_set_margin_right)
    set_bg_blend_mode, func(lvbe_style_set_bg_blend_mode)
    set_bg_main_stop, func(lvbe_style_set_bg_main_stop)
    set_bg_grad_stop, func(lvbe_style_set_bg_grad_stop)
    set_bg_grad_dir, func(lvbe_style_set_bg_grad_dir)
    set_bg_color, func(lvbe_style_set_bg_color)
    set_bg_grad_color, func(lvbe_style_set_bg_grad_color)
    set_bg_opa, func(lvbe_style_set_bg_opa)
    set_border_width, func(lvbe_style_set_border_width)
    set_border_side, func(lvbe_style_set_border_side)
    set_border_blend_mode, func(lvbe_style_set_border_blend_mode)
    set_border_post, func(lvbe_style_set_border_post)
    set_border_color, func(lvbe_style_set_border_color)
    set_border_opa, func(lvbe_style_set_border_opa)
    set_outline_width, func(lvbe_style_set_outline_width)
    set_outline_pad, func(lvbe_style_set_outline_pad)
    set_outline_blend_mode, func(lvbe_style_set_outline_blend_mode)
    set_outline_color, func(lvbe_style_set_outline_color)
    set_outline_opa, func(lvbe_style_set_outline_opa)
    set_shadow_width, func(lvbe_style_set_shadow_width)
    set_shadow_ofs_x, func(lvbe_style_set_shadow_ofs_x)
    set_shadow_ofs_y, func(lvbe_style_set_shadow_ofs_y)
    set_shadow_spread, func(lvbe_style_set_shadow_spread)
    set_shadow_blend_mode, func(lvbe_style_set_shadow_blend_mode)
    set_shadow_color, func(lvbe_style_set_shadow_color)
    set_shadow_opa, func(lvbe_style_set_shadow_opa)
    set_pattern_repeat, func(lvbe_style_set_pattern_repeat)
    set_pattern_blend_mode, func(lvbe_style_set_pattern_blend_mode)
    set_pattern_recolor, func(lvbe_style_set_pattern_recolor)
    set_pattern_opa, func(lvbe_style_set_pattern_opa)
    set_pattern_recolor_opa, func(lvbe_style_set_pattern_recolor_opa)
    set_pattern_image, func(lvbe_style_set_pattern_image)
    set_value_letter_space, func(lvbe_style_set_value_letter_space)
    set_value_line_space, func(lvbe_style_set_value_line_space)
    set_value_blend_mode, func(lvbe_style_set_value_blend_mode)
    set_value_ofs_x, func(lvbe_style_set_value_ofs_x)
    set_value_ofs_y, func(lvbe_style_set_value_ofs_y)
    set_value_align, func(lvbe_style_set_value_align)
    set_value_color, func(lvbe_style_set_value_color)
    set_value_opa, func(lvbe_style_set_value_opa)
    set_value_font, func(lvbe_style_set_value_font)
    set_value_str, func(lvbe_style_set_value_str)
    set_text_letter_space, func(lvbe_style_set_text_letter_space)
    set_text_line_space, func(lvbe_style_set_text_line_space)
    set_text_decor, func(lvbe_style_set_text_decor)
    set_text_blend_mode, func(lvbe_style_set_text_blend_mode)
    set_text_color, func(lvbe_style_set_text_color)
    set_text_sel_color, func(lvbe_style_set_text_sel_color)
    set_text_sel_bg_color, func(lvbe_style_set_text_sel_bg_color)
    set_text_opa, func(lvbe_style_set_text_opa)
    set_text_font, func(lvbe_style_set_text_font)
    set_line_width, func(lvbe_style_set_line_width)
    set_line_blend_mode, func(lvbe_style_set_line_blend_mode)
    set_line_dash_width, func(lvbe_style_set_line_dash_width)
    set_line_dash_gap, func(lvbe_style_set_line_dash_gap)
    set_line_rounded, func(lvbe_style_set_line_rounded)
    set_line_color, func(lvbe_style_set_line_color)
    set_line_opa, func(lvbe_style_set_line_opa)
    set_image_blend_mode, func(lvbe_style_set_image_blend_mode)
    set_image_recolor, func(lvbe_style_set_image_recolor)
    set_image_opa, func(lvbe_style_set_image_opa)
    set_image_recolor_opa, func(lvbe_style_set_image_recolor_opa)
    set_transition_time, func(lvbe_style_set_transition_time)
    set_transition_delay, func(lvbe_style_set_transition_delay)
    set_transition_prop_1, func(lvbe_style_set_transition_prop_1)
    set_transition_prop_2, func(lvbe_style_set_transition_prop_2)
    set_transition_prop_3, func(lvbe_style_set_transition_prop_3)
    set_transition_prop_4, func(lvbe_style_set_transition_prop_4)
    set_transition_prop_5, func(lvbe_style_set_transition_prop_5)
    set_transition_prop_6, func(lvbe_style_set_transition_prop_6)
    set_transition_path, func(lvbe_style_set_transition_path)
    set_scale_width, func(lvbe_style_set_scale_width)
    set_scale_border_width, func(lvbe_style_set_scale_border_width)
    set_scale_end_border_width, func(lvbe_style_set_scale_end_border_width)
    set_scale_end_line_width, func(lvbe_style_set_scale_end_line_width)
    set_scale_grad_color, func(lvbe_style_set_scale_grad_color)
    set_scale_end_color, func(lvbe_style_set_scale_end_color)
}
@const_object_info_end */

void be_load_lv_group_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_group_create },
    { "tostring", lvx_tostring },

    { "create", lvbe_group_create },
    { "del", lvbe_group_del },
    { "add_obj", lvbe_group_add_obj },
    { "remove_obj", lvbe_group_remove_obj },
    { "remove_all_objs", lvbe_group_remove_all_objs },
    { "focus_obj", lvbe_group_focus_obj },
    { "focus_next", lvbe_group_focus_next },
    { "focus_prev", lvbe_group_focus_prev },
    { "focus_freeze", lvbe_group_focus_freeze },
    { "send_data", lvbe_group_send_data },
    { "set_focus_cb", lvbe_group_set_focus_cb },
    { "set_refocus_policy", lvbe_group_set_refocus_policy },
    { "set_editing", lvbe_group_set_editing },
    { "set_click_focus", lvbe_group_set_click_focus },
    { "set_wrap", lvbe_group_set_wrap },
    { "get_focused", lvbe_group_get_focused },
    { "get_focus_cb", lvbe_group_get_focus_cb },
    { "get_editing", lvbe_group_get_editing },
    { "get_click_focus", lvbe_group_get_click_focus },
    { "get_wrap", lvbe_group_get_wrap },

    // { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */

    { NULL, NULL }
  };
  be_regclass(vm, "lv_group", members);
#else
    be_pushntvclass(vm, &be_class_lv_group);
    be_setglobal(vm, "lv_group");
    be_pop(vm, 1);
#endif
};

/* @const_object_info_begin
class be_class_lv_group (scope: global, name: lv_group) {
    .p, var
    init, func(lvbe_group_create)
    tostring, func(lvx_tostring)
    create, func(lvbe_group_create)
    del, func(lvbe_group_del)
    add_obj, func(lvbe_group_add_obj)
    remove_obj, func(lvbe_group_remove_obj)
    remove_all_objs, func(lvbe_group_remove_all_objs)
    focus_obj, func(lvbe_group_focus_obj)
    focus_next, func(lvbe_group_focus_next)
    focus_prev, func(lvbe_group_focus_prev)
    focus_freeze, func(lvbe_group_focus_freeze)
    send_data, func(lvbe_group_send_data)
    set_focus_cb, func(lvbe_group_set_focus_cb)
    set_refocus_policy, func(lvbe_group_set_refocus_policy)
    set_editing, func(lvbe_group_set_editing)
    set_click_focus, func(lvbe_group_set_click_focus)
    set_wrap, func(lvbe_group_set_wrap)
    get_focused, func(lvbe_group_get_focused)
    get_focus_cb, func(lvbe_group_get_focus_cb)
    get_editing, func(lvbe_group_get_editing)
    get_click_focus, func(lvbe_group_get_click_focus)
    get_wrap, func(lvbe_group_get_wrap)
}
@const_object_info_end */

void be_load_lv_obj_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_obj_create },
    { "tostring", lvx_tostring },

    { "create", lvbe_obj_create },
    { "del", lvbe_obj_del },
    { "del_anim_ready_cb", lvbe_obj_del_anim_ready_cb },
    { "del_async", lvbe_obj_del_async },
    { "clean", lvbe_obj_clean },
    { "invalidate_area", lvbe_obj_invalidate_area },
    { "invalidate", lvbe_obj_invalidate },
    { "area_is_visible", lvbe_obj_area_is_visible },
    { "is_visible", lvbe_obj_is_visible },
    { "set_parent", lvbe_obj_set_parent },
    { "move_foreground", lvbe_obj_move_foreground },
    { "move_background", lvbe_obj_move_background },
    { "set_pos", lvbe_obj_set_pos },
    { "set_x", lvbe_obj_set_x },
    { "set_y", lvbe_obj_set_y },
    { "set_size", lvbe_obj_set_size },
    { "set_width", lvbe_obj_set_width },
    { "set_height", lvbe_obj_set_height },
    { "set_width_fit", lvbe_obj_set_width_fit },
    { "set_height_fit", lvbe_obj_set_height_fit },
    { "set_width_margin", lvbe_obj_set_width_margin },
    { "set_height_margin", lvbe_obj_set_height_margin },
    { "align", lvbe_obj_align },
    { "align_x", lvbe_obj_align_x },
    { "align_y", lvbe_obj_align_y },
    { "align_mid", lvbe_obj_align_mid },
    { "align_mid_x", lvbe_obj_align_mid_x },
    { "align_mid_y", lvbe_obj_align_mid_y },
    { "realign", lvbe_obj_realign },
    { "set_auto_realign", lvbe_obj_set_auto_realign },
    { "set_ext_click_area", lvbe_obj_set_ext_click_area },
    { "add_style", lvbe_obj_add_style },
    { "remove_style", lvbe_obj_remove_style },
    { "clean_style_list", lvbe_obj_clean_style_list },
    { "reset_style_list", lvbe_obj_reset_style_list },
    { "refresh_style", lvbe_obj_refresh_style },
    { "report_style_mod", lvbe_obj_report_style_mod },
    { "remove_style_local_prop", lvbe_obj_remove_style_local_prop },
    { "set_hidden", lvbe_obj_set_hidden },
    { "set_adv_hittest", lvbe_obj_set_adv_hittest },
    { "set_click", lvbe_obj_set_click },
    { "set_top", lvbe_obj_set_top },
    { "set_drag", lvbe_obj_set_drag },
    { "set_drag_dir", lvbe_obj_set_drag_dir },
    { "set_drag_throw", lvbe_obj_set_drag_throw },
    { "set_drag_parent", lvbe_obj_set_drag_parent },
    { "set_focus_parent", lvbe_obj_set_focus_parent },
    { "set_gesture_parent", lvbe_obj_set_gesture_parent },
    { "set_parent_event", lvbe_obj_set_parent_event },
    { "set_base_dir", lvbe_obj_set_base_dir },
    { "add_protect", lvbe_obj_add_protect },
    { "clear_protect", lvbe_obj_clear_protect },
    { "set_state", lvbe_obj_set_state },
    { "add_state", lvbe_obj_add_state },
    { "clear_state", lvbe_obj_clear_state },
    { "finish_transitions", lvbe_obj_finish_transitions },
    { "set_event_cb", lvbe_obj_set_event_cb },
    { "set_signal_cb", lvbe_obj_set_signal_cb },
    { "set_design_cb", lvbe_obj_set_design_cb },
    { "allocate_ext_attr", lvbe_obj_allocate_ext_attr },
    { "refresh_ext_draw_pad", lvbe_obj_refresh_ext_draw_pad },
    { "get_screen", lvbe_obj_get_screen },
    { "get_parent", lvbe_obj_get_parent },
    { "get_child", lvbe_obj_get_child },
    { "get_child_back", lvbe_obj_get_child_back },
    { "count_children", lvbe_obj_count_children },
    { "count_children_recursive", lvbe_obj_count_children_recursive },
    { "get_coords", lvbe_obj_get_coords },
    { "get_inner_coords", lvbe_obj_get_inner_coords },
    { "get_x", lvbe_obj_get_x },
    { "get_y", lvbe_obj_get_y },
    { "get_width", lvbe_obj_get_width },
    { "get_height", lvbe_obj_get_height },
    { "get_width_fit", lvbe_obj_get_width_fit },
    { "get_height_fit", lvbe_obj_get_height_fit },
    { "get_height_margin", lvbe_obj_get_height_margin },
    { "get_width_margin", lvbe_obj_get_width_margin },
    { "get_width_grid", lvbe_obj_get_width_grid },
    { "get_height_grid", lvbe_obj_get_height_grid },
    { "get_auto_realign", lvbe_obj_get_auto_realign },
    { "get_ext_click_pad_left", lvbe_obj_get_ext_click_pad_left },
    { "get_ext_click_pad_right", lvbe_obj_get_ext_click_pad_right },
    { "get_ext_click_pad_top", lvbe_obj_get_ext_click_pad_top },
    { "get_ext_click_pad_bottom", lvbe_obj_get_ext_click_pad_bottom },
    { "get_ext_draw_pad", lvbe_obj_get_ext_draw_pad },
    { "get_local_style", lvbe_obj_get_local_style },
    { "get_hidden", lvbe_obj_get_hidden },
    { "get_adv_hittest", lvbe_obj_get_adv_hittest },
    { "get_click", lvbe_obj_get_click },
    { "get_top", lvbe_obj_get_top },
    { "get_drag", lvbe_obj_get_drag },
    { "get_drag_dir", lvbe_obj_get_drag_dir },
    { "get_drag_throw", lvbe_obj_get_drag_throw },
    { "get_drag_parent", lvbe_obj_get_drag_parent },
    { "get_focus_parent", lvbe_obj_get_focus_parent },
    { "get_parent_event", lvbe_obj_get_parent_event },
    { "get_gesture_parent", lvbe_obj_get_gesture_parent },
    { "get_base_dir", lvbe_obj_get_base_dir },
    { "get_protect", lvbe_obj_get_protect },
    { "is_protected", lvbe_obj_is_protected },
    { "get_state", lvbe_obj_get_state },
    { "is_point_on_coords", lvbe_obj_is_point_on_coords },
    { "hittest", lvbe_obj_hittest },
    { "get_ext_attr", lvbe_obj_get_ext_attr },
    { "get_type", lvbe_obj_get_type },
    { "get_user_data", lvbe_obj_get_user_data },
    { "set_user_data", lvbe_obj_set_user_data },
    { "get_group", lvbe_obj_get_group },
    { "is_focused", lvbe_obj_is_focused },
    { "get_focused_obj", lvbe_obj_get_focused_obj },
    { "handle_get_type_signal", lvbe_obj_handle_get_type_signal },
    { "init_draw_rect_dsc", lvbe_obj_init_draw_rect_dsc },
    { "init_draw_label_dsc", lvbe_obj_init_draw_label_dsc },
    { "init_draw_img_dsc", lvbe_obj_init_draw_img_dsc },
    { "init_draw_line_dsc", lvbe_obj_init_draw_line_dsc },
    { "get_draw_rect_ext_pad_size", lvbe_obj_get_draw_rect_ext_pad_size },
    { "fade_in", lvbe_obj_fade_in },
    { "fade_out", lvbe_obj_fade_out },
    { "get_style_radius", lvbe_obj_get_style_radius },
    { "set_style_local_radius", lvbe_obj_set_style_local_radius },
    { "get_style_clip_corner", lvbe_obj_get_style_clip_corner },
    { "set_style_local_clip_corner", lvbe_obj_set_style_local_clip_corner },
    { "get_style_size", lvbe_obj_get_style_size },
    { "set_style_local_size", lvbe_obj_set_style_local_size },
    { "get_style_transform_width", lvbe_obj_get_style_transform_width },
    { "set_style_local_transform_width", lvbe_obj_set_style_local_transform_width },
    { "get_style_transform_height", lvbe_obj_get_style_transform_height },
    { "set_style_local_transform_height", lvbe_obj_set_style_local_transform_height },
    { "get_style_transform_angle", lvbe_obj_get_style_transform_angle },
    { "set_style_local_transform_angle", lvbe_obj_set_style_local_transform_angle },
    { "get_style_transform_zoom", lvbe_obj_get_style_transform_zoom },
    { "set_style_local_transform_zoom", lvbe_obj_set_style_local_transform_zoom },
    { "get_style_opa_scale", lvbe_obj_get_style_opa_scale },
    { "set_style_local_opa_scale", lvbe_obj_set_style_local_opa_scale },
    { "get_style_pad_top", lvbe_obj_get_style_pad_top },
    { "set_style_local_pad_top", lvbe_obj_set_style_local_pad_top },
    { "get_style_pad_bottom", lvbe_obj_get_style_pad_bottom },
    { "set_style_local_pad_bottom", lvbe_obj_set_style_local_pad_bottom },
    { "get_style_pad_left", lvbe_obj_get_style_pad_left },
    { "set_style_local_pad_left", lvbe_obj_set_style_local_pad_left },
    { "get_style_pad_right", lvbe_obj_get_style_pad_right },
    { "set_style_local_pad_right", lvbe_obj_set_style_local_pad_right },
    { "get_style_pad_inner", lvbe_obj_get_style_pad_inner },
    { "set_style_local_pad_inner", lvbe_obj_set_style_local_pad_inner },
    { "get_style_margin_top", lvbe_obj_get_style_margin_top },
    { "set_style_local_margin_top", lvbe_obj_set_style_local_margin_top },
    { "get_style_margin_bottom", lvbe_obj_get_style_margin_bottom },
    { "set_style_local_margin_bottom", lvbe_obj_set_style_local_margin_bottom },
    { "get_style_margin_left", lvbe_obj_get_style_margin_left },
    { "set_style_local_margin_left", lvbe_obj_set_style_local_margin_left },
    { "get_style_margin_right", lvbe_obj_get_style_margin_right },
    { "set_style_local_margin_right", lvbe_obj_set_style_local_margin_right },
    { "set_style_local_bg_blend_mode", lvbe_obj_set_style_local_bg_blend_mode },
    { "get_style_bg_main_stop", lvbe_obj_get_style_bg_main_stop },
    { "set_style_local_bg_main_stop", lvbe_obj_set_style_local_bg_main_stop },
    { "get_style_bg_grad_stop", lvbe_obj_get_style_bg_grad_stop },
    { "set_style_local_bg_grad_stop", lvbe_obj_set_style_local_bg_grad_stop },
    { "set_style_local_bg_grad_dir", lvbe_obj_set_style_local_bg_grad_dir },
    { "get_style_bg_color", lvbe_obj_get_style_bg_color },
    { "set_style_local_bg_color", lvbe_obj_set_style_local_bg_color },
    { "get_style_bg_grad_color", lvbe_obj_get_style_bg_grad_color },
    { "set_style_local_bg_grad_color", lvbe_obj_set_style_local_bg_grad_color },
    { "get_style_bg_opa", lvbe_obj_get_style_bg_opa },
    { "set_style_local_bg_opa", lvbe_obj_set_style_local_bg_opa },
    { "get_style_border_width", lvbe_obj_get_style_border_width },
    { "set_style_local_border_width", lvbe_obj_set_style_local_border_width },
    { "set_style_local_border_side", lvbe_obj_set_style_local_border_side },
    { "set_style_local_border_blend_mode", lvbe_obj_set_style_local_border_blend_mode },
    { "get_style_border_post", lvbe_obj_get_style_border_post },
    { "set_style_local_border_post", lvbe_obj_set_style_local_border_post },
    { "get_style_border_color", lvbe_obj_get_style_border_color },
    { "set_style_local_border_color", lvbe_obj_set_style_local_border_color },
    { "get_style_border_opa", lvbe_obj_get_style_border_opa },
    { "set_style_local_border_opa", lvbe_obj_set_style_local_border_opa },
    { "get_style_outline_width", lvbe_obj_get_style_outline_width },
    { "set_style_local_outline_width", lvbe_obj_set_style_local_outline_width },
    { "get_style_outline_pad", lvbe_obj_get_style_outline_pad },
    { "set_style_local_outline_pad", lvbe_obj_set_style_local_outline_pad },
    { "set_style_local_outline_blend_mode", lvbe_obj_set_style_local_outline_blend_mode },
    { "get_style_outline_color", lvbe_obj_get_style_outline_color },
    { "set_style_local_outline_color", lvbe_obj_set_style_local_outline_color },
    { "get_style_outline_opa", lvbe_obj_get_style_outline_opa },
    { "set_style_local_outline_opa", lvbe_obj_set_style_local_outline_opa },
    { "get_style_shadow_width", lvbe_obj_get_style_shadow_width },
    { "set_style_local_shadow_width", lvbe_obj_set_style_local_shadow_width },
    { "get_style_shadow_ofs_x", lvbe_obj_get_style_shadow_ofs_x },
    { "set_style_local_shadow_ofs_x", lvbe_obj_set_style_local_shadow_ofs_x },
    { "get_style_shadow_ofs_y", lvbe_obj_get_style_shadow_ofs_y },
    { "set_style_local_shadow_ofs_y", lvbe_obj_set_style_local_shadow_ofs_y },
    { "get_style_shadow_spread", lvbe_obj_get_style_shadow_spread },
    { "set_style_local_shadow_spread", lvbe_obj_set_style_local_shadow_spread },
    { "set_style_local_shadow_blend_mode", lvbe_obj_set_style_local_shadow_blend_mode },
    { "get_style_shadow_color", lvbe_obj_get_style_shadow_color },
    { "set_style_local_shadow_color", lvbe_obj_set_style_local_shadow_color },
    { "get_style_shadow_opa", lvbe_obj_get_style_shadow_opa },
    { "set_style_local_shadow_opa", lvbe_obj_set_style_local_shadow_opa },
    { "get_style_pattern_repeat", lvbe_obj_get_style_pattern_repeat },
    { "set_style_local_pattern_repeat", lvbe_obj_set_style_local_pattern_repeat },
    { "set_style_local_pattern_blend_mode", lvbe_obj_set_style_local_pattern_blend_mode },
    { "get_style_pattern_recolor", lvbe_obj_get_style_pattern_recolor },
    { "set_style_local_pattern_recolor", lvbe_obj_set_style_local_pattern_recolor },
    { "get_style_pattern_opa", lvbe_obj_get_style_pattern_opa },
    { "set_style_local_pattern_opa", lvbe_obj_set_style_local_pattern_opa },
    { "get_style_pattern_recolor_opa", lvbe_obj_get_style_pattern_recolor_opa },
    { "set_style_local_pattern_recolor_opa", lvbe_obj_set_style_local_pattern_recolor_opa },
    { "get_style_pattern_image", lvbe_obj_get_style_pattern_image },
    { "set_style_local_pattern_image", lvbe_obj_set_style_local_pattern_image },
    { "get_style_value_letter_space", lvbe_obj_get_style_value_letter_space },
    { "set_style_local_value_letter_space", lvbe_obj_set_style_local_value_letter_space },
    { "get_style_value_line_space", lvbe_obj_get_style_value_line_space },
    { "set_style_local_value_line_space", lvbe_obj_set_style_local_value_line_space },
    { "set_style_local_value_blend_mode", lvbe_obj_set_style_local_value_blend_mode },
    { "get_style_value_ofs_x", lvbe_obj_get_style_value_ofs_x },
    { "set_style_local_value_ofs_x", lvbe_obj_set_style_local_value_ofs_x },
    { "get_style_value_ofs_y", lvbe_obj_get_style_value_ofs_y },
    { "set_style_local_value_ofs_y", lvbe_obj_set_style_local_value_ofs_y },
    { "set_style_local_value_align", lvbe_obj_set_style_local_value_align },
    { "get_style_value_color", lvbe_obj_get_style_value_color },
    { "set_style_local_value_color", lvbe_obj_set_style_local_value_color },
    { "get_style_value_opa", lvbe_obj_get_style_value_opa },
    { "set_style_local_value_opa", lvbe_obj_set_style_local_value_opa },
    { "set_style_local_value_font", lvbe_obj_set_style_local_value_font },
    { "get_style_value_str", lvbe_obj_get_style_value_str },
    { "set_style_local_value_str", lvbe_obj_set_style_local_value_str },
    { "get_style_text_letter_space", lvbe_obj_get_style_text_letter_space },
    { "set_style_local_text_letter_space", lvbe_obj_set_style_local_text_letter_space },
    { "get_style_text_line_space", lvbe_obj_get_style_text_line_space },
    { "set_style_local_text_line_space", lvbe_obj_set_style_local_text_line_space },
    { "set_style_local_text_decor", lvbe_obj_set_style_local_text_decor },
    { "set_style_local_text_blend_mode", lvbe_obj_set_style_local_text_blend_mode },
    { "get_style_text_color", lvbe_obj_get_style_text_color },
    { "set_style_local_text_color", lvbe_obj_set_style_local_text_color },
    { "get_style_text_sel_color", lvbe_obj_get_style_text_sel_color },
    { "set_style_local_text_sel_color", lvbe_obj_set_style_local_text_sel_color },
    { "get_style_text_sel_bg_color", lvbe_obj_get_style_text_sel_bg_color },
    { "set_style_local_text_sel_bg_color", lvbe_obj_set_style_local_text_sel_bg_color },
    { "get_style_text_opa", lvbe_obj_get_style_text_opa },
    { "set_style_local_text_opa", lvbe_obj_set_style_local_text_opa },
    { "set_style_local_text_font", lvbe_obj_set_style_local_text_font },
    { "get_style_line_width", lvbe_obj_get_style_line_width },
    { "set_style_local_line_width", lvbe_obj_set_style_local_line_width },
    { "set_style_local_line_blend_mode", lvbe_obj_set_style_local_line_blend_mode },
    { "get_style_line_dash_width", lvbe_obj_get_style_line_dash_width },
    { "set_style_local_line_dash_width", lvbe_obj_set_style_local_line_dash_width },
    { "get_style_line_dash_gap", lvbe_obj_get_style_line_dash_gap },
    { "set_style_local_line_dash_gap", lvbe_obj_set_style_local_line_dash_gap },
    { "get_style_line_rounded", lvbe_obj_get_style_line_rounded },
    { "set_style_local_line_rounded", lvbe_obj_set_style_local_line_rounded },
    { "get_style_line_color", lvbe_obj_get_style_line_color },
    { "set_style_local_line_color", lvbe_obj_set_style_local_line_color },
    { "get_style_line_opa", lvbe_obj_get_style_line_opa },
    { "set_style_local_line_opa", lvbe_obj_set_style_local_line_opa },
    { "set_style_local_image_blend_mode", lvbe_obj_set_style_local_image_blend_mode },
    { "get_style_image_recolor", lvbe_obj_get_style_image_recolor },
    { "set_style_local_image_recolor", lvbe_obj_set_style_local_image_recolor },
    { "get_style_image_opa", lvbe_obj_get_style_image_opa },
    { "set_style_local_image_opa", lvbe_obj_set_style_local_image_opa },
    { "get_style_image_recolor_opa", lvbe_obj_get_style_image_recolor_opa },
    { "set_style_local_image_recolor_opa", lvbe_obj_set_style_local_image_recolor_opa },
    { "get_style_transition_time", lvbe_obj_get_style_transition_time },
    { "set_style_local_transition_time", lvbe_obj_set_style_local_transition_time },
    { "get_style_transition_delay", lvbe_obj_get_style_transition_delay },
    { "set_style_local_transition_delay", lvbe_obj_set_style_local_transition_delay },
    { "get_style_transition_prop_1", lvbe_obj_get_style_transition_prop_1 },
    { "set_style_local_transition_prop_1", lvbe_obj_set_style_local_transition_prop_1 },
    { "get_style_transition_prop_2", lvbe_obj_get_style_transition_prop_2 },
    { "set_style_local_transition_prop_2", lvbe_obj_set_style_local_transition_prop_2 },
    { "get_style_transition_prop_3", lvbe_obj_get_style_transition_prop_3 },
    { "set_style_local_transition_prop_3", lvbe_obj_set_style_local_transition_prop_3 },
    { "get_style_transition_prop_4", lvbe_obj_get_style_transition_prop_4 },
    { "set_style_local_transition_prop_4", lvbe_obj_set_style_local_transition_prop_4 },
    { "get_style_transition_prop_5", lvbe_obj_get_style_transition_prop_5 },
    { "set_style_local_transition_prop_5", lvbe_obj_set_style_local_transition_prop_5 },
    { "get_style_transition_prop_6", lvbe_obj_get_style_transition_prop_6 },
    { "set_style_local_transition_prop_6", lvbe_obj_set_style_local_transition_prop_6 },
    { "set_style_local_transition_path", lvbe_obj_set_style_local_transition_path },
    { "get_style_scale_width", lvbe_obj_get_style_scale_width },
    { "set_style_local_scale_width", lvbe_obj_set_style_local_scale_width },
    { "get_style_scale_border_width", lvbe_obj_get_style_scale_border_width },
    { "set_style_local_scale_border_width", lvbe_obj_set_style_local_scale_border_width },
    { "get_style_scale_end_border_width", lvbe_obj_get_style_scale_end_border_width },
    { "set_style_local_scale_end_border_width", lvbe_obj_set_style_local_scale_end_border_width },
    { "get_style_scale_end_line_width", lvbe_obj_get_style_scale_end_line_width },
    { "set_style_local_scale_end_line_width", lvbe_obj_set_style_local_scale_end_line_width },
    { "get_style_scale_grad_color", lvbe_obj_get_style_scale_grad_color },
    { "set_style_local_scale_grad_color", lvbe_obj_set_style_local_scale_grad_color },
    { "get_style_scale_end_color", lvbe_obj_get_style_scale_end_color },
    { "set_style_local_scale_end_color", lvbe_obj_set_style_local_scale_end_color },

    // { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */

    { NULL, NULL }
  };
  be_regclass(vm, "lv_obj", members);
#else
    be_pushntvclass(vm, &be_class_lv_obj);
    be_setglobal(vm, "lv_obj");
    be_pop(vm, 1);
#endif
};

/* @const_object_info_begin
class be_class_lv_obj (scope: global, name: lv_obj) {
    .p, var
    init, func(lvbe_obj_create)
    tostring, func(lvx_tostring)
    create, func(lvbe_obj_create)
    del, func(lvbe_obj_del)
    del_anim_ready_cb, func(lvbe_obj_del_anim_ready_cb)
    del_async, func(lvbe_obj_del_async)
    clean, func(lvbe_obj_clean)
    invalidate_area, func(lvbe_obj_invalidate_area)
    invalidate, func(lvbe_obj_invalidate)
    area_is_visible, func(lvbe_obj_area_is_visible)
    is_visible, func(lvbe_obj_is_visible)
    set_parent, func(lvbe_obj_set_parent)
    move_foreground, func(lvbe_obj_move_foreground)
    move_background, func(lvbe_obj_move_background)
    set_pos, func(lvbe_obj_set_pos)
    set_x, func(lvbe_obj_set_x)
    set_y, func(lvbe_obj_set_y)
    set_size, func(lvbe_obj_set_size)
    set_width, func(lvbe_obj_set_width)
    set_height, func(lvbe_obj_set_height)
    set_width_fit, func(lvbe_obj_set_width_fit)
    set_height_fit, func(lvbe_obj_set_height_fit)
    set_width_margin, func(lvbe_obj_set_width_margin)
    set_height_margin, func(lvbe_obj_set_height_margin)
    align, func(lvbe_obj_align)
    align_x, func(lvbe_obj_align_x)
    align_y, func(lvbe_obj_align_y)
    align_mid, func(lvbe_obj_align_mid)
    align_mid_x, func(lvbe_obj_align_mid_x)
    align_mid_y, func(lvbe_obj_align_mid_y)
    realign, func(lvbe_obj_realign)
    set_auto_realign, func(lvbe_obj_set_auto_realign)
    set_ext_click_area, func(lvbe_obj_set_ext_click_area)
    add_style, func(lvbe_obj_add_style)
    remove_style, func(lvbe_obj_remove_style)
    clean_style_list, func(lvbe_obj_clean_style_list)
    reset_style_list, func(lvbe_obj_reset_style_list)
    refresh_style, func(lvbe_obj_refresh_style)
    report_style_mod, func(lvbe_obj_report_style_mod)
    remove_style_local_prop, func(lvbe_obj_remove_style_local_prop)
    set_hidden, func(lvbe_obj_set_hidden)
    set_adv_hittest, func(lvbe_obj_set_adv_hittest)
    set_click, func(lvbe_obj_set_click)
    set_top, func(lvbe_obj_set_top)
    set_drag, func(lvbe_obj_set_drag)
    set_drag_dir, func(lvbe_obj_set_drag_dir)
    set_drag_throw, func(lvbe_obj_set_drag_throw)
    set_drag_parent, func(lvbe_obj_set_drag_parent)
    set_focus_parent, func(lvbe_obj_set_focus_parent)
    set_gesture_parent, func(lvbe_obj_set_gesture_parent)
    set_parent_event, func(lvbe_obj_set_parent_event)
    set_base_dir, func(lvbe_obj_set_base_dir)
    add_protect, func(lvbe_obj_add_protect)
    clear_protect, func(lvbe_obj_clear_protect)
    set_state, func(lvbe_obj_set_state)
    add_state, func(lvbe_obj_add_state)
    clear_state, func(lvbe_obj_clear_state)
    finish_transitions, func(lvbe_obj_finish_transitions)
    set_event_cb, func(lvbe_obj_set_event_cb)
    set_signal_cb, func(lvbe_obj_set_signal_cb)
    set_design_cb, func(lvbe_obj_set_design_cb)
    allocate_ext_attr, func(lvbe_obj_allocate_ext_attr)
    refresh_ext_draw_pad, func(lvbe_obj_refresh_ext_draw_pad)
    get_screen, func(lvbe_obj_get_screen)
    get_parent, func(lvbe_obj_get_parent)
    get_child, func(lvbe_obj_get_child)
    get_child_back, func(lvbe_obj_get_child_back)
    count_children, func(lvbe_obj_count_children)
    count_children_recursive, func(lvbe_obj_count_children_recursive)
    get_coords, func(lvbe_obj_get_coords)
    get_inner_coords, func(lvbe_obj_get_inner_coords)
    get_x, func(lvbe_obj_get_x)
    get_y, func(lvbe_obj_get_y)
    get_width, func(lvbe_obj_get_width)
    get_height, func(lvbe_obj_get_height)
    get_width_fit, func(lvbe_obj_get_width_fit)
    get_height_fit, func(lvbe_obj_get_height_fit)
    get_height_margin, func(lvbe_obj_get_height_margin)
    get_width_margin, func(lvbe_obj_get_width_margin)
    get_width_grid, func(lvbe_obj_get_width_grid)
    get_height_grid, func(lvbe_obj_get_height_grid)
    get_auto_realign, func(lvbe_obj_get_auto_realign)
    get_ext_click_pad_left, func(lvbe_obj_get_ext_click_pad_left)
    get_ext_click_pad_right, func(lvbe_obj_get_ext_click_pad_right)
    get_ext_click_pad_top, func(lvbe_obj_get_ext_click_pad_top)
    get_ext_click_pad_bottom, func(lvbe_obj_get_ext_click_pad_bottom)
    get_ext_draw_pad, func(lvbe_obj_get_ext_draw_pad)
    get_local_style, func(lvbe_obj_get_local_style)
    get_hidden, func(lvbe_obj_get_hidden)
    get_adv_hittest, func(lvbe_obj_get_adv_hittest)
    get_click, func(lvbe_obj_get_click)
    get_top, func(lvbe_obj_get_top)
    get_drag, func(lvbe_obj_get_drag)
    get_drag_dir, func(lvbe_obj_get_drag_dir)
    get_drag_throw, func(lvbe_obj_get_drag_throw)
    get_drag_parent, func(lvbe_obj_get_drag_parent)
    get_focus_parent, func(lvbe_obj_get_focus_parent)
    get_parent_event, func(lvbe_obj_get_parent_event)
    get_gesture_parent, func(lvbe_obj_get_gesture_parent)
    get_base_dir, func(lvbe_obj_get_base_dir)
    get_protect, func(lvbe_obj_get_protect)
    is_protected, func(lvbe_obj_is_protected)
    get_state, func(lvbe_obj_get_state)
    is_point_on_coords, func(lvbe_obj_is_point_on_coords)
    hittest, func(lvbe_obj_hittest)
    get_ext_attr, func(lvbe_obj_get_ext_attr)
    get_type, func(lvbe_obj_get_type)
    get_user_data, func(lvbe_obj_get_user_data)
    set_user_data, func(lvbe_obj_set_user_data)
    get_group, func(lvbe_obj_get_group)
    is_focused, func(lvbe_obj_is_focused)
    get_focused_obj, func(lvbe_obj_get_focused_obj)
    handle_get_type_signal, func(lvbe_obj_handle_get_type_signal)
    init_draw_rect_dsc, func(lvbe_obj_init_draw_rect_dsc)
    init_draw_label_dsc, func(lvbe_obj_init_draw_label_dsc)
    init_draw_img_dsc, func(lvbe_obj_init_draw_img_dsc)
    init_draw_line_dsc, func(lvbe_obj_init_draw_line_dsc)
    get_draw_rect_ext_pad_size, func(lvbe_obj_get_draw_rect_ext_pad_size)
    fade_in, func(lvbe_obj_fade_in)
    fade_out, func(lvbe_obj_fade_out)
    get_style_radius, func(lvbe_obj_get_style_radius)
    set_style_local_radius, func(lvbe_obj_set_style_local_radius)
    get_style_clip_corner, func(lvbe_obj_get_style_clip_corner)
    set_style_local_clip_corner, func(lvbe_obj_set_style_local_clip_corner)
    get_style_size, func(lvbe_obj_get_style_size)
    set_style_local_size, func(lvbe_obj_set_style_local_size)
    get_style_transform_width, func(lvbe_obj_get_style_transform_width)
    set_style_local_transform_width, func(lvbe_obj_set_style_local_transform_width)
    get_style_transform_height, func(lvbe_obj_get_style_transform_height)
    set_style_local_transform_height, func(lvbe_obj_set_style_local_transform_height)
    get_style_transform_angle, func(lvbe_obj_get_style_transform_angle)
    set_style_local_transform_angle, func(lvbe_obj_set_style_local_transform_angle)
    get_style_transform_zoom, func(lvbe_obj_get_style_transform_zoom)
    set_style_local_transform_zoom, func(lvbe_obj_set_style_local_transform_zoom)
    get_style_opa_scale, func(lvbe_obj_get_style_opa_scale)
    set_style_local_opa_scale, func(lvbe_obj_set_style_local_opa_scale)
    get_style_pad_top, func(lvbe_obj_get_style_pad_top)
    set_style_local_pad_top, func(lvbe_obj_set_style_local_pad_top)
    get_style_pad_bottom, func(lvbe_obj_get_style_pad_bottom)
    set_style_local_pad_bottom, func(lvbe_obj_set_style_local_pad_bottom)
    get_style_pad_left, func(lvbe_obj_get_style_pad_left)
    set_style_local_pad_left, func(lvbe_obj_set_style_local_pad_left)
    get_style_pad_right, func(lvbe_obj_get_style_pad_right)
    set_style_local_pad_right, func(lvbe_obj_set_style_local_pad_right)
    get_style_pad_inner, func(lvbe_obj_get_style_pad_inner)
    set_style_local_pad_inner, func(lvbe_obj_set_style_local_pad_inner)
    get_style_margin_top, func(lvbe_obj_get_style_margin_top)
    set_style_local_margin_top, func(lvbe_obj_set_style_local_margin_top)
    get_style_margin_bottom, func(lvbe_obj_get_style_margin_bottom)
    set_style_local_margin_bottom, func(lvbe_obj_set_style_local_margin_bottom)
    get_style_margin_left, func(lvbe_obj_get_style_margin_left)
    set_style_local_margin_left, func(lvbe_obj_set_style_local_margin_left)
    get_style_margin_right, func(lvbe_obj_get_style_margin_right)
    set_style_local_margin_right, func(lvbe_obj_set_style_local_margin_right)
    set_style_local_bg_blend_mode, func(lvbe_obj_set_style_local_bg_blend_mode)
    get_style_bg_main_stop, func(lvbe_obj_get_style_bg_main_stop)
    set_style_local_bg_main_stop, func(lvbe_obj_set_style_local_bg_main_stop)
    get_style_bg_grad_stop, func(lvbe_obj_get_style_bg_grad_stop)
    set_style_local_bg_grad_stop, func(lvbe_obj_set_style_local_bg_grad_stop)
    set_style_local_bg_grad_dir, func(lvbe_obj_set_style_local_bg_grad_dir)
    get_style_bg_color, func(lvbe_obj_get_style_bg_color)
    set_style_local_bg_color, func(lvbe_obj_set_style_local_bg_color)
    get_style_bg_grad_color, func(lvbe_obj_get_style_bg_grad_color)
    set_style_local_bg_grad_color, func(lvbe_obj_set_style_local_bg_grad_color)
    get_style_bg_opa, func(lvbe_obj_get_style_bg_opa)
    set_style_local_bg_opa, func(lvbe_obj_set_style_local_bg_opa)
    get_style_border_width, func(lvbe_obj_get_style_border_width)
    set_style_local_border_width, func(lvbe_obj_set_style_local_border_width)
    set_style_local_border_side, func(lvbe_obj_set_style_local_border_side)
    set_style_local_border_blend_mode, func(lvbe_obj_set_style_local_border_blend_mode)
    get_style_border_post, func(lvbe_obj_get_style_border_post)
    set_style_local_border_post, func(lvbe_obj_set_style_local_border_post)
    get_style_border_color, func(lvbe_obj_get_style_border_color)
    set_style_local_border_color, func(lvbe_obj_set_style_local_border_color)
    get_style_border_opa, func(lvbe_obj_get_style_border_opa)
    set_style_local_border_opa, func(lvbe_obj_set_style_local_border_opa)
    get_style_outline_width, func(lvbe_obj_get_style_outline_width)
    set_style_local_outline_width, func(lvbe_obj_set_style_local_outline_width)
    get_style_outline_pad, func(lvbe_obj_get_style_outline_pad)
    set_style_local_outline_pad, func(lvbe_obj_set_style_local_outline_pad)
    set_style_local_outline_blend_mode, func(lvbe_obj_set_style_local_outline_blend_mode)
    get_style_outline_color, func(lvbe_obj_get_style_outline_color)
    set_style_local_outline_color, func(lvbe_obj_set_style_local_outline_color)
    get_style_outline_opa, func(lvbe_obj_get_style_outline_opa)
    set_style_local_outline_opa, func(lvbe_obj_set_style_local_outline_opa)
    get_style_shadow_width, func(lvbe_obj_get_style_shadow_width)
    set_style_local_shadow_width, func(lvbe_obj_set_style_local_shadow_width)
    get_style_shadow_ofs_x, func(lvbe_obj_get_style_shadow_ofs_x)
    set_style_local_shadow_ofs_x, func(lvbe_obj_set_style_local_shadow_ofs_x)
    get_style_shadow_ofs_y, func(lvbe_obj_get_style_shadow_ofs_y)
    set_style_local_shadow_ofs_y, func(lvbe_obj_set_style_local_shadow_ofs_y)
    get_style_shadow_spread, func(lvbe_obj_get_style_shadow_spread)
    set_style_local_shadow_spread, func(lvbe_obj_set_style_local_shadow_spread)
    set_style_local_shadow_blend_mode, func(lvbe_obj_set_style_local_shadow_blend_mode)
    get_style_shadow_color, func(lvbe_obj_get_style_shadow_color)
    set_style_local_shadow_color, func(lvbe_obj_set_style_local_shadow_color)
    get_style_shadow_opa, func(lvbe_obj_get_style_shadow_opa)
    set_style_local_shadow_opa, func(lvbe_obj_set_style_local_shadow_opa)
    get_style_pattern_repeat, func(lvbe_obj_get_style_pattern_repeat)
    set_style_local_pattern_repeat, func(lvbe_obj_set_style_local_pattern_repeat)
    set_style_local_pattern_blend_mode, func(lvbe_obj_set_style_local_pattern_blend_mode)
    get_style_pattern_recolor, func(lvbe_obj_get_style_pattern_recolor)
    set_style_local_pattern_recolor, func(lvbe_obj_set_style_local_pattern_recolor)
    get_style_pattern_opa, func(lvbe_obj_get_style_pattern_opa)
    set_style_local_pattern_opa, func(lvbe_obj_set_style_local_pattern_opa)
    get_style_pattern_recolor_opa, func(lvbe_obj_get_style_pattern_recolor_opa)
    set_style_local_pattern_recolor_opa, func(lvbe_obj_set_style_local_pattern_recolor_opa)
    get_style_pattern_image, func(lvbe_obj_get_style_pattern_image)
    set_style_local_pattern_image, func(lvbe_obj_set_style_local_pattern_image)
    get_style_value_letter_space, func(lvbe_obj_get_style_value_letter_space)
    set_style_local_value_letter_space, func(lvbe_obj_set_style_local_value_letter_space)
    get_style_value_line_space, func(lvbe_obj_get_style_value_line_space)
    set_style_local_value_line_space, func(lvbe_obj_set_style_local_value_line_space)
    set_style_local_value_blend_mode, func(lvbe_obj_set_style_local_value_blend_mode)
    get_style_value_ofs_x, func(lvbe_obj_get_style_value_ofs_x)
    set_style_local_value_ofs_x, func(lvbe_obj_set_style_local_value_ofs_x)
    get_style_value_ofs_y, func(lvbe_obj_get_style_value_ofs_y)
    set_style_local_value_ofs_y, func(lvbe_obj_set_style_local_value_ofs_y)
    set_style_local_value_align, func(lvbe_obj_set_style_local_value_align)
    get_style_value_color, func(lvbe_obj_get_style_value_color)
    set_style_local_value_color, func(lvbe_obj_set_style_local_value_color)
    get_style_value_opa, func(lvbe_obj_get_style_value_opa)
    set_style_local_value_opa, func(lvbe_obj_set_style_local_value_opa)
    set_style_local_value_font, func(lvbe_obj_set_style_local_value_font)
    get_style_value_str, func(lvbe_obj_get_style_value_str)
    set_style_local_value_str, func(lvbe_obj_set_style_local_value_str)
    get_style_text_letter_space, func(lvbe_obj_get_style_text_letter_space)
    set_style_local_text_letter_space, func(lvbe_obj_set_style_local_text_letter_space)
    get_style_text_line_space, func(lvbe_obj_get_style_text_line_space)
    set_style_local_text_line_space, func(lvbe_obj_set_style_local_text_line_space)
    set_style_local_text_decor, func(lvbe_obj_set_style_local_text_decor)
    set_style_local_text_blend_mode, func(lvbe_obj_set_style_local_text_blend_mode)
    get_style_text_color, func(lvbe_obj_get_style_text_color)
    set_style_local_text_color, func(lvbe_obj_set_style_local_text_color)
    get_style_text_sel_color, func(lvbe_obj_get_style_text_sel_color)
    set_style_local_text_sel_color, func(lvbe_obj_set_style_local_text_sel_color)
    get_style_text_sel_bg_color, func(lvbe_obj_get_style_text_sel_bg_color)
    set_style_local_text_sel_bg_color, func(lvbe_obj_set_style_local_text_sel_bg_color)
    get_style_text_opa, func(lvbe_obj_get_style_text_opa)
    set_style_local_text_opa, func(lvbe_obj_set_style_local_text_opa)
    set_style_local_text_font, func(lvbe_obj_set_style_local_text_font)
    get_style_line_width, func(lvbe_obj_get_style_line_width)
    set_style_local_line_width, func(lvbe_obj_set_style_local_line_width)
    set_style_local_line_blend_mode, func(lvbe_obj_set_style_local_line_blend_mode)
    get_style_line_dash_width, func(lvbe_obj_get_style_line_dash_width)
    set_style_local_line_dash_width, func(lvbe_obj_set_style_local_line_dash_width)
    get_style_line_dash_gap, func(lvbe_obj_get_style_line_dash_gap)
    set_style_local_line_dash_gap, func(lvbe_obj_set_style_local_line_dash_gap)
    get_style_line_rounded, func(lvbe_obj_get_style_line_rounded)
    set_style_local_line_rounded, func(lvbe_obj_set_style_local_line_rounded)
    get_style_line_color, func(lvbe_obj_get_style_line_color)
    set_style_local_line_color, func(lvbe_obj_set_style_local_line_color)
    get_style_line_opa, func(lvbe_obj_get_style_line_opa)
    set_style_local_line_opa, func(lvbe_obj_set_style_local_line_opa)
    set_style_local_image_blend_mode, func(lvbe_obj_set_style_local_image_blend_mode)
    get_style_image_recolor, func(lvbe_obj_get_style_image_recolor)
    set_style_local_image_recolor, func(lvbe_obj_set_style_local_image_recolor)
    get_style_image_opa, func(lvbe_obj_get_style_image_opa)
    set_style_local_image_opa, func(lvbe_obj_set_style_local_image_opa)
    get_style_image_recolor_opa, func(lvbe_obj_get_style_image_recolor_opa)
    set_style_local_image_recolor_opa, func(lvbe_obj_set_style_local_image_recolor_opa)
    get_style_transition_time, func(lvbe_obj_get_style_transition_time)
    set_style_local_transition_time, func(lvbe_obj_set_style_local_transition_time)
    get_style_transition_delay, func(lvbe_obj_get_style_transition_delay)
    set_style_local_transition_delay, func(lvbe_obj_set_style_local_transition_delay)
    get_style_transition_prop_1, func(lvbe_obj_get_style_transition_prop_1)
    set_style_local_transition_prop_1, func(lvbe_obj_set_style_local_transition_prop_1)
    get_style_transition_prop_2, func(lvbe_obj_get_style_transition_prop_2)
    set_style_local_transition_prop_2, func(lvbe_obj_set_style_local_transition_prop_2)
    get_style_transition_prop_3, func(lvbe_obj_get_style_transition_prop_3)
    set_style_local_transition_prop_3, func(lvbe_obj_set_style_local_transition_prop_3)
    get_style_transition_prop_4, func(lvbe_obj_get_style_transition_prop_4)
    set_style_local_transition_prop_4, func(lvbe_obj_set_style_local_transition_prop_4)
    get_style_transition_prop_5, func(lvbe_obj_get_style_transition_prop_5)
    set_style_local_transition_prop_5, func(lvbe_obj_set_style_local_transition_prop_5)
    get_style_transition_prop_6, func(lvbe_obj_get_style_transition_prop_6)
    set_style_local_transition_prop_6, func(lvbe_obj_set_style_local_transition_prop_6)
    set_style_local_transition_path, func(lvbe_obj_set_style_local_transition_path)
    get_style_scale_width, func(lvbe_obj_get_style_scale_width)
    set_style_local_scale_width, func(lvbe_obj_set_style_local_scale_width)
    get_style_scale_border_width, func(lvbe_obj_get_style_scale_border_width)
    set_style_local_scale_border_width, func(lvbe_obj_set_style_local_scale_border_width)
    get_style_scale_end_border_width, func(lvbe_obj_get_style_scale_end_border_width)
    set_style_local_scale_end_border_width, func(lvbe_obj_set_style_local_scale_end_border_width)
    get_style_scale_end_line_width, func(lvbe_obj_get_style_scale_end_line_width)
    set_style_local_scale_end_line_width, func(lvbe_obj_set_style_local_scale_end_line_width)
    get_style_scale_grad_color, func(lvbe_obj_get_style_scale_grad_color)
    set_style_local_scale_grad_color, func(lvbe_obj_set_style_local_scale_grad_color)
    get_style_scale_end_color, func(lvbe_obj_get_style_scale_end_color)
    set_style_local_scale_end_color, func(lvbe_obj_set_style_local_scale_end_color)
}
@const_object_info_end */

void be_load_lv_arc_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_arc_create },
    { "tostring", lvx_tostring },

    { "create", lvbe_arc_create },
    { "set_start_angle", lvbe_arc_set_start_angle },
    { "set_end_angle", lvbe_arc_set_end_angle },
    { "set_angles", lvbe_arc_set_angles },
    { "set_bg_start_angle", lvbe_arc_set_bg_start_angle },
    { "set_bg_end_angle", lvbe_arc_set_bg_end_angle },
    { "set_bg_angles", lvbe_arc_set_bg_angles },
    { "set_rotation", lvbe_arc_set_rotation },
    { "set_type", lvbe_arc_set_type },
    { "set_value", lvbe_arc_set_value },
    { "set_range", lvbe_arc_set_range },
    { "set_chg_rate", lvbe_arc_set_chg_rate },
    { "set_adjustable", lvbe_arc_set_adjustable },
    { "get_angle_start", lvbe_arc_get_angle_start },
    { "get_angle_end", lvbe_arc_get_angle_end },
    { "get_bg_angle_start", lvbe_arc_get_bg_angle_start },
    { "get_bg_angle_end", lvbe_arc_get_bg_angle_end },
    { "get_type", lvbe_arc_get_type },
    { "get_value", lvbe_arc_get_value },
    { "get_min_value", lvbe_arc_get_min_value },
    { "get_max_value", lvbe_arc_get_max_value },
    { "is_dragged", lvbe_arc_is_dragged },
    { "get_adjustable", lvbe_arc_get_adjustable },

    // { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */

    { NULL, NULL }
  };
  be_regclass(vm, "lv_arc", members);

  be_getglobal(vm, "lv_arc");
  be_getglobal(vm, "lv_obj");
  be_setsuper(vm, -2);
  be_pop(vm, 2);
#else
    be_pushntvclass(vm, &be_class_lv_arc);
    be_setglobal(vm, "lv_arc");
    be_pop(vm, 1);
#endif
};

/* @const_object_info_begin
class be_class_lv_arc (scope: global, name: lv_arc, super: be_class_lv_obj) {
    .p, var
    init, func(lvbe_arc_create)
    tostring, func(lvx_tostring)
    create, func(lvbe_arc_create)
    set_start_angle, func(lvbe_arc_set_start_angle)
    set_end_angle, func(lvbe_arc_set_end_angle)
    set_angles, func(lvbe_arc_set_angles)
    set_bg_start_angle, func(lvbe_arc_set_bg_start_angle)
    set_bg_end_angle, func(lvbe_arc_set_bg_end_angle)
    set_bg_angles, func(lvbe_arc_set_bg_angles)
    set_rotation, func(lvbe_arc_set_rotation)
    set_type, func(lvbe_arc_set_type)
    set_value, func(lvbe_arc_set_value)
    set_range, func(lvbe_arc_set_range)
    set_chg_rate, func(lvbe_arc_set_chg_rate)
    set_adjustable, func(lvbe_arc_set_adjustable)
    get_angle_start, func(lvbe_arc_get_angle_start)
    get_angle_end, func(lvbe_arc_get_angle_end)
    get_bg_angle_start, func(lvbe_arc_get_bg_angle_start)
    get_bg_angle_end, func(lvbe_arc_get_bg_angle_end)
    get_type, func(lvbe_arc_get_type)
    get_value, func(lvbe_arc_get_value)
    get_min_value, func(lvbe_arc_get_min_value)
    get_max_value, func(lvbe_arc_get_max_value)
    is_dragged, func(lvbe_arc_is_dragged)
    get_adjustable, func(lvbe_arc_get_adjustable)
}
@const_object_info_end */

void be_load_lv_bar_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_bar_create },
    { "tostring", lvx_tostring },

    { "create", lvbe_bar_create },
    { "set_value", lvbe_bar_set_value },
    { "set_start_value", lvbe_bar_set_start_value },
    { "set_range", lvbe_bar_set_range },
    { "set_type", lvbe_bar_set_type },
    { "set_anim_time", lvbe_bar_set_anim_time },
    { "get_value", lvbe_bar_get_value },
    { "get_start_value", lvbe_bar_get_start_value },
    { "get_min_value", lvbe_bar_get_min_value },
    { "get_max_value", lvbe_bar_get_max_value },
    { "get_type", lvbe_bar_get_type },
    { "get_anim_time", lvbe_bar_get_anim_time },

    // { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */

    { NULL, NULL }
  };
  be_regclass(vm, "lv_bar", members);

  be_getglobal(vm, "lv_bar");
  be_getglobal(vm, "lv_obj");
  be_setsuper(vm, -2);
  be_pop(vm, 2);
#else
    be_pushntvclass(vm, &be_class_lv_bar);
    be_setglobal(vm, "lv_bar");
    be_pop(vm, 1);
#endif
};

/* @const_object_info_begin
class be_class_lv_bar (scope: global, name: lv_bar, super: be_class_lv_obj) {
    .p, var
    init, func(lvbe_bar_create)
    tostring, func(lvx_tostring)
    create, func(lvbe_bar_create)
    set_value, func(lvbe_bar_set_value)
    set_start_value, func(lvbe_bar_set_start_value)
    set_range, func(lvbe_bar_set_range)
    set_type, func(lvbe_bar_set_type)
    set_anim_time, func(lvbe_bar_set_anim_time)
    get_value, func(lvbe_bar_get_value)
    get_start_value, func(lvbe_bar_get_start_value)
    get_min_value, func(lvbe_bar_get_min_value)
    get_max_value, func(lvbe_bar_get_max_value)
    get_type, func(lvbe_bar_get_type)
    get_anim_time, func(lvbe_bar_get_anim_time)
}
@const_object_info_end */

void be_load_lv_btn_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_btn_create },
    { "tostring", lvx_tostring },

    { "create", lvbe_btn_create },
    { "set_checkable", lvbe_btn_set_checkable },
    { "set_state", lvbe_btn_set_state },
    { "toggle", lvbe_btn_toggle },
    { "set_layout", lvbe_btn_set_layout },
    { "set_fit4", lvbe_btn_set_fit4 },
    { "set_fit2", lvbe_btn_set_fit2 },
    { "set_fit", lvbe_btn_set_fit },
    { "get_state", lvbe_btn_get_state },
    { "get_checkable", lvbe_btn_get_checkable },
    { "get_layout", lvbe_btn_get_layout },
    { "get_fit_left", lvbe_btn_get_fit_left },
    { "get_fit_right", lvbe_btn_get_fit_right },
    { "get_fit_top", lvbe_btn_get_fit_top },
    { "get_fit_bottom", lvbe_btn_get_fit_bottom },

    // { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */

    { NULL, NULL }
  };
  be_regclass(vm, "lv_btn", members);

  be_getglobal(vm, "lv_btn");
  be_getglobal(vm, "lv_obj");
  be_setsuper(vm, -2);
  be_pop(vm, 2);
#else
    be_pushntvclass(vm, &be_class_lv_btn);
    be_setglobal(vm, "lv_btn");
    be_pop(vm, 1);
#endif
};

/* @const_object_info_begin
class be_class_lv_btn (scope: global, name: lv_btn, super: be_class_lv_obj) {
    .p, var
    init, func(lvbe_btn_create)
    tostring, func(lvx_tostring)
    create, func(lvbe_btn_create)
    set_checkable, func(lvbe_btn_set_checkable)
    set_state, func(lvbe_btn_set_state)
    toggle, func(lvbe_btn_toggle)
    set_layout, func(lvbe_btn_set_layout)
    set_fit4, func(lvbe_btn_set_fit4)
    set_fit2, func(lvbe_btn_set_fit2)
    set_fit, func(lvbe_btn_set_fit)
    get_state, func(lvbe_btn_get_state)
    get_checkable, func(lvbe_btn_get_checkable)
    get_layout, func(lvbe_btn_get_layout)
    get_fit_left, func(lvbe_btn_get_fit_left)
    get_fit_right, func(lvbe_btn_get_fit_right)
    get_fit_top, func(lvbe_btn_get_fit_top)
    get_fit_bottom, func(lvbe_btn_get_fit_bottom)
}
@const_object_info_end */

void be_load_lv_btnmatrix_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_btnmatrix_create },
    { "tostring", lvx_tostring },

    { "create", lvbe_btnmatrix_create },
    { "set_map", lvbe_btnmatrix_set_map },
    { "set_ctrl_map", lvbe_btnmatrix_set_ctrl_map },
    { "set_focused_btn", lvbe_btnmatrix_set_focused_btn },
    { "set_recolor", lvbe_btnmatrix_set_recolor },
    { "set_btn_ctrl", lvbe_btnmatrix_set_btn_ctrl },
    { "clear_btn_ctrl", lvbe_btnmatrix_clear_btn_ctrl },
    { "set_btn_ctrl_all", lvbe_btnmatrix_set_btn_ctrl_all },
    { "clear_btn_ctrl_all", lvbe_btnmatrix_clear_btn_ctrl_all },
    { "set_btn_width", lvbe_btnmatrix_set_btn_width },
    { "set_one_check", lvbe_btnmatrix_set_one_check },
    { "set_align", lvbe_btnmatrix_set_align },
    { "get_recolor", lvbe_btnmatrix_get_recolor },
    { "get_active_btn", lvbe_btnmatrix_get_active_btn },
    { "get_active_btn_text", lvbe_btnmatrix_get_active_btn_text },
    { "get_focused_btn", lvbe_btnmatrix_get_focused_btn },
    { "get_btn_text", lvbe_btnmatrix_get_btn_text },
    { "get_btn_ctrl", lvbe_btnmatrix_get_btn_ctrl },
    { "get_one_check", lvbe_btnmatrix_get_one_check },
    { "get_align", lvbe_btnmatrix_get_align },

    // { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */

    { NULL, NULL }
  };
  be_regclass(vm, "lv_btnmatrix", members);

  be_getglobal(vm, "lv_btnmatrix");
  be_getglobal(vm, "lv_obj");
  be_setsuper(vm, -2);
  be_pop(vm, 2);
#else
    be_pushntvclass(vm, &be_class_lv_btnmatrix);
    be_setglobal(vm, "lv_btnmatrix");
    be_pop(vm, 1);
#endif
};

/* @const_object_info_begin
class be_class_lv_btnmatrix (scope: global, name: lv_btnmatrix, super: be_class_lv_obj) {
    .p, var
    init, func(lvbe_btnmatrix_create)
    tostring, func(lvx_tostring)
    create, func(lvbe_btnmatrix_create)
    set_map, func(lvbe_btnmatrix_set_map)
    set_ctrl_map, func(lvbe_btnmatrix_set_ctrl_map)
    set_focused_btn, func(lvbe_btnmatrix_set_focused_btn)
    set_recolor, func(lvbe_btnmatrix_set_recolor)
    set_btn_ctrl, func(lvbe_btnmatrix_set_btn_ctrl)
    clear_btn_ctrl, func(lvbe_btnmatrix_clear_btn_ctrl)
    set_btn_ctrl_all, func(lvbe_btnmatrix_set_btn_ctrl_all)
    clear_btn_ctrl_all, func(lvbe_btnmatrix_clear_btn_ctrl_all)
    set_btn_width, func(lvbe_btnmatrix_set_btn_width)
    set_one_check, func(lvbe_btnmatrix_set_one_check)
    set_align, func(lvbe_btnmatrix_set_align)
    get_recolor, func(lvbe_btnmatrix_get_recolor)
    get_active_btn, func(lvbe_btnmatrix_get_active_btn)
    get_active_btn_text, func(lvbe_btnmatrix_get_active_btn_text)
    get_focused_btn, func(lvbe_btnmatrix_get_focused_btn)
    get_btn_text, func(lvbe_btnmatrix_get_btn_text)
    get_btn_ctrl, func(lvbe_btnmatrix_get_btn_ctrl)
    get_one_check, func(lvbe_btnmatrix_get_one_check)
    get_align, func(lvbe_btnmatrix_get_align)
}
@const_object_info_end */

void be_load_lv_calendar_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_calendar_create },
    { "tostring", lvx_tostring },

    { "create", lvbe_calendar_create },
    { "set_today_date", lvbe_calendar_set_today_date },
    { "set_showed_date", lvbe_calendar_set_showed_date },
    { "set_highlighted_dates", lvbe_calendar_set_highlighted_dates },
    { "set_day_names", lvbe_calendar_set_day_names },
    { "set_month_names", lvbe_calendar_set_month_names },
    { "get_highlighted_dates_num", lvbe_calendar_get_highlighted_dates_num },
    { "get_day_of_week", lvbe_calendar_get_day_of_week },

    // { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */

    { NULL, NULL }
  };
  be_regclass(vm, "lv_calendar", members);

  be_getglobal(vm, "lv_calendar");
  be_getglobal(vm, "lv_obj");
  be_setsuper(vm, -2);
  be_pop(vm, 2);
#else
    be_pushntvclass(vm, &be_class_lv_calendar);
    be_setglobal(vm, "lv_calendar");
    be_pop(vm, 1);
#endif
};

/* @const_object_info_begin
class be_class_lv_calendar (scope: global, name: lv_calendar, super: be_class_lv_obj) {
    .p, var
    init, func(lvbe_calendar_create)
    tostring, func(lvx_tostring)
    create, func(lvbe_calendar_create)
    set_today_date, func(lvbe_calendar_set_today_date)
    set_showed_date, func(lvbe_calendar_set_showed_date)
    set_highlighted_dates, func(lvbe_calendar_set_highlighted_dates)
    set_day_names, func(lvbe_calendar_set_day_names)
    set_month_names, func(lvbe_calendar_set_month_names)
    get_highlighted_dates_num, func(lvbe_calendar_get_highlighted_dates_num)
    get_day_of_week, func(lvbe_calendar_get_day_of_week)
}
@const_object_info_end */

void be_load_lv_canvas_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_canvas_create },
    { "tostring", lvx_tostring },

    { "create", lvbe_canvas_create },
    { "set_buffer", lvbe_canvas_set_buffer },
    { "set_px", lvbe_canvas_set_px },
    { "set_palette", lvbe_canvas_set_palette },
    { "get_px", lvbe_canvas_get_px },
    { "copy_buf", lvbe_canvas_copy_buf },
    { "transform", lvbe_canvas_transform },
    { "blur_hor", lvbe_canvas_blur_hor },
    { "blur_ver", lvbe_canvas_blur_ver },
    { "fill_bg", lvbe_canvas_fill_bg },
    { "draw_rect", lvbe_canvas_draw_rect },
    { "draw_text", lvbe_canvas_draw_text },
    { "draw_img", lvbe_canvas_draw_img },
    { "draw_line", lvbe_canvas_draw_line },
    { "draw_polygon", lvbe_canvas_draw_polygon },
    { "draw_arc", lvbe_canvas_draw_arc },

    // { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */

    { NULL, NULL }
  };
  be_regclass(vm, "lv_canvas", members);

  be_getglobal(vm, "lv_canvas");
  be_getglobal(vm, "lv_obj");
  be_setsuper(vm, -2);
  be_pop(vm, 2);
#else
    be_pushntvclass(vm, &be_class_lv_canvas);
    be_setglobal(vm, "lv_canvas");
    be_pop(vm, 1);
#endif
};

/* @const_object_info_begin
class be_class_lv_canvas (scope: global, name: lv_canvas, super: be_class_lv_obj) {
    .p, var
    init, func(lvbe_canvas_create)
    tostring, func(lvx_tostring)
    create, func(lvbe_canvas_create)
    set_buffer, func(lvbe_canvas_set_buffer)
    set_px, func(lvbe_canvas_set_px)
    set_palette, func(lvbe_canvas_set_palette)
    get_px, func(lvbe_canvas_get_px)
    copy_buf, func(lvbe_canvas_copy_buf)
    transform, func(lvbe_canvas_transform)
    blur_hor, func(lvbe_canvas_blur_hor)
    blur_ver, func(lvbe_canvas_blur_ver)
    fill_bg, func(lvbe_canvas_fill_bg)
    draw_rect, func(lvbe_canvas_draw_rect)
    draw_text, func(lvbe_canvas_draw_text)
    draw_img, func(lvbe_canvas_draw_img)
    draw_line, func(lvbe_canvas_draw_line)
    draw_polygon, func(lvbe_canvas_draw_polygon)
    draw_arc, func(lvbe_canvas_draw_arc)
}
@const_object_info_end */

void be_load_lv_chart_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_chart_create },
    { "tostring", lvx_tostring },

    { "create", lvbe_chart_create },
    { "remove_series", lvbe_chart_remove_series },
    { "clear_series", lvbe_chart_clear_series },
    { "hide_series", lvbe_chart_hide_series },
    { "set_div_line_count", lvbe_chart_set_div_line_count },
    { "set_y_range", lvbe_chart_set_y_range },
    { "set_type", lvbe_chart_set_type },
    { "set_point_count", lvbe_chart_set_point_count },
    { "init_points", lvbe_chart_init_points },
    { "set_points", lvbe_chart_set_points },
    { "set_next", lvbe_chart_set_next },
    { "set_update_mode", lvbe_chart_set_update_mode },
    { "set_x_tick_length", lvbe_chart_set_x_tick_length },
    { "set_y_tick_length", lvbe_chart_set_y_tick_length },
    { "set_secondary_y_tick_length", lvbe_chart_set_secondary_y_tick_length },
    { "set_x_tick_texts", lvbe_chart_set_x_tick_texts },
    { "set_secondary_y_tick_texts", lvbe_chart_set_secondary_y_tick_texts },
    { "set_y_tick_texts", lvbe_chart_set_y_tick_texts },
    { "set_x_start_point", lvbe_chart_set_x_start_point },
    { "set_ext_array", lvbe_chart_set_ext_array },
    { "set_point_id", lvbe_chart_set_point_id },
    { "set_series_axis", lvbe_chart_set_series_axis },
    { "set_cursor_point", lvbe_chart_set_cursor_point },
    { "get_type", lvbe_chart_get_type },
    { "get_point_count", lvbe_chart_get_point_count },
    { "get_x_start_point", lvbe_chart_get_x_start_point },
    { "get_point_id", lvbe_chart_get_point_id },
    { "get_series_axis", lvbe_chart_get_series_axis },
    { "get_series_area", lvbe_chart_get_series_area },
    { "get_cursor_point", lvbe_chart_get_cursor_point },
    { "get_nearest_index_from_coord", lvbe_chart_get_nearest_index_from_coord },
    { "get_x_from_index", lvbe_chart_get_x_from_index },
    { "get_y_from_index", lvbe_chart_get_y_from_index },
    { "refresh", lvbe_chart_refresh },

    // { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */

    { NULL, NULL }
  };
  be_regclass(vm, "lv_chart", members);

  be_getglobal(vm, "lv_chart");
  be_getglobal(vm, "lv_obj");
  be_setsuper(vm, -2);
  be_pop(vm, 2);
#else
    be_pushntvclass(vm, &be_class_lv_chart);
    be_setglobal(vm, "lv_chart");
    be_pop(vm, 1);
#endif
};

/* @const_object_info_begin
class be_class_lv_chart (scope: global, name: lv_chart, super: be_class_lv_obj) {
    .p, var
    init, func(lvbe_chart_create)
    tostring, func(lvx_tostring)
    create, func(lvbe_chart_create)
    remove_series, func(lvbe_chart_remove_series)
    clear_series, func(lvbe_chart_clear_series)
    hide_series, func(lvbe_chart_hide_series)
    set_div_line_count, func(lvbe_chart_set_div_line_count)
    set_y_range, func(lvbe_chart_set_y_range)
    set_type, func(lvbe_chart_set_type)
    set_point_count, func(lvbe_chart_set_point_count)
    init_points, func(lvbe_chart_init_points)
    set_points, func(lvbe_chart_set_points)
    set_next, func(lvbe_chart_set_next)
    set_update_mode, func(lvbe_chart_set_update_mode)
    set_x_tick_length, func(lvbe_chart_set_x_tick_length)
    set_y_tick_length, func(lvbe_chart_set_y_tick_length)
    set_secondary_y_tick_length, func(lvbe_chart_set_secondary_y_tick_length)
    set_x_tick_texts, func(lvbe_chart_set_x_tick_texts)
    set_secondary_y_tick_texts, func(lvbe_chart_set_secondary_y_tick_texts)
    set_y_tick_texts, func(lvbe_chart_set_y_tick_texts)
    set_x_start_point, func(lvbe_chart_set_x_start_point)
    set_ext_array, func(lvbe_chart_set_ext_array)
    set_point_id, func(lvbe_chart_set_point_id)
    set_series_axis, func(lvbe_chart_set_series_axis)
    set_cursor_point, func(lvbe_chart_set_cursor_point)
    get_type, func(lvbe_chart_get_type)
    get_point_count, func(lvbe_chart_get_point_count)
    get_x_start_point, func(lvbe_chart_get_x_start_point)
    get_point_id, func(lvbe_chart_get_point_id)
    get_series_axis, func(lvbe_chart_get_series_axis)
    get_series_area, func(lvbe_chart_get_series_area)
    get_cursor_point, func(lvbe_chart_get_cursor_point)
    get_nearest_index_from_coord, func(lvbe_chart_get_nearest_index_from_coord)
    get_x_from_index, func(lvbe_chart_get_x_from_index)
    get_y_from_index, func(lvbe_chart_get_y_from_index)
    refresh, func(lvbe_chart_refresh)
}
@const_object_info_end */

void be_load_lv_checkbox_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_checkbox_create },
    { "tostring", lvx_tostring },

    { "create", lvbe_checkbox_create },
    { "set_text", lvbe_checkbox_set_text },
    { "set_text_static", lvbe_checkbox_set_text_static },
    { "set_checked", lvbe_checkbox_set_checked },
    { "set_disabled", lvbe_checkbox_set_disabled },
    { "set_state", lvbe_checkbox_set_state },
    { "get_text", lvbe_checkbox_get_text },
    { "is_checked", lvbe_checkbox_is_checked },
    { "is_inactive", lvbe_checkbox_is_inactive },
    { "get_state", lvbe_checkbox_get_state },

    // { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */

    { NULL, NULL }
  };
  be_regclass(vm, "lv_checkbox", members);

  be_getglobal(vm, "lv_checkbox");
  be_getglobal(vm, "lv_obj");
  be_setsuper(vm, -2);
  be_pop(vm, 2);
#else
    be_pushntvclass(vm, &be_class_lv_checkbox);
    be_setglobal(vm, "lv_checkbox");
    be_pop(vm, 1);
#endif
};

/* @const_object_info_begin
class be_class_lv_checkbox (scope: global, name: lv_checkbox, super: be_class_lv_obj) {
    .p, var
    init, func(lvbe_checkbox_create)
    tostring, func(lvx_tostring)
    create, func(lvbe_checkbox_create)
    set_text, func(lvbe_checkbox_set_text)
    set_text_static, func(lvbe_checkbox_set_text_static)
    set_checked, func(lvbe_checkbox_set_checked)
    set_disabled, func(lvbe_checkbox_set_disabled)
    set_state, func(lvbe_checkbox_set_state)
    get_text, func(lvbe_checkbox_get_text)
    is_checked, func(lvbe_checkbox_is_checked)
    is_inactive, func(lvbe_checkbox_is_inactive)
    get_state, func(lvbe_checkbox_get_state)
}
@const_object_info_end */

void be_load_lv_cont_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_cont_create },
    { "tostring", lvx_tostring },

    { "create", lvbe_cont_create },
    { "set_layout", lvbe_cont_set_layout },
    { "set_fit4", lvbe_cont_set_fit4 },
    { "set_fit2", lvbe_cont_set_fit2 },
    { "set_fit", lvbe_cont_set_fit },
    { "get_layout", lvbe_cont_get_layout },
    { "get_fit_left", lvbe_cont_get_fit_left },
    { "get_fit_right", lvbe_cont_get_fit_right },
    { "get_fit_top", lvbe_cont_get_fit_top },
    { "get_fit_bottom", lvbe_cont_get_fit_bottom },

    // { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */

    { NULL, NULL }
  };
  be_regclass(vm, "lv_cont", members);

  be_getglobal(vm, "lv_cont");
  be_getglobal(vm, "lv_obj");
  be_setsuper(vm, -2);
  be_pop(vm, 2);
#else
    be_pushntvclass(vm, &be_class_lv_cont);
    be_setglobal(vm, "lv_cont");
    be_pop(vm, 1);
#endif
};

/* @const_object_info_begin
class be_class_lv_cont (scope: global, name: lv_cont, super: be_class_lv_obj) {
    .p, var
    init, func(lvbe_cont_create)
    tostring, func(lvx_tostring)
    create, func(lvbe_cont_create)
    set_layout, func(lvbe_cont_set_layout)
    set_fit4, func(lvbe_cont_set_fit4)
    set_fit2, func(lvbe_cont_set_fit2)
    set_fit, func(lvbe_cont_set_fit)
    get_layout, func(lvbe_cont_get_layout)
    get_fit_left, func(lvbe_cont_get_fit_left)
    get_fit_right, func(lvbe_cont_get_fit_right)
    get_fit_top, func(lvbe_cont_get_fit_top)
    get_fit_bottom, func(lvbe_cont_get_fit_bottom)
}
@const_object_info_end */

void be_load_lv_cpicker_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_cpicker_create },
    { "tostring", lvx_tostring },

    { "create", lvbe_cpicker_create },
    { "set_type", lvbe_cpicker_set_type },
    { "set_hue", lvbe_cpicker_set_hue },
    { "set_saturation", lvbe_cpicker_set_saturation },
    { "set_value", lvbe_cpicker_set_value },
    { "set_hsv", lvbe_cpicker_set_hsv },
    { "set_color", lvbe_cpicker_set_color },
    { "set_color_mode", lvbe_cpicker_set_color_mode },
    { "set_color_mode_fixed", lvbe_cpicker_set_color_mode_fixed },
    { "set_knob_colored", lvbe_cpicker_set_knob_colored },
    { "get_color_mode", lvbe_cpicker_get_color_mode },
    { "get_color_mode_fixed", lvbe_cpicker_get_color_mode_fixed },
    { "get_hue", lvbe_cpicker_get_hue },
    { "get_saturation", lvbe_cpicker_get_saturation },
    { "get_value", lvbe_cpicker_get_value },
    { "get_hsv", lvbe_cpicker_get_hsv },
    { "get_color", lvbe_cpicker_get_color },
    { "get_knob_colored", lvbe_cpicker_get_knob_colored },

    // { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */

    { NULL, NULL }
  };
  be_regclass(vm, "lv_cpicker", members);

  be_getglobal(vm, "lv_cpicker");
  be_getglobal(vm, "lv_obj");
  be_setsuper(vm, -2);
  be_pop(vm, 2);
#else
    be_pushntvclass(vm, &be_class_lv_cpicker);
    be_setglobal(vm, "lv_cpicker");
    be_pop(vm, 1);
#endif
};

/* @const_object_info_begin
class be_class_lv_cpicker (scope: global, name: lv_cpicker, super: be_class_lv_obj) {
    .p, var
    init, func(lvbe_cpicker_create)
    tostring, func(lvx_tostring)
    create, func(lvbe_cpicker_create)
    set_type, func(lvbe_cpicker_set_type)
    set_hue, func(lvbe_cpicker_set_hue)
    set_saturation, func(lvbe_cpicker_set_saturation)
    set_value, func(lvbe_cpicker_set_value)
    set_hsv, func(lvbe_cpicker_set_hsv)
    set_color, func(lvbe_cpicker_set_color)
    set_color_mode, func(lvbe_cpicker_set_color_mode)
    set_color_mode_fixed, func(lvbe_cpicker_set_color_mode_fixed)
    set_knob_colored, func(lvbe_cpicker_set_knob_colored)
    get_color_mode, func(lvbe_cpicker_get_color_mode)
    get_color_mode_fixed, func(lvbe_cpicker_get_color_mode_fixed)
    get_hue, func(lvbe_cpicker_get_hue)
    get_saturation, func(lvbe_cpicker_get_saturation)
    get_value, func(lvbe_cpicker_get_value)
    get_hsv, func(lvbe_cpicker_get_hsv)
    get_color, func(lvbe_cpicker_get_color)
    get_knob_colored, func(lvbe_cpicker_get_knob_colored)
}
@const_object_info_end */

void be_load_lv_dropdown_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_dropdown_create },
    { "tostring", lvx_tostring },

    { "create", lvbe_dropdown_create },
    { "set_text", lvbe_dropdown_set_text },
    { "clear_options", lvbe_dropdown_clear_options },
    { "set_options", lvbe_dropdown_set_options },
    { "set_options_static", lvbe_dropdown_set_options_static },
    { "add_option", lvbe_dropdown_add_option },
    { "set_selected", lvbe_dropdown_set_selected },
    { "set_dir", lvbe_dropdown_set_dir },
    { "set_max_height", lvbe_dropdown_set_max_height },
    { "set_symbol", lvbe_dropdown_set_symbol },
    { "set_show_selected", lvbe_dropdown_set_show_selected },
    { "get_text", lvbe_dropdown_get_text },
    { "get_options", lvbe_dropdown_get_options },
    { "get_selected", lvbe_dropdown_get_selected },
    { "get_option_cnt", lvbe_dropdown_get_option_cnt },
    { "get_selected_str", lvbe_dropdown_get_selected_str },
    { "get_max_height", lvbe_dropdown_get_max_height },
    { "get_symbol", lvbe_dropdown_get_symbol },
    { "get_dir", lvbe_dropdown_get_dir },
    { "get_show_selected", lvbe_dropdown_get_show_selected },
    { "open", lvbe_dropdown_open },
    { "close", lvbe_dropdown_close },

    // { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */

    { NULL, NULL }
  };
  be_regclass(vm, "lv_dropdown", members);

  be_getglobal(vm, "lv_dropdown");
  be_getglobal(vm, "lv_obj");
  be_setsuper(vm, -2);
  be_pop(vm, 2);
#else
    be_pushntvclass(vm, &be_class_lv_dropdown);
    be_setglobal(vm, "lv_dropdown");
    be_pop(vm, 1);
#endif
};

/* @const_object_info_begin
class be_class_lv_dropdown (scope: global, name: lv_dropdown, super: be_class_lv_obj) {
    .p, var
    init, func(lvbe_dropdown_create)
    tostring, func(lvx_tostring)
    create, func(lvbe_dropdown_create)
    set_text, func(lvbe_dropdown_set_text)
    clear_options, func(lvbe_dropdown_clear_options)
    set_options, func(lvbe_dropdown_set_options)
    set_options_static, func(lvbe_dropdown_set_options_static)
    add_option, func(lvbe_dropdown_add_option)
    set_selected, func(lvbe_dropdown_set_selected)
    set_dir, func(lvbe_dropdown_set_dir)
    set_max_height, func(lvbe_dropdown_set_max_height)
    set_symbol, func(lvbe_dropdown_set_symbol)
    set_show_selected, func(lvbe_dropdown_set_show_selected)
    get_text, func(lvbe_dropdown_get_text)
    get_options, func(lvbe_dropdown_get_options)
    get_selected, func(lvbe_dropdown_get_selected)
    get_option_cnt, func(lvbe_dropdown_get_option_cnt)
    get_selected_str, func(lvbe_dropdown_get_selected_str)
    get_max_height, func(lvbe_dropdown_get_max_height)
    get_symbol, func(lvbe_dropdown_get_symbol)
    get_dir, func(lvbe_dropdown_get_dir)
    get_show_selected, func(lvbe_dropdown_get_show_selected)
    open, func(lvbe_dropdown_open)
    close, func(lvbe_dropdown_close)
}
@const_object_info_end */

void be_load_lv_gauge_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_gauge_create },
    { "tostring", lvx_tostring },

    { "create", lvbe_gauge_create },
    { "set_needle_count", lvbe_gauge_set_needle_count },
    { "set_value", lvbe_gauge_set_value },
    { "set_range", lvbe_gauge_set_range },
    { "set_critical_value", lvbe_gauge_set_critical_value },
    { "set_scale", lvbe_gauge_set_scale },
    { "set_angle_offset", lvbe_gauge_set_angle_offset },
    { "set_needle_img", lvbe_gauge_set_needle_img },
    { "set_formatter_cb", lvbe_gauge_set_formatter_cb },
    { "get_value", lvbe_gauge_get_value },
    { "get_needle_count", lvbe_gauge_get_needle_count },
    { "get_min_value", lvbe_gauge_get_min_value },
    { "get_max_value", lvbe_gauge_get_max_value },
    { "get_critical_value", lvbe_gauge_get_critical_value },
    { "get_label_count", lvbe_gauge_get_label_count },
    { "get_line_count", lvbe_gauge_get_line_count },
    { "get_scale_angle", lvbe_gauge_get_scale_angle },
    { "get_angle_offset", lvbe_gauge_get_angle_offset },
    { "get_needle_img", lvbe_gauge_get_needle_img },
    { "get_needle_img_pivot_x", lvbe_gauge_get_needle_img_pivot_x },
    { "get_needle_img_pivot_y", lvbe_gauge_get_needle_img_pivot_y },

    // { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */

    { NULL, NULL }
  };
  be_regclass(vm, "lv_gauge", members);

  be_getglobal(vm, "lv_gauge");
  be_getglobal(vm, "lv_obj");
  be_setsuper(vm, -2);
  be_pop(vm, 2);
#else
    be_pushntvclass(vm, &be_class_lv_gauge);
    be_setglobal(vm, "lv_gauge");
    be_pop(vm, 1);
#endif
};

/* @const_object_info_begin
class be_class_lv_gauge (scope: global, name: lv_gauge, super: be_class_lv_obj) {
    .p, var
    init, func(lvbe_gauge_create)
    tostring, func(lvx_tostring)
    create, func(lvbe_gauge_create)
    set_needle_count, func(lvbe_gauge_set_needle_count)
    set_value, func(lvbe_gauge_set_value)
    set_range, func(lvbe_gauge_set_range)
    set_critical_value, func(lvbe_gauge_set_critical_value)
    set_scale, func(lvbe_gauge_set_scale)
    set_angle_offset, func(lvbe_gauge_set_angle_offset)
    set_needle_img, func(lvbe_gauge_set_needle_img)
    set_formatter_cb, func(lvbe_gauge_set_formatter_cb)
    get_value, func(lvbe_gauge_get_value)
    get_needle_count, func(lvbe_gauge_get_needle_count)
    get_min_value, func(lvbe_gauge_get_min_value)
    get_max_value, func(lvbe_gauge_get_max_value)
    get_critical_value, func(lvbe_gauge_get_critical_value)
    get_label_count, func(lvbe_gauge_get_label_count)
    get_line_count, func(lvbe_gauge_get_line_count)
    get_scale_angle, func(lvbe_gauge_get_scale_angle)
    get_angle_offset, func(lvbe_gauge_get_angle_offset)
    get_needle_img, func(lvbe_gauge_get_needle_img)
    get_needle_img_pivot_x, func(lvbe_gauge_get_needle_img_pivot_x)
    get_needle_img_pivot_y, func(lvbe_gauge_get_needle_img_pivot_y)
}
@const_object_info_end */

void be_load_lv_imgbtn_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_imgbtn_create },
    { "tostring", lvx_tostring },

    { "create", lvbe_imgbtn_create },
    { "set_src", lvbe_imgbtn_set_src },
    { "set_state", lvbe_imgbtn_set_state },
    { "toggle", lvbe_imgbtn_toggle },
    { "set_checkable", lvbe_imgbtn_set_checkable },
    { "get_src", lvbe_imgbtn_get_src },
    { "get_state", lvbe_imgbtn_get_state },

    // { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */

    { NULL, NULL }
  };
  be_regclass(vm, "lv_imgbtn", members);

  be_getglobal(vm, "lv_imgbtn");
  be_getglobal(vm, "lv_obj");
  be_setsuper(vm, -2);
  be_pop(vm, 2);
#else
    be_pushntvclass(vm, &be_class_lv_imgbtn);
    be_setglobal(vm, "lv_imgbtn");
    be_pop(vm, 1);
#endif
};

/* @const_object_info_begin
class be_class_lv_imgbtn (scope: global, name: lv_imgbtn, super: be_class_lv_obj) {
    .p, var
    init, func(lvbe_imgbtn_create)
    tostring, func(lvx_tostring)
    create, func(lvbe_imgbtn_create)
    set_src, func(lvbe_imgbtn_set_src)
    set_state, func(lvbe_imgbtn_set_state)
    toggle, func(lvbe_imgbtn_toggle)
    set_checkable, func(lvbe_imgbtn_set_checkable)
    get_src, func(lvbe_imgbtn_get_src)
    get_state, func(lvbe_imgbtn_get_state)
}
@const_object_info_end */

void be_load_lv_keyboard_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_keyboard_create },
    { "tostring", lvx_tostring },

    { "create", lvbe_keyboard_create },
    { "set_textarea", lvbe_keyboard_set_textarea },
    { "set_mode", lvbe_keyboard_set_mode },
    { "set_cursor_manage", lvbe_keyboard_set_cursor_manage },
    { "set_map", lvbe_keyboard_set_map },
    { "set_ctrl_map", lvbe_keyboard_set_ctrl_map },
    { "get_textarea", lvbe_keyboard_get_textarea },
    { "get_cursor_manage", lvbe_keyboard_get_cursor_manage },
    { "def_event_cb", lvbe_keyboard_def_event_cb },

    // { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */

    { NULL, NULL }
  };
  be_regclass(vm, "lv_keyboard", members);

  be_getglobal(vm, "lv_keyboard");
  be_getglobal(vm, "lv_obj");
  be_setsuper(vm, -2);
  be_pop(vm, 2);
#else
    be_pushntvclass(vm, &be_class_lv_keyboard);
    be_setglobal(vm, "lv_keyboard");
    be_pop(vm, 1);
#endif
};

/* @const_object_info_begin
class be_class_lv_keyboard (scope: global, name: lv_keyboard, super: be_class_lv_obj) {
    .p, var
    init, func(lvbe_keyboard_create)
    tostring, func(lvx_tostring)
    create, func(lvbe_keyboard_create)
    set_textarea, func(lvbe_keyboard_set_textarea)
    set_mode, func(lvbe_keyboard_set_mode)
    set_cursor_manage, func(lvbe_keyboard_set_cursor_manage)
    set_map, func(lvbe_keyboard_set_map)
    set_ctrl_map, func(lvbe_keyboard_set_ctrl_map)
    get_textarea, func(lvbe_keyboard_get_textarea)
    get_cursor_manage, func(lvbe_keyboard_get_cursor_manage)
    def_event_cb, func(lvbe_keyboard_def_event_cb)
}
@const_object_info_end */

void be_load_lv_label_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_label_create },
    { "tostring", lvx_tostring },

    { "create", lvbe_label_create },
    { "set_text", lvbe_label_set_text },
    { "set_text_fmt", lvbe_label_set_text_fmt },
    { "set_text_static", lvbe_label_set_text_static },
    { "set_long_mode", lvbe_label_set_long_mode },
    { "set_align", lvbe_label_set_align },
    { "set_recolor", lvbe_label_set_recolor },
    { "set_anim_speed", lvbe_label_set_anim_speed },
    { "set_text_sel_start", lvbe_label_set_text_sel_start },
    { "set_text_sel_end", lvbe_label_set_text_sel_end },
    { "get_text", lvbe_label_get_text },
    { "get_long_mode", lvbe_label_get_long_mode },
    { "get_align", lvbe_label_get_align },
    { "get_recolor", lvbe_label_get_recolor },
    { "get_anim_speed", lvbe_label_get_anim_speed },
    { "get_letter_pos", lvbe_label_get_letter_pos },
    { "get_letter_on", lvbe_label_get_letter_on },
    { "is_char_under_pos", lvbe_label_is_char_under_pos },
    { "get_text_sel_start", lvbe_label_get_text_sel_start },
    { "get_text_sel_end", lvbe_label_get_text_sel_end },
    { "ins_text", lvbe_label_ins_text },
    { "cut_text", lvbe_label_cut_text },
    { "refr_text", lvbe_label_refr_text },

    // { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */

    { NULL, NULL }
  };
  be_regclass(vm, "lv_label", members);

  be_getglobal(vm, "lv_label");
  be_getglobal(vm, "lv_obj");
  be_setsuper(vm, -2);
  be_pop(vm, 2);
#else
    be_pushntvclass(vm, &be_class_lv_label);
    be_setglobal(vm, "lv_label");
    be_pop(vm, 1);
#endif
};

/* @const_object_info_begin
class be_class_lv_label (scope: global, name: lv_label, super: be_class_lv_obj) {
    .p, var
    init, func(lvbe_label_create)
    tostring, func(lvx_tostring)
    create, func(lvbe_label_create)
    set_text, func(lvbe_label_set_text)
    set_text_fmt, func(lvbe_label_set_text_fmt)
    set_text_static, func(lvbe_label_set_text_static)
    set_long_mode, func(lvbe_label_set_long_mode)
    set_align, func(lvbe_label_set_align)
    set_recolor, func(lvbe_label_set_recolor)
    set_anim_speed, func(lvbe_label_set_anim_speed)
    set_text_sel_start, func(lvbe_label_set_text_sel_start)
    set_text_sel_end, func(lvbe_label_set_text_sel_end)
    get_text, func(lvbe_label_get_text)
    get_long_mode, func(lvbe_label_get_long_mode)
    get_align, func(lvbe_label_get_align)
    get_recolor, func(lvbe_label_get_recolor)
    get_anim_speed, func(lvbe_label_get_anim_speed)
    get_letter_pos, func(lvbe_label_get_letter_pos)
    get_letter_on, func(lvbe_label_get_letter_on)
    is_char_under_pos, func(lvbe_label_is_char_under_pos)
    get_text_sel_start, func(lvbe_label_get_text_sel_start)
    get_text_sel_end, func(lvbe_label_get_text_sel_end)
    ins_text, func(lvbe_label_ins_text)
    cut_text, func(lvbe_label_cut_text)
    refr_text, func(lvbe_label_refr_text)
}
@const_object_info_end */

void be_load_lv_led_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_led_create },
    { "tostring", lvx_tostring },

    { "create", lvbe_led_create },
    { "set_bright", lvbe_led_set_bright },
    { "on", lvbe_led_on },
    { "off", lvbe_led_off },
    { "toggle", lvbe_led_toggle },
    { "get_bright", lvbe_led_get_bright },

    // { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */

    { NULL, NULL }
  };
  be_regclass(vm, "lv_led", members);

  be_getglobal(vm, "lv_led");
  be_getglobal(vm, "lv_obj");
  be_setsuper(vm, -2);
  be_pop(vm, 2);
#else
    be_pushntvclass(vm, &be_class_lv_led);
    be_setglobal(vm, "lv_led");
    be_pop(vm, 1);
#endif
};

/* @const_object_info_begin
class be_class_lv_led (scope: global, name: lv_led, super: be_class_lv_obj) {
    .p, var
    init, func(lvbe_led_create)
    tostring, func(lvx_tostring)
    create, func(lvbe_led_create)
    set_bright, func(lvbe_led_set_bright)
    on, func(lvbe_led_on)
    off, func(lvbe_led_off)
    toggle, func(lvbe_led_toggle)
    get_bright, func(lvbe_led_get_bright)
}
@const_object_info_end */

void be_load_lv_line_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_line_create },
    { "tostring", lvx_tostring },

    { "create", lvbe_line_create },
    { "set_points", lvbe_line_set_points },
    { "set_auto_size", lvbe_line_set_auto_size },
    { "set_y_invert", lvbe_line_set_y_invert },
    { "get_auto_size", lvbe_line_get_auto_size },
    { "get_y_invert", lvbe_line_get_y_invert },

    // { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */

    { NULL, NULL }
  };
  be_regclass(vm, "lv_line", members);

  be_getglobal(vm, "lv_line");
  be_getglobal(vm, "lv_obj");
  be_setsuper(vm, -2);
  be_pop(vm, 2);
#else
    be_pushntvclass(vm, &be_class_lv_line);
    be_setglobal(vm, "lv_line");
    be_pop(vm, 1);
#endif
};

/* @const_object_info_begin
class be_class_lv_line (scope: global, name: lv_line, super: be_class_lv_obj) {
    .p, var
    init, func(lvbe_line_create)
    tostring, func(lvx_tostring)
    create, func(lvbe_line_create)
    set_points, func(lvbe_line_set_points)
    set_auto_size, func(lvbe_line_set_auto_size)
    set_y_invert, func(lvbe_line_set_y_invert)
    get_auto_size, func(lvbe_line_get_auto_size)
    get_y_invert, func(lvbe_line_get_y_invert)
}
@const_object_info_end */

void be_load_lv_linemeter_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_linemeter_create },
    { "tostring", lvx_tostring },

    { "create", lvbe_linemeter_create },
    { "set_value", lvbe_linemeter_set_value },
    { "set_range", lvbe_linemeter_set_range },
    { "set_scale", lvbe_linemeter_set_scale },
    { "set_angle_offset", lvbe_linemeter_set_angle_offset },
    { "set_mirror", lvbe_linemeter_set_mirror },
    { "get_value", lvbe_linemeter_get_value },
    { "get_min_value", lvbe_linemeter_get_min_value },
    { "get_max_value", lvbe_linemeter_get_max_value },
    { "get_line_count", lvbe_linemeter_get_line_count },
    { "get_scale_angle", lvbe_linemeter_get_scale_angle },
    { "get_angle_offset", lvbe_linemeter_get_angle_offset },
    { "draw_scale", lvbe_linemeter_draw_scale },
    { "get_mirror", lvbe_linemeter_get_mirror },

    // { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */

    { NULL, NULL }
  };
  be_regclass(vm, "lv_linemeter", members);

  be_getglobal(vm, "lv_linemeter");
  be_getglobal(vm, "lv_obj");
  be_setsuper(vm, -2);
  be_pop(vm, 2);
#else
    be_pushntvclass(vm, &be_class_lv_linemeter);
    be_setglobal(vm, "lv_linemeter");
    be_pop(vm, 1);
#endif
};

/* @const_object_info_begin
class be_class_lv_linemeter (scope: global, name: lv_linemeter, super: be_class_lv_obj) {
    .p, var
    init, func(lvbe_linemeter_create)
    tostring, func(lvx_tostring)
    create, func(lvbe_linemeter_create)
    set_value, func(lvbe_linemeter_set_value)
    set_range, func(lvbe_linemeter_set_range)
    set_scale, func(lvbe_linemeter_set_scale)
    set_angle_offset, func(lvbe_linemeter_set_angle_offset)
    set_mirror, func(lvbe_linemeter_set_mirror)
    get_value, func(lvbe_linemeter_get_value)
    get_min_value, func(lvbe_linemeter_get_min_value)
    get_max_value, func(lvbe_linemeter_get_max_value)
    get_line_count, func(lvbe_linemeter_get_line_count)
    get_scale_angle, func(lvbe_linemeter_get_scale_angle)
    get_angle_offset, func(lvbe_linemeter_get_angle_offset)
    draw_scale, func(lvbe_linemeter_draw_scale)
    get_mirror, func(lvbe_linemeter_get_mirror)
}
@const_object_info_end */

void be_load_lv_list_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_list_create },
    { "tostring", lvx_tostring },

    { "create", lvbe_list_create },
    { "clean", lvbe_list_clean },
    { "add_btn", lvbe_list_add_btn },
    { "remove", lvbe_list_remove },
    { "focus_btn", lvbe_list_focus_btn },
    { "set_scrollbar_mode", lvbe_list_set_scrollbar_mode },
    { "set_scroll_propagation", lvbe_list_set_scroll_propagation },
    { "set_edge_flash", lvbe_list_set_edge_flash },
    { "set_anim_time", lvbe_list_set_anim_time },
    { "set_layout", lvbe_list_set_layout },
    { "get_btn_text", lvbe_list_get_btn_text },
    { "get_btn_label", lvbe_list_get_btn_label },
    { "get_btn_img", lvbe_list_get_btn_img },
    { "get_prev_btn", lvbe_list_get_prev_btn },
    { "get_next_btn", lvbe_list_get_next_btn },
    { "get_btn_index", lvbe_list_get_btn_index },
    { "get_size", lvbe_list_get_size },
    { "get_btn_selected", lvbe_list_get_btn_selected },
    { "get_layout", lvbe_list_get_layout },
    { "get_scrollbar_mode", lvbe_list_get_scrollbar_mode },
    { "get_scroll_propagation", lvbe_list_get_scroll_propagation },
    { "get_edge_flash", lvbe_list_get_edge_flash },
    { "get_anim_time", lvbe_list_get_anim_time },
    { "up", lvbe_list_up },
    { "down", lvbe_list_down },
    { "focus", lvbe_list_focus },

    // { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */

    { NULL, NULL }
  };
  be_regclass(vm, "lv_list", members);

  be_getglobal(vm, "lv_list");
  be_getglobal(vm, "lv_obj");
  be_setsuper(vm, -2);
  be_pop(vm, 2);
#else
    be_pushntvclass(vm, &be_class_lv_list);
    be_setglobal(vm, "lv_list");
    be_pop(vm, 1);
#endif
};

/* @const_object_info_begin
class be_class_lv_list (scope: global, name: lv_list, super: be_class_lv_obj) {
    .p, var
    init, func(lvbe_list_create)
    tostring, func(lvx_tostring)
    create, func(lvbe_list_create)
    clean, func(lvbe_list_clean)
    add_btn, func(lvbe_list_add_btn)
    remove, func(lvbe_list_remove)
    focus_btn, func(lvbe_list_focus_btn)
    set_scrollbar_mode, func(lvbe_list_set_scrollbar_mode)
    set_scroll_propagation, func(lvbe_list_set_scroll_propagation)
    set_edge_flash, func(lvbe_list_set_edge_flash)
    set_anim_time, func(lvbe_list_set_anim_time)
    set_layout, func(lvbe_list_set_layout)
    get_btn_text, func(lvbe_list_get_btn_text)
    get_btn_label, func(lvbe_list_get_btn_label)
    get_btn_img, func(lvbe_list_get_btn_img)
    get_prev_btn, func(lvbe_list_get_prev_btn)
    get_next_btn, func(lvbe_list_get_next_btn)
    get_btn_index, func(lvbe_list_get_btn_index)
    get_size, func(lvbe_list_get_size)
    get_btn_selected, func(lvbe_list_get_btn_selected)
    get_layout, func(lvbe_list_get_layout)
    get_scrollbar_mode, func(lvbe_list_get_scrollbar_mode)
    get_scroll_propagation, func(lvbe_list_get_scroll_propagation)
    get_edge_flash, func(lvbe_list_get_edge_flash)
    get_anim_time, func(lvbe_list_get_anim_time)
    up, func(lvbe_list_up)
    down, func(lvbe_list_down)
    focus, func(lvbe_list_focus)
}
@const_object_info_end */

void be_load_lv_msgbox_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_msgbox_create },
    { "tostring", lvx_tostring },

    { "create", lvbe_msgbox_create },
    { "add_btns", lvbe_msgbox_add_btns },
    { "set_text", lvbe_msgbox_set_text },
    { "set_text_fmt", lvbe_msgbox_set_text_fmt },
    { "set_anim_time", lvbe_msgbox_set_anim_time },
    { "start_auto_close", lvbe_msgbox_start_auto_close },
    { "stop_auto_close", lvbe_msgbox_stop_auto_close },
    { "set_recolor", lvbe_msgbox_set_recolor },
    { "get_text", lvbe_msgbox_get_text },
    { "get_active_btn", lvbe_msgbox_get_active_btn },
    { "get_active_btn_text", lvbe_msgbox_get_active_btn_text },
    { "get_anim_time", lvbe_msgbox_get_anim_time },
    { "get_recolor", lvbe_msgbox_get_recolor },
    { "get_btnmatrix", lvbe_msgbox_get_btnmatrix },

    // { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */

    { NULL, NULL }
  };
  be_regclass(vm, "lv_msgbox", members);

  be_getglobal(vm, "lv_msgbox");
  be_getglobal(vm, "lv_obj");
  be_setsuper(vm, -2);
  be_pop(vm, 2);
#else
    be_pushntvclass(vm, &be_class_lv_msgbox);
    be_setglobal(vm, "lv_msgbox");
    be_pop(vm, 1);
#endif
};

/* @const_object_info_begin
class be_class_lv_msgbox (scope: global, name: lv_msgbox, super: be_class_lv_obj) {
    .p, var
    init, func(lvbe_msgbox_create)
    tostring, func(lvx_tostring)
    create, func(lvbe_msgbox_create)
    add_btns, func(lvbe_msgbox_add_btns)
    set_text, func(lvbe_msgbox_set_text)
    set_text_fmt, func(lvbe_msgbox_set_text_fmt)
    set_anim_time, func(lvbe_msgbox_set_anim_time)
    start_auto_close, func(lvbe_msgbox_start_auto_close)
    stop_auto_close, func(lvbe_msgbox_stop_auto_close)
    set_recolor, func(lvbe_msgbox_set_recolor)
    get_text, func(lvbe_msgbox_get_text)
    get_active_btn, func(lvbe_msgbox_get_active_btn)
    get_active_btn_text, func(lvbe_msgbox_get_active_btn_text)
    get_anim_time, func(lvbe_msgbox_get_anim_time)
    get_recolor, func(lvbe_msgbox_get_recolor)
    get_btnmatrix, func(lvbe_msgbox_get_btnmatrix)
}
@const_object_info_end */

void be_load_lv_objmask_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_objmask_create },
    { "tostring", lvx_tostring },

    { "create", lvbe_objmask_create },
    { "update_mask", lvbe_objmask_update_mask },
    { "remove_mask", lvbe_objmask_remove_mask },

    // { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */

    { NULL, NULL }
  };
  be_regclass(vm, "lv_objmask", members);

  be_getglobal(vm, "lv_objmask");
  be_getglobal(vm, "lv_obj");
  be_setsuper(vm, -2);
  be_pop(vm, 2);
#else
    be_pushntvclass(vm, &be_class_lv_objmask);
    be_setglobal(vm, "lv_objmask");
    be_pop(vm, 1);
#endif
};

/* @const_object_info_begin
class be_class_lv_objmask (scope: global, name: lv_objmask, super: be_class_lv_obj) {
    .p, var
    init, func(lvbe_objmask_create)
    tostring, func(lvx_tostring)
    create, func(lvbe_objmask_create)
    update_mask, func(lvbe_objmask_update_mask)
    remove_mask, func(lvbe_objmask_remove_mask)
}
@const_object_info_end */

void be_load_lv_page_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_page_create },
    { "tostring", lvx_tostring },

    { "create", lvbe_page_create },
    { "clean", lvbe_page_clean },
    { "get_scrollable", lvbe_page_get_scrollable },
    { "get_anim_time", lvbe_page_get_anim_time },
    { "set_scrollbar_mode", lvbe_page_set_scrollbar_mode },
    { "set_anim_time", lvbe_page_set_anim_time },
    { "set_scroll_propagation", lvbe_page_set_scroll_propagation },
    { "set_edge_flash", lvbe_page_set_edge_flash },
    { "set_scrollable_fit2", lvbe_page_set_scrollable_fit2 },
    { "set_scrollable_fit", lvbe_page_set_scrollable_fit },
    { "set_scrl_width", lvbe_page_set_scrl_width },
    { "set_scrl_height", lvbe_page_set_scrl_height },
    { "set_scrl_layout", lvbe_page_set_scrl_layout },
    { "get_scrollbar_mode", lvbe_page_get_scrollbar_mode },
    { "get_scroll_propagation", lvbe_page_get_scroll_propagation },
    { "get_edge_flash", lvbe_page_get_edge_flash },
    { "get_width_fit", lvbe_page_get_width_fit },
    { "get_height_fit", lvbe_page_get_height_fit },
    { "get_width_grid", lvbe_page_get_width_grid },
    { "get_height_grid", lvbe_page_get_height_grid },
    { "get_scrl_width", lvbe_page_get_scrl_width },
    { "get_scrl_height", lvbe_page_get_scrl_height },
    { "get_scrl_layout", lvbe_page_get_scrl_layout },
    { "get_scrl_fit_left", lvbe_page_get_scrl_fit_left },
    { "get_scrl_fit_right", lvbe_page_get_scrl_fit_right },
    { "get_scrl_fit_top", lvbe_page_get_scrl_fit_top },
    { "get_scrl_fit_bottom", lvbe_page_get_scrl_fit_bottom },
    { "on_edge", lvbe_page_on_edge },
    { "glue_obj", lvbe_page_glue_obj },
    { "focus", lvbe_page_focus },
    { "scroll_hor", lvbe_page_scroll_hor },
    { "scroll_ver", lvbe_page_scroll_ver },
    { "start_edge_flash", lvbe_page_start_edge_flash },

    // { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */

    { NULL, NULL }
  };
  be_regclass(vm, "lv_page", members);

  be_getglobal(vm, "lv_page");
  be_getglobal(vm, "lv_obj");
  be_setsuper(vm, -2);
  be_pop(vm, 2);
#else
    be_pushntvclass(vm, &be_class_lv_page);
    be_setglobal(vm, "lv_page");
    be_pop(vm, 1);
#endif
};

/* @const_object_info_begin
class be_class_lv_page (scope: global, name: lv_page, super: be_class_lv_obj) {
    .p, var
    init, func(lvbe_page_create)
    tostring, func(lvx_tostring)
    create, func(lvbe_page_create)
    clean, func(lvbe_page_clean)
    get_scrollable, func(lvbe_page_get_scrollable)
    get_anim_time, func(lvbe_page_get_anim_time)
    set_scrollbar_mode, func(lvbe_page_set_scrollbar_mode)
    set_anim_time, func(lvbe_page_set_anim_time)
    set_scroll_propagation, func(lvbe_page_set_scroll_propagation)
    set_edge_flash, func(lvbe_page_set_edge_flash)
    set_scrollable_fit2, func(lvbe_page_set_scrollable_fit2)
    set_scrollable_fit, func(lvbe_page_set_scrollable_fit)
    set_scrl_width, func(lvbe_page_set_scrl_width)
    set_scrl_height, func(lvbe_page_set_scrl_height)
    set_scrl_layout, func(lvbe_page_set_scrl_layout)
    get_scrollbar_mode, func(lvbe_page_get_scrollbar_mode)
    get_scroll_propagation, func(lvbe_page_get_scroll_propagation)
    get_edge_flash, func(lvbe_page_get_edge_flash)
    get_width_fit, func(lvbe_page_get_width_fit)
    get_height_fit, func(lvbe_page_get_height_fit)
    get_width_grid, func(lvbe_page_get_width_grid)
    get_height_grid, func(lvbe_page_get_height_grid)
    get_scrl_width, func(lvbe_page_get_scrl_width)
    get_scrl_height, func(lvbe_page_get_scrl_height)
    get_scrl_layout, func(lvbe_page_get_scrl_layout)
    get_scrl_fit_left, func(lvbe_page_get_scrl_fit_left)
    get_scrl_fit_right, func(lvbe_page_get_scrl_fit_right)
    get_scrl_fit_top, func(lvbe_page_get_scrl_fit_top)
    get_scrl_fit_bottom, func(lvbe_page_get_scrl_fit_bottom)
    on_edge, func(lvbe_page_on_edge)
    glue_obj, func(lvbe_page_glue_obj)
    focus, func(lvbe_page_focus)
    scroll_hor, func(lvbe_page_scroll_hor)
    scroll_ver, func(lvbe_page_scroll_ver)
    start_edge_flash, func(lvbe_page_start_edge_flash)
}
@const_object_info_end */

void be_load_lv_roller_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_roller_create },
    { "tostring", lvx_tostring },

    { "create", lvbe_roller_create },
    { "set_options", lvbe_roller_set_options },
    { "set_align", lvbe_roller_set_align },
    { "set_selected", lvbe_roller_set_selected },
    { "set_visible_row_count", lvbe_roller_set_visible_row_count },
    { "set_auto_fit", lvbe_roller_set_auto_fit },
    { "set_anim_time", lvbe_roller_set_anim_time },
    { "get_selected", lvbe_roller_get_selected },
    { "get_option_cnt", lvbe_roller_get_option_cnt },
    { "get_selected_str", lvbe_roller_get_selected_str },
    { "get_align", lvbe_roller_get_align },
    { "get_auto_fit", lvbe_roller_get_auto_fit },
    { "get_options", lvbe_roller_get_options },
    { "get_anim_time", lvbe_roller_get_anim_time },

    // { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */

    { NULL, NULL }
  };
  be_regclass(vm, "lv_roller", members);

  be_getglobal(vm, "lv_roller");
  be_getglobal(vm, "lv_obj");
  be_setsuper(vm, -2);
  be_pop(vm, 2);
#else
    be_pushntvclass(vm, &be_class_lv_roller);
    be_setglobal(vm, "lv_roller");
    be_pop(vm, 1);
#endif
};

/* @const_object_info_begin
class be_class_lv_roller (scope: global, name: lv_roller, super: be_class_lv_obj) {
    .p, var
    init, func(lvbe_roller_create)
    tostring, func(lvx_tostring)
    create, func(lvbe_roller_create)
    set_options, func(lvbe_roller_set_options)
    set_align, func(lvbe_roller_set_align)
    set_selected, func(lvbe_roller_set_selected)
    set_visible_row_count, func(lvbe_roller_set_visible_row_count)
    set_auto_fit, func(lvbe_roller_set_auto_fit)
    set_anim_time, func(lvbe_roller_set_anim_time)
    get_selected, func(lvbe_roller_get_selected)
    get_option_cnt, func(lvbe_roller_get_option_cnt)
    get_selected_str, func(lvbe_roller_get_selected_str)
    get_align, func(lvbe_roller_get_align)
    get_auto_fit, func(lvbe_roller_get_auto_fit)
    get_options, func(lvbe_roller_get_options)
    get_anim_time, func(lvbe_roller_get_anim_time)
}
@const_object_info_end */

void be_load_lv_slider_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_slider_create },
    { "tostring", lvx_tostring },

    { "create", lvbe_slider_create },
    { "set_value", lvbe_slider_set_value },
    { "set_left_value", lvbe_slider_set_left_value },
    { "set_range", lvbe_slider_set_range },
    { "set_anim_time", lvbe_slider_set_anim_time },
    { "set_type", lvbe_slider_set_type },
    { "get_value", lvbe_slider_get_value },
    { "get_left_value", lvbe_slider_get_left_value },
    { "get_min_value", lvbe_slider_get_min_value },
    { "get_max_value", lvbe_slider_get_max_value },
    { "is_dragged", lvbe_slider_is_dragged },
    { "get_anim_time", lvbe_slider_get_anim_time },
    { "get_type", lvbe_slider_get_type },

    // { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */

    { NULL, NULL }
  };
  be_regclass(vm, "lv_slider", members);

  be_getglobal(vm, "lv_slider");
  be_getglobal(vm, "lv_obj");
  be_setsuper(vm, -2);
  be_pop(vm, 2);
#else
    be_pushntvclass(vm, &be_class_lv_slider);
    be_setglobal(vm, "lv_slider");
    be_pop(vm, 1);
#endif
};

/* @const_object_info_begin
class be_class_lv_slider (scope: global, name: lv_slider, super: be_class_lv_obj) {
    .p, var
    init, func(lvbe_slider_create)
    tostring, func(lvx_tostring)
    create, func(lvbe_slider_create)
    set_value, func(lvbe_slider_set_value)
    set_left_value, func(lvbe_slider_set_left_value)
    set_range, func(lvbe_slider_set_range)
    set_anim_time, func(lvbe_slider_set_anim_time)
    set_type, func(lvbe_slider_set_type)
    get_value, func(lvbe_slider_get_value)
    get_left_value, func(lvbe_slider_get_left_value)
    get_min_value, func(lvbe_slider_get_min_value)
    get_max_value, func(lvbe_slider_get_max_value)
    is_dragged, func(lvbe_slider_is_dragged)
    get_anim_time, func(lvbe_slider_get_anim_time)
    get_type, func(lvbe_slider_get_type)
}
@const_object_info_end */

void be_load_lv_spinbox_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_spinbox_create },
    { "tostring", lvx_tostring },

    { "create", lvbe_spinbox_create },
    { "set_rollover", lvbe_spinbox_set_rollover },
    { "set_value", lvbe_spinbox_set_value },
    { "set_digit_format", lvbe_spinbox_set_digit_format },
    { "set_step", lvbe_spinbox_set_step },
    { "set_range", lvbe_spinbox_set_range },
    { "set_padding_left", lvbe_spinbox_set_padding_left },
    { "get_rollover", lvbe_spinbox_get_rollover },
    { "get_value", lvbe_spinbox_get_value },
    { "get_step", lvbe_spinbox_get_step },
    { "step_next", lvbe_spinbox_step_next },
    { "step_prev", lvbe_spinbox_step_prev },
    { "increment", lvbe_spinbox_increment },
    { "decrement", lvbe_spinbox_decrement },

    // { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */

    { NULL, NULL }
  };
  be_regclass(vm, "lv_spinbox", members);

  be_getglobal(vm, "lv_spinbox");
  be_getglobal(vm, "lv_obj");
  be_setsuper(vm, -2);
  be_pop(vm, 2);
#else
    be_pushntvclass(vm, &be_class_lv_spinbox);
    be_setglobal(vm, "lv_spinbox");
    be_pop(vm, 1);
#endif
};

/* @const_object_info_begin
class be_class_lv_spinbox (scope: global, name: lv_spinbox, super: be_class_lv_obj) {
    .p, var
    init, func(lvbe_spinbox_create)
    tostring, func(lvx_tostring)
    create, func(lvbe_spinbox_create)
    set_rollover, func(lvbe_spinbox_set_rollover)
    set_value, func(lvbe_spinbox_set_value)
    set_digit_format, func(lvbe_spinbox_set_digit_format)
    set_step, func(lvbe_spinbox_set_step)
    set_range, func(lvbe_spinbox_set_range)
    set_padding_left, func(lvbe_spinbox_set_padding_left)
    get_rollover, func(lvbe_spinbox_get_rollover)
    get_value, func(lvbe_spinbox_get_value)
    get_step, func(lvbe_spinbox_get_step)
    step_next, func(lvbe_spinbox_step_next)
    step_prev, func(lvbe_spinbox_step_prev)
    increment, func(lvbe_spinbox_increment)
    decrement, func(lvbe_spinbox_decrement)
}
@const_object_info_end */

void be_load_lv_spinner_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_spinner_create },
    { "tostring", lvx_tostring },

    { "create", lvbe_spinner_create },
    { "set_arc_length", lvbe_spinner_set_arc_length },
    { "set_spin_time", lvbe_spinner_set_spin_time },
    { "set_type", lvbe_spinner_set_type },
    { "set_dir", lvbe_spinner_set_dir },
    { "get_arc_length", lvbe_spinner_get_arc_length },
    { "get_spin_time", lvbe_spinner_get_spin_time },
    { "get_type", lvbe_spinner_get_type },
    { "get_dir", lvbe_spinner_get_dir },
    { "anim_cb", lvbe_spinner_anim_cb },

    // { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */

    { NULL, NULL }
  };
  be_regclass(vm, "lv_spinner", members);

  be_getglobal(vm, "lv_spinner");
  be_getglobal(vm, "lv_obj");
  be_setsuper(vm, -2);
  be_pop(vm, 2);
#else
    be_pushntvclass(vm, &be_class_lv_spinner);
    be_setglobal(vm, "lv_spinner");
    be_pop(vm, 1);
#endif
};

/* @const_object_info_begin
class be_class_lv_spinner (scope: global, name: lv_spinner, super: be_class_lv_obj) {
    .p, var
    init, func(lvbe_spinner_create)
    tostring, func(lvx_tostring)
    create, func(lvbe_spinner_create)
    set_arc_length, func(lvbe_spinner_set_arc_length)
    set_spin_time, func(lvbe_spinner_set_spin_time)
    set_type, func(lvbe_spinner_set_type)
    set_dir, func(lvbe_spinner_set_dir)
    get_arc_length, func(lvbe_spinner_get_arc_length)
    get_spin_time, func(lvbe_spinner_get_spin_time)
    get_type, func(lvbe_spinner_get_type)
    get_dir, func(lvbe_spinner_get_dir)
    anim_cb, func(lvbe_spinner_anim_cb)
}
@const_object_info_end */

void be_load_lv_switch_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_switch_create },
    { "tostring", lvx_tostring },

    { "create", lvbe_switch_create },
    { "on", lvbe_switch_on },
    { "off", lvbe_switch_off },
    { "toggle", lvbe_switch_toggle },
    { "set_anim_time", lvbe_switch_set_anim_time },
    { "get_state", lvbe_switch_get_state },
    { "get_anim_time", lvbe_switch_get_anim_time },

    // { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */

    { NULL, NULL }
  };
  be_regclass(vm, "lv_switch", members);

  be_getglobal(vm, "lv_switch");
  be_getglobal(vm, "lv_obj");
  be_setsuper(vm, -2);
  be_pop(vm, 2);
#else
    be_pushntvclass(vm, &be_class_lv_switch);
    be_setglobal(vm, "lv_switch");
    be_pop(vm, 1);
#endif
};

/* @const_object_info_begin
class be_class_lv_switch (scope: global, name: lv_switch, super: be_class_lv_obj) {
    .p, var
    init, func(lvbe_switch_create)
    tostring, func(lvx_tostring)
    create, func(lvbe_switch_create)
    on, func(lvbe_switch_on)
    off, func(lvbe_switch_off)
    toggle, func(lvbe_switch_toggle)
    set_anim_time, func(lvbe_switch_set_anim_time)
    get_state, func(lvbe_switch_get_state)
    get_anim_time, func(lvbe_switch_get_anim_time)
}
@const_object_info_end */

void be_load_lv_table_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_table_create },
    { "tostring", lvx_tostring },

    { "create", lvbe_table_create },
    { "set_cell_value", lvbe_table_set_cell_value },
    { "set_cell_value_fmt", lvbe_table_set_cell_value_fmt },
    { "set_row_cnt", lvbe_table_set_row_cnt },
    { "set_col_cnt", lvbe_table_set_col_cnt },
    { "set_col_width", lvbe_table_set_col_width },
    { "set_cell_align", lvbe_table_set_cell_align },
    { "set_cell_type", lvbe_table_set_cell_type },
    { "set_cell_crop", lvbe_table_set_cell_crop },
    { "set_cell_merge_right", lvbe_table_set_cell_merge_right },
    { "get_cell_value", lvbe_table_get_cell_value },
    { "get_row_cnt", lvbe_table_get_row_cnt },
    { "get_col_cnt", lvbe_table_get_col_cnt },
    { "get_col_width", lvbe_table_get_col_width },
    { "get_cell_align", lvbe_table_get_cell_align },
    { "get_cell_type", lvbe_table_get_cell_type },
    { "get_cell_crop", lvbe_table_get_cell_crop },
    { "get_cell_merge_right", lvbe_table_get_cell_merge_right },
    { "get_pressed_cell", lvbe_table_get_pressed_cell },

    // { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */

    { NULL, NULL }
  };
  be_regclass(vm, "lv_table", members);

  be_getglobal(vm, "lv_table");
  be_getglobal(vm, "lv_obj");
  be_setsuper(vm, -2);
  be_pop(vm, 2);
#else
    be_pushntvclass(vm, &be_class_lv_table);
    be_setglobal(vm, "lv_table");
    be_pop(vm, 1);
#endif
};

/* @const_object_info_begin
class be_class_lv_table (scope: global, name: lv_table, super: be_class_lv_obj) {
    .p, var
    init, func(lvbe_table_create)
    tostring, func(lvx_tostring)
    create, func(lvbe_table_create)
    set_cell_value, func(lvbe_table_set_cell_value)
    set_cell_value_fmt, func(lvbe_table_set_cell_value_fmt)
    set_row_cnt, func(lvbe_table_set_row_cnt)
    set_col_cnt, func(lvbe_table_set_col_cnt)
    set_col_width, func(lvbe_table_set_col_width)
    set_cell_align, func(lvbe_table_set_cell_align)
    set_cell_type, func(lvbe_table_set_cell_type)
    set_cell_crop, func(lvbe_table_set_cell_crop)
    set_cell_merge_right, func(lvbe_table_set_cell_merge_right)
    get_cell_value, func(lvbe_table_get_cell_value)
    get_row_cnt, func(lvbe_table_get_row_cnt)
    get_col_cnt, func(lvbe_table_get_col_cnt)
    get_col_width, func(lvbe_table_get_col_width)
    get_cell_align, func(lvbe_table_get_cell_align)
    get_cell_type, func(lvbe_table_get_cell_type)
    get_cell_crop, func(lvbe_table_get_cell_crop)
    get_cell_merge_right, func(lvbe_table_get_cell_merge_right)
    get_pressed_cell, func(lvbe_table_get_pressed_cell)
}
@const_object_info_end */

void be_load_lv_tabview_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_tabview_create },
    { "tostring", lvx_tostring },

    { "create", lvbe_tabview_create },
    { "add_tab", lvbe_tabview_add_tab },
    { "clean_tab", lvbe_tabview_clean_tab },
    { "set_tab_act", lvbe_tabview_set_tab_act },
    { "set_tab_name", lvbe_tabview_set_tab_name },
    { "set_anim_time", lvbe_tabview_set_anim_time },
    { "set_btns_pos", lvbe_tabview_set_btns_pos },
    { "get_tab_act", lvbe_tabview_get_tab_act },
    { "get_tab_count", lvbe_tabview_get_tab_count },
    { "get_tab", lvbe_tabview_get_tab },
    { "get_anim_time", lvbe_tabview_get_anim_time },
    { "get_btns_pos", lvbe_tabview_get_btns_pos },

    // { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */

    { NULL, NULL }
  };
  be_regclass(vm, "lv_tabview", members);

  be_getglobal(vm, "lv_tabview");
  be_getglobal(vm, "lv_obj");
  be_setsuper(vm, -2);
  be_pop(vm, 2);
#else
    be_pushntvclass(vm, &be_class_lv_tabview);
    be_setglobal(vm, "lv_tabview");
    be_pop(vm, 1);
#endif
};

/* @const_object_info_begin
class be_class_lv_tabview (scope: global, name: lv_tabview, super: be_class_lv_obj) {
    .p, var
    init, func(lvbe_tabview_create)
    tostring, func(lvx_tostring)
    create, func(lvbe_tabview_create)
    add_tab, func(lvbe_tabview_add_tab)
    clean_tab, func(lvbe_tabview_clean_tab)
    set_tab_act, func(lvbe_tabview_set_tab_act)
    set_tab_name, func(lvbe_tabview_set_tab_name)
    set_anim_time, func(lvbe_tabview_set_anim_time)
    set_btns_pos, func(lvbe_tabview_set_btns_pos)
    get_tab_act, func(lvbe_tabview_get_tab_act)
    get_tab_count, func(lvbe_tabview_get_tab_count)
    get_tab, func(lvbe_tabview_get_tab)
    get_anim_time, func(lvbe_tabview_get_anim_time)
    get_btns_pos, func(lvbe_tabview_get_btns_pos)
}
@const_object_info_end */

void be_load_lv_textarea_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_textarea_create },
    { "tostring", lvx_tostring },

    { "create", lvbe_textarea_create },
    { "add_char", lvbe_textarea_add_char },
    { "add_text", lvbe_textarea_add_text },
    { "del_char", lvbe_textarea_del_char },
    { "del_char_forward", lvbe_textarea_del_char_forward },
    { "set_text", lvbe_textarea_set_text },
    { "set_placeholder_text", lvbe_textarea_set_placeholder_text },
    { "set_cursor_pos", lvbe_textarea_set_cursor_pos },
    { "set_cursor_hidden", lvbe_textarea_set_cursor_hidden },
    { "set_cursor_click_pos", lvbe_textarea_set_cursor_click_pos },
    { "set_pwd_mode", lvbe_textarea_set_pwd_mode },
    { "set_one_line", lvbe_textarea_set_one_line },
    { "set_text_align", lvbe_textarea_set_text_align },
    { "set_accepted_chars", lvbe_textarea_set_accepted_chars },
    { "set_max_length", lvbe_textarea_set_max_length },
    { "set_insert_replace", lvbe_textarea_set_insert_replace },
    { "set_scrollbar_mode", lvbe_textarea_set_scrollbar_mode },
    { "set_scroll_propagation", lvbe_textarea_set_scroll_propagation },
    { "set_edge_flash", lvbe_textarea_set_edge_flash },
    { "set_text_sel", lvbe_textarea_set_text_sel },
    { "set_pwd_show_time", lvbe_textarea_set_pwd_show_time },
    { "set_cursor_blink_time", lvbe_textarea_set_cursor_blink_time },
    { "get_text", lvbe_textarea_get_text },
    { "get_placeholder_text", lvbe_textarea_get_placeholder_text },
    { "get_label", lvbe_textarea_get_label },
    { "get_cursor_pos", lvbe_textarea_get_cursor_pos },
    { "get_cursor_hidden", lvbe_textarea_get_cursor_hidden },
    { "get_cursor_click_pos", lvbe_textarea_get_cursor_click_pos },
    { "get_pwd_mode", lvbe_textarea_get_pwd_mode },
    { "get_one_line", lvbe_textarea_get_one_line },
    { "get_accepted_chars", lvbe_textarea_get_accepted_chars },
    { "get_max_length", lvbe_textarea_get_max_length },
    { "get_scrollbar_mode", lvbe_textarea_get_scrollbar_mode },
    { "get_scroll_propagation", lvbe_textarea_get_scroll_propagation },
    { "get_edge_flash", lvbe_textarea_get_edge_flash },
    { "text_is_selected", lvbe_textarea_text_is_selected },
    { "get_text_sel_en", lvbe_textarea_get_text_sel_en },
    { "get_pwd_show_time", lvbe_textarea_get_pwd_show_time },
    { "get_cursor_blink_time", lvbe_textarea_get_cursor_blink_time },
    { "clear_selection", lvbe_textarea_clear_selection },
    { "cursor_right", lvbe_textarea_cursor_right },
    { "cursor_left", lvbe_textarea_cursor_left },
    { "cursor_down", lvbe_textarea_cursor_down },
    { "cursor_up", lvbe_textarea_cursor_up },

    // { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */

    { NULL, NULL }
  };
  be_regclass(vm, "lv_textarea", members);

  be_getglobal(vm, "lv_textarea");
  be_getglobal(vm, "lv_obj");
  be_setsuper(vm, -2);
  be_pop(vm, 2);
#else
    be_pushntvclass(vm, &be_class_lv_textarea);
    be_setglobal(vm, "lv_textarea");
    be_pop(vm, 1);
#endif
};

/* @const_object_info_begin
class be_class_lv_textarea (scope: global, name: lv_textarea, super: be_class_lv_obj) {
    .p, var
    init, func(lvbe_textarea_create)
    tostring, func(lvx_tostring)
    create, func(lvbe_textarea_create)
    add_char, func(lvbe_textarea_add_char)
    add_text, func(lvbe_textarea_add_text)
    del_char, func(lvbe_textarea_del_char)
    del_char_forward, func(lvbe_textarea_del_char_forward)
    set_text, func(lvbe_textarea_set_text)
    set_placeholder_text, func(lvbe_textarea_set_placeholder_text)
    set_cursor_pos, func(lvbe_textarea_set_cursor_pos)
    set_cursor_hidden, func(lvbe_textarea_set_cursor_hidden)
    set_cursor_click_pos, func(lvbe_textarea_set_cursor_click_pos)
    set_pwd_mode, func(lvbe_textarea_set_pwd_mode)
    set_one_line, func(lvbe_textarea_set_one_line)
    set_text_align, func(lvbe_textarea_set_text_align)
    set_accepted_chars, func(lvbe_textarea_set_accepted_chars)
    set_max_length, func(lvbe_textarea_set_max_length)
    set_insert_replace, func(lvbe_textarea_set_insert_replace)
    set_scrollbar_mode, func(lvbe_textarea_set_scrollbar_mode)
    set_scroll_propagation, func(lvbe_textarea_set_scroll_propagation)
    set_edge_flash, func(lvbe_textarea_set_edge_flash)
    set_text_sel, func(lvbe_textarea_set_text_sel)
    set_pwd_show_time, func(lvbe_textarea_set_pwd_show_time)
    set_cursor_blink_time, func(lvbe_textarea_set_cursor_blink_time)
    get_text, func(lvbe_textarea_get_text)
    get_placeholder_text, func(lvbe_textarea_get_placeholder_text)
    get_label, func(lvbe_textarea_get_label)
    get_cursor_pos, func(lvbe_textarea_get_cursor_pos)
    get_cursor_hidden, func(lvbe_textarea_get_cursor_hidden)
    get_cursor_click_pos, func(lvbe_textarea_get_cursor_click_pos)
    get_pwd_mode, func(lvbe_textarea_get_pwd_mode)
    get_one_line, func(lvbe_textarea_get_one_line)
    get_accepted_chars, func(lvbe_textarea_get_accepted_chars)
    get_max_length, func(lvbe_textarea_get_max_length)
    get_scrollbar_mode, func(lvbe_textarea_get_scrollbar_mode)
    get_scroll_propagation, func(lvbe_textarea_get_scroll_propagation)
    get_edge_flash, func(lvbe_textarea_get_edge_flash)
    text_is_selected, func(lvbe_textarea_text_is_selected)
    get_text_sel_en, func(lvbe_textarea_get_text_sel_en)
    get_pwd_show_time, func(lvbe_textarea_get_pwd_show_time)
    get_cursor_blink_time, func(lvbe_textarea_get_cursor_blink_time)
    clear_selection, func(lvbe_textarea_clear_selection)
    cursor_right, func(lvbe_textarea_cursor_right)
    cursor_left, func(lvbe_textarea_cursor_left)
    cursor_down, func(lvbe_textarea_cursor_down)
    cursor_up, func(lvbe_textarea_cursor_up)
}
@const_object_info_end */

void be_load_lv_tileview_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_tileview_create },
    { "tostring", lvx_tostring },

    { "create", lvbe_tileview_create },
    { "add_element", lvbe_tileview_add_element },
    { "set_valid_positions", lvbe_tileview_set_valid_positions },
    { "set_tile_act", lvbe_tileview_set_tile_act },
    { "set_edge_flash", lvbe_tileview_set_edge_flash },
    { "set_anim_time", lvbe_tileview_set_anim_time },
    { "get_tile_act", lvbe_tileview_get_tile_act },
    { "get_edge_flash", lvbe_tileview_get_edge_flash },
    { "get_anim_time", lvbe_tileview_get_anim_time },

    // { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */

    { NULL, NULL }
  };
  be_regclass(vm, "lv_tileview", members);

  be_getglobal(vm, "lv_tileview");
  be_getglobal(vm, "lv_obj");
  be_setsuper(vm, -2);
  be_pop(vm, 2);
#else
    be_pushntvclass(vm, &be_class_lv_tileview);
    be_setglobal(vm, "lv_tileview");
    be_pop(vm, 1);
#endif
};

/* @const_object_info_begin
class be_class_lv_tileview (scope: global, name: lv_tileview, super: be_class_lv_obj) {
    .p, var
    init, func(lvbe_tileview_create)
    tostring, func(lvx_tostring)
    create, func(lvbe_tileview_create)
    add_element, func(lvbe_tileview_add_element)
    set_valid_positions, func(lvbe_tileview_set_valid_positions)
    set_tile_act, func(lvbe_tileview_set_tile_act)
    set_edge_flash, func(lvbe_tileview_set_edge_flash)
    set_anim_time, func(lvbe_tileview_set_anim_time)
    get_tile_act, func(lvbe_tileview_get_tile_act)
    get_edge_flash, func(lvbe_tileview_get_edge_flash)
    get_anim_time, func(lvbe_tileview_get_anim_time)
}
@const_object_info_end */

void be_load_lv_win_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_win_create },
    { "tostring", lvx_tostring },

    { "create", lvbe_win_create },
    { "clean", lvbe_win_clean },
    { "add_btn_right", lvbe_win_add_btn_right },
    { "add_btn_left", lvbe_win_add_btn_left },
    { "close_event_cb", lvbe_win_close_event_cb },
    { "set_title", lvbe_win_set_title },
    { "set_header_height", lvbe_win_set_header_height },
    { "set_btn_width", lvbe_win_set_btn_width },
    { "set_content_size", lvbe_win_set_content_size },
    { "set_layout", lvbe_win_set_layout },
    { "set_scrollbar_mode", lvbe_win_set_scrollbar_mode },
    { "set_anim_time", lvbe_win_set_anim_time },
    { "set_drag", lvbe_win_set_drag },
    { "title_set_alignment", lvbe_win_title_set_alignment },
    { "get_title", lvbe_win_get_title },
    { "get_content", lvbe_win_get_content },
    { "get_header_height", lvbe_win_get_header_height },
    { "get_btn_width", lvbe_win_get_btn_width },
    { "get_from_btn", lvbe_win_get_from_btn },
    { "get_layout", lvbe_win_get_layout },
    { "get_sb_mode", lvbe_win_get_sb_mode },
    { "get_anim_time", lvbe_win_get_anim_time },
    { "get_width", lvbe_win_get_width },
    { "get_drag", lvbe_win_get_drag },
    { "title_get_alignment", lvbe_win_title_get_alignment },
    { "focus", lvbe_win_focus },
    { "scroll_hor", lvbe_win_scroll_hor },
    { "scroll_ver", lvbe_win_scroll_ver },

    // { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */

    { NULL, NULL }
  };
  be_regclass(vm, "lv_win", members);

  be_getglobal(vm, "lv_win");
  be_getglobal(vm, "lv_obj");
  be_setsuper(vm, -2);
  be_pop(vm, 2);
#else
    be_pushntvclass(vm, &be_class_lv_win);
    be_setglobal(vm, "lv_win");
    be_pop(vm, 1);
#endif
};

/* @const_object_info_begin
class be_class_lv_win (scope: global, name: lv_win, super: be_class_lv_obj) {
    .p, var
    init, func(lvbe_win_create)
    tostring, func(lvx_tostring)
    create, func(lvbe_win_create)
    clean, func(lvbe_win_clean)
    add_btn_right, func(lvbe_win_add_btn_right)
    add_btn_left, func(lvbe_win_add_btn_left)
    close_event_cb, func(lvbe_win_close_event_cb)
    set_title, func(lvbe_win_set_title)
    set_header_height, func(lvbe_win_set_header_height)
    set_btn_width, func(lvbe_win_set_btn_width)
    set_content_size, func(lvbe_win_set_content_size)
    set_layout, func(lvbe_win_set_layout)
    set_scrollbar_mode, func(lvbe_win_set_scrollbar_mode)
    set_anim_time, func(lvbe_win_set_anim_time)
    set_drag, func(lvbe_win_set_drag)
    title_set_alignment, func(lvbe_win_title_set_alignment)
    get_title, func(lvbe_win_get_title)
    get_content, func(lvbe_win_get_content)
    get_header_height, func(lvbe_win_get_header_height)
    get_btn_width, func(lvbe_win_get_btn_width)
    get_from_btn, func(lvbe_win_get_from_btn)
    get_layout, func(lvbe_win_get_layout)
    get_sb_mode, func(lvbe_win_get_sb_mode)
    get_anim_time, func(lvbe_win_get_anim_time)
    get_width, func(lvbe_win_get_width)
    get_drag, func(lvbe_win_get_drag)
    title_get_alignment, func(lvbe_win_title_get_alignment)
    focus, func(lvbe_win_focus)
    scroll_hor, func(lvbe_win_scroll_hor)
    scroll_ver, func(lvbe_win_scroll_ver)
}
@const_object_info_end */


#endif // USE_LVGL

