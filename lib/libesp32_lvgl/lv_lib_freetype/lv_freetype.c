/**
 * @file lv_freetype.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#include "lv_freetype.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
  
#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

/*
 * FreeType requires up to 32KB of stack to run, which overflows the stack of 8KB.
 *
 * We delegate to a FreeRTOS sub-task with a bigger stack.
 * 
 * Parameters are passed via a RequestQueue, and response back via ResponseQueue
 * 
 * The function that uses this scheme is `get_glyph_dsc_cb()``
 *
 */

QueueHandle_t FTRequestQueue;
QueueHandle_t FTResponseQueue;
TaskHandle_t  FTTaskHandle;
void FT_loop_task(void *pvParameters);

typedef struct FT_glyph_dsc_request {
    const lv_font_t * font;
    lv_font_glyph_dsc_t * dsc_out;
    uint32_t unicode_letter;
    uint32_t unicode_letter_next;
} FT_glyph_dsc_request;

typedef bool FT_glyph_dsc_response;


/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static FT_Face face_find_in_list(lv_ft_info_t *info);
static void face_add_to_list(FT_Face face);
static void face_remove_from_list(FT_Face face);

static void face_generic_finalizer(void* object);
static bool get_glyph_dsc_cb(const lv_font_t * font, lv_font_glyph_dsc_t * dsc_out,
    uint32_t unicode_letter, uint32_t unicode_letter_next);
static const uint8_t * get_glyph_bitmap_cb(const lv_font_t * font, uint32_t unicode_letter);

#if LV_USE_FT_CACHE_MANAGER
static FT_Error font_face_requester(FTC_FaceID face_id,
    FT_Library library_is,FT_Pointer req_data,FT_Face* aface);
#endif

 /**********************
 *  STATIC VARIABLES
 **********************/
static FT_Library library;

#if LV_USE_FT_CACHE_MANAGER
static FTC_Manager cache_manager;
static FTC_CMapCache cmap_cache;
static FTC_SBitCache sbit_cache;
static FTC_SBit sbit;
#endif

static lv_faces_control_t face_control;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
bool lv_freetype_init(FT_UInt max_faces, FT_UInt max_sizes, FT_ULong max_bytes)
{
    face_control.cnt = 0;
    face_control.num = max_faces;
    _lv_ll_init(&face_control.face_ll, sizeof(FT_Face *));

    FT_Error error = FT_Init_FreeType(&library);
    if (error) {
        LV_LOG_ERROR("init freeType error(%d)", error);
        return false;
    }

#if LV_USE_FT_CACHE_MANAGER
    error = FTC_Manager_New(library, max_faces, max_sizes,
        max_bytes, font_face_requester, NULL, &cache_manager);
    if (error) {
        FT_Done_FreeType(library);
        LV_LOG_ERROR("Failed to open cache manager");
        return false;
    }

    error = FTC_CMapCache_New(cache_manager, &cmap_cache);
    if(error) {
        LV_LOG_ERROR("Failed to open Cmap Cache");
        goto Fail;
    }

    error = FTC_SBitCache_New(cache_manager, &sbit_cache);
    if(error){
        LV_LOG_ERROR("Failed to open sbit cache");
        goto Fail;
    }

    // initialize the queues to send request and receive response
    FTRequestQueue = xQueueCreate(1, sizeof(FT_glyph_dsc_request));
    FTResponseQueue = xQueueCreate(1, sizeof(FT_glyph_dsc_response));

    xTaskCreatePinnedToCore(FT_loop_task, "FreeType_task", LV_USE_FT_STACK_SIZE, NULL, 1, &FTTaskHandle, ARDUINO_RUNNING_CORE);

    if (FTRequestQueue && FTResponseQueue) {
        return true;
    }
Fail:
    FTC_Manager_Done(cache_manager);
    FT_Done_FreeType(library);
    return false;
#else
    LV_UNUSED(max_sizes);
    LV_UNUSED(max_bytes);
    return true;
#endif/* LV_USE_FT_CACHE_MANAGER */
}

void lv_freetype_destroy(void)
{
#if LV_USE_FT_CACHE_MANAGER
    FTC_Manager_Done(cache_manager);
#endif
    FT_Done_FreeType(library);
}

