#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_BUTTON 0x3

int button_subscribe(subscribe_upcall callback, void *ud);
int button_enable_interrupt(int button_num);
int button_disable_interrupt(int button_num);
int button_read(int button_num, int* button_value);
int button_count(int* count);


#ifdef __cplusplus
}
#endif

