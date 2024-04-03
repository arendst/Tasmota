/**
 * @file lv_observer.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#include "lv_observer.h"
#if LV_USE_OBSERVER

#include "../../lvgl.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
typedef struct {
    uint32_t flag;
    lv_subject_value_t value;
    uint32_t inv    : 1;
} flag_and_cond_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void unsubscribe_on_delete_cb(lv_event_t * e);
static void group_notify_cb(lv_observer_t * observer, lv_subject_t * subject);
static lv_observer_t * bind_to_bitfield(lv_subject_t * subject, lv_obj_t * obj, lv_observer_cb_t cb, uint32_t flag,
                                        int32_t ref_value, bool inv);
static void obj_flag_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
static void obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject);

#if LV_USE_BUTTON
    static void btn_value_changed_event_cb(lv_event_t * e);
#endif

#if LV_USE_LABEL
    static void label_text_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
#endif

#if LV_USE_ARC
    static void arc_value_changed_event_cb(lv_event_t * e);
    static void arc_value_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
#endif

#if LV_USE_SLIDER
    static void slider_value_changed_event_cb(lv_event_t * e);
    static void slider_value_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
#endif

#if LV_USE_ROLLER
    static void roller_value_changed_event_cb(lv_event_t * e);
    static void roller_value_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
#endif

#if LV_USE_DROPDOWN
    static void dropdown_value_changed_event_cb(lv_event_t * e);
    static void dropdown_value_observer_cb(lv_observer_t * observer, lv_subject_t * subject);
#endif

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_subject_init_int(lv_subject_t * subject, int32_t value)
{
    lv_memzero(subject, sizeof(lv_subject_t));
    subject->type = LV_SUBJECT_TYPE_INT;
    subject->value.num = value;
    subject->prev_value.num = value;
    _lv_ll_init(&(subject->subs_ll), sizeof(lv_observer_t));
}

void lv_subject_set_int(lv_subject_t * subject, int32_t value)
{
    if(subject->type != LV_SUBJECT_TYPE_INT) {
        LV_LOG_WARN("Subject type is not LV_SUBJECT_TYPE_INT");
        return;
    }

    subject->prev_value.num = subject->value.num;
    subject->value.num = value;
    lv_subject_notify(subject);
}

int32_t lv_subject_get_int(lv_subject_t * subject)
{
    if(subject->type != LV_SUBJECT_TYPE_INT) {
        LV_LOG_WARN("Subject type is not LV_SUBJECT_TYPE_INT");
        return 0;
    }

    return subject->value.num;
}

int32_t lv_subject_get_previous_int(lv_subject_t * subject)
{
    if(subject->type != LV_SUBJECT_TYPE_INT) {
        LV_LOG_WARN("Subject type is not LV_SUBJECT_TYPE_INT");
        return 0;
    }

    return subject->prev_value.num;
}

void lv_subject_init_string(lv_subject_t * subject, char * buf, char * prev_buf, size_t size, const char * value)
{
    lv_memzero(subject, sizeof(lv_subject_t));
    lv_strncpy(buf, value, size);
    if(prev_buf) lv_strncpy(prev_buf, value, size);

    subject->type = LV_SUBJECT_TYPE_STRING;
    subject->size = size;
    subject->value.pointer = buf;
    subject->prev_value.pointer = prev_buf;

    _lv_ll_init(&(subject->subs_ll), sizeof(lv_observer_t));
}

void lv_subject_copy_string(lv_subject_t * subject, const char * buf)
{
    if(subject->type != LV_SUBJECT_TYPE_STRING) {
        LV_LOG_WARN("Subject type is not LV_SUBJECT_TYPE_INT");
        return;
    }

    if(subject->size < 1) return;
    if(subject->prev_value.pointer) {
        lv_strncpy((char *)subject->prev_value.pointer, subject->value.pointer, subject->size - 1);
    }

    lv_strncpy((char *)subject->value.pointer, buf, subject->size - 1);

    lv_subject_notify(subject);

}

const char * lv_subject_get_string(lv_subject_t * subject)
{
    if(subject->type != LV_SUBJECT_TYPE_STRING) {
        LV_LOG_WARN("Subject type is not LV_SUBJECT_TYPE_STRING");
        return "";
    }

    return subject->value.pointer;
}

const char * lv_subject_get_previous_string(lv_subject_t * subject)
{
    if(subject->type != LV_SUBJECT_TYPE_STRING) {
        LV_LOG_WARN("Subject type is not LV_SUBJECT_TYPE_STRING");
        return NULL;
    }

    return subject->prev_value.pointer;
}

void lv_subject_init_pointer(lv_subject_t * subject, void * value)
{
    lv_memzero(subject, sizeof(lv_subject_t));
    subject->type = LV_SUBJECT_TYPE_POINTER;
    subject->value.pointer = value;
    subject->prev_value.pointer = value;
    _lv_ll_init(&(subject->subs_ll), sizeof(lv_observer_t));
}

void lv_subject_set_pointer(lv_subject_t * subject, void * ptr)
{
    if(subject->type != LV_SUBJECT_TYPE_POINTER) {
        LV_LOG_WARN("Subject type is not LV_SUBJECT_TYPE_POINTER");
        return;
    }

    subject->prev_value.pointer = subject->value.pointer;
    subject->value.pointer = ptr;
    lv_subject_notify(subject);
}

const void * lv_subject_get_pointer(lv_subject_t * subject)
{
    if(subject->type != LV_SUBJECT_TYPE_POINTER) {
        LV_LOG_WARN("Subject type is not LV_SUBJECT_TYPE_POINTER");
        return NULL;
    }

    return subject->value.pointer;
}

const void * lv_subject_get_previous_pointer(lv_subject_t * subject)
{
    if(subject->type != LV_SUBJECT_TYPE_POINTER) {
        LV_LOG_WARN("Subject type is not LV_SUBJECT_TYPE_POINTER");
        return NULL;
    }

    return subject->prev_value.pointer;
}

void lv_subject_init_color(lv_subject_t * subject, lv_color_t color)
{
    lv_memzero(subject, sizeof(lv_subject_t));
    subject->type = LV_SUBJECT_TYPE_COLOR;
    subject->value.color = color;
    subject->prev_value.color = color;
    _lv_ll_init(&(subject->subs_ll), sizeof(lv_observer_t));
}

void lv_subject_set_color(lv_subject_t * subject, lv_color_t color)
{
    if(subject->type != LV_SUBJECT_TYPE_COLOR) {
        LV_LOG_WARN("Subject type is not LV_SUBJECT_TYPE_COLOR");
        return;
    }

    subject->prev_value.color = subject->value.color;
    subject->value.color = color;
    lv_subject_notify(subject);
}

lv_color_t lv_subject_get_color(lv_subject_t * subject)
{
    if(subject->type != LV_SUBJECT_TYPE_COLOR) {
        LV_LOG_WARN("Subject type is not LV_SUBJECT_TYPE_COLOR");
        return lv_color_black();
    }

    return subject->value.color;
}

lv_color_t lv_subject_get_previous_color(lv_subject_t * subject)
{
    if(subject->type != LV_SUBJECT_TYPE_COLOR) {
        LV_LOG_WARN("Subject type is not LV_SUBJECT_TYPE_COLOR");
        return lv_color_black();
    }

    return subject->prev_value.color;
}

void lv_subject_init_group(lv_subject_t * subject, lv_subject_t * list[], uint32_t list_len)
{
    subject->type = LV_SUBJECT_TYPE_GROUP;
    subject->size = list_len;
    _lv_ll_init(&(subject->subs_ll), sizeof(lv_observer_t));
    subject->value.pointer = list;

    /* bind all subjects to this subject */
    uint32_t i;
    for(i = 0; i < list_len; i++) {
        /*If a subject in the group changes notify the group itself*/
        lv_subject_add_observer(list[i], group_notify_cb, subject);
    }
}

