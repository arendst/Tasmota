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

const be_ctypes_structure_t be_lv_coord = {
  2,  /* size in bytes */
  1,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[1]) {
    { "v", 0, 0, 0, ctypes_i16, 0 },
}};

const be_ctypes_structure_t be_lv_point = {
  4,  /* size in bytes */
  2,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[2]) {
    { "x", 0, 0, 0, ctypes_i16, 0 },
    { "y", 2, 0, 0, ctypes_i16, 0 },
}};

const be_ctypes_structure_t be_lv_area = {
  8,  /* size in bytes */
  4,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[4]) {
    { "x1", 0, 0, 0, ctypes_i16, 0 },
    { "x2", 4, 0, 0, ctypes_i16, 0 },
    { "y1", 2, 0, 0, ctypes_i16, 0 },
    { "y2", 6, 0, 0, ctypes_i16, 0 },
}};

const be_ctypes_structure_t be_lv_gradient_stop = {
  4,  /* size in bytes */
  2,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[2]) {
    { "color", 0, 0, 0, ctypes_u16, 1 },
    { "frac", 2, 0, 0, ctypes_u8, 0 },
}};

const be_ctypes_structure_t be_lv_grad_dsc = {
  12,  /* size in bytes */
  7,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[7]) {
    { "dir", 8, 0, 3, ctypes_bf, 0 },
    { "dither", 8, 3, 3, ctypes_bf, 0 },
    { "stops_0_color", 0, 0, 0, ctypes_u16, 1 },
    { "stops_0_frac", 2, 0, 0, ctypes_u8, 0 },
    { "stops_1_color", 4, 0, 0, ctypes_u16, 1 },
    { "stops_1_frac", 6, 0, 0, ctypes_u8, 0 },
    { "stops_count", 7, 0, 0, ctypes_u8, 0 },
}};

const be_ctypes_structure_t be_lv_draw_rect_dsc = {
  60,  /* size in bytes */
  32,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[32]) {
    { "bg_color", 4, 0, 0, ctypes_u16, 1 },
    { "bg_grad_dir", 16, 0, 3, ctypes_bf, 0 },
    { "bg_grad_dither", 16, 3, 3, ctypes_bf, 0 },
    { "bg_grad_stops_0_color", 8, 0, 0, ctypes_u16, 1 },
    { "bg_grad_stops_0_frac", 10, 0, 0, ctypes_u8, 0 },
    { "bg_grad_stops_1_color", 12, 0, 0, ctypes_u16, 1 },
    { "bg_grad_stops_1_frac", 14, 0, 0, ctypes_u8, 0 },
    { "bg_grad_stops_count", 15, 0, 0, ctypes_u8, 0 },
    { "bg_img_opa", 30, 0, 0, ctypes_u8, 0 },
    { "bg_img_recolor", 28, 0, 0, ctypes_u16, 1 },
    { "bg_img_recolor_opa", 31, 0, 0, ctypes_u8, 0 },
    { "bg_img_src", 20, 0, 0, ctypes_ptr32, 0 },
    { "bg_img_symbol_font", 24, 0, 0, ctypes_ptr32, 0 },
    { "bg_img_tiled", 32, 0, 0, ctypes_u8, 0 },
    { "bg_opa", 3, 0, 0, ctypes_u8, 0 },
    { "blend_mode", 2, 0, 0, ctypes_u8, 0 },
    { "border_color", 34, 0, 0, ctypes_u16, 1 },
    { "border_opa", 38, 0, 0, ctypes_u8, 0 },
    { "border_post", 39, 0, 1, ctypes_bf, 0 },
    { "border_side", 39, 1, 5, ctypes_bf, 0 },
    { "border_width", 36, 0, 0, ctypes_i16, 0 },
    { "outline_color", 40, 0, 0, ctypes_u16, 1 },
    { "outline_opa", 46, 0, 0, ctypes_u8, 0 },
    { "outline_pad", 44, 0, 0, ctypes_i16, 0 },
    { "outline_width", 42, 0, 0, ctypes_i16, 0 },
    { "radius", 0, 0, 0, ctypes_i16, 0 },
    { "shadow_color", 48, 0, 0, ctypes_u16, 1 },
    { "shadow_ofs_x", 52, 0, 0, ctypes_i16, 0 },
    { "shadow_ofs_y", 54, 0, 0, ctypes_i16, 0 },
    { "shadow_opa", 58, 0, 0, ctypes_u8, 0 },
    { "shadow_spread", 56, 0, 0, ctypes_i16, 0 },
    { "shadow_width", 50, 0, 0, ctypes_i16, 0 },
}};

const be_ctypes_structure_t be_lv_draw_line_dsc = {
  12,  /* size in bytes */
  9,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[9]) {
    { "blend_mode", 9, 0, 2, ctypes_bf, 0 },
    { "color", 0, 0, 0, ctypes_u16, 1 },
    { "dash_gap", 6, 0, 0, ctypes_i16, 0 },
    { "dash_width", 4, 0, 0, ctypes_i16, 0 },
    { "opa", 8, 0, 0, ctypes_u8, 0 },
    { "raw_end", 9, 4, 1, ctypes_bf, 0 },
    { "round_end", 9, 3, 1, ctypes_bf, 0 },
    { "round_start", 9, 2, 1, ctypes_bf, 0 },
    { "width", 2, 0, 0, ctypes_i16, 0 },
}};

const be_ctypes_structure_t be_lv_draw_arc_dsc = {
  16,  /* size in bytes */
  8,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[8]) {
    { "blend_mode", 13, 0, 2, ctypes_bf, 0 },
    { "color", 0, 0, 0, ctypes_u16, 1 },
    { "end_angle", 6, 0, 0, ctypes_u16, 0 },
    { "img_src", 8, 0, 0, ctypes_ptr32, 0 },
    { "opa", 12, 0, 0, ctypes_u8, 0 },
    { "rounded", 13, 2, 1, ctypes_bf, 0 },
    { "start_angle", 4, 0, 0, ctypes_u16, 0 },
    { "width", 2, 0, 0, ctypes_i16, 0 },
}};

