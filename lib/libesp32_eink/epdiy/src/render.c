#include "render.h"

#include "epd_board.h"
#include "epd_internals.h"
#include "epdiy.h"

#include <assert.h>
#include <esp_log.h>
#include <esp_timer.h>
#include <esp_types.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "esp_heap_caps.h"
#include "output_common/line_queue.h"
#include "output_common/lut.h"
#include "output_common/render_context.h"
#include "output_common/render_method.h"
#include "output_i2s/render_i2s.h"
#include "output_lcd/render_lcd.h"

static inline int min(int x, int y) {
    return x < y ? x : y;
}
static inline int max(int x, int y) {
    return x > y ? x : y;
}

const int clear_cycle_time = 12;

#define RTOS_ERROR_CHECK(x)       \
    do {                          \
        esp_err_t __err_rc = (x); \
        if (__err_rc != pdPASS) { \
            abort();              \
        }                         \
    } while (0)

static RenderContext_t render_context;

void epd_push_pixels(EpdRect area, short time, int color) {
    render_context.area = area;
#ifdef RENDER_METHOD_LCD
    epd_push_pixels_lcd(&render_context, time, color);
#else
    epd_push_pixels_i2s(&render_context, area, time, color);
#endif
}

///////////////////////////// Coordination ///////////////////////////////

/**
 * Find the waveform temperature range index for a given temperature in °C.
 * If no range in the waveform data fits the given temperature, return the
 * closest one.
 * Returns -1 if the waveform does not contain any temperature range.
 */
int waveform_temp_range_index(const EpdWaveform* waveform, int temperature) {
    int idx = 0;
    if (waveform->num_temp_ranges == 0) {
        return -1;
    }
    while (idx < waveform->num_temp_ranges - 1 && waveform->temp_intervals[idx].min < temperature) {
        idx++;
    }
    return idx;
}

static int get_waveform_index(const EpdWaveform* waveform, enum EpdDrawMode mode) {
    for (int i = 0; i < waveform->num_modes; i++) {
        if (waveform->mode_data[i]->type == (mode & 0x3F)) {
            return i;
        }
    }
    return -1;
}

/////////////////////////////  API Procedures //////////////////////////////////

/// Rounded up display height for even division into multi-line buffers.
static inline int rounded_display_height() {
    return (((epd_height() + 7) / 8) * 8);
}

// FIXME: fix misleading naming:
//  area -> buffer dimensions
//  crop -> area taken out of buffer
enum EpdDrawError IRAM_ATTR epd_draw_base(
    EpdRect area,
    const uint8_t* data,
    EpdRect crop_to,
    enum EpdDrawMode mode,
    int temperature,
    const bool* drawn_lines,
    const uint8_t* drawn_columns,
    const EpdWaveform* waveform
) {
    if (waveform == NULL) {
        return EPD_DRAW_NO_PHASES_AVAILABLE;
    }
    int waveform_range = waveform_temp_range_index(waveform, temperature);
    if (waveform_range < 0) {
        return EPD_DRAW_NO_PHASES_AVAILABLE;
    }
    int waveform_index = 0;
    uint8_t frame_count = 0;
    const EpdWaveformPhases* waveform_phases = NULL;

    // no waveform required for monochrome mode
    if (!(mode & MODE_EPDIY_MONOCHROME)) {
        waveform_index = get_waveform_index(waveform, mode);
        if (waveform_index < 0) {
            return EPD_DRAW_MODE_NOT_FOUND;
        }

        waveform_phases = waveform->mode_data[waveform_index]->range_data[waveform_range];
        // FIXME: error if not present
        frame_count = waveform_phases->phases;
    } else {
        frame_count = 1;
    }

    if (crop_to.width < 0 || crop_to.height < 0) {
        return EPD_DRAW_INVALID_CROP;
    }

    const bool crop = (crop_to.width > 0 && crop_to.height > 0);
    if (crop
        && (crop_to.width > area.width || crop_to.height > area.height || crop_to.x > area.width
            || crop_to.y > area.height)) {
        return EPD_DRAW_INVALID_CROP;
    }

#ifdef RENDER_METHOD_LCD
    if (mode & MODE_PACKING_1PPB_DIFFERENCE && render_context.conversion_lut_size > 1 << 10) {
        ESP_LOGI(
            "epdiy",
            "Using optimized vector implementation on the ESP32-S3, only 1k of %d LUT in use!",
            render_context.conversion_lut_size
        );
    }
#endif

    LutFunctionPair lut_functions = find_lut_functions(mode, render_context.conversion_lut_size);
    if (lut_functions.build_func == NULL || lut_functions.lookup_func == NULL) {
        ESP_LOGE("epdiy", "no output lookup method found for your mode and LUT size!");
        return EPD_DRAW_LOOKUP_NOT_IMPLEMENTED;
    }

    render_context.area = area;
    render_context.crop_to = crop_to;
    render_context.waveform_range = waveform_range;
    render_context.waveform_index = waveform_index;
    render_context.mode = mode;
    render_context.waveform = waveform;
    render_context.error = EPD_DRAW_SUCCESS;
    render_context.drawn_lines = drawn_lines;
    render_context.data_ptr = data;
    render_context.lut_build_func = lut_functions.build_func;
    render_context.lut_lookup_func = lut_functions.lookup_func;

    render_context.lines_prepared = 0;
    render_context.lines_consumed = 0;
    render_context.lines_total = rounded_display_height();
    render_context.current_frame = 0;
    render_context.cycle_frames = frame_count;
    render_context.phase_times = NULL;
    if (waveform_phases != NULL && waveform_phases->phase_times != NULL) {
        render_context.phase_times = waveform_phases->phase_times;
    }

    epd_populate_line_mask(
        render_context.line_mask, drawn_columns, render_context.display_width / 4
    );

#ifdef RENDER_METHOD_I2S
    i2s_do_update(&render_context);
#elif defined(RENDER_METHOD_LCD)
    lcd_do_update(&render_context);
#endif

    if (render_context.error & EPD_DRAW_EMPTY_LINE_QUEUE) {
        ESP_LOGE("epdiy", "line buffer underrun occurred!");
    }

    if (render_context.error != EPD_DRAW_SUCCESS) {
        return render_context.error;
    }
    return EPD_DRAW_SUCCESS;
}

