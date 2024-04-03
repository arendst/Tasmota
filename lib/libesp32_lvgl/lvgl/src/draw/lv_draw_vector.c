/**
* @file lv_draw_vector.c
 *
 */

/*********************
*      INCLUDES
 *********************/
#include "lv_draw_vector.h"

#if LV_USE_VECTOR_GRAPHIC

#include "../misc/lv_ll.h"
#include "../stdlib/lv_string.h"
#include <stdbool.h>
#include <math.h>

/*********************
*      DEFINES
 *********************/

#ifndef M_PI
    #define M_PI 3.1415926f
#endif

#define CHECK_AND_RESIZE_PATH_CONTAINER(P, N) \
    do { \
        if ((lv_array_size(&(P)->ops) + (N)) > lv_array_capacity(&(P)->ops)) { \
            lv_array_resize(&(P)->ops, ((P)->ops.capacity << 1)); \
        } \
        if ((lv_array_size(&(P)->points) + (N)) > lv_array_capacity(&(P)->points)) { \
            lv_array_resize(&(P)->points, ((P)->points.capacity << 1)); \
        } \
    } while(0)

/**********************
*      TYPEDEFS
 **********************/

typedef struct {
    lv_vector_path_t * path;
    lv_vector_draw_dsc_t dsc;
} _lv_vector_draw_task;

/**********************
*  STATIC PROTOTYPES
 **********************/

static bool _is_identity_or_translation(const lv_matrix_t * matrix)
{
    return (matrix->m[0][0] == 1.0f &&
            matrix->m[0][1] == 0.0f &&
            matrix->m[1][0] == 0.0f &&
            matrix->m[1][1] == 1.0f &&
            matrix->m[2][0] == 0.0f &&
            matrix->m[2][1] == 0.0f &&
            matrix->m[2][2] == 1.0f);
}

static void _multiply_matrix(lv_matrix_t * matrix, const lv_matrix_t * mul)
{
    /*TODO: use NEON to optimize this function on ARM architecture.*/
    lv_matrix_t tmp;

    for(int y = 0; y < 3; y++) {
        for(int x = 0; x < 3; x++) {
            tmp.m[y][x] = (matrix->m[y][0] * mul->m[0][x])
                          + (matrix->m[y][1] * mul->m[1][x])
                          + (matrix->m[y][2] * mul->m[2][x]);
        }
    }

    lv_memcpy(matrix, &tmp, sizeof(lv_matrix_t));
}

static void _copy_draw_dsc(lv_vector_draw_dsc_t * dst, const lv_vector_draw_dsc_t * src)
{
    dst->fill_dsc.style = src->fill_dsc.style;
    dst->fill_dsc.color = src->fill_dsc.color;
    dst->fill_dsc.opa = src->fill_dsc.opa;
    dst->fill_dsc.fill_rule = src->fill_dsc.fill_rule;
    dst->fill_dsc.gradient.style = src->fill_dsc.gradient.style;
    dst->fill_dsc.gradient.cx = src->fill_dsc.gradient.cx;
    dst->fill_dsc.gradient.cy = src->fill_dsc.gradient.cy;
    dst->fill_dsc.gradient.cr = src->fill_dsc.gradient.cr;
    dst->fill_dsc.gradient.spread = src->fill_dsc.gradient.spread;
    lv_memcpy(&(dst->fill_dsc.gradient.grad), &(src->fill_dsc.gradient.grad), sizeof(lv_grad_dsc_t));
    lv_memcpy(&(dst->fill_dsc.img_dsc), &(src->fill_dsc.img_dsc), sizeof(lv_draw_image_dsc_t));
    lv_memcpy(&(dst->fill_dsc.matrix), &(src->fill_dsc.matrix), sizeof(lv_matrix_t));

    dst->stroke_dsc.style = src->stroke_dsc.style;
    dst->stroke_dsc.color = src->stroke_dsc.color;
    dst->stroke_dsc.opa = src->stroke_dsc.opa;
    dst->stroke_dsc.width = src->stroke_dsc.width;
    dst->stroke_dsc.cap = src->stroke_dsc.cap;
    dst->stroke_dsc.join = src->stroke_dsc.join;
    dst->stroke_dsc.miter_limit = src->stroke_dsc.miter_limit;
    lv_array_copy(&(dst->stroke_dsc.dash_pattern), &(src->stroke_dsc.dash_pattern));
    dst->stroke_dsc.gradient.style = src->stroke_dsc.gradient.style;
    dst->stroke_dsc.gradient.cx = src->stroke_dsc.gradient.cx;
    dst->stroke_dsc.gradient.cy = src->stroke_dsc.gradient.cy;
    dst->stroke_dsc.gradient.cr = src->stroke_dsc.gradient.cr;
    dst->stroke_dsc.gradient.spread = src->fill_dsc.gradient.spread;
    lv_memcpy(&(dst->stroke_dsc.gradient.grad), &(src->stroke_dsc.gradient.grad), sizeof(lv_grad_dsc_t));
    lv_memcpy(&(dst->stroke_dsc.matrix), &(src->stroke_dsc.matrix), sizeof(lv_matrix_t));

    dst->blend_mode = src->blend_mode;
    lv_memcpy(&(dst->matrix), &(src->matrix), sizeof(lv_matrix_t));
    lv_area_copy(&(dst->scissor_area), &(src->scissor_area));
}
/**********************
*   GLOBAL FUNCTIONS
 **********************/

