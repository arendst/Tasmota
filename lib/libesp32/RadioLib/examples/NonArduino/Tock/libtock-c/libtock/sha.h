#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

int sha_set_callback (subscribe_upcall callback, void* callback_args);

int sha_set_data_buffer(uint8_t* buffer, uint32_t len);
int sha_set_dest_buffer(uint8_t* buffer, uint32_t len);

int sha_set_algorithm(uint8_t hash);
int sha_run(void);
int sha_update(void);
int sha_finish(void);

#ifdef __cplusplus
}
#endif
