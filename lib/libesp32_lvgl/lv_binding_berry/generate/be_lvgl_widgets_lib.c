
/********************************************************************
 * Generated code, don't edit
 *******************************************************************/
 
 /********************************************************************
 * Tasmota LVGL classes for widgets
 *******************************************************************/
#include "be_constobj.h"

#include "lvgl.h"

extern int lv0_init(bvm *vm);

extern int lco_init(bvm *vm);           // generic function
extern int lco_tostring(bvm *vm);       // generic function
extern int lco_toint(bvm *vm);          // generic function

extern int lv_x_member(bvm *vm);
extern int lv_x_tostring(bvm *vm);       // generic function

extern int lv_be_style_init(bvm *vm);
extern int lv_be_anim_init(bvm *vm);
extern int lv_x_tostring(bvm *vm);

BE_EXPORT_VARIABLE extern const bclass be_class_lv_obj;

extern int lvbe_font_create(bvm *vm);
extern int lvbe_theme_create(bvm *vm);


/* `lv_style` external functions definitions */
extern int lvbe_style_is_const(bvm *vm);
extern int lvbe_style_is_empty(bvm *vm);
extern int lvbe_style_remove_prop(bvm *vm);
extern int lvbe_style_reset(bvm *vm);
extern int lvbe_style_set_align(bvm *vm);
extern int lvbe_style_set_anim(bvm *vm);
extern int lvbe_style_set_anim_duration(bvm *vm);
extern int lvbe_style_set_anim_duration(bvm *vm);
extern int lvbe_style_set_arc_color(bvm *vm);
extern int lvbe_style_set_arc_image_src(bvm *vm);
extern int lvbe_style_set_arc_opa(bvm *vm);
extern int lvbe_style_set_arc_rounded(bvm *vm);
extern int lvbe_style_set_arc_width(bvm *vm);
extern int lvbe_style_set_base_dir(bvm *vm);
extern int lvbe_style_set_bg_color(bvm *vm);
extern int lvbe_style_set_bg_grad(bvm *vm);
extern int lvbe_style_set_bg_grad_color(bvm *vm);
extern int lvbe_style_set_bg_grad_dir(bvm *vm);
extern int lvbe_style_set_bg_grad_opa(bvm *vm);
extern int lvbe_style_set_bg_grad_stop(bvm *vm);
extern int lvbe_style_set_bg_image_opa(bvm *vm);
extern int lvbe_style_set_bg_image_recolor(bvm *vm);
extern int lvbe_style_set_bg_image_recolor_opa(bvm *vm);
extern int lvbe_style_set_bg_image_src(bvm *vm);
extern int lvbe_style_set_bg_image_tiled(bvm *vm);
extern int lvbe_style_set_bg_image_opa(bvm *vm);
extern int lvbe_style_set_bg_image_recolor(bvm *vm);
extern int lvbe_style_set_bg_image_recolor_opa(bvm *vm);
extern int lvbe_style_set_bg_image_src(bvm *vm);
extern int lvbe_style_set_bg_image_tiled(bvm *vm);
extern int lvbe_style_set_bg_main_opa(bvm *vm);
extern int lvbe_style_set_bg_main_stop(bvm *vm);
extern int lvbe_style_set_bg_opa(bvm *vm);
extern int lvbe_style_set_blend_mode(bvm *vm);
extern int lvbe_style_set_border_color(bvm *vm);
extern int lvbe_style_set_border_opa(bvm *vm);
extern int lvbe_style_set_border_post(bvm *vm);
extern int lvbe_style_set_border_side(bvm *vm);
extern int lvbe_style_set_border_width(bvm *vm);
extern int lvbe_style_set_clip_corner(bvm *vm);
extern int lvbe_style_set_color_filter_dsc(bvm *vm);
extern int lvbe_style_set_color_filter_opa(bvm *vm);
extern int lvbe_style_set_flex_cross_place(bvm *vm);
extern int lvbe_style_set_flex_flow(bvm *vm);
extern int lvbe_style_set_flex_grow(bvm *vm);
extern int lvbe_style_set_flex_main_place(bvm *vm);
extern int lvbe_style_set_flex_track_place(bvm *vm);
extern int lvbe_style_set_grid_cell_column_pos(bvm *vm);
extern int lvbe_style_set_grid_cell_column_span(bvm *vm);
extern int lvbe_style_set_grid_cell_row_pos(bvm *vm);
extern int lvbe_style_set_grid_cell_row_span(bvm *vm);
extern int lvbe_style_set_grid_cell_x_align(bvm *vm);
extern int lvbe_style_set_grid_cell_y_align(bvm *vm);
extern int lvbe_style_set_grid_column_align(bvm *vm);
extern int lvbe_style_set_grid_column_dsc_array(bvm *vm);
extern int lvbe_style_set_grid_row_align(bvm *vm);
extern int lvbe_style_set_grid_row_dsc_array(bvm *vm);
extern int lvbe_style_set_height(bvm *vm);
extern int lvbe_style_set_image_opa(bvm *vm);
extern int lvbe_style_set_image_recolor(bvm *vm);
extern int lvbe_style_set_image_recolor_opa(bvm *vm);
extern int lvbe_style_set_image_opa(bvm *vm);
extern int lvbe_style_set_image_recolor(bvm *vm);
extern int lvbe_style_set_image_recolor_opa(bvm *vm);
extern int lvbe_style_set_layout(bvm *vm);
extern int lvbe_style_set_length(bvm *vm);
extern int lvbe_style_set_line_color(bvm *vm);
extern int lvbe_style_set_line_dash_gap(bvm *vm);
extern int lvbe_style_set_line_dash_width(bvm *vm);
extern int lvbe_style_set_line_opa(bvm *vm);
extern int lvbe_style_set_line_rounded(bvm *vm);
extern int lvbe_style_set_line_width(bvm *vm);
extern int lvbe_style_set_margin_bottom(bvm *vm);
extern int lvbe_style_set_margin_left(bvm *vm);
extern int lvbe_style_set_margin_right(bvm *vm);
extern int lvbe_style_set_margin_top(bvm *vm);
extern int lvbe_style_set_max_height(bvm *vm);
extern int lvbe_style_set_max_width(bvm *vm);
extern int lvbe_style_set_min_height(bvm *vm);
extern int lvbe_style_set_min_width(bvm *vm);
extern int lvbe_style_set_opa(bvm *vm);
extern int lvbe_style_set_opa_layered(bvm *vm);
extern int lvbe_style_set_outline_color(bvm *vm);
extern int lvbe_style_set_outline_opa(bvm *vm);
extern int lvbe_style_set_outline_pad(bvm *vm);
extern int lvbe_style_set_outline_width(bvm *vm);
extern int lvbe_style_set_pad_all(bvm *vm);
extern int lvbe_style_set_pad_bottom(bvm *vm);
extern int lvbe_style_set_pad_column(bvm *vm);
extern int lvbe_style_set_pad_gap(bvm *vm);
extern int lvbe_style_set_pad_hor(bvm *vm);
extern int lvbe_style_set_pad_left(bvm *vm);
extern int lvbe_style_set_pad_right(bvm *vm);
extern int lvbe_style_set_pad_row(bvm *vm);
extern int lvbe_style_set_pad_top(bvm *vm);
extern int lvbe_style_set_pad_ver(bvm *vm);
extern int lvbe_style_set_prop(bvm *vm);
extern int lvbe_style_set_radius(bvm *vm);
extern int lvbe_style_set_shadow_color(bvm *vm);
extern int lvbe_style_set_shadow_offset_x(bvm *vm);
extern int lvbe_style_set_shadow_offset_y(bvm *vm);
extern int lvbe_style_set_shadow_offset_x(bvm *vm);
extern int lvbe_style_set_shadow_offset_y(bvm *vm);
extern int lvbe_style_set_shadow_opa(bvm *vm);
extern int lvbe_style_set_shadow_spread(bvm *vm);
extern int lvbe_style_set_shadow_width(bvm *vm);
extern int lvbe_style_set_size(bvm *vm);
extern int lvbe_style_set_text_align(bvm *vm);
extern int lvbe_style_set_text_color(bvm *vm);
extern int lvbe_style_set_text_decor(bvm *vm);
extern int lvbe_style_set_text_font(bvm *vm);
extern int lvbe_style_set_text_letter_space(bvm *vm);
extern int lvbe_style_set_text_line_space(bvm *vm);
extern int lvbe_style_set_text_opa(bvm *vm);
extern int lvbe_style_set_transform_rotation(bvm *vm);
extern int lvbe_style_set_transform_height(bvm *vm);
extern int lvbe_style_set_transform_pivot_x(bvm *vm);
extern int lvbe_style_set_transform_pivot_y(bvm *vm);
extern int lvbe_style_set_transform_rotation(bvm *vm);
extern int lvbe_style_set_transform_scale(bvm *vm);
extern int lvbe_style_set_transform_scale_x(bvm *vm);
extern int lvbe_style_set_transform_scale_y(bvm *vm);
extern int lvbe_style_set_transform_skew_x(bvm *vm);
extern int lvbe_style_set_transform_skew_y(bvm *vm);
extern int lvbe_style_set_transform_width(bvm *vm);
extern int lvbe_style_set_transform_scale(bvm *vm);
extern int lvbe_style_set_transition(bvm *vm);
extern int lvbe_style_set_translate_x(bvm *vm);
extern int lvbe_style_set_translate_y(bvm *vm);
extern int lvbe_style_set_width(bvm *vm);
extern int lvbe_style_set_x(bvm *vm);
extern int lvbe_style_set_y(bvm *vm);

/* `lv_font` external functions definitions */

/* `lv_color` external functions definitions */

/* `lv_theme` external functions definitions */

/* `lv_image` external functions definitions */
extern int lvbe_image_create(bvm *vm);
extern int lvbe_image_get_rotation(bvm *vm);
extern int lvbe_image_get_antialias(bvm *vm);
extern int lvbe_image_get_blend_mode(bvm *vm);
extern int lvbe_image_get_inner_align(bvm *vm);
extern int lvbe_image_get_offset_x(bvm *vm);
extern int lvbe_image_get_offset_x(bvm *vm);
extern int lvbe_image_get_offset_y(bvm *vm);
extern int lvbe_image_get_offset_y(bvm *vm);
extern int lvbe_image_get_pivot(bvm *vm);
extern int lvbe_image_get_rotation(bvm *vm);
extern int lvbe_image_get_rotation(bvm *vm);
extern int lvbe_image_get_scale(bvm *vm);
extern int lvbe_image_get_scale_x(bvm *vm);
extern int lvbe_image_get_scale_y(bvm *vm);
extern int lvbe_image_get_src(bvm *vm);
extern int lvbe_image_get_scale(bvm *vm);
extern int lvbe_image_set_rotation(bvm *vm);
extern int lvbe_image_set_antialias(bvm *vm);
extern int lvbe_image_set_blend_mode(bvm *vm);
extern int lvbe_image_set_inner_align(bvm *vm);
extern int lvbe_image_set_offset_x(bvm *vm);
extern int lvbe_image_set_offset_y(bvm *vm);
extern int lvbe_image_set_pivot(bvm *vm);
extern int lvbe_image_set_rotation(bvm *vm);
extern int lvbe_image_set_rotation(bvm *vm);
extern int lvbe_image_set_scale(bvm *vm);
extern int lvbe_image_set_scale_x(bvm *vm);
extern int lvbe_image_set_scale_y(bvm *vm);
extern int lvbe_image_set_src(bvm *vm);
extern int lvbe_image_set_tasmota_logo(bvm *vm);
extern int lvbe_image_set_scale(bvm *vm);

