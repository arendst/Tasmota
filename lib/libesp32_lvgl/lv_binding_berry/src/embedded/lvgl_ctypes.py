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
lv_color = [ct.u24, "lv.color"]        # cast to the class instance, constructor is called with 2 args: (nil, value)
lv_grad_dir = ct.u8
lv_meter_indicator_type_t = ct.u8
lv_opa = ct.u8
lv_blend_mode = ct.u8
lv_align = ct.u8
lv_bidi_dir = ct.u8
lv_txt_flag = ct.u8
lv_text_decor = ct.u8
lv_font = ct.u32
lv_blend_mode = ct.u8
lv_draw_mask_line_side = ct.u8
lv_dir = ct.u8
lv_coord_t = ct.i32
lv_value_precise = ct.i32
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
size_t = ct.u32
ptr = ct.ptr32

lv_point = [            # valid LVGL9
    [int32_t, "x"],
    [int32_t, "y"],
]
lv_point = ct.structure(lv_point, "lv_point")

# typedef struct {
#     lv_value_precise_t x;
#     lv_value_precise_t y;
# } lv_point_precise_t;
lv_point_precise = [            # valid LVGL9
    [lv_value_precise, "x"],
    [lv_value_precise, "y"],
]
lv_point_precise = ct.structure(lv_point_precise, "lv_point_precise")

lv_area = [            # valid LVGL9
    [int32_t, "x1"],
    [int32_t, "y1"],
    [int32_t, "x2"],
    [int32_t, "y2"],
]
lv_area = ct.structure(lv_area, "lv_area")

#######################################################################
#
# typedef struct {
#     lv_color_t color;   /**< The stop color */
#     lv_opa_t   opa;     /**< The opacity of the color*/
#     uint8_t    frac;    /**< The stop position in 1/255 unit */
# } lv_gradient_stop_t;
lv_gradient_stop = [            # valid LVGL9
    [lv_color, "color"],
    [lv_opa, "opa"],
    [uint8_t, "frac"],
]
lv_gradient_stop = ct.structure(lv_gradient_stop, "lv_gradient_stop")

# typedef struct {
#     lv_gradient_stop_t   stops[LV_GRADIENT_MAX_STOPS]; /**< A gradient stop array */
#     uint8_t              stops_count;                  /**< The number of used stops in the array */
#     lv_grad_dir_t        dir : 3;                      /**< The gradient direction.
#                                                         * Any of LV_GRAD_DIR_HOR, LV_GRAD_DIR_VER, LV_GRAD_DIR_NONE */
# } lv_grad_dsc_t;
lv_grad_dsc = [            # valid LVGL9
    [lv_gradient_stop, "stops_0"],
    [lv_gradient_stop, "stops_1"],
    [uint8_t, "stops_count"],
    [uint8_t_3, "dir"],
]
lv_grad_dsc = ct.structure(lv_grad_dsc, "lv_grad_dsc")


# typedef struct {
#     lv_obj_t * obj;
#     uint32_t part;
#     uint32_t id1;
#     uint32_t id2;
#     lv_layer_t * layer;
#     size_t dsc_size;
#     void * user_data;
# } lv_draw_dsc_base_t;
lv_draw_dsc_base = [            # valid LVGL9
    [ptr, "obj"],
    [uint32_t, "part"],
    [uint32_t, "id1"],
    [uint32_t, "id2"],
    [ptr, "layer"],
    [size_t, "dsc_size"],
    [ptr, "user_data"],
]
lv_draw_dsc_base = ct.structure(lv_draw_dsc_base, "lv_draw_dsc_base")

# typedef struct {
#     lv_draw_dsc_base_t base;

#     int32_t radius;

#     /*Background*/
#     lv_opa_t bg_opa;
#     lv_color_t bg_color;        /**< First element of a gradient is a color, so it maps well here*/
#     lv_grad_dsc_t bg_grad;

#     /*Background img*/
#     const void * bg_image_src;
#     const void * bg_image_symbol_font;
#     lv_color_t bg_image_recolor;
#     lv_opa_t bg_image_opa;
#     lv_opa_t bg_image_recolor_opa;
#     uint8_t bg_image_tiled;

#     /*Border*/
#     lv_color_t border_color;
#     int32_t border_width;
#     lv_opa_t border_opa;
#     lv_border_side_t border_side : 5;
#     uint8_t border_post : 1; /*The border will be drawn later*/

