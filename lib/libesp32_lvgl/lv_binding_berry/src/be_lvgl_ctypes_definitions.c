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

const be_ctypes_structure_t be_lv_grad_stop = {
  8,  /* size in bytes */
  3,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[3]) {
    { "color", 0, 0, 0, ctypes_u24, 1 },
    { "frac", 4, 0, 0, ctypes_u8, 0 },
    { "opa", 3, 0, 0, ctypes_u8, 0 },
}};

const be_ctypes_structure_t be_lv_grad_dsc = {
  48,  /* size in bytes */
  18,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[18]) {
    { "dir", 11, 0, 4, ctypes_bf, 0 },
    { "end_extent_x", 36, 0, 0, ctypes_i32, 0 },
    { "end_extent_y", 40, 0, 0, ctypes_i32, 0 },
    { "end_x", 28, 0, 0, ctypes_i32, 0 },
    { "end_y", 32, 0, 0, ctypes_i32, 0 },
    { "extend", 11, 4, 3, ctypes_bf, 0 },
    { "focal_extent_x", 20, 0, 0, ctypes_i32, 0 },
    { "focal_extent_y", 24, 0, 0, ctypes_i32, 0 },
    { "focal_x", 12, 0, 0, ctypes_i32, 0 },
    { "focal_y", 16, 0, 0, ctypes_i32, 0 },
    { "state", 44, 0, 0, ctypes_ptr32, 0 },
    { "stops_0_color", 0, 0, 0, ctypes_u24, 1 },
    { "stops_0_frac", 4, 0, 0, ctypes_u8, 0 },
    { "stops_0_opa", 3, 0, 0, ctypes_u8, 0 },
    { "stops_1_color", 5, 0, 0, ctypes_u24, 1 },
    { "stops_1_frac", 9, 0, 0, ctypes_u8, 0 },
    { "stops_1_opa", 8, 0, 0, ctypes_u8, 0 },
    { "stops_count", 10, 0, 0, ctypes_u8, 0 },
}};

const be_ctypes_structure_t be_lv_draw_dsc_base = {
  40,  /* size in bytes */
  13,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[13]) {
    { "drop_shadow_blur_radius", 28, 0, 20, ctypes_bf, 0 },
    { "drop_shadow_color", 24, 0, 0, ctypes_u24, 1 },
    { "drop_shadow_ofs_x", 20, 0, 0, ctypes_i16, 0 },
    { "drop_shadow_ofs_y", 22, 0, 0, ctypes_i16, 0 },
    { "drop_shadow_opa", 27, 0, 0, ctypes_u8, 0 },
    { "drop_shadow_quality", 30, 4, 3, ctypes_bf, 0 },
    { "dsc_size", 32, 0, 0, ctypes_u32, 0 },
    { "id1", 8, 0, 0, ctypes_u32, 0 },
    { "id2", 12, 0, 0, ctypes_u32, 0 },
    { "layer", 16, 0, 0, ctypes_ptr32, 0 },
    { "obj", 0, 0, 0, ctypes_ptr32, 0 },
    { "part", 4, 0, 0, ctypes_u32, 0 },
    { "user_data", 36, 0, 0, ctypes_ptr32, 0 },
}};

