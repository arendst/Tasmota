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

#include <stdio.h>
#include "../../include/esp_nn_defs.h"

#include "../common/common_functions.h"

static int16_t *scratch_buffer = NULL;

extern void esp_nn_depthwise_conv_s16_mult8_3x3_esp32s3(const int16_t *input_data,
                                                        const uint16_t input_wd,
                                                        const uint16_t input_ht,
                                                        const uint16_t channels,
                                                        const uint16_t pad_wd,
                                                        const uint16_t pad_ht,
                                                        const uint16_t stride_wd,
                                                        const uint16_t stride_ht,
                                                        const uint16_t ch_mult,
                                                        const int16_t *filter_data,
                                                        const int32_t *bias,
                                                        int8_t *out_data,
                                                        const uint16_t out_wd,
                                                        const uint16_t out_ht,
                                                        const int32_t out_offset,
                                                        const int32_t *out_shift,
                                                        const int32_t *out_mult,
                                                        const int32_t activation_min,
                                                        const int32_t activation_max);

extern void esp_nn_depthwise_conv_s8_mult1_3x3_padded_esp32s3(const int8_t *input_data,
                                                              const uint16_t input_wd,
                                                              const uint16_t input_ht,
                                                              const uint16_t channels,
                                                              const int32_t input_offset,
                                                              const uint16_t stride_wd,
                                                              const uint16_t stride_ht,
                                                              const int8_t *filter_data,
                                                              const int32_t *bias,
                                                              int8_t *out_data,
                                                              const uint16_t out_wd,
                                                              const uint16_t out_ht,
                                                              const int32_t out_offset,
                                                              const int32_t *out_shift,
                                                              const int32_t *out_mult,
                                                              const int32_t activation_min,
                                                              const int32_t activation_max);

extern void esp_nn_depthwise_conv_s16_mult1_3x3_no_pad_esp32s3(const int16_t *input_data,
                                                               const uint16_t input_wd,
                                                               const uint16_t input_ht,
                                                               const uint16_t channels,
                                                               const uint16_t stride_wd,
                                                               const uint16_t stride_ht,
                                                               const int16_t *filter_data,
                                                               const int32_t *bias,
                                                               int8_t *out_data,
                                                               const uint16_t out_wd,
                                                               const uint16_t out_ht,
                                                               const int32_t out_offset,
                                                               const int32_t *out_shift,
                                                               const int32_t *out_mult,
                                                               const int32_t activation_min,
                                                               const int32_t activation_max);

extern void esp_nn_depthwise_conv_s16_mult8_esp32s3(const int16_t *input_data,
                                                    const uint16_t input_wd,
                                                    const uint16_t input_ht,
                                                    const uint16_t channels,
                                                    const uint16_t pad_wd,
                                                    const uint16_t pad_ht,
                                                    const uint16_t stride_wd,
                                                    const uint16_t stride_ht,
                                                    const uint16_t ch_mult,
                                                    const int16_t *filter_data,
                                                    const uint16_t filter_wd,
                                                    const uint16_t filter_ht,
                                                    const int32_t *bias,
                                                    int8_t *out_data,
                                                    const uint16_t out_wd,
                                                    const uint16_t out_ht,
                                                    const int32_t out_offset,
                                                    const int32_t *out_shift,
                                                    const int32_t *out_mult,
                                                    const int32_t activation_min,
                                                    const int32_t activation_max);

extern void esp_nn_depthwise_conv_s16_mult4_esp32s3(const int16_t *input_data,
                                                    const uint16_t input_wd,
                                                    const uint16_t input_ht,
                                                    const uint16_t channels,
                                                    const uint16_t pad_wd,
                                                    const uint16_t pad_ht,
                                                    const uint16_t stride_wd,
                                                    const uint16_t stride_ht,
                                                    const uint16_t ch_mult,
                                                    const int16_t *filter_data,
                                                    const uint16_t filter_wd,
                                                    const uint16_t filter_ht,
                                                    const int32_t *bias,
                                                    int8_t *out_data,
                                                    const uint16_t out_wd,
                                                    const uint16_t out_ht,
                                                    const int32_t out_offset,
                                                    const int32_t *out_shift,
                                                    const int32_t *out_mult,
                                                    const int32_t activation_min,
                                                    const int32_t activation_max);