#     /*Outline*/
#     lv_color_t outline_color;
#     int32_t outline_width;
#     int32_t outline_pad;
#     lv_opa_t outline_opa;

#     /*Shadow*/
#     lv_color_t shadow_color;
#     int32_t shadow_width;
#     int32_t shadow_offset_x;
#     int32_t shadow_offset_y;
#     int32_t shadow_spread;
#     lv_opa_t shadow_opa;
# } lv_draw_rect_dsc_t;
lv_draw_rect_dsc = [            # valid LVGL9
    [lv_draw_dsc_base, "base"],

    [int32_t, "radius"],

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
    [int32_t, "border_width"],
    [lv_opa, "border_opa"],
    [uint8_t_1, "border_post"],
    [uint8_t_5, "border_side"],

    #/*Outline*/
    [lv_color, "outline_color"],
    [int32_t, "outline_width"],
    [int32_t, "outline_pad"],
    [lv_opa, "outline_opa"],

    #/*Shadow*/
    [lv_color, "shadow_color"],
    [int32_t, "shadow_width"],
    [int32_t, "shadow_ofs_x"],
    [int32_t, "shadow_ofs_y"],
    [int32_t, "shadow_spread"],
    [lv_opa, "shadow_opa"],
]
lv_draw_rect_dsc = ct.structure(lv_draw_rect_dsc, "lv_draw_rect_dsc")

# typedef struct {
#     lv_draw_dsc_base_t base;

#     lv_point_precise_t p1;
#     lv_point_precise_t p2;
#     lv_color_t color;
#     int32_t width;
#     int32_t dash_width;
#     int32_t dash_gap;
#     lv_opa_t opa;
#     lv_blend_mode_t blend_mode  : 2;
#     uint8_t round_start : 1;
#     uint8_t round_end   : 1;
#     uint8_t raw_end     : 1;    /*Do not bother with perpendicular line ending if it's not visible for any reason*/
# } lv_draw_line_dsc_t;
lv_draw_line_dsc = [            # valid LVGL9
    [lv_draw_dsc_base, "base"],

    [lv_point_precise, "p1"],
    [lv_point_precise, "p2"],
    [lv_color, "color"],
    [int32_t, "width"],
    [int32_t, "dash_width"],
    [int32_t, "dash_gap"],
    [lv_opa, "opa"],
    [uint8_t_2, "blend_mode"],
    [uint8_t_1, "round_start"],
    [uint8_t_1, "round_end"],
    [uint8_t_1, "raw_end"],
]
lv_draw_line_dsc = ct.structure(lv_draw_line_dsc, "lv_draw_line_dsc")

# typedef struct {
#     lv_draw_dsc_base_t base;

#     lv_color_t color;
#     int32_t width;
#     lv_value_precise_t start_angle;
#     lv_value_precise_t end_angle;
#     lv_point_t center;
#     uint16_t radius;
#     const void * img_src;
#     lv_opa_t opa;
#     uint8_t rounded : 1;
# } lv_draw_arc_dsc_t;
lv_draw_arc_dsc = [            # valid LVGL8.3
    [lv_draw_dsc_base, "base"],

    [lv_color, "color"],
    [int32_t, "width"],
    [lv_value_precise, "start_angle"],
    [lv_value_precise, "end_angle"],
    [lv_point, "center"],
    [uint16_t, "radius"],
    [ptr, "img_src"],
    [lv_opa, "opa"],
    [uint8_t_1, "rounded"],
]
lv_draw_arc_dsc = ct.structure(lv_draw_arc_dsc, "lv_draw_arc_dsc")


# typedef struct {
#     uint32_t magic: 8;          /*Magic number. Must be LV_IMAGE_HEADER_MAGIC*/
#     uint32_t cf : 8;            /*Color format: See `lv_color_format_t`*/
#     uint32_t flags: 16;         /*Image flags, see `lv_image_flags_t`*/

#     uint32_t w: 16;
#     uint32_t h: 16;
#     uint32_t stride: 16;        /*Number of bytes in a row*/
#     uint32_t reserved_2: 16;    /*Reserved to be used later*/
# } lv_image_header_t;

lv_image_header = [            # valid LVGL9
    [uint8_t, "magic"],
    [uint8_t, "cf"],
    [uint16_t, "flags"],

    [uint16_t, "w"],
    [uint16_t, "h"],
    [uint16_t, "stride"],
    [uint16_t, "reserved_2"],
]
lv_image_header = ct.structure(lv_image_header, "lv_image_header")