lv_subject_t * lv_subject_get_group_element(lv_subject_t * subject, int32_t index)
{
    if(subject->type != LV_SUBJECT_TYPE_GROUP) {
        LV_LOG_WARN("Subject type is not LV_SUBJECT_TYPE_GROUP");
        return NULL;
    }

    if(index >= subject->size)  return NULL;

    return ((lv_subject_t **)(subject->value.pointer))[index];
}

lv_observer_t * lv_subject_add_observer(lv_subject_t * subject, lv_observer_cb_t cb, void * user_data)
{
    lv_observer_t * observer = lv_subject_add_observer_obj(subject, cb, NULL, user_data);
    return observer;
}

lv_observer_t * lv_subject_add_observer_obj(lv_subject_t * subject, lv_observer_cb_t cb, lv_obj_t * obj,
                                            void * user_data)
{
    LV_ASSERT_NULL(subject);
    if(subject->type == LV_SUBJECT_TYPE_INVALID) {
        LV_LOG_WARN("Subject not initialized yet");
        return NULL;
    }
    lv_observer_t * observer = _lv_ll_ins_tail(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    if(observer == NULL) return NULL;

    lv_memzero(observer, sizeof(*observer));

    observer->subject = subject;
    observer->cb = cb;
    observer->user_data = user_data;
    observer->target = obj;
    /* subscribe to delete event of the object */
    if(obj != NULL) {
        lv_obj_add_event_cb(obj, unsubscribe_on_delete_cb, LV_EVENT_DELETE, observer);
    }

    /* update object immediately */
    if(observer->cb) observer->cb(observer, subject);

    return observer;
}

lv_observer_t * lv_subject_add_observer_with_target(lv_subject_t * subject, lv_observer_cb_t cb, void * target,
                                                    void * user_data)
{
    LV_ASSERT_NULL(subject);
    if(subject->type == LV_SUBJECT_TYPE_INVALID) {
        LV_LOG_WARN("Subject not initialized yet");
        return NULL;
    }
    lv_observer_t * observer = _lv_ll_ins_tail(&(subject->subs_ll));
    LV_ASSERT_MALLOC(observer);
    if(observer == NULL) return NULL;

    lv_memzero(observer, sizeof(*observer));

    observer->subject = subject;
    observer->cb = cb;
    observer->user_data = user_data;
    observer->target = target;

    /* update object immediately */
    if(observer->cb) observer->cb(observer, subject);

    return observer;
}

void lv_observer_remove(lv_observer_t * observer)
{
    LV_ASSERT_NULL(observer);

    observer->subject->notify_restart_query = 1;

    _lv_ll_remove(&(observer->subject->subs_ll), observer);

    if(observer->auto_free_user_data) {
        lv_free(observer->user_data);
    }
    lv_free(observer);
}

void lv_subject_remove_all_obj(lv_subject_t * subject, lv_obj_t * obj)
{
    LV_ASSERT_NULL(subject);
    if(subject->type == LV_SUBJECT_TYPE_INVALID) {
        LV_LOG_WARN("Subject not initialized yet");
        return;
    }
    while(lv_obj_remove_event_cb(obj, unsubscribe_on_delete_cb));
    while(lv_obj_remove_event_cb(obj, btn_value_changed_event_cb));
    while(lv_obj_remove_event_cb(obj, arc_value_changed_event_cb));
#if LV_USE_ROLLER
    while(lv_obj_remove_event_cb(obj, roller_value_changed_event_cb));
#endif

#if LV_USE_DROPDOWN
    while(lv_obj_remove_event_cb(obj, dropdown_value_changed_event_cb));
#endif

    lv_observer_t * observer = _lv_ll_get_head(&subject->subs_ll);
    while(observer) {
        lv_observer_t * observer_next = _lv_ll_get_next(&subject->subs_ll, observer);
        if(observer->target == obj) {
            lv_observer_remove(observer);
        }
        observer = observer_next;
    }
}

void * lv_observer_get_target(lv_observer_t * observer)
{
    LV_ASSERT_NULL(observer);

    return observer->target;
}

void lv_subject_notify(lv_subject_t * subject)
{
    LV_ASSERT_NULL(subject);

    lv_observer_t * observer;
    _LV_LL_READ(&(subject->subs_ll), observer) {
        observer->notified = 0;
    }

    do {
        subject->notify_restart_query = 0;
        _LV_LL_READ(&(subject->subs_ll), observer) {
            if(observer->cb && observer->notified == 0) {
                observer->cb(observer, subject);
                if(subject->notify_restart_query) break;
                observer->notified = 1;
            }
        }
    } while(subject->notify_restart_query);
}

lv_observer_t * lv_obj_bind_flag_if_eq(lv_obj_t * obj, lv_subject_t * subject, lv_obj_flag_t flag, int32_t ref_value)
{
    lv_observer_t * observable = bind_to_bitfield(subject, obj, obj_flag_observer_cb, flag, ref_value, false);
    return observable;
}

lv_observer_t * lv_obj_bind_flag_if_not_eq(lv_obj_t * obj, lv_subject_t * subject, lv_obj_flag_t flag,
                                           int32_t ref_value)
{
    lv_observer_t * observable = bind_to_bitfield(subject, obj, obj_flag_observer_cb, flag, ref_value, true);
    return observable;
}

lv_observer_t * lv_obj_bind_state_if_eq(lv_obj_t * obj, lv_subject_t * subject, lv_state_t state, int32_t ref_value)
{
    lv_observer_t * observable = bind_to_bitfield(subject, obj, obj_state_observer_cb, state, ref_value, false);
    return observable;
}

lv_observer_t * lv_obj_bind_state_if_not_eq(lv_obj_t * obj, lv_subject_t * subject, lv_state_t state, int32_t ref_value)
{
    lv_observer_t * observable = bind_to_bitfield(subject, obj, obj_state_observer_cb, state, ref_value, true);
    return observable;
}

#if LV_USE_BUTTON
lv_observer_t * lv_button_bind_checked(lv_obj_t * obj, lv_subject_t * subject)
{
    lv_observer_t * observable = bind_to_bitfield(subject, obj, obj_state_observer_cb, LV_STATE_CHECKED, 1, false);
    lv_obj_add_event_cb(obj, btn_value_changed_event_cb, LV_EVENT_VALUE_CHANGED, subject);
    return observable;
}
#endif /*LV_USE_BUTTON*/

#if LV_USE_LABEL
lv_observer_t * lv_label_bind_text(lv_obj_t * obj, lv_subject_t * subject, const char * fmt)
{
    if(fmt == NULL) {
        if(subject->type != LV_SUBJECT_TYPE_STRING && subject->type != LV_SUBJECT_TYPE_POINTER) {
            LV_LOG_WARN("Incompatible subject type: %d", subject->type);
            return NULL;
        }
    }
    else {
        if(subject->type != LV_SUBJECT_TYPE_STRING && subject->type != LV_SUBJECT_TYPE_POINTER &&
           subject->type != LV_SUBJECT_TYPE_INT) {
            LV_LOG_WARN("Incompatible subject type: %d", subject->type);
            return NULL;
        }
    }

    lv_observer_t * observer = lv_subject_add_observer_obj(subject, label_text_observer_cb, obj, (void *)fmt);
    return observer;
}
#endif /*LV_USE_LABEL*/

#if LV_USE_ARC
lv_observer_t * lv_arc_bind_value(lv_obj_t * obj, lv_subject_t * subject)
{
    if(subject->type != LV_SUBJECT_TYPE_INT) {
        LV_LOG_WARN("Incompatible subject type: %d", subject->type);
        return NULL;
    }

    lv_obj_add_event_cb(obj, arc_value_changed_event_cb, LV_EVENT_VALUE_CHANGED, subject);

    lv_observer_t * observer = lv_subject_add_observer_obj(subject, arc_value_observer_cb, obj, NULL);
    return observer;
}
#endif /*LV_USE_ARC*/

#if LV_USE_SLIDER
lv_observer_t * lv_slider_bind_value(lv_obj_t * obj, lv_subject_t * subject)
{
    if(subject->type != LV_SUBJECT_TYPE_INT) {
        LV_LOG_WARN("Incompatible subject type: %d", subject->type);
        return NULL;
    }

    lv_obj_add_event_cb(obj, slider_value_changed_event_cb, LV_EVENT_VALUE_CHANGED, subject);

    lv_observer_t * observer = lv_subject_add_observer_obj(subject, slider_value_observer_cb, obj, NULL);
    return observer;
}
#endif /*LV_USE_SLIDER*/

#if LV_USE_ROLLER

lv_observer_t * lv_roller_bind_value(lv_obj_t * obj, lv_subject_t * subject)
{
    if(subject->type != LV_SUBJECT_TYPE_INT) {
        LV_LOG_WARN("Incompatible subject type: %d", subject->type);
        return NULL;
    }

    lv_obj_add_event_cb(obj, roller_value_changed_event_cb, LV_EVENT_VALUE_CHANGED, subject);

    lv_observer_t * observer = lv_subject_add_observer_obj(subject, roller_value_observer_cb, obj, NULL);
    return observer;

}
#endif /*LV_USE_ROLLER*/

#if LV_USE_DROPDOWN

lv_observer_t * lv_dropdown_bind_value(lv_obj_t * obj, lv_subject_t * subject)
{
    if(subject->type != LV_SUBJECT_TYPE_INT) {
        LV_LOG_WARN("Incompatible subject type: %d", subject->type);
        return NULL;
    }

    lv_obj_add_event_cb(obj, dropdown_value_changed_event_cb, LV_EVENT_VALUE_CHANGED, subject);

    lv_observer_t * observer = lv_subject_add_observer_obj(subject, dropdown_value_observer_cb, obj, NULL);
    return observer;

}

#endif /*LV_USE_DROPDOWN*/

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void group_notify_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    LV_UNUSED(subject);
    lv_subject_t * subject_group = observer->user_data;
    lv_subject_notify(subject_group);
}

