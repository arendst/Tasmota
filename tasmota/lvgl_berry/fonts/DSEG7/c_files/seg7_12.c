/*******************************************************************************
 * Size: 12 px
 * Bpp: 1
 * Opts: --font DSEG7Classic-Bold.ttf -r 0x20-0x3F --size 12 --format lvgl --bpp 1 -o c_files//seg7_12.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef SEG7_12
#define SEG7_12 1
#endif

#if SEG7_12

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
    0xfc,

    /* U+002E "." */
    0xc0,

    /* U+0030 "0" */
    0x7d, 0x6, 0xc, 0x18, 0x30, 0x60, 0xc1, 0x83,
    0x6, 0xb, 0xe0,

    /* U+0031 "1" */
    0xfb, 0xe0,

    /* U+0032 "2" */
    0x7c, 0x4, 0x8, 0x10, 0x2f, 0xa0, 0x40, 0x81,
    0x2, 0x3, 0xe0,

    /* U+0033 "3" */
    0xf8, 0x10, 0x41, 0x7, 0xe0, 0x41, 0x4, 0x10,
    0x7e,

    /* U+0034 "4" */
    0x83, 0x6, 0xc, 0x18, 0x2f, 0x80, 0x81, 0x2,
    0x4, 0x8,

    /* U+0035 "5" */
    0x7d, 0x2, 0x4, 0x8, 0xf, 0x80, 0x81, 0x2,
    0x4, 0xb, 0xe0,

    /* U+0036 "6" */
    0x7d, 0x2, 0x4, 0x8, 0xf, 0xa0, 0xc1, 0x83,
    0x6, 0xb, 0xe0,

    /* U+0037 "7" */
    0x7d, 0x6, 0xc, 0x18, 0x30, 0x40, 0x81, 0x2,
    0x4, 0x8, 0x0,

    /* U+0038 "8" */
    0x7d, 0x6, 0xc, 0x18, 0x2f, 0xa0, 0xc1, 0x83,
    0x6, 0xb, 0xe0,

    /* U+0039 "9" */
    0x7d, 0x6, 0xc, 0x18, 0x2f, 0x80, 0x81, 0x2,
    0x4, 0xb, 0xe0,

    /* U+003A ":" */
    0xc0, 0x30
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 38, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 157, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 2, .adv_w = 157, .box_w = 6, .box_h = 1, .ofs_x = 2, .ofs_y = 6},
    {.bitmap_index = 3, .adv_w = 0, .box_w = 2, .box_h = 1, .ofs_x = -1, .ofs_y = 0},
    {.bitmap_index = 4, .adv_w = 157, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 15, .adv_w = 157, .box_w = 1, .box_h = 12, .ofs_x = 7, .ofs_y = 0},
    {.bitmap_index = 17, .adv_w = 157, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 28, .adv_w = 157, .box_w = 6, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 37, .adv_w = 157, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 47, .adv_w = 157, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 58, .adv_w = 157, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 69, .adv_w = 157, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 80, .adv_w = 157, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 91, .adv_w = 157, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 102, .adv_w = 38, .box_w = 2, .box_h = 6, .ofs_x = 0, .ofs_y = 3}
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

#if LV_VERSION_CHECK(8, 0, 0)
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
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
#if LV_VERSION_CHECK(8, 0, 0)
    .cache = &cache
#endif
};


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LV_VERSION_CHECK(8, 0, 0)
const lv_font_t seg7_12 = {
#else
lv_font_t seg7_12 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 12,          /*The maximum line height required by the font*/
    .base_line = 0,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0)
    .underline_position = -1,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if SEG7_12*/