static void IRAM_ATTR render_thread(void* arg) {
    int thread_id = (int)arg;

    while (true) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

#ifdef RENDER_METHOD_LCD
        lcd_calculate_frame(&render_context, thread_id);
#elif defined(RENDER_METHOD_I2S)
        if (thread_id == 0) {
            i2s_fetch_frame_data(&render_context, thread_id);
        } else {
            i2s_output_frame(&render_context, thread_id);
        }
#endif

        xSemaphoreGive(render_context.feed_done_smphr[thread_id]);
    }
}

void epd_clear_area(EpdRect area) {
    epd_clear_area_cycles(area, 3, clear_cycle_time);
}

void epd_clear_area_cycles(EpdRect area, int cycles, int cycle_time) {
    const short white_time = cycle_time;
    const short dark_time = cycle_time;

    for (int c = 0; c < cycles; c++) {
        for (int i = 0; i < 10; i++) {
            epd_push_pixels(area, dark_time, 0);
        }
        for (int i = 0; i < 10; i++) {
            epd_push_pixels(area, white_time, 1);
        }
        for (int i = 0; i < 2; i++) {
            epd_push_pixels(area, white_time, 2);
        }
    }
}

void epd_renderer_init(enum EpdInitOptions options) {
    // Either the board should be set in menuconfig or the epd_set_board() must
    // be called before epd_init()
    assert((epd_current_board() != NULL));

    epd_current_board()->init(epd_width());
    epd_control_reg_init();

    render_context.display_width = epd_width();
    render_context.display_height = epd_height();

    size_t lut_size = 0;
    if (options & EPD_LUT_1K) {
        lut_size = 1 << 10;
    } else if (options & EPD_LUT_64K) {
        lut_size = 1 << 16;
    } else if (options == EPD_OPTIONS_DEFAULT) {
#ifdef RENDER_METHOD_LCD
        lut_size = 1 << 10;
#else
        lut_size = 1 << 16;
#endif
    } else {
        ESP_LOGE("epd", "invalid init options: %d", options);
        return;
    }

    ESP_LOGI("epd", "Space used for waveform LUT: %dK", lut_size / 1024);
    render_context.conversion_lut
        = (uint8_t*)heap_caps_malloc(lut_size, MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL);
    if (render_context.conversion_lut == NULL) {
        ESP_LOGE("epd", "could not allocate LUT!");
        abort();
    }
    render_context.conversion_lut_size = lut_size;
    render_context.static_line_buffer = NULL;

    render_context.frame_done = xSemaphoreCreateBinary();

    for (int i = 0; i < NUM_RENDER_THREADS; i++) {
        render_context.feed_done_smphr[i] = xSemaphoreCreateBinary();
    }

    // When using the LCD peripheral, we may need padding lines to
    // satisfy the bounce buffer size requirements
    render_context.line_threads = (uint8_t*)heap_caps_malloc(
        rounded_display_height(), MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL
    );

    int queue_len = 32;
    if (options & EPD_FEED_QUEUE_32) {
        queue_len = 32;
    } else if (options & EPD_FEED_QUEUE_8) {
        queue_len = 8;
    }

    if (render_context.conversion_lut == NULL) {
        ESP_LOGE("epd", "could not allocate line mask!");
        abort();
    }

    render_context.line_mask
        = heap_caps_aligned_alloc(16, epd_width() / 4, MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL);
    assert(render_context.line_mask != NULL);

#ifdef RENDER_METHOD_LCD
    size_t queue_elem_size = render_context.display_width / 4;
#elif defined(RENDER_METHOD_I2S)
    size_t queue_elem_size = render_context.display_width;
#endif

    for (int i = 0; i < NUM_RENDER_THREADS; i++) {
        render_context.line_queues[i] = lq_init(queue_len, queue_elem_size);
        render_context.feed_line_buffers[i] = (uint8_t*)heap_caps_malloc(
            render_context.display_width, MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL
        );
        assert(render_context.feed_line_buffers[i] != NULL);
        RTOS_ERROR_CHECK(xTaskCreatePinnedToCore(
            render_thread,
            "epd_prep",
            1 << 12,
            (void*)i,
            configMAX_PRIORITIES - 1,
            &render_context.feed_tasks[i],
            i
        ));
    }
}

