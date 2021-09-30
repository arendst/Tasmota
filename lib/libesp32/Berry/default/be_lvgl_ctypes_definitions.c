/********************************************************************
 * Tasmota LVGL ctypes mapping
 *******************************************************************/
#include "be_ctypes.h"

#ifdef USE_LVGL

#include "lvgl.h"

/********************************************************************
 * Generated code, don't edit
 *******************************************************************/

static const char * be_ctypes_instance_mappings[];    /* forward definition */

const be_ctypes_structure_t be_lv_point = {
  4,  /* size in bytes */
  2,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[2]) {
    { "x", 0, 0, 0, 12, 0 },
    { "y", 2, 0, 0, 12, 0 },
}};

const be_ctypes_structure_t be_lv_area = {
  8,  /* size in bytes */
  4,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[4]) {
    { "x1", 0, 0, 0, 12, 0 },
    { "x2", 4, 0, 0, 12, 0 },
    { "y1", 2, 0, 0, 12, 0 },
    { "y2", 6, 0, 0, 12, 0 },
}};

const be_ctypes_structure_t be_lv_draw_rect_dsc = {
  77,  /* size in bytes */
  43,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[43]) {
    { "bg_blend_mode", 12, 0, 0, 1, 0 },
    { "bg_color", 2, 0, 0, 2, 1 },
    { "bg_grad_color", 4, 0, 0, 2, 1 },
    { "bg_grad_color_stop", 9, 0, 0, 12, 0 },
    { "bg_grad_dir", 6, 0, 0, 1, 0 },
    { "bg_main_color_stop", 7, 0, 0, 12, 0 },
    { "bg_opa", 11, 0, 0, 1, 0 },
    { "border_blend_mode", 20, 0, 0, 1, 0 },
    { "border_color", 13, 0, 0, 2, 1 },
    { "border_opa", 19, 0, 0, 1, 0 },
    { "border_post", 21, 0, 1, 0, 0 },
    { "border_side", 17, 0, 0, 12, 0 },
    { "border_width", 15, 0, 0, 12, 0 },
    { "outline_blend_mode", 29, 0, 0, 1, 0 },
    { "outline_color", 22, 0, 0, 2, 1 },
    { "outline_opa", 28, 0, 0, 1, 0 },
    { "outline_pad", 26, 0, 0, 12, 0 },
    { "outline_width", 24, 0, 0, 12, 0 },
    { "pattern_blend_mode", 55, 0, 0, 1, 0 },
    { "pattern_font", 46, 0, 0, 4, 0 },
    { "pattern_image", 42, 0, 0, 4, 0 },
    { "pattern_opa", 52, 0, 0, 1, 0 },
    { "pattern_recolor", 50, 0, 0, 2, 1 },
    { "pattern_recolor_opa", 53, 0, 0, 1, 0 },
    { "pattern_repeat", 54, 0, 1, 0, 0 },
    { "radius", 0, 0, 0, 12, 0 },
    { "shadow_blend_mode", 41, 0, 0, 1, 0 },
    { "shadow_color", 30, 0, 0, 2, 1 },
    { "shadow_ofs_x", 34, 0, 0, 12, 0 },
    { "shadow_ofs_y", 36, 0, 0, 12, 0 },
    { "shadow_opa", 40, 0, 0, 1, 0 },
    { "shadow_spread", 38, 0, 0, 12, 0 },
    { "shadow_width", 32, 0, 0, 12, 0 },
    { "value_align", 75, 0, 0, 1, 0 },
    { "value_blend_mode", 76, 0, 0, 1, 0 },
    { "value_color", 65, 0, 0, 2, 1 },
    { "value_font", 60, 0, 0, 4, 0 },
    { "value_letter_space", 71, 0, 0, 12, 0 },
    { "value_line_space", 73, 0, 0, 12, 0 },
    { "value_ofs_x", 67, 0, 0, 12, 0 },
    { "value_ofs_y", 69, 0, 0, 12, 0 },
    { "value_opa", 64, 0, 0, 1, 0 },
    { "value_str", 56, 0, 0, 4, 0 },
}};

