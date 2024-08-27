﻿/**
 * @file lv_draw_sw_blend_al88.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_draw_sw_blend_to_al88.h"
#if LV_USE_DRAW_SW

#if LV_DRAW_SW_SUPPORT_AL88

#include "lv_draw_sw_blend_private.h"
#include "../../../misc/lv_math.h"
#include "../../../display/lv_display.h"
#include "../../../core/lv_refr.h"
#include "../../../misc/lv_color.h"
#include "../../../stdlib/lv_string.h"

#if LV_USE_DRAW_SW_ASM == LV_DRAW_SW_ASM_NEON
    #include "neon/lv_blend_neon.h"
#elif LV_USE_DRAW_SW_ASM == LV_DRAW_SW_ASM_HELIUM
    #include "helium/lv_blend_helium.h"
#elif LV_USE_DRAW_SW_ASM == LV_DRAW_SW_ASM_CUSTOM
    #include LV_DRAW_SW_ASM_CUSTOM_INCLUDE
#endif

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

typedef struct {
    lv_color16a_t fg_saved;
    lv_color16a_t bg_saved;
    lv_color16a_t res_saved;
    lv_opa_t res_alpha_saved;
    lv_opa_t ratio_saved;
} lv_color_mix_alpha_cache_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/

#if LV_DRAW_SW_SUPPORT_L8
    static void /* LV_ATTRIBUTE_FAST_MEM */ l8_image_blend(lv_draw_sw_blend_image_dsc_t * dsc);
#endif

#if LV_DRAW_SW_SUPPORT_I1
    static void /* LV_ATTRIBUTE_FAST_MEM */ i1_image_blend(lv_draw_sw_blend_image_dsc_t * dsc);

    static inline uint8_t /* LV_ATTRIBUTE_FAST_MEM */ get_bit(const uint8_t * buf, int32_t bit_idx);
#endif

static void /* LV_ATTRIBUTE_FAST_MEM */ al88_image_blend(lv_draw_sw_blend_image_dsc_t * dsc);

#if LV_DRAW_SW_SUPPORT_RGB565
    static void /* LV_ATTRIBUTE_FAST_MEM */ rgb565_image_blend(lv_draw_sw_blend_image_dsc_t * dsc);
#endif

#if LV_DRAW_SW_SUPPORT_RGB888
static void /* LV_ATTRIBUTE_FAST_MEM */ rgb888_image_blend(lv_draw_sw_blend_image_dsc_t * dsc,
                                                           const uint8_t src_px_size);
#endif

#if LV_DRAW_SW_SUPPORT_ARGB8888
    static void /* LV_ATTRIBUTE_FAST_MEM */ argb8888_image_blend(lv_draw_sw_blend_image_dsc_t * dsc);
#endif

static void lv_color_mix_with_alpha_cache_init(lv_color_mix_alpha_cache_t * cache);

static inline void /* LV_ATTRIBUTE_FAST_MEM */ blend_non_normal_pixel(lv_color16a_t * dest, lv_color16a_t src,
                                                                      lv_blend_mode_t mode, lv_color_mix_alpha_cache_t * cache);

static inline void * /* LV_ATTRIBUTE_FAST_MEM */ drawbuf_next_row(const void * buf, uint32_t stride);

static inline bool lv_color16a_eq(lv_color16a_t c1, lv_color16a_t c2);

static inline lv_color16a_t /* LV_ATTRIBUTE_FAST_MEM */ lv_color_mix16a(lv_color16a_t fg, lv_color16a_t bg);

static inline void /* LV_ATTRIBUTE_FAST_MEM */ lv_color_16a_16a_mix(lv_color16a_t src, lv_color16a_t * dest,
                                                                    lv_color_mix_alpha_cache_t * cache);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

#ifndef LV_DRAW_SW_COLOR_BLEND_TO_AL88
    #define LV_DRAW_SW_COLOR_BLEND_TO_AL88(...)                         LV_RESULT_INVALID
#endif

#ifndef LV_DRAW_SW_COLOR_BLEND_TO_AL88_WITH_OPA
    #define LV_DRAW_SW_COLOR_BLEND_TO_AL88_WITH_OPA(...)                LV_RESULT_INVALID
#endif

#ifndef LV_DRAW_SW_COLOR_BLEND_TO_AL88_WITH_MASK
    #define LV_DRAW_SW_COLOR_BLEND_TO_AL88_WITH_MASK(...)               LV_RESULT_INVALID
#endif

#ifndef LV_DRAW_SW_COLOR_BLEND_TO_AL88_MIX_MASK_OPA
    #define LV_DRAW_SW_COLOR_BLEND_TO_AL88_MIX_MASK_OPA(...)            LV_RESULT_INVALID
#endif

#ifndef LV_DRAW_SW_RGB565_BLEND_NORMAL_TO_AL88
    #define LV_DRAW_SW_RGB565_BLEND_NORMAL_TO_AL88(...)                 LV_RESULT_INVALID
#endif

#ifndef LV_DRAW_SW_RGB565_BLEND_NORMAL_TO_AL88_WITH_OPA
    #define LV_DRAW_SW_RGB565_BLEND_NORMAL_TO_AL88_WITH_OPA(...)        LV_RESULT_INVALID
#endif

#ifndef LV_DRAW_SW_RGB565_BLEND_NORMAL_TO_AL88_WITH_MASK
    #define LV_DRAW_SW_RGB565_BLEND_NORMAL_TO_AL88_WITH_MASK(...)       LV_RESULT_INVALID
#endif

#ifndef LV_DRAW_SW_RGB565_BLEND_NORMAL_TO_AL88_MIX_MASK_OPA
    #define LV_DRAW_SW_RGB565_BLEND_NORMAL_TO_AL88_MIX_MASK_OPA(...)    LV_RESULT_INVALID
#endif

#ifndef LV_DRAW_SW_RGB888_BLEND_NORMAL_TO_AL88
    #define LV_DRAW_SW_RGB888_BLEND_NORMAL_TO_AL88(...)                 LV_RESULT_INVALID
#endif

#ifndef LV_DRAW_SW_RGB888_BLEND_NORMAL_TO_AL88_WITH_OPA
    #define LV_DRAW_SW_RGB888_BLEND_NORMAL_TO_AL88_WITH_OPA(...)        LV_RESULT_INVALID
#endif

