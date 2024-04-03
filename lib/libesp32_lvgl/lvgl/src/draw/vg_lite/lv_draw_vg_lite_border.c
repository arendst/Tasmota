/**
 * @file lv_draw_vg_lite_border.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#include "lv_draw_vg_lite.h"

#if LV_USE_DRAW_VG_LITE

#include "lv_draw_vg_lite_type.h"
#include "lv_vg_lite_utils.h"
#include "lv_vg_lite_path.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_draw_vg_lite_border(lv_draw_unit_t * draw_unit, const lv_draw_border_dsc_t * dsc,
                            const lv_area_t * coords)
{
    if(dsc->opa <= LV_OPA_MIN)
        return;
    if(dsc->width == 0)
        return;
    if(dsc->side == LV_BORDER_SIDE_NONE)
        return;

    lv_draw_vg_lite_unit_t * u = (lv_draw_vg_lite_unit_t *)draw_unit;

    lv_area_t clip_area;
    if(!_lv_area_intersect(&clip_area, coords, draw_unit->clip_area)) {
        /*Fully clipped, nothing to do*/
        return;
    }

    int32_t w = lv_area_get_width(coords);
    int32_t h = lv_area_get_height(coords);
    int32_t r_out = dsc->radius;
    if(r_out) {
        int32_t r_short = LV_MIN(w, h) / 2;
        r_out = LV_MIN(r_out, r_short);
    }

    int32_t border_w = dsc->width;
    int32_t r_in = LV_MAX(0, r_out - border_w);

    lv_vg_lite_path_t * path = lv_vg_lite_path_get(u, VG_LITE_S16);
    lv_vg_lite_path_set_quality(path, dsc->radius == 0 ? VG_LITE_LOW : VG_LITE_HIGH);
    lv_vg_lite_path_set_bonding_box_area(path, &clip_area);

    /* outer rect */
    lv_vg_lite_path_append_rect(path,
                                coords->x1, coords->y1,
                                w, h,
                                r_out, r_out);

    /* inner rect */
    lv_vg_lite_path_append_rect(path,
                                coords->x1 + border_w, coords->y1 + border_w,
                                w - border_w * 2, h - border_w * 2,
                                r_in, r_in);

    lv_vg_lite_path_end(path);

    vg_lite_matrix_t matrix;
    vg_lite_identity(&matrix);
    lv_vg_lite_matrix_multiply(&matrix, &u->global_matrix);

    vg_lite_color_t color = lv_vg_lite_color(dsc->color, dsc->opa, true);

    vg_lite_path_t * vg_lite_path = lv_vg_lite_path_get_path(path);

    LV_VG_LITE_ASSERT_DEST_BUFFER(&u->target_buffer);
    LV_VG_LITE_ASSERT_PATH(vg_lite_path);

    LV_VG_LITE_CHECK_ERROR(vg_lite_draw(
                               &u->target_buffer,
                               vg_lite_path,
                               VG_LITE_FILL_EVEN_ODD,
                               &matrix,
                               VG_LITE_BLEND_SRC_OVER,
                               color));

    lv_vg_lite_path_drop(u, path);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

#endif /*LV_USE_DRAW_VG_LITE*/
