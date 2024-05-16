/**
 * @file lv_draw_vg_lite_vector.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#include "lv_draw_vg_lite.h"

#if LV_USE_DRAW_VG_LITE && LV_USE_VECTOR_GRAPHIC

#include "lv_draw_vg_lite_type.h"
#include "lv_vg_lite_path.h"
#include "lv_vg_lite_pending.h"
#include "lv_vg_lite_utils.h"
#include "lv_vg_lite_grad.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void task_draw_cb(void * ctx, const lv_vector_path_t * path, const lv_vector_draw_dsc_t * dsc);
static void lv_matrix_to_vg(vg_lite_matrix_t * desy, const lv_matrix_t * src);
static void lv_path_to_vg(lv_vg_lite_path_t * dest, const lv_vector_path_t * src);
static void lv_path_opa_to_vg(lv_vg_lite_path_t * dest, const lv_vector_draw_dsc_t * dsc);
static void lv_stroke_to_vg(lv_vg_lite_path_t * dest, const lv_vector_stroke_dsc_t * dsc);
static vg_lite_blend_t lv_blend_to_vg(lv_vector_blend_t blend);
static vg_lite_fill_t lv_fill_to_vg(lv_vector_fill_t fill_rule);
static vg_lite_gradient_spreadmode_t lv_spread_to_vg(lv_vector_gradient_spread_t spread);
static vg_lite_cap_style_t lv_stroke_cap_to_vg(lv_vector_stroke_cap_t cap);
static vg_lite_join_style_t lv_stroke_join_to_vg(lv_vector_stroke_join_t join);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_draw_vg_lite_vector(lv_draw_unit_t * draw_unit, const lv_draw_vector_task_dsc_t * dsc)
{
    if(dsc->task_list == NULL)
        return;

    lv_layer_t * layer = dsc->base.layer;
    if(layer->draw_buf == NULL)
        return;

    LV_PROFILER_BEGIN;
    _lv_vector_for_each_destroy_tasks(dsc->task_list, task_draw_cb, draw_unit);
    LV_PROFILER_END;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static vg_lite_color_t lv_color32_to_vg(lv_color32_t color, lv_opa_t opa)
{
    uint8_t a = LV_OPA_MIX2(color.alpha, opa);
    if(a < LV_OPA_MAX) {
        color.red = LV_UDIV255(color.red * opa);
        color.green = LV_UDIV255(color.green * opa);
        color.blue = LV_UDIV255(color.blue * opa);
    }
    return (uint32_t)a << 24 | (uint32_t)color.blue << 16 | (uint32_t)color.green << 8 | color.red;
}

static void task_draw_cb(void * ctx, const lv_vector_path_t * path, const lv_vector_draw_dsc_t * dsc)
{
    LV_PROFILER_BEGIN;
    lv_draw_vg_lite_unit_t * u = ctx;
    LV_VG_LITE_ASSERT_DEST_BUFFER(&u->target_buffer);

    /* clear area */
    if(!path) {
        /* clear color needs to ignore fill_dsc.opa */
        vg_lite_color_t c = lv_color32_to_vg(dsc->fill_dsc.color, LV_OPA_COVER);
        vg_lite_rectangle_t rect;
        lv_vg_lite_rect(&rect, &dsc->scissor_area);
        LV_PROFILER_BEGIN_TAG("vg_lite_clear");
        LV_VG_LITE_CHECK_ERROR(vg_lite_clear(&u->target_buffer, &rect, c));
        LV_PROFILER_END_TAG("vg_lite_clear");
        LV_PROFILER_END;
        return;
    }

    /* convert color */
    vg_lite_color_t vg_color = lv_color32_to_vg(dsc->fill_dsc.color, dsc->fill_dsc.opa);

    /* transform matrix */
    vg_lite_matrix_t matrix;
    lv_matrix_to_vg(&matrix, &dsc->matrix);
    LV_VG_LITE_ASSERT_MATRIX(&matrix);

    /* convert path */
    lv_vg_lite_path_t * lv_vg_path = lv_vg_lite_path_get(u, VG_LITE_FP32);
    lv_path_to_vg(lv_vg_path, path);
    vg_lite_path_t * vg_path = lv_vg_lite_path_get_path(lv_vg_path);
    LV_VG_LITE_ASSERT_PATH(vg_path);

    /* convert path type */
    lv_path_opa_to_vg(lv_vg_path, dsc);

    /* convert blend mode and fill rule */
    vg_lite_blend_t blend = lv_blend_to_vg(dsc->blend_mode);
    vg_lite_fill_t fill = lv_fill_to_vg(dsc->fill_dsc.fill_rule);

    /* convert stroke style */
    lv_stroke_to_vg(lv_vg_path, &dsc->stroke_dsc);

    /* get path bounds */
    float min_x, min_y, max_x, max_y;
    lv_vg_lite_path_get_bonding_box(lv_vg_path, &min_x, &min_y, &max_x, &max_y);

    if(vg_lite_query_feature(gcFEATURE_BIT_VG_SCISSOR)) {
        /* set scissor area */
        lv_vg_lite_set_scissor_area(&dsc->scissor_area);
    }
    else {
        /* calc inverse matrix */
        vg_lite_matrix_t result;
        if(!lv_vg_lite_matrix_inverse(&result, &matrix)) {
            LV_LOG_ERROR("no inverse matrix");
            LV_PROFILER_END;
            return;
        }

        /* Reverse the clip area on the source */
        lv_point_precise_t p1 = { dsc->scissor_area.x1, dsc->scissor_area.y1 };
        lv_point_precise_t p1_res = lv_vg_lite_matrix_transform_point(&result, &p1);

        /* vg-lite bounding_box will crop the pixels on the edge, so +1px is needed here */
        lv_point_precise_t p2 = { dsc->scissor_area.x2 + 1, dsc->scissor_area.y2 + 1 };
        lv_point_precise_t p2_res = lv_vg_lite_matrix_transform_point(&result, &p2);

        lv_vg_lite_path_set_bonding_box(lv_vg_path, p1_res.x, p1_res.y, p2_res.x, p2_res.y);
    }

    switch(dsc->fill_dsc.style) {
        case LV_VECTOR_DRAW_STYLE_SOLID: {
                /* normal draw shape */
                LV_PROFILER_BEGIN_TAG("vg_lite_draw");
                LV_VG_LITE_CHECK_ERROR(vg_lite_draw(
                                           &u->target_buffer,
                                           vg_path,
                                           fill,
                                           &matrix,
                                           blend,
                                           vg_color));
                LV_PROFILER_END_TAG("vg_lite_draw");
            }
            break;
        case LV_VECTOR_DRAW_STYLE_PATTERN: {
                /* draw image */
                vg_lite_buffer_t image_buffer;
                lv_image_decoder_dsc_t decoder_dsc;
                if(lv_vg_lite_buffer_open_image(&image_buffer, &decoder_dsc, dsc->fill_dsc.img_dsc.src, false)) {
                    /* Calculate pattern matrix. Should start from path bond box, and also apply fill matrix. */
                    lv_matrix_t m = dsc->matrix;
                    lv_matrix_translate(&m, min_x, min_y);
                    lv_matrix_multiply(&m, &dsc->fill_dsc.matrix);

                    vg_lite_matrix_t pattern_matrix;
                    lv_matrix_to_vg(&pattern_matrix, &m);

                    vg_lite_color_t recolor = lv_vg_lite_color(dsc->fill_dsc.img_dsc.recolor, dsc->fill_dsc.img_dsc.recolor_opa, true);

                    LV_VG_LITE_ASSERT_MATRIX(&pattern_matrix);

                    LV_PROFILER_BEGIN_TAG("vg_lite_draw_pattern");
                    LV_VG_LITE_CHECK_ERROR(vg_lite_draw_pattern(
                                               &u->target_buffer,
                                               vg_path,
                                               fill,
                                               &matrix,
                                               &image_buffer,
                                               &pattern_matrix,
                                               blend,
                                               VG_LITE_PATTERN_COLOR,
                                               recolor,
                                               vg_color,
                                               VG_LITE_FILTER_BI_LINEAR));
                    LV_PROFILER_END_TAG("vg_lite_draw_pattern");

                    lv_vg_lite_pending_add(u->image_dsc_pending, &decoder_dsc);
                }
            }
            break;
        case LV_VECTOR_DRAW_STYLE_GRADIENT: {
                /* draw gradient */
                lv_vector_gradient_style_t style = dsc->fill_dsc.gradient.style;
                vg_lite_gradient_spreadmode_t spreadmode = lv_spread_to_vg(dsc->fill_dsc.gradient.spread);
                LV_UNUSED(spreadmode);

                if(style == LV_VECTOR_GRADIENT_STYLE_LINEAR) {
                    vg_lite_matrix_t grad_matrix, fill_matrix;
                    lv_area_t grad_area;
                    lv_area_set(&grad_area, (int32_t)min_x, (int32_t)min_y, (int32_t)max_x, (int32_t)max_y);
                    lv_vg_lite_grad_area_to_matrix(&grad_matrix, &grad_area, LV_GRAD_DIR_HOR);

                    lv_matrix_to_vg(&fill_matrix, &dsc->fill_dsc.matrix);
                    lv_vg_lite_matrix_multiply(&grad_matrix, &matrix);
                    lv_vg_lite_matrix_multiply(&grad_matrix, &fill_matrix);

                    lv_vg_lite_draw_linear_grad(
                        u,
                        &u->target_buffer,
                        vg_path,
                        &dsc->fill_dsc.gradient.grad,
                        &grad_matrix,
                        &matrix,
                        fill,
                        blend);
                }
                else if(style == LV_VECTOR_GRADIENT_STYLE_RADIAL) {
                    if(vg_lite_query_feature(gcFEATURE_BIT_VG_RADIAL_GRADIENT)) {
                        /* TODO: radial gradient */
                    }
                    else {
                        LV_LOG_WARN("radial gradient is not supported");
                    }
                }
            }
            break;
        default:
            LV_LOG_WARN("unknown style: %d", dsc->fill_dsc.style);
            break;
    }

    /* Flush in time to avoid accumulation of drawing commands */
    lv_vg_lite_flush(u);

    /* drop path */
    lv_vg_lite_path_drop(u, lv_vg_path);

    if(vg_lite_query_feature(gcFEATURE_BIT_VG_SCISSOR)) {
        /* disable scissor */
        lv_vg_lite_disable_scissor();
    }

    LV_PROFILER_END;
}