#ifndef LV_DRAW_SW_RGB888_BLEND_NORMAL_TO_AL88_WITH_MASK
    #define LV_DRAW_SW_RGB888_BLEND_NORMAL_TO_AL88_WITH_MASK(...)       LV_RESULT_INVALID
#endif

#ifndef LV_DRAW_SW_RGB888_BLEND_NORMAL_TO_AL88_MIX_MASK_OPA
    #define LV_DRAW_SW_RGB888_BLEND_NORMAL_TO_AL88_MIX_MASK_OPA(...)    LV_RESULT_INVALID
#endif

#ifndef LV_DRAW_SW_ARGB8888_BLEND_NORMAL_TO_AL88
    #define LV_DRAW_SW_ARGB8888_BLEND_NORMAL_TO_AL88(...)                 LV_RESULT_INVALID
#endif

#ifndef LV_DRAW_SW_ARGB8888_BLEND_NORMAL_TO_AL88_WITH_OPA
    #define LV_DRAW_SW_ARGB8888_BLEND_NORMAL_TO_AL88_WITH_OPA(...)        LV_RESULT_INVALID
#endif

#ifndef LV_DRAW_SW_ARGB8888_BLEND_NORMAL_TO_AL88_WITH_MASK
    #define LV_DRAW_SW_ARGB8888_BLEND_NORMAL_TO_AL88_WITH_MASK(...)       LV_RESULT_INVALID
#endif

#ifndef LV_DRAW_SW_ARGB8888_BLEND_NORMAL_TO_AL88_MIX_MASK_OPA
    #define LV_DRAW_SW_ARGB8888_BLEND_NORMAL_TO_AL88_MIX_MASK_OPA(...)    LV_RESULT_INVALID
#endif


#ifndef LV_DRAW_SW_AL88_BLEND_NORMAL_TO_AL88
    #define LV_DRAW_SW_AL88_BLEND_NORMAL_TO_AL88(...)               LV_RESULT_INVALID
#endif

#ifndef LV_DRAW_SW_AL88_BLEND_NORMAL_TO_AL88_WITH_OPA
    #define LV_DRAW_SW_AL88_BLEND_NORMAL_TO_AL88_WITH_OPA(...)      LV_RESULT_INVALID
#endif

#ifndef LV_DRAW_SW_AL88_BLEND_NORMAL_TO_AL88_WITH_MASK
    #define LV_DRAW_SW_AL88_BLEND_NORMAL_TO_AL88_WITH_MASK(...)     LV_RESULT_INVALID
#endif

#ifndef LV_DRAW_SW_AL88_BLEND_NORMAL_TO_AL88_MIX_MASK_OPA
    #define LV_DRAW_SW_AL88_BLEND_NORMAL_TO_AL88_MIX_MASK_OPA(...)  LV_RESULT_INVALID
#endif

#ifndef LV_DRAW_SW_I1_BLEND_NORMAL_TO_AL88
    #define LV_DRAW_SW_I1_BLEND_NORMAL_TO_AL88(...)  LV_RESULT_INVALID
#endif

#ifndef LV_DRAW_SW_I1_BLEND_NORMAL_TO_AL88_WITH_OPA
    #define LV_DRAW_SW_I1_BLEND_NORMAL_TO_AL88_WITH_OPA(...)  LV_RESULT_INVALID
#endif

#ifndef LV_DRAW_SW_I1_BLEND_NORMAL_TO_AL88_WITH_MASK
    #define LV_DRAW_SW_I1_BLEND_NORMAL_TO_AL88_WITH_MASK(...)  LV_RESULT_INVALID
#endif

#ifndef LV_DRAW_SW_I1_BLEND_NORMAL_TO_AL88_MIX_MASK_OPA
    #define LV_DRAW_SW_I1_BLEND_NORMAL_TO_AL88_MIX_MASK_OPA(...)  LV_RESULT_INVALID
#endif

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void LV_ATTRIBUTE_FAST_MEM lv_draw_sw_blend_color_to_al88(lv_draw_sw_blend_fill_dsc_t * dsc)
{
    int32_t w = dsc->dest_w;
    int32_t h = dsc->dest_h;
    lv_opa_t opa = dsc->opa;
    const lv_opa_t * mask = dsc->mask_buf;
    int32_t mask_stride = dsc->mask_stride;
    int32_t dest_stride = dsc->dest_stride;

    lv_color_mix_alpha_cache_t cache;
    lv_color_mix_with_alpha_cache_init(&cache);

    int32_t x;
    int32_t y;

    LV_UNUSED(w);
    LV_UNUSED(h);
    LV_UNUSED(x);
    LV_UNUSED(y);
    LV_UNUSED(opa);
    LV_UNUSED(mask);
    LV_UNUSED(mask_stride);
    LV_UNUSED(dest_stride);

    /*Simple fill*/
    if(mask == NULL && opa >= LV_OPA_MAX) {
        if(LV_RESULT_INVALID == LV_DRAW_SW_COLOR_BLEND_TO_AL88(dsc)) {
            lv_color16a_t color16a;
            color16a.lumi = lv_color_luminance(dsc->color);
            color16a.alpha = 255;
            lv_color16a_t * dest_buf = dsc->dest_buf;
            for(y = 0; y < h; y++) {
                for(x = 0; x < w - 16; x += 16) {
                    dest_buf[x + 0] = color16a;
                    dest_buf[x + 1] = color16a;
                    dest_buf[x + 2] = color16a;
                    dest_buf[x + 3] = color16a;

                    dest_buf[x + 4] = color16a;
                    dest_buf[x + 5] = color16a;
                    dest_buf[x + 6] = color16a;
                    dest_buf[x + 7] = color16a;

                    dest_buf[x + 8] = color16a;
                    dest_buf[x + 9] = color16a;
                    dest_buf[x + 10] = color16a;
                    dest_buf[x + 11] = color16a;

                    dest_buf[x + 12] = color16a;
                    dest_buf[x + 13] = color16a;
                    dest_buf[x + 14] = color16a;
                    dest_buf[x + 15] = color16a;
                }
                for(; x < w; x ++) {
                    dest_buf[x] = color16a;
                }

                dest_buf = drawbuf_next_row(dest_buf, dest_stride);
            }
        }
    }
    /*Opacity only*/
    else if(mask == NULL && opa < LV_OPA_MAX) {
        if(LV_RESULT_INVALID == LV_DRAW_SW_COLOR_BLEND_TO_AL88_WITH_OPA(dsc)) {
            lv_color16a_t color16a;
            color16a.lumi = lv_color_luminance(dsc->color);
            color16a.alpha = opa;
            lv_color16a_t * dest_buf = dsc->dest_buf;
            for(y = 0; y < h; y++) {
                for(x = 0; x < w; x++) {
                    lv_color_16a_16a_mix(color16a, &dest_buf[x], &cache);
                }
                dest_buf = drawbuf_next_row(dest_buf, dest_stride);
            }
        }

    }
    /*Masked with full opacity*/
    else if(mask && opa >= LV_OPA_MAX) {
        if(LV_RESULT_INVALID == LV_DRAW_SW_COLOR_BLEND_TO_AL88_WITH_MASK(dsc)) {
            lv_color16a_t color16a;
            color16a.lumi = lv_color_luminance(dsc->color);
            lv_color16a_t * dest_buf = (lv_color16a_t *)dsc->dest_buf;
            for(y = 0; y < h; y++) {
                for(x = 0; x < w; x++) {
                    color16a.alpha = mask[x];
                    lv_color_16a_16a_mix(color16a, &dest_buf[x], &cache);
                }
                dest_buf = drawbuf_next_row(dest_buf, dest_stride);
                mask += mask_stride;
            }
        }

    }
    /*Masked with opacity*/
    else {
        if(LV_RESULT_INVALID == LV_DRAW_SW_COLOR_BLEND_TO_AL88_MIX_MASK_OPA(dsc)) {
            lv_color16a_t color16a;
            color16a.lumi = lv_color_luminance(dsc->color);
            lv_color16a_t * dest_buf = (lv_color16a_t *)dsc->dest_buf;
            for(y = 0; y < h; y++) {
                for(x = 0; x < w; x++) {
                    color16a.alpha = LV_OPA_MIX2(mask[x], opa);
                    lv_color_16a_16a_mix(color16a, &dest_buf[x], &cache);
                }
                dest_buf = drawbuf_next_row(dest_buf, dest_stride);
                mask += mask_stride;
            }
        }
    }
}

