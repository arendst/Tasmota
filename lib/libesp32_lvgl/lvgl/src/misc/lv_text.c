/**
 * @file lv_text.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include <stdarg.h>
#include "lv_text.h"
#include "lv_text_ap.h"
#include "lv_math.h"
#include "lv_log.h"
#include "lv_assert.h"
#include "../stdlib/lv_mem.h"
#include "../stdlib/lv_string.h"

/*********************
 *      DEFINES
 *********************/
#define NO_BREAK_FOUND UINT32_MAX

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

#if LV_TXT_ENC == LV_TXT_ENC_UTF8
    static uint8_t lv_text_utf8_size(const char * str);
    static uint32_t lv_text_unicode_to_utf8(uint32_t letter_uni);
    static uint32_t lv_text_utf8_conv_wc(uint32_t c);
    static uint32_t lv_text_utf8_next(const char * txt, uint32_t * i);
    static uint32_t lv_text_utf8_prev(const char * txt, uint32_t * i_start);
    static uint32_t lv_text_utf8_get_byte_id(const char * txt, uint32_t utf8_id);
    static uint32_t lv_text_utf8_get_char_id(const char * txt, uint32_t byte_id);
    static uint32_t lv_text_utf8_get_length(const char * txt);
#elif LV_TXT_ENC == LV_TXT_ENC_ASCII
    static uint8_t lv_text_iso8859_1_size(const char * str);
    static uint32_t lv_text_unicode_to_iso8859_1(uint32_t letter_uni);
    static uint32_t lv_text_iso8859_1_conv_wc(uint32_t c);
    static uint32_t lv_text_iso8859_1_next(const char * txt, uint32_t * i);
    static uint32_t lv_text_iso8859_1_prev(const char * txt, uint32_t * i_start);
    static uint32_t lv_text_iso8859_1_get_byte_id(const char * txt, uint32_t utf8_id);
    static uint32_t lv_text_iso8859_1_get_char_id(const char * txt, uint32_t byte_id);
    static uint32_t lv_text_iso8859_1_get_length(const char * txt);
#endif
/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *  GLOBAL VARIABLES
 **********************/
#if LV_TXT_ENC == LV_TXT_ENC_UTF8
    uint8_t (*_lv_text_encoded_size)(const char *)                   = lv_text_utf8_size;
    uint32_t (*_lv_text_unicode_to_encoded)(uint32_t)                = lv_text_unicode_to_utf8;
    uint32_t (*_lv_text_encoded_conv_wc)(uint32_t)                   = lv_text_utf8_conv_wc;
    uint32_t (*_lv_text_encoded_next)(const char *, uint32_t *)      = lv_text_utf8_next;
    uint32_t (*_lv_text_encoded_prev)(const char *, uint32_t *)      = lv_text_utf8_prev;
    uint32_t (*_lv_text_encoded_get_byte_id)(const char *, uint32_t) = lv_text_utf8_get_byte_id;
    uint32_t (*_lv_text_encoded_get_char_id)(const char *, uint32_t) = lv_text_utf8_get_char_id;
    uint32_t (*_lv_text_get_encoded_length)(const char *)            = lv_text_utf8_get_length;
#elif LV_TXT_ENC == LV_TXT_ENC_ASCII
    uint8_t (*_lv_text_encoded_size)(const char *)                   = lv_text_iso8859_1_size;
    uint32_t (*_lv_text_unicode_to_encoded)(uint32_t)                = lv_text_unicode_to_iso8859_1;
    uint32_t (*_lv_text_encoded_conv_wc)(uint32_t)                   = lv_text_iso8859_1_conv_wc;
    uint32_t (*_lv_text_encoded_next)(const char *, uint32_t *)      = lv_text_iso8859_1_next;
    uint32_t (*_lv_text_encoded_prev)(const char *, uint32_t *)      = lv_text_iso8859_1_prev;
    uint32_t (*_lv_text_encoded_get_byte_id)(const char *, uint32_t) = lv_text_iso8859_1_get_byte_id;
    uint32_t (*_lv_text_encoded_get_char_id)(const char *, uint32_t)     = lv_text_iso8859_1_get_char_id;
    uint32_t (*_lv_text_get_encoded_length)(const char *)            = lv_text_iso8859_1_get_length;

