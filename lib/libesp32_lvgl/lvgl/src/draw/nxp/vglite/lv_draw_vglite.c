/**
 * @file lv_draw_vglite.c
 *
 */

/**
 * Copyright 2023-2024 NXP
 *
 * SPDX-License-Identifier: MIT
 */

/*********************
 *      INCLUDES
 *********************/

#include "lv_draw_vglite.h"

#if LV_USE_DRAW_VGLITE
#include "lv_vglite_buf.h"
#include "lv_vglite_utils.h"

#include "../../../core/lv_global.h"

/*********************
 *      DEFINES
 *********************/

#define DRAW_UNIT_ID_VGLITE 2

#if LV_USE_VGLITE_DRAW_ASYNC
    #define VGLITE_TASK_BUF_SIZE 100
#endif

/**********************
 *      TYPEDEFS
 **********************/

#if LV_USE_VGLITE_DRAW_ASYNC
/**
 * Structure of pending vglite draw task
 */
typedef struct _vglite_flush_task {
    vglite_draw_task_t * task;
    bool flushed;
} vglite_flush_task_t;
#endif

/**********************
 *  STATIC PROTOTYPES
 **********************/

static inline void _vglite_cleanup_task(vglite_draw_task_t * task);

/*
 * Evaluate a task and set the score and preferred VGLite draw unit.
 * Return 1 if task is preferred, 0 otherwise (task is not supported).
 */
static int32_t _vglite_evaluate(lv_draw_unit_t * draw_unit, lv_draw_task_t * task);

/*
 * Dispatch (assign) a task to VGLite draw unit (itself).
 * Return 1 if task was dispatched, 0 otherwise (task not supported).
 */
static int32_t _vglite_dispatch(lv_draw_unit_t * draw_unit, lv_layer_t * layer);

/*
 * Wait for VG-Lite draw unit to finish.
 */
#if LV_USE_VGLITE_DRAW_ASYNC
    static int32_t _vglite_wait_for_finish(lv_draw_unit_t * draw_unit);
#endif

/*
 * Delete the VGLite draw unit.
 */
static int32_t _vglite_delete(lv_draw_unit_t * draw_unit);

#if LV_USE_VGLITE_DRAW_THREAD
    static void _vglite_render_thread_cb(void * ptr);
#endif

static void _vglite_execute_drawing(lv_draw_vglite_unit_t * u);

/**********************
 *  STATIC VARIABLES
 **********************/

#define _draw_info LV_GLOBAL_DEFAULT()->draw_info

#if LV_USE_VGLITE_DRAW_ASYNC
    /*
    * Circular buffer to hold the queued and the flushed tasks.
    * Two indexes, _head and _tail, are used to signal the beginning
    * and the end of the valid tasks that are pending.
    */
    static vglite_flush_task_t _draw_task_buf[VGLITE_TASK_BUF_SIZE];
    static volatile int _head = 0;
    static volatile int _tail = 0;
#endif

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_draw_vglite_init(void)
{
    lv_draw_buf_vglite_init_handlers();

    lv_draw_vglite_unit_t * draw_vglite_unit = lv_draw_create_unit(sizeof(lv_draw_vglite_unit_t));
    draw_vglite_unit->base_unit.evaluate_cb = _vglite_evaluate;
    draw_vglite_unit->base_unit.dispatch_cb = _vglite_dispatch;
#if LV_USE_VGLITE_DRAW_ASYNC
    draw_vglite_unit->base_unit.wait_for_finish_cb = _vglite_wait_for_finish;
#endif
    draw_vglite_unit->base_unit.delete_cb = _vglite_delete;
    draw_vglite_unit->base_unit.name = "NXP_VGLITE";

#if LV_USE_VGLITE_DRAW_THREAD
    lv_thread_init(&draw_vglite_unit->thread, "vglitedraw", LV_DRAW_THREAD_PRIO, _vglite_render_thread_cb, 4 * 1024,
                   draw_vglite_unit);
#endif
}

