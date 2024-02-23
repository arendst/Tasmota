#include "pca9544a.h"
#include "tock.h"

struct pca9544a_data {
  bool fired;
  int value;
};

static struct pca9544a_data result = { .fired = false };

// Internal callback for faking synchronous reads
static void pca9544a_upcall(__attribute__ ((unused)) int value,
                            __attribute__ ((unused)) int unused1,
                            __attribute__ ((unused)) int unused2,
                            void*                        ud) {
  struct pca9544a_data* data = (struct pca9544a_data*) ud;
  data->value = value;
  data->fired = true;
}


int pca9544a_set_callback(subscribe_upcall callback, void* callback_args) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_PCA9544A, 0, callback, callback_args);
  return tock_subscribe_return_to_returncode(sval);
}

int pca9544a_select_channels(uint32_t channels) {
  syscall_return_t com = command(DRIVER_NUM_PCA9544A, 1, channels, 0);
  return tock_command_return_novalue_to_returncode(com);
}

int pca9544a_disable_all_channels(void) {
  syscall_return_t com = command(DRIVER_NUM_PCA9544A, 2, 0, 0);
  return tock_command_return_novalue_to_returncode(com);
}

int pca9544a_read_interrupts(void) {
  syscall_return_t com = command(DRIVER_NUM_PCA9544A, 3, 0, 0);
  return tock_command_return_novalue_to_returncode(com);
}

int pca9544a_read_selected(void) {
  syscall_return_t com = command(DRIVER_NUM_PCA9544A, 4, 0, 0);
  return tock_command_return_novalue_to_returncode(com);
}



int pca9544a_select_channels_sync(uint32_t channels) {
  int err;
  result.fired = false;

  err = pca9544a_set_callback(pca9544a_upcall, (void*) &result);
  if (err < 0) return err;

  err = pca9544a_select_channels(channels);
  if (err < 0) return err;

  // Wait for the callback.
  yield_for(&result.fired);

  return RETURNCODE_SUCCESS;
}

int pca9544a_disable_all_channels_sync(void) {
  int err;
  result.fired = false;

  err = pca9544a_set_callback(pca9544a_upcall, (void*) &result);
  if (err < 0) return err;

  err = pca9544a_disable_all_channels();
  if (err < 0) return err;

  // Wait for the callback.
  yield_for(&result.fired);

  return RETURNCODE_SUCCESS;
}

int pca9544a_read_interrupts_sync(int* value) {
  int err;
  result.fired = false;

  err = pca9544a_set_callback(pca9544a_upcall, (void*) &result);
  if (err < 0) return err;

  err = pca9544a_read_interrupts();
  if (err < 0) return err;

  // Wait for the callback.
  yield_for(&result.fired);

  *value = result.value;

  return RETURNCODE_SUCCESS;
}

int pca9544a_read_selected_sync(int* value) {
  int err;
  result.fired = false;

  err = pca9544a_set_callback(pca9544a_upcall, (void*) &result);
  if (err < 0) return err;

  err = pca9544a_read_selected();
  if (err < 0) return err;

  // Wait for the callback.
  yield_for(&result.fired);

  *value = result.value;

  return RETURNCODE_SUCCESS;
}