#endif

/**********************
 *      MACROS
 **********************/

#define LV_IS_ASCII(value)              ((value & 0x80U) == 0x00U)
#define LV_IS_2BYTES_UTF8_CODE(value)   ((value & 0xE0U) == 0xC0U)
#define LV_IS_3BYTES_UTF8_CODE(value)   ((value & 0xF0U) == 0xE0U)
#define LV_IS_4BYTES_UTF8_CODE(value)   ((value & 0xF8U) == 0xF0U)
#define LV_IS_INVALID_UTF8_CODE(value)  ((value & 0xC0U) != 0x80U)

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_text_get_size(lv_point_t * size_res, const char * text, const lv_font_t * font, int32_t letter_space,
                      int32_t line_space, int32_t max_width, lv_text_flag_t flag)
{
    size_res->x = 0;
    size_res->y = 0;

    if(text == NULL) return;
    if(font == NULL) return;

    if(flag & LV_TEXT_FLAG_EXPAND) max_width = LV_COORD_MAX;

    uint32_t line_start     = 0;
    uint32_t new_line_start = 0;
    uint16_t letter_height = lv_font_get_line_height(font);

    /*Calc. the height and longest line*/
    while(text[line_start] != '\0') {
        new_line_start += _lv_text_get_next_line(&text[line_start], font, letter_space, max_width, NULL, flag);

        if((unsigned long)size_res->y + (unsigned long)letter_height + (unsigned long)line_space > LV_MAX_OF(int32_t)) {
            LV_LOG_WARN("integer overflow while calculating text height");
            return;
        }
        else {
            size_res->y += letter_height;
            size_res->y += line_space;
        }

        /*Calculate the longest line*/
        int32_t act_line_length = lv_text_get_width(&text[line_start], new_line_start - line_start, font, letter_space);

        size_res->x = LV_MAX(act_line_length, size_res->x);
        line_start  = new_line_start;
    }

    /*Make the text one line taller if the last character is '\n' or '\r'*/
    if((line_start != 0) && (text[line_start - 1] == '\n' || text[line_start - 1] == '\r')) {
        size_res->y += letter_height + line_space;
    }

    /*Correction with the last line space or set the height manually if the text is empty*/
    if(size_res->y == 0)
        size_res->y = letter_height;
    else
        size_res->y -= line_space;
}

/**
 * Get the next word of text. A word is delimited by break characters.
 *
 * If the word cannot fit in the max_width space, obey LV_TXT_LINE_BREAK_LONG_* rules.
 *
 * If the next word cannot fit anything, return 0.
 *
 * If the first character is a break character, returns the next index.
 *
 * Example calls from lv_text_get_next_line() assuming sufficient max_width and
 * txt = "Test text\n"
 *        0123456789
 *
 * Calls would be as follows:
 *     1. Return i=4, pointing at breakchar ' ', for the string "Test"
 *     2. Return i=5, since i=4 was a breakchar.
 *     3. Return i=9, pointing at breakchar '\n'
 *     4. Parenting lv_text_get_next_line() would detect subsequent '\0'
 *
 * TODO: Returned word_w_ptr may overestimate the returned word's width when
 * max_width is reached. In current usage, this has no impact.
 *
 * @param txt a '\0' terminated string
 * @param font pointer to a font
 * @param letter_space letter space
 * @param max_width max width of the text (break the lines to fit this size). Set COORD_MAX to avoid line breaks
 * @param flags settings for the text from 'txt_flag_type' enum
 * @param[out] word_w_ptr width (in pixels) of the parsed word. May be NULL.
 * @param force Force return the fraction of the word that can fit in the provided space.
 * @return the index of the first char of the next word (in byte index not letter index. With UTF-8 they are different)
 */
