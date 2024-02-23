#include <stdio.h>

#include <gpio.h>
#include <gpio_async.h>
#include <timer.h>

int interrupt_count = 0;

static void gpio_async_cb(__attribute__ ((unused)) int   callback_type,
                          __attribute__ ((unused)) int   value,
                          __attribute__ ((unused)) int   unused,
                          __attribute__ ((unused)) void* ud) {
  printf("INTERRUPT\n");
  interrupt_count++;

  if (interrupt_count > 5) {
    printf("Now disabling interrupt\n");
    gpio_async_disable_interrupt(0, 1);
  }
}

int main (void) {
  printf("GPIO Async Test App\n");

  printf("Enabling rising edge interrupt on port 0 pin 1\n");
  gpio_async_interrupt_callback(gpio_async_cb, NULL);
  gpio_async_make_input_sync(0, 1, PullNone);
  gpio_async_enable_interrupt(0, 1, RisingEdge);

  printf("Toggling port 0 pin 0\n");
  gpio_async_make_output_sync(0, 0);

  while (1) {
    gpio_async_set(0, 0);
    delay_ms(500);
    gpio_async_clear(0, 0);
    delay_ms(500);
  }

  return 0;
}