const be_ctypes_structure_t be_lv_draw_rect_dsc = {
  160,  /* size in bytes */
  56,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[56]) {
    { "base_drop_shadow_blur_radius", 28, 0, 20, ctypes_bf, 0 },
    { "base_drop_shadow_color", 24, 0, 0, ctypes_u24, 1 },
    { "base_drop_shadow_ofs_x", 20, 0, 0, ctypes_i16, 0 },
    { "base_drop_shadow_ofs_y", 22, 0, 0, ctypes_i16, 0 },
    { "base_drop_shadow_opa", 27, 0, 0, ctypes_u8, 0 },
    { "base_drop_shadow_quality", 30, 4, 3, ctypes_bf, 0 },
    { "base_dsc_size", 32, 0, 0, ctypes_u32, 0 },
    { "base_id1", 8, 0, 0, ctypes_u32, 0 },
    { "base_id2", 12, 0, 0, ctypes_u32, 0 },
    { "base_layer", 16, 0, 0, ctypes_ptr32, 0 },
    { "base_obj", 0, 0, 0, ctypes_ptr32, 0 },
    { "base_part", 4, 0, 0, ctypes_u32, 0 },
    { "base_user_data", 36, 0, 0, ctypes_ptr32, 0 },
    { "bg_color", 68, 0, 0, ctypes_u24, 1 },
    { "bg_grad_dir", 83, 0, 4, ctypes_bf, 0 },
    { "bg_grad_end_extent_x", 108, 0, 0, ctypes_i32, 0 },
    { "bg_grad_end_extent_y", 112, 0, 0, ctypes_i32, 0 },
    { "bg_grad_end_x", 100, 0, 0, ctypes_i32, 0 },
    { "bg_grad_end_y", 104, 0, 0, ctypes_i32, 0 },
    { "bg_grad_extend", 83, 4, 3, ctypes_bf, 0 },
    { "bg_grad_focal_extent_x", 92, 0, 0, ctypes_i32, 0 },
    { "bg_grad_focal_extent_y", 96, 0, 0, ctypes_i32, 0 },
    { "bg_grad_focal_x", 84, 0, 0, ctypes_i32, 0 },
    { "bg_grad_focal_y", 88, 0, 0, ctypes_i32, 0 },
    { "bg_grad_state", 116, 0, 0, ctypes_ptr32, 0 },
    { "bg_grad_stops_0_color", 72, 0, 0, ctypes_u24, 1 },
    { "bg_grad_stops_0_frac", 76, 0, 0, ctypes_u8, 0 },
    { "bg_grad_stops_0_opa", 75, 0, 0, ctypes_u8, 0 },
    { "bg_grad_stops_1_color", 77, 0, 0, ctypes_u24, 1 },
    { "bg_grad_stops_1_frac", 81, 0, 0, ctypes_u8, 0 },
    { "bg_grad_stops_1_opa", 80, 0, 0, ctypes_u8, 0 },
    { "bg_grad_stops_count", 82, 0, 0, ctypes_u8, 0 },
    { "bg_image_colorkey", 64, 0, 0, ctypes_ptr32, 0 },
    { "bg_img_opa", 55, 0, 0, ctypes_u8, 0 },
    { "bg_img_recolor", 52, 0, 0, ctypes_u24, 1 },
    { "bg_img_recolor_opa", 56, 0, 0, ctypes_u8, 0 },
    { "bg_img_src", 44, 0, 0, ctypes_ptr32, 0 },
    { "bg_img_symbol_font", 48, 0, 0, ctypes_ptr32, 0 },
    { "bg_img_tiled", 57, 0, 0, ctypes_u8, 0 },
    { "bg_opa", 58, 0, 0, ctypes_u8, 0 },
    { "border_color", 120, 0, 0, ctypes_u24, 1 },
    { "border_opa", 59, 0, 0, ctypes_u8, 0 },
    { "border_post", 128, 5, 1, ctypes_bf, 0 },
    { "border_side", 128, 0, 5, ctypes_bf, 0 },
    { "border_width", 124, 0, 0, ctypes_i32, 0 },
    { "outline_color", 129, 0, 0, ctypes_u24, 1 },
    { "outline_opa", 60, 0, 0, ctypes_u8, 0 },
    { "outline_pad", 136, 0, 0, ctypes_i32, 0 },
    { "outline_width", 132, 0, 0, ctypes_i32, 0 },
    { "radius", 40, 0, 0, ctypes_i32, 0 },
    { "shadow_color", 140, 0, 0, ctypes_u24, 1 },
    { "shadow_ofs_x", 148, 0, 0, ctypes_i32, 0 },
    { "shadow_ofs_y", 152, 0, 0, ctypes_i32, 0 },
    { "shadow_opa", 61, 0, 0, ctypes_u8, 0 },
    { "shadow_spread", 156, 0, 0, ctypes_i32, 0 },
    { "shadow_width", 144, 0, 0, ctypes_i32, 0 },
}};

