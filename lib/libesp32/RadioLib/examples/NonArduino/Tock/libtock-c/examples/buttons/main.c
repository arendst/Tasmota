// \file
// This program waits for button presses on each of the buttons attached
// to a board and toggles the LED with the same index. For example, if the first
// button is pressed, the first LED is toggled. If the third button is pressed,
// the third LED is toggled.

#include <button.h>
#include <led.h>

// Callback for button presses.
//   btn_num: The index of the button associated with the callback
//   val: 1 if pressed, 0 if depressed
static void button_callback(int                            btn_num,
                            int                            val,
                            __attribute__ ((unused)) int   arg2,
                            __attribute__ ((unused)) void *ud) {
  if (val == 1) {
    led_toggle(btn_num);
  }
}

int main(void) {
  int err;

  err = button_subscribe(button_callback, NULL);
  if (err < 0) return err;

  // Enable interrupts on each button.
  int count;
  err = button_count(&count);
  if (err < 0) return err;

  for (int i = 0; i < count; i++) {
    button_enable_interrupt(i);
  }

  return 0;
}
