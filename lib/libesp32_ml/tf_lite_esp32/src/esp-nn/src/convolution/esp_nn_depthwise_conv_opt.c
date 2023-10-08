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

int esp_nn_get_depthwise_conv_scratch_size_opt(const data_dims_t *input_dims,
                                               const data_dims_t *filter_dims,
                                               const data_dims_t *output_dims,
                                               const dw_conv_params_t *conv_params)
{
    return 0;
}

void esp_nn_set_depthwise_conv_scratch_buf_opt(const void *buf)
{

}

/* common channel multiplier == 1 case */
__attribute__ ((noinline))
static void esp_nn_depthwise_conv_s8_ch_mult_1(const data_dims_t *input_dims,
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
    const int32_t activation_min = conv_params->activation.min;
    const int32_t activation_max = conv_params->activation.max;

    int out_idx = 0;
    for (int out_y = 0; out_y < out_ht; out_y++) { //height loop
        const int16_t base_y = (out_y * stride_ht) - pad_ht;
        for (int out_x = 0; out_x < out_wd; out_x++) { //width_loop
            const int16_t base_x = (out_x * stride_wd) - pad_wd;

            const int32_t *out_shift = quant_data->shift;
            const int32_t *out_mult = quant_data->mult;

            /* Select filter so as the point doesn't lie outside block */
            int filter_y_start = max(0, -base_y);
            int filter_x_start = max(0, -base_x);
            int filter_y_end = min(filter_ht, input_ht - base_y);
            int filter_x_end = min(filter_wd, input_wd - base_x);

            int ch_idx = 0;
            for (; ch_idx < channels - 3; ch_idx += 4) {//channel_loop
                int32_t result0 = 0;
                int32_t result1 = 0;
                int32_t result2 = 0;
                int32_t result3 = 0;

                for (int filter_y_idx = filter_y_start; filter_y_idx < filter_y_end; filter_y_idx++) {
                    const int32_t idx_y = base_y + filter_y_idx;
                    for (int filter_x_idx = filter_x_start; filter_x_idx < filter_x_end; filter_x_idx++) {
                        const int32_t idx_x = base_x + filter_x_idx;
                        int32_t input_index = (idx_y * input_wd + idx_x) * channels + ch_idx;
                        int32_t filter_index = (filter_y_idx * filter_wd + filter_x_idx) * (channels) + ch_idx;
                        int32_t input_val0 = input_data[input_index + 0] + input_offset;
                        int32_t input_val1 = input_data[input_index + 1] + input_offset;
                        int32_t input_val2 = input_data[input_index + 2] + input_offset;
                        int32_t input_val3 = input_data[input_index + 3] + input_offset;
                        int32_t filter_val0 = filter_data[filter_index + 0];
                        int32_t filter_val1 = filter_data[filter_index + 1];
                        int32_t filter_val2 = filter_data[filter_index + 2];
                        int32_t filter_val3 = filter_data[filter_index + 3];
                        result0 += input_val0 * filter_val0;
                        result1 += input_val1 * filter_val1;
                        result2 += input_val2 * filter_val2;
                        result3 += input_val3 * filter_val3;
                    }
                }
                if (bias) {
                    result0 += bias[ch_idx + 0];
                    result1 += bias[ch_idx + 1];
                    result2 += bias[ch_idx + 2];
                    result3 += bias[ch_idx + 3];
                }
                result0 = esp_nn_multiply_by_quantized_mult_fast(result0, *out_mult++, *out_shift++);
                result1 = esp_nn_multiply_by_quantized_mult_fast(result1, *out_mult++, *out_shift++);
                result2 = esp_nn_multiply_by_quantized_mult_fast(result2, *out_mult++, *out_shift++);
                result3 = esp_nn_multiply_by_quantized_mult_fast(result3, *out_mult++, *out_shift++);

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
            for (; ch_idx < channels; ch_idx++) {//channel_loop
                int32_t result = 0;

                for (int filter_y_idx = filter_y_start; filter_y_idx < filter_y_end; filter_y_idx++) {
                    const int32_t idx_y = base_y + filter_y_idx;
                    for (int filter_x_idx = filter_x_start; filter_x_idx < filter_x_end; filter_x_idx++) {
                        const int32_t idx_x = base_x + filter_x_idx;
                        int32_t input_index = (idx_y * input_wd + idx_x) * channels + ch_idx;
                        int32_t filter_index = (filter_y_idx * filter_wd + filter_x_idx) * (channels) + ch_idx;
                        int32_t input_val = input_data[input_index] + input_offset;
                        int32_t filter_val = filter_data[filter_index];
                        result += input_val * filter_val;
                    }
                }
                if (bias) {
                    result += bias[ch_idx];
                }
                result = esp_nn_multiply_by_quantized_mult_fast(result, *out_mult++, *out_shift++);
                result += out_offset;
                result = max(result, activation_min);
                result = min(result, activation_max);

                out_data[out_idx++] = result;
            }
        }
    }
}

void esp_nn_depthwise_conv_s8_opt(const data_dims_t *input_dims,
                                  const int8_t *input_data,
                                  const data_dims_t *filter_dims,
                                  const int8_t *filter_data,
                                  const int32_t *bias,
                                  const data_dims_t *output_dims,
                                  int8_t *out_data,
                                  const dw_conv_params_t *conv_params,
                                  const quant_data_t *quant_data)
{
    const uint16_t ch_mult = conv_params->ch_mult;
    if (ch_mult == 1) {
        esp_nn_depthwise_conv_s8_ch_mult_1(input_dims, input_data, filter_dims, filter_data,
                                           bias, output_dims, out_data, conv_params, quant_data);
        return;
    }
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
    const int32_t activation_min = conv_params->activation.min;
    const int32_t activation_max = conv_params->activation.max;

    int out_idx = 0;
    for (int out_y = 0; out_y < out_ht; out_y++) { //height loop
        const int16_t base_y = (out_y * stride_ht) - pad_ht;
        for (int out_x = 0; out_x < out_wd; out_x++) { //width_loop
            const int16_t base_x = (out_x * stride_wd) - pad_wd;

            const int32_t *out_shift = quant_data->shift;
            const int32_t *out_mult = quant_data->mult;

            /* Select filter so as the point doesn't lie outside block */
            int filter_y_start = max(0, -base_y);
            int filter_x_start = max(0, -base_x);
            int filter_y_end = min(filter_ht, input_ht - base_y);
            int filter_x_end = min(filter_wd, input_wd - base_x);

            for (int ch_idx = 0; ch_idx < channels; ch_idx++) {//channel_loop
                int ch_mult_idx = 0;
                for (; ch_mult_idx < ch_mult - 3; ch_mult_idx += 4) {
                    int32_t result0 = 0;
                    int32_t result1 = 0;
                    int32_t result2 = 0;
                    int32_t result3 = 0;
                    const int out_ch_idx =  ch_idx * ch_mult + ch_mult_idx;

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
                    result0 = esp_nn_multiply_by_quantized_mult_fast(result0, *out_mult++, *out_shift++);
                    result1 = esp_nn_multiply_by_quantized_mult_fast(result1, *out_mult++, *out_shift++);
                    result2 = esp_nn_multiply_by_quantized_mult_fast(result2, *out_mult++, *out_shift++);
                    result3 = esp_nn_multiply_by_quantized_mult_fast(result3, *out_mult++, *out_shift++);

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
                for (; ch_mult_idx < ch_mult; ch_mult_idx++) {
                    int32_t result = 0;
                    const int out_ch_idx =  ch_idx * ch_mult + ch_mult_idx;

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
                    result = esp_nn_multiply_by_quantized_mult_fast(result, *out_mult++, *out_shift++);
                    result += out_offset;
                    result = max(result, activation_min);
                    result = min(result, activation_max);

                    out_data[out_idx++] = result;
                }
            }
        }
    }
}
