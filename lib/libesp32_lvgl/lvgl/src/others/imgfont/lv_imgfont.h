/**
 * @file lv_imgfont.h
 *
 */

#ifndef LV_IMGFONT_H
#define LV_IMGFONT_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "../../../lvgl.h"

#if LV_USE_IMGFONT

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/* gets the image path name of this character */
typedef const void * (*lv_imgfont_get_path_cb_t)(const lv_font_t * font,
                                                 uint32_t unicode, uint32_t unicode_next,
                                                 int32_t * offset_y, void * user_data);

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Creates a image font with info parameter specified.
 * @param height font size
 * @param path_cb a function to get the image path name of character.
 * @param user_data pointer to user data
 * @return pointer to the new imgfont or NULL if create error.
 */
lv_font_t * lv_imgfont_create(uint16_t height, lv_imgfont_get_path_cb_t path_cb, void * user_data);

/**
 * Destroy a image font that has been created.
 * @param font pointer to image font handle.
 */
void lv_imgfont_destroy(lv_font_t * font);

/**********************
 *      MACROS
 **********************/

#endif /*LV_USE_IMGFONT*/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /* LV_IMGFONT_H */
