// Copyright 2020-2021 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/**
 * c99 standard still doesn't strictly inline functions
 * We need to use attribute as well to do this.
 */
#define __NN_FORCE_INLINE__ __attribute((always_inline)) static inline

/* min/max macros */
#ifndef max
#define max(a, b) ({            \
    __typeof__ (a) _a = (a);    \
    __typeof__ (b) _b = (b);    \
    _a > _b ? _a : _b;          \
})

#define min(a, b) ({            \
    __typeof__ (a) _a = (a);    \
    __typeof__ (b) _b = (b);    \
    _a < _b ? _a : _b;          \
})
#endif

__NN_FORCE_INLINE__ int32_t esp_nn_clz32(uint32_t in)
{
#if CONFIG_IDF_TARGET_ARCH_XTENSA
    __asm__ volatile("nsau %0, %0" : "+r" (in));
    return in;
#elif defined(__GNUC__)
    return __builtin_clz(in);
#else
    int32_t count = 32;
    uint32_t x = in, y = in >> 16;
    if (y != 0) {
        count -= 16;
        x = y;
    }
    y = x >> 8;
    if (y != 0) {
        count -= 8;
        x = y;
    }
    y = x >> 4;
    if (y != 0) {
        count -= 4;
        x = y;
    }
    y = x >> 2;
    if (y != 0) {
        count -= 2;
        x = y;
    }
    y = x >> 1;
    if (y != 0) {
        return count - 2;
    }
    return count - x;
#endif
}

/**
 * Signed saturate a 32 bit value to 8 bits keeping output in 32 bit variable.
 */
__NN_FORCE_INLINE__ int32_t esp_nn_saturate8(int32_t in)
{
#if CONFIG_IDF_TARGET_ARCH_XTENSA
    __asm__ volatile("clamps %0, %0, 7" : "+a"(in));
    return in;
#else
    return max(INT8_MIN, min(in, INT8_MAX));
#endif
}

__NN_FORCE_INLINE__ int32_t esp_nn_pick_sat_high32_of64(int64_t val64)
{
    int32_t sign = (int32_t) (val64 >> 63);
    int32_t to_add = sign & ((1ul << 31) - 1);
    return (int32_t) ((int64_t) (val64 + to_add) >> 31);
}

__NN_FORCE_INLINE__ int32_t esp_nn_sat_round_doubling_high_mul(int32_t in0, int32_t in1)
{
    int32_t result;
    int64_t in0_64 = (int64_t) in0;
    bool overflow = (in0 == in1) && (in0 == (int32_t) INT32_MIN);

    /* Nudge value */
    int64_t nudge_val = 1 << 30;
    if ((in0 < 0) ^ (in1 < 0)) {
        nudge_val = 1 - nudge_val;
    }

    /* Multiply and add nudge */
    int64_t mult = in0_64 * in1 + nudge_val;

    /* Round and pickup 32 bits */
    result = esp_nn_pick_sat_high32_of64(mult);

    return overflow ? INT32_MAX : result;
}

/**
 * fast version
 * this will fail for values closer to INT32_MAX and INT32_MIN by `1 << (exponent - 1)`.
 * We can afford to do this because we are at the very last stage of filter.
 * Also it is pretty rare condition as our output is going to be 8 bit.
 */
__NN_FORCE_INLINE__ int32_t esp_nn_div_by_power_of_two_fast(int32_t val, int32_t exponent)
{
    int32_t to_add = (1 << (exponent - 1)) - (val < 0);
    return (int32_t) ((val + to_add) >> exponent);
}

__NN_FORCE_INLINE__ int32_t esp_nn_div_by_power_of_two(int32_t val, int32_t exponent)
{
    int32_t result;

    const int32_t mask = (1 << exponent) - 1;
    const int32_t remainder = val & mask;

    result = val >> exponent;
    int32_t threshold = (mask >> 1) + (result < 0);

    if (remainder > threshold) {
        result += 1;
    }
    return result;
}

