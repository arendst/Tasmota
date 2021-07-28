/********************************************************************
 * Tasmota LVGL ctypes mapping
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_LVGL

#include "lvgl.h"

enum {

    ctypes_i32    = 14,
    ctypes_i16    = 12,
    ctypes_i8     = 11,
    ctypes_u32    =  4,
    ctypes_u16    =  2,
    ctypes_u8     =  1,

    // big endian
    ctypes_be_i32 = -14,
    ctypes_be_i16 = -12,
    ctypes_be_i8  = -11,
    ctypes_be_u32 =  -4,
    ctypes_be_u16 =  -2,
    ctypes_be_u8  =  -1,

    ctypes_bf     = 0,    //bif-field
};

typedef struct be_ctypes_structure_item_t {
    const char * name;
    uint16_t  offset_bytes;
    uint8_t   offset_bits : 3;
    uint8_t   len_bits : 5;
    int8_t    type;
} be_ctypes_structure_item_t;

typedef struct be_ctypes_structure_t {
    uint16_t  size_bytes;       /* size in bytes */
    uint16_t  size_elt;         /* number of elements */
    const be_ctypes_structure_item_t * items;
} be_ctypes_structure_t;

typedef struct be_ctypes_class_t {
    const char * name;
    const be_ctypes_structure_t * definitions;
} be_ctypes_class_t;

typedef struct be_ctypes_classes_t {
    uint16_t  size;
    const be_ctypes_class_t * classes;
} be_ctypes_classes_t;

// extern const be_ctypes_class_t *g_ctypes_classes;


BE_EXPORT_VARIABLE extern const bclass be_class_lv_ctypes;



// const be_ctypes_structure_t be_lv_area_t = {
//   4,
//   (const be_ctypes_structure_item_t[4]) {
//     { "x1", 0, 16, ctypes_int },
//     { "x2", 32, 16, ctypes_int },
//     { "y1", 16, 16, ctypes_int },
//     { "y2", 48, 16, ctypes_int },
// // }};

// #if BE_USE_PRECOMPILED_OBJECT
// #include "../generate/be_fixed_be_class_lv_area_t.h"
// #endif

// void be_load_lvgl_lv_area_t_lib(bvm *vm) {
// #if !BE_USE_PRECOMPILED_OBJECT
//     static const bnfuncinfo members[] = {
//         { NULL, NULL }
//     };
//     be_regclass(vm, "lv_area_t", members);

//     be_getglobal(vm, "lv_area_t");
//     be_getglobal(vm, "lv_ctypes");
//     be_setsuper(vm, -2);
//     be_pop(vm, 2);
// #else
//     be_pushntvclass(vm, &be_class_lv_area_t);
//     be_setglobal(vm, "lv_area_t");
//     be_pop(vm, 1);
// #endif
// }

void ctypes_register_class(bvm *vm, const bclass * ctypes_class, const be_ctypes_structure_t * definitions) {
    be_pushntvclass(vm, ctypes_class);
    be_setglobal(vm, str(ctypes_class->name));
    be_pop(vm, 1);
}

/********************************************************************
 * Generated code, don't edit
 *******************************************************************/
const be_ctypes_structure_t be_lv_point_t = {
  4,  /* size in bytes */
  2,  /* number of elements */
  (const be_ctypes_structure_item_t[2]) {
    { "x", 0, 0, 0, 12 },
    { "y", 2, 0, 0, 12 },
}};

const be_ctypes_structure_t be_lv_area_t = {
  8,  /* size in bytes */
  4,  /* number of elements */
  (const be_ctypes_structure_item_t[4]) {
    { "x1", 0, 0, 0, 12 },
    { "x2", 4, 0, 0, 12 },
    { "y1", 2, 0, 0, 12 },
    { "y2", 6, 0, 0, 12 },
}};

