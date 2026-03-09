#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>
#include <stdatomic.h>
#include <stdint.h>

#include "../epdiy.h"
#include "line_queue.h"
#include "lut.h"

#define NUM_RENDER_THREADS 2

typedef struct {
    EpdRect area;
    EpdRect crop_to;
    const bool* drawn_lines;
    const uint8_t* data_ptr;

    /// The display width for quick access.
    int display_width;
    /// The display height for quick access.
    int display_height;

    /// index of the next line of data to process
    atomic_int lines_prepared;
    volatile int lines_consumed;
    int lines_total;

    /// frame currently in the current update cycle
    int current_frame;
    /// number of frames in the current update cycle
    int cycle_frames;

    TaskHandle_t feed_tasks[NUM_RENDER_THREADS];
    SemaphoreHandle_t feed_done_smphr[NUM_RENDER_THREADS];
    SemaphoreHandle_t frame_done;
    /// Line buffers for feed tasks
    uint8_t* feed_line_buffers[NUM_RENDER_THREADS];

    /// index of the waveform mode when using vendor waveforms.
    /// This is not necessarily the mode number if the waveform header
    // only contains a selection of modes!
    int waveform_index;
    /// waveform range when using vendor waveforms
    int waveform_range;
    /// Draw time for the current frame in 1/10ths of us.
    int frame_time;

    const int* phase_times;

    const EpdWaveform* waveform;
    enum EpdDrawMode mode;
    enum EpdDrawError error;

    // Lookup table size.
    size_t conversion_lut_size;
    // Lookup table space.
    uint8_t* conversion_lut;

    /// LUT lookup function. Must not be NULL.
    lut_func_t lut_lookup_func;
    /// LUT building function. Must not be NULL
    lut_build_func_t lut_build_func;

    /// Queue of lines prepared for output to the display,
    /// one for each thread.
    LineQueue_t line_queues[NUM_RENDER_THREADS];
    uint8_t* line_threads;

    // Output line mask
    uint8_t* line_mask;

    /// track line skipping when working in old i2s mode
    int skipping;

    /// line buffer when using epd_push_pixels
    uint8_t* static_line_buffer;
} RenderContext_t;

/**
 * Based on the render context, assign the bytes per line,
 * framebuffer start pointer, min and max vertical positions and the pixels per byte.
 */
void get_buffer_params(
    RenderContext_t* ctx,
    int* bytes_per_line,
    const uint8_t** start_ptr,
    int* min_y,
    int* max_y,
    int* pixels_per_byte
);

/**
 * Prepare the render context for drawing the next frame.
 *
 * (Reset counters, etc)
 */
void prepare_context_for_next_frame(RenderContext_t* ctx);

/**
 * Populate an output line mask from line dirtyness with two bits per pixel.
 * If the dirtyness data is NULL, set the mask to neutral.
 *
 * don't inline for to ensure availability in tests.
 */
void __attribute__((noinline)) epd_populate_line_mask(
    uint8_t* line_mask, const uint8_t* dirty_columns, int mask_len
);