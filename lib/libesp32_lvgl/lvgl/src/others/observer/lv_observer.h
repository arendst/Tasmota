/**
 * @file lv_observer.h
 *
 */

#ifndef LV_OBSERVER_H
#define LV_OBSERVER_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#include "../../core/lv_obj.h"
#if LV_USE_OBSERVER

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

struct _lv_observer_t;
typedef struct _lv_observer_t lv_observer_t;

typedef enum {
    LV_SUBJECT_TYPE_INVALID =   0,   /**< indicates subject not initialized yet*/
    LV_SUBJECT_TYPE_NONE =      1,   /**< a null value like None or NILt*/
    LV_SUBJECT_TYPE_INT =       2,   /**< an int32_t*/
    LV_SUBJECT_TYPE_POINTER =   3,   /**< a void pointer*/
    LV_SUBJECT_TYPE_COLOR   =   4,   /**< an lv_color_t*/
    LV_SUBJECT_TYPE_GROUP  =    5,   /**< an array of subjects*/
    LV_SUBJECT_TYPE_STRING  =   6,   /**< a char pointer*/
} lv_subject_type_t;

/**
 * A common type to handle all the various observable types in the same way
 */
typedef union {
    int32_t num; /**< Integer number (opacity, enums, booleans or "normal" numbers)*/
    const void * pointer; /**< Constant pointer  (string buffer, format string, font, cone text, etc)*/
    lv_color_t color; /**< Color */
} lv_subject_value_t;

/**
 * The subject (an observable value)
 */
typedef struct {
    lv_ll_t subs_ll;                    /**< Subscribers*/
    uint32_t type   : 4;
    uint32_t size   : 28;               /**< Might be used to store a size related to `type`*/
    lv_subject_value_t value;           /**< Actual value*/
    lv_subject_value_t prev_value;      /**< Previous value*/
    uint32_t notify_restart_query : 1; /**< If an observer deleted start notifying from the beginning. */
    void * user_data;                   /**< Additional parameter, can be used freely by the user*/
} lv_subject_t;

/**
  * Callback called when the observed value changes
  * @param observer     pointer to the observer of the callback
  * @param subject      pointer to the subject of the observer
  */
typedef void (*lv_observer_cb_t)(lv_observer_t * observer, lv_subject_t * subject);

/**
 * The observer object: a descriptor returned when subscribing LVGL widgets to subjects
 */
struct _lv_observer_t {
    lv_subject_t * subject;             /**< The observed value */
    lv_observer_cb_t cb;                /**< Callback that should be called when the value changes*/
    void * target;                      /**< A target for the observer, e.g. a widget or style*/
    void * user_data;                   /**< Additional parameter supplied when subscribing*/
    uint32_t auto_free_user_data : 1;   /**< Automatically free user data when the observer is removed */
    uint32_t notified : 1;              /**< Mark if this observer was already notified*/
};

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Initialize an integer type subject
 * @param subject   pointer to the subject
 * @param value     initial value
 */
void lv_subject_init_int(lv_subject_t * subject, int32_t value);

/**
 * Set the value of an integer subject. It will notify all the observers as well.
 * @param subject   pointer to the subject
 * @param value     the new value
 */
void lv_subject_set_int(lv_subject_t * subject, int32_t value);

/**
 * Get the current value of an integer subject
 * @param subject   pointer to the subject
 * @return          the current value
 */
int32_t lv_subject_get_int(lv_subject_t * subject);

/**
 * Get the previous value of an integer subject
 * @param subject   pointer to the subject
 * @return          the current value
 */
int32_t lv_subject_get_previous_int(lv_subject_t * subject);

/**
 * Initialize a string type subject
 * @param subject   pointer to the subject
 * @param buf       pointer to a buffer to store the string
 * @param prev_buf  pointer to a buffer to store the previous string, can be NULL if not used
 * @param size      size of the buffer
 * @param value     initial value as a string, e.g. "hello"
 * @note            the string subject stores the whole string, not only a pointer
 */
void lv_subject_init_string(lv_subject_t * subject, char * buf, char * prev_buf, size_t size, const char * value);

/**
 * Copy a string to a subject. It will notify all the observers as well.
 * @param subject   pointer to the subject
 * @param buf       the new string
 */
void lv_subject_copy_string(lv_subject_t * subject, const char * buf);

/**
 * Get the current value of an string subject
 * @param subject   pointer to the subject
 * @return          pointer to the buffer containing the current value
 */
const char * lv_subject_get_string(lv_subject_t * subject);

