#
# ctype buidings for LVGL
#
# To generate C bindings, do:
# > compile("lvgl_ctypes.be","file")()
#
# and copy/paste output in C format in `be_lvgl_ctypes_definitions.c`
#
import ctypes

ctypes.print_types()

lv_style_int = ctypes.i16
lv_color = [ctypes.u16, "lv_color"]        # cast to the class instance, constructor is called with 2 args: (nil, value)
lv_grad_dir = ctypes.u8
lv_meter_indicator_type_t = ctypes.u8
lv_opa = ctypes.u8
lv_blend_mode = ctypes.u8
lv_align = ctypes.u8
lv_coord = ctypes.i16
lv_bidi_dir = ctypes.u8
lv_txt_flag = ctypes.u8
lv_text_decor = ctypes.u8
lv_font = ctypes.u32
lv_blend_mode = ctypes.u8
lv_draw_mask_line_side = ctypes.u8
lv_dir = ctypes.u8
uint8_t_1 = ctypes.bf_1
uint8_t_2 = ctypes.bf_2
uint8_t_3 = ctypes.bf_3
uint8_t_4 = ctypes.bf_4
uint8_t_5 = ctypes.bf_5
uint8_t_11 = ctypes.bf_11
uint8_t = ctypes.u8
int16_t = ctypes.i16
uint16_t = ctypes.u16
uint16_t_16 = ctypes.bf_16
uint32_t = ctypes.u32
int32_t = ctypes.i32
ptr = ctypes.ptr32

lv_point = [            # valid LVGL8
    [lv_coord, "x"],
    [lv_coord, "y"],
]
lv_point = ctypes.structure(lv_point, "lv_point")

lv_area = [            # valid LVGL8
    [lv_coord, "x1"],
    [lv_coord, "y1"],
    [lv_coord, "x2"],
    [lv_coord, "y2"],
]
lv_area = ctypes.structure(lv_area, "lv_area")

lv_draw_rect_dsc = [            # valid LVGL8
    [lv_coord, "radius"],
    [lv_blend_mode, "blend_mode"],

    #/*Background*/
    [lv_color, "bg_color"],
    [lv_color, "bg_grad_color"],
    [uint8_t, "bg_main_color_stop"],
    [uint8_t, "bg_grad_color_stop"],
    [lv_opa, "bg_opa"],
    [uint8_t_3, "bg_grad_dir"],

    #/*Background img*/
    [ptr, "bg_img_src"],
    [ptr, "bg_img_symbol_font"],
    [lv_color, "bg_img_recolor"],
    [lv_opa, "bg_img_opa"],
    [lv_opa, "bg_img_recolor_opa"],
    [uint8_t, "bg_img_tiled"],

    #/*Border*/
    [lv_color, "border_color"],
    [lv_coord, "border_width"],
    [lv_opa, "border_opa"],
    [uint8_t_1, "border_post"],
    [uint8_t_5, "border_side"],

    #/*Outline*/
    [lv_color, "outline_color"],
    [lv_coord, "outline_width"],
    [lv_coord, "outline_pad"],
    [lv_opa, "outline_opa"],

    #/*Shadow*/
    [lv_color, "shadow_color"],
    [lv_coord, "shadow_width"],
    [lv_coord, "shadow_ofs_x"],
    [lv_coord, "shadow_ofs_y"],
    [lv_coord, "shadow_spread"],
    [lv_opa, "shadow_opa"],
]
lv_draw_rect_dsc = ctypes.structure(lv_draw_rect_dsc, "lv_draw_rect_dsc")

lv_draw_line_dsc = [            # valid LVGL8
    [lv_color, "color"],
    [lv_coord, "width"],
    [lv_coord, "dash_width"],
    [lv_coord, "dash_gap"],
    [lv_opa, "opa"],
    [uint8_t_2, "blend_mode"],
    [uint8_t_1, "round_start"],
    [uint8_t_1, "round_end"],
    [uint8_t_1, "raw_end"],
]
lv_draw_line_dsc = ctypes.structure(lv_draw_line_dsc, "lv_draw_line_dsc")

lv_draw_img_dsc = [            # valid LVGL8
    [uint16_t, "angle"],
    [uint16_t, "zoom"],
    [lv_point, "pivot"],

    [lv_color, "recolor"],
    [lv_opa, "recolor_opa"],

    [lv_opa, "opa"],
    [uint8_t_4, "blend_mode"],

    [int32_t, "frame_id"],
    [uint8_t_1, "antialias"],
]
lv_draw_img_dsc = ctypes.structure(lv_draw_img_dsc, "lv_draw_img_dsc")

#- --------- lv_mask --------- -#
lv_draw_mask_xcb = ptr    # callback
lv_draw_mask_type = ctypes.u8
lv_draw_mask_line_side = ctypes.u8

