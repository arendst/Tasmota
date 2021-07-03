#
# ctype buidings for LVGL
#
# To generate C bindings, do:
# > compile("lvgl_ctypes.be","file")()
#
# and copy/paste output in C format
#
import ctypes

ctypes.print_types()

lv_style_int_t = ctypes.i16
lv_color_t = ctypes.u16        # depends on colors
lv_grad_dir_t = ctypes.u8
lv_opa_t = ctypes.u8
lv_blend_mode_t = ctypes.u8
lv_align_t = ctypes.u8
lv_coord_t = ctypes.i16
lv_bidi_dir_t = ctypes.u8
lv_txt_flag_t = ctypes.u8
lv_text_decor_t = ctypes.u8
lv_font_t = ctypes.u32
uint8_t_1 = ctypes.bf_1
uint8_t_2 = ctypes.bf_2
uint8_t = ctypes.u8
uint16_t = ctypes.u16
uint32_t = ctypes.u32
int32_t = ctypes.i32
ptr = ctypes.u32

lv_point_t = [
    [lv_coord_t, "x"],
    [lv_coord_t, "y"],
]
lv_point_t = ctypes.structure(lv_point_t, "lv_point_t")

lv_area_t = [
    [lv_coord_t, "x1"],
    [lv_coord_t, "y1"],
    [lv_coord_t, "x2"],
    [lv_coord_t, "y2"],
]
lv_area_t = ctypes.structure(lv_area_t, "lv_area_t")

# test_t = [
#     [lv_area_t, "a"],
#     [lv_area_t, "b"]
# ]
# test_t = ctypes.structure(test_t, "test_t")

lv_draw_rect_dsc_t = [
    [lv_style_int_t, "radius"],

    #/*Background*/
    [lv_color_t, "bg_color"],
    [lv_color_t, "bg_grad_color"],
    [lv_grad_dir_t, "bg_grad_dir"],
    [lv_style_int_t, "bg_main_color_stop"],
    [lv_style_int_t, "bg_grad_color_stop"],
    [lv_opa_t, "bg_opa"],
    [lv_blend_mode_t, "bg_blend_mode"],

    #/*Border*/
    [lv_color_t, "border_color"],
    [lv_style_int_t, "border_width"],
    [lv_style_int_t, "border_side"],
    [lv_opa_t, "border_opa"],
    [lv_blend_mode_t, "border_blend_mode"],
    [uint8_t_1, "border_post"],

    #/*Outline*/
    [lv_color_t, "outline_color"],
    [lv_style_int_t, "outline_width"],
    [lv_style_int_t, "outline_pad"],
    [lv_opa_t, "outline_opa"],
    [lv_blend_mode_t, "outline_blend_mode"],

    #/*Shadow*/
    [lv_color_t, "shadow_color"],
    [lv_style_int_t, "shadow_width"],
    [lv_style_int_t, "shadow_ofs_x"],
    [lv_style_int_t, "shadow_ofs_y"],
    [lv_style_int_t, "shadow_spread"],
    [lv_opa_t, "shadow_opa"],
    [lv_blend_mode_t, "shadow_blend_mode"],

    #/*Pattern*/
    [ptr, "pattern_image"],
    [ptr, "pattern_font"],
    [lv_color_t, "pattern_recolor"],
    [lv_opa_t, "pattern_opa"],
    [lv_opa_t, "pattern_recolor_opa"],
    [uint8_t_1, "pattern_repeat"],
    [lv_blend_mode_t, "pattern_blend_mode"],

    #/*Value*/
    [ptr, "value_str"],
    [ptr, "value_font"],
    [lv_opa_t, "value_opa"],
    [lv_color_t, "value_color"],
    [lv_style_int_t, "value_ofs_x"],
    [lv_style_int_t, "value_ofs_y"],
    [lv_style_int_t, "value_letter_space"],
    [lv_style_int_t, "value_line_space"],
    [lv_align_t, "value_align"],
    [lv_blend_mode_t, "value_blend_mode"],
]
lv_draw_rect_dsc_t = ctypes.structure(lv_draw_rect_dsc_t, "lv_draw_rect_dsc_t")

