#include "dac.h"
#include "tock.h"

int dac_initialize(void) {
  syscall_return_t res = command(DRIVER_NUM_DAC, 1, 0, 0);
  return tock_command_return_novalue_to_returncode(res);
}

int dac_set_value(uint32_t value) {
  syscall_return_t res = command(DRIVER_NUM_DAC, 2, value, 0);
  return tock_command_return_novalue_to_returncode(res);
}