const be_ctypes_structure_t be_lv_draw_img_dsc = {
  24,  /* size in bytes */
  10,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[10]) {
    { "angle", 0, 0, 0, ctypes_i16, 0 },
    { "antialias", 20, 0, 1, ctypes_bf, 0 },
    { "blend_mode", 12, 0, 4, ctypes_bf, 0 },
    { "frame_id", 16, 0, 0, ctypes_i32, 0 },
    { "opa", 11, 0, 0, ctypes_u8, 0 },
    { "pivot_x", 4, 0, 0, ctypes_i16, 0 },
    { "pivot_y", 6, 0, 0, ctypes_i16, 0 },
    { "recolor", 8, 0, 0, ctypes_u16, 1 },
    { "recolor_opa", 10, 0, 0, ctypes_u8, 0 },
    { "zoom", 2, 0, 0, ctypes_u16, 0 },
}};

const be_ctypes_structure_t be_lv_obj_draw_part_dsc = {
  72,  /* size in bytes */
  18,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[18]) {
    { "arc_dsc", 32, 0, 0, ctypes_ptr32, 0 },
    { "class_p", 4, 0, 0, ctypes_ptr32, 0 },
    { "draw_area", 12, 0, 0, ctypes_ptr32, 0 },
    { "draw_ctx", 0, 0, 0, ctypes_ptr32, 0 },
    { "id", 56, 0, 0, ctypes_u32, 0 },
    { "img_dsc", 28, 0, 0, ctypes_ptr32, 0 },
    { "label_dsc", 20, 0, 0, ctypes_ptr32, 0 },
    { "line_dsc", 24, 0, 0, ctypes_ptr32, 0 },
    { "p1", 36, 0, 0, ctypes_ptr32, 0 },
    { "p2", 40, 0, 0, ctypes_ptr32, 0 },
    { "part", 52, 0, 0, ctypes_u32, 0 },
    { "radius", 60, 0, 0, ctypes_i16, 0 },
    { "rect_dsc", 16, 0, 0, ctypes_ptr32, 0 },
    { "sub_part_ptr", 68, 0, 0, ctypes_ptr32, 0 },
    { "text", 44, 0, 0, ctypes_ptr32, 0 },
    { "text_length", 48, 0, 0, ctypes_u32, 0 },
    { "type", 8, 0, 0, ctypes_u32, 0 },
    { "value", 64, 0, 0, ctypes_i32, 0 },
}};

const be_ctypes_structure_t be_lv_draw_layer_ctx = {
  40,  /* size in bytes */
  14,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[14]) {
    { "area_act_x1", 8, 0, 0, ctypes_i16, 0 },
    { "area_act_x2", 12, 0, 0, ctypes_i16, 0 },
    { "area_act_y1", 10, 0, 0, ctypes_i16, 0 },
    { "area_act_y2", 14, 0, 0, ctypes_i16, 0 },
    { "area_full_x1", 0, 0, 0, ctypes_i16, 0 },
    { "area_full_x2", 4, 0, 0, ctypes_i16, 0 },
    { "area_full_y1", 2, 0, 0, ctypes_i16, 0 },
    { "area_full_y2", 6, 0, 0, ctypes_i16, 0 },
    { "buf", 32, 0, 0, ctypes_ptr32, 0 },
    { "buf_area", 28, 0, 0, ctypes_ptr32, 0 },
    { "clip_area", 24, 0, 0, ctypes_ptr32, 0 },
    { "max_row_with_alpha", 16, 0, 0, ctypes_i16, 0 },
    { "max_row_with_no_alpha", 18, 0, 0, ctypes_i16, 0 },
    { "screen_transp", 36, 0, 0, ctypes_u8, 0 },
}};

const be_ctypes_structure_t be_lv_draw_mask_common_dsc = {
  8,  /* size in bytes */
  2,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[2]) {
    { "cb", 0, 0, 0, ctypes_ptr32, 0 },
    { "type", 4, 0, 0, ctypes_u8, 0 },
}};

const be_ctypes_structure_t be_lv_draw_mask_line_param_cfg = {
  12,  /* size in bytes */
  5,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[5]) {
    { "p1_x", 0, 0, 0, ctypes_i16, 0 },
    { "p1_y", 2, 0, 0, ctypes_i16, 0 },
    { "p2_x", 4, 0, 0, ctypes_i16, 0 },
    { "p2_y", 6, 0, 0, ctypes_i16, 0 },
    { "side", 8, 0, 2, ctypes_bf, 0 },
}};

const be_ctypes_structure_t be_lv_draw_mask_line_param = {
  44,  /* size in bytes */
  15,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[15]) {
    { "cfg_p1_x", 8, 0, 0, ctypes_i16, 0 },
    { "cfg_p1_y", 10, 0, 0, ctypes_i16, 0 },
    { "cfg_p2_x", 12, 0, 0, ctypes_i16, 0 },
    { "cfg_p2_y", 14, 0, 0, ctypes_i16, 0 },
    { "cfg_side", 16, 0, 2, ctypes_bf, 0 },
    { "dsc_cb", 0, 0, 0, ctypes_ptr32, 0 },
    { "dsc_type", 4, 0, 0, ctypes_u8, 0 },
    { "flat", 40, 0, 1, ctypes_bf, 0 },
    { "inv", 40, 1, 1, ctypes_bf, 0 },
    { "origo_x", 20, 0, 0, ctypes_i16, 0 },
    { "origo_y", 22, 0, 0, ctypes_i16, 0 },
    { "spx", 36, 0, 0, ctypes_i32, 0 },
    { "steep", 32, 0, 0, ctypes_i32, 0 },
    { "xy_steep", 24, 0, 0, ctypes_i32, 0 },
    { "yx_steep", 28, 0, 0, ctypes_i32, 0 },
}};

