/**
* @file lv_image_header_cache.c
*
 */

/*********************
 *      INCLUDES
 *********************/

#include "../../draw/lv_image_decoder_private.h"
#include "../lv_assert.h"
#include "../../core/lv_global.h"

#include "lv_image_header_cache.h"

/*********************
 *      DEFINES
 *********************/

#define CACHE_NAME  "IMAGE_HEADER"

#define img_header_cache_p (LV_GLOBAL_DEFAULT()->img_header_cache)

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static lv_cache_compare_res_t image_header_cache_compare_cb(const lv_image_header_cache_data_t * lhs,
                                                            const lv_image_header_cache_data_t * rhs);
static void image_header_cache_free_cb(lv_image_header_cache_data_t * entry, void * user_data);

/**********************
 *  GLOBAL VARIABLES
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

lv_result_t lv_image_header_cache_init(uint32_t count)
{
    if(img_header_cache_p != NULL) {
        return LV_RESULT_OK;
    }

    img_header_cache_p = lv_cache_create(&lv_cache_class_lru_rb_count,
    sizeof(lv_image_header_cache_data_t), count, (lv_cache_ops_t) {
        .compare_cb = (lv_cache_compare_cb_t) image_header_cache_compare_cb,
        .create_cb = NULL,
        .free_cb = (lv_cache_free_cb_t) image_header_cache_free_cb
    });

    lv_cache_set_name(img_header_cache_p, CACHE_NAME);
    return img_header_cache_p != NULL ? LV_RESULT_OK : LV_RESULT_INVALID;
}

void lv_image_header_cache_resize(uint32_t count, bool evict_now)
{
    lv_cache_set_max_size(img_header_cache_p, count, NULL);
    if(evict_now) {
        lv_cache_reserve(img_header_cache_p, count, NULL);
    }
}

void lv_image_header_cache_drop(const void * src)
{
    if(src == NULL) {
        lv_cache_drop_all(img_header_cache_p, NULL);
        return;
    }

    lv_image_header_cache_data_t search_key = {
        .src = src,
        .src_type = lv_image_src_get_type(src),
    };

    lv_cache_drop(img_header_cache_p, &search_key, NULL);
}

bool lv_image_header_cache_is_enabled(void)
{
    return lv_cache_is_enabled(img_header_cache_p);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

inline static lv_cache_compare_res_t image_cache_common_compare(const void * lhs_src, lv_image_src_t lhs_src_type,
                                                                const void * rhs_src, lv_image_src_t rhs_src_type)
{
    if(lhs_src_type == rhs_src_type) {
        if(lhs_src_type == LV_IMAGE_SRC_FILE) {
            int32_t cmp_res = lv_strcmp(lhs_src, rhs_src);
            if(cmp_res != 0) {
                return cmp_res > 0 ? 1 : -1;
            }
        }
        else if(lhs_src_type == LV_IMAGE_SRC_VARIABLE) {
            if(lhs_src != rhs_src) {
                return lhs_src > rhs_src ? 1 : -1;
            }
        }
        return 0;
    }
    return lhs_src_type > rhs_src_type ? 1 : -1;
}

static lv_cache_compare_res_t image_header_cache_compare_cb(
    const lv_image_header_cache_data_t * lhs,
    const lv_image_header_cache_data_t * rhs)
{
    return image_cache_common_compare(lhs->src, lhs->src_type, rhs->src, rhs->src_type);
}

static void image_header_cache_free_cb(lv_image_header_cache_data_t * entry, void * user_data)
{
    LV_UNUSED(user_data); /*Unused*/

    if(entry->src_type == LV_IMAGE_SRC_FILE) lv_free((void *)entry->src);
}
