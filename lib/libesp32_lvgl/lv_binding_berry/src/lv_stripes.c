/**
 * @file lv_stripes.c
 *
 * A rectangle widget decorated with a repeating pattern of diagonal
 * (or any angle) stripes, drawn as thick lines on top of the normal
 * `lv_obj` background.
 *
 * Part of the Tasmota project (https://github.com/arendst/Tasmota).
 * Author: Stephan Hadinger
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_stripes.h"

#include "../../lvgl/src/core/lv_obj_class_private.h"
#include "../../lvgl/src/core/lv_obj_private.h"
#include "../../lvgl/src/misc/lv_area_private.h"
#include "../../lvgl/src/misc/lv_math.h"
#include "../../lvgl/src/core/lv_obj_scroll.h"
#include "../../lvgl/src/draw/lv_draw.h"
#include "../../lvgl/src/draw/lv_draw_mask.h"
#include "../../lvgl/src/draw/lv_draw_image.h"
#include "../../lvgl/src/draw/lv_draw_line.h"

/*********************
 *      DEFINES
 *********************/
#define MY_CLASS &lv_stripes_class

/**********************
 *      TYPEDEFS
 **********************/

/*Data of the stripes widget*/
typedef struct {
    lv_obj_t obj;
    lv_color_t stripe_color;
    lv_opa_t stripe_opa;
    int32_t stripe_width;
    int32_t gap_width;
    int32_t angle;              /*in degrees, 0..359*/
    int32_t offset;             /*pattern shift in pixels along the normal*/
} lv_stripes_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void lv_stripes_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void lv_stripes_event(const lv_obj_class_t * class_p, lv_event_t * e);
static void draw_stripes(lv_event_t * e);
static void draw_stripes_pattern(lv_stripes_t * stripes, lv_layer_t * target_layer, const lv_area_t * coords);
static void draw_stripes_masked_band(lv_stripes_t * stripes, lv_layer_t * layer, const lv_area_t * coords,
                                     int32_t radius, const lv_area_t * band);

/**********************
 *  STATIC VARIABLES
 **********************/
const lv_obj_class_t lv_stripes_class = {
    .constructor_cb = lv_stripes_constructor,
    .event_cb = lv_stripes_event,
    .instance_size = sizeof(lv_stripes_t),
    .base_class = &lv_obj_class,
    .width_def = LV_DPI_DEF,
    .height_def = LV_DPI_DEF,
};

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t * lv_stripes_create(lv_obj_t * parent)
{
    LV_LOG_INFO("begin");
    lv_obj_t * obj = lv_obj_class_create_obj(MY_CLASS, parent);
    lv_obj_class_init_obj(obj);
    return obj;
}

/*=====================
 * Setter functions
 *====================*/

void lv_stripes_set_stripe_color(lv_obj_t * obj, lv_color_t color)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_stripes_t * stripes = (lv_stripes_t *)obj;

    if(lv_color_eq(stripes->stripe_color, color)) return;
    stripes->stripe_color = color;
    lv_obj_invalidate(obj);
}

void lv_stripes_set_stripe_opa(lv_obj_t * obj, lv_opa_t opa)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_stripes_t * stripes = (lv_stripes_t *)obj;

    if(stripes->stripe_opa == opa) return;
    stripes->stripe_opa = opa;
    lv_obj_invalidate(obj);
}

void lv_stripes_set_stripe_width(lv_obj_t * obj, int32_t width)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_stripes_t * stripes = (lv_stripes_t *)obj;

    if(width < 1) width = 1;
    if(stripes->stripe_width == width) return;
    stripes->stripe_width = width;
    lv_obj_invalidate(obj);
}

void lv_stripes_set_gap_width(lv_obj_t * obj, int32_t width)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_stripes_t * stripes = (lv_stripes_t *)obj;

    if(width < 0) width = 0;
    if(stripes->gap_width == width) return;
    stripes->gap_width = width;
    lv_obj_invalidate(obj);
}

void lv_stripes_set_angle(lv_obj_t * obj, int32_t angle)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_stripes_t * stripes = (lv_stripes_t *)obj;

    angle %= 360;
    if(angle < 0) angle += 360;
    if(stripes->angle == angle) return;
    stripes->angle = angle;
    lv_obj_invalidate(obj);
}

void lv_stripes_set_offset(lv_obj_t * obj, int32_t offset)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_stripes_t * stripes = (lv_stripes_t *)obj;

    if(stripes->offset == offset) return;
    stripes->offset = offset;
    lv_obj_invalidate(obj);
}

/*=====================
 * Getter functions
 *====================*/

lv_color_t lv_stripes_get_stripe_color(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_stripes_t * stripes = (lv_stripes_t *)obj;
    return stripes->stripe_color;
}

lv_opa_t lv_stripes_get_stripe_opa(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_stripes_t * stripes = (lv_stripes_t *)obj;
    return stripes->stripe_opa;
}