static void lv_matrix_to_vg(vg_lite_matrix_t * dest, const lv_matrix_t * src)
{
    lv_memcpy(dest, src, sizeof(lv_matrix_t));
}

static vg_lite_quality_t lv_quality_to_vg(lv_vector_path_quality_t quality)
{
    switch(quality) {
        case LV_VECTOR_PATH_QUALITY_LOW:
            return VG_LITE_LOW;
        case LV_VECTOR_PATH_QUALITY_MEDIUM:
            return VG_LITE_MEDIUM;
        case LV_VECTOR_PATH_QUALITY_HIGH:
            return VG_LITE_HIGH;
        default:
            return VG_LITE_MEDIUM;
    }
}

static void lv_path_to_vg(lv_vg_lite_path_t * dest, const lv_vector_path_t * src)
{
    LV_PROFILER_BEGIN;
    lv_vg_lite_path_set_quality(dest, lv_quality_to_vg(src->quality));

    /* init bounds */
    float min_x = __FLT_MAX__;
    float min_y = __FLT_MAX__;
    float max_x = __FLT_MIN__;
    float max_y = __FLT_MIN__;

#define CMP_BOUNDS(point)                           \
    do {                                            \
        if((point)->x < min_x) min_x = (point)->x;  \
        if((point)->y < min_y) min_y = (point)->y;  \
        if((point)->x > max_x) max_x = (point)->x;  \
        if((point)->y > max_y) max_y = (point)->y;  \
    } while(0)

    uint32_t pidx = 0;
    lv_vector_path_op_t * op = lv_array_front(&src->ops);
    uint32_t size = lv_array_size(&src->ops);
    for(uint32_t i = 0; i < size; i++) {
        switch(op[i]) {
            case LV_VECTOR_PATH_OP_MOVE_TO: {
                    const lv_fpoint_t * pt = lv_array_at(&src->points, pidx);
                    CMP_BOUNDS(pt);
                    lv_vg_lite_path_move_to(dest, pt->x, pt->y);
                    pidx += 1;
                }
                break;
            case LV_VECTOR_PATH_OP_LINE_TO: {
                    const lv_fpoint_t * pt = lv_array_at(&src->points, pidx);
                    CMP_BOUNDS(pt);
                    lv_vg_lite_path_line_to(dest, pt->x, pt->y);
                    pidx += 1;
                }
                break;
            case LV_VECTOR_PATH_OP_QUAD_TO: {
                    const lv_fpoint_t * pt1 = lv_array_at(&src->points, pidx);
                    const lv_fpoint_t * pt2 = lv_array_at(&src->points, pidx + 1);
                    CMP_BOUNDS(pt1);
                    CMP_BOUNDS(pt2);
                    lv_vg_lite_path_quad_to(dest, pt1->x, pt1->y, pt2->x, pt2->y);
                    pidx += 2;
                }
                break;
            case LV_VECTOR_PATH_OP_CUBIC_TO: {
                    const lv_fpoint_t * pt1 = lv_array_at(&src->points, pidx);
                    const lv_fpoint_t * pt2 = lv_array_at(&src->points, pidx + 1);
                    const lv_fpoint_t * pt3 = lv_array_at(&src->points, pidx + 2);
                    CMP_BOUNDS(pt1);
                    CMP_BOUNDS(pt2);
                    CMP_BOUNDS(pt3);
                    lv_vg_lite_path_cubic_to(dest, pt1->x, pt1->y, pt2->x, pt2->y, pt3->x, pt3->y);
                    pidx += 3;
                }
                break;
            case LV_VECTOR_PATH_OP_CLOSE: {
                    lv_vg_lite_path_close(dest);
                }
                break;
        }
    }

    lv_vg_lite_path_end(dest);
    lv_vg_lite_path_set_bonding_box(dest, min_x, min_y, max_x, max_y);
    LV_PROFILER_END;
}