const be_ctypes_structure_t be_lv_draw_line_dsc = {
  10,  /* size in bytes */
  9,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[9]) {
    { "blend_mode", 9, 0, 2, 0, 0 },
    { "color", 0, 0, 0, 2, 1 },
    { "dash_gap", 6, 0, 0, 12, 0 },
    { "dash_width", 4, 0, 0, 12, 0 },
    { "opa", 8, 0, 0, 1, 0 },
    { "raw_end", 9, 4, 1, 0, 0 },
    { "round_end", 9, 3, 1, 0, 0 },
    { "round_start", 9, 2, 1, 0, 0 },
    { "width", 2, 0, 0, 12, 0 },
}};

const be_ctypes_structure_t be_lv_draw_img_dsc = {
  14,  /* size in bytes */
  9,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[9]) {
    { "angle", 1, 0, 0, 2, 0 },
    { "antialias", 13, 0, 1, 0, 0 },
    { "blend_mode", 12, 0, 0, 1, 0 },
    { "opa", 0, 0, 0, 1, 0 },
    { "pivot_x", 3, 0, 0, 12, 0 },
    { "pivot_y", 5, 0, 0, 12, 0 },
    { "recolor", 10, 0, 0, 2, 1 },
    { "recolor_opa", 9, 0, 0, 1, 0 },
    { "zoom", 7, 0, 0, 2, 0 },
}};

const be_ctypes_structure_t be_lv_draw_label_dsc = {
  31,  /* size in bytes */
  15,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[15]) {
    { "bidi_dir", 27, 0, 0, 1, 0 },
    { "blend_mode", 30, 0, 0, 1, 0 },
    { "color", 0, 0, 0, 2, 1 },
    { "decor", 29, 0, 0, 1, 0 },
    { "flag", 28, 0, 0, 1, 0 },
    { "font", 6, 0, 0, 4, 0 },
    { "letter_space", 13, 0, 0, 12, 0 },
    { "line_space", 11, 0, 0, 12, 0 },
    { "ofs_x", 23, 0, 0, 12, 0 },
    { "ofs_y", 25, 0, 0, 12, 0 },
    { "opa", 10, 0, 0, 1, 0 },
    { "sel_bg_color", 4, 0, 0, 2, 1 },
    { "sel_color", 2, 0, 0, 2, 1 },
    { "sel_end", 19, 0, 0, 4, 0 },
    { "sel_start", 15, 0, 0, 4, 0 },
}};

const be_ctypes_structure_t be_lv_draw_mask_common_dsc = {
  5,  /* size in bytes */
  2,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[2]) {
    { "cb", 0, 0, 0, 4, 0 },
    { "type", 4, 0, 0, 1, 0 },
}};

const be_ctypes_structure_t be_lv_draw_mask_line_param_cfg = {
  9,  /* size in bytes */
  5,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[5]) {
    { "p1_x", 0, 0, 0, 12, 0 },
    { "p1_y", 2, 0, 0, 12, 0 },
    { "p2_x", 4, 0, 0, 12, 0 },
    { "p2_y", 6, 0, 0, 12, 0 },
    { "side", 8, 0, 2, 0, 0 },
}};

const be_ctypes_structure_t be_lv_draw_mask_line_param = {
  35,  /* size in bytes */
  15,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[15]) {
    { "cfg_p1_x", 5, 0, 0, 12, 0 },
    { "cfg_p1_y", 7, 0, 0, 12, 0 },
    { "cfg_p2_x", 9, 0, 0, 12, 0 },
    { "cfg_p2_y", 11, 0, 0, 12, 0 },
    { "cfg_side", 13, 0, 2, 0, 0 },
    { "dsc_cb", 0, 0, 0, 4, 0 },
    { "dsc_type", 4, 0, 0, 1, 0 },
    { "flat", 34, 0, 1, 0, 0 },
    { "inv", 34, 1, 1, 0, 0 },
    { "origo_x", 14, 0, 0, 12, 0 },
    { "origo_y", 16, 0, 0, 12, 0 },
    { "spx", 30, 0, 0, 14, 0 },
    { "steep", 26, 0, 0, 14, 0 },
    { "xy_steep", 18, 0, 0, 14, 0 },
    { "yx_steep", 22, 0, 0, 14, 0 },
}};

const be_ctypes_structure_t be_lv_draw_mask_angle_param_cfg = {
  8,  /* size in bytes */
  4,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[4]) {
    { "end_angle", 6, 0, 0, 12, 0 },
    { "start_angle", 4, 0, 0, 12, 0 },
    { "vertex_p_x", 0, 0, 0, 12, 0 },
    { "vertex_p_y", 2, 0, 0, 12, 0 },
}};

