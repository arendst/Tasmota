#include "ambient_light.h"
#include "tock.h"

typedef struct {
  int intensity;
  bool fired;
} ambient_light_data_t;

// callback for synchronous reads
static void ambient_light_upcall(int intensity,
                                 __attribute__ ((unused)) int unused1,
                                 __attribute__ ((unused)) int unused2, void* ud) {
  ambient_light_data_t* result = (ambient_light_data_t*)ud;
  result->intensity = intensity;
  result->fired     = true;
}

int ambient_light_read_intensity_sync(int* lux_value) {
  int err;
  ambient_light_data_t result = {0};
  result.fired = false;

  err = ambient_light_subscribe(ambient_light_upcall, (void*)(&result));
  if (err < RETURNCODE_SUCCESS) {
    return err;
  }

  err = ambient_light_start_intensity_reading();
  if (err < RETURNCODE_SUCCESS) {
    return err;
  }

  yield_for(&result.fired);

  *lux_value = result.intensity;

  return RETURNCODE_SUCCESS;
}

int ambient_light_subscribe(subscribe_upcall callback, void* userdata) {
  subscribe_return_t ret = subscribe(DRIVER_NUM_AMBIENT_LIGHT, 0, callback, userdata);
  return tock_subscribe_return_to_returncode(ret);
}

int ambient_light_start_intensity_reading(void) {
  syscall_return_t ret = command(DRIVER_NUM_AMBIENT_LIGHT, 1, 0, 0);
  return tock_command_return_novalue_to_returncode(ret);
}