static uint32_t lv_text_get_next_word(const char * txt, const lv_font_t * font,
                                      int32_t letter_space, int32_t max_width,
                                      lv_text_flag_t flag, uint32_t * word_w_ptr, bool force)
{
    if(txt == NULL || txt[0] == '\0') return 0;
    if(font == NULL) return 0;

    if(flag & LV_TEXT_FLAG_EXPAND) max_width = LV_COORD_MAX;

    uint32_t i = 0, i_next = 0, i_next_next = 0;  /*Iterating index into txt*/
    uint32_t letter = 0;      /*Letter at i*/
    uint32_t letter_next = 0; /*Letter at i_next*/
    int32_t letter_w;
    int32_t cur_w = 0;  /*Pixel Width of transversed string*/
    uint32_t word_len = 0;   /*Number of characters in the transversed word*/
    uint32_t break_index = NO_BREAK_FOUND; /*only used for "long" words*/
    uint32_t break_letter_count = 0; /*Number of characters up to the long word break point*/

    letter = _lv_text_encoded_next(txt, &i_next);
    i_next_next = i_next;

    /*Obtain the full word, regardless if it fits or not in max_width*/
    while(txt[i] != '\0') {
        letter_next = _lv_text_encoded_next(txt, &i_next_next);
        word_len++;

        letter_w = lv_font_get_glyph_width(font, letter, letter_next);
        cur_w += letter_w;

        if(letter_w > 0) {
            cur_w += letter_space;
        }

        /*Test if this character fits within max_width*/
        if(break_index == NO_BREAK_FOUND && (cur_w - letter_space) > max_width) {
            break_index = i;
            break_letter_count = word_len - 1;
            /*break_index is now pointing at the character that doesn't fit*/
        }

        /*Check for new line chars and breakchars*/
        if(letter == '\n' || letter == '\r' || _lv_text_is_break_char(letter)) {
            /*Update the output width on the first character if it fits.
             *Must do this here in case first letter is a break character.*/
            if(i == 0 && break_index == NO_BREAK_FOUND && word_w_ptr != NULL) *word_w_ptr = cur_w;
            word_len--;
            break;
        }
        else if(_lv_text_is_a_word(letter_next) || _lv_text_is_a_word(letter)) {
            /*Found a word for single letter, usually true for CJK*/
            *word_w_ptr = cur_w;
            i = i_next;
            break;
        }

        /*Update the output width*/
        if(word_w_ptr != NULL && break_index == NO_BREAK_FOUND) *word_w_ptr = cur_w;

        i = i_next;
        i_next = i_next_next;
        letter = letter_next;
    }

    /*Entire Word fits in the provided space*/
    if(break_index == NO_BREAK_FOUND) {
        if(word_len == 0 || (letter == '\r' && letter_next == '\n')) i = i_next;
        return i;
    }

#if LV_TXT_LINE_BREAK_LONG_LEN > 0
    /*Word doesn't fit in provided space, but isn't "long"*/
    if(word_len < LV_TXT_LINE_BREAK_LONG_LEN) {
        if(force) return break_index;
        if(word_w_ptr != NULL) *word_w_ptr = 0; /*Return no word*/
        return 0;
    }

    /*Word is "long," but insufficient amounts can fit in provided space*/
    if(break_letter_count < LV_TXT_LINE_BREAK_LONG_PRE_MIN_LEN) {
        if(force) return break_index;
        if(word_w_ptr != NULL) *word_w_ptr = 0;
        return 0;
    }

    /*Word is a "long", but letters may need to be better distributed*/
    {
        i = break_index;
        int32_t n_move = LV_TXT_LINE_BREAK_LONG_POST_MIN_LEN - (word_len - break_letter_count);
        /*Move pointer "i" backwards*/
        for(; n_move > 0; n_move--) {
            _lv_text_encoded_prev(txt, &i);
            /**
             * TODO: it would be appropriate to update the returned
             * word width hereHowever, in current usage, this doesn't impact anything.
             */
        }
    }
    return i;
#else
    if(force) return break_index;
    if(word_w_ptr != NULL) *word_w_ptr = 0; /*Return no word*/
    (void) break_letter_count;
    return 0;
#endif
}

