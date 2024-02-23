#include "internal/alarm.h"

int alarm_internal_subscribe(subscribe_upcall cb, void *userdata) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_ALARM, 0, cb, userdata);
  return tock_subscribe_return_to_returncode(sval);
}

int alarm_internal_set(uint32_t reference, uint32_t tics) {
  syscall_return_t rval = command(DRIVER_NUM_ALARM, 6, reference, tics);
  uint32_t rc;
  return tock_command_return_u32_to_returncode(rval, &rc);
}

int alarm_internal_stop(void) {
  syscall_return_t rval = command(DRIVER_NUM_ALARM, 3, 0, 0);
  return tock_command_return_novalue_to_returncode(rval);
}

int alarm_internal_frequency(uint32_t* frequency) {
  syscall_return_t rval = command(DRIVER_NUM_ALARM, 1, 0, 0);
  return tock_command_return_u32_to_returncode(rval, frequency);
}

int alarm_internal_read(uint32_t* time) {
  syscall_return_t rval = command(DRIVER_NUM_ALARM, 2, 0, 0);
  return tock_command_return_u32_to_returncode(rval, time);
}