const be_ctypes_structure_t be_lv_draw_mask_angle_param_cfg = {
  8,  /* size in bytes */
  4,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[4]) {
    { "end_angle", 6, 0, 0, ctypes_i16, 0 },
    { "start_angle", 4, 0, 0, ctypes_i16, 0 },
    { "vertex_p_x", 0, 0, 0, ctypes_i16, 0 },
    { "vertex_p_y", 2, 0, 0, ctypes_i16, 0 },
}};

const be_ctypes_structure_t be_lv_draw_mask_angle_param = {
  104,  /* size in bytes */
  37,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[37]) {
    { "cfg_end_angle", 14, 0, 0, ctypes_i16, 0 },
    { "cfg_start_angle", 12, 0, 0, ctypes_i16, 0 },
    { "cfg_vertex_p_x", 8, 0, 0, ctypes_i16, 0 },
    { "cfg_vertex_p_y", 10, 0, 0, ctypes_i16, 0 },
    { "delta_deg", 102, 0, 0, ctypes_u16, 0 },
    { "dsc_cb", 0, 0, 0, ctypes_ptr32, 0 },
    { "dsc_type", 4, 0, 0, ctypes_u8, 0 },
    { "end_line_cfg_p1_x", 68, 0, 0, ctypes_i16, 0 },
    { "end_line_cfg_p1_y", 70, 0, 0, ctypes_i16, 0 },
    { "end_line_cfg_p2_x", 72, 0, 0, ctypes_i16, 0 },
    { "end_line_cfg_p2_y", 74, 0, 0, ctypes_i16, 0 },
    { "end_line_cfg_side", 76, 0, 2, ctypes_bf, 0 },
    { "end_line_dsc_cb", 60, 0, 0, ctypes_ptr32, 0 },
    { "end_line_dsc_type", 64, 0, 0, ctypes_u8, 0 },
    { "end_line_flat", 100, 0, 1, ctypes_bf, 0 },
    { "end_line_inv", 100, 1, 1, ctypes_bf, 0 },
    { "end_line_origo_x", 80, 0, 0, ctypes_i16, 0 },
    { "end_line_origo_y", 82, 0, 0, ctypes_i16, 0 },
    { "end_line_spx", 96, 0, 0, ctypes_i32, 0 },
    { "end_line_steep", 92, 0, 0, ctypes_i32, 0 },
    { "end_line_xy_steep", 84, 0, 0, ctypes_i32, 0 },
    { "end_line_yx_steep", 88, 0, 0, ctypes_i32, 0 },
    { "start_line_cfg_p1_x", 24, 0, 0, ctypes_i16, 0 },
    { "start_line_cfg_p1_y", 26, 0, 0, ctypes_i16, 0 },
    { "start_line_cfg_p2_x", 28, 0, 0, ctypes_i16, 0 },
    { "start_line_cfg_p2_y", 30, 0, 0, ctypes_i16, 0 },
    { "start_line_cfg_side", 32, 0, 2, ctypes_bf, 0 },
    { "start_line_dsc_cb", 16, 0, 0, ctypes_ptr32, 0 },
    { "start_line_dsc_type", 20, 0, 0, ctypes_u8, 0 },
    { "start_line_flat", 56, 0, 1, ctypes_bf, 0 },
    { "start_line_inv", 56, 1, 1, ctypes_bf, 0 },
    { "start_line_origo_x", 36, 0, 0, ctypes_i16, 0 },
    { "start_line_origo_y", 38, 0, 0, ctypes_i16, 0 },
    { "start_line_spx", 52, 0, 0, ctypes_i32, 0 },
    { "start_line_steep", 48, 0, 0, ctypes_i32, 0 },
    { "start_line_xy_steep", 40, 0, 0, ctypes_i32, 0 },
    { "start_line_yx_steep", 44, 0, 0, ctypes_i32, 0 },
}};

const be_ctypes_structure_t be_lv_draw_mask_radius_param_cfg = {
  12,  /* size in bytes */
  6,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[6]) {
    { "outer", 10, 0, 1, ctypes_bf, 0 },
    { "radius", 8, 0, 0, ctypes_i16, 0 },
    { "rect_x1", 0, 0, 0, ctypes_i16, 0 },
    { "rect_x2", 4, 0, 0, ctypes_i16, 0 },
    { "rect_y1", 2, 0, 0, ctypes_i16, 0 },
    { "rect_y2", 6, 0, 0, ctypes_i16, 0 },
}};

const be_ctypes_structure_t be_lv_draw_mask_radius_circle_dsc = {
  28,  /* size in bytes */
  7,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[7]) {
    { "buf", 0, 0, 0, ctypes_ptr32, 0 },
    { "cir_opa", 4, 0, 0, ctypes_ptr32, 0 },
    { "life", 16, 0, 0, ctypes_i32, 0 },
    { "opa_start_on_y", 12, 0, 0, ctypes_ptr32, 0 },
    { "radius", 24, 0, 0, ctypes_i16, 0 },
    { "used_cnt", 20, 0, 0, ctypes_u32, 0 },
    { "x_start_on_y", 8, 0, 0, ctypes_ptr32, 0 },
}};

const be_ctypes_structure_t be_lv_draw_mask_radius_param = {
  48,  /* size in bytes */
  15,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[15]) {
    { "cfg_outer", 18, 0, 1, ctypes_bf, 0 },
    { "cfg_radius", 16, 0, 0, ctypes_i16, 0 },
    { "cfg_rect_x1", 8, 0, 0, ctypes_i16, 0 },
    { "cfg_rect_x2", 12, 0, 0, ctypes_i16, 0 },
    { "cfg_rect_y1", 10, 0, 0, ctypes_i16, 0 },
    { "cfg_rect_y2", 14, 0, 0, ctypes_i16, 0 },
    { "circle_buf", 20, 0, 0, ctypes_ptr32, 0 },
    { "circle_cir_opa", 24, 0, 0, ctypes_ptr32, 0 },
    { "circle_life", 36, 0, 0, ctypes_i32, 0 },
    { "circle_opa_start_on_y", 32, 0, 0, ctypes_ptr32, 0 },
    { "circle_radius", 44, 0, 0, ctypes_i16, 0 },
    { "circle_used_cnt", 40, 0, 0, ctypes_u32, 0 },
    { "circle_x_start_on_y", 28, 0, 0, ctypes_ptr32, 0 },
    { "dsc_cb", 0, 0, 0, ctypes_ptr32, 0 },
    { "dsc_type", 4, 0, 0, ctypes_u8, 0 },
}};

