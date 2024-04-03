/**
 * @file lv_vg_lite_draw.h
 *
 */

#ifndef LV_VG_LITE_DRAW_H
#define LV_VG_LITE_DRAW_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#include "../../lv_conf_internal.h"

#if LV_USE_DRAW_VG_LITE

#include "../lv_draw.h"
#include "../../draw/lv_draw_vector.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

void lv_draw_buf_vg_lite_init_handlers(void);

void lv_draw_vg_lite_init(void);

void lv_draw_vg_lite_deinit(void);

void lv_draw_vg_lite_arc(lv_draw_unit_t * draw_unit, const lv_draw_arc_dsc_t * dsc,
                         const lv_area_t * coords);

void lv_draw_vg_lite_box_shadow(lv_draw_unit_t * draw_unit, const lv_draw_box_shadow_dsc_t * dsc,
                                const lv_area_t * coords);

void lv_draw_vg_lite_border(lv_draw_unit_t * draw_unit, const lv_draw_border_dsc_t * dsc,
                            const lv_area_t * coords);

void lv_draw_vg_lite_fill(lv_draw_unit_t * draw_unit, const lv_draw_fill_dsc_t * dsc,
                          const lv_area_t * coords);

void lv_draw_vg_lite_img(lv_draw_unit_t * draw_unit, const lv_draw_image_dsc_t * dsc,
                         const lv_area_t * coords, bool no_cache);

void lv_draw_vg_lite_label(lv_draw_unit_t * draw_unit, const lv_draw_label_dsc_t * dsc,
                           const lv_area_t * coords);

void lv_draw_vg_lite_layer(lv_draw_unit_t * draw_unit, const lv_draw_image_dsc_t * draw_dsc,
                           const lv_area_t * coords);

void lv_draw_vg_lite_line(lv_draw_unit_t * draw_unit, const lv_draw_line_dsc_t * dsc);

void lv_draw_vg_lite_triangle(lv_draw_unit_t * draw_unit, const lv_draw_triangle_dsc_t * dsc);

void lv_draw_vg_lite_mask_rect(lv_draw_unit_t * draw_unit, const lv_draw_mask_rect_dsc_t * dsc,
                               const lv_area_t * coords);

#if LV_USE_VECTOR_GRAPHIC
void lv_draw_vg_lite_vector(lv_draw_unit_t * draw_unit, const lv_draw_vector_task_dsc_t * dsc);
#endif

/**********************
 *      MACROS
 **********************/

#endif /*LV_USE_DRAW_VG_LITE*/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_VG_LITE_DRAW_H*/