const be_ctypes_structure_t be_lv_draw_mask_angle_param = {
  85,  /* size in bytes */
  37,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[37]) {
    { "cfg_end_angle", 11, 0, 0, 12, 0 },
    { "cfg_start_angle", 9, 0, 0, 12, 0 },
    { "cfg_vertex_p_x", 5, 0, 0, 12, 0 },
    { "cfg_vertex_p_y", 7, 0, 0, 12, 0 },
    { "delta_deg", 83, 0, 0, 2, 0 },
    { "dsc_cb", 0, 0, 0, 4, 0 },
    { "dsc_type", 4, 0, 0, 1, 0 },
    { "end_line_cfg_p1_x", 53, 0, 0, 12, 0 },
    { "end_line_cfg_p1_y", 55, 0, 0, 12, 0 },
    { "end_line_cfg_p2_x", 57, 0, 0, 12, 0 },
    { "end_line_cfg_p2_y", 59, 0, 0, 12, 0 },
    { "end_line_cfg_side", 61, 0, 2, 0, 0 },
    { "end_line_dsc_cb", 48, 0, 0, 4, 0 },
    { "end_line_dsc_type", 52, 0, 0, 1, 0 },
    { "end_line_flat", 82, 0, 1, 0, 0 },
    { "end_line_inv", 82, 1, 1, 0, 0 },
    { "end_line_origo_x", 62, 0, 0, 12, 0 },
    { "end_line_origo_y", 64, 0, 0, 12, 0 },
    { "end_line_spx", 78, 0, 0, 14, 0 },
    { "end_line_steep", 74, 0, 0, 14, 0 },
    { "end_line_xy_steep", 66, 0, 0, 14, 0 },
    { "end_line_yx_steep", 70, 0, 0, 14, 0 },
    { "start_line_cfg_p1_x", 18, 0, 0, 12, 0 },
    { "start_line_cfg_p1_y", 20, 0, 0, 12, 0 },
    { "start_line_cfg_p2_x", 22, 0, 0, 12, 0 },
    { "start_line_cfg_p2_y", 24, 0, 0, 12, 0 },
    { "start_line_cfg_side", 26, 0, 2, 0, 0 },
    { "start_line_dsc_cb", 13, 0, 0, 4, 0 },
    { "start_line_dsc_type", 17, 0, 0, 1, 0 },
    { "start_line_flat", 47, 0, 1, 0, 0 },
    { "start_line_inv", 47, 1, 1, 0, 0 },
    { "start_line_origo_x", 27, 0, 0, 12, 0 },
    { "start_line_origo_y", 29, 0, 0, 12, 0 },
    { "start_line_spx", 43, 0, 0, 14, 0 },
    { "start_line_steep", 39, 0, 0, 14, 0 },
    { "start_line_xy_steep", 31, 0, 0, 14, 0 },
    { "start_line_yx_steep", 35, 0, 0, 14, 0 },
}};

const be_ctypes_structure_t be_lv_draw_mask_radius_param_cfg = {
  11,  /* size in bytes */
  6,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[6]) {
    { "outer", 10, 0, 1, 0, 0 },
    { "radius", 8, 0, 0, 12, 0 },
    { "rect_x1", 0, 0, 0, 12, 0 },
    { "rect_x2", 4, 0, 0, 12, 0 },
    { "rect_y1", 2, 0, 0, 12, 0 },
    { "rect_y2", 6, 0, 0, 12, 0 },
}};

const be_ctypes_structure_t be_lv_sqrt_res = {
  4,  /* size in bytes */
  2,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[2]) {
    { "f", 2, 0, 0, 2, 0 },
    { "i", 0, 0, 0, 2, 0 },
}};

const be_ctypes_structure_t be_lv_draw_mask_radius_param = {
  24,  /* size in bytes */
  11,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[11]) {
    { "cfg_outer", 15, 0, 1, 0, 0 },
    { "cfg_radius", 13, 0, 0, 12, 0 },
    { "cfg_rect_x1", 5, 0, 0, 12, 0 },
    { "cfg_rect_x2", 9, 0, 0, 12, 0 },
    { "cfg_rect_y1", 7, 0, 0, 12, 0 },
    { "cfg_rect_y2", 11, 0, 0, 12, 0 },
    { "dsc_cb", 0, 0, 0, 4, 0 },
    { "dsc_type", 4, 0, 0, 1, 0 },
    { "y_prev", 16, 0, 0, 14, 0 },
    { "y_prev_x_f", 22, 0, 0, 2, 0 },
    { "y_prev_x_i", 20, 0, 0, 2, 0 },
}};