const be_ctypes_structure_t be_lv_draw_mask_fade_param_cfg = {
  16,  /* size in bytes */
  8,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[8]) {
    { "coords_x1", 0, 0, 0, ctypes_i16, 0 },
    { "coords_x2", 4, 0, 0, ctypes_i16, 0 },
    { "coords_y1", 2, 0, 0, ctypes_i16, 0 },
    { "coords_y2", 6, 0, 0, ctypes_i16, 0 },
    { "opa_bottom", 13, 0, 0, ctypes_u8, 0 },
    { "opa_top", 12, 0, 0, ctypes_u8, 0 },
    { "y_bottom", 10, 0, 0, ctypes_i16, 0 },
    { "y_top", 8, 0, 0, ctypes_i16, 0 },
}};

const be_ctypes_structure_t be_lv_draw_mask_fade_param = {
  24,  /* size in bytes */
  10,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[10]) {
    { "cfg_coords_x1", 8, 0, 0, ctypes_i16, 0 },
    { "cfg_coords_x2", 12, 0, 0, ctypes_i16, 0 },
    { "cfg_coords_y1", 10, 0, 0, ctypes_i16, 0 },
    { "cfg_coords_y2", 14, 0, 0, ctypes_i16, 0 },
    { "cfg_opa_bottom", 21, 0, 0, ctypes_u8, 0 },
    { "cfg_opa_top", 20, 0, 0, ctypes_u8, 0 },
    { "cfg_y_bottom", 18, 0, 0, ctypes_i16, 0 },
    { "cfg_y_top", 16, 0, 0, ctypes_i16, 0 },
    { "dsc_cb", 0, 0, 0, ctypes_ptr32, 0 },
    { "dsc_type", 4, 0, 0, ctypes_u8, 0 },
}};

const be_ctypes_structure_t be_lv_draw_mask_map_param_cfg = {
  12,  /* size in bytes */
  5,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[5]) {
    { "coords_x1", 0, 0, 0, ctypes_i16, 0 },
    { "coords_x2", 4, 0, 0, ctypes_i16, 0 },
    { "coords_y1", 2, 0, 0, ctypes_i16, 0 },
    { "coords_y2", 6, 0, 0, ctypes_i16, 0 },
    { "map", 8, 0, 0, ctypes_ptr32, 0 },
}};

const be_ctypes_structure_t be_lv_draw_mask_map_param = {
  20,  /* size in bytes */
  7,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[7]) {
    { "cfg_coords_x1", 8, 0, 0, ctypes_i16, 0 },
    { "cfg_coords_x2", 12, 0, 0, ctypes_i16, 0 },
    { "cfg_coords_y1", 10, 0, 0, ctypes_i16, 0 },
    { "cfg_coords_y2", 14, 0, 0, ctypes_i16, 0 },
    { "cfg_map", 16, 0, 0, ctypes_ptr32, 0 },
    { "dsc_cb", 0, 0, 0, ctypes_ptr32, 0 },
    { "dsc_type", 4, 0, 0, ctypes_u8, 0 },
}};

const be_ctypes_structure_t be_lv_draw_mask_polygon_param_cfg = {
  8,  /* size in bytes */
  2,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[2]) {
    { "point_cnt", 4, 0, 0, ctypes_u16, 0 },
    { "points", 0, 0, 0, ctypes_ptr32, 0 },
}};

const be_ctypes_structure_t be_lv_draw_mask_polygon_param = {
  16,  /* size in bytes */
  4,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[4]) {
    { "cfg_point_cnt", 12, 0, 0, ctypes_u16, 0 },
    { "cfg_points", 8, 0, 0, ctypes_ptr32, 0 },
    { "dsc_cb", 0, 0, 0, ctypes_ptr32, 0 },
    { "dsc_type", 4, 0, 0, ctypes_u8, 0 },
}};

const be_ctypes_structure_t be_lv_draw_mask_saved = {
  8,  /* size in bytes */
  2,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[2]) {
    { "custom_id", 4, 0, 0, ctypes_ptr32, 0 },
    { "param", 0, 0, 0, ctypes_ptr32, 0 },
}};

const be_ctypes_structure_t be_lv_meter_scale = {
  36,  /* size in bytes */
  15,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[15]) {
    { "angle_range", 30, 0, 0, ctypes_u16, 0 },
    { "label_color", 18, 0, 0, ctypes_i16, 0 },
    { "label_gap", 16, 0, 0, ctypes_i16, 0 },
    { "max", 24, 0, 0, ctypes_i32, 0 },
    { "min", 20, 0, 0, ctypes_i32, 0 },
    { "r_mod", 28, 0, 0, ctypes_i16, 0 },
    { "rotation", 32, 0, 0, ctypes_i16, 0 },
    { "tick_cnt", 2, 0, 0, ctypes_u16, 0 },
    { "tick_color", 0, 0, 0, ctypes_u16, 1 },
    { "tick_length", 4, 0, 0, ctypes_u16, 0 },
    { "tick_major_color", 8, 0, 0, ctypes_u16, 1 },
    { "tick_major_length", 12, 0, 0, ctypes_u16, 0 },
    { "tick_major_nth", 10, 0, 0, ctypes_u16, 0 },
    { "tick_major_width", 14, 0, 0, ctypes_u16, 0 },
    { "tick_width", 6, 0, 0, ctypes_u16, 0 },
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
  16,  /* size in bytes */
  9,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[9]) {
    { "color", 8, 0, 0, ctypes_u16, 1 },
    { "hidden", 12, 0, 1, ctypes_bf, 0 },
    { "start_point", 10, 0, 0, ctypes_u16, 0 },
    { "x_axis_sec", 12, 3, 1, ctypes_bf, 0 },
    { "x_ext_buf_assigned", 12, 1, 1, ctypes_bf, 0 },
    { "x_points", 0, 0, 0, ctypes_ptr32, 0 },
    { "y_axis_sec", 12, 4, 1, ctypes_bf, 0 },
    { "y_ext_buf_assigned", 12, 2, 1, ctypes_bf, 0 },
    { "y_points", 4, 0, 0, ctypes_ptr32, 0 },
}};