static void lv_path_opa_to_vg(lv_vg_lite_path_t * dest, const lv_vector_draw_dsc_t * dsc)
{
    vg_lite_path_type_t path_type = VG_LITE_DRAW_ZERO;
    lv_opa_t fill_opa = dsc->fill_dsc.opa;
    lv_opa_t stroke_opa = dsc->stroke_dsc.opa;

    if(fill_opa > LV_OPA_0 && stroke_opa > LV_OPA_0) {
        path_type = VG_LITE_DRAW_FILL_STROKE_PATH;
    }
    else if(fill_opa == LV_OPA_0 && stroke_opa > LV_OPA_0) {
        path_type = VG_LITE_DRAW_STROKE_PATH;
    }
    else if(fill_opa > LV_OPA_0) {
        path_type = VG_LITE_DRAW_FILL_PATH;
    }

    LV_VG_LITE_CHECK_ERROR(vg_lite_set_path_type(lv_vg_lite_path_get_path(dest), path_type));
}

static void lv_stroke_to_vg(lv_vg_lite_path_t * dest, const lv_vector_stroke_dsc_t * dsc)
{
    LV_ASSERT_NULL(dest);
    LV_ASSERT_NULL(dsc);

    /* if stroke opa is 0, no need to set stroke */
    if(dsc->opa == LV_OPA_0) {
        return;
    }

    vg_lite_path_t * path = lv_vg_lite_path_get_path(dest);

    LV_VG_LITE_CHECK_ERROR(
        vg_lite_set_stroke(
            path,
            lv_stroke_cap_to_vg(dsc->cap),
            lv_stroke_join_to_vg(dsc->join),
            dsc->width,
            dsc->miter_limit,
            lv_array_front(&dsc->dash_pattern),
            dsc->dash_pattern.size,
            dsc->width / 2,
            lv_color32_to_vg(dsc->color, dsc->opa))
    );

    LV_VG_LITE_CHECK_ERROR(vg_lite_update_stroke(path));
}

