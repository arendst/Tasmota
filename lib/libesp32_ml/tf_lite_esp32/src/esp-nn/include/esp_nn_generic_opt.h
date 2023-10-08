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
 * @file        Header definitions to include for esp_nn generic optimisations
 *              For functions which not having optimisations, _ansi versions are picked.
 */

#pragma once

#include "esp_nn_defs.h"
#include "esp_nn_ansi_headers.h"

#define esp_nn_add_elementwise_s8 esp_nn_add_elementwise_s8_ansi
#define esp_nn_mul_elementwise_s8 esp_nn_mul_elementwise_s8_ansi

#define esp_nn_depthwise_conv_s8 esp_nn_depthwise_conv_s8_opt

#define esp_nn_conv_s8 esp_nn_conv_s8_opt

#define esp_nn_get_conv_scratch_size esp_nn_get_conv_scratch_size_opt
#define esp_nn_set_conv_scratch_buf esp_nn_set_conv_scratch_buf_opt

#define esp_nn_get_depthwise_conv_scratch_size esp_nn_get_depthwise_conv_scratch_size_opt
#define esp_nn_set_depthwise_conv_scratch_buf esp_nn_set_depthwise_conv_scratch_buf_opt

#define esp_nn_relu6_s8 esp_nn_relu6_s8_ansi

#define esp_nn_avg_pool_s8 esp_nn_avg_pool_s8_ansi
#define esp_nn_max_pool_s8 esp_nn_max_pool_s8_ansi

#define esp_nn_fully_connected_s8 esp_nn_fully_connected_s8_ansi

#define esp_nn_get_softmax_scratch_size esp_nn_get_softmax_scratch_size_opt
#define esp_nn_set_softmax_scratch_buf esp_nn_set_softmax_scratch_buf_opt
#define esp_nn_softmax_s8 esp_nn_softmax_s8_opt
