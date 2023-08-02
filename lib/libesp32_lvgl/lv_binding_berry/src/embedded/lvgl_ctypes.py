# define LVGL low-level C structures

import sys
import berry_ctypes as ct

sys.stdout = open("../be_lvgl_ctypes_definitions.c", 'w')

print("""/********************************************************************
 * Tasmota LVGL ctypes mapping
 *******************************************************************/
#include "be_ctypes.h"
#include "lvgl.h"
#include "be_mapping.h"

""")

ct.print_types()

lv_style_int = ct.i16
lv_color = [ct.u16, "lv_color"]        # cast to the class instance, constructor is called with 2 args: (nil, value)
lv_grad_dir = ct.u8
lv_meter_indicator_type_t = ct.u8
lv_opa = ct.u8
lv_blend_mode = ct.u8
lv_align = ct.u8
lv_coord_t = ct.i16
lv_bidi_dir = ct.u8
lv_txt_flag = ct.u8
lv_text_decor = ct.u8
lv_font = ct.u32
lv_blend_mode = ct.u8
lv_draw_mask_line_side = ct.u8
lv_dir = ct.u8
uint8_t_1 = ct.bf_1
uint8_t_2 = ct.bf_2
uint8_t_3 = ct.bf_3
uint8_t_4 = ct.bf_4
uint8_t_5 = ct.bf_5
uint8_t_11 = ct.bf_11
uint8_t = ct.u8
int16_t = ct.i16
uint16_t = ct.u16
uint16_t_15 = ct.bf_15
uint16_t_16 = ct.bf_16
uint32_t = ct.u32
int32_t = ct.i32
ptr = ct.ptr32

lv_coord = [            # valid LVGL8.3
    [lv_coord_t, "v"],
]
lv_coord = ct.structure(lv_coord, "lv_coord")

lv_point = [            # valid LVGL8.3
    [lv_coord_t, "x"],
    [lv_coord_t, "y"],
]
lv_point = ct.structure(lv_point, "lv_point")

lv_area = [            # valid LVGL8.3
    [lv_coord_t, "x1"],
    [lv_coord_t, "y1"],
    [lv_coord_t, "x2"],
    [lv_coord_t, "y2"],
]
lv_area = ct.structure(lv_area, "lv_area")

#######################################################################
# lv_grad_dsc
lv_gradient_stop = [            # valid LVGL8.3
    [lv_color, "color"],
    [uint8_t, "frac"],
]
lv_gradient_stop = ct.structure(lv_gradient_stop, "lv_gradient_stop")

lv_grad_dsc = [            # valid LVGL8.3
    [lv_gradient_stop, "stops_0"],
    [lv_gradient_stop, "stops_1"],
    [uint8_t, "stops_count"],
    [uint8_t_3, "dir"],
    [uint8_t_3, "dither"],
]
lv_grad_dsc = ct.structure(lv_grad_dsc, "lv_grad_dsc")

lv_draw_rect_dsc = [            # valid LVGL8.3
    [lv_coord_t, "radius"],
    [lv_blend_mode, "blend_mode"],

    #/*Background*/
    [lv_opa, "bg_opa"],
    [lv_color, "bg_color"],
    [lv_grad_dsc, "bg_grad"],

    #/*Background img*/
    [ptr, "bg_img_src"],
    [ptr, "bg_img_symbol_font"],
    [lv_color, "bg_img_recolor"],
    [lv_opa, "bg_img_opa"],
    [lv_opa, "bg_img_recolor_opa"],
    [uint8_t, "bg_img_tiled"],

    #/*Border*/
    [lv_color, "border_color"],
    [lv_coord_t, "border_width"],
    [lv_opa, "border_opa"],
    [uint8_t_1, "border_post"],
    [uint8_t_5, "border_side"],

    #/*Outline*/
    [lv_color, "outline_color"],
    [lv_coord_t, "outline_width"],
    [lv_coord_t, "outline_pad"],
    [lv_opa, "outline_opa"],

    #/*Shadow*/
    [lv_color, "shadow_color"],
    [lv_coord_t, "shadow_width"],
    [lv_coord_t, "shadow_ofs_x"],
    [lv_coord_t, "shadow_ofs_y"],
    [lv_coord_t, "shadow_spread"],
    [lv_opa, "shadow_opa"],
]
lv_draw_rect_dsc = ct.structure(lv_draw_rect_dsc, "lv_draw_rect_dsc")

