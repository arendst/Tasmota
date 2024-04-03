/**
 * @file lv_span.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_span.h"

#if LV_USE_SPAN != 0

#include "../../misc/lv_assert.h"
#include "../../core/lv_global.h"

/*********************
 *      DEFINES
 *********************/
#define MY_CLASS &lv_spangroup_class
#define snippet_stack LV_GLOBAL_DEFAULT()->span_snippet_stack

/**********************
 *      TYPEDEFS
 **********************/
typedef struct {
    lv_span_t * span;
    const char * txt;
    const lv_font_t * font;
    uint32_t   bytes;
    int32_t txt_w;
    int32_t line_h;
    int32_t letter_space;
} lv_snippet_t;

struct _snippet_stack {
    lv_snippet_t    stack[LV_SPAN_SNIPPET_STACK_SIZE];
    uint32_t        index;
};

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void lv_spangroup_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void lv_spangroup_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj);
static void lv_spangroup_event(const lv_obj_class_t * class_p, lv_event_t * e);
static void draw_main(lv_event_t * e);
static void refresh_self_size(lv_obj_t * obj);

static const lv_font_t * lv_span_get_style_text_font(lv_obj_t * par, lv_span_t * span);
static int32_t lv_span_get_style_text_letter_space(lv_obj_t * par, lv_span_t * span);
static lv_color_t lv_span_get_style_text_color(lv_obj_t * par, lv_span_t * span);
static lv_opa_t lv_span_get_style_text_opa(lv_obj_t * par, lv_span_t * span);
static lv_opa_t lv_span_get_style_text_blend_mode(lv_obj_t * par, lv_span_t * span);
static int32_t lv_span_get_style_text_decor(lv_obj_t * par, lv_span_t * span);

static inline void span_text_check(const char ** text);
static void lv_draw_span(lv_obj_t * obj, lv_layer_t * layer);
static bool lv_text_get_snippet(const char * txt, const lv_font_t * font, int32_t letter_space,
                                int32_t max_width, lv_text_flag_t flag, int32_t * use_width,
                                uint32_t * end_ofs);

static void lv_snippet_clear(void);
static uint32_t lv_get_snippet_count(void);
static void lv_snippet_push(lv_snippet_t * item);
static lv_snippet_t * lv_get_snippet(uint32_t index);
static int32_t convert_indent_pct(lv_obj_t * spans, int32_t width);

/**********************
 *  STATIC VARIABLES
 **********************/

const lv_obj_class_t lv_spangroup_class  = {
    .base_class = &lv_obj_class,
    .constructor_cb = lv_spangroup_constructor,
    .destructor_cb = lv_spangroup_destructor,
    .event_cb = lv_spangroup_event,
    .instance_size = sizeof(lv_spangroup_t),
    .width_def = LV_SIZE_CONTENT,
    .height_def = LV_SIZE_CONTENT,
    .name = "span",
};

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void lv_span_stack_init(void)
{
    struct _snippet_stack * stack = snippet_stack = lv_malloc(sizeof(struct _snippet_stack));
    LV_ASSERT_MALLOC(stack);
    if(!stack) {
        LV_LOG_ERROR("malloc failed for snippet_stack");
    }
}

void lv_span_stack_deinit(void)
{
    lv_free(snippet_stack);
}

lv_obj_t * lv_spangroup_create(lv_obj_t * par)
{
    lv_obj_t * obj = lv_obj_class_create_obj(&lv_spangroup_class, par);
    lv_obj_class_init_obj(obj);
    return obj;
}

lv_span_t * lv_spangroup_new_span(lv_obj_t * obj)
{
    if(obj == NULL) {
        return NULL;
    }

    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_spangroup_t * spans = (lv_spangroup_t *)obj;
    lv_span_t * span = _lv_ll_ins_tail(&spans->child_ll);
    LV_ASSERT_MALLOC(span);

    lv_style_init(&span->style);
    span->txt = (char *)"";
    span->static_flag = 1;
    span->spangroup = obj;

    refresh_self_size(obj);

    return span;
}

void lv_spangroup_delete_span(lv_obj_t * obj, lv_span_t * span)
{
    if(obj == NULL || span == NULL) {
        return;
    }

    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_spangroup_t * spans = (lv_spangroup_t *)obj;
    lv_span_t * cur_span;
    _LV_LL_READ(&spans->child_ll, cur_span) {
        if(cur_span == span) {
            _lv_ll_remove(&spans->child_ll, cur_span);
            if(cur_span->txt && cur_span->static_flag == 0) {
                lv_free(cur_span->txt);
                cur_span->txt = NULL;
            }
            lv_style_reset(&cur_span->style);
            lv_free(cur_span);
            cur_span = NULL;
            break;
        }
    }

    refresh_self_size(obj);
}

