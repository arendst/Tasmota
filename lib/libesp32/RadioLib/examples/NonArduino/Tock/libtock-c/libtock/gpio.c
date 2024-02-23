#include "gpio.h"

int gpio_count(int* count) {
  syscall_return_t rval = command(GPIO_DRIVER_NUM, 0, 0, 0);
  return tock_command_return_u32_to_returncode(rval, (uint32_t*) count);
}

int gpio_enable_output(GPIO_Pin_t pin) {
  syscall_return_t rval = command(GPIO_DRIVER_NUM, 1, pin, 0);
  return tock_command_return_novalue_to_returncode(rval);
}

int gpio_set(GPIO_Pin_t pin) {
  syscall_return_t rval = command(GPIO_DRIVER_NUM, 2, pin, 0);
  return tock_command_return_novalue_to_returncode(rval);
}

int gpio_clear(GPIO_Pin_t pin) {
  syscall_return_t rval = command(GPIO_DRIVER_NUM, 3, pin, 0);
  return tock_command_return_novalue_to_returncode(rval);
}

int gpio_toggle(GPIO_Pin_t pin) {
  syscall_return_t rval = command(GPIO_DRIVER_NUM, 4, pin, 0);
  return tock_command_return_novalue_to_returncode(rval);
}

int gpio_enable_input(GPIO_Pin_t pin, GPIO_InputMode_t pin_config) {
  syscall_return_t rval = command(GPIO_DRIVER_NUM, 5, pin, pin_config);
  return tock_command_return_novalue_to_returncode(rval);
}

int gpio_read(GPIO_Pin_t pin, int* pin_value) {
  syscall_return_t rval = command(GPIO_DRIVER_NUM, 6, pin, 0);
  return tock_command_return_u32_to_returncode(rval, (uint32_t*) pin_value);
}

int gpio_enable_interrupt(GPIO_Pin_t pin, GPIO_InterruptMode_t irq_config) {
  syscall_return_t rval = command(GPIO_DRIVER_NUM, 7, pin, irq_config);
  return tock_command_return_novalue_to_returncode(rval);
}

int gpio_disable_interrupt(GPIO_Pin_t pin) {
  syscall_return_t rval = command(GPIO_DRIVER_NUM, 8, pin, 0);
  return tock_command_return_novalue_to_returncode(rval);
}

int gpio_disable(GPIO_Pin_t pin) {
  syscall_return_t rval = command(GPIO_DRIVER_NUM, 9, pin, 0);
  return tock_command_return_novalue_to_returncode(rval);
}

int gpio_interrupt_callback(subscribe_upcall callback, void* callback_args) {
  subscribe_return_t sval = subscribe(GPIO_DRIVER_NUM, 0, callback, callback_args);
  return tock_subscribe_return_to_returncode(sval);
}