lv_draw_line_dsc_t = [
    [lv_color_t, "color"],
    [lv_style_int_t, "width"],
    [lv_style_int_t, "dash_width"],
    [lv_style_int_t, "dash_gap"],
    [lv_opa_t, "opa"],
    [uint8_t_2, "blend_mode"],
    [uint8_t_1, "round_start"],
    [uint8_t_1, "round_end"],
    [uint8_t_1, "raw_end"],
]
lv_draw_line_dsc_t = ctypes.structure(lv_draw_line_dsc_t, "lv_draw_line_dsc_t")

lv_draw_img_dsc_t = [
    [lv_opa_t, "opa"],
    [uint16_t, "angle"],
    [lv_point_t, "pivot"],
    [uint16_t, "zoom"],
    [lv_opa_t, "recolor_opa"],
    [lv_color_t, "recolor"],
    [lv_blend_mode_t, "blend_mode"],
    [uint8_t_1, "antialias"],
]
lv_draw_img_dsc_t = ctypes.structure(lv_draw_img_dsc_t, "lv_draw_img_dsc_t")

lv_draw_label_dsc_t = [
    [lv_color_t, "color"],
    [lv_color_t, "sel_color"],
    [lv_color_t, "sel_bg_color"],
    [lv_font_t, "font"],
    [lv_opa_t, "opa"],
    [lv_style_int_t, "line_space"],
    [lv_style_int_t, "letter_space"],
    [uint32_t, "sel_start"],
    [uint32_t, "sel_end"],
    [lv_coord_t, "ofs_x"],
    [lv_coord_t, "ofs_y"],
    [lv_bidi_dir_t, "bidi_dir"],
    [lv_txt_flag_t, "flag"],
    [lv_text_decor_t, "decor"],
    [lv_blend_mode_t, "blend_mode"],
]
lv_draw_label_dsc_t = ctypes.structure(lv_draw_label_dsc_t, "lv_draw_label_dsc_t")

#- --------- lv_mask --------- -#
lv_draw_mask_xcb_t = ptr    # callback
lv_draw_mask_type_t = ctypes.u8
lv_draw_mask_line_side_t = ctypes.u8

lv_draw_mask_common_dsc_t = [
    [lv_draw_mask_xcb_t, "cb"],
    [lv_draw_mask_type_t, "type"],
]
lv_draw_mask_common_dsc_t = ctypes.structure(lv_draw_mask_common_dsc_t, "lv_draw_mask_common_dsc_t")

lv_draw_mask_line_param_cfg_t = [
    #/*First point */
    [lv_point_t, "p1"],

    #/*Second point*/
    [lv_point_t, "p2"],

    #/*Which side to keep?*/
    [uint8_t_2, "side"],
]
lv_draw_mask_line_param_cfg_t = ctypes.structure(lv_draw_mask_line_param_cfg_t, "lv_draw_mask_line_param_cfg_t")

