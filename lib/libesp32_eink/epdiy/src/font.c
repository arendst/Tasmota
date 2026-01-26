#include <esp_assert.h>
#include <esp_heap_caps.h>
#include <esp_idf_version.h>
#include <esp_log.h>

#include "epdiy.h"

#include <miniz.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    uint8_t mask;    /* char data will be bitwise AND with this */
    uint8_t lead;    /* start bytes of current char in utf-8 encoded character */
    uint32_t beg;    /* beginning of codepoint range */
    uint32_t end;    /* end of codepoint range */
    int bits_stored; /* the number of bits from the codepoint that fits in char */
} utf_t;

/*
 * UTF-8 decode inspired from rosetta code
 * https://rosettacode.org/wiki/UTF-8_encode_and_decode#C
 */
static utf_t* utf[] = {
    /*             mask        lead        beg      end       bits */
    [0] = &(utf_t){ 0b00111111, 0b10000000, 0, 0, 6 },
    [1] = &(utf_t){ 0b01111111, 0b00000000, 0000, 0177, 7 },
    [2] = &(utf_t){ 0b00011111, 0b11000000, 0200, 03777, 5 },
    [3] = &(utf_t){ 0b00001111, 0b11100000, 04000, 0177777, 4 },
    [4] = &(utf_t){ 0b00000111, 0b11110000, 0200000, 04177777, 3 },
    &(utf_t){ 0 },
};

static inline int min(int x, int y) {
    return x < y ? x : y;
}
static inline int max(int x, int y) {
    return x > y ? x : y;
}

static int utf8_len(const uint8_t ch) {
    int len = 0;
    for (utf_t** u = utf; (*u)->mask; ++u) {
        if ((ch & ~(*u)->mask) == (*u)->lead) {
            break;
        }
        ++len;
    }
    if (len > 4) { /* Malformed leading byte */
        assert("invalid unicode.");
    }
    return len;
}

static uint32_t next_cp(const uint8_t** string) {
    if (**string == 0) {
        return 0;
    }
    int bytes = utf8_len(**string);
    const uint8_t* chr = *string;
    *string += bytes;
    int shift = utf[0]->bits_stored * (bytes - 1);
    uint32_t codep = (*chr++ & utf[bytes]->mask) << shift;

    for (int i = 1; i < bytes; ++i, ++chr) {
        shift -= utf[0]->bits_stored;
        codep |= ((const uint8_t)*chr & utf[0]->mask) << shift;
    }

    return codep;
}

EpdFontProperties epd_font_properties_default() {
    EpdFontProperties props
        = { .fg_color = 0, .bg_color = 15, .fallback_glyph = 0, .flags = EPD_DRAW_ALIGN_LEFT };
    return props;
}

const EpdGlyph* epd_get_glyph(const EpdFont* font, uint32_t code_point) {
    const EpdUnicodeInterval* intervals = font->intervals;
    for (int i = 0; i < font->interval_count; i++) {
        const EpdUnicodeInterval* interval = &intervals[i];
        if (code_point >= interval->first && code_point <= interval->last) {
            return &font->glyph[interval->offset + (code_point - interval->first)];
        }
        if (code_point < interval->first) {
            return NULL;
        }
    }
    return NULL;
}

static int uncompress(
    uint8_t* dest, size_t uncompressed_size, const uint8_t* source, size_t source_size
) {
    tinfl_decompressor* decomp;
    if (uncompressed_size == 0 || dest == NULL || source_size == 0 || source == NULL) {
        return -1;
    }
    decomp = malloc(sizeof(tinfl_decompressor));
    if (!decomp) {
        // Out of memory
        return -1;
    }
    tinfl_init(decomp);

    // we know everything will fit into the buffer.
    tinfl_status decomp_status = tinfl_decompress(
        decomp,
        source,
        &source_size,
        dest,
        dest,
        &uncompressed_size,
        TINFL_FLAG_PARSE_ZLIB_HEADER | TINFL_FLAG_USING_NON_WRAPPING_OUTPUT_BUF
    );
    free(decomp);
    if (decomp_status != TINFL_STATUS_DONE) {
        return decomp_status;
    }
    return 0;
}