/* `lv_group` external functions definitions */
extern int lvbe_group_add_obj(bvm *vm);
extern int lvbe_group_create(bvm *vm);
extern int lvbe_group_delete(bvm *vm);
extern int lvbe_group_delete(bvm *vm);
extern int lvbe_group_focus_freeze(bvm *vm);
extern int lvbe_group_focus_next(bvm *vm);
extern int lvbe_group_focus_obj(bvm *vm);
extern int lvbe_group_focus_prev(bvm *vm);
extern int lvbe_group_get_edge_cb(bvm *vm);
extern int lvbe_group_get_editing(bvm *vm);
extern int lvbe_group_get_focus_cb(bvm *vm);
extern int lvbe_group_get_focused(bvm *vm);
extern int lvbe_group_get_obj_count(bvm *vm);
extern int lvbe_group_get_wrap(bvm *vm);
extern int lvbe_group_delete(bvm *vm);
extern int lvbe_group_remove_all_objs(bvm *vm);
extern int lvbe_group_remove_obj(bvm *vm);
extern int lvbe_group_send_data(bvm *vm);
extern int lvbe_group_set_default(bvm *vm);
extern int lvbe_group_set_default(bvm *vm);
extern int lvbe_group_set_editing(bvm *vm);
extern int lvbe_group_set_focus_cb(bvm *vm);
extern int lvbe_group_set_refocus_policy(bvm *vm);
extern int lvbe_group_set_wrap(bvm *vm);
extern int lvbe_group_swap_obj(bvm *vm);

