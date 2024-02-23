#include "temperature.h"
#include "tock.h"

struct data {
  bool fired;
  int temp;
};

static struct data result = { .fired = false };

// Internal upcall  for faking synchronous reads
static void temp_upcall(int                          temp,
                        __attribute__ ((unused)) int unused,
                        __attribute__ ((unused)) int unused1,
                        void*                        ud) {
  struct data* data = (struct data*) ud;
  data->temp  = temp;
  data->fired = true;
}

int temperature_set_callback(subscribe_upcall callback, void* callback_args) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_TEMPERATURE, 0, callback, callback_args);
  return tock_subscribe_return_to_returncode(sval);
}

int temperature_read(void) {
  syscall_return_t cval = command(DRIVER_NUM_TEMPERATURE, 1, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int temperature_read_sync(int* temperature) {
  int err;
  result.fired = false;

  err = temperature_set_callback(temp_upcall, (void*) &result);
  if (err < 0) return err;

  err = temperature_read();
  if (err < 0) return err;

  // Wait for the callback.
  yield_for(&result.fired);

  *temperature = result.temp;

  return RETURNCODE_SUCCESS;
}
