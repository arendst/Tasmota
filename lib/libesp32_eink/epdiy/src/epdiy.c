#include "epdiy.h"
#include "epd_board.h"
#include "epd_display.h"
#include "output_common/render_method.h"
#include "render.h"

#include <esp_assert.h>
#include <esp_heap_caps.h>
#include <esp_log.h>
#include <esp_types.h>

// Simple x and y coordinate
typedef struct {
    uint16_t x;
    uint16_t y;
} Coord_xy;

static const EpdDisplay_t* display = NULL;

// Display rotation. Can be updated using epd_set_rotation(enum EpdRotation)
static enum EpdRotation display_rotation = EPD_ROT_LANDSCAPE;

#ifndef _swap_int
#define _swap_int(a, b) \
    {                   \
        int t = a;      \
        a = b;          \
        b = t;          \
    }
#endif

EpdRect epd_full_screen() {
    EpdRect area = { .x = 0, .y = 0, .width = epd_width(), .height = epd_height() };
    return area;
}

void epd_clear() {
    epd_clear_area(epd_full_screen());
}

void epd_draw_hline(int x, int y, int length, uint8_t color, uint8_t* framebuffer) {
    for (int i = 0; i < length; i++) {
        int xx = x + i;
        epd_draw_pixel(xx, y, color, framebuffer);
    }
}

void epd_draw_vline(int x, int y, int length, uint8_t color, uint8_t* framebuffer) {
    for (int i = 0; i < length; i++) {
        int yy = y + i;
        epd_draw_pixel(x, yy, color, framebuffer);
    }
}

Coord_xy _rotate(uint16_t x, uint16_t y) {
    switch (display_rotation) {
        case EPD_ROT_LANDSCAPE:
            break;
        case EPD_ROT_PORTRAIT:
            _swap_int(x, y);
            x = epd_width() - x - 1;
            break;
        case EPD_ROT_INVERTED_LANDSCAPE:
            x = epd_width() - x - 1;
            y = epd_height() - y - 1;
            break;
        case EPD_ROT_INVERTED_PORTRAIT:
            _swap_int(x, y);
            y = epd_height() - y - 1;
            break;
    }
    Coord_xy coord = { x, y };
    return coord;
}

void epd_draw_pixel(int x, int y, uint8_t color, uint8_t* framebuffer) {
    // Check rotation and move pixel around if necessary
    Coord_xy coord = _rotate(x, y);
    x = coord.x;
    y = coord.y;

    if (x < 0 || x >= epd_width()) {
        return;
    }
    if (y < 0 || y >= epd_height()) {
        return;
    }

    uint8_t* buf_ptr = &framebuffer[y * epd_width() / 2 + x / 2];
    if (x % 2) {
        *buf_ptr = (*buf_ptr & 0x0F) | (color & 0xF0);
    } else {
        *buf_ptr = (*buf_ptr & 0xF0) | (color >> 4);
    }
}

void epd_draw_circle(int x0, int y0, int r, uint8_t color, uint8_t* framebuffer) {
    int f = 1 - r;
    int ddF_x = 1;
    int ddF_y = -2 * r;
    int x = 0;
    int y = r;

    epd_draw_pixel(x0, y0 + r, color, framebuffer);
    epd_draw_pixel(x0, y0 - r, color, framebuffer);
    epd_draw_pixel(x0 + r, y0, color, framebuffer);
    epd_draw_pixel(x0 - r, y0, color, framebuffer);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        epd_draw_pixel(x0 + x, y0 + y, color, framebuffer);
        epd_draw_pixel(x0 - x, y0 + y, color, framebuffer);
        epd_draw_pixel(x0 + x, y0 - y, color, framebuffer);
        epd_draw_pixel(x0 - x, y0 - y, color, framebuffer);
        epd_draw_pixel(x0 + y, y0 + x, color, framebuffer);
        epd_draw_pixel(x0 - y, y0 + x, color, framebuffer);
        epd_draw_pixel(x0 + y, y0 - x, color, framebuffer);
        epd_draw_pixel(x0 - y, y0 - x, color, framebuffer);
    }
}

void epd_fill_circle(int x0, int y0, int r, uint8_t color, uint8_t* framebuffer) {
    epd_draw_vline(x0, y0 - r, 2 * r + 1, color, framebuffer);
    epd_fill_circle_helper(x0, y0, r, 3, 0, color, framebuffer);
}

