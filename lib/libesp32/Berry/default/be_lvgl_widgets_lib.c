
/********************************************************************
 * Generated code, don't edit
 *******************************************************************/
 
 /********************************************************************
 * Tasmota LVGL classes for widgets
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_LVGL

#include "lvgl.h"
#include "be_lvgl.h"

extern int lv0_init(bvm *vm);

extern int lco_init(bvm *vm);
extern int lco_tostring(bvm *vm);

extern int lvx_init_2(bvm *vm);           // generic function
extern int lvx_member(bvm *vm);
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
extern int lvbe_group_get_editing(bvm *vm);
extern int lvbe_group_get_click_focus(bvm *vm);
extern int lvbe_group_get_wrap(bvm *vm);

/* `lv_indev` external functions definitions */
extern int lvbe_indev_get_type(bvm *vm);
extern int lvbe_indev_enable(bvm *vm);
extern int lvbe_indev_set_group(bvm *vm);
extern int lvbe_indev_get_obj_act(bvm *vm);
extern int lvbe_indev_search_obj(bvm *vm);

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
extern int lvbe_obj_get_style_bg_blend_mode(bvm *vm);
extern int lvbe_obj_set_style_local_bg_blend_mode(bvm *vm);
extern int lvbe_obj_get_style_bg_main_stop(bvm *vm);
extern int lvbe_obj_set_style_local_bg_main_stop(bvm *vm);
extern int lvbe_obj_get_style_bg_grad_stop(bvm *vm);
extern int lvbe_obj_set_style_local_bg_grad_stop(bvm *vm);
extern int lvbe_obj_get_style_bg_grad_dir(bvm *vm);
extern int lvbe_obj_set_style_local_bg_grad_dir(bvm *vm);
extern int lvbe_obj_get_style_bg_color(bvm *vm);
extern int lvbe_obj_set_style_local_bg_color(bvm *vm);
extern int lvbe_obj_get_style_bg_grad_color(bvm *vm);
extern int lvbe_obj_set_style_local_bg_grad_color(bvm *vm);
extern int lvbe_obj_get_style_bg_opa(bvm *vm);
extern int lvbe_obj_set_style_local_bg_opa(bvm *vm);
extern int lvbe_obj_get_style_border_width(bvm *vm);
extern int lvbe_obj_set_style_local_border_width(bvm *vm);
extern int lvbe_obj_get_style_border_side(bvm *vm);
extern int lvbe_obj_set_style_local_border_side(bvm *vm);
extern int lvbe_obj_get_style_border_blend_mode(bvm *vm);
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
extern int lvbe_obj_get_style_outline_blend_mode(bvm *vm);
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
extern int lvbe_obj_get_style_shadow_blend_mode(bvm *vm);
extern int lvbe_obj_set_style_local_shadow_blend_mode(bvm *vm);
extern int lvbe_obj_get_style_shadow_color(bvm *vm);
extern int lvbe_obj_set_style_local_shadow_color(bvm *vm);
extern int lvbe_obj_get_style_shadow_opa(bvm *vm);
extern int lvbe_obj_set_style_local_shadow_opa(bvm *vm);
extern int lvbe_obj_get_style_pattern_repeat(bvm *vm);
extern int lvbe_obj_set_style_local_pattern_repeat(bvm *vm);
extern int lvbe_obj_get_style_pattern_blend_mode(bvm *vm);
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
extern int lvbe_obj_get_style_value_blend_mode(bvm *vm);
extern int lvbe_obj_set_style_local_value_blend_mode(bvm *vm);
extern int lvbe_obj_get_style_value_ofs_x(bvm *vm);
extern int lvbe_obj_set_style_local_value_ofs_x(bvm *vm);
extern int lvbe_obj_get_style_value_ofs_y(bvm *vm);
extern int lvbe_obj_set_style_local_value_ofs_y(bvm *vm);
extern int lvbe_obj_get_style_value_align(bvm *vm);
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
extern int lvbe_obj_get_style_text_blend_mode(bvm *vm);
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
extern int lvbe_obj_get_style_line_blend_mode(bvm *vm);
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
extern int lvbe_obj_get_style_image_blend_mode(bvm *vm);
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
extern int lvbe_keyboard_get_mode(bvm *vm);
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
#include "../generate/be_fixed_be_class_lv_indev.h"
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
    { "member", lvx_member },


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
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_style_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvs_init },
    { "tostring", lvs_tostring },
    { "member", lvx_member },


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
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_group_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_group_create },
    { "tostring", lvx_tostring },
    { "member", lvx_member },


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
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_indev_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lv0_init },
    { "tostring", lvx_tostring },
    { "member", lvx_member },


    // { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */

    { NULL, NULL }
  };
  be_regclass(vm, "lv_indev", members);

  be_getglobal(vm, "lv_indev");
  be_getglobal(vm, "lv_obj");
  be_setsuper(vm, -2);
  be_pop(vm, 2);