/* `lv_obj` external functions definitions */
extern int lvbe_obj_add_event_cb(bvm *vm);
extern int lvbe_obj_add_flag(bvm *vm);
extern int lvbe_obj_add_state(bvm *vm);
extern int lvbe_obj_add_style(bvm *vm);
extern int lvbe_obj_align(bvm *vm);
extern int lvbe_obj_align_to(bvm *vm);
extern int lvbe_obj_allocate_spec_attr(bvm *vm);
extern int lvbe_obj_area_is_visible(bvm *vm);
extern int lvbe_obj_calculate_ext_draw_size(bvm *vm);
extern int lvbe_obj_calculate_style_text_align(bvm *vm);
extern int lvbe_obj_center(bvm *vm);
extern int lvbe_obj_check_type(bvm *vm);
extern int lvbe_obj_class_init_obj(bvm *vm);
extern int lvbe_obj_clean(bvm *vm);
extern int lvbe_obj_remove_flag(bvm *vm);
extern int lvbe_obj_remove_state(bvm *vm);
extern int lvbe_obj_create(bvm *vm);
extern int lvbe_obj_delete(bvm *vm);
extern int lvbe_obj_delete_async(bvm *vm);
extern int lvbe_obj_delete(bvm *vm);
extern int lvbe_obj_delete_async(bvm *vm);
extern int lvbe_obj_delete_delayed(bvm *vm);
extern int lvbe_obj_dump_tree(bvm *vm);
extern int lvbe_obj_fade_in(bvm *vm);
extern int lvbe_obj_fade_out(bvm *vm);
extern int lvbe_obj_free_id(bvm *vm);
extern int lvbe_obj_get_child(bvm *vm);
extern int lvbe_obj_get_child_by_type(bvm *vm);
extern int lvbe_obj_get_child_count(bvm *vm);
extern int lvbe_obj_get_child_count(bvm *vm);
extern int lvbe_obj_get_child_count_by_type(bvm *vm);
extern int lvbe_obj_get_class(bvm *vm);
extern int lvbe_obj_get_click_area(bvm *vm);
extern int lvbe_obj_get_content_coords(bvm *vm);
extern int lvbe_obj_get_content_height(bvm *vm);
extern int lvbe_obj_get_content_width(bvm *vm);
extern int lvbe_obj_get_coords(bvm *vm);
extern int lvbe_obj_get_display(bvm *vm);
extern int lvbe_obj_get_display(bvm *vm);
extern int lvbe_obj_get_event_count(bvm *vm);
extern int lvbe_obj_get_group(bvm *vm);
extern int lvbe_obj_get_height(bvm *vm);
extern int lvbe_obj_get_index(bvm *vm);
extern int lvbe_obj_get_index_by_type(bvm *vm);
extern int lvbe_obj_get_parent(bvm *vm);
extern int lvbe_obj_get_screen(bvm *vm);
extern int lvbe_obj_get_scroll_bottom(bvm *vm);
extern int lvbe_obj_get_scroll_dir(bvm *vm);
extern int lvbe_obj_get_scroll_end(bvm *vm);
extern int lvbe_obj_get_scroll_left(bvm *vm);
extern int lvbe_obj_get_scroll_right(bvm *vm);
extern int lvbe_obj_get_scroll_snap_x(bvm *vm);
extern int lvbe_obj_get_scroll_snap_y(bvm *vm);
extern int lvbe_obj_get_scroll_top(bvm *vm);
extern int lvbe_obj_get_scroll_x(bvm *vm);
extern int lvbe_obj_get_scroll_y(bvm *vm);
extern int lvbe_obj_get_scrollbar_area(bvm *vm);
extern int lvbe_obj_get_scrollbar_mode(bvm *vm);
extern int lvbe_obj_get_self_height(bvm *vm);
extern int lvbe_obj_get_self_width(bvm *vm);
extern int lvbe_obj_get_sibling(bvm *vm);
extern int lvbe_obj_get_sibling_by_type(bvm *vm);
extern int lvbe_obj_get_state(bvm *vm);
extern int lvbe_obj_get_style_align(bvm *vm);
extern int lvbe_obj_get_style_anim(bvm *vm);
extern int lvbe_obj_get_style_anim_duration(bvm *vm);
extern int lvbe_obj_get_style_anim_duration(bvm *vm);
extern int lvbe_obj_get_style_arc_color(bvm *vm);
extern int lvbe_obj_get_style_arc_color_filtered(bvm *vm);
extern int lvbe_obj_get_style_arc_image_src(bvm *vm);
extern int lvbe_obj_get_style_arc_opa(bvm *vm);
extern int lvbe_obj_get_style_arc_rounded(bvm *vm);
extern int lvbe_obj_get_style_arc_width(bvm *vm);
extern int lvbe_obj_get_style_base_dir(bvm *vm);
extern int lvbe_obj_get_style_bg_color(bvm *vm);
extern int lvbe_obj_get_style_bg_color_filtered(bvm *vm);
extern int lvbe_obj_get_style_bg_grad(bvm *vm);
extern int lvbe_obj_get_style_bg_grad_color(bvm *vm);
extern int lvbe_obj_get_style_bg_grad_color_filtered(bvm *vm);
extern int lvbe_obj_get_style_bg_grad_dir(bvm *vm);
extern int lvbe_obj_get_style_bg_grad_opa(bvm *vm);
extern int lvbe_obj_get_style_bg_grad_stop(bvm *vm);
extern int lvbe_obj_get_style_bg_image_opa(bvm *vm);
extern int lvbe_obj_get_style_bg_image_recolor(bvm *vm);
extern int lvbe_obj_get_style_bg_image_recolor_filtered(bvm *vm);
extern int lvbe_obj_get_style_bg_image_recolor_opa(bvm *vm);
extern int lvbe_obj_get_style_bg_image_src(bvm *vm);
extern int lvbe_obj_get_style_bg_image_tiled(bvm *vm);
extern int lvbe_obj_get_style_bg_main_opa(bvm *vm);
extern int lvbe_obj_get_style_bg_main_stop(bvm *vm);
extern int lvbe_obj_get_style_bg_opa(bvm *vm);
extern int lvbe_obj_get_style_blend_mode(bvm *vm);
extern int lvbe_obj_get_style_border_color(bvm *vm);
extern int lvbe_obj_get_style_border_color_filtered(bvm *vm);
extern int lvbe_obj_get_style_border_opa(bvm *vm);
extern int lvbe_obj_get_style_border_post(bvm *vm);
extern int lvbe_obj_get_style_border_side(bvm *vm);
extern int lvbe_obj_get_style_border_width(bvm *vm);
extern int lvbe_obj_get_style_clip_corner(bvm *vm);
extern int lvbe_obj_get_style_color_filter_dsc(bvm *vm);
extern int lvbe_obj_get_style_color_filter_opa(bvm *vm);
extern int lvbe_obj_get_style_flex_cross_place(bvm *vm);
extern int lvbe_obj_get_style_flex_flow(bvm *vm);
extern int lvbe_obj_get_style_flex_grow(bvm *vm);
extern int lvbe_obj_get_style_flex_main_place(bvm *vm);
extern int lvbe_obj_get_style_flex_track_place(bvm *vm);
extern int lvbe_obj_get_style_grid_cell_column_pos(bvm *vm);
extern int lvbe_obj_get_style_grid_cell_column_span(bvm *vm);
extern int lvbe_obj_get_style_grid_cell_row_pos(bvm *vm);
extern int lvbe_obj_get_style_grid_cell_row_span(bvm *vm);
extern int lvbe_obj_get_style_grid_cell_x_align(bvm *vm);
extern int lvbe_obj_get_style_grid_cell_y_align(bvm *vm);
extern int lvbe_obj_get_style_grid_column_align(bvm *vm);
extern int lvbe_obj_get_style_grid_column_dsc_array(bvm *vm);
extern int lvbe_obj_get_style_grid_row_align(bvm *vm);
extern int lvbe_obj_get_style_grid_row_dsc_array(bvm *vm);
extern int lvbe_obj_get_style_height(bvm *vm);
extern int lvbe_obj_get_style_image_opa(bvm *vm);
extern int lvbe_obj_get_style_image_recolor(bvm *vm);
extern int lvbe_obj_get_style_image_recolor_filtered(bvm *vm);
extern int lvbe_obj_get_style_image_recolor_opa(bvm *vm);
extern int lvbe_obj_get_style_image_opa(bvm *vm);
extern int lvbe_obj_get_style_image_recolor(bvm *vm);
extern int lvbe_obj_get_style_image_recolor_filtered(bvm *vm);
extern int lvbe_obj_get_style_image_recolor_opa(bvm *vm);
extern int lvbe_obj_get_style_layout(bvm *vm);
extern int lvbe_obj_get_style_length(bvm *vm);
extern int lvbe_obj_get_style_line_color(bvm *vm);
extern int lvbe_obj_get_style_line_color_filtered(bvm *vm);
extern int lvbe_obj_get_style_line_dash_gap(bvm *vm);
extern int lvbe_obj_get_style_line_dash_width(bvm *vm);
extern int lvbe_obj_get_style_line_opa(bvm *vm);
extern int lvbe_obj_get_style_line_rounded(bvm *vm);
extern int lvbe_obj_get_style_line_width(bvm *vm);
extern int lvbe_obj_get_style_margin_bottom(bvm *vm);
extern int lvbe_obj_get_style_margin_left(bvm *vm);
extern int lvbe_obj_get_style_margin_right(bvm *vm);
extern int lvbe_obj_get_style_margin_top(bvm *vm);
extern int lvbe_obj_get_style_max_height(bvm *vm);
extern int lvbe_obj_get_style_max_width(bvm *vm);
extern int lvbe_obj_get_style_min_height(bvm *vm);
extern int lvbe_obj_get_style_min_width(bvm *vm);
extern int lvbe_obj_get_style_opa(bvm *vm);
extern int lvbe_obj_get_style_opa_layered(bvm *vm);
extern int lvbe_obj_get_style_opa_recursive(bvm *vm);
extern int lvbe_obj_get_style_outline_color(bvm *vm);
extern int lvbe_obj_get_style_outline_color_filtered(bvm *vm);
extern int lvbe_obj_get_style_outline_opa(bvm *vm);
extern int lvbe_obj_get_style_outline_pad(bvm *vm);
extern int lvbe_obj_get_style_outline_width(bvm *vm);
extern int lvbe_obj_get_style_pad_bottom(bvm *vm);
extern int lvbe_obj_get_style_pad_column(bvm *vm);
extern int lvbe_obj_get_style_pad_left(bvm *vm);
extern int lvbe_obj_get_style_pad_right(bvm *vm);
extern int lvbe_obj_get_style_pad_row(bvm *vm);
extern int lvbe_obj_get_style_pad_top(bvm *vm);
extern int lvbe_obj_get_style_prop(bvm *vm);
extern int lvbe_obj_get_style_radius(bvm *vm);
extern int lvbe_obj_get_style_shadow_color(bvm *vm);
extern int lvbe_obj_get_style_shadow_color_filtered(bvm *vm);
extern int lvbe_obj_get_style_shadow_offset_x(bvm *vm);
extern int lvbe_obj_get_style_shadow_offset_y(bvm *vm);
extern int lvbe_obj_get_style_shadow_offset_x(bvm *vm);
extern int lvbe_obj_get_style_shadow_offset_y(bvm *vm);
extern int lvbe_obj_get_style_shadow_opa(bvm *vm);
extern int lvbe_obj_get_style_shadow_spread(bvm *vm);
extern int lvbe_obj_get_style_shadow_width(bvm *vm);
extern int lvbe_obj_get_style_space_bottom(bvm *vm);
extern int lvbe_obj_get_style_space_left(bvm *vm);
extern int lvbe_obj_get_style_space_right(bvm *vm);
extern int lvbe_obj_get_style_space_top(bvm *vm);
extern int lvbe_obj_get_style_text_align(bvm *vm);
extern int lvbe_obj_get_style_text_color(bvm *vm);
extern int lvbe_obj_get_style_text_color_filtered(bvm *vm);
extern int lvbe_obj_get_style_text_decor(bvm *vm);
extern int lvbe_obj_get_style_text_font(bvm *vm);
extern int lvbe_obj_get_style_text_letter_space(bvm *vm);
extern int lvbe_obj_get_style_text_line_space(bvm *vm);
extern int lvbe_obj_get_style_text_opa(bvm *vm);
extern int lvbe_obj_get_style_transform_rotation(bvm *vm);
extern int lvbe_obj_get_style_transform_height(bvm *vm);
extern int lvbe_obj_get_style_transform_pivot_x(bvm *vm);
extern int lvbe_obj_get_style_transform_pivot_y(bvm *vm);
extern int lvbe_obj_get_style_transform_rotation(bvm *vm);
extern int lvbe_obj_get_style_transform_scale_x(bvm *vm);
extern int lvbe_obj_get_style_transform_scale_x_safe(bvm *vm);
extern int lvbe_obj_get_style_transform_scale_y(bvm *vm);
extern int lvbe_obj_get_style_transform_scale_y_safe(bvm *vm);
extern int lvbe_obj_get_style_transform_skew_x(bvm *vm);
extern int lvbe_obj_get_style_transform_skew_y(bvm *vm);
extern int lvbe_obj_get_style_transform_width(bvm *vm);
extern int lvbe_obj_get_style_transition(bvm *vm);
extern int lvbe_obj_get_style_translate_x(bvm *vm);
extern int lvbe_obj_get_style_translate_y(bvm *vm);
extern int lvbe_obj_get_style_width(bvm *vm);
extern int lvbe_obj_get_style_x(bvm *vm);
extern int lvbe_obj_get_style_y(bvm *vm);
extern int lvbe_obj_get_transformed_area(bvm *vm);
extern int lvbe_obj_get_user_data(bvm *vm);
extern int lvbe_obj_get_width(bvm *vm);
extern int lvbe_obj_get_width(bvm *vm);
extern int lvbe_obj_get_x(bvm *vm);
extern int lvbe_obj_get_x2(bvm *vm);
extern int lvbe_obj_get_x_aligned(bvm *vm);
extern int lvbe_obj_get_y(bvm *vm);
extern int lvbe_obj_get_y2(bvm *vm);
extern int lvbe_obj_get_y_aligned(bvm *vm);
extern int lvbe_obj_has_class(bvm *vm);
extern int lvbe_obj_has_flag(bvm *vm);
extern int lvbe_obj_has_flag_any(bvm *vm);
extern int lvbe_obj_has_state(bvm *vm);
extern int lvbe_obj_has_style_prop(bvm *vm);
extern int lvbe_obj_hit_test(bvm *vm);
extern int lvbe_obj_init_draw_arc_dsc(bvm *vm);
extern int lvbe_obj_init_draw_image_dsc(bvm *vm);
extern int lvbe_obj_init_draw_label_dsc(bvm *vm);
extern int lvbe_obj_init_draw_line_dsc(bvm *vm);
extern int lvbe_obj_init_draw_rect_dsc(bvm *vm);
extern int lvbe_obj_invalidate(bvm *vm);
extern int lvbe_obj_invalidate_area(bvm *vm);
extern int lvbe_obj_is_editable(bvm *vm);
extern int lvbe_obj_is_group_def(bvm *vm);
extern int lvbe_obj_is_layout_positioned(bvm *vm);
extern int lvbe_obj_is_scrolling(bvm *vm);
extern int lvbe_obj_is_valid(bvm *vm);
extern int lvbe_obj_is_visible(bvm *vm);
extern int lvbe_obj_mark_layout_as_dirty(bvm *vm);
extern int lvbe_obj_move_background(bvm *vm);
extern int lvbe_obj_move_children_by(bvm *vm);
extern int lvbe_obj_move_foreground(bvm *vm);
extern int lvbe_obj_move_to(bvm *vm);
extern int lvbe_obj_move_to_index(bvm *vm);
extern int lvbe_obj_readjust_scroll(bvm *vm);
extern int lvbe_obj_refr_pos(bvm *vm);
extern int lvbe_obj_refr_size(bvm *vm);
extern int lvbe_obj_refresh_ext_draw_size(bvm *vm);
extern int lvbe_obj_refresh_self_size(bvm *vm);
extern int lvbe_obj_refresh_style(bvm *vm);
extern int lvbe_obj_delete(bvm *vm);
extern int lvbe_obj_remove_event(bvm *vm);
extern int lvbe_obj_remove_event_cb(bvm *vm);
extern int lvbe_obj_remove_event_cb_with_user_data(bvm *vm);
extern int lvbe_obj_remove_flag(bvm *vm);
extern int lvbe_obj_remove_local_style_prop(bvm *vm);
extern int lvbe_obj_remove_state(bvm *vm);
extern int lvbe_obj_remove_style(bvm *vm);
extern int lvbe_obj_remove_style_all(bvm *vm);
extern int lvbe_obj_replace_style(bvm *vm);
extern int lvbe_obj_scroll_by(bvm *vm);
extern int lvbe_obj_scroll_by_bounded(bvm *vm);
extern int lvbe_obj_scroll_to(bvm *vm);
extern int lvbe_obj_scroll_to_view(bvm *vm);
extern int lvbe_obj_scroll_to_view_recursive(bvm *vm);
extern int lvbe_obj_scroll_to_x(bvm *vm);
extern int lvbe_obj_scroll_to_y(bvm *vm);
extern int lvbe_obj_scrollbar_invalidate(bvm *vm);
extern int lvbe_obj_send_event(bvm *vm);
extern int lvbe_obj_send_event(bvm *vm);
extern int lvbe_obj_set_align(bvm *vm);
extern int lvbe_obj_set_content_height(bvm *vm);
extern int lvbe_obj_set_content_width(bvm *vm);
extern int lvbe_obj_set_ext_click_area(bvm *vm);
extern int lvbe_obj_set_flex_align(bvm *vm);
extern int lvbe_obj_set_flex_flow(bvm *vm);
extern int lvbe_obj_set_flex_grow(bvm *vm);
extern int lvbe_obj_set_grid_align(bvm *vm);
extern int lvbe_obj_set_grid_cell(bvm *vm);
extern int lvbe_obj_set_grid_dsc_array(bvm *vm);
extern int lvbe_obj_set_height(bvm *vm);
extern int lvbe_obj_set_layout(bvm *vm);
extern int lvbe_obj_set_local_style_prop(bvm *vm);
extern int lvbe_obj_set_parent(bvm *vm);
extern int lvbe_obj_set_pos(bvm *vm);
extern int lvbe_obj_set_scroll_dir(bvm *vm);
extern int lvbe_obj_set_scroll_snap_x(bvm *vm);
extern int lvbe_obj_set_scroll_snap_y(bvm *vm);
extern int lvbe_obj_set_scrollbar_mode(bvm *vm);
extern int lvbe_obj_set_size(bvm *vm);
extern int lvbe_obj_set_state(bvm *vm);
extern int lvbe_obj_set_style_align(bvm *vm);
extern int lvbe_obj_set_style_anim(bvm *vm);
extern int lvbe_obj_set_style_anim_duration(bvm *vm);
extern int lvbe_obj_set_style_anim_duration(bvm *vm);
extern int lvbe_obj_set_style_arc_color(bvm *vm);
extern int lvbe_obj_set_style_arc_image_src(bvm *vm);
extern int lvbe_obj_set_style_arc_opa(bvm *vm);
extern int lvbe_obj_set_style_arc_rounded(bvm *vm);
extern int lvbe_obj_set_style_arc_width(bvm *vm);
extern int lvbe_obj_set_style_base_dir(bvm *vm);
extern int lvbe_obj_set_style_bg_color(bvm *vm);
extern int lvbe_obj_set_style_bg_grad(bvm *vm);
extern int lvbe_obj_set_style_bg_grad_color(bvm *vm);
extern int lvbe_obj_set_style_bg_grad_dir(bvm *vm);
extern int lvbe_obj_set_style_bg_grad_opa(bvm *vm);
extern int lvbe_obj_set_style_bg_grad_stop(bvm *vm);
extern int lvbe_obj_set_style_bg_image_opa(bvm *vm);
extern int lvbe_obj_set_style_bg_image_recolor(bvm *vm);
extern int lvbe_obj_set_style_bg_image_recolor_opa(bvm *vm);
extern int lvbe_obj_set_style_bg_image_src(bvm *vm);
extern int lvbe_obj_set_style_bg_image_tiled(bvm *vm);
extern int lvbe_obj_set_style_bg_main_opa(bvm *vm);
extern int lvbe_obj_set_style_bg_main_stop(bvm *vm);
extern int lvbe_obj_set_style_bg_opa(bvm *vm);
extern int lvbe_obj_set_style_blend_mode(bvm *vm);
extern int lvbe_obj_set_style_border_color(bvm *vm);
extern int lvbe_obj_set_style_border_opa(bvm *vm);
extern int lvbe_obj_set_style_border_post(bvm *vm);
extern int lvbe_obj_set_style_border_side(bvm *vm);
extern int lvbe_obj_set_style_border_width(bvm *vm);
extern int lvbe_obj_set_style_clip_corner(bvm *vm);
extern int lvbe_obj_set_style_color_filter_dsc(bvm *vm);
extern int lvbe_obj_set_style_color_filter_opa(bvm *vm);
extern int lvbe_obj_set_style_flex_cross_place(bvm *vm);
extern int lvbe_obj_set_style_flex_flow(bvm *vm);
extern int lvbe_obj_set_style_flex_grow(bvm *vm);
extern int lvbe_obj_set_style_flex_main_place(bvm *vm);
extern int lvbe_obj_set_style_flex_track_place(bvm *vm);
extern int lvbe_obj_set_style_grid_cell_column_pos(bvm *vm);
extern int lvbe_obj_set_style_grid_cell_column_span(bvm *vm);
extern int lvbe_obj_set_style_grid_cell_row_pos(bvm *vm);
extern int lvbe_obj_set_style_grid_cell_row_span(bvm *vm);
extern int lvbe_obj_set_style_grid_cell_x_align(bvm *vm);
extern int lvbe_obj_set_style_grid_cell_y_align(bvm *vm);
extern int lvbe_obj_set_style_grid_column_align(bvm *vm);
extern int lvbe_obj_set_style_grid_column_dsc_array(bvm *vm);
extern int lvbe_obj_set_style_grid_row_align(bvm *vm);
extern int lvbe_obj_set_style_grid_row_dsc_array(bvm *vm);
extern int lvbe_obj_set_style_height(bvm *vm);
extern int lvbe_obj_set_style_image_opa(bvm *vm);
extern int lvbe_obj_set_style_image_recolor(bvm *vm);
extern int lvbe_obj_set_style_image_recolor_opa(bvm *vm);
extern int lvbe_obj_set_style_image_opa(bvm *vm);
extern int lvbe_obj_set_style_image_recolor(bvm *vm);
extern int lvbe_obj_set_style_image_recolor_opa(bvm *vm);
extern int lvbe_obj_set_style_layout(bvm *vm);
extern int lvbe_obj_set_style_length(bvm *vm);
extern int lvbe_obj_set_style_line_color(bvm *vm);
extern int lvbe_obj_set_style_line_dash_gap(bvm *vm);
extern int lvbe_obj_set_style_line_dash_width(bvm *vm);
extern int lvbe_obj_set_style_line_opa(bvm *vm);
extern int lvbe_obj_set_style_line_rounded(bvm *vm);
extern int lvbe_obj_set_style_line_width(bvm *vm);
extern int lvbe_obj_set_style_margin_all(bvm *vm);
extern int lvbe_obj_set_style_margin_bottom(bvm *vm);
extern int lvbe_obj_set_style_margin_hor(bvm *vm);
extern int lvbe_obj_set_style_margin_left(bvm *vm);
extern int lvbe_obj_set_style_margin_right(bvm *vm);
extern int lvbe_obj_set_style_margin_top(bvm *vm);
extern int lvbe_obj_set_style_margin_ver(bvm *vm);
extern int lvbe_obj_set_style_max_height(bvm *vm);
extern int lvbe_obj_set_style_max_width(bvm *vm);
extern int lvbe_obj_set_style_min_height(bvm *vm);
extern int lvbe_obj_set_style_min_width(bvm *vm);
extern int lvbe_obj_set_style_opa(bvm *vm);
extern int lvbe_obj_set_style_opa_layered(bvm *vm);
extern int lvbe_obj_set_style_outline_color(bvm *vm);
extern int lvbe_obj_set_style_outline_opa(bvm *vm);
extern int lvbe_obj_set_style_outline_pad(bvm *vm);
extern int lvbe_obj_set_style_outline_width(bvm *vm);
extern int lvbe_obj_set_style_pad_all(bvm *vm);
extern int lvbe_obj_set_style_pad_bottom(bvm *vm);
extern int lvbe_obj_set_style_pad_column(bvm *vm);
extern int lvbe_obj_set_style_pad_gap(bvm *vm);
extern int lvbe_obj_set_style_pad_hor(bvm *vm);
extern int lvbe_obj_set_style_pad_left(bvm *vm);
extern int lvbe_obj_set_style_pad_right(bvm *vm);
extern int lvbe_obj_set_style_pad_row(bvm *vm);
extern int lvbe_obj_set_style_pad_top(bvm *vm);
extern int lvbe_obj_set_style_pad_ver(bvm *vm);
extern int lvbe_obj_set_style_radius(bvm *vm);
extern int lvbe_obj_set_style_shadow_color(bvm *vm);
extern int lvbe_obj_set_style_shadow_offset_x(bvm *vm);
extern int lvbe_obj_set_style_shadow_offset_y(bvm *vm);
extern int lvbe_obj_set_style_shadow_offset_x(bvm *vm);
extern int lvbe_obj_set_style_shadow_offset_y(bvm *vm);
extern int lvbe_obj_set_style_shadow_opa(bvm *vm);
extern int lvbe_obj_set_style_shadow_spread(bvm *vm);
extern int lvbe_obj_set_style_shadow_width(bvm *vm);
extern int lvbe_obj_set_style_size(bvm *vm);
extern int lvbe_obj_set_style_text_align(bvm *vm);
extern int lvbe_obj_set_style_text_color(bvm *vm);
extern int lvbe_obj_set_style_text_decor(bvm *vm);
extern int lvbe_obj_set_style_text_font(bvm *vm);
extern int lvbe_obj_set_style_text_letter_space(bvm *vm);
extern int lvbe_obj_set_style_text_line_space(bvm *vm);
extern int lvbe_obj_set_style_text_opa(bvm *vm);
extern int lvbe_obj_set_style_transform_rotation(bvm *vm);
extern int lvbe_obj_set_style_transform_height(bvm *vm);
extern int lvbe_obj_set_style_transform_pivot_x(bvm *vm);
extern int lvbe_obj_set_style_transform_pivot_y(bvm *vm);
extern int lvbe_obj_set_style_transform_rotation(bvm *vm);
extern int lvbe_obj_set_style_transform_scale(bvm *vm);
extern int lvbe_obj_set_style_transform_scale_x(bvm *vm);
extern int lvbe_obj_set_style_transform_scale_y(bvm *vm);
extern int lvbe_obj_set_style_transform_skew_x(bvm *vm);
extern int lvbe_obj_set_style_transform_skew_y(bvm *vm);
extern int lvbe_obj_set_style_transform_width(bvm *vm);
extern int lvbe_obj_set_style_transform_scale(bvm *vm);
extern int lvbe_obj_set_style_transition(bvm *vm);
extern int lvbe_obj_set_style_translate_x(bvm *vm);
extern int lvbe_obj_set_style_translate_y(bvm *vm);
extern int lvbe_obj_set_style_width(bvm *vm);
extern int lvbe_obj_set_style_x(bvm *vm);
extern int lvbe_obj_set_style_y(bvm *vm);
extern int lvbe_obj_set_user_data(bvm *vm);
extern int lvbe_obj_set_width(bvm *vm);
extern int lvbe_obj_set_x(bvm *vm);
extern int lvbe_obj_set_y(bvm *vm);
extern int lvbe_obj_stringify_id(bvm *vm);
extern int lvbe_obj_swap(bvm *vm);
extern int lvbe_obj_transform_point(bvm *vm);
extern int lvbe_obj_update_flag(bvm *vm);
extern int lvbe_obj_update_layout(bvm *vm);
extern int lvbe_obj_update_snap(bvm *vm);