lv_draw_mask_common_dsc = [            # valid LVGL8
    [lv_draw_mask_xcb, "cb"],
    [lv_draw_mask_type, "type"],
]
lv_draw_mask_common_dsc = ctypes.structure(lv_draw_mask_common_dsc, "lv_draw_mask_common_dsc")

lv_draw_mask_line_param_cfg = [            # valid LVGL8
    #/*First point */
    [lv_point, "p1"],

    #/*Second point*/
    [lv_point, "p2"],

    #/*Which side to keep?*/
    [uint8_t_2, "side"],
]
lv_draw_mask_line_param_cfg = ctypes.structure(lv_draw_mask_line_param_cfg, "lv_draw_mask_line_param_cfg")

lv_draw_mask_line_param = [            # valid LVGL8
    #/*The first element must be the common descriptor*/
    [lv_draw_mask_common_dsc, "dsc"],
    [lv_draw_mask_line_param_cfg, "cfg"],
    #/*A point of the line*/
    [lv_point, "origo"],
    #/* X / (1024*Y) steepness (X is 0..1023 range). What is the change of X in 1024 Y?*/
    [int32_t, "xy_steep"],
    #/* Y / (1024*X) steepness (Y is 0..1023 range). What is the change of Y in 1024 X?*/
    [int32_t, "yx_steep"],
    #/*Helper which stores yx_steep for flat lines and xy_steep for steep (non flat) lines */
    [int32_t, "steep"],
    #/*Steepness in 1 px in 0..255 range. Used only by flat lines. */
    [int32_t, "spx"],
    #/*1: It's a flat line? (Near to horizontal)*/
    [uint8_t_1, "flat"],
    #/* Invert the mask. The default is: Keep the left part.
    # * It is used to select left/right/top/bottom*/
    [uint8_t_1, "inv"],
]
lv_draw_mask_line_param = ctypes.structure(lv_draw_mask_line_param, "lv_draw_mask_line_param")

lv_draw_mask_angle_param_cfg = [            # valid LVGL8
    [lv_point, "vertex_p"],
    [lv_coord, "start_angle"],
    [lv_coord, "end_angle"],
]
lv_draw_mask_angle_param_cfg = ctypes.structure(lv_draw_mask_angle_param_cfg, "lv_draw_mask_angle_param_cfg")

lv_draw_mask_angle_param = [            # valid LVGL8
    #/*The first element must be the common descriptor*/
    [lv_draw_mask_common_dsc, "dsc"],
    [lv_draw_mask_angle_param_cfg, "cfg"],

    [lv_draw_mask_line_param, "start_line"],
    [lv_draw_mask_line_param, "end_line"],
    [uint16_t, "delta_deg"],
]
lv_draw_mask_angle_param = ctypes.structure(lv_draw_mask_angle_param, "lv_draw_mask_angle_param")


lv_draw_mask_radius_param_cfg = [            # valid LVGL8
    [lv_area, "rect"],
    [lv_coord, "radius"],
    [uint8_t_1, "outer"],
]
lv_draw_mask_radius_param_cfg = ctypes.structure(lv_draw_mask_radius_param_cfg, "lv_draw_mask_radius_param_cfg")

lv_sqrt_res = [            # valid LVGL8
    [uint16_t, "i"],
    [uint16_t, "f"],
]
lv_sqrt_res = ctypes.structure(lv_sqrt_res, "lv_sqrt_res")

lv_draw_mask_radius_param = [            # valid LVGL8
    #/*The first element must be the common descriptor*/
    [lv_draw_mask_common_dsc, "dsc"],
    [lv_draw_mask_radius_param_cfg, "cfg"],
    [int32_t, "y_prev"],
    [lv_sqrt_res, "y_prev_x"],
]
lv_draw_mask_radius_param = ctypes.structure(lv_draw_mask_radius_param, "lv_draw_mask_radius_param")


lv_draw_mask_fade_param_cfg = [            # valid LVGL8
    [lv_area, "coords"],
    [lv_coord, "y_top"],
    [lv_coord, "y_bottom"],
    [lv_opa, "opa_top"],
    [lv_opa, "opa_bottom"],
]
lv_draw_mask_fade_param_cfg = ctypes.structure(lv_draw_mask_fade_param_cfg, "lv_draw_mask_fade_param_cfg")

lv_draw_mask_fade_param = [            # valid LVGL8
    # /*The first element must be the common descriptor*/
    [lv_draw_mask_common_dsc, "dsc"],
    [lv_draw_mask_fade_param_cfg, "cfg"],
]
lv_draw_mask_fade_param = ctypes.structure(lv_draw_mask_fade_param, "lv_draw_mask_fade_param")