uint32_t _lv_text_get_next_line(const char * txt, const lv_font_t * font,
                                int32_t letter_space, int32_t max_width,
                                int32_t * used_width, lv_text_flag_t flag)
{
    if(used_width) *used_width = 0;

    if(txt == NULL) return 0;
    if(txt[0] == '\0') return 0;
    if(font == NULL) return 0;

    int32_t line_w = 0;

    /*If max_width doesn't matter simply find the new line character
     *without thinking about word wrapping*/
    if((flag & LV_TEXT_FLAG_EXPAND) || (flag & LV_TEXT_FLAG_FIT)) {
        uint32_t i;
        for(i = 0; txt[i] != '\n' && txt[i] != '\r' && txt[i] != '\0'; i++) {
            /*Just find the new line chars or string ends by incrementing `i`*/
        }
        if(txt[i] != '\0') i++;    /*To go beyond `\n`*/
        if(used_width) *used_width = -1;
        return i;
    }

    if(flag & LV_TEXT_FLAG_EXPAND) max_width = LV_COORD_MAX;
    uint32_t i = 0;                                        /*Iterating index into txt*/

    while(txt[i] != '\0' && max_width > 0) {
        uint32_t word_w = 0;
        uint32_t advance = lv_text_get_next_word(&txt[i], font, letter_space, max_width, flag, &word_w, i == 0);
        max_width -= word_w;
        line_w += word_w;

        if(advance == 0) {
            break;
        }

        i += advance;

        if(txt[0] == '\n' || txt[0] == '\r') break;

        if(txt[i] == '\n' || txt[i] == '\r') {
            i++;  /*Include the following newline in the current line*/
            break;
        }

    }

    /*Always step at least one to avoid infinite loops*/
    if(i == 0) {
        uint32_t letter = _lv_text_encoded_next(txt, &i);
        if(used_width != NULL) {
            line_w = lv_font_get_glyph_width(font, letter, '\0');
        }
    }

    if(used_width != NULL) {
        *used_width = line_w;
    }

    return i;
}

int32_t lv_text_get_width(const char * txt, uint32_t length, const lv_font_t * font, int32_t letter_space)
{
    if(txt == NULL) return 0;
    if(font == NULL) return 0;
    if(txt[0] == '\0') return 0;

    uint32_t i                   = 0;
    int32_t width             = 0;

    if(length != 0) {
        while(i < length) {
            uint32_t letter;
            uint32_t letter_next;
            _lv_text_encoded_letter_next_2(txt, &letter, &letter_next, &i);

            int32_t char_width = lv_font_get_glyph_width(font, letter, letter_next);
            if(char_width > 0) {
                width += char_width;
                width += letter_space;
            }
        }

        if(width > 0) {
            width -= letter_space; /*Trim the last letter space. Important if the text is center
                                      aligned*/
        }
    }

    return width;
}

void _lv_text_ins(char * txt_buf, uint32_t pos, const char * ins_txt)
{
    if(txt_buf == NULL || ins_txt == NULL) return;

    size_t old_len = lv_strlen(txt_buf);
    size_t ins_len = lv_strlen(ins_txt);
    if(ins_len == 0) return;

    size_t new_len = ins_len + old_len;
    pos              = _lv_text_encoded_get_byte_id(txt_buf, pos); /*Convert to byte index instead of letter index*/

    /*Copy the second part into the end to make place to text to insert*/
    size_t i;
    for(i = new_len; i >= pos + ins_len; i--) {
        txt_buf[i] = txt_buf[i - ins_len];
    }

    /*Copy the text into the new space*/
    lv_memcpy(txt_buf + pos, ins_txt, ins_len);
}