const be_ctypes_structure_t be_lv_draw_line_dsc = {
  84,  /* size in bytes */
  27,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[27]) {
    { "base_drop_shadow_blur_radius", 28, 0, 20, ctypes_bf, 0 },
    { "base_drop_shadow_color", 24, 0, 0, ctypes_u24, 1 },
    { "base_drop_shadow_ofs_x", 20, 0, 0, ctypes_i16, 0 },
    { "base_drop_shadow_ofs_y", 22, 0, 0, ctypes_i16, 0 },
    { "base_drop_shadow_opa", 27, 0, 0, ctypes_u8, 0 },
    { "base_drop_shadow_quality", 30, 4, 3, ctypes_bf, 0 },
    { "base_dsc_size", 32, 0, 0, ctypes_u32, 0 },
    { "base_id1", 8, 0, 0, ctypes_u32, 0 },
    { "base_id2", 12, 0, 0, ctypes_u32, 0 },
    { "base_layer", 16, 0, 0, ctypes_ptr32, 0 },
    { "base_obj", 0, 0, 0, ctypes_ptr32, 0 },
    { "base_part", 4, 0, 0, ctypes_u32, 0 },
    { "base_user_data", 36, 0, 0, ctypes_ptr32, 0 },
    { "color", 64, 0, 0, ctypes_u24, 1 },
    { "dash_gap", 76, 0, 0, ctypes_i32, 0 },
    { "dash_width", 72, 0, 0, ctypes_i32, 0 },
    { "opa", 80, 0, 0, ctypes_u8, 0 },
    { "p1_x", 40, 0, 0, ctypes_i32, 0 },
    { "p1_y", 44, 0, 0, ctypes_i32, 0 },
    { "p2_x", 48, 0, 0, ctypes_i32, 0 },
    { "p2_y", 52, 0, 0, ctypes_i32, 0 },
    { "point_cnt", 60, 0, 0, ctypes_i32, 0 },
    { "points", 56, 0, 0, ctypes_ptr32, 0 },
    { "raw_end", 81, 2, 1, ctypes_bf, 0 },
    { "round_end", 81, 1, 1, ctypes_bf, 0 },
    { "round_start", 81, 0, 1, ctypes_bf, 0 },
    { "width", 68, 0, 0, ctypes_i32, 0 },
}};

