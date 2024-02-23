#include "hmac.h"

#define DRIVER_NUM_HMAC 0x40003

#define TOCK_HMAC_CB       0

#define TOCK_HMAC_KEY_BUF      0
#define TOCK_HMAC_DATA_BUF     1
#define TOCK_HMAC_DEST_BUF     2

#define TOCK_HMAC_SET_ALGORITHM   0
#define TOCK_HMAC_RUN             1
#define TOCK_HMAC_UPDATE          2
#define TOCK_HMAC_FINISH          3

int hmac_set_callback (subscribe_upcall callback, void* callback_args) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_HMAC, TOCK_HMAC_CB, callback, callback_args);
  return tock_subscribe_return_to_returncode(sval);
}

int hmac_set_key_buffer(uint8_t* buffer, uint32_t len) {
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_HMAC, TOCK_HMAC_KEY_BUF, (void*) buffer, len);
  return tock_allow_ro_return_to_returncode(aval);
}

int hmac_set_data_buffer(uint8_t* buffer, uint32_t len) {
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_HMAC, TOCK_HMAC_DATA_BUF, (void*) buffer, len);
  return tock_allow_ro_return_to_returncode(aval);
}

int hmac_set_dest_buffer(uint8_t* buffer, uint32_t len) {
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_HMAC, TOCK_HMAC_DEST_BUF, (void*) buffer, len);
  return tock_allow_rw_return_to_returncode(aval);
}

int hmac_set_algorithm(uint8_t hash) {
  syscall_return_t cval = command(DRIVER_NUM_HMAC, TOCK_HMAC_SET_ALGORITHM, hash, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int hmac_run(void) {
  syscall_return_t cval = command(DRIVER_NUM_HMAC, TOCK_HMAC_RUN, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int hmac_update(void) {
  syscall_return_t cval = command(DRIVER_NUM_HMAC, TOCK_HMAC_UPDATE, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int hmac_finish(void) {
  syscall_return_t cval = command(DRIVER_NUM_HMAC, TOCK_HMAC_FINISH, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}