static vg_lite_blend_t lv_blend_to_vg(lv_vector_blend_t blend)
{
    switch(blend) {
        case LV_VECTOR_BLEND_SRC_OVER:
            return VG_LITE_BLEND_SRC_OVER;
        case LV_VECTOR_BLEND_SCREEN:
            return VG_LITE_BLEND_SCREEN;
        case LV_VECTOR_BLEND_MULTIPLY:
            return VG_LITE_BLEND_MULTIPLY;
        case LV_VECTOR_BLEND_NONE:
            return VG_LITE_BLEND_NONE;
        case LV_VECTOR_BLEND_ADDITIVE:
            return VG_LITE_BLEND_ADDITIVE;
        case LV_VECTOR_BLEND_SRC_IN:
            return VG_LITE_BLEND_SRC_IN;
        case LV_VECTOR_BLEND_DST_OVER:
            return VG_LITE_BLEND_DST_OVER;
        case LV_VECTOR_BLEND_DST_IN:
            return VG_LITE_BLEND_DST_IN;
        case LV_VECTOR_BLEND_SUBTRACTIVE:
            return VG_LITE_BLEND_SUBTRACT;
        default:
            return VG_LITE_BLEND_SRC_OVER;
    }
}

static vg_lite_fill_t lv_fill_to_vg(lv_vector_fill_t fill_rule)
{
    switch(fill_rule) {
        case LV_VECTOR_FILL_NONZERO:
            return VG_LITE_FILL_NON_ZERO;
        case LV_VECTOR_FILL_EVENODD:
            return VG_LITE_FILL_EVEN_ODD;
        default:
            return VG_LITE_FILL_NON_ZERO;
    }
}

