
/********************************************************************
 * Generated code, don't edit
  *******************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "be_ctypes.h"
#include "be_mapping.h"
#include "../src/lv_berry.h"
#include "../src/lv_colorwheel.h"
#include "../src/lv_stripes.h"

/* verb prefixes, index is the verb code in `lv_ntv_method_t.sig` */
#define LV_METHOD_VERBS_NB  6
static const char * const lv_method_verbs[LV_METHOD_VERBS_NB] = {
  "", "get_", "get_style_", "is_", "set_", "set_style_"
};

/* method names without verb prefix, `\0` separated */
static const char lv_method_names[] =
  /*     0 */ "text_outline_stroke_color_filtered\0"
  /*    35 */ "original_horizontal_resolution\0"
  /*    66 */ "physical_horizontal_resolution\0"
  /*    97 */ "remove_event_cb_with_user_data\0"
  /*   128 */ "header_dropdown_set_year_list\0"
  /*   158 */ "original_vertical_resolution\0"
  /*   187 */ "physical_vertical_resolution\0"
  /*   216 */ "calculate_style_text_align\0"
  /*   243 */ "sidebar_header_back_button\0"
  /*   270 */ "bg_image_recolor_filtered\0"
  /*   296 */ "first_point_center_offset\0"
  /*   322 */ "invalidated_draw_buf_size\0"
  /*   348 */ "text_outline_stroke_color\0"
  /*   374 */ "text_outline_stroke_width\0"
  /*   400 */ "scroll_to_view_recursive\0"
  /*   425 */ "style_any_height_content\0"
  /*   450 */ "style_apply_color_filter\0"
  /*   475 */ "add_play_timeline_event\0"
  /*   499 */ "calculate_ext_draw_size\0"
  /*   523 */ "main_header_back_button\0"
  /*   547 */ "remove_local_style_prop\0"
  /*   571 */ "section_style_indicator\0"
  /*   595 */ "style_any_width_content\0"
  /*   619 */ "text_outline_stroke_opa\0"
  /*   643 */ "bg_grad_color_filtered\0"
  /*   666 */ "long_press_repeat_time\0"
  /*   689 */ "outline_color_filtered\0"
  /*   712 */ "transform_scale_x_safe\0"
  /*   735 */ "transform_scale_y_safe\0"
  /*   758 */ "unregister_vsync_event\0"
  /*   781 */ "accepted_chars_static\0"
  /*   803 */ "add_screen_load_event\0"
  /*   825 */ "border_color_filtered\0"
  /*   847 */ "clear_button_ctrl_all\0"
  /*   869 */ "grid_cell_column_span\0"
  /*   891 */ "grid_column_dsc_array\0"
  /*   913 */ "highlighted_dates_num\0"
  /*   935 */ "mode_root_back_button\0"
  /*   957 */ "refresh_ext_draw_size\0"
  /*   979 */ "shadow_color_filtered\0"
  /*  1001 */ "text_horizontal_align\0"
  /*  1023 */ "transform_point_array\0"
  /*  1045 */ "bg_image_recolor_opa\0"
  /*  1066 */ "digit_step_direction\0"
  /*  1087 */ "grid_cell_column_pos\0"
  /*  1108 */ "img_recolor_filtered\0"
  /*  1129 */ "invalidation_enabled\0"
  /*  1150 */ "mark_layout_as_dirty\0"
  /*  1171 */ "scrollbar_invalidate\0"
  /*  1192 */ "text_selection_start\0"
  /*  1213 */ "add_header_dropdown\0"
  /*  1233 */ "back_button_is_root\0"
  /*  1253 */ "buffers_with_stride\0"
  /*  1273 */ "calc_dynamic_height\0"
  /*  1293 */ "child_count_by_type\0"
  /*  1313 */ "delete_char_forward\0"
  /*  1333 */ "enable_invalidation\0"
  /*  1353 */ "init_draw_image_dsc\0"
  /*  1373 */ "init_draw_label_dsc\0"
  /*  1393 */ "physical_resolution\0"
  /*  1413 */ "point_array_mutable\0"
  /*  1433 */ "rotate_obj_to_angle\0"
  /*  1453 */ "section_style_items\0"
  /*  1473 */ "series_value_by_id2\0"
  /*  1493 */ "style_apply_recolor\0"
  /*  1513 */ "text_color_filtered\0"
  /*  1533 */ "text_vertical_align\0"
  /*  1553 */ "align_obj_to_angle\0"
  /*  1572 */ "arc_color_filtered\0"
  /*  1591 */ "bg_img_recolor_opa\0"
  /*  1610 */ "calc_dynamic_width\0"
  /*  1629 */ "current_target_obj\0"
  /*  1648 */ "grid_cell_row_span\0"
  /*  1667 */ "grid_row_dsc_array\0"
  /*  1686 */ "hor_div_line_count\0"
  /*  1705 */ "image_needle_value\0"
  /*  1724 */ "init_draw_line_dsc\0"
  /*  1743 */ "init_draw_rect_dsc\0"
  /*  1762 */ "password_show_time\0"
  /*  1781 */ "rotary_sensitivity\0"
  /*  1800 */ "section_style_main\0"
  /*  1819 */ "selected_highlight\0"
  /*  1838 */ "series_ext_x_array\0"
  /*  1857 */ "series_ext_y_array\0"
  /*  1876 */ "series_value_by_id\0"
  /*  1895 */ "short_click_streak\0"
  /*  1914 */ "style_get_disabled\0"
  /*  1933 */ "style_set_disabled\0"
  /*  1952 */ "text_selection_end\0"
  /*  1971 */ "transform_rotation\0"
  /*  1990 */ "transformed_height\0"
  /*  2009 */ "ver_div_line_count\0"
  /*  2028 */ "add_footer_button\0"
  /*  2046 */ "add_header_button\0"
  /*  2064 */ "bg_color_filtered\0"
  /*  2082 */ "clear_button_ctrl\0"
  /*  2100 */ "delete_refr_timer\0"
  /*  2118 */ "double_click_time\0"
  /*  2136 */ "draw_ticks_on_top\0"
  /*  2154 */ "flex_in_new_track\0"
  /*  2172 */ "free_user_data_cb\0"
  /*  2190 */ "grid_cell_row_pos\0"
  /*  2208 */ "grid_cell_x_align\0"
  /*  2226 */ "grid_cell_y_align\0"
  /*  2244 */ "grid_column_align\0"
  /*  2262 */ "init_draw_arc_dsc\0"
  /*  2280 */ "layout_positioned\0"
  /*  2298 */ "line_needle_value\0"
  /*  2316 */ "page_title_static\0"
  /*  2334 */ "recolor_recursive\0"
  /*  2352 */ "refresh_self_size\0"
  /*  2370 */ "scroll_by_bounded\0"
  /*  2388 */ "section_max_value\0"
  /*  2406 */ "section_min_value\0"
  /*  2424 */ "text_leading_trim\0"
  /*  2442 */ "text_letter_space\0"
  /*  2460 */ "transform_pivot_x\0"
  /*  2478 */ "transform_pivot_y\0"
  /*  2496 */ "transform_scale_x\0"
  /*  2514 */ "transform_scale_y\0"
  /*  2532 */ "transformed_width\0"
  /*  2550 */ "visible_row_count\0"
  /*  2568 */ "add_close_button\0"
  /*  2585 */ "add_header_arrow\0"
  /*  2602 */ "bg_image_recolor\0"
  /*  2619 */ "color_filter_dsc\0"
  /*  2636 */ "color_filter_opa\0"
  /*  2653 */ "color_mode_fixed\0"
  /*  2670 */ "cur_sidebar_page\0"
  /*  2687 */ "cursor_click_pos\0"
  /*  2704 */ "flex_cross_place\0"
  /*  2721 */ "flex_track_place\0"
  /*  2738 */ "invalidated_area\0"
  /*  2755 */ "major_tick_every\0"
  /*  2772 */ "move_children_by\0"
  /*  2789 */ "physical_hor_res\0"
  /*  2806 */ "physical_ver_res\0"
  /*  2823 */ "placeholder_text\0"
  /*  2840 */ "remove_event_dsc\0"
  /*  2857 */ "remove_style_all\0"
  /*  2874 */ "reset_long_press\0"
  /*  2891 */ "reverse_duration\0"
  /*  2908 */ "rotate_point_ccw\0"
  /*  2925 */ "send_vsync_event\0"
  /*  2942 */ "span_text_static\0"
  /*  2959 */ "stop_scroll_anim\0"
  /*  2976 */ "tab_bar_position\0"
  /*  2993 */ "text_is_selected\0"
  /*  3010 */ "total_tick_count\0"
  /*  3027 */ "transform_height\0"
  /*  3044 */ "transform_skew_x\0"
  /*  3061 */ "transform_skew_y\0"
  /*  3078 */ "transformed_area\0"
  /*  3095 */ "translate_radial\0"
  /*  3112 */ "trigger_activity\0"
  /*  3129 */ "area_is_visible\0"
  /*  3145 */ "bitmap_mask_src\0"
  /*  3161 */ "center_offset_x\0"
  /*  3177 */ "center_offset_y\0"
  /*  3193 */ "clear_cell_ctrl\0"
  /*  3209 */ "clear_selection\0"
  /*  3225 */ "double_buffered\0"
  /*  3241 */ "flex_main_place\0"
  /*  3257 */ "has_button_ctrl\0"
  /*  3273 */ "invalidate_area\0"
  /*  3289 */ "line_dash_width\0"
  /*  3305 */ "load_page_event\0"
  /*  3321 */ "long_press_time\0"
  /*  3337 */ "matrix_rotation\0"
  /*  3353 */ "max_line_height\0"
  /*  3369 */ "move_background\0"
  /*  3385 */ "move_foreground\0"
  /*  3401 */ "password_bullet\0"
  /*  3417 */ "point_pos_by_id\0"
  /*  3433 */ "range_max_value\0"
  /*  3449 */ "range_min_value\0"
  /*  3465 */ "readjust_scroll\0"
  /*  3481 */ "remove_all_objs\0"
  /*  3497 */ "remove_event_cb\0"
  /*  3513 */ "reset_transform\0"
  /*  3529 */ "selected_button\0"
  /*  3545 */ "shadow_offset_x\0"
  /*  3561 */ "shadow_offset_y\0"
  /*  3577 */ "sibling_by_type\0"
  /*  3593 */ "stop_processing\0"
  /*  3609 */ "text_line_space\0"
  /*  3625 */ "transform_angle\0"
  /*  3641 */ "transform_point\0"
  /*  3657 */ "transform_scale\0"
  /*  3673 */ "transform_width\0"
  /*  3689 */ "visible_row_cnt\0"
  /*  3705 */ "accepted_chars\0"
  /*  3720 */ "axis_max_value\0"
  /*  3735 */ "axis_min_value\0"
  /*  3750 */ "bg_angle_start\0"
  /*  3765 */ "bg_image_tiled\0"
  /*  3780 */ "bg_img_recolor\0"
  /*  3795 */ "bg_start_angle\0"
  /*  3810 */ "bitmap_map_src\0"
  /*  3825 */ "cell_user_data\0"
  /*  3840 */ "cell_value_fmt\0"
  /*  3855 */ "char_under_pos\0"
  /*  3870 */ "clamped_height\0"
  /*  3885 */ "class_init_obj\0"
  /*  3900 */ "content_coords\0"
  /*  3915 */ "content_height\0"
  /*  3930 */ "current_target\0"
  /*  3945 */ "custom_exec_cb\0"
  /*  3960 */ "delete_delayed\0"
  /*  3975 */ "ext_click_area\0"
  /*  3990 */ "grid_dsc_array\0"
  /*  4005 */ "grid_row_align\0"
  /*  4020 */ "has_style_prop\0"
  /*  4035 */ "image_colorkey\0"
  /*  4050 */ "in_widget_tree\0"
  /*  4065 */ "insert_replace\0"
  /*  4080 */ "options_static\0"
  /*  4095 */ "points_mutable\0"
  /*  4110 */ "refocus_policy\0"
  /*  4125 */ "screen_loading\0"
  /*  4140 */ "scroll_to_view\0"
  /*  4155 */ "scrollbar_area\0"
  /*  4170 */ "scrollbar_mode\0"
  /*  4185 */ "self_size_info\0"
  /*  4200 */ "series_values2\0"
  /*  4215 */ "series_x_array\0"
  /*  4230 */ "series_y_array\0"
  /*  4245 */ "sidebar_header\0"
  /*  4260 */ "stop_trickling\0"
  /*  4275 */ "tasmota_logo36\0"
  /*  4290 */ "text_selection\0"
  /*  4305 */ "transform_zoom\0"
  /*  4320 */ "anim_duration\0"
  /*  4334 */ "arc_image_src\0"
  /*  4348 */ "bezier3_param\0"
  /*  4362 */ "bg_grad_color\0"
  /*  4376 */ "button_points\0"
  /*  4390 */ "child_by_type\0"
  /*  4404 */ "clamped_width\0"
  /*  4418 */ "clear_history\0"
  /*  4432 */ "clear_options\0"
  /*  4446 */ "content_width\0"
  /*  4460 */ "cur_main_page\0"
  /*  4474 */ "custom_delete\0"
  /*  4488 */ "dec_point_pos\0"
  /*  4502 */ "expand_height\0"
  /*  4516 */ "has_cell_ctrl\0"
  /*  4530 */ "hit_test_info\0"
  /*  4544 */ "inactive_time\0"
  /*  4558 */ "index_by_type\0"
  /*  4572 */ "line_dash_gap\0"
  /*  4586 */ "margin_bottom\0"
  /*  4600 */ "move_to_index\0"
  /*  4614 */ "opa_recursive\0"
  /*  4628 */ "outline_color\0"
  /*  4642 */ "outline_width\0"
  /*  4656 */ "password_mode\0"
  /*  4670 */ "pressed_point\0"
  /*  4684 */ "radial_offset\0"
  /*  4698 */ "refresh_style\0"
  /*  4712 */ "remove_cursor\0"
  /*  4726 */ "remove_series\0"
  /*  4740 */ "replace_style\0"
  /*  4754 */ "reverse_delay\0"
  /*  4768 */ "screen_active\0"
  /*  4782 */ "scroll_bottom\0"
  /*  4796 */ "scroll_snap_x\0"
  /*  4810 */ "scroll_snap_y\0"
  /*  4824 */ "section_range\0"
  /*  4838 */ "selected_cell\0"
  /*  4852 */ "series_values\0"
  /*  4866 */ "shadow_spread\0"
  /*  4880 */ "span_by_point\0"
  /*  4894 */ "span_text_fmt\0"
  /*  4908 */ "stop_bubbling\0"
  /*  4922 */ "tile_by_index\0"
  /*  4936 */ "trig_activity\0"
  /*  4950 */ "update_layout\0"
  /*  4964 */ "x_start_point\0"
  /*  4978 */ "add_event_cb\0"
  /*  4991 */ "add_text_fmt\0"
  /*  5004 */ "antialiasing\0"
  /*  5017 */ "bg_angle_end\0"
  /*  5030 */ "bg_end_angle\0"
  /*  5043 */ "bg_grad_stop\0"
  /*  5056 */ "bg_image_opa\0"
  /*  5069 */ "bg_image_src\0"
  /*  5082 */ "bg_img_tiled\0"
  /*  5095 */ "bg_main_stop\0"
  /*  5108 */ "border_color\0"
  /*  5121 */ "border_width\0"
  /*  5134 */ "button_width\0"
  /*  5147 */ "chinese_mode\0"
  /*  5160 */ "color_format\0"
  /*  5173 */ "column_count\0"
  /*  5186 */ "column_width\0"
  /*  5199 */ "completed_cb\0"
  /*  5212 */ "current_char\0"
  /*  5225 */ "cursor_point\0"
  /*  5238 */ "cursor_pos_x\0"
  /*  5251 */ "cursor_pos_y\0"
  /*  5264 */ "cursor_right\0"
  /*  5277 */ "delete_async\0"
  /*  5290 */ "digit_format\0"
  /*  5303 */ "expand_width\0"
  /*  5316 */ "finish_layer\0"
  /*  5329 */ "focus_freeze\0"
  /*  5342 */ "get_value_cb\0"
  /*  5355 */ "has_flag_any\0"
  /*  5368 */ "layer_bottom\0"
  /*  5381 */ "line_rounded\0"
  /*  5394 */ "margin_right\0"
  /*  5407 */ "obj_by_index\0"
  /*  5420 */ "option_count\0"
  /*  5433 */ "option_index\0"
  /*  5446 */ "radio_button\0"
  /*  5459 */ "remove_event\0"
  /*  5472 */ "remove_state\0"
  /*  5485 */ "remove_style\0"
  /*  5498 */ "remove_theme\0"
  /*  5511 */ "render_valid\0"
  /*  5524 */ "repeat_count\0"
  /*  5537 */ "repeat_delay\0"
  /*  5550 */ "reverse_time\0"
  /*  5563 */ "rotate_point\0"
  /*  5576 */ "scroll_limit\0"
  /*  5589 */ "scroll_right\0"
  /*  5602 */ "scroll_throw\0"
  /*  5615 */ "selected_str\0"
  /*  5628 */ "series_color\0"
  /*  5641 */ "shadow_color\0"
  /*  5654 */ "shadow_ofs_x\0"
  /*  5667 */ "shadow_ofs_y\0"
  /*  5680 */ "shadow_width\0"
  /*  5693 */ "space_bottom\0"
  /*  5706 */ "stringify_id\0"
  /*  5719 */ "stripe_color\0"
  /*  5732 */ "stripe_width\0"
  /*  5745 */ "tab_bar_size\0"
  /*  5758 */ "tasmota_logo\0"
  /*  5771 */ "wait_release\0"
  /*  5784 */ "add_section\0"
  /*  5796 */ "angle_range\0"
  /*  5808 */ "anim_params\0"
  /*  5820 */ "arc_rounded\0"
  /*  5832 */ "auto_delete\0"
  /*  5844 */ "bg_grad_dir\0"
  /*  5856 */ "bg_grad_opa\0"
  /*  5868 */ "bg_main_opa\0"
  /*  5880 */ "border_post\0"
  /*  5892 */ "border_side\0"
  /*  5904 */ "button_text\0"
  /*  5916 */ "change_rate\0"
  /*  5928 */ "child_count\0"
  /*  5940 */ "clear_state\0"
  /*  5952 */ "clip_corner\0"
  /*  5964 */ "close_async\0"
  /*  5976 */ "cursor_down\0"
  /*  5988 */ "cursor_left\0"
  /*  6000 */ "delete_char\0"
  /*  6012 */ "delete_span\0"
  /*  6024 */ "digit_count\0"
  /*  6036 */ "driver_data\0"
  /*  6048 */ "early_apply\0"
  /*  6060 */ "end_overlap\0"
  /*  6072 */ "event_count\0"
  /*  6084 */ "gesture_dir\0"
  /*  6096 */ "hide_series\0"
  /*  6108 */ "inner_align\0"
  /*  6120 */ "knob_offset\0"
  /*  6132 */ "light_color\0"
  /*  6144 */ "main_header\0"
  /*  6156 */ "margin_left\0"
  /*  6168 */ "mode_header\0"
  /*  6180 */ "month_shown\0"
  /*  6192 */ "next_value2\0"
  /*  6204 */ "one_checked\0"
  /*  6216 */ "opa_layered\0"
  /*  6228 */ "orientation\0"
  /*  6240 */ "outline_opa\0"
  /*  6252 */ "outline_pad\0"
  /*  6264 */ "point_count\0"
  /*  6276 */ "press_moved\0"
  /*  6288 */ "remove_flag\0"
  /*  6300 */ "remove_prop\0"
  /*  6312 */ "render_mode\0"
  /*  6324 */ "rotary_diff\0"
  /*  6336 */ "rotate_area\0"
  /*  6348 */ "screen_prev\0"
  /*  6360 */ "scroll_left\0"
  /*  6372 */ "scroll_to_x\0"
  /*  6384 */ "scroll_to_y\0"
  /*  6396 */ "self_height\0"
  /*  6408 */ "series_next\0"
  /*  6420 */ "shown_month\0"
  /*  6432 */ "space_right\0"
  /*  6444 */ "span_coords\0"
  /*  6456 */ "src_reverse\0"
  /*  6468 */ "start_value\0"
  /*  6480 */ "symmetrical\0"
  /*  6492 */ "tile_active\0"
  /*  6504 */ "today_month\0"
  /*  6516 */ "translate_x\0"
  /*  6528 */ "translate_y\0"
  /*  6540 */ "update_mode\0"
  /*  6552 */ "update_snap\0"
  /*  6564 */ "add_button\0"
  /*  6575 */ "add_cursor\0"
  /*  6586 */ "add_option\0"
  /*  6597 */ "add_series\0"
  /*  6608 */ "all_values\0"
  /*  6619 */ "angle_size\0"
  /*  6630 */ "axis_range\0"
  /*  6641 */ "bg_img_opa\0"
  /*  6652 */ "bg_img_src\0"
  /*  6663 */ "blend_mode\0"
  /*  6674 */ "border_opa\0"
  /*  6685 */ "brightness\0"
  /*  6696 */ "cell_value\0"
  /*  6707 */ "check_type\0"
  /*  6718 */ "clear_flag\0"
  /*  6729 */ "color_mode\0"
  /*  6740 */ "cover_area\0"
  /*  6751 */ "cursor_pos\0"
  /*  6762 */ "custom_get\0"
  /*  6773 */ "dark_color\0"
  /*  6784 */ "find_by_id\0"
  /*  6795 */ "flex_align\0"
  /*  6806 */ "focus_next\0"
  /*  6817 */ "focus_prev\0"
  /*  6828 */ "grid_align\0"
  /*  6839 */ "height_max\0"
  /*  6850 */ "height_min\0"
  /*  6861 */ "indic_area\0"
  /*  6872 */ "init_layer\0"
  /*  6883 */ "label_show\0"
  /*  6894 */ "left_value\0"
  /*  6905 */ "letter_pos\0"
  /*  6916 */ "margin_all\0"
  /*  6927 */ "margin_hor\0"
  /*  6938 */ "margin_top\0"
  /*  6949 */ "margin_ver\0"
  /*  6960 */ "max_height\0"
  /*  6971 */ "max_length\0"
  /*  6982 */ "min_height\0"
  /*  6993 */ "next_value\0"
  /*  7004 */ "option_cnt\0"
  /*  7015 */ "option_str\0"
  /*  7026 */ "pad_bottom\0"
  /*  7037 */ "pad_column\0"
  /*  7048 */ "pad_radial\0"
  /*  7059 */ "page_title\0"
  /*  7070 */ "prev_state\0"
  /*  7081 */ "quiet_zone\0"
  /*  7092 */ "read_timer\0"
  /*  7103 */ "remove_obj\0"
  /*  7114 */ "scroll_dir\0"
  /*  7125 */ "scroll_end\0"
  /*  7136 */ "scroll_obj\0"
  /*  7147 */ "scroll_top\0"
  /*  7158 */ "scrollable\0"
  /*  7169 */ "search_obj\0"
  /*  7180 */ "self_width\0"
  /*  7191 */ "send_event\0"
  /*  7202 */ "shadow_opa\0"
  /*  7213 */ "shown_year\0"
  /*  7224 */ "space_left\0"
  /*  7235 */ "span_count\0"
  /*  7246 */ "span_style\0"
  /*  7257 */ "src_height\0"
  /*  7268 */ "stripe_opa\0"
  /*  7279 */ "tab_active\0"
  /*  7290 */ "tab_button\0"
  /*  7301 */ "text_angle\0"
  /*  7312 */ "text_color\0"
  /*  7323 */ "text_decor\0"
  /*  7334 */ "today_date\0"
  /*  7345 */ "today_year\0"
  /*  7356 */ "transition\0"
  /*  7367 */ "_btn_text\0"
  /*  7377 */ "add_state\0"
  /*  7387 */ "add_style\0"
  /*  7397 */ "add_title\0"
  /*  7407 */ "anim_time\0"
  /*  7417 */ "antialias\0"
  /*  7427 */ "arc_color\0"
  /*  7437 */ "arc_sweep\0"
  /*  7447 */ "arc_width\0"
  /*  7457 */ "bg_angles\0"
  /*  7467 */ "btnmatrix\0"
  /*  7477 */ "checkable\0"
  /*  7487 */ "child_cnt\0"
  /*  7497 */ "clear_ccw\0"
  /*  7507 */ "clickable\0"
  /*  7517 */ "col_width\0"
  /*  7527 */ "cover_res\0"
  /*  7537 */ "cursor_up\0"
  /*  7547 */ "day_names\0"
  /*  7557 */ "decrement\0"
  /*  7567 */ "del_async\0"
  /*  7577 */ "dump_tree\0"
  /*  7587 */ "flex_flow\0"
  /*  7597 */ "flex_grow\0"
  /*  7607 */ "focus_obj\0"
  /*  7617 */ "gap_width\0"
  /*  7627 */ "grid_cell\0"
  /*  7637 */ "group_def\0"
  /*  7647 */ "has_class\0"
  /*  7657 */ "has_state\0"
  /*  7667 */ "increment\0"
  /*  7677 */ "layer_sys\0"
  /*  7687 */ "layer_top\0"
  /*  7697 */ "letter_on\0"
  /*  7707 */ "long_mode\0"
  /*  7717 */ "map_array\0"
  /*  7727 */ "max_lines\0"
  /*  7737 */ "max_width\0"
  /*  7747 */ "min_width\0"
  /*  7757 */ "obj_count\0"
  /*  7767 */ "pad_right\0"
  /*  7777 */ "pause_for\0"
  /*  7787 */ "post_draw\0"
  /*  7797 */ "refr_size\0"
  /*  7807 */ "scroll_by\0"
  /*  7817 */ "scroll_to\0"
  /*  7827 */ "scrolling\0"
  /*  7837 */ "send_data\0"
  /*  7847 */ "space_top\0"
  /*  7857 */ "span_text\0"
  /*  7867 */ "src_count\0"
  /*  7877 */ "src_right\0"
  /*  7887 */ "src_width\0"
  /*  7897 */ "step_next\0"
  /*  7907 */ "step_prev\0"
  /*  7917 */ "tab_count\0"
  /*  7927 */ "text_font\0"
  /*  7937 */ "today_day\0"
  /*  7947 */ "user_flag\0"
  /*  7957 */ "width_max\0"
  /*  7967 */ "width_min\0"
  /*  7977 */ "x_aligned\0"
  /*  7987 */ "y_aligned\0"
  /*  7997 */ "add_char\0"
  /*  8006 */ "add_flag\0"
  /*  8015 */ "add_span\0"
  /*  8024 */ "add_text\0"
  /*  8033 */ "add_tile\0"
  /*  8042 */ "align_to\0"
  /*  8051 */ "base_dir\0"
  /*  8060 */ "bg_color\0"
  /*  8069 */ "cut_text\0"
  /*  8078 */ "editable\0"
  /*  8087 */ "fade_out\0"
  /*  8096 */ "focus_cb\0"
  /*  8105 */ "has_flag\0"
  /*  8114 */ "hit_test\0"
  /*  8123 */ "ins_text\0"
  /*  8132 */ "old_size\0"
  /*  8141 */ "one_line\0"
  /*  8150 */ "overflow\0"
  /*  8159 */ "pad_left\0"
  /*  8168 */ "playtime\0"
  /*  8177 */ "popovers\0"
  /*  8186 */ "ready_cb\0"
  /*  8195 */ "refr_pos\0"
  /*  8204 */ "rollover\0"
  /*  8213 */ "scr_prev\0"
  /*  8222 */ "scroll_x\0"
  /*  8231 */ "scroll_y\0"
  /*  8240 */ "src_left\0"
  /*  8249 */ "start_cb\0"
  /*  8258 */ "swap_obj\0"
  /*  8267 */ "tab_btns\0"
  /*  8276 */ "tab_text\0"
  /*  8285 */ "text_opa\0"
  /*  8294 */ "text_src\0"
  /*  8303 */ "textarea\0"
  /*  8312 */ "tile_act\0"
  /*  8321 */ "tile_cnt\0"
  /*  8330 */ "y_invert\0"
  /*  8339 */ "add_btn\0"
  /*  8347 */ "add_obj\0"
  /*  8355 */ "add_tab\0"
  /*  8363 */ "arc_opa\0"
  /*  8371 */ "bg_grad\0"
  /*  8379 */ "buffers\0"
  /*  8387 */ "col_cnt\0"
  /*  8395 */ "default\0"
  /*  8403 */ "display\0"
  /*  8411 */ "dragged\0"
  /*  8419 */ "edge_cb\0"
  /*  8427 */ "editing\0"
  /*  8435 */ "fade_in\0"
  /*  8443 */ "fill_bg\0"
  /*  8451 */ "focused\0"
  /*  8459 */ "free_id\0"
  /*  8467 */ "move_to\0"
  /*  8475 */ "pad_all\0"
  /*  8483 */ "pad_gap\0"
  /*  8491 */ "pad_hor\0"
  /*  8499 */ "pad_row\0"
  /*  8507 */ "pad_top\0"
  /*  8515 */ "pad_ver\0"
  /*  8523 */ "palette\0"
  /*  8531 */ "path_cb\0"
  /*  8539 */ "refresh\0"
  /*  8547 */ "scr_act\0"
  /*  8555 */ "sibling\0"
  /*  8563 */ "src_mid\0"
  /*  8571 */ "tab_act\0"
  /*  8579 */ "tab_bar\0"
  /*  8587 */ "tile_id\0"
  /*  8595 */ "bg_opa\0"
  /*  8602 */ "buffer\0"
  /*  8609 */ "center\0"
  /*  8616 */ "enable\0"
  /*  8623 */ "footer\0"
  /*  8630 */ "hidden\0"
  /*  8637 */ "indent\0"
  /*  8644 */ "parent\0"
  /*  8651 */ "paused\0"
  /*  8658 */ "period\0"
  /*  8665 */ "radius\0"
  /*  8672 */ "remove\0"
  /*  8679 */ "render\0"
  /*  8686 */ "resume\0"
  /*  8693 */ "screen\0"
  /*  8700 */ "symbol\0"
  /*  8707 */ "toggle\0"
  /*  8714 */ "update\0"
  /*  8721 */ "child\0"
  /*  8727 */ "clean\0"
  /*  8733 */ "close\0"
  /*  8739 */ "const\0"
  /*  8745 */ "empty\0"
  /*  8751 */ "group\0"
  /*  8757 */ "image\0"
  /*  8763 */ "indev\0"
  /*  8769 */ "label\0"
  /*  8775 */ "merge\0"
  /*  8781 */ "pause\0"
  /*  8787 */ "pivot\0"
  /*  8793 */ "ready\0"
  /*  8799 */ "reset\0"
  /*  8805 */ "code\0"
  /*  8810 */ "copy\0"
  /*  8815 */ "disp\0"
  /*  8820 */ "init\0"
  /*  8825 */ "open\0"
  /*  8830 */ "step\0"
  /*  8835 */ "swap\0"
  /*  8840 */ "vect\0"
  /*  8845 */ "wrap\0"
  /*  8850 */ "buf\0"
  /*  8854 */ "del\0"
  /*  8858 */ "dpi\0"
  /*  8862 */ "dpx\0"
  /*  8866 */ "hsv\0"
  /*  8870 */ "map\0"
  /*  8874 */ "off\0"
  /*  8878 */ "rgb\0"
  /*  8882 */ "var\0"
  /*  8886 */ "x2\0"
  /*  8889 */ "y2\0"