void _lv_text_cut(char * txt, uint32_t pos, uint32_t len)
{
    if(txt == NULL) return;

    size_t old_len = lv_strlen(txt);

    pos = _lv_text_encoded_get_byte_id(txt, pos); /*Convert to byte index instead of letter index*/
    len = _lv_text_encoded_get_byte_id(&txt[pos], len);

    /*Copy the second part into the end to make place to text to insert*/
    uint32_t i;
    for(i = pos; i <= old_len - len; i++) {
        txt[i] = txt[i + len];
    }
}

char * _lv_text_set_text_vfmt(const char * fmt, va_list ap)
{
    /*Allocate space for the new text by using trick from C99 standard section 7.19.6.12*/
    va_list ap_copy;
    va_copy(ap_copy, ap);
    uint32_t len = lv_vsnprintf(NULL, 0, fmt, ap_copy);
    va_end(ap_copy);

    char * text = 0;
#if LV_USE_ARABIC_PERSIAN_CHARS
    /*Put together the text according to the format string*/
    char * raw_txt = lv_malloc(len + 1);
    LV_ASSERT_MALLOC(raw_txt);
    if(raw_txt == NULL) {
        return NULL;
    }

    lv_vsnprintf(raw_txt, len + 1, fmt, ap);

    /*Get the size of the Arabic text and process it*/
    size_t len_ap = _lv_text_ap_calc_bytes_count(raw_txt);
    text = lv_malloc(len_ap + 1);
    LV_ASSERT_MALLOC(text);
    if(text == NULL) {
        return NULL;
    }
    _lv_text_ap_proc(raw_txt, text);

    lv_free(raw_txt);
#else
    text = lv_malloc(len + 1);
    LV_ASSERT_MALLOC(text);
    if(text == NULL) {
        return NULL;
    }

    lv_vsnprintf(text, len + 1, fmt, ap);
#endif

    return text;
}

void _lv_text_encoded_letter_next_2(const char * txt, uint32_t * letter, uint32_t * letter_next, uint32_t * ofs)
{
    *letter = _lv_text_encoded_next(txt, ofs);
    *letter_next = *letter != '\0' ? _lv_text_encoded_next(&txt[*ofs], NULL) : 0;
}

#if LV_TXT_ENC == LV_TXT_ENC_UTF8
/*******************************
 *   UTF-8 ENCODER/DECODER
 ******************************/

/**
 * Give the size of an UTF-8 coded character
 * @param str pointer to a character in a string
 * @return length of the UTF-8 character (1,2,3 or 4), 0 on invalid code.
 */
static uint8_t lv_text_utf8_size(const char * str)
{
    if(LV_IS_ASCII(str[0]))
        return 1;
    else if(LV_IS_2BYTES_UTF8_CODE(str[0]))
        return 2;
    else if(LV_IS_3BYTES_UTF8_CODE(str[0]))
        return 3;
    else if(LV_IS_4BYTES_UTF8_CODE(str[0]))
        return 4;
    return 0;
}

/**
 * Convert a Unicode letter to UTF-8.
 * @param letter_uni a Unicode letter
 * @return UTF-8 coded character in Little Endian to be compatible with C chars (e.g. 'Á', 'Ű')
 */