extern void esp_nn_depthwise_conv_s16_mult1_3x3_esp32s3(const int16_t *input_data,
                                                        const uint16_t input_wd,
                                                        const uint16_t input_ht,
                                                        const uint16_t channels,
                                                        const uint16_t pad_wd,
                                                        const uint16_t pad_ht,
                                                        const uint16_t stride_wd,
                                                        const uint16_t stride_ht,
                                                        const int16_t *filter_data,
                                                        const int32_t *bias,
                                                        int8_t *out_data,
                                                        const uint16_t out_wd,
                                                        const uint16_t out_ht,
                                                        const int32_t out_offset,
                                                        const int32_t *out_shift,
                                                        const int32_t *out_mult,
                                                        const int32_t activation_min,
                                                        const int32_t activation_max);

extern void esp_nn_depthwise_conv_s16_mult1_esp32s3(const int16_t *input_data,
                                                    const uint16_t input_wd,
                                                    const uint16_t input_ht,
                                                    const uint16_t channels,
                                                    const uint16_t pad_wd,
                                                    const uint16_t pad_ht,
                                                    const uint16_t stride_wd,
                                                    const uint16_t stride_ht,
                                                    const int16_t *filter_data,
                                                    const uint16_t filter_wd,
                                                    const uint16_t filter_ht,
                                                    const int32_t *bias,
                                                    int8_t *out_data,
                                                    const uint16_t out_wd,
                                                    const uint16_t out_ht,
                                                    const int32_t out_offset,
                                                    const int32_t *out_shift,
                                                    const int32_t *out_mult,
                                                    const int32_t activation_min,
                                                    const int32_t activation_max);

extern void esp_nn_s8_to_s16_esp32s3(const int8_t *src, int16_t *dst, const int size);

extern void esp_nn_aligned_s8_to_s16_with_offset_esp32s3(const int8_t *src, int16_t *dst,
                                                         const int size, const int32_t offset);

