/*
  xdrv_52_3_berry_native.ino - Berry scripting language, native fucnctions

  Copyright (C) 2021 Stephan Hadinger, Berry language by Guan Wenliang https://github.com/Skiars/berry

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifdef USE_BERRY
#ifdef USE_LVGL

#include <berry.h>
#include "lvgl.h"
#include "be_lvgl.h"

// Berry easy logging
extern "C" {
  extern void berry_log_C(const char * berry_buf, ...);
}


/********************************************************************
 * Generated code, don't edit
 *******************************************************************/
enum {
  ctypes_ptr = 0,
  ctypes_uint = 1,
  ctypes_int = 2,
  ctypes_str = 3,
};

typedef struct be_ctypes_structure_t {
  const char * name;
  uint16_t  offset_bits;
  uint16_t  len_bits : 13;
  uint16_t  type : 3;
} be_ctypes_structure_t;
const be_ctypes_structure_t be_lv_point_t[2] = {
  { "x", 0, 16, ctypes_int },
  { "y", 16, 16, ctypes_int },
};

const be_ctypes_structure_t be_lv_area_t[4] = {
  { "x1", 0, 16, ctypes_int },
  { "x2", 32, 16, ctypes_int },
  { "y1", 16, 16, ctypes_int },
  { "y2", 48, 16, ctypes_int },
};

const be_ctypes_structure_t be_test_t[8] = {
  { "a_x1", 0, 16, ctypes_int },
  { "a_x2", 32, 16, ctypes_int },
  { "a_y1", 16, 16, ctypes_int },
  { "a_y2", 48, 16, ctypes_int },
  { "b_x1", 64, 16, ctypes_int },
  { "b_x2", 96, 16, ctypes_int },
  { "b_y1", 80, 16, ctypes_int },
  { "b_y2", 112, 16, ctypes_int },
};

const be_ctypes_structure_t be_lv_draw_rect_dsc_t[43] = {
  { "bg_blend_mode", 96, 8, ctypes_uint },
  { "bg_color", 16, 16, ctypes_uint },
  { "bg_grad_color", 32, 16, ctypes_uint },
  { "bg_grad_color_stop", 72, 16, ctypes_int },
  { "bg_grad_dir", 48, 8, ctypes_uint },
  { "bg_main_color_stop", 56, 16, ctypes_int },
  { "bg_opa", 88, 8, ctypes_uint },
  { "border_blend_mode", 160, 8, ctypes_uint },
  { "border_color", 104, 16, ctypes_uint },
  { "border_opa", 152, 8, ctypes_uint },
  { "border_post", 168, 1, ctypes_uint },
  { "border_side", 136, 16, ctypes_int },
  { "border_width", 120, 16, ctypes_int },
  { "outline_blend_mode", 232, 8, ctypes_uint },
  { "outline_color", 176, 16, ctypes_uint },
  { "outline_opa", 224, 8, ctypes_uint },
  { "outline_pad", 208, 16, ctypes_int },
  { "outline_width", 192, 16, ctypes_int },
  { "pattern_blend_mode", 440, 8, ctypes_uint },
  { "pattern_font", 368, 32, ctypes_uint },
  { "pattern_image", 336, 32, ctypes_uint },
  { "pattern_opa", 416, 8, ctypes_uint },
  { "pattern_recolor", 400, 16, ctypes_uint },
  { "pattern_recolor_opa", 424, 8, ctypes_uint },
  { "pattern_repeat", 432, 1, ctypes_uint },
  { "radius", 0, 16, ctypes_int },
  { "shadow_blend_mode", 328, 8, ctypes_uint },
  { "shadow_color", 240, 16, ctypes_uint },
  { "shadow_ofs_x", 272, 16, ctypes_int },
  { "shadow_ofs_y", 288, 16, ctypes_int },
  { "shadow_opa", 320, 8, ctypes_uint },
  { "shadow_spread", 304, 16, ctypes_int },
  { "shadow_width", 256, 16, ctypes_int },
  { "value_align", 600, 8, ctypes_uint },
  { "value_blend_mode", 608, 8, ctypes_uint },
  { "value_color", 520, 16, ctypes_uint },
  { "value_font", 480, 32, ctypes_uint },
  { "value_letter_space", 568, 16, ctypes_int },
  { "value_line_space", 584, 16, ctypes_int },
  { "value_ofs_x", 536, 16, ctypes_int },
  { "value_ofs_y", 552, 16, ctypes_int },
  { "value_opa", 512, 8, ctypes_uint },
  { "value_str", 448, 32, ctypes_uint },
};

const be_ctypes_structure_t be_lv_draw_line_dsc_t[9] = {
  { "blend_mode", 72, 2, ctypes_uint },
  { "color", 0, 16, ctypes_uint },
  { "dash_gap", 48, 16, ctypes_int },
  { "dash_width", 32, 16, ctypes_int },
  { "opa", 64, 8, ctypes_uint },
  { "raw_end", 76, 1, ctypes_uint },
  { "round_end", 75, 1, ctypes_uint },
  { "round_start", 74, 1, ctypes_uint },
  { "width", 16, 16, ctypes_int },
};