;

/* (return_type, arg_type) signatures */
static const lv_ntv_sig_t lv_method_sigs[] = {
  /*   0 */ { "", "(lv.anim)" },
  /*   1 */ { "", "(lv.anim)." },
  /*   2 */ { "", "(lv.anim)b" },
  /*   3 */ { "", "(lv.anim)c" },
  /*   4 */ { "", "(lv.anim)i" },
  /*   5 */ { "", "(lv.anim)ii" },
  /*   6 */ { "", "(lv.anim)iiii" },
  /*   7 */ { "", "(lv.display)" },
  /*   8 */ { "", "(lv.display)(lv.area)" },
  /*   9 */ { "", "(lv.display)(lv.theme)" },
  /*  10 */ { "", "(lv.display)." },
  /*  11 */ { "", "(lv.display)..ii" },
  /*  12 */ { "", "(lv.display)..iii" },
  /*  13 */ { "", "(lv.display)b" },
  /*  14 */ { "", "(lv.display)c" },
  /*  15 */ { "", "(lv.display)i" },
  /*  16 */ { "", "(lv.display)ii" },
  /*  17 */ { "", "(lv.event)" },
  /*  18 */ { "", "(lv.event)i" },
  /*  19 */ { "", "(lv.group)" },
  /*  20 */ { "", "(lv.group)(lv.obj)" },
  /*  21 */ { "", "(lv.group)." },
  /*  22 */ { "", "(lv.group)^lv_group_focus_cb^" },
  /*  23 */ { "", "(lv.group)b" },
  /*  24 */ { "", "(lv.group)i" },
  /*  25 */ { "", "(lv.indev)" },
  /*  26 */ { "", "(lv.indev)(lv.display)" },
  /*  27 */ { "", "(lv.indev)(lv.group)" },
  /*  28 */ { "", "(lv.indev)(lv.obj)" },
  /*  29 */ { "", "(lv.indev)(lv.point_arr)" },
  /*  30 */ { "", "(lv.indev)." },
  /*  31 */ { "", "(lv.indev)b" },
  /*  32 */ { "", "(lv.indev)c" },
  /*  33 */ { "", "(lv.indev)i" },
  /*  34 */ { "", "(lv.obj)" },
  /*  35 */ { "", "(lv.obj)(lv.area)" },
  /*  36 */ { "", "(lv.obj)(lv.area)(lv.area)" },
  /*  37 */ { "", "(lv.obj)(lv.area)i" },
  /*  38 */ { "", "(lv.obj)(lv.chart_cursor)" },
  /*  39 */ { "", "(lv.obj)(lv.chart_cursor)(lv.chart_series)i" },
  /*  40 */ { "", "(lv.obj)(lv.chart_cursor)c" },
  /*  41 */ { "", "(lv.obj)(lv.chart_cursor)i" },
  /*  42 */ { "", "(lv.obj)(lv.chart_series)" },
  /*  43 */ { "", "(lv.obj)(lv.chart_series)(lv.color)" },
  /*  44 */ { "", "(lv.obj)(lv.chart_series)(lv.int_arr)" },
  /*  45 */ { "", "(lv.obj)(lv.chart_series)(lv.int_arr)(lv.int_arr)i" },
  /*  46 */ { "", "(lv.obj)(lv.chart_series)(lv.int_arr)i" },
  /*  47 */ { "", "(lv.obj)(lv.chart_series)b" },
  /*  48 */ { "", "(lv.obj)(lv.chart_series)i" },
  /*  49 */ { "", "(lv.obj)(lv.chart_series)ic" },
  /*  50 */ { "", "(lv.obj)(lv.chart_series)ii" },
  /*  51 */ { "", "(lv.obj)(lv.chart_series)iii" },
  /*  52 */ { "", "(lv.obj)(lv.color)" },
  /*  53 */ { "", "(lv.obj)(lv.color)i" },
  /*  54 */ { "", "(lv.obj)(lv.font)i" },
  /*  55 */ { "", "(lv.obj)(lv.image_dsc)" },
  /*  56 */ { "", "(lv.obj)(lv.int_arr)(lv.int_arr)" },
  /*  57 */ { "", "(lv.obj)(lv.layer)" },
  /*  58 */ { "", "(lv.obj)(lv.obj)" },
  /*  59 */ { "", "(lv.obj)(lv.obj)(lv.obj)" },
  /*  60 */ { "", "(lv.obj)(lv.obj)i" },
  /*  61 */ { "", "(lv.obj)(lv.obj)ii" },
  /*  62 */ { "", "(lv.obj)(lv.obj)iii" },
  /*  63 */ { "", "(lv.obj)(lv.obj)s" },
  /*  64 */ { "", "(lv.obj)(lv.point_arr)i" },
  /*  65 */ { "", "(lv.obj)(lv.point_arr)ii" },
  /*  66 */ { "", "(lv.obj)(lv.scale_section)(lv.style)" },
  /*  67 */ { "", "(lv.obj)(lv.scale_section)i" },
  /*  68 */ { "", "(lv.obj)(lv.scale_section)ii" },
  /*  69 */ { "", "(lv.obj)(lv.span)" },
  /*  70 */ { "", "(lv.obj)(lv.span)(lv.style)" },
  /*  71 */ { "", "(lv.obj)(lv.span)s" },
  /*  72 */ { "", "(lv.obj)(lv.span)s[......]" },
  /*  73 */ { "", "(lv.obj)(lv.str_arr)" },
  /*  74 */ { "", "(lv.obj)(lv.style)i" },
  /*  75 */ { "", "(lv.obj)(lv.style)ib" },
  /*  76 */ { "", "(lv.obj)." },
  /*  77 */ { "", "(lv.obj).iii" },
  /*  78 */ { "", "(lv.obj)b" },
  /*  79 */ { "", "(lv.obj)c" },
  /*  80 */ { "", "(lv.obj)ci" },
  /*  81 */ { "", "(lv.obj)i" },
  /*  82 */ { "", "(lv.obj)i(lv.anim_timeline_dsc)ib" },
  /*  83 */ { "", "(lv.obj)i(lv.draw_arc_dsc)" },
  /*  84 */ { "", "(lv.obj)i(lv.draw_image_dsc)" },
  /*  85 */ { "", "(lv.obj)i(lv.draw_label_dsc)" },
  /*  86 */ { "", "(lv.obj)i(lv.draw_line_dsc)" },
  /*  87 */ { "", "(lv.obj)i(lv.draw_rect_dsc)" },
  /*  88 */ { "", "(lv.obj)i(lv.obj)iii" },
  /*  89 */ { "", "(lv.obj)i." },
  /*  90 */ { "", "(lv.obj)i..." },
  /*  91 */ { "", "(lv.obj)ib" },
  /*  92 */ { "", "(lv.obj)ic" },
  /*  93 */ { "", "(lv.obj)ii" },
  /*  94 */ { "", "(lv.obj)ii(lv.color)i" },
  /*  95 */ { "", "(lv.obj)ii." },
  /*  96 */ { "", "(lv.obj)iib" },
  /*  97 */ { "", "(lv.obj)iii" },
  /*  98 */ { "", "(lv.obj)iiiiii" },
  /*  99 */ { "", "(lv.obj)iis" },
  /* 100 */ { "", "(lv.obj)iis[......]" },
  /* 101 */ { "", "(lv.obj)is" },
  /* 102 */ { "", "(lv.obj)s" },
  /* 103 */ { "", "(lv.obj)s[......]" },
  /* 104 */ { "", "(lv.obj)si" },
  /* 105 */ { "", "(lv.scale_section)i(lv.style)" },
  /* 106 */ { "", "(lv.scale_section)ii" },
  /* 107 */ { "", "(lv.span)s" },
  /* 108 */ { "", "(lv.span)s[......]" },
  /* 109 */ { "", "(lv.style)" },
  /* 110 */ { "", "(lv.style)(lv.font)" },
  /* 111 */ { "", "(lv.style)(lv.style)" },
  /* 112 */ { "", "(lv.style)i" },
  /* 113 */ { "", "(lv.style)ii" },
  /* 114 */ { "", "(lv.timer)" },
  /* 115 */ { "", "(lv.timer)." },
  /* 116 */ { "", "(lv.timer)^lv_timer_cb^" },
  /* 117 */ { "", "(lv.timer)b" },
  /* 118 */ { "", "(lv.timer)i" },
  /* 119 */ { "C", "(lv.group)" },
  /* 120 */ { "b", "(lv.anim)" },
  /* 121 */ { "b", "(lv.anim)c" },
  /* 122 */ { "b", "(lv.display)" },
  /* 123 */ { "b", "(lv.display)^lv_event_cb^." },
  /* 124 */ { "b", "(lv.display)i" },
  /* 125 */ { "b", "(lv.group)" },
  /* 126 */ { "b", "(lv.indev)" },
  /* 127 */ { "b", "(lv.indev)i" },
  /* 128 */ { "b", "(lv.obj)" },
  /* 129 */ { "b", "(lv.obj)(lv.area)" },
  /* 130 */ { "b", "(lv.obj)(lv.color)" },
  /* 131 */ { "b", "(lv.obj)(lv.event_dsc)" },
  /* 132 */ { "b", "(lv.obj)(lv.obj)" },
  /* 133 */ { "b", "(lv.obj)(lv.obj_class)" },
  /* 134 */ { "b", "(lv.obj)(lv.style)(lv.style)i" },
  /* 135 */ { "b", "(lv.obj)(lv.style)i" },
  /* 136 */ { "b", "(lv.obj)c" },
  /* 137 */ { "b", "(lv.obj)i" },
  /* 138 */ { "b", "(lv.obj)ii" },
  /* 139 */ { "b", "(lv.obj)iii" },
  /* 140 */ { "b", "(lv.obj)si" },
  /* 141 */ { "b", "(lv.style)" },
  /* 142 */ { "b", "(lv.style)i" },
  /* 143 */ { "b", "(lv.timer)" },
  /* 144 */ { "c", "(lv.anim)" },
  /* 145 */ { "c", "(lv.display)" },
  /* 146 */ { "c", "(lv.event)" },
  /* 147 */ { "c", "(lv.group)" },
  /* 148 */ { "c", "(lv.indev)" },
  /* 149 */ { "c", "(lv.obj)" },
  /* 150 */ { "c", "(lv.obj)ii" },
  /* 151 */ { "c", "(lv.timer)" },
  /* 152 */ { "i", "(lv.anim)" },
  /* 153 */ { "i", "(lv.display)" },
  /* 154 */ { "i", "(lv.display)." },
  /* 155 */ { "i", "(lv.display).." },
  /* 156 */ { "i", "(lv.display)i" },
  /* 157 */ { "i", "(lv.display)i." },
  /* 158 */ { "i", "(lv.display)ii" },
  /* 159 */ { "i", "(lv.event)" },
  /* 160 */ { "i", "(lv.group)" },
  /* 161 */ { "i", "(lv.group)i" },
  /* 162 */ { "i", "(lv.indev)" },
  /* 163 */ { "i", "(lv.indev).." },
  /* 164 */ { "i", "(lv.indev)i." },
  /* 165 */ { "i", "(lv.obj)" },
  /* 166 */ { "i", "(lv.obj)(lv.area)" },
  /* 167 */ { "i", "(lv.obj)(lv.chart_cursor)" },
  /* 168 */ { "i", "(lv.obj)(lv.chart_series)" },
  /* 169 */ { "i", "(lv.obj)(lv.obj_class)" },
  /* 170 */ { "i", "(lv.obj)." },
  /* 171 */ { "i", "(lv.obj).." },
  /* 172 */ { "i", "(lv.obj).i" },
  /* 173 */ { "i", "(lv.obj)cb" },
  /* 174 */ { "i", "(lv.obj)i" },
  /* 175 */ { "i", "(lv.obj)i." },
  /* 176 */ { "i", "(lv.obj)ici" },
  /* 177 */ { "i", "(lv.obj)ii" },
  /* 178 */ { "i", "(lv.obj)is" },
  /* 179 */ { "i", "(lv.obj)s" },
  /* 180 */ { "lv.anim", "(lv.anim)" },
  /* 181 */ { "lv.anim", "(lv.anim)c" },
  /* 182 */ { "lv.anim", "(lv.event)" },
  /* 183 */ { "lv.anim", "(lv.obj)" },
  /* 184 */ { "lv.area", "(lv.event)" },
  /* 185 */ { "lv.area", "(lv.obj)" },
  /* 186 */ { "lv.chart_cursor", "(lv.obj)(lv.color)i" },
  /* 187 */ { "lv.chart_series", "(lv.obj)(lv.chart_series)" },
  /* 188 */ { "lv.chart_series", "(lv.obj)(lv.color)i" },
  /* 189 */ { "lv.color", "(lv.obj)" },
  /* 190 */ { "lv.color", "(lv.obj)(lv.chart_series)" },
  /* 191 */ { "lv.color", "(lv.obj)i" },
  /* 192 */ { "lv.display", "(lv.display)" },
  /* 193 */ { "lv.display", "(lv.indev)" },
  /* 194 */ { "lv.display", "(lv.obj)" },
  /* 195 */ { "lv.event_dsc", "(lv.display)^lv_event_cb^i." },
  /* 196 */ { "lv.event_dsc", "(lv.display)i" },
  /* 197 */ { "lv.event_dsc", "(lv.indev)^lv_event_cb^i." },
  /* 198 */ { "lv.event_dsc", "(lv.indev)i" },
  /* 199 */ { "lv.event_dsc", "(lv.obj)^lv_event_cb^i." },
  /* 200 */ { "lv.event_dsc", "(lv.obj)i" },
  /* 201 */ { "lv.group", "(lv.indev)" },
  /* 202 */ { "lv.group", "(lv.obj)" },
  /* 203 */ { "lv.group_focus_cb", "(lv.group)" },
  /* 204 */ { "lv.image_dsc", "(lv.obj)" },
  /* 205 */ { "lv.indev", "(lv.event)" },
  /* 206 */ { "lv.indev", "(lv.indev)" },
  /* 207 */ { "lv.int_arr", "(lv.obj)(lv.chart_series)" },
  /* 208 */ { "lv.layer", "(lv.event)" },
  /* 209 */ { "lv.obj", "(lv.display)" },
  /* 210 */ { "lv.obj", "(lv.event)" },
  /* 211 */ { "lv.obj", "(lv.group)" },
  /* 212 */ { "lv.obj", "(lv.group)i" },
  /* 213 */ { "lv.obj", "(lv.indev)" },
  /* 214 */ { "lv.obj", "(lv.obj)" },
  /* 215 */ { "lv.obj", "(lv.obj)." },
  /* 216 */ { "lv.obj", "(lv.obj).s" },
  /* 217 */ { "lv.obj", "(lv.obj)c" },
  /* 218 */ { "lv.obj", "(lv.obj)i" },
  /* 219 */ { "lv.obj", "(lv.obj)i(lv.obj_class)" },
  /* 220 */ { "lv.obj", "(lv.obj)iii" },
  /* 221 */ { "lv.obj", "(lv.obj)s" },
  /* 222 */ { "lv.obj", "(lv.obj)s[......]" },
  /* 223 */ { "lv.obj_class", "(lv.obj)" },
  /* 224 */ { "lv.point_precise", "(lv.obj)" },
  /* 225 */ { "lv.scale_section", "(lv.obj)" },
  /* 226 */ { "lv.span", "(lv.obj)" },
  /* 227 */ { "lv.span", "(lv.obj)c" },
  /* 228 */ { "lv.span", "(lv.obj)i" },
  /* 229 */ { "lv.span_coords", "(lv.obj)(lv.span)" },
  /* 230 */ { "lv.style", "(lv.span)" },
  /* 231 */ { "lv.theme", "(lv.display)" },
  /* 232 */ { "lv.timer", "(lv.display)" },
  /* 233 */ { "lv.timer", "(lv.indev)" },
  /* 234 */ { "lv.timer", "(lv.timer)" },
  /* 235 */ { "s", "(lv.obj)" },
  /* 236 */ { "s", "(lv.obj)(lv.obj)" },
  /* 237 */ { "s", "(lv.obj)ci" },
  /* 238 */ { "s", "(lv.obj)i" },
  /* 239 */ { "s", "(lv.obj)ii" },
  /* 240 */ { "s", "(lv.span)" },
};

/* generic style accessors: value types, see `lv_style_ntv_type_t` */
_Static_assert(LV_STYLE_LAST_BUILT_IN_PROP <= 0xFF, "style prop IDs must fit in uint8_t");
static const lv_style_ntv_type_t lv_style_types[] = {
  /*  0 */ { "", "lv.font", LV_SKIND_PTR },
  /*  1 */ { "(lv.anim)", "lv.anim", LV_SKIND_PTR },
  /*  2 */ { "(lv.color)", "", LV_SKIND_COLOR },
  /*  3 */ { "(lv.color)", "lv.color", LV_SKIND_COLOR },
  /*  4 */ { "(lv.color_filter_dsc)", "lv.color_filter_dsc", LV_SKIND_PTR },
  /*  5 */ { "(lv.grad_dsc)", "lv.grad_dsc", LV_SKIND_PTR },
  /*  6 */ { "(lv.int_arr)", "lv.int_arr", LV_SKIND_PTR },
  /*  7 */ { "(lv.style_transition_dsc)", "lv.style_transition_dsc", LV_SKIND_PTR },
  /*  8 */ { ".", "", LV_SKIND_PTR },
  /*  9 */ { ".", "c", LV_SKIND_PTR },
  /* 10 */ { "b", "", LV_SKIND_BOOL },
  /* 11 */ { "b", "b", LV_SKIND_BOOL },
  /* 12 */ { "c", "c", LV_SKIND_PTR },
  /* 13 */ { "i", "", LV_SKIND_U8 },
  /* 14 */ { "i", "i", LV_SKIND_NUM },
  /* 15 */ { "i", "i", LV_SKIND_U16 },
  /* 16 */ { "i", "i", LV_SKIND_U8 },
};

