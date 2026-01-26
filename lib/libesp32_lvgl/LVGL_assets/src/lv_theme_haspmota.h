/**
 * @file lv_theme_haspmota.h
 *
 */

#ifndef LV_THEME_OPENHASP_H
#define LV_THEME_OPENHASP_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "core/lv_obj.h"

/*********************
 *      DEFINES
 *********************/
typedef enum {
    LV_THEME_HASP_FLAG_DARK          = 0x01,
    LV_THEME_HASP_FLAG_LIGHT         = 0x02,
    LV_THEME_HASP_FLAG_NO_TRANSITION = 0x10,
    LV_THEME_HASP_FLAG_NO_FOCUS      = 0x20,
} lv_theme_hasp_flag_t;

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Initialize the theme
 * @param color_primary the primary color of the theme
 * @param color_secondary the secondary color for the theme
 * @param font pointer to a font to use.
 * @return a pointer to reference this theme later
 */
lv_theme_t * lv_theme_haspmota_init(lv_display_t * disp, lv_color_t * colors, const lv_font_t * font_small, const lv_font_t * font_normal, const lv_font_t * font_large);
bool lv_theme_haspmota_is_inited(void);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_THEME_DEFAULT_H*/
