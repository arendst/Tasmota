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

#include <stdint.h>
#include "../common/common_functions.h"

#define MASK_IF_ZERO(x)                 (x) == 0 ? ~0 : 0
#define MASK_IF_NON_ZERO(x)             (x) != 0 ? ~0 : 0
#define SELECT_USING_MASK(mask, a, b)   ((mask) & (a)) ^ (~(mask) & (b))
#define SAT_HIGH_MUL(x, y)              esp_nn_sat_round_doubling_high_mul((x), (y))
#define DIV_POW2(x,y)                   esp_nn_div_by_power_of_two((x), (y))

__NN_FORCE_INLINE__ int32_t mul_power_of_2(int val, int exp)
{
    const int32_t thresh = ((1 << (31 - exp)) - 1);
    int32_t result = val << exp;
    result = SELECT_USING_MASK(MASK_IF_NON_ZERO(val > thresh), INT32_MAX, result);
    result = SELECT_USING_MASK(MASK_IF_NON_ZERO(val < -thresh), INT32_MIN, result);
    return result;
}

/**
 * @brief   Calculate `1 / (1 + x)` for x in [0, 1]
 *
 * @param   val     input value to calculate `1/(1+x)` for
 * @return  `int32_t` result
 * @note    Newton-Raphson division
 *
 *          https://en.wikipedia.org/wiki/Division_algorithm#Newton.E2.80.93Raphson_division
 *          Refer to that page for the logic behind the 48/17 and 32/17 constants.
 *          Pseudocode: https://en.wikipedia.org/wiki/Division_algorithm#Pseudocode
 */
__NN_FORCE_INLINE__ int32_t esp_nn_one_over_one_plus_x_for_x_in_0_1(int32_t val)
{
    const int64_t sum = (int64_t) val + INT32_MAX;
    const int32_t half_denominator = (int32_t) ((sum + (sum >= 0 ? 1 : -1)) / 2L);
    int32_t constant_48_over_17 = 1515870810;
    int32_t constant_neg_32_over_17 = -1010580540;
    int32_t x = constant_48_over_17 + SAT_HIGH_MUL(half_denominator, constant_neg_32_over_17);
    const int32_t fixed_2_one = (1 << 29);

    x += mul_power_of_2(SAT_HIGH_MUL(x, fixed_2_one - SAT_HIGH_MUL(half_denominator, x)), 2);
    x += mul_power_of_2(SAT_HIGH_MUL(x, fixed_2_one - SAT_HIGH_MUL(half_denominator, x)), 2);
    x += mul_power_of_2(SAT_HIGH_MUL(x, fixed_2_one - SAT_HIGH_MUL(half_denominator, x)), 2);

    return mul_power_of_2(x, 1);
}

#define ONE_OVER_ONE_X(x)   esp_nn_one_over_one_plus_x_for_x_in_0_1((x))

/**
 * @brief   Return exp(x) for x < 0.
 *
 */
__NN_FORCE_INLINE__ int32_t esp_nn_exp_on_negative_values(int32_t val)
{
    int32_t shift = 24;

    const int32_t one_quarter = (1 << shift);
    int32_t mask = one_quarter - 1;
    const int32_t val_mod_minus_quarter = (val & mask) - one_quarter;
    const int32_t remainder             = val_mod_minus_quarter - val;

    // calculate exponent for x in [-1/4, 0) in `result`
    const int32_t x                     = (val_mod_minus_quarter << 5) + (1 << 28);
    const int32_t x2                    = SAT_HIGH_MUL(x, x);
    const int32_t x3                    = SAT_HIGH_MUL(x2, x);
    const int32_t x4                    = SAT_HIGH_MUL(x2, x2);
    const int32_t one_over_3            = 715827883;
    const int32_t one_over_8            = 1895147668;

    const int32_t x4_over_4 = DIV_POW2(x4, 2);
    const int32_t x4_over_4_plus_x3_over_6_plus_x2_over_2 = DIV_POW2(SAT_HIGH_MUL(x4_over_4 + x3, one_over_3) + x2, 1);
    int32_t result = one_over_8 + SAT_HIGH_MUL(one_over_8, x + x4_over_4_plus_x3_over_6_plus_x2_over_2);

#define SELECT_IF_NON_ZERO(x) {                                   \
    mask   = MASK_IF_NON_ZERO(remainder & (1 << shift++));        \
    result = SELECT_USING_MASK(mask, SAT_HIGH_MUL(result, x), result); \
}

    SELECT_IF_NON_ZERO(1672461947)
    SELECT_IF_NON_ZERO(1302514674)
    SELECT_IF_NON_ZERO(790015084)
    SELECT_IF_NON_ZERO(290630308)
    SELECT_IF_NON_ZERO(39332535)
    SELECT_IF_NON_ZERO(720401)
    SELECT_IF_NON_ZERO(242)

#undef SELECT_IF_NON_ZERO

    mask = MASK_IF_ZERO(val);
    return SELECT_USING_MASK(mask, INT32_MAX, result);
}