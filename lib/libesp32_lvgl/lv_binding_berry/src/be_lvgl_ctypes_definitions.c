/********************************************************************
 * Tasmota LVGL ctypes mapping
 *******************************************************************/
#include "be_ctypes.h"
#include "lvgl.h"
#include "be_mapping.h"


/********************************************************************
 * Generated code, don't edit
 *******************************************************************/

static const char * be_ctypes_instance_mappings[];    /* forward definition */

const be_ctypes_structure_t be_lv_point = {
  8,  /* size in bytes */
  2,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[2]) {
    { "x", 0, 0, 0, ctypes_i32, 0 },
    { "y", 4, 0, 0, ctypes_i32, 0 },
}};

const be_ctypes_structure_t be_lv_point_precise = {
  8,  /* size in bytes */
  2,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[2]) {
    { "x", 0, 0, 0, ctypes_i32, 0 },
    { "y", 4, 0, 0, ctypes_i32, 0 },
}};

const be_ctypes_structure_t be_lv_area = {
  16,  /* size in bytes */
  4,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[4]) {
    { "x1", 0, 0, 0, ctypes_i32, 0 },
    { "x2", 8, 0, 0, ctypes_i32, 0 },
    { "y1", 4, 0, 0, ctypes_i32, 0 },
    { "y2", 12, 0, 0, ctypes_i32, 0 },
}};

const be_ctypes_structure_t be_lv_gradient_stop = {
  8,  /* size in bytes */
  3,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[3]) {
    { "color", 0, 0, 0, ctypes_u24, 1 },
    { "frac", 4, 0, 0, ctypes_u8, 0 },
    { "opa", 3, 0, 0, ctypes_u8, 0 },
}};

const be_ctypes_structure_t be_lv_grad_dsc = {
  16,  /* size in bytes */
  8,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[8]) {
    { "dir", 14, 0, 3, ctypes_bf, 0 },
    { "stops_0_color", 0, 0, 0, ctypes_u24, 1 },
    { "stops_0_frac", 4, 0, 0, ctypes_u8, 0 },
    { "stops_0_opa", 3, 0, 0, ctypes_u8, 0 },
    { "stops_1_color", 8, 0, 0, ctypes_u24, 1 },
    { "stops_1_frac", 12, 0, 0, ctypes_u8, 0 },
    { "stops_1_opa", 11, 0, 0, ctypes_u8, 0 },
    { "stops_count", 13, 0, 0, ctypes_u8, 0 },
}};

const be_ctypes_structure_t be_lv_draw_dsc_base = {
  28,  /* size in bytes */
  7,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[7]) {
    { "dsc_size", 20, 0, 0, ctypes_u32, 0 },
    { "id1", 8, 0, 0, ctypes_u32, 0 },
    { "id2", 12, 0, 0, ctypes_u32, 0 },
    { "layer", 16, 0, 0, ctypes_ptr32, 0 },
    { "obj", 0, 0, 0, ctypes_ptr32, 0 },
    { "part", 4, 0, 0, ctypes_u32, 0 },
    { "user_data", 24, 0, 0, ctypes_ptr32, 0 },
}};