/*=====================
 * Setter functions
 *====================*/

void lv_span_set_text(lv_span_t * span, const char * text)
{
    if(span == NULL || text == NULL) {
        return;
    }

    size_t text_alloc_len = lv_strlen(text) + 1;

    if(span->txt == NULL || span->static_flag == 1) {
        span->txt = lv_malloc(text_alloc_len);
        LV_ASSERT_MALLOC(span->txt);
    }
    else {
        span->txt = lv_realloc(span->txt, text_alloc_len);
        LV_ASSERT_MALLOC(span->txt);
    }

    if(span->txt == NULL) return;

    span->static_flag = 0;
    lv_memcpy(span->txt, text, text_alloc_len);

    refresh_self_size(span->spangroup);
}

void lv_span_set_text_static(lv_span_t * span, const char * text)
{
    if(span == NULL || text == NULL) {
        return;
    }

    if(span->txt && span->static_flag == 0) {
        lv_free(span->txt);
        span->txt = NULL;
    }
    span->static_flag = 1;
    span->txt = (char *)text;

    refresh_self_size(span->spangroup);
}

void lv_spangroup_set_align(lv_obj_t * obj, lv_text_align_t align)
{
    lv_obj_set_style_text_align(obj, align, LV_PART_MAIN);
}

void lv_spangroup_set_overflow(lv_obj_t * obj, lv_span_overflow_t overflow)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_spangroup_t * spans = (lv_spangroup_t *)obj;
    if(spans->overflow == overflow) return;
    if(overflow >= _LV_SPAN_OVERFLOW_LAST) return;
    spans->overflow = overflow;
    lv_obj_invalidate(obj);
}

void lv_spangroup_set_indent(lv_obj_t * obj, int32_t indent)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_spangroup_t * spans = (lv_spangroup_t *)obj;
    if(spans->indent == indent) return;

    spans->indent = indent;

    refresh_self_size(obj);
}

void lv_spangroup_set_mode(lv_obj_t * obj, lv_span_mode_t mode)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_spangroup_t * spans = (lv_spangroup_t *)obj;

    if(mode >= _LV_SPAN_MODE_LAST) return;

    spans->mode = mode;
    lv_spangroup_refr_mode(obj);
}

void lv_spangroup_set_max_lines(lv_obj_t * obj, int32_t lines)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_spangroup_t * spans = (lv_spangroup_t *)obj;
    spans->lines = lines;
    lv_spangroup_refr_mode(obj);
}

/*=====================
 * Getter functions
 *====================*/

lv_span_t * lv_spangroup_get_child(const lv_obj_t * obj, int32_t id)
{
    if(obj == NULL) {
        return NULL;
    }

    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_spangroup_t * spans = (lv_spangroup_t *)obj;
    lv_ll_t * linked_list = &spans->child_ll;

    bool traverse_forwards = (id >= 0);
    int32_t cur_idx = 0;
    lv_ll_node_t * cur_node = linked_list->head;

    /*If using a negative index, start from the tail and use cur -1 to indicate the end*/
    if(!traverse_forwards) {
        cur_idx = -1;
        cur_node = linked_list->tail;
    }

    while(cur_node != NULL) {
        if(cur_idx == id) {
            return (lv_span_t *) cur_node;
        }
        if(traverse_forwards) {
            cur_node = (lv_ll_node_t *) _lv_ll_get_next(linked_list, cur_node);
            cur_idx++;
        }
        else {
            cur_node = (lv_ll_node_t *) _lv_ll_get_prev(linked_list, cur_node);
            cur_idx--;
        }
    }

    return NULL;
}

uint32_t lv_spangroup_get_span_count(const lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);

    if(obj == NULL) {
        return 0;
    }

    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_spangroup_t * spans = (lv_spangroup_t *)obj;
    return _lv_ll_get_len(&(spans->child_ll));
}

lv_text_align_t lv_spangroup_get_align(lv_obj_t * obj)
{
    return lv_obj_get_style_text_align(obj, LV_PART_MAIN);
}

lv_span_overflow_t lv_spangroup_get_overflow(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_spangroup_t * spans = (lv_spangroup_t *)obj;
    return spans->overflow;
}

int32_t lv_spangroup_get_indent(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_spangroup_t * spans = (lv_spangroup_t *)obj;
    return spans->indent;
}

lv_span_mode_t lv_spangroup_get_mode(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_spangroup_t * spans = (lv_spangroup_t *)obj;
    return spans->mode;
}

int32_t lv_spangroup_get_max_lines(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_spangroup_t * spans = (lv_spangroup_t *)obj;
    return spans->lines;
}