static uint32_t lv_text_unicode_to_utf8(uint32_t letter_uni)
{
    if(letter_uni < 128) return letter_uni;
    uint8_t bytes[4];

    if(letter_uni < 0x0800) {
        bytes[0] = ((letter_uni >> 6) & 0x1F) | 0xC0;
        bytes[1] = ((letter_uni >> 0) & 0x3F) | 0x80;
        bytes[2] = 0;
        bytes[3] = 0;
    }
    else if(letter_uni < 0x010000) {
        bytes[0] = ((letter_uni >> 12) & 0x0F) | 0xE0;
        bytes[1] = ((letter_uni >> 6) & 0x3F) | 0x80;
        bytes[2] = ((letter_uni >> 0) & 0x3F) | 0x80;
        bytes[3] = 0;
    }
    else if(letter_uni < 0x110000) {
        bytes[0] = ((letter_uni >> 18) & 0x07) | 0xF0;
        bytes[1] = ((letter_uni >> 12) & 0x3F) | 0x80;
        bytes[2] = ((letter_uni >> 6) & 0x3F) | 0x80;
        bytes[3] = ((letter_uni >> 0) & 0x3F) | 0x80;
    }
    else {
        return 0;
    }

    uint32_t * res_p = (uint32_t *)bytes;
    return *res_p;
}

/**
 * Convert a wide character, e.g. 'Á' little endian to be UTF-8 compatible
 * @param c a wide character or a  Little endian number
 * @return `c` in big endian
 */
static uint32_t lv_text_utf8_conv_wc(uint32_t c)
{
#if LV_BIG_ENDIAN_SYSTEM == 0
    /*Swap the bytes (UTF-8 is big endian, but the MCUs are little endian)*/
    if((c & 0x80) != 0) {
        uint32_t swapped;
        uint8_t c8[4];
        lv_memcpy(c8, &c, 4);
        swapped = (c8[0] << 24) + (c8[1] << 16) + (c8[2] << 8) + (c8[3]);
        uint8_t i;
        for(i = 0; i < 4; i++) {
            if((swapped & 0xFF) == 0)
                swapped = (swapped >> 8); /*Ignore leading zeros (they were in the end originally)*/
        }
        c = swapped;
    }
#endif
    return c;
}

/**
 * Decode an UTF-8 character from a string.
 * @param txt pointer to '\0' terminated string
 * @param i start byte index in 'txt' where to start.
 *          After call it will point to the next UTF-8 char in 'txt'.
 *          NULL to use txt[0] as index
 * @return the decoded Unicode character or 0 on invalid UTF-8 code
 */
static uint32_t lv_text_utf8_next(const char * txt, uint32_t * i)
{
    /**
     * Unicode to UTF-8
     * 00000000 00000000 00000000 0xxxxxxx -> 0xxxxxxx
     * 00000000 00000000 00000yyy yyxxxxxx -> 110yyyyy 10xxxxxx
     * 00000000 00000000 zzzzyyyy yyxxxxxx -> 1110zzzz 10yyyyyy 10xxxxxx
     * 00000000 000wwwzz zzzzyyyy yyxxxxxx -> 11110www 10zzzzzz 10yyyyyy 10xxxxxx
     */

    uint32_t result = 0;

    /*Dummy 'i' pointer is required*/
    uint32_t i_tmp = 0;
    if(i == NULL) i = &i_tmp;

    /*Normal ASCII*/
    if(LV_IS_ASCII(txt[*i])) {
        result = txt[*i];
        (*i)++;
    }
    /*Real UTF-8 decode*/
    else {
        /*2 bytes UTF-8 code*/
        if(LV_IS_2BYTES_UTF8_CODE(txt[*i])) {
            result = (uint32_t)(txt[*i] & 0x1F) << 6;
            (*i)++;
            if(LV_IS_INVALID_UTF8_CODE(txt[*i])) return 0;
            result += (txt[*i] & 0x3F);
            (*i)++;
        }
        /*3 bytes UTF-8 code*/
        else if(LV_IS_3BYTES_UTF8_CODE(txt[*i])) {
            result = (uint32_t)(txt[*i] & 0x0F) << 12;
            (*i)++;

            if(LV_IS_INVALID_UTF8_CODE(txt[*i])) return 0;
            result += (uint32_t)(txt[*i] & 0x3F) << 6;
            (*i)++;

            if(LV_IS_INVALID_UTF8_CODE(txt[*i])) return 0;
            result += (txt[*i] & 0x3F);
            (*i)++;
        }
        /*4 bytes UTF-8 code*/
        else if(LV_IS_4BYTES_UTF8_CODE(txt[*i])) {
            result = (uint32_t)(txt[*i] & 0x07) << 18;
            (*i)++;

            if(LV_IS_INVALID_UTF8_CODE(txt[*i])) return 0;
            result += (uint32_t)(txt[*i] & 0x3F) << 12;
            (*i)++;

            if(LV_IS_INVALID_UTF8_CODE(txt[*i])) return 0;
            result += (uint32_t)(txt[*i] & 0x3F) << 6;
            (*i)++;

            if(LV_IS_INVALID_UTF8_CODE(txt[*i])) return 0;
            result += txt[*i] & 0x3F;
            (*i)++;
        }
        else {
            (*i)++; /*Not UTF-8 char. Go the next.*/
        }
    }
    return result;
}