static void esp_nn_depthwise_conv_s8_unrolled(const int8_t *input_data,
                                              const uint16_t input_wd,
                                              const uint16_t input_ht,
                                              const uint16_t channels,
                                              const int32_t input_offset,
                                              const uint16_t pad_wd,
                                              const uint16_t pad_ht,
                                              const uint16_t stride_wd,
                                              const uint16_t stride_ht,
                                              const uint16_t ch_mult,
                                              const int8_t *filter_data,
                                              const uint16_t filter_wd,
                                              const uint16_t filter_ht,
                                              const int32_t *bias,
                                              int8_t *out_data,
                                              const uint16_t out_wd,
                                              const uint16_t out_ht,
                                              const int32_t out_offset,
                                              const int32_t *out_shift,
                                              const int32_t *out_mult,
                                              const int32_t activation_min,
                                              const int32_t activation_max)
{
    int out_idx = 0;
    for (int out_y = 0; out_y < out_ht; out_y++) { //height loop
        const int16_t base_y = (out_y * stride_ht) - pad_ht;
        for (int out_x = 0; out_x < out_wd; out_x++) { //width_loop
            const int16_t base_x = (out_x * stride_wd) - pad_wd;
            for (int ch_idx = 0; ch_idx < channels; ch_idx++) {//channel_loop
                int ch_mult_idx = 0;
                for (; ch_mult_idx < ch_mult - 3; ch_mult_idx += 4) {
                    int32_t result0 = 0, result1 = 0, result2 = 0, result3 = 0;
                    const int out_ch_idx = ch_mult_idx + ch_idx * ch_mult;

                    /* Select filter so as the point doesn't lie outside block */
                    int filter_y_start = max(0, -base_y);
                    int filter_x_start = max(0, -base_x);
                    int filter_y_end = min(filter_ht, input_ht - base_y);
                    int filter_x_end = min(filter_wd, input_wd - base_x);

                    for (int filter_y_idx = filter_y_start; filter_y_idx < filter_y_end; filter_y_idx++) {
                        const int32_t idx_y = base_y + filter_y_idx;
                        for (int filter_x_idx = filter_x_start; filter_x_idx < filter_x_end; filter_x_idx++) {
                            const int32_t idx_x = base_x + filter_x_idx;
                            int32_t input_index = (idx_y * input_wd + idx_x) * channels + ch_idx;
                            int32_t filter_index = (filter_y_idx * filter_wd + filter_x_idx) * (channels * ch_mult) + out_ch_idx;
                            int32_t input_val = input_data[input_index] + input_offset;
                            int32_t filter_val0 = filter_data[filter_index + 0];
                            int32_t filter_val1 = filter_data[filter_index + 1];
                            int32_t filter_val2 = filter_data[filter_index + 2];
                            int32_t filter_val3 = filter_data[filter_index + 3];
                            result0 += input_val * filter_val0;
                            result1 += input_val * filter_val1;
                            result2 += input_val * filter_val2;
                            result3 += input_val * filter_val3;
                        }
                    }
                    if (bias) {
                        result0 += bias[out_ch_idx + 0];
                        result1 += bias[out_ch_idx + 1];
                        result2 += bias[out_ch_idx + 2];
                        result3 += bias[out_ch_idx + 3];
                    }
                    result0 = esp_nn_multiply_by_quantized_mult(result0,
                                out_mult[out_ch_idx + 0], out_shift[out_ch_idx + 0]);
                    result1 = esp_nn_multiply_by_quantized_mult(result1,
                                out_mult[out_ch_idx + 1], out_shift[out_ch_idx + 1]);
                    result2 = esp_nn_multiply_by_quantized_mult(result2,
                                out_mult[out_ch_idx + 2], out_shift[out_ch_idx + 2]);
                    result3 = esp_nn_multiply_by_quantized_mult(result3,
                                out_mult[out_ch_idx + 3], out_shift[out_ch_idx + 3]);

                    result0 += out_offset;
                    result1 += out_offset;
                    result2 += out_offset;
                    result3 += out_offset;

                    result0 = max(result0, activation_min);
                    result1 = max(result1, activation_min);
                    result2 = max(result2, activation_min);
                    result3 = max(result3, activation_min);

                    result0 = min(result0, activation_max);
                    result1 = min(result1, activation_max);
                    result2 = min(result2, activation_max);
                    result3 = min(result3, activation_max);

                    out_data[out_idx++] = result0;
                    out_data[out_idx++] = result1;
                    out_data[out_idx++] = result2;
                    out_data[out_idx++] = result3;
                }

                /* left-over */
                for (; ch_mult_idx < ch_mult; ch_mult_idx++) {
                    int32_t result = 0;
                    const int out_ch_idx = ch_mult_idx + ch_idx * ch_mult;

                    /* Select filter so as the point doesn't lie outside block */
                    int filter_y_start = max(0, -base_y);
                    int filter_x_start = max(0, -base_x);
                    int filter_y_end = min(filter_ht, input_ht - base_y);
                    int filter_x_end = min(filter_wd, input_wd - base_x);

                    for (int filter_y_idx = filter_y_start; filter_y_idx < filter_y_end; filter_y_idx++) {
                        const int32_t idx_y = base_y + filter_y_idx;
                        for (int filter_x_idx = filter_x_start; filter_x_idx < filter_x_end; filter_x_idx++) {
                            const int32_t idx_x = base_x + filter_x_idx;
                            int32_t input_index = (idx_y * input_wd + idx_x) * channels + ch_idx;
                            int32_t filter_index = (filter_y_idx * filter_wd + filter_x_idx) * (channels * ch_mult) + out_ch_idx;
                            int32_t input_val = input_data[input_index] + input_offset;
                            int32_t filter_val = filter_data[filter_index];
                            result += input_val * filter_val;
                        }
                    }
                    if (bias) {
                        result += bias[out_ch_idx];
                    }
                    result = esp_nn_multiply_by_quantized_mult(result, out_mult[out_ch_idx], out_shift[out_ch_idx]);
                    result += out_offset;
                    result = max(result, activation_min);
                    result = min(result, activation_max);

                    out_data[out_idx++] = result;
                }
            }
        }
    }
}