__NN_FORCE_INLINE__ int32_t esp_nn_multiply_by_quantized_mult(int32_t x, int32_t mult, int32_t shift)
{
    int32_t left_shift = shift > 0 ? shift : 0;
    int32_t right_shift = shift > 0 ? 0 : -shift;
    int32_t result = esp_nn_sat_round_doubling_high_mul(x * (1 << left_shift), mult);
    return esp_nn_div_by_power_of_two(result, right_shift);
}

__NN_FORCE_INLINE__ int32_t esp_nn_multiply_by_quantized_mult_fast(int32_t x, int32_t mult, int32_t shift)
{
    int32_t left_shift = max(shift, 0);
    int32_t right_shift = left_shift - shift;

    int64_t nudge_val = 1 << 30;
    int64_t in0_64 = (int64_t) (x << left_shift);

    /* Multiply and add nudge */
    int64_t mult_64 = in0_64 * mult + nudge_val;
    int32_t result = (int32_t) (mult_64 >> 31);
    if (right_shift) {
        result = esp_nn_div_by_power_of_two_fast(result, right_shift);
    }
    return result;
}

static void esp_nn_aligned_s8_pad_with_value(const int8_t *src, int8_t *dst,
                                             const uint16_t input_wd,
                                             const uint16_t input_ht,
                                             const uint16_t channels,
                                             const int32_t pad_val,
                                             const uint16_t pad_wd,
                                             const uint16_t pad_ht)
{
    /* memset with pad_val */
    memset(dst, pad_val, ((input_wd + 2 * pad_wd) * (input_ht + 2 * pad_ht)) * channels);
    dst += (pad_wd + input_wd + pad_wd) * channels;

    for (int i = 0; i < input_ht; i++) {
        dst += pad_wd * channels;
        for (int j = 0; j < input_wd * channels; j++) {
            *dst++ = *src++;
        }
        dst += pad_wd * channels;
    }
}

static void esp_nn_aligned_s8_pad_end_with_value(const int8_t *src, int8_t *dst,
                                                 const uint16_t input_wd,
                                                 const uint16_t input_ht,
                                                 const uint16_t channels,
                                                 const int32_t pad_val,
                                                 const uint16_t pad_wd,
                                                 const uint16_t pad_ht)
{
    for (int i = 0; i < input_ht; i++) {
        for (int j = 0; j < input_wd * channels; j++) {
            *dst++ = *src++;
        }
        if (pad_wd) {
            memset(dst, pad_val, pad_wd * channels);
            dst += pad_wd * channels;
        }
    }
    /* pad end `pad_ht` lines at end */
    if (pad_ht) {
        memset(dst, pad_val, (input_wd + pad_wd) * pad_ht * channels);
    }
}

/**
 * @brief       convert 8 bit input data to 16 bit
 *
 * @param       src int8_t source data
 * @param       dst int16_t dst data
 * @param       size length of data
 * @param       offset  offset to be added to src data. Range: [-128, 127]
 */
__NN_FORCE_INLINE__ void esp_nn_s8_to_s16_with_offset(const int8_t *src, int16_t *dst,
                                                      const int size, const int32_t offset)
{
    int i = 0;
    for (; i < size; i += 2) {
        dst[i + 0] = src[i + 0] + offset;
        dst[i + 1] = src[i + 1] + offset;
    }
    if(i < size) {
        dst[i] = src[i] + offset;
    }
}

/**
 * @brief       convert 8 bit input data to 16 bit
 *
 * @param       src int8_t source data
 * @param       dst int16_t dst data
 * @param       size length of data
 */
__NN_FORCE_INLINE__ void esp_nn_s8_to_s16(const int8_t *src, int16_t *dst, const int size)
{
    int i = 0;
    for (; i < size; i += 2) {
        dst[i + 0] = src[i + 0];
        dst[i + 1] = src[i + 1];
    }
    if(i < size) {
        dst[i] = src[i];
    }
}
