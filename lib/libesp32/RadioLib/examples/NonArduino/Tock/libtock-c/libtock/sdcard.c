// SD card interface

#include "sdcard.h"
#include "tock.h"

// used for creating synchronous versions of functions
//
// fired - set when callback has been called
// block_size - block size of SD card, set upon initialization complete
// size_in_kB - size in kilobytes of SD card, set upon initialization complete
// error - error code signalled in callback, 0 if successful
typedef struct {
  bool fired;
  uint32_t block_size;
  uint32_t size_in_kB;
  int32_t error;
} sdcard_data_t;

// Internal callback for creating synchronous functions
//
// callback_type - number indicating which type of callback occurred
// arg1, arg2 - meaning varies based on callback_type
// callback_args - user data passed into the set_callback function
//
// Possible callbacks
// 0: card_detection_changed, SD card was either installed or removed
//    arg1, arg2 - no meaning
// 1: init_done, intialization completed successfully
//    arg1 - block_size, block size of SD card in bytes
//    arg2 - size_in_kB, total size of SD card in kilobytes
// 2: read_done, read block completed successfully
//    arg1 - len, number of bytes read
// 3: write_done, write block completed successfully
//    arg1 - len, number of bytes written
// 4: error, an error occurred
//    arg1 - error, number representing the error that occurred
static void sdcard_upcall (int callback_type, int arg1, int arg2, void* callback_args) {

  sdcard_data_t* result = (sdcard_data_t*) callback_args;
  switch (callback_type) {
    case 0:
      // card_detection_changed
      result->error = RETURNCODE_EUNINSTALLED;
      break;

    case 1:
      // init_done
      result->block_size = arg1;
      result->size_in_kB = arg2;
      result->error      = RETURNCODE_SUCCESS;
      break;

    case 2:
      // read_done
      result->error = RETURNCODE_SUCCESS;
      break;

    case 3:
      // write_done
      result->error = RETURNCODE_SUCCESS;
      break;

    case 4:
      // error
      // This is not a STATUSCODE, but rather an SD Card specific error
      // See capsule sdcard.rs `enum SdCardError`
      result->error = arg1;
      break;
  }
  result->fired = true;
}

int sdcard_set_callback (subscribe_upcall callback, void* callback_args) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_SDCARD, 0, callback, callback_args);
  return tock_subscribe_return_to_returncode(sval);
}

int sdcard_set_read_buffer (uint8_t* buffer, uint32_t len) {
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_SDCARD, 0, (void*) buffer, len);
  return tock_allow_rw_return_to_returncode(aval);
}

int sdcard_set_write_buffer (uint8_t* buffer, uint32_t len) {
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_SDCARD, 0, (void*) buffer, len);
  return tock_allow_ro_return_to_returncode(aval);
}

int sdcard_is_installed (void) {
  syscall_return_t cval = command(DRIVER_NUM_SDCARD, 1, 0, 0);
  uint32_t result       = 0;
  tock_command_return_u32_to_returncode(cval, &result);
  return (int)result;
}

int sdcard_initialize (void) {
  syscall_return_t cval = command(DRIVER_NUM_SDCARD, 2, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int sdcard_initialize_sync (uint32_t* block_size, uint32_t* size_in_kB) {
  int err;
  sdcard_data_t result;
  result.fired = false;
  result.error = RETURNCODE_SUCCESS;

  err = sdcard_set_callback(sdcard_upcall, (void*) &result);
  if (err < 0) return err;

  err = sdcard_initialize();
  if (err < 0) return err;

  // wait for callback
  yield_for(&result.fired);

  // copy args
  if (block_size != NULL) {
    *block_size = result.block_size;
  }
  if (size_in_kB != NULL) {
    *size_in_kB = result.size_in_kB;
  }

  return result.error;
}

int sdcard_read_block (uint32_t sector) {
  syscall_return_t cval = command(DRIVER_NUM_SDCARD, 3, sector, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int sdcard_read_block_sync (uint32_t sector) {
  int err;
  sdcard_data_t result;
  result.fired = false;
  result.error = RETURNCODE_SUCCESS;

  err = sdcard_set_callback(sdcard_upcall, (void*) &result);
  if (err < 0) return err;

  err = sdcard_read_block(sector);
  if (err < 0) return err;

  // wait for callback
  yield_for(&result.fired);

  return result.error;
}

int sdcard_write_block (uint32_t sector) {
  syscall_return_t cval = command(DRIVER_NUM_SDCARD, 4, sector, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int sdcard_write_block_sync (uint32_t sector) {
  int err;
  sdcard_data_t result;
  result.fired = false;
  result.error = RETURNCODE_SUCCESS;

  err = sdcard_set_callback(sdcard_upcall, (void*) &result);
  if (err < 0) return err;

  err = sdcard_write_block(sector);
  if (err < 0) return err;

  // wait for callback
  yield_for(&result.fired);

  return result.error;
}
