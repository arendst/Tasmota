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
#ifndef _IMG_CONVERTERS_H_
#define _IMG_CONVERTERS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "esp_camera.h"
#include "esp_jpg_decode.h"

typedef size_t (* jpg_out_cb)(void * arg, size_t index, const void* data, size_t len);

/**
 * @brief Convert image buffer to JPEG
 *
 * @param src       Source buffer in RGB565, RGB888, YUYV or GRAYSCALE format
 * @param src_len   Length in bytes of the source buffer
 * @param width     Width in pixels of the source image
 * @param height    Height in pixels of the source image
 * @param format    Format of the source image
 * @param quality   JPEG quality of the resulting image
 * @param cp        Callback to be called to write the bytes of the output JPEG
 * @param arg       Pointer to be passed to the callback
 *
 * @return true on success
 */
bool fmt2jpg_cb(uint8_t *src, size_t src_len, uint16_t width, uint16_t height, pixformat_t format, uint8_t quality, jpg_out_cb cb, void * arg);

/**
 * @brief Convert camera frame buffer to JPEG
 *
 * @param fb        Source camera frame buffer
 * @param quality   JPEG quality of the resulting image
 * @param cp        Callback to be called to write the bytes of the output JPEG
 * @param arg       Pointer to be passed to the callback
 *
 * @return true on success
 */
bool frame2jpg_cb(camera_fb_t * fb, uint8_t quality, jpg_out_cb cb, void * arg);

/**
 * @brief Convert image buffer to JPEG buffer
 *
 * @param src       Source buffer in RGB565, RGB888, YUYV or GRAYSCALE format
 * @param src_len   Length in bytes of the source buffer
 * @param width     Width in pixels of the source image
 * @param height    Height in pixels of the source image
 * @param format    Format of the source image
 * @param quality   JPEG quality of the resulting image
 * @param out       Pointer to be populated with the address of the resulting buffer.
 *                  You MUST free the pointer once you are done with it.
 * @param out_len   Pointer to be populated with the length of the output buffer
 *
 * @return true on success
 */
bool fmt2jpg(uint8_t *src, size_t src_len, uint16_t width, uint16_t height, pixformat_t format, uint8_t quality, uint8_t ** out, size_t * out_len);

/**
 * @brief Convert camera frame buffer to JPEG buffer
 *
 * @param fb        Source camera frame buffer
 * @param quality   JPEG quality of the resulting image
 * @param out       Pointer to be populated with the address of the resulting buffer
 * @param out_len   Pointer to be populated with the length of the output buffer
 *
 * @return true on success
 */
bool frame2jpg(camera_fb_t * fb, uint8_t quality, uint8_t ** out, size_t * out_len);

/**
 * @brief Convert image buffer to BMP buffer
 *
 * @param src       Source buffer in JPEG, RGB565, RGB888, YUYV or GRAYSCALE format
 * @param src_len   Length in bytes of the source buffer
 * @param width     Width in pixels of the source image
 * @param height    Height in pixels of the source image
 * @param format    Format of the source image
 * @param out       Pointer to be populated with the address of the resulting buffer
 * @param out_len   Pointer to be populated with the length of the output buffer
 *
 * @return true on success
 */
bool fmt2bmp(uint8_t *src, size_t src_len, uint16_t width, uint16_t height, pixformat_t format, uint8_t ** out, size_t * out_len);

/**
 * @brief Convert camera frame buffer to BMP buffer
 *
 * @param fb        Source camera frame buffer
 * @param out       Pointer to be populated with the address of the resulting buffer
 * @param out_len   Pointer to be populated with the length of the output buffer
 *
 * @return true on success
 */
bool frame2bmp(camera_fb_t * fb, uint8_t ** out, size_t * out_len);

/**
 * @brief Convert image buffer to RGB888 buffer (used for face detection)
 *
 * @param src       Source buffer in JPEG, RGB565, RGB888, YUYV or GRAYSCALE format
 * @param src_len   Length in bytes of the source buffer
 * @param format    Format of the source image
 * @param rgb_buf   Pointer to the output buffer (width * height * 3)
 *
 * @return true on success
 */
bool fmt2rgb888(const uint8_t *src_buf, size_t src_len, pixformat_t format, uint8_t * rgb_buf);

bool jpg2rgb565(const uint8_t *src, size_t src_len, uint8_t * out, jpg_scale_t scale);

#ifdef __cplusplus
}
#endif

#endif /* _IMG_CONVERTERS_H_ */
