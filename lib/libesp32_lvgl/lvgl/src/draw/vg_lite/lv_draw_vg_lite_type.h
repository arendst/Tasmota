/**
 * @file lv_draw_vg_lite_type.h
 *
 */

#ifndef LV_DRAW_VG_LITE_TYPE_H
#define LV_DRAW_VG_LITE_TYPE_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#include "../../lv_conf_internal.h"

#if LV_USE_DRAW_VG_LITE

#include "../lv_draw.h"

#if LV_USE_VG_LITE_THORVG
#include "../../others/vg_lite_tvg/vg_lite.h"
#else
#include <vg_lite.h>
#endif

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

struct _lv_draw_vg_lite_unit_t {
    lv_draw_unit_t base_unit;
    lv_draw_task_t * task_act;
    vg_lite_buffer_t target_buffer;
    vg_lite_matrix_t global_matrix;
    struct _lv_vg_lite_path_t * global_path;
    bool path_in_use;
};

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *      MACROS
 **********************/

#endif /*LV_USE_DRAW_VG_LITE*/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_VG_LITE_DRAW_H*/