const be_ctypes_structure_t be_lv_draw_rect_dsc_t = {
  77,  /* size in bytes */
  43,  /* number of elements */
  (const be_ctypes_structure_item_t[43]) {
    { "bg_blend_mode", 12, 0, 0, 1 },
    { "bg_color", 2, 0, 0, 2 },
    { "bg_grad_color", 4, 0, 0, 2 },
    { "bg_grad_color_stop", 9, 0, 0, 12 },
    { "bg_grad_dir", 6, 0, 0, 1 },
    { "bg_main_color_stop", 7, 0, 0, 12 },
    { "bg_opa", 11, 0, 0, 1 },
    { "border_blend_mode", 20, 0, 0, 1 },
    { "border_color", 13, 0, 0, 2 },
    { "border_opa", 19, 0, 0, 1 },
    { "border_post", 21, 0, 1, 0 },
    { "border_side", 17, 0, 0, 12 },
    { "border_width", 15, 0, 0, 12 },
    { "outline_blend_mode", 29, 0, 0, 1 },
    { "outline_color", 22, 0, 0, 2 },
    { "outline_opa", 28, 0, 0, 1 },
    { "outline_pad", 26, 0, 0, 12 },
    { "outline_width", 24, 0, 0, 12 },
    { "pattern_blend_mode", 55, 0, 0, 1 },
    { "pattern_font", 46, 0, 0, 4 },
    { "pattern_image", 42, 0, 0, 4 },
    { "pattern_opa", 52, 0, 0, 1 },
    { "pattern_recolor", 50, 0, 0, 2 },
    { "pattern_recolor_opa", 53, 0, 0, 1 },
    { "pattern_repeat", 54, 0, 1, 0 },
    { "radius", 0, 0, 0, 12 },
    { "shadow_blend_mode", 41, 0, 0, 1 },
    { "shadow_color", 30, 0, 0, 2 },
    { "shadow_ofs_x", 34, 0, 0, 12 },
    { "shadow_ofs_y", 36, 0, 0, 12 },
    { "shadow_opa", 40, 0, 0, 1 },
    { "shadow_spread", 38, 0, 0, 12 },
    { "shadow_width", 32, 0, 0, 12 },
    { "value_align", 75, 0, 0, 1 },
    { "value_blend_mode", 76, 0, 0, 1 },
    { "value_color", 65, 0, 0, 2 },
    { "value_font", 60, 0, 0, 4 },
    { "value_letter_space", 71, 0, 0, 12 },
    { "value_line_space", 73, 0, 0, 12 },
    { "value_ofs_x", 67, 0, 0, 12 },
    { "value_ofs_y", 69, 0, 0, 12 },
    { "value_opa", 64, 0, 0, 1 },
    { "value_str", 56, 0, 0, 4 },
}};

const be_ctypes_structure_t be_lv_draw_line_dsc_t = {
  10,  /* size in bytes */
  9,  /* number of elements */
  (const be_ctypes_structure_item_t[9]) {
    { "blend_mode", 9, 0, 2, 0 },
    { "color", 0, 0, 0, 2 },
    { "dash_gap", 6, 0, 0, 12 },
    { "dash_width", 4, 0, 0, 12 },
    { "opa", 8, 0, 0, 1 },
    { "raw_end", 9, 4, 1, 0 },
    { "round_end", 9, 3, 1, 0 },
    { "round_start", 9, 2, 1, 0 },
    { "width", 2, 0, 0, 12 },
}};

const be_ctypes_structure_t be_lv_draw_img_dsc_t = {
  14,  /* size in bytes */
  9,  /* number of elements */
  (const be_ctypes_structure_item_t[9]) {
    { "angle", 1, 0, 0, 2 },
    { "antialias", 13, 0, 1, 0 },
    { "blend_mode", 12, 0, 0, 1 },
    { "opa", 0, 0, 0, 1 },
    { "pivot_x", 3, 0, 0, 12 },
    { "pivot_y", 5, 0, 0, 12 },
    { "recolor", 10, 0, 0, 2 },
    { "recolor_opa", 9, 0, 0, 1 },
    { "zoom", 7, 0, 0, 2 },
}};