/* matrix functions */
void lv_matrix_identity(lv_matrix_t * matrix)
{
    matrix->m[0][0] = 1.0f;
    matrix->m[0][1] = 0.0f;
    matrix->m[0][2] = 0.0f;
    matrix->m[1][0] = 0.0f;
    matrix->m[1][1] = 1.0f;
    matrix->m[1][2] = 0.0f;
    matrix->m[2][0] = 0.0f;
    matrix->m[2][1] = 0.0f;
    matrix->m[2][2] = 1.0f;
}

void lv_matrix_translate(lv_matrix_t * matrix, float dx, float dy)
{
    if(_is_identity_or_translation(matrix)) {
        /*optimization for matrix translation.*/
        matrix->m[0][2] += dx;
        matrix->m[1][2] += dy;
        return;
    }

    lv_matrix_t tlm = {{
            {1.0f, 0.0f, dx},
            {0.0f, 1.0f, dy},
            {0.0f, 0.0f, 1.0f},
        }
    };

    _multiply_matrix(matrix, &tlm);
}

void lv_matrix_scale(lv_matrix_t * matrix, float scale_x, float scale_y)
{
    lv_matrix_t scm = {{
            {scale_x, 0.0f, 0.0f},
            {0.0f, scale_y, 0.0f},
            {0.0f, 0.0f, 1.0f},
        }
    };

    _multiply_matrix(matrix, &scm);
}

void lv_matrix_rotate(lv_matrix_t * matrix, float degree)
{
    float radian = degree / 180.0f * (float)M_PI;
    float cos_r = cosf(radian);
    float sin_r = sinf(radian);

    lv_matrix_t rtm = {{
            {cos_r, -sin_r, 0.0f},
            {sin_r, cos_r, 0.0f},
            {0.0f, 0.0f, 1.0f},
        }
    };

    _multiply_matrix(matrix, &rtm);
}

void lv_matrix_skew(lv_matrix_t * matrix, float skew_x, float skew_y)
{
    float rskew_x = skew_x / 180.0f * (float)M_PI;
    float rskew_y = skew_y / 180.0f * (float)M_PI;
    float tan_x = tanf(rskew_x);
    float tan_y = tanf(rskew_y);

    lv_matrix_t skm = {{
            {1.0f, tan_x, 0.0f},
            {tan_y, 1.0f, 0.0f},
            {0.0f, 0.0f, 1.0f},
        }
    };

    _multiply_matrix(matrix, &skm);
}

