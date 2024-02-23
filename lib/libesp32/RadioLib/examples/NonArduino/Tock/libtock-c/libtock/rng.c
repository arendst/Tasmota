#include "tock.h"
#include <rng.h>
#include <stdlib.h>

struct rng_data {
  bool fired;
  int received;
};

// Global state for faking synchronous reads using a callback and
// yield
static struct rng_data result = { .fired = false, .received = 0 };

// Internal callback for faking synchronous reads
static void rng_upcall(__attribute__ ((unused)) int callback_type,
                       int                          received,
                       __attribute__ ((unused)) int val2,
                       void*                        ud) {
  struct rng_data* data = (struct rng_data*) ud;
  data->fired    = true;
  data->received = received;
}

int rng_set_buffer(uint8_t* buf, uint32_t len) {
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_RNG, 0, (void*) buf, len);
  return tock_allow_rw_return_to_returncode(aval);
}

int rng_set_callback(subscribe_upcall callback, void* callback_args) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_RNG, 0, callback, callback_args);
  return tock_subscribe_return_to_returncode(sval);
}

int rng_get_random(int num_bytes) {
  syscall_return_t com = command(DRIVER_NUM_RNG, 1, num_bytes, 0);
  return tock_command_return_novalue_to_returncode(com);
}

int rng_async(subscribe_upcall callback, uint8_t* buf, uint32_t len, uint32_t num) {
  int ret = rng_set_callback(callback, NULL);
  if (ret < 0) return ret;

  ret = rng_set_buffer(buf, len);
  if (ret < 0) return ret;

  return rng_get_random(num);
}

int rng_sync(uint8_t* buf, uint32_t len, uint32_t num, int* num_received) {
  int ret = rng_set_buffer(buf, len);
  if (ret < 0) return ret;

  ret = rng_set_callback(rng_upcall, (void*) &result);
  if (ret < 0) return ret;

  result.fired = false;
  ret = rng_get_random(num);
  if (ret < 0) return ret;

  yield_for(&result.fired);

  *num_received = result.received;

  return RETURNCODE_SUCCESS;
}
