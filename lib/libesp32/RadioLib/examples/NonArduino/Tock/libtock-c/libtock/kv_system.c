#include "kv_system.h"

#define DRIVER_NUM_KV_SYSTEM 0x50003

#define TOCK_KV_SYSTEM_CB           0

#define TOCK_KV_SYSTEM_KEY_BUF      0
#define TOCK_KV_SYSTEM_INPUT_BUF    1
#define TOCK_KV_SYSTEM_OUTPUT_BUF   0

#define TOCK_KV_SYSTEM_CHECK_PRESENT   0
#define TOCK_KV_SYSTEM_GET             1
#define TOCK_KV_SYSTEM_SET             2
#define TOCK_KV_SYSTEM_DELETE          3

int kv_system_set_callback(subscribe_upcall callback, void* callback_args) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_KV_SYSTEM, TOCK_KV_SYSTEM_CB, callback, callback_args);
  return tock_subscribe_return_to_returncode(sval);
}

int kv_system_set_key_buffer(const uint8_t* buffer, uint32_t len) {
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_KV_SYSTEM, TOCK_KV_SYSTEM_KEY_BUF, (void*) buffer, len);
  return tock_allow_ro_return_to_returncode(aval);
}

int kv_system_set_input_buffer(const uint8_t* buffer, uint32_t len) {
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_KV_SYSTEM, TOCK_KV_SYSTEM_INPUT_BUF, (void*) buffer, len);
  return tock_allow_ro_return_to_returncode(aval);
}

int kv_system_set_output_buffer(uint8_t* buffer, uint32_t len) {
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_KV_SYSTEM, TOCK_KV_SYSTEM_OUTPUT_BUF, (void*) buffer, len);
  return tock_allow_rw_return_to_returncode(aval);
}

int kv_system_check_status(void) {
  syscall_return_t cval = command(DRIVER_NUM_KV_SYSTEM, TOCK_KV_SYSTEM_CHECK_PRESENT, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int kv_system_get(void) {
  syscall_return_t cval = command(DRIVER_NUM_KV_SYSTEM, TOCK_KV_SYSTEM_GET, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int kv_system_set(void) {
  syscall_return_t cval = command(DRIVER_NUM_KV_SYSTEM, TOCK_KV_SYSTEM_SET, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int kv_system_delete(void) {
  syscall_return_t cval = command(DRIVER_NUM_KV_SYSTEM, TOCK_KV_SYSTEM_DELETE, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}