int32_t lv_stripes_get_stripe_width(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_stripes_t * stripes = (lv_stripes_t *)obj;
    return stripes->stripe_width;
}

int32_t lv_stripes_get_gap_width(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_stripes_t * stripes = (lv_stripes_t *)obj;
    return stripes->gap_width;
}

int32_t lv_stripes_get_angle(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_stripes_t * stripes = (lv_stripes_t *)obj;
    return stripes->angle;
}

int32_t lv_stripes_get_offset(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_stripes_t * stripes = (lv_stripes_t *)obj;
    return stripes->offset;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void lv_stripes_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);
    lv_stripes_t * stripes = (lv_stripes_t *)obj;

    stripes->stripe_color = lv_color_black();
    stripes->stripe_opa = LV_OPA_COVER;
    stripes->stripe_width = 10;
    stripes->gap_width = 10;
    stripes->angle = 45;
    stripes->offset = 0;

    /*Sensible default background so the widget is visible out of the box*/
    lv_obj_set_style_bg_color(obj, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, LV_PART_MAIN);

    /*Draw the border after the main content (our stripes), so it stays on
     *top of them. This is LVGL's built-in mechanism for "border above
     *content" and avoids drawing the border twice.*/
    lv_obj_set_style_border_post(obj, true, LV_PART_MAIN);

    /*Clip children (e.g. an oversized animated stripe child) to the
     *rounded-corner shape, hide scrollbars that would otherwise appear when a
     *child is larger than the container.*/
    lv_obj_set_style_clip_corner(obj, true, LV_PART_MAIN);
    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
}

/**
 * Draw the stripe pattern onto `target_layer`.
 *
 * The pattern is a family of parallel stripes, each drawn as a single thick
 * line `stripe_width` pixels wide, spaced every `stripe_width + gap_width`
 * pixels along the direction perpendicular to the stripe orientation. Each
 * line runs the full diagonal of the widget (extended past the edges), so
 * rotation never leaves gaps at the corners. Drawing one line per stripe -
 * rather than a pair of triangles - avoids any internal anti-aliasing seam
 * along the stripe. The renderer clips everything to `target_layer->_clip_area`.
 */
static void draw_stripes_pattern(lv_stripes_t * stripes, lv_layer_t * target_layer, const lv_area_t * coords)
{
    int32_t w = lv_area_get_width(coords);
    int32_t h = lv_area_get_height(coords);
    int32_t cx = coords->x1 + w / 2;
    int32_t cy = coords->y1 + h / 2;

    int32_t period = stripes->stripe_width + stripes->gap_width;
    if(period < 1) period = 1;

    /*Half-diagonal: long enough so stripe lines always cover the full widget
     *once rotated to any angle, plus a small margin.*/
    int32_t half_diag = lv_sqrt32((uint32_t)((int64_t)w * w + (int64_t)h * h)) / 2 + period;

    /*Direction vector along the stripe length, and its normal (the direction
     *along which stripes are spaced).*/
    int32_t dx = lv_trigo_sin(stripes->angle + 90);
    int32_t dy = lv_trigo_sin(stripes->angle);
    int32_t nx = lv_trigo_sin(stripes->angle);
    int32_t ny = lv_trigo_sin(stripes->angle - 90);

    lv_draw_line_dsc_t line_dsc;
    lv_draw_line_dsc_init(&line_dsc);
    line_dsc.color = stripes->stripe_color;
    line_dsc.opa = stripes->stripe_opa;
    line_dsc.width = stripes->stripe_width;

    /*Offset from a stripe center to its two ends (half diagonal along the
     *stripe direction). Same for every stripe.*/
    int32_t lx = ((int64_t)half_diag * dx) / LV_TRIGO_SIN_MAX;
    int32_t ly = ((int64_t)half_diag * dy) / LV_TRIGO_SIN_MAX;

    /*Pattern shift along the normal, wrapped to one period so any offset
     *value (e.g. an ever-increasing animation counter) stays in range.*/
    int32_t phase = stripes->offset % period;

    int32_t n_stripes = half_diag / period + 1;

    for(int32_t i = -n_stripes; i <= n_stripes; i++) {
        /*Center of this stripe, offset from widget center along the normal,
         *plus the animation phase.*/
        int32_t d = i * period + phase;
        int32_t bcx = cx + ((int64_t)d * nx) / LV_TRIGO_SIN_MAX;
        int32_t bcy = cy + ((int64_t)d * ny) / LV_TRIGO_SIN_MAX;

        line_dsc.p1.x = bcx - lx;
        line_dsc.p1.y = bcy - ly;
        line_dsc.p2.x = bcx + lx;
        line_dsc.p2.y = bcy + ly;
        lv_draw_line(target_layer, &line_dsc);
    }
}

/**
 * Draw the stripe pattern into an offscreen ARGB layer covering only
 * `band`, mask it to the object's rounded rectangle, then composite it
 * back. Used only for the two thin corner strips (top and bottom) where
 * rounded corners actually exist.
 */