/* `lv_event` external functions definitions */
extern int lvbe_event_get_code(bvm *vm);
extern int lvbe_event_get_cover_area(bvm *vm);
extern int lvbe_event_get_current_target(bvm *vm);
extern int lvbe_event_get_current_target_obj(bvm *vm);
extern int lvbe_event_get_hit_test_info(bvm *vm);
extern int lvbe_event_get_indev(bvm *vm);
extern int lvbe_event_get_key(bvm *vm);
extern int lvbe_event_get_layer(bvm *vm);
extern int lvbe_event_get_old_size(bvm *vm);
extern int lvbe_event_get_param(bvm *vm);
extern int lvbe_event_get_scroll_anim(bvm *vm);
extern int lvbe_event_get_self_size_info(bvm *vm);
extern int lvbe_event_get_target(bvm *vm);
extern int lvbe_event_get_target_obj(bvm *vm);
extern int lvbe_event_get_user_data(bvm *vm);
extern int lvbe_event_set_cover_res(bvm *vm);
extern int lvbe_event_set_ext_draw_size(bvm *vm);
extern int lvbe_event_stop_bubbling(bvm *vm);
extern int lvbe_event_stop_processing(bvm *vm);

/* `lv_display` external functions definitions */
extern int lvbe_display_add_event_cb(bvm *vm);
extern int lvbe_display_delete(bvm *vm);
extern int lvbe_display_delete(bvm *vm);
extern int lvbe_display_delete_event(bvm *vm);
extern int lvbe_display_delete_refr_timer(bvm *vm);
extern int lvbe_display_dpx(bvm *vm);
extern int lvbe_display_enable_invalidation(bvm *vm);
extern int lvbe_display_enable_invalidation(bvm *vm);
extern int lvbe_display_get_rotation(bvm *vm);
extern int lvbe_display_get_antialiasing(bvm *vm);
extern int lvbe_display_get_antialiasing(bvm *vm);
extern int lvbe_display_get_color_format(bvm *vm);
extern int lvbe_display_get_dpi(bvm *vm);
extern int lvbe_display_get_dpi(bvm *vm);
extern int lvbe_display_get_driver_data(bvm *vm);
extern int lvbe_display_get_event_count(bvm *vm);
extern int lvbe_display_get_horizontal_resolution(bvm *vm);
extern int lvbe_display_get_horizontal_resolution(bvm *vm);
extern int lvbe_display_get_inactive_time(bvm *vm);
extern int lvbe_display_get_inactive_time(bvm *vm);
extern int lvbe_display_get_layer_bottom(bvm *vm);
extern int lvbe_display_get_layer_sys(bvm *vm);
extern int lvbe_display_get_layer_sys(bvm *vm);
extern int lvbe_display_get_layer_top(bvm *vm);
extern int lvbe_display_get_layer_top(bvm *vm);
extern int lvbe_display_get_next(bvm *vm);
extern int lvbe_display_get_next(bvm *vm);
extern int lvbe_display_get_offset_x(bvm *vm);
extern int lvbe_display_get_offset_x(bvm *vm);
extern int lvbe_display_get_offset_y(bvm *vm);
extern int lvbe_display_get_offset_y(bvm *vm);
extern int lvbe_display_get_physical_horizontal_resolution(bvm *vm);
extern int lvbe_display_get_physical_horizontal_resolution(bvm *vm);
extern int lvbe_display_get_physical_vertical_resolution(bvm *vm);
extern int lvbe_display_get_physical_vertical_resolution(bvm *vm);
extern int lvbe_display_get_refr_timer(bvm *vm);
extern int lvbe_display_get_rotation(bvm *vm);
extern int lvbe_display_get_rotation(bvm *vm);
extern int lvbe_display_get_screen_active(bvm *vm);
extern int lvbe_display_get_screen_prev(bvm *vm);
extern int lvbe_display_get_screen_active(bvm *vm);
extern int lvbe_display_get_screen_prev(bvm *vm);
extern int lvbe_display_get_theme(bvm *vm);
extern int lvbe_display_get_theme(bvm *vm);
extern int lvbe_display_get_user_data(bvm *vm);
extern int lvbe_display_get_vertical_resolution(bvm *vm);
extern int lvbe_display_get_vertical_resolution(bvm *vm);
extern int lvbe_display_is_double_buffered(bvm *vm);
extern int lvbe_display_is_invalidation_enabled(bvm *vm);
extern int lvbe_display_is_invalidation_enabled(bvm *vm);
extern int lvbe_display_delete(bvm *vm);
extern int lvbe_display_remove_event_cb_with_user_data(bvm *vm);
extern int lvbe_display_send_event(bvm *vm);
extern int lvbe_display_send_event(bvm *vm);
extern int lvbe_display_set_rotation(bvm *vm);
extern int lvbe_display_set_antialiasing(bvm *vm);
extern int lvbe_display_set_buffers(bvm *vm);
extern int lvbe_display_set_color_format(bvm *vm);
extern int lvbe_display_set_default(bvm *vm);
extern int lvbe_display_set_default(bvm *vm);
extern int lvbe_display_set_dpi(bvm *vm);
extern int lvbe_display_set_driver_data(bvm *vm);
extern int lvbe_display_set_offset(bvm *vm);
extern int lvbe_display_set_physical_resolution(bvm *vm);
extern int lvbe_display_set_render_mode(bvm *vm);
extern int lvbe_display_set_resolution(bvm *vm);
extern int lvbe_display_set_rotation(bvm *vm);
extern int lvbe_display_set_rotation(bvm *vm);
extern int lvbe_display_set_theme(bvm *vm);
extern int lvbe_display_set_theme(bvm *vm);
extern int lvbe_display_set_user_data(bvm *vm);
extern int lvbe_display_trigger_activity(bvm *vm);
extern int lvbe_display_trigger_activity(bvm *vm);