/* generic style accessors, sorted by name without verb */
static const lv_style_ntv_prop_t lv_style_props[] = {
  { 237, LV_STYLE_ALIGN, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* align */
  { 2971, LV_STYLE_ANIM, LV_SPROP_INFO(1, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* anim */
  { 4320, LV_STYLE_ANIM_DURATION, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* anim_duration */
  { 7407, LV_STYLE_ANIM_DURATION, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* anim_time */
  { 7427, LV_STYLE_ARC_COLOR, LV_SPROP_INFO(3, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* arc_color */
  { 4334, LV_STYLE_ARC_IMAGE_SRC, LV_SPROP_INFO(9, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* arc_image_src */
  { 8363, LV_STYLE_ARC_OPA, LV_SPROP_INFO(16, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* arc_opa */
  { 5820, LV_STYLE_ARC_ROUNDED, LV_SPROP_INFO(11, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* arc_rounded */
  { 7447, LV_STYLE_ARC_WIDTH, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* arc_width */
  { 8051, LV_STYLE_BASE_DIR, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* base_dir */
  { 8060, LV_STYLE_BG_COLOR, LV_SPROP_INFO(3, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* bg_color */
  { 8371, LV_STYLE_BG_GRAD, LV_SPROP_INFO(5, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* bg_grad */
  { 4362, LV_STYLE_BG_GRAD_COLOR, LV_SPROP_INFO(3, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* bg_grad_color */
  { 5844, LV_STYLE_BG_GRAD_DIR, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* bg_grad_dir */
  { 5856, LV_STYLE_BG_GRAD_OPA, LV_SPROP_INFO(16, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* bg_grad_opa */
  { 5043, LV_STYLE_BG_GRAD_STOP, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* bg_grad_stop */
  { 5056, LV_STYLE_BG_IMAGE_OPA, LV_SPROP_INFO(16, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* bg_image_opa */
  { 2602, LV_STYLE_BG_IMAGE_RECOLOR, LV_SPROP_INFO(3, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* bg_image_recolor */
  { 1045, LV_STYLE_BG_IMAGE_RECOLOR_OPA, LV_SPROP_INFO(16, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* bg_image_recolor_opa */
  { 5069, LV_STYLE_BG_IMAGE_SRC, LV_SPROP_INFO(9, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* bg_image_src */
  { 3765, LV_STYLE_BG_IMAGE_TILED, LV_SPROP_INFO(11, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* bg_image_tiled */
  { 6641, LV_STYLE_BG_IMAGE_OPA, LV_SPROP_INFO(13, LV_SPROP_STYLE_SET) },    /* bg_img_opa */
  { 3780, LV_STYLE_BG_IMAGE_RECOLOR, LV_SPROP_INFO(2, LV_SPROP_STYLE_SET) },    /* bg_img_recolor */
  { 1591, LV_STYLE_BG_IMAGE_RECOLOR_OPA, LV_SPROP_INFO(13, LV_SPROP_STYLE_SET) },    /* bg_img_recolor_opa */
  { 6652, LV_STYLE_BG_IMAGE_SRC, LV_SPROP_INFO(8, LV_SPROP_STYLE_SET) },    /* bg_img_src */
  { 5082, LV_STYLE_BG_IMAGE_TILED, LV_SPROP_INFO(10, LV_SPROP_STYLE_SET) },    /* bg_img_tiled */
  { 5868, LV_STYLE_BG_MAIN_OPA, LV_SPROP_INFO(16, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* bg_main_opa */
  { 5095, LV_STYLE_BG_MAIN_STOP, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* bg_main_stop */
  { 8595, LV_STYLE_BG_OPA, LV_SPROP_INFO(16, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* bg_opa */
  { 3145, LV_STYLE_BITMAP_MASK_SRC, LV_SPROP_INFO(9, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* bitmap_mask_src */
  { 6663, LV_STYLE_BLEND_MODE, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* blend_mode */
  { 5108, LV_STYLE_BORDER_COLOR, LV_SPROP_INFO(3, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* border_color */
  { 6674, LV_STYLE_BORDER_OPA, LV_SPROP_INFO(16, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* border_opa */
  { 5880, LV_STYLE_BORDER_POST, LV_SPROP_INFO(11, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* border_post */
  { 5892, LV_STYLE_BORDER_SIDE, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* border_side */
  { 5121, LV_STYLE_BORDER_WIDTH, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* border_width */
  { 5952, LV_STYLE_CLIP_CORNER, LV_SPROP_INFO(11, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* clip_corner */
  { 2619, LV_STYLE_COLOR_FILTER_DSC, LV_SPROP_INFO(4, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* color_filter_dsc */
  { 2636, LV_STYLE_COLOR_FILTER_OPA, LV_SPROP_INFO(16, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* color_filter_opa */
  { 2704, LV_STYLE_FLEX_CROSS_PLACE, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* flex_cross_place */
  { 7587, LV_STYLE_FLEX_FLOW, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* flex_flow */
  { 7597, LV_STYLE_FLEX_GROW, LV_SPROP_INFO(16, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* flex_grow */
  { 3241, LV_STYLE_FLEX_MAIN_PLACE, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* flex_main_place */
  { 2721, LV_STYLE_FLEX_TRACK_PLACE, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* flex_track_place */
  { 1087, LV_STYLE_GRID_CELL_COLUMN_POS, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* grid_cell_column_pos */
  { 869, LV_STYLE_GRID_CELL_COLUMN_SPAN, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* grid_cell_column_span */
  { 2190, LV_STYLE_GRID_CELL_ROW_POS, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* grid_cell_row_pos */
  { 1648, LV_STYLE_GRID_CELL_ROW_SPAN, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* grid_cell_row_span */
  { 2208, LV_STYLE_GRID_CELL_X_ALIGN, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* grid_cell_x_align */
  { 2226, LV_STYLE_GRID_CELL_Y_ALIGN, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* grid_cell_y_align */
  { 2244, LV_STYLE_GRID_COLUMN_ALIGN, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* grid_column_align */
  { 891, LV_STYLE_GRID_COLUMN_DSC_ARRAY, LV_SPROP_INFO(6, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* grid_column_dsc_array */
  { 4005, LV_STYLE_GRID_ROW_ALIGN, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* grid_row_align */
  { 1667, LV_STYLE_GRID_ROW_DSC_ARRAY, LV_SPROP_INFO(6, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* grid_row_dsc_array */
  { 1286, LV_STYLE_HEIGHT, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* height */
  { 4035, LV_STYLE_IMAGE_COLORKEY, LV_SPROP_INFO(12, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* image_colorkey */
  { 5059, LV_STYLE_IMAGE_OPA, LV_SPROP_INFO(16, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* image_opa */
  { 2605, LV_STYLE_IMAGE_RECOLOR, LV_SPROP_INFO(3, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* image_recolor */
  { 1048, LV_STYLE_IMAGE_RECOLOR_OPA, LV_SPROP_INFO(16, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* image_recolor_opa */
  { 6644, LV_STYLE_IMAGE_OPA, LV_SPROP_INFO(16, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* img_opa */
  { 3783, LV_STYLE_IMAGE_RECOLOR, LV_SPROP_INFO(3, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* img_recolor */
  { 1594, LV_STYLE_IMAGE_RECOLOR_OPA, LV_SPROP_INFO(16, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* img_recolor_opa */
  { 4957, LV_STYLE_LAYOUT, LV_SPROP_INFO(15, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* layout */
  { 6975, LV_STYLE_LENGTH, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* length */
  { 4631, LV_STYLE_LINE_COLOR, LV_SPROP_INFO(3, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* line_color */
  { 4572, LV_STYLE_LINE_DASH_GAP, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* line_dash_gap */
  { 3289, LV_STYLE_LINE_DASH_WIDTH, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* line_dash_width */
  { 6243, LV_STYLE_LINE_OPA, LV_SPROP_INFO(16, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* line_opa */
  { 5381, LV_STYLE_LINE_ROUNDED, LV_SPROP_INFO(11, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* line_rounded */
  { 4645, LV_STYLE_LINE_WIDTH, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* line_width */
  { 4586, LV_STYLE_MARGIN_BOTTOM, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* margin_bottom */
  { 6156, LV_STYLE_MARGIN_LEFT, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* margin_left */
  { 5394, LV_STYLE_MARGIN_RIGHT, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* margin_right */
  { 6938, LV_STYLE_MARGIN_TOP, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* margin_top */
  { 6960, LV_STYLE_MAX_HEIGHT, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* max_height */
  { 7737, LV_STYLE_MAX_WIDTH, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* max_width */
  { 6982, LV_STYLE_MIN_HEIGHT, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* min_height */
  { 7747, LV_STYLE_MIN_WIDTH, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* min_width */
  { 639, LV_STYLE_OPA, LV_SPROP_INFO(16, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* opa */
  { 6216, LV_STYLE_OPA_LAYERED, LV_SPROP_INFO(16, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* opa_layered */
  { 4628, LV_STYLE_OUTLINE_COLOR, LV_SPROP_INFO(3, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* outline_color */
  { 6240, LV_STYLE_OUTLINE_OPA, LV_SPROP_INFO(16, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* outline_opa */
  { 6252, LV_STYLE_OUTLINE_PAD, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* outline_pad */
  { 4642, LV_STYLE_OUTLINE_WIDTH, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* outline_width */
  { 7026, LV_STYLE_PAD_BOTTOM, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* pad_bottom */
  { 7037, LV_STYLE_PAD_COLUMN, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* pad_column */
  { 8159, LV_STYLE_PAD_LEFT, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* pad_left */
  { 7048, LV_STYLE_PAD_RADIAL, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* pad_radial */
  { 7767, LV_STYLE_PAD_RIGHT, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* pad_right */
  { 8499, LV_STYLE_PAD_ROW, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* pad_row */
  { 8507, LV_STYLE_PAD_TOP, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* pad_top */
  { 4684, LV_STYLE_RADIAL_OFFSET, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* radial_offset */
  { 8665, LV_STYLE_RADIUS, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* radius */
  { 1505, LV_STYLE_RECOLOR, LV_SPROP_INFO(3, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* recolor */
  { 1054, LV_STYLE_RECOLOR_OPA, LV_SPROP_INFO(16, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* recolor_opa */
  { 1781, LV_STYLE_ROTARY_SENSITIVITY, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* rotary_sensitivity */
  { 5641, LV_STYLE_SHADOW_COLOR, LV_SPROP_INFO(3, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* shadow_color */
  { 3545, LV_STYLE_SHADOW_OFFSET_X, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* shadow_offset_x */
  { 3561, LV_STYLE_SHADOW_OFFSET_Y, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* shadow_offset_y */
  { 5654, LV_STYLE_SHADOW_OFFSET_X, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* shadow_ofs_x */
  { 5667, LV_STYLE_SHADOW_OFFSET_Y, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* shadow_ofs_y */
  { 7202, LV_STYLE_SHADOW_OPA, LV_SPROP_INFO(16, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* shadow_opa */
  { 4866, LV_STYLE_SHADOW_SPREAD, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* shadow_spread */
  { 5680, LV_STYLE_SHADOW_WIDTH, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* shadow_width */
  { 232, LV_STYLE_TEXT_ALIGN, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* text_align */
  { 7312, LV_STYLE_TEXT_COLOR, LV_SPROP_INFO(3, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* text_color */
  { 7323, LV_STYLE_TEXT_DECOR, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* text_decor */
  { 7927, LV_STYLE_TEXT_FONT, LV_SPROP_INFO(0, LV_SPROP_OBJ_GET) },    /* text_font */
  { 2424, LV_STYLE_TEXT_LEADING_TRIM, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* text_leading_trim */
  { 2442, LV_STYLE_TEXT_LETTER_SPACE, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* text_letter_space */
  { 3609, LV_STYLE_TEXT_LINE_SPACE, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* text_line_space */
  { 8285, LV_STYLE_TEXT_OPA, LV_SPROP_INFO(16, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* text_opa */
  { 348, LV_STYLE_TEXT_OUTLINE_STROKE_COLOR, LV_SPROP_INFO(3, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* text_outline_stroke_color */
  { 619, LV_STYLE_TEXT_OUTLINE_STROKE_OPA, LV_SPROP_INFO(16, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* text_outline_stroke_opa */
  { 374, LV_STYLE_TEXT_OUTLINE_STROKE_WIDTH, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* text_outline_stroke_width */
  { 3625, LV_STYLE_TRANSFORM_ROTATION, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* transform_angle */
  { 3027, LV_STYLE_TRANSFORM_HEIGHT, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* transform_height */
  { 2460, LV_STYLE_TRANSFORM_PIVOT_X, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* transform_pivot_x */
  { 2478, LV_STYLE_TRANSFORM_PIVOT_Y, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* transform_pivot_y */
  { 1971, LV_STYLE_TRANSFORM_ROTATION, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* transform_rotation */
  { 2496, LV_STYLE_TRANSFORM_SCALE_X, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* transform_scale_x */
  { 2514, LV_STYLE_TRANSFORM_SCALE_Y, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* transform_scale_y */
  { 3044, LV_STYLE_TRANSFORM_SKEW_X, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* transform_skew_x */
  { 3061, LV_STYLE_TRANSFORM_SKEW_Y, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* transform_skew_y */
  { 3673, LV_STYLE_TRANSFORM_WIDTH, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* transform_width */
  { 7356, LV_STYLE_TRANSITION, LV_SPROP_INFO(7, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* transition */
  { 3095, LV_STYLE_TRANSLATE_RADIAL, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* translate_radial */
  { 6516, LV_STYLE_TRANSLATE_X, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* translate_x */
  { 6528, LV_STYLE_TRANSLATE_Y, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* translate_y */
  { 394, LV_STYLE_WIDTH, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* width */
  { 2476, LV_STYLE_X, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* x */
  { 911, LV_STYLE_Y, LV_SPROP_INFO(14, LV_SPROP_OBJ_SET | LV_SPROP_OBJ_GET | LV_SPROP_STYLE_SET) },    /* y */
};
#define LV_STYLE_PROPS_NB  (sizeof(lv_style_props) / sizeof(lv_style_props[0]))

/* `lv_anim` methods */
const lv_ntv_method_t lv_anim_func[] = {
  { (const void*) &lv_anim_custom_delete, 4474, LV_MSIG(0, 121) },    /* custom_delete */
  { (const void*) &lv_anim_custom_get, 6762, LV_MSIG(0, 181) },    /* custom_get */
  { (const void*) &lv_anim_init, 8820, LV_MSIG(0, 0) },    /* init */
  { (const void*) &lv_anim_pause, 8781, LV_MSIG(0, 0) },    /* pause */
  { (const void*) &lv_anim_pause_for, 7777, LV_MSIG(0, 4) },    /* pause_for */
  { (const void*) &lv_anim_resume, 8686, LV_MSIG(0, 0) },    /* resume */
  { (const void*) &lv_anim_start, 1207, LV_MSIG(0, 180) },    /* start */
  { (const void*) &lv_anim_get_delay, 4762, LV_MSIG(1, 152) },    /* get_delay */
  { (const void*) &lv_anim_get_playtime, 8168, LV_MSIG(1, 152) },    /* get_playtime */
  { (const void*) &lv_anim_get_repeat_count, 5524, LV_MSIG(1, 152) },    /* get_repeat_count */
  { (const void*) &lv_anim_get_time, 684, LV_MSIG(1, 152) },    /* get_time */
  { (const void*) &lv_anim_get_user_data, 118, LV_MSIG(1, 144) },    /* get_user_data */
  { (const void*) &lv_anim_is_paused, 8651, LV_MSIG(3, 120) },    /* is_paused */
  { (const void*) &lv_anim_set_bezier3_param, 4348, LV_MSIG(4, 6) },    /* set_bezier3_param */
  { (const void*) &lv_anim_set_completed_cb, 5199, LV_MSIG(4, 3) },    /* set_completed_cb */
  { (const void*) &lv_anim_set_custom_exec_cb, 3945, LV_MSIG(4, 3) },    /* set_custom_exec_cb */
  { (const void*) &lv_anim_set_delay, 4762, LV_MSIG(4, 4) },    /* set_delay */
  { (const void*) &lv_anim_set_duration, 2899, LV_MSIG(4, 4) },    /* set_duration */
  { (const void*) &lv_anim_set_early_apply, 6048, LV_MSIG(4, 2) },    /* set_early_apply */
  { (const void*) &lv_anim_set_exec_cb, 3952, LV_MSIG(4, 3) },    /* set_exec_cb */
  { (const void*) &lv_anim_set_get_value_cb, 5342, LV_MSIG(4, 3) },    /* set_get_value_cb */
  { (const void*) &lv_anim_set_path_cb, 8531, LV_MSIG(4, 3) },    /* set_path_cb */
  { (const void*) &lv_anim_set_completed_cb, 8186, LV_MSIG(4, 3) },    /* set_ready_cb */
  { (const void*) &lv_anim_set_repeat_count, 5524, LV_MSIG(4, 4) },    /* set_repeat_count */
  { (const void*) &lv_anim_set_repeat_delay, 5537, LV_MSIG(4, 4) },    /* set_repeat_delay */
  { (const void*) &lv_anim_set_reverse_delay, 4754, LV_MSIG(4, 4) },    /* set_reverse_delay */
  { (const void*) &lv_anim_set_reverse_duration, 2891, LV_MSIG(4, 4) },    /* set_reverse_duration */
  { (const void*) &lv_anim_set_reverse_time, 5550, LV_MSIG(4, 4) },    /* set_reverse_time */
  { (const void*) &lv_anim_set_start_cb, 8249, LV_MSIG(4, 3) },    /* set_start_cb */
  { (const void*) &lv_anim_set_user_data, 118, LV_MSIG(4, 1) },    /* set_user_data */
  { (const void*) &lv_anim_set_values, 4859, LV_MSIG(4, 5) },    /* set_values */
  { (const void*) &lv_anim_set_var, 8882, LV_MSIG(4, 1) },    /* set_var */
};

/* `lv_animimg` methods */
#ifdef BE_LV_WIDGET_ANIMIMG
const lv_ntv_method_t lv_animimg_func[] = {
  { (const void*) &lv_animimg_delete, 8854, LV_MSIG(0, 128) },    /* del */
  { (const void*) &lv_animimg_delete, 4481, LV_MSIG(0, 128) },    /* delete */
  { (const void*) &lv_animimg_delete, 8672, LV_MSIG(0, 128) },    /* remove */
  { (const void*) &lv_animimg_start, 1207, LV_MSIG(0, 34) },    /* start */
  { (const void*) &lv_animimg_get_anim, 2971, LV_MSIG(1, 183) },    /* get_anim */
  { (const void*) &lv_animimg_get_duration, 2899, LV_MSIG(1, 165) },    /* get_duration */
  { (const void*) &lv_animimg_get_repeat_count, 5524, LV_MSIG(1, 165) },    /* get_repeat_count */
  { (const void*) &lv_animimg_get_src_count, 7867, LV_MSIG(1, 165) },    /* get_src_count */
  { (const void*) &lv_animimg_set_completed_cb, 5199, LV_MSIG(4, 79) },    /* set_completed_cb */
  { (const void*) &lv_animimg_set_duration, 2899, LV_MSIG(4, 81) },    /* set_duration */
  { (const void*) &lv_animimg_set_completed_cb, 8186, LV_MSIG(4, 79) },    /* set_ready_cb */
  { (const void*) &lv_animimg_set_repeat_count, 5524, LV_MSIG(4, 81) },    /* set_repeat_count */
  { (const void*) &lv_animimg_set_reverse_delay, 4754, LV_MSIG(4, 81) },    /* set_reverse_delay */
  { (const void*) &lv_animimg_set_reverse_duration, 2891, LV_MSIG(4, 81) },    /* set_reverse_duration */
  { (const void*) &lv_animimg_set_src, 3157, LV_MSIG(4, 80) },    /* set_src */
  { (const void*) &lv_animimg_set_src_reverse, 6456, LV_MSIG(4, 80) },    /* set_src_reverse */
  { (const void*) &lv_animimg_set_start_cb, 8249, LV_MSIG(4, 79) },    /* set_start_cb */
};
#endif // BE_LV_WIDGET_ANIMIMG

/* `lv_arc` methods */
#ifdef BE_LV_WIDGET_ARC
const lv_ntv_method_t lv_arc_func[] = {
  { (const void*) &lv_arc_align_obj_to_angle, 1553, LV_MSIG(0, 60) },    /* align_obj_to_angle */
  { (const void*) &lv_arc_rotate_obj_to_angle, 1433, LV_MSIG(0, 60) },    /* rotate_obj_to_angle */
  { (const void*) &lv_arc_get_rotation, 1447, LV_MSIG(1, 165) },    /* get_angle */
  { (const void*) &lv_arc_get_angle_end, 5020, LV_MSIG(1, 165) },    /* get_angle_end */
  { (const void*) &lv_arc_get_angle_start, 3753, LV_MSIG(1, 165) },    /* get_angle_start */
  { (const void*) &lv_arc_get_bg_angle_end, 5017, LV_MSIG(1, 165) },    /* get_bg_angle_end */
  { (const void*) &lv_arc_get_bg_angle_start, 3750, LV_MSIG(1, 165) },    /* get_bg_angle_start */
  { (const void*) &lv_arc_get_change_rate, 5916, LV_MSIG(1, 165) },    /* get_change_rate */
  { (const void*) &lv_arc_get_knob_offset, 6120, LV_MSIG(1, 165) },    /* get_knob_offset */
  { (const void*) &lv_arc_get_max_value, 2396, LV_MSIG(1, 165) },    /* get_max_value */
  { (const void*) &lv_arc_get_min_value, 2414, LV_MSIG(1, 165) },    /* get_min_value */
  { (const void*) &lv_arc_get_mode, 4180, LV_MSIG(1, 165) },    /* get_mode */
  { (const void*) &lv_arc_get_rotation, 1981, LV_MSIG(1, 165) },    /* get_rotation */
  { (const void*) &lv_arc_get_value, 1718, LV_MSIG(1, 165) },    /* get_value */
  { (const void*) &lv_arc_set_rotation, 1447, LV_MSIG(4, 81) },    /* set_angle */
  { (const void*) &lv_arc_set_angles, 7460, LV_MSIG(4, 93) },    /* set_angles */
  { (const void*) &lv_arc_set_bg_angles, 7457, LV_MSIG(4, 93) },    /* set_bg_angles */
  { (const void*) &lv_arc_set_bg_end_angle, 5030, LV_MSIG(4, 81) },    /* set_bg_end_angle */
  { (const void*) &lv_arc_set_bg_start_angle, 3795, LV_MSIG(4, 81) },    /* set_bg_start_angle */
  { (const void*) &lv_arc_set_change_rate, 5916, LV_MSIG(4, 81) },    /* set_change_rate */
  { (const void*) &lv_arc_set_end_angle, 5033, LV_MSIG(4, 81) },    /* set_end_angle */
  { (const void*) &lv_arc_set_knob_offset, 6120, LV_MSIG(4, 81) },    /* set_knob_offset */
  { (const void*) &lv_arc_set_max_value, 2396, LV_MSIG(4, 81) },    /* set_max_value */
  { (const void*) &lv_arc_set_min_value, 2414, LV_MSIG(4, 81) },    /* set_min_value */
  { (const void*) &lv_arc_set_mode, 4180, LV_MSIG(4, 81) },    /* set_mode */
  { (const void*) &lv_arc_set_range, 4832, LV_MSIG(4, 93) },    /* set_range */
  { (const void*) &lv_arc_set_rotation, 1981, LV_MSIG(4, 81) },    /* set_rotation */
  { (const void*) &lv_arc_set_start_angle, 3798, LV_MSIG(4, 81) },    /* set_start_angle */
  { (const void*) &lv_arc_set_value, 1718, LV_MSIG(4, 81) },    /* set_value */
};
#endif // BE_LV_WIDGET_ARC

/* `lv_arclabel` methods */
#ifdef BE_LV_WIDGET_ARCLABEL
const lv_ntv_method_t lv_arclabel_func[] = {
  { (const void*) &lv_arclabel_get_angle_size, 6619, LV_MSIG(1, 165) },    /* get_angle_size */
  { (const void*) &lv_arclabel_get_angle_start, 3753, LV_MSIG(1, 165) },    /* get_angle_start */
  { (const void*) &lv_arclabel_get_center_offset_x, 3161, LV_MSIG(1, 165) },    /* get_center_offset_x */
  { (const void*) &lv_arclabel_get_center_offset_y, 3177, LV_MSIG(1, 165) },    /* get_center_offset_y */
  { (const void*) &lv_arclabel_get_dir, 5852, LV_MSIG(1, 165) },    /* get_dir */
  { (const void*) &lv_arclabel_get_end_overlap, 6060, LV_MSIG(1, 128) },    /* get_end_overlap */
  { (const void*) &lv_arclabel_get_overflow, 8150, LV_MSIG(1, 165) },    /* get_overflow */
  { (const void*) &lv_arclabel_get_radius, 8665, LV_MSIG(1, 165) },    /* get_radius */
  { (const void*) &lv_arclabel_get_recolor, 1505, LV_MSIG(1, 128) },    /* get_recolor */
  { (const void*) &lv_arclabel_get_text_angle, 7301, LV_MSIG(1, 165) },    /* get_text_angle */
  { (const void*) &lv_arclabel_get_text_horizontal_align, 1001, LV_MSIG(1, 165) },    /* get_text_horizontal_align */
  { (const void*) &lv_arclabel_get_text_vertical_align, 1533, LV_MSIG(1, 165) },    /* get_text_vertical_align */
  { (const void*) &lv_arclabel_set_angle_size, 6619, LV_MSIG(4, 81) },    /* set_angle_size */
  { (const void*) &lv_arclabel_set_angle_start, 3753, LV_MSIG(4, 81) },    /* set_angle_start */
  { (const void*) &lv_arclabel_set_center_offset_x, 3161, LV_MSIG(4, 81) },    /* set_center_offset_x */
  { (const void*) &lv_arclabel_set_center_offset_y, 3177, LV_MSIG(4, 81) },    /* set_center_offset_y */
  { (const void*) &lv_arclabel_set_dir, 5852, LV_MSIG(4, 81) },    /* set_dir */
  { (const void*) &lv_arclabel_set_end_overlap, 6060, LV_MSIG(4, 78) },    /* set_end_overlap */
  { (const void*) &lv_arclabel_set_offset, 315, LV_MSIG(4, 81) },    /* set_offset */
  { (const void*) &lv_arclabel_set_overflow, 8150, LV_MSIG(4, 81) },    /* set_overflow */
  { (const void*) &lv_arclabel_set_radius, 8665, LV_MSIG(4, 81) },    /* set_radius */
  { (const void*) &lv_arclabel_set_recolor, 1505, LV_MSIG(4, 78) },    /* set_recolor */
  { (const void*) &lv_arclabel_set_text, 2835, LV_MSIG(4, 102) },    /* set_text */
  { (const void*) &lv_arclabel_set_text_fmt, 4899, LV_MSIG(4, 103) },    /* set_text_fmt */
  { (const void*) &lv_arclabel_set_text_horizontal_align, 1001, LV_MSIG(4, 81) },    /* set_text_horizontal_align */
  { (const void*) &lv_arclabel_set_text_static, 2947, LV_MSIG(4, 102) },    /* set_text_static */
  { (const void*) &lv_arclabel_set_text_vertical_align, 1533, LV_MSIG(4, 81) },    /* set_text_vertical_align */
};
#endif // BE_LV_WIDGET_ARCLABEL

/* `lv_bar` methods */
#ifdef BE_LV_WIDGET_BAR
const lv_ntv_method_t lv_bar_func[] = {
  { (const void*) &lv_bar_get_indic_area, 6861, LV_MSIG(1, 185) },    /* get_indic_area */
  { (const void*) &lv_bar_get_max_value, 2396, LV_MSIG(1, 165) },    /* get_max_value */
  { (const void*) &lv_bar_get_min_value, 2414, LV_MSIG(1, 165) },    /* get_min_value */
  { (const void*) &lv_bar_get_mode, 4180, LV_MSIG(1, 165) },    /* get_mode */
  { (const void*) &lv_bar_get_orientation, 6228, LV_MSIG(1, 165) },    /* get_orientation */
  { (const void*) &lv_bar_get_start_value, 6468, LV_MSIG(1, 165) },    /* get_start_value */
  { (const void*) &lv_bar_get_value, 1718, LV_MSIG(1, 165) },    /* get_value */
  { (const void*) &lv_bar_is_symmetrical, 6480, LV_MSIG(3, 128) },    /* is_symmetrical */
  { (const void*) &lv_bar_set_max_value, 2396, LV_MSIG(4, 81) },    /* set_max_value */
  { (const void*) &lv_bar_set_min_value, 2414, LV_MSIG(4, 81) },    /* set_min_value */
  { (const void*) &lv_bar_set_mode, 4180, LV_MSIG(4, 81) },    /* set_mode */
  { (const void*) &lv_bar_set_orientation, 6228, LV_MSIG(4, 81) },    /* set_orientation */
  { (const void*) &lv_bar_set_range, 4832, LV_MSIG(4, 93) },    /* set_range */
  { (const void*) &lv_bar_set_start_value, 6468, LV_MSIG(4, 93) },    /* set_start_value */
  { (const void*) &lv_bar_set_value, 1718, LV_MSIG(4, 93) },    /* set_value */
};
#endif // BE_LV_WIDGET_BAR

/* `lv_button` methods */
#ifdef BE_LV_WIDGET_BUTTON
const lv_ntv_method_t lv_button_func[] = {
};
#endif // BE_LV_WIDGET_BUTTON

/* `lv_buttonmatrix` methods */
#ifdef BE_LV_WIDGET_BUTTONMATRIX
const lv_ntv_method_t lv_buttonmatrix_func[] = {
  { (const void*) &lv_buttonmatrix_get_button_text, 7367, LV_MSIG(0, 238) },    /* _btn_text */
  { (const void*) &lv_buttonmatrix_clear_button_ctrl, 2082, LV_MSIG(0, 93) },    /* clear_button_ctrl */
  { (const void*) &lv_buttonmatrix_clear_button_ctrl_all, 847, LV_MSIG(0, 81) },    /* clear_button_ctrl_all */
  { (const void*) &lv_buttonmatrix_has_button_ctrl, 3257, LV_MSIG(0, 138) },    /* has_button_ctrl */
  { (const void*) &lv_buttonmatrix_get_button_text, 5904, LV_MSIG(1, 238) },    /* get_button_text */
  { (const void*) &lv_buttonmatrix_get_map, 8870, LV_MSIG(1, 149) },    /* get_map */
  { (const void*) &lv_buttonmatrix_get_one_checked, 6204, LV_MSIG(1, 128) },    /* get_one_checked */
  { (const void*) &lv_buttonmatrix_get_selected_button, 3529, LV_MSIG(1, 165) },    /* get_selected_button */
  { (const void*) &lv_buttonmatrix_set_button_ctrl, 2088, LV_MSIG(4, 93) },    /* set_button_ctrl */
  { (const void*) &lv_buttonmatrix_set_button_ctrl_all, 853, LV_MSIG(4, 81) },    /* set_button_ctrl_all */
  { (const void*) &lv_buttonmatrix_set_button_width, 5134, LV_MSIG(4, 93) },    /* set_button_width */
  { (const void*) &lv_buttonmatrix_set_map, 8870, LV_MSIG(4, 73) },    /* set_map */
  { (const void*) &lv_buttonmatrix_set_one_checked, 6204, LV_MSIG(4, 78) },    /* set_one_checked */
  { (const void*) &lv_buttonmatrix_set_selected_button, 3529, LV_MSIG(4, 81) },    /* set_selected_button */
};
#endif // BE_LV_WIDGET_BUTTONMATRIX

/* `lv_calendar` methods */
#ifdef BE_LV_WIDGET_CALENDAR
const lv_ntv_method_t lv_calendar_func[] = {
  { (const void*) &lv_calendar_add_header_arrow, 2585, LV_MSIG(0, 214) },    /* add_header_arrow */
  { (const void*) &lv_calendar_add_header_dropdown, 1213, LV_MSIG(0, 214) },    /* add_header_dropdown */
  { (const void*) &lv_calendar_header_dropdown_set_year_list, 128, LV_MSIG(0, 102) },    /* header_dropdown_set_year_list */
  { (const void*) &lv_calendar_get_btnmatrix, 7467, LV_MSIG(1, 214) },    /* get_btnmatrix */
  { (const void*) &lv_calendar_get_highlighted_dates_num, 913, LV_MSIG(1, 165) },    /* get_highlighted_dates_num */
  { (const void*) &lv_calendar_set_chinese_mode, 5147, LV_MSIG(4, 78) },    /* set_chinese_mode */
  { (const void*) &lv_calendar_set_day_names, 7547, LV_MSIG(4, 79) },    /* set_day_names */
  { (const void*) &lv_calendar_set_month_shown, 6180, LV_MSIG(4, 93) },    /* set_month_shown */
  { (const void*) &lv_calendar_set_shown_month, 6420, LV_MSIG(4, 81) },    /* set_shown_month */
  { (const void*) &lv_calendar_set_shown_year, 7213, LV_MSIG(4, 81) },    /* set_shown_year */
  { (const void*) &lv_calendar_set_today_date, 7334, LV_MSIG(4, 97) },    /* set_today_date */
  { (const void*) &lv_calendar_set_today_day, 7937, LV_MSIG(4, 81) },    /* set_today_day */
  { (const void*) &lv_calendar_set_today_month, 6504, LV_MSIG(4, 81) },    /* set_today_month */
  { (const void*) &lv_calendar_set_today_year, 7345, LV_MSIG(4, 81) },    /* set_today_year */
};
#endif // BE_LV_WIDGET_CALENDAR

/* `lv_canvas` methods */
#ifdef BE_LV_WIDGET_CANVAS
const lv_ntv_method_t lv_canvas_func[] = {
  { (const void*) &lv_canvas_fill_bg, 8443, LV_MSIG(0, 53) },    /* fill_bg */
  { (const void*) &lv_canvas_finish_layer, 5316, LV_MSIG(0, 57) },    /* finish_layer */
  { (const void*) &lv_canvas_init_layer, 6872, LV_MSIG(0, 57) },    /* init_layer */
  { (const void*) &lv_canvas_get_buf, 8850, LV_MSIG(1, 149) },    /* get_buf */
  { (const void*) &lv_canvas_get_image, 8757, LV_MSIG(1, 204) },    /* get_image */
  { (const void*) &lv_canvas_get_px, 8863, LV_MSIG(1, 177) },    /* get_px */
  { (const void*) &lv_canvas_set_buffer, 8602, LV_MSIG(4, 77) },    /* set_buffer */
  { (const void*) &lv_canvas_set_palette, 8523, LV_MSIG(4, 93) },    /* set_palette */
  { (const void*) &lv_canvas_set_px, 8863, LV_MSIG(4, 94) },    /* set_px */
};
#endif // BE_LV_WIDGET_CANVAS

/* `lv_chart` methods */
#ifdef BE_LV_WIDGET_CHART
const lv_ntv_method_t lv_chart_func[] = {
  { (const void*) &lv_chart_add_cursor, 6575, LV_MSIG(0, 186) },    /* add_cursor */
  { (const void*) &lv_chart_add_series, 6597, LV_MSIG(0, 188) },    /* add_series */
  { (const void*) &lv_chart_hide_series, 6096, LV_MSIG(0, 47) },    /* hide_series */
  { (const void*) &lv_chart_refresh, 8539, LV_MSIG(0, 34) },    /* refresh */
  { (const void*) &lv_chart_remove_cursor, 4712, LV_MSIG(0, 38) },    /* remove_cursor */
  { (const void*) &lv_chart_remove_series, 4726, LV_MSIG(0, 42) },    /* remove_series */
  { (const void*) &lv_chart_get_cursor_point, 5225, LV_MSIG(1, 167) },    /* get_cursor_point */
  { (const void*) &lv_chart_get_first_point_center_offset, 296, LV_MSIG(1, 165) },    /* get_first_point_center_offset */
  { (const void*) &lv_chart_get_hor_div_line_count, 1686, LV_MSIG(1, 165) },    /* get_hor_div_line_count */
  { (const void*) &lv_chart_get_point_count, 6264, LV_MSIG(1, 165) },    /* get_point_count */
  { (const void*) &lv_chart_get_point_pos_by_id, 3417, LV_MSIG(1, 49) },    /* get_point_pos_by_id */
  { (const void*) &lv_chart_get_pressed_point, 4670, LV_MSIG(1, 165) },    /* get_pressed_point */
  { (const void*) &lv_chart_get_series_color, 5628, LV_MSIG(1, 190) },    /* get_series_color */
  { (const void*) &lv_chart_get_series_next, 6408, LV_MSIG(1, 187) },    /* get_series_next */
  { (const void*) &lv_chart_get_series_x_array, 4215, LV_MSIG(1, 207) },    /* get_series_x_array */
  { (const void*) &lv_chart_get_series_y_array, 4230, LV_MSIG(1, 207) },    /* get_series_y_array */
  { (const void*) &lv_chart_get_type, 1308, LV_MSIG(1, 165) },    /* get_type */
  { (const void*) &lv_chart_get_update_mode, 6540, LV_MSIG(1, 165) },    /* get_update_mode */
  { (const void*) &lv_chart_get_ver_div_line_count, 2009, LV_MSIG(1, 165) },    /* get_ver_div_line_count */
  { (const void*) &lv_chart_get_x_start_point, 4964, LV_MSIG(1, 168) },    /* get_x_start_point */
  { (const void*) &lv_chart_set_all_values, 6608, LV_MSIG(4, 48) },    /* set_all_values */
  { (const void*) &lv_chart_set_axis_max_value, 3720, LV_MSIG(4, 93) },    /* set_axis_max_value */
  { (const void*) &lv_chart_set_axis_min_value, 3735, LV_MSIG(4, 93) },    /* set_axis_min_value */
  { (const void*) &lv_chart_set_axis_range, 6630, LV_MSIG(4, 97) },    /* set_axis_range */
  { (const void*) &lv_chart_set_cursor_point, 5225, LV_MSIG(4, 39) },    /* set_cursor_point */
  { (const void*) &lv_chart_set_cursor_pos, 6751, LV_MSIG(4, 40) },    /* set_cursor_pos */
  { (const void*) &lv_chart_set_cursor_pos_x, 5238, LV_MSIG(4, 41) },    /* set_cursor_pos_x */
  { (const void*) &lv_chart_set_cursor_pos_y, 5251, LV_MSIG(4, 41) },    /* set_cursor_pos_y */
  { (const void*) &lv_chart_set_div_line_count, 1690, LV_MSIG(4, 93) },    /* set_div_line_count */
  { (const void*) &lv_chart_set_hor_div_line_count, 1686, LV_MSIG(4, 81) },    /* set_hor_div_line_count */
  { (const void*) &lv_chart_set_next_value, 6993, LV_MSIG(4, 48) },    /* set_next_value */
  { (const void*) &lv_chart_set_next_value2, 6192, LV_MSIG(4, 50) },    /* set_next_value2 */
  { (const void*) &lv_chart_set_point_count, 6264, LV_MSIG(4, 81) },    /* set_point_count */
  { (const void*) &lv_chart_set_axis_range, 4832, LV_MSIG(4, 97) },    /* set_range */
  { (const void*) &lv_chart_set_series_color, 5628, LV_MSIG(4, 43) },    /* set_series_color */
  { (const void*) &lv_chart_set_series_ext_x_array, 1838, LV_MSIG(4, 44) },    /* set_series_ext_x_array */
  { (const void*) &lv_chart_set_series_ext_y_array, 1857, LV_MSIG(4, 44) },    /* set_series_ext_y_array */
  { (const void*) &lv_chart_set_series_value_by_id, 1876, LV_MSIG(4, 50) },    /* set_series_value_by_id */
  { (const void*) &lv_chart_set_series_value_by_id2, 1473, LV_MSIG(4, 51) },    /* set_series_value_by_id2 */
  { (const void*) &lv_chart_set_series_values, 4852, LV_MSIG(4, 46) },    /* set_series_values */
  { (const void*) &lv_chart_set_series_values2, 4200, LV_MSIG(4, 45) },    /* set_series_values2 */
  { (const void*) &lv_chart_set_type, 1308, LV_MSIG(4, 81) },    /* set_type */
  { (const void*) &lv_chart_set_update_mode, 6540, LV_MSIG(4, 81) },    /* set_update_mode */
  { (const void*) &lv_chart_set_ver_div_line_count, 2009, LV_MSIG(4, 81) },    /* set_ver_div_line_count */
  { (const void*) &lv_chart_set_x_start_point, 4964, LV_MSIG(4, 48) },    /* set_x_start_point */
};
#endif // BE_LV_WIDGET_CHART

/* `lv_checkbox` methods */
#ifdef BE_LV_WIDGET_CHECKBOX
const lv_ntv_method_t lv_checkbox_func[] = {
  { (const void*) &lv_checkbox_get_text, 2835, LV_MSIG(1, 235) },    /* get_text */
  { (const void*) &lv_checkbox_set_text, 2835, LV_MSIG(4, 102) },    /* set_text */
  { (const void*) &lv_checkbox_set_text_static, 2947, LV_MSIG(4, 102) },    /* set_text_static */
};
#endif // BE_LV_WIDGET_CHECKBOX

/* `lv_color` methods */
const lv_ntv_method_t lv_color_func[] = {
};

/* `lv_colorwheel` methods */
#ifdef BE_LV_WIDGET_COLORWHEEL
const lv_ntv_method_t lv_colorwheel_func[] = {
  { (const void*) &lv_colorwheel_get_color_mode, 6729, LV_MSIG(1, 165) },    /* get_color_mode */
  { (const void*) &lv_colorwheel_get_color_mode_fixed, 2653, LV_MSIG(1, 128) },    /* get_color_mode_fixed */
  { (const void*) &lv_colorwheel_get_hsv, 8866, LV_MSIG(1, 165) },    /* get_hsv */
  { (const void*) &lv_colorwheel_get_rgb, 8878, LV_MSIG(1, 189) },    /* get_rgb */
  { (const void*) &lv_colorwheel_set_hsv, 8866, LV_MSIG(4, 137) },    /* set_hsv */
  { (const void*) &lv_colorwheel_set_mode, 4180, LV_MSIG(4, 81) },    /* set_mode */
  { (const void*) &lv_colorwheel_set_mode_fixed, 2659, LV_MSIG(4, 78) },    /* set_mode_fixed */
  { (const void*) &lv_colorwheel_set_rgb, 8878, LV_MSIG(4, 130) },    /* set_rgb */
};
#endif // BE_LV_WIDGET_COLORWHEEL

/* `lv_display` methods */
const lv_ntv_method_t lv_display_func[] = {
  { (const void*) &lv_display_add_event_cb, 4978, LV_MSIG(0, 195) },    /* add_event_cb */
  { (const void*) &lv_display_delete, 8854, LV_MSIG(0, 7) },    /* del */
  { (const void*) &lv_display_delete, 4481, LV_MSIG(0, 7) },    /* delete */
  { (const void*) &lv_display_delete_refr_timer, 2100, LV_MSIG(0, 7) },    /* delete_refr_timer */
  { (const void*) &lv_display_dpx, 8862, LV_MSIG(0, 156) },    /* dpx */
  { (const void*) &lv_display_enable_invalidation, 1333, LV_MSIG(0, 13) },    /* enable_invalidation */
  { (const void*) &lv_display_register_vsync_event, 760, LV_MSIG(0, 123) },    /* register_vsync_event */
  { (const void*) &lv_display_delete, 8672, LV_MSIG(0, 7) },    /* remove */
  { (const void*) &lv_display_remove_event, 5459, LV_MSIG(0, 124) },    /* remove_event */
  { (const void*) &lv_display_remove_event_cb_with_user_data, 97, LV_MSIG(0, 155) },    /* remove_event_cb_with_user_data */
  { (const void*) &lv_display_rotate_area, 6336, LV_MSIG(0, 8) },    /* rotate_area */
  { (const void*) &lv_display_rotate_point, 5563, LV_MSIG(0, 14) },    /* rotate_point */
  { (const void*) &lv_display_rotate_point_ccw, 2908, LV_MSIG(0, 14) },    /* rotate_point_ccw */
  { (const void*) &lv_display_send_event, 7191, LV_MSIG(0, 157) },    /* send_event */
  { (const void*) &lv_display_send_vsync_event, 2925, LV_MSIG(0, 154) },    /* send_vsync_event */
  { (const void*) &lv_display_trigger_activity, 4936, LV_MSIG(0, 7) },    /* trig_activity */
  { (const void*) &lv_display_trigger_activity, 3112, LV_MSIG(0, 7) },    /* trigger_activity */
  { (const void*) &lv_display_unregister_vsync_event, 758, LV_MSIG(0, 123) },    /* unregister_vsync_event */
  { (const void*) &lv_display_get_rotation, 1447, LV_MSIG(1, 153) },    /* get_angle */
  { (const void*) &lv_display_get_antialiasing, 5004, LV_MSIG(1, 122) },    /* get_antialiasing */
  { (const void*) &lv_display_get_color_format, 5160, LV_MSIG(1, 153) },    /* get_color_format */
  { (const void*) &lv_display_get_dpi, 8858, LV_MSIG(1, 153) },    /* get_dpi */
  { (const void*) &lv_display_get_draw_buf_size, 334, LV_MSIG(1, 153) },    /* get_draw_buf_size */
  { (const void*) &lv_display_get_driver_data, 6036, LV_MSIG(1, 145) },    /* get_driver_data */
  { (const void*) &lv_display_get_event_count, 6072, LV_MSIG(1, 153) },    /* get_event_count */
  { (const void*) &lv_display_get_event_dsc, 2847, LV_MSIG(1, 196) },    /* get_event_dsc */
  { (const void*) &lv_display_get_horizontal_resolution, 2798, LV_MSIG(1, 153) },    /* get_hor_res */
  { (const void*) &lv_display_get_horizontal_resolution, 44, LV_MSIG(1, 153) },    /* get_horizontal_resolution */
  { (const void*) &lv_display_get_inactive_time, 4544, LV_MSIG(1, 153) },    /* get_inactive_time */
  { (const void*) &lv_display_get_invalidated_draw_buf_size, 322, LV_MSIG(1, 158) },    /* get_invalidated_draw_buf_size */
  { (const void*) &lv_display_get_layer_bottom, 5368, LV_MSIG(1, 209) },    /* get_layer_bottom */
  { (const void*) &lv_display_get_layer_sys, 7677, LV_MSIG(1, 209) },    /* get_layer_sys */
  { (const void*) &lv_display_get_layer_top, 7687, LV_MSIG(1, 209) },    /* get_layer_top */
  { (const void*) &lv_display_get_matrix_rotation, 3337, LV_MSIG(1, 122) },    /* get_matrix_rotation */
  { (const void*) &lv_display_get_next, 6415, LV_MSIG(1, 192) },    /* get_next */
  { (const void*) &lv_display_get_offset_x, 3168, LV_MSIG(1, 153) },    /* get_offset_x */
  { (const void*) &lv_display_get_offset_y, 3184, LV_MSIG(1, 153) },    /* get_offset_y */
  { (const void*) &lv_display_get_original_horizontal_resolution, 35, LV_MSIG(1, 153) },    /* get_original_horizontal_resolution */
  { (const void*) &lv_display_get_original_vertical_resolution, 158, LV_MSIG(1, 153) },    /* get_original_vertical_resolution */
  { (const void*) &lv_display_get_physical_horizontal_resolution, 2789, LV_MSIG(1, 153) },    /* get_physical_hor_res */
  { (const void*) &lv_display_get_physical_horizontal_resolution, 66, LV_MSIG(1, 153) },    /* get_physical_horizontal_resolution */
  { (const void*) &lv_display_get_physical_vertical_resolution, 2806, LV_MSIG(1, 153) },    /* get_physical_ver_res */
  { (const void*) &lv_display_get_physical_vertical_resolution, 187, LV_MSIG(1, 153) },    /* get_physical_vertical_resolution */
  { (const void*) &lv_display_get_refr_timer, 2107, LV_MSIG(1, 232) },    /* get_refr_timer */
  { (const void*) &lv_display_get_render_mode, 6312, LV_MSIG(1, 153) },    /* get_render_mode */
  { (const void*) &lv_display_get_rotation, 1981, LV_MSIG(1, 153) },    /* get_rotation */
  { (const void*) &lv_display_get_screen_active, 8547, LV_MSIG(1, 209) },    /* get_scr_act */
  { (const void*) &lv_display_get_screen_prev, 8213, LV_MSIG(1, 209) },    /* get_scr_prev */
  { (const void*) &lv_display_get_screen_active, 4768, LV_MSIG(1, 209) },    /* get_screen_active */
  { (const void*) &lv_display_get_screen_loading, 4125, LV_MSIG(1, 209) },    /* get_screen_loading */
  { (const void*) &lv_display_get_screen_prev, 6348, LV_MSIG(1, 209) },    /* get_screen_prev */
  { (const void*) &lv_display_get_theme, 5505, LV_MSIG(1, 231) },    /* get_theme */
  { (const void*) &lv_display_get_tile_cnt, 8321, LV_MSIG(1, 153) },    /* get_tile_cnt */
  { (const void*) &lv_display_get_user_data, 118, LV_MSIG(1, 145) },    /* get_user_data */
  { (const void*) &lv_display_get_vertical_resolution, 2815, LV_MSIG(1, 153) },    /* get_ver_res */
  { (const void*) &lv_display_get_vertical_resolution, 167, LV_MSIG(1, 153) },    /* get_vertical_resolution */
  { (const void*) &lv_display_is_double_buffered, 3225, LV_MSIG(3, 122) },    /* is_double_buffered */
  { (const void*) &lv_display_is_invalidation_enabled, 1129, LV_MSIG(3, 122) },    /* is_invalidation_enabled */
  { (const void*) &lv_display_set_rotation, 1447, LV_MSIG(4, 15) },    /* set_angle */
  { (const void*) &lv_display_set_antialiasing, 5004, LV_MSIG(4, 13) },    /* set_antialiasing */
  { (const void*) &lv_display_set_buffers, 8379, LV_MSIG(4, 11) },    /* set_buffers */
  { (const void*) &lv_display_set_buffers_with_stride, 1253, LV_MSIG(4, 12) },    /* set_buffers_with_stride */
  { (const void*) &lv_display_set_color_format, 5160, LV_MSIG(4, 15) },    /* set_color_format */
  { (const void*) &lv_display_set_default, 8395, LV_MSIG(4, 7) },    /* set_default */
  { (const void*) &lv_display_set_dpi, 8858, LV_MSIG(4, 15) },    /* set_dpi */
  { (const void*) &lv_display_set_driver_data, 6036, LV_MSIG(4, 10) },    /* set_driver_data */
  { (const void*) &lv_display_set_matrix_rotation, 3337, LV_MSIG(4, 13) },    /* set_matrix_rotation */
  { (const void*) &lv_display_set_offset, 315, LV_MSIG(4, 16) },    /* set_offset */
  { (const void*) &lv_display_set_physical_resolution, 1393, LV_MSIG(4, 16) },    /* set_physical_resolution */
  { (const void*) &lv_display_set_render_mode, 6312, LV_MSIG(4, 15) },    /* set_render_mode */
  { (const void*) &lv_display_set_resolution, 55, LV_MSIG(4, 16) },    /* set_resolution */
  { (const void*) &lv_display_set_rotation, 1981, LV_MSIG(4, 15) },    /* set_rotation */
  { (const void*) &lv_display_set_theme, 5505, LV_MSIG(4, 9) },    /* set_theme */
  { (const void*) &lv_display_set_tile_cnt, 8321, LV_MSIG(4, 15) },    /* set_tile_cnt */
  { (const void*) &lv_display_set_user_data, 118, LV_MSIG(4, 10) },    /* set_user_data */
};

/* `lv_dropdown` methods */
#ifdef BE_LV_WIDGET_DROPDOWN
const lv_ntv_method_t lv_dropdown_func[] = {
  { (const void*) &lv_dropdown_add_option, 6586, LV_MSIG(0, 104) },    /* add_option */
  { (const void*) &lv_dropdown_clear_options, 4432, LV_MSIG(0, 34) },    /* clear_options */
  { (const void*) &lv_dropdown_close, 8733, LV_MSIG(0, 34) },    /* close */
  { (const void*) &lv_dropdown_open, 8825, LV_MSIG(0, 34) },    /* open */
  { (const void*) &lv_dropdown_get_dir, 5852, LV_MSIG(1, 165) },    /* get_dir */
  { (const void*) &lv_dropdown_get_list, 153, LV_MSIG(1, 214) },    /* get_list */
  { (const void*) &lv_dropdown_get_option_count, 7004, LV_MSIG(1, 165) },    /* get_option_cnt */
  { (const void*) &lv_dropdown_get_option_count, 5420, LV_MSIG(1, 165) },    /* get_option_count */
  { (const void*) &lv_dropdown_get_option_index, 5433, LV_MSIG(1, 179) },    /* get_option_index */
  { (const void*) &lv_dropdown_get_options, 4438, LV_MSIG(1, 235) },    /* get_options */
  { (const void*) &lv_dropdown_get_selected, 3001, LV_MSIG(1, 165) },    /* get_selected */
  { (const void*) &lv_dropdown_get_selected_highlight, 1819, LV_MSIG(1, 128) },    /* get_selected_highlight */
  { (const void*) &lv_dropdown_get_selected_str, 5615, LV_MSIG(1, 80) },    /* get_selected_str */
  { (const void*) &lv_dropdown_get_symbol, 8700, LV_MSIG(1, 235) },    /* get_symbol */
  { (const void*) &lv_dropdown_get_text, 2835, LV_MSIG(1, 235) },    /* get_text */
  { (const void*) &lv_dropdown_is_open, 8825, LV_MSIG(3, 128) },    /* is_open */
  { (const void*) &lv_dropdown_set_dir, 5852, LV_MSIG(4, 81) },    /* set_dir */
  { (const void*) &lv_dropdown_set_options, 4438, LV_MSIG(4, 102) },    /* set_options */
  { (const void*) &lv_dropdown_set_options_static, 4080, LV_MSIG(4, 102) },    /* set_options_static */
  { (const void*) &lv_dropdown_set_selected, 3001, LV_MSIG(4, 81) },    /* set_selected */
  { (const void*) &lv_dropdown_set_selected_highlight, 1819, LV_MSIG(4, 78) },    /* set_selected_highlight */
  { (const void*) &lv_dropdown_set_symbol, 8700, LV_MSIG(4, 76) },    /* set_symbol */
  { (const void*) &lv_dropdown_set_text, 2835, LV_MSIG(4, 102) },    /* set_text */
  { (const void*) &lv_dropdown_set_text_static, 2947, LV_MSIG(4, 102) },    /* set_text_static */
};
#endif // BE_LV_WIDGET_DROPDOWN

/* `lv_event` methods */
const lv_ntv_method_t lv_event_func[] = {
  { (const void*) &lv_event_free_user_data_cb, 2172, LV_MSIG(0, 17) },    /* free_user_data_cb */
  { (const void*) &lv_event_stop_bubbling, 4908, LV_MSIG(0, 17) },    /* stop_bubbling */
  { (const void*) &lv_event_stop_processing, 3593, LV_MSIG(0, 17) },    /* stop_processing */
  { (const void*) &lv_event_stop_trickling, 4260, LV_MSIG(0, 17) },    /* stop_trickling */
  { (const void*) &lv_event_get_code, 8805, LV_MSIG(1, 159) },    /* get_code */
  { (const void*) &lv_event_get_cover_area, 6740, LV_MSIG(1, 184) },    /* get_cover_area */
  { (const void*) &lv_event_get_current_target, 3930, LV_MSIG(1, 146) },    /* get_current_target */
  { (const void*) &lv_event_get_current_target_obj, 1629, LV_MSIG(1, 210) },    /* get_current_target_obj */
  { (const void*) &lv_event_get_hit_test_info, 4530, LV_MSIG(1, 146) },    /* get_hit_test_info */
  { (const void*) &lv_event_get_indev, 8763, LV_MSIG(1, 205) },    /* get_indev */
  { (const void*) &lv_event_get_invalidated_area, 2738, LV_MSIG(1, 184) },    /* get_invalidated_area */
  { (const void*) &lv_event_get_key, 4046, LV_MSIG(1, 159) },    /* get_key */
  { (const void*) &lv_event_get_layer, 5323, LV_MSIG(1, 208) },    /* get_layer */
  { (const void*) &lv_event_get_old_size, 8132, LV_MSIG(1, 184) },    /* get_old_size */
  { (const void*) &lv_event_get_param, 4356, LV_MSIG(1, 146) },    /* get_param */
  { (const void*) &lv_event_get_prev_state, 7070, LV_MSIG(1, 159) },    /* get_prev_state */
  { (const void*) &lv_event_get_rotary_diff, 6324, LV_MSIG(1, 159) },    /* get_rotary_diff */
  { (const void*) &lv_event_get_scroll_anim, 2964, LV_MSIG(1, 182) },    /* get_scroll_anim */
  { (const void*) &lv_event_get_self_size_info, 4185, LV_MSIG(1, 146) },    /* get_self_size_info */
  { (const void*) &lv_event_get_target, 3938, LV_MSIG(1, 146) },    /* get_target */
  { (const void*) &lv_event_get_target_obj, 1637, LV_MSIG(1, 210) },    /* get_target_obj */
  { (const void*) &lv_event_get_user_data, 118, LV_MSIG(1, 146) },    /* get_user_data */
  { (const void*) &lv_event_set_cover_res, 7527, LV_MSIG(4, 18) },    /* set_cover_res */
  { (const void*) &lv_event_set_ext_draw_size, 509, LV_MSIG(4, 18) },    /* set_ext_draw_size */
};

/* `lv_font` methods */
const lv_ntv_method_t lv_font_func[] = {
};

/* `lv_group` methods */
const lv_ntv_method_t lv_group_func[] = {
  { (const void*) &lv_group_add_obj, 8347, LV_MSIG(0, 20) },    /* add_obj */
  { (const void*) &lv_group_delete, 8854, LV_MSIG(0, 19) },    /* del */
  { (const void*) &lv_group_delete, 4481, LV_MSIG(0, 19) },    /* delete */
  { (const void*) &lv_group_focus_freeze, 5329, LV_MSIG(0, 23) },    /* focus_freeze */
  { (const void*) &lv_group_focus_next, 6806, LV_MSIG(0, 19) },    /* focus_next */
  { (const void*) &lv_group_focus_obj, 7607, LV_MSIG(0, 34) },    /* focus_obj */
  { (const void*) &lv_group_focus_prev, 6817, LV_MSIG(0, 19) },    /* focus_prev */
  { (const void*) &lv_group_delete, 8672, LV_MSIG(0, 19) },    /* remove */
  { (const void*) &lv_group_remove_all_objs, 3481, LV_MSIG(0, 19) },    /* remove_all_objs */
  { (const void*) &lv_group_remove_obj, 7103, LV_MSIG(0, 34) },    /* remove_obj */
  { (const void*) &lv_group_send_data, 7837, LV_MSIG(0, 161) },    /* send_data */
  { (const void*) &lv_group_swap_obj, 8258, LV_MSIG(0, 58) },    /* swap_obj */
  { (const void*) &lv_group_get_edge_cb, 8419, LV_MSIG(1, 119) },    /* get_edge_cb */
  { (const void*) &lv_group_get_editing, 8427, LV_MSIG(1, 125) },    /* get_editing */
  { (const void*) &lv_group_get_focus_cb, 8096, LV_MSIG(1, 203) },    /* get_focus_cb */
  { (const void*) &lv_group_get_focused, 8451, LV_MSIG(1, 211) },    /* get_focused */
  { (const void*) &lv_group_get_obj_by_index, 5407, LV_MSIG(1, 212) },    /* get_obj_by_index */
  { (const void*) &lv_group_get_obj_count, 7757, LV_MSIG(1, 160) },    /* get_obj_count */
  { (const void*) &lv_group_get_user_data, 118, LV_MSIG(1, 147) },    /* get_user_data */
  { (const void*) &lv_group_get_wrap, 8845, LV_MSIG(1, 125) },    /* get_wrap */
  { (const void*) &lv_group_set_default, 8395, LV_MSIG(4, 19) },    /* set_default */
  { (const void*) &lv_group_set_editing, 8427, LV_MSIG(4, 23) },    /* set_editing */
  { (const void*) &lv_group_set_focus_cb, 8096, LV_MSIG(4, 22) },    /* set_focus_cb */
  { (const void*) &lv_group_set_refocus_policy, 4110, LV_MSIG(4, 24) },    /* set_refocus_policy */
  { (const void*) &lv_group_set_user_data, 118, LV_MSIG(4, 21) },    /* set_user_data */
  { (const void*) &lv_group_set_wrap, 8845, LV_MSIG(4, 23) },    /* set_wrap */
};

/* `lv_image` methods */
#ifdef BE_LV_WIDGET_IMAGE
const lv_ntv_method_t lv_image_func[] = {
  { (const void*) &lv_image_get_rotation, 1447, LV_MSIG(1, 165) },    /* get_angle */
  { (const void*) &lv_image_get_antialias, 7417, LV_MSIG(1, 128) },    /* get_antialias */
  { (const void*) &lv_image_get_bitmap_map_src, 3810, LV_MSIG(1, 204) },    /* get_bitmap_map_src */
  { (const void*) &lv_image_get_blend_mode, 6663, LV_MSIG(1, 165) },    /* get_blend_mode */
  { (const void*) &lv_image_get_inner_align, 6108, LV_MSIG(1, 165) },    /* get_inner_align */
  { (const void*) &lv_image_get_offset_x, 3168, LV_MSIG(1, 165) },    /* get_offset_x */
  { (const void*) &lv_image_get_offset_y, 3184, LV_MSIG(1, 165) },    /* get_offset_y */
  { (const void*) &lv_image_get_pivot, 8787, LV_MSIG(1, 79) },    /* get_pivot */
  { (const void*) &lv_image_get_rotation, 1981, LV_MSIG(1, 165) },    /* get_rotation */
  { (const void*) &lv_image_get_scale, 3667, LV_MSIG(1, 165) },    /* get_scale */
  { (const void*) &lv_image_get_scale_x, 2506, LV_MSIG(1, 165) },    /* get_scale_x */
  { (const void*) &lv_image_get_scale_y, 2524, LV_MSIG(1, 165) },    /* get_scale_y */
  { (const void*) &lv_image_get_src, 3157, LV_MSIG(1, 149) },    /* get_src */
  { (const void*) &lv_image_get_src_height, 7257, LV_MSIG(1, 165) },    /* get_src_height */
  { (const void*) &lv_image_get_src_width, 7887, LV_MSIG(1, 165) },    /* get_src_width */
  { (const void*) &lv_image_get_transformed_height, 1990, LV_MSIG(1, 165) },    /* get_transformed_height */
  { (const void*) &lv_image_get_transformed_width, 2532, LV_MSIG(1, 165) },    /* get_transformed_width */
  { (const void*) &lv_image_get_scale, 4315, LV_MSIG(1, 165) },    /* get_zoom */
  { (const void*) &lv_image_set_rotation, 1447, LV_MSIG(4, 81) },    /* set_angle */
  { (const void*) &lv_image_set_antialias, 7417, LV_MSIG(4, 78) },    /* set_antialias */
  { (const void*) &lv_image_set_bitmap_map_src, 3810, LV_MSIG(4, 55) },    /* set_bitmap_map_src */
  { (const void*) &lv_image_set_blend_mode, 6663, LV_MSIG(4, 81) },    /* set_blend_mode */
  { (const void*) &lv_image_set_inner_align, 6108, LV_MSIG(4, 81) },    /* set_inner_align */
  { (const void*) &lv_image_set_offset_x, 3168, LV_MSIG(4, 81) },    /* set_offset_x */
  { (const void*) &lv_image_set_offset_y, 3184, LV_MSIG(4, 81) },    /* set_offset_y */
  { (const void*) &lv_image_set_pivot, 8787, LV_MSIG(4, 93) },    /* set_pivot */
  { (const void*) &lv_image_set_pivot_x, 2470, LV_MSIG(4, 81) },    /* set_pivot_x */
  { (const void*) &lv_image_set_pivot_y, 2488, LV_MSIG(4, 81) },    /* set_pivot_y */
  { (const void*) &lv_image_set_rotation, 1981, LV_MSIG(4, 81) },    /* set_rotation */
  { (const void*) &lv_image_set_scale, 3667, LV_MSIG(4, 81) },    /* set_scale */
  { (const void*) &lv_image_set_scale_x, 2506, LV_MSIG(4, 81) },    /* set_scale_x */
  { (const void*) &lv_image_set_scale_y, 2524, LV_MSIG(4, 81) },    /* set_scale_y */
  { (const void*) &lv_image_set_src, 3157, LV_MSIG(4, 76) },    /* set_src */
  { (const void*) &lv_image_set_tasmota_logo, 5758, LV_MSIG(4, 34) },    /* set_tasmota_logo */
  { (const void*) &lv_image_set_tasmota_logo36, 4275, LV_MSIG(4, 34) },    /* set_tasmota_logo36 */
  { (const void*) &lv_image_set_scale, 4315, LV_MSIG(4, 81) },    /* set_zoom */
};
#endif // BE_LV_WIDGET_IMAGE

/* `lv_imagebutton` methods */
#ifdef BE_LV_WIDGET_IMAGEBUTTON
const lv_ntv_method_t lv_imagebutton_func[] = {
  { (const void*) &lv_imagebutton_set_src, 3157, LV_MSIG(4, 90) },    /* set_src */
  { (const void*) &lv_imagebutton_set_src_left, 8240, LV_MSIG(4, 89) },    /* set_src_left */
  { (const void*) &lv_imagebutton_set_src_mid, 8563, LV_MSIG(4, 89) },    /* set_src_mid */
  { (const void*) &lv_imagebutton_set_src_right, 7877, LV_MSIG(4, 89) },    /* set_src_right */
  { (const void*) &lv_imagebutton_set_state, 5479, LV_MSIG(4, 81) },    /* set_state */
};
#endif // BE_LV_WIDGET_IMAGEBUTTON

/* `lv_indev` methods */
const lv_ntv_method_t lv_indev_func[] = {
  { (const void*) &lv_indev_add_event_cb, 4978, LV_MSIG(0, 197) },    /* add_event_cb */
  { (const void*) &lv_indev_clear_ccw, 7497, LV_MSIG(0, 25) },    /* clear_ccw */
  { (const void*) &lv_indev_delete, 8854, LV_MSIG(0, 25) },    /* del */
  { (const void*) &lv_indev_delete, 4481, LV_MSIG(0, 25) },    /* delete */
  { (const void*) &lv_indev_enable, 8616, LV_MSIG(0, 31) },    /* enable */
  { (const void*) &lv_indev_read, 4875, LV_MSIG(0, 25) },    /* read */
  { (const void*) &lv_indev_delete, 8672, LV_MSIG(0, 25) },    /* remove */
  { (const void*) &lv_indev_remove_event, 5459, LV_MSIG(0, 127) },    /* remove_event */
  { (const void*) &lv_indev_remove_event_cb_with_user_data, 97, LV_MSIG(0, 163) },    /* remove_event_cb_with_user_data */
  { (const void*) &lv_indev_reset, 8799, LV_MSIG(0, 28) },    /* reset */
  { (const void*) &lv_indev_reset_long_press, 2874, LV_MSIG(0, 25) },    /* reset_long_press */
  { (const void*) &lv_indev_search_obj, 7169, LV_MSIG(0, 217) },    /* search_obj */
  { (const void*) &lv_indev_send_event, 7191, LV_MSIG(0, 164) },    /* send_event */
  { (const void*) &lv_indev_stop_processing, 3593, LV_MSIG(0, 25) },    /* stop_processing */
  { (const void*) &lv_indev_wait_release, 5771, LV_MSIG(0, 25) },    /* wait_release */
  { (const void*) &lv_indev_get_ccw, 2921, LV_MSIG(1, 126) },    /* get_ccw */
  { (const void*) &lv_indev_get_cursor, 4719, LV_MSIG(1, 213) },    /* get_cursor */
  { (const void*) &lv_indev_get_display, 8815, LV_MSIG(1, 193) },    /* get_disp */
  { (const void*) &lv_indev_get_display, 8403, LV_MSIG(1, 193) },    /* get_display */
  { (const void*) &lv_indev_get_driver_data, 6036, LV_MSIG(1, 148) },    /* get_driver_data */
  { (const void*) &lv_indev_get_event_count, 6072, LV_MSIG(1, 162) },    /* get_event_count */
  { (const void*) &lv_indev_get_event_dsc, 2847, LV_MSIG(1, 198) },    /* get_event_dsc */
  { (const void*) &lv_indev_get_gesture_dir, 6084, LV_MSIG(1, 162) },    /* get_gesture_dir */
  { (const void*) &lv_indev_get_group, 8751, LV_MSIG(1, 201) },    /* get_group */
  { (const void*) &lv_indev_get_key, 4046, LV_MSIG(1, 162) },    /* get_key */
  { (const void*) &lv_indev_get_mode, 4180, LV_MSIG(1, 162) },    /* get_mode */
  { (const void*) &lv_indev_get_next, 6415, LV_MSIG(1, 206) },    /* get_next */
  { (const void*) &lv_indev_get_point, 3651, LV_MSIG(1, 32) },    /* get_point */
  { (const void*) &lv_indev_get_press_moved, 6276, LV_MSIG(1, 126) },    /* get_press_moved */
  { (const void*) &lv_indev_get_read_timer, 7092, LV_MSIG(1, 233) },    /* get_read_timer */
  { (const void*) &lv_indev_get_scroll_dir, 7114, LV_MSIG(1, 162) },    /* get_scroll_dir */
  { (const void*) &lv_indev_get_scroll_obj, 7136, LV_MSIG(1, 213) },    /* get_scroll_obj */
  { (const void*) &lv_indev_get_short_click_streak, 1895, LV_MSIG(1, 162) },    /* get_short_click_streak */
  { (const void*) &lv_indev_get_state, 5479, LV_MSIG(1, 162) },    /* get_state */
  { (const void*) &lv_indev_get_type, 1308, LV_MSIG(1, 162) },    /* get_type */
  { (const void*) &lv_indev_get_user_data, 118, LV_MSIG(1, 148) },    /* get_user_data */
  { (const void*) &lv_indev_get_vect, 8840, LV_MSIG(1, 32) },    /* get_vect */
  { (const void*) &lv_indev_set_button_points, 4376, LV_MSIG(4, 29) },    /* set_button_points */
  { (const void*) &lv_indev_set_ccw, 2921, LV_MSIG(4, 25) },    /* set_ccw */
  { (const void*) &lv_indev_set_cursor, 4719, LV_MSIG(4, 28) },    /* set_cursor */
  { (const void*) &lv_indev_set_display, 8815, LV_MSIG(4, 26) },    /* set_disp */
  { (const void*) &lv_indev_set_display, 8403, LV_MSIG(4, 26) },    /* set_display */
  { (const void*) &lv_indev_set_double_click_time, 2118, LV_MSIG(4, 33) },    /* set_double_click_time */
  { (const void*) &lv_indev_set_driver_data, 6036, LV_MSIG(4, 30) },    /* set_driver_data */
  { (const void*) &lv_indev_set_group, 8751, LV_MSIG(4, 27) },    /* set_group */
  { (const void*) &lv_indev_set_long_press_repeat_time, 666, LV_MSIG(4, 33) },    /* set_long_press_repeat_time */
  { (const void*) &lv_indev_set_long_press_time, 3321, LV_MSIG(4, 33) },    /* set_long_press_time */
  { (const void*) &lv_indev_set_mode, 4180, LV_MSIG(4, 33) },    /* set_mode */
  { (const void*) &lv_indev_set_scroll_limit, 5576, LV_MSIG(4, 33) },    /* set_scroll_limit */
  { (const void*) &lv_indev_set_scroll_throw, 5602, LV_MSIG(4, 33) },    /* set_scroll_throw */
  { (const void*) &lv_indev_set_type, 1308, LV_MSIG(4, 33) },    /* set_type */
  { (const void*) &lv_indev_set_user_data, 118, LV_MSIG(4, 30) },    /* set_user_data */
};

/* `lv_keyboard` methods */
#ifdef BE_LV_WIDGET_KEYBOARD
const lv_ntv_method_t lv_keyboard_func[] = {
  { (const void*) &lv_keyboard_get_button_text, 7367, LV_MSIG(0, 238) },    /* _btn_text */
  { (const void*) &lv_keyboard_get_button_text, 5904, LV_MSIG(1, 238) },    /* get_button_text */
  { (const void*) &lv_keyboard_get_map_array, 7717, LV_MSIG(1, 149) },    /* get_map_array */
  { (const void*) &lv_keyboard_get_mode, 4180, LV_MSIG(1, 165) },    /* get_mode */
  { (const void*) &lv_keyboard_get_popovers, 8177, LV_MSIG(1, 128) },    /* get_popovers */
  { (const void*) &lv_keyboard_get_selected_button, 3529, LV_MSIG(1, 165) },    /* get_selected_button */
  { (const void*) &lv_keyboard_get_textarea, 8303, LV_MSIG(1, 214) },    /* get_textarea */
  { (const void*) &lv_keyboard_set_mode, 4180, LV_MSIG(4, 81) },    /* set_mode */
  { (const void*) &lv_keyboard_set_popovers, 8177, LV_MSIG(4, 78) },    /* set_popovers */
  { (const void*) &lv_keyboard_set_textarea, 8303, LV_MSIG(4, 58) },    /* set_textarea */
};
#endif // BE_LV_WIDGET_KEYBOARD

/* `lv_label` methods */
#ifdef BE_LV_WIDGET_LABEL
const lv_ntv_method_t lv_label_func[] = {
  { (const void*) &lv_label_cut_text, 8069, LV_MSIG(0, 93) },    /* cut_text */
  { (const void*) &lv_label_ins_text, 8123, LV_MSIG(0, 101) },    /* ins_text */
  { (const void*) &lv_label_get_letter_on, 7697, LV_MSIG(1, 173) },    /* get_letter_on */
  { (const void*) &lv_label_get_letter_pos, 6905, LV_MSIG(1, 92) },    /* get_letter_pos */
  { (const void*) &lv_label_get_long_mode, 7707, LV_MSIG(1, 165) },    /* get_long_mode */
  { (const void*) &lv_label_get_max_lines, 7727, LV_MSIG(1, 165) },    /* get_max_lines */
  { (const void*) &lv_label_get_recolor, 1505, LV_MSIG(1, 128) },    /* get_recolor */
  { (const void*) &lv_label_get_text, 2835, LV_MSIG(1, 235) },    /* get_text */
  { (const void*) &lv_label_get_text_selection_end, 1952, LV_MSIG(1, 165) },    /* get_text_selection_end */
  { (const void*) &lv_label_get_text_selection_start, 1192, LV_MSIG(1, 165) },    /* get_text_selection_start */
  { (const void*) &lv_label_is_char_under_pos, 3855, LV_MSIG(3, 136) },    /* is_char_under_pos */
  { (const void*) &lv_label_set_long_mode, 7707, LV_MSIG(4, 81) },    /* set_long_mode */
  { (const void*) &lv_label_set_max_lines, 7727, LV_MSIG(4, 81) },    /* set_max_lines */
  { (const void*) &lv_label_set_recolor, 1505, LV_MSIG(4, 78) },    /* set_recolor */
  { (const void*) &lv_label_set_text, 2835, LV_MSIG(4, 102) },    /* set_text */
  { (const void*) &lv_label_set_text_fmt, 4899, LV_MSIG(4, 103) },    /* set_text_fmt */
  { (const void*) &lv_label_set_text_selection_end, 1952, LV_MSIG(4, 81) },    /* set_text_selection_end */
  { (const void*) &lv_label_set_text_selection_start, 1192, LV_MSIG(4, 81) },    /* set_text_selection_start */
  { (const void*) &lv_label_set_text_static, 2947, LV_MSIG(4, 102) },    /* set_text_static */
};
#endif // BE_LV_WIDGET_LABEL

/* `lv_led` methods */
#ifdef BE_LV_WIDGET_LED
const lv_ntv_method_t lv_led_func[] = {
  { (const void*) &lv_led_off, 8874, LV_MSIG(0, 34) },    /* off */
  { (const void*) &lv_led_on, 63, LV_MSIG(0, 34) },    /* on */
  { (const void*) &lv_led_toggle, 8707, LV_MSIG(0, 34) },    /* toggle */
  { (const void*) &lv_led_get_brightness, 6685, LV_MSIG(1, 165) },    /* get_brightness */
  { (const void*) &lv_led_get_color, 368, LV_MSIG(1, 189) },    /* get_color */
  { (const void*) &lv_led_set_brightness, 6685, LV_MSIG(4, 81) },    /* set_brightness */
  { (const void*) &lv_led_set_color, 368, LV_MSIG(4, 52) },    /* set_color */
};
#endif // BE_LV_WIDGET_LED

/* `lv_line` methods */
#ifdef BE_LV_WIDGET_LINE
const lv_ntv_method_t lv_line_func[] = {
  { (const void*) &lv_line_get_point_count, 6264, LV_MSIG(1, 165) },    /* get_point_count */
  { (const void*) &lv_line_get_points, 4383, LV_MSIG(1, 224) },    /* get_points */
  { (const void*) &lv_line_get_points_mutable, 4095, LV_MSIG(1, 224) },    /* get_points_mutable */
  { (const void*) &lv_line_get_y_invert, 8330, LV_MSIG(1, 128) },    /* get_y_invert */
  { (const void*) &lv_line_is_point_array_mutable, 1413, LV_MSIG(3, 128) },    /* is_point_array_mutable */
  { (const void*) &lv_line_set_points, 4383, LV_MSIG(4, 64) },    /* set_points */
  { (const void*) &lv_line_set_points_mutable, 4095, LV_MSIG(4, 64) },    /* set_points_mutable */
  { (const void*) &lv_line_set_y_invert, 8330, LV_MSIG(4, 78) },    /* set_y_invert */
};
#endif // BE_LV_WIDGET_LINE

/* `lv_list` methods */
#ifdef BE_LV_WIDGET_LIST
const lv_ntv_method_t lv_list_func[] = {
  { (const void*) &lv_list_get_button_text, 7367, LV_MSIG(0, 236) },    /* _btn_text */
  { (const void*) &lv_list_add_button, 8339, LV_MSIG(0, 216) },    /* add_btn */
  { (const void*) &lv_list_add_button, 6564, LV_MSIG(0, 216) },    /* add_button */
  { (const void*) &lv_list_add_text, 8024, LV_MSIG(0, 221) },    /* add_text */
  { (const void*) &lv_list_get_button_text, 5904, LV_MSIG(1, 236) },    /* get_button_text */
  { (const void*) &lv_list_set_button_text, 7368, LV_MSIG(4, 63) },    /* set_btn_text */
  { (const void*) &lv_list_set_button_text, 5904, LV_MSIG(4, 63) },    /* set_button_text */
};
#endif // BE_LV_WIDGET_LIST

/* `lv_menu` methods */
#ifdef BE_LV_WIDGET_MENU
const lv_ntv_method_t lv_menu_func[] = {
  { (const void*) &lv_menu_back_button_is_root, 1233, LV_MSIG(0, 132) },    /* back_button_is_root */
  { (const void*) &lv_menu_clear_history, 4418, LV_MSIG(0, 34) },    /* clear_history */
  { (const void*) &lv_menu_get_cur_main_page, 4460, LV_MSIG(1, 214) },    /* get_cur_main_page */
  { (const void*) &lv_menu_get_cur_sidebar_page, 2670, LV_MSIG(1, 214) },    /* get_cur_sidebar_page */
  { (const void*) &lv_menu_get_main_header, 6144, LV_MSIG(1, 214) },    /* get_main_header */
  { (const void*) &lv_menu_get_main_header_back_button, 523, LV_MSIG(1, 214) },    /* get_main_header_back_button */
  { (const void*) &lv_menu_get_mode_header, 6168, LV_MSIG(1, 165) },    /* get_mode_header */
  { (const void*) &lv_menu_get_mode_root_back_button, 935, LV_MSIG(1, 165) },    /* get_mode_root_back_button */
  { (const void*) &lv_menu_get_sidebar_header, 4245, LV_MSIG(1, 214) },    /* get_sidebar_header */
  { (const void*) &lv_menu_get_sidebar_header_back_button, 243, LV_MSIG(1, 214) },    /* get_sidebar_header_back_button */
  { (const void*) &lv_menu_set_load_page_event, 3305, LV_MSIG(4, 59) },    /* set_load_page_event */
  { (const void*) &lv_menu_set_mode_header, 6168, LV_MSIG(4, 81) },    /* set_mode_header */
  { (const void*) &lv_menu_set_mode_root_back_button, 935, LV_MSIG(4, 81) },    /* set_mode_root_back_button */
  { (const void*) &lv_menu_set_page, 2682, LV_MSIG(4, 58) },    /* set_page */
  { (const void*) &lv_menu_set_page_title, 7059, LV_MSIG(4, 79) },    /* set_page_title */
  { (const void*) &lv_menu_set_page_title_static, 2316, LV_MSIG(4, 79) },    /* set_page_title_static */
  { (const void*) &lv_menu_set_sidebar_page, 2674, LV_MSIG(4, 58) },    /* set_sidebar_page */
};
#endif // BE_LV_WIDGET_MENU

/* `lv_menu_cont` methods */
#ifdef BE_LV_WIDGET_MENU_CONT
const lv_ntv_method_t lv_menu_cont_func[] = {
};
#endif // BE_LV_WIDGET_MENU_CONT

/* `lv_menu_page` methods */
#ifdef BE_LV_WIDGET_MENU_PAGE
const lv_ntv_method_t lv_menu_page_func[] = {
};
#endif // BE_LV_WIDGET_MENU_PAGE

/* `lv_menu_section` methods */
#ifdef BE_LV_WIDGET_MENU_SECTION
const lv_ntv_method_t lv_menu_section_func[] = {
};
#endif // BE_LV_WIDGET_MENU_SECTION

/* `lv_menu_separator` methods */
#ifdef BE_LV_WIDGET_MENU_SEPARATOR
const lv_ntv_method_t lv_menu_separator_func[] = {
};
#endif // BE_LV_WIDGET_MENU_SEPARATOR

/* `lv_msgbox` methods */
#ifdef BE_LV_WIDGET_MSGBOX
const lv_ntv_method_t lv_msgbox_func[] = {
  { (const void*) &lv_msgbox_add_close_button, 2568, LV_MSIG(0, 214) },    /* add_close_button */
  { (const void*) &lv_msgbox_add_footer_button, 2028, LV_MSIG(0, 221) },    /* add_footer_button */
  { (const void*) &lv_msgbox_add_header_button, 2046, LV_MSIG(0, 215) },    /* add_header_button */
  { (const void*) &lv_msgbox_add_text, 8024, LV_MSIG(0, 221) },    /* add_text */
  { (const void*) &lv_msgbox_add_text_fmt, 4991, LV_MSIG(0, 222) },    /* add_text_fmt */
  { (const void*) &lv_msgbox_add_title, 7397, LV_MSIG(0, 221) },    /* add_title */
  { (const void*) &lv_msgbox_close, 8733, LV_MSIG(0, 34) },    /* close */
  { (const void*) &lv_msgbox_close_async, 5964, LV_MSIG(0, 34) },    /* close_async */
  { (const void*) &lv_msgbox_get_content, 442, LV_MSIG(1, 214) },    /* get_content */
  { (const void*) &lv_msgbox_get_footer, 8623, LV_MSIG(1, 214) },    /* get_footer */
  { (const void*) &lv_msgbox_get_header, 4253, LV_MSIG(1, 214) },    /* get_header */
  { (const void*) &lv_msgbox_get_title, 7064, LV_MSIG(1, 214) },    /* get_title */
};
#endif // BE_LV_WIDGET_MSGBOX

/* `lv_obj` methods */
#ifdef BE_LV_WIDGET_OBJ
const lv_ntv_method_t lv_obj_func[] = {
  { (const void*) &lv_obj_add_event_cb, 4978, LV_MSIG(0, 199) },    /* add_event_cb */
  { (const void*) &lv_obj_add_flag, 8006, LV_MSIG(0, 81) },    /* add_flag */
  { (const void*) &lv_obj_add_play_timeline_event, 475, LV_MSIG(0, 82) },    /* add_play_timeline_event */
  { (const void*) &lv_obj_add_screen_load_event, 803, LV_MSIG(0, 88) },    /* add_screen_load_event */
  { (const void*) &lv_obj_add_state, 7377, LV_MSIG(0, 81) },    /* add_state */
  { (const void*) &lv_obj_add_style, 7387, LV_MSIG(0, 74) },    /* add_style */
  { (const void*) &lv_obj_align, 237, LV_MSIG(0, 97) },    /* align */
  { (const void*) &lv_obj_align_to, 8042, LV_MSIG(0, 62) },    /* align_to */
  { (const void*) &lv_obj_area_is_visible, 3129, LV_MSIG(0, 129) },    /* area_is_visible */
  { (const void*) &lv_obj_calc_dynamic_height, 1273, LV_MSIG(0, 174) },    /* calc_dynamic_height */
  { (const void*) &lv_obj_calc_dynamic_width, 1610, LV_MSIG(0, 174) },    /* calc_dynamic_width */
  { (const void*) &lv_obj_calculate_ext_draw_size, 499, LV_MSIG(0, 174) },    /* calculate_ext_draw_size */
  { (const void*) &lv_obj_calculate_style_text_align, 216, LV_MSIG(0, 178) },    /* calculate_style_text_align */
  { (const void*) &lv_obj_center, 8609, LV_MSIG(0, 34) },    /* center */
  { (const void*) &lv_obj_check_type, 6707, LV_MSIG(0, 133) },    /* check_type */
  { (const void*) &lv_obj_class_init_obj, 3885, LV_MSIG(0, 34) },    /* class_init_obj */
  { (const void*) &lv_obj_clean, 8727, LV_MSIG(0, 34) },    /* clean */
  { (const void*) &lv_obj_remove_flag, 6718, LV_MSIG(0, 81) },    /* clear_flag */
  { (const void*) &lv_obj_remove_state, 5940, LV_MSIG(0, 81) },    /* clear_state */
  { (const void*) &lv_obj_delete, 8854, LV_MSIG(0, 34) },    /* del */
  { (const void*) &lv_obj_delete_async, 7567, LV_MSIG(0, 34) },    /* del_async */
  { (const void*) &lv_obj_delete, 4481, LV_MSIG(0, 34) },    /* delete */
  { (const void*) &lv_obj_delete_async, 5277, LV_MSIG(0, 34) },    /* delete_async */
  { (const void*) &lv_obj_delete_delayed, 3960, LV_MSIG(0, 81) },    /* delete_delayed */
  { (const void*) &lv_obj_dump_tree, 7577, LV_MSIG(0, 34) },    /* dump_tree */
  { (const void*) &lv_obj_fade_in, 8435, LV_MSIG(0, 93) },    /* fade_in */
  { (const void*) &lv_obj_fade_out, 8087, LV_MSIG(0, 93) },    /* fade_out */
  { (const void*) &lv_obj_find_by_id, 6784, LV_MSIG(0, 215) },    /* find_by_id */
  { (const void*) &lv_obj_free_id, 8459, LV_MSIG(0, 34) },    /* free_id */
  { (const void*) &lv_obj_has_class, 7647, LV_MSIG(0, 133) },    /* has_class */
  { (const void*) &lv_obj_has_flag, 8105, LV_MSIG(0, 137) },    /* has_flag */
  { (const void*) &lv_obj_has_flag_any, 5355, LV_MSIG(0, 137) },    /* has_flag_any */
  { (const void*) &lv_obj_has_state, 7657, LV_MSIG(0, 137) },    /* has_state */
  { (const void*) &lv_obj_has_style_prop, 4020, LV_MSIG(0, 138) },    /* has_style_prop */
  { (const void*) &lv_obj_hit_test, 8114, LV_MSIG(0, 136) },    /* hit_test */
  { (const void*) &lv_obj_init_draw_arc_dsc, 2262, LV_MSIG(0, 83) },    /* init_draw_arc_dsc */
  { (const void*) &lv_obj_init_draw_image_dsc, 1353, LV_MSIG(0, 84) },    /* init_draw_image_dsc */
  { (const void*) &lv_obj_init_draw_label_dsc, 1373, LV_MSIG(0, 85) },    /* init_draw_label_dsc */
  { (const void*) &lv_obj_init_draw_line_dsc, 1724, LV_MSIG(0, 86) },    /* init_draw_line_dsc */
  { (const void*) &lv_obj_init_draw_rect_dsc, 1743, LV_MSIG(0, 87) },    /* init_draw_rect_dsc */
  { (const void*) &lv_obj_invalidate, 1181, LV_MSIG(0, 165) },    /* invalidate */
  { (const void*) &lv_obj_invalidate_area, 3273, LV_MSIG(0, 166) },    /* invalidate_area */
  { (const void*) &lv_obj_mark_layout_as_dirty, 1150, LV_MSIG(0, 34) },    /* mark_layout_as_dirty */
  { (const void*) &lv_obj_move_background, 3369, LV_MSIG(0, 34) },    /* move_background */
  { (const void*) &lv_obj_move_children_by, 2772, LV_MSIG(0, 96) },    /* move_children_by */
  { (const void*) &lv_obj_move_foreground, 3385, LV_MSIG(0, 34) },    /* move_foreground */
  { (const void*) &lv_obj_move_to, 8467, LV_MSIG(0, 93) },    /* move_to */
  { (const void*) &lv_obj_move_to_index, 4600, LV_MSIG(0, 81) },    /* move_to_index */
  { (const void*) &lv_obj_readjust_scroll, 3465, LV_MSIG(0, 81) },    /* readjust_scroll */
  { (const void*) &lv_obj_refr_pos, 8195, LV_MSIG(0, 34) },    /* refr_pos */
  { (const void*) &lv_obj_refr_size, 7797, LV_MSIG(0, 128) },    /* refr_size */
  { (const void*) &lv_obj_refresh_ext_draw_size, 957, LV_MSIG(0, 34) },    /* refresh_ext_draw_size */
  { (const void*) &lv_obj_refresh_self_size, 2352, LV_MSIG(0, 128) },    /* refresh_self_size */
  { (const void*) &lv_obj_refresh_style, 4698, LV_MSIG(0, 93) },    /* refresh_style */
  { (const void*) &lv_obj_delete, 8672, LV_MSIG(0, 34) },    /* remove */
  { (const void*) &lv_obj_remove_event, 5459, LV_MSIG(0, 137) },    /* remove_event */
  { (const void*) &lv_obj_remove_event_cb, 3497, LV_MSIG(0, 170) },    /* remove_event_cb */
  { (const void*) &lv_obj_remove_event_cb_with_user_data, 97, LV_MSIG(0, 171) },    /* remove_event_cb_with_user_data */
  { (const void*) &lv_obj_remove_event_dsc, 2840, LV_MSIG(0, 131) },    /* remove_event_dsc */
  { (const void*) &lv_obj_remove_flag, 6288, LV_MSIG(0, 81) },    /* remove_flag */
  { (const void*) &lv_obj_remove_local_style_prop, 547, LV_MSIG(0, 138) },    /* remove_local_style_prop */
  { (const void*) &lv_obj_remove_state, 5472, LV_MSIG(0, 81) },    /* remove_state */
  { (const void*) &lv_obj_remove_style, 5485, LV_MSIG(0, 74) },    /* remove_style */
  { (const void*) &lv_obj_remove_style_all, 2857, LV_MSIG(0, 34) },    /* remove_style_all */
  { (const void*) &lv_obj_remove_theme, 5498, LV_MSIG(0, 81) },    /* remove_theme */
  { (const void*) &lv_obj_replace_style, 4740, LV_MSIG(0, 134) },    /* replace_style */
  { (const void*) &lv_obj_reset_transform, 3513, LV_MSIG(0, 34) },    /* reset_transform */
  { (const void*) &lv_obj_scroll_by, 7807, LV_MSIG(0, 97) },    /* scroll_by */
  { (const void*) &lv_obj_scroll_by_bounded, 2370, LV_MSIG(0, 97) },    /* scroll_by_bounded */
  { (const void*) &lv_obj_scroll_to, 7817, LV_MSIG(0, 97) },    /* scroll_to */
  { (const void*) &lv_obj_scroll_to_view, 4140, LV_MSIG(0, 81) },    /* scroll_to_view */
  { (const void*) &lv_obj_scroll_to_view_recursive, 400, LV_MSIG(0, 81) },    /* scroll_to_view_recursive */
  { (const void*) &lv_obj_scroll_to_x, 6372, LV_MSIG(0, 93) },    /* scroll_to_x */
  { (const void*) &lv_obj_scroll_to_y, 6384, LV_MSIG(0, 93) },    /* scroll_to_y */
  { (const void*) &lv_obj_scrollbar_invalidate, 1171, LV_MSIG(0, 34) },    /* scrollbar_invalidate */
  { (const void*) &lv_obj_send_event, 7191, LV_MSIG(0, 175) },    /* send_event */
  { (const void*) &lv_obj_stop_scroll_anim, 2959, LV_MSIG(0, 34) },    /* stop_scroll_anim */
  { (const void*) &lv_obj_stringify_id, 5706, LV_MSIG(0, 237) },    /* stringify_id */
  { (const void*) &lv_obj_style_apply_color_filter, 450, LV_MSIG(0, 177) },    /* style_apply_color_filter */
  { (const void*) &lv_obj_style_apply_recolor, 1493, LV_MSIG(0, 177) },    /* style_apply_recolor */
  { (const void*) &lv_obj_style_get_disabled, 1914, LV_MSIG(0, 135) },    /* style_get_disabled */
  { (const void*) &lv_obj_style_set_disabled, 1933, LV_MSIG(0, 75) },    /* style_set_disabled */
  { (const void*) &lv_obj_swap, 8835, LV_MSIG(0, 58) },    /* swap */
  { (const void*) &lv_obj_transform_point, 3641, LV_MSIG(0, 80) },    /* transform_point */
  { (const void*) &lv_obj_transform_point_array, 1023, LV_MSIG(0, 65) },    /* transform_point_array */
  { (const void*) &lv_obj_update_layout, 4950, LV_MSIG(0, 34) },    /* update_layout */
  { (const void*) &lv_obj_update_snap, 6552, LV_MSIG(0, 81) },    /* update_snap */
  { (const void*) &lv_obj_get_child, 8721, LV_MSIG(1, 218) },    /* get_child */
  { (const void*) &lv_obj_get_child_by_type, 4390, LV_MSIG(1, 219) },    /* get_child_by_type */
  { (const void*) &lv_obj_get_child_count, 7487, LV_MSIG(1, 165) },    /* get_child_cnt */
  { (const void*) &lv_obj_get_child_count, 5928, LV_MSIG(1, 165) },    /* get_child_count */
  { (const void*) &lv_obj_get_child_count_by_type, 1293, LV_MSIG(1, 169) },    /* get_child_count_by_type */
  { (const void*) &lv_obj_get_class, 7651, LV_MSIG(1, 223) },    /* get_class */
  { (const void*) &lv_obj_get_click_area, 3979, LV_MSIG(1, 35) },    /* get_click_area */
  { (const void*) &lv_obj_get_content_coords, 3900, LV_MSIG(1, 35) },    /* get_content_coords */
  { (const void*) &lv_obj_get_content_height, 3915, LV_MSIG(1, 165) },    /* get_content_height */
  { (const void*) &lv_obj_get_content_width, 4446, LV_MSIG(1, 165) },    /* get_content_width */
  { (const void*) &lv_obj_get_coords, 3908, LV_MSIG(1, 35) },    /* get_coords */
  { (const void*) &lv_obj_get_display, 8815, LV_MSIG(1, 194) },    /* get_disp */
  { (const void*) &lv_obj_get_display, 8403, LV_MSIG(1, 194) },    /* get_display */
  { (const void*) &lv_obj_get_event_count, 6072, LV_MSIG(1, 165) },    /* get_event_count */
  { (const void*) &lv_obj_get_event_dsc, 2847, LV_MSIG(1, 200) },    /* get_event_dsc */
  { (const void*) &lv_obj_get_group, 8751, LV_MSIG(1, 202) },    /* get_group */
  { (const void*) &lv_obj_get_height, 1286, LV_MSIG(1, 165) },    /* get_height */
  { (const void*) &lv_obj_get_id, 1892, LV_MSIG(1, 149) },    /* get_id */
  { (const void*) &lv_obj_get_index, 4608, LV_MSIG(1, 165) },    /* get_index */
  { (const void*) &lv_obj_get_index_by_type, 4558, LV_MSIG(1, 169) },    /* get_index_by_type */
  { (const void*) &lv_obj_get_parent, 8644, LV_MSIG(1, 214) },    /* get_parent */
  { (const void*) &lv_obj_get_screen, 8693, LV_MSIG(1, 214) },    /* get_screen */
  { (const void*) &lv_obj_get_scroll_bottom, 4782, LV_MSIG(1, 165) },    /* get_scroll_bottom */
  { (const void*) &lv_obj_get_scroll_dir, 7114, LV_MSIG(1, 165) },    /* get_scroll_dir */
  { (const void*) &lv_obj_get_scroll_end, 7125, LV_MSIG(1, 79) },    /* get_scroll_end */
  { (const void*) &lv_obj_get_scroll_left, 6360, LV_MSIG(1, 165) },    /* get_scroll_left */
  { (const void*) &lv_obj_get_scroll_right, 5589, LV_MSIG(1, 165) },    /* get_scroll_right */
  { (const void*) &lv_obj_get_scroll_snap_x, 4796, LV_MSIG(1, 165) },    /* get_scroll_snap_x */
  { (const void*) &lv_obj_get_scroll_snap_y, 4810, LV_MSIG(1, 165) },    /* get_scroll_snap_y */
  { (const void*) &lv_obj_get_scroll_top, 7147, LV_MSIG(1, 165) },    /* get_scroll_top */
  { (const void*) &lv_obj_get_scroll_x, 8222, LV_MSIG(1, 165) },    /* get_scroll_x */
  { (const void*) &lv_obj_get_scroll_y, 8231, LV_MSIG(1, 165) },    /* get_scroll_y */
  { (const void*) &lv_obj_get_scrollbar_area, 4155, LV_MSIG(1, 36) },    /* get_scrollbar_area */
  { (const void*) &lv_obj_get_scrollbar_mode, 4170, LV_MSIG(1, 165) },    /* get_scrollbar_mode */
  { (const void*) &lv_obj_get_self_height, 6396, LV_MSIG(1, 165) },    /* get_self_height */
  { (const void*) &lv_obj_get_self_width, 7180, LV_MSIG(1, 165) },    /* get_self_width */
  { (const void*) &lv_obj_get_sibling, 8555, LV_MSIG(1, 218) },    /* get_sibling */
  { (const void*) &lv_obj_get_sibling_by_type, 3577, LV_MSIG(1, 219) },    /* get_sibling_by_type */
  { (const void*) &lv_obj_get_state, 5479, LV_MSIG(1, 165) },    /* get_state */
  { (const void*) &lv_obj_get_transformed_area, 3078, LV_MSIG(1, 37) },    /* get_transformed_area */
  { (const void*) &lv_obj_get_user_data, 118, LV_MSIG(1, 149) },    /* get_user_data */
  { (const void*) &lv_obj_get_user_flag, 7947, LV_MSIG(1, 137) },    /* get_user_flag */
  { (const void*) &lv_obj_get_width, 394, LV_MSIG(1, 165) },    /* get_width */
  { (const void*) &lv_obj_get_x, 2476, LV_MSIG(1, 165) },    /* get_x */
  { (const void*) &lv_obj_get_x2, 8886, LV_MSIG(1, 165) },    /* get_x2 */
  { (const void*) &lv_obj_get_x_aligned, 7977, LV_MSIG(1, 165) },    /* get_x_aligned */
  { (const void*) &lv_obj_get_y, 911, LV_MSIG(1, 165) },    /* get_y */
  { (const void*) &lv_obj_get_y2, 8889, LV_MSIG(1, 165) },    /* get_y2 */
  { (const void*) &lv_obj_get_y_aligned, 7987, LV_MSIG(1, 165) },    /* get_y_aligned */
  { (const void*) &lv_obj_get_style_arc_color_filtered, 1572, LV_MSIG(2, 191) },    /* get_style_arc_color_filtered */
  { (const void*) &lv_obj_get_style_bg_color_filtered, 2064, LV_MSIG(2, 191) },    /* get_style_bg_color_filtered */
  { (const void*) &lv_obj_get_style_bg_grad_color_filtered, 643, LV_MSIG(2, 191) },    /* get_style_bg_grad_color_filtered */
  { (const void*) &lv_obj_get_style_bg_image_recolor_filtered, 270, LV_MSIG(2, 191) },    /* get_style_bg_image_recolor_filtered */
  { (const void*) &lv_obj_get_style_border_color_filtered, 825, LV_MSIG(2, 191) },    /* get_style_border_color_filtered */
  { (const void*) &lv_obj_get_style_clamped_height, 3870, LV_MSIG(2, 165) },    /* get_style_clamped_height */
  { (const void*) &lv_obj_get_style_clamped_width, 4404, LV_MSIG(2, 165) },    /* get_style_clamped_width */
  { (const void*) &lv_obj_get_style_enabled, 1142, LV_MSIG(2, 135) },    /* get_style_enabled */
  { (const void*) &lv_obj_get_style_image_recolor_filtered, 273, LV_MSIG(2, 191) },    /* get_style_image_recolor_filtered */
  { (const void*) &lv_obj_get_style_image_recolor_filtered, 1108, LV_MSIG(2, 191) },    /* get_style_img_recolor_filtered */
  { (const void*) &lv_obj_get_style_line_color_filtered, 692, LV_MSIG(2, 191) },    /* get_style_line_color_filtered */
  { (const void*) &lv_obj_get_style_opa_recursive, 4614, LV_MSIG(2, 174) },    /* get_style_opa_recursive */
  { (const void*) &lv_obj_get_style_outline_color_filtered, 689, LV_MSIG(2, 191) },    /* get_style_outline_color_filtered */
  { (const void*) &lv_obj_get_style_prop, 566, LV_MSIG(2, 177) },    /* get_style_prop */
  { (const void*) &lv_obj_get_style_recolor_recursive, 2334, LV_MSIG(2, 174) },    /* get_style_recolor_recursive */
  { (const void*) &lv_obj_get_style_shadow_color_filtered, 979, LV_MSIG(2, 191) },    /* get_style_shadow_color_filtered */
  { (const void*) &lv_obj_get_style_space_bottom, 5693, LV_MSIG(2, 174) },    /* get_style_space_bottom */
  { (const void*) &lv_obj_get_style_space_left, 7224, LV_MSIG(2, 174) },    /* get_style_space_left */
  { (const void*) &lv_obj_get_style_space_right, 6432, LV_MSIG(2, 174) },    /* get_style_space_right */
  { (const void*) &lv_obj_get_style_space_top, 7847, LV_MSIG(2, 174) },    /* get_style_space_top */
  { (const void*) &lv_obj_get_style_text_color_filtered, 1513, LV_MSIG(2, 191) },    /* get_style_text_color_filtered */
  { (const void*) &lv_obj_get_style_text_outline_stroke_color_filtered, 0, LV_MSIG(2, 191) },    /* get_style_text_outline_stroke_color_filtered */
  { (const void*) &lv_obj_get_style_transform_scale_x_safe, 712, LV_MSIG(2, 174) },    /* get_style_transform_scale_x_safe */
  { (const void*) &lv_obj_get_style_transform_scale_y_safe, 735, LV_MSIG(2, 174) },    /* get_style_transform_scale_y_safe */
  { (const void*) &lv_obj_is_checkable, 7477, LV_MSIG(3, 128) },    /* is_checkable */
  { (const void*) &lv_obj_is_checked, 6208, LV_MSIG(3, 128) },    /* is_checked */
  { (const void*) &lv_obj_is_clickable, 7507, LV_MSIG(3, 128) },    /* is_clickable */
  { (const void*) &lv_obj_is_disabled, 1924, LV_MSIG(3, 128) },    /* is_disabled */
  { (const void*) &lv_obj_is_editable, 8078, LV_MSIG(3, 128) },    /* is_editable */
  { (const void*) &lv_obj_is_flex_in_new_track, 2154, LV_MSIG(3, 128) },    /* is_flex_in_new_track */
  { (const void*) &lv_obj_is_group_def, 7637, LV_MSIG(3, 128) },    /* is_group_def */
  { (const void*) &lv_obj_is_height_max, 6839, LV_MSIG(3, 128) },    /* is_height_max */
  { (const void*) &lv_obj_is_height_min, 6850, LV_MSIG(3, 128) },    /* is_height_min */
  { (const void*) &lv_obj_is_hidden, 8630, LV_MSIG(3, 128) },    /* is_hidden */
  { (const void*) &lv_obj_is_in_widget_tree, 4050, LV_MSIG(3, 128) },    /* is_in_widget_tree */
  { (const void*) &lv_obj_is_layout_positioned, 2280, LV_MSIG(3, 128) },    /* is_layout_positioned */
  { (const void*) &lv_obj_is_radio_button, 5446, LV_MSIG(3, 128) },    /* is_radio_button */
  { (const void*) &lv_obj_is_scrollable, 7158, LV_MSIG(3, 128) },    /* is_scrollable */
  { (const void*) &lv_obj_is_scrolling, 7827, LV_MSIG(3, 128) },    /* is_scrolling */
  { (const void*) &lv_obj_is_style_any_height_content, 425, LV_MSIG(3, 128) },    /* is_style_any_height_content */
  { (const void*) &lv_obj_is_style_any_width_content, 595, LV_MSIG(3, 128) },    /* is_style_any_width_content */
  { (const void*) &lv_obj_is_visible, 3137, LV_MSIG(3, 128) },    /* is_visible */
  { (const void*) &lv_obj_is_width_max, 7957, LV_MSIG(3, 128) },    /* is_width_max */
  { (const void*) &lv_obj_is_width_min, 7967, LV_MSIG(3, 128) },    /* is_width_min */
  { (const void*) &lv_obj_set_align, 237, LV_MSIG(4, 81) },    /* set_align */
  { (const void*) &lv_obj_set_checkable, 7477, LV_MSIG(4, 78) },    /* set_checkable */
  { (const void*) &lv_obj_set_checked, 6208, LV_MSIG(4, 78) },    /* set_checked */
  { (const void*) &lv_obj_set_clickable, 7507, LV_MSIG(4, 78) },    /* set_clickable */
  { (const void*) &lv_obj_set_content_height, 3915, LV_MSIG(4, 81) },    /* set_content_height */
  { (const void*) &lv_obj_set_content_width, 4446, LV_MSIG(4, 81) },    /* set_content_width */
  { (const void*) &lv_obj_set_disabled, 1924, LV_MSIG(4, 78) },    /* set_disabled */
  { (const void*) &lv_obj_set_ext_click_area, 3975, LV_MSIG(4, 81) },    /* set_ext_click_area */
  { (const void*) &lv_obj_set_ext_draw_size, 509, LV_MSIG(4, 81) },    /* set_ext_draw_size */
  { (const void*) &lv_obj_set_flag, 6295, LV_MSIG(4, 91) },    /* set_flag */
  { (const void*) &lv_obj_set_flex_align, 6795, LV_MSIG(4, 97) },    /* set_flex_align */
  { (const void*) &lv_obj_set_flex_flow, 7587, LV_MSIG(4, 81) },    /* set_flex_flow */
  { (const void*) &lv_obj_set_flex_grow, 7597, LV_MSIG(4, 81) },    /* set_flex_grow */
  { (const void*) &lv_obj_set_flex_in_new_track, 2154, LV_MSIG(4, 78) },    /* set_flex_in_new_track */
  { (const void*) &lv_obj_set_grid_align, 6828, LV_MSIG(4, 93) },    /* set_grid_align */
  { (const void*) &lv_obj_set_grid_cell, 7627, LV_MSIG(4, 98) },    /* set_grid_cell */
  { (const void*) &lv_obj_set_grid_dsc_array, 3990, LV_MSIG(4, 56) },    /* set_grid_dsc_array */
  { (const void*) &lv_obj_set_height, 1286, LV_MSIG(4, 81) },    /* set_height */
  { (const void*) &lv_obj_set_hidden, 8630, LV_MSIG(4, 78) },    /* set_hidden */
  { (const void*) &lv_obj_set_id, 1892, LV_MSIG(4, 76) },    /* set_id */
  { (const void*) &lv_obj_set_layout, 4957, LV_MSIG(4, 81) },    /* set_layout */
  { (const void*) &lv_obj_set_local_style_prop, 554, LV_MSIG(4, 97) },    /* set_local_style_prop */
  { (const void*) &lv_obj_set_parent, 8644, LV_MSIG(4, 58) },    /* set_parent */
  { (const void*) &lv_obj_set_pos, 1104, LV_MSIG(4, 93) },    /* set_pos */
  { (const void*) &lv_obj_set_radio_button, 5446, LV_MSIG(4, 78) },    /* set_radio_button */
  { (const void*) &lv_obj_set_scroll_dir, 7114, LV_MSIG(4, 81) },    /* set_scroll_dir */
  { (const void*) &lv_obj_set_scroll_snap_x, 4796, LV_MSIG(4, 81) },    /* set_scroll_snap_x */
  { (const void*) &lv_obj_set_scroll_snap_y, 4810, LV_MSIG(4, 81) },    /* set_scroll_snap_y */
  { (const void*) &lv_obj_set_scrollable, 7158, LV_MSIG(4, 78) },    /* set_scrollable */
  { (const void*) &lv_obj_set_scrollbar_mode, 4170, LV_MSIG(4, 81) },    /* set_scrollbar_mode */
  { (const void*) &lv_obj_set_size, 343, LV_MSIG(4, 93) },    /* set_size */
  { (const void*) &lv_obj_set_state, 5479, LV_MSIG(4, 91) },    /* set_state */
  { (const void*) &lv_obj_set_user_data, 118, LV_MSIG(4, 76) },    /* set_user_data */
  { (const void*) &lv_obj_set_user_flag, 7947, LV_MSIG(4, 91) },    /* set_user_flag */
  { (const void*) &lv_obj_set_width, 394, LV_MSIG(4, 81) },    /* set_width */
  { (const void*) &lv_obj_set_x, 2476, LV_MSIG(4, 81) },    /* set_x */
  { (const void*) &lv_obj_set_y, 911, LV_MSIG(4, 81) },    /* set_y */
  { (const void*) &lv_obj_set_style_enabled, 1142, LV_MSIG(5, 75) },    /* set_style_enabled */
  { (const void*) &lv_obj_set_style_margin_all, 6916, LV_MSIG(5, 93) },    /* set_style_margin_all */
  { (const void*) &lv_obj_set_style_margin_hor, 6927, LV_MSIG(5, 93) },    /* set_style_margin_hor */
  { (const void*) &lv_obj_set_style_margin_ver, 6949, LV_MSIG(5, 93) },    /* set_style_margin_ver */
  { (const void*) &lv_obj_set_style_pad_all, 8475, LV_MSIG(5, 93) },    /* set_style_pad_all */
  { (const void*) &lv_obj_set_style_pad_gap, 8483, LV_MSIG(5, 93) },    /* set_style_pad_gap */
  { (const void*) &lv_obj_set_style_pad_hor, 8491, LV_MSIG(5, 93) },    /* set_style_pad_hor */
  { (const void*) &lv_obj_set_style_pad_ver, 8515, LV_MSIG(5, 93) },    /* set_style_pad_ver */
  { (const void*) &lv_obj_set_style_size, 343, LV_MSIG(5, 97) },    /* set_style_size */
  { (const void*) &lv_obj_set_style_text_font, 7927, LV_MSIG(5, 54) },    /* set_style_text_font */
  { (const void*) &lv_obj_set_style_transform_scale, 3657, LV_MSIG(5, 93) },    /* set_style_transform_scale */
  { (const void*) &lv_obj_set_style_transform_scale, 4305, LV_MSIG(5, 93) },    /* set_style_transform_zoom */
};
#endif // BE_LV_WIDGET_OBJ

/* `lv_qrcode` methods */
#ifdef BE_LV_WIDGET_QRCODE
const lv_ntv_method_t lv_qrcode_func[] = {
  { (const void*) &lv_qrcode_render, 8679, LV_MSIG(0, 165) },    /* render */
  { (const void*) &lv_qrcode_update, 8714, LV_MSIG(0, 172) },    /* update */
  { (const void*) &lv_qrcode_get_update_mode, 6540, LV_MSIG(1, 165) },    /* get_update_mode */
  { (const void*) &lv_qrcode_is_render_valid, 5511, LV_MSIG(3, 128) },    /* is_render_valid */
  { (const void*) &lv_qrcode_set_dark_color, 6773, LV_MSIG(4, 52) },    /* set_dark_color */
  { (const void*) &lv_qrcode_set_data, 123, LV_MSIG(4, 102) },    /* set_data */
  { (const void*) &lv_qrcode_set_light_color, 6132, LV_MSIG(4, 52) },    /* set_light_color */
  { (const void*) &lv_qrcode_set_quiet_zone, 7081, LV_MSIG(4, 78) },    /* set_quiet_zone */
  { (const void*) &lv_qrcode_set_size, 343, LV_MSIG(4, 81) },    /* set_size */
  { (const void*) &lv_qrcode_set_update_mode, 6540, LV_MSIG(4, 81) },    /* set_update_mode */
};
#endif // BE_LV_WIDGET_QRCODE

/* `lv_roller` methods */
#ifdef BE_LV_WIDGET_ROLLER
const lv_ntv_method_t lv_roller_func[] = {
  { (const void*) &lv_roller_get_option_count, 7004, LV_MSIG(1, 165) },    /* get_option_cnt */
  { (const void*) &lv_roller_get_option_count, 5420, LV_MSIG(1, 165) },    /* get_option_count */
  { (const void*) &lv_roller_get_option_str, 7015, LV_MSIG(1, 176) },    /* get_option_str */
  { (const void*) &lv_roller_get_options, 4438, LV_MSIG(1, 235) },    /* get_options */
  { (const void*) &lv_roller_get_selected, 3001, LV_MSIG(1, 165) },    /* get_selected */
  { (const void*) &lv_roller_get_selected_str, 5615, LV_MSIG(1, 80) },    /* get_selected_str */
  { (const void*) &lv_roller_set_options, 4438, LV_MSIG(4, 104) },    /* set_options */
  { (const void*) &lv_roller_set_selected, 3001, LV_MSIG(4, 93) },    /* set_selected */
  { (const void*) &lv_roller_set_selected_str, 5615, LV_MSIG(4, 140) },    /* set_selected_str */
  { (const void*) &lv_roller_set_visible_row_count, 3689, LV_MSIG(4, 81) },    /* set_visible_row_cnt */
  { (const void*) &lv_roller_set_visible_row_count, 2550, LV_MSIG(4, 81) },    /* set_visible_row_count */
};
#endif // BE_LV_WIDGET_ROLLER

/* `lv_scale` methods */
#ifdef BE_LV_WIDGET_SCALE
const lv_ntv_method_t lv_scale_func[] = {
  { (const void*) &lv_scale_add_section, 5784, LV_MSIG(0, 225) },    /* add_section */
  { (const void*) &lv_scale_get_rotation, 1447, LV_MSIG(1, 165) },    /* get_angle */
  { (const void*) &lv_scale_get_angle_range, 5796, LV_MSIG(1, 165) },    /* get_angle_range */
  { (const void*) &lv_scale_get_label_show, 6883, LV_MSIG(1, 128) },    /* get_label_show */
  { (const void*) &lv_scale_get_major_tick_every, 2755, LV_MSIG(1, 165) },    /* get_major_tick_every */
  { (const void*) &lv_scale_get_mode, 4180, LV_MSIG(1, 165) },    /* get_mode */
  { (const void*) &lv_scale_get_range_max_value, 3433, LV_MSIG(1, 165) },    /* get_range_max_value */
  { (const void*) &lv_scale_get_range_min_value, 3449, LV_MSIG(1, 165) },    /* get_range_min_value */
  { (const void*) &lv_scale_get_rotation, 1981, LV_MSIG(1, 165) },    /* get_rotation */
  { (const void*) &lv_scale_get_total_tick_count, 3010, LV_MSIG(1, 165) },    /* get_total_tick_count */
  { (const void*) &lv_scale_set_rotation, 1447, LV_MSIG(4, 81) },    /* set_angle */
  { (const void*) &lv_scale_set_angle_range, 5796, LV_MSIG(4, 81) },    /* set_angle_range */
  { (const void*) &lv_scale_set_draw_ticks_on_top, 2136, LV_MSIG(4, 78) },    /* set_draw_ticks_on_top */
  { (const void*) &lv_scale_set_image_needle_value, 1705, LV_MSIG(4, 60) },    /* set_image_needle_value */
  { (const void*) &lv_scale_set_label_show, 6883, LV_MSIG(4, 78) },    /* set_label_show */
  { (const void*) &lv_scale_set_line_needle_value, 2298, LV_MSIG(4, 61) },    /* set_line_needle_value */
  { (const void*) &lv_scale_set_major_tick_every, 2755, LV_MSIG(4, 81) },    /* set_major_tick_every */
  { (const void*) &lv_scale_set_max_value, 2396, LV_MSIG(4, 81) },    /* set_max_value */
  { (const void*) &lv_scale_set_min_value, 2414, LV_MSIG(4, 81) },    /* set_min_value */
  { (const void*) &lv_scale_set_mode, 4180, LV_MSIG(4, 81) },    /* set_mode */
  { (const void*) &lv_scale_set_post_draw, 7787, LV_MSIG(4, 78) },    /* set_post_draw */
  { (const void*) &lv_scale_set_range, 4832, LV_MSIG(4, 93) },    /* set_range */
  { (const void*) &lv_scale_set_rotation, 1981, LV_MSIG(4, 81) },    /* set_rotation */
  { (const void*) &lv_scale_set_section_max_value, 2388, LV_MSIG(4, 67) },    /* set_section_max_value */
  { (const void*) &lv_scale_set_section_min_value, 2406, LV_MSIG(4, 67) },    /* set_section_min_value */
  { (const void*) &lv_scale_set_section_range, 4824, LV_MSIG(4, 68) },    /* set_section_range */
  { (const void*) &lv_scale_set_section_style_indicator, 571, LV_MSIG(4, 66) },    /* set_section_style_indicator */
  { (const void*) &lv_scale_set_section_style_items, 1453, LV_MSIG(4, 66) },    /* set_section_style_items */
  { (const void*) &lv_scale_set_section_style_main, 1800, LV_MSIG(4, 66) },    /* set_section_style_main */
  { (const void*) &lv_scale_set_text_src, 8294, LV_MSIG(4, 73) },    /* set_text_src */
  { (const void*) &lv_scale_set_total_tick_count, 3010, LV_MSIG(4, 81) },    /* set_total_tick_count */
};
#endif // BE_LV_WIDGET_SCALE

/* `lv_scale_section` methods */
#ifdef BE_LV_WIDGET_SCALE_SECTION
const lv_ntv_method_t lv_scale_section_func[] = {
  { (const void*) &lv_scale_section_set_range, 4832, LV_MSIG(4, 106) },    /* set_range */
  { (const void*) &lv_scale_section_set_style, 4706, LV_MSIG(4, 105) },    /* set_style */
};
#endif // BE_LV_WIDGET_SCALE_SECTION

/* `lv_slider` methods */
#ifdef BE_LV_WIDGET_SLIDER
const lv_ntv_method_t lv_slider_func[] = {
  { (const void*) &lv_slider_get_left_value, 6894, LV_MSIG(1, 165) },    /* get_left_value */
  { (const void*) &lv_slider_get_max_value, 2396, LV_MSIG(1, 165) },    /* get_max_value */
  { (const void*) &lv_slider_get_min_value, 2414, LV_MSIG(1, 165) },    /* get_min_value */
  { (const void*) &lv_slider_get_mode, 4180, LV_MSIG(1, 165) },    /* get_mode */
  { (const void*) &lv_slider_get_orientation, 6228, LV_MSIG(1, 165) },    /* get_orientation */
  { (const void*) &lv_slider_get_value, 1718, LV_MSIG(1, 165) },    /* get_value */
  { (const void*) &lv_slider_is_dragged, 8411, LV_MSIG(3, 128) },    /* is_dragged */
  { (const void*) &lv_slider_is_symmetrical, 6480, LV_MSIG(3, 128) },    /* is_symmetrical */
  { (const void*) &lv_slider_set_max_value, 2396, LV_MSIG(4, 81) },    /* set_max_value */
  { (const void*) &lv_slider_set_min_value, 2414, LV_MSIG(4, 81) },    /* set_min_value */
  { (const void*) &lv_slider_set_mode, 4180, LV_MSIG(4, 81) },    /* set_mode */
  { (const void*) &lv_slider_set_orientation, 6228, LV_MSIG(4, 81) },    /* set_orientation */
  { (const void*) &lv_slider_set_range, 4832, LV_MSIG(4, 93) },    /* set_range */
  { (const void*) &lv_slider_set_start_value, 6468, LV_MSIG(4, 93) },    /* set_start_value */
  { (const void*) &lv_slider_set_value, 1718, LV_MSIG(4, 93) },    /* set_value */
};
#endif // BE_LV_WIDGET_SLIDER

/* `lv_span` methods */
#ifdef BE_LV_WIDGET_SPAN
const lv_ntv_method_t lv_span_func[] = {
  { (const void*) &lv_span_get_style, 4706, LV_MSIG(1, 230) },    /* get_style */
  { (const void*) &lv_span_get_text, 2835, LV_MSIG(1, 240) },    /* get_text */
  { (const void*) &lv_span_set_text, 2835, LV_MSIG(4, 107) },    /* set_text */
  { (const void*) &lv_span_set_text_fmt, 4899, LV_MSIG(4, 108) },    /* set_text_fmt */
  { (const void*) &lv_span_set_text_static, 2947, LV_MSIG(4, 107) },    /* set_text_static */
};
#endif // BE_LV_WIDGET_SPAN

/* `lv_spangroup` methods */
#ifdef BE_LV_WIDGET_SPANGROUP
const lv_ntv_method_t lv_spangroup_func[] = {
  { (const void*) &lv_spangroup_add_span, 8015, LV_MSIG(0, 226) },    /* add_span */
  { (const void*) &lv_spangroup_delete_span, 6012, LV_MSIG(0, 69) },    /* delete_span */
  { (const void*) &lv_spangroup_refresh, 8539, LV_MSIG(0, 34) },    /* refresh */
  { (const void*) &lv_spangroup_get_align, 237, LV_MSIG(1, 165) },    /* get_align */
  { (const void*) &lv_spangroup_get_child, 8721, LV_MSIG(1, 228) },    /* get_child */
  { (const void*) &lv_spangroup_get_expand_height, 4502, LV_MSIG(1, 174) },    /* get_expand_height */
  { (const void*) &lv_spangroup_get_expand_width, 5303, LV_MSIG(1, 174) },    /* get_expand_width */
  { (const void*) &lv_spangroup_get_indent, 8637, LV_MSIG(1, 165) },    /* get_indent */
  { (const void*) &lv_spangroup_get_max_line_height, 3353, LV_MSIG(1, 165) },    /* get_max_line_height */
  { (const void*) &lv_spangroup_get_max_lines, 7727, LV_MSIG(1, 165) },    /* get_max_lines */
  { (const void*) &lv_spangroup_get_mode, 4180, LV_MSIG(1, 165) },    /* get_mode */
  { (const void*) &lv_spangroup_get_overflow, 8150, LV_MSIG(1, 165) },    /* get_overflow */
  { (const void*) &lv_spangroup_get_span_by_point, 4880, LV_MSIG(1, 227) },    /* get_span_by_point */
  { (const void*) &lv_spangroup_get_span_coords, 6444, LV_MSIG(1, 229) },    /* get_span_coords */
  { (const void*) &lv_spangroup_get_span_count, 7235, LV_MSIG(1, 165) },    /* get_span_count */
  { (const void*) &lv_spangroup_set_align, 237, LV_MSIG(4, 81) },    /* set_align */
  { (const void*) &lv_spangroup_set_indent, 8637, LV_MSIG(4, 81) },    /* set_indent */
  { (const void*) &lv_spangroup_set_max_lines, 7727, LV_MSIG(4, 81) },    /* set_max_lines */
  { (const void*) &lv_spangroup_set_mode, 4180, LV_MSIG(4, 81) },    /* set_mode */
  { (const void*) &lv_spangroup_set_overflow, 8150, LV_MSIG(4, 81) },    /* set_overflow */
  { (const void*) &lv_spangroup_set_span_style, 7246, LV_MSIG(4, 70) },    /* set_span_style */
  { (const void*) &lv_spangroup_set_span_text, 7857, LV_MSIG(4, 71) },    /* set_span_text */
  { (const void*) &lv_spangroup_set_span_text_fmt, 4894, LV_MSIG(4, 72) },    /* set_span_text_fmt */
  { (const void*) &lv_spangroup_set_span_text_static, 2942, LV_MSIG(4, 71) },    /* set_span_text_static */
};
#endif // BE_LV_WIDGET_SPANGROUP

/* `lv_spinbox` methods */
#ifdef BE_LV_WIDGET_SPINBOX
const lv_ntv_method_t lv_spinbox_func[] = {
  { (const void*) &lv_spinbox_decrement, 7557, LV_MSIG(0, 34) },    /* decrement */
  { (const void*) &lv_spinbox_increment, 7667, LV_MSIG(0, 34) },    /* increment */
  { (const void*) &lv_spinbox_step_next, 7897, LV_MSIG(0, 34) },    /* step_next */
  { (const void*) &lv_spinbox_step_prev, 7907, LV_MSIG(0, 34) },    /* step_prev */
  { (const void*) &lv_spinbox_get_dec_point_pos, 4488, LV_MSIG(1, 165) },    /* get_dec_point_pos */
  { (const void*) &lv_spinbox_get_digit_count, 6024, LV_MSIG(1, 165) },    /* get_digit_count */
  { (const void*) &lv_spinbox_get_digit_step_direction, 1066, LV_MSIG(1, 165) },    /* get_digit_step_direction */
  { (const void*) &lv_spinbox_get_max_value, 2396, LV_MSIG(1, 165) },    /* get_max_value */
  { (const void*) &lv_spinbox_get_min_value, 2414, LV_MSIG(1, 165) },    /* get_min_value */
  { (const void*) &lv_spinbox_get_rollover, 8204, LV_MSIG(1, 128) },    /* get_rollover */
  { (const void*) &lv_spinbox_get_step, 8830, LV_MSIG(1, 165) },    /* get_step */
  { (const void*) &lv_spinbox_get_value, 1718, LV_MSIG(1, 165) },    /* get_value */
  { (const void*) &lv_spinbox_set_cursor_pos, 6751, LV_MSIG(4, 81) },    /* set_cursor_pos */
  { (const void*) &lv_spinbox_set_dec_point_pos, 4488, LV_MSIG(4, 81) },    /* set_dec_point_pos */
  { (const void*) &lv_spinbox_set_digit_count, 6024, LV_MSIG(4, 81) },    /* set_digit_count */
  { (const void*) &lv_spinbox_set_digit_format, 5290, LV_MSIG(4, 93) },    /* set_digit_format */
  { (const void*) &lv_spinbox_set_digit_step_direction, 1066, LV_MSIG(4, 81) },    /* set_digit_step_direction */
  { (const void*) &lv_spinbox_set_max_value, 2396, LV_MSIG(4, 81) },    /* set_max_value */
  { (const void*) &lv_spinbox_set_min_value, 2414, LV_MSIG(4, 81) },    /* set_min_value */
  { (const void*) &lv_spinbox_set_range, 4832, LV_MSIG(4, 93) },    /* set_range */
  { (const void*) &lv_spinbox_set_rollover, 8204, LV_MSIG(4, 78) },    /* set_rollover */
  { (const void*) &lv_spinbox_set_step, 8830, LV_MSIG(4, 81) },    /* set_step */
  { (const void*) &lv_spinbox_set_value, 1718, LV_MSIG(4, 81) },    /* set_value */
};
#endif // BE_LV_WIDGET_SPINBOX

/* `lv_spinner` methods */
#ifdef BE_LV_WIDGET_SPINNER
const lv_ntv_method_t lv_spinner_func[] = {
  { (const void*) &lv_spinner_get_anim_duration, 4320, LV_MSIG(1, 165) },    /* get_anim_duration */
  { (const void*) &lv_spinner_get_arc_sweep, 7437, LV_MSIG(1, 165) },    /* get_arc_sweep */
  { (const void*) &lv_spinner_set_anim_duration, 4320, LV_MSIG(4, 81) },    /* set_anim_duration */
  { (const void*) &lv_spinner_set_anim_params, 5808, LV_MSIG(4, 93) },    /* set_anim_params */
  { (const void*) &lv_spinner_set_anim_duration, 7407, LV_MSIG(4, 81) },    /* set_anim_time */
  { (const void*) &lv_spinner_set_arc_sweep, 7437, LV_MSIG(4, 81) },    /* set_arc_sweep */
};
#endif // BE_LV_WIDGET_SPINNER

/* `lv_stripes` methods */
#ifdef BE_LV_WIDGET_STRIPES
const lv_ntv_method_t lv_stripes_func[] = {
  { (const void*) &lv_stripes_get_angle, 1447, LV_MSIG(1, 165) },    /* get_angle */
  { (const void*) &lv_stripes_get_gap_width, 7617, LV_MSIG(1, 165) },    /* get_gap_width */
  { (const void*) &lv_stripes_get_offset, 315, LV_MSIG(1, 165) },    /* get_offset */
  { (const void*) &lv_stripes_get_stripe_color, 5719, LV_MSIG(1, 189) },    /* get_stripe_color */
  { (const void*) &lv_stripes_get_stripe_opa, 7268, LV_MSIG(1, 165) },    /* get_stripe_opa */
  { (const void*) &lv_stripes_get_stripe_width, 5732, LV_MSIG(1, 165) },    /* get_stripe_width */
  { (const void*) &lv_stripes_set_angle, 1447, LV_MSIG(4, 81) },    /* set_angle */
  { (const void*) &lv_stripes_set_gap_width, 7617, LV_MSIG(4, 81) },    /* set_gap_width */
  { (const void*) &lv_stripes_set_offset, 315, LV_MSIG(4, 81) },    /* set_offset */
  { (const void*) &lv_stripes_set_stripe_color, 5719, LV_MSIG(4, 52) },    /* set_stripe_color */
  { (const void*) &lv_stripes_set_stripe_opa, 7268, LV_MSIG(4, 81) },    /* set_stripe_opa */
  { (const void*) &lv_stripes_set_stripe_width, 5732, LV_MSIG(4, 81) },    /* set_stripe_width */
};
#endif // BE_LV_WIDGET_STRIPES

/* `lv_style` methods */
const lv_ntv_method_t lv_style_func[] = {
  { (const void*) &lv_style_copy, 8810, LV_MSIG(0, 111) },    /* copy */
  { (const void*) &lv_style_merge, 8775, LV_MSIG(0, 111) },    /* merge */
  { (const void*) &lv_style_remove_prop, 6300, LV_MSIG(0, 142) },    /* remove_prop */
  { (const void*) &lv_style_reset, 8799, LV_MSIG(0, 109) },    /* reset */
  { (const void*) &lv_style_is_const, 8739, LV_MSIG(3, 141) },    /* is_const */
  { (const void*) &lv_style_is_empty, 8745, LV_MSIG(3, 141) },    /* is_empty */
  { (const void*) &lv_style_set_margin_all, 6916, LV_MSIG(4, 112) },    /* set_margin_all */
  { (const void*) &lv_style_set_margin_hor, 6927, LV_MSIG(4, 112) },    /* set_margin_hor */
  { (const void*) &lv_style_set_margin_ver, 6949, LV_MSIG(4, 112) },    /* set_margin_ver */
  { (const void*) &lv_style_set_pad_all, 8475, LV_MSIG(4, 112) },    /* set_pad_all */
  { (const void*) &lv_style_set_pad_gap, 8483, LV_MSIG(4, 112) },    /* set_pad_gap */
  { (const void*) &lv_style_set_pad_hor, 8491, LV_MSIG(4, 112) },    /* set_pad_hor */
  { (const void*) &lv_style_set_pad_ver, 8515, LV_MSIG(4, 112) },    /* set_pad_ver */
  { (const void*) &lv_style_set_prop, 566, LV_MSIG(4, 113) },    /* set_prop */
  { (const void*) &lv_style_set_size, 343, LV_MSIG(4, 113) },    /* set_size */
  { (const void*) &lv_style_set_text_font, 7927, LV_MSIG(4, 110) },    /* set_text_font */
  { (const void*) &lv_style_set_transform_scale, 3657, LV_MSIG(4, 112) },    /* set_transform_scale */
  { (const void*) &lv_style_set_transform_scale, 4305, LV_MSIG(4, 112) },    /* set_transform_zoom */
};

/* `lv_switch` methods */
#ifdef BE_LV_WIDGET_SWITCH
const lv_ntv_method_t lv_switch_func[] = {
  { (const void*) &lv_switch_get_orientation, 6228, LV_MSIG(1, 165) },    /* get_orientation */
  { (const void*) &lv_switch_set_orientation, 6228, LV_MSIG(4, 81) },    /* set_orientation */
};
#endif // BE_LV_WIDGET_SWITCH

/* `lv_table` methods */
#ifdef BE_LV_WIDGET_TABLE
const lv_ntv_method_t lv_table_func[] = {
  { (const void*) &lv_table_clear_cell_ctrl, 3193, LV_MSIG(0, 97) },    /* clear_cell_ctrl */
  { (const void*) &lv_table_has_cell_ctrl, 4516, LV_MSIG(0, 139) },    /* has_cell_ctrl */
  { (const void*) &lv_table_get_cell_user_data, 3825, LV_MSIG(1, 150) },    /* get_cell_user_data */
  { (const void*) &lv_table_get_cell_value, 6696, LV_MSIG(1, 239) },    /* get_cell_value */
  { (const void*) &lv_table_get_column_count, 8387, LV_MSIG(1, 165) },    /* get_col_cnt */
  { (const void*) &lv_table_get_column_width, 7517, LV_MSIG(1, 174) },    /* get_col_width */
  { (const void*) &lv_table_get_column_count, 5173, LV_MSIG(1, 165) },    /* get_column_count */
  { (const void*) &lv_table_get_column_width, 5186, LV_MSIG(1, 174) },    /* get_column_width */
  { (const void*) &lv_table_get_row_count, 3697, LV_MSIG(1, 165) },    /* get_row_cnt */
  { (const void*) &lv_table_get_row_count, 2558, LV_MSIG(1, 165) },    /* get_row_count */
  { (const void*) &lv_table_get_selected_cell, 4838, LV_MSIG(1, 56) },    /* get_selected_cell */
  { (const void*) &lv_table_set_cell_ctrl, 3199, LV_MSIG(4, 97) },    /* set_cell_ctrl */
  { (const void*) &lv_table_set_cell_user_data, 3825, LV_MSIG(4, 95) },    /* set_cell_user_data */
  { (const void*) &lv_table_set_cell_value, 6696, LV_MSIG(4, 99) },    /* set_cell_value */
  { (const void*) &lv_table_set_cell_value_fmt, 3840, LV_MSIG(4, 100) },    /* set_cell_value_fmt */
  { (const void*) &lv_table_set_column_count, 8387, LV_MSIG(4, 81) },    /* set_col_cnt */
  { (const void*) &lv_table_set_column_width, 7517, LV_MSIG(4, 93) },    /* set_col_width */
  { (const void*) &lv_table_set_column_count, 5173, LV_MSIG(4, 81) },    /* set_column_count */
  { (const void*) &lv_table_set_column_width, 5186, LV_MSIG(4, 93) },    /* set_column_width */
  { (const void*) &lv_table_set_row_count, 3697, LV_MSIG(4, 81) },    /* set_row_cnt */
  { (const void*) &lv_table_set_row_count, 2558, LV_MSIG(4, 81) },    /* set_row_count */
  { (const void*) &lv_table_set_selected_cell, 4838, LV_MSIG(4, 93) },    /* set_selected_cell */
};
#endif // BE_LV_WIDGET_TABLE

/* `lv_tabview` methods */
#ifdef BE_LV_WIDGET_TABVIEW
const lv_ntv_method_t lv_tabview_func[] = {
  { (const void*) &lv_tabview_add_tab, 8355, LV_MSIG(0, 221) },    /* add_tab */
  { (const void*) &lv_tabview_get_content, 442, LV_MSIG(1, 214) },    /* get_content */
  { (const void*) &lv_tabview_get_tab_active, 8571, LV_MSIG(1, 165) },    /* get_tab_act */
  { (const void*) &lv_tabview_get_tab_active, 7279, LV_MSIG(1, 165) },    /* get_tab_active */
  { (const void*) &lv_tabview_get_tab_bar, 8579, LV_MSIG(1, 214) },    /* get_tab_bar */
  { (const void*) &lv_tabview_get_tab_bar_position, 2976, LV_MSIG(1, 165) },    /* get_tab_bar_position */
  { (const void*) &lv_tabview_get_tab_bar, 8267, LV_MSIG(1, 214) },    /* get_tab_btns */
  { (const void*) &lv_tabview_get_tab_button, 7290, LV_MSIG(1, 218) },    /* get_tab_button */
  { (const void*) &lv_tabview_get_tab_count, 7917, LV_MSIG(1, 165) },    /* get_tab_count */
  { (const void*) &lv_tabview_set_active, 8317, LV_MSIG(4, 93) },    /* set_act */
  { (const void*) &lv_tabview_set_active, 4775, LV_MSIG(4, 93) },    /* set_active */
  { (const void*) &lv_tabview_set_tab_bar_position, 2976, LV_MSIG(4, 81) },    /* set_tab_bar_position */
  { (const void*) &lv_tabview_set_tab_bar_size, 5745, LV_MSIG(4, 81) },    /* set_tab_bar_size */
  { (const void*) &lv_tabview_set_tab_text, 8276, LV_MSIG(4, 101) },    /* set_tab_text */
};
#endif // BE_LV_WIDGET_TABVIEW

/* `lv_textarea` methods */
#ifdef BE_LV_WIDGET_TEXTAREA
const lv_ntv_method_t lv_textarea_func[] = {
  { (const void*) &lv_textarea_add_char, 7997, LV_MSIG(0, 81) },    /* add_char */
  { (const void*) &lv_textarea_add_text, 8024, LV_MSIG(0, 102) },    /* add_text */
  { (const void*) &lv_textarea_clear_selection, 3209, LV_MSIG(0, 34) },    /* clear_selection */
  { (const void*) &lv_textarea_cursor_down, 5976, LV_MSIG(0, 34) },    /* cursor_down */
  { (const void*) &lv_textarea_cursor_left, 5988, LV_MSIG(0, 34) },    /* cursor_left */
  { (const void*) &lv_textarea_cursor_right, 5264, LV_MSIG(0, 34) },    /* cursor_right */
  { (const void*) &lv_textarea_cursor_up, 7537, LV_MSIG(0, 34) },    /* cursor_up */
  { (const void*) &lv_textarea_delete_char, 6000, LV_MSIG(0, 34) },    /* delete_char */
  { (const void*) &lv_textarea_delete_char_forward, 1313, LV_MSIG(0, 34) },    /* delete_char_forward */
  { (const void*) &lv_textarea_text_is_selected, 2993, LV_MSIG(0, 128) },    /* text_is_selected */
  { (const void*) &lv_textarea_get_accepted_chars, 3705, LV_MSIG(1, 235) },    /* get_accepted_chars */
  { (const void*) &lv_textarea_get_current_char, 5212, LV_MSIG(1, 165) },    /* get_current_char */
  { (const void*) &lv_textarea_get_cursor_click_pos, 2687, LV_MSIG(1, 128) },    /* get_cursor_click_pos */
  { (const void*) &lv_textarea_get_cursor_pos, 6751, LV_MSIG(1, 165) },    /* get_cursor_pos */
  { (const void*) &lv_textarea_get_label, 8769, LV_MSIG(1, 214) },    /* get_label */
  { (const void*) &lv_textarea_get_max_length, 6971, LV_MSIG(1, 165) },    /* get_max_length */
  { (const void*) &lv_textarea_get_one_line, 8141, LV_MSIG(1, 128) },    /* get_one_line */
  { (const void*) &lv_textarea_get_password_bullet, 3401, LV_MSIG(1, 235) },    /* get_password_bullet */
  { (const void*) &lv_textarea_get_password_mode, 4656, LV_MSIG(1, 128) },    /* get_password_mode */
  { (const void*) &lv_textarea_get_password_show_time, 1762, LV_MSIG(1, 165) },    /* get_password_show_time */
  { (const void*) &lv_textarea_get_placeholder_text, 2823, LV_MSIG(1, 235) },    /* get_placeholder_text */
  { (const void*) &lv_textarea_get_text, 2835, LV_MSIG(1, 235) },    /* get_text */
  { (const void*) &lv_textarea_get_text_selection, 4290, LV_MSIG(1, 128) },    /* get_text_selection */
  { (const void*) &lv_textarea_set_accepted_chars, 3705, LV_MSIG(4, 102) },    /* set_accepted_chars */
  { (const void*) &lv_textarea_set_accepted_chars_static, 781, LV_MSIG(4, 102) },    /* set_accepted_chars_static */
  { (const void*) &lv_textarea_set_align, 237, LV_MSIG(4, 81) },    /* set_align */
  { (const void*) &lv_textarea_set_cursor_click_pos, 2687, LV_MSIG(4, 78) },    /* set_cursor_click_pos */
  { (const void*) &lv_textarea_set_cursor_pos, 6751, LV_MSIG(4, 81) },    /* set_cursor_pos */
  { (const void*) &lv_textarea_set_insert_replace, 4065, LV_MSIG(4, 102) },    /* set_insert_replace */
  { (const void*) &lv_textarea_set_max_length, 6971, LV_MSIG(4, 81) },    /* set_max_length */
  { (const void*) &lv_textarea_set_one_line, 8141, LV_MSIG(4, 78) },    /* set_one_line */
  { (const void*) &lv_textarea_set_password_bullet, 3401, LV_MSIG(4, 102) },    /* set_password_bullet */
  { (const void*) &lv_textarea_set_password_mode, 4656, LV_MSIG(4, 78) },    /* set_password_mode */
  { (const void*) &lv_textarea_set_password_show_time, 1762, LV_MSIG(4, 81) },    /* set_password_show_time */
  { (const void*) &lv_textarea_set_placeholder_text, 2823, LV_MSIG(4, 102) },    /* set_placeholder_text */
  { (const void*) &lv_textarea_set_text, 2835, LV_MSIG(4, 102) },    /* set_text */
  { (const void*) &lv_textarea_set_text_selection, 4290, LV_MSIG(4, 78) },    /* set_text_selection */
};
#endif // BE_LV_WIDGET_TEXTAREA

/* `lv_theme` methods */
const lv_ntv_method_t lv_theme_func[] = {
};

/* `lv_tileview` methods */
#ifdef BE_LV_WIDGET_TILEVIEW
const lv_ntv_method_t lv_tileview_func[] = {
  { (const void*) &lv_tileview_add_tile, 8033, LV_MSIG(0, 220) },    /* add_tile */
  { (const void*) &lv_tileview_get_tile_active, 8312, LV_MSIG(1, 214) },    /* get_tile_act */
  { (const void*) &lv_tileview_get_tile_active, 6492, LV_MSIG(1, 214) },    /* get_tile_active */
  { (const void*) &lv_tileview_set_tile, 8037, LV_MSIG(4, 60) },    /* set_tile */
  { (const void*) &lv_tileview_set_tile_by_index, 4922, LV_MSIG(4, 97) },    /* set_tile_by_index */
  { (const void*) &lv_tileview_set_tile_by_index, 8587, LV_MSIG(4, 97) },    /* set_tile_id */
};
#endif // BE_LV_WIDGET_TILEVIEW

/* `lv_timer` methods */
const lv_ntv_method_t lv_timer_func[] = {
  { (const void*) &lv_timer_delete, 8854, LV_MSIG(0, 114) },    /* del */
  { (const void*) &lv_timer_delete, 4481, LV_MSIG(0, 114) },    /* delete */
  { (const void*) &lv_timer_pause, 8781, LV_MSIG(0, 114) },    /* pause */
  { (const void*) &lv_timer_ready, 8793, LV_MSIG(0, 114) },    /* ready */
  { (const void*) &lv_timer_delete, 8672, LV_MSIG(0, 114) },    /* remove */
  { (const void*) &lv_timer_reset, 8799, LV_MSIG(0, 114) },    /* reset */
  { (const void*) &lv_timer_resume, 8686, LV_MSIG(0, 114) },    /* resume */
  { (const void*) &lv_timer_get_next, 6415, LV_MSIG(1, 234) },    /* get_next */
  { (const void*) &lv_timer_get_paused, 8651, LV_MSIG(1, 143) },    /* get_paused */
  { (const void*) &lv_timer_get_user_data, 118, LV_MSIG(1, 151) },    /* get_user_data */
  { (const void*) &lv_timer_set_auto_delete, 5832, LV_MSIG(4, 117) },    /* set_auto_delete */
  { (const void*) &lv_timer_set_cb, 2187, LV_MSIG(4, 116) },    /* set_cb */
  { (const void*) &lv_timer_set_period, 8658, LV_MSIG(4, 118) },    /* set_period */
  { (const void*) &lv_timer_set_repeat_count, 5524, LV_MSIG(4, 118) },    /* set_repeat_count */
  { (const void*) &lv_timer_set_user_data, 118, LV_MSIG(4, 115) },    /* set_user_data */
};

extern const bclass be_class_lv_anim;
extern const bclass be_class_lv_animimg;
extern const bclass be_class_lv_arc;
extern const bclass be_class_lv_arclabel;
extern const bclass be_class_lv_bar;
extern const bclass be_class_lv_button;
extern const bclass be_class_lv_buttonmatrix;
extern const bclass be_class_lv_calendar;
extern const bclass be_class_lv_canvas;
extern const bclass be_class_lv_chart;
extern const bclass be_class_lv_checkbox;
extern const bclass be_class_lv_color;
extern const bclass be_class_lv_colorwheel;
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
extern const bclass be_class_lv_menu_cont;
extern const bclass be_class_lv_menu_page;
extern const bclass be_class_lv_menu_section;
extern const bclass be_class_lv_menu_separator;
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
extern const bclass be_class_lv_stripes;
extern const bclass be_class_lv_style;
extern const bclass be_class_lv_switch;
extern const bclass be_class_lv_table;
extern const bclass be_class_lv_tabview;
extern const bclass be_class_lv_textarea;
extern const bclass be_class_lv_theme;
extern const bclass be_class_lv_tileview;
extern const bclass be_class_lv_timer;


// map of clases
const lv_ntv_class_def_t lv_classes[] = {
  { "lv_anim", &be_class_lv_anim, lv_anim_func, sizeof(lv_anim_func) / sizeof(lv_anim_func[0]) },
#ifdef BE_LV_WIDGET_ANIMIMG
  { "lv_animimg", &be_class_lv_animimg, lv_animimg_func, sizeof(lv_animimg_func) / sizeof(lv_animimg_func[0]) },
#endif // BE_LV_WIDGET_ANIMIMG
#ifdef BE_LV_WIDGET_ARC
  { "lv_arc", &be_class_lv_arc, lv_arc_func, sizeof(lv_arc_func) / sizeof(lv_arc_func[0]) },
#endif // BE_LV_WIDGET_ARC
#ifdef BE_LV_WIDGET_ARCLABEL
  { "lv_arclabel", &be_class_lv_arclabel, lv_arclabel_func, sizeof(lv_arclabel_func) / sizeof(lv_arclabel_func[0]) },
#endif // BE_LV_WIDGET_ARCLABEL
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
#ifdef BE_LV_WIDGET_COLORWHEEL
  { "lv_colorwheel", &be_class_lv_colorwheel, lv_colorwheel_func, sizeof(lv_colorwheel_func) / sizeof(lv_colorwheel_func[0]) },
#endif // BE_LV_WIDGET_COLORWHEEL
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
#ifdef BE_LV_WIDGET_MENU_CONT
  { "lv_menu_cont", &be_class_lv_menu_cont, lv_menu_cont_func, sizeof(lv_menu_cont_func) / sizeof(lv_menu_cont_func[0]) },
#endif // BE_LV_WIDGET_MENU_CONT
#ifdef BE_LV_WIDGET_MENU_PAGE
  { "lv_menu_page", &be_class_lv_menu_page, lv_menu_page_func, sizeof(lv_menu_page_func) / sizeof(lv_menu_page_func[0]) },
#endif // BE_LV_WIDGET_MENU_PAGE
#ifdef BE_LV_WIDGET_MENU_SECTION
  { "lv_menu_section", &be_class_lv_menu_section, lv_menu_section_func, sizeof(lv_menu_section_func) / sizeof(lv_menu_section_func[0]) },
#endif // BE_LV_WIDGET_MENU_SECTION
#ifdef BE_LV_WIDGET_MENU_SEPARATOR
  { "lv_menu_separator", &be_class_lv_menu_separator, lv_menu_separator_func, sizeof(lv_menu_separator_func) / sizeof(lv_menu_separator_func[0]) },
#endif // BE_LV_WIDGET_MENU_SEPARATOR
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
#ifdef BE_LV_WIDGET_STRIPES
  { "lv_stripes", &be_class_lv_stripes, lv_stripes_func, sizeof(lv_stripes_func) / sizeof(lv_stripes_func[0]) },
#endif // BE_LV_WIDGET_STRIPES
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

  /* `lv_anim` methods */
  /* `lv_animimg` methods */
#ifdef BE_LV_WIDGET_ANIMIMG
  int be_ntv_lv_animimg_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_animimg_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_ANIMIMG
  /* `lv_arc` methods */
#ifdef BE_LV_WIDGET_ARC
  int be_ntv_lv_arc_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_arc_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_ARC
  /* `lv_arclabel` methods */
#ifdef BE_LV_WIDGET_ARCLABEL
  int be_ntv_lv_arclabel_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_arclabel_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_ARCLABEL
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
  /* `lv_color` methods */
  /* `lv_colorwheel` methods */
#ifdef BE_LV_WIDGET_COLORWHEEL
  int be_ntv_lv_colorwheel_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_colorwheel_create, "+_p", "(lv.obj)b"); }
#endif // BE_LV_WIDGET_COLORWHEEL
  /* `lv_display` methods */
  /* `lv_dropdown` methods */
#ifdef BE_LV_WIDGET_DROPDOWN
  int be_ntv_lv_dropdown_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_dropdown_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_DROPDOWN
  /* `lv_event` methods */
  /* `lv_font` methods */
  /* `lv_group` methods */
  int be_ntv_lv_group_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_group_create, "+_p", ""); }
  /* `lv_image` methods */
#ifdef BE_LV_WIDGET_IMAGE
  int be_ntv_lv_image_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_image_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_IMAGE
  /* `lv_imagebutton` methods */
#ifdef BE_LV_WIDGET_IMAGEBUTTON
  int be_ntv_lv_imagebutton_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_imagebutton_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_IMAGEBUTTON
  /* `lv_indev` methods */
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
  int be_ntv_lv_menu_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_menu_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_MENU
  /* `lv_menu_cont` methods */
#ifdef BE_LV_WIDGET_MENU_CONT
  int be_ntv_lv_menu_cont_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_menu_cont_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_MENU_CONT
  /* `lv_menu_page` methods */
#ifdef BE_LV_WIDGET_MENU_PAGE
  int be_ntv_lv_menu_page_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_menu_page_create, "+_p", "(lv.obj)c"); }
#endif // BE_LV_WIDGET_MENU_PAGE
  /* `lv_menu_section` methods */
#ifdef BE_LV_WIDGET_MENU_SECTION
  int be_ntv_lv_menu_section_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_menu_section_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_MENU_SECTION
  /* `lv_menu_separator` methods */
#ifdef BE_LV_WIDGET_MENU_SEPARATOR
  int be_ntv_lv_menu_separator_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_menu_separator_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_MENU_SEPARATOR
  /* `lv_msgbox` methods */
#ifdef BE_LV_WIDGET_MSGBOX
  int be_ntv_lv_msgbox_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_msgbox_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_MSGBOX
  /* `lv_obj` methods */
#ifdef BE_LV_WIDGET_OBJ
  int be_ntv_lv_obj_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_obj_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_OBJ
  /* `lv_qrcode` methods */
#ifdef BE_LV_WIDGET_QRCODE
  int be_ntv_lv_qrcode_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_qrcode_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_QRCODE
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
  /* `lv_stripes` methods */
#ifdef BE_LV_WIDGET_STRIPES
  int be_ntv_lv_stripes_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_stripes_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_STRIPES
  /* `lv_style` methods */
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
  /* `lv_theme` methods */
  /* `lv_tileview` methods */
#ifdef BE_LV_WIDGET_TILEVIEW
  int be_ntv_lv_tileview_init(bvm *vm)       { return be_call_c_func(vm, (void*) &lv_tileview_create, "+_p", "(lv.obj)"); }
#endif // BE_LV_WIDGET_TILEVIEW
  /* `lv_timer` methods */

// create font either empty or from parameter on stack
int lvbe_font_create(bvm *vm)       { return be_call_c_func(vm, NULL, "+_p", ""); }
int lvbe_theme_create(bvm *vm)       { return be_call_c_func(vm, NULL, "+_p", ""); }


#ifdef __cplusplus
} /* extern "C" */
#endif
/********************************************************************/
