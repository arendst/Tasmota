/**
 * @file lv_bar.h
 *
 */

#ifndef LV_BAR_H
#define LV_BAR_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "../../lv_conf_internal.h"

#if LV_USE_BAR != 0

#include "../../core/lv_obj.h"
#include "../../misc/lv_anim.h"
#include "../label/lv_label.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
enum _lv_bar_mode_t {
    LV_BAR_MODE_NORMAL,
    LV_BAR_MODE_SYMMETRICAL,
    LV_BAR_MODE_RANGE
};

#ifdef DOXYGEN
typedef _lv_bar_mode_t lv_bar_mode_t;
#else
typedef uint8_t lv_bar_mode_t;
#endif /*DOXYGEN*/

typedef struct {
    lv_obj_t * bar;
    int32_t anim_start;
    int32_t anim_end;
    int32_t anim_state;
} _lv_bar_anim_t;

typedef struct {
    lv_obj_t obj;
    int32_t cur_value;          /**< Current value of the bar*/
    int32_t min_value;          /**< Minimum value of the bar*/
    int32_t max_value;          /**< Maximum value of the bar*/
    int32_t start_value;        /**< Start value of the bar*/
    lv_area_t indic_area;       /**< Save the indicator area. Might be used by derived types*/
    bool val_reversed;          /**< Whether value been reversed */
    _lv_bar_anim_t cur_value_anim;
    _lv_bar_anim_t start_value_anim;
    lv_bar_mode_t mode : 2;     /**< Type of bar*/
} lv_bar_t;

LV_ATTRIBUTE_EXTERN_DATA extern const lv_obj_class_t lv_bar_class;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Create a bar object
 * @param parent        pointer to an object, it will be the parent of the new bar
 * @return              pointer to the created bar
 */
lv_obj_t * lv_bar_create(lv_obj_t * parent);

/*=====================
 * Setter functions
 *====================*/

/**
 * Set a new value on the bar
 * @param obj           pointer to a bar object
 * @param value         new value
 * @param anim          LV_ANIM_ON: set the value with an animation; LV_ANIM_OFF: change the value immediately
 */
void lv_bar_set_value(lv_obj_t * obj, int32_t value, lv_anim_enable_t anim);

/**
 * Set a new start value on the bar
 * @param obj             pointer to a bar object
 * @param start_value     new start value
 * @param anim            LV_ANIM_ON: set the value with an animation; LV_ANIM_OFF: change the value immediately
 */
void lv_bar_set_start_value(lv_obj_t * obj, int32_t start_value, lv_anim_enable_t anim);

/**
 * Set minimum and the maximum values of a bar
 * @param obj       pointer to the bar object
 * @param min       minimum value
 * @param max       maximum value
 * @note If min is greater than max, the drawing direction becomes to the oppsite direction.
 */
void lv_bar_set_range(lv_obj_t * obj, int32_t min, int32_t max);

/**
 * Set the type of bar.
 * @param obj       pointer to bar object
 * @param mode      bar type from ::lv_bar_mode_t
 */
void lv_bar_set_mode(lv_obj_t * obj, lv_bar_mode_t mode);

/*=====================
 * Getter functions
 *====================*/

/**
 * Get the value of a bar
 * @param obj       pointer to a bar object
 * @return          the value of the bar
 */
int32_t lv_bar_get_value(const lv_obj_t * obj);

/**
 * Get the start value of a bar
 * @param obj       pointer to a bar object
 * @return          the start value of the bar
 */
int32_t lv_bar_get_start_value(const lv_obj_t * obj);

/**
 * Get the minimum value of a bar
 * @param obj       pointer to a bar object
 * @return          the minimum value of the bar
 */
int32_t lv_bar_get_min_value(const lv_obj_t * obj);

/**
 * Get the maximum value of a bar
 * @param obj       pointer to a bar object
 * @return          the maximum value of the bar
 */
int32_t lv_bar_get_max_value(const lv_obj_t * obj);

/**
 * Get the type of bar.
 * @param obj       pointer to bar object
 * @return          bar type from ::lv_bar_mode_t
 */
lv_bar_mode_t lv_bar_get_mode(lv_obj_t * obj);

/**
 * Give the bar is in symmetrical mode or not
 * @param obj       pointer to bar object
 * @return          true: in symmetrical mode false : not in
*/
bool lv_bar_is_symmetrical(lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#endif /*LV_USE_BAR*/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_BAR_H*/
