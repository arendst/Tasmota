#include "sha.h"

#define DRIVER_NUM_SHA 0x40005

#define TOCK_SHA_CB       0

#define TOCK_SHA_DATA_BUF     1
#define TOCK_SHA_DEST_BUF     2

#define TOCK_SHA_SET_ALGORITHM   0
#define TOCK_SHA_RUN             1
#define TOCK_SHA_UPDATE          2
#define TOCK_SHA_FINISH          3

int sha_set_callback (subscribe_upcall callback, void* callback_args) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_SHA, TOCK_SHA_CB, callback, callback_args);
  return tock_subscribe_return_to_returncode(sval);
}

int sha_set_data_buffer(uint8_t* buffer, uint32_t len) {
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_SHA, TOCK_SHA_DATA_BUF, (void*) buffer, len);
  return tock_allow_ro_return_to_returncode(aval);
}

int sha_set_dest_buffer(uint8_t* buffer, uint32_t len) {
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_SHA, TOCK_SHA_DEST_BUF, (void*) buffer, len);
  return tock_allow_rw_return_to_returncode(aval);
}

int sha_set_algorithm(uint8_t hash) {
  syscall_return_t cval = command(DRIVER_NUM_SHA, TOCK_SHA_SET_ALGORITHM, hash, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int sha_run(void) {
  syscall_return_t cval = command(DRIVER_NUM_SHA, TOCK_SHA_RUN, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int sha_update(void) {
  syscall_return_t cval = command(DRIVER_NUM_SHA, TOCK_SHA_UPDATE, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int sha_finish(void) {
  syscall_return_t cval = command(DRIVER_NUM_SHA, TOCK_SHA_FINISH, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}