/**
 * Get previous UTF-8 character form a string.
 * @param txt pointer to '\0' terminated string
 * @param i start byte index in 'txt' where to start. After the call it will point to the previous
 * UTF-8 char in 'txt'.
 * @return the decoded Unicode character or 0 on invalid UTF-8 code
 */
static uint32_t lv_text_utf8_prev(const char * txt, uint32_t * i)
{
    uint8_t c_size;
    uint8_t cnt = 0;

    /*Try to find a !0 long UTF-8 char by stepping one character back*/
    (*i)--;
    do {
        if(cnt >= 4) return 0; /*No UTF-8 char found before the initial*/

        c_size = _lv_text_encoded_size(&txt[*i]);
        if(c_size == 0) {
            if(*i != 0)
                (*i)--;
            else
                return 0;
        }
        cnt++;
    } while(c_size == 0);

    uint32_t i_tmp  = *i;
    uint32_t letter = _lv_text_encoded_next(txt, &i_tmp); /*Character found, get it*/

    return letter;
}

/**
 * Convert a character index (in an UTF-8 text) to byte index.
 * E.g. in "AÁRT" index of 'R' is 2th char but start at byte 3 because 'Á' is 2 bytes long
 * @param txt a '\0' terminated UTF-8 string
 * @param utf8_id character index
 * @return byte index of the 'utf8_id'th letter
 */
static uint32_t lv_text_utf8_get_byte_id(const char * txt, uint32_t utf8_id)
{
    uint32_t i;
    uint32_t byte_cnt = 0;
    for(i = 0; i < utf8_id && txt[byte_cnt] != '\0'; i++) {
        uint8_t c_size = _lv_text_encoded_size(&txt[byte_cnt]);
        /* If the char was invalid tell it's 1 byte long*/
        byte_cnt += c_size ? c_size : 1;
    }

    return byte_cnt;
}

/**
 * Convert a byte index (in an UTF-8 text) to character index.
 * E.g. in "AÁRT" index of 'R' is 2th char but start at byte 3 because 'Á' is 2 bytes long
 * @param txt a '\0' terminated UTF-8 string
 * @param byte_id byte index
 * @return character index of the letter at 'byte_id'th position
 */
static uint32_t lv_text_utf8_get_char_id(const char * txt, uint32_t byte_id)
{
    uint32_t i        = 0;
    uint32_t char_cnt = 0;

    while(i < byte_id) {
        _lv_text_encoded_next(txt, &i); /*'i' points to the next letter so use the prev. value*/
        char_cnt++;
    }

    return char_cnt;
}

/**
 * Get the number of characters (and NOT bytes) in a string. Decode it with UTF-8 if enabled.
 * E.g.: "ÁBC" is 3 characters (but 4 bytes)
 * @param txt a '\0' terminated char string
 * @return number of characters
 */
static uint32_t lv_text_utf8_get_length(const char * txt)
{
    uint32_t len = 0;
    uint32_t i   = 0;

    while(txt[i] != '\0') {
        _lv_text_encoded_next(txt, &i);
        len++;
    }

    return len;
}