void lv_spangroup_refr_mode(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_spangroup_t * spans = (lv_spangroup_t *)obj;

    if(spans->mode == LV_SPAN_MODE_EXPAND) {
        lv_obj_set_width(obj, LV_SIZE_CONTENT);
        lv_obj_set_height(obj, LV_SIZE_CONTENT);
    }
    else if(spans->mode == LV_SPAN_MODE_BREAK) {
        if(lv_obj_get_style_width(obj, LV_PART_MAIN) == LV_SIZE_CONTENT) {
            lv_obj_set_width(obj, 100);
        }
        lv_obj_set_height(obj, LV_SIZE_CONTENT);
    }
    else if(spans->mode == LV_SPAN_MODE_FIXED) {
        /* use this mode, The user needs to set the size. */
        /* This is just to prevent an infinite loop. */
        if(lv_obj_get_style_width(obj, LV_PART_MAIN) == LV_SIZE_CONTENT) {
            lv_obj_set_width(obj, 100);
        }
        if(lv_obj_get_style_height(obj, LV_PART_MAIN) == LV_SIZE_CONTENT) {
            int32_t width = lv_obj_get_style_width(obj, LV_PART_MAIN);
            if(LV_COORD_IS_PCT(width)) {
                width = 100;
            }
            int32_t height = lv_spangroup_get_expand_height(obj, width);
            lv_obj_set_content_height(obj, height);
        }
    }

    refresh_self_size(obj);
}

int32_t lv_spangroup_get_max_line_height(lv_obj_t * obj)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_spangroup_t * spans = (lv_spangroup_t *)obj;

    int32_t max_line_h = 0;
    lv_span_t * cur_span;
    _LV_LL_READ(&spans->child_ll, cur_span) {
        const lv_font_t * font = lv_span_get_style_text_font(obj, cur_span);
        int32_t line_h = lv_font_get_line_height(font);
        if(line_h > max_line_h) {
            max_line_h = line_h;
        }
    }

    return max_line_h;
}

uint32_t lv_spangroup_get_expand_width(lv_obj_t * obj, uint32_t max_width)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_spangroup_t * spans = (lv_spangroup_t *)obj;

    if(_lv_ll_get_head(&spans->child_ll) == NULL) {
        return 0;
    }

    uint32_t width = LV_COORD_IS_PCT(spans->indent) ? 0 : spans->indent;
    lv_span_t * cur_span;
    int32_t letter_space = 0;
    _LV_LL_READ(&spans->child_ll, cur_span) {
        const lv_font_t * font = lv_span_get_style_text_font(obj, cur_span);
        letter_space = lv_span_get_style_text_letter_space(obj, cur_span);
        uint32_t j = 0;
        const char * cur_txt = cur_span->txt;
        span_text_check(&cur_txt);
        while(cur_txt[j] != '\0') {
            if(max_width > 0 && width >= max_width) {
                return max_width;
            }
            uint32_t letter      = _lv_text_encoded_next(cur_txt, &j);
            uint32_t letter_next = _lv_text_encoded_next(&cur_txt[j], NULL);
            uint32_t letter_w = lv_font_get_glyph_width(font, letter, letter_next);
            width = width + letter_w + letter_space;
        }
    }

    return width - letter_space;
}