#else
    be_pushntvclass(vm, &be_class_lv_indev);
    be_setglobal(vm, "lv_indev");
    be_pop(vm, 1);
#endif
};

/* @const_object_info_begin
class be_class_lv_indev (scope: global, name: lv_indev, super: be_class_lv_obj) {
    .p, var
    init, func(lv0_init)
    tostring, func(lvx_tostring)
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_obj_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_obj_create },
    { "tostring", lvx_tostring },
    { "member", lvx_member },


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
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_arc_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_arc_create },
    { "tostring", lvx_tostring },
    { "member", lvx_member },


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
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_bar_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_bar_create },
    { "tostring", lvx_tostring },
    { "member", lvx_member },


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
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_btn_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_btn_create },
    { "tostring", lvx_tostring },
    { "member", lvx_member },


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
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_btnmatrix_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_btnmatrix_create },
    { "tostring", lvx_tostring },
    { "member", lvx_member },


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
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_calendar_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_calendar_create },
    { "tostring", lvx_tostring },
    { "member", lvx_member },


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
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_canvas_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_canvas_create },
    { "tostring", lvx_tostring },
    { "member", lvx_member },


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
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_chart_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_chart_create },
    { "tostring", lvx_tostring },
    { "member", lvx_member },


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
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_checkbox_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_checkbox_create },
    { "tostring", lvx_tostring },
    { "member", lvx_member },


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
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_cont_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_cont_create },
    { "tostring", lvx_tostring },
    { "member", lvx_member },


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
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_cpicker_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_cpicker_create },
    { "tostring", lvx_tostring },
    { "member", lvx_member },


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
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_dropdown_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_dropdown_create },
    { "tostring", lvx_tostring },
    { "member", lvx_member },


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
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_gauge_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_gauge_create },
    { "tostring", lvx_tostring },
    { "member", lvx_member },


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
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_imgbtn_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_imgbtn_create },
    { "tostring", lvx_tostring },
    { "member", lvx_member },


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
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_keyboard_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_keyboard_create },
    { "tostring", lvx_tostring },
    { "member", lvx_member },


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
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_label_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_label_create },
    { "tostring", lvx_tostring },
    { "member", lvx_member },


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
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_led_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_led_create },
    { "tostring", lvx_tostring },
    { "member", lvx_member },


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
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_line_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_line_create },
    { "tostring", lvx_tostring },
    { "member", lvx_member },


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
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_linemeter_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_linemeter_create },
    { "tostring", lvx_tostring },
    { "member", lvx_member },


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
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_list_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_list_create },
    { "tostring", lvx_tostring },
    { "member", lvx_member },


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
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_msgbox_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_msgbox_create },
    { "tostring", lvx_tostring },
    { "member", lvx_member },


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
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_objmask_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_objmask_create },
    { "tostring", lvx_tostring },
    { "member", lvx_member },


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
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_page_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_page_create },
    { "tostring", lvx_tostring },
    { "member", lvx_member },


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
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_roller_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_roller_create },
    { "tostring", lvx_tostring },
    { "member", lvx_member },


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
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_slider_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_slider_create },
    { "tostring", lvx_tostring },
    { "member", lvx_member },


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
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_spinbox_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_spinbox_create },
    { "tostring", lvx_tostring },
    { "member", lvx_member },


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
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_spinner_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_spinner_create },
    { "tostring", lvx_tostring },
    { "member", lvx_member },


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
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_switch_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_switch_create },
    { "tostring", lvx_tostring },
    { "member", lvx_member },


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
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_table_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_table_create },
    { "tostring", lvx_tostring },
    { "member", lvx_member },


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
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_tabview_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_tabview_create },
    { "tostring", lvx_tostring },
    { "member", lvx_member },


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
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_textarea_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_textarea_create },
    { "tostring", lvx_tostring },
    { "member", lvx_member },


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
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_tileview_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_tileview_create },
    { "tostring", lvx_tostring },
    { "member", lvx_member },


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
    member, func(lvx_member)

}
@const_object_info_end */

void be_load_lv_win_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
  static const bnfuncinfo members[] = {
    { ".p", NULL },
    { "init", lvbe_win_create },
    { "tostring", lvx_tostring },
    { "member", lvx_member },


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
    member, func(lvx_member)

}
@const_object_info_end */


#endif // USE_LVGL