/*!
   @brief   Draw a single character to a pre-allocated buffer.
*/
static enum EpdDrawError IRAM_ATTR draw_char(
    const EpdFont* font,
    uint8_t* buffer,
    int* cursor_x,
    int cursor_y,
    uint32_t cp,
    const EpdFontProperties* props
) {
    assert(props != NULL);

    const EpdGlyph* glyph = epd_get_glyph(font, cp);
    if (!glyph) {
        glyph = epd_get_glyph(font, props->fallback_glyph);
    }

    if (!glyph) {
        return EPD_DRAW_GLYPH_FALLBACK_FAILED;
    }

    uint32_t offset = glyph->data_offset;
    uint16_t width = glyph->width, height = glyph->height;
    int left = glyph->left;

    int byte_width = (width / 2 + width % 2);
    unsigned long bitmap_size = byte_width * height;
    const uint8_t* bitmap = NULL;
    if (bitmap_size > 0 && font->compressed) {
        uint8_t* tmp_bitmap = (uint8_t*)malloc(bitmap_size);
        if (tmp_bitmap == NULL && bitmap_size) {
            ESP_LOGE("font", "malloc failed.");
            return EPD_DRAW_FAILED_ALLOC;
        }
        uncompress(tmp_bitmap, bitmap_size, &font->bitmap[offset], glyph->compressed_size);
        bitmap = tmp_bitmap;
    } else {
        bitmap = &font->bitmap[offset];
    }

    uint8_t color_lut[16];
    for (int c = 0; c < 16; c++) {
        int color_difference = (int)props->fg_color - (int)props->bg_color;
        color_lut[c] = max(0, min(15, props->bg_color + c * color_difference / 15));
    }
    bool background_needed = props->flags & EPD_DRAW_BACKGROUND;

    for (int y = 0; y < height; y++) {
        int yy = cursor_y - glyph->top + y;
        int start_pos = *cursor_x + left;
        bool byte_complete = start_pos % 2;
        int x = max(0, -start_pos);
        int max_x = start_pos + width;
        uint8_t color;

        for (int xx = start_pos; xx < max_x; xx++) {
            uint8_t bm = bitmap[y * byte_width + x / 2];
            if ((x & 1) == 0) {
                bm = bm & 0xF;
            } else {
                bm = bm >> 4;
            }
            if (background_needed || bm) {
                color = color_lut[bm] << 4;
                epd_draw_pixel(xx, yy, color, buffer);
            }
            byte_complete = !byte_complete;
            x++;
        }
    }
    if (bitmap_size > 0 && font->compressed) {
        free((uint8_t*)bitmap);
    }
    *cursor_x += glyph->advance_x;
    return EPD_DRAW_SUCCESS;
}

/*!
 * @brief Calculate the bounds of a character when drawn at (x, y), move the
 * cursor (*x) forward, adjust the given bounds.
 */
static void get_char_bounds(
    const EpdFont* font,
    uint32_t cp,
    int* x,
    int* y,
    int* minx,
    int* miny,
    int* maxx,
    int* maxy,
    const EpdFontProperties* props
) {
    assert(props != NULL);

    const EpdGlyph* glyph = epd_get_glyph(font, cp);

    if (!glyph) {
        glyph = epd_get_glyph(font, props->fallback_glyph);
    }

    if (!glyph) {
        return;
    }

    int x1 = *x + glyph->left, y1 = *y + glyph->top - glyph->height, x2 = x1 + glyph->width,
        y2 = y1 + glyph->height;

    // background needs to be taken into account
    if (props->flags & EPD_DRAW_BACKGROUND) {
        *minx = min(*x, min(*minx, x1));
        *maxx = max(max(*x + glyph->advance_x, x2), *maxx);
        *miny = min(*y + font->descender, min(*miny, y1));
        *maxy = max(*y + font->ascender, max(*maxy, y2));
    } else {
        if (x1 < *minx)
            *minx = x1;
        if (y1 < *miny)
            *miny = y1;
        if (x2 > *maxx)
            *maxx = x2;
        if (y2 > *maxy)
            *maxy = y2;
    }
    *x += glyph->advance_x;
}

EpdRect epd_get_string_rect(
    const EpdFont* font,
    const char* string,
    int x,
    int y,
    int margin,
    const EpdFontProperties* properties
) {
    assert(properties != NULL);
    EpdFontProperties props = *properties;
    props.flags |= EPD_DRAW_BACKGROUND;
    EpdRect temp = { .x = x, .y = y, .width = 0, .height = 0 };
    if (*string == '\0')
        return temp;
    int minx = 100000, miny = 100000, maxx = -1, maxy = -1;
    int temp_x = x;
    int temp_y = y + font->ascender;

    // Go through each line and get it's co-ordinates
    uint32_t c;
    while ((c = next_cp((const uint8_t**)&string))) {
        if (c == 0x000A)  // newline
        {
            temp_x = x;
            temp_y += font->advance_y;
        } else
            get_char_bounds(font, c, &temp_x, &temp_y, &minx, &miny, &maxx, &maxy, &props);
    }
    temp.width = maxx - x + (margin * 2);
    temp.height = maxy - miny + (margin * 2);
    return temp;
}