const be_ctypes_structure_t be_lv_draw_arc_dsc = {
  72,  /* size in bytes */
  23,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[23]) {
    { "base_drop_shadow_blur_radius", 28, 0, 20, ctypes_bf, 0 },
    { "base_drop_shadow_color", 24, 0, 0, ctypes_u24, 1 },
    { "base_drop_shadow_ofs_x", 20, 0, 0, ctypes_i16, 0 },
    { "base_drop_shadow_ofs_y", 22, 0, 0, ctypes_i16, 0 },
    { "base_drop_shadow_opa", 27, 0, 0, ctypes_u8, 0 },
    { "base_drop_shadow_quality", 30, 4, 3, ctypes_bf, 0 },
    { "base_dsc_size", 32, 0, 0, ctypes_u32, 0 },
    { "base_id1", 8, 0, 0, ctypes_u32, 0 },
    { "base_id2", 12, 0, 0, ctypes_u32, 0 },
    { "base_layer", 16, 0, 0, ctypes_ptr32, 0 },
    { "base_obj", 0, 0, 0, ctypes_ptr32, 0 },
    { "base_part", 4, 0, 0, ctypes_u32, 0 },
    { "base_user_data", 36, 0, 0, ctypes_ptr32, 0 },
    { "center_x", 56, 0, 0, ctypes_i32, 0 },
    { "center_y", 60, 0, 0, ctypes_i32, 0 },
    { "color", 40, 0, 0, ctypes_u24, 1 },
    { "end_angle", 52, 0, 0, ctypes_i32, 0 },
    { "img_src", 64, 0, 0, ctypes_ptr32, 0 },
    { "opa", 70, 0, 0, ctypes_u8, 0 },
    { "radius", 68, 0, 0, ctypes_u16, 0 },
    { "rounded", 71, 0, 1, ctypes_bf, 0 },
    { "start_angle", 48, 0, 0, ctypes_i32, 0 },
    { "width", 44, 0, 0, ctypes_i32, 0 },
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
  124,  /* size in bytes */
  42,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[42]) {
    { "antialias", 93, 4, 1, ctypes_bf, 0 },
    { "base_drop_shadow_blur_radius", 28, 0, 20, ctypes_bf, 0 },
    { "base_drop_shadow_color", 24, 0, 0, ctypes_u24, 1 },
    { "base_drop_shadow_ofs_x", 20, 0, 0, ctypes_i16, 0 },
    { "base_drop_shadow_ofs_y", 22, 0, 0, ctypes_i16, 0 },
    { "base_drop_shadow_opa", 27, 0, 0, ctypes_u8, 0 },
    { "base_drop_shadow_quality", 30, 4, 3, ctypes_bf, 0 },
    { "base_dsc_size", 32, 0, 0, ctypes_u32, 0 },
    { "base_id1", 8, 0, 0, ctypes_u32, 0 },
    { "base_id2", 12, 0, 0, ctypes_u32, 0 },
    { "base_layer", 16, 0, 0, ctypes_ptr32, 0 },
    { "base_obj", 0, 0, 0, ctypes_ptr32, 0 },
    { "base_part", 4, 0, 0, ctypes_u32, 0 },
    { "base_user_data", 36, 0, 0, ctypes_ptr32, 0 },
    { "bitmap_mask_src", 120, 0, 0, ctypes_ptr32, 0 },
    { "blend_mode", 93, 0, 4, ctypes_bf, 0 },
    { "clip_radius", 56, 0, 0, ctypes_i32, 0 },
    { "header_cf", 45, 0, 0, ctypes_u8, 0 },
    { "header_flags", 46, 0, 0, ctypes_u16, 0 },
    { "header_h", 50, 0, 0, ctypes_u16, 0 },
    { "header_magic", 44, 0, 0, ctypes_u8, 0 },
    { "header_reserved_2", 54, 0, 0, ctypes_u16, 0 },
    { "header_stride", 52, 0, 0, ctypes_u16, 0 },
    { "header_w", 48, 0, 0, ctypes_u16, 0 },
    { "image_area_x1", 104, 0, 0, ctypes_i32, 0 },
    { "image_area_x2", 112, 0, 0, ctypes_i32, 0 },
    { "image_area_y1", 108, 0, 0, ctypes_i32, 0 },
    { "image_area_y2", 116, 0, 0, ctypes_i32, 0 },
    { "lv_image_colorkey_t", 96, 0, 0, ctypes_ptr32, 0 },
    { "opa", 92, 0, 0, ctypes_u8, 0 },
    { "pivot_x", 80, 0, 0, ctypes_i32, 0 },
    { "pivot_y", 84, 0, 0, ctypes_i32, 0 },
    { "recolor", 88, 0, 0, ctypes_u24, 1 },
    { "recolor_opa", 91, 0, 0, ctypes_u8, 0 },
    { "rotation", 60, 0, 0, ctypes_i32, 0 },
    { "scale_x", 64, 0, 0, ctypes_i32, 0 },
    { "scale_y", 68, 0, 0, ctypes_i32, 0 },
    { "skew_x", 72, 0, 0, ctypes_i32, 0 },
    { "skew_y", 76, 0, 0, ctypes_i32, 0 },
    { "src", 40, 0, 0, ctypes_ptr32, 0 },
    { "sup", 100, 0, 0, ctypes_ptr32, 0 },
    { "tile", 93, 5, 1, ctypes_bf, 0 },
}};

