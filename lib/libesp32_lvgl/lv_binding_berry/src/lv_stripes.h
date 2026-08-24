/**
 * @file lv_stripes.h
 *
 * A rectangle widget filled with a repeating pattern of straight
 * stripes drawn at an arbitrary angle, on top of the normal `lv_obj`
 * background. Stripe color, opacity, width and inter-stripe gap width
 * are all configurable, as is the pattern angle.
 *
 * The background (color/opacity/radius/border/...) is the regular
 * `LV_PART_MAIN` style of `lv_obj`, so it composes with the normal
 * LVGL style system. Stripes are drawn as a set of thick lines,
 * clipped to the object's bounds.
 *
 * Part of the Tasmota project (https://github.com/arendst/Tasmota).
 * Author: Stephan Hadinger
 */

#ifndef LV_STRIPES_H
#define LV_STRIPES_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

extern const lv_obj_class_t lv_stripes_class;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Create a stripes object.
 * @param parent pointer to an object, it will be the parent of the new stripes object
 * @return pointer to the created stripes object
 */
lv_obj_t * lv_stripes_create(lv_obj_t * parent);

/*=====================
 * Setter functions
 *====================*/

/**
 * Set the color of the stripes.
 * @param obj pointer to a stripes object
 * @param color color of the stripes
 */
void lv_stripes_set_stripe_color(lv_obj_t * obj, lv_color_t color);

/**
 * Set the opacity of the stripes.
 * @param obj pointer to a stripes object
 * @param opa opacity 0 (transparent) .. 255 (opaque). `LV_OPA_*` constants can be used.
 */
void lv_stripes_set_stripe_opa(lv_obj_t * obj, lv_opa_t opa);

/**
 * Set the width (thickness) of each stripe, in pixels.
 * @param obj pointer to a stripes object
 * @param width width of a stripe in pixels, minimum 1
 */
void lv_stripes_set_stripe_width(lv_obj_t * obj, int32_t width);

/**
 * Set the width of the gap between two consecutive stripes, in pixels.
 * @param obj pointer to a stripes object
 * @param width width of the gap in pixels, 0 means stripes are contiguous
 */
void lv_stripes_set_gap_width(lv_obj_t * obj, int32_t width);

/**
 * Set the angle of the stripes.
 * @param obj pointer to a stripes object
 * @param angle angle in degrees, 0 is horizontal stripes, 90 is vertical
 *              stripes. Any value is accepted and normalized to 0..359.
 */
void lv_stripes_set_angle(lv_obj_t * obj, int32_t angle);

/**
 * Set the offset of the pattern.
 * The pattern is shifted by `offset` pixels along the direction perpendicular
 * to the stripes (the direction along which stripes are spaced). Because the
 * pattern repeats every `stripe_width + gap_width` pixels, the offset wraps
 * seamlessly, so it can be freely incremented to animate a scrolling motion.
 * @param obj pointer to a stripes object
 * @param offset offset in pixels, any value is accepted (wrapped internally)
 */
void lv_stripes_set_offset(lv_obj_t * obj, int32_t offset);

/*=====================
 * Getter functions
 *====================*/

/**
 * Get the color of the stripes.
 * @param obj pointer to a stripes object
 * @return color of the stripes
 */
lv_color_t lv_stripes_get_stripe_color(lv_obj_t * obj);

/**
 * Get the opacity of the stripes.
 * @param obj pointer to a stripes object
 * @return opacity 0..255
 */
lv_opa_t lv_stripes_get_stripe_opa(lv_obj_t * obj);

/**
 * Get the width of each stripe.
 * @param obj pointer to a stripes object
 * @return width of a stripe in pixels
 */
int32_t lv_stripes_get_stripe_width(lv_obj_t * obj);

/**
 * Get the width of the gap between two consecutive stripes.
 * @param obj pointer to a stripes object
 * @return width of the gap in pixels
 */
int32_t lv_stripes_get_gap_width(lv_obj_t * obj);

/**
 * Get the angle of the stripes.
 * @param obj pointer to a stripes object
 * @return angle in degrees (0..359)
 */
int32_t lv_stripes_get_angle(lv_obj_t * obj);

/**
 * Get the offset of the pattern.
 * @param obj pointer to a stripes object
 * @return offset in pixels (as last set, not wrapped)
 */
int32_t lv_stripes_get_offset(lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_STRIPES_H*/