const be_ctypes_structure_t be_lv_draw_rect_dsc = {
  124,  /* size in bytes */
  39,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[39]) {
    { "base_dsc_size", 20, 0, 0, ctypes_u32, 0 },
    { "base_id1", 8, 0, 0, ctypes_u32, 0 },
    { "base_id2", 12, 0, 0, ctypes_u32, 0 },
    { "base_layer", 16, 0, 0, ctypes_ptr32, 0 },
    { "base_obj", 0, 0, 0, ctypes_ptr32, 0 },
    { "base_part", 4, 0, 0, ctypes_u32, 0 },
    { "base_user_data", 24, 0, 0, ctypes_ptr32, 0 },
    { "bg_color", 36, 0, 0, ctypes_u24, 1 },
    { "bg_grad_dir", 54, 0, 3, ctypes_bf, 0 },
    { "bg_grad_stops_0_color", 40, 0, 0, ctypes_u24, 1 },
    { "bg_grad_stops_0_frac", 44, 0, 0, ctypes_u8, 0 },
    { "bg_grad_stops_0_opa", 43, 0, 0, ctypes_u8, 0 },
    { "bg_grad_stops_1_color", 48, 0, 0, ctypes_u24, 1 },
    { "bg_grad_stops_1_frac", 52, 0, 0, ctypes_u8, 0 },
    { "bg_grad_stops_1_opa", 51, 0, 0, ctypes_u8, 0 },
    { "bg_grad_stops_count", 53, 0, 0, ctypes_u8, 0 },
    { "bg_img_opa", 67, 0, 0, ctypes_u8, 0 },
    { "bg_img_recolor", 64, 0, 0, ctypes_u24, 1 },
    { "bg_img_recolor_opa", 68, 0, 0, ctypes_u8, 0 },
    { "bg_img_src", 56, 0, 0, ctypes_ptr32, 0 },
    { "bg_img_symbol_font", 60, 0, 0, ctypes_ptr32, 0 },
    { "bg_img_tiled", 69, 0, 0, ctypes_u8, 0 },
    { "bg_opa", 32, 0, 0, ctypes_u8, 0 },
    { "border_color", 72, 0, 0, ctypes_u24, 1 },
    { "border_opa", 80, 0, 0, ctypes_u8, 0 },
    { "border_post", 81, 0, 1, ctypes_bf, 0 },
    { "border_side", 81, 1, 5, ctypes_bf, 0 },
    { "border_width", 76, 0, 0, ctypes_i32, 0 },
    { "outline_color", 84, 0, 0, ctypes_u24, 1 },
    { "outline_opa", 96, 0, 0, ctypes_u8, 0 },
    { "outline_pad", 92, 0, 0, ctypes_i32, 0 },
    { "outline_width", 88, 0, 0, ctypes_i32, 0 },
    { "radius", 28, 0, 0, ctypes_i32, 0 },
    { "shadow_color", 100, 0, 0, ctypes_u24, 1 },
    { "shadow_ofs_x", 108, 0, 0, ctypes_i32, 0 },
    { "shadow_ofs_y", 112, 0, 0, ctypes_i32, 0 },
    { "shadow_opa", 120, 0, 0, ctypes_u8, 0 },
    { "shadow_spread", 116, 0, 0, ctypes_i32, 0 },
    { "shadow_width", 104, 0, 0, ctypes_i32, 0 },
}};

const be_ctypes_structure_t be_lv_draw_line_dsc = {
  64,  /* size in bytes */
  20,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[20]) {
    { "base_dsc_size", 20, 0, 0, ctypes_u32, 0 },
    { "base_id1", 8, 0, 0, ctypes_u32, 0 },
    { "base_id2", 12, 0, 0, ctypes_u32, 0 },
    { "base_layer", 16, 0, 0, ctypes_ptr32, 0 },
    { "base_obj", 0, 0, 0, ctypes_ptr32, 0 },
    { "base_part", 4, 0, 0, ctypes_u32, 0 },
    { "base_user_data", 24, 0, 0, ctypes_ptr32, 0 },
    { "blend_mode", 61, 0, 2, ctypes_bf, 0 },
    { "color", 44, 0, 0, ctypes_u24, 1 },
    { "dash_gap", 56, 0, 0, ctypes_i32, 0 },
    { "dash_width", 52, 0, 0, ctypes_i32, 0 },
    { "opa", 60, 0, 0, ctypes_u8, 0 },
    { "p1_x", 28, 0, 0, ctypes_i32, 0 },
    { "p1_y", 32, 0, 0, ctypes_i32, 0 },
    { "p2_x", 36, 0, 0, ctypes_i32, 0 },
    { "p2_y", 40, 0, 0, ctypes_i32, 0 },
    { "raw_end", 61, 4, 1, ctypes_bf, 0 },
    { "round_end", 61, 3, 1, ctypes_bf, 0 },
    { "round_start", 61, 2, 1, ctypes_bf, 0 },
    { "width", 48, 0, 0, ctypes_i32, 0 },
}};