lv_draw_mask_map_param_cfg = [            # valid LVGL8
    [lv_area, "coords"],
    [ptr, "map"],
]
lv_draw_mask_map_param_cfg = ctypes.structure(lv_draw_mask_map_param_cfg, "lv_draw_mask_map_param_cfg")

lv_draw_mask_map_param = [            # valid LVGL8
    #/*The first element must be the common descriptor*/
    [lv_draw_mask_common_dsc, "dsc"],
    [lv_draw_mask_map_param_cfg, "cfg"],
]
lv_draw_mask_map_param = ctypes.structure(lv_draw_mask_map_param, "lv_draw_mask_map_param")

lv_draw_mask_saved = [            # valid LVGL8
    [ptr, "param"],
    [ptr, "custom_id"],
]
lv_draw_mask_saved = ctypes.structure(lv_draw_mask_saved, "lv_draw_mask_saved")

# lv_meter

# lv_meter_scale_t
lv_meter_scale = [            # valid LVGL8
    [lv_color, "tick_color"],
    [uint16_t, "tick_cnt"],
    [uint16_t, "tick_length"],
    [uint16_t, "tick_width"],

    [lv_color, "tick_major_color"],
    [uint16_t, "tick_major_nth"],
    [uint16_t, "tick_major_length"],
    [uint16_t, "tick_major_width"],

    [int16_t, "label_gap"],
    [int16_t, "label_color"],

    [int32_t, "min"],
    [int32_t, "max"],
    [int16_t, "r_mod"],
    [uint16_t, "angle_range"],
    [int16_t, "rotation"],
]
lv_meter_scale = ctypes.structure(lv_meter_scale, "lv_meter_scale")

# lv_meter_indicator_t
lv_meter_indicator = [            # valid LVGL8
    [ptr, "scale"],
    [lv_meter_indicator_type_t, "type"],
    [lv_opa, "opa"],
    [int32_t, "start_value"],
    [int32_t, "end_value"],
]
lv_meter_indicator = ctypes.structure(lv_meter_indicator, "lv_meter_indicator")

# variants
lv_meter_indicator_needle_img = [            # valid LVGL8
    [ptr, "scale"],
    [lv_meter_indicator_type_t, "type"],
    [lv_opa, "opa"],
    [int32_t, "start_value"],
    [int32_t, "end_value"],
    # specifc portion
    [ptr, "src"],
    [lv_point, "pivot"],
]
lv_meter_indicator_needle_img = ctypes.structure(lv_meter_indicator_needle_img, "lv_meter_indicator_needle_img")

lv_meter_indicator_needle_line = [            # valid LVGL8
    [ptr, "scale"],
    [lv_meter_indicator_type_t, "type"],
    [lv_opa, "opa"],
    [int32_t, "start_value"],
    [int32_t, "end_value"],
    # specifc portion
    [uint16_t, "width"],
    [int16_t, "r_mod"],
    [lv_color, "color"],
]
lv_meter_indicator_needle_line = ctypes.structure(lv_meter_indicator_needle_line, "lv_meter_indicator_needle_line")

lv_meter_indicator_arc = [            # valid LVGL8
    [ptr, "scale"],
    [lv_meter_indicator_type_t, "type"],
    [lv_opa, "opa"],
    [int32_t, "start_value"],
    [int32_t, "end_value"],
    # specifc portion
    [uint16_t, "width"],
    [ptr, "src"],
    [lv_color, "color"],
    [int16_t, "r_mod"],
]
lv_meter_indicator_arc = ctypes.structure(lv_meter_indicator_arc, "lv_meter_indicator_arc")

lv_meter_indicator_scale_lines = [            # valid LVGL8
    [ptr, "scale"],
    [lv_meter_indicator_type_t, "type"],
    [lv_opa, "opa"],
    [int32_t, "start_value"],
    [int32_t, "end_value"],
    # specifc portion
    [int16_t, "width_mod"],
    [lv_color, "color_start"],
    [lv_color, "color_end"],
    [uint8_t_1, "local_grad"],
]
lv_meter_indicator_scale_lines = ctypes.structure(lv_meter_indicator_scale_lines, "lv_meter_indicator_scale_lines")

lv_chart_series = [
    [ptr, "x_points"],
    [ptr, "y_points"],
    [lv_color, "color"],
    [uint16_t, "start_point"],
    [uint8_t_1, "hidden"],
    [uint8_t_1, "x_ext_buf_assigned"],
    [uint8_t_1, "y_ext_buf_assigned"],
    [uint8_t_1, "x_axis_sec"],
    [uint8_t_1, "y_axis_sec"],
]
lv_chart_series = ctypes.structure(lv_chart_series, "lv_chart_series")

lv_chart_cursor = [
    [lv_point, "pos"],
    [uint16_t, "point_id"],
    [lv_color, "color"],
    [ptr, "ser"],
    [lv_dir, "dir"],
    [uint8_t_1, "pos_set"],
]
lv_chart_cursor = ctypes.structure(lv_chart_cursor, "lv_chart_cursor")