const be_ctypes_structure_t be_lv_draw_img_dsc_t[9] = {
  { "angle", 8, 16, ctypes_uint },
  { "antialias", 104, 1, ctypes_uint },
  { "blend_mode", 96, 8, ctypes_uint },
  { "opa", 0, 8, ctypes_uint },
  { "pivot_x", 24, 16, ctypes_int },
  { "pivot_y", 40, 16, ctypes_int },
  { "recolor", 80, 16, ctypes_uint },
  { "recolor_opa", 72, 8, ctypes_uint },
  { "zoom", 56, 16, ctypes_uint },
};

const be_ctypes_structure_t be_lv_draw_label_dsc_t[15] = {
  { "bidi_dir", 216, 8, ctypes_uint },
  { "blend_mode", 240, 8, ctypes_uint },
  { "color", 0, 16, ctypes_uint },
  { "decor", 232, 8, ctypes_uint },
  { "flag", 224, 8, ctypes_uint },
  { "font", 48, 32, ctypes_uint },
  { "letter_space", 104, 16, ctypes_int },
  { "line_space", 88, 16, ctypes_int },
  { "ofs_x", 184, 16, ctypes_int },
  { "ofs_y", 200, 16, ctypes_int },
  { "opa", 80, 8, ctypes_uint },
  { "sel_bg_color", 32, 16, ctypes_uint },
  { "sel_color", 16, 16, ctypes_uint },
  { "sel_end", 152, 32, ctypes_uint },
  { "sel_start", 120, 32, ctypes_uint },
};

const be_ctypes_structure_t be_lv_draw_mask_common_dsc_t[2] = {
  { "cb", 0, 32, ctypes_uint },
  { "type", 32, 8, ctypes_uint },
};

const be_ctypes_structure_t be_lv_draw_mask_line_param_cfg_t[5] = {
  { "p1_x", 0, 16, ctypes_int },
  { "p1_y", 16, 16, ctypes_int },
  { "p2_x", 32, 16, ctypes_int },
  { "p2_y", 48, 16, ctypes_int },
  { "side", 64, 2, ctypes_uint },
};

const be_ctypes_structure_t be_lv_draw_mask_line_param_t[15] = {
  { "cfg_p1_x", 40, 16, ctypes_int },
  { "cfg_p1_y", 56, 16, ctypes_int },
  { "cfg_p2_x", 72, 16, ctypes_int },
  { "cfg_p2_y", 88, 16, ctypes_int },
  { "cfg_side", 104, 2, ctypes_uint },
  { "dsc_cb", 0, 32, ctypes_uint },
  { "dsc_type", 32, 8, ctypes_uint },
  { "flat", 272, 1, ctypes_uint },
  { "inv", 273, 1, ctypes_uint },
  { "origo_x", 112, 16, ctypes_int },
  { "origo_y", 128, 16, ctypes_int },
  { "spx", 240, 32, ctypes_int },
  { "steep", 208, 32, ctypes_int },
  { "xy_steep", 144, 32, ctypes_int },
  { "yx_steep", 176, 32, ctypes_int },
};

const be_ctypes_structure_t be_lv_draw_mask_angle_param_cfg_t[4] = {
  { "end_angle", 48, 16, ctypes_int },
  { "start_angle", 32, 16, ctypes_int },
  { "vertex_p_x", 0, 16, ctypes_int },
  { "vertex_p_y", 16, 16, ctypes_int },
};

const be_ctypes_structure_t be_lv_draw_mask_angle_param_t[37] = {
  { "cfg_end_angle", 88, 16, ctypes_int },
  { "cfg_start_angle", 72, 16, ctypes_int },
  { "cfg_vertex_p_x", 40, 16, ctypes_int },
  { "cfg_vertex_p_y", 56, 16, ctypes_int },
  { "delta_deg", 664, 16, ctypes_uint },
  { "dsc_cb", 0, 32, ctypes_uint },
  { "dsc_type", 32, 8, ctypes_uint },
  { "end_line_cfg_p1_x", 424, 16, ctypes_int },
  { "end_line_cfg_p1_y", 440, 16, ctypes_int },
  { "end_line_cfg_p2_x", 456, 16, ctypes_int },
  { "end_line_cfg_p2_y", 472, 16, ctypes_int },
  { "end_line_cfg_side", 488, 2, ctypes_uint },
  { "end_line_dsc_cb", 384, 32, ctypes_uint },
  { "end_line_dsc_type", 416, 8, ctypes_uint },
  { "end_line_flat", 656, 1, ctypes_uint },
  { "end_line_inv", 657, 1, ctypes_uint },
  { "end_line_origo_x", 496, 16, ctypes_int },
  { "end_line_origo_y", 512, 16, ctypes_int },
  { "end_line_spx", 624, 32, ctypes_int },
  { "end_line_steep", 592, 32, ctypes_int },
  { "end_line_xy_steep", 528, 32, ctypes_int },
  { "end_line_yx_steep", 560, 32, ctypes_int },
  { "start_line_cfg_p1_x", 144, 16, ctypes_int },
  { "start_line_cfg_p1_y", 160, 16, ctypes_int },
  { "start_line_cfg_p2_x", 176, 16, ctypes_int },
  { "start_line_cfg_p2_y", 192, 16, ctypes_int },
  { "start_line_cfg_side", 208, 2, ctypes_uint },
  { "start_line_dsc_cb", 104, 32, ctypes_uint },
  { "start_line_dsc_type", 136, 8, ctypes_uint },
  { "start_line_flat", 376, 1, ctypes_uint },
  { "start_line_inv", 377, 1, ctypes_uint },
  { "start_line_origo_x", 216, 16, ctypes_int },
  { "start_line_origo_y", 232, 16, ctypes_int },
  { "start_line_spx", 344, 32, ctypes_int },
  { "start_line_steep", 312, 32, ctypes_int },
  { "start_line_xy_steep", 248, 32, ctypes_int },
  { "start_line_yx_steep", 280, 32, ctypes_int },
};

