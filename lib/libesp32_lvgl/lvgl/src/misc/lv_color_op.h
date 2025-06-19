/**
 * @file lv_color_op.h
 *
 */

#ifndef LV_COLOR_OP_H
#define LV_COLOR_OP_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lv_assert.h"
#include "lv_math.h"
#include "lv_color.h"
#include "lv_types.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

struct _lv_color_filter_dsc_t;

typedef lv_color_t (*lv_color_filter_cb_t)(const struct _lv_color_filter_dsc_t *, lv_color_t, lv_opa_t);

struct _lv_color_filter_dsc_t {
    lv_color_filter_cb_t filter_cb;
    void * user_data;
};

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Mix two colors with a given ratio.
 * @param c1 the first color to mix (usually the foreground)
 * @param c2 the second color to mix (usually the background)
 * @param mix The ratio of the colors. 0: full `c2`, 255: full `c1`, 127: half `c1` and half`c2`
 * @return the mixed color
 */
lv_color_t LV_ATTRIBUTE_FAST_MEM lv_color_mix(lv_color_t c1, lv_color_t c2, uint8_t mix);

/**
 *
 * @param fg
 * @param bg
 * @return
 * @note Use bg.alpha in the return value
 * @note Use fg.alpha as mix ratio
 */
lv_color32_t lv_color_mix32(lv_color32_t fg, lv_color32_t bg);

/**
 * @brief Blends two premultiplied ARGB8888 colors while maintaining correct alpha compositing.
 *
 * This function correctly blends the foreground (fg) and background (bg) colors,
 * ensuring that the output remains in a premultiplied alpha format.
 *
 * @param fg The foreground color in premultiplied ARGB8888 format.
 * @param bg The background color in premultiplied ARGB8888 format.
 * @return The resulting blended color in premultiplied ARGB8888 format.
 *
 * @note If the foreground is fully opaque, it is returned as is.
 * @note If the foreground is fully transparent, the background is returned.
 */
lv_color32_t lv_color_mix32_premultiplied(lv_color32_t fg, lv_color32_t bg);

/**
 * Get the brightness of a color
 * @param c   a color
 * @return brightness in range [0..255]
 */
uint8_t lv_color_brightness(lv_color_t c);

void lv_color_filter_dsc_init(lv_color_filter_dsc_t * dsc, lv_color_filter_cb_t cb);

/**
 * Blend two colors that have not been pre-multiplied using their alpha values
 * @param fg the foreground color
 * @param bg the background color
 * @return result color
 */
lv_color32_t lv_color_over32(lv_color32_t fg, lv_color32_t bg);

/**********************
 *  PREDEFINED COLORS
 **********************/

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_COLOR_H*/