void epd_fill_circle_helper(
    int x0, int y0, int r, int corners, int delta, uint8_t color, uint8_t* framebuffer
) {
    int f = 1 - r;
    int ddF_x = 1;
    int ddF_y = -2 * r;
    int x = 0;
    int y = r;
    int px = x;
    int py = y;

    delta++;  // Avoid some +1's in the loop

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        // These checks avoid double-drawing certain lines, important
        // for the SSD1306 library which has an INVERT drawing mode.
        if (x < (y + 1)) {
            if (corners & 1)
                epd_draw_vline(x0 + x, y0 - y, 2 * y + delta, color, framebuffer);
            if (corners & 2)
                epd_draw_vline(x0 - x, y0 - y, 2 * y + delta, color, framebuffer);
        }
        if (y != py) {
            if (corners & 1)
                epd_draw_vline(x0 + py, y0 - px, 2 * px + delta, color, framebuffer);
            if (corners & 2)
                epd_draw_vline(x0 - py, y0 - px, 2 * px + delta, color, framebuffer);
            py = y;
        }
        px = x;
    }
}

void epd_draw_rect(EpdRect rect, uint8_t color, uint8_t* framebuffer) {
    int x = rect.x;
    int y = rect.y;
    int w = rect.width;
    int h = rect.height;
    epd_draw_hline(x, y, w, color, framebuffer);
    epd_draw_hline(x, y + h - 1, w, color, framebuffer);
    epd_draw_vline(x, y, h, color, framebuffer);
    epd_draw_vline(x + w - 1, y, h, color, framebuffer);
}

void epd_fill_rect(EpdRect rect, uint8_t color, uint8_t* framebuffer) {
    int x = rect.x;
    int y = rect.y;
    int w = rect.width;
    int h = rect.height;
    for (int i = y; i < y + h; i++) {
        epd_draw_hline(x, i, w, color, framebuffer);
    }
}

