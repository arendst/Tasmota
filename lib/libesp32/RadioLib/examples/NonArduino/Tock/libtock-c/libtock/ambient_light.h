#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_AMBIENT_LIGHT 0x60002

// units: ambient light reading in lux (lx).

int ambient_light_subscribe(subscribe_upcall callback, void* userdata);
int ambient_light_start_intensity_reading(void);

int ambient_light_read_intensity_sync(int* lux_value);

#ifdef __cplusplus
}
#endif
