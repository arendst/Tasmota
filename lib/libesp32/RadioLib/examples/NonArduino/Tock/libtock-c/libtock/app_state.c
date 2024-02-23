#include <string.h>

#include "app_state.h"
#include "tock.h"

// Internal callback for synchronous interfaces
static void app_state_sync_upcall(__attribute__ ((unused)) int callback_type,
                                  __attribute__ ((unused)) int value,
                                  __attribute__ ((unused)) int unused,
                                  void*                        ud) {
  *((bool*) ud) = true;
}


static bool _app_state_inited = false;
static int app_state_init(void) {
  allow_ro_return_t aret =
    allow_readonly(DRIVER_NUM_APP_FLASH, 0, _app_state_ram_pointer, _app_state_size);
  if (!aret.success) {
    return tock_status_to_returncode(aret.status);
  }

  // Check that we have a region to use for this.
  int number_regions = tock_app_number_writeable_flash_regions();
  if (number_regions == 0) return RETURNCODE_ENOMEM;

  // Get the pointer to flash which we need to ask the kernel where it is.
  _app_state_flash_pointer = tock_app_writeable_flash_region_begins_at(0);

  _app_state_inited = true;
  return 0;
}


int app_state_load_sync(void) {
  if (!_app_state_inited) {
    int err;
    err = app_state_init();
    if (err < 0) return err;
  }

  memcpy(_app_state_ram_pointer, _app_state_flash_pointer, _app_state_size);
  return 0;
}

int app_state_save(subscribe_upcall callback, void* callback_args) {
  if (!_app_state_inited) {
    int err = app_state_init();
    if (err < 0) return err;
  }

  subscribe_return_t sret =
    subscribe(DRIVER_NUM_APP_FLASH, 0, callback, callback_args);
  if (!sret.success) {
    return tock_status_to_returncode(sret.status);
  }

  syscall_return_t cret =
    command(DRIVER_NUM_APP_FLASH, 1, (uint32_t) _app_state_flash_pointer, 0);
  return tock_command_return_novalue_to_returncode(cret);
}


static bool save_sync_flag;
int app_state_save_sync(void) {
  int err;
  save_sync_flag = false;

  err = app_state_save(app_state_sync_upcall, (void*) &save_sync_flag);
  if (err < 0) return err;

  // Wait for the callback.
  yield_for(&save_sync_flag);

  return 0;
}