void lv_matrix_multiply(lv_matrix_t * matrix, const lv_matrix_t * m)
{
    _multiply_matrix(matrix, m);
}

/* path functions */
lv_vector_path_t * lv_vector_path_create(lv_vector_path_quality_t quality)
{
    lv_vector_path_t * path = lv_malloc(sizeof(lv_vector_path_t));
    LV_ASSERT_MALLOC(path);
    lv_memzero(path, sizeof(lv_vector_path_t));
    path->quality = quality;
    lv_array_init(&path->ops, 8, sizeof(uint8_t));
    lv_array_init(&path->points, 8, sizeof(lv_fpoint_t));
    return path;
}

void lv_vector_path_copy(lv_vector_path_t * target_path, const lv_vector_path_t * path)
{
    target_path->quality = path->quality;
    lv_array_copy(&target_path->ops, &path->ops);
    lv_array_copy(&target_path->points, &path->points);
}

void lv_vector_path_clear(lv_vector_path_t * path)
{
    lv_array_clear(&path->ops);
    lv_array_clear(&path->points);
}

void lv_vector_path_delete(lv_vector_path_t * path)
{
    lv_array_deinit(&path->ops);
    lv_array_deinit(&path->points);
    lv_free(path);
}

void lv_vector_path_move_to(lv_vector_path_t * path, const lv_fpoint_t * p)
{
    CHECK_AND_RESIZE_PATH_CONTAINER(path, 1);

    uint8_t op = LV_VECTOR_PATH_OP_MOVE_TO;
    lv_array_push_back(&path->ops, &op);
    lv_array_push_back(&path->points, p);
}

void lv_vector_path_line_to(lv_vector_path_t * path, const lv_fpoint_t * p)
{
    if(lv_array_is_empty(&path->ops)) {
        /*first op must be move_to*/
        return;
    }

    CHECK_AND_RESIZE_PATH_CONTAINER(path, 1);

    uint8_t op = LV_VECTOR_PATH_OP_LINE_TO;
    lv_array_push_back(&path->ops, &op);
    lv_array_push_back(&path->points, p);
}

void lv_vector_path_quad_to(lv_vector_path_t * path, const lv_fpoint_t * p1, const lv_fpoint_t * p2)
{
    if(lv_array_is_empty(&path->ops)) {
        /*first op must be move_to*/
        return;
    }

    CHECK_AND_RESIZE_PATH_CONTAINER(path, 2);

    uint8_t op = LV_VECTOR_PATH_OP_QUAD_TO;
    lv_array_push_back(&path->ops, &op);
    lv_array_push_back(&path->points, p1);
    lv_array_push_back(&path->points, p2);
}

void lv_vector_path_cubic_to(lv_vector_path_t * path, const lv_fpoint_t * p1, const lv_fpoint_t * p2,
                             const lv_fpoint_t * p3)
{
    if(lv_array_is_empty(&path->ops)) {
        /*first op must be move_to*/
        return;
    }

    CHECK_AND_RESIZE_PATH_CONTAINER(path, 3);

    uint8_t op = LV_VECTOR_PATH_OP_CUBIC_TO;
    lv_array_push_back(&path->ops, &op);
    lv_array_push_back(&path->points, p1);
    lv_array_push_back(&path->points, p2);
    lv_array_push_back(&path->points, p3);
}

void lv_vector_path_close(lv_vector_path_t * path)
{
    if(lv_array_is_empty(&path->ops)) {
        /*first op must be move_to*/
        return;
    }

    CHECK_AND_RESIZE_PATH_CONTAINER(path, 1);

    uint8_t op = LV_VECTOR_PATH_OP_CLOSE;
    lv_array_push_back(&path->ops, &op);
}