void epd_renderer_deinit() {
    const EpdBoardDefinition* epd_board = epd_current_board();

    epd_board->poweroff(epd_ctrl_state());

    for (int i = 0; i < NUM_RENDER_THREADS; i++) {
        vTaskDelete(render_context.feed_tasks[i]);
        lq_free(&render_context.line_queues[i]);
        heap_caps_free(render_context.feed_line_buffers[i]);
        vSemaphoreDelete(render_context.feed_done_smphr[i]);
    }

#ifdef RENDER_METHOD_I2S
    i2s_deinit();
#endif

    epd_control_reg_deinit();

    if (epd_board->deinit) {
        epd_board->deinit();
    }

    heap_caps_free(render_context.conversion_lut);
    heap_caps_free(render_context.line_threads);
    heap_caps_free(render_context.line_mask);
    vSemaphoreDelete(render_context.frame_done);
}

#ifdef RENDER_METHOD_LCD
uint32_t epd_interlace_4bpp_line_VE(
    const uint8_t* to,
    const uint8_t* from,
    uint8_t* interlaced,
    uint8_t* col_dirtyness,
    int fb_width
);
#endif

/**
 * Interlaces `len` nibbles from the buffers `to` and `from` into `interlaced`.
 * In the process, tracks which nibbles differ in `col_dirtyness`.
 * Returns `1` if there are differences, `0` otherwise.
 * Does not require special alignment of the buffers beyond 32 bit alignment.
 */
__attribute__((optimize("O3"))) static inline int _interlace_line_unaligned(
    const uint8_t* to, const uint8_t* from, uint8_t* interlaced, uint8_t* col_dirtyness, int len
) {
    int dirty = 0;
    for (int x = 0; x < len; x++) {
        uint8_t t = *(to + x / 2);
        uint8_t f = *(from + x / 2);
        t = (x % 2) ? (t >> 4) : (t & 0x0f);
        f = (x % 2) ? (f >> 4) : (f & 0x0f);
        col_dirtyness[x / 2] |= (t ^ f) << (4 * (x % 2));
        dirty |= (t ^ f);
        interlaced[x] = (t << 4) | f;
    }
    return dirty;
}

/**
 * Interlaces the lines at `to`, `from` into `interlaced`.
 * returns `1` if there are differences, `0` otherwise.
 */
