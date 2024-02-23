// Screen API

#pragma once

#include "tock.h"

#define SCREEN_ROTATION_NORMAL 0
#define SCREEN_ROTATION_90 1
#define SCREEN_ROTATION_180 2
#define SCREEN_ROTATION_270 3

#define SCREEN_PIXEL_FORMAT_MONO 0
#define SCREEN_PIXEL_FORMAT_RGB_233 1
#define SCREEN_PIXEL_FORMAT_RGB_565 2
#define SCREEN_PIXEL_FORMAT_RGB_888 3
#define SCREEN_PIXEL_FORMAT_ARGB_8888 4

#define SCREEN_PIXEL_FORMAT_ERROR -1

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_SCREEN 0x90001

// init
int screen_init (size_t len);
uint8_t * screen_buffer (void);

// query
bool screen_setup_enabled (void);
int screen_get_supported_resolutions (int* resolutions);
int screen_get_supported_resolution (size_t index, size_t *width, size_t *height);
int screen_get_supported_pixel_formats (int* formats);
int screen_get_supported_pixel_format (size_t index, int* format);

// power
int screen_set_brightness (size_t brightness);
int screen_invert_on (void);
int screen_invert_off (void);

// pixel format
int screen_get_bits_per_pixel (size_t format);
int screen_get_pixel_format (int* format);
int screen_set_pixel_format (size_t format);

// resolution and rotation
int screen_get_resolution (size_t *width, size_t *height);
int screen_set_resolution (size_t width, size_t height);

int screen_get_rotation (int* rotation);
int screen_set_rotation (size_t rotation);

// drawing
int screen_set_color (size_t position, size_t color);
int screen_set_frame (uint16_t x, uint16_t y, uint16_t width, uint16_t height);
int screen_fill (size_t color);
int screen_write (size_t length);

#ifdef __cplusplus
}
#endif