const be_ctypes_structure_t be_lv_draw_arc_dsc = {
  64,  /* size in bytes */
  17,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[17]) {
    { "base_dsc_size", 20, 0, 0, ctypes_u32, 0 },
    { "base_id1", 8, 0, 0, ctypes_u32, 0 },
    { "base_id2", 12, 0, 0, ctypes_u32, 0 },
    { "base_layer", 16, 0, 0, ctypes_ptr32, 0 },
    { "base_obj", 0, 0, 0, ctypes_ptr32, 0 },
    { "base_part", 4, 0, 0, ctypes_u32, 0 },
    { "base_user_data", 24, 0, 0, ctypes_ptr32, 0 },
    { "center_x", 44, 0, 0, ctypes_i32, 0 },
    { "center_y", 48, 0, 0, ctypes_i32, 0 },
    { "color", 28, 0, 0, ctypes_u24, 1 },
    { "end_angle", 40, 0, 0, ctypes_i32, 0 },
    { "img_src", 56, 0, 0, ctypes_ptr32, 0 },
    { "opa", 60, 0, 0, ctypes_u8, 0 },
    { "radius", 52, 0, 0, ctypes_u16, 0 },
    { "rounded", 61, 0, 1, ctypes_bf, 0 },
    { "start_angle", 36, 0, 0, ctypes_i32, 0 },
    { "width", 32, 0, 0, ctypes_i32, 0 },
}};

const be_ctypes_structure_t be_lv_image_header = {
  12,  /* size in bytes */
  7,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[7]) {
    { "cf", 1, 0, 0, ctypes_u8, 0 },
    { "flags", 2, 0, 0, ctypes_u16, 0 },
    { "h", 6, 0, 0, ctypes_u16, 0 },
    { "magic", 0, 0, 0, ctypes_u8, 0 },
    { "reserved_2", 10, 0, 0, ctypes_u16, 0 },
    { "stride", 8, 0, 0, ctypes_u16, 0 },
    { "w", 4, 0, 0, ctypes_u16, 0 },
}};

const be_ctypes_structure_t be_lv_draw_image_dsc = {
  84,  /* size in bytes */
  29,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[29]) {
    { "antialias", 77, 4, 1, ctypes_bf, 0 },
    { "base_dsc_size", 20, 0, 0, ctypes_u32, 0 },
    { "base_id1", 8, 0, 0, ctypes_u32, 0 },
    { "base_id2", 12, 0, 0, ctypes_u32, 0 },
    { "base_layer", 16, 0, 0, ctypes_ptr32, 0 },
    { "base_obj", 0, 0, 0, ctypes_ptr32, 0 },
    { "base_part", 4, 0, 0, ctypes_u32, 0 },
    { "base_user_data", 24, 0, 0, ctypes_ptr32, 0 },
    { "blend_mode", 77, 0, 4, ctypes_bf, 0 },
    { "header_cf", 33, 0, 0, ctypes_u8, 0 },
    { "header_flags", 34, 0, 0, ctypes_u16, 0 },
    { "header_h", 38, 0, 0, ctypes_u16, 0 },
    { "header_magic", 32, 0, 0, ctypes_u8, 0 },
    { "header_reserved_2", 42, 0, 0, ctypes_u16, 0 },
    { "header_stride", 40, 0, 0, ctypes_u16, 0 },
    { "header_w", 36, 0, 0, ctypes_u16, 0 },
    { "opa", 76, 0, 0, ctypes_u8, 0 },
    { "pivot_x", 64, 0, 0, ctypes_i32, 0 },
    { "pivot_y", 68, 0, 0, ctypes_i32, 0 },
    { "recolor", 72, 0, 0, ctypes_u24, 1 },
    { "recolor_opa", 75, 0, 0, ctypes_u8, 0 },
    { "rotation", 44, 0, 0, ctypes_i32, 0 },
    { "scale_x", 48, 0, 0, ctypes_i32, 0 },
    { "scale_y", 52, 0, 0, ctypes_i32, 0 },
    { "skew_x", 56, 0, 0, ctypes_i32, 0 },
    { "skew_y", 60, 0, 0, ctypes_i32, 0 },
    { "src", 28, 0, 0, ctypes_ptr32, 0 },
    { "sup", 80, 0, 0, ctypes_ptr32, 0 },
    { "tile", 77, 5, 1, ctypes_bf, 0 },
}};