bool lv_ft_font_init(lv_ft_info_t *info)
{
    lv_font_fmt_ft_dsc_t * dsc = lv_mem_alloc(sizeof(lv_font_fmt_ft_dsc_t));
    if(dsc == NULL) return false;

    dsc->font = lv_mem_alloc(sizeof(lv_font_t));
    if(dsc->font == NULL) {
        lv_mem_free(dsc);
        return false;
    }

    lv_face_info_t *face_info = NULL;
    FT_Face face = face_find_in_list(info);
    if (face == NULL) {
        if (face_control.cnt == face_control.num - 1) {
            LV_LOG_WARN("face full");
            goto Fail;
        }
        face_info = lv_mem_alloc(sizeof(lv_face_info_t) + strlen(info->name) + 1);
        if(face_info == NULL) {
            goto Fail;
        }
        FT_Error error = FT_New_Face(library, info->name, 0, &face);
        if(error){
            lv_mem_free(face_info);
            LV_LOG_WARN("create face error(%d)", error);
            goto Fail;
        }

        face_info->name = ((char *)face_info) + sizeof(lv_face_info_t);
        strcpy(face_info->name, info->name);
        face_info->cnt = 1;
        face->generic.data = face_info;
        face->generic.finalizer = face_generic_finalizer;
        face_add_to_list(face);
    }
    else {
#if LV_USE_FT_CACHE_MANAGER == 0
        FT_Size size;
        FT_Error error = FT_New_Size(face, &size);
        if (error) {
            goto Fail;
        }
        FT_Activate_Size(size);
        FT_Reference_Face(face);
#else
    face_info = (lv_face_info_t *)face->generic.data;
    face_info->cnt++;
#endif
    }
    FT_Set_Pixel_Sizes(face, 0, info->weight);

    dsc->face = face;
    dsc->size = face->size;
    dsc->weight = info->weight;
    dsc->style = info->style;
    lv_font_t *font = dsc->font;
    font->user_data = dsc;
    font->get_glyph_dsc = get_glyph_dsc_cb;
    font->get_glyph_bitmap = get_glyph_bitmap_cb;
    font->line_height = (dsc->face->size->metrics.height >> 6);
    font->base_line = -(dsc->face->size->metrics.descender >> 6);
    font->subpx = LV_FONT_SUBPX_NONE;
    font->underline_position = dsc->face->underline_position;
    font->underline_thickness = dsc->face->underline_thickness;
    font->dsc = NULL;
    info->font = font;
    return true;

Fail:
    lv_mem_free(dsc->font);
    lv_mem_free(dsc);
    return false;
}