void epd_get_text_bounds(
    const EpdFont* font,
    const char* string,
    const int* x,
    const int* y,
    int* x1,
    int* y1,
    int* w,
    int* h,
    const EpdFontProperties* properties
) {
    // FIXME: Does not respect alignment!

    assert(properties != NULL);
    EpdFontProperties props = *properties;

    if (*string == '\0') {
        *w = 0;
        *h = 0;
        *y1 = *y;
        *x1 = *x;
        return;
    }
    int minx = 100000, miny = 100000, maxx = -1, maxy = -1;
    int original_x = *x;
    int temp_x = *x;
    int temp_y = *y;
    uint32_t c;
    while ((c = next_cp((const uint8_t**)&string))) {
        get_char_bounds(font, c, &temp_x, &temp_y, &minx, &miny, &maxx, &maxy, &props);
    }
    *x1 = min(original_x, minx);
    *w = maxx - *x1;
    *y1 = miny;
    *h = maxy - miny;
}

static enum EpdDrawError epd_write_line(
    const EpdFont* font,
    const char* string,
    int* cursor_x,
    int* cursor_y,
    uint8_t* framebuffer,
    const EpdFontProperties* properties
) {
    assert(framebuffer != NULL);

    if (*string == '\0') {
        return EPD_DRAW_SUCCESS;
    }

    assert(properties != NULL);
    EpdFontProperties props = *properties;
    enum EpdFontFlags alignment_mask
        = EPD_DRAW_ALIGN_LEFT | EPD_DRAW_ALIGN_RIGHT | EPD_DRAW_ALIGN_CENTER;
    enum EpdFontFlags alignment = props.flags & alignment_mask;

    // alignments are mutually exclusive!
    if ((alignment & (alignment - 1)) != 0) {
        return EPD_DRAW_INVALID_FONT_FLAGS;
    }

    int x1 = 0, y1 = 0, w = 0, h = 0;
    int tmp_cur_x = *cursor_x;
    int tmp_cur_y = *cursor_y;
    epd_get_text_bounds(font, string, &tmp_cur_x, &tmp_cur_y, &x1, &y1, &w, &h, &props);

    // no printable characters
    if (w < 0 || h < 0) {
        return EPD_DRAW_NO_DRAWABLE_CHARACTERS;
    }

    uint8_t* buffer = framebuffer;
    int local_cursor_x = *cursor_x;
    int local_cursor_y = *cursor_y;
    uint32_t c;

    int cursor_x_init = local_cursor_x;
    int cursor_y_init = local_cursor_y;

    switch (alignment) {
        case EPD_DRAW_ALIGN_LEFT: {
            break;
        }
        case EPD_DRAW_ALIGN_CENTER: {
            local_cursor_x -= w / 2;
            break;
        }
        case EPD_DRAW_ALIGN_RIGHT: {
            local_cursor_x -= w;
            break;
        }
        default:
            break;
    }

    uint8_t bg = props.bg_color;
    if (props.flags & EPD_DRAW_BACKGROUND) {
        for (int l = local_cursor_y - font->ascender; l < local_cursor_y - font->descender; l++) {
            epd_draw_hline(local_cursor_x, l, w, bg << 4, buffer);
        }
    }
    enum EpdDrawError err = EPD_DRAW_SUCCESS;
    while ((c = next_cp((const uint8_t**)&string))) {
        err |= draw_char(font, buffer, &local_cursor_x, local_cursor_y, c, &props);
    }

    *cursor_x += local_cursor_x - cursor_x_init;
    *cursor_y += local_cursor_y - cursor_y_init;
    return err;
}

enum EpdDrawError epd_write_default(
    const EpdFont* font, const char* string, int* cursor_x, int* cursor_y, uint8_t* framebuffer
) {
    const EpdFontProperties props = epd_font_properties_default();
    return epd_write_string(font, string, cursor_x, cursor_y, framebuffer, &props);
}

enum EpdDrawError epd_write_string(
    const EpdFont* font,
    const char* string,
    int* cursor_x,
    int* cursor_y,
    uint8_t* framebuffer,
    const EpdFontProperties* properties
) {
    char *token, *newstring, *tofree;
    if (string == NULL) {
        ESP_LOGE("font.c", "cannot draw a NULL string!");
        return EPD_DRAW_STRING_INVALID;
    }
    tofree = newstring = strdup(string);
    if (newstring == NULL) {
        ESP_LOGE("font.c", "cannot allocate string copy!");
        return EPD_DRAW_FAILED_ALLOC;
    }

    enum EpdDrawError err = EPD_DRAW_SUCCESS;
    // taken from the strsep manpage
    int line_start = *cursor_x;
    while ((token = strsep(&newstring, "\n")) != NULL) {
        *cursor_x = line_start;
        err |= epd_write_line(font, token, cursor_x, cursor_y, framebuffer, properties);
        *cursor_y += font->advance_y;
    }

    free(tofree);
    return err;
}