/* `lv_indev` external functions definitions */
extern int lvbe_indev_add_event_cb(bvm *vm);
extern int lvbe_indev_delete(bvm *vm);
extern int lvbe_indev_delete(bvm *vm);
extern int lvbe_indev_enable(bvm *vm);
extern int lvbe_indev_get_display(bvm *vm);
extern int lvbe_indev_get_display(bvm *vm);
extern int lvbe_indev_get_driver_data(bvm *vm);
extern int lvbe_indev_get_event_count(bvm *vm);
extern int lvbe_indev_get_gesture_dir(bvm *vm);
extern int lvbe_indev_get_group(bvm *vm);
extern int lvbe_indev_get_key(bvm *vm);
extern int lvbe_indev_get_mode(bvm *vm);
extern int lvbe_indev_get_next(bvm *vm);
extern int lvbe_indev_get_next(bvm *vm);
extern int lvbe_indev_get_point(bvm *vm);
extern int lvbe_indev_get_read_timer(bvm *vm);
extern int lvbe_indev_get_scroll_dir(bvm *vm);
extern int lvbe_indev_get_scroll_obj(bvm *vm);
extern int lvbe_indev_get_state(bvm *vm);
extern int lvbe_indev_get_type(bvm *vm);
extern int lvbe_indev_get_user_data(bvm *vm);
extern int lvbe_indev_get_vect(bvm *vm);
extern int lvbe_indev_read(bvm *vm);
extern int lvbe_indev_delete(bvm *vm);
extern int lvbe_indev_remove_event(bvm *vm);
extern int lvbe_indev_remove_event_cb_with_user_data(bvm *vm);
extern int lvbe_indev_reset(bvm *vm);
extern int lvbe_indev_reset_long_press(bvm *vm);
extern int lvbe_indev_search_obj(bvm *vm);
extern int lvbe_indev_send_event(bvm *vm);
extern int lvbe_indev_send_event(bvm *vm);
extern int lvbe_indev_set_button_points(bvm *vm);
extern int lvbe_indev_set_cursor(bvm *vm);
extern int lvbe_indev_set_display(bvm *vm);
extern int lvbe_indev_set_display(bvm *vm);
extern int lvbe_indev_set_driver_data(bvm *vm);
extern int lvbe_indev_set_group(bvm *vm);
extern int lvbe_indev_set_mode(bvm *vm);
extern int lvbe_indev_set_type(bvm *vm);
extern int lvbe_indev_set_user_data(bvm *vm);
extern int lvbe_indev_wait_release(bvm *vm);

/* `lv_qrcode` external functions definitions */
extern int lvbe_qrcode_create(bvm *vm);
extern int lvbe_qrcode_set_dark_color(bvm *vm);
extern int lvbe_qrcode_set_light_color(bvm *vm);
extern int lvbe_qrcode_set_size(bvm *vm);
extern int lvbe_qrcode_update(bvm *vm);

/* `lv_anim` external functions definitions */
extern int lvbe_anim_custom_delete(bvm *vm);
extern int lvbe_anim_custom_get(bvm *vm);
extern int lvbe_anim_get_delay(bvm *vm);
extern int lvbe_anim_get_playtime(bvm *vm);
extern int lvbe_anim_get_repeat_count(bvm *vm);
extern int lvbe_anim_get_time(bvm *vm);
extern int lvbe_anim_get_user_data(bvm *vm);
extern int lvbe_anim_init(bvm *vm);
extern int lvbe_anim_set_bezier3_param(bvm *vm);
extern int lvbe_anim_set_completed_cb(bvm *vm);
extern int lvbe_anim_set_custom_exec_cb(bvm *vm);
extern int lvbe_anim_set_delay(bvm *vm);
extern int lvbe_anim_set_duration(bvm *vm);
extern int lvbe_anim_set_early_apply(bvm *vm);
extern int lvbe_anim_set_exec_cb(bvm *vm);
extern int lvbe_anim_set_get_value_cb(bvm *vm);
extern int lvbe_anim_set_path_cb(bvm *vm);
extern int lvbe_anim_set_playback_delay(bvm *vm);
extern int lvbe_anim_set_playback_duration(bvm *vm);
extern int lvbe_anim_set_playback_time(bvm *vm);
extern int lvbe_anim_set_completed_cb(bvm *vm);
extern int lvbe_anim_set_repeat_count(bvm *vm);
extern int lvbe_anim_set_repeat_delay(bvm *vm);
extern int lvbe_anim_set_start_cb(bvm *vm);
extern int lvbe_anim_set_time(bvm *vm);
extern int lvbe_anim_set_user_data(bvm *vm);
extern int lvbe_anim_set_values(bvm *vm);
extern int lvbe_anim_set_var(bvm *vm);
extern int lvbe_anim_start(bvm *vm);

/* `lv_timer` external functions definitions */
extern int lvbe_timer_delete(bvm *vm);
extern int lvbe_timer_delete(bvm *vm);
extern int lvbe_timer_get_next(bvm *vm);
extern int lvbe_timer_get_next(bvm *vm);
extern int lvbe_timer_get_paused(bvm *vm);
extern int lvbe_timer_get_user_data(bvm *vm);
extern int lvbe_timer_pause(bvm *vm);
extern int lvbe_timer_ready(bvm *vm);
extern int lvbe_timer_delete(bvm *vm);
extern int lvbe_timer_reset(bvm *vm);
extern int lvbe_timer_resume(bvm *vm);
extern int lvbe_timer_set_auto_delete(bvm *vm);
extern int lvbe_timer_set_cb(bvm *vm);
extern int lvbe_timer_set_period(bvm *vm);
extern int lvbe_timer_set_repeat_count(bvm *vm);
extern int lvbe_timer_set_user_data(bvm *vm);

/* `lv_animimg` external functions definitions */
extern int lvbe_animimg_create(bvm *vm);
extern int lvbe_animimg_get_duration(bvm *vm);
extern int lvbe_animimg_get_repeat_count(bvm *vm);
extern int lvbe_animimg_get_src_count(bvm *vm);
extern int lvbe_animimg_set_duration(bvm *vm);
extern int lvbe_animimg_set_repeat_count(bvm *vm);
extern int lvbe_animimg_set_src(bvm *vm);
extern int lvbe_animimg_start(bvm *vm);

/* `lv_arc` external functions definitions */
extern int lvbe_arc_align_obj_to_angle(bvm *vm);
extern int lvbe_arc_create(bvm *vm);
extern int lvbe_arc_get_rotation(bvm *vm);
extern int lvbe_arc_get_angle_end(bvm *vm);
extern int lvbe_arc_get_angle_start(bvm *vm);
extern int lvbe_arc_get_bg_angle_end(bvm *vm);
extern int lvbe_arc_get_bg_angle_start(bvm *vm);
extern int lvbe_arc_get_knob_offset(bvm *vm);
extern int lvbe_arc_get_max_value(bvm *vm);
extern int lvbe_arc_get_min_value(bvm *vm);
extern int lvbe_arc_get_mode(bvm *vm);
extern int lvbe_arc_get_rotation(bvm *vm);
extern int lvbe_arc_get_rotation(bvm *vm);
extern int lvbe_arc_get_value(bvm *vm);
extern int lvbe_arc_rotate_obj_to_angle(bvm *vm);
extern int lvbe_arc_set_rotation(bvm *vm);
extern int lvbe_arc_set_angles(bvm *vm);
extern int lvbe_arc_set_bg_angles(bvm *vm);
extern int lvbe_arc_set_bg_end_angle(bvm *vm);
extern int lvbe_arc_set_bg_start_angle(bvm *vm);
extern int lvbe_arc_set_change_rate(bvm *vm);
extern int lvbe_arc_set_end_angle(bvm *vm);
extern int lvbe_arc_set_knob_offset(bvm *vm);
extern int lvbe_arc_set_mode(bvm *vm);
extern int lvbe_arc_set_range(bvm *vm);
extern int lvbe_arc_set_rotation(bvm *vm);
extern int lvbe_arc_set_rotation(bvm *vm);
extern int lvbe_arc_set_start_angle(bvm *vm);
extern int lvbe_arc_set_value(bvm *vm);

/* `lv_bar` external functions definitions */
extern int lvbe_bar_create(bvm *vm);
extern int lvbe_bar_get_max_value(bvm *vm);
extern int lvbe_bar_get_min_value(bvm *vm);
extern int lvbe_bar_get_mode(bvm *vm);
extern int lvbe_bar_get_start_value(bvm *vm);
extern int lvbe_bar_get_value(bvm *vm);
extern int lvbe_bar_is_symmetrical(bvm *vm);
extern int lvbe_bar_set_mode(bvm *vm);
extern int lvbe_bar_set_range(bvm *vm);
extern int lvbe_bar_set_start_value(bvm *vm);
extern int lvbe_bar_set_value(bvm *vm);

/* `lv_button` external functions definitions */
extern int lvbe_button_create(bvm *vm);

/* `lv_buttonmatrix` external functions definitions */
extern int lvbe_buttonmatrix_get_button_text(bvm *vm);
extern int lvbe_buttonmatrix_clear_button_ctrl(bvm *vm);
extern int lvbe_buttonmatrix_clear_button_ctrl_all(bvm *vm);
extern int lvbe_buttonmatrix_create(bvm *vm);
extern int lvbe_buttonmatrix_get_button_text(bvm *vm);
extern int lvbe_buttonmatrix_get_map(bvm *vm);
extern int lvbe_buttonmatrix_get_one_checked(bvm *vm);
extern int lvbe_buttonmatrix_get_popovers(bvm *vm);
extern int lvbe_buttonmatrix_get_selected_button(bvm *vm);
extern int lvbe_buttonmatrix_has_button_ctrl(bvm *vm);
extern int lvbe_buttonmatrix_set_button_ctrl(bvm *vm);
extern int lvbe_buttonmatrix_set_button_ctrl_all(bvm *vm);
extern int lvbe_buttonmatrix_set_button_width(bvm *vm);
extern int lvbe_buttonmatrix_set_map(bvm *vm);
extern int lvbe_buttonmatrix_set_one_checked(bvm *vm);
extern int lvbe_buttonmatrix_set_selected_button(bvm *vm);

/* `lv_calendar` external functions definitions */
extern int lvbe_calendar_create(bvm *vm);
extern int lvbe_calendar_get_btnmatrix(bvm *vm);
extern int lvbe_calendar_get_highlighted_dates_num(bvm *vm);
extern int lvbe_calendar_header_arrow_create(bvm *vm);
extern int lvbe_calendar_header_dropdown_create(bvm *vm);
extern int lvbe_calendar_header_dropdown_set_year_list(bvm *vm);
extern int lvbe_calendar_set_day_names(bvm *vm);
extern int lvbe_calendar_set_showed_date(bvm *vm);
extern int lvbe_calendar_set_today_date(bvm *vm);

/* `lv_canvas` external functions definitions */
extern int lvbe_canvas_create(bvm *vm);
extern int lvbe_canvas_fill_bg(bvm *vm);
extern int lvbe_canvas_finish_layer(bvm *vm);
extern int lvbe_canvas_get_buf(bvm *vm);
extern int lvbe_canvas_get_image(bvm *vm);
extern int lvbe_canvas_get_px(bvm *vm);
extern int lvbe_canvas_init_layer(bvm *vm);
extern int lvbe_canvas_set_buffer(bvm *vm);
extern int lvbe_canvas_set_palette(bvm *vm);
extern int lvbe_canvas_set_px(bvm *vm);