# typedef struct _lv_draw_image_dsc_t {
#     lv_draw_dsc_base_t base;

#     const void * src;
#     lv_image_header_t header;

#     int32_t rotation;
#     int32_t scale_x;
#     int32_t scale_y;
#     int32_t skew_x;
#     int32_t skew_y;
#     lv_point_t pivot;

#     lv_color_t recolor;
#     lv_opa_t recolor_opa;

#     lv_opa_t opa;
#     lv_blend_mode_t blend_mode : 4;

#     uint16_t antialias      : 1;
#     uint16_t tile           : 1;
#     lv_draw_image_sup_t * sup;
# } lv_draw_image_dsc_t;
lv_draw_image_dsc = [            # valid LVGL9
    [lv_draw_dsc_base, "base"],

    [ptr, "src"],
    [lv_image_header, "header"],

    [int32_t, "rotation"],
    [int32_t, "scale_x"],
    [int32_t, "scale_y"],
    [int32_t, "skew_x"],
    [int32_t, "skew_y"],
    [lv_point, "pivot"],

    [lv_color, "recolor"],
    [lv_opa, "recolor_opa"],

    [lv_opa, "opa"],
    [uint8_t_4, "blend_mode"],
    [uint8_t_1, "antialias"],
    [uint8_t_1, "tile"],
    [ptr, "sup"],
]
lv_draw_image_dsc = ct.structure(lv_draw_image_dsc, "lv_draw_image_dsc")

# lv_obj_draw_part_dsc = [            # valid LVGL8.3
#     [ptr, "draw_ctx"],
#     [ptr, "class_p"],
#     [uint32_t, "type"],
#     [ptr, "draw_area"],
#     [ptr, "rect_dsc"],
#     [ptr, "label_dsc"],
#     [ptr, "line_dsc"],
#     [ptr, "img_dsc"],
#     [ptr, "arc_dsc"],
#     [ptr, "p1"],
#     [ptr, "p2"],
#     [ptr, "text"],
#     [uint32_t, "text_length"],
#     [uint32_t, "part"],
#     [uint32_t, "id"],
#     [lv_coord_t, "radius"],
#     [int32_t, "value"],
#     [ptr, "sub_part_ptr"],
# ]
# lv_obj_draw_part_dsc = ct.structure(lv_obj_draw_part_dsc, "lv_obj_draw_part_dsc")

# lv_draw_layer_ctx = [            # valid LVGL8.3
#     [lv_area, "area_full"],
#     [lv_area, "area_act"],
#     [lv_coord_t, "max_row_with_alpha"],
#     [lv_coord_t, "max_row_with_no_alpha"],
#     [ptr, "buf"],
#     [ptr, "clip_area"],
#     [ptr, "buf_area"],
#     [ptr, "buf"],
#     [uint8_t, "screen_transp"],
# ]
# lv_draw_layer_ctx = ct.structure(lv_draw_layer_ctx, "lv_draw_layer_ctx")

#- --------- lv_mask --------- -#
# lv_draw_mask_xcb = ptr    # callback
# lv_draw_mask_type = ct.u8
# lv_draw_mask_line_side = ct.u8

# lv_draw_mask_common_dsc = [            # valid LVGL8.3
#     [lv_draw_mask_xcb, "cb"],
#     [lv_draw_mask_type, "type"],
# ]
# lv_draw_mask_common_dsc = ct.structure(lv_draw_mask_common_dsc, "lv_draw_mask_common_dsc")

# lv_draw_mask_line_param_cfg = [            # valid LVGL8.3
#     #/*First point */
#     [lv_point, "p1"],

#     #/*Second point*/
#     [lv_point, "p2"],

#     #/*Which side to keep?*/
#     [uint8_t_2, "side"],
# ]
# lv_draw_mask_line_param_cfg = ct.structure(lv_draw_mask_line_param_cfg, "lv_draw_mask_line_param_cfg")

