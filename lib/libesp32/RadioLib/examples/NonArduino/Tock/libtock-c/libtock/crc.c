#include "crc.h"
#include "tock.h"

int crc_exists(void) {
  return driver_exists(DRIVER_NUM_CRC);
}

int crc_request(enum crc_alg alg, size_t len) {
  syscall_return_t ret = command(DRIVER_NUM_CRC, 1, alg, (uint32_t)len);
  if (ret.type == TOCK_SYSCALL_SUCCESS) {
    return RETURNCODE_SUCCESS;
  } else {
    // printf("Failure on crc_request: %s\n", tock_strerr(ret.data[0]));
    return tock_status_to_returncode(ret.data[0]);
  }
}

int crc_subscribe(subscribe_upcall callback, void *ud) {
  subscribe_return_t ret = subscribe(DRIVER_NUM_CRC, 0, callback, ud);
  return tock_subscribe_return_to_returncode(ret);
}

int crc_set_buffer(const void* buf, size_t len) {
  allow_ro_return_t ret = allow_readonly(DRIVER_NUM_CRC, 0, (void*) buf, len);
  return tock_allow_ro_return_to_returncode(ret);
}

struct data {
  bool fired;
  int status;
  uint32_t result;
};

static void callback(int status, int v1, __attribute__((unused)) int v2, void *data) {
  struct data *d = data;
  d->fired  = true;
  d->status = status;
  d->result = v1;
}

int crc_compute(const void *buf, size_t buflen, enum crc_alg alg, uint32_t *result) {
  struct data d = { .fired = false };
  int ret;
  ret = crc_set_buffer(buf, buflen);
  if (ret < 0) return ret;
  ret = crc_subscribe(callback, (void *) &d);
  if (ret < 0) return ret;
  ret = crc_request(alg, buflen);
  if (ret < 0) return ret;
  yield_for(&d.fired);

  if (d.status == TOCK_STATUSCODE_SUCCESS) {
    *result = d.result;
  }

  return tock_status_to_returncode(d.status);
}
