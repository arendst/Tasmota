// Copyright 2010-2020 Espressif Systems (Shanghai) PTE LTD
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

#pragma once

#include "esp_camera.h"


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Uninitialize the lcd_cam module
 *
 * @param handle Provide handle pointer to release resources
 *
 * @return
 *     - ESP_OK Success
 *     - ESP_FAIL Uninitialize fail
 */
esp_err_t cam_deinit(void);

/**
 * @brief Initialize the lcd_cam module
 *
 * @param config Configurations - see lcd_cam_config_t struct
 *
 * @return
 *     - ESP_OK Success
 *     - ESP_ERR_INVALID_ARG Parameter error
 *     - ESP_ERR_NO_MEM No memory to initialize lcd_cam
 *     - ESP_FAIL Initialize fail
 */
esp_err_t cam_init(const camera_config_t *config);

esp_err_t cam_config(const camera_config_t *config, framesize_t frame_size, uint16_t sensor_pid);

void cam_stop(void);

void cam_start(void);

camera_fb_t *cam_take(TickType_t timeout);

void cam_give(camera_fb_t *dma_buffer);

#ifdef __cplusplus
}
#endif