void lv_vector_path_append_rect(lv_vector_path_t * path, const lv_area_t * rect, float rx, float ry)
{
    float x = rect->x1;
    float y = rect->y1;
    float w = (float)lv_area_get_width(rect);
    float h = (float)lv_area_get_height(rect);

    float hw = w * 0.5f;
    float hh = h * 0.5f;

    if(rx > hw) rx = hw;
    if(ry > hh) ry = hh;

    if(rx == 0 && ry == 0) {
        lv_fpoint_t pt = {x, y};
        lv_vector_path_move_to(path, &pt);
        pt.x += w;
        lv_vector_path_line_to(path, &pt);
        pt.y += h;
        lv_vector_path_line_to(path, &pt);
        pt.x -= w;
        lv_vector_path_line_to(path, &pt);
        lv_vector_path_close(path);
    }
    else if(rx == hw && ry == hh) {
        lv_fpoint_t pt = {x + w * 0.5f, y + h * 0.5f};
        lv_vector_path_append_circle(path, &pt, rx, ry);
    }
    else {
        float hrx = rx * 0.5f;
        float hry = ry * 0.5f;
        lv_fpoint_t pt, pt2, pt3;

        pt.x = x + rx;
        pt.y = y;
        lv_vector_path_move_to(path, &pt);

        pt.x = x + w - rx;
        pt.y = y;
        lv_vector_path_line_to(path, &pt);

        pt.x = x + w - rx + hrx;
        pt.y = y;
        pt2.x = x + w;
        pt2.y = y + ry - hry;
        pt3.x = x + w;
        pt3.y = y + ry;
        lv_vector_path_cubic_to(path, &pt, &pt2, &pt3);

        pt.x = x + w;
        pt.y = y + h - ry;
        lv_vector_path_line_to(path, &pt);

        pt.x = x + w;
        pt.y = y + h - ry + hry;
        pt2.x = x + w - rx + hrx;
        pt2.y = y + h;
        pt3.x = x + w - rx;
        pt3.y = y + h;
        lv_vector_path_cubic_to(path, &pt, &pt2, &pt3);

        pt.x = x + rx;
        pt.y = y + h;
        lv_vector_path_line_to(path, &pt);

        pt.x = x + rx - hrx;
        pt.y = y + h;
        pt2.x = x;
        pt2.y = y + h - ry + hry;
        pt3.x = x;
        pt3.y = y + h - ry;
        lv_vector_path_cubic_to(path, &pt, &pt2, &pt3);

        pt.x = x;
        pt.y = y + ry;
        lv_vector_path_line_to(path, &pt);

        pt.x = x;
        pt.y = y + ry - hry;
        pt2.x = x + rx - hrx;
        pt2.y = y;
        pt3.x = x + rx;
        pt3.y = y;
        lv_vector_path_cubic_to(path, &pt, &pt2, &pt3);
        lv_vector_path_close(path);
    }
}

void lv_vector_path_append_circle(lv_vector_path_t * path, const lv_fpoint_t * c, float rx, float ry)
{
    float krx = rx * 0.552284f;
    float kry = ry * 0.552284f;
    float cx = c->x;
    float cy = c->y;

    lv_fpoint_t pt, pt2, pt3;
    pt.x = cx;
    pt.y = cy - ry;
    lv_vector_path_move_to(path, &pt);

    pt.x = cx + krx;
    pt.y = cy - ry;
    pt2.x = cx + rx;
    pt2.y = cy - kry;
    pt3.x = cx + rx;
    pt3.y = cy;
    lv_vector_path_cubic_to(path, &pt, &pt2, &pt3);

    pt.x = cx + rx;
    pt.y = cy + kry;
    pt2.x = cx + krx;
    pt2.y = cy + ry;
    pt3.x = cx;
    pt3.y = cy + ry;
    lv_vector_path_cubic_to(path, &pt, &pt2, &pt3);

    pt.x = cx - krx;
    pt.y = cy + ry;
    pt2.x = cx - rx;
    pt2.y = cy + kry;
    pt3.x = cx - rx;
    pt3.y = cy;
    lv_vector_path_cubic_to(path, &pt, &pt2, &pt3);

    pt.x = cx - rx;
    pt.y = cy - kry;
    pt2.x = cx - krx;
    pt2.y = cy - ry;
    pt3.x = cx;
    pt3.y = cy - ry;
    lv_vector_path_cubic_to(path, &pt, &pt2, &pt3);

    lv_vector_path_close(path);
}