const be_ctypes_structure_t be_lv_chart_cursor = {
  16,  /* size in bytes */
  7,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[7]) {
    { "color", 6, 0, 0, ctypes_u16, 1 },
    { "dir", 12, 0, 0, ctypes_u8, 0 },
    { "point_id", 4, 0, 0, ctypes_u16, 0 },
    { "pos_set", 13, 0, 1, ctypes_bf, 0 },
    { "pos_x", 0, 0, 0, ctypes_i16, 0 },
    { "pos_y", 2, 0, 0, ctypes_i16, 0 },
    { "ser", 8, 0, 0, ctypes_ptr32, 0 },
}};

const be_ctypes_structure_t be_lv_chart_tick_dsc = {
  12,  /* size in bytes */
  6,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[6]) {
    { "draw_size", 4, 0, 0, ctypes_i16, 0 },
    { "label_en", 9, 6, 1, ctypes_bf, 0 },
    { "major_cnt", 7, 7, 15, ctypes_bf, 0 },
    { "major_len", 0, 0, 0, ctypes_i16, 0 },
    { "minor_cnt", 6, 0, 15, ctypes_bf, 0 },
    { "minor_len", 2, 0, 0, ctypes_i16, 0 },
}};

const be_ctypes_structure_t be_lv_obj_class = {
  28,  /* size in bytes */
  10,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[10]) {
    { "base_class", 0, 0, 0, ctypes_ptr32, 0 },
    { "constructor_cb", 4, 0, 0, ctypes_ptr32, 0 },
    { "destructor_cb", 8, 0, 0, ctypes_ptr32, 0 },
    { "editable", 24, 0, 2, ctypes_bf, 0 },
    { "event_cb", 16, 0, 0, ctypes_ptr32, 0 },
    { "group_def", 24, 2, 2, ctypes_bf, 0 },
    { "height_def", 22, 0, 0, ctypes_i16, 0 },
    { "instance_size", 24, 4, 16, ctypes_bf, 0 },
    { "user_data", 12, 0, 0, ctypes_ptr32, 0 },
    { "width_def", 20, 0, 0, ctypes_i16, 0 },
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

const be_ctypes_structure_t be_lv_sqrt_res = {
  4,  /* size in bytes */
  2,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[2]) {
    { "f", 2, 0, 0, ctypes_u16, 0 },
    { "i", 0, 0, 0, ctypes_u16, 0 },
}};

const be_ctypes_structure_t be_lv_img_header = {
  4,  /* size in bytes */
  5,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[5]) {
    { "always_zero", 0, 5, 3, ctypes_bf, 0 },
    { "cf", 0, 0, 5, ctypes_bf, 0 },
    { "h", 2, 5, 11, ctypes_bf, 0 },
    { "reserved", 1, 0, 2, ctypes_bf, 0 },
    { "w", 1, 2, 11, ctypes_bf, 0 },
}};

const be_ctypes_structure_t be_lv_img_dsc = {
  12,  /* size in bytes */
  7,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[7]) {
    { "data", 8, 0, 0, ctypes_ptr32, 0 },
    { "data_size", 4, 0, 0, ctypes_u32, 0 },
    { "header_always_zero", 0, 5, 3, ctypes_bf, 0 },
    { "header_cf", 0, 0, 5, ctypes_bf, 0 },
    { "header_h", 2, 5, 11, ctypes_bf, 0 },
    { "header_reserved", 1, 0, 2, ctypes_bf, 0 },
    { "header_w", 1, 2, 11, ctypes_bf, 0 },
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
  6,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[6]) {
    { "last_run", 4, 0, 0, ctypes_u32, 0 },
    { "paused", 20, 0, 1, ctypes_bf, 0 },
    { "period", 0, 0, 0, ctypes_u32, 0 },
    { "repeat_count", 16, 0, 0, ctypes_i32, 0 },
    { "timer_cb", 8, 0, 0, ctypes_ptr32, 0 },
    { "user_data", 12, 0, 0, ctypes_ptr32, 0 },
}};

const be_ctypes_structure_t be_lv_anim_ntv = {
  72,  /* size in bytes */
  21,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[21]) {
    { "act_time", 48, 0, 0, ctypes_i32, 0 },
    { "current_value", 36, 0, 0, ctypes_i32, 0 },
    { "deleted_cb", 16, 0, 0, ctypes_ptr32, 0 },
    { "early_apply", 68, 0, 1, ctypes_bf, 0 },
    { "end_value", 40, 0, 0, ctypes_i32, 0 },
    { "exec_cb", 4, 0, 0, ctypes_ptr32, 0 },
    { "get_value_cb", 20, 0, 0, ctypes_ptr32, 0 },
    { "path_cb", 28, 0, 0, ctypes_ptr32, 0 },
    { "playback_delay", 52, 0, 0, ctypes_u32, 0 },
    { "playback_now", 68, 1, 1, ctypes_bf, 0 },
    { "playback_time", 56, 0, 0, ctypes_u32, 0 },
    { "ready_cb", 12, 0, 0, ctypes_ptr32, 0 },
    { "repeat_cnt", 64, 0, 0, ctypes_u32, 0 },
    { "repeat_delay", 60, 0, 0, ctypes_u32, 0 },
    { "run_round", 68, 2, 1, ctypes_bf, 0 },
    { "start_cb", 8, 0, 0, ctypes_ptr32, 0 },
    { "start_cb_called", 68, 3, 1, ctypes_bf, 0 },
    { "start_value", 32, 0, 0, ctypes_i32, 0 },
    { "time", 44, 0, 0, ctypes_i32, 0 },
    { "user_data", 24, 0, 0, ctypes_ptr32, 0 },
    { "var", 0, 0, 0, ctypes_ptr32, 0 },
}};

