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

void esp_nn_add_elementwise_u8_ansi(const uint8_t *input1_data,
                                    const uint8_t *input2_data,
                                    const int32_t input1_offset,
                                    const int32_t input2_offset,
                                    const int32_t input1_mult,
                                    const int32_t input2_mult,
                                    const int32_t input1_shift,
                                    const int32_t input2_shift,
                                    const int32_t left_shift,
                                    uint8_t *output,
                                    const int32_t out_offset,
                                    const int32_t out_mult,
                                    const int32_t out_shift,
                                    const int32_t activation_min,
                                    const int32_t activation_max,
                                    const int32_t size)
{
    for (int i = 0; i < size; i++) {
        int32_t tmp1 = input1_data[i] + input1_offset;
        int32_t tmp2 = input2_data[i] + input2_offset;

        tmp1 <<= left_shift;
        tmp2 <<= left_shift;

        tmp1 = esp_nn_sat_round_doubling_high_mul(tmp1, input1_mult);
        tmp2 = esp_nn_sat_round_doubling_high_mul(tmp2, input2_mult);

        tmp1 = esp_nn_div_by_power_of_two(tmp1, -input1_shift);
        tmp2 = esp_nn_div_by_power_of_two(tmp2, -input2_shift);

        int32_t out = tmp1 + tmp2;
        out = esp_nn_sat_round_doubling_high_mul(out, out_mult);
        out = esp_nn_div_by_power_of_two(out, -out_shift);
        out = out + out_offset;

        out = max(activation_min, min(out, activation_max));
        output[i] = (uint8_t) out;
    }
}

void esp_nn_add_elementwise_s8_ansi(const int8_t *input1_data,
                                    const int8_t *input2_data,
                                    const int32_t input1_offset,
                                    const int32_t input2_offset,
                                    const int32_t input1_mult,
                                    const int32_t input2_mult,
                                    const int32_t input1_shift,
                                    const int32_t input2_shift,
                                    const int32_t left_shift,
                                    int8_t *output,
                                    const int32_t out_offset,
                                    const int32_t out_mult,
                                    const int32_t out_shift,
                                    const int32_t activation_min,
                                    const int32_t activation_max,
                                    const int32_t size)
{
    for (int i = 0; i < size; i++) {
        int32_t tmp1 = input1_data[i] + input1_offset;
        int32_t tmp2 = input2_data[i] + input2_offset;

        tmp1 <<= left_shift;
        tmp2 <<= left_shift;

        tmp1 = esp_nn_sat_round_doubling_high_mul(tmp1, input1_mult);
        tmp2 = esp_nn_sat_round_doubling_high_mul(tmp2, input2_mult);

        tmp1 = esp_nn_div_by_power_of_two(tmp1, -input1_shift);
        tmp2 = esp_nn_div_by_power_of_two(tmp2, -input2_shift);

        int32_t out = tmp1 + tmp2;
        out = esp_nn_sat_round_doubling_high_mul(out, out_mult);
        out = esp_nn_div_by_power_of_two(out, -out_shift);
        out = out + out_offset;

        out = max(activation_min, min(out, activation_max));
        output[i] = (int8_t) out;
    }
}