const be_ctypes_structure_t be_lv_meter_scale = {
  40,  /* size in bytes */
  14,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[14]) {
    { "angle_range", 34, 0, 0, ctypes_u16, 0 },
    { "label_gap", 22, 0, 0, ctypes_i16, 0 },
    { "max", 28, 0, 0, ctypes_i32, 0 },
    { "min", 24, 0, 0, ctypes_i32, 0 },
    { "r_mod", 32, 0, 0, ctypes_i16, 0 },
    { "rotation", 36, 0, 0, ctypes_i16, 0 },
    { "tick_cnt", 4, 0, 0, ctypes_u16, 0 },
    { "tick_color", 0, 0, 0, ctypes_u24, 1 },
    { "tick_length", 6, 0, 0, ctypes_u16, 0 },
    { "tick_major_color", 12, 0, 0, ctypes_u24, 1 },
    { "tick_major_length", 18, 0, 0, ctypes_u16, 0 },
    { "tick_major_nth", 16, 0, 0, ctypes_u16, 0 },
    { "tick_major_width", 20, 0, 0, ctypes_u16, 0 },
    { "tick_width", 8, 0, 0, ctypes_u16, 0 },
}};

const be_ctypes_structure_t be_lv_meter_indicator = {
  28,  /* size in bytes */
  8,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[8]) {
    { "data0", 16, 0, 0, ctypes_ptr32, 0 },
    { "data1", 20, 0, 0, ctypes_ptr32, 0 },
    { "data2", 24, 0, 0, ctypes_ptr32, 0 },
    { "end_value", 12, 0, 0, ctypes_i32, 0 },
    { "opa", 5, 0, 0, ctypes_u8, 0 },
    { "scale", 0, 0, 0, ctypes_ptr32, 0 },
    { "start_value", 8, 0, 0, ctypes_i32, 0 },
    { "type", 4, 0, 0, ctypes_u8, 0 },
}};

const be_ctypes_structure_t be_lv_chart_series = {
  20,  /* size in bytes */
  9,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[9]) {
    { "color", 8, 0, 0, ctypes_u24, 1 },
    { "hidden", 16, 0, 1, ctypes_bf, 0 },
    { "start_point", 12, 0, 0, ctypes_u32, 0 },
    { "x_axis_sec", 16, 3, 1, ctypes_bf, 0 },
    { "x_ext_buf_assigned", 16, 1, 1, ctypes_bf, 0 },
    { "x_points", 0, 0, 0, ctypes_ptr32, 0 },
    { "y_axis_sec", 16, 4, 1, ctypes_bf, 0 },
    { "y_ext_buf_assigned", 16, 2, 1, ctypes_bf, 0 },
    { "y_points", 4, 0, 0, ctypes_ptr32, 0 },
}};

const be_ctypes_structure_t be_lv_chart_cursor = {
  24,  /* size in bytes */
  7,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[7]) {
    { "color", 12, 0, 0, ctypes_u24, 1 },
    { "dir", 20, 0, 0, ctypes_u8, 0 },
    { "point_id", 8, 0, 0, ctypes_i32, 0 },
    { "pos_set", 21, 0, 1, ctypes_bf, 0 },
    { "pos_x", 0, 0, 0, ctypes_i32, 0 },
    { "pos_y", 4, 0, 0, ctypes_i32, 0 },
    { "ser", 16, 0, 0, ctypes_ptr32, 0 },
}};

