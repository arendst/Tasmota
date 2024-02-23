#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_TSL2561 0x70000

int tsl2561_set_callback (subscribe_upcall callback, void* callback_args);
int tsl2561_get_lux (void);

int tsl2561_get_lux_sync (int* lux);

#ifdef __cplusplus
}
#endif
