#include "led.h"

int led_count(int* count) {
  syscall_return_t rval = command(DRIVER_NUM_LEDS, 0, 0, 0);
  return tock_command_return_u32_to_returncode(rval, (uint32_t*) count);
}

int led_on(int led_num) {
  syscall_return_t rval = command(DRIVER_NUM_LEDS, 1, led_num, 0);
  return tock_command_return_novalue_to_returncode(rval);
}

int led_off(int led_num) {
  syscall_return_t rval = command(DRIVER_NUM_LEDS, 2, led_num, 0);
  return tock_command_return_novalue_to_returncode(rval);
}

int led_toggle(int led_num) {
  syscall_return_t rval = command(DRIVER_NUM_LEDS, 3, led_num, 0);
  return tock_command_return_novalue_to_returncode(rval);
}