const be_ctypes_structure_t be_lv_obj_class = {
  36,  /* size in bytes */
  12,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[12]) {
    { "base_class", 0, 0, 0, ctypes_ptr32, 0 },
    { "constructor_cb", 4, 0, 0, ctypes_ptr32, 0 },
    { "destructor_cb", 8, 0, 0, ctypes_ptr32, 0 },
    { "editable", 32, 0, 2, ctypes_bf, 0 },
    { "event_cb", 12, 0, 0, ctypes_ptr32, 0 },
    { "group_def", 32, 2, 2, ctypes_bf, 0 },
    { "height_def", 28, 0, 0, ctypes_i32, 0 },
    { "instance_size", 32, 4, 16, ctypes_bf, 0 },
    { "name", 20, 0, 0, ctypes_ptr32, 0 },
    { "theme_inheritable", 34, 4, 1, ctypes_bf, 0 },
    { "user_data", 16, 0, 0, ctypes_ptr32, 0 },
    { "width_def", 24, 0, 0, ctypes_i32, 0 },
}};

const be_ctypes_structure_t be_lv_event = {
  28,  /* size in bytes */
  9,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[9]) {
    { "code", 8, 0, 0, ctypes_i32, 0 },
    { "current_target", 4, 0, 0, ctypes_ptr32, 0 },
    { "deleted", 24, 0, 1, ctypes_bf, 0 },
    { "param", 16, 0, 0, ctypes_ptr32, 0 },
    { "prev", 20, 0, 0, ctypes_ptr32, 0 },
    { "stop_bubbling", 24, 2, 1, ctypes_bf, 0 },
    { "stop_processing", 24, 1, 1, ctypes_bf, 0 },
    { "target", 0, 0, 0, ctypes_ptr32, 0 },
    { "user_data", 12, 0, 0, ctypes_ptr32, 0 },
}};

const be_ctypes_structure_t be_lv_image_dsc = {
  20,  /* size in bytes */
  9,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[9]) {
    { "data", 16, 0, 0, ctypes_ptr32, 0 },
    { "data_size", 12, 0, 0, ctypes_u32, 0 },
    { "header_cf", 1, 0, 0, ctypes_u8, 0 },
    { "header_flags", 2, 0, 0, ctypes_u16, 0 },
    { "header_h", 6, 0, 0, ctypes_u16, 0 },
    { "header_magic", 0, 0, 0, ctypes_u8, 0 },
    { "header_reserved_2", 10, 0, 0, ctypes_u16, 0 },
    { "header_stride", 8, 0, 0, ctypes_u16, 0 },
    { "header_w", 4, 0, 0, ctypes_u16, 0 },
}};

const be_ctypes_structure_t be_lv_style_transition_dsc = {
  20,  /* size in bytes */
  5,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[5]) {
    { "delay", 16, 0, 0, ctypes_u32, 0 },
    { "path_xcb", 8, 0, 0, ctypes_ptr32, 0 },
    { "props", 0, 0, 0, ctypes_ptr32, 0 },
    { "time", 12, 0, 0, ctypes_u32, 0 },
    { "user_data", 4, 0, 0, ctypes_ptr32, 0 },
}};

const be_ctypes_structure_t be_lv_color_filter_dsc = {
  8,  /* size in bytes */
  2,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[2]) {
    { "filter_cb", 0, 0, 0, ctypes_ptr32, 0 },
    { "user_data", 4, 0, 0, ctypes_ptr32, 0 },
}};

