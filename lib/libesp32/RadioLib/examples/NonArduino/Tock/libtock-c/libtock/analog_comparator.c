#include "analog_comparator.h"
#include "tock.h"

bool analog_comparator_exists(void) {
  return driver_exists(DRIVER_NUM_ANALOG_COMPARATOR);
}

int analog_comparator_count(int* count) {
  syscall_return_t com = command(DRIVER_NUM_ANALOG_COMPARATOR, 0, 0, 0);
  return tock_command_return_u32_to_returncode(com, (uint32_t*) count);
}

int analog_comparator_comparison(uint8_t channel, bool* comparison) {
  syscall_return_t com = command(DRIVER_NUM_ANALOG_COMPARATOR, 1, channel, 0);
  return tock_command_return_u32_to_returncode(com, (uint32_t*) comparison);
}

int analog_comparator_start_comparing(uint8_t channel) {
  syscall_return_t com = command(DRIVER_NUM_ANALOG_COMPARATOR, 2, channel, 0);
  return tock_command_return_novalue_to_returncode(com);
}

int analog_comparator_stop_comparing(uint8_t channel) {
  syscall_return_t com = command(DRIVER_NUM_ANALOG_COMPARATOR, 3, channel, 0);
  return tock_command_return_novalue_to_returncode(com);
}

int analog_comparator_interrupt_callback(subscribe_upcall callback, void* callback_args) {
  subscribe_return_t sub = subscribe(DRIVER_NUM_ANALOG_COMPARATOR, 0, callback, callback_args);
  return tock_subscribe_return_to_returncode(sub);
}