static void draw_stripes_masked_band(lv_stripes_t * stripes, lv_layer_t * layer, const lv_area_t * coords,
                                     int32_t radius, const lv_area_t * band)
{
    lv_layer_t * layer_band = lv_draw_layer_create(layer, LV_COLOR_FORMAT_ARGB8888, band);
    if(layer_band == NULL) {
        /*Out of memory: draw directly, corners of this strip may overflow*/
        const lv_area_t clip_ori = layer->_clip_area;
        layer->_clip_area = *band;
        draw_stripes_pattern(stripes, layer, coords);
        layer->_clip_area = clip_ori;
        return;
    }

    draw_stripes_pattern(stripes, layer_band, coords);

    lv_draw_mask_rect_dsc_t mask_dsc;
    lv_draw_mask_rect_dsc_init(&mask_dsc);
    mask_dsc.area = *coords;
    mask_dsc.radius = radius;
    lv_draw_mask_rect(layer_band, &mask_dsc);

    lv_draw_image_dsc_t img_dsc;
    lv_draw_image_dsc_init(&img_dsc);
    img_dsc.src = layer_band;
    lv_draw_layer(layer, &img_dsc, band);
}

/**
 * Draw the stripes for LV_EVENT_DRAW_MAIN.
 *
 * Fast path (no rounded corners): the object is a plain rectangle, so the
 * stripes are drawn directly onto the main layer. LVGL already restricted
 * `layer->_clip_area` to the object's rectangular bounds, which is all the
 * clipping we need, so this costs nothing more than the line draws.
 *
 * Rounded path (radius > 0): rounded corners only exist in the top and
 * bottom `radius`-high strips of the widget. So, following LVGL's own
 * `clip_corner` implementation in `lv_refr.c`, the widget is split into
 * three horizontal bands:
 *   - the middle band (the bulk of the widget) is drawn directly with a
 *     plain rectangular clip - no offscreen buffer, full speed;
 *   - the top and bottom strips (each only `radius` pixels high) are each
 *     rendered into a small ARGB layer, masked to the rounded rectangle,
 *     and composited back.
 * This keeps the expensive offscreen path limited to the small corner
 * regions instead of the whole widget.
 */
static void draw_stripes(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_current_target(e);
    lv_layer_t * layer = lv_event_get_layer(e);
    lv_stripes_t * stripes = (lv_stripes_t *)obj;

    lv_area_t coords;
    lv_area_copy(&coords, &obj->coords);

    int32_t w = lv_area_get_width(&coords);
    int32_t h = lv_area_get_height(&coords);

    int32_t radius = lv_obj_get_style_radius(obj, LV_PART_MAIN);
    int32_t short_side = LV_MIN(w, h);
    int32_t rout = radius;
    if(rout > short_side >> 1) rout = short_side >> 1;

    /*Fast path: plain rectangle, draw straight onto the main layer.*/
    if(rout <= 0) {
        draw_stripes_pattern(stripes, layer, &coords);
        return;
    }

    const lv_area_t clip_area_ori = layer->_clip_area;

    /*Middle band: no rounded corners here, plain rectangular clip, drawn
     *directly onto the main layer (this is the bulk of the widget).*/
    lv_area_t mid = coords;
    mid.y1 += rout;
    mid.y2 -= rout;
    lv_area_t mid_clip;
    if(lv_area_intersect(&mid_clip, &mid, &clip_area_ori)) {
        layer->_clip_area = mid_clip;
        draw_stripes_pattern(stripes, layer, &coords);
        layer->_clip_area = clip_area_ori;
    }

    /*Top strip: contains the two top rounded corners.*/
    lv_area_t top = coords;
    top.y2 = top.y1 + rout - 1;
    lv_area_t top_clip;
    if(lv_area_intersect(&top_clip, &top, &clip_area_ori)) {
        draw_stripes_masked_band(stripes, layer, &coords, radius, &top_clip);
    }

    /*Bottom strip: contains the two bottom rounded corners.*/
    lv_area_t bottom = coords;
    bottom.y1 = bottom.y2 - rout + 1;
    lv_area_t bottom_clip;
    if(lv_area_intersect(&bottom_clip, &bottom, &clip_area_ori)) {
        draw_stripes_masked_band(stripes, layer, &coords, radius, &bottom_clip);
    }

    layer->_clip_area = clip_area_ori;
}

static void lv_stripes_event(const lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_UNUSED(class_p);

    /*Call the ancestor's event handler. For LV_EVENT_DRAW_MAIN it draws the
     *standard lv_obj background and shadow (the border is deferred to
     *LV_EVENT_DRAW_POST because we set `border_post`), and it handles
     *LV_EVENT_COVER_CHECK, so the stripes' coverage matches the background.*/
    lv_result_t res = lv_obj_event_base(MY_CLASS, e);
    if(res != LV_RESULT_OK) return;

    if(lv_event_get_code(e) == LV_EVENT_DRAW_MAIN) {
        draw_stripes(e);
    }
}