void lv_ft_font_destroy(lv_font_t* font)
{
    if (font == NULL) {
        return;
    }

    lv_font_fmt_ft_dsc_t * dsc = (lv_font_fmt_ft_dsc_t *)(font->user_data);
    if (dsc) {
#if LV_USE_FT_CACHE_MANAGER == 0
        FT_Done_Size(dsc->size);
        FT_Done_Face(dsc->face);
#else
        lv_face_info_t *face_info = (lv_face_info_t *)dsc->face->generic.data;
        face_info->cnt--;
        if(face_info->cnt == 0){
            FTC_Manager_RemoveFaceID(cache_manager, (FTC_FaceID)dsc->face);
        }
#endif
        lv_mem_free(dsc->font);
        lv_mem_free(dsc);
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
static void face_generic_finalizer(void* object)
{
    FT_Face face = (FT_Face)object;
    face_remove_from_list(face);
    if(face->generic.data){
        lv_face_info_t *face_info = (lv_face_info_t *)face->generic.data;
        lv_mem_free(face_info);
    }
    LV_LOG_INFO("face finalizer(%p)\n", face);
}

static FT_Face face_find_in_list(lv_ft_info_t *info)
{
    lv_face_info_t *face_info;
    FT_Face *pface = _lv_ll_get_head(&face_control.face_ll);
    while(pface) {
        face_info = (lv_face_info_t *)(*pface)->generic.data;
        if (strcmp(face_info->name, info->name) == 0) {
            return *pface;
        }
        pface = _lv_ll_get_next(&face_control.face_ll, pface);
    }

    return NULL;
}

static void face_add_to_list(FT_Face face)
{
    FT_Face *pface;
    pface = (FT_Face *)_lv_ll_ins_tail(&face_control.face_ll);
    *pface = face;
    face_control.cnt++;
}

static void face_remove_from_list(FT_Face face)
{
    FT_Face *pface = _lv_ll_get_head(&face_control.face_ll);
    while(pface) {
        if (*pface == face) {
            _lv_ll_remove(&face_control.face_ll, pface);
            lv_mem_free(pface);
            face_control.cnt--;
            break;
        }
        pface = _lv_ll_get_next(&face_control.face_ll, pface);
    }
}

#if LV_USE_FT_CACHE_MANAGER

static FT_Error font_face_requester(FTC_FaceID face_id,
    FT_Library library_is,FT_Pointer req_data,FT_Face* aface)
{
    LV_UNUSED(library_is);
    LV_UNUSED(req_data);
    *aface = face_id;
    return FT_Err_Ok;
}

static bool get_glyph_dsc_cb_cache(const lv_font_t * font, 
    lv_font_glyph_dsc_t * dsc_out, uint32_t unicode_letter, uint32_t unicode_letter_next)
{
    LV_UNUSED(unicode_letter_next);
    if(unicode_letter < 0x20) {
        dsc_out->adv_w = 0;
        dsc_out->box_h = 0;
        dsc_out->box_w = 0;
        dsc_out->ofs_x = 0;
        dsc_out->ofs_y = 0;
        dsc_out->bpp = 0;
        return true;
    }

    lv_font_fmt_ft_dsc_t * dsc = (lv_font_fmt_ft_dsc_t *)(font->user_data);
    FT_Face face = dsc->face;

    static FTC_ImageTypeRec desc_sbit_type;
    desc_sbit_type.face_id = (FTC_FaceID)face;
    desc_sbit_type.flags = FT_LOAD_RENDER | FT_LOAD_TARGET_NORMAL;
    desc_sbit_type.height = dsc->weight;
    desc_sbit_type.width = dsc->weight;
    FT_UInt charmap_index = FT_Get_Charmap_Index(face->charmap);
    FT_UInt glyph_index = FTC_CMapCache_Lookup(cmap_cache, (FTC_FaceID)face, charmap_index, unicode_letter);
    FT_Error error = FTC_SBitCache_Lookup(sbit_cache, &desc_sbit_type, glyph_index, &sbit, NULL);

    dsc_out->adv_w = sbit->xadvance;
    dsc_out->box_h = sbit->height;  /*Height of the bitmap in [px]*/
    dsc_out->box_w = sbit->width;   /*Width of the bitmap in [px]*/
    dsc_out->ofs_x = sbit->left;    /*X offset of the bitmap in [pf]*/
    dsc_out->ofs_y = sbit->top - sbit->height; /*Y offset of the bitmap measured from the as line*/
    dsc_out->bpp = 8;               /*Bit per pixel: 1/2/4/8*/

    return true;
}

static const uint8_t * get_glyph_bitmap_cb_cache(const lv_font_t * font, uint32_t unicode_letter)
{
    LV_UNUSED(font);
    LV_UNUSED(unicode_letter);
    return (const uint8_t *)sbit->buffer;
}

#else/* LV_USE_FT_CACHE_MANAGER */
// extern void berry_log_C(const char * berry_buf, ...);

static bool get_glyph_dsc_cb_nocache(const lv_font_t * font, 
    lv_font_glyph_dsc_t * dsc_out, uint32_t unicode_letter, uint32_t unicode_letter_next)
{
    // berry_log_C(">> get_glyph_dsc_cb_nocache %i %i", unicode_letter, unicode_letter_next);
    LV_UNUSED(unicode_letter_next);
    if(unicode_letter < 0x20) {
        dsc_out->adv_w = 0;
        dsc_out->box_h = 0;
        dsc_out->box_w = 0;
        dsc_out->ofs_x = 0;
        dsc_out->ofs_y = 0;
        dsc_out->bpp = 0;
        return true;
    }

    FT_Error error;
    FT_Face face;
    lv_font_fmt_ft_dsc_t * dsc = (lv_font_fmt_ft_dsc_t *)(font->user_data);
    face = dsc->face;

    FT_UInt glyph_index = FT_Get_Char_Index( face, unicode_letter );

    if (face->size != dsc->size) {
        // berry_log_C(">> FT_Activate_Size %i", dsc->size);
        FT_Activate_Size(dsc->size);
    }

    error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT );
    // berry_log_C(">> after FT_Load_Glyph error = %i", error);
    if (error){
        return false;
    }

    error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL);
    // berry_log_C(">> after FT_Render_Glyph error = %i", error);
    if (error){
        return false;
    }

    dsc_out->adv_w = (face->glyph->metrics.horiAdvance >> 6);
    dsc_out->box_h = face->glyph->bitmap.rows;         /*Height of the bitmap in [px]*/
    dsc_out->box_w = face->glyph->bitmap.width;         /*Width of the bitmap in [px]*/
    dsc_out->ofs_x = face->glyph->bitmap_left;         /*X offset of the bitmap in [pf]*/
    dsc_out->ofs_y = face->glyph->bitmap_top - face->glyph->bitmap.rows;         /*Y offset of the bitmap measured from the as line*/
    dsc_out->bpp = 8;         /*Bit per pixel: 1/2/4/8*/

    // berry_log_C("+++ adv_w %i, h %i, w %i, x %i, y %i", dsc_out->adv_w, dsc_out->box_h, dsc_out->box_w, dsc_out->ofs_x, dsc_out->ofs_y);

    return true;
}