const be_ctypes_structure_t be_lv_draw_label_dsc_t = {
  31,  /* size in bytes */
  15,  /* number of elements */
  (const be_ctypes_structure_item_t[15]) {
    { "bidi_dir", 27, 0, 0, 1 },
    { "blend_mode", 30, 0, 0, 1 },
    { "color", 0, 0, 0, 2 },
    { "decor", 29, 0, 0, 1 },
    { "flag", 28, 0, 0, 1 },
    { "font", 6, 0, 0, 4 },
    { "letter_space", 13, 0, 0, 12 },
    { "line_space", 11, 0, 0, 12 },
    { "ofs_x", 23, 0, 0, 12 },
    { "ofs_y", 25, 0, 0, 12 },
    { "opa", 10, 0, 0, 1 },
    { "sel_bg_color", 4, 0, 0, 2 },
    { "sel_color", 2, 0, 0, 2 },
    { "sel_end", 19, 0, 0, 4 },
    { "sel_start", 15, 0, 0, 4 },
}};

const be_ctypes_structure_t be_lv_draw_mask_common_dsc_t = {
  5,  /* size in bytes */
  2,  /* number of elements */
  (const be_ctypes_structure_item_t[2]) {
    { "cb", 0, 0, 0, 4 },
    { "type", 4, 0, 0, 1 },
}};

const be_ctypes_structure_t be_lv_draw_mask_line_param_cfg_t = {
  9,  /* size in bytes */
  5,  /* number of elements */
  (const be_ctypes_structure_item_t[5]) {
    { "p1_x", 0, 0, 0, 12 },
    { "p1_y", 2, 0, 0, 12 },
    { "p2_x", 4, 0, 0, 12 },
    { "p2_y", 6, 0, 0, 12 },
    { "side", 8, 0, 2, 0 },
}};

const be_ctypes_structure_t be_lv_draw_mask_line_param_t = {
  35,  /* size in bytes */
  15,  /* number of elements */
  (const be_ctypes_structure_item_t[15]) {
    { "cfg_p1_x", 5, 0, 0, 12 },
    { "cfg_p1_y", 7, 0, 0, 12 },
    { "cfg_p2_x", 9, 0, 0, 12 },
    { "cfg_p2_y", 11, 0, 0, 12 },
    { "cfg_side", 13, 0, 2, 0 },
    { "dsc_cb", 0, 0, 0, 4 },
    { "dsc_type", 4, 0, 0, 1 },
    { "flat", 34, 0, 1, 0 },
    { "inv", 34, 1, 1, 0 },
    { "origo_x", 14, 0, 0, 12 },
    { "origo_y", 16, 0, 0, 12 },
    { "spx", 30, 0, 0, 14 },
    { "steep", 26, 0, 0, 14 },
    { "xy_steep", 18, 0, 0, 14 },
    { "yx_steep", 22, 0, 0, 14 },
}};

const be_ctypes_structure_t be_lv_draw_mask_angle_param_cfg_t = {
  8,  /* size in bytes */
  4,  /* number of elements */
  (const be_ctypes_structure_item_t[4]) {
    { "end_angle", 6, 0, 0, 12 },
    { "start_angle", 4, 0, 0, 12 },
    { "vertex_p_x", 0, 0, 0, 12 },
    { "vertex_p_y", 2, 0, 0, 12 },
}};

