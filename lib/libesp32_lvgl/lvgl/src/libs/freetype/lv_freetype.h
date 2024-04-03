/**
 * @file lv_freetype.h
 *
 */
#ifndef LV_FREETYPE_H
#define LV_FREETYPE_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "../../../lvgl.h"

#if LV_USE_FREETYPE

/*********************
 *      DEFINES
 *********************/

#define LV_FREETYPE_F26DOT6_TO_INT(x)   ((x) >> 6)
#define LV_FREETYPE_F26DOT6_TO_FLOAT(x) ((float)(x) / 64)

#define FT_FONT_STYLE_NORMAL LV_FREETYPE_FONT_STYLE_NORMAL
#define FT_FONT_STYLE_ITALIC LV_FREETYPE_FONT_STYLE_ITALIC
#define FT_FONT_STYLE_BOLD   LV_FREETYPE_FONT_STYLE_BOLD

/**********************
 *      TYPEDEFS
 **********************/

enum {
    LV_FREETYPE_FONT_STYLE_NORMAL = 0,
    LV_FREETYPE_FONT_STYLE_ITALIC = 1 << 0,
    LV_FREETYPE_FONT_STYLE_BOLD = 1 << 1,
};

typedef uint16_t lv_freetype_font_style_t;
typedef lv_freetype_font_style_t LV_FT_FONT_STYLE;

enum {
    LV_FREETYPE_FONT_RENDER_MODE_BITMAP = 0,
    LV_FREETYPE_FONT_RENDER_MODE_OUTLINE = 1,
};

typedef uint16_t lv_freetype_font_render_mode_t;

typedef void * lv_freetype_outline_t;

typedef enum {
    LV_FREETYPE_OUTLINE_END,
    LV_FREETYPE_OUTLINE_MOVE_TO,
    LV_FREETYPE_OUTLINE_LINE_TO,
    LV_FREETYPE_OUTLINE_CUBIC_TO,
    LV_FREETYPE_OUTLINE_CONIC_TO,
} lv_freetype_outline_type_t;

typedef struct {
    int32_t x;
    int32_t y;
} lv_freetype_outline_vector_t;

typedef struct {
    lv_freetype_outline_t outline;
    lv_freetype_outline_type_t type;
    lv_freetype_outline_vector_t to;
    lv_freetype_outline_vector_t control1;
    lv_freetype_outline_vector_t control2;
} lv_freetype_outline_event_param_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Initialize the freetype library.
 * @param max_faces Maximum number of opened FT_Face objects managed by this cache instance. Use 0 for defaults.
 * @param max_sizes Maximum number of opened FT_Size objects managed by this cache instance. Use 0 for defaults.
 * @param max_kilobytes Maximum number of kilobytes to use for cached data nodes. Use 0 for defaults.
 *                  Note that this value does not account for managed FT_Face and FT_Size objects.
 * @return LV_RESULT_OK on success, otherwise LV_RESULT_INVALID.
 */
lv_result_t lv_freetype_init(uint32_t max_faces, uint32_t max_sizes, uint32_t max_kilobytes);

/**
 * Uninitialize the freetype library
 */
void lv_freetype_uninit(void);

/**
 * Create a freetype font.
 * @param pathname font file path.
 * @param render_mode font render mode(see @lv_freetype_font_render_mode_t for details).
 * @param size font size.
 * @param style font style(see lv_freetype_font_style_t for details).
 * @return Created font, or NULL on failure.
 */
lv_font_t * lv_freetype_font_create(const char * pathname, lv_freetype_font_render_mode_t render_mode, uint32_t size,
                                    lv_freetype_font_style_t style);

/**
 * Delete a freetype font.
 * @param font freetype font to be deleted.
 */
void lv_freetype_font_delete(lv_font_t * font);

/**
 * Register a callback function to generate outlines for FreeType fonts.
 *
 * @param cb The callback function to be registered.
 * @param user_data User data to be passed to the callback function.
 * @return The ID of the registered callback function, or a negative value on failure.
 */
void lv_freetype_outline_add_event(lv_event_cb_t event_cb, lv_event_code_t filter, void * user_data);

/**
 * Get the scale of a FreeType font.
 *
 * @param font The FreeType font to get the scale of.
 * @return The scale of the FreeType font.
 */
uint32_t lv_freetype_outline_get_scale(const lv_font_t * font);

/**
 * Check if the font is an outline font.
 *
 * @param font The FreeType font.
 * @return Is outline font on success, otherwise false.
 */
bool lv_freetype_is_outline_font(const lv_font_t * font);

/**********************
 *      MACROS
 **********************/

#endif /*LV_USE_FREETYPE*/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LV_FREETYPE_H */
