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

void esp_nn_fully_connected_s8_ansi(const int8_t *input_data,
                                    const int32_t input_offset,
                                    const uint16_t row_len,
                                    const int8_t *filter_data,
                                    const int32_t filter_offset,
                                    const int32_t *bias,
                                    int8_t *out_data,
                                    const uint16_t out_channels,
                                    const int32_t out_offset,
                                    const int32_t out_shift,
                                    const int32_t out_mult,
                                    const int32_t activation_min,
                                    const int32_t activation_max)
{
    for (int32_t out_c = 0; out_c < out_channels; ++out_c) {
        int32_t result = 0;
        for (int32_t data_idx = 0; data_idx < row_len; data_idx++) {
            int32_t filter_index = row_len * out_c + data_idx;
            int32_t input_val = input_data[data_idx];
            int32_t filter_val = filter_data[filter_index];
            result += (filter_val + filter_offset) * (input_val + input_offset);
        }
        if (bias) {
            result += bias[out_c];
        }
        result = esp_nn_multiply_by_quantized_mult(result, out_mult, out_shift);
        result += out_offset;
        result = max(result, activation_min);
        result = min(result, activation_max);
        out_data[out_c] = (int8_t) result;
    }
}
