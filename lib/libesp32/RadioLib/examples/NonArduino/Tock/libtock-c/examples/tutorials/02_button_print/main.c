#include <stdio.h>

#include <button.h>

// Callback for button presses.
//   btn_num: The index of the button associated with the callback
//   val: 1 if pressed, 0 if depressed
static void button_callback(__attribute__ ((unused)) int   btn_num,
                            int                            val,
                            __attribute__ ((unused)) int   arg2,
                            __attribute__ ((unused)) void *ud) {
  // Only print on the down press.
  if (val == 1) {
    printf("Hello!\n");
  }
}

int main(void) {
  button_subscribe(button_callback, NULL);

  // Ensure there is a button to use.
  int count;
  button_count(&count);
  if (count < 1) {
    // There are no buttons on this platform.
    printf("Error! No buttons on this platform.\n");
  } else {
    // Enable an interrupt on the first button.
    button_enable_interrupt(0);
  }

  // Can just return here. The application will continue to execute.
  return 0;
}
