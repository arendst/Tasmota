#include <stdio.h>

#include <console.h>
#include <gpio.h>
#include <tock.h>

static void gpio_cb (__attribute__ ((unused)) int   pin_num,
                     __attribute__ ((unused)) int   arg2,
                     __attribute__ ((unused)) int   arg3,
                     __attribute__ ((unused)) void* userdata) {}

int main(void) {
  printf("[GPIO Interrupt Test]\n");
  printf("Jump GPIO pin 0 high to test.\n");

  // set callback for GPIO interrupts
  gpio_interrupt_callback(gpio_cb, NULL);

  // set GPIO 0 as input and enable interrupts on it
  gpio_enable_input(0, PullDown);
  gpio_enable_interrupt(0, Change);

  while (1) {
    yield();
    printf("\tGPIO Interrupt! (App: %p)\n", tock_app_flash_begins_at());
  }

  return 0;
}
