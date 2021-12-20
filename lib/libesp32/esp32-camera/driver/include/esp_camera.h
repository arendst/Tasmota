// Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
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
/*
 * Example Use
 *
    static camera_config_t camera_example_config = {
        .pin_pwdn       = PIN_PWDN,
        .pin_reset      = PIN_RESET,
        .pin_xclk       = PIN_XCLK,
        .pin_sscb_sda   = PIN_SIOD,
        .pin_sscb_scl   = PIN_SIOC,
        .pin_d7         = PIN_D7,
        .pin_d6         = PIN_D6,
        .pin_d5         = PIN_D5,
        .pin_d4         = PIN_D4,
        .pin_d3         = PIN_D3,
        .pin_d2         = PIN_D2,
        .pin_d1         = PIN_D1,
        .pin_d0         = PIN_D0,
        .pin_vsync      = PIN_VSYNC,
        .pin_href       = PIN_HREF,
        .pin_pclk       = PIN_PCLK,

        .xclk_freq_hz   = 20000000,
        .ledc_timer     = LEDC_TIMER_0,
        .ledc_channel   = LEDC_CHANNEL_0,
        .pixel_format   = PIXFORMAT_JPEG,
        .frame_size     = FRAMESIZE_SVGA,
        .jpeg_quality   = 10,
        .fb_count       = 2,
        .grab_mode      = CAMERA_GRAB_WHEN_EMPTY
    };

    esp_err_t camera_example_init(){
        return esp_camera_init(&camera_example_config);
    }

    esp_err_t camera_example_capture(){
        //capture a frame
        camera_fb_t * fb = esp_camera_fb_get();
        if (!fb) {
            ESP_LOGE(TAG, "Frame buffer could not be acquired");
            return ESP_FAIL;
        }

        //replace this with your own function
        display_image(fb->width, fb->height, fb->pixformat, fb->buf, fb->len);

        //return the frame buffer back to be reused
        esp_camera_fb_return(fb);

        return ESP_OK;
    }
*/

#pragma once

#include "esp_err.h"
#include "driver/ledc.h"
#include "sensor.h"
#include "sys/time.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Configuration structure for camera initialization
 */
typedef enum {
    CAMERA_GRAB_WHEN_EMPTY,         /*!< Fills buffers when they are empty. Less resources but first 'fb_count' frames might be old */
    CAMERA_GRAB_LATEST              /*!< Except when 1 frame buffer is used, queue will always contain the last 'fb_count' frames */
} camera_grab_mode_t;

/**
 * @brief Camera frame buffer location
 */
typedef enum {
    CAMERA_FB_IN_PSRAM,         /*!< Frame buffer is placed in external PSRAM */
    CAMERA_FB_IN_DRAM           /*!< Frame buffer is placed in internal DRAM */
} camera_fb_location_t;

/**
 * @brief Configuration structure for camera initialization
 */