const be_ctypes_structure_t be_lv_draw_ctx = {
  88,  /* size in bytes */
  26,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[26]) {
    { "buf", 0, 0, 0, ctypes_ptr32, 0 },
    { "buf_area_x1", 4, 0, 0, ctypes_i16, 0 },
    { "buf_area_x2", 8, 0, 0, ctypes_i16, 0 },
    { "buf_area_y1", 6, 0, 0, ctypes_i16, 0 },
    { "buf_area_y2", 10, 0, 0, ctypes_i16, 0 },
    { "buffer_copy", 60, 0, 0, ctypes_ptr32, 0 },
    { "clip_area_x1", 12, 0, 0, ctypes_i16, 0 },
    { "clip_area_x2", 16, 0, 0, ctypes_i16, 0 },
    { "clip_area_y1", 14, 0, 0, ctypes_i16, 0 },
    { "clip_area_y2", 18, 0, 0, ctypes_i16, 0 },
    { "draw_arc", 24, 0, 0, ctypes_ptr32, 0 },
    { "draw_bg", 52, 0, 0, ctypes_ptr32, 0 },
    { "draw_img", 32, 0, 0, ctypes_ptr32, 0 },
    { "draw_img_decoded", 28, 0, 0, ctypes_ptr32, 0 },
    { "draw_letter", 36, 0, 0, ctypes_ptr32, 0 },
    { "draw_line", 40, 0, 0, ctypes_ptr32, 0 },
    { "draw_polygon", 44, 0, 0, ctypes_ptr32, 0 },
    { "draw_rect", 20, 0, 0, ctypes_ptr32, 0 },
    { "draw_transform", 48, 0, 0, ctypes_ptr32, 0 },
    { "layer_adjust", 68, 0, 0, ctypes_ptr32, 0 },
    { "layer_blend", 72, 0, 0, ctypes_ptr32, 0 },
    { "layer_destroy", 76, 0, 0, ctypes_ptr32, 0 },
    { "layer_init", 64, 0, 0, ctypes_ptr32, 0 },
    { "layer_instance_size", 80, 0, 0, ctypes_u32, 0 },
    { "user_data", 84, 0, 0, ctypes_ptr32, 0 },
    { "wait_for_finish", 56, 0, 0, ctypes_ptr32, 0 },
}};

const be_ctypes_structure_t be_lv_ts_calibration = {
  12,  /* size in bytes */
  5,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[5]) {
    { "raw_x", 0, 0, 0, ctypes_i16, 0 },
    { "raw_y", 2, 0, 0, ctypes_i16, 0 },
    { "state", 8, 0, 0, ctypes_i32, 0 },
    { "x", 4, 0, 0, ctypes_i16, 0 },
    { "y", 6, 0, 0, ctypes_i16, 0 },
}};

static const char * be_ctypes_instance_mappings[] = {
  "lv_color",
  NULL
};