const be_ctypes_structure_t be_lv_draw_mask_fade_param_cfg = {
  14,  /* size in bytes */
  8,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[8]) {
    { "coords_x1", 0, 0, 0, 12, 0 },
    { "coords_x2", 4, 0, 0, 12, 0 },
    { "coords_y1", 2, 0, 0, 12, 0 },
    { "coords_y2", 6, 0, 0, 12, 0 },
    { "opa_bottom", 13, 0, 0, 1, 0 },
    { "opa_top", 12, 0, 0, 1, 0 },
    { "y_bottom", 10, 0, 0, 12, 0 },
    { "y_top", 8, 0, 0, 12, 0 },
}};

const be_ctypes_structure_t be_lv_draw_mask_fade_param = {
  19,  /* size in bytes */
  10,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[10]) {
    { "cfg_coords_x1", 5, 0, 0, 12, 0 },
    { "cfg_coords_x2", 9, 0, 0, 12, 0 },
    { "cfg_coords_y1", 7, 0, 0, 12, 0 },
    { "cfg_coords_y2", 11, 0, 0, 12, 0 },
    { "cfg_opa_bottom", 18, 0, 0, 1, 0 },
    { "cfg_opa_top", 17, 0, 0, 1, 0 },
    { "cfg_y_bottom", 15, 0, 0, 12, 0 },
    { "cfg_y_top", 13, 0, 0, 12, 0 },
    { "dsc_cb", 0, 0, 0, 4, 0 },
    { "dsc_type", 4, 0, 0, 1, 0 },
}};

const be_ctypes_structure_t be_lv_draw_mask_map_param_cfg = {
  12,  /* size in bytes */
  5,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[5]) {
    { "coords_x1", 0, 0, 0, 12, 0 },
    { "coords_x2", 4, 0, 0, 12, 0 },
    { "coords_y1", 2, 0, 0, 12, 0 },
    { "coords_y2", 6, 0, 0, 12, 0 },
    { "map", 8, 0, 0, 4, 0 },
}};

const be_ctypes_structure_t be_lv_draw_mask_map_param = {
  17,  /* size in bytes */
  7,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[7]) {
    { "cfg_coords_x1", 5, 0, 0, 12, 0 },
    { "cfg_coords_x2", 9, 0, 0, 12, 0 },
    { "cfg_coords_y1", 7, 0, 0, 12, 0 },
    { "cfg_coords_y2", 11, 0, 0, 12, 0 },
    { "cfg_map", 13, 0, 0, 4, 0 },
    { "dsc_cb", 0, 0, 0, 4, 0 },
    { "dsc_type", 4, 0, 0, 1, 0 },
}};

const be_ctypes_structure_t be_lv_draw_mask_saved = {
  8,  /* size in bytes */
  2,  /* number of elements */
  be_ctypes_instance_mappings,
  (const be_ctypes_structure_item_t[2]) {
    { "custom_id", 4, 0, 0, 4, 0 },
    { "param", 0, 0, 0, 4, 0 },
}};

static const char * be_ctypes_instance_mappings[] = {
  "lv_color",
  NULL
};

