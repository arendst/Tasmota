/**
 * @file lv_roller.h
 *
 */

#ifndef LV_ROLLER_H
#define LV_ROLLER_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "../../core/lv_obj.h"

#if LV_USE_ROLLER != 0

/*Testing of dependencies*/
#if LV_USE_LABEL == 0
#error "lv_roller: lv_label is required. Enable it in lv_conf.h (LV_USE_ROLLER 1)"
#endif

#include "../label/lv_label.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/** Roller mode. */
typedef enum {
    LV_ROLLER_MODE_NORMAL,   /**< Normal mode (roller ends at the end of the options). */
    LV_ROLLER_MODE_INFINITE, /**< Infinite mode (roller can be scrolled forever). */
} lv_roller_mode_t;

#if LV_USE_OBJ_PROPERTY
enum {
    LV_PROPERTY_ID(ROLLER, OPTIONS,             LV_PROPERTY_TYPE_TEXT,   0),
    LV_PROPERTY_ID(ROLLER, SELECTED,            LV_PROPERTY_TYPE_INT,    1),
    LV_PROPERTY_ID(ROLLER, VISIBLE_ROW_COUNT,   LV_PROPERTY_TYPE_INT,    2),
    LV_PROPERTY_ROLLER_END,
};
#endif

LV_ATTRIBUTE_EXTERN_DATA extern const lv_obj_class_t lv_roller_class;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Create a roller object
 * @param parent    pointer to an object, it will be the parent of the new roller.
 * @return          pointer to the created roller
 */
lv_obj_t * lv_roller_create(lv_obj_t * parent);

/*=====================
 * Setter functions
 *====================*/

/**
 * Set the options on a roller
 * @param obj       pointer to roller object
 * @param options   a string with '\n' separated options. E.g. "One\nTwo\nThree"
 * @param mode      `LV_ROLLER_MODE_NORMAL` or `LV_ROLLER_MODE_INFINITE`
 */
void lv_roller_set_options(lv_obj_t * obj, const char * options, lv_roller_mode_t mode);

/**
 * Set the selected option
 * @param obj       pointer to a roller object
 * @param sel_opt   index of the selected option (0 ... number of option - 1);
 * @param anim   LV_ANIM_ON: set with animation; LV_ANOM_OFF set immediately
 */
void lv_roller_set_selected(lv_obj_t * obj, uint32_t sel_opt, lv_anim_enable_t anim);

/**
 * Set the height to show the given number of rows (options)
 * @param obj       pointer to a roller object
 * @param row_cnt   number of desired visible rows
 */
void lv_roller_set_visible_row_count(lv_obj_t * obj, uint32_t row_cnt);

/*=====================
 * Getter functions
 *====================*/

/**
 * Get the index of the selected option
 * @param obj       pointer to a roller object
 * @return          index of the selected option (0 ... number of option - 1);
 */
uint32_t lv_roller_get_selected(const lv_obj_t * obj);

/**
 * Get the current selected option as a string.
 * @param obj       pointer to roller object
 * @param buf       pointer to an array to store the string
 * @param buf_size  size of `buf` in bytes. 0: to ignore it.
 */
void lv_roller_get_selected_str(const lv_obj_t * obj, char * buf, uint32_t buf_size);

/**
 * Get the options of a roller
 * @param obj       pointer to roller object
 * @return          the options separated by '\n'-s (E.g. "Option1\nOption2\nOption3")
 */
const char * lv_roller_get_options(const lv_obj_t * obj);

/**
 * Get the total number of options
 * @param obj   pointer to a roller object
 * @return      the total number of options
 */
uint32_t lv_roller_get_option_count(const lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

#endif /*LV_USE_ROLLER*/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_ROLLER_H*/