# lv_draw_mask_line_param = [            # valid LVGL8.3
#     #/*The first element must be the common descriptor*/
#     [lv_draw_mask_common_dsc, "dsc"],
#     [lv_draw_mask_line_param_cfg, "cfg"],
#     #/*A point of the line*/
#     [lv_point, "origo"],
#     #/* X / (1024*Y) steepness (X is 0..1023 range). What is the change of X in 1024 Y?*/
#     [int32_t, "xy_steep"],
#     #/* Y / (1024*X) steepness (Y is 0..1023 range). What is the change of Y in 1024 X?*/
#     [int32_t, "yx_steep"],
#     #/*Helper which stores yx_steep for flat lines and xy_steep for steep (non flat) lines */
#     [int32_t, "steep"],
#     #/*Steepness in 1 px in 0..255 range. Used only by flat lines. */
#     [int32_t, "spx"],
#     #/*1: It's a flat line? (Near to horizontal)*/
#     [uint8_t_1, "flat"],
#     #/* Invert the mask. The default is: Keep the left part.
#     # * It is used to select left/right/top/bottom*/
#     [uint8_t_1, "inv"],
# ]
# lv_draw_mask_line_param = ct.structure(lv_draw_mask_line_param, "lv_draw_mask_line_param")

# lv_draw_mask_angle_param_cfg = [            # valid LVGL8.3
#     [lv_point, "vertex_p"],
#     [lv_coord_t, "start_angle"],
#     [lv_coord_t, "end_angle"],
# ]
# lv_draw_mask_angle_param_cfg = ct.structure(lv_draw_mask_angle_param_cfg, "lv_draw_mask_angle_param_cfg")

# lv_draw_mask_angle_param = [            # valid LVGL8.3
#     #/*The first element must be the common descriptor*/
#     [lv_draw_mask_common_dsc, "dsc"],
#     [lv_draw_mask_angle_param_cfg, "cfg"],

#     [lv_draw_mask_line_param, "start_line"],
#     [lv_draw_mask_line_param, "end_line"],
#     [uint16_t, "delta_deg"],
# ]
# lv_draw_mask_angle_param = ct.structure(lv_draw_mask_angle_param, "lv_draw_mask_angle_param")


# lv_draw_mask_radius_param_cfg = [            # valid LVGL8.3
#     [lv_area, "rect"],
#     [lv_coord_t, "radius"],
#     [uint8_t_1, "outer"],
# ]
# lv_draw_mask_radius_param_cfg = ct.structure(lv_draw_mask_radius_param_cfg, "lv_draw_mask_radius_param_cfg")

# lv_draw_mask_radius_circle_dsc = [            # valid LVGL8.3
#     [ptr, "buf"],
#     [ptr, "cir_opa"],
#     [ptr, "x_start_on_y"],
#     [ptr, "opa_start_on_y"],
#     [int32_t, "life"],
#     [uint32_t, "used_cnt"],
#     [lv_coord_t, "radius"],
# ]
# lv_draw_mask_radius_circle_dsc = ct.structure(lv_draw_mask_radius_circle_dsc, "lv_draw_mask_radius_circle_dsc")

# lv_draw_mask_radius_param = [            # valid LVGL8.3
#     #/*The first element must be the common descriptor*/
#     [lv_draw_mask_common_dsc, "dsc"],
#     [lv_draw_mask_radius_param_cfg, "cfg"],
#     [lv_draw_mask_radius_circle_dsc, "circle"],
# ]
# lv_draw_mask_radius_param = ct.structure(lv_draw_mask_radius_param, "lv_draw_mask_radius_param")


# lv_draw_mask_fade_param_cfg = [            # valid LVGL8.3
#     [lv_area, "coords"],
#     [lv_coord_t, "y_top"],
#     [lv_coord_t, "y_bottom"],
#     [lv_opa, "opa_top"],
#     [lv_opa, "opa_bottom"],
# ]
# lv_draw_mask_fade_param_cfg = ct.structure(lv_draw_mask_fade_param_cfg, "lv_draw_mask_fade_param_cfg")

# lv_draw_mask_fade_param = [            # valid LVGL8.3
#     # /*The first element must be the common descriptor*/
#     [lv_draw_mask_common_dsc, "dsc"],
#     [lv_draw_mask_fade_param_cfg, "cfg"],
# ]
# lv_draw_mask_fade_param = ct.structure(lv_draw_mask_fade_param, "lv_draw_mask_fade_param")


# lv_draw_mask_map_param_cfg = [            # valid LVGL8.3
#     [lv_area, "coords"],
#     [ptr, "map"],
# ]
# lv_draw_mask_map_param_cfg = ct.structure(lv_draw_mask_map_param_cfg, "lv_draw_mask_map_param_cfg")