const be_ctypes_structure_t be_lv_draw_label_dsc = {
  120,  /* size in bytes */
  41,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[41]) {
    { "align", 100, 0, 0, ctypes_u8, 0 },
    { "base_drop_shadow_blur_radius", 28, 0, 20, ctypes_bf, 0 },
    { "base_drop_shadow_color", 24, 0, 0, ctypes_u24, 1 },
    { "base_drop_shadow_ofs_x", 20, 0, 0, ctypes_i16, 0 },
    { "base_drop_shadow_ofs_y", 22, 0, 0, ctypes_i16, 0 },
    { "base_drop_shadow_opa", 27, 0, 0, ctypes_u8, 0 },
    { "base_drop_shadow_quality", 30, 4, 3, ctypes_bf, 0 },
    { "base_dsc_size", 32, 0, 0, ctypes_u32, 0 },
    { "base_id1", 8, 0, 0, ctypes_u32, 0 },
    { "base_id2", 12, 0, 0, ctypes_u32, 0 },
    { "base_layer", 16, 0, 0, ctypes_ptr32, 0 },
    { "base_obj", 0, 0, 0, ctypes_ptr32, 0 },
    { "base_part", 4, 0, 0, ctypes_u32, 0 },
    { "base_user_data", 36, 0, 0, ctypes_ptr32, 0 },
    { "bidi_dir", 101, 0, 0, ctypes_u8, 0 },
    { "blend_mode", 106, 0, 3, ctypes_bf, 0 },
    { "color", 56, 0, 0, ctypes_u24, 1 },
    { "decor", 105, 0, 3, ctypes_bf, 0 },
    { "flag", 105, 3, 5, ctypes_bf, 0 },
    { "font", 52, 0, 0, ctypes_ptr32, 0 },
    { "has_bided", 106, 5, 1, ctypes_bf, 0 },
    { "hint", 108, 0, 0, ctypes_ptr32, 0 },
    { "letter_space", 64, 0, 0, ctypes_i32, 0 },
    { "line_space", 60, 0, 0, ctypes_i32, 0 },
    { "ofs_x", 68, 0, 0, ctypes_i32, 0 },
    { "ofs_y", 72, 0, 0, ctypes_i32, 0 },
    { "opa", 102, 0, 0, ctypes_u8, 0 },
    { "outline_stroke_color", 112, 0, 0, ctypes_u24, 1 },
    { "outline_stroke_opa", 103, 0, 0, ctypes_u8, 0 },
    { "outline_stroke_width", 116, 0, 0, ctypes_i32, 0 },
    { "rotation", 76, 0, 0, ctypes_i32, 0 },
    { "sel_bg_color", 91, 0, 0, ctypes_u24, 1 },
    { "sel_color", 88, 0, 0, ctypes_u24, 1 },
    { "sel_end", 84, 0, 0, ctypes_u32, 0 },
    { "sel_start", 80, 0, 0, ctypes_u32, 0 },
    { "text", 40, 0, 0, ctypes_ptr32, 0 },
    { "text_length", 96, 0, 0, ctypes_u32, 0 },
    { "text_local", 106, 3, 1, ctypes_bf, 0 },
    { "text_size_x", 44, 0, 0, ctypes_i32, 0 },
    { "text_size_y", 48, 0, 0, ctypes_i32, 0 },
    { "text_static", 106, 4, 1, ctypes_bf, 0 },
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
  10,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[10]) {
    { "code", 8, 0, 0, ctypes_i32, 0 },
    { "current_target", 0, 0, 0, ctypes_ptr32, 0 },
    { "deleted", 24, 0, 1, ctypes_bf, 0 },
    { "original_target", 4, 0, 0, ctypes_ptr32, 0 },
    { "param", 16, 0, 0, ctypes_ptr32, 0 },
    { "prev", 20, 0, 0, ctypes_ptr32, 0 },
    { "stop_bubbling", 24, 2, 1, ctypes_bf, 0 },
    { "stop_processing", 24, 1, 1, ctypes_bf, 0 },
    { "stop_trickling", 24, 3, 1, ctypes_bf, 0 },
    { "user_data", 12, 0, 0, ctypes_ptr32, 0 },
}};

const be_ctypes_structure_t be_lv_image_dsc = {
  28,  /* size in bytes */
  11,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[11]) {
    { "data", 16, 0, 0, ctypes_ptr32, 0 },
    { "data_size", 12, 0, 0, ctypes_u32, 0 },
    { "header_cf", 1, 0, 0, ctypes_u8, 0 },
    { "header_flags", 2, 0, 0, ctypes_u16, 0 },
    { "header_h", 6, 0, 0, ctypes_u16, 0 },
    { "header_magic", 0, 0, 0, ctypes_u8, 0 },
    { "header_reserved_2", 10, 0, 0, ctypes_u16, 0 },
    { "header_stride", 8, 0, 0, ctypes_u16, 0 },
    { "header_w", 4, 0, 0, ctypes_u16, 0 },
    { "reserved", 20, 0, 0, ctypes_ptr32, 0 },
    { "reserved_2", 24, 0, 0, ctypes_ptr32, 0 },
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

const be_ctypes_structure_t be_lv_anim_timeline_dsc = {
  28,  /* size in bytes */
  7,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[7]) {
    { "act_time", 8, 0, 0, ctypes_u32, 0 },
    { "anim_dsc", 0, 0, 0, ctypes_ptr32, 0 },
    { "anim_dsc_cnt", 4, 0, 0, ctypes_u32, 0 },
    { "delay", 12, 0, 0, ctypes_u32, 0 },
    { "repeat_count", 16, 0, 0, ctypes_u32, 0 },
    { "repeat_delay", 20, 0, 0, ctypes_u32, 0 },
    { "user_data", 24, 0, 0, ctypes_ptr32, 0 },
}};