const be_ctypes_structure_t be_lv_draw_mask_angle_param_t = {
  85,  /* size in bytes */
  37,  /* number of elements */
  (const be_ctypes_structure_item_t[37]) {
    { "cfg_end_angle", 11, 0, 0, 12 },
    { "cfg_start_angle", 9, 0, 0, 12 },
    { "cfg_vertex_p_x", 5, 0, 0, 12 },
    { "cfg_vertex_p_y", 7, 0, 0, 12 },
    { "delta_deg", 83, 0, 0, 2 },
    { "dsc_cb", 0, 0, 0, 4 },
    { "dsc_type", 4, 0, 0, 1 },
    { "end_line_cfg_p1_x", 53, 0, 0, 12 },
    { "end_line_cfg_p1_y", 55, 0, 0, 12 },
    { "end_line_cfg_p2_x", 57, 0, 0, 12 },
    { "end_line_cfg_p2_y", 59, 0, 0, 12 },
    { "end_line_cfg_side", 61, 0, 2, 0 },
    { "end_line_dsc_cb", 48, 0, 0, 4 },
    { "end_line_dsc_type", 52, 0, 0, 1 },
    { "end_line_flat", 82, 0, 1, 0 },
    { "end_line_inv", 82, 1, 1, 0 },
    { "end_line_origo_x", 62, 0, 0, 12 },
    { "end_line_origo_y", 64, 0, 0, 12 },
    { "end_line_spx", 78, 0, 0, 14 },
    { "end_line_steep", 74, 0, 0, 14 },
    { "end_line_xy_steep", 66, 0, 0, 14 },
    { "end_line_yx_steep", 70, 0, 0, 14 },
    { "start_line_cfg_p1_x", 18, 0, 0, 12 },
    { "start_line_cfg_p1_y", 20, 0, 0, 12 },
    { "start_line_cfg_p2_x", 22, 0, 0, 12 },
    { "start_line_cfg_p2_y", 24, 0, 0, 12 },
    { "start_line_cfg_side", 26, 0, 2, 0 },
    { "start_line_dsc_cb", 13, 0, 0, 4 },
    { "start_line_dsc_type", 17, 0, 0, 1 },
    { "start_line_flat", 47, 0, 1, 0 },
    { "start_line_inv", 47, 1, 1, 0 },
    { "start_line_origo_x", 27, 0, 0, 12 },
    { "start_line_origo_y", 29, 0, 0, 12 },
    { "start_line_spx", 43, 0, 0, 14 },
    { "start_line_steep", 39, 0, 0, 14 },
    { "start_line_xy_steep", 31, 0, 0, 14 },
    { "start_line_yx_steep", 35, 0, 0, 14 },
}};

const be_ctypes_structure_t be_lv_draw_mask_radius_param_cfg_t = {
  11,  /* size in bytes */
  6,  /* number of elements */
  (const be_ctypes_structure_item_t[6]) {
    { "outer", 10, 0, 1, 0 },
    { "radius", 8, 0, 0, 12 },
    { "rect_x1", 0, 0, 0, 12 },
    { "rect_x2", 4, 0, 0, 12 },
    { "rect_y1", 2, 0, 0, 12 },
    { "rect_y2", 6, 0, 0, 12 },
}};

const be_ctypes_structure_t be_lv_sqrt_res_t = {
  4,  /* size in bytes */
  2,  /* number of elements */
  (const be_ctypes_structure_item_t[2]) {
    { "f", 2, 0, 0, 2 },
    { "i", 0, 0, 0, 2 },
}};

const be_ctypes_structure_t be_lv_draw_mask_radius_param_t = {
  24,  /* size in bytes */
  11,  /* number of elements */
  (const be_ctypes_structure_item_t[11]) {
    { "cfg_outer", 15, 0, 1, 0 },
    { "cfg_radius", 13, 0, 0, 12 },
    { "cfg_rect_x1", 5, 0, 0, 12 },
    { "cfg_rect_x2", 9, 0, 0, 12 },
    { "cfg_rect_y1", 7, 0, 0, 12 },
    { "cfg_rect_y2", 11, 0, 0, 12 },
    { "dsc_cb", 0, 0, 0, 4 },
    { "dsc_type", 4, 0, 0, 1 },
    { "y_prev", 16, 0, 0, 14 },
    { "y_prev_x_f", 22, 0, 0, 2 },
    { "y_prev_x_i", 20, 0, 0, 2 },
}};

