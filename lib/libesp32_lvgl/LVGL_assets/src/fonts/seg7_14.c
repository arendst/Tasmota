/*******************************************************************************
 * Size: 14 px
 * Bpp: 1
 * Opts: --font DSEG7Classic-Bold.ttf -r 0x20-0x3F --size 14 --format lvgl --bpp 1 -o c_files//seg7_14.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef SEG7_14
#define SEG7_14 1
#endif

#if SEG7_14

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0x0,

    /* U+002D "-" */
    0x7e, 0xfc,

    /* U+002E "." */
    0xf0,

    /* U+0030 "0" */
    0x7f, 0x5f, 0x70, 0x78, 0x3c, 0x1e, 0xf, 0x6,
    0x0, 0xc1, 0xe0, 0xf0, 0x78, 0x3c, 0x1d, 0xf5,
    0xfc,

    /* U+0031 "1" */
    0x7f, 0xf7, 0xff, 0xc0,

    /* U+0032 "2" */
    0x7f, 0x1f, 0x40, 0x60, 0x30, 0x18, 0xc, 0x6,
    0x7d, 0xbe, 0x60, 0x30, 0x18, 0xc, 0x5, 0xf1,
    0xfc,

    /* U+0033 "3" */
    0xfe, 0x7d, 0x3, 0x3, 0x3, 0x3, 0x3, 0x7d,
    0x7d, 0x3, 0x3, 0x3, 0x3, 0x7d, 0xfe,

    /* U+0034 "4" */
    0x80, 0xe0, 0xf0, 0x78, 0x3c, 0x1e, 0xe, 0xfa,
    0x7d, 0x1, 0x80, 0xc0, 0x60, 0x30, 0x18, 0x4,

    /* U+0035 "5" */
    0x7f, 0x5f, 0x30, 0x18, 0xc, 0x6, 0x3, 0x1,
    0x7c, 0x3e, 0x80, 0xc0, 0x60, 0x30, 0x19, 0xf5,
    0xfc,

    /* U+0036 "6" */
    0x7f, 0x5f, 0x30, 0x18, 0xc, 0x6, 0x3, 0x1,
    0x7c, 0xbe, 0xe0, 0xf0, 0x78, 0x3c, 0x1d, 0xf5,
    0xfc,

    /* U+0037 "7" */
    0x7f, 0x5f, 0x70, 0x78, 0x3c, 0x1e, 0xf, 0x6,
    0x1, 0x1, 0x80, 0xc0, 0x60, 0x30, 0x18, 0x4,

    /* U+0038 "8" */
    0x7f, 0x5f, 0x70, 0x78, 0x3c, 0x1e, 0xf, 0x7,
    0x7d, 0xbe, 0xe0, 0xf0, 0x78, 0x3c, 0x1d, 0xf5,
    0xfc,

    /* U+0039 "9" */
    0x7f, 0x5f, 0x70, 0x78, 0x3c, 0x1e, 0xf, 0x7,
    0x7d, 0x3e, 0x80, 0xc0, 0x60, 0x30, 0x19, 0xf5,
    0xfc,

    /* U+003A ":" */
    0xc3
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 45, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 183, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 2, .adv_w = 183, .box_w = 7, .box_h = 2, .ofs_x = 2, .ofs_y = 6},
    {.bitmap_index = 4, .adv_w = 0, .box_w = 2, .box_h = 2, .ofs_x = -1, .ofs_y = 0},
    {.bitmap_index = 5, .adv_w = 183, .box_w = 9, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 22, .adv_w = 183, .box_w = 2, .box_h = 13, .ofs_x = 8, .ofs_y = 1},
    {.bitmap_index = 26, .adv_w = 183, .box_w = 9, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 43, .adv_w = 183, .box_w = 8, .box_h = 15, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 58, .adv_w = 183, .box_w = 9, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 74, .adv_w = 183, .box_w = 9, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 91, .adv_w = 183, .box_w = 9, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 108, .adv_w = 183, .box_w = 9, .box_h = 14, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 124, .adv_w = 183, .box_w = 9, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 141, .adv_w = 183, .box_w = 9, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 158, .adv_w = 45, .box_w = 1, .box_h = 8, .ofs_x = 1, .ofs_y = 3}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_0[] = {
    0x0, 0x1, 0xd, 0xe
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 15, .glyph_id_start = 1,
        .unicode_list = unicode_list_0, .glyph_id_ofs_list = NULL, .list_length = 4, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    },
    {
        .range_start = 48, .range_length = 11, .glyph_id_start = 5,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR >= 8
/*Store all the custom data of the font*/
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 2,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
};


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t seg7_14 = {
#else
lv_font_t seg7_14 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 15,          /*The maximum line height required by the font*/
    .base_line = 0,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0)
    .underline_position = -2,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if SEG7_14*/