const be_ctypes_structure_t be_lv_layer = {
  84,  /* size in bytes */
  21,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[21]) {
    { "_clip_area_x1", 52, 0, 0, ctypes_i32, 0 },
    { "_clip_area_x2", 60, 0, 0, ctypes_i32, 0 },
    { "_clip_area_y1", 56, 0, 0, ctypes_i32, 0 },
    { "_clip_area_y2", 64, 0, 0, ctypes_i32, 0 },
    { "buf_area_x1", 20, 0, 0, ctypes_i32, 0 },
    { "buf_area_x2", 28, 0, 0, ctypes_i32, 0 },
    { "buf_area_y1", 24, 0, 0, ctypes_i32, 0 },
    { "buf_area_y2", 32, 0, 0, ctypes_i32, 0 },
    { "color_format", 76, 0, 0, ctypes_u32, 0 },
    { "draw_buf", 0, 0, 0, ctypes_ptr32, 0 },
    { "draw_task_head", 4, 0, 0, ctypes_ptr32, 0 },
    { "next", 12, 0, 0, ctypes_ptr32, 0 },
    { "opa", 80, 0, 0, ctypes_u8, 0 },
    { "parent", 8, 0, 0, ctypes_ptr32, 0 },
    { "partial_y_offset", 68, 0, 0, ctypes_i32, 0 },
    { "phy_clip_area_x1", 36, 0, 0, ctypes_i32, 0 },
    { "phy_clip_area_x2", 44, 0, 0, ctypes_i32, 0 },
    { "phy_clip_area_y1", 40, 0, 0, ctypes_i32, 0 },
    { "phy_clip_area_y2", 48, 0, 0, ctypes_i32, 0 },
    { "recolor", 72, 0, 0, ctypes_u32, 0 },
    { "user_data", 16, 0, 0, ctypes_ptr32, 0 },
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
  28,  /* size in bytes */
  7,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[7]) {
    { "auto_delete", 24, 0, 1, ctypes_bf, 0 },
    { "last_run", 4, 0, 0, ctypes_u32, 0 },
    { "paused", 20, 0, 0, ctypes_i32, 0 },
    { "period", 0, 0, 0, ctypes_u32, 0 },
    { "repeat_count", 16, 0, 0, ctypes_i32, 0 },
    { "timer_cb", 8, 0, 0, ctypes_ptr32, 0 },
    { "user_data", 12, 0, 0, ctypes_ptr32, 0 },
}};

const be_ctypes_structure_t be_lv_event_dsc = {
  12,  /* size in bytes */
  3,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[3]) {
    { "cb", 0, 0, 0, ctypes_ptr32, 0 },
    { "filter", 8, 0, 0, ctypes_u32, 0 },
    { "user_data", 4, 0, 0, ctypes_ptr32, 0 },
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

const be_ctypes_structure_t be_lv_span_coords = {
  48,  /* size in bytes */
  12,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[12]) {
    { "heading_x1", 0, 0, 0, ctypes_i32, 0 },
    { "heading_x2", 8, 0, 0, ctypes_i32, 0 },
    { "heading_y1", 4, 0, 0, ctypes_i32, 0 },
    { "heading_y2", 12, 0, 0, ctypes_i32, 0 },
    { "middle_x1", 16, 0, 0, ctypes_i32, 0 },
    { "middle_x2", 24, 0, 0, ctypes_i32, 0 },
    { "middle_y1", 20, 0, 0, ctypes_i32, 0 },
    { "middle_y2", 28, 0, 0, ctypes_i32, 0 },
    { "trailing_x1", 32, 0, 0, ctypes_i32, 0 },
    { "trailing_x2", 40, 0, 0, ctypes_i32, 0 },
    { "trailing_y1", 36, 0, 0, ctypes_i32, 0 },
    { "trailing_y2", 44, 0, 0, ctypes_i32, 0 },
}};

const be_ctypes_structure_t be_lv_font_info = {
  24,  /* size in bytes */
  6,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[6]) {
    { "class_p", 4, 0, 0, ctypes_ptr32, 0 },
    { "kerning", 20, 0, 0, ctypes_u32, 0 },
    { "name", 0, 0, 0, ctypes_ptr32, 0 },
    { "render_mode", 12, 0, 0, ctypes_u32, 0 },
    { "size", 8, 0, 0, ctypes_u32, 0 },
    { "style", 16, 0, 0, ctypes_u32, 0 },
}};

