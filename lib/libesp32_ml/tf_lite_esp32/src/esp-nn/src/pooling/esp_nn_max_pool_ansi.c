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

#include <stdint.h>

#include "../common/common_functions.h"

void esp_nn_max_pool_s8_ansi(const int8_t *input,
                             const uint16_t input_wd,
                             const uint16_t input_ht,
                             int8_t *output,
                             const uint16_t output_wd,
                             const uint16_t output_ht,
                             const uint16_t stride_wd,
                             const uint16_t stride_ht,
                             const uint16_t filter_wd,
                             const uint16_t filter_ht,
                             const uint16_t pad_wd,
                             const uint16_t pad_ht,
                             const int32_t activation_min,
                             const int32_t activation_max,
                             const uint16_t channels)
{
    int32_t base_y = -pad_ht;
    for (int32_t out_y = 0; out_y < output_ht; out_y++, base_y += stride_ht) {
        int32_t base_x = -pad_wd;
        for (int32_t out_x = 0; out_x < output_wd; out_x++, base_x += stride_wd) {
            /* Make sure filter does not cross the input box */
            int32_t filter_y_start = max(0, -base_y);
            int32_t filter_x_start = max(0, -base_x);
            int32_t filter_y_end = min(filter_ht, input_ht - base_y);
            int32_t filter_x_end = min(filter_wd, input_wd - base_x);

            for (int32_t ch_idx = 0; ch_idx < channels; ch_idx++) {
                int8_t result = INT8_MIN;

                for (int32_t filter_y = filter_y_start; filter_y < filter_y_end; filter_y++) {
                    for (int32_t filter_x = filter_x_start; filter_x < filter_x_end; filter_x++) {
                        int32_t in_x_idx = base_x + filter_x;
                        int32_t in_y_idx = base_y + filter_y;
                        int32_t input_index = (in_y_idx * input_wd + in_x_idx) * channels + ch_idx;
                        result = max(input[input_index], result);
                    }
                }

                /* Activation function */
                result = max(result, activation_min);
                result = min(result, activation_max);

                int32_t output_index = (out_y * output_wd + out_x) * channels + ch_idx;
                output[output_index] = result;
            }
        }
    }
}
