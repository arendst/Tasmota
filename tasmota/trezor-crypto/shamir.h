/*
 * Low level API for Daan Sprenkels' Shamir secret sharing library
 * Copyright (c) 2017 Daan Sprenkels <hello@dsprenkels.com>
 * Copyright (c) 2019 SatoshiLabs
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Usage of this API is hazardous and is only reserved for beings with a
 * good understanding of the Shamir secret sharing scheme and who know how
 * crypto code is implemented. If you are unsure about this, use the
 * intermediate level API. You have been warned!
 */

#ifndef __SHAMIR_H__
#define __SHAMIR_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SHAMIR_MAX_LEN 32

/*
 * Computes f(x) given the Shamir shares (x_1, f(x_1)), ... , (x_m, f(x_m)).
 * The x coordinates of the shares must be pairwise distinct. Returns true on
 * success, otherwise false.
 * result: Array of length len where the evaluations of the polynomials in x
 *     will be written.
 * result_index: The x coordinate of the result.
 * share_indices: Points to the array of integers x_1, ... , x_m.
 * share_values: Points to the array of y_1, ... , y_m, where each y_i is an
 *     array of bytes of length len representing the evaluations of the
 *     polynomials in x_i.
 * share_count: The number of shares m.
 * len: The length of the result array and of each of the y_1, ... , y_m arrays.
 *
 * The number of shares used to compute the result may be larger than the
 * required threshold.
 *
 * This function does *not* do *any* checking for integrity. If any of the
 * shares are not original, this will result in an invalid restored value.
 * All values written to `result` should be treated as secret. Even if some of
 * the shares that were provided as input were incorrect, the result *still*
 * allows an attacker to gain information about the correct result.
 *
 * This function treats `shares_values`, `share_indices` and `result` as secret
 * values. `share_count` is treated as a public value (for performance reasons).
 */
bool shamir_interpolate(uint8_t *result, uint8_t result_index,
                        const uint8_t *share_indices,
                        const uint8_t **share_values, uint8_t share_count,
                        size_t len);
    
#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* __SHAMIR_H__ */