#- --------- class system --------- -#
lv_obj_class_ptr = ptr
lv_obj_ptr = ptr
lv_event_ptr = ptr
constructor_cb = ptr    # callback
destructor_cb = ptr    # callback
event_cb = ptr    # callback
lv_event_code = ctypes.i32

lv_obj_class = [            # valid LVGL8
    [lv_obj_class_ptr, "base_class"],
    [constructor_cb, "constructor_cb"],
    [destructor_cb, "destructor_cb"],
    [ptr, "user_data"],
    [event_cb, "event_cb"],
    [lv_coord, "width_def"],
    [lv_coord, "height_def"],
    [uint8_t_2, "editable"],
    [uint8_t_2, "group_def"],
    [uint16_t_16, "instance_size"],
]
lv_obj_class = ctypes.structure(lv_obj_class, "lv_obj_class")

lv_event = [            # valid LVGL8
    [lv_obj_ptr, "target"],
    [lv_obj_ptr, "current_target"],
    [lv_event_code, "code"],
    [ptr, "user_data"],
    [ptr, "param"],
    [lv_event_ptr, "prev"],
    [uint8_t_1, "deleted"],
]
lv_event = ctypes.structure(lv_event, "lv_event")

#######################################################################
# lv_img structures
lv_img_header = [            # valid LVGL8
    [uint8_t_5, "cf"],
    [uint8_t_3, "always_zero"],
    [uint8_t_2, "reserved"],
    [uint8_t_11, "w"],
    [uint8_t_11, "h"],
]
lv_img_header = ctypes.structure(lv_img_header, "lv_img_header")

lv_img_dsc = [            # valid LVGL8
    [lv_img_header, "header"],
    [uint8_t_5, "cf"],
    [uint32_t, "data_size"],
    [ptr, "data"],
]
lv_img_dsc = ctypes.structure(lv_img_dsc, "lv_img_dsc")

#######################################################################
# lv_style
lv_style_transition_dsc = [            # valid LVGL8
    [ptr, "props"],
    [ptr, "user_data"],
    [ptr, "path_xcb"],
    [uint32_t, "time"],
    [uint32_t, "delay"],
]
lv_style_transition_dsc = ctypes.structure(lv_style_transition_dsc, "lv_style_transition_dsc")

#######################################################################
# lv_color
# lv_color_hsv = [            # valid LVGL8
#     [uint16_t, "h"],
#     [uint8_t, "s"],
#     [uint8_t, "v"],
# ]
# lv_color_hsv = ctypes.structure(lv_color_hsv, "lv_color_hsv")

lv_color_filter_dsc = [            # valid LVGL8
    [ptr, "filter_cb"],
    [ptr, "user_data"],
]
lv_color_filter_dsc = ctypes.structure(lv_color_filter_dsc, "lv_color_filter_dsc")

#######################################################################
# lv_timer
lv_timer = [            # valid LVGL8
    [uint32_t, "period"],
    [uint32_t, "last_run"],
    [ptr, "timer_cb"],
    [ptr, "user_data"],
    [int32_t, "repeat_count"],
    [uint8_t_1, "paused"],
]
lv_timer = ctypes.structure(lv_timer, "lv_timer")

#######################################################################
# Special structure used to calibrate resistive touchscreens
#######################################################################
lv_ts_calibration = [            # valid LVGL8
    [lv_coord, "raw_x"],
    [lv_coord, "raw_y"],
    [lv_coord, "x"],
    [lv_coord, "y"],
    [int32_t, "state"],
]
lv_ts_calibration = ctypes.structure(lv_ts_calibration, "lv_ts_calibration")

#
ctypes.print_classes("lvgl")

# Ex:
# bb = ctypes.buffer(test_t, bytes("0101020203030404FFFFFEFEFCFC8080"))

# Ex:
# bb = ctypes.buffer(lv_draw_rect)

#- Ex

bb=ctypes.buffer(lv_draw_line_dsc)
bb.color
bb.blend_mode

bb.color = -1
- bytes('FFFF0000000000000003')

bb.blend_mode=3
- bytes('FFFF0000000000000003')

bb.raw_end = 1
- bytes('FFFF0000000000000013')

bb.blend_mode=0
- bytes('FFFF0000000000000010')

lv_draw_line_dsc = [
    [lv_color, "color"],
    [lv_style_int, "width"],
    [lv_style_int, "dash_width"],
    [lv_style_int, "dash_gap"],
    [lv_opa, "opa"],
    [uint8_t_2, "blend_mode"],
    [uint8_t_1, "round_start"],
    [uint8_t_1, "round_end"],
    [uint8_t_1, "raw_end"],
]

-#