static const uint8_t * get_glyph_bitmap_cb_nocache(const lv_font_t * font, uint32_t unicode_letter)
{
    LV_UNUSED(unicode_letter);
    lv_font_fmt_ft_dsc_t * dsc = (lv_font_fmt_ft_dsc_t *)(font->user_data);
    FT_Face face = dsc->face;
    return (const uint8_t *)(face->glyph->bitmap.buffer);
}

#endif/* LV_USE_FT_CACHE_MANAGER */

static bool get_glyph_dsc_cb(const lv_font_t * font, 
    lv_font_glyph_dsc_t * dsc_out, uint32_t unicode_letter, uint32_t unicode_letter_next)
{
// #if LV_USE_FT_CACHE_MANAGER
//     return get_glyph_dsc_cb_cache(font, dsc_out, unicode_letter, unicode_letter_next);
// #else
//     return get_glyph_dsc_cb_nocache(font, dsc_out, unicode_letter, unicode_letter_next);
// #endif
    static FT_glyph_dsc_request request;
    static FT_glyph_dsc_response response;

    request.font = font;
    request.dsc_out = dsc_out;
    request.unicode_letter = unicode_letter;
    request.unicode_letter_next = unicode_letter_next;
    xQueueSendToBack(FTRequestQueue, &request, portMAX_DELAY);
    if (xQueueReceive(FTResponseQueue, &response, portMAX_DELAY)) {
        return response;
    } else {
        return false;   // should never happen
    }
}

static const uint8_t * get_glyph_bitmap_cb(const lv_font_t * font, uint32_t unicode_letter)
{
#if LV_USE_FT_CACHE_MANAGER
    return get_glyph_bitmap_cb_cache(font, unicode_letter);
#else
    return get_glyph_bitmap_cb_nocache(font, unicode_letter);
#endif
}

void FT_loop_task(void *pvParameters) {
    (void) pvParameters;

    while (1) {
        FT_glyph_dsc_request request;
        FT_glyph_dsc_response response;

        if (xQueueReceive(FTRequestQueue, &request, portMAX_DELAY)) {
            #if LV_USE_FT_CACHE_MANAGER
                response = get_glyph_dsc_cb_cache(request.font, request.dsc_out, request.unicode_letter, request.unicode_letter_next);
            #else
                response = get_glyph_dsc_cb_nocache(request.font, request.dsc_out, request.unicode_letter, request.unicode_letter_next);
            #endif
            xQueueSendToBack(FTResponseQueue, &response, portMAX_DELAY);        // send back response
        }
    }
}