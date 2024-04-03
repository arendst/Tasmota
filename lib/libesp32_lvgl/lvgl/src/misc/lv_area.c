/**
 * @file lv_area.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "../lv_conf_internal.h"
#include "../core/lv_global.h"

#include "lv_area.h"
#include "lv_math.h"

/*********************
 *      DEFINES
 *********************/
#define trans_cache LV_GLOBAL_DEFAULT()->area_trans_cache

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static bool lv_point_within_circle(const lv_area_t * area, const lv_point_t * p);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_area_set(lv_area_t * area_p, int32_t x1, int32_t y1, int32_t x2, int32_t y2)
{
    area_p->x1 = x1;
    area_p->y1 = y1;
    area_p->x2 = x2;
    area_p->y2 = y2;
}

void lv_area_set_width(lv_area_t * area_p, int32_t w)
{
    area_p->x2 = area_p->x1 + w - 1;
}

void lv_area_set_height(lv_area_t * area_p, int32_t h)
{
    area_p->y2 = area_p->y1 + h - 1;
}

void _lv_area_set_pos(lv_area_t * area_p, int32_t x, int32_t y)
{
    int32_t w = lv_area_get_width(area_p);
    int32_t h = lv_area_get_height(area_p);
    area_p->x1   = x;
    area_p->y1   = y;
    lv_area_set_width(area_p, w);
    lv_area_set_height(area_p, h);
}

uint32_t lv_area_get_size(const lv_area_t * area_p)
{
    uint32_t size;

    size = (uint32_t)(area_p->x2 - area_p->x1 + 1) * (area_p->y2 - area_p->y1 + 1);

    return size;
}

void lv_area_increase(lv_area_t * area, int32_t w_extra, int32_t h_extra)
{
    area->x1 -= w_extra;
    area->x2 += w_extra;
    area->y1 -= h_extra;
    area->y2 += h_extra;
}

void lv_area_move(lv_area_t * area, int32_t x_ofs, int32_t y_ofs)
{
    area->x1 += x_ofs;
    area->x2 += x_ofs;
    area->y1 += y_ofs;
    area->y2 += y_ofs;
}

bool _lv_area_intersect(lv_area_t * res_p, const lv_area_t * a1_p, const lv_area_t * a2_p)
{
    /*Get the smaller area from 'a1_p' and 'a2_p'*/
    res_p->x1 = LV_MAX(a1_p->x1, a2_p->x1);
    res_p->y1 = LV_MAX(a1_p->y1, a2_p->y1);
    res_p->x2 = LV_MIN(a1_p->x2, a2_p->x2);
    res_p->y2 = LV_MIN(a1_p->y2, a2_p->y2);

    /*If x1 or y1 greater than x2 or y2 then the areas union is empty*/
    bool union_ok = true;
    if((res_p->x1 > res_p->x2) || (res_p->y1 > res_p->y2)) {
        union_ok = false;
    }

    return union_ok;
}

int8_t _lv_area_diff(lv_area_t res_p[], const lv_area_t * a1_p, const lv_area_t * a2_p)
{
    /*Areas have no common parts*/
    if(!_lv_area_is_on(a1_p, a2_p)) return -1;

    /*No remaining areas after removing common parts*/
    if(_lv_area_is_in(a1_p, a2_p, 0)) return 0;

    /*Result counter*/
    int8_t res_c = 0;

    /*Get required information*/
    lv_area_t n;
    int32_t a1_w = lv_area_get_width(a1_p) - 1;
    int32_t a1_h = lv_area_get_height(a1_p) - 1;

    /*Compute top rectangle*/
    int32_t th = a2_p->y1 - a1_p->y1;
    if(th > 0) {
        n.x1 = a1_p->x1;
        n.y1 = a1_p->y1;
        n.x2 = a1_p->x2;
        n.y2 = a1_p->y1 + th;
        res_p[res_c++] = n;
    }

    /*Compute the bottom rectangle*/
    int32_t bh = a1_h - (a2_p->y2 - a1_p->y1);
    if(bh > 0 && a2_p->y2 < a1_p->y2) {
        n.x1 = a1_p->x1;
        n.y1 = a2_p->y2;
        n.x2 = a1_p->x2;
        n.y2 = a2_p->y2 + bh;
        res_p[res_c++] = n;
    }

    /*Compute side height*/
    int32_t y1 = a2_p->y1 > a1_p->y1 ? a2_p->y1 : a1_p->y1;
    int32_t y2 = a2_p->y2 < a1_p->y2 ? a2_p->y2 : a1_p->y2;
    int32_t sh = y2 - y1;

    /*Compute the left rectangle*/
    int32_t lw = a2_p->x1 - a1_p->x1;
    if(lw > 0 && sh > 0) {
        n.x1 = a1_p->x1;
        n.y1 = y1;
        n.x2 = a1_p->x1 + lw;
        n.y2 = y1 + sh;
        res_p[res_c++] = n;
    }

    /*Compute the right rectangle*/
    int32_t rw = a1_w - (a2_p->x2 - a1_p->x1);
    if(rw > 0) {
        n.x1 = a2_p->x2;
        n.y1 = y1;
        n.x2 = a2_p->x2 + rw;
        n.y2 = y1 + sh;
        res_p[res_c++] = n;
    }

    //Return number of results
    return res_c;
}