const be_ctypes_structure_t be_lv_timer_ntv = {
  24,  /* size in bytes */
  7,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[7]) {
    { "auto_delete", 20, 1, 1, ctypes_bf, 0 },
    { "last_run", 4, 0, 0, ctypes_u32, 0 },
    { "paused", 20, 0, 1, ctypes_bf, 0 },
    { "period", 0, 0, 0, ctypes_u32, 0 },
    { "repeat_count", 16, 0, 0, ctypes_i32, 0 },
    { "timer_cb", 8, 0, 0, ctypes_ptr32, 0 },
    { "user_data", 12, 0, 0, ctypes_ptr32, 0 },
}};

const be_ctypes_structure_t be_lv_ts_calibration = {
  20,  /* size in bytes */
  5,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[5]) {
    { "raw_x", 0, 0, 0, ctypes_i32, 0 },
    { "raw_y", 4, 0, 0, ctypes_i32, 0 },
    { "state", 16, 0, 0, ctypes_i32, 0 },
    { "x", 8, 0, 0, ctypes_i32, 0 },
    { "y", 12, 0, 0, ctypes_i32, 0 },
}};

static const char * be_ctypes_instance_mappings[] = {
  "lv.color",
  NULL
};

static be_define_ctypes_class(lv_area, &be_lv_area, &be_class_ctypes_bytes, "lv_area");
static be_define_ctypes_class(lv_chart_cursor, &be_lv_chart_cursor, &be_class_ctypes_bytes, "lv_chart_cursor");
static be_define_ctypes_class(lv_chart_series, &be_lv_chart_series, &be_class_ctypes_bytes, "lv_chart_series");
static be_define_ctypes_class(lv_color_filter_dsc, &be_lv_color_filter_dsc, &be_class_ctypes_bytes, "lv_color_filter_dsc");
static be_define_ctypes_class(lv_draw_arc_dsc, &be_lv_draw_arc_dsc, &be_class_ctypes_bytes, "lv_draw_arc_dsc");
static be_define_ctypes_class(lv_draw_dsc_base, &be_lv_draw_dsc_base, &be_class_ctypes_bytes, "lv_draw_dsc_base");
static be_define_ctypes_class(lv_draw_image_dsc, &be_lv_draw_image_dsc, &be_class_ctypes_bytes, "lv_draw_image_dsc");
static be_define_ctypes_class(lv_draw_line_dsc, &be_lv_draw_line_dsc, &be_class_ctypes_bytes, "lv_draw_line_dsc");
static be_define_ctypes_class(lv_draw_rect_dsc, &be_lv_draw_rect_dsc, &be_class_ctypes_bytes, "lv_draw_rect_dsc");
static be_define_ctypes_class(lv_event, &be_lv_event, &be_class_ctypes_bytes, "lv_event");
static be_define_ctypes_class(lv_grad_dsc, &be_lv_grad_dsc, &be_class_ctypes_bytes, "lv_grad_dsc");
static be_define_ctypes_class(lv_gradient_stop, &be_lv_gradient_stop, &be_class_ctypes_bytes, "lv_gradient_stop");
static be_define_ctypes_class(lv_image_dsc, &be_lv_image_dsc, &be_class_ctypes_bytes, "lv_image_dsc");
static be_define_ctypes_class(lv_image_header, &be_lv_image_header, &be_class_ctypes_bytes, "lv_image_header");
static be_define_ctypes_class(lv_meter_indicator, &be_lv_meter_indicator, &be_class_ctypes_bytes, "lv_meter_indicator");
static be_define_ctypes_class(lv_meter_scale, &be_lv_meter_scale, &be_class_ctypes_bytes, "lv_meter_scale");
static be_define_ctypes_class(lv_obj_class, &be_lv_obj_class, &be_class_ctypes_bytes, "lv_obj_class");
static be_define_ctypes_class(lv_point, &be_lv_point, &be_class_ctypes_bytes, "lv_point");
static be_define_ctypes_class(lv_point_precise, &be_lv_point_precise, &be_class_ctypes_bytes, "lv_point_precise");
static be_define_ctypes_class(lv_style_transition_dsc, &be_lv_style_transition_dsc, &be_class_ctypes_bytes, "lv_style_transition_dsc");
static be_define_ctypes_class(lv_timer_ntv, &be_lv_timer_ntv, &be_class_ctypes_bytes, "lv_timer_ntv");
static be_define_ctypes_class(lv_ts_calibration, &be_lv_ts_calibration, &be_class_ctypes_bytes, "lv_ts_calibration");