typedef struct {
    int pin_pwdn;                   /*!< GPIO pin for camera power down line */
    int pin_reset;                  /*!< GPIO pin for camera reset line */
    int pin_xclk;                   /*!< GPIO pin for camera XCLK line */
    int pin_sscb_sda;               /*!< GPIO pin for camera SDA line */
    int pin_sscb_scl;               /*!< GPIO pin for camera SCL line */
    int pin_d7;                     /*!< GPIO pin for camera D7 line */
    int pin_d6;                     /*!< GPIO pin for camera D6 line */
    int pin_d5;                     /*!< GPIO pin for camera D5 line */
    int pin_d4;                     /*!< GPIO pin for camera D4 line */
    int pin_d3;                     /*!< GPIO pin for camera D3 line */
    int pin_d2;                     /*!< GPIO pin for camera D2 line */
    int pin_d1;                     /*!< GPIO pin for camera D1 line */
    int pin_d0;                     /*!< GPIO pin for camera D0 line */
    int pin_vsync;                  /*!< GPIO pin for camera VSYNC line */
    int pin_href;                   /*!< GPIO pin for camera HREF line */
    int pin_pclk;                   /*!< GPIO pin for camera PCLK line */

    int xclk_freq_hz;               /*!< Frequency of XCLK signal, in Hz. EXPERIMENTAL: Set to 16MHz on ESP32-S2 or ESP32-S3 to enable EDMA mode */

    ledc_timer_t ledc_timer;        /*!< LEDC timer to be used for generating XCLK  */
    ledc_channel_t ledc_channel;    /*!< LEDC channel to be used for generating XCLK  */

    pixformat_t pixel_format;       /*!< Format of the pixel data: PIXFORMAT_ + YUV422|GRAYSCALE|RGB565|JPEG  */
    framesize_t frame_size;         /*!< Size of the output image: FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA  */

    int jpeg_quality;               /*!< Quality of JPEG output. 0-63 lower means higher quality  */
    size_t fb_count;                /*!< Number of frame buffers to be allocated. If more than one, then each frame will be acquired (double speed)  */
    camera_fb_location_t fb_location; /*!< The location where the frame buffer will be allocated */
    camera_grab_mode_t grab_mode;   /*!< When buffers should be filled */
} camera_config_t;

/**
 * @brief Data structure of camera frame buffer
 */
typedef struct {
    uint8_t * buf;              /*!< Pointer to the pixel data */
    size_t len;                 /*!< Length of the buffer in bytes */
    size_t width;               /*!< Width of the buffer in pixels */
    size_t height;              /*!< Height of the buffer in pixels */
    pixformat_t format;         /*!< Format of the pixel data */
    struct timeval timestamp;   /*!< Timestamp since boot of the first DMA buffer of the frame */
} camera_fb_t;

#define ESP_ERR_CAMERA_BASE 0x20000
#define ESP_ERR_CAMERA_NOT_DETECTED             (ESP_ERR_CAMERA_BASE + 1)
#define ESP_ERR_CAMERA_FAILED_TO_SET_FRAME_SIZE (ESP_ERR_CAMERA_BASE + 2)
#define ESP_ERR_CAMERA_FAILED_TO_SET_OUT_FORMAT (ESP_ERR_CAMERA_BASE + 3)
#define ESP_ERR_CAMERA_NOT_SUPPORTED            (ESP_ERR_CAMERA_BASE + 4)

/**
 * @brief Initialize the camera driver
 *
 * @note call camera_probe before calling this function
 *
 * This function detects and configures camera over I2C interface,
 * allocates framebuffer and DMA buffers,
 * initializes parallel I2S input, and sets up DMA descriptors.
 *
 * Currently this function can only be called once and there is
 * no way to de-initialize this module.
 *
 * @param config  Camera configuration parameters
 *
 * @return ESP_OK on success
 */
esp_err_t esp_camera_init(const camera_config_t* config);

/**
 * @brief Deinitialize the camera driver
 *
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_INVALID_STATE if the driver hasn't been initialized yet
 */
esp_err_t esp_camera_deinit();

/**
 * @brief Obtain pointer to a frame buffer.
 *
 * @return pointer to the frame buffer
 */
camera_fb_t* esp_camera_fb_get();

/**
 * @brief Return the frame buffer to be reused again.
 *
 * @param fb    Pointer to the frame buffer
 */
void esp_camera_fb_return(camera_fb_t * fb);

/**
 * @brief Get a pointer to the image sensor control structure
 *
 * @return pointer to the sensor
 */
sensor_t * esp_camera_sensor_get();

/**
 * @brief Save camera settings to non-volatile-storage (NVS)
 *
 * @param key   A unique nvs key name for the camera settings
 */
esp_err_t esp_camera_save_to_nvs(const char *key);

/**
 * @brief Load camera settings from non-volatile-storage (NVS)
 *
 * @param key   A unique nvs key name for the camera settings
 */
esp_err_t esp_camera_load_from_nvs(const char *key);

#ifdef __cplusplus
}
#endif

#include "img_converters.h"

