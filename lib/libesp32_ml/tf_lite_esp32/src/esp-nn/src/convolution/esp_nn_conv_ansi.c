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

#include "../../include/esp_nn_defs.h"

#include "../common/common_functions.h"

int esp_nn_get_conv_scratch_size_ansi(const data_dims_t *input_dims,
                                      const data_dims_t *filter_dims,
                                      const data_dims_t *output_dims,
                                      const conv_params_t *conv_params)
{
    return 0;
}

void esp_nn_set_conv_scratch_buf_ansi(const void *buf)
{

}

/**
 * Assumption 1: i/p channels == o/p channels
 * Assumption 2: Pointers are valid
 * Assumption 3: dialation width = 1
 */
void esp_nn_conv_u8_ansi(const uint8_t *input_data,
                         const uint16_t input_wd,
                         const uint16_t input_ht,
                         const uint16_t in_channels,
                         const int32_t input_offset,
                         const uint16_t pad_wd,
                         const uint16_t pad_ht,
                         const uint16_t stride_wd,
                         const uint16_t stride_ht,
                         const uint8_t *filter_data,
                         const uint16_t filter_wd,
                         const uint16_t filter_ht,
                         const int32_t filter_offset,
                         const int32_t *bias,
                         uint8_t *out_data,
                         const uint16_t out_wd,
                         const uint16_t out_ht,
                         const uint16_t out_channels,
                         const int32_t out_offset,
                         const int32_t out_shift,
                         const int32_t out_mult,
                         const int32_t activation_min,
                         const int32_t activation_max)
{
    for (int out_y = 0; out_y < out_ht; out_y++) { //height loop
        const int16_t base_y = (out_y * stride_ht) - pad_ht;
        for (int out_x = 0; out_x < out_wd; out_x++) { //width_loop
            const int16_t base_x = (out_x * stride_wd) - pad_wd;
            for (int out_ch_idx = 0; out_ch_idx < out_channels; out_ch_idx++) {//channel_loop
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
                        for (int in_ch_idx = 0; in_ch_idx < in_channels; in_ch_idx++) {
                            int32_t input_index = (idx_y * input_wd + idx_x) * in_channels + in_ch_idx;
                            int32_t filter_index = ((out_ch_idx * filter_ht + filter_y_idx)
                                                    * filter_wd + filter_x_idx) * in_channels
                                                   + in_ch_idx;
                            int32_t input_val = input_data[input_index] + input_offset;
                            int32_t filter_val = filter_data[filter_index] + filter_offset;
                            result += input_val * filter_val;
                        }
                    }
                }
                if (bias) {
                    result += bias[out_ch_idx];
                }
                result = esp_nn_multiply_by_quantized_mult(result, out_mult, out_shift);
                result += out_offset;
                result = max(result, activation_min);
                result = min(result, activation_max);

                int out_index = (out_y * out_wd + out_x) * out_channels + out_ch_idx;
                out_data[out_index] = (uint8_t) result;
            }
        }
    }
}

/**
 * Assumption 1: i/p channels == o/p channels
 * Assumption 2: Pointers are valid
 * Assumption 3: dialation width = 1
 */
void esp_nn_conv_s8_ansi(const data_dims_t *input_dims,
                         const int8_t *input_data,
                         const data_dims_t *filter_dims,
                         const int8_t *filter_data,
                         const int32_t *bias,
                         const data_dims_t *output_dims,
                         int8_t *out_data,
                         const conv_params_t *conv_params,
                         const quant_data_t *quant_data)
{
    const uint16_t input_wd = input_dims->width;
    const uint16_t input_ht = input_dims->height;
    const uint16_t in_channels = input_dims->channels;
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
    const uint16_t out_channels = output_dims->channels;
    const int32_t *out_shift = quant_data->shift;
    const int32_t *out_mult = quant_data->mult;
    const int32_t activation_min = conv_params->activation.min;
    const int32_t activation_max = conv_params->activation.max;

    int32_t out_ch_idx, out_y, out_x, in_ch_idx, filter_y_idx, filter_x_idx;

    for (out_y = 0; out_y < out_ht; out_y++) {
        for (out_x = 0; out_x < out_wd; out_x++) {
            for (out_ch_idx = 0; out_ch_idx < out_channels; out_ch_idx++) {
                int32_t conv_out = 0;

                const int32_t base_y = stride_ht * out_y - pad_ht;
                const int32_t base_x = stride_wd * out_x - pad_wd;

                const int32_t filter_y_start = max(0, -base_y);
                const int32_t filter_x_start = max(0, -base_x);

                const int32_t filter_y_end = min(filter_ht, input_ht - base_y);
                const int32_t filter_x_end = min(filter_wd, input_wd - base_x);

                for (filter_y_idx = filter_y_start; filter_y_idx < filter_y_end; filter_y_idx++) {
                    for (filter_x_idx = filter_x_start; filter_x_idx < filter_x_end; filter_x_idx++) {
                        const int32_t in_row = base_y + filter_y_idx;
                        const int32_t in_col = base_x + filter_x_idx;
                        int32_t input_base_offset = (in_row * input_wd + in_col) * in_channels;
                        int32_t filter_base_offset = out_ch_idx * in_channels * filter_ht * filter_wd +
                                                       (filter_y_idx * filter_wd + filter_x_idx) * in_channels;
                        for (in_ch_idx = 0; in_ch_idx < in_channels; in_ch_idx++) {
                            conv_out +=
                                (input_data[input_base_offset + in_ch_idx] + input_offset) *
                                filter_data[filter_base_offset + in_ch_idx];
                        }
                    }
                }
                if (bias) {
                    conv_out += bias[out_ch_idx];
                }
                conv_out = esp_nn_multiply_by_quantized_mult(conv_out, out_mult[out_ch_idx], out_shift[out_ch_idx]);
                conv_out += out_offset;
                conv_out = max(conv_out, activation_min);
                conv_out = min(conv_out, activation_max);
                *out_data++ = (int8_t) conv_out;
            }
        }
    }
}