static void epd_write_line(int x0, int y0, int x1, int y1, uint8_t color, uint8_t* framebuffer) {
    int steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        _swap_int(x0, y0);
        _swap_int(x1, y1);
    }

    if (x0 > x1) {
        _swap_int(x0, x1);
        _swap_int(y0, y1);
    }

    int dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int err = dx / 2;
    int ystep;

    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }

    for (; x0 <= x1; x0++) {
        if (steep) {
            epd_draw_pixel(y0, x0, color, framebuffer);
        } else {
            epd_draw_pixel(x0, y0, color, framebuffer);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

void epd_draw_line(int x0, int y0, int x1, int y1, uint8_t color, uint8_t* framebuffer) {
    // Update in subclasses if desired!
    if (x0 == x1) {
        if (y0 > y1)
            _swap_int(y0, y1);
        epd_draw_vline(x0, y0, y1 - y0 + 1, color, framebuffer);
    } else if (y0 == y1) {
        if (x0 > x1)
            _swap_int(x0, x1);
        epd_draw_hline(x0, y0, x1 - x0 + 1, color, framebuffer);
    } else {
        epd_write_line(x0, y0, x1, y1, color, framebuffer);
    }
}

void epd_draw_triangle(
    int x0, int y0, int x1, int y1, int x2, int y2, uint8_t color, uint8_t* framebuffer
) {
    epd_draw_line(x0, y0, x1, y1, color, framebuffer);
    epd_draw_line(x1, y1, x2, y2, color, framebuffer);
    epd_draw_line(x2, y2, x0, y0, color, framebuffer);
}

void epd_fill_triangle(
    int x0, int y0, int x1, int y1, int x2, int y2, uint8_t color, uint8_t* framebuffer
) {
    int a, b, y, last;

    // Sort coordinates by Y order (y2 >= y1 >= y0)
    if (y0 > y1) {
        _swap_int(y0, y1);
        _swap_int(x0, x1);
    }
    if (y1 > y2) {
        _swap_int(y2, y1);
        _swap_int(x2, x1);
    }
    if (y0 > y1) {
        _swap_int(y0, y1);
        _swap_int(x0, x1);
    }

    if (y0 == y2) {  // Handle awkward all-on-same-line case as its own thing
        a = b = x0;
        if (x1 < a)
            a = x1;
        else if (x1 > b)
            b = x1;
        if (x2 < a)
            a = x2;
        else if (x2 > b)
            b = x2;
        epd_draw_hline(a, y0, b - a + 1, color, framebuffer);
        return;
    }

    int dx01 = x1 - x0, dy01 = y1 - y0, dx02 = x2 - x0, dy02 = y2 - y0, dx12 = x2 - x1,
        dy12 = y2 - y1;
    int32_t sa = 0, sb = 0;

    // For upper part of triangle, find scanline crossings for segments
    // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
    // is included here (and second loop will be skipped, avoiding a /0
    // error there), otherwise scanline y1 is skipped here and handled
    // in the second loop...which also avoids a /0 error here if y0=y1
    // (flat-topped triangle).
    if (y1 == y2)
        last = y1;  // Include y1 scanline
    else
        last = y1 - 1;  // Skip it

    for (y = y0; y <= last; y++) {
        a = x0 + sa / dy01;
        b = x0 + sb / dy02;
        sa += dx01;
        sb += dx02;
        /* longhand:
        a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if (a > b)
            _swap_int(a, b);
        epd_draw_hline(a, y, b - a + 1, color, framebuffer);
    }

    // For lower part of triangle, find scanline crossings for segments
    // 0-2 and 1-2.  This loop is skipped if y1=y2.
    sa = (int32_t)dx12 * (y - y1);
    sb = (int32_t)dx02 * (y - y0);
    for (; y <= y2; y++) {
        a = x1 + sa / dy12;
        b = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;
        /* longhand:
        a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if (a > b)
            _swap_int(a, b);
        epd_draw_hline(a, y, b - a + 1, color, framebuffer);
    }
}

void epd_copy_to_framebuffer(EpdRect image_area, const uint8_t* image_data, uint8_t* framebuffer) {
    assert(framebuffer != NULL);

    for (uint32_t i = 0; i < image_area.width * image_area.height; i++) {
        uint32_t value_index = i;
        // for images of uneven width,
        // consume an additional nibble per row.
        if (image_area.width % 2) {
            value_index += i / image_area.width;
        }
        uint8_t val = (value_index % 2) ? (image_data[value_index / 2] & 0xF0) >> 4
                                        : image_data[value_index / 2] & 0x0F;

        int xx = image_area.x + i % image_area.width;
        if (xx < 0 || xx >= epd_width()) {
            continue;
        }
        int yy = image_area.y + i / image_area.width;
        if (yy < 0 || yy >= epd_height()) {
            continue;
        }
        uint8_t* buf_ptr = &framebuffer[yy * epd_width() / 2 + xx / 2];
        if (xx % 2) {
            *buf_ptr = (*buf_ptr & 0x0F) | (val << 4);
        } else {
            *buf_ptr = (*buf_ptr & 0xF0) | val;
        }
    }
}

enum EpdDrawError epd_draw_image(EpdRect area, const uint8_t* data, const EpdWaveform* waveform) {
    int temperature = epd_ambient_temperature();
    assert(waveform != NULL);
    EpdRect no_crop = {
        .x = 0,
        .y = 0,
        .width = 0,
        .height = 0,
    };
    return epd_draw_base(area, data, no_crop, EPD_MODE_DEFAULT, temperature, NULL, NULL, waveform);
}

void epd_set_rotation(enum EpdRotation rotation) {
    display_rotation = rotation;
}

enum EpdRotation epd_get_rotation() {
    return display_rotation;
}

int epd_rotated_display_width() {
    int display_width = epd_width();
    switch (display_rotation) {
        case EPD_ROT_PORTRAIT:
            display_width = epd_height();
            break;
        case EPD_ROT_INVERTED_PORTRAIT:
            display_width = epd_height();
            break;
        case EPD_ROT_INVERTED_LANDSCAPE:
        case EPD_ROT_LANDSCAPE:
            break;
    }
    return display_width;
}

int epd_rotated_display_height() {
    int display_height = epd_height();
    switch (display_rotation) {
        case EPD_ROT_PORTRAIT:
            display_height = epd_width();
            break;
        case EPD_ROT_INVERTED_PORTRAIT:
            display_height = epd_width();
            break;
        case EPD_ROT_INVERTED_LANDSCAPE:
        case EPD_ROT_LANDSCAPE:
            break;
    }
    return display_height;
}

uint8_t epd_get_pixel(int x, int y, int fb_width, int fb_height, const uint8_t* framebuffer) {
    if (x < 0 || x >= fb_width) {
        return 0;
    }
    if (y < 0 || y >= fb_height) {
        return 0;
    }
    int fb_width_bytes = fb_width / 2 + fb_width % 2;
    uint8_t buf_val = framebuffer[y * fb_width_bytes + x / 2];
    if (x % 2) {
        buf_val = (buf_val & 0xF0) >> 4;
    } else {
        buf_val = (buf_val & 0x0F);
    }

    // epd_draw_pixel needs a 0 -> 255 value
    return buf_val << 4;
}

static void draw_rotated_transparent_image(
    EpdRect image_area,
    const uint8_t* image_buffer,
    uint8_t* framebuffer,
    uint8_t* transparent_color
) {
    uint16_t x_offset = 0;
    uint16_t y_offset = 0;
    uint8_t pixel_color;
    for (uint16_t y = 0; y < image_area.height; y++) {
        for (uint16_t x = 0; x < image_area.width; x++) {
            x_offset = image_area.x + x;
            y_offset = image_area.y + y;
            if (x_offset >= epd_rotated_display_width())
                continue;
            if (y_offset >= epd_rotated_display_height())
                continue;
            pixel_color = epd_get_pixel(x, y, image_area.width, image_area.height, image_buffer);
            if (transparent_color == NULL || pixel_color != (*transparent_color))
                epd_draw_pixel(x_offset, y_offset, pixel_color, framebuffer);
        }
    }
}

void epd_draw_rotated_transparent_image(
    EpdRect image_area, const uint8_t* image_buffer, uint8_t* framebuffer, uint8_t transparent_color
) {
    draw_rotated_transparent_image(image_area, image_buffer, framebuffer, &transparent_color);
}

void epd_draw_rotated_image(EpdRect image_area, const uint8_t* image_buffer, uint8_t* framebuffer) {
    if (epd_get_rotation() != EPD_ROT_LANDSCAPE) {
        draw_rotated_transparent_image(image_area, image_buffer, framebuffer, NULL);
    } else {
        epd_copy_to_framebuffer(image_area, image_buffer, framebuffer);
    }
}

void epd_poweron() {
    epd_current_board()->poweron(epd_ctrl_state());
}

void epd_poweroff() {
    epd_current_board()->poweroff(epd_ctrl_state());
}

void epd_init(
    const EpdBoardDefinition* board, const EpdDisplay_t* disp, enum EpdInitOptions options
) {
    display = disp;
    epd_set_board(board);
    epd_renderer_init(options);
}

void epd_deinit() {
    epd_renderer_deinit();
}

float epd_ambient_temperature() {
    if (!epd_current_board()) {
        ESP_LOGE("epdiy", "Could not read temperature: board not set!");
        return 0.0;
    }

    if (!epd_current_board()->get_temperature) {
        ESP_LOGW("epdiy", "No ambient temperature sensor - returning 21C");
        return 21.0;
    }
    return epd_current_board()->get_temperature();
}

void epd_set_vcom(uint16_t vcom) {
    if (!epd_current_board()) {
        ESP_LOGE("epdiy", "Could not set VCOM: board not set!");
        return;
    }

    if (!epd_current_board()->set_vcom) {
        ESP_LOGE("epdiy", "Could not set VCOM: board does not support setting VCOM in software.");
        return;
    }
    return epd_current_board()->set_vcom(vcom);
}

const EpdDisplay_t* epd_get_display() {
    assert(display != NULL);
    return display;
}

int epd_width() {
    return display->width;
}

int epd_height() {
    return display->height;
}

void epd_set_lcd_pixel_clock_MHz(int frequency) {
#ifdef RENDER_METHOD_LCD
    void epd_lcd_set_pixel_clock_MHz(int frequency);
    epd_lcd_set_pixel_clock_MHz(frequency);
#else
    ESP_LOGW("epdiy", "called set_lcd_pixel_clock_MHz, but LCD driver is not used!");
#endif
}