lv_draw_line_dsc = [            # valid LVGL8.3
    [lv_color, "color"],
    [lv_coord_t, "width"],
    [lv_coord_t, "dash_width"],
    [lv_coord_t, "dash_gap"],
    [lv_opa, "opa"],
    [uint8_t_2, "blend_mode"],
    [uint8_t_1, "round_start"],
    [uint8_t_1, "round_end"],
    [uint8_t_1, "raw_end"],
]
lv_draw_line_dsc = ct.structure(lv_draw_line_dsc, "lv_draw_line_dsc")

lv_draw_arc_dsc = [            # valid LVGL8.3
    [lv_color, "color"],
    [lv_coord_t, "width"],
    [uint16_t, "start_angle"],
    [uint16_t, "end_angle"],
    [ptr, "img_src"],
    [lv_opa, "opa"],
    [uint8_t_2, "blend_mode"],
    [uint8_t_1, "rounded"],
]
lv_draw_arc_dsc = ct.structure(lv_draw_arc_dsc, "lv_draw_arc_dsc")

lv_draw_img_dsc = [            # valid LVGL8.3
    [int16_t, "angle"],
    [uint16_t, "zoom"],
    [lv_point, "pivot"],

    [lv_color, "recolor"],
    [lv_opa, "recolor_opa"],

    [lv_opa, "opa"],
    [uint8_t_4, "blend_mode"],

    [int32_t, "frame_id"],
    [uint8_t_1, "antialias"],
]
lv_draw_img_dsc = ct.structure(lv_draw_img_dsc, "lv_draw_img_dsc")

lv_obj_draw_part_dsc = [            # valid LVGL8.3
    [ptr, "draw_ctx"],
    [ptr, "class_p"],
    [uint32_t, "type"],
    [ptr, "draw_area"],
    [ptr, "rect_dsc"],
    [ptr, "label_dsc"],
    [ptr, "line_dsc"],
    [ptr, "img_dsc"],
    [ptr, "arc_dsc"],
    [ptr, "p1"],
    [ptr, "p2"],
    [ptr, "text"],
    [uint32_t, "text_length"],
    [uint32_t, "part"],
    [uint32_t, "id"],
    [lv_coord_t, "radius"],
    [int32_t, "value"],
    [ptr, "sub_part_ptr"],
]
lv_obj_draw_part_dsc = ct.structure(lv_obj_draw_part_dsc, "lv_obj_draw_part_dsc")

lv_draw_layer_ctx = [            # valid LVGL8.3
    [lv_area, "area_full"],
    [lv_area, "area_act"],
    [lv_coord_t, "max_row_with_alpha"],
    [lv_coord_t, "max_row_with_no_alpha"],
    [ptr, "buf"],
    [ptr, "clip_area"],
    [ptr, "buf_area"],
    [ptr, "buf"],
    [uint8_t, "screen_transp"],
]
lv_draw_layer_ctx = ct.structure(lv_draw_layer_ctx, "lv_draw_layer_ctx")

#- --------- lv_mask --------- -#
lv_draw_mask_xcb = ptr    # callback
lv_draw_mask_type = ct.u8
lv_draw_mask_line_side = ct.u8

lv_draw_mask_common_dsc = [            # valid LVGL8.3
    [lv_draw_mask_xcb, "cb"],
    [lv_draw_mask_type, "type"],
]
lv_draw_mask_common_dsc = ct.structure(lv_draw_mask_common_dsc, "lv_draw_mask_common_dsc")

lv_draw_mask_line_param_cfg = [            # valid LVGL8.3
    #/*First point */
    [lv_point, "p1"],

    #/*Second point*/
    [lv_point, "p2"],

    #/*Which side to keep?*/
    [uint8_t_2, "side"],
]
lv_draw_mask_line_param_cfg = ct.structure(lv_draw_mask_line_param_cfg, "lv_draw_mask_line_param_cfg")