# lv_draw_mask_map_param = [            # valid LVGL8.3
#     [lv_draw_mask_common_dsc, "dsc"],
#     [lv_draw_mask_map_param_cfg, "cfg"],
# ]
# lv_draw_mask_map_param = ct.structure(lv_draw_mask_map_param, "lv_draw_mask_map_param")


# lv_draw_mask_polygon_param_cfg = [            # valid LVGL8.3
#     [ptr, "points"],
#     [uint16_t, "point_cnt"],
# ]
# lv_draw_mask_polygon_param_cfg = ct.structure(lv_draw_mask_polygon_param_cfg, "lv_draw_mask_polygon_param_cfg")

# lv_draw_mask_polygon_param = [            # valid LVGL8.3
#     [lv_draw_mask_common_dsc, "dsc"],
#     [lv_draw_mask_polygon_param_cfg, "cfg"],
# ]
# lv_draw_mask_polygon_param = ct.structure(lv_draw_mask_polygon_param, "lv_draw_mask_polygon_param")


# lv_draw_mask_saved = [            # valid LVGL8.3
#     [ptr, "param"],
#     [ptr, "custom_id"],
# ]
# lv_draw_mask_saved = ct.structure(lv_draw_mask_saved, "lv_draw_mask_saved")

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

# typedef struct {
#     int32_t * x_points;
#     int32_t * y_points;
#     lv_color_t color;
#     uint32_t start_point;
#     uint32_t hidden : 1;
#     uint32_t x_ext_buf_assigned : 1;
#     uint32_t y_ext_buf_assigned : 1;
#     uint32_t x_axis_sec : 1;
#     uint32_t y_axis_sec : 1;
# } lv_chart_series_t;
lv_chart_series = [            # valid LVGL9
    [ptr, "x_points"],
    [ptr, "y_points"],
    [lv_color, "color"],
    [uint32_t, "start_point"],
    [uint8_t_1, "hidden"],
    [uint8_t_1, "x_ext_buf_assigned"],
    [uint8_t_1, "y_ext_buf_assigned"],
    [uint8_t_1, "x_axis_sec"],
    [uint8_t_1, "y_axis_sec"],
]
lv_chart_series = ct.structure(lv_chart_series, "lv_chart_series")

# typedef struct {
#     lv_point_t pos;
#     int32_t point_id;
#     lv_color_t color;
#     lv_chart_series_t * ser;
#     lv_dir_t dir;
#     uint32_t pos_set: 1; /*1: pos is set; 0: point_id is set*/
# } lv_chart_cursor_t;
lv_chart_cursor = [            # valid LVGL9
    [lv_point, "pos"],
    [int32_t, "point_id"],
    [lv_color, "color"],
    [ptr, "ser"],
    [lv_dir, "dir"],
    [uint8_t_1, "pos_set"],
]
lv_chart_cursor = ct.structure(lv_chart_cursor, "lv_chart_cursor")

# lv_chart_tick_dsc = [            # valid LVGL8.3
#     [lv_coord_t, "major_len"],
#     [lv_coord_t, "minor_len"],
#     [lv_coord_t, "draw_size"],
#     [uint16_t_15, "minor_cnt"],
#     [uint16_t_15, "major_cnt"],
#     [uint8_t_1, "label_en"],
# ]
# lv_chart_tick_dsc = ct.structure(lv_chart_tick_dsc, "lv_chart_tick_dsc")


#- --------- class system --------- -#
lv_obj_class_ptr = ptr
lv_obj_ptr = ptr
lv_event_ptr = ptr
constructor_cb = ptr    # callback
destructor_cb = ptr    # callback
event_cb = ptr    # callback
lv_event_code = ct.i32

# struct _lv_obj_class_t {
#     const lv_obj_class_t * base_class;
#     /*class_p is the final class while obj->class_p is the class currently being [de]constructed.*/
#     void (*constructor_cb)(const lv_obj_class_t * class_p, lv_obj_t * obj);
#     void (*destructor_cb)(const lv_obj_class_t * class_p, lv_obj_t * obj);

#     /*class_p is the class in which event is being processed.*/
#     void (*event_cb)(const lv_obj_class_t * class_p, lv_event_t * e);  /**< Widget type specific event function*/

# #if LV_USE_OBJ_PROPERTY
#     uint32_t prop_index_start;
#     uint32_t prop_index_end;
#     const lv_property_ops_t * properties;
#     uint32_t properties_count;
# #endif

