#include "internal/nonvolatile_storage.h"

int nonvolatile_storage_internal_read_done_subscribe(subscribe_upcall cb, void *userdata) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_NONVOLATILE_STORAGE, 0, cb, userdata);
  return tock_subscribe_return_to_returncode(sval);
}

int nonvolatile_storage_internal_write_done_subscribe(subscribe_upcall cb, void *userdata) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_NONVOLATILE_STORAGE, 1, cb, userdata);
  return tock_subscribe_return_to_returncode(sval);
}

int nonvolatile_storage_internal_read_buffer(uint8_t* buffer, uint32_t len) {
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_NONVOLATILE_STORAGE, 0, (void*) buffer, len);
  return tock_allow_rw_return_to_returncode(aval);
}

int nonvolatile_storage_internal_write_buffer(uint8_t* buffer, uint32_t len) {
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_NONVOLATILE_STORAGE, 0, (void*) buffer, len);
  return tock_allow_ro_return_to_returncode(aval);
}

int nonvolatile_storage_internal_get_number_bytes(int* number_bytes) {
  syscall_return_t res = command(DRIVER_NUM_NONVOLATILE_STORAGE, 1, 0, 0);
  return tock_command_return_u32_to_returncode(res, (uint32_t*) number_bytes);
}

int nonvolatile_storage_internal_read(uint32_t offset, uint32_t length) {
  syscall_return_t res = command(DRIVER_NUM_NONVOLATILE_STORAGE, 2, (int) offset, (int) length);
  return tock_command_return_novalue_to_returncode(res);
}

int nonvolatile_storage_internal_write(uint32_t offset, uint32_t length) {
  syscall_return_t res = command(DRIVER_NUM_NONVOLATILE_STORAGE, 3, (int) offset, (int) length);
  return tock_command_return_novalue_to_returncode(res);
}
