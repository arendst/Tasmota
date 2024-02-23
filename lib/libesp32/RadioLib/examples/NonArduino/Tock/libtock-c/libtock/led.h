#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_LEDS 0x00000002

int led_on(int led_num);
int led_off(int led_num);
int led_toggle(int led_num);

// Returns the number of LEDs on the host platform.
int led_count(int* count);

#ifdef __cplusplus
}
#endif