void lv_vector_path_append_path(lv_vector_path_t * path, const lv_vector_path_t * subpath)
{
    uint32_t ops_size = lv_array_size(&path->ops);
    uint32_t nops_size = lv_array_size(&subpath->ops);
    uint32_t point_size = lv_array_size(&path->points);
    uint32_t npoint_size = lv_array_size(&subpath->points);

    lv_array_concat(&path->ops, &subpath->ops);
    path->ops.size = ops_size + nops_size;

    lv_array_concat(&path->points, &subpath->points);
    path->points.size = point_size + npoint_size;
}

/* draw dsc functions */

lv_vector_dsc_t * lv_vector_dsc_create(lv_layer_t * layer)
{
    lv_vector_dsc_t * dsc = lv_malloc(sizeof(lv_vector_dsc_t));
    LV_ASSERT_MALLOC(dsc);
    lv_memzero(dsc, sizeof(lv_vector_dsc_t));

    dsc->layer = layer;

    lv_vector_fill_dsc_t * fill_dsc = &(dsc->current_dsc.fill_dsc);
    fill_dsc->style = LV_VECTOR_DRAW_STYLE_SOLID;
    fill_dsc->color = lv_color_to_32(lv_color_black(), 0xFF);
    fill_dsc->opa = LV_OPA_COVER;
    fill_dsc->fill_rule = LV_VECTOR_FILL_NONZERO;
    lv_matrix_identity(&(fill_dsc->matrix)); /*identity matrix*/

    lv_vector_stroke_dsc_t * stroke_dsc = &(dsc->current_dsc.stroke_dsc);
    stroke_dsc->style = LV_VECTOR_DRAW_STYLE_SOLID;
    stroke_dsc->color = lv_color_to_32(lv_color_black(), 0xFF);
    stroke_dsc->opa = LV_OPA_0; /*default no stroke*/
    stroke_dsc->width = 1.0f;
    stroke_dsc->cap = LV_VECTOR_STROKE_CAP_BUTT;
    stroke_dsc->join = LV_VECTOR_STROKE_JOIN_MITER;
    stroke_dsc->miter_limit = 4.0f;
    lv_matrix_identity(&(stroke_dsc->matrix)); /*identity matrix*/

    dsc->current_dsc.blend_mode = LV_VECTOR_BLEND_SRC_OVER;
    dsc->current_dsc.scissor_area = layer->_clip_area;
    lv_matrix_identity(&(dsc->current_dsc.matrix)); /*identity matrix*/
    dsc->tasks.task_list = NULL;
    return dsc;
}

void lv_vector_dsc_delete(lv_vector_dsc_t * dsc)
{
    if(dsc->tasks.task_list) {
        lv_ll_t * task_list = dsc->tasks.task_list;
        _lv_vector_for_each_destroy_tasks(task_list, NULL, NULL);
        dsc->tasks.task_list = NULL;
    }
    lv_array_deinit(&(dsc->current_dsc.stroke_dsc.dash_pattern));
    lv_free(dsc);
}

void lv_vector_dsc_set_blend_mode(lv_vector_dsc_t * dsc, lv_vector_blend_t blend)
{
    dsc->current_dsc.blend_mode = blend;
}

void lv_vector_dsc_set_transform(lv_vector_dsc_t * dsc, const lv_matrix_t * matrix)
{
    lv_memcpy(&(dsc->current_dsc.matrix), matrix, sizeof(lv_matrix_t));
}

void lv_vector_dsc_set_fill_color(lv_vector_dsc_t * dsc, lv_color_t color)
{
    dsc->current_dsc.fill_dsc.style = LV_VECTOR_DRAW_STYLE_SOLID;
    dsc->current_dsc.fill_dsc.color = lv_color_to_32(color, 0xFF);
}