void _lv_area_join(lv_area_t * a_res_p, const lv_area_t * a1_p, const lv_area_t * a2_p)
{
    a_res_p->x1 = LV_MIN(a1_p->x1, a2_p->x1);
    a_res_p->y1 = LV_MIN(a1_p->y1, a2_p->y1);
    a_res_p->x2 = LV_MAX(a1_p->x2, a2_p->x2);
    a_res_p->y2 = LV_MAX(a1_p->y2, a2_p->y2);
}

bool _lv_area_is_point_on(const lv_area_t * a_p, const lv_point_t * p_p, int32_t radius)
{
    /*First check the basic area*/
    bool is_on_rect = false;
    if((p_p->x >= a_p->x1 && p_p->x <= a_p->x2) && ((p_p->y >= a_p->y1 && p_p->y <= a_p->y2))) {
        is_on_rect = true;
    }
    if(!is_on_rect)
        return false;
    /*Now handle potential rounded rectangles*/
    if(radius <= 0) {
        /*No radius, it is within the rectangle*/
        return true;
    }
    int32_t w = lv_area_get_width(a_p) / 2;
    int32_t h = lv_area_get_height(a_p) / 2;
    int32_t max_radius = LV_MIN(w, h);
    if(radius > max_radius)
        radius = max_radius;

    /*Check if it's in one of the corners*/
    lv_area_t corner_area;
    /*Top left*/
    corner_area.x1 = a_p->x1;
    corner_area.x2 = a_p->x1 + radius;
    corner_area.y1 = a_p->y1;
    corner_area.y2 = a_p->y1 + radius;
    if(_lv_area_is_point_on(&corner_area, p_p, 0)) {
        corner_area.x2 += radius;
        corner_area.y2 += radius;
        return lv_point_within_circle(&corner_area, p_p);
    }
    /*Bottom left*/
    corner_area.y1 = a_p->y2 - radius;
    corner_area.y2 = a_p->y2;
    if(_lv_area_is_point_on(&corner_area, p_p, 0)) {
        corner_area.x2 += radius;
        corner_area.y1 -= radius;
        return lv_point_within_circle(&corner_area, p_p);
    }
    /*Bottom right*/
    corner_area.x1 = a_p->x2 - radius;
    corner_area.x2 = a_p->x2;
    if(_lv_area_is_point_on(&corner_area, p_p, 0)) {
        corner_area.x1 -= radius;
        corner_area.y1 -= radius;
        return lv_point_within_circle(&corner_area, p_p);
    }
    /*Top right*/
    corner_area.y1 = a_p->y1;
    corner_area.y2 = a_p->y1 + radius;
    if(_lv_area_is_point_on(&corner_area, p_p, 0)) {
        corner_area.x1 -= radius;
        corner_area.y2 += radius;
        return lv_point_within_circle(&corner_area, p_p);
    }
    /*Not within corners*/
    return true;
}

bool _lv_area_is_on(const lv_area_t * a1_p, const lv_area_t * a2_p)
{
    if((a1_p->x1 <= a2_p->x2) && (a1_p->x2 >= a2_p->x1) && (a1_p->y1 <= a2_p->y2) && (a1_p->y2 >= a2_p->y1)) {
        return true;
    }
    else {
        return false;
    }
}

bool _lv_area_is_in(const lv_area_t * ain_p, const lv_area_t * aholder_p, int32_t radius)
{
    bool is_in = false;

    if(ain_p->x1 >= aholder_p->x1 && ain_p->y1 >= aholder_p->y1 && ain_p->x2 <= aholder_p->x2 &&
       ain_p->y2 <= aholder_p->y2) {
        is_in = true;
    }

    if(!is_in) return false;
    if(radius == 0) return true;

    /*Check if the corner points are inside the radius or not*/
    lv_point_t p;

    lv_point_set(&p, ain_p->x1, ain_p->y1);
    if(_lv_area_is_point_on(aholder_p, &p, radius) == false) return false;

    lv_point_set(&p, ain_p->x2, ain_p->y1);
    if(_lv_area_is_point_on(aholder_p, &p, radius) == false) return false;

    lv_point_set(&p, ain_p->x1, ain_p->y2);
    if(_lv_area_is_point_on(aholder_p, &p, radius) == false) return false;

    lv_point_set(&p, ain_p->x2, ain_p->y2);
    if(_lv_area_is_point_on(aholder_p, &p, radius) == false) return false;

    return true;
}

bool _lv_area_is_out(const lv_area_t * aout_p, const lv_area_t * aholder_p, int32_t radius)
{
    if(aout_p->x2 < aholder_p->x1 || aout_p->y2 < aholder_p->y1 || aout_p->x1 > aholder_p->x2 ||
       aout_p->y1 > aholder_p->y2) {
        return true;
    }

    if(radius == 0) return false;

    /*Check if the corner points are outside the radius or not*/
    lv_point_t p;

    lv_point_set(&p, aout_p->x1, aout_p->y1);
    if(_lv_area_is_point_on(aholder_p, &p, radius)) return false;

    lv_point_set(&p, aout_p->x2, aout_p->y1);
    if(_lv_area_is_point_on(aholder_p, &p, radius)) return false;

    lv_point_set(&p, aout_p->x1, aout_p->y2);
    if(_lv_area_is_point_on(aholder_p, &p, radius)) return false;

    lv_point_set(&p, aout_p->x2, aout_p->y2);
    if(_lv_area_is_point_on(aholder_p, &p, radius)) return false;

    return true;
}

bool _lv_area_is_equal(const lv_area_t * a, const lv_area_t * b)
{
    return a->x1 == b->x1 && a->x2 == b->x2 && a->y1 == b->y1 && a->y2 == b->y2;
}

void lv_area_align(const lv_area_t * base, lv_area_t * to_align, lv_align_t align, int32_t ofs_x, int32_t ofs_y)
{

    int32_t x;
    int32_t y;
    switch(align) {
        case LV_ALIGN_CENTER:
            x = lv_area_get_width(base) / 2 - lv_area_get_width(to_align) / 2;
            y = lv_area_get_height(base) / 2 - lv_area_get_height(to_align) / 2;
            break;

        case LV_ALIGN_TOP_LEFT:
            x = 0;
            y = 0;
            break;
        case LV_ALIGN_TOP_MID:
            x = lv_area_get_width(base) / 2 - lv_area_get_width(to_align) / 2;
            y = 0;
            break;

        case LV_ALIGN_TOP_RIGHT:
            x = lv_area_get_width(base) - lv_area_get_width(to_align);
            y = 0;
            break;

        case LV_ALIGN_BOTTOM_LEFT:
            x = 0;
            y = lv_area_get_height(base) - lv_area_get_height(to_align);
            break;
        case LV_ALIGN_BOTTOM_MID:
            x = lv_area_get_width(base) / 2 - lv_area_get_width(to_align) / 2;
            y = lv_area_get_height(base) - lv_area_get_height(to_align);
            break;

        case LV_ALIGN_BOTTOM_RIGHT:
            x = lv_area_get_width(base) - lv_area_get_width(to_align);
            y = lv_area_get_height(base) - lv_area_get_height(to_align);
            break;

        case LV_ALIGN_LEFT_MID:
            x = 0;
            y = lv_area_get_height(base) / 2 - lv_area_get_height(to_align) / 2;
            break;

        case LV_ALIGN_RIGHT_MID:
            x = lv_area_get_width(base) - lv_area_get_width(to_align);
            y = lv_area_get_height(base) / 2 - lv_area_get_height(to_align) / 2;
            break;

        case LV_ALIGN_OUT_TOP_LEFT:
            x = 0;
            y = -lv_area_get_height(to_align);
            break;

        case LV_ALIGN_OUT_TOP_MID:
            x = lv_area_get_width(base) / 2 - lv_area_get_width(to_align) / 2;
            y = -lv_area_get_height(to_align);
            break;

        case LV_ALIGN_OUT_TOP_RIGHT:
            x = lv_area_get_width(base) - lv_area_get_width(to_align);
            y = -lv_area_get_height(to_align);
            break;

        case LV_ALIGN_OUT_BOTTOM_LEFT:
            x = 0;
            y = lv_area_get_height(base);
            break;

        case LV_ALIGN_OUT_BOTTOM_MID:
            x = lv_area_get_width(base) / 2 - lv_area_get_width(to_align) / 2;
            y = lv_area_get_height(base);
            break;

        case LV_ALIGN_OUT_BOTTOM_RIGHT:
            x = lv_area_get_width(base) - lv_area_get_width(to_align);
            y = lv_area_get_height(base);
            break;

        case LV_ALIGN_OUT_LEFT_TOP:
            x = -lv_area_get_width(to_align);
            y = 0;
            break;

        case LV_ALIGN_OUT_LEFT_MID:
            x = -lv_area_get_width(to_align);
            y = lv_area_get_height(base) / 2 - lv_area_get_height(to_align) / 2;
            break;

        case LV_ALIGN_OUT_LEFT_BOTTOM:
            x = -lv_area_get_width(to_align);
            y = lv_area_get_height(base) - lv_area_get_height(to_align);
            break;

        case LV_ALIGN_OUT_RIGHT_TOP:
            x = lv_area_get_width(base);
            y = 0;
            break;

        case LV_ALIGN_OUT_RIGHT_MID:
            x = lv_area_get_width(base);
            y = lv_area_get_height(base) / 2 - lv_area_get_height(to_align) / 2;
            break;

        case LV_ALIGN_OUT_RIGHT_BOTTOM:
            x = lv_area_get_width(base);
            y = lv_area_get_height(base) - lv_area_get_height(to_align);
            break;
        default:
            x = 0;
            y = 0;
            break;
    }

    x += base->x1;
    y += base->y1;

    int32_t w = lv_area_get_width(to_align);
    int32_t h = lv_area_get_height(to_align);
    to_align->x1 = x + ofs_x;
    to_align->y1 = y + ofs_y;
    to_align->x2 = to_align->x1 + w - 1;
    to_align->y2 = to_align->y1 + h - 1;
}

#define _LV_TRANSFORM_TRIGO_SHIFT 10
void lv_point_transform(lv_point_t * p, int32_t angle, int32_t scale_x, int32_t scale_y, const lv_point_t * pivot,
                        bool zoom_first)
{
    if(angle == 0 && scale_x == 256 && scale_y == 256) {
        return;
    }

    p->x -= pivot->x;
    p->y -= pivot->y;

    if(angle == 0) {
        p->x = (((int32_t)(p->x) * scale_x) >> 8) + pivot->x;
        p->y = (((int32_t)(p->y) * scale_y) >> 8) + pivot->y;
        return;
    }
    lv_area_transform_cache_t * cache = &trans_cache;
    if(cache->angle_prev != angle) {
        int32_t angle_limited = angle;
        if(angle_limited > 3600) angle_limited -= 3600;
        if(angle_limited < 0) angle_limited += 3600;

        int32_t angle_low = angle_limited / 10;
        int32_t angle_high = angle_low + 1;
        int32_t angle_rem = angle_limited  - (angle_low * 10);

        int32_t s1 = lv_trigo_sin(angle_low);
        int32_t s2 = lv_trigo_sin(angle_high);

        int32_t c1 = lv_trigo_sin(angle_low + 90);
        int32_t c2 = lv_trigo_sin(angle_high + 90);

        cache->sinma = (s1 * (10 - angle_rem) + s2 * angle_rem) / 10;
        cache->cosma = (c1 * (10 - angle_rem) + c2 * angle_rem) / 10;
        cache->sinma = cache->sinma >> (LV_TRIGO_SHIFT - _LV_TRANSFORM_TRIGO_SHIFT);
        cache->cosma = cache->cosma >> (LV_TRIGO_SHIFT - _LV_TRANSFORM_TRIGO_SHIFT);
        cache->angle_prev = angle;
    }
    int32_t x = p->x;
    int32_t y = p->y;
    if(scale_x == 256 && scale_y == 256) {
        p->x = ((cache->cosma * x - cache->sinma * y) >> _LV_TRANSFORM_TRIGO_SHIFT) + pivot->x;
        p->y = ((cache->sinma * x + cache->cosma * y) >> _LV_TRANSFORM_TRIGO_SHIFT) + pivot->y;
    }
    else {
        if(zoom_first) {
            x *= scale_x;
            y *= scale_y;
            p->x = (((cache->cosma * x - cache->sinma * y)) >> (_LV_TRANSFORM_TRIGO_SHIFT + 8)) + pivot->x;
            p->y = (((cache->sinma * x + cache->cosma * y)) >> (_LV_TRANSFORM_TRIGO_SHIFT + 8)) + pivot->y;
        }
        else {
            p->x = (((cache->cosma * x - cache->sinma * y) * scale_x) >> (_LV_TRANSFORM_TRIGO_SHIFT + 8)) + pivot->x;
            p->y = (((cache->sinma * x + cache->cosma * y) * scale_y) >> (_LV_TRANSFORM_TRIGO_SHIFT + 8)) + pivot->y;
        }

    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static bool lv_point_within_circle(const lv_area_t * area, const lv_point_t * p)
{
    int32_t r = (area->x2 - area->x1) / 2;

    /*Circle center*/
    int32_t cx = area->x1 + r;
    int32_t cy = area->y1 + r;

    /*Simplify the code by moving everything to (0, 0)*/
    int32_t px = p->x - cx;
    int32_t py = p->y - cy;

    uint32_t r_sqrd = r * r;
    uint32_t dist = (px * px) + (py * py);

    if(dist <= r_sqrd)
        return true;
    else
        return false;
}