/* `lv_chart` external functions definitions */
extern int lvbe_chart_add_cursor(bvm *vm);
extern int lvbe_chart_add_series(bvm *vm);
extern int lvbe_chart_create(bvm *vm);
extern int lvbe_chart_get_cursor_point(bvm *vm);
extern int lvbe_chart_get_first_point_center_offset(bvm *vm);
extern int lvbe_chart_get_point_count(bvm *vm);
extern int lvbe_chart_get_point_pos_by_id(bvm *vm);
extern int lvbe_chart_get_pressed_point(bvm *vm);
extern int lvbe_chart_get_series_next(bvm *vm);
extern int lvbe_chart_get_type(bvm *vm);
extern int lvbe_chart_get_x_array(bvm *vm);
extern int lvbe_chart_get_x_start_point(bvm *vm);
extern int lvbe_chart_get_y_array(bvm *vm);
extern int lvbe_chart_hide_series(bvm *vm);
extern int lvbe_chart_refresh(bvm *vm);
extern int lvbe_chart_remove_series(bvm *vm);
extern int lvbe_chart_set_all_value(bvm *vm);
extern int lvbe_chart_set_cursor_point(bvm *vm);
extern int lvbe_chart_set_cursor_pos(bvm *vm);
extern int lvbe_chart_set_div_line_count(bvm *vm);
extern int lvbe_chart_set_ext_x_array(bvm *vm);
extern int lvbe_chart_set_ext_y_array(bvm *vm);
extern int lvbe_chart_set_next_value(bvm *vm);
extern int lvbe_chart_set_next_value2(bvm *vm);
extern int lvbe_chart_set_point_count(bvm *vm);
extern int lvbe_chart_set_range(bvm *vm);
extern int lvbe_chart_set_series_color(bvm *vm);
extern int lvbe_chart_set_type(bvm *vm);
extern int lvbe_chart_set_update_mode(bvm *vm);
extern int lvbe_chart_set_value_by_id(bvm *vm);
extern int lvbe_chart_set_value_by_id2(bvm *vm);
extern int lvbe_chart_set_x_start_point(bvm *vm);

/* `lv_checkbox` external functions definitions */
extern int lvbe_checkbox_create(bvm *vm);
extern int lvbe_checkbox_get_text(bvm *vm);
extern int lvbe_checkbox_set_text(bvm *vm);
extern int lvbe_checkbox_set_text_static(bvm *vm);

/* `lv_dropdown` external functions definitions */
extern int lvbe_dropdown_add_option(bvm *vm);
extern int lvbe_dropdown_clear_options(bvm *vm);
extern int lvbe_dropdown_close(bvm *vm);
extern int lvbe_dropdown_create(bvm *vm);
extern int lvbe_dropdown_get_dir(bvm *vm);
extern int lvbe_dropdown_get_list(bvm *vm);
extern int lvbe_dropdown_get_option_count(bvm *vm);
extern int lvbe_dropdown_get_option_count(bvm *vm);
extern int lvbe_dropdown_get_option_index(bvm *vm);
extern int lvbe_dropdown_get_options(bvm *vm);
extern int lvbe_dropdown_get_selected(bvm *vm);
extern int lvbe_dropdown_get_selected_highlight(bvm *vm);
extern int lvbe_dropdown_get_selected_str(bvm *vm);
extern int lvbe_dropdown_get_symbol(bvm *vm);
extern int lvbe_dropdown_get_text(bvm *vm);
extern int lvbe_dropdown_is_open(bvm *vm);
extern int lvbe_dropdown_open(bvm *vm);
extern int lvbe_dropdown_set_dir(bvm *vm);
extern int lvbe_dropdown_set_options(bvm *vm);
extern int lvbe_dropdown_set_options_static(bvm *vm);
extern int lvbe_dropdown_set_selected(bvm *vm);
extern int lvbe_dropdown_set_selected_highlight(bvm *vm);
extern int lvbe_dropdown_set_symbol(bvm *vm);
extern int lvbe_dropdown_set_text(bvm *vm);

/* `lv_imagebutton` external functions definitions */
extern int lvbe_imagebutton_create(bvm *vm);
extern int lvbe_imagebutton_set_src(bvm *vm);
extern int lvbe_imagebutton_set_state(bvm *vm);

/* `lv_keyboard` external functions definitions */
extern int lvbe_keyboard_get_button_text(bvm *vm);
extern int lvbe_keyboard_create(bvm *vm);
extern int lvbe_keyboard_get_button_text(bvm *vm);
extern int lvbe_keyboard_get_map_array(bvm *vm);
extern int lvbe_keyboard_get_mode(bvm *vm);
extern int lvbe_keyboard_get_selected_button(bvm *vm);
extern int lvbe_keyboard_get_textarea(bvm *vm);
extern int lvbe_keyboard_set_mode(bvm *vm);
extern int lvbe_keyboard_set_popovers(bvm *vm);
extern int lvbe_keyboard_set_textarea(bvm *vm);

/* `lv_label` external functions definitions */
extern int lvbe_label_create(bvm *vm);
extern int lvbe_label_cut_text(bvm *vm);
extern int lvbe_label_get_letter_on(bvm *vm);
extern int lvbe_label_get_letter_pos(bvm *vm);
extern int lvbe_label_get_long_mode(bvm *vm);
extern int lvbe_label_get_text(bvm *vm);
extern int lvbe_label_get_text_selection_end(bvm *vm);
extern int lvbe_label_get_text_selection_start(bvm *vm);
extern int lvbe_label_ins_text(bvm *vm);
extern int lvbe_label_is_char_under_pos(bvm *vm);
extern int lvbe_label_set_long_mode(bvm *vm);
extern int lvbe_label_set_text(bvm *vm);
extern int lvbe_label_set_text_fmt(bvm *vm);
extern int lvbe_label_set_text_selection_end(bvm *vm);
extern int lvbe_label_set_text_selection_start(bvm *vm);
extern int lvbe_label_set_text_static(bvm *vm);

/* `lv_led` external functions definitions */
extern int lvbe_led_create(bvm *vm);
extern int lvbe_led_get_brightness(bvm *vm);
extern int lvbe_led_off(bvm *vm);
extern int lvbe_led_on(bvm *vm);
extern int lvbe_led_set_brightness(bvm *vm);
extern int lvbe_led_set_color(bvm *vm);
extern int lvbe_led_toggle(bvm *vm);

/* `lv_line` external functions definitions */
extern int lvbe_line_create(bvm *vm);
extern int lvbe_line_get_y_invert(bvm *vm);
extern int lvbe_line_set_points(bvm *vm);
extern int lvbe_line_set_y_invert(bvm *vm);

/* `lv_list` external functions definitions */
extern int lvbe_list_get_button_text(bvm *vm);
extern int lvbe_list_add_button(bvm *vm);
extern int lvbe_list_add_button(bvm *vm);
extern int lvbe_list_add_text(bvm *vm);
extern int lvbe_list_create(bvm *vm);
extern int lvbe_list_get_button_text(bvm *vm);
extern int lvbe_list_set_button_text(bvm *vm);
extern int lvbe_list_set_button_text(bvm *vm);

/* `lv_menu` external functions definitions */
extern int lvbe_menu_back_button_is_root(bvm *vm);
extern int lvbe_menu_clear_history(bvm *vm);
extern int lvbe_menu_cont_create(bvm *vm);
extern int lvbe_menu_create(bvm *vm);
extern int lvbe_menu_get_cur_main_page(bvm *vm);
extern int lvbe_menu_get_cur_sidebar_page(bvm *vm);
extern int lvbe_menu_get_main_header(bvm *vm);
extern int lvbe_menu_get_main_header_back_button(bvm *vm);
extern int lvbe_menu_get_sidebar_header(bvm *vm);
extern int lvbe_menu_get_sidebar_header_back_button(bvm *vm);
extern int lvbe_menu_page_create(bvm *vm);
extern int lvbe_menu_section_create(bvm *vm);
extern int lvbe_menu_separator_create(bvm *vm);
extern int lvbe_menu_set_load_page_event(bvm *vm);
extern int lvbe_menu_set_mode_header(bvm *vm);
extern int lvbe_menu_set_mode_root_back_button(bvm *vm);
extern int lvbe_menu_set_page(bvm *vm);
extern int lvbe_menu_set_page_title(bvm *vm);
extern int lvbe_menu_set_page_title_static(bvm *vm);
extern int lvbe_menu_set_sidebar_page(bvm *vm);

/* `lv_msgbox` external functions definitions */
extern int lvbe_msgbox_add_close_button(bvm *vm);
extern int lvbe_msgbox_add_footer_button(bvm *vm);
extern int lvbe_msgbox_add_header_button(bvm *vm);
extern int lvbe_msgbox_add_text(bvm *vm);
extern int lvbe_msgbox_add_title(bvm *vm);
extern int lvbe_msgbox_close(bvm *vm);
extern int lvbe_msgbox_close_async(bvm *vm);
extern int lvbe_msgbox_create(bvm *vm);
extern int lvbe_msgbox_get_content(bvm *vm);
extern int lvbe_msgbox_get_footer(bvm *vm);
extern int lvbe_msgbox_get_header(bvm *vm);
extern int lvbe_msgbox_get_title(bvm *vm);

/* `lv_roller` external functions definitions */
extern int lvbe_roller_create(bvm *vm);
extern int lvbe_roller_get_option_count(bvm *vm);
extern int lvbe_roller_get_option_count(bvm *vm);
extern int lvbe_roller_get_options(bvm *vm);
extern int lvbe_roller_get_selected(bvm *vm);
extern int lvbe_roller_get_selected_str(bvm *vm);
extern int lvbe_roller_set_options(bvm *vm);
extern int lvbe_roller_set_selected(bvm *vm);
extern int lvbe_roller_set_visible_row_count(bvm *vm);
extern int lvbe_roller_set_visible_row_count(bvm *vm);

/* `lv_scale` external functions definitions */
extern int lvbe_scale_add_section(bvm *vm);
extern int lvbe_scale_create(bvm *vm);
extern int lvbe_scale_get_angle_range(bvm *vm);
extern int lvbe_scale_get_label_show(bvm *vm);
extern int lvbe_scale_get_major_tick_every(bvm *vm);
extern int lvbe_scale_get_mode(bvm *vm);
extern int lvbe_scale_get_range_max_value(bvm *vm);
extern int lvbe_scale_get_range_min_value(bvm *vm);
extern int lvbe_scale_get_total_tick_count(bvm *vm);
extern int lvbe_scale_set_rotation(bvm *vm);
extern int lvbe_scale_set_angle_range(bvm *vm);
extern int lvbe_scale_set_image_needle_value(bvm *vm);
extern int lvbe_scale_set_label_show(bvm *vm);
extern int lvbe_scale_set_line_needle_value(bvm *vm);
extern int lvbe_scale_set_major_tick_every(bvm *vm);
extern int lvbe_scale_set_mode(bvm *vm);
extern int lvbe_scale_set_post_draw(bvm *vm);
extern int lvbe_scale_set_range(bvm *vm);
extern int lvbe_scale_set_rotation(bvm *vm);
extern int lvbe_scale_set_rotation(bvm *vm);
extern int lvbe_scale_set_text_src(bvm *vm);
extern int lvbe_scale_set_total_tick_count(bvm *vm);

/* `lv_slider` external functions definitions */
extern int lvbe_slider_create(bvm *vm);
extern int lvbe_slider_get_left_value(bvm *vm);
extern int lvbe_slider_get_max_value(bvm *vm);
extern int lvbe_slider_get_min_value(bvm *vm);
extern int lvbe_slider_get_mode(bvm *vm);
extern int lvbe_slider_get_value(bvm *vm);
extern int lvbe_slider_is_dragged(bvm *vm);
extern int lvbe_slider_is_symmetrical(bvm *vm);
extern int lvbe_slider_set_left_value(bvm *vm);
extern int lvbe_slider_set_mode(bvm *vm);
extern int lvbe_slider_set_range(bvm *vm);
extern int lvbe_slider_set_value(bvm *vm);