int32_t lv_spangroup_get_expand_height(lv_obj_t * obj, int32_t width)
{
    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_spangroup_t * spans = (lv_spangroup_t *)obj;
    if(_lv_ll_get_head(&spans->child_ll) == NULL || width <= 0) {
        return 0;
    }

    /* init draw variable */
    lv_text_flag_t txt_flag = LV_TEXT_FLAG_NONE;
    int32_t line_space = lv_obj_get_style_text_line_space(obj, LV_PART_MAIN);
    int32_t max_width = width;
    int32_t indent = convert_indent_pct(obj, max_width);
    int32_t max_w  = max_width - indent; /* first line need minus indent */

    /* coords of draw span-txt */
    lv_point_t txt_pos;
    lv_point_set(&txt_pos, 0, indent); /* first line need add indent */

    lv_span_t * cur_span = _lv_ll_get_head(&spans->child_ll);
    const char * cur_txt = cur_span->txt;
    span_text_check(&cur_txt);
    uint32_t cur_txt_ofs = 0;
    lv_snippet_t snippet;   /* use to save cur_span info and push it to stack */
    memset(&snippet, 0, sizeof(snippet));

    int32_t line_cnt = 0;
    int32_t lines = spans->lines < 0 ? INT32_MAX : spans->lines;
    /* the loop control how many lines need to draw */
    while(cur_span) {
        int snippet_cnt = 0;
        int32_t max_line_h = 0;  /* the max height of span-font when a line have a lot of span */

        /* the loop control to find a line and push the relevant span info into stack  */
        while(1) {
            /* switch to the next span when current is end */
            if(cur_txt[cur_txt_ofs] == '\0') {
                cur_span = _lv_ll_get_next(&spans->child_ll, cur_span);
                if(cur_span == NULL) break;
                cur_txt = cur_span->txt;
                span_text_check(&cur_txt);
                cur_txt_ofs = 0;
                /* maybe also cur_txt[cur_txt_ofs] == '\0' */
                continue;
            }

            /* init span info to snippet. */
            if(cur_txt_ofs == 0) {
                snippet.span = cur_span;
                snippet.font = lv_span_get_style_text_font(obj, cur_span);
                snippet.letter_space = lv_span_get_style_text_letter_space(obj, cur_span);
                snippet.line_h = lv_font_get_line_height(snippet.font) + line_space;
            }

            /* get current span text line info */
            uint32_t next_ofs = 0;
            int32_t use_width = 0;
            bool isfill = lv_text_get_snippet(&cur_txt[cur_txt_ofs], snippet.font, snippet.letter_space,
                                              max_w, txt_flag, &use_width, &next_ofs);

            /* break word deal width */
            if(isfill && next_ofs > 0 && snippet_cnt > 0) {
                if(max_w < use_width) {
                    break;
                }

                uint32_t tmp_ofs = next_ofs;
                uint32_t letter = _lv_text_encoded_prev(&cur_txt[cur_txt_ofs], &tmp_ofs);
                if(!(letter == '\0' || letter == '\n' || letter == '\r' || _lv_text_is_break_char(letter))) {
                    tmp_ofs = 0;
                    letter = _lv_text_encoded_next(&cur_txt[cur_txt_ofs + next_ofs], &tmp_ofs);
                    if(!(letter == '\0' || letter == '\n'  || letter == '\r' || _lv_text_is_break_char(letter))) {
                        break;
                    }
                }
            }

            snippet.txt = &cur_txt[cur_txt_ofs];
            snippet.bytes = next_ofs;
            snippet.txt_w = use_width;
            cur_txt_ofs += next_ofs;
            if(max_line_h < snippet.line_h) {
                max_line_h = snippet.line_h;
            }
            snippet_cnt ++;
            max_w = max_w - use_width - snippet.letter_space;
            if(isfill  || max_w <= 0) {
                break;
            }
        }

        /* next line init */
        txt_pos.x = 0;
        txt_pos.y += max_line_h;
        max_w = max_width;
        line_cnt += 1;
        if(line_cnt >= lines) {
            break;
        }
    }
    txt_pos.y -= line_space;

    return txt_pos.y;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void lv_spangroup_constructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);
    lv_spangroup_t * spans = (lv_spangroup_t *)obj;
    _lv_ll_init(&spans->child_ll, sizeof(lv_span_t));
    spans->indent = 0;
    spans->lines = -1;
    spans->mode = LV_SPAN_MODE_EXPAND;
    spans->overflow = LV_SPAN_OVERFLOW_CLIP;
    spans->cache_w = 0;
    spans->cache_h = 0;
    spans->refresh = 1;
}

static void lv_spangroup_destructor(const lv_obj_class_t * class_p, lv_obj_t * obj)
{
    LV_UNUSED(class_p);
    lv_spangroup_t * spans = (lv_spangroup_t *)obj;
    lv_span_t * cur_span = _lv_ll_get_head(&spans->child_ll);
    while(cur_span) {
        _lv_ll_remove(&spans->child_ll, cur_span);
        if(cur_span->txt && cur_span->static_flag == 0) {
            lv_free(cur_span->txt);
            cur_span->txt = NULL;
        }
        lv_style_reset(&cur_span->style);
        lv_free(cur_span);
        cur_span = _lv_ll_get_head(&spans->child_ll);
    }
}

