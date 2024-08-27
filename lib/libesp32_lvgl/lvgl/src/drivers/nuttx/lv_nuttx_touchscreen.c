/**
 * @file lv_nuttx_touchscreen.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#include "lv_nuttx_touchscreen.h"

#if LV_USE_NUTTX

#if LV_USE_NUTTX_TOUCHSCREEN

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <debug.h>
#include <errno.h>
#include <fcntl.h>
#include <nuttx/input/touchscreen.h>
#include "../../lvgl_private.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

typedef struct {
    /* fd should be defined at the beginning */
    int fd;
    struct touch_sample_s last_sample;
    bool has_last_sample;
    lv_indev_state_t last_state;
    lv_indev_t * indev_drv;
} lv_nuttx_touchscreen_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void touchscreen_read(lv_indev_t * drv, lv_indev_data_t * data);
static void touchscreen_delete_cb(lv_event_t * e);
static lv_indev_t * touchscreen_init(int fd);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_indev_t * lv_nuttx_touchscreen_create(const char * dev_path)
{
    lv_indev_t * indev;
    int fd;

    LV_ASSERT_NULL(dev_path);
    LV_LOG_USER("touchscreen %s opening", dev_path);
    fd = open(dev_path, O_RDONLY | O_NONBLOCK);
    if(fd < 0) {
        perror("Error: cannot open touchscreen device");
        return NULL;
    }

    LV_LOG_USER("touchscreen %s open success", dev_path);

    indev = touchscreen_init(fd);

    if(indev == NULL) {
        close(fd);
    }

    return indev;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void conv_touch_sample(lv_indev_t * drv,
                              lv_indev_data_t * data,
                              struct touch_sample_s * sample)
{
    lv_nuttx_touchscreen_t * touchscreen = drv->driver_data;
    uint8_t touch_flags = sample->point[0].flags;

    if(touch_flags & (TOUCH_DOWN | TOUCH_MOVE)) {
        lv_display_t * disp = lv_indev_get_display(drv);
        int32_t hor_max = lv_display_get_horizontal_resolution(disp) - 1;
        int32_t ver_max = lv_display_get_vertical_resolution(disp) - 1;

        data->point.x = LV_CLAMP(0, sample->point[0].x, hor_max);
        data->point.y = LV_CLAMP(0, sample->point[0].y, ver_max);
        touchscreen->last_state = LV_INDEV_STATE_PRESSED;
    }
    else if(touch_flags & TOUCH_UP) {
        touchscreen->last_state = LV_INDEV_STATE_RELEASED;
    }
}

static bool touchscreen_read_sample(int fd, struct touch_sample_s * sample)
{
    int nbytes = read(fd, sample, sizeof(struct touch_sample_s));
    return nbytes == sizeof(struct touch_sample_s);
}

static void touchscreen_read(lv_indev_t * drv, lv_indev_data_t * data)
{
    lv_nuttx_touchscreen_t * touchscreen = drv->driver_data;
    struct touch_sample_s sample;

    /*
     * Note: Since it is necessary to avoid multi-processing click events
     * caused by redundant continue_reading, a two-unit sample sliding window
     * algorithm is used here. continue_reading is only activated when there
     * are two points in the window.
     */

    /* If has last sample, use it first */
    if(touchscreen->has_last_sample) {
        conv_touch_sample(drv, data, &touchscreen->last_sample);
    }
    else {
        /* Read first sample */
        if(!touchscreen_read_sample(touchscreen->fd, &sample)) {
            /* No sample available, return last state */
            data->state = touchscreen->last_state;
            return;
        }

        conv_touch_sample(drv, data, &sample);
    }

    /* Try to read next sample */
    if(touchscreen_read_sample(touchscreen->fd, &sample)) {
        /* Save last sample and let lvgl continue reading */
        touchscreen->last_sample = sample;
        touchscreen->has_last_sample = true;
        data->continue_reading = true;
    }
    else {
        /* No more sample available, clear last sample flag */
        touchscreen->has_last_sample = false;
    }

    data->state = touchscreen->last_state;
}

static void touchscreen_delete_cb(lv_event_t * e)
{
    lv_indev_t * indev = (lv_indev_t *) lv_event_get_user_data(e);
    lv_nuttx_touchscreen_t * touchscreen = lv_indev_get_driver_data(indev);
    if(touchscreen) {
        lv_indev_set_driver_data(indev, NULL);
        lv_indev_set_read_cb(indev, NULL);

        if(touchscreen->fd >= 0) {
            close(touchscreen->fd);
            touchscreen->fd = -1;
        }
        lv_free(touchscreen);
        LV_LOG_USER("done");
    }
}

static lv_indev_t * touchscreen_init(int fd)
{
    lv_nuttx_touchscreen_t * touchscreen;
    lv_indev_t * indev = NULL;

    touchscreen = lv_malloc_zeroed(sizeof(lv_nuttx_touchscreen_t));
    if(touchscreen == NULL) {
        LV_LOG_ERROR("touchscreen_s malloc failed");
        return NULL;
    }

    touchscreen->fd = fd;
    touchscreen->last_state = LV_INDEV_STATE_RELEASED;
    touchscreen->indev_drv = indev = lv_indev_create();

    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, touchscreen_read);
    lv_indev_set_driver_data(indev, touchscreen);
    lv_indev_add_event_cb(indev, touchscreen_delete_cb, LV_EVENT_DELETE, indev);
    return indev;
}

#endif /*LV_USE_NUTTX_TOUCHSCREEN*/

#endif /* LV_USE_NUTTX*/
