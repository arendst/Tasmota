#pragma once

#include <stdint.h>
#include "epdiy.h"

// Make a block of 4 pixels lighter on the EPD.
#define CLEAR_BYTE 0B10101010
// Make a block of 4 pixels darker on the EPD.
#define DARK_BYTE 0B01010101

/**
 * Type signature of a framebuffer to display output lookup function.
 */
typedef void (*lut_func_t)(
    const uint32_t* line_buffer, uint8_t* epd_input, const uint8_t* lut, uint32_t epd_width
);

/**
 * Type signature of a LUT preparation function.
 */
typedef void (*lut_build_func_t)(uint8_t* lut, const EpdWaveformPhases* phases, int frame);

typedef struct {
    lut_build_func_t build_func;
    lut_func_t lookup_func;
} LutFunctionPair;

/**
 * Select the appropriate LUT building and lookup function
 * for the selected draw mode and allocated LUT size.
 */
LutFunctionPair find_lut_functions(enum EpdDrawMode mode, uint32_t lut_size);

/*
 * Reorder the output buffer to account for I2S FIFO order.
 */
void reorder_line_buffer(uint32_t* line_data, int buf_len);

/**
 * Apply a mask to a line buffer.
 * `len` must be divisible by 4.
 */
void epd_apply_line_mask(uint8_t* buf, const uint8_t* mask, int len);

// legacy functions
void bit_shift_buffer_right(uint8_t* buf, uint32_t len, int shift);
void nibble_shift_buffer_right(uint8_t* buf, uint32_t len);