const be_ctypes_structure_t be_lv_draw_letter_dsc = {
  92,  /* size in bytes */
  29,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[29]) {
    { "base_drop_shadow_blur_radius", 28, 0, 20, ctypes_bf, 0 },
    { "base_drop_shadow_color", 24, 0, 0, ctypes_u24, 1 },
    { "base_drop_shadow_ofs_x", 20, 0, 0, ctypes_i16, 0 },
    { "base_drop_shadow_ofs_y", 22, 0, 0, ctypes_i16, 0 },
    { "base_drop_shadow_opa", 27, 0, 0, ctypes_u8, 0 },
    { "base_drop_shadow_quality", 30, 4, 3, ctypes_bf, 0 },
    { "base_dsc_size", 32, 0, 0, ctypes_u32, 0 },
    { "base_id1", 8, 0, 0, ctypes_u32, 0 },
    { "base_id2", 12, 0, 0, ctypes_u32, 0 },
    { "base_layer", 16, 0, 0, ctypes_ptr32, 0 },
    { "base_obj", 0, 0, 0, ctypes_ptr32, 0 },
    { "base_part", 4, 0, 0, ctypes_u32, 0 },
    { "base_user_data", 36, 0, 0, ctypes_ptr32, 0 },
    { "blend_mode", 81, 3, 4, ctypes_bf, 0 },
    { "color", 48, 0, 0, ctypes_u24, 1 },
    { "decor", 81, 0, 3, ctypes_bf, 0 },
    { "font", 44, 0, 0, ctypes_ptr32, 0 },
    { "opa", 80, 0, 0, ctypes_u8, 0 },
    { "outline_stroke_color", 88, 0, 0, ctypes_u24, 1 },
    { "outline_stroke_opa", 82, 0, 0, ctypes_u8, 0 },
    { "outline_stroke_width", 84, 0, 0, ctypes_i32, 0 },
    { "pivot_x", 72, 0, 0, ctypes_i32, 0 },
    { "pivot_y", 76, 0, 0, ctypes_i32, 0 },
    { "rotation", 52, 0, 0, ctypes_i32, 0 },
    { "scale_x", 56, 0, 0, ctypes_i32, 0 },
    { "scale_y", 60, 0, 0, ctypes_i32, 0 },
    { "skew_x", 64, 0, 0, ctypes_i32, 0 },
    { "skew_y", 68, 0, 0, ctypes_i32, 0 },
    { "unicode", 40, 0, 0, ctypes_u32, 0 },
}};

static const char * be_ctypes_instance_mappings[] = {
  "lv.color",
  NULL
};

