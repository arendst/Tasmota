/**
 * @file lv_svg_render.h
 *
 */

#ifndef LV_SVG_RENDER_H
#define LV_SVG_RENDER_H

/*********************
 *      INCLUDES
 *********************/
#include "../../lv_conf_internal.h"

#if LV_USE_SVG
#if !LV_USE_VECTOR_GRAPHIC
    #error "LV_USE_SVG requires LV_USE_VECTOR_GRAPHIC = 1"
#endif

#include "lv_svg.h"
#include "../../misc/lv_types.h"
#include "../../draw/lv_draw_vector_private.h"

/*********************
 *      DEFINES
 *********************/

#define LV_SVG_RENDER_OBJ(n) ((lv_svg_render_obj_t*)(n))

/**********************
 *      TYPEDEFS
 **********************/
struct _lv_svg_render_class;

typedef struct _lv_svg_render_obj {
    struct _lv_svg_render_obj * next;
    lv_svg_tag_t tag;
    uint32_t flags;
    char * id;
    lv_vector_draw_dsc_t dsc;
    lv_matrix_t matrix;

    /* for url(XXX) reference */
    struct _lv_svg_render_obj * head;
    char * fill_ref;
    char * stroke_ref;
    struct _lv_svg_render_class * clz;
} lv_svg_render_obj_t;

typedef struct _lv_svg_render_class {
    void (*set_paint_ref)(struct _lv_svg_render_obj * obj, lv_vector_draw_dsc_t * dsc,
                          const struct _lv_svg_render_obj * target_obj, bool fill);

    void (*init)(struct _lv_svg_render_obj * obj, const lv_svg_node_t * node);
    void (*render)(const struct _lv_svg_render_obj * obj, lv_vector_dsc_t * dsc, const lv_matrix_t * matrix);
    void (*set_attr)(struct _lv_svg_render_obj * obj, lv_vector_draw_dsc_t * dsc, const lv_svg_attr_t * attr);
    void (*get_bounds)(const struct _lv_svg_render_obj * obj, lv_area_t * area);
    void (*get_size)(const struct _lv_svg_render_obj * obj, uint32_t * size);
    void (*destroy)(struct _lv_svg_render_obj * obj);
} lv_svg_render_class;

typedef struct _lv_svg_render_hal {
    void (*load_image)(const char * image_url, lv_draw_image_dsc_t * img_dsc);
    const char * (*get_font_path)(const char * font_family);
} lv_svg_render_hal_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * @brief Initialize the SVG render
 * @param hal pointer to a structure with rendering functions
 */
void lv_svg_render_init(const lv_svg_render_hal_t * hal);

/**
 * @brief Create a new SVG render from an SVG document
 * @param svg_doc pointer to the SVG document
 * @return pointer to the new SVG render object
 */
lv_svg_render_obj_t * lv_svg_render_create(const lv_svg_node_t * svg_doc);

/**
 * @brief Delete an SVG render object
 * @param render pointer to the SVG render object to delete
 */
void lv_svg_render_delete(lv_svg_render_obj_t * render);

/**
 * @brief Get size of render objects
 * @param render pointer to the SVG render object
 * @return the bytes of SVG render objects
 */
uint32_t lv_svg_render_get_size(const lv_svg_render_obj_t * render);

/**
 * @brief Get viewport's width and height of the render object
 * @param render pointer to the SVG render object
 * @param width pointer to save the width of the viewport of the SVG render object
 * @param height pointer to save the height of the viewport of the SVG render object
 * @return lv_result_t, LV_RESULT_OK if success, LV_RESULT_INVALID if fail
 */
lv_result_t lv_svg_render_get_viewport_size(const lv_svg_render_obj_t * render, float * width, float * height);

/**
 * @brief Render an SVG object to a vector graphics
 * @param dsc pointer to the vector graphics descriptor
 * @param render pointer to the SVG render object to render
 */
void lv_draw_svg_render(lv_vector_dsc_t * dsc, const lv_svg_render_obj_t * render);

/**
 * @brief Draw an SVG document to a layer
 * @param layer pointer to the target layer
 * @param svg_doc pointer to the SVG document to draw
 */
void lv_draw_svg(lv_layer_t * layer, const lv_svg_node_t * svg_doc);

/**********************
 *      MACROS
 **********************/

#endif /*LV_USE_SVG*/

#endif /*LV_SVG_RENDER_H*/