static be_define_ctypes_class(lv_area, &be_lv_area, &be_class_ctypes, "lv_area");
static be_define_ctypes_class(lv_draw_img_dsc, &be_lv_draw_img_dsc, &be_class_ctypes, "lv_draw_img_dsc");
static be_define_ctypes_class(lv_draw_label_dsc, &be_lv_draw_label_dsc, &be_class_ctypes, "lv_draw_label_dsc");
static be_define_ctypes_class(lv_draw_line_dsc, &be_lv_draw_line_dsc, &be_class_ctypes, "lv_draw_line_dsc");
static be_define_ctypes_class(lv_draw_mask_angle_param, &be_lv_draw_mask_angle_param, &be_class_ctypes, "lv_draw_mask_angle_param");
static be_define_ctypes_class(lv_draw_mask_angle_param_cfg, &be_lv_draw_mask_angle_param_cfg, &be_class_ctypes, "lv_draw_mask_angle_param_cfg");
static be_define_ctypes_class(lv_draw_mask_common_dsc, &be_lv_draw_mask_common_dsc, &be_class_ctypes, "lv_draw_mask_common_dsc");
static be_define_ctypes_class(lv_draw_mask_fade_param, &be_lv_draw_mask_fade_param, &be_class_ctypes, "lv_draw_mask_fade_param");
static be_define_ctypes_class(lv_draw_mask_fade_param_cfg, &be_lv_draw_mask_fade_param_cfg, &be_class_ctypes, "lv_draw_mask_fade_param_cfg");
static be_define_ctypes_class(lv_draw_mask_line_param, &be_lv_draw_mask_line_param, &be_class_ctypes, "lv_draw_mask_line_param");
static be_define_ctypes_class(lv_draw_mask_line_param_cfg, &be_lv_draw_mask_line_param_cfg, &be_class_ctypes, "lv_draw_mask_line_param_cfg");
static be_define_ctypes_class(lv_draw_mask_map_param, &be_lv_draw_mask_map_param, &be_class_ctypes, "lv_draw_mask_map_param");
static be_define_ctypes_class(lv_draw_mask_map_param_cfg, &be_lv_draw_mask_map_param_cfg, &be_class_ctypes, "lv_draw_mask_map_param_cfg");
static be_define_ctypes_class(lv_draw_mask_radius_param, &be_lv_draw_mask_radius_param, &be_class_ctypes, "lv_draw_mask_radius_param");
static be_define_ctypes_class(lv_draw_mask_radius_param_cfg, &be_lv_draw_mask_radius_param_cfg, &be_class_ctypes, "lv_draw_mask_radius_param_cfg");
static be_define_ctypes_class(lv_draw_mask_saved, &be_lv_draw_mask_saved, &be_class_ctypes, "lv_draw_mask_saved");
static be_define_ctypes_class(lv_draw_rect_dsc, &be_lv_draw_rect_dsc, &be_class_ctypes, "lv_draw_rect_dsc");
static be_define_ctypes_class(lv_point, &be_lv_point, &be_class_ctypes, "lv_point");
static be_define_ctypes_class(lv_sqrt_res, &be_lv_sqrt_res, &be_class_ctypes, "lv_sqrt_res");

void be_load_ctypes_lvgl_definitions_lib(bvm *vm) {
  ctypes_register_class(vm, &be_class_lv_area, &be_lv_area);
  ctypes_register_class(vm, &be_class_lv_draw_img_dsc, &be_lv_draw_img_dsc);
  ctypes_register_class(vm, &be_class_lv_draw_label_dsc, &be_lv_draw_label_dsc);
  ctypes_register_class(vm, &be_class_lv_draw_line_dsc, &be_lv_draw_line_dsc);
  ctypes_register_class(vm, &be_class_lv_draw_mask_angle_param, &be_lv_draw_mask_angle_param);
  ctypes_register_class(vm, &be_class_lv_draw_mask_angle_param_cfg, &be_lv_draw_mask_angle_param_cfg);
  ctypes_register_class(vm, &be_class_lv_draw_mask_common_dsc, &be_lv_draw_mask_common_dsc);
  ctypes_register_class(vm, &be_class_lv_draw_mask_fade_param, &be_lv_draw_mask_fade_param);
  ctypes_register_class(vm, &be_class_lv_draw_mask_fade_param_cfg, &be_lv_draw_mask_fade_param_cfg);
  ctypes_register_class(vm, &be_class_lv_draw_mask_line_param, &be_lv_draw_mask_line_param);
  ctypes_register_class(vm, &be_class_lv_draw_mask_line_param_cfg, &be_lv_draw_mask_line_param_cfg);
  ctypes_register_class(vm, &be_class_lv_draw_mask_map_param, &be_lv_draw_mask_map_param);
  ctypes_register_class(vm, &be_class_lv_draw_mask_map_param_cfg, &be_lv_draw_mask_map_param_cfg);
  ctypes_register_class(vm, &be_class_lv_draw_mask_radius_param, &be_lv_draw_mask_radius_param);
  ctypes_register_class(vm, &be_class_lv_draw_mask_radius_param_cfg, &be_lv_draw_mask_radius_param_cfg);
  ctypes_register_class(vm, &be_class_lv_draw_mask_saved, &be_lv_draw_mask_saved);
  ctypes_register_class(vm, &be_class_lv_draw_rect_dsc, &be_lv_draw_rect_dsc);
  ctypes_register_class(vm, &be_class_lv_point, &be_lv_point);
  ctypes_register_class(vm, &be_class_lv_sqrt_res, &be_lv_sqrt_res);
}
/********************************************************************/

#endif // USE_LVGL