static void unsubscribe_on_delete_cb(lv_event_t * e)
{
    lv_observer_t * observer = lv_event_get_user_data(e);
    lv_observer_remove(observer);
}

static lv_observer_t * bind_to_bitfield(lv_subject_t * subject, lv_obj_t * obj, lv_observer_cb_t cb, uint32_t flag,
                                        int32_t ref_value, bool inv)
{
    if(subject->type != LV_SUBJECT_TYPE_INT) {
        LV_LOG_WARN("Incompatible subject type: %d", subject->type);
        return NULL;
    }

    flag_and_cond_t * p = lv_malloc(sizeof(flag_and_cond_t));
    if(p == NULL) {
        LV_LOG_WARN("Out of memory");
        return NULL;
    }

    p->flag = flag;
    p->value.num = ref_value;
    p->inv = inv;

    lv_observer_t * observable = lv_subject_add_observer_obj(subject, cb, obj, p);
    observable->auto_free_user_data = 1;
    return observable;
}

static void obj_flag_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    flag_and_cond_t * p = observer->user_data;

    bool res = subject->value.num == p->value.num;
    if(p->inv) res = !res;

    if(res) {
        lv_obj_add_flag(observer->target, p->flag);
    }
    else {
        lv_obj_remove_flag(observer->target, p->flag);
    }
}