void be_load_ctypes_lvgl_definitions_lib(bvm *vm) {
  ctypes_register_class(vm, &be_class_lv_area);
  ctypes_register_class(vm, &be_class_lv_chart_cursor);
  ctypes_register_class(vm, &be_class_lv_chart_series);
  ctypes_register_class(vm, &be_class_lv_color_filter_dsc);
  ctypes_register_class(vm, &be_class_lv_draw_arc_dsc);
  ctypes_register_class(vm, &be_class_lv_draw_dsc_base);
  ctypes_register_class(vm, &be_class_lv_draw_image_dsc);
  ctypes_register_class(vm, &be_class_lv_draw_line_dsc);
  ctypes_register_class(vm, &be_class_lv_draw_rect_dsc);
  ctypes_register_class(vm, &be_class_lv_event);
  ctypes_register_class(vm, &be_class_lv_grad_dsc);
  ctypes_register_class(vm, &be_class_lv_gradient_stop);
  ctypes_register_class(vm, &be_class_lv_image_dsc);
  ctypes_register_class(vm, &be_class_lv_image_header);
  ctypes_register_class(vm, &be_class_lv_meter_indicator);
  ctypes_register_class(vm, &be_class_lv_meter_scale);
  ctypes_register_class(vm, &be_class_lv_obj_class);
  ctypes_register_class(vm, &be_class_lv_point);
  ctypes_register_class(vm, &be_class_lv_point_precise);
  ctypes_register_class(vm, &be_class_lv_style_transition_dsc);
  ctypes_register_class(vm, &be_class_lv_timer_ntv);
  ctypes_register_class(vm, &be_class_lv_ts_calibration);
}

be_ctypes_class_by_name_t be_ctypes_lvgl_classes[] = {
  { "lv_area", &be_class_lv_area },
  { "lv_chart_cursor", &be_class_lv_chart_cursor },
  { "lv_chart_series", &be_class_lv_chart_series },
  { "lv_color_filter_dsc", &be_class_lv_color_filter_dsc },
  { "lv_draw_arc_dsc", &be_class_lv_draw_arc_dsc },
  { "lv_draw_dsc_base", &be_class_lv_draw_dsc_base },
  { "lv_draw_image_dsc", &be_class_lv_draw_image_dsc },
  { "lv_draw_line_dsc", &be_class_lv_draw_line_dsc },
  { "lv_draw_rect_dsc", &be_class_lv_draw_rect_dsc },
  { "lv_event", &be_class_lv_event },
  { "lv_grad_dsc", &be_class_lv_grad_dsc },
  { "lv_gradient_stop", &be_class_lv_gradient_stop },
  { "lv_image_dsc", &be_class_lv_image_dsc },
  { "lv_image_header", &be_class_lv_image_header },
  { "lv_meter_indicator", &be_class_lv_meter_indicator },
  { "lv_meter_scale", &be_class_lv_meter_scale },
  { "lv_obj_class", &be_class_lv_obj_class },
  { "lv_point", &be_class_lv_point },
  { "lv_point_precise", &be_class_lv_point_precise },
  { "lv_style_transition_dsc", &be_class_lv_style_transition_dsc },
  { "lv_timer_ntv", &be_class_lv_timer_ntv },
  { "lv_ts_calibration", &be_class_lv_ts_calibration },
};
const size_t be_ctypes_lvgl_classes_size = sizeof(be_ctypes_lvgl_classes)/sizeof(be_ctypes_lvgl_classes[0]);

/********************************************************************/