#     void * user_data;
#     const char * name;
#     int32_t width_def;
#     int32_t height_def;
#     uint32_t editable : 2;             /**< Value from ::lv_obj_class_editable_t*/
#     uint32_t group_def : 2;            /**< Value from ::lv_obj_class_group_def_t*/
#     uint32_t instance_size : 16;
#     uint32_t theme_inheritable : 1;    /**< Value from ::lv_obj_class_theme_inheritable_t*/
# };
lv_obj_class = [            # valid LVGL9
    [lv_obj_class_ptr, "base_class"],
    [constructor_cb, "constructor_cb"],
    [destructor_cb, "destructor_cb"],
    [event_cb, "event_cb"],
    [ptr, "user_data"],
    [ptr, "name"],
    [int32_t, "width_def"],
    [int32_t, "height_def"],
    [uint8_t_2, "editable"],
    [uint8_t_2, "group_def"],
    [uint16_t_16, "instance_size"],
    [uint8_t_1, "theme_inheritable"],
]
lv_obj_class = ct.structure(lv_obj_class, "lv_obj_class")

# struct _lv_event_t {
#     void * current_target;
#     void * original_target;
#     lv_event_code_t code;
#     void * user_data;
#     void * param;
#     lv_event_t * prev;
#     uint8_t deleted : 1;
#     uint8_t stop_processing : 1;
#     uint8_t stop_bubbling : 1;
# };
lv_event = [            # valid LVGL9
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

#######################################################################
# lv_image structures

# typedef struct {
#     lv_image_header_t header; /**< A header describing the basics of the image*/
#     uint32_t data_size;     /**< Size of the image in bytes*/
#     const uint8_t * data;   /**< Pointer to the data of the image*/
# } lv_image_dsc_t;
lv_image_dsc = [            # valid LVGL9
    [lv_image_header, "header"],
    [uint32_t, "data_size"],
    [ptr, "data"],
]
lv_image_dsc = ct.structure(lv_image_dsc, "lv_image_dsc")

#######################################################################
# lv_style
# typedef struct {
#     const lv_style_prop_t * props; /**< An array with the properties to animate.*/
#     void * user_data;              /**< A custom user data that will be passed to the animation's user_data */
#     lv_anim_path_cb_t path_xcb;     /**< A path for the animation.*/
#     uint32_t time;                 /**< Duration of the transition in [ms]*/
#     uint32_t delay;                /**< Delay before the transition in [ms]*/
# } lv_style_transition_dsc_t;
lv_style_transition_dsc = [            # valid LVGL9
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

# typedef struct _lv_color_filter_dsc_t {
#     lv_color_filter_cb_t filter_cb;
#     void * user_data;
# } lv_color_filter_dsc_t;
lv_color_filter_dsc = [            # valid LVGL9
    [ptr, "filter_cb"],
    [ptr, "user_data"],
]
lv_color_filter_dsc = ct.structure(lv_color_filter_dsc, "lv_color_filter_dsc")

#######################################################################
# lv_timer native, superseded by lv_timer

# struct _lv_timer_t {
#     uint32_t period; /**< How often the timer should run*/
#     uint32_t last_run; /**< Last time the timer ran*/
#     lv_timer_cb_t timer_cb; /**< Timer function*/
#     void * user_data; /**< Custom user data*/
#     int32_t repeat_count; /**< 1: One time;  -1 : infinity;  n>0: residual times*/
#     uint32_t paused : 1;
#     uint32_t auto_delete : 1;
# };
lv_timer_ntv = [            # valid LVGL9
    [uint32_t, "period"],
    [uint32_t, "last_run"],
    [ptr, "timer_cb"],
    [ptr, "user_data"],
    [int32_t, "repeat_count"],
    [uint8_t_1, "paused"],
    [uint8_t_1, "auto_delete"],
]
lv_timer_ntv = ct.structure(lv_timer_ntv, "lv_timer_ntv")

# #######################################################################
# lv_anim native structure, is superseded by lv_anim

# /** Describes an animation*/
# struct _lv_anim_t {
#     void * var;                                 /**<Variable to animate*/
#     lv_anim_exec_xcb_t exec_cb;                 /**< Function to execute to animate*/
#     lv_anim_custom_exec_cb_t custom_exec_cb;/**< Function to execute to animate,
#                                                  same purpose as exec_cb but different parameters*/
#     lv_anim_start_cb_t start_cb;         /**< Call it when the animation is starts (considering `delay`)*/
#     lv_anim_completed_cb_t completed_cb; /**< Call it when the animation is fully completed*/
#     lv_anim_deleted_cb_t deleted_cb;     /**< Call it when the animation is deleted*/
#     lv_anim_get_value_cb_t get_value_cb; /**< Get the current value in relative mode*/
#     void * user_data;                    /**< Custom user data*/
#     lv_anim_path_cb_t path_cb;         /**< Describe the path (curve) of animations*/
#     int32_t start_value;               /**< Start value*/
#     int32_t current_value;             /**< Current value*/
#     int32_t end_value;                 /**< End value*/
#     int32_t duration;                /**< Animation time in ms*/
#     int32_t act_time;            /**< Current time in animation. Set to negative to make delay.*/
#     uint32_t playback_delay;     /**< Wait before play back*/
#     uint32_t playback_duration;      /**< Duration of playback animation*/
#     uint32_t repeat_delay;       /**< Wait before repeat*/
#     uint16_t repeat_cnt;         /**< Repeat count for the animation*/
#     union _lv_anim_path_para_t {
#         lv_anim_bezier3_para_t bezier3; /**< Parameter used when path is custom_bezier*/
#     } parameter;

#     uint8_t early_apply  : 1;    /**< 1: Apply start value immediately even is there is `delay`*/

#     /*Animation system use these - user shouldn't set*/
#     uint32_t last_timer_run;
#     uint8_t playback_now : 1; /**< Play back is in progress*/
#     uint8_t run_round : 1;    /**< Indicates the animation has run in this round*/
#     uint8_t start_cb_called : 1;    /**< Indicates that the `start_cb` was already called*/
# };
# typedef struct _lv_anim_bezier3_para_t {
#     int16_t x1;
#     int16_t y1;
#     int16_t x2;
#     int16_t y2;
# } lv_anim_bezier3_para_t; /**< Parameter used when path is custom_bezier*/
# lv_anim_ntv = [            # valid LVGL8.3
#     [ptr, "var"],
#     [ptr, "exec_cb"],
#     [ptr, "custom_exec_cb"],
#     [ptr, "start_cb"],
#     [ptr, "completed_cb"],
#     [ptr, "deleted_cb"],
#     [ptr, "get_value_cb"],

#     [ptr, "user_data"],

#     [ptr, "path_cb"],
#     [int32_t, "start_value"],
#     [int32_t, "current_value"],
#     [int32_t, "end_value"],
#     [int32_t, "duration"],
#     [int32_t, "act_time"],
#     [uint32_t, "playback_delay"],
#     [uint32_t, "playback_duration"],
#     [uint32_t, "repeat_delay"],
#     [uint32_t, "repeat_cnt"],

#     [int16_t, "bezier3_x1"],
#     [int16_t, "bezier3_y1"],
#     [int16_t, "bezier3_x2"],
#     [int16_t, "bezier3_y2"],

#     [uint8_t_1, "early_apply"],

#     [uint32_t, "last_timer_run"],
#     [uint8_t_1, "playback_now"],
#     [uint8_t_1, "run_round"],
#     [uint8_t_1, "start_cb_called"],
    
# ]
# lv_anim_ntv = ct.structure(lv_anim_ntv, "lv_anim_ntv")

#######################################################################
# lv_draw_ctx
# lv_draw_ctx = [            # valid LVGL8.3
#     [ptr, "buf"],
#     [lv_area, "buf_area"],
#     [lv_area, "clip_area"],
#     [ptr, "draw_rect"],
#     [ptr, "draw_arc"],
#     [ptr, "draw_img_decoded"],
#     [ptr, "draw_img"],
#     [ptr, "draw_letter"],
#     [ptr, "draw_line"],
#     [ptr, "draw_polygon"],
#     [ptr, "draw_transform"],
#     [ptr, "draw_bg"],
#     [ptr, "wait_for_finish"],
#     [ptr, "buffer_copy"],
#     [ptr, "layer_init"],
#     [ptr, "layer_adjust"],
#     [ptr, "layer_blend"],
#     [ptr, "layer_destroy"],
#     [uint32_t, "layer_instance_size"],
#     [ptr, "user_data"],
# ]
# lv_draw_ctx = ct.structure(lv_draw_ctx, "lv_draw_ctx")

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