/**
 * Get the previous value of an string subject
 * @param subject   pointer to the subject
 * @return          pointer to the buffer containing the current value
 * @note            NULL will be returned if NULL was passed in `lv_subject_init_string()`
 *                  as `prev_buf`
 */
const char * lv_subject_get_previous_string(lv_subject_t * subject);

/**
 * Initialize an pointer type subject
 * @param subject   pointer to the subject
 * @param value     initial value
 */
void lv_subject_init_pointer(lv_subject_t * subject, void * value);

/**
 * Set the value of a pointer subject. It will notify all the observers as well.
 * @param subject   pointer to the subject
 * @param value     the new value
 */
void lv_subject_set_pointer(lv_subject_t * subject, void * ptr);

/**
 * Get the current value of a pointer subject
 * @param subject   pointer to the subject
 * @return          the current value
 */
const void * lv_subject_get_pointer(lv_subject_t * subject);

/**
 * Get the previous value of a pointer subject
 * @param subject   pointer to the subject
 * @return          the current value
 */
const void * lv_subject_get_previous_pointer(lv_subject_t * subject);

/**
 * Initialize an color type subject
 * @param subject   pointer to the subject
 * @param value     initial value
 */
void lv_subject_init_color(lv_subject_t * subject, lv_color_t color);

/**
 * Set the value of a color subject. It will notify all the observers as well.
 * @param subject   pointer to the subject
 * @param value     the new value
 */
void lv_subject_set_color(lv_subject_t * subject, lv_color_t color);

/**
 * Get the current value of a color subject
 * @param subject   pointer to the subject
 * @return          the current value
 */
lv_color_t lv_subject_get_color(lv_subject_t * subject);

/**
 * Get the previous value of a color subject
 * @param subject   pointer to the subject
 * @return          the current value
 */
lv_color_t lv_subject_get_previous_color(lv_subject_t * subject);

/**
 * Initialize a subject group
 * @param subject   pointer to the subject
 * @param list      list of other subject addresses, any of these changes `subject` will be notified
 * @param list_len  number of elements in `list`
 */
void lv_subject_init_group(lv_subject_t * subject, lv_subject_t * list[], uint32_t list_len);

/**
 * Get an element from the subject group's list
 * @param subject   pointer to the subject
 * @param index     index of the element to get
 * @return          pointer a subject from the list, or NULL if the index is out of bounds
 */
lv_subject_t * lv_subject_get_group_element(lv_subject_t * subject, int32_t index);

/**
 * Add an observer to a subject. When the subject changes `observer_cb` will be called.
 * @param subject       pointer to the subject
 * @param observer_cb   the callback to call
 * @param user_data     optional user data
 * @return              pointer to the created observer
 */
lv_observer_t * lv_subject_add_observer(lv_subject_t * subject, lv_observer_cb_t observer_cb, void * user_data);

/**
 * Add an observer to a subject for an object.
 * When the object is deleted, it will be removed from the subject automatically.
 * @param subject       pointer to the subject
 * @param observer_cb   the callback to call
 * @param obj           pointer to an object
 * @param user_data     optional user data
 * @return              pointer to the created observer
 */
lv_observer_t * lv_subject_add_observer_obj(lv_subject_t * subject, lv_observer_cb_t observer_cb, lv_obj_t * obj,
                                            void * user_data);

/**
 * Add an observer to a subject and also save a target.
 * @param subject       pointer to the subject
 * @param observer_cb   the callback to call
 * @param target        pointer to any data
 * @param user_data     optional user data
 * @return              pointer to the created observer
 */
lv_observer_t * lv_subject_add_observer_with_target(lv_subject_t * subject, lv_observer_cb_t cb, void * target,
                                                    void * user_data);

/**
 * Remove an observer from its subject
 * @param observer      pointer to an observer
 */
void lv_observer_remove(lv_observer_t * observer);

/**
 * Remove all observers from their subject related to an object
 * @param observer      pointer to an observer
 * @param obj           pointer to an object
 */
void lv_subject_remove_all_obj(lv_subject_t * subject, lv_obj_t * obj);

/**
 * Get the target of an observer
 * @param observer      pointer to an observer
 * @return              pointer to the saved target
 */
void * lv_observer_get_target(lv_observer_t * observer);

/**
 * Get the target object of the observer.
 * It's the same as `lv_observer_get_target` and added only
 * for semantic reasons
 * @param observer      pointer to an observer
 * @return              pointer to the saved object target
 */
static inline lv_obj_t * lv_observer_get_target_obj(lv_observer_t * observer)
{
    return (lv_obj_t *)lv_observer_get_target(observer);
}

/**
 * Notify all observers of subject
 * @param subject       pointer to a subject
 */