static void obj_state_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    flag_and_cond_t * p = observer->user_data;

    bool res = subject->value.num == p->value.num;
    if(p->inv) res = !res;

    if(res) {
        lv_obj_add_state(observer->target, p->flag);
    }
    else {
        lv_obj_remove_state(observer->target, p->flag);
    }
}

#if LV_USE_BUTTON

static void btn_value_changed_event_cb(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_current_target(e);
    lv_subject_t * subject = lv_event_get_user_data(e);

    lv_subject_set_int(subject, lv_obj_has_state(obj, LV_STATE_CHECKED));
}

#endif /*LV_USE_BUTTON*/

#if LV_USE_LABEL

static void label_text_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    const char * fmt = observer->user_data;

    if(fmt == NULL) {
        lv_label_set_text(observer->target, subject->value.pointer);
    }
    else {
        switch(subject->type) {
            case LV_SUBJECT_TYPE_INT:
                lv_label_set_text_fmt(observer->target, fmt, subject->value.num);
                break;
            case LV_SUBJECT_TYPE_STRING:
            case LV_SUBJECT_TYPE_POINTER:
                lv_label_set_text_fmt(observer->target, fmt, subject->value.pointer);
                break;
            default:
                break;
        }
    }
}

#endif /*LV_USE_LABEL*/