lv_draw_mask_line_param_t = [
    #/*The first element must be the common descriptor*/
    [lv_draw_mask_common_dsc_t, "dsc"],
    [lv_draw_mask_line_param_cfg_t, "cfg"],
    #/*A point of the line*/
    [lv_point_t, "origo"],
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
lv_draw_mask_line_param_t = ctypes.structure(lv_draw_mask_line_param_t, "lv_draw_mask_line_param_t")

lv_draw_mask_angle_param_cfg_t = [
    [lv_point_t, "vertex_p"],
    [lv_coord_t, "start_angle"],
    [lv_coord_t, "end_angle"],
]
lv_draw_mask_angle_param_cfg_t = ctypes.structure(lv_draw_mask_angle_param_cfg_t, "lv_draw_mask_angle_param_cfg_t")

lv_draw_mask_angle_param_t = [
    #/*The first element must be the common descriptor*/
    [lv_draw_mask_common_dsc_t, "dsc"],
    [lv_draw_mask_angle_param_cfg_t, "cfg"],

    [lv_draw_mask_line_param_t, "start_line"],
    [lv_draw_mask_line_param_t, "end_line"],
    [uint16_t, "delta_deg"],
]
lv_draw_mask_angle_param_t = ctypes.structure(lv_draw_mask_angle_param_t, "lv_draw_mask_angle_param_t")


lv_draw_mask_radius_param_cfg_t = [
    [lv_area_t, "rect"],
    [lv_coord_t, "radius"],
    [uint8_t_1, "outer"],
]
lv_draw_mask_radius_param_cfg_t = ctypes.structure(lv_draw_mask_radius_param_cfg_t, "lv_draw_mask_radius_param_cfg_t")

lv_sqrt_res_t = [
    [uint16_t, "i"],
    [uint16_t, "f"],
]
lv_sqrt_res_t = ctypes.structure(lv_sqrt_res_t, "lv_sqrt_res_t")

lv_draw_mask_radius_param_t = [
    #/*The first element must be the common descriptor*/
    [lv_draw_mask_common_dsc_t, "dsc"],
    [lv_draw_mask_radius_param_cfg_t, "cfg"],
    [int32_t, "y_prev"],
    [lv_sqrt_res_t, "y_prev_x"],
]
lv_draw_mask_radius_param_t = ctypes.structure(lv_draw_mask_radius_param_t, "lv_draw_mask_radius_param_t")


lv_draw_mask_fade_param_cfg_t = [
    [lv_area_t, "coords"],
    [lv_coord_t, "y_top"],
    [lv_coord_t, "y_bottom"],
    [lv_opa_t, "opa_top"],
    [lv_opa_t, "opa_bottom"],
]
lv_draw_mask_fade_param_cfg_t = ctypes.structure(lv_draw_mask_fade_param_cfg_t, "lv_draw_mask_fade_param_cfg_t")

lv_draw_mask_fade_param_t = [
    # /*The first element must be the common descriptor*/
    [lv_draw_mask_common_dsc_t, "dsc"],
    [lv_draw_mask_fade_param_cfg_t, "cfg"],
]
lv_draw_mask_fade_param_t = ctypes.structure(lv_draw_mask_fade_param_t, "lv_draw_mask_fade_param_t")


lv_draw_mask_map_param_cfg_t = [
    [lv_area_t, "coords"],
    [ptr, "map"],
]
lv_draw_mask_map_param_cfg_t = ctypes.structure(lv_draw_mask_map_param_cfg_t, "lv_draw_mask_map_param_cfg_t")

lv_draw_mask_map_param_t = [
    #/*The first element must be the common descriptor*/
    [lv_draw_mask_common_dsc_t, "dsc"],
    [lv_draw_mask_map_param_cfg_t, "cfg"],
]
lv_draw_mask_map_param_t = ctypes.structure(lv_draw_mask_map_param_t, "lv_draw_mask_map_param_t")

lv_draw_mask_saved_t = [
    [ptr, "param"],
    [ptr, "custom_id"],
]
lv_draw_mask_saved_t = ctypes.structure(lv_draw_mask_saved_t, "lv_draw_mask_saved_t")

#
ctypes.print_classes()

# Ex:
# bb = ctypes.buffer(test_t, bytes("0101020203030404FFFFFEFEFCFC8080"))

# Ex:
# bb = ctypes.buffer(lv_draw_rect)

#- Ex

bb=ctypes.buffer(lv_draw_line_dsc_t)
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

lv_draw_line_dsc_t = [
    [lv_color_t, "color"],
    [lv_style_int_t, "width"],
    [lv_style_int_t, "dash_width"],
    [lv_style_int_t, "dash_gap"],
    [lv_opa_t, "opa"],
    [uint8_t_2, "blend_mode"],
    [uint8_t_1, "round_start"],
    [uint8_t_1, "round_end"],
    [uint8_t_1, "raw_end"],
]

-#