void lv_draw_vglite_deinit(void)
{
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static inline bool _vglite_src_cf_supported(lv_color_format_t cf)
{
    bool is_cf_supported = false;

    switch(cf) {
#if CHIPID == 0x255 || CHIPID == 0x555
        case LV_COLOR_FORMAT_I1:
        case LV_COLOR_FORMAT_I2:
        case LV_COLOR_FORMAT_I4:
        case LV_COLOR_FORMAT_I8:
#endif
        case LV_COLOR_FORMAT_A4:
        case LV_COLOR_FORMAT_A8:
        case LV_COLOR_FORMAT_L8:
        case LV_COLOR_FORMAT_RGB565:
#if CHIPID == 0x555
        case LV_COLOR_FORMAT_ARGB8565:
        case LV_COLOR_FORMAT_RGB888:
#endif
        case LV_COLOR_FORMAT_ARGB8888:
        case LV_COLOR_FORMAT_XRGB8888:
            is_cf_supported = true;
            break;
        default:
            break;
    }

    return is_cf_supported;
}

static inline bool _vglite_dest_cf_supported(lv_color_format_t cf)
{
    bool is_cf_supported = false;

    switch(cf) {
        case LV_COLOR_FORMAT_A8:
#if CHIPID == 0x255 || CHIPID == 0x555
        case LV_COLOR_FORMAT_L8:
#endif
        case LV_COLOR_FORMAT_RGB565:
#if CHIPID == 0x555
        case LV_COLOR_FORMAT_ARGB8565:
        case LV_COLOR_FORMAT_RGB888:
#endif
        case LV_COLOR_FORMAT_ARGB8888:
        case LV_COLOR_FORMAT_XRGB8888:
            is_cf_supported = true;
            break;
        default:
            break;
    }

    return is_cf_supported;
}

static int32_t _vglite_evaluate(lv_draw_unit_t * u, lv_draw_task_t * t)
{
    LV_UNUSED(u);

    const lv_draw_dsc_base_t * draw_dsc_base = (lv_draw_dsc_base_t *) t->draw_dsc;

    if(!_vglite_dest_cf_supported(draw_dsc_base->layer->color_format))
        return 0;

    switch(t->type) {
        case LV_DRAW_TASK_TYPE_FILL:
            if(t->preference_score > 80) {
                t->preference_score = 80;
                t->preferred_draw_unit_id = DRAW_UNIT_ID_VGLITE;
            }
            return 1;

        case LV_DRAW_TASK_TYPE_LINE:
        case LV_DRAW_TASK_TYPE_ARC:
        case LV_DRAW_TASK_TYPE_TRIANGLE:
            if(t->preference_score > 90) {
                t->preference_score = 90;
                t->preferred_draw_unit_id = DRAW_UNIT_ID_VGLITE;
            }
            return 1;

        case LV_DRAW_TASK_TYPE_LABEL:
            if(t->preference_score > 95) {
                t->preference_score = 95;
                t->preferred_draw_unit_id = DRAW_UNIT_ID_VGLITE;
            }
            return 1;

        case LV_DRAW_TASK_TYPE_BORDER: {
                if(t->preference_score > 90) {
                    t->preference_score = 90;
                    t->preferred_draw_unit_id = DRAW_UNIT_ID_VGLITE;
                }
                return 1;
            }

        case LV_DRAW_TASK_TYPE_LAYER: {
                const lv_draw_image_dsc_t * draw_dsc = (lv_draw_image_dsc_t *) t->draw_dsc;
                lv_layer_t * layer_to_draw = (lv_layer_t *)draw_dsc->src;

#if LV_USE_VGLITE_BLIT_SPLIT
                bool has_transform = (draw_dsc->rotation != 0 || draw_dsc->scale_x != LV_SCALE_NONE ||
                                      draw_dsc->scale_y != LV_SCALE_NONE);
#endif
                if(!_vglite_src_cf_supported(layer_to_draw->color_format)
#if LV_USE_VGLITE_BLIT_SPLIT
                   || has_transform
#endif
                  )
                    return 0;

                if(t->preference_score > 80) {
                    t->preference_score = 80;
                    t->preferred_draw_unit_id = DRAW_UNIT_ID_VGLITE;
                }
                return 1;
            }

        case LV_DRAW_TASK_TYPE_IMAGE: {
                lv_draw_image_dsc_t * draw_dsc = (lv_draw_image_dsc_t *) t->draw_dsc;
                const lv_image_dsc_t * img_dsc = draw_dsc->src;

                if(img_dsc->header.cf >= LV_COLOR_FORMAT_PROPRIETARY_START) {
                    return 0;
                }

#if CHIPID == 0x255
                if(draw_dsc->tile)
                    return 0;
#endif
#if LV_USE_VGLITE_BLIT_SPLIT
                bool has_transform = (draw_dsc->rotation != 0 || draw_dsc->scale_x != LV_SCALE_NONE ||
                                      draw_dsc->scale_y != LV_SCALE_NONE);
                bool is_tiled = draw_dsc->tile;
#endif

                if((!_vglite_src_cf_supported(img_dsc->header.cf))
#if LV_USE_VGLITE_BLIT_SPLIT
                   || has_transform || is_tiled
#endif
                   || (!vglite_src_buf_aligned(img_dsc->data, img_dsc->header.stride, img_dsc->header.cf))
                  )
                    return 0;

                if(t->preference_score > 80) {
                    t->preference_score = 80;
                    t->preferred_draw_unit_id = DRAW_UNIT_ID_VGLITE;
                }
                return 1;
            }
        default:
            return 0;
    }

    return 0;
}

static int32_t _vglite_dispatch(lv_draw_unit_t * draw_unit, lv_layer_t * layer)
{
    lv_draw_vglite_unit_t * draw_vglite_unit = (lv_draw_vglite_unit_t *) draw_unit;

    /* Return immediately if draw unit is busy. */
    if(draw_vglite_unit->task_act
#if LV_USE_VGLITE_DRAW_ASYNC
       || draw_vglite_unit->wait_for_finish
#endif
      )
        return 0;

    /* Try to get an ready to draw. */
    lv_draw_task_t * t = lv_draw_get_available_task(layer, NULL, DRAW_UNIT_ID_VGLITE);

    if(t == NULL)
        return LV_DRAW_UNIT_IDLE;

    if(t->preferred_draw_unit_id != DRAW_UNIT_ID_VGLITE) {
        /* Let the preferred known unit to draw this task. */
        if(t->preferred_draw_unit_id != LV_DRAW_UNIT_NONE) {
            return LV_DRAW_UNIT_IDLE;
        }
        else {
            /* Fake unsupported tasks as ready. */
            t->state = LV_DRAW_TASK_STATE_READY;
            /* Request a new dispatching as it can get a new task. */
            lv_draw_dispatch_request();

            return 1;
        }
    }
    vglite_draw_task_t * vglite_task = lv_malloc_zeroed(sizeof(vglite_draw_task_t));
    LV_ASSERT_MALLOC(vglite_task);

    vglite_task->t = t;

    if(lv_draw_layer_alloc_buf(layer) == NULL)
        return LV_DRAW_UNIT_IDLE;
    t->state = LV_DRAW_TASK_STATE_IN_PROGRESS;
    draw_vglite_unit->task_act = vglite_task;

#if LV_USE_VGLITE_DRAW_THREAD
    /* Let the render thread work. */
    if(draw_vglite_unit->inited)
        lv_thread_sync_signal(&draw_vglite_unit->sync);
#else
    _vglite_execute_drawing(draw_vglite_unit);

    draw_vglite_unit->task_act->t->state = LV_DRAW_TASK_STATE_READY;
    _vglite_cleanup_task(draw_vglite_unit->task_act);
    draw_vglite_unit->task_act = NULL;

    /* The draw unit is free now. Request a new dispatching as it can get a new task. */
    lv_draw_dispatch_request();
#endif

    return 1;
}

#if LV_USE_VGLITE_DRAW_ASYNC
static int32_t _vglite_wait_for_finish(lv_draw_unit_t * draw_unit)
{
    lv_draw_vglite_unit_t * draw_vglite_unit = (lv_draw_vglite_unit_t *) draw_unit;
    draw_vglite_unit->wait_for_finish = true;

    /* Signal draw unit to finish its tasks and return READY state after completion. */
    if(draw_vglite_unit->inited)
        lv_thread_sync_signal(&draw_vglite_unit->sync);

    /* Wait for finish now. */
    lv_draw_dispatch_wait_for_request();

    return 1;
}
#endif

static int32_t _vglite_delete(lv_draw_unit_t * draw_unit)
{
#if LV_USE_VGLITE_DRAW_THREAD
    lv_draw_vglite_unit_t * draw_vglite_unit = (lv_draw_vglite_unit_t *) draw_unit;

    LV_LOG_INFO("Cancel VGLite draw thread.");
    draw_vglite_unit->exit_status = true;

    if(draw_vglite_unit->inited)
        lv_thread_sync_signal(&draw_vglite_unit->sync);

    lv_result_t res = lv_thread_delete(&draw_vglite_unit->thread);

    return res;
#else
    LV_UNUSED(draw_unit);

    return 1;
#endif
}

static void _vglite_execute_drawing(lv_draw_vglite_unit_t * u)
{
    vglite_draw_task_t * vglite_task = u->task_act;
    lv_layer_t * layer = vglite_task->t->target_layer;
    lv_draw_buf_t * draw_buf = layer->draw_buf;

    /* Set target buffer */
    vglite_set_dest_buf(draw_buf->data, draw_buf->header.w, draw_buf->header.h, draw_buf->header.stride,
                        draw_buf->header.cf);

    lv_area_t clip_area;
    lv_area_copy(&clip_area, &vglite_task->t->clip_area);
    lv_area_move(&clip_area, -layer->buf_area.x1, -layer->buf_area.y1);

    lv_area_t draw_area;
    lv_area_copy(&draw_area, &vglite_task->t->area);
    lv_area_move(&draw_area, -layer->buf_area.x1, -layer->buf_area.y1);

    if(!lv_area_intersect(&draw_area, &draw_area, &clip_area))
        return; /*Fully clipped, nothing to do*/

    if(_draw_info.unit_cnt > 1)
        lv_draw_buf_invalidate_cache(draw_buf, &draw_area);

#if LV_USE_PARALLEL_DRAW_DEBUG
    /* remember draw unit for debug purposes */
    vglite_task->t->draw_unit = &u->base_unit;
#endif

    /* Set scissor area, excluding the split blit case */
#if LV_USE_VGLITE_BLIT_SPLIT
    if(vglite_task->t->type != LV_DRAW_TASK_TYPE_IMAGE || vglite_task->t->type != LV_DRAW_TASK_TYPE_LAYER)
#endif
        vglite_set_scissor(&clip_area);

    switch(vglite_task->t->type) {
        case LV_DRAW_TASK_TYPE_LABEL:
            lv_draw_vglite_label(vglite_task);
            break;
        case LV_DRAW_TASK_TYPE_FILL:
            lv_draw_vglite_fill(vglite_task);
            break;
        case LV_DRAW_TASK_TYPE_BORDER:
            lv_draw_vglite_border(vglite_task);
            break;
        case LV_DRAW_TASK_TYPE_IMAGE:
            lv_draw_vglite_img(vglite_task);
            break;
        case LV_DRAW_TASK_TYPE_ARC:
            lv_draw_vglite_arc(vglite_task);
            break;
        case LV_DRAW_TASK_TYPE_LINE:
            lv_draw_vglite_line(vglite_task);
            break;
        case LV_DRAW_TASK_TYPE_LAYER:
            lv_draw_vglite_layer(vglite_task);
            break;
        case LV_DRAW_TASK_TYPE_TRIANGLE:
            lv_draw_vglite_triangle(vglite_task);
            break;
        default:
            break;
    }

    /* Disable scissor */
    vglite_set_scissor(&layer->buf_area);

#if LV_USE_PARALLEL_DRAW_DEBUG
    /*Layers manage it for themselves*/
    if(vglite_task->t->type != LV_DRAW_TASK_TYPE_LAYER) {
        lv_area_t draw_area;
        if(!lv_area_intersect(&draw_area, &vglite_task->t->area, &vglite_task->t->clip_area))
            return;

        int32_t idx = u->base_unit.idx;

        lv_draw_rect_dsc_t rect_dsc;
        lv_draw_rect_dsc_init(&rect_dsc);
        rect_dsc.bg_color = lv_palette_main(idx % LV_PALETTE_LAST);
        rect_dsc.border_color = rect_dsc.bg_color;
        rect_dsc.bg_opa = LV_OPA_10;
        rect_dsc.border_opa = LV_OPA_80;
        rect_dsc.border_width = 1;
        lv_draw_sw_fill(vglite_task->t, &rect_dsc, &draw_area);

        lv_point_t txt_size;
        lv_text_get_size(&txt_size, "W", LV_FONT_DEFAULT, 0, 0, 100, LV_TEXT_FLAG_NONE);

        lv_area_t txt_area;
        txt_area.x1 = draw_area.x1;
        txt_area.y1 = draw_area.y1;
        txt_area.x2 = draw_area.x1 + txt_size.x - 1;
        txt_area.y2 = draw_area.y1 + txt_size.y - 1;

        lv_draw_rect_dsc_init(&rect_dsc);
        rect_dsc.bg_color = lv_color_white();
        lv_draw_sw_fill(vglite_task->t, &rect_dsc, &txt_area);

        char buf[8];
        lv_snprintf(buf, sizeof(buf), "%d", idx);
        lv_draw_label_dsc_t label_dsc;
        lv_draw_label_dsc_init(&label_dsc);
        label_dsc.color = lv_color_black();
        label_dsc.text = buf;
        lv_draw_sw_label(vglite_task->t, &label_dsc, &txt_area);
    }
#endif
}

static inline void _vglite_cleanup_task(vglite_draw_task_t * task)
{
    if(task->path != NULL) {
        VGLITE_CHECK_ERROR(vg_lite_clear_path(task->path));
        lv_free(task->path);
    }
    if(task->gradient != NULL) {
        VGLITE_CHECK_ERROR(vg_lite_clear_grad(task->gradient));
        lv_free(task->gradient);
    }
    if(task->path_data != NULL)
        lv_free(task->path_data);

    lv_free(task);
}

#if LV_USE_VGLITE_DRAW_ASYNC
static inline void _vglite_queue_task(vglite_draw_task_t * task)
{
    VGLITE_ASSERT_MSG(((_tail + 1) % VGLITE_TASK_BUF_SIZE) != _head, "VGLite task buffer full.");

    _draw_task_buf[_tail].task = task;
    _draw_task_buf[_tail].flushed = false;
    _tail = (_tail + 1) % VGLITE_TASK_BUF_SIZE;
}

static inline void _vglite_signal_task_ready(vglite_draw_task_t * task)
{
    /* Signal the ready state to dispatcher. */
    task->t->state = LV_DRAW_TASK_STATE_READY;
    _head = (_head + 1) % VGLITE_TASK_BUF_SIZE;

    _vglite_cleanup_task(task);
    /* No need to cleanup the tasks in buffer as we advance with the _head. */
}

static inline void _vglite_signal_all_task_ready(void)
{
    int end = (_head <= _tail) ? _tail : _tail + VGLITE_TASK_BUF_SIZE;

    for(int i = _head; i < end; i++) {
        vglite_draw_task_t * task = _draw_task_buf[i % VGLITE_TASK_BUF_SIZE].task;

        _vglite_signal_task_ready(task);
    }
}

static inline void _vglite_signal_flushed_task_ready(void)
{
    if(vglite_cmd_buf_is_flushed()) {
        int end = (_head <= _tail) ? _tail : _tail + VGLITE_TASK_BUF_SIZE;

        for(int i = _head; i < end; i++) {
            if(_draw_task_buf[i % VGLITE_TASK_BUF_SIZE].flushed) {
                vglite_draw_task_t * task = _draw_task_buf[i % VGLITE_TASK_BUF_SIZE].task;

                _vglite_signal_task_ready(task);

            }
            else {
                /* Those tasks have been flushed now. */
                _draw_task_buf[i % VGLITE_TASK_BUF_SIZE].flushed = true;
            }
        }
    }
}
#endif

#if LV_USE_VGLITE_DRAW_THREAD
static void _vglite_render_thread_cb(void * ptr)
{
    lv_draw_vglite_unit_t * u = ptr;

    lv_thread_sync_init(&u->sync);
    u->inited = true;

    while(1) {
        /* Wait for sync if there is no task set. */
        while(u->task_act == NULL
#if LV_USE_VGLITE_DRAW_ASYNC
              /*
               * Wait for sync if wait_for_finish is triggered.
               * The thread will have to run and mark as complete any pending tasks.
               */
              && !u->wait_for_finish
#endif
             ) {
            if(u->exit_status)
                break;

            lv_thread_sync_wait(&u->sync);
        }

        if(u->exit_status) {
            LV_LOG_INFO("Ready to exit VGLite draw thread.");
            break;
        }

        if(u->task_act) {
#if LV_USE_VGLITE_DRAW_ASYNC
            _vglite_queue_task((void *)u->task_act);
#endif
            _vglite_execute_drawing(u);
        }
#if LV_USE_VGLITE_DRAW_ASYNC
        if(u->wait_for_finish) {
            vglite_wait_for_finish();
            _vglite_signal_all_task_ready();
        }
        else {   /* u->task_act */
            _vglite_signal_flushed_task_ready();
        }
#else
        /* Signal the ready state to dispatcher. */
        u->task_act->t->state = LV_DRAW_TASK_STATE_READY;
        _vglite_cleanup_task(u->task_act);
#endif

        /* Cleanup draw unit running condition. */
#if LV_USE_VGLITE_DRAW_ASYNC
        if(u->wait_for_finish)
            u->wait_for_finish = false;
        else
            u->task_act = NULL;
#else
        u->task_act = NULL;
#endif

        /* The draw unit is free now. Request a new dispatching as it can get a new task. */
        lv_draw_dispatch_request();
    }

    u->inited = false;
    lv_thread_sync_delete(&u->sync);
    LV_LOG_INFO("Exit VGLite draw thread.");
}
#endif

#endif /*LV_USE_DRAW_VGLITE*/