static void lv_spangroup_event(const lv_obj_class_t * class_p, lv_event_t * e)
{
    LV_UNUSED(class_p);

    /* Call the ancestor's event handler */
    if(lv_obj_event_base(MY_CLASS, e) != LV_RESULT_OK) return;

    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_current_target(e);
    lv_spangroup_t * spans = (lv_spangroup_t *)obj;

    if(code == LV_EVENT_DRAW_MAIN) {
        draw_main(e);
    }
    else if(code == LV_EVENT_STYLE_CHANGED) {
        refresh_self_size(obj);
    }
    else if(code == LV_EVENT_SIZE_CHANGED) {
        refresh_self_size(obj);
    }
    else if(code == LV_EVENT_GET_SELF_SIZE) {
        int32_t width = 0;
        int32_t height = 0;
        lv_point_t * self_size = lv_event_get_param(e);

        if(spans->mode == LV_SPAN_MODE_EXPAND) {
            if(spans->refresh) {
                spans->cache_w = (int32_t)lv_spangroup_get_expand_width(obj, 0);
                spans->cache_h = lv_spangroup_get_max_line_height(obj);
                spans->refresh = 0;
            }
            width = spans->cache_w;
            height = spans->cache_h;
        }
        else if(spans->mode == LV_SPAN_MODE_BREAK) {
            width = lv_obj_get_content_width(obj);
            if(self_size->y >= 0) {
                if(width != spans->cache_w || spans->refresh) {
                    height = lv_spangroup_get_expand_height(obj, width);
                    spans->cache_w = width;
                    spans->cache_h = height;
                    spans->refresh = 0;
                }
                else {
                    height = spans->cache_h;
                }
            }
        }
        else if(spans->mode == LV_SPAN_MODE_FIXED) {
            width =  self_size->x >= 0 ? lv_obj_get_content_width(obj) : 0;
            height = self_size->y >= 0 ? lv_obj_get_content_height(obj) : 0;
        }
        self_size->x = LV_MAX(self_size->x, width);
        self_size->y = LV_MAX(self_size->y, height);
    }
}

static void draw_main(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_current_target(e);
    lv_layer_t * layer = lv_event_get_layer(e);

    lv_draw_span(obj, layer);
}

/**
 * @return true for txt fill the max_width.
 */
static bool lv_text_get_snippet(const char * txt, const lv_font_t * font,
                                int32_t letter_space, int32_t max_width, lv_text_flag_t flag,
                                int32_t * use_width, uint32_t * end_ofs)
{
    if(txt == NULL || txt[0] == '\0') {
        *end_ofs = 0;
        *use_width = 0;
        return false;
    }

    int32_t real_max_width = max_width;
#if !LV_USE_FONT_PLACEHOLDER
    /* fix incomplete text display when disable the placeholder. */
    /* workaround by: https://github.com/lvgl/lvgl/issues/3685 */
    real_max_width++;
#endif

    uint32_t ofs = _lv_text_get_next_line(txt, font, letter_space, real_max_width, use_width, flag);
    *end_ofs = ofs;

    if(txt[ofs] == '\0' && *use_width < max_width) {
        return false;
    }
    else {
        return true;
    }
}

static void lv_snippet_push(lv_snippet_t * item)
{
    struct _snippet_stack * stack_p = snippet_stack;
    if(stack_p->index < LV_SPAN_SNIPPET_STACK_SIZE) {
        memcpy(&stack_p->stack[stack_p->index], item, sizeof(lv_snippet_t));
        stack_p->index++;
    }
    else {
        LV_LOG_ERROR("span draw stack overflow, please set LV_SPAN_SNIPPET_STACK_SIZE too larger");
    }
}

static uint32_t lv_get_snippet_count(void)
{
    return snippet_stack->index;
}

static lv_snippet_t * lv_get_snippet(uint32_t index)
{
    return &snippet_stack->stack[index];
}

static void lv_snippet_clear(void)
{
    snippet_stack->index = 0;
}

static const lv_font_t * lv_span_get_style_text_font(lv_obj_t * par, lv_span_t * span)
{
    const lv_font_t * font;
    lv_style_value_t value;
    lv_result_t res = lv_style_get_prop(&span->style, LV_STYLE_TEXT_FONT, &value);
    if(res != LV_RESULT_OK) {
        font = lv_obj_get_style_text_font(par, LV_PART_MAIN);
    }
    else {
        font = (const lv_font_t *)value.ptr;
    }
    return font;
}

static int32_t lv_span_get_style_text_letter_space(lv_obj_t * par, lv_span_t * span)
{
    int32_t letter_space;
    lv_style_value_t value;
    lv_result_t res = lv_style_get_prop(&span->style, LV_STYLE_TEXT_LETTER_SPACE, &value);
    if(res != LV_RESULT_OK) {
        letter_space = lv_obj_get_style_text_letter_space(par, LV_PART_MAIN);
    }
    else {
        letter_space = (int32_t)value.num;
    }
    return letter_space;
}

static lv_color_t lv_span_get_style_text_color(lv_obj_t * par, lv_span_t * span)
{
    lv_style_value_t value;
    lv_result_t res = lv_style_get_prop(&span->style, LV_STYLE_TEXT_COLOR, &value);
    if(res != LV_RESULT_OK) {
        value.color = lv_obj_get_style_text_color(par, LV_PART_MAIN);
    }
    return value.color;
}

static lv_opa_t lv_span_get_style_text_opa(lv_obj_t * par, lv_span_t * span)
{
    lv_opa_t opa;
    lv_style_value_t value;
    lv_result_t res = lv_style_get_prop(&span->style, LV_STYLE_TEXT_OPA, &value);
    if(res != LV_RESULT_OK) {
        opa = (lv_opa_t)lv_obj_get_style_text_opa(par, LV_PART_MAIN);
    }
    else {
        opa = (lv_opa_t)value.num;
    }
    return opa;
}