/* `lv_spangroup` external functions definitions */
extern int lvbe_spangroup_create(bvm *vm);
extern int lvbe_spangroup_delete_span(bvm *vm);
extern int lvbe_spangroup_get_align(bvm *vm);
extern int lvbe_spangroup_get_child(bvm *vm);
extern int lvbe_spangroup_get_expand_height(bvm *vm);
extern int lvbe_spangroup_get_expand_width(bvm *vm);
extern int lvbe_spangroup_get_indent(bvm *vm);
extern int lvbe_spangroup_get_max_line_height(bvm *vm);
extern int lvbe_spangroup_get_max_lines(bvm *vm);
extern int lvbe_spangroup_get_mode(bvm *vm);
extern int lvbe_spangroup_get_overflow(bvm *vm);
extern int lvbe_spangroup_get_span_count(bvm *vm);
extern int lvbe_spangroup_new_span(bvm *vm);
extern int lvbe_spangroup_refr_mode(bvm *vm);
extern int lvbe_spangroup_set_align(bvm *vm);
extern int lvbe_spangroup_set_indent(bvm *vm);
extern int lvbe_spangroup_set_max_lines(bvm *vm);
extern int lvbe_spangroup_set_mode(bvm *vm);
extern int lvbe_spangroup_set_overflow(bvm *vm);

/* `lv_spinbox` external functions definitions */
extern int lvbe_spinbox_create(bvm *vm);
extern int lvbe_spinbox_decrement(bvm *vm);
extern int lvbe_spinbox_get_rollover(bvm *vm);
extern int lvbe_spinbox_get_step(bvm *vm);
extern int lvbe_spinbox_get_value(bvm *vm);
extern int lvbe_spinbox_increment(bvm *vm);
extern int lvbe_spinbox_set_cursor_pos(bvm *vm);
extern int lvbe_spinbox_set_digit_format(bvm *vm);
extern int lvbe_spinbox_set_digit_step_direction(bvm *vm);
extern int lvbe_spinbox_set_range(bvm *vm);
extern int lvbe_spinbox_set_rollover(bvm *vm);
extern int lvbe_spinbox_set_step(bvm *vm);
extern int lvbe_spinbox_set_value(bvm *vm);
extern int lvbe_spinbox_step_next(bvm *vm);
extern int lvbe_spinbox_step_prev(bvm *vm);

/* `lv_spinner` external functions definitions */
extern int lvbe_spinner_create(bvm *vm);
extern int lvbe_spinner_set_anim_params(bvm *vm);

/* `lv_switch` external functions definitions */
extern int lvbe_switch_create(bvm *vm);

/* `lv_table` external functions definitions */
extern int lvbe_table_add_cell_ctrl(bvm *vm);
extern int lvbe_table_clear_cell_ctrl(bvm *vm);
extern int lvbe_table_create(bvm *vm);
extern int lvbe_table_get_cell_user_data(bvm *vm);
extern int lvbe_table_get_cell_value(bvm *vm);
extern int lvbe_table_get_column_count(bvm *vm);
extern int lvbe_table_get_column_width(bvm *vm);
extern int lvbe_table_get_column_count(bvm *vm);
extern int lvbe_table_get_column_width(bvm *vm);
extern int lvbe_table_get_row_count(bvm *vm);
extern int lvbe_table_get_row_count(bvm *vm);
extern int lvbe_table_get_selected_cell(bvm *vm);
extern int lvbe_table_has_cell_ctrl(bvm *vm);
extern int lvbe_table_set_cell_user_data(bvm *vm);
extern int lvbe_table_set_cell_value(bvm *vm);
extern int lvbe_table_set_cell_value_fmt(bvm *vm);
extern int lvbe_table_set_column_count(bvm *vm);
extern int lvbe_table_set_column_width(bvm *vm);
extern int lvbe_table_set_column_count(bvm *vm);
extern int lvbe_table_set_column_width(bvm *vm);
extern int lvbe_table_set_row_count(bvm *vm);
extern int lvbe_table_set_row_count(bvm *vm);

/* `lv_tabview` external functions definitions */
extern int lvbe_tabview_add_tab(bvm *vm);
extern int lvbe_tabview_create(bvm *vm);
extern int lvbe_tabview_get_content(bvm *vm);
extern int lvbe_tabview_get_tab_active(bvm *vm);
extern int lvbe_tabview_get_tab_active(bvm *vm);
extern int lvbe_tabview_get_tab_bar(bvm *vm);
extern int lvbe_tabview_get_tab_bar(bvm *vm);
extern int lvbe_tabview_get_tab_count(bvm *vm);
extern int lvbe_tabview_rename_tab(bvm *vm);
extern int lvbe_tabview_set_active(bvm *vm);
extern int lvbe_tabview_set_active(bvm *vm);
extern int lvbe_tabview_set_tab_bar_position(bvm *vm);
extern int lvbe_tabview_set_tab_bar_size(bvm *vm);

/* `lv_textarea` external functions definitions */
extern int lvbe_textarea_add_char(bvm *vm);
extern int lvbe_textarea_add_text(bvm *vm);
extern int lvbe_textarea_clear_selection(bvm *vm);
extern int lvbe_textarea_create(bvm *vm);
extern int lvbe_textarea_cursor_down(bvm *vm);
extern int lvbe_textarea_cursor_left(bvm *vm);
extern int lvbe_textarea_cursor_right(bvm *vm);
extern int lvbe_textarea_cursor_up(bvm *vm);
extern int lvbe_textarea_delete_char(bvm *vm);
extern int lvbe_textarea_delete_char_forward(bvm *vm);
extern int lvbe_textarea_get_accepted_chars(bvm *vm);
extern int lvbe_textarea_get_current_char(bvm *vm);
extern int lvbe_textarea_get_cursor_click_pos(bvm *vm);
extern int lvbe_textarea_get_cursor_pos(bvm *vm);
extern int lvbe_textarea_get_label(bvm *vm);
extern int lvbe_textarea_get_max_length(bvm *vm);
extern int lvbe_textarea_get_one_line(bvm *vm);
extern int lvbe_textarea_get_password_bullet(bvm *vm);
extern int lvbe_textarea_get_password_mode(bvm *vm);
extern int lvbe_textarea_get_password_show_time(bvm *vm);
extern int lvbe_textarea_get_placeholder_text(bvm *vm);
extern int lvbe_textarea_get_text(bvm *vm);
extern int lvbe_textarea_get_text_selection(bvm *vm);
extern int lvbe_textarea_set_accepted_chars(bvm *vm);
extern int lvbe_textarea_set_align(bvm *vm);
extern int lvbe_textarea_set_cursor_click_pos(bvm *vm);
extern int lvbe_textarea_set_cursor_pos(bvm *vm);
extern int lvbe_textarea_set_insert_replace(bvm *vm);
extern int lvbe_textarea_set_max_length(bvm *vm);
extern int lvbe_textarea_set_one_line(bvm *vm);
extern int lvbe_textarea_set_password_bullet(bvm *vm);
extern int lvbe_textarea_set_password_mode(bvm *vm);
extern int lvbe_textarea_set_password_show_time(bvm *vm);
extern int lvbe_textarea_set_placeholder_text(bvm *vm);
extern int lvbe_textarea_set_text(bvm *vm);
extern int lvbe_textarea_set_text_selection(bvm *vm);
extern int lvbe_textarea_text_is_selected(bvm *vm);

/* `lv_tileview` external functions definitions */
extern int lvbe_tileview_add_tile(bvm *vm);
extern int lvbe_tileview_create(bvm *vm);
extern int lvbe_tileview_get_tile_active(bvm *vm);
extern int lvbe_tileview_get_tile_active(bvm *vm);
extern int lvbe_tileview_set_tile(bvm *vm);
extern int lvbe_tileview_set_tile_by_index(bvm *vm);
extern int lvbe_tileview_set_tile_by_index(bvm *vm);

extern int be_ntv_lv_style_init(bvm *vm);
extern int be_ntv_lv_font_init(bvm *vm);
extern int be_ntv_lv_color_init(bvm *vm);
extern int be_ntv_lv_theme_init(bvm *vm);
extern int be_ntv_lv_image_init(bvm *vm);
extern int be_ntv_lv_group_init(bvm *vm);
extern int be_ntv_lv_obj_init(bvm *vm);
extern int be_ntv_lv_event_init(bvm *vm);
extern int be_ntv_lv_display_init(bvm *vm);
extern int be_ntv_lv_indev_init(bvm *vm);
extern int be_ntv_lv_qrcode_init(bvm *vm);
extern int be_ntv_lv_anim_init(bvm *vm);
extern int be_ntv_lv_timer_init(bvm *vm);
extern int be_ntv_lv_animimg_init(bvm *vm);
extern int be_ntv_lv_arc_init(bvm *vm);
extern int be_ntv_lv_bar_init(bvm *vm);
extern int be_ntv_lv_button_init(bvm *vm);
extern int be_ntv_lv_buttonmatrix_init(bvm *vm);
extern int be_ntv_lv_calendar_init(bvm *vm);
extern int be_ntv_lv_canvas_init(bvm *vm);
extern int be_ntv_lv_chart_init(bvm *vm);
extern int be_ntv_lv_checkbox_init(bvm *vm);
extern int be_ntv_lv_dropdown_init(bvm *vm);
extern int be_ntv_lv_imagebutton_init(bvm *vm);
extern int be_ntv_lv_keyboard_init(bvm *vm);
extern int be_ntv_lv_label_init(bvm *vm);
extern int be_ntv_lv_led_init(bvm *vm);
extern int be_ntv_lv_line_init(bvm *vm);
extern int be_ntv_lv_list_init(bvm *vm);
extern int be_ntv_lv_menu_init(bvm *vm);
extern int be_ntv_lv_msgbox_init(bvm *vm);
extern int be_ntv_lv_roller_init(bvm *vm);
extern int be_ntv_lv_scale_init(bvm *vm);
extern int be_ntv_lv_slider_init(bvm *vm);
extern int be_ntv_lv_spangroup_init(bvm *vm);
extern int be_ntv_lv_spinbox_init(bvm *vm);
extern int be_ntv_lv_spinner_init(bvm *vm);
extern int be_ntv_lv_switch_init(bvm *vm);
extern int be_ntv_lv_table_init(bvm *vm);
extern int be_ntv_lv_tabview_init(bvm *vm);
extern int be_ntv_lv_textarea_init(bvm *vm);
extern int be_ntv_lv_tileview_init(bvm *vm);

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
extern const bclass be_class_lv_slider;
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


/********************************************************************
** Solidified class: lv_style
********************************************************************/
#include "be_fixed_be_class_lv_style.h"
/* @const_object_info_begin
class be_class_lv_style (scope: global, name: lv_style, strings: weak) {
    _p, var
    init, func(lv_be_style_init)
    tostring, func(lv_x_tostring)
    member, func(lv_x_member)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_obj
********************************************************************/
#include "be_fixed_be_class_lv_obj.h"
/* @const_object_info_begin
class be_class_lv_obj (scope: global, name: lv_obj, strings: weak) {
    _p, var
    _class, comptr(&lv_obj_class)
    init, func(be_ntv_lv_obj_init)
    tostring, func(lv_x_tostring)
    member, func(lv_x_member)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_group
********************************************************************/
#include "be_fixed_be_class_lv_group.h"
/* @const_object_info_begin
class be_class_lv_group (scope: global, name: lv_group, strings: weak) {
    _p, var
    init, func(be_ntv_lv_group_init)
    tostring, func(lv_x_tostring)
    member, func(lv_x_member)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_indev
********************************************************************/
#include "be_fixed_be_class_lv_indev.h"
/* @const_object_info_begin
class be_class_lv_indev (scope: global, name: lv_indev, strings: weak) {
    _p, var
    init, func(lv0_init)
    tostring, func(lv_x_tostring)
    member, func(lv_x_member)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_display
********************************************************************/
#include "be_fixed_be_class_lv_display.h"
/* @const_object_info_begin
class be_class_lv_display (scope: global, name: lv_display, strings: weak) {
    _p, var
    init, func(lv0_init)
    tostring, func(lv_x_tostring)
    member, func(lv_x_member)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_timer
********************************************************************/
#include "be_fixed_be_class_lv_timer.h"
/* @const_object_info_begin
class be_class_lv_timer (scope: global, name: lv_timer, strings: weak) {
    _p, var
    init, func(lv0_init)
    tostring, func(lv_x_tostring)
    member, func(lv_x_member)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_anim
********************************************************************/
#include "be_fixed_be_class_lv_anim.h"
/* @const_object_info_begin
class be_class_lv_anim (scope: global, name: lv_anim, strings: weak) {
    _p, var
    init, func(lv_be_anim_init)
    tostring, func(lv_x_tostring)
    member, func(lv_x_member)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_event
********************************************************************/
#include "be_fixed_be_class_lv_event.h"
/* @const_object_info_begin
class be_class_lv_event (scope: global, name: lv_event, strings: weak) {
    _p, var
    init, func(lv0_init)
    tostring, func(lv_x_tostring)
    member, func(lv_x_member)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_font
********************************************************************/
#include "be_fixed_be_class_lv_font.h"
/* @const_object_info_begin
class be_class_lv_font (scope: global, name: lv_font, strings: weak) {
    _p, var
    init, func(lvbe_font_create)
    tostring, func(lv_x_tostring)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_theme
********************************************************************/
#include "be_fixed_be_class_lv_theme.h"
/* @const_object_info_begin
class be_class_lv_theme (scope: global, name: lv_theme, strings: weak) {
    _p, var
    init, func(lvbe_theme_create)
    tostring, func(lv_x_tostring)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_color
********************************************************************/
#include "be_fixed_be_class_lv_color.h"
/* @const_object_info_begin
class be_class_lv_color (scope: global, name: lv_color, strings: weak) {
    _p, var
    init, func(lco_init)
    tostring, func(lco_tostring)
    toint, func(lco_toint)
}
@const_object_info_end */

