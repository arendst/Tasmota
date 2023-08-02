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

int esp_nn_get_conv_scratch_size_opt(const data_dims_t *input_dims,
                                     const data_dims_t *filter_dims,
                                     const data_dims_t *output_dims,
                                     const conv_params_t *conv_params)
{
    return 0;
}

void esp_nn_set_conv_scratch_buf_opt(const void *buf)
{

}

__attribute__ ((noinline))
static void esp_nn_conv_s8_1x1(const data_dims_t *input_dims,
                               const int8_t *input_data,
                               const int8_t *filter_data,
                               const int32_t *bias,
                               const data_dims_t *output_dims,
                               int8_t *out_data,
                               const conv_params_t *conv_params,
                               const quant_data_t *quant_data)
{
    const uint16_t input_wd = input_dims->width;
    const uint16_t in_channels = input_dims->channels;
    const int32_t input_offset = conv_params->in_offset;
    const int32_t out_offset = conv_params->out_offset;
    const uint16_t stride_wd = conv_params->stride.width;
    const uint16_t stride_ht = conv_params->stride.height;
    const uint16_t out_wd = output_dims->width;
    const uint16_t out_ht = output_dims->height;
    const uint16_t out_channels = output_dims->channels;
    const int32_t activation_min = conv_params->activation.min;
    const int32_t activation_max = conv_params->activation.max;

    for (int32_t in_row = 0; in_row < out_ht * stride_ht; in_row += stride_ht) {
        for (int32_t in_col = 0; in_col < out_wd * stride_wd; in_col += stride_wd) {
            const int32_t *out_mult = quant_data->mult;
            const int32_t *out_shift = quant_data->shift;
            const int8_t *filter_ptr = filter_data;
            const int8_t *input_base_ptr = input_data + (in_row * input_wd + in_col) * in_channels;
            int32_t out_ch_idx = 0;
            for (; out_ch_idx < out_channels; out_ch_idx++) {
                int32_t conv_out = 0;

                const int8_t *input_ptr = input_base_ptr;

                int32_t in_ch_idx = 0;
                for (; in_ch_idx < in_channels - 3; in_ch_idx += 4) {
                    conv_out += (*input_ptr++ + input_offset) * *filter_ptr++;
                    conv_out += (*input_ptr++ + input_offset) * *filter_ptr++;
                    conv_out += (*input_ptr++ + input_offset) * *filter_ptr++;
                    conv_out += (*input_ptr++ + input_offset) * *filter_ptr++;
                }
                for (; in_ch_idx < in_channels; in_ch_idx ++) {
                    conv_out += (*input_ptr++ + input_offset) * *filter_ptr++;
                }
                if (bias) {
                    conv_out += bias[out_ch_idx];
                }
                conv_out = esp_nn_multiply_by_quantized_mult_fast(conv_out, *out_mult++, *out_shift++);
                conv_out += out_offset;
                conv_out = max(conv_out, activation_min);
                conv_out = min(conv_out, activation_max);
                *out_data++ = (int8_t) conv_out;
            }
        }
    }
}

/**
 * Assumption 1: i/p channels == o/p channels
 * Assumption 2: Pointers are valid
 * Assumption 3: dialation width = 1
 */
void esp_nn_conv_s8_opt(const data_dims_t *input_dims,
                        const int8_t *input_data,
                        const data_dims_t *filter_dims,
                        const int8_t *filter_data,
                        const int32_t *bias,
                        const data_dims_t *output_dims,
                        int8_t *out_data,
                        const conv_params_t *conv_params,
                        const quant_data_t *quant_data)
{
    const uint16_t filter_wd = filter_dims->width;
    const uint16_t filter_ht = filter_dims->height;

    if (filter_wd == 1 && filter_ht == 1) {
        esp_nn_conv_s8_1x1(input_dims, input_data, filter_data, bias,
                           output_dims, out_data, conv_params, quant_data);
        return;
    }

    const uint16_t input_wd = input_dims->width;
    const uint16_t input_ht = input_dims->height;
    const uint16_t in_channels = input_dims->channels;
    const int32_t input_offset = conv_params->in_offset;
    const int32_t out_offset = conv_params->out_offset;
    const uint16_t pad_wd = conv_params->padding.width;
    const uint16_t pad_ht = conv_params->padding.height;
    const uint16_t stride_wd = conv_params->stride.width;
    const uint16_t stride_ht = conv_params->stride.height;
    const uint16_t out_wd = output_dims->width;
    const uint16_t out_ht = output_dims->height;
    const uint16_t out_channels = output_dims->channels;
    const int32_t activation_min = conv_params->activation.min;
    const int32_t activation_max = conv_params->activation.max;

    int32_t out_ch_idx, out_y, out_x, filter_y_idx, filter_x_idx;

    for (out_y = 0; out_y < out_ht; out_y++) {
        for (out_x = 0; out_x < out_wd; out_x++) {
            const int32_t *out_shift = quant_data->shift;
            const int32_t *out_mult = quant_data->mult;
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

                        const int8_t *input_ptr = input_data +
                                        (in_row * input_wd + in_col) * in_channels;
                        const int8_t *filter_ptr = filter_data +
                                        out_ch_idx * in_channels * filter_ht * filter_wd +
                                        (filter_y_idx * filter_wd + filter_x_idx) * in_channels;
                        int32_t in_ch_idx = 0;
                        for (; in_ch_idx < in_channels - 3; in_ch_idx += 4) {
                            conv_out += (*input_ptr++ + input_offset) * *filter_ptr++;
                            conv_out += (*input_ptr++ + input_offset) * *filter_ptr++;
                            conv_out += (*input_ptr++ + input_offset) * *filter_ptr++;
                            conv_out += (*input_ptr++ + input_offset) * *filter_ptr++;
                        }
                        for (; in_ch_idx < in_channels; in_ch_idx ++) {
                            conv_out += (*input_ptr++ + input_offset) * *filter_ptr++;
                        }
                    }
                }
                if (bias) {
                    conv_out += bias[out_ch_idx];
                }
                conv_out = esp_nn_multiply_by_quantized_mult_fast(conv_out, *out_mult++, *out_shift++);
                conv_out += out_offset;
                conv_out = max(conv_out, activation_min);
                conv_out = min(conv_out, activation_max);
                *out_data++ = (int8_t) conv_out;
            }
        }
    }
}
