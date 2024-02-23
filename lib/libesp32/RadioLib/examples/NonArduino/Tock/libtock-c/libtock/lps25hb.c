#include "lps25hb.h"
#include "tock.h"

struct lps25hb_data {
  bool fired;
  int value;
};

static struct lps25hb_data result = { .fired = false };

// Internal callback for faking synchronous reads
static void lps25hb_upcall(int                          value,
                           __attribute__ ((unused)) int unused1,
                           __attribute__ ((unused)) int unused2,
                           void*                        ud) {
  struct lps25hb_data* data = (struct lps25hb_data*) ud;
  data->value = value;
  data->fired = true;
}

int lps25hb_set_callback (subscribe_upcall callback, void* callback_args) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_LPS25HB, 0, callback, callback_args);
  return tock_subscribe_return_to_returncode(sval);
}

int lps25hb_get_pressure (void) {
  syscall_return_t com = command(DRIVER_NUM_LPS25HB, 1, 0, 0);
  return tock_command_return_novalue_to_returncode(com);
}

int lps25hb_get_pressure_sync (int* pressure) {
  int err;
  result.fired = false;

  err = lps25hb_set_callback(lps25hb_upcall, (void*) &result);
  if (err < 0) return err;

  err = lps25hb_get_pressure();
  if (err < 0) return err;

  // Wait for the callback.
  yield_for(&result.fired);

  *pressure = result.value;

  return RETURNCODE_SUCCESS;
}
