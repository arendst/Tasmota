/**
 * @file lv_freetype.h
 *
 */
#ifndef _LV_FONT_TTF_H
#define _LV_FONT_TTF_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
// #include "lvgl/lvgl.h"   // TODO
#include "lvgl.h"
#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_CACHE_H
#include FT_SIZES_H

/*********************
 *      DEFINES
 *********************/
#ifndef LV_USE_FT_CACHE_MANAGER
#define LV_USE_FT_CACHE_MANAGER 0
#endif

#define LV_USE_FT_STACK_SIZE 24*1024        // FreeType consumes a large amount of stack

/**********************
 *      TYPEDEFS
 **********************/
typedef struct {
    uint16_t cnt;
    char*   name;
} lv_face_info_t;

typedef struct {
    uint16_t num;
    uint16_t cnt;
    lv_ll_t  face_ll;
} lv_faces_control_t;

typedef enum {
    FT_FONT_STYLE_NORMAL = 0,
    FT_FONT_STYLE_ITALIC = 1 << 0,
    FT_FONT_STYLE_BOLD   = 1 << 1
} LV_FT_FONT_STYLE;

typedef struct {
    const char* name;   /* The name of the font file */
    lv_font_t* font;    /* point to lvgl font */
    uint16_t weight;    /* font size */
    uint16_t style;     /* font style */
} lv_ft_info_t;

typedef struct {
    FT_Face     face;
    FT_Size     size;
    lv_font_t*  font;
    uint16_t    style;
    uint16_t weight;
} lv_font_fmt_freetype_dsc_t;

typedef lv_font_fmt_freetype_dsc_t lv_font_fmt_ft_dsc_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * init freetype library
 * @param max_faces Maximum number of opened FT_Face objects managed by this cache instance. Use 0 for defaults.
 * @param max_sizes Maximum number of opened FT_Size objects managed by this cache instance. Use 0 for defaults.
 * @param max_bytes Maximum number of bytes to use for cached data nodes. Use 0 for defaults.
 *                  Note that this value does not account for managed FT_Face and FT_Size objects.
 * @return true on success, otherwise false.
 */
bool lv_freetype_init(FT_UInt max_faces, FT_UInt max_sizes, FT_ULong max_bytes);

/**
 * Destroy freetype library
 */
void lv_freetype_destroy(void);

/**
 * Creates a font with info parameter specified.
 * @param info See lv_ft_info_t for details.
 *             when success, lv_ft_info_t->font point to the font you created.
 * @return true on success, otherwise false.
 */
bool lv_ft_font_init(lv_ft_info_t *info);

/**
 * Destroy a font that has been created.
 * @param font pointer to font.
 */
void lv_ft_font_destroy(lv_font_t* font);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