static vg_lite_gradient_spreadmode_t lv_spread_to_vg(lv_vector_gradient_spread_t spread)
{
    switch(spread) {
        case LV_VECTOR_GRADIENT_SPREAD_PAD:
            return VG_LITE_GRADIENT_SPREAD_PAD;
        case LV_VECTOR_GRADIENT_SPREAD_REPEAT:
            return VG_LITE_GRADIENT_SPREAD_REPEAT;
        case LV_VECTOR_GRADIENT_SPREAD_REFLECT:
            return VG_LITE_GRADIENT_SPREAD_REFLECT;
        default:
            return VG_LITE_GRADIENT_SPREAD_FILL;
    }
}

static vg_lite_cap_style_t lv_stroke_cap_to_vg(lv_vector_stroke_cap_t cap)
{
    switch(cap) {
        case LV_VECTOR_STROKE_CAP_SQUARE:
            return VG_LITE_CAP_SQUARE;
        case LV_VECTOR_STROKE_CAP_ROUND:
            return VG_LITE_CAP_ROUND;
        case LV_VECTOR_STROKE_CAP_BUTT:
            return VG_LITE_CAP_BUTT;
        default:
            return VG_LITE_CAP_SQUARE;
    }
}

static vg_lite_join_style_t lv_stroke_join_to_vg(lv_vector_stroke_join_t join)
{
    switch(join) {
        case LV_VECTOR_STROKE_JOIN_BEVEL:
            return VG_LITE_JOIN_BEVEL;
        case LV_VECTOR_STROKE_JOIN_ROUND:
            return VG_LITE_JOIN_ROUND;
        case LV_VECTOR_STROKE_JOIN_MITER:
            return VG_LITE_JOIN_MITER;
        default:
            return VG_LITE_JOIN_BEVEL;
    }
}

#endif /*LV_USE_DRAW_VG_LITE && LV_USE_VECTOR_GRAPHIC*/