void LV_ATTRIBUTE_FAST_MEM lv_draw_sw_blend_image_to_al88(lv_draw_sw_blend_image_dsc_t * dsc)
{
    switch(dsc->src_color_format) {
#if LV_DRAW_SW_SUPPORT_RGB565
        case LV_COLOR_FORMAT_RGB565:
            rgb565_image_blend(dsc);
            break;
#endif
#if LV_DRAW_SW_SUPPORT_RGB888
        case LV_COLOR_FORMAT_RGB888:
            rgb888_image_blend(dsc, 3);
            break;
#endif
#if LV_DRAW_SW_SUPPORT_XRGB8888
        case LV_COLOR_FORMAT_XRGB8888:
            rgb888_image_blend(dsc, 4);
            break;
#endif
#if LV_DRAW_SW_SUPPORT_ARGB8888
        case LV_COLOR_FORMAT_ARGB8888:
            argb8888_image_blend(dsc);
            break;
#endif
#if LV_DRAW_SW_SUPPORT_L8
        case LV_COLOR_FORMAT_L8:
            l8_image_blend(dsc);
            break;
#endif
        case LV_COLOR_FORMAT_AL88:
            al88_image_blend(dsc);
            break;
#if LV_DRAW_SW_SUPPORT_I1
        case LV_COLOR_FORMAT_I1:
            i1_image_blend(dsc);
            break;
#endif
        default:
            LV_LOG_WARN("Not supported source color format");
            break;
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
#if LV_DRAW_SW_SUPPORT_I1
static void LV_ATTRIBUTE_FAST_MEM i1_image_blend(lv_draw_sw_blend_image_dsc_t * dsc)
{
    int32_t w = dsc->dest_w;
    int32_t h = dsc->dest_h;
    lv_opa_t opa = dsc->opa;
    lv_color16a_t * dest_buf_al88 = dsc->dest_buf;
    int32_t dest_stride = dsc->dest_stride;
    const uint8_t * src_buf_i1 = dsc->src_buf;
    int32_t src_stride = dsc->src_stride;
    const lv_opa_t * mask_buf = dsc->mask_buf;
    int32_t mask_stride = dsc->mask_stride;

    lv_color_mix_alpha_cache_t cache;
    lv_color_mix_with_alpha_cache_init(&cache);

    int32_t x, y;

    if(dsc->blend_mode == LV_BLEND_MODE_NORMAL) {
        if(mask_buf == NULL && opa >= LV_OPA_MAX) {
            if(LV_RESULT_INVALID == LV_DRAW_SW_I1_BLEND_NORMAL_TO_AL88(dsc)) {
                for(y = 0; y < h; y++) {
                    for(x = 0; x < w; x++) {
                        dest_buf_al88[x].lumi = get_bit(src_buf_i1, x) * 255;
                        dest_buf_al88[x].alpha = 255;
                    }
                    dest_buf_al88 = drawbuf_next_row(dest_buf_al88, dest_stride);
                    src_buf_i1 = drawbuf_next_row(src_buf_i1, src_stride);
                }
            }
        }
        else if(mask_buf == NULL && opa < LV_OPA_MAX) {
            if(LV_RESULT_INVALID == LV_DRAW_SW_I1_BLEND_NORMAL_TO_AL88_WITH_OPA(dsc)) {
                for(y = 0; y < h; y++) {
                    for(x = 0; x < w; x++) {
                        lv_color16a_t src_color;
                        src_color.lumi = get_bit(src_buf_i1, x) * 255;
                        src_color.alpha = opa;
                        lv_color_16a_16a_mix(src_color, &dest_buf_al88[x], &cache);
                    }
                    dest_buf_al88 = drawbuf_next_row(dest_buf_al88, dest_stride);
                    src_buf_i1 = drawbuf_next_row(src_buf_i1, src_stride);
                }
            }
        }
        else if(mask_buf && opa >= LV_OPA_MAX) {
            if(LV_RESULT_INVALID == LV_DRAW_SW_I1_BLEND_NORMAL_TO_AL88_WITH_MASK(dsc)) {
                for(y = 0; y < h; y++) {
                    for(x = 0; x < w; x++) {
                        lv_color16a_t src_color;
                        src_color.lumi = get_bit(src_buf_i1, x) * 255;
                        src_color.alpha = mask_buf[x];
                        lv_color_16a_16a_mix(src_color, &dest_buf_al88[x], &cache);
                    }
                    dest_buf_al88 = drawbuf_next_row(dest_buf_al88, dest_stride);
                    src_buf_i1 = drawbuf_next_row(src_buf_i1, src_stride);
                    mask_buf += mask_stride;
                }
            }
        }
        else if(mask_buf && opa < LV_OPA_MAX) {
            if(LV_RESULT_INVALID == LV_DRAW_SW_I1_BLEND_NORMAL_TO_AL88_MIX_MASK_OPA(dsc)) {
                for(y = 0; y < h; y++) {
                    for(x = 0; x < w; x++) {
                        lv_color16a_t src_color;
                        src_color.lumi = get_bit(src_buf_i1, x) * 255;
                        src_color.alpha = LV_OPA_MIX2(mask_buf[x], opa);
                        lv_color_16a_16a_mix(src_color, &dest_buf_al88[x], &cache);
                    }
                    dest_buf_al88 = drawbuf_next_row(dest_buf_al88, dest_stride);
                    src_buf_i1 = drawbuf_next_row(src_buf_i1, src_stride);
                    mask_buf += mask_stride;
                }
            }
        }
        else {
            for(y = 0; y < h; y++) {
                for(x = 0; x < w; x++) {
                    lv_color16a_t src_color;
                    src_color.lumi = get_bit(src_buf_i1, x) * 255;
                    if(mask_buf == NULL) src_color.alpha = opa;
                    else src_color.alpha = LV_OPA_MIX2(mask_buf[x], opa);
                    blend_non_normal_pixel(&dest_buf_al88[x], src_color, dsc->blend_mode, &cache);
                }
                if(mask_buf) mask_buf += mask_stride;
                dest_buf_al88 = drawbuf_next_row(dest_buf_al88, dest_stride);
                src_buf_i1 = drawbuf_next_row(src_buf_i1, src_stride);
            }
        }
    }
}
#endif

#if LV_DRAW_SW_SUPPORT_L8
static void LV_ATTRIBUTE_FAST_MEM l8_image_blend(lv_draw_sw_blend_image_dsc_t * dsc)
{
    int32_t w = dsc->dest_w;
    int32_t h = dsc->dest_h;
    lv_opa_t opa = dsc->opa;
    lv_color16a_t * dest_buf_al88 = dsc->dest_buf;
    int32_t dest_stride = dsc->dest_stride;
    const uint8_t * src_buf_l8 = dsc->src_buf;
    int32_t src_stride = dsc->src_stride;
    const lv_opa_t * mask_buf = dsc->mask_buf;
    int32_t mask_stride = dsc->mask_stride;

    lv_color_mix_alpha_cache_t cache;
    lv_color_mix_with_alpha_cache_init(&cache);

    int32_t x, y;

    if(dsc->blend_mode == LV_BLEND_MODE_NORMAL) {
        if(mask_buf == NULL && opa >= LV_OPA_MAX) {
            if(LV_RESULT_INVALID == LV_DRAW_SW_AL88_BLEND_NORMAL_TO_AL88(dsc)) {
                for(y = 0; y < h; y++) {
                    for(x = 0; x < w; x++) {
                        dest_buf_al88[x].lumi = src_buf_l8[x];
                        dest_buf_al88[x].alpha = 255;
                    }
                    dest_buf_al88 = drawbuf_next_row(dest_buf_al88, dest_stride);
                    src_buf_l8 = drawbuf_next_row(src_buf_l8, src_stride);
                }
            }
        }
        else if(mask_buf == NULL && opa < LV_OPA_MAX) {
            if(LV_RESULT_INVALID == LV_DRAW_SW_AL88_BLEND_NORMAL_TO_AL88_WITH_OPA(dsc)) {
                for(y = 0; y < h; y++) {
                    for(x = 0; x < w; x++) {
                        lv_color16a_t src_color;
                        src_color.lumi = src_buf_l8[x];
                        src_color.alpha = opa;
                        lv_color_16a_16a_mix(src_color, &dest_buf_al88[x], &cache);
                    }
                    dest_buf_al88 = drawbuf_next_row(dest_buf_al88, dest_stride);
                    src_buf_l8 = drawbuf_next_row(src_buf_l8, src_stride);
                }
            }
        }
        else if(mask_buf && opa >= LV_OPA_MAX) {
            if(LV_RESULT_INVALID == LV_DRAW_SW_AL88_BLEND_NORMAL_TO_AL88_WITH_MASK(dsc)) {
                for(y = 0; y < h; y++) {
                    for(x = 0; x < w; x++) {
                        lv_color16a_t src_color;
                        src_color.lumi = src_buf_l8[x];
                        src_color.alpha = mask_buf[x];
                        lv_color_16a_16a_mix(src_color, &dest_buf_al88[x], &cache);
                    }
                    dest_buf_al88 = drawbuf_next_row(dest_buf_al88, dest_stride);
                    src_buf_l8 = drawbuf_next_row(src_buf_l8, src_stride);
                    mask_buf += mask_stride;
                }
            }
        }
        else if(mask_buf && opa < LV_OPA_MAX) {
            if(LV_RESULT_INVALID == LV_DRAW_SW_AL88_BLEND_NORMAL_TO_AL88_MIX_MASK_OPA(dsc)) {
                for(y = 0; y < h; y++) {
                    for(x = 0; x < w; x++) {
                        lv_color16a_t src_color;
                        src_color.lumi = src_buf_l8[x];
                        src_color.alpha = LV_OPA_MIX2(mask_buf[x], opa);
                        lv_color_16a_16a_mix(src_color, &dest_buf_al88[x], &cache);
                    }
                    dest_buf_al88 = drawbuf_next_row(dest_buf_al88, dest_stride);
                    src_buf_l8 = drawbuf_next_row(src_buf_l8, src_stride);
                    mask_buf += mask_stride;
                }
            }
        }
    }
    else {
        for(y = 0; y < h; y++) {
            for(x = 0; x < w; x++) {
                lv_color16a_t src_color;
                src_color.lumi = src_buf_l8[x];
                if(mask_buf == NULL) src_color.alpha = opa;
                else src_color.alpha = LV_OPA_MIX2(mask_buf[x], opa);
                blend_non_normal_pixel(&dest_buf_al88[x], src_color, dsc->blend_mode, &cache);
            }
            if(mask_buf) mask_buf += mask_stride;
            dest_buf_al88 = drawbuf_next_row(dest_buf_al88, dest_stride);
            src_buf_l8 = drawbuf_next_row(src_buf_l8, src_stride);
        }
    }
}

#endif

static void LV_ATTRIBUTE_FAST_MEM al88_image_blend(lv_draw_sw_blend_image_dsc_t * dsc)
{
    int32_t w = dsc->dest_w;
    int32_t h = dsc->dest_h;
    lv_opa_t opa = dsc->opa;
    lv_color16a_t * dest_buf_al88 = dsc->dest_buf;
    int32_t dest_stride = dsc->dest_stride;
    const lv_color16a_t * src_buf_al88 = dsc->src_buf;
    int32_t src_stride = dsc->src_stride;
    const lv_opa_t * mask_buf = dsc->mask_buf;
    int32_t mask_stride = dsc->mask_stride;

    lv_color_mix_alpha_cache_t cache;
    lv_color_mix_with_alpha_cache_init(&cache);

    int32_t x, y;

    if(dsc->blend_mode == LV_BLEND_MODE_NORMAL) {
        if(mask_buf == NULL && opa >= LV_OPA_MAX) {
            if(LV_RESULT_INVALID == LV_DRAW_SW_AL88_BLEND_NORMAL_TO_AL88(dsc)) {
                for(y = 0; y < h; y++) {
                    for(x = 0; x < w; x++) {
                        lv_color_16a_16a_mix(src_buf_al88[x], &dest_buf_al88[x], &cache);
                    }
                    dest_buf_al88 = drawbuf_next_row(dest_buf_al88, dest_stride);
                    src_buf_al88 = drawbuf_next_row(src_buf_al88, src_stride);
                }
            }
        }
        else if(mask_buf == NULL && opa < LV_OPA_MAX) {
            if(LV_RESULT_INVALID == LV_DRAW_SW_AL88_BLEND_NORMAL_TO_AL88_WITH_OPA(dsc)) {
                for(y = 0; y < h; y++) {
                    for(x = 0; x < w; x++) {
                        lv_color16a_t src_color = src_buf_al88[x];
                        src_color.alpha = LV_OPA_MIX2(src_color.alpha, opa);
                        lv_color_16a_16a_mix(src_color, &dest_buf_al88[x], &cache);
                    }
                    dest_buf_al88 = drawbuf_next_row(dest_buf_al88, dest_stride);
                    src_buf_al88 = drawbuf_next_row(src_buf_al88, src_stride);
                }
            }
        }
        else if(mask_buf && opa >= LV_OPA_MAX) {
            if(LV_RESULT_INVALID == LV_DRAW_SW_AL88_BLEND_NORMAL_TO_AL88_WITH_MASK(dsc)) {
                for(y = 0; y < h; y++) {
                    for(x = 0; x < w; x++) {
                        lv_color16a_t src_color = src_buf_al88[x];
                        src_color.alpha = LV_OPA_MIX2(src_color.alpha, mask_buf[x]);
                        lv_color_16a_16a_mix(src_color, &dest_buf_al88[x], &cache);
                    }
                    dest_buf_al88 = drawbuf_next_row(dest_buf_al88, dest_stride);
                    src_buf_al88 = drawbuf_next_row(src_buf_al88, src_stride);
                    mask_buf += mask_stride;
                }
            }
        }
        else if(mask_buf && opa < LV_OPA_MAX) {
            if(LV_RESULT_INVALID == LV_DRAW_SW_AL88_BLEND_NORMAL_TO_AL88_MIX_MASK_OPA(dsc)) {
                for(y = 0; y < h; y++) {
                    for(x = 0; x < w; x++) {
                        lv_color16a_t src_color = src_buf_al88[x];
                        src_color.alpha = LV_OPA_MIX3(src_color.alpha, mask_buf[x], opa);
                        lv_color_16a_16a_mix(src_color, &dest_buf_al88[x], &cache);
                    }
                    dest_buf_al88 = drawbuf_next_row(dest_buf_al88, dest_stride);
                    src_buf_al88 = drawbuf_next_row(src_buf_al88, src_stride);
                    mask_buf += mask_stride;
                }
            }
        }
    }
    else {
        for(y = 0; y < h; y++) {
            for(x = 0; x < w; x++) {
                lv_color16a_t src_color = src_buf_al88[x];
                if(mask_buf == NULL) src_color.alpha = LV_OPA_MIX2(src_color.alpha, opa);
                else src_color.alpha = LV_OPA_MIX3(src_color.alpha, mask_buf[x], opa);
                blend_non_normal_pixel(&dest_buf_al88[x], src_color, dsc->blend_mode, &cache);
            }
            if(mask_buf) mask_buf += mask_stride;
            dest_buf_al88 = drawbuf_next_row(dest_buf_al88, dest_stride);
            src_buf_al88 = drawbuf_next_row(src_buf_al88, src_stride);
        }
    }
}

#if LV_DRAW_SW_SUPPORT_RGB565

static void LV_ATTRIBUTE_FAST_MEM rgb565_image_blend(lv_draw_sw_blend_image_dsc_t * dsc)
{
    int32_t w = dsc->dest_w;
    int32_t h = dsc->dest_h;
    lv_opa_t opa = dsc->opa;
    lv_color16a_t * dest_buf_al88 = dsc->dest_buf;
    int32_t dest_stride = dsc->dest_stride;
    const lv_color16_t * src_buf_c16 = (const lv_color16_t *)dsc->src_buf;
    int32_t src_stride = dsc->src_stride;
    const lv_opa_t * mask_buf = dsc->mask_buf;
    int32_t mask_stride = dsc->mask_stride;

    lv_color_mix_alpha_cache_t cache;
    lv_color_mix_with_alpha_cache_init(&cache);

    int32_t x, y;

    if(dsc->blend_mode == LV_BLEND_MODE_NORMAL) {
        if(mask_buf == NULL && opa >= LV_OPA_MAX) {
            if(LV_RESULT_INVALID == LV_DRAW_SW_RGB565_BLEND_NORMAL_TO_AL88(dsc)) {
                for(y = 0; y < h; y++) {
                    for(x = 0; x < w; x++) {
                        dest_buf_al88[x].lumi = lv_color16_luminance(src_buf_c16[x]);
                        dest_buf_al88[x].alpha = 255;
                    }
                    dest_buf_al88 = drawbuf_next_row(dest_buf_al88, dest_stride);
                    src_buf_c16 = drawbuf_next_row(src_buf_c16, src_stride);
                }
            }
        }
        else if(mask_buf == NULL && opa < LV_OPA_MAX) {
            if(LV_RESULT_INVALID == LV_DRAW_SW_RGB565_BLEND_NORMAL_TO_AL88_WITH_OPA(dsc, dest_px_size)) {
                for(y = 0; y < h; y++) {
                    for(x = 0; x < w; x++) {
                        lv_color16a_t src_color;
                        src_color.lumi = lv_color16_luminance(src_buf_c16[x]);
                        src_color.alpha = opa;
                        lv_color_16a_16a_mix(src_color, &dest_buf_al88[x], &cache);
                    }
                    dest_buf_al88 = drawbuf_next_row(dest_buf_al88, dest_stride);
                    src_buf_c16 = drawbuf_next_row(src_buf_c16, src_stride);
                }
            }
        }
        else if(mask_buf && opa >= LV_OPA_MAX) {
            if(LV_RESULT_INVALID == LV_DRAW_SW_RGB565_BLEND_NORMAL_TO_AL88_WITH_MASK(dsc, dest_px_size)) {
                for(y = 0; y < h; y++) {
                    for(x = 0; x < w; x++) {
                        lv_color16a_t src_color;
                        src_color.lumi = lv_color16_luminance(src_buf_c16[x]);
                        src_color.alpha = mask_buf[x];
                        lv_color_16a_16a_mix(src_color, &dest_buf_al88[x], &cache);
                    }
                    dest_buf_al88 = drawbuf_next_row(dest_buf_al88, dest_stride);
                    src_buf_c16 = drawbuf_next_row(src_buf_c16, src_stride);
                    mask_buf += mask_stride;
                }
            }
        }
        else {
            if(LV_RESULT_INVALID == LV_DRAW_SW_RGB565_BLEND_NORMAL_TO_AL88_MIX_MASK_OPA(dsc, dest_px_size)) {
                for(y = 0; y < h; y++) {
                    for(x = 0; x < w; x++) {
                        lv_color16a_t src_color;
                        src_color.lumi = lv_color16_luminance(src_buf_c16[x]);
                        src_color.alpha = LV_OPA_MIX2(mask_buf[x], opa);
                        lv_color_16a_16a_mix(src_color, &dest_buf_al88[x], &cache);
                    }
                    dest_buf_al88 = drawbuf_next_row(dest_buf_al88, dest_stride);
                    src_buf_c16 = drawbuf_next_row(src_buf_c16, src_stride);
                    mask_buf += mask_stride;
                }
            }
        }
    }
    else {
        for(y = 0; y < h; y++) {
            for(x = 0; x < w; x++) {
                lv_color16a_t src_color;
                src_color.lumi = lv_color16_luminance(src_buf_c16[x]);
                if(mask_buf == NULL) src_color.alpha = opa;
                else src_color.alpha = LV_OPA_MIX2(mask_buf[x], opa);
                blend_non_normal_pixel(&dest_buf_al88[x], src_color, dsc->blend_mode, &cache);
            }
            if(mask_buf) mask_buf += mask_stride;
            dest_buf_al88 = drawbuf_next_row(dest_buf_al88, dest_stride);
            src_buf_c16 = drawbuf_next_row(src_buf_c16, src_stride);
        }
    }
}

#endif

#if LV_DRAW_SW_SUPPORT_RGB888

static void LV_ATTRIBUTE_FAST_MEM rgb888_image_blend(lv_draw_sw_blend_image_dsc_t * dsc,
                                                     const uint8_t src_px_size)
{
    int32_t w = dsc->dest_w;
    int32_t h = dsc->dest_h;
    lv_opa_t opa = dsc->opa;
    lv_color16a_t * dest_buf_al88 = dsc->dest_buf;
    int32_t dest_stride = dsc->dest_stride;
    const uint8_t * src_buf_u8 = dsc->src_buf;
    int32_t src_stride = dsc->src_stride;
    const lv_opa_t * mask_buf = dsc->mask_buf;
    int32_t mask_stride = dsc->mask_stride;

    lv_color_mix_alpha_cache_t cache;
    lv_color_mix_with_alpha_cache_init(&cache);

    int32_t dest_x;
    int32_t src_x;
    int32_t y;

    if(dsc->blend_mode == LV_BLEND_MODE_NORMAL) {
        /*Special case*/
        if(mask_buf == NULL && opa >= LV_OPA_MAX) {
            if(LV_RESULT_INVALID == LV_DRAW_SW_RGB888_BLEND_NORMAL_TO_AL88(dsc, src_px_size)) {
                for(y = 0; y < h; y++) {
                    for(dest_x = 0, src_x = 0; dest_x < w; dest_x++, src_x += src_px_size) {
                        dest_buf_al88[dest_x].lumi = lv_color24_luminance(&src_buf_u8[src_x]);
                        dest_buf_al88[dest_x].alpha = 255;
                    }
                    dest_buf_al88 = drawbuf_next_row(dest_buf_al88, dest_stride);
                    src_buf_u8 += src_stride;
                }
            }
        }
        if(mask_buf == NULL && opa < LV_OPA_MAX) {
            if(LV_RESULT_INVALID == LV_DRAW_SW_RGB888_BLEND_NORMAL_TO_AL88_WITH_OPA(dsc, dest_px_size, src_px_size)) {
                for(y = 0; y < h; y++) {
                    for(dest_x = 0, src_x = 0; dest_x < w; dest_x++, src_x += src_px_size) {
                        lv_color16a_t src_color;
                        src_color.lumi = lv_color24_luminance(&src_buf_u8[src_x]);
                        src_color.alpha = opa;
                        lv_color_16a_16a_mix(src_color, &dest_buf_al88[dest_x], &cache);
                    }
                    dest_buf_al88 = drawbuf_next_row(dest_buf_al88, dest_stride);
                    src_buf_u8 += src_stride;
                }
            }
        }
        if(mask_buf && opa >= LV_OPA_MAX) {
            if(LV_RESULT_INVALID == LV_DRAW_SW_RGB888_BLEND_NORMAL_TO_AL88_WITH_MASK(dsc, dest_px_size, src_px_size)) {
                uint32_t mask_x;
                for(y = 0; y < h; y++) {
                    for(mask_x = 0, dest_x = 0, src_x = 0; dest_x < w; mask_x++, dest_x++, src_x += src_px_size) {
                        lv_color16a_t src_color;
                        src_color.lumi = lv_color24_luminance(&src_buf_u8[src_x]);
                        src_color.alpha = mask_buf[mask_x];
                        lv_color_16a_16a_mix(src_color, &dest_buf_al88[dest_x], &cache);
                    }
                    dest_buf_al88 = drawbuf_next_row(dest_buf_al88, dest_stride);
                    src_buf_u8 += src_stride;
                    mask_buf += mask_stride;
                }
            }
        }
        if(mask_buf && opa < LV_OPA_MAX) {
            if(LV_RESULT_INVALID == LV_DRAW_SW_RGB888_BLEND_NORMAL_TO_AL88_MIX_MASK_OPA(dsc, dest_px_size, src_px_size)) {
                uint32_t mask_x;
                for(y = 0; y < h; y++) {
                    for(mask_x = 0, dest_x = 0, src_x = 0; dest_x < w; mask_x++, dest_x++, src_x += src_px_size) {
                        lv_color16a_t src_color;
                        src_color.lumi = lv_color24_luminance(&src_buf_u8[src_x]);
                        src_color.alpha = LV_OPA_MIX2(mask_buf[mask_x], opa);
                        lv_color_16a_16a_mix(src_color, &dest_buf_al88[dest_x], &cache);
                    }
                    dest_buf_al88 = drawbuf_next_row(dest_buf_al88, dest_stride);
                    src_buf_u8 += src_stride;
                    mask_buf += mask_stride;
                }
            }
        }
    }
    else {
        for(y = 0; y < h; y++) {
            for(dest_x = 0, src_x = 0; dest_x < w; dest_x++, src_x += src_px_size) {
                lv_color16a_t src_color;
                src_color.lumi = lv_color24_luminance(&src_buf_u8[src_x]);
                if(mask_buf == NULL) src_color.alpha = opa;
                else src_color.alpha = LV_OPA_MIX2(mask_buf[dest_x], opa);

                blend_non_normal_pixel(&dest_buf_al88[dest_x], src_color, dsc->blend_mode, &cache);
            }
            if(mask_buf) mask_buf += mask_stride;
            dest_buf_al88 = drawbuf_next_row(dest_buf_al88, dest_stride);
            src_buf_u8 += src_stride;
        }
    }
}

#endif

#if LV_DRAW_SW_SUPPORT_ARGB8888

static void LV_ATTRIBUTE_FAST_MEM argb8888_image_blend(lv_draw_sw_blend_image_dsc_t * dsc)
{
    int32_t w = dsc->dest_w;
    int32_t h = dsc->dest_h;
    lv_opa_t opa = dsc->opa;
    lv_color16a_t * dest_buf_al88 = dsc->dest_buf;
    int32_t dest_stride = dsc->dest_stride;
    const lv_color32_t * src_buf_c32 = dsc->src_buf;
    int32_t src_stride = dsc->src_stride;
    const lv_opa_t * mask_buf = dsc->mask_buf;
    int32_t mask_stride = dsc->mask_stride;

    lv_color_mix_alpha_cache_t cache;
    lv_color_mix_with_alpha_cache_init(&cache);

    int32_t x;
    int32_t y;

    if(dsc->blend_mode == LV_BLEND_MODE_NORMAL) {
        if(mask_buf == NULL && opa >= LV_OPA_MAX) {
            if(LV_RESULT_INVALID == LV_DRAW_SW_ARGB8888_BLEND_NORMAL_TO_AL88(dsc)) {
                for(y = 0; y < h; y++) {
                    for(x = 0; x < w; x++) {
                        lv_color16a_t src_color;
                        src_color.lumi = lv_color32_luminance(src_buf_c32[x]);
                        src_color.alpha = src_buf_c32[x].alpha;
                        lv_color_16a_16a_mix(src_color, &dest_buf_al88[x], &cache);
                    }
                    dest_buf_al88 = drawbuf_next_row(dest_buf_al88, dest_stride);
                    src_buf_c32 = drawbuf_next_row(src_buf_c32, src_stride);
                }
            }
        }
        else if(mask_buf == NULL && opa < LV_OPA_MAX) {
            if(LV_RESULT_INVALID == LV_DRAW_SW_ARGB8888_BLEND_NORMAL_TO_AL88_WITH_OPA(dsc)) {
                for(y = 0; y < h; y++) {
                    for(x = 0; x < w; x++) {
                        lv_color16a_t src_color;
                        src_color.lumi = lv_color32_luminance(src_buf_c32[x]);
                        src_color.alpha = LV_OPA_MIX2(src_buf_c32[x].alpha, opa);
                        lv_color_16a_16a_mix(src_color, &dest_buf_al88[x], &cache);
                    }
                    dest_buf_al88 = drawbuf_next_row(dest_buf_al88, dest_stride);
                    src_buf_c32 = drawbuf_next_row(src_buf_c32, src_stride);
                }
            }
        }
        else if(mask_buf && opa >= LV_OPA_MAX) {
            if(LV_RESULT_INVALID == LV_DRAW_SW_ARGB8888_BLEND_NORMAL_TO_AL88_WITH_MASK(dsc)) {
                for(y = 0; y < h; y++) {
                    for(x = 0; x < w; x++) {
                        lv_color16a_t src_color;
                        src_color.lumi = lv_color32_luminance(src_buf_c32[x]);
                        src_color.alpha = LV_OPA_MIX2(src_buf_c32[x].alpha, mask_buf[x]);
                        lv_color_16a_16a_mix(src_color, &dest_buf_al88[x], &cache);
                    }
                    dest_buf_al88 = drawbuf_next_row(dest_buf_al88, dest_stride);
                    src_buf_c32 = drawbuf_next_row(src_buf_c32, src_stride);
                    mask_buf += mask_stride;
                }
            }
        }
        else if(mask_buf && opa < LV_OPA_MAX) {
            if(LV_RESULT_INVALID == LV_DRAW_SW_ARGB8888_BLEND_NORMAL_TO_AL88_MIX_MASK_OPA(dsc)) {
                for(y = 0; y < h; y++) {
                    for(x = 0; x < w; x++) {
                        lv_color16a_t src_color;
                        src_color.lumi = lv_color32_luminance(src_buf_c32[x]);
                        src_color.alpha = LV_OPA_MIX3(src_buf_c32[x].alpha, mask_buf[x], opa);
                        lv_color_16a_16a_mix(src_color, &dest_buf_al88[x], &cache);
                    }
                    dest_buf_al88 = drawbuf_next_row(dest_buf_al88, dest_stride);
                    src_buf_c32 = drawbuf_next_row(src_buf_c32, src_stride);
                    mask_buf += mask_stride;
                }
            }
        }
    }
    else {
        for(y = 0; y < h; y++) {
            for(x = 0; x < w; x++) {
                lv_color16a_t src_color;
                src_color.lumi = lv_color32_luminance(src_buf_c32[x]);
                src_color.alpha = src_buf_c32[x].alpha;
                if(mask_buf == NULL) src_color.alpha = LV_OPA_MIX2(src_color.alpha, opa);
                else src_color.alpha = LV_OPA_MIX3(src_color.alpha, mask_buf[x], opa);
                blend_non_normal_pixel(&dest_buf_al88[x], src_color, dsc->blend_mode, &cache);
            }
            if(mask_buf) mask_buf += mask_stride;
            dest_buf_al88 = drawbuf_next_row(dest_buf_al88, dest_stride);
            src_buf_c32 = drawbuf_next_row(src_buf_c32, src_stride);
        }
    }
}

#endif

/**
 * Check if two AL88 colors are equal
 * @param c1    the first color
 * @param c2    the second color
 * @return      true: equal
 */
static inline bool lv_color16a_eq(lv_color16a_t c1, lv_color16a_t c2)
{
    return *((uint16_t *)&c1) == *((uint16_t *)&c2);
}

static inline lv_color16a_t LV_ATTRIBUTE_FAST_MEM lv_color_mix16a(lv_color16a_t fg, lv_color16a_t bg)
{
#if 0
    if(fg.alpha >= LV_OPA_MAX) {
        fg.alpha = bg.alpha;
        return fg;
    }
    if(fg.alpha <= LV_OPA_MIN) {
        return bg;
    }
#endif
    bg.lumi = (uint32_t)((uint32_t)fg.lumi * fg.alpha + (uint32_t)bg.lumi * (255 - fg.alpha)) >> 8;
    return bg;
}

static inline void LV_ATTRIBUTE_FAST_MEM lv_color_16a_16a_mix(lv_color16a_t fg, lv_color16a_t * bg,
                                                              lv_color_mix_alpha_cache_t * cache)
{
    /*Pick the foreground if it's fully opaque or the Background is fully transparent*/
    if(fg.alpha >= LV_OPA_MAX || bg->alpha <= LV_OPA_MIN) {
        *bg = fg;
    }
    /*Transparent foreground: use the Background*/
    else if(fg.alpha <= LV_OPA_MIN) {
        /* no need to copy */
    }
    /*Opaque background: use simple mix*/
    else if(bg->alpha == 255) {
        *bg = lv_color_mix16a(fg, *bg);
    }
    /*Both colors have alpha. Expensive calculation needs to be applied*/
    else {
        /*Save the parameters and the result. If they will be asked again don't compute again*/

        /*Update the ratio and the result alpha value if the input alpha values change*/
        if(bg->alpha != cache->bg_saved.alpha || fg.alpha != cache->fg_saved.alpha) {
            /*Info:
             * https://en.wikipedia.org/wiki/Alpha_compositing#Analytical_derivation_of_the_over_operator*/
            cache->res_alpha_saved = 255 - LV_OPA_MIX2(255 - fg.alpha, 255 - bg->alpha);
            LV_ASSERT(cache->res_alpha_saved != 0);
            cache->ratio_saved = (uint32_t)((uint32_t)fg.alpha * 255) / cache->res_alpha_saved;
        }

        if(!lv_color16a_eq(*bg, cache->bg_saved) || !lv_color16a_eq(fg, cache->fg_saved)) {
            cache->fg_saved = fg;
            cache->bg_saved = *bg;
            fg.alpha = cache->ratio_saved;
            cache->res_saved = lv_color_mix16a(fg, *bg);
            cache->res_saved.alpha = cache->res_alpha_saved;
        }

        *bg = cache->res_saved;
    }
}

void lv_color_mix_with_alpha_cache_init(lv_color_mix_alpha_cache_t * cache)
{
    lv_memzero(&cache->fg_saved, sizeof(lv_color16a_t));
    lv_memzero(&cache->bg_saved, sizeof(lv_color16a_t));
    lv_memzero(&cache->res_saved, sizeof(lv_color16a_t));
    cache->res_alpha_saved = 255;
    cache->ratio_saved = 255;
}

#if LV_DRAW_SW_SUPPORT_I1

static inline uint8_t LV_ATTRIBUTE_FAST_MEM get_bit(const uint8_t * buf, int32_t bit_idx)
{
    return (buf[bit_idx / 8] >> (7 - (bit_idx % 8))) & 1;
}

#endif

static inline void LV_ATTRIBUTE_FAST_MEM blend_non_normal_pixel(lv_color16a_t * dest, lv_color16a_t src,
                                                                lv_blend_mode_t mode, lv_color_mix_alpha_cache_t * cache)
{
    lv_color16a_t res;
    switch(mode) {
        case LV_BLEND_MODE_ADDITIVE:
            res.lumi = LV_MIN(dest->lumi + src.lumi, 255);
            break;
        case LV_BLEND_MODE_SUBTRACTIVE:
            res.lumi = LV_MAX(dest->lumi - src.lumi, 0);
            break;
        case LV_BLEND_MODE_MULTIPLY:
            res.lumi = (dest->lumi * src.lumi) >> 8;
            break;
        default:
            LV_LOG_WARN("Not supported blend mode: %d", mode);
            return;
    }
    res.alpha = src.alpha;
    lv_color_16a_16a_mix(res, dest, cache);
}

static inline void * LV_ATTRIBUTE_FAST_MEM drawbuf_next_row(const void * buf, uint32_t stride)
{
    return (void *)((uint8_t *)buf + stride);
}

#endif

#endif