__attribute__((optimize("O3"))) bool _epd_interlace_line(
    const uint8_t* to,
    const uint8_t* from,
    uint8_t* interlaced,
    uint8_t* col_dirtyness,
    int fb_width
) {
#ifdef RENDER_METHOD_I2S
    return _interlace_line_unaligned(to, from, interlaced, col_dirtyness, fb_width) > 0;
#elif defined(RENDER_METHOD_LCD)
    // Use Vector Extensions with the ESP32-S3.
    // Both input buffers should have the same alignment w.r.t. 16 bytes,
    // as asserted in epd_difference_image_base.
    uint32_t dirty = 0;

    // alignment boundaries in pixels
    int unaligned_len_front_px = ((16 - (uint32_t)to % 16) * 2) % 32;
    int unaligned_len_back_px = (((uint32_t)to + fb_width / 2) % 16) * 2;
    int unaligned_back_start_px = fb_width - unaligned_len_back_px;
    int aligned_len_px = fb_width - unaligned_len_front_px - unaligned_len_back_px;

    dirty |= _interlace_line_unaligned(to, from, interlaced, col_dirtyness, unaligned_len_front_px);
    dirty |= epd_interlace_4bpp_line_VE(
        to + unaligned_len_front_px / 2,
        from + unaligned_len_front_px / 2,
        interlaced + unaligned_len_front_px,
        col_dirtyness + unaligned_len_front_px / 2,
        aligned_len_px
    );
    dirty |= _interlace_line_unaligned(
        to + unaligned_back_start_px / 2,
        from + unaligned_back_start_px / 2,
        interlaced + unaligned_back_start_px,
        col_dirtyness + unaligned_back_start_px / 2,
        unaligned_len_back_px
    );
    return dirty;
#endif
}

EpdRect epd_difference_image_base(
    const uint8_t* to,
    const uint8_t* from,
    EpdRect crop_to,
    int fb_width,
    int fb_height,
    uint8_t* interlaced,
    bool* dirty_lines,
    uint8_t* col_dirtyness
) {
    assert(fb_width % 8 == 0);
    assert(col_dirtyness != NULL);

    // these buffers should be allocated 16 byte aligned
    assert((uint32_t)to % 16 == 0);
    assert((uint32_t)from % 16 == 0);
    assert((uint32_t)col_dirtyness % 16 == 0);
    assert((uint32_t)interlaced % 16 == 0);

    memset(col_dirtyness, 0, fb_width / 2);
    memset(dirty_lines, 0, sizeof(bool) * fb_height);

    int x_end = min(fb_width, crop_to.x + crop_to.width);
    int y_end = min(fb_height, crop_to.y + crop_to.height);

    for (int y = crop_to.y; y < y_end; y++) {
        uint32_t offset = y * fb_width / 2;
        int dirty = _epd_interlace_line(
            to + offset, from + offset, interlaced + offset * 2, col_dirtyness, fb_width
        );
        dirty_lines[y] = dirty;
    }

    int min_x, min_y, max_x, max_y;
    for (min_x = crop_to.x; min_x < x_end; min_x++) {
        uint8_t mask = min_x % 2 ? 0xF0 : 0x0F;
        if ((col_dirtyness[min_x / 2] & mask) != 0)
            break;
    }
    for (max_x = x_end - 1; max_x >= crop_to.x; max_x--) {
        uint8_t mask = min_x % 2 ? 0xF0 : 0x0F;
        if ((col_dirtyness[max_x / 2] & mask) != 0)
            break;
    }
    for (min_y = crop_to.y; min_y < y_end; min_y++) {
        if (dirty_lines[min_y] != 0)
            break;
    }
    for (max_y = y_end - 1; max_y >= crop_to.y; max_y--) {
        if (dirty_lines[max_y] != 0)
            break;
    }

    EpdRect crop_rect = {
        .x = min_x,
        .y = min_y,
        .width = max(max_x - min_x + 1, 0),
        .height = max(max_y - min_y + 1, 0),
    };

    return crop_rect;
}

EpdRect epd_difference_image(
    const uint8_t* to,
    const uint8_t* from,
    uint8_t* interlaced,
    bool* dirty_lines,
    uint8_t* col_dirtyness
) {
    return epd_difference_image_base(
        to,
        from,
        epd_full_screen(),
        epd_width(),
        epd_height(),
        interlaced,
        dirty_lines,
        col_dirtyness
    );
}

EpdRect epd_difference_image_cropped(
    const uint8_t* to,
    const uint8_t* from,
    EpdRect crop_to,
    uint8_t* interlaced,
    bool* dirty_lines,
    uint8_t* col_dirtyness
) {
    EpdRect result = epd_difference_image_base(
        to, from, crop_to, epd_width(), epd_height(), interlaced, dirty_lines, col_dirtyness
    );
    return result;
}