const be_ctypes_structure_t be_lv_draw_mask_radius_param_cfg_t[6] = {
  { "outer", 80, 1, ctypes_uint },
  { "radius", 64, 16, ctypes_int },
  { "rect_x1", 0, 16, ctypes_int },
  { "rect_x2", 32, 16, ctypes_int },
  { "rect_y1", 16, 16, ctypes_int },
  { "rect_y2", 48, 16, ctypes_int },
};

const be_ctypes_structure_t be_lv_sqrt_res_t[2] = {
  { "f", 16, 16, ctypes_uint },
  { "i", 0, 16, ctypes_uint },
};

const be_ctypes_structure_t be_lv_draw_mask_radius_param_t[11] = {
  { "cfg_outer", 120, 1, ctypes_uint },
  { "cfg_radius", 104, 16, ctypes_int },
  { "cfg_rect_x1", 40, 16, ctypes_int },
  { "cfg_rect_x2", 72, 16, ctypes_int },
  { "cfg_rect_y1", 56, 16, ctypes_int },
  { "cfg_rect_y2", 88, 16, ctypes_int },
  { "dsc_cb", 0, 32, ctypes_uint },
  { "dsc_type", 32, 8, ctypes_uint },
  { "y_prev", 128, 32, ctypes_int },
  { "y_prev_x_f", 176, 16, ctypes_uint },
  { "y_prev_x_i", 160, 16, ctypes_uint },
};

const be_ctypes_structure_t be_lv_draw_mask_fade_param_cfg_t[8] = {
  { "coords_x1", 0, 16, ctypes_int },
  { "coords_x2", 32, 16, ctypes_int },
  { "coords_y1", 16, 16, ctypes_int },
  { "coords_y2", 48, 16, ctypes_int },
  { "opa_bottom", 104, 8, ctypes_uint },
  { "opa_top", 96, 8, ctypes_uint },
  { "y_bottom", 80, 16, ctypes_int },
  { "y_top", 64, 16, ctypes_int },
};

const be_ctypes_structure_t be_lv_draw_mask_fade_param_t[10] = {
  { "cfg_coords_x1", 40, 16, ctypes_int },
  { "cfg_coords_x2", 72, 16, ctypes_int },
  { "cfg_coords_y1", 56, 16, ctypes_int },
  { "cfg_coords_y2", 88, 16, ctypes_int },
  { "cfg_opa_bottom", 144, 8, ctypes_uint },
  { "cfg_opa_top", 136, 8, ctypes_uint },
  { "cfg_y_bottom", 120, 16, ctypes_int },
  { "cfg_y_top", 104, 16, ctypes_int },
  { "dsc_cb", 0, 32, ctypes_uint },
  { "dsc_type", 32, 8, ctypes_uint },
};

const be_ctypes_structure_t be_lv_draw_mask_map_param_cfg_t[5] = {
  { "coords_x1", 0, 16, ctypes_int },
  { "coords_x2", 32, 16, ctypes_int },
  { "coords_y1", 16, 16, ctypes_int },
  { "coords_y2", 48, 16, ctypes_int },
  { "map", 64, 32, ctypes_uint },
};

const be_ctypes_structure_t be_lv_draw_mask_map_param_t[7] = {
  { "cfg_coords_x1", 40, 16, ctypes_int },
  { "cfg_coords_x2", 72, 16, ctypes_int },
  { "cfg_coords_y1", 56, 16, ctypes_int },
  { "cfg_coords_y2", 88, 16, ctypes_int },
  { "cfg_map", 104, 32, ctypes_uint },
  { "dsc_cb", 0, 32, ctypes_uint },
  { "dsc_type", 32, 8, ctypes_uint },
};

const be_ctypes_structure_t be_lv_draw_mask_saved_t[2] = {
  { "custom_id", 32, 32, ctypes_uint },
  { "param", 0, 32, ctypes_uint },
};

/********************************************************************/

#endif // USE_LVGL

#endif  // USE_BERRY
