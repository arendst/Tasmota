// Copyright 2022 Espressif Systems (Shanghai) PTE LTD
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

#include "softmax_common.h"
#include <stdio.h>

static int32_t *scratch_buf = NULL;

/**
 * @brief   Get scratch buffer size needed by softmax function
 *
 * @param   width
 * @param   height
 * @return  size in bytes
 *
 * @note    buffer must be 4 byte aligned
 */
int32_t esp_nn_get_softmax_scratch_size_opt(const int32_t width, const int32_t height)
{
    (void) height;
    return width * 4;
}

/**
 * @brief   Set scratch buffer to be used by softmax function
 *
 * @param   buffer  this can be NULL if one needs to unset it
 *                  must be aligned to 4 bytes
 */
void esp_nn_set_softmax_scratch_buf_opt(void *buffer)
{
    scratch_buf = (int32_t *) buffer;
}

void esp_nn_softmax_s8_opt(const int8_t *input_data,
                           const int32_t height,
                           const int32_t width,
                           const int32_t mult,
                           const int32_t shift,
                           const int32_t diff_min,
                           int8_t *output_data)
{
    if (scratch_buf == NULL) {
        printf("%s error! scratch buffer not set\n", __FUNCTION__);
        return;
    }
    // The representation chosen for the input to the exp() function is Q5.26.
    // We need to leave extra space since values that we skip might be as large as
    // -32 before multiplying by input mult, and therefore as large as
    // -16 afterwards.  Note that exp(-8) is definitely not insignificant to
    // accumulation, but exp(-16) definitely is.
#define ACCUM_BITS  12
#define DIFF_BITS   5

    const int32_t mask = (1 << shift);
    int32_t col = 0;
    const int8_t *in_ptr = input_data;
    int8_t *out_ptr = output_data;

    for (int row_idx = 0; row_idx < height; row_idx++) {
        int8_t max_in_row = in_ptr[0];
        for (col = 1; col < width; col++) {
            max_in_row = max(max_in_row, in_ptr[col]);
        }

        int32_t input_diff = 0;
        int32_t sum_of_exps = 0;

        for (col = 0; col < width; col++) {
            input_diff = in_ptr[col] - max_in_row;
            if (input_diff >= diff_min) {
                const int32_t input_diff_rescaled = SAT_HIGH_MUL(input_diff * mask, mult);
                const int32_t exp_raw = esp_nn_exp_on_negative_values(input_diff_rescaled);
                scratch_buf[col] = exp_raw; // store to avoid duplicate calculation later
                sum_of_exps += DIV_POW2(exp_raw, ACCUM_BITS);
            }
        }

        const int32_t headroom_plus1 = esp_nn_clz32((uint32_t) sum_of_exps);
        const int32_t shifted_scale = ONE_OVER_ONE_X((sum_of_exps << headroom_plus1) - (1 << 31));
        const int32_t bits_over_unit = ACCUM_BITS - headroom_plus1 + 31 - sizeof(int8_t) * 8;

        for (col = 0; col < width; col++) {
            input_diff = in_ptr[col] - max_in_row;
            if (input_diff >= diff_min) {
                int32_t exp_raw = scratch_buf[col];
                const int32_t shifted_output = SAT_HIGH_MUL(shifted_scale, exp_raw);
                const int32_t result = DIV_POW2(shifted_output, bits_over_unit) - 128;
                out_ptr[col] = (int8_t) esp_nn_saturate8(result);
            } else {
                out_ptr[col] = -128;
            }
        }
        in_ptr  += width;
        out_ptr += width;
    }
}
