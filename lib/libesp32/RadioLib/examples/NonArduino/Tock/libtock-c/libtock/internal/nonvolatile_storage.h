#pragma once

#include "tock.h"

#define DRIVER_NUM_NONVOLATILE_STORAGE 0x50001

#ifdef __cplusplus
extern "C" {
#endif

int nonvolatile_storage_internal_read_done_subscribe(subscribe_upcall cb, void *userdata);
int nonvolatile_storage_internal_write_done_subscribe(subscribe_upcall cb, void *userdata);

int nonvolatile_storage_internal_read_buffer(uint8_t* buffer, uint32_t len);
int nonvolatile_storage_internal_write_buffer(uint8_t* buffer, uint32_t len);

int nonvolatile_storage_internal_get_number_bytes(int* number_bytes);
int nonvolatile_storage_internal_read(uint32_t offset, uint32_t length);
int nonvolatile_storage_internal_write(uint32_t offset, uint32_t length);

#ifdef __cplusplus
}
#endif