static lv_blend_mode_t lv_span_get_style_text_blend_mode(lv_obj_t * par, lv_span_t * span)
{
    lv_blend_mode_t mode;
    lv_style_value_t value;
    lv_result_t res = lv_style_get_prop(&span->style, LV_STYLE_BLEND_MODE, &value);
    if(res != LV_RESULT_OK) {
        mode = (lv_blend_mode_t)lv_obj_get_style_blend_mode(par, LV_PART_MAIN);
    }
    else {
        mode = (lv_blend_mode_t)value.num;
    }
    return mode;
}

static int32_t lv_span_get_style_text_decor(lv_obj_t * par, lv_span_t * span)
{
    int32_t decor;
    lv_style_value_t value;
    lv_result_t res = lv_style_get_prop(&span->style, LV_STYLE_TEXT_DECOR, &value);
    if(res != LV_RESULT_OK) {
        decor = (lv_text_decor_t)lv_obj_get_style_text_decor(par, LV_PART_MAIN);;
    }
    else {
        decor = (int32_t)value.num;
    }
    return decor;
}

static inline void span_text_check(const char ** text)
{
    if(*text == NULL) {
        *text = "";
        LV_LOG_ERROR("occur an error that span text == NULL");
    }
}

static int32_t convert_indent_pct(lv_obj_t * obj, int32_t width)
{
    lv_spangroup_t * spans = (lv_spangroup_t *)obj;

    int32_t indent = spans->indent;
    if(LV_COORD_IS_PCT(spans->indent)) {
        if(spans->mode == LV_SPAN_MODE_EXPAND) {
            indent = 0;
        }
        else {
            indent = (width * LV_COORD_GET_PCT(spans->indent)) / 100;
        }
    }

    return indent;
}

/**
 * draw span group
 * @param spans obj handle
 * @param coords coordinates of the label
 * @param mask the label will be drawn only in this area
 */