/* define extern function for arc_anim_start_angle and arc_anim_end_angle*/
extern void arc_anim_start_angle(void * obj, int32_t v);
extern void arc_anim_end_angle(void * obj, int32_t v);


/********************************************************************
** Solidified class: lv_image
********************************************************************/
#include "be_fixed_be_class_lv_image.h"
/* @const_object_info_begin
class be_class_lv_image (scope: global, name: lv_image, super: be_class_lv_obj, strings: weak) {
    _class, comptr(&lv_image_class)
    init, func(be_ntv_lv_image_init)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_qrcode
********************************************************************/
#include "be_fixed_be_class_lv_qrcode.h"
/* @const_object_info_begin
class be_class_lv_qrcode (scope: global, name: lv_qrcode, super: be_class_lv_obj, strings: weak) {
    _class, comptr(&lv_qrcode_class)
    init, func(be_ntv_lv_qrcode_init)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_animimg
********************************************************************/
#include "be_fixed_be_class_lv_animimg.h"
/* @const_object_info_begin
class be_class_lv_animimg (scope: global, name: lv_animimg, super: be_class_lv_image, strings: weak) {
    _class, comptr(&lv_animimg_class)
    init, func(be_ntv_lv_animimg_init)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_arc
********************************************************************/
#include "be_fixed_be_class_lv_arc.h"
/* @const_object_info_begin
class be_class_lv_arc (scope: global, name: lv_arc, super: be_class_lv_obj, strings: weak) {
    _class, comptr(&lv_arc_class)
    init, func(be_ntv_lv_arc_init)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_bar
********************************************************************/
#include "be_fixed_be_class_lv_bar.h"
/* @const_object_info_begin
class be_class_lv_bar (scope: global, name: lv_bar, super: be_class_lv_obj, strings: weak) {
    _class, comptr(&lv_bar_class)
    init, func(be_ntv_lv_bar_init)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_button
********************************************************************/
#include "be_fixed_be_class_lv_button.h"
/* @const_object_info_begin
class be_class_lv_button (scope: global, name: lv_button, super: be_class_lv_obj, strings: weak) {
    _class, comptr(&lv_button_class)
    init, func(be_ntv_lv_button_init)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_buttonmatrix
********************************************************************/
#include "be_fixed_be_class_lv_buttonmatrix.h"
/* @const_object_info_begin
class be_class_lv_buttonmatrix (scope: global, name: lv_buttonmatrix, super: be_class_lv_obj, strings: weak) {
    _class, comptr(&lv_buttonmatrix_class)
    init, func(be_ntv_lv_buttonmatrix_init)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_calendar
********************************************************************/
#include "be_fixed_be_class_lv_calendar.h"
/* @const_object_info_begin
class be_class_lv_calendar (scope: global, name: lv_calendar, super: be_class_lv_buttonmatrix, strings: weak) {
    _class, comptr(&lv_calendar_class)
    init, func(be_ntv_lv_calendar_init)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_canvas
********************************************************************/
#include "be_fixed_be_class_lv_canvas.h"
/* @const_object_info_begin
class be_class_lv_canvas (scope: global, name: lv_canvas, super: be_class_lv_image, strings: weak) {
    _class, comptr(&lv_canvas_class)
    init, func(be_ntv_lv_canvas_init)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_chart
********************************************************************/
#include "be_fixed_be_class_lv_chart.h"
/* @const_object_info_begin
class be_class_lv_chart (scope: global, name: lv_chart, super: be_class_lv_obj, strings: weak) {
    _class, comptr(&lv_chart_class)
    init, func(be_ntv_lv_chart_init)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_checkbox
********************************************************************/
#include "be_fixed_be_class_lv_checkbox.h"
/* @const_object_info_begin
class be_class_lv_checkbox (scope: global, name: lv_checkbox, super: be_class_lv_obj, strings: weak) {
    _class, comptr(&lv_checkbox_class)
    init, func(be_ntv_lv_checkbox_init)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_dropdown
********************************************************************/
#include "be_fixed_be_class_lv_dropdown.h"
/* @const_object_info_begin
class be_class_lv_dropdown (scope: global, name: lv_dropdown, super: be_class_lv_obj, strings: weak) {
    _class, comptr(&lv_dropdown_class)
    init, func(be_ntv_lv_dropdown_init)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_imagebutton
********************************************************************/
#include "be_fixed_be_class_lv_imagebutton.h"
/* @const_object_info_begin
class be_class_lv_imagebutton (scope: global, name: lv_imagebutton, super: be_class_lv_obj, strings: weak) {
    _class, comptr(&lv_imagebutton_class)
    init, func(be_ntv_lv_imagebutton_init)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_keyboard
********************************************************************/
#include "be_fixed_be_class_lv_keyboard.h"
/* @const_object_info_begin
class be_class_lv_keyboard (scope: global, name: lv_keyboard, super: be_class_lv_buttonmatrix, strings: weak) {
    _class, comptr(&lv_keyboard_class)
    init, func(be_ntv_lv_keyboard_init)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_label
********************************************************************/
#include "be_fixed_be_class_lv_label.h"
/* @const_object_info_begin
class be_class_lv_label (scope: global, name: lv_label, super: be_class_lv_obj, strings: weak) {
    _class, comptr(&lv_label_class)
    init, func(be_ntv_lv_label_init)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_led
********************************************************************/
#include "be_fixed_be_class_lv_led.h"
/* @const_object_info_begin
class be_class_lv_led (scope: global, name: lv_led, super: be_class_lv_obj, strings: weak) {
    _class, comptr(&lv_led_class)
    init, func(be_ntv_lv_led_init)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_line
********************************************************************/
#include "be_fixed_be_class_lv_line.h"
/* @const_object_info_begin
class be_class_lv_line (scope: global, name: lv_line, super: be_class_lv_obj, strings: weak) {
    _class, comptr(&lv_line_class)
    init, func(be_ntv_lv_line_init)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_list
********************************************************************/
#include "be_fixed_be_class_lv_list.h"
/* @const_object_info_begin
class be_class_lv_list (scope: global, name: lv_list, super: be_class_lv_obj, strings: weak) {
    _class, comptr(&lv_list_class)
    init, func(be_ntv_lv_list_init)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_menu
********************************************************************/
#include "be_fixed_be_class_lv_menu.h"
/* @const_object_info_begin
class be_class_lv_menu (scope: global, name: lv_menu, super: be_class_lv_obj, strings: weak) {
    _class, comptr(&lv_menu_class)
    init, func(be_ntv_lv_menu_init)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_msgbox
********************************************************************/
#include "be_fixed_be_class_lv_msgbox.h"
/* @const_object_info_begin
class be_class_lv_msgbox (scope: global, name: lv_msgbox, super: be_class_lv_obj, strings: weak) {
    _class, comptr(&lv_msgbox_class)
    init, func(be_ntv_lv_msgbox_init)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_roller
********************************************************************/
#include "be_fixed_be_class_lv_roller.h"
/* @const_object_info_begin
class be_class_lv_roller (scope: global, name: lv_roller, super: be_class_lv_obj, strings: weak) {
    _class, comptr(&lv_roller_class)
    init, func(be_ntv_lv_roller_init)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_scale
********************************************************************/
#include "be_fixed_be_class_lv_scale.h"
/* @const_object_info_begin
class be_class_lv_scale (scope: global, name: lv_scale, super: be_class_lv_obj, strings: weak) {
    _class, comptr(&lv_scale_class)
    init, func(be_ntv_lv_scale_init)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_slider
********************************************************************/
#include "be_fixed_be_class_lv_slider.h"
/* @const_object_info_begin
class be_class_lv_slider (scope: global, name: lv_slider, super: be_class_lv_obj, strings: weak) {
    _class, comptr(&lv_slider_class)
    init, func(be_ntv_lv_slider_init)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_spangroup
********************************************************************/
#include "be_fixed_be_class_lv_spangroup.h"
/* @const_object_info_begin
class be_class_lv_spangroup (scope: global, name: lv_spangroup, super: be_class_lv_obj, strings: weak) {
    _class, comptr(&lv_spangroup_class)
    init, func(be_ntv_lv_spangroup_init)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_spinbox
********************************************************************/
#include "be_fixed_be_class_lv_spinbox.h"
/* @const_object_info_begin
class be_class_lv_spinbox (scope: global, name: lv_spinbox, super: be_class_lv_textarea, strings: weak) {
    _class, comptr(&lv_spinbox_class)
    init, func(be_ntv_lv_spinbox_init)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_spinner
********************************************************************/
#include "be_fixed_be_class_lv_spinner.h"
/* @const_object_info_begin
class be_class_lv_spinner (scope: global, name: lv_spinner, super: be_class_lv_arc, strings: weak) {
    _class, comptr(&lv_spinner_class)
    init, func(be_ntv_lv_spinner_init)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_switch
********************************************************************/
#include "be_fixed_be_class_lv_switch.h"
/* @const_object_info_begin
class be_class_lv_switch (scope: global, name: lv_switch, super: be_class_lv_obj, strings: weak) {
    _class, comptr(&lv_switch_class)
    init, func(be_ntv_lv_switch_init)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_table
********************************************************************/
#include "be_fixed_be_class_lv_table.h"
/* @const_object_info_begin
class be_class_lv_table (scope: global, name: lv_table, super: be_class_lv_obj, strings: weak) {
    _class, comptr(&lv_table_class)
    init, func(be_ntv_lv_table_init)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_tabview
********************************************************************/
#include "be_fixed_be_class_lv_tabview.h"
/* @const_object_info_begin
class be_class_lv_tabview (scope: global, name: lv_tabview, super: be_class_lv_obj, strings: weak) {
    _class, comptr(&lv_tabview_class)
    init, func(be_ntv_lv_tabview_init)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_textarea
********************************************************************/
#include "be_fixed_be_class_lv_textarea.h"
/* @const_object_info_begin
class be_class_lv_textarea (scope: global, name: lv_textarea, super: be_class_lv_obj, strings: weak) {
    _class, comptr(&lv_textarea_class)
    init, func(be_ntv_lv_textarea_init)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_tileview
********************************************************************/
#include "be_fixed_be_class_lv_tileview.h"
/* @const_object_info_begin
class be_class_lv_tileview (scope: global, name: lv_tileview, super: be_class_lv_obj, strings: weak) {
    _class, comptr(&lv_tileview_class)
    init, func(be_ntv_lv_tileview_init)
}
@const_object_info_end */