const be_ctypes_structure_t be_lv_draw_mask_fade_param_cfg_t = {
  14,  /* size in bytes */
  8,  /* number of elements */
  (const be_ctypes_structure_item_t[8]) {
    { "coords_x1", 0, 0, 0, 12 },
    { "coords_x2", 4, 0, 0, 12 },
    { "coords_y1", 2, 0, 0, 12 },
    { "coords_y2", 6, 0, 0, 12 },
    { "opa_bottom", 13, 0, 0, 1 },
    { "opa_top", 12, 0, 0, 1 },
    { "y_bottom", 10, 0, 0, 12 },
    { "y_top", 8, 0, 0, 12 },
}};

const be_ctypes_structure_t be_lv_draw_mask_fade_param_t = {
  19,  /* size in bytes */
  10,  /* number of elements */
  (const be_ctypes_structure_item_t[10]) {
    { "cfg_coords_x1", 5, 0, 0, 12 },
    { "cfg_coords_x2", 9, 0, 0, 12 },
    { "cfg_coords_y1", 7, 0, 0, 12 },
    { "cfg_coords_y2", 11, 0, 0, 12 },
    { "cfg_opa_bottom", 18, 0, 0, 1 },
    { "cfg_opa_top", 17, 0, 0, 1 },
    { "cfg_y_bottom", 15, 0, 0, 12 },
    { "cfg_y_top", 13, 0, 0, 12 },
    { "dsc_cb", 0, 0, 0, 4 },
    { "dsc_type", 4, 0, 0, 1 },
}};

const be_ctypes_structure_t be_lv_draw_mask_map_param_cfg_t = {
  12,  /* size in bytes */
  5,  /* number of elements */
  (const be_ctypes_structure_item_t[5]) {
    { "coords_x1", 0, 0, 0, 12 },
    { "coords_x2", 4, 0, 0, 12 },
    { "coords_y1", 2, 0, 0, 12 },
    { "coords_y2", 6, 0, 0, 12 },
    { "map", 8, 0, 0, 4 },
}};

const be_ctypes_structure_t be_lv_draw_mask_map_param_t = {
  17,  /* size in bytes */
  7,  /* number of elements */
  (const be_ctypes_structure_item_t[7]) {
    { "cfg_coords_x1", 5, 0, 0, 12 },
    { "cfg_coords_x2", 9, 0, 0, 12 },
    { "cfg_coords_y1", 7, 0, 0, 12 },
    { "cfg_coords_y2", 11, 0, 0, 12 },
    { "cfg_map", 13, 0, 0, 4 },
    { "dsc_cb", 0, 0, 0, 4 },
    { "dsc_type", 4, 0, 0, 1 },
}};

const be_ctypes_structure_t be_lv_draw_mask_saved_t = {
  8,  /* size in bytes */
  2,  /* number of elements */
  (const be_ctypes_structure_item_t[2]) {
    { "custom_id", 4, 0, 0, 4 },
    { "param", 0, 0, 0, 4 },
}};