void lv_vector_dsc_set_fill_color32(lv_vector_dsc_t * dsc, lv_color32_t color)
{
    dsc->current_dsc.fill_dsc.style = LV_VECTOR_DRAW_STYLE_SOLID;
    dsc->current_dsc.fill_dsc.color = color;
}

void lv_vector_dsc_set_fill_opa(lv_vector_dsc_t * dsc, lv_opa_t opa)
{
    dsc->current_dsc.fill_dsc.opa = opa;
}

void lv_vector_dsc_set_fill_rule(lv_vector_dsc_t * dsc, lv_vector_fill_t rule)
{
    dsc->current_dsc.fill_dsc.fill_rule = rule;
}

void lv_vector_dsc_set_fill_image(lv_vector_dsc_t * dsc, const lv_draw_image_dsc_t * img_dsc)
{
    dsc->current_dsc.fill_dsc.style = LV_VECTOR_DRAW_STYLE_PATTERN;
    lv_memcpy(&(dsc->current_dsc.fill_dsc.img_dsc), img_dsc, sizeof(lv_draw_image_dsc_t));
}

void lv_vector_dsc_set_fill_linear_gradient(lv_vector_dsc_t * dsc, const lv_grad_dsc_t * grad,
                                            lv_vector_gradient_spread_t spread)
{
    dsc->current_dsc.fill_dsc.style = LV_VECTOR_DRAW_STYLE_GRADIENT;
    dsc->current_dsc.fill_dsc.gradient.style = LV_VECTOR_GRADIENT_STYLE_LINEAR;
    dsc->current_dsc.fill_dsc.gradient.spread = spread;
    lv_memcpy(&(dsc->current_dsc.fill_dsc.gradient.grad), grad, sizeof(lv_grad_dsc_t));
}

void lv_vector_dsc_set_fill_radial_gradient(lv_vector_dsc_t * dsc, const lv_grad_dsc_t * grad, float cx, float cy,
                                            float radius, lv_vector_gradient_spread_t spread)
{
    dsc->current_dsc.fill_dsc.style = LV_VECTOR_DRAW_STYLE_GRADIENT;
    dsc->current_dsc.fill_dsc.gradient.style = LV_VECTOR_GRADIENT_STYLE_RADIAL;
    dsc->current_dsc.fill_dsc.gradient.cx = cx;
    dsc->current_dsc.fill_dsc.gradient.cy = cy;
    dsc->current_dsc.fill_dsc.gradient.cr = radius;
    dsc->current_dsc.fill_dsc.gradient.spread = spread;
    lv_memcpy(&(dsc->current_dsc.fill_dsc.gradient.grad), grad, sizeof(lv_grad_dsc_t));
}

void lv_vector_dsc_set_fill_transform(lv_vector_dsc_t * dsc, const lv_matrix_t * matrix)
{
    lv_memcpy(&(dsc->current_dsc.fill_dsc.matrix), matrix, sizeof(lv_matrix_t));
}

void lv_vector_dsc_set_stroke_transform(lv_vector_dsc_t * dsc, const lv_matrix_t * matrix)
{
    lv_memcpy(&(dsc->current_dsc.stroke_dsc.matrix), matrix, sizeof(lv_matrix_t));
}

void lv_vector_dsc_set_stroke_color32(lv_vector_dsc_t * dsc, lv_color32_t color)
{
    dsc->current_dsc.stroke_dsc.style = LV_VECTOR_DRAW_STYLE_SOLID;
    dsc->current_dsc.stroke_dsc.color = color;
}

void lv_vector_dsc_set_stroke_color(lv_vector_dsc_t * dsc, lv_color_t color)
{
    dsc->current_dsc.stroke_dsc.style = LV_VECTOR_DRAW_STYLE_SOLID;
    dsc->current_dsc.stroke_dsc.color = lv_color_to_32(color, 0xFF);
}

void lv_vector_dsc_set_stroke_opa(lv_vector_dsc_t * dsc, lv_opa_t opa)
{
    dsc->current_dsc.stroke_dsc.opa = opa;
}