void lv_subject_notify(lv_subject_t * subject);

/**
 * Set an object flag if an integer subject's value is equal to a reference value, clear the flag otherwise
 * @param obj           pointer to an object
 * @param subject       pointer to a subject
 * @param flag          a flag to set or clear (e.g. `LV_OBJ_FLAG_HIDDEN`)
 * @param ref_value     a reference value to compare the subject's value with
 * @return              pointer to the created observer
 */
lv_observer_t * lv_obj_bind_flag_if_eq(lv_obj_t * obj, lv_subject_t * subject, lv_obj_flag_t flag, int32_t ref_value);

/**
 * Set an object flag if an integer subject's value is not equal to a reference value, clear the flag otherwise
 * @param obj           pointer to an object
 * @param subject       pointer to a subject
 * @param flag          a flag to set or clear (e.g. `LV_OBJ_FLAG_HIDDEN`)
 * @param ref_value     a reference value to compare the subject's value with
 * @return              pointer to the created observer
 */
lv_observer_t * lv_obj_bind_flag_if_not_eq(lv_obj_t * obj, lv_subject_t * subject, lv_obj_flag_t flag,
                                           int32_t ref_value);

/**
 * Set an object state if an integer subject's value is equal to a reference value, clear the flag otherwise
 * @param obj           pointer to an object
 * @param subject       pointer to a subject
 * @param flag          a state to set or clear (e.g. `LV_STATE_CHECKED`)
 * @param ref_value     a reference value to compare the subject's value with
 * @return              pointer to the created observer
 */
lv_observer_t * lv_obj_bind_state_if_eq(lv_obj_t * obj, lv_subject_t * subject, lv_state_t state, int32_t ref_value);

/**
 * Set an object state if an integer subject's value is not equal to a reference value, clear the flag otherwise
 * @param obj           pointer to an object
 * @param subject       pointer to a subject
 * @param flag          a state to set or clear (e.g. `LV_STATE_CHECKED`)
 * @param ref_value     a reference value to compare the subject's value with
 * @return              pointer to the created observer
 */
lv_observer_t * lv_obj_bind_state_if_not_eq(lv_obj_t * obj, lv_subject_t * subject, lv_state_t state,
                                            int32_t ref_value);

#if LV_USE_BUTTON
/**
 * Set an integer subject to 1 when a button is checked and set it 0 when unchecked.
 * @param obj       pointer to a button
 * @param subject   pointer to a subject
 * @return          pointer to the created observer
 */
lv_observer_t * lv_button_bind_checked(lv_obj_t * obj, lv_subject_t * subject);
#endif

#if LV_USE_LABEL
/**
 * Bind an integer, string, or pointer subject to a label.
 * @param obj       pointer to a label
 * @param subject   pointer to a subject
 * @param fmt       an optional format string with 1 format specifier (e.g. "%d °C")
 *                  or NULL to bind the value directly.
 * @return          pointer to the created observer
 * @note            fmt == NULL can be used only with string and pointer subjects.
 * @note            if the subject is a pointer must point to a `\0` terminated string.
 */
lv_observer_t * lv_label_bind_text(lv_obj_t * obj, lv_subject_t * subject, const char * fmt);
#endif

#if LV_USE_ARC
/**
 * Bind an integer subject to an arc's value
 * @param obj       pointer to an arc
 * @param subject   pointer to a subject
 * @return          pointer to the created observer
 */
lv_observer_t * lv_arc_bind_value(lv_obj_t * obj, lv_subject_t * subject);
#endif

#if LV_USE_SLIDER
/**
 * Bind an integer subject to a slider's value
 * @param obj       pointer to a slider
 * @param subject   pointer to a subject
 * @return          pointer to the created observer
 */
lv_observer_t * lv_slider_bind_value(lv_obj_t * obj, lv_subject_t * subject);
#endif

#if LV_USE_ROLLER
/**
 * Bind an integer subject to a roller's value
 * @param obj       pointer to a roller
 * @param subject   pointer to a subject
 * @return          pointer to the created observer
 */
lv_observer_t * lv_roller_bind_value(lv_obj_t * obj, lv_subject_t * subject);
#endif

#if LV_USE_DROPDOWN
/**
 * Bind an integer subject to a dropdown's value
 * @param obj       pointer to a drop down
 * @param subject   pointer to a subject
 * @return          pointer to the created observer
 */
lv_observer_t * lv_dropdown_bind_value(lv_obj_t * obj, lv_subject_t * subject);
#endif

/**********************
 *      MACROS
 **********************/

#endif /*LV_USE_OBSERVER*/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_OBSERVER_H*/