static be_define_ctypes_class(lv_anim_ntv, &be_lv_anim_ntv, &be_class_ctypes_bytes, "lv_anim_ntv");
static be_define_ctypes_class(lv_area, &be_lv_area, &be_class_ctypes_bytes, "lv_area");
static be_define_ctypes_class(lv_chart_cursor, &be_lv_chart_cursor, &be_class_ctypes_bytes, "lv_chart_cursor");
static be_define_ctypes_class(lv_chart_series, &be_lv_chart_series, &be_class_ctypes_bytes, "lv_chart_series");
static be_define_ctypes_class(lv_chart_tick_dsc, &be_lv_chart_tick_dsc, &be_class_ctypes_bytes, "lv_chart_tick_dsc");
static be_define_ctypes_class(lv_color_filter_dsc, &be_lv_color_filter_dsc, &be_class_ctypes_bytes, "lv_color_filter_dsc");
static be_define_ctypes_class(lv_coord, &be_lv_coord, &be_class_ctypes_bytes, "lv_coord");
static be_define_ctypes_class(lv_draw_arc_dsc, &be_lv_draw_arc_dsc, &be_class_ctypes_bytes, "lv_draw_arc_dsc");
static be_define_ctypes_class(lv_draw_ctx, &be_lv_draw_ctx, &be_class_ctypes_bytes, "lv_draw_ctx");
static be_define_ctypes_class(lv_draw_img_dsc, &be_lv_draw_img_dsc, &be_class_ctypes_bytes, "lv_draw_img_dsc");
static be_define_ctypes_class(lv_draw_layer_ctx, &be_lv_draw_layer_ctx, &be_class_ctypes_bytes, "lv_draw_layer_ctx");
static be_define_ctypes_class(lv_draw_line_dsc, &be_lv_draw_line_dsc, &be_class_ctypes_bytes, "lv_draw_line_dsc");
static be_define_ctypes_class(lv_draw_mask_angle_param, &be_lv_draw_mask_angle_param, &be_class_ctypes_bytes, "lv_draw_mask_angle_param");
static be_define_ctypes_class(lv_draw_mask_angle_param_cfg, &be_lv_draw_mask_angle_param_cfg, &be_class_ctypes_bytes, "lv_draw_mask_angle_param_cfg");
static be_define_ctypes_class(lv_draw_mask_common_dsc, &be_lv_draw_mask_common_dsc, &be_class_ctypes_bytes, "lv_draw_mask_common_dsc");
static be_define_ctypes_class(lv_draw_mask_fade_param, &be_lv_draw_mask_fade_param, &be_class_ctypes_bytes, "lv_draw_mask_fade_param");
static be_define_ctypes_class(lv_draw_mask_fade_param_cfg, &be_lv_draw_mask_fade_param_cfg, &be_class_ctypes_bytes, "lv_draw_mask_fade_param_cfg");
static be_define_ctypes_class(lv_draw_mask_line_param, &be_lv_draw_mask_line_param, &be_class_ctypes_bytes, "lv_draw_mask_line_param");
static be_define_ctypes_class(lv_draw_mask_line_param_cfg, &be_lv_draw_mask_line_param_cfg, &be_class_ctypes_bytes, "lv_draw_mask_line_param_cfg");
static be_define_ctypes_class(lv_draw_mask_map_param, &be_lv_draw_mask_map_param, &be_class_ctypes_bytes, "lv_draw_mask_map_param");
static be_define_ctypes_class(lv_draw_mask_map_param_cfg, &be_lv_draw_mask_map_param_cfg, &be_class_ctypes_bytes, "lv_draw_mask_map_param_cfg");
static be_define_ctypes_class(lv_draw_mask_polygon_param, &be_lv_draw_mask_polygon_param, &be_class_ctypes_bytes, "lv_draw_mask_polygon_param");
static be_define_ctypes_class(lv_draw_mask_polygon_param_cfg, &be_lv_draw_mask_polygon_param_cfg, &be_class_ctypes_bytes, "lv_draw_mask_polygon_param_cfg");
static be_define_ctypes_class(lv_draw_mask_radius_circle_dsc, &be_lv_draw_mask_radius_circle_dsc, &be_class_ctypes_bytes, "lv_draw_mask_radius_circle_dsc");
static be_define_ctypes_class(lv_draw_mask_radius_param, &be_lv_draw_mask_radius_param, &be_class_ctypes_bytes, "lv_draw_mask_radius_param");
static be_define_ctypes_class(lv_draw_mask_radius_param_cfg, &be_lv_draw_mask_radius_param_cfg, &be_class_ctypes_bytes, "lv_draw_mask_radius_param_cfg");
static be_define_ctypes_class(lv_draw_mask_saved, &be_lv_draw_mask_saved, &be_class_ctypes_bytes, "lv_draw_mask_saved");
static be_define_ctypes_class(lv_draw_rect_dsc, &be_lv_draw_rect_dsc, &be_class_ctypes_bytes, "lv_draw_rect_dsc");
static be_define_ctypes_class(lv_event, &be_lv_event, &be_class_ctypes_bytes, "lv_event");
static be_define_ctypes_class(lv_grad_dsc, &be_lv_grad_dsc, &be_class_ctypes_bytes, "lv_grad_dsc");
static be_define_ctypes_class(lv_gradient_stop, &be_lv_gradient_stop, &be_class_ctypes_bytes, "lv_gradient_stop");
static be_define_ctypes_class(lv_img_dsc, &be_lv_img_dsc, &be_class_ctypes_bytes, "lv_img_dsc");
static be_define_ctypes_class(lv_img_header, &be_lv_img_header, &be_class_ctypes_bytes, "lv_img_header");
static be_define_ctypes_class(lv_meter_indicator, &be_lv_meter_indicator, &be_class_ctypes_bytes, "lv_meter_indicator");
static be_define_ctypes_class(lv_meter_scale, &be_lv_meter_scale, &be_class_ctypes_bytes, "lv_meter_scale");
static be_define_ctypes_class(lv_obj_class, &be_lv_obj_class, &be_class_ctypes_bytes, "lv_obj_class");
static be_define_ctypes_class(lv_obj_draw_part_dsc, &be_lv_obj_draw_part_dsc, &be_class_ctypes_bytes, "lv_obj_draw_part_dsc");
static be_define_ctypes_class(lv_point, &be_lv_point, &be_class_ctypes_bytes, "lv_point");
static be_define_ctypes_class(lv_sqrt_res, &be_lv_sqrt_res, &be_class_ctypes_bytes, "lv_sqrt_res");
static be_define_ctypes_class(lv_style_transition_dsc, &be_lv_style_transition_dsc, &be_class_ctypes_bytes, "lv_style_transition_dsc");
static be_define_ctypes_class(lv_timer_ntv, &be_lv_timer_ntv, &be_class_ctypes_bytes, "lv_timer_ntv");
static be_define_ctypes_class(lv_ts_calibration, &be_lv_ts_calibration, &be_class_ctypes_bytes, "lv_ts_calibration");