void lv_vector_dsc_set_stroke_width(lv_vector_dsc_t * dsc, float width)
{
    dsc->current_dsc.stroke_dsc.width = width;
}

void lv_vector_dsc_set_stroke_dash(lv_vector_dsc_t * dsc, float * dash_pattern, uint16_t dash_count)
{
    lv_array_t * dash_array = &(dsc->current_dsc.stroke_dsc.dash_pattern);
    if(dash_pattern) {
        lv_array_clear(dash_array);
        if(lv_array_capacity(dash_array) == 0) {
            lv_array_init(dash_array, dash_count, sizeof(float));
        }
        else {
            lv_array_resize(dash_array, dash_count);
        }
        for(uint16_t i = 0; i < dash_count; i++) {
            lv_array_push_back(dash_array, &dash_pattern[i]);
        }
    }
    else {   /*clear dash*/
        lv_array_clear(dash_array);
    }
}

void lv_vector_dsc_set_stroke_cap(lv_vector_dsc_t * dsc, lv_vector_stroke_cap_t cap)
{
    dsc->current_dsc.stroke_dsc.cap = cap;
}

void lv_vector_dsc_set_stroke_join(lv_vector_dsc_t * dsc, lv_vector_stroke_join_t join)
{
    dsc->current_dsc.stroke_dsc.join = join;
}

void lv_vector_dsc_set_stroke_miter_limit(lv_vector_dsc_t * dsc, uint16_t miter_limit)
{
    dsc->current_dsc.stroke_dsc.miter_limit = miter_limit;
}

void lv_vector_dsc_set_stroke_linear_gradient(lv_vector_dsc_t * dsc, const lv_grad_dsc_t * grad,
                                              lv_vector_gradient_spread_t spread)
{
    dsc->current_dsc.stroke_dsc.style = LV_VECTOR_DRAW_STYLE_GRADIENT;
    dsc->current_dsc.stroke_dsc.gradient.style = LV_VECTOR_GRADIENT_STYLE_LINEAR;
    dsc->current_dsc.stroke_dsc.gradient.spread = spread;
    lv_memcpy(&(dsc->current_dsc.stroke_dsc.gradient.grad), grad, sizeof(lv_grad_dsc_t));
}

void lv_vector_dsc_set_stroke_radial_gradient(lv_vector_dsc_t * dsc, const lv_grad_dsc_t * grad, float cx, float cy,
                                              float radius, lv_vector_gradient_spread_t spread)
{
    dsc->current_dsc.stroke_dsc.style = LV_VECTOR_DRAW_STYLE_GRADIENT;
    dsc->current_dsc.stroke_dsc.gradient.style = LV_VECTOR_GRADIENT_STYLE_RADIAL;
    dsc->current_dsc.stroke_dsc.gradient.cx = cx;
    dsc->current_dsc.stroke_dsc.gradient.cy = cy;
    dsc->current_dsc.stroke_dsc.gradient.cr = radius;
    dsc->current_dsc.stroke_dsc.gradient.spread = spread;
    lv_memcpy(&(dsc->current_dsc.stroke_dsc.gradient.grad), grad, sizeof(lv_grad_dsc_t));
}

/* draw functions */
void lv_vector_dsc_add_path(lv_vector_dsc_t * dsc, const lv_vector_path_t * path)
{
    lv_area_t rect;
    if(!_lv_area_intersect(&rect, &(dsc->layer->_clip_area), &(dsc->current_dsc.scissor_area))) {
        return;
    }

    if(dsc->current_dsc.fill_dsc.opa == 0
       && dsc->current_dsc.stroke_dsc.opa == 0) {
        return;
    }

    if(!dsc->tasks.task_list) {
        dsc->tasks.task_list = lv_malloc(sizeof(lv_ll_t));
        LV_ASSERT_MALLOC(dsc->tasks.task_list);
        _lv_ll_init(dsc->tasks.task_list, sizeof(_lv_vector_draw_task));
    }

    _lv_vector_draw_task * new_task = (_lv_vector_draw_task *)_lv_ll_ins_tail(dsc->tasks.task_list);
    lv_memset(new_task, 0, sizeof(_lv_vector_draw_task));

    new_task->path = lv_vector_path_create(0);

    _copy_draw_dsc(&(new_task->dsc), &(dsc->current_dsc));
    lv_vector_path_copy(new_task->path, path);
    new_task->dsc.scissor_area = rect;
}

