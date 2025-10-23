/**
 * High-level API implementation for epdiy.
 */

#include <assert.h>
#include <esp_heap_caps.h>
#include <esp_log.h>
#include <esp_timer.h>
#include <esp_types.h>
#include <string.h>

#include "epd_highlevel.h"
#include "epdiy.h"

#ifndef _swap_int
#define _swap_int(a, b) \
    {                   \
        int t = a;      \
        a = b;          \
        b = t;          \
    }
#endif

static bool already_initialized = 0;

EpdiyHighlevelState epd_hl_init(const EpdWaveform* waveform) {
    assert(!already_initialized);
    if (waveform == NULL) {
        waveform = epd_get_display()->default_waveform;
    }

    int fb_size = epd_width() / 2 * epd_height();

#if !(defined(CONFIG_ESP32_SPIRAM_SUPPORT) || defined(CONFIG_ESP32S3_SPIRAM_SUPPORT))
    ESP_LOGW(
        "EPDiy", "Please enable PSRAM for the ESP32 (menuconfig→ Component config→ ESP32-specific)"
    );
#endif
    EpdiyHighlevelState state;
    state.back_fb = heap_caps_aligned_alloc(16, fb_size, MALLOC_CAP_SPIRAM);
    assert(state.back_fb != NULL);
    state.front_fb = heap_caps_aligned_alloc(16, fb_size, MALLOC_CAP_SPIRAM);
    assert(state.front_fb != NULL);
    state.difference_fb = heap_caps_aligned_alloc(16, 2 * fb_size, MALLOC_CAP_SPIRAM);
    assert(state.difference_fb != NULL);
    state.dirty_lines = malloc(epd_height() * sizeof(bool));
    assert(state.dirty_lines != NULL);
    state.dirty_columns
        = heap_caps_aligned_alloc(16, epd_width() / 2, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
    assert(state.dirty_columns != NULL);
    state.waveform = waveform;

    memset(state.front_fb, 0xFF, fb_size);
    memset(state.back_fb, 0xFF, fb_size);

    already_initialized = true;
    return state;
}

uint8_t* epd_hl_get_framebuffer(EpdiyHighlevelState* state) {
    assert(state != NULL);
    return state->front_fb;
}

enum EpdDrawError epd_hl_update_screen(
    EpdiyHighlevelState* state, enum EpdDrawMode mode, int temperature
) {
    return epd_hl_update_area(state, mode, temperature, epd_full_screen());
}

EpdRect _inverse_rotated_area(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    // If partial update uses full screen do not rotate anything
    if (!(x == 0 && y == 0 && epd_width() == w && epd_height() == h)) {
        // invert the current display rotation
        switch (epd_get_rotation()) {
            // 0 landscape: Leave it as is
            case EPD_ROT_LANDSCAPE:
                break;
            // 1 90 ° clockwise
            case EPD_ROT_PORTRAIT:
                _swap_int(x, y);
                _swap_int(w, h);
                x = epd_width() - x - w;
                break;

            case EPD_ROT_INVERTED_LANDSCAPE:
                // 3 180°
                x = epd_width() - x - w;
                y = epd_height() - y - h;
                break;

            case EPD_ROT_INVERTED_PORTRAIT:
                // 3 270 °
                _swap_int(x, y);
                _swap_int(w, h);
                y = epd_height() - y - h;
                break;
        }
    }

    EpdRect rotated = { x, y, w, h };
    return rotated;
}

enum EpdDrawError epd_hl_update_area(
    EpdiyHighlevelState* state, enum EpdDrawMode mode, int temperature, EpdRect area
) {
    assert(state != NULL);
    // Not right to rotate here since this copies part of buffer directly

    // Check rotation FIX
    EpdRect rotated_area = _inverse_rotated_area(area.x, area.y, area.width, area.height);
    area.x = rotated_area.x;
    area.y = rotated_area.y;
    area.width = rotated_area.width;
    area.height = rotated_area.height;

    uint32_t ts = esp_timer_get_time() / 1000;

    // FIXME: use crop information here, if available
    EpdRect diff_area = epd_difference_image_cropped(
        state->front_fb,
        state->back_fb,
        area,
        state->difference_fb,
        state->dirty_lines,
        state->dirty_columns
    );

    if (diff_area.height == 0 || diff_area.width == 0) {
        return EPD_DRAW_SUCCESS;
    }

    uint32_t t1 = esp_timer_get_time() / 1000;

    diff_area.x = 0;
    diff_area.y = 0;
    diff_area.width = epd_width();
    diff_area.height = epd_height();

    enum EpdDrawError err = EPD_DRAW_SUCCESS;
    err = epd_draw_base(
        epd_full_screen(),
        state->difference_fb,
        diff_area,
        MODE_PACKING_1PPB_DIFFERENCE | mode,
        temperature,
        state->dirty_lines,
        state->dirty_columns,
        state->waveform
    );

    uint32_t t2 = esp_timer_get_time() / 1000;

    diff_area.x = 0;
    diff_area.y = 0;
    diff_area.width = epd_width();
    diff_area.height = epd_height();

    int buf_width = epd_width();

    for (int l = diff_area.y; l < diff_area.y + diff_area.height; l++) {
        if (state->dirty_lines[l] > 0) {
            uint8_t* lfb = state->front_fb + buf_width / 2 * l;
            uint8_t* lbb = state->back_fb + buf_width / 2 * l;

            int x = diff_area.x;
            int x_last = diff_area.x + diff_area.width - 1;

            if (x % 2) {
                *(lbb + x / 2) = (*(lfb + x / 2) & 0xF0) | (*(lbb + x / 2) & 0x0F);
                x += 1;
            }

            if (!(x_last % 2)) {
                *(lbb + x_last / 2) = (*(lfb + x_last / 2) & 0x0F) | (*(lbb + x_last / 2) & 0xF0);
                x_last -= 1;
            }

            memcpy(lbb + (x / 2), lfb + (x / 2), (x_last - x + 1) / 2);
        }
    }

    uint32_t t3 = esp_timer_get_time() / 1000;

    ESP_LOGI(
        "epdiy",
        "diff: %dms, draw: %dms, buffer update: %dms, total: %dms",
        t1 - ts,
        t2 - t1,
        t3 - t2,
        t3 - ts
    );
    return err;
}

void epd_hl_set_all_white(EpdiyHighlevelState* state) {
    assert(state != NULL);
    int fb_size = epd_width() / 2 * epd_height();
    memset(state->front_fb, 0xFF, fb_size);
}

void epd_fullclear(EpdiyHighlevelState* state, int temperature) {
    assert(state != NULL);
    epd_hl_set_all_white(state);
    enum EpdDrawError err = epd_hl_update_screen(state, MODE_GC16, temperature);
    assert(err == EPD_DRAW_SUCCESS);
    epd_clear();
}

void epd_hl_waveform(EpdiyHighlevelState* state, const EpdWaveform* waveform) {
    if (waveform == NULL) {
        waveform = epd_get_display()->default_waveform;
    }
    state->waveform = waveform;
}