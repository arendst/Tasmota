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
lv_color32 = ct.u32
lv_grad_dir = ct.u8
lv_meter_indicator_type_t = ct.u8
lv_opa = ct.u8
lv_blend_mode = ct.u8
lv_align = ct.u8
lv_bidi_dir = ct.u8
lv_base_dir = ct.u8
lv_text_align = ct.u8
lv_text_flag = ct.u8
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

lv_point = [            # valid LVGL93
    [int32_t, "x"],
    [int32_t, "y"],
]
lv_point = ct.structure(lv_point, "lv_point")

# typedef struct {
#     lv_value_precise_t x;
#     lv_value_precise_t y;
# } lv_point_precise_t;
lv_point_precise = [            # valid LVGL93
    [lv_value_precise, "x"],
    [lv_value_precise, "y"],
]
lv_point_precise = ct.structure(lv_point_precise, "lv_point_precise")

lv_area = [            # valid LVGL93
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
# } lv_grad_stop_t;
lv_gradient_stop = [            # valid LVGL93
    [lv_color, "color"],
    [lv_opa, "opa"],
    [uint8_t, "frac"],
]
lv_grad_stop = ct.structure(lv_gradient_stop, "lv_grad_stop")
lv_gradient_stop = ct.structure(lv_gradient_stop, "lv_gradient_stop")

# typedef struct {
#     lv_grad_stop_t   stops[LV_GRADIENT_MAX_STOPS];  /**< A gradient stop array */
#     uint8_t          stops_count;                   /**< The number of used stops in the array */
#     lv_grad_dir_t    dir : 4;                       /**< The gradient direction.
#                                                          * Any of LV_GRAD_DIR_NONE, LV_GRAD_DIR_VER, LV_GRAD_DIR_HOR,
#                                                          * LV_GRAD_TYPE_LINEAR, LV_GRAD_TYPE_RADIAL, LV_GRAD_TYPE_CONICAL */
#     lv_grad_extend_t     extend : 3;                    /**< Behaviour outside the defined range.
#                                                          * LV_GRAD_EXTEND_NONE, LV_GRAD_EXTEND_PAD, LV_GRAD_EXTEND_REPEAT, LV_GRAD_EXTEND_REFLECT */
# #if LV_USE_DRAW_SW_COMPLEX_GRADIENTS
#     union {
#         /*Linear gradient parameters*/
#         struct {
#             lv_point_t  start;                          /**< Linear gradient vector start point */
#             lv_point_t  end;                            /**< Linear gradient vector end point */
#         } linear;
#         /*Radial gradient parameters*/
#         struct {
#             lv_point_t  focal;                          /**< Center of the focal (starting) circle in local coordinates */
#             /* (can be the same as the ending circle to create concentric circles) */
#             lv_point_t  focal_extent;                   /**< Point on the circle (can be the same as the center) */
#             lv_point_t  end;                            /**< Center of the ending circle in local coordinates */
#             lv_point_t  end_extent;                     /**< Point on the circle determining the radius of the gradient */
#         } radial;
#         /*Conical gradient parameters*/
#         struct {
#             lv_point_t  center;                         /**< Conical gradient center point */
#             int16_t     start_angle;                    /**< Start angle 0..3600 */
#             int16_t     end_angle;                      /**< End angle 0..3600 */
#         } conical;
#     } params;
#     void * state;
# #endif
# } lv_grad_dsc_t;
lv_grad_dsc = [            # valid LVGL93
    # since it's an array and not two structures, we need to explicitly unroll it here or the alignment is wrong
    # [lv_gradient_stop, "stops_0"],
    [lv_color, "stops_0_color"],
    [lv_opa, "stops_0_opa"],
    [uint8_t, "stops_0_frac"],
    # [lv_gradient_stop, "stops_1"],
    [lv_color, "stops_1_color"],
    [lv_opa, "stops_1_opa"],
    [uint8_t, "stops_1_frac"],
    
    [uint8_t, "stops_count"],
    [uint8_t_4, "dir"],
    [uint8_t_3, "extend"],
]
lv_grad_dsc = ct.structure(lv_grad_dsc, "lv_grad_dsc")


# typedef struct {
#     /**The widget for which draw descriptor was created */
#     lv_obj_t * obj;

#     /**The widget part for which draw descriptor was created */
#     lv_part_t part;

#     /**A widget type specific ID (e.g. table row index). See the docs of the given widget.*/
#     uint32_t id1;

#     /**A widget type specific ID (e.g. table column index). See the docs of the given widget.*/
#     uint32_t id2;

#     /**The target layer */
#     lv_layer_t * layer;

#     /**Size of the specific draw descriptor into which this base descriptor is embedded*/
#     size_t dsc_size;

#     /**Any custom user data*/
#     void * user_data;
# } lv_draw_dsc_base_t;
lv_draw_dsc_base = [            # valid LVGL93
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
lv_draw_rect_dsc = [            # valid LVGL93
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
    [uint8_t_5, "border_side"],
    [uint8_t_1, "border_post"],

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

#     /**The first point of the line. If `LV_USE_FLOAT` is enabled float number can be also used*/
#     lv_point_precise_t p1;

#     /**The second point of the line. If `LV_USE_FLOAT` is enabled float number can be also used*/
#     lv_point_precise_t p2;

#     /**The color of the line*/
#     lv_color_t color;

#     /**The width (thickness) of the line*/
#     int32_t width;

#     /** The length of a dash (0: don't dash)*/
#     int32_t dash_width;

#     /** The length of the gaps between dashes (0: don't dash)*/
#     int32_t dash_gap;

#     /**Opacity of the line in 0...255 range.
#      * LV_OPA_TRANSP, LV_OPA_10, LV_OPA_20, .. LV_OPA_COVER can be used as well*/
#     lv_opa_t opa;

#     /**Make the line start rounded*/
#     uint8_t round_start : 1;

#     /**Make the line end rounded*/
#     uint8_t round_end   : 1;

#     /**1: Do not bother with line ending (if it's not visible for any reason) */
#     uint8_t raw_end     : 1;
# } lv_draw_line_dsc_t;
lv_draw_line_dsc = [            # valid LVGL93
    [lv_draw_dsc_base, "base"],

    [lv_point_precise, "p1"],
    [lv_point_precise, "p2"],
    [lv_color, "color"],
    [int32_t, "width"],
    [int32_t, "dash_width"],
    [int32_t, "dash_gap"],
    [lv_opa, "opa"],
    [uint8_t_1, "round_start"],
    [uint8_t_1, "round_end"],
    [uint8_t_1, "raw_end"],
]
lv_draw_line_dsc = ct.structure(lv_draw_line_dsc, "lv_draw_line_dsc")

# typedef struct {
#     lv_draw_dsc_base_t base;

#     /**The color of the arc*/
#     lv_color_t color;

#     /**The width (thickness) of the arc */
#     int32_t width;

#     /**The start angle in 1 degree units (if `LV_USE_FLOAT` is enabled a float number can be also used)
#      * 0° is the 3 o'clock position, 90° is the 6 o'clock, etc. */
#     lv_value_precise_t start_angle;

#     /**The end angle, similarly to start_angle. */
#     lv_value_precise_t end_angle;

#     /**The center point of the arc. */
#     lv_point_t center;

#     /**The outer radius of the arc*/
#     uint16_t radius;

#     /**An image source to be used instead of `color`. `NULL` if unused*/
#     const void * img_src;

#     /**Opacity of the arc in 0...255 range.
#      * LV_OPA_TRANSP, LV_OPA_10, LV_OPA_20, .. LV_OPA_COVER can be used as well*/
#     lv_opa_t opa;

#     /**1: Make the arc ends rounded*/
#     uint8_t rounded : 1;
# } lv_draw_arc_dsc_t;
lv_draw_arc_dsc = [            # valid LVGL93
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
#     uint32_t magic: 8;          /**< Magic number. Must be LV_IMAGE_HEADER_MAGIC*/
#     uint32_t cf : 8;            /**< Color format: See `lv_color_format_t`*/
#     uint32_t flags: 16;         /**< Image flags, see `lv_image_flags_t`*/

#     uint32_t w: 16;
#     uint32_t h: 16;
#     uint32_t stride: 16;        /**< Number of bytes in a row*/
#     uint32_t reserved_2: 16;    /**< Reserved to be used later*/
# } lv_image_header_t;
lv_image_header = [            # valid LVGL93
    [uint8_t, "magic"],
    [uint8_t, "cf"],
    [uint16_t, "flags"],

    [uint16_t, "w"],
    [uint16_t, "h"],
    [uint16_t, "stride"],
    [uint16_t, "reserved_2"],
]
lv_image_header = ct.structure(lv_image_header, "lv_image_header")

# struct _lv_draw_image_dsc_t {
#     lv_draw_dsc_base_t base;

#     /**The image source: pointer to `lv_image_dsc_t` or a path to a file*/
#     const void * src;

#     /**The header of the image. Initialized internally in `lv_draw_image` */
#     lv_image_header_t header;

#     /**Clip the corner of the image with this radius. Use `LV_RADIUS_CIRCLE` for max. radius */
#     int32_t clip_radius;

#     /**The rotation of the image in 0.1 degree unit. E.g. 234 means 23.4° */
#     int32_t rotation;

#     /**Horizontal scale (zoom) of the image.
#      * 256 (LV_SCALE_NONE): means no zoom, 512 double size, 128 half size.*/
#     int32_t scale_x;

#     /**Same as `scale_y` but vertically*/
#     int32_t scale_y;

#     /**Parallelogram like transformation of the image horizontally in 0.1 degree unit. E.g. 456 means 45.6°.*/
#     int32_t skew_x;

#     /**Same as `skew_x` but vertically*/
#     int32_t skew_y;

#     /**The pivot point of transformation (scale and rotation).
#      * 0;0 is the top left corner of the image. Can be outside of the image too.*/
#     lv_point_t pivot;

#     /**Mix this color to the images. In case of `LV_COLOR_FORMAT_A8` it will be the color of the visible pixels*/
#     lv_color_t recolor;

#     /**The intensity of recoloring. 0 means, no recolor, 255 means full cover (transparent pixels remain transparent)*/
#     lv_opa_t recolor_opa;

#     /**Opacity in 0...255 range.
#      * LV_OPA_TRANSP, LV_OPA_10, LV_OPA_20, .. LV_OPA_COVER can be used as well*/
#     lv_opa_t opa;

#     /**Describes how to blend the pixels of the image to the background.
#      * See `lv_blend_mode_t` for more details.
#      */
#     lv_blend_mode_t blend_mode : 4;

#     /**1: perform the transformation with anti-alaising */
#     uint16_t antialias          : 1;

#     /**If the image is smaller than the `image_area` field of `lv_draw_image_dsc_t`
#      * tile the image (repeat is both horizontally and vertically) to fill the
#      * `image_area` area*/
#     uint16_t tile               : 1;

#     /**Used internally to store some information about the palette or the color of A8 images*/
#     lv_draw_image_sup_t * sup;

#     /** Used to indicate the entire original, non-clipped area where the image is to be drawn.
#      * This is important for:
#      *  1. Layer rendering, where it might happen that only a smaller area of the layer is rendered and e.g.
#      *     `clip_radius` needs to know what the original image was.
#      *  2. Tiled images, where the target draw area is larger than the image to be tiled.
#      */
#     lv_area_t image_area;

#     /**Pointer to an A8 or L8 image descriptor to mask the image with.
#      * The mask is always center aligned. */
#     const lv_image_dsc_t * bitmap_mask_src;
# };
lv_draw_image_dsc = [            # valid LVGL93
    [lv_draw_dsc_base, "base"],

    [ptr, "src"],
    [lv_image_header, "header"],

    [int32_t, "clip_radius"],
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
    [lv_area, "image_area"],

    [ptr, "bitmap_mask_src"],
]
lv_draw_image_dsc = ct.structure(lv_draw_image_dsc, "lv_draw_image_dsc")


# typedef struct {
#     lv_draw_dsc_base_t base;

#     /**The text to draw*/
#     const char * text;

#     /**The size of the text*/
#     lv_point_t text_size;

#     /**The font to use. Fallback fonts are also handled.*/
#     const lv_font_t * font;

#     /**Color of the text*/
#     lv_color_t color;

#     /**Extra space between the lines*/
#     int32_t line_space;

#     /**Extra space between the characters*/
#     int32_t letter_space;

#     /**Offset the text with this value horizontally*/
#     int32_t ofs_x;

#     /**Offset the text with this value vertically*/
#     int32_t ofs_y;

#     /**Rotation of the letters in 0.1 degree unit*/
#     int32_t rotation;

#     /**The first characters index for selection (not byte index). `LV_DRAW_LABEL_NO_TXT_SEL` for no selection*/
#     uint32_t sel_start;

#     /**The last characters's index for selection (not byte index). `LV_DRAW_LABEL_NO_TXT_SEL` for no selection*/
#     uint32_t sel_end;

#     /**Color of the selected characters*/
#     lv_color_t sel_color;

#     /**Background color of the selected characters*/
#     lv_color_t sel_bg_color;

#     /**The number of characters to render. 0: means render until reaching the `\0` termination.*/
#     uint32_t text_length;

#     /**Opacity of the text in 0...255 range.
#      * LV_OPA_TRANSP, LV_OPA_10, LV_OPA_20, .. LV_OPA_COVER can be used as well*/
#     lv_opa_t opa;

#     /**The alignment of the text `LV_TEXT_ALIGN_LEFT/RIGHT/CENTER`*/
#     lv_text_align_t align;

#     /**The base direction. Used when type setting Right-to-left (e.g. Arabic) texts*/
#     lv_base_dir_t bidi_dir;

#     /**Text decoration, e.g. underline*/
#     lv_text_decor_t decor : 3;

#     /**Some flags to control type setting*/
#     lv_text_flag_t flag : 5;

#     /**1: malloc a buffer and copy `text` there.
#      * 0: `text` will be valid during rendering.*/
#     uint8_t text_local : 1;

#     /**Indicate that the text is constant and its pointer can be safely saved e.g. in a cache.*/
#     uint8_t text_static : 1;

#     /**1: already executed lv_bidi_process_paragraph.
#      * 0: has not been executed lv_bidi_process_paragraph.*/
#     uint8_t has_bided : 1;

#     /**Pointer to an externally stored struct where some data can be cached to speed up rendering*/
#     lv_draw_label_hint_t * hint;

#     /* Properties of the letter outlines */
#     lv_opa_t outline_stroke_opa;
#     lv_color_t outline_stroke_color;
#     int32_t outline_stroke_width;

# } lv_draw_label_dsc_t;
lv_draw_label_dsc = [            # valid LVGL93
    [lv_draw_dsc_base, "base"],
    [ptr, "text"],
    [lv_point, "text_size"],
    [ptr, "font"],
    [lv_color, "color"],
    [int32_t, "line_space"],
    [int32_t, "letter_space"],
    [int32_t, "ofs_x"],
    [int32_t, "ofs_y"],
    [int32_t, "rotation"],
    [uint32_t, "sel_start"],
    [uint32_t, "sel_end"],
    [lv_color, "sel_color"],
    [lv_color, "sel_bg_color"],
    [uint32_t, "text_length"],
    [lv_opa, "opa"],
    [lv_text_align, "align"],
    [lv_base_dir, "bidi_dir"],
    [lv_text_flag, "flag"],
    [uint8_t_3, "decor"],
    [uint8_t_5, "flag"],
    [uint8_t_3, "blend_mode"],
    [uint8_t_1, "text_local"],
    [uint8_t_1, "text_static"],
    [uint8_t_1, "has_bided"],
    [ptr, "hint"],
    [lv_opa, "outline_stroke_opa"],
    [lv_color, "outline_stroke_color"],
    [int32_t, "outline_stroke_width"],
]
lv_draw_label_dsc = ct.structure(lv_draw_label_dsc, "lv_draw_label_dsc")

# struct _lv_chart_series_t {
#     int32_t * x_points;
#     int32_t * y_points;
#     lv_color_t color;
#     uint32_t start_point;
#     uint32_t hidden : 1;
#     uint32_t x_ext_buf_assigned : 1;
#     uint32_t y_ext_buf_assigned : 1;
#     uint32_t x_axis_sec : 1;
#     uint32_t y_axis_sec : 1;
# };
lv_chart_series = [            # valid LVGL93
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

# struct _lv_chart_cursor_t {
#     lv_point_t pos;
#     int32_t point_id;
#     lv_color_t color;
#     lv_chart_series_t * ser;
#     lv_dir_t dir;
#     uint32_t pos_set: 1;        /**< 1: pos is set; 0: point_id is set */
# };
lv_chart_cursor = [            # valid LVGL93
    [lv_point, "pos"],
    [int32_t, "point_id"],
    [lv_color, "color"],
    [ptr, "ser"],
    [lv_dir, "dir"],
    [uint8_t_1, "pos_set"],
]
lv_chart_cursor = ct.structure(lv_chart_cursor, "lv_chart_cursor")

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
#     /** class_p is the final class while obj->class_p is the class currently being [de]constructed. */
#     void (*constructor_cb)(const lv_obj_class_t * class_p, lv_obj_t * obj);
#     void (*destructor_cb)(const lv_obj_class_t * class_p, lv_obj_t * obj);

#     /** class_p is the class in which event is being processed. */
#     void (*event_cb)(const lv_obj_class_t * class_p, lv_event_t * e);  /**< Widget type specific event function*/

# #if LV_USE_OBJ_PROPERTY
#     uint32_t prop_index_start;
#     uint32_t prop_index_end;
#     const lv_property_ops_t * properties;
#     uint32_t properties_count;

# #if LV_USE_OBJ_PROPERTY_NAME
#     /* An array of property ID and name */
#     const lv_property_name_t * property_names;
#     uint32_t names_count;
# #endif
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
lv_obj_class = [            # valid LVGL93
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
lv_event = [            # valid LVGL93
    [lv_obj_ptr, "current_target"],
    [lv_obj_ptr, "original_target"],
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
#     lv_image_header_t header;   /**< A header describing the basics of the image*/
#     uint32_t data_size;         /**< Size of the image in bytes*/
#     const uint8_t * data;       /**< Pointer to the data of the image*/
#     const void * reserved;      /**< A reserved field to make it has same size as lv_draw_buf_t*/
#     const void * reserved_2;    /**< A reserved field to make it has same size as lv_draw_buf_t*/
# } lv_image_dsc_t;
lv_image_dsc = [                        # valid LVGL93
    [lv_image_header, "header"],
    [uint32_t, "data_size"],
    [ptr, "data"],
    [ptr, "reserved"],
    [ptr, "reserved_2"],
]
lv_image_dsc = ct.structure(lv_image_dsc, "lv_image_dsc")

#######################################################################
# lv_style

# typedef struct {
#     const lv_style_prop_t * props; /**< An array with the properties to animate.*/
#     void * user_data;              /**< A custom user data that will be passed to the animation's user_data */
#     lv_anim_path_cb_t path_xcb;    /**< A path for the animation.*/
#     uint32_t time;                 /**< Duration of the transition in [ms]*/
#     uint32_t delay;                /**< Delay before the transition in [ms]*/
# } lv_style_transition_dsc_t;
lv_style_transition_dsc = [            # valid LVGL93
    [ptr, "props"],
    [ptr, "user_data"],
    [ptr, "path_xcb"],
    [uint32_t, "time"],
    [uint32_t, "delay"],
]
lv_style_transition_dsc = ct.structure(lv_style_transition_dsc, "lv_style_transition_dsc")


# struct _lv_layer_t  {

#     /** Target draw buffer of the layer*/
#     lv_draw_buf_t * draw_buf;

#     /** The absolute coordinates of the buffer */
#     lv_area_t buf_area;

#     /** The color format of the layer. LV_COLOR_FORMAT_...  */
#     lv_color_format_t color_format;

#     /**
#      * NEVER USE IT DRAW UNITS. USED INTERNALLY DURING DRAW TASK CREATION.
#      * The current clip area with absolute coordinates, always the same or smaller than `buf_area`
#      * Can be set before new draw tasks are added to indicate the clip area of the draw tasks.
#      * Therefore `lv_draw_add_task()` always saves it in the new draw task to know the clip area when the draw task was added.
#      * During drawing the draw units also sees the saved clip_area and should use it during drawing.
#      * During drawing the layer's clip area shouldn't be used as it might be already changed for other draw tasks.
#      */
#     lv_area_t _clip_area;

#     /**
#      * The physical clipping area relative to the display.
#      */
#     lv_area_t phy_clip_area;

# #if LV_DRAW_TRANSFORM_USE_MATRIX
#     /** Transform matrix to be applied when rendering the layer */
#     lv_matrix_t matrix;
# #endif

#     /** Opacity of the layer */
#     lv_opa_t opa;

#     /*Recolor of the layer*/
#     lv_color32_t recolor;

#     /** Partial y offset */
#     int32_t partial_y_offset;

#     /** Linked list of draw tasks */
#     lv_draw_task_t * draw_task_head;

#     lv_layer_t * parent;
#     lv_layer_t * next;
#     bool all_tasks_added;
#     void * user_data;
# };
lv_layer = [                            # valid LVGL93
    [ptr, "draw_buf"],
    [lv_area, "buf_area"],
    [uint32_t, "color_format"],
    [lv_area, "_clip_area"],
    [lv_area, "phy_clip_area"],
    [lv_opa, "opa"],
    [lv_color32, "recolor"],
    [int32_t, "partial_y_offset"],
    [ptr, "draw_task_head"],
    [ptr, "parent"],
    [ptr, "next"],
    [bool, "all_tasks_added"],
    [ptr, "user_data"],
]
lv_layer = ct.structure(lv_layer, "lv_layer")

#######################################################################
# lv_color

# struct _lv_color_filter_dsc_t {
#     lv_color_filter_cb_t filter_cb;
#     void * user_data;
# };
lv_color_filter_dsc = [            # valid LVGL93
    [ptr, "filter_cb"],
    [ptr, "user_data"],
]
lv_color_filter_dsc = ct.structure(lv_color_filter_dsc, "lv_color_filter_dsc")

#######################################################################
# lv_timer native, superseded by lv_timer

# struct _lv_timer_t {
#     uint32_t period;           /**< How often the timer should run */
#     uint32_t last_run;         /**< Last time the timer ran */
#     lv_timer_cb_t timer_cb;    /**< Timer function */
#     void * user_data;          /**< Custom user data */
#     int32_t repeat_count;      /**< 1: One time;  -1 : infinity;  n>0: residual times */
#     uint32_t paused : 1;
#     uint32_t auto_delete : 1;
# };
lv_timer_ntv = [                    # valid LVGL93
    [uint32_t, "period"],
    [uint32_t, "last_run"],
    [ptr, "timer_cb"],
    [ptr, "user_data"],
    [int32_t, "repeat_count"],
    [uint8_t_1, "paused"],
    [uint8_t_1, "auto_delete"],
]
lv_timer_ntv = ct.structure(lv_timer_ntv, "lv_timer_ntv")


#######################################################################
# lv_event_dsc
# struct _lv_event_dsc_t {
#     lv_event_cb_t cb;
#     void * user_data;
#     uint32_t filter;
# };
lv_event_dsc = [            # valid LVGL93
    [ptr, "cb"],
    [ptr, "user_data"],
    [uint32_t, "filter"],
]
lv_event_dsc = ct.structure(lv_event_dsc, "lv_event_dsc")

#######################################################################
# Special structure used to calibrate resistive touchscreens
#######################################################################
lv_ts_calibration = [            # valid LVGL93
    [lv_coord_t, "raw_x"],
    [lv_coord_t, "raw_y"],
    [lv_coord_t, "x"],
    [lv_coord_t, "y"],
    [int32_t, "state"],
]
lv_ts_calibration = ct.structure(lv_ts_calibration, "lv_ts_calibration")

#######################################################################
# New in LVGL 9.3.0
#######################################################################
# /** Coords of a span */
# typedef struct _lv_span_coords_t {
#     lv_area_t heading;
#     lv_area_t middle;
#     lv_area_t trailing;
# } lv_span_coords_t;
lv_span_coords = [
    [lv_area, "heading"],
    [lv_area, "middle"],
    [lv_area, "trailing"],
]
lv_span_coords = ct.structure(lv_span_coords, "lv_span_coords")


#######################################################################
# lv_font_info

# struct _lv_font_info_t {
#     const char * name;               /**< Font name, used to distinguish different font resources*/
#     const lv_font_class_t * class_p; /**< Font backend implementation*/
#     uint32_t size;                   /**< Font size in pixel*/
#     uint32_t render_mode;            /**< Font rendering mode, see `lv_freetype_font_render_mode_t`*/
#     uint32_t style;                  /**< Font style, see `lv_freetype_font_style_t`*/
#     lv_font_kerning_t kerning;       /**< Font kerning, see `lv_font_kerning_t`*/
# };
lv_font_info = [            # valid LVGL92
    [ptr, "name"],
    [ptr, "class_p"],
    [uint32_t, "size"],
    [uint32_t, "render_mode"],
    [uint32_t, "style"],
    [uint32_t, "kerning"],
]
lv_font_info = ct.structure(lv_font_info, "lv_font_info")


#######################################################################
# lv_draw_letter

# typedef struct {
#     lv_draw_dsc_base_t base;

#     uint32_t unicode;
#     const lv_font_t * font;
#     lv_color_t color;

#     int32_t rotation;
#     int32_t scale_x;
#     int32_t scale_y;
#     int32_t skew_x;
#     int32_t skew_y;
#     lv_point_t pivot;

#     lv_opa_t opa;
#     lv_text_decor_t decor : 3;
#     lv_blend_mode_t blend_mode : 3;

#     /* Properties of the letter outlines */
#     lv_opa_t outline_stroke_opa;
#     int32_t outline_stroke_width;
#     lv_color_t outline_stroke_color;

# } lv_draw_letter_dsc_t;
lv_draw_letter_dsc = [            # valid LVGL93
    [lv_draw_dsc_base, "base"],
    [uint32_t, "unicode"],
    [ptr, "font"],
    [lv_color, "color"],
    [int32_t, "rotation"],
    [int32_t, "scale_x"],
    [int32_t, "scale_y"],
    [int32_t, "skew_x"],
    [int32_t, "skew_y"],
    [lv_point, "pivot"],
    [lv_opa, "opa"],
    [uint8_t_3, "decor"],
    [uint8_t_3, "blend_mode"],
    [lv_opa, "outline_stroke_opa"],
    [int32_t, "outline_stroke_width"],
    [lv_color, "outline_stroke_color"],
]
lv_draw_letter_dsc = ct.structure(lv_draw_letter_dsc, "lv_draw_letter_dsc")

#
ct.print_classes("lvgl")
