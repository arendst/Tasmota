#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

int kv_system_set_callback(subscribe_upcall callback, void* callback_args);

int kv_system_set_key_buffer(const uint8_t* buffer, uint32_t len);
int kv_system_set_input_buffer(const uint8_t* buffer, uint32_t len);
int kv_system_set_output_buffer(uint8_t* buffer, uint32_t len);

/*
 * Check that the KV system exists
 */
int kv_system_check_status(void);

int kv_system_get(void);
int kv_system_set(void);
int kv_system_delete(void);

#ifdef __cplusplus
}
#endif