static be_define_ctypes_class(lv_anim_timeline_dsc, &be_lv_anim_timeline_dsc, &be_class_ctypes_bytes, "lv_anim_timeline_dsc");
static be_define_ctypes_class(lv_area, &be_lv_area, &be_class_ctypes_bytes, "lv_area");
static be_define_ctypes_class(lv_chart_cursor, &be_lv_chart_cursor, &be_class_ctypes_bytes, "lv_chart_cursor");
static be_define_ctypes_class(lv_chart_series, &be_lv_chart_series, &be_class_ctypes_bytes, "lv_chart_series");
static be_define_ctypes_class(lv_color_filter_dsc, &be_lv_color_filter_dsc, &be_class_ctypes_bytes, "lv_color_filter_dsc");
static be_define_ctypes_class(lv_draw_arc_dsc, &be_lv_draw_arc_dsc, &be_class_ctypes_bytes, "lv_draw_arc_dsc");
static be_define_ctypes_class(lv_draw_dsc_base, &be_lv_draw_dsc_base, &be_class_ctypes_bytes, "lv_draw_dsc_base");
static be_define_ctypes_class(lv_draw_image_dsc, &be_lv_draw_image_dsc, &be_class_ctypes_bytes, "lv_draw_image_dsc");
static be_define_ctypes_class(lv_draw_label_dsc, &be_lv_draw_label_dsc, &be_class_ctypes_bytes, "lv_draw_label_dsc");
static be_define_ctypes_class(lv_draw_letter_dsc, &be_lv_draw_letter_dsc, &be_class_ctypes_bytes, "lv_draw_letter_dsc");
static be_define_ctypes_class(lv_draw_line_dsc, &be_lv_draw_line_dsc, &be_class_ctypes_bytes, "lv_draw_line_dsc");
static be_define_ctypes_class(lv_draw_rect_dsc, &be_lv_draw_rect_dsc, &be_class_ctypes_bytes, "lv_draw_rect_dsc");
static be_define_ctypes_class(lv_event, &be_lv_event, &be_class_ctypes_bytes, "lv_event");
static be_define_ctypes_class(lv_event_dsc, &be_lv_event_dsc, &be_class_ctypes_bytes, "lv_event_dsc");
static be_define_ctypes_class(lv_font_info, &be_lv_font_info, &be_class_ctypes_bytes, "lv_font_info");
static be_define_ctypes_class(lv_grad_dsc, &be_lv_grad_dsc, &be_class_ctypes_bytes, "lv_grad_dsc");
static be_define_ctypes_class(lv_grad_stop, &be_lv_grad_stop, &be_class_ctypes_bytes, "lv_grad_stop");
static be_define_ctypes_class(lv_image_dsc, &be_lv_image_dsc, &be_class_ctypes_bytes, "lv_image_dsc");
static be_define_ctypes_class(lv_image_header, &be_lv_image_header, &be_class_ctypes_bytes, "lv_image_header");
static be_define_ctypes_class(lv_layer, &be_lv_layer, &be_class_ctypes_bytes, "lv_layer");
static be_define_ctypes_class(lv_obj_class, &be_lv_obj_class, &be_class_ctypes_bytes, "lv_obj_class");
static be_define_ctypes_class(lv_point, &be_lv_point, &be_class_ctypes_bytes, "lv_point");
static be_define_ctypes_class(lv_point_precise, &be_lv_point_precise, &be_class_ctypes_bytes, "lv_point_precise");
static be_define_ctypes_class(lv_span_coords, &be_lv_span_coords, &be_class_ctypes_bytes, "lv_span_coords");
static be_define_ctypes_class(lv_style_transition_dsc, &be_lv_style_transition_dsc, &be_class_ctypes_bytes, "lv_style_transition_dsc");
static be_define_ctypes_class(lv_timer_ntv, &be_lv_timer_ntv, &be_class_ctypes_bytes, "lv_timer_ntv");
static be_define_ctypes_class(lv_ts_calibration, &be_lv_ts_calibration, &be_class_ctypes_bytes, "lv_ts_calibration");

be_ctypes_class_by_name_t be_ctypes_lvgl_classes[] = {
  { "lv_anim_timeline_dsc", &be_class_lv_anim_timeline_dsc },
  { "lv_area", &be_class_lv_area },
  { "lv_chart_cursor", &be_class_lv_chart_cursor },
  { "lv_chart_series", &be_class_lv_chart_series },
  { "lv_color_filter_dsc", &be_class_lv_color_filter_dsc },
  { "lv_draw_arc_dsc", &be_class_lv_draw_arc_dsc },
  { "lv_draw_dsc_base", &be_class_lv_draw_dsc_base },
  { "lv_draw_image_dsc", &be_class_lv_draw_image_dsc },
  { "lv_draw_label_dsc", &be_class_lv_draw_label_dsc },
  { "lv_draw_letter_dsc", &be_class_lv_draw_letter_dsc },
  { "lv_draw_line_dsc", &be_class_lv_draw_line_dsc },
  { "lv_draw_rect_dsc", &be_class_lv_draw_rect_dsc },
  { "lv_event", &be_class_lv_event },
  { "lv_event_dsc", &be_class_lv_event_dsc },
  { "lv_font_info", &be_class_lv_font_info },
  { "lv_grad_dsc", &be_class_lv_grad_dsc },
  { "lv_grad_stop", &be_class_lv_grad_stop },
  { "lv_image_dsc", &be_class_lv_image_dsc },
  { "lv_image_header", &be_class_lv_image_header },
  { "lv_layer", &be_class_lv_layer },
  { "lv_obj_class", &be_class_lv_obj_class },
  { "lv_point", &be_class_lv_point },
  { "lv_point_precise", &be_class_lv_point_precise },
  { "lv_span_coords", &be_class_lv_span_coords },
  { "lv_style_transition_dsc", &be_class_lv_style_transition_dsc },
  { "lv_timer_ntv", &be_class_lv_timer_ntv },
  { "lv_ts_calibration", &be_class_lv_ts_calibration },
};
const size_t be_ctypes_lvgl_classes_size = sizeof(be_ctypes_lvgl_classes)/sizeof(be_ctypes_lvgl_classes[0]);

/********************************************************************/
