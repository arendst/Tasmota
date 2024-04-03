/**
 * @file lv_draw_vglite_blend.c
 *
 */

/**
 * Copyright 2020-2024 NXP
 *
 * SPDX-License-Identifier: MIT
 */

/*********************
 *      INCLUDES
 *********************/

#include "lv_draw_vglite.h"

#if LV_USE_DRAW_VGLITE
#include "lv_vglite_buf.h"
#include "lv_vglite_matrix.h"
#include "lv_vglite_utils.h"

#include "../../../misc/lv_log.h"

/*********************
 *      DEFINES
 *********************/

#if LV_USE_VGLITE_BLIT_SPLIT
/**
* BLIT split threshold - BLITs with width or height higher than this value will
* be done in multiple steps. Value must be multiple of stride alignment in px.
* For most color formats the alignment is 16px (except the index formats).
*/
#define VGLITE_BLIT_SPLIT_THR 352

/* Enable for logging debug traces. */
#define VGLITE_LOG_TRACE 0

#if VGLITE_LOG_TRACE
#define VGLITE_TRACE(fmt, ...)                \
    do {                                      \
        LV_LOG(fmt, ##__VA_ARGS__);           \
    } while (0)
#else
#define VGLITE_TRACE(fmt, ...)                \
    do {                                      \
    } while (0)
#endif
#endif /*LV_USE_VGLITE_BLIT_SPLIT*/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**
 * BLock Image Transfer - copy rectangular image from src_buf to dst_buf with effects.
 * By default, image is copied directly, with optional opacity.
 *
 * @param[in] dest_area Destination area with relative coordinates to dest buffer
 * @param[in] src_area Source area with relative coordinates to src buffer
 * @param[in] dsc Image descriptor
 *
 */
static void _vglite_blit_single(const lv_area_t * dest_area, const lv_area_t * src_area,
                                const lv_draw_image_dsc_t * dsc);

#if LV_USE_VGLITE_BLIT_SPLIT
/**
 * Move buffer pointer as close as possible to area, but with respect to alignment requirements.
 *
 * @param[in] buf Buffer address pointer
 * @param[in] area Area with relative coordinates to the buffer
 * @param[in] stride Stride of buffer in bytes
 * @param[in] cf Color format of buffer
 */
static void _move_buf_close_to_area(void ** buf, lv_area_t * area, uint32_t stride, lv_color_format_t cf);

/**
 * BLock Image Transfer - copy rectangular image from src_buf to dst_buf with effects.
 * By default, image is copied directly, with optional opacity.
 *
 * @param dest_buf Destination buffer
 * @param[in] dest_area Destination area with relative coordinates to dest buffer
 * @param[in] dest_stride Stride of destination buffer in bytes
 * @param[in] dest_cf Color format of destination buffer
 * @param[in] src_buf Source buffer
 * @param[in] src_area Source area with relative coordinates to src buffer
 * @param[in] src_stride Stride of source buffer in bytes
 * @param[in] src_cf Color format of source buffer
 * @param[in] dsc Image descriptor
 *
 */
static void _vglite_blit_split(void * dest_buf, lv_area_t * dest_area, uint32_t dest_stride, lv_color_format_t dest_cf,
                               const void * src_buf, lv_area_t * src_area, uint32_t src_stride, lv_color_format_t src_cf,
                               const lv_draw_image_dsc_t * dsc);
#else
/**
 * BLock Image Transfer - copy rectangular image from src_buf to dst_buf with transformation.
 * By default, image is copied directly, with optional opacity.
 *
 * @param[in] dest_area Area with relative coordinates to dest buffer
 * @param[in] src_area Source area with relative coordinates to src buffer
 * @param[in] dsc Image descriptor
 *
 */
static void _vglite_blit_transform(const lv_area_t * dest_area, const lv_area_t * src_area,
                                   const lv_draw_image_dsc_t * dsc);
#endif /*LV_USE_VGLITE_BLIT_SPLIT*/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_draw_vglite_img(lv_draw_unit_t * draw_unit, const lv_draw_image_dsc_t * dsc,
                        const lv_area_t * coords)
{
    if(dsc->opa <= (lv_opa_t)LV_OPA_MIN)
        return;

    lv_layer_t * layer = draw_unit->target_layer;
    const lv_image_dsc_t * img_dsc = dsc->src;

    lv_area_t relative_coords;
    lv_area_copy(&relative_coords, coords);
    lv_area_move(&relative_coords, -layer->buf_area.x1, -layer->buf_area.y1);

    lv_area_t clip_area;
    lv_area_copy(&clip_area, draw_unit->clip_area);
    lv_area_move(&clip_area, -layer->buf_area.x1, -layer->buf_area.y1);

    lv_area_t blend_area;
    bool has_transform = (dsc->rotation != 0 || dsc->scale_x != LV_SCALE_NONE || dsc->scale_y != LV_SCALE_NONE);
    if(has_transform)
        lv_area_copy(&blend_area, &relative_coords);
    else if(!_lv_area_intersect(&blend_area, &relative_coords, &clip_area))
        return; /*Fully clipped, nothing to do*/

    const void * src_buf = img_dsc->data;

    lv_area_t src_area;
    src_area.x1 = blend_area.x1 - (coords->x1 - layer->buf_area.x1);
    src_area.y1 = blend_area.y1 - (coords->y1 - layer->buf_area.y1);
    src_area.x2 = img_dsc->header.w - 1;
    src_area.y2 = img_dsc->header.h - 1;

    lv_color_format_t src_cf = img_dsc->header.cf;
    uint32_t src_stride = img_dsc->header.stride;

    /* Set src_vgbuf structure. */
    vglite_set_src_buf(src_buf, lv_area_get_width(&src_area), lv_area_get_height(&src_area), src_stride, src_cf);

#if LV_USE_VGLITE_BLIT_SPLIT
    void * dest_buf = layer->draw_buf->data;
    uint32_t dest_stride = layer->draw_buf->header.stride;
    lv_color_format_t dest_cf = layer->draw_buf->header.cf;

    if(!has_transform)
        _vglite_blit_split(dest_buf, &blend_area, dest_stride, dest_cf,
                           src_buf, &src_area, src_stride, src_cf, dsc);
#else
    if(has_transform)
        _vglite_blit_transform(&blend_area, &src_area, dsc);
    else
        _vglite_blit_single(&blend_area, &src_area, dsc);
#endif /*LV_USE_VGLITE_BLIT_SPLIT*/
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
static void _vglite_blit(const lv_area_t * src_area, const lv_draw_image_dsc_t * dsc)
{
    vg_lite_buffer_t * dst_vgbuf = vglite_get_dest_buf();
    vg_lite_buffer_t * src_vgbuf = vglite_get_src_buf();

    vg_lite_rectangle_t rect = {
        .x = (vg_lite_int32_t)src_area->x1,
        .y = (vg_lite_int32_t)src_area->y1,
        .width = (vg_lite_int32_t)lv_area_get_width(src_area),
        .height = (vg_lite_int32_t)lv_area_get_height(src_area)
    };

    src_vgbuf->image_mode = VG_LITE_MULTIPLY_IMAGE_MODE;
    src_vgbuf->transparency_mode = VG_LITE_IMAGE_TRANSPARENT;

    lv_color_t color;
    lv_opa_t opa;

    bool has_recolor = (dsc->recolor_opa > LV_OPA_MIN);
    if(has_recolor) {
        color = dsc->recolor;
        opa = LV_OPA_MIX2(dsc->recolor_opa, dsc->opa);
    }
    else {
        color.red = 0xFF;
        color.green = 0xFF;
        color.blue = 0xFF;
        opa = dsc->opa;
    }

    lv_color32_t col32 = lv_color_to_32(color, opa);
    vg_lite_color_t vgcol = vglite_get_color(col32, false);

    vg_lite_matrix_t * vgmatrix = vglite_get_matrix();
    vg_lite_blend_t vgblend = vglite_get_blend_mode(dsc->blend_mode);

    VGLITE_CHECK_ERROR(vg_lite_blit_rect(dst_vgbuf, src_vgbuf, &rect, vgmatrix, vgblend, vgcol, VG_LITE_FILTER_POINT));

    vglite_run();
}

static void _vglite_blit_single(const lv_area_t * dest_area, const lv_area_t * src_area,
                                const lv_draw_image_dsc_t * dsc)
{
    /* Set vgmatrix. */
    vglite_set_translation_matrix(dest_area);

    /* Start blit. */
    _vglite_blit(src_area, dsc);
}

#if LV_USE_VGLITE_BLIT_SPLIT
static void _move_buf_close_to_area(void ** buf, lv_area_t * area, uint32_t stride, lv_color_format_t cf)
{
    uint8_t ** buf_u8 = (uint8_t **)buf;
    uint8_t align_bytes = vglite_get_alignment(cf);
    uint8_t bits_per_pixel = lv_color_format_get_bpp(cf);

    uint16_t align_pixels = align_bytes * 8 / bits_per_pixel;

    if(area->x1 >= (int32_t)(area->x1 % align_pixels)) {
        uint16_t shift_x = area->x1 - (area->x1 % align_pixels);

        area->x1 -= shift_x;
        area->x2 -= shift_x;
        *buf_u8 += (shift_x * bits_per_pixel) / 8;
    }

    if(area->y1) {
        uint16_t shift_y = area->y1;

        area->y1 -= shift_y;
        area->y2 -= shift_y;
        *buf_u8 += shift_y * stride;
    }
}

static void _vglite_blit_split(void * dest_buf, lv_area_t * dest_area, uint32_t dest_stride, lv_color_format_t dest_cf,
                               const void * src_buf, lv_area_t * src_area, uint32_t src_stride, lv_color_format_t src_cf,
                               const lv_draw_image_dsc_t * dsc)
{
    VGLITE_TRACE("Blit "
                 "Area: ([%d,%d], [%d,%d]) -> ([%d,%d], [%d,%d]) | "
                 "Size: ([%dx%d] -> [%dx%d]) | "
                 "Addr: (0x%x -> 0x%x)",
                 src_area->x1, src_area->y1, src_area->x2, src_area->y2,
                 dest_area->x1, dest_area->y1, dest_area->x2, dest_area->y2,
                 lv_area_get_width(src_area), lv_area_get_height(src_area),
                 lv_area_get_width(dest_area), lv_area_get_height(dest_area),
                 (uintptr_t)src_buf, (uintptr_t)dest_buf);

    /* Move starting pointers as close as possible to [x1, y1], so coordinates are as small as possible */
    _move_buf_close_to_area((void **)&src_buf, src_area, src_stride, src_cf);
    _move_buf_close_to_area(&dest_buf, dest_area, dest_stride, dest_cf);

    /* If we're in limit, do a single BLIT */
    if((src_area->x2 < VGLITE_BLIT_SPLIT_THR) &&
       (src_area->y2 < VGLITE_BLIT_SPLIT_THR)) {

        /* Set new dest_vgbuf and src_vgbuf memory addresses */
        vglite_set_dest_buf_ptr(dest_buf);
        vglite_set_src_buf_ptr(src_buf);

        _vglite_blit_single(dest_area, src_area, dsc);

        VGLITE_TRACE("Single "
                     "Area: ([%d,%d], [%d,%d]) -> ([%d,%d], [%d,%d]) | "
                     "Size: ([%dx%d] -> [%dx%d]) | "
                     "Addr: (0x%x -> 0x%x)",
                     src_area->x1, src_area->y1, src_area->x2, src_area->y2,
                     dest_area->x1, dest_area->y1, dest_area->x2, dest_area->y2,
                     lv_area_get_width(src_area), lv_area_get_height(src_area),
                     lv_area_get_width(dest_area), lv_area_get_height(dest_area),
                     (uintptr_t)src_buf, (uintptr_t)dest_buf);
    };

    /* Split the BLIT into multiple tiles */
    VGLITE_TRACE("Split "
                 "Area: ([%d,%d], [%d,%d]) -> ([%d,%d], [%d,%d]) | "
                 "Size: ([%dx%d] -> [%dx%d]) | "
                 "Addr: (0x%x -> 0x%x)",
                 src_area->x1, src_area->y1, src_area->x2, src_area->y2,
                 dest_area->x1, dest_area->y1, dest_area->x2, dest_area->y2,
                 lv_area_get_width(src_area), lv_area_get_height(src_area),
                 lv_area_get_width(dest_area), lv_area_get_height(dest_area),
                 (uintptr_t)src_buf, (uintptr_t)dest_buf);

    int32_t width = LV_MIN(lv_area_get_width(src_area), lv_area_get_width(dest_area));
    int32_t height = LV_MIN(lv_area_get_height(src_area), lv_area_get_height(dest_area));

    /* Number of tiles needed */
    uint8_t total_tiles_x = (src_area->x1 + width + VGLITE_BLIT_SPLIT_THR - 1) /
                            VGLITE_BLIT_SPLIT_THR;
    uint8_t total_tiles_y = (src_area->y1 + height + VGLITE_BLIT_SPLIT_THR - 1) /
                            VGLITE_BLIT_SPLIT_THR;

    uint16_t shift_src_x = src_area->x1;
    uint16_t shift_dest_x = dest_area->x1;

    VGLITE_TRACE("X shift: src: %d, dst: %d", shift_src_x, shift_dest_x);

    uint8_t * tile_dest_buf;
    lv_area_t tile_dest_area;
    const uint8_t * tile_src_buf;
    lv_area_t tile_src_area;

    for(uint8_t y = 0; y < total_tiles_y; y++) {
        /* y1 always start from 0 */
        tile_src_area.y1 = 0;

        /* Calculate y2 coordinates */
        if(y < total_tiles_y - 1)
            tile_src_area.y2 = VGLITE_BLIT_SPLIT_THR - 1;
        else
            tile_src_area.y2 = height - y * VGLITE_BLIT_SPLIT_THR - 1;

        /* No vertical shift, dest y is always in sync with src y */
        tile_dest_area.y1 = tile_src_area.y1;
        tile_dest_area.y2 = tile_src_area.y2;

        /* Advance start pointer for every tile, except the first column (y = 0) */
        tile_src_buf = (uint8_t *)src_buf + y * VGLITE_BLIT_SPLIT_THR * src_stride;
        tile_dest_buf = (uint8_t *)dest_buf + y * VGLITE_BLIT_SPLIT_THR * dest_stride;

        for(uint8_t x = 0; x < total_tiles_x; x++) {
            /* x1 always start from the same shift */
            tile_src_area.x1 = shift_src_x;
            tile_dest_area.x1 = shift_dest_x;
            if(x > 0) {
                /* Advance start pointer for every tile, except the first raw (x = 0) */
                tile_src_buf += VGLITE_BLIT_SPLIT_THR * lv_color_format_get_bpp(src_cf) / 8;
                tile_dest_buf += VGLITE_BLIT_SPLIT_THR * lv_color_format_get_bpp(dest_cf) / 8;
            }

            /* Calculate x2 coordinates */
            if(x < total_tiles_x - 1)
                tile_src_area.x2 = VGLITE_BLIT_SPLIT_THR - 1;
            else
                tile_src_area.x2 = width - x * VGLITE_BLIT_SPLIT_THR - 1;

            tile_dest_area.x2 = tile_src_area.x2;

            /* Shift x2 coordinates */
            tile_src_area.x2 += shift_src_x;
            tile_dest_area.x2 += shift_dest_x;

            /* Set new dest_vgbuf and src_vgbuf memory addresses */
            vglite_set_dest_buf_ptr(tile_dest_buf);
            vglite_set_src_buf_ptr(tile_src_buf);

            _vglite_blit_single(&tile_dest_area, &tile_src_area, dsc);

            VGLITE_TRACE("Tile [%d, %d] "
                         "Area: ([%d,%d], [%d,%d]) -> ([%d,%d], [%d,%d]) | "
                         "Size: ([%dx%d] -> [%dx%d]) | "
                         "Addr: (0x%x -> 0x%x)",
                         x, y,
                         tile_src_area.x1, tile_src_area.y1, tile_src_area.x2, tile_src_area.y2,
                         tile_dest_area.x1, tile_dest_area.y1, tile_dest_area.x2, tile_dest_area.y2,
                         lv_area_get_width(&tile_src_area), lv_area_get_height(&tile_src_area),
                         lv_area_get_width(&tile_dest_area), lv_area_get_height(&tile_dest_area),
                         (uintptr_t)tile_src_buf, (uintptr_t)tile_dest_buf);
        }
    }
}
#else
static void _vglite_blit_transform(const lv_area_t * dest_area, const lv_area_t * src_area,
                                   const lv_draw_image_dsc_t * dsc)
{
    /* Set vgmatrix. */
    vglite_set_transformation_matrix(dest_area, dsc);

    /* Start blit. */
    _vglite_blit(src_area, dsc);
}
#endif /*LV_USE_VGLITE_BLIT_SPLIT*/

#endif /*LV_USE_DRAW_VGLITE*/
