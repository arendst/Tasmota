#include "sound_pressure.h"
#include "tock.h"

struct data {
  bool fired;
  unsigned char temp;
};

static struct data result = { .fired = false };

// Internal callback for faking synchronous reads
static void cb(int                          temp,
               __attribute__ ((unused)) int unused,
               __attribute__ ((unused)) int unused1,
               void*                        ud) {
  struct data* data = (struct data*) ud;
  data->temp  = (unsigned char)temp;
  data->fired = true;
}

int sound_pressure_set_callback(subscribe_upcall callback, void* callback_args) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_SOUND_PRESSURE, 0, callback, callback_args);
  return tock_subscribe_return_to_returncode(sval);
}

int sound_pressure_read(void) {
  syscall_return_t cval = command(DRIVER_NUM_SOUND_PRESSURE, 1, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

// enable sound pressure sensor
int sound_pressure_enable(void) {
  syscall_return_t cval = command(DRIVER_NUM_SOUND_PRESSURE, 2, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

// disable sound pressure sensor
int sound_pressure_disable(void) {
  syscall_return_t cval = command(DRIVER_NUM_SOUND_PRESSURE, 3, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int sound_pressure_read_sync(unsigned char* sound_pressure) {
  int err;
  result.fired = false;

  err = sound_pressure_set_callback(cb, (void*) &result);
  if (err < 0) return err;

  err = sound_pressure_read();
  if (err < 0) return err;

  // Wait for the callback.
  yield_for(&result.fired);

  *sound_pressure = result.temp;

  return RETURNCODE_SUCCESS;
}