const be_ctypes_classes_t be_ctypes_classes[] = {
  19,
  (const be_ctypes_class_t[19]) {
    { "lv_area_t", &be_lv_area_t },
    { "lv_draw_img_dsc_t", &be_lv_draw_img_dsc_t },
    { "lv_draw_label_dsc_t", &be_lv_draw_label_dsc_t },
    { "lv_draw_line_dsc_t", &be_lv_draw_line_dsc_t },
    { "lv_draw_mask_angle_param_cfg_t", &be_lv_draw_mask_angle_param_cfg_t },
    { "lv_draw_mask_angle_param_t", &be_lv_draw_mask_angle_param_t },
    { "lv_draw_mask_common_dsc_t", &be_lv_draw_mask_common_dsc_t },
    { "lv_draw_mask_fade_param_cfg_t", &be_lv_draw_mask_fade_param_cfg_t },
    { "lv_draw_mask_fade_param_t", &be_lv_draw_mask_fade_param_t },
    { "lv_draw_mask_line_param_cfg_t", &be_lv_draw_mask_line_param_cfg_t },
    { "lv_draw_mask_line_param_t", &be_lv_draw_mask_line_param_t },
    { "lv_draw_mask_map_param_cfg_t", &be_lv_draw_mask_map_param_cfg_t },
    { "lv_draw_mask_map_param_t", &be_lv_draw_mask_map_param_t },
    { "lv_draw_mask_radius_param_cfg_t", &be_lv_draw_mask_radius_param_cfg_t },
    { "lv_draw_mask_radius_param_t", &be_lv_draw_mask_radius_param_t },
    { "lv_draw_mask_saved_t", &be_lv_draw_mask_saved_t },
    { "lv_draw_rect_dsc_t", &be_lv_draw_rect_dsc_t },
    { "lv_point_t", &be_lv_point_t },
    { "lv_sqrt_res_t", &be_lv_sqrt_res_t },
}};

/* @const_object_info_begin
class be_class_ctypes_classes (scope: global) {
    lv_area_t, int(0)
    lv_draw_img_dsc_t, int(0)
    lv_draw_label_dsc_t, int(0)
    lv_draw_line_dsc_t, int(0)
    lv_draw_mask_angle_param_cfg_t, int(0)
    lv_draw_mask_angle_param_t, int(0)
    lv_draw_mask_common_dsc_t, int(0)
    lv_draw_mask_fade_param_cfg_t, int(0)
    lv_draw_mask_fade_param_t, int(0)
    lv_draw_mask_line_param_cfg_t, int(0)
    lv_draw_mask_line_param_t, int(0)
    lv_draw_mask_map_param_cfg_t, int(0)
    lv_draw_mask_map_param_t, int(0)
    lv_draw_mask_radius_param_cfg_t, int(0)
    lv_draw_mask_radius_param_t, int(0)
    lv_draw_mask_saved_t, int(0)
    lv_draw_rect_dsc_t, int(0)
    lv_point_t, int(0)
    lv_sqrt_res_t, int(0)
}
@const_object_info_end */