static void lv_draw_span(lv_obj_t * obj, lv_layer_t * layer)
{

    lv_area_t coords;
    lv_obj_get_content_coords(obj, &coords);

    lv_spangroup_t * spans = (lv_spangroup_t *)obj;

    /* return if not span */
    if(_lv_ll_get_head(&spans->child_ll) == NULL) {
        return;
    }

    /* return if no draw area */
    lv_area_t clip_area;
    if(!_lv_area_intersect(&clip_area, &coords, &layer->_clip_area))  return;
    const lv_area_t clip_area_ori = layer->_clip_area;
    layer->_clip_area = clip_area;

    /* init draw variable */
    lv_text_flag_t txt_flag = LV_TEXT_FLAG_NONE;
    int32_t line_space = lv_obj_get_style_text_line_space(obj, LV_PART_MAIN);;
    int32_t max_width = lv_area_get_width(&coords);
    int32_t indent = convert_indent_pct(obj, max_width);
    int32_t max_w  = max_width - indent; /* first line need minus indent */
    lv_opa_t obj_opa = lv_obj_get_style_opa_recursive(obj, LV_PART_MAIN);

    /* coords of draw span-txt */
    lv_point_t txt_pos;
    txt_pos.y = coords.y1;
    txt_pos.x = coords.x1 + indent; /* first line need add indent */

    lv_span_t * cur_span = _lv_ll_get_head(&spans->child_ll);
    const char * cur_txt = cur_span->txt;
    span_text_check(&cur_txt);
    uint32_t cur_txt_ofs = 0;
    lv_snippet_t snippet;   /* use to save cur_span info and push it to stack */
    lv_memzero(&snippet, sizeof(snippet));

    lv_draw_label_dsc_t label_draw_dsc;
    lv_draw_label_dsc_init(&label_draw_dsc);

    bool is_first_line = true;
    /* the loop control how many lines need to draw */
    while(cur_span) {
        bool is_end_line = false;
        bool ellipsis_valid = false;
        int32_t max_line_h = 0;  /* the max height of span-font when a line have a lot of span */
        int32_t max_baseline = 0; /*baseline of the highest span*/
        lv_snippet_clear();

        /* the loop control to find a line and push the relevant span info into stack  */
        while(1) {
            /* switch to the next span when current is end */
            if(cur_txt[cur_txt_ofs] == '\0') {
                cur_span = _lv_ll_get_next(&spans->child_ll, cur_span);
                if(cur_span == NULL) break;
                cur_txt = cur_span->txt;
                span_text_check(&cur_txt);
                cur_txt_ofs = 0;
                /* maybe also cur_txt[cur_txt_ofs] == '\0' */
                continue;
            }

            /* init span info to snippet. */
            if(cur_txt_ofs == 0) {
                snippet.span = cur_span;
                snippet.font = lv_span_get_style_text_font(obj, cur_span);
                snippet.letter_space = lv_span_get_style_text_letter_space(obj, cur_span);
                snippet.line_h = lv_font_get_line_height(snippet.font) + line_space;
            }

            /* get current span text line info */
            uint32_t next_ofs = 0;
            int32_t use_width = 0;
            bool isfill = lv_text_get_snippet(&cur_txt[cur_txt_ofs], snippet.font, snippet.letter_space,
                                              max_w, txt_flag, &use_width, &next_ofs);

            if(isfill) {
                if(next_ofs > 0 && lv_get_snippet_count() > 0) {
                    /* To prevent infinite loops, the _lv_text_get_next_line() may return incomplete words, */
                    /* This phenomenon should be avoided when lv_get_snippet_count() > 0 */
                    if(max_w < use_width) {
                        break;
                    }
                    uint32_t tmp_ofs = next_ofs;
                    uint32_t letter = _lv_text_encoded_prev(&cur_txt[cur_txt_ofs], &tmp_ofs);
                    if(!(letter == '\0' || letter == '\n' || letter == '\r' || _lv_text_is_break_char(letter))) {
                        tmp_ofs = 0;
                        letter = _lv_text_encoded_next(&cur_txt[cur_txt_ofs + next_ofs], &tmp_ofs);
                        if(!(letter == '\0' || letter == '\n'  || letter == '\r' || _lv_text_is_break_char(letter))) {
                            break;
                        }
                    }
                }
            }

            snippet.txt = &cur_txt[cur_txt_ofs];
            snippet.bytes = next_ofs;
            snippet.txt_w = use_width;
            cur_txt_ofs += next_ofs;
            if(max_line_h < snippet.line_h) {
                max_line_h = snippet.line_h;
                max_baseline = snippet.font->base_line;
            }

            lv_snippet_push(&snippet);
            max_w = max_w - use_width - snippet.letter_space;
            if(isfill || max_w <= 0) {
                break;
            }
        }

        /* start current line deal with */

        uint32_t item_cnt = lv_get_snippet_count();
        if(item_cnt == 0) {     /* break if stack is empty */
            break;
        }

        /* Whether the current line is the end line and does overflow processing */
        {
            lv_snippet_t * last_snippet = lv_get_snippet(item_cnt - 1);
            int32_t next_line_h = last_snippet->line_h;
            if(last_snippet->txt[last_snippet->bytes] == '\0') {
                next_line_h = 0;
                lv_span_t * next_span = _lv_ll_get_next(&spans->child_ll, last_snippet->span);
                if(next_span) { /* have the next line */
                    next_line_h = lv_font_get_line_height(lv_span_get_style_text_font(obj, next_span)) + line_space;
                }
            }
            if(txt_pos.y + max_line_h + next_line_h - line_space > coords.y2 + 1) { /* for overflow if is end line. */
                if(last_snippet->txt[last_snippet->bytes] != '\0') {
                    last_snippet->bytes = lv_strlen(last_snippet->txt);
                    last_snippet->txt_w = lv_text_get_width(last_snippet->txt, last_snippet->bytes, last_snippet->font,
                                                            last_snippet->letter_space);
                }
                ellipsis_valid = spans->overflow == LV_SPAN_OVERFLOW_ELLIPSIS;
                is_end_line = true;
            }
        }

        /*Go the first visible line*/
        if(txt_pos.y + max_line_h < clip_area.y1) {
            goto Next_line_init;
        }

        /* align deal with */
        lv_text_align_t align = lv_obj_get_style_text_align(obj, LV_PART_MAIN);
        if(align == LV_TEXT_ALIGN_CENTER || align == LV_TEXT_ALIGN_RIGHT) {
            int32_t align_ofs = 0;
            int32_t txts_w = is_first_line ? indent : 0;
            uint32_t i;
            for(i = 0; i < item_cnt; i++) {
                lv_snippet_t * pinfo = lv_get_snippet(i);
                txts_w = txts_w + pinfo->txt_w + pinfo->letter_space;
            }
            txts_w -= lv_get_snippet(item_cnt - 1)->letter_space;
            align_ofs = max_width > txts_w ? max_width - txts_w : 0;
            if(align == LV_TEXT_ALIGN_CENTER) {
                align_ofs = align_ofs >> 1;
            }
            txt_pos.x += align_ofs;
        }

        /* draw line letters */
        uint32_t i;
        for(i = 0; i < item_cnt; i++) {
            lv_snippet_t * pinfo = lv_get_snippet(i);

            /* bidi deal with:todo */
            const char * bidi_txt = pinfo->txt;

            lv_point_t pos;
            pos.x = txt_pos.x;
            pos.y = txt_pos.y + max_line_h - pinfo->line_h - (max_baseline - pinfo->font->base_line);
            label_draw_dsc.color = lv_span_get_style_text_color(obj, pinfo->span);
            label_draw_dsc.opa = lv_span_get_style_text_opa(obj, pinfo->span);
            label_draw_dsc.font = lv_span_get_style_text_font(obj, pinfo->span);
            label_draw_dsc.blend_mode = lv_span_get_style_text_blend_mode(obj, pinfo->span);
            if(obj_opa < LV_OPA_MAX) {
                label_draw_dsc.opa = LV_OPA_MIX2(label_draw_dsc.opa, obj_opa);
            }
            uint32_t txt_bytes = pinfo->bytes;

            /* overflow */
            uint32_t dot_letter_w = 0;
            uint32_t dot_width = 0;
            if(ellipsis_valid) {
                dot_letter_w = lv_font_get_glyph_width(pinfo->font, '.', '.');
                dot_width = dot_letter_w * 3;
            }
            int32_t ellipsis_width = coords.x1 + max_width - dot_width;

            uint32_t j = 0;
            while(j < txt_bytes) {
                /* skip invalid fields */
                if(pos.x > clip_area.x2) {
                    break;
                }
                uint32_t letter      = _lv_text_encoded_next(bidi_txt, &j);
                uint32_t letter_next = _lv_text_encoded_next(&bidi_txt[j], NULL);
                int32_t letter_w = lv_font_get_glyph_width(pinfo->font, letter, letter_next);

                /* skip invalid fields */
                if(pos.x + letter_w + pinfo->letter_space < clip_area.x1) {
                    if(letter_w > 0) {
                        pos.x = pos.x + letter_w + pinfo->letter_space;
                    }
                    continue;
                }

                if(ellipsis_valid && pos.x + letter_w + pinfo->letter_space > ellipsis_width) {
                    for(int ell = 0; ell < 3; ell++) {
                        lv_draw_character(layer, &label_draw_dsc, &pos, '.');
                        pos.x = pos.x + dot_letter_w + pinfo->letter_space;
                    }
                    if(pos.x <= ellipsis_width) {
                        pos.x = ellipsis_width + 1;
                    }
                    break;
                }
                else {
                    lv_draw_character(layer, &label_draw_dsc, &pos, letter);
                    if(letter_w > 0) {
                        pos.x = pos.x + letter_w + pinfo->letter_space;
                    }
                }
            }

            /* draw decor */
            lv_text_decor_t decor = lv_span_get_style_text_decor(obj, pinfo->span);
            if(decor != LV_TEXT_DECOR_NONE) {
                lv_draw_line_dsc_t line_dsc;
                lv_draw_line_dsc_init(&line_dsc);
                line_dsc.color = label_draw_dsc.color;
                line_dsc.width = label_draw_dsc.font->underline_thickness ? pinfo->font->underline_thickness : 1;
                line_dsc.opa = label_draw_dsc.opa;
                line_dsc.blend_mode = label_draw_dsc.blend_mode;

                if(decor & LV_TEXT_DECOR_STRIKETHROUGH) {
                    int32_t y = pos.y + ((pinfo->line_h - line_space) >> 1)  + (line_dsc.width >> 1);
                    lv_point_precise_set(&line_dsc.p1, txt_pos.x, y);
                    lv_point_precise_set(&line_dsc.p2, pos.x, y);
                    lv_draw_line(layer, &line_dsc);
                }

                if(decor & LV_TEXT_DECOR_UNDERLINE) {
                    int32_t y = pos.y + pinfo->line_h - line_space - pinfo->font->base_line - pinfo->font->underline_position;
                    lv_point_precise_set(&line_dsc.p1, txt_pos.x, y);
                    lv_point_precise_set(&line_dsc.p2, pos.x, y);
                    lv_draw_line(layer, &line_dsc);
                }
            }
            txt_pos.x = pos.x;
        }

Next_line_init:
        /* next line init */
        is_first_line = false;
        txt_pos.x = coords.x1;
        txt_pos.y += max_line_h;
        if(is_end_line || txt_pos.y > clip_area.y2 + 1) {
            layer->_clip_area = clip_area_ori;
            return;
        }
        max_w = max_width;
    }
    layer->_clip_area = clip_area_ori;
}

static void refresh_self_size(lv_obj_t * obj)
{
    lv_spangroup_t * spans = (lv_spangroup_t *)obj;
    spans->refresh = 1;
    lv_obj_invalidate(obj);
    lv_obj_refresh_self_size(obj);
}

#endif