#elif LV_TXT_ENC == LV_TXT_ENC_ASCII
/*******************************
 *  ASCII ENCODER/DECODER
 ******************************/

/**
 * Give the size of an ISO8859-1 coded character
 * @param str pointer to a character in a string
 * @return length of the UTF-8 character (1,2,3 or 4). O on invalid code
 */
static uint8_t lv_text_iso8859_1_size(const char * str)
{
    LV_UNUSED(str); /*Unused*/
    return 1;
}

/**
 * Convert a Unicode letter to ISO8859-1.
 * @param letter_uni a Unicode letter
 * @return ISO8859-1 coded character in Little Endian to be compatible with C chars (e.g. 'Á', 'Ű')
 */
static uint32_t lv_text_unicode_to_iso8859_1(uint32_t letter_uni)
{
    if(letter_uni < 256)
        return letter_uni;
    else
        return ' ';
}

/**
 * Convert wide characters to ASCII, however wide characters in ASCII range (e.g. 'A') are ASCII compatible by default.
 * So this function does nothing just returns with `c`.
 * @param c a character, e.g. 'A'
 * @return same as `c`
 */
static uint32_t lv_text_iso8859_1_conv_wc(uint32_t c)
{
    return c;
}

/**
 * Decode an ISO8859-1 character from a string.
 * @param txt pointer to '\0' terminated string
 * @param i start byte index in 'txt' where to start.
 *          After call it will point to the next UTF-8 char in 'txt'.
 *          NULL to use txt[0] as index
 * @return the decoded Unicode character or 0 on invalid UTF-8 code
 */
static uint32_t lv_text_iso8859_1_next(const char * txt, uint32_t * i)
{
    if(i == NULL) return txt[0]; /*Get the next char*/

    uint8_t letter = txt[*i];
    (*i)++;
    return letter;
}

/**
 * Get previous ISO8859-1 character form a string.
 * @param txt pointer to '\0' terminated string
 * @param i start byte index in 'txt' where to start. After the call it will point to the previous UTF-8 char in 'txt'.
 * @return the decoded Unicode character or 0 on invalid UTF-8 code
 */
static uint32_t lv_text_iso8859_1_prev(const char * txt, uint32_t * i)
{
    if(i == NULL) return *(txt - 1); /*Get the prev. char*/

    (*i)--;
    uint8_t letter = txt[*i];

    return letter;
}

/**
 * Convert a character index (in an ISO8859-1 text) to byte index.
 * E.g. in "AÁRT" index of 'R' is 2th char but start at byte 3 because 'Á' is 2 bytes long
 * @param txt a '\0' terminated UTF-8 string
 * @param utf8_id character index
 * @return byte index of the 'utf8_id'th letter
 */
static uint32_t lv_text_iso8859_1_get_byte_id(const char * txt, uint32_t utf8_id)
{
    LV_UNUSED(txt); /*Unused*/
    return utf8_id; /*In Non encoded no difference*/
}

/**
 * Convert a byte index (in an ISO8859-1 text) to character index.
 * E.g. in "AÁRT" index of 'R' is 2th char but start at byte 3 because 'Á' is 2 bytes long
 * @param txt a '\0' terminated UTF-8 string
 * @param byte_id byte index
 * @return character index of the letter at 'byte_id'th position
 */
static uint32_t lv_text_iso8859_1_get_char_id(const char * txt, uint32_t byte_id)
{
    LV_UNUSED(txt); /*Unused*/
    return byte_id; /*In Non encoded no difference*/
}

/**
 * Get the number of characters (and NOT bytes) in a string. Decode it with UTF-8 if enabled.
 * E.g.: "ÁBC" is 3 characters (but 4 bytes)
 * @param txt a '\0' terminated char string
 * @return number of characters
 */
static uint32_t lv_text_iso8859_1_get_length(const char * txt)
{
    return lv_strlen(txt);
}
#else

#error "Invalid character encoding. See `LV_TXT_ENC` in `lv_conf.h`"

#endif
