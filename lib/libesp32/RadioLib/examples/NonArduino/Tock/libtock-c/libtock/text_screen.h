#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_TEXT_SCREEN 0x90003

int text_screen_init (size_t len);
uint8_t* text_screen_buffer (void);

int text_screen_display_on (void);
int text_screen_display_off (void);
int text_screen_blink_on (void);
int text_screen_blink_off (void);
int text_screen_show_cursor (void);
int text_screen_hide_cursor (void);
int text_screen_clear (void);
int text_screen_home (void);

int text_screen_get_size (size_t* width, size_t* height);

int text_screen_set_cursor (uint8_t col, uint8_t row);
int text_screen_write (size_t len);

#ifdef __cplusplus
}
#endif