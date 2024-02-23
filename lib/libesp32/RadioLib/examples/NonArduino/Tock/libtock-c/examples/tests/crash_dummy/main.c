// Crash on button press.

#include <button.h>

volatile int* nullptr = 0;

static void button_callback(__attribute__ ((unused)) int   btn_num,
                            __attribute__ ((unused)) int   val,
                            __attribute__ ((unused)) int   arg2,
                            __attribute__ ((unused)) void *ud) {
  __attribute__ ((unused)) volatile int k = *nullptr;
}

int main(void) {
  button_subscribe(button_callback, NULL);
  button_enable_interrupt(0);

  return 0;
}
