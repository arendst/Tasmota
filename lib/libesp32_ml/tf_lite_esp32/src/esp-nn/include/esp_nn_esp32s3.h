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

/**
 * @file        Header definitions to include for esp_nn optimized functions for
 *              the ESP32-S3 platform
 */

#pragma once
#ifdef CONFIG_IDF_TARGET_ESP32S3
#include "esp_nn_defs.h"
#include "esp_nn_ansi_headers.h"

/************************** Basic math functions *****************************/


/**
 * @brief       elementwise addition
 *
 * @note        inputs type: int8_t, output: int8_t
 *              input offsets: although int32_t, they are contained in 8 bits [-128, 127]
 *
 *              shift values are expected to be <= 0
 */
void esp_nn_add_elementwise_s8_esp32s3(const int8_t *input1_data,
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
                                       const int32_t size);

/**
 * @brief       elementwise multiplication
 *
 * @note        inputs type: int8_t, output: int8_t
 *              input offsets: although int32_t, they are contained in 8 bits [-128, 127]
 *
 *              output shift is expected to be <= 0
 */
void esp_nn_mul_elementwise_s8_esp32s3(const int8_t *input1_data,
                                       const int8_t *input2_data,
                                       const int32_t input1_offset,
                                       const int32_t input2_offset,
                                       int8_t *output,
                                       const int32_t out_offset,
                                       const int32_t out_mult,
                                       const int32_t out_shift,
                                       const int32_t activation_min,
                                       const int32_t activation_max,
                                       const int32_t size);


/************************** Convolution functions *****************************/

/**
 * @brief       depthwise convolution per channel
 *
 * @note        inputs type: int8_t, output: int8_t
 *              Version used in tflite is per channel.
 *              This version follows the same footsprints.
 *              Meaning, it has per out_channel shift and multiplier for
 *              requantization
 *
 *              optimization notes: Though input_offset is int32 type,
 *              offset values are contained in 8 bits [-128, 127]
 */
void esp_nn_depthwise_conv_s8_esp32s3(const data_dims_t *input_dims,
                                      const int8_t *input_data,
                                      const data_dims_t *filter_dims,
                                      const int8_t *filter_data,
                                      const int32_t *bias,
                                      const data_dims_t *output_dims,
                                      int8_t *output_data,
                                      const dw_conv_params_t *conv_params,
                                      const quant_data_t *quant_data);

/**
 * @brief       2d - convolution channelwise
 *
 * @note        operation: result += (input + offset) * filter
 *
 *              inputs type: int8_t, output: int8_t
 *              input offsets: although int32_t, they are contained in 8 bits [-128, 127]
 */
void esp_nn_conv_s8_esp32s3(const data_dims_t *input_dims,
                            const int8_t *input_data,
                            const data_dims_t *filter_dims,
                            const int8_t *filter_data,
                            const int32_t *bias,
                            const data_dims_t *output_dims,
                            int8_t *output_data,
                            const conv_params_t *conv_params,
                            const quant_data_t *quant_data);

int esp_nn_get_conv_scratch_size_esp32s3(const data_dims_t *input_dims,
                                         const data_dims_t *filter_dims,
                                         const data_dims_t *output_dims,
                                         const conv_params_t *conv_params);
void esp_nn_set_conv_scratch_buf_esp32s3(const void *buf);

int esp_nn_get_depthwise_conv_scratch_size_esp32s3(const data_dims_t *input_dims,
                                                   const data_dims_t *filter_dims,
                                                   const data_dims_t *output_dims,
                                                   const dw_conv_params_t *conv_params);
void esp_nn_set_depthwise_conv_scratch_buf_esp32s3(const void *buf);

/************************** Pooling functions *****************************/

/**
 * @brief       max_pool
 *
 * @note        inputs type: int8_t, output: int8_t
 *              input offsets: although int32_t, they are contained in 8 bits [-128, 127]
 */
void esp_nn_max_pool_s8_esp32s3(const int8_t *input,
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
                                const uint16_t channels);

/**
 * @brief       avg_pool
 *
 * @note        inputs type: int8_t, output: int8_t
 *              input offsets: although int32_t, they are contained in 8 bits [-128, 127]
 */
void esp_nn_avg_pool_s8_esp32s3(const int8_t *input,
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
                                const uint16_t channels);


/************************** Fully connected functions *****************************/

/**
 * @brief       fully connected
 *
 * @note        inputs type: int8_t, output: int8_t
 *              input offsets: although int32_t, they are contained in 8 bits [-128, 127]
 *
 *              Current version works only on aligned input.
 *              row_len and channels should both be multiple of 8.
 */
void esp_nn_fully_connected_s8_esp32s3(const int8_t *input_data,
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
                                       const int32_t activation_max);

/**
 * @brief       relu6
 *
 * @note        inout: int8_t
 */
void esp_nn_relu6_s8_esp32s3(int8_t *data, uint16_t size);

/********************** function defines ***************************/

#define esp_nn_add_elementwise_s8 esp_nn_add_elementwise_s8_esp32s3
#define esp_nn_mul_elementwise_s8 esp_nn_mul_elementwise_s8_esp32s3

#define esp_nn_depthwise_conv_s8 esp_nn_depthwise_conv_s8_esp32s3

#define esp_nn_get_conv_scratch_size esp_nn_get_conv_scratch_size_esp32s3
#define esp_nn_set_conv_scratch_buf esp_nn_set_conv_scratch_buf_esp32s3

#define esp_nn_get_depthwise_conv_scratch_size esp_nn_get_depthwise_conv_scratch_size_esp32s3
#define esp_nn_set_depthwise_conv_scratch_buf esp_nn_set_depthwise_conv_scratch_buf_esp32s3

#define esp_nn_conv_s8 esp_nn_conv_s8_esp32s3

#define esp_nn_relu6_s8 esp_nn_relu6_s8_esp32s3

#define esp_nn_avg_pool_s8 esp_nn_avg_pool_s8_esp32s3
#define esp_nn_max_pool_s8 esp_nn_max_pool_s8_esp32s3

#define esp_nn_fully_connected_s8 esp_nn_fully_connected_s8_esp32s3

#define esp_nn_get_softmax_scratch_size esp_nn_get_softmax_scratch_size_opt
#define esp_nn_set_softmax_scratch_buf esp_nn_set_softmax_scratch_buf_opt
#define esp_nn_softmax_s8 esp_nn_softmax_s8_opt

#endif // CONFIG_IDF_TARGET_ESP32S3