void be_load_ctypes_lvgl_definitions_lib(bvm *vm) {
  ctypes_register_class(vm, &be_class_lv_anim_ntv);
  ctypes_register_class(vm, &be_class_lv_area);
  ctypes_register_class(vm, &be_class_lv_chart_cursor);
  ctypes_register_class(vm, &be_class_lv_chart_series);
  ctypes_register_class(vm, &be_class_lv_chart_tick_dsc);
  ctypes_register_class(vm, &be_class_lv_color_filter_dsc);
  ctypes_register_class(vm, &be_class_lv_coord);
  ctypes_register_class(vm, &be_class_lv_draw_arc_dsc);
  ctypes_register_class(vm, &be_class_lv_draw_ctx);
  ctypes_register_class(vm, &be_class_lv_draw_img_dsc);
  ctypes_register_class(vm, &be_class_lv_draw_layer_ctx);
  ctypes_register_class(vm, &be_class_lv_draw_line_dsc);
  ctypes_register_class(vm, &be_class_lv_draw_mask_angle_param);
  ctypes_register_class(vm, &be_class_lv_draw_mask_angle_param_cfg);
  ctypes_register_class(vm, &be_class_lv_draw_mask_common_dsc);
  ctypes_register_class(vm, &be_class_lv_draw_mask_fade_param);
  ctypes_register_class(vm, &be_class_lv_draw_mask_fade_param_cfg);
  ctypes_register_class(vm, &be_class_lv_draw_mask_line_param);
  ctypes_register_class(vm, &be_class_lv_draw_mask_line_param_cfg);
  ctypes_register_class(vm, &be_class_lv_draw_mask_map_param);
  ctypes_register_class(vm, &be_class_lv_draw_mask_map_param_cfg);
  ctypes_register_class(vm, &be_class_lv_draw_mask_polygon_param);
  ctypes_register_class(vm, &be_class_lv_draw_mask_polygon_param_cfg);
  ctypes_register_class(vm, &be_class_lv_draw_mask_radius_circle_dsc);
  ctypes_register_class(vm, &be_class_lv_draw_mask_radius_param);
  ctypes_register_class(vm, &be_class_lv_draw_mask_radius_param_cfg);
  ctypes_register_class(vm, &be_class_lv_draw_mask_saved);
  ctypes_register_class(vm, &be_class_lv_draw_rect_dsc);
  ctypes_register_class(vm, &be_class_lv_event);
  ctypes_register_class(vm, &be_class_lv_grad_dsc);
  ctypes_register_class(vm, &be_class_lv_gradient_stop);
  ctypes_register_class(vm, &be_class_lv_img_dsc);
  ctypes_register_class(vm, &be_class_lv_img_header);
  ctypes_register_class(vm, &be_class_lv_meter_indicator);
  ctypes_register_class(vm, &be_class_lv_meter_scale);
  ctypes_register_class(vm, &be_class_lv_obj_class);
  ctypes_register_class(vm, &be_class_lv_obj_draw_part_dsc);
  ctypes_register_class(vm, &be_class_lv_point);
  ctypes_register_class(vm, &be_class_lv_sqrt_res);
  ctypes_register_class(vm, &be_class_lv_style_transition_dsc);
  ctypes_register_class(vm, &be_class_lv_timer_ntv);
  ctypes_register_class(vm, &be_class_lv_ts_calibration);
}

be_ctypes_class_by_name_t be_ctypes_lvgl_classes[] = {
  { "lv_anim_ntv", &be_class_lv_anim_ntv },
  { "lv_area", &be_class_lv_area },
  { "lv_chart_cursor", &be_class_lv_chart_cursor },
  { "lv_chart_series", &be_class_lv_chart_series },
  { "lv_chart_tick_dsc", &be_class_lv_chart_tick_dsc },
  { "lv_color_filter_dsc", &be_class_lv_color_filter_dsc },
  { "lv_coord", &be_class_lv_coord },
  { "lv_draw_arc_dsc", &be_class_lv_draw_arc_dsc },
  { "lv_draw_ctx", &be_class_lv_draw_ctx },
  { "lv_draw_img_dsc", &be_class_lv_draw_img_dsc },
  { "lv_draw_layer_ctx", &be_class_lv_draw_layer_ctx },
  { "lv_draw_line_dsc", &be_class_lv_draw_line_dsc },
  { "lv_draw_mask_angle_param", &be_class_lv_draw_mask_angle_param },
  { "lv_draw_mask_angle_param_cfg", &be_class_lv_draw_mask_angle_param_cfg },
  { "lv_draw_mask_common_dsc", &be_class_lv_draw_mask_common_dsc },
  { "lv_draw_mask_fade_param", &be_class_lv_draw_mask_fade_param },
  { "lv_draw_mask_fade_param_cfg", &be_class_lv_draw_mask_fade_param_cfg },
  { "lv_draw_mask_line_param", &be_class_lv_draw_mask_line_param },
  { "lv_draw_mask_line_param_cfg", &be_class_lv_draw_mask_line_param_cfg },
  { "lv_draw_mask_map_param", &be_class_lv_draw_mask_map_param },
  { "lv_draw_mask_map_param_cfg", &be_class_lv_draw_mask_map_param_cfg },
  { "lv_draw_mask_polygon_param", &be_class_lv_draw_mask_polygon_param },
  { "lv_draw_mask_polygon_param_cfg", &be_class_lv_draw_mask_polygon_param_cfg },
  { "lv_draw_mask_radius_circle_dsc", &be_class_lv_draw_mask_radius_circle_dsc },
  { "lv_draw_mask_radius_param", &be_class_lv_draw_mask_radius_param },
  { "lv_draw_mask_radius_param_cfg", &be_class_lv_draw_mask_radius_param_cfg },
  { "lv_draw_mask_saved", &be_class_lv_draw_mask_saved },
  { "lv_draw_rect_dsc", &be_class_lv_draw_rect_dsc },
  { "lv_event", &be_class_lv_event },
  { "lv_grad_dsc", &be_class_lv_grad_dsc },
  { "lv_gradient_stop", &be_class_lv_gradient_stop },
  { "lv_img_dsc", &be_class_lv_img_dsc },
  { "lv_img_header", &be_class_lv_img_header },
  { "lv_meter_indicator", &be_class_lv_meter_indicator },
  { "lv_meter_scale", &be_class_lv_meter_scale },
  { "lv_obj_class", &be_class_lv_obj_class },
  { "lv_obj_draw_part_dsc", &be_class_lv_obj_draw_part_dsc },
  { "lv_point", &be_class_lv_point },
  { "lv_sqrt_res", &be_class_lv_sqrt_res },
  { "lv_style_transition_dsc", &be_class_lv_style_transition_dsc },
  { "lv_timer_ntv", &be_class_lv_timer_ntv },
  { "lv_ts_calibration", &be_class_lv_ts_calibration },
};
const size_t be_ctypes_lvgl_classes_size = sizeof(be_ctypes_lvgl_classes)/sizeof(be_ctypes_lvgl_classes[0]);

/********************************************************************/