#if LV_USE_ARC

static void arc_value_changed_event_cb(lv_event_t * e)
{
    lv_obj_t * arc = lv_event_get_current_target(e);
    lv_subject_t * subject = lv_event_get_user_data(e);

    lv_subject_set_int(subject, lv_arc_get_value(arc));
}

static void arc_value_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_arc_set_value(observer->target, subject->value.num);
}

#endif /*LV_USE_ARC*/

#if LV_USE_SLIDER

static void slider_value_changed_event_cb(lv_event_t * e)
{
    lv_obj_t * slider = lv_event_get_current_target(e);
    lv_subject_t * subject = lv_event_get_user_data(e);

    lv_subject_set_int(subject, lv_slider_get_value(slider));
}

static void slider_value_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_slider_set_value(observer->target, subject->value.num, LV_ANIM_OFF);
}

#endif /*LV_USE_SLIDER*/

#if LV_USE_ROLLER

static void roller_value_changed_event_cb(lv_event_t * e)
{
    lv_obj_t * roller = lv_event_get_current_target(e);
    lv_subject_t * subject = lv_event_get_user_data(e);

    lv_subject_set_int(subject, lv_roller_get_selected(roller));
}

static void roller_value_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    if((int32_t)lv_roller_get_selected(observer->target) != subject->value.num) {
        lv_roller_set_selected(observer->target, subject->value.num, LV_ANIM_OFF);
    }
}

#endif /*LV_USE_ROLLER*/

#if LV_USE_DROPDOWN

static void dropdown_value_changed_event_cb(lv_event_t * e)
{
    lv_obj_t * dropdown = lv_event_get_current_target(e);
    lv_subject_t * subject = lv_event_get_user_data(e);

    lv_subject_set_int(subject, lv_dropdown_get_selected(dropdown));
}

static void dropdown_value_observer_cb(lv_observer_t * observer, lv_subject_t * subject)
{
    lv_dropdown_set_selected(observer->target, subject->value.num);
}

#endif /*LV_USE_DROPDOWN*/

#endif /*LV_USE_OBSERVER*/