void esp_nn_depthwise_conv_s8_ch_mult1(const int8_t *input_data,
                                       const uint16_t input_wd,
                                       const uint16_t input_ht,
                                       const uint16_t channels,
                                       const int32_t input_offset,
                                       const uint16_t pad_wd,
                                       const uint16_t pad_ht,
                                       const uint16_t stride_wd,
                                       const uint16_t stride_ht,
                                       const int8_t *filter_data,
                                       const uint16_t filter_wd,
                                       const uint16_t filter_ht,
                                       const int32_t *bias,
                                       int8_t *out_data,
                                       const uint16_t out_wd,
                                       const uint16_t out_ht,
                                       const int32_t out_offset,
                                       const int32_t *out_shift,
                                       const int32_t *out_mult,
                                       const int32_t activation_min,
                                       const int32_t activation_max)
{
    int out_idx = 0;
    for (int out_y = 0; out_y < out_ht; out_y++) { //height loop
        const int16_t base_y = (out_y * stride_ht) - pad_ht;
        for (int out_x = 0; out_x < out_wd; out_x++) { //width_loop
            const int16_t base_x = (out_x * stride_wd) - pad_wd;
            for (int ch_idx = 0; ch_idx < channels; ch_idx++) {//channel_loop
                int32_t result = 0;
                /* Select filter so as the point doesn't lie outside block */
                int filter_y_start = max(0, -base_y);
                int filter_x_start = max(0, -base_x);
                int filter_y_end = min(filter_ht, input_ht - base_y);
                int filter_x_end = min(filter_wd, input_wd - base_x);

                for (int filter_y_idx = filter_y_start; filter_y_idx < filter_y_end; filter_y_idx++) {
                    const int32_t idx_y = base_y + filter_y_idx;
                    for (int filter_x_idx = filter_x_start; filter_x_idx < filter_x_end; filter_x_idx++) {
                        const int32_t idx_x = base_x + filter_x_idx;
                        int32_t input_index = (idx_y * input_wd + idx_x) * channels + ch_idx;
                        int32_t filter_index = (filter_y_idx * filter_wd + filter_x_idx) * channels + ch_idx;
                        int32_t input_val = input_data[input_index] + input_offset;
                        int32_t filter_val = filter_data[filter_index];
                        result += input_val * filter_val;
                    }
                }
                if (bias) {
                    result += bias[ch_idx];
                }
                result = esp_nn_multiply_by_quantized_mult(result, out_mult[ch_idx], out_shift[ch_idx]);
                result += out_offset;
                result = max(result, activation_min);
                result = min(result, activation_max);

                out_data[out_idx++] = result;
            }
        }
    }
}

int esp_nn_get_depthwise_conv_scratch_size_esp32s3(const data_dims_t *input_dims,
                                                   const data_dims_t *filter_dims,
                                                   const data_dims_t *output_dims,
                                                   const dw_conv_params_t *conv_params)
{
    const uint16_t input_wd = input_dims->width;
    const uint16_t input_ht = input_dims->height;
    const uint16_t channels = input_dims->channels;
    const uint16_t filter_wd = filter_dims->width;
    const uint16_t filter_ht = filter_dims->height;
    const uint16_t ch_mult = conv_params->ch_mult;
    const uint16_t out_wd = output_dims->width;
    const uint16_t out_ht = output_dims->height;
    const uint16_t pad_wd = conv_params->padding.width;
    const uint16_t pad_ht = conv_params->padding.height;
    const uint16_t stride_wd = conv_params->stride.width;
    const uint16_t stride_ht = conv_params->stride.height;

    int filter_size = filter_wd * filter_ht * channels * ch_mult;
    int pad_width = 0, pad_height = 0;

    if ((ch_mult == 1) && (channels % 8 == 0) && (filter_wd == 3) && (filter_ht == 3)) {
        if (channels % 16 == 0) {
            if (pad_wd || pad_ht) {
                pad_width = pad_wd * 2;
                pad_height = pad_ht * 2;
            } else {
                // check if we need to pad additionally
                pad_width = (out_wd * stride_wd + filter_wd - 1) - input_wd;
                pad_height = (out_ht * stride_ht + filter_ht - 1) - input_ht;
                // printf("in(%d %d %d), out(%d %d), filter (%d %d) stride (%d %d), pad (%d %d)",
                //         input_wd, input_ht, channels, out_wd, out_ht, filter_wd, filter_ht,
                //         stride_wd, stride_ht, pad_wd, pad_ht);
            }
            if (pad_width || pad_height) {
                int input_size = (input_wd + pad_width) * (input_ht + pad_height) * channels;
                // printf("ask1 %d\n", filter_size + input_size + 16);
                return filter_size + input_size + 16;  // 16 for alignment
            } else {
                // printf("ask2 %d\n", filter_size + 16);
                return filter_size + 16;  // 16 for alignment
            }
        } else {
            int input_size = input_wd * input_ht * channels;
            // printf("ask3 %d\n", 2 * (filter_size + input_size) + 16);
            return  2 * (filter_size + input_size) + 16; // 16 for alignment
        }
    } else if (ch_mult % 4 == 0) {
        int input_size = input_wd * input_ht * channels;
        // printf("ask4 %d\n", 2 * (filter_size + input_size) + 16);
        return  2 * (filter_size + input_size) + 16; // 16 for alignment
    }
    return 32; // just few bytes
}