void lv_vector_clear_area(lv_vector_dsc_t * dsc, const lv_area_t * rect)
{
    lv_area_t r;
    if(!_lv_area_intersect(&r, &(dsc->layer->_clip_area), &(dsc->current_dsc.scissor_area))) {
        return;
    }

    if(!dsc->tasks.task_list) {
        dsc->tasks.task_list = lv_malloc(sizeof(lv_ll_t));
        LV_ASSERT_MALLOC(dsc->tasks.task_list);
        _lv_ll_init(dsc->tasks.task_list, sizeof(_lv_vector_draw_task));
    }

    _lv_vector_draw_task * new_task = (_lv_vector_draw_task *)_lv_ll_ins_tail(dsc->tasks.task_list);
    lv_memset(new_task, 0, sizeof(_lv_vector_draw_task));

    new_task->dsc.fill_dsc.color = dsc->current_dsc.fill_dsc.color;
    lv_area_copy(&(new_task->dsc.scissor_area), rect);
}

void lv_draw_vector(lv_vector_dsc_t * dsc)
{
    if(!dsc->tasks.task_list) {
        return;
    }

    lv_layer_t * layer = dsc->layer;

    lv_draw_task_t * t = lv_draw_add_task(layer, &(layer->_clip_area));
    t->type = LV_DRAW_TASK_TYPE_VECTOR;
    t->draw_dsc = lv_malloc(sizeof(lv_draw_vector_task_dsc_t));
    lv_memcpy(t->draw_dsc, &(dsc->tasks), sizeof(lv_draw_vector_task_dsc_t));
    lv_draw_finalize_task_creation(layer, t);
    dsc->tasks.task_list = NULL;
}

/* draw dsc transform */
void lv_vector_dsc_identity(lv_vector_dsc_t * dsc)
{
    lv_matrix_identity(&(dsc->current_dsc.matrix)); /*identity matrix*/
}

void lv_vector_dsc_scale(lv_vector_dsc_t * dsc, float scale_x, float scale_y)
{
    lv_matrix_scale(&(dsc->current_dsc.matrix), scale_x, scale_y);
}

void lv_vector_dsc_rotate(lv_vector_dsc_t * dsc, float degree)
{
    lv_matrix_rotate(&(dsc->current_dsc.matrix), degree);
}

void lv_vector_dsc_translate(lv_vector_dsc_t * dsc, float tx, float ty)
{
    lv_matrix_translate(&(dsc->current_dsc.matrix), tx, ty);
}

void lv_vector_dsc_skew(lv_vector_dsc_t * dsc, float skew_x, float skew_y)
{
    lv_matrix_skew(&(dsc->current_dsc.matrix), skew_x, skew_y);
}

void _lv_vector_for_each_destroy_tasks(lv_ll_t * task_list, vector_draw_task_cb cb, void * data)
{
    _lv_vector_draw_task * task = _lv_ll_get_head(task_list);
    _lv_vector_draw_task * next_task = NULL;

    while(task != NULL) {
        next_task = _lv_ll_get_next(task_list, task);
        _lv_ll_remove(task_list, task);

        if(cb) {
            cb(data, task->path, &(task->dsc));
        }

        if(task->path) {
            lv_vector_path_delete(task->path);
        }
        lv_array_deinit(&(task->dsc.stroke_dsc.dash_pattern));

        lv_free(task);
        task = next_task;
    }
    lv_free(task_list);
}
#endif /* LV_USE_VECTOR_GRAPHIC */
