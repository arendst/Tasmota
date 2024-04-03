/**
 *@file lv_theme.h
 *
 */

#ifndef LV_THEME_H
#define LV_THEME_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *    INCLUDES
 *********************/
#include "../core/lv_obj.h"

/*********************
 *    DEFINES
 *********************/

/**********************
 *    TYPEDEFS
 **********************/

typedef void (*lv_theme_apply_cb_t)(lv_theme_t *, lv_obj_t *);

struct _lv_theme_t {
    lv_theme_apply_cb_t apply_cb;
    lv_theme_t * parent;   /**< Apply the current theme's style on top of this theme.*/
    void * user_data;
    lv_display_t * disp;
    lv_color_t color_primary;
    lv_color_t color_secondary;
    const lv_font_t * font_small;
    const lv_font_t * font_normal;
    const lv_font_t * font_large;
    uint32_t flags;                 /*Any custom flag used by the theme*/
};

/**********************
 *  GLOBAL PROTOTYPES
 **********************/

/**
 * Get the theme assigned to the display of the object
 * @param obj       pointer to a theme object
 * @return          the theme of the object's display (can be NULL)
 */
lv_theme_t  * lv_theme_get_from_obj(lv_obj_t * obj);

/**
 * Apply the active theme on an object
 * @param obj pointer to an object
 */
void lv_theme_apply(lv_obj_t * obj);

/**
 * Set a base theme for a theme.
 * The styles from the base them will be added before the styles of the current theme.
 * Arbitrary long chain of themes can be created by setting base themes.
 * @param new_theme pointer to theme which base should be set
 * @param parent pointer to the base theme
 */
void lv_theme_set_parent(lv_theme_t * new_theme, lv_theme_t * parent);

/**
 * Set an apply callback for a theme.
 * The apply callback is used to add styles to different objects
 * @param theme pointer to theme which callback should be set
 * @param apply_cb pointer to the callback
 */
void lv_theme_set_apply_cb(lv_theme_t * theme, lv_theme_apply_cb_t apply_cb);

/**
 * Get the small font of the theme
 * @param obj pointer to an object
 * @return pointer to the font
 */
const lv_font_t * lv_theme_get_font_small(lv_obj_t * obj);
/**
 * Get the normal font of the theme
 * @param obj pointer to an object
 * @return pointer to the font
 */
const lv_font_t * lv_theme_get_font_normal(lv_obj_t * obj);

/**
 * Get the subtitle font of the theme
 * @param obj pointer to an object
 * @return pointer to the font
 */
const lv_font_t * lv_theme_get_font_large(lv_obj_t * obj);

/**
 * Get the primary color of the theme
 * @param obj pointer to an object
 * @return the color
 */
lv_color_t lv_theme_get_color_primary(lv_obj_t * obj);

/**
 * Get the secondary color of the theme
 * @param obj pointer to an object
 * @return the color
 */
lv_color_t lv_theme_get_color_secondary(lv_obj_t * obj);

/**********************
 *    MACROS
 **********************/

#include "default/lv_theme_default.h"
#include "mono/lv_theme_mono.h"
#include "simple/lv_theme_simple.h"

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_THEME_H*/