void esp_nn_set_depthwise_conv_scratch_buf_esp32s3(void *buf)
{
    scratch_buffer = (int16_t *) buf;
}

/**
 * Assumption 1: i/p channels == o/p channels
 * Assumption 2: Pointers are valid
 * Assumption 3: dialation width = 1
 */



void esp_nn_depthwise_conv_s8_esp32s3(const data_dims_t *input_dims,
                                      const int8_t *input_data,
                                      const data_dims_t *filter_dims,
                                      const int8_t *filter_data,
                                      const int32_t *bias,
                                      const data_dims_t *output_dims,
                                      int8_t *out_data,
                                      const dw_conv_params_t *conv_params,
                                      const quant_data_t *quant_data)
{
    const uint16_t input_wd = input_dims->width;
    const uint16_t input_ht = input_dims->height;
    const uint16_t channels = input_dims->channels;
    const int32_t input_offset = conv_params->in_offset;
    const int32_t out_offset = conv_params->out_offset;
    const uint16_t pad_wd = conv_params->padding.width;
    const uint16_t pad_ht = conv_params->padding.height;
    const uint16_t stride_wd = conv_params->stride.width;
    const uint16_t stride_ht = conv_params->stride.height;
    const uint16_t filter_wd = filter_dims->width;
    const uint16_t filter_ht = filter_dims->height;
    const uint16_t out_wd = output_dims->width;
    const uint16_t out_ht = output_dims->height;
    const int32_t *out_shift = quant_data->shift;
    const int32_t *out_mult = quant_data->mult;
    const int32_t activation_min = conv_params->activation.min;
    const int32_t activation_max = conv_params->activation.max;
    const uint16_t ch_mult = conv_params->ch_mult;

    int filter_size = filter_wd * filter_ht * channels * ch_mult;
    int align_len = 16 - (filter_size & 15);
    int input_size = input_wd * input_ht * channels;
    int16_t *filter_data16 = scratch_buffer;
    int16_t *input_data16 = scratch_buffer + filter_size + align_len;
    if (scratch_buffer == NULL) {
        printf("esp_nn_depthwise_conv error! scratch_buffer not set!\n");
        return;
    }

    if ((ch_mult == 1) && (channels % 8 == 0)) {
        if ((filter_wd == 3) && (filter_ht == 3)) {
            if ((channels % 16 == 0) && (pad_wd == 1) && (pad_ht == 1)) {
                /* process in 8 bits */
                int8_t *filter_aligned = (int8_t *) scratch_buffer;
                int8_t *input_padded = (int8_t *) scratch_buffer + filter_size + align_len;
                memcpy(filter_aligned, filter_data, filter_size);
                esp_nn_aligned_s8_pad_with_value(input_data, input_padded, input_wd, input_ht, channels,
                                                 -input_offset, pad_wd, pad_ht);
                esp_nn_depthwise_conv_s8_mult1_3x3_padded_esp32s3(input_padded, input_wd + 2 * pad_wd,
                                                                  input_ht + 2 * pad_ht, channels, input_offset,
                                                                  stride_wd, stride_ht, filter_aligned, bias,
                                                                  out_data, out_wd, out_ht, out_offset, out_shift,
                                                                  out_mult, activation_min, activation_max);
            } else if ((channels % 16 == 0) && (pad_wd == 0) && (pad_ht == 0)) {
                /* process in 8 bits */
                int8_t *filter_aligned = (int8_t *) scratch_buffer;
                int8_t *input_padded = (int8_t *) scratch_buffer + filter_size + align_len;

                // check if we need to pad additionally
                int pad_right = (out_wd * stride_wd + filter_wd - 1) - input_wd;
                int pad_bottom = (out_ht * stride_ht + filter_ht - 1) - input_ht;
                if (pad_right || pad_bottom) { // pad right and bottom
                    esp_nn_aligned_s8_pad_end_with_value(input_data, input_padded, input_wd, input_ht,
                                                         channels, -input_offset, pad_right, pad_bottom);
                } else {
                    input_padded = (int8_t *) input_data;
                }
                memcpy(filter_aligned, filter_data, filter_size);
                esp_nn_depthwise_conv_s8_mult1_3x3_padded_esp32s3(input_padded, input_wd + pad_right,
                                                                  input_ht + pad_bottom, channels, input_offset,
                                                                  stride_wd, stride_ht, filter_aligned, bias,
                                                                  out_data, out_wd, out_ht, out_offset, out_shift,
                                                                  out_mult, activation_min, activation_max);
            } else { /* (channels % 8) == 0 */
                esp_nn_s8_to_s16_esp32s3(filter_data, filter_data16, filter_size);
                esp_nn_aligned_s8_to_s16_with_offset_esp32s3(input_data, input_data16, input_size, input_offset);
                esp_nn_depthwise_conv_s16_mult1_3x3_esp32s3(input_data16, input_wd, input_ht, channels,
                                                            pad_wd, pad_ht, stride_wd, stride_ht, filter_data16,
                                                            bias, out_data, out_wd, out_ht, out_offset, out_shift,
                                                            out_mult, activation_min, activation_max);
            }
        } else { // all other ch_mult == 1, `channels % 8 == 0`
            esp_nn_depthwise_conv_s8_ch_mult1(input_data, input_wd, input_ht, channels, input_offset,
                                              pad_wd, pad_ht, stride_wd, stride_ht,
                                              filter_data, filter_wd, filter_ht,
                                              bias, out_data, out_wd, out_ht, out_offset, out_shift,
                                              out_mult, activation_min, activation_max);
        }
    } else if (ch_mult % 8 == 0) {
        esp_nn_s8_to_s16_esp32s3(filter_data, filter_data16, filter_size);
        esp_nn_aligned_s8_to_s16_with_offset_esp32s3(input_data, input_data16, input_size, input_offset);
        if (filter_wd == 3 && filter_ht == 3) {
            esp_nn_depthwise_conv_s16_mult8_3x3_esp32s3(input_data16, input_wd, input_ht, channels,
                                                        pad_wd, pad_ht, stride_wd, stride_ht, ch_mult,
                                                        filter_data16, bias,
                                                        out_data, out_wd, out_ht, out_offset, out_shift,
                                                        out_mult, activation_min, activation_max);
        } else {
            esp_nn_depthwise_conv_s16_mult8_esp32s3(input_data16, input_wd, input_ht, channels,
                                                    pad_wd, pad_ht, stride_wd, stride_ht, ch_mult,
                                                    filter_data16, filter_wd, filter_ht, bias,
                                                    out_data, out_wd, out_ht, out_offset, out_shift,
                                                    out_mult, activation_min, activation_max);
        }
    } else if (ch_mult % 4 == 0) {
        esp_nn_s8_to_s16_esp32s3(filter_data, filter_data16, filter_size);
        esp_nn_aligned_s8_to_s16_with_offset_esp32s3(input_data, input_data16, input_size, input_offset);
        esp_nn_depthwise_conv_s16_mult4_esp32s3(input_data16, input_wd, input_ht, channels,
                                                pad_wd, pad_ht, stride_wd, stride_ht, ch_mult,
                                                filter_data16, filter_wd, filter_ht, bias,
                                                out_data, out_wd, out_ht, out_offset, out_shift,
                                                out_mult, activation_min, activation_max);
    } else {
        esp_nn_depthwise_conv_s8_unrolled(input_data, input_wd, input_ht, channels, input_offset,
                                          pad_wd, pad_ht, stride_wd, stride_ht, ch_mult,
                                          filter_data, filter_wd, filter_ht,
                                          bias, out_data, out_wd, out_ht, out_offset, out_shift,
                                          out_mult, activation_min, activation_max);
    }
}