void be_load_ctypes_definitions_lib(bvm *vm) {
  be_pushcomptr(vm, (void*) be_ctypes_classes);
  be_setglobal(vm, ".ctypes_classes");
  be_pop(vm, 1);

  static be_define_const_empty_class(be_class_lv_area_t, &be_class_lv_ctypes, lv_area_t);
  ctypes_register_class(vm, &be_class_lv_area_t, &be_lv_area_t);
  static be_define_const_empty_class(be_class_lv_draw_img_dsc_t, &be_class_lv_ctypes, lv_draw_img_dsc_t);
  ctypes_register_class(vm, &be_class_lv_draw_img_dsc_t, &be_lv_draw_img_dsc_t);
  static be_define_const_empty_class(be_class_lv_draw_label_dsc_t, &be_class_lv_ctypes, lv_draw_label_dsc_t);
  ctypes_register_class(vm, &be_class_lv_draw_label_dsc_t, &be_lv_draw_label_dsc_t);
  static be_define_const_empty_class(be_class_lv_draw_line_dsc_t, &be_class_lv_ctypes, lv_draw_line_dsc_t);
  ctypes_register_class(vm, &be_class_lv_draw_line_dsc_t, &be_lv_draw_line_dsc_t);
  static be_define_const_empty_class(be_class_lv_draw_mask_angle_param_cfg_t, &be_class_lv_ctypes, lv_draw_mask_angle_param_cfg_t);
  ctypes_register_class(vm, &be_class_lv_draw_mask_angle_param_cfg_t, &be_lv_draw_mask_angle_param_cfg_t);
  static be_define_const_empty_class(be_class_lv_draw_mask_angle_param_t, &be_class_lv_ctypes, lv_draw_mask_angle_param_t);
  ctypes_register_class(vm, &be_class_lv_draw_mask_angle_param_t, &be_lv_draw_mask_angle_param_t);
  static be_define_const_empty_class(be_class_lv_draw_mask_common_dsc_t, &be_class_lv_ctypes, lv_draw_mask_common_dsc_t);
  ctypes_register_class(vm, &be_class_lv_draw_mask_common_dsc_t, &be_lv_draw_mask_common_dsc_t);
  static be_define_const_empty_class(be_class_lv_draw_mask_fade_param_cfg_t, &be_class_lv_ctypes, lv_draw_mask_fade_param_cfg_t);
  ctypes_register_class(vm, &be_class_lv_draw_mask_fade_param_cfg_t, &be_lv_draw_mask_fade_param_cfg_t);
  static be_define_const_empty_class(be_class_lv_draw_mask_fade_param_t, &be_class_lv_ctypes, lv_draw_mask_fade_param_t);
  ctypes_register_class(vm, &be_class_lv_draw_mask_fade_param_t, &be_lv_draw_mask_fade_param_t);
  static be_define_const_empty_class(be_class_lv_draw_mask_line_param_cfg_t, &be_class_lv_ctypes, lv_draw_mask_line_param_cfg_t);
  ctypes_register_class(vm, &be_class_lv_draw_mask_line_param_cfg_t, &be_lv_draw_mask_line_param_cfg_t);
  static be_define_const_empty_class(be_class_lv_draw_mask_line_param_t, &be_class_lv_ctypes, lv_draw_mask_line_param_t);
  ctypes_register_class(vm, &be_class_lv_draw_mask_line_param_t, &be_lv_draw_mask_line_param_t);
  static be_define_const_empty_class(be_class_lv_draw_mask_map_param_cfg_t, &be_class_lv_ctypes, lv_draw_mask_map_param_cfg_t);
  ctypes_register_class(vm, &be_class_lv_draw_mask_map_param_cfg_t, &be_lv_draw_mask_map_param_cfg_t);
  static be_define_const_empty_class(be_class_lv_draw_mask_map_param_t, &be_class_lv_ctypes, lv_draw_mask_map_param_t);
  ctypes_register_class(vm, &be_class_lv_draw_mask_map_param_t, &be_lv_draw_mask_map_param_t);
  static be_define_const_empty_class(be_class_lv_draw_mask_radius_param_cfg_t, &be_class_lv_ctypes, lv_draw_mask_radius_param_cfg_t);
  ctypes_register_class(vm, &be_class_lv_draw_mask_radius_param_cfg_t, &be_lv_draw_mask_radius_param_cfg_t);
  static be_define_const_empty_class(be_class_lv_draw_mask_radius_param_t, &be_class_lv_ctypes, lv_draw_mask_radius_param_t);
  ctypes_register_class(vm, &be_class_lv_draw_mask_radius_param_t, &be_lv_draw_mask_radius_param_t);
  static be_define_const_empty_class(be_class_lv_draw_mask_saved_t, &be_class_lv_ctypes, lv_draw_mask_saved_t);
  ctypes_register_class(vm, &be_class_lv_draw_mask_saved_t, &be_lv_draw_mask_saved_t);
  static be_define_const_empty_class(be_class_lv_draw_rect_dsc_t, &be_class_lv_ctypes, lv_draw_rect_dsc_t);
  ctypes_register_class(vm, &be_class_lv_draw_rect_dsc_t, &be_lv_draw_rect_dsc_t);
  static be_define_const_empty_class(be_class_lv_point_t, &be_class_lv_ctypes, lv_point_t);
  ctypes_register_class(vm, &be_class_lv_point_t, &be_lv_point_t);
  static be_define_const_empty_class(be_class_lv_sqrt_res_t, &be_class_lv_ctypes, lv_sqrt_res_t);
  ctypes_register_class(vm, &be_class_lv_sqrt_res_t, &be_lv_sqrt_res_t);
}

/*******************************************************************/


#endif // USE_LVGL