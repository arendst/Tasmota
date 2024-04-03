/**
 * @file lv_math.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_math.h"
#include "../core/lv_global.h"

/*********************
 *      DEFINES
 *********************/
#define rand_seed LV_GLOBAL_DEFAULT()->math_rand_seed

/**********************
 *      TYPEDEFS
 **********************/

#define CUBIC_NEWTON_ITERATIONS     8
#define CUBIC_PRECISION_BITS        10 /* 10 or 14 bits recommended, int64_t calculation is used for >14bit precision */

#if CUBIC_PRECISION_BITS < 10 || CUBIC_PRECISION_BITS > 20
    #error "cubic precision bits should be in range of [10, 20] for 32bit/64bit calculations."
#endif

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
static const uint16_t sin0_90_table[] = {
    0,     572,   1144,  1715,  2286,  2856,  3425,  3993,  4560,  5126,  5690,  6252,  6813,  7371,  7927,  8481,
    9032,  9580,  10126, 10668, 11207, 11743, 12275, 12803, 13328, 13848, 14365, 14876, 15384, 15886, 16384, 16877,
    17364, 17847, 18324, 18795, 19261, 19720, 20174, 20622, 21063, 21498, 21926, 22348, 22763, 23170, 23571, 23965,
    24351, 24730, 25102, 25466, 25822, 26170, 26510, 26842, 27166, 27482, 27789, 28088, 28378, 28660, 28932, 29197,
    29452, 29698, 29935, 30163, 30382, 30592, 30792, 30983, 31164, 31336, 31499, 31651, 31795, 31928, 32052, 32166,
    32270, 32365, 32449, 32524, 32588, 32643, 32688, 32723, 32748, 32763, 32768
};

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

LV_ATTRIBUTE_FAST_MEM int32_t lv_trigo_sin(int16_t angle)
{
    int32_t ret = 0;
    while(angle < 0) angle += 360;
    while(angle >= 360) angle -= 360;

    if(angle < 90) {
        ret = sin0_90_table[angle];
    }
    else if(angle >= 90 && angle < 180) {
        angle = 180 - angle;
        ret   = sin0_90_table[angle];
    }
    else if(angle >= 180 && angle < 270) {
        angle = angle - 180;
        ret   = -sin0_90_table[angle];
    }
    else {   /*angle >=270*/
        angle = 360 - angle;
        ret   = -sin0_90_table[angle];
    }

    if(ret == 32767) return 32768;
    else if(ret == -32767) return -32768;
    else return ret;
}

/**
 * cubic-bezier Reference:
 *
 * https://github.com/gre/bezier-easing
 * https://opensource.apple.com/source/WebCore/WebCore-955.66/platform/graphics/UnitBezier.h
 *
 * Copyright (c) 2014 Gaëtan Renaudeau
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 */

static int32_t do_cubic_bezier(int32_t t, int32_t a, int32_t b, int32_t c)
{
    /*a * t^3 + b * t^2 + c * t*/
#if CUBIC_PRECISION_BITS > 14
    int64_t ret;
#else
    int32_t ret;
#endif

    ret = a;
    ret = (ret * t) >> CUBIC_PRECISION_BITS;
    ret = ((ret + b) * t) >> CUBIC_PRECISION_BITS;
    ret = ((ret + c) * t) >> CUBIC_PRECISION_BITS;
    return ret;
}

int32_t lv_cubic_bezier(int32_t x, int32_t x1, int32_t y1, int32_t x2, int32_t y2)
{
    int32_t ax, bx, cx, ay, by, cy;
    int32_t tl, tr, t;  /*t in cubic-bezier function, used for bisection */
    int32_t xs;  /*x sampled on curve */
#if CUBIC_PRECISION_BITS > 14
    int64_t d; /*slope value at specified t*/
#else
    int32_t d;
#endif

    if(x == 0 || x == LV_BEZIER_VAL_MAX) return x;

    /* input is always LV_BEZIER_VAL_SHIFT bit precision */

#if CUBIC_PRECISION_BITS != LV_BEZIER_VAL_SHIFT
    x <<= CUBIC_PRECISION_BITS - LV_BEZIER_VAL_SHIFT;
    x1 <<= CUBIC_PRECISION_BITS - LV_BEZIER_VAL_SHIFT;
    x2 <<= CUBIC_PRECISION_BITS - LV_BEZIER_VAL_SHIFT;
    y1 <<= CUBIC_PRECISION_BITS - LV_BEZIER_VAL_SHIFT;
    y2 <<= CUBIC_PRECISION_BITS - LV_BEZIER_VAL_SHIFT;
#endif

    cx = 3 * x1;
    bx = 3 * (x2 - x1) - cx;
    ax = (1L << CUBIC_PRECISION_BITS) - cx - bx;

    cy = 3 * y1;
    by = 3 * (y2 - y1) - cy;
    ay = (1L << CUBIC_PRECISION_BITS)  - cy - by;

    /*Try Newton's method firstly */
    t = x; /*Make a guess*/
    for(int i = 0; i < CUBIC_NEWTON_ITERATIONS; i++) {
        /*Check if x on curve at t matches input x*/
        xs = do_cubic_bezier(t, ax, bx, cx) - x;
        if(LV_ABS(xs) <= 1) goto found;

        /* get slop at t, d = 3 * ax * t^2 + 2 * bx + t + cx */
        d = ax; /* use 64bit operation if needed. */
        d = (3 * d * t) >> CUBIC_PRECISION_BITS;
        d = ((d + 2 * bx) * t) >> CUBIC_PRECISION_BITS;
        d += cx;

        if(LV_ABS(d) <= 1) break;

        d = ((int64_t)xs * (1L << CUBIC_PRECISION_BITS)) / d;
        if(d == 0) break;  /*Reached precision limits*/
        t -= d;
    }

    /*Fallback to bisection method for reliability*/
    tl = 0, tr = 1L << CUBIC_PRECISION_BITS, t = x;

    if(t < tl) {
        t = tl;
        goto found;
    }

    if(t > tr) {
        t = tr;
        goto found;
    }

    while(tl < tr) {
        xs = do_cubic_bezier(t, ax, bx, cx);
        if(LV_ABS(xs - x) <= 1) goto found;
        x > xs ? (tl = t) : (tr = t);
        t = (tr - tl) / 2 + tl;
        if(t == tl) break;
    }

    /*Failed to find suitable t for given x, return a value anyway.*/
found:
    /*Return y at t*/
#if CUBIC_PRECISION_BITS != LV_BEZIER_VAL_SHIFT
    return do_cubic_bezier(t, ay, by, cy) >> (CUBIC_PRECISION_BITS - LV_BEZIER_VAL_SHIFT);
#else
    return do_cubic_bezier(t, ay, by, cy);
#endif
}

LV_ATTRIBUTE_FAST_MEM void lv_sqrt(uint32_t x, lv_sqrt_res_t * q, uint32_t mask)
{
    x = x << 8; /*To get 4 bit precision. (sqrt(256) = 16 = 4 bit)*/

    uint32_t root = 0;
    uint32_t trial;
    /*http://ww1.microchip.com/...en/AppNotes/91040a.pdf*/
    do {
        trial = root + mask;
        if(trial * trial <= x) root = trial;
        mask = mask >> 1;
    } while(mask);

    q->i = root >> 4;
    q->f = (root & 0xf) << 4;
}

uint16_t lv_atan2(int x, int y)
{
    /**
     * Fast XY vector to integer degree algorithm - Jan 2011 www.RomanBlack.com
     * Converts any XY values including 0 to a degree value that should be
     * within +/- 1 degree of the accurate value without needing
     * large slow trig functions like ArcTan() or ArcCos().
     * NOTE! at least one of the X or Y values must be non-zero!
     * This is the full version, for all 4 quadrants and will generate
     * the angle in integer degrees from 0-360.
     * Any values of X and Y are usable including negative values provided
     * they are between -1456 and 1456 so the 16bit multiply does not overflow.
     */
    unsigned char negflag;
    unsigned char tempdegree;
    unsigned char comp;
    unsigned int degree;     /*this will hold the result*/
    unsigned int ux;
    unsigned int uy;

    /*Save the sign flags then remove signs and get XY as unsigned ints*/
    negflag = 0;
    if(x < 0) {
        negflag += 0x01;    /*x flag bit*/
        x = (0 - x);        /*is now +*/
    }
    ux = x;                /*copy to unsigned var before multiply*/
    if(y < 0) {
        negflag += 0x02;    /*y flag bit*/
        y = (0 - y);        /*is now +*/
    }
    uy = y;                /*copy to unsigned var before multiply*/

    /*1. Calc the scaled "degrees"*/
    if(ux > uy) {
        degree = (uy * 45) / ux;   /*degree result will be 0-45 range*/
        negflag += 0x10;    /*octant flag bit*/
    }
    else {
        degree = (ux * 45) / uy;   /*degree result will be 0-45 range*/
    }

    /*2. Compensate for the 4 degree error curve*/
    comp = 0;
    tempdegree = degree;    /*use an unsigned char for speed!*/
    if(tempdegree > 22) {    /*if top half of range*/
        if(tempdegree <= 44) comp++;
        if(tempdegree <= 41) comp++;
        if(tempdegree <= 37) comp++;
        if(tempdegree <= 32) comp++;  /*max is 4 degrees compensated*/
    }
    else {   /*else is lower half of range*/
        if(tempdegree >= 2) comp++;
        if(tempdegree >= 6) comp++;
        if(tempdegree >= 10) comp++;
        if(tempdegree >= 15) comp++;  /*max is 4 degrees compensated*/
    }
    degree += comp;   /*degree is now accurate to +/- 1 degree!*/

    /*Invert degree if it was X>Y octant, makes 0-45 into 90-45*/
    if(negflag & 0x10) degree = (90 - degree);

    /*3. Degree is now 0-90 range for this quadrant,*/
    /*need to invert it for whichever quadrant it was in*/
    if(negflag & 0x02) { /*if -Y*/
        if(negflag & 0x01)   /*if -Y -X*/
            degree = (180 + degree);
        else        /*else is -Y +X*/
            degree = (180 - degree);
    }
    else {   /*else is +Y*/
        if(negflag & 0x01)   /*if +Y -X*/
            degree = (360 - degree);
    }
    return degree;
}

int64_t lv_pow(int64_t base, int8_t exp)
{
    int64_t result = 1;
    while(exp) {
        if(exp & 1)
            result *= base;
        exp >>= 1;
        base *= base;
    }

    return result;
}

int32_t lv_map(int32_t x, int32_t min_in, int32_t max_in, int32_t min_out, int32_t max_out)
{
    if(max_in >= min_in && x >= max_in) return max_out;
    if(max_in >= min_in && x <= min_in) return min_out;

    if(max_in <= min_in && x <= max_in) return max_out;
    if(max_in <= min_in && x >= min_in) return min_out;

    /**
     * The equation should be:
     *   ((x - min_in) * delta_out) / delta in) + min_out
     * To avoid rounding error reorder the operations:
     *   (x - min_in) * (delta_out / delta_min) + min_out
     */

    int32_t delta_in = max_in - min_in;
    int32_t delta_out = max_out - min_out;

    return ((x - min_in) * delta_out) / delta_in + min_out;
}

void lv_rand_set_seed(uint32_t seed)
{
    rand_seed = seed;
}

uint32_t lv_rand(uint32_t min, uint32_t max)
{
    /*Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs"*/
    uint32_t x = rand_seed;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    rand_seed = x;

    return (rand_seed % (max - min + 1)) + min;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