lv_draw_mask_line_param = [            # valid LVGL8.3
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
lv_draw_mask_line_param = ct.structure(lv_draw_mask_line_param, "lv_draw_mask_line_param")

lv_draw_mask_angle_param_cfg = [            # valid LVGL8.3
    [lv_point, "vertex_p"],
    [lv_coord_t, "start_angle"],
    [lv_coord_t, "end_angle"],
]
lv_draw_mask_angle_param_cfg = ct.structure(lv_draw_mask_angle_param_cfg, "lv_draw_mask_angle_param_cfg")

lv_draw_mask_angle_param = [            # valid LVGL8.3
    #/*The first element must be the common descriptor*/
    [lv_draw_mask_common_dsc, "dsc"],
    [lv_draw_mask_angle_param_cfg, "cfg"],

    [lv_draw_mask_line_param, "start_line"],
    [lv_draw_mask_line_param, "end_line"],
    [uint16_t, "delta_deg"],
]
lv_draw_mask_angle_param = ct.structure(lv_draw_mask_angle_param, "lv_draw_mask_angle_param")


lv_draw_mask_radius_param_cfg = [            # valid LVGL8.3
    [lv_area, "rect"],
    [lv_coord_t, "radius"],
    [uint8_t_1, "outer"],
]
lv_draw_mask_radius_param_cfg = ct.structure(lv_draw_mask_radius_param_cfg, "lv_draw_mask_radius_param_cfg")

lv_draw_mask_radius_circle_dsc = [            # valid LVGL8.3
    [ptr, "buf"],
    [ptr, "cir_opa"],
    [ptr, "x_start_on_y"],
    [ptr, "opa_start_on_y"],
    [int32_t, "life"],
    [uint32_t, "used_cnt"],
    [lv_coord_t, "radius"],
]
lv_draw_mask_radius_circle_dsc = ct.structure(lv_draw_mask_radius_circle_dsc, "lv_draw_mask_radius_circle_dsc")

lv_draw_mask_radius_param = [            # valid LVGL8.3
    #/*The first element must be the common descriptor*/
    [lv_draw_mask_common_dsc, "dsc"],
    [lv_draw_mask_radius_param_cfg, "cfg"],
    [lv_draw_mask_radius_circle_dsc, "circle"],
]
lv_draw_mask_radius_param = ct.structure(lv_draw_mask_radius_param, "lv_draw_mask_radius_param")


lv_draw_mask_fade_param_cfg = [            # valid LVGL8.3
    [lv_area, "coords"],
    [lv_coord_t, "y_top"],
    [lv_coord_t, "y_bottom"],
    [lv_opa, "opa_top"],
    [lv_opa, "opa_bottom"],
]
lv_draw_mask_fade_param_cfg = ct.structure(lv_draw_mask_fade_param_cfg, "lv_draw_mask_fade_param_cfg")

lv_draw_mask_fade_param = [            # valid LVGL8.3
    # /*The first element must be the common descriptor*/
    [lv_draw_mask_common_dsc, "dsc"],
    [lv_draw_mask_fade_param_cfg, "cfg"],
]
lv_draw_mask_fade_param = ct.structure(lv_draw_mask_fade_param, "lv_draw_mask_fade_param")


lv_draw_mask_map_param_cfg = [            # valid LVGL8.3
    [lv_area, "coords"],
    [ptr, "map"],
]
lv_draw_mask_map_param_cfg = ct.structure(lv_draw_mask_map_param_cfg, "lv_draw_mask_map_param_cfg")

lv_draw_mask_map_param = [            # valid LVGL8.3
    [lv_draw_mask_common_dsc, "dsc"],
    [lv_draw_mask_map_param_cfg, "cfg"],
]
lv_draw_mask_map_param = ct.structure(lv_draw_mask_map_param, "lv_draw_mask_map_param")


lv_draw_mask_polygon_param_cfg = [            # valid LVGL8.3
    [ptr, "points"],
    [uint16_t, "point_cnt"],
]
lv_draw_mask_polygon_param_cfg = ct.structure(lv_draw_mask_polygon_param_cfg, "lv_draw_mask_polygon_param_cfg")

lv_draw_mask_polygon_param = [            # valid LVGL8.3
    [lv_draw_mask_common_dsc, "dsc"],
    [lv_draw_mask_polygon_param_cfg, "cfg"],
]
lv_draw_mask_polygon_param = ct.structure(lv_draw_mask_polygon_param, "lv_draw_mask_polygon_param")


lv_draw_mask_saved = [            # valid LVGL8.3
    [ptr, "param"],
    [ptr, "custom_id"],
]
lv_draw_mask_saved = ct.structure(lv_draw_mask_saved, "lv_draw_mask_saved")

# lv_meter

# lv_meter_scale_t
lv_meter_scale = [            # valid LVGL8.3
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
lv_meter_scale = ct.structure(lv_meter_scale, "lv_meter_scale")

# lv_meter_indicator_t
lv_meter_indicator = [            # valid LVGL8.3
    [ptr, "scale"],
    [lv_meter_indicator_type_t, "type"],
    [lv_opa, "opa"],
    [int32_t, "start_value"],
    [int32_t, "end_value"],
    # more unions here that we can hardly manage simply
    [ptr, "data0"],
    [ptr, "data1"],
    [ptr, "data2"],
]
lv_meter_indicator = ct.structure(lv_meter_indicator, "lv_meter_indicator")

# # variants
# lv_meter_indicator_needle_img = [            # valid LVGL8.2
#     [ptr, "scale"],
#     [lv_meter_indicator_type_t, "type"],
#     [lv_opa, "opa"],
#     [int32_t, "start_value"],
#     [int32_t, "end_value"],
#     # specifc portion
#     [ptr, "src"],
#     [lv_point, "pivot"],
# ]
# lv_meter_indicator_needle_img = ct.structure(lv_meter_indicator_needle_img, "lv_meter_indicator_needle_img")

# lv_meter_indicator_needle_line = [            # valid LVGL8.2
#     [ptr, "scale"],
#     [lv_meter_indicator_type_t, "type"],
#     [lv_opa, "opa"],
#     [int32_t, "start_value"],
#     [int32_t, "end_value"],
#     # specifc portion
#     [uint16_t, "width"],
#     [int16_t, "r_mod"],
#     [lv_color, "color"],
# ]
# lv_meter_indicator_needle_line = ct.structure(lv_meter_indicator_needle_line, "lv_meter_indicator_needle_line")

# lv_meter_indicator_arc = [            # valid LVGL8.2
#     [ptr, "scale"],
#     [lv_meter_indicator_type_t, "type"],
#     [lv_opa, "opa"],
#     [int32_t, "start_value"],
#     [int32_t, "end_value"],
#     # specifc portion
#     [uint16_t, "width"],
#     [ptr, "src"],
#     [lv_color, "color"],
#     [int16_t, "r_mod"],
# ]
# lv_meter_indicator_arc = ct.structure(lv_meter_indicator_arc, "lv_meter_indicator_arc")

# lv_meter_indicator_scale_lines = [            # valid LVGL8.2
#     [ptr, "scale"],
#     [lv_meter_indicator_type_t, "type"],
#     [lv_opa, "opa"],
#     [int32_t, "start_value"],
#     [int32_t, "end_value"],
#     # specifc portion
#     [int16_t, "width_mod"],
#     [lv_color, "color_start"],
#     [lv_color, "color_end"],
#     [uint8_t_1, "local_grad"],
# ]
# lv_meter_indicator_scale_lines = ct.structure(lv_meter_indicator_scale_lines, "lv_meter_indicator_scale_lines")

lv_chart_series = [            # valid LVGL8.3
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
lv_chart_series = ct.structure(lv_chart_series, "lv_chart_series")

lv_chart_cursor = [            # valid LVGL8.3
    [lv_point, "pos"],
    [uint16_t, "point_id"],
    [lv_color, "color"],
    [ptr, "ser"],
    [lv_dir, "dir"],
    [uint8_t_1, "pos_set"],
]
lv_chart_cursor = ct.structure(lv_chart_cursor, "lv_chart_cursor")

lv_chart_tick_dsc = [            # valid LVGL8.3
    [lv_coord_t, "major_len"],
    [lv_coord_t, "minor_len"],
    [lv_coord_t, "draw_size"],
    [uint16_t_15, "minor_cnt"],
    [uint16_t_15, "major_cnt"],
    [uint8_t_1, "label_en"],
]
lv_chart_tick_dsc = ct.structure(lv_chart_tick_dsc, "lv_chart_tick_dsc")


#- --------- class system --------- -#
lv_obj_class_ptr = ptr
lv_obj_ptr = ptr
lv_event_ptr = ptr
constructor_cb = ptr    # callback
destructor_cb = ptr    # callback
event_cb = ptr    # callback
lv_event_code = ct.i32

lv_obj_class = [            # valid LVGL8.3
    [lv_obj_class_ptr, "base_class"],
    [constructor_cb, "constructor_cb"],
    [destructor_cb, "destructor_cb"],
    [ptr, "user_data"],
    [event_cb, "event_cb"],
    [lv_coord_t, "width_def"],
    [lv_coord_t, "height_def"],
    [uint8_t_2, "editable"],
    [uint8_t_2, "group_def"],
    [uint16_t_16, "instance_size"],
]
lv_obj_class = ct.structure(lv_obj_class, "lv_obj_class")

lv_event = [            # valid LVGL8.3
    [lv_obj_ptr, "target"],
    [lv_obj_ptr, "current_target"],
    [lv_event_code, "code"],
    [ptr, "user_data"],
    [ptr, "param"],
    [lv_event_ptr, "prev"],
    [uint8_t_1, "deleted"],
    [uint8_t_1, "stop_processing"],
    [uint8_t_1, "stop_bubbling"],
]
lv_event = ct.structure(lv_event, "lv_event")

lv_sqrt_res = [            # valid LVGL8.3
    [uint16_t, "i"],
    [uint16_t, "f"],
]
lv_sqrt_res = ct.structure(lv_sqrt_res, "lv_sqrt_res")

#######################################################################
# lv_img structures
lv_img_header = [            # valid LVGL8.3
    [uint8_t_5, "cf"],
    [uint8_t_3, "always_zero"],
    [uint8_t_2, "reserved"],
    [uint8_t_11, "w"],
    [uint8_t_11, "h"],
]
lv_img_header = ct.structure(lv_img_header, "lv_img_header")

lv_img_dsc = [            # valid LVGL8.3
    [lv_img_header, "header"],
    [uint32_t, "data_size"],
    [ptr, "data"],
]
lv_img_dsc = ct.structure(lv_img_dsc, "lv_img_dsc")

#######################################################################
# lv_style
lv_style_transition_dsc = [            # valid LVGL8.3
    [ptr, "props"],
    [ptr, "user_data"],
    [ptr, "path_xcb"],
    [uint32_t, "time"],
    [uint32_t, "delay"],
]
lv_style_transition_dsc = ct.structure(lv_style_transition_dsc, "lv_style_transition_dsc")

#######################################################################
# lv_color
# lv_color_hsv = [            # valid LVGL8
#     [uint16_t, "h"],
#     [uint8_t, "s"],
#     [uint8_t, "v"],
# ]
# lv_color_hsv = ct.structure(lv_color_hsv, "lv_color_hsv")

lv_color_filter_dsc = [            # valid LVGL8.3
    [ptr, "filter_cb"],
    [ptr, "user_data"],
]
lv_color_filter_dsc = ct.structure(lv_color_filter_dsc, "lv_color_filter_dsc")

#######################################################################
# lv_timer native, superseded by lv_timer
lv_timer_ntv = [            # valid LVGL8.3
    [uint32_t, "period"],
    [uint32_t, "last_run"],
    [ptr, "timer_cb"],
    [ptr, "user_data"],
    [int32_t, "repeat_count"],
    [uint8_t_1, "paused"],
]
lv_timer_ntv = ct.structure(lv_timer_ntv, "lv_timer_ntv")

# #######################################################################
# lv_anim native structure, is superseded by lv_anim
lv_anim_ntv = [            # valid LVGL8.3
    [ptr, "var"],
    [ptr, "exec_cb"],
    [ptr, "start_cb"],
    [ptr, "ready_cb"],
    [ptr, "deleted_cb"],
    [ptr, "get_value_cb"],

    [ptr, "user_data"],

    [ptr, "path_cb"],
    [int32_t, "start_value"],
    [int32_t, "current_value"],
    [int32_t, "end_value"],
    [int32_t, "time"],
    [int32_t, "act_time"],
    [uint32_t, "playback_delay"],
    [uint32_t, "playback_time"],
    [uint32_t, "repeat_delay"],
    [uint32_t, "repeat_cnt"],

    [uint8_t_1, "early_apply"],
    [uint8_t_1, "playback_now"],
    [uint8_t_1, "run_round"],
    [uint8_t_1, "start_cb_called"],
    
]
lv_anim_ntv = ct.structure(lv_anim_ntv, "lv_anim_ntv")

#######################################################################
# lv_draw_ctx
lv_draw_ctx = [            # valid LVGL8.3
    [ptr, "buf"],
    [lv_area, "buf_area"],
    [lv_area, "clip_area"],
    [ptr, "draw_rect"],
    [ptr, "draw_arc"],
    [ptr, "draw_img_decoded"],
    [ptr, "draw_img"],
    [ptr, "draw_letter"],
    [ptr, "draw_line"],
    [ptr, "draw_polygon"],
    [ptr, "draw_transform"],
    [ptr, "draw_bg"],
    [ptr, "wait_for_finish"],
    [ptr, "buffer_copy"],
    [ptr, "layer_init"],
    [ptr, "layer_adjust"],
    [ptr, "layer_blend"],
    [ptr, "layer_destroy"],
    [uint32_t, "layer_instance_size"],
    [ptr, "user_data"],
]
lv_draw_ctx = ct.structure(lv_draw_ctx, "lv_draw_ctx")

#######################################################################
# Special structure used to calibrate resistive touchscreens
#######################################################################
lv_ts_calibration = [            # valid LVGL8.3
    [lv_coord_t, "raw_x"],
    [lv_coord_t, "raw_y"],
    [lv_coord_t, "x"],
    [lv_coord_t, "y"],
    [int32_t, "state"],
]
lv_ts_calibration = ct.structure(lv_ts_calibration, "lv_ts_calibration")

#
ct.print_classes("lvgl")
