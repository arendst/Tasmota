#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include <analog_comparator.h>
#include <timer.h>
#include <tock.h>
static int callback_channel;

static void analog_comparator_comparison_polling(uint8_t channel) {
  uint count = 0;
  while (1) {
    count++;
    bool result;
    analog_comparator_comparison(channel, &result);
    printf("Try %d. Result = %d.\n", count, result);
    if (result == 1) {
      printf("This means Vinp > Vinn!\n\n");
    } else {
      printf("This means Vinp < Vinn!\n\n");
    }
    delay_ms(1000);
  }
}

// Callback for AC interrupts. Channel on which the interrupt is generated is
// passed through here.
static void analog_comparator_callback (int                          arg0,
                                        __attribute__ ((unused)) int arg1,
                                        __attribute__ ((unused)) int arg2,
                                        void*                        userdata) {
  callback_channel   = arg0;
  *((bool*)userdata) = 1;
}

static void analog_comparator_comparison_interrupt(uint8_t channel) {
  // Enable AC interrupts on the desired channel (i.e. two pins)
  analog_comparator_start_comparing(channel);

  static bool resume = 0;
  // Set callback for AC interrupts
  analog_comparator_interrupt_callback(analog_comparator_callback, &resume);

  while (1) {
    yield_for(&resume);
    resume = 0;
    printf("Interrupt received on channel %d, Vinp > Vinn!\n", callback_channel);
  }
}

int main(void) {
  printf("\nAnalog Comparator test application\n");

  if (!analog_comparator_exists()) {
    printf("Analog Comparator driver does not exist\n");
    exit(1);
  }

  int count;
  analog_comparator_count(&count);
  printf("Analog Comparator driver exists with %d channels\n", count);

  // Set mode according to which implementation you want.
  // mode = 0 --> polling comparison
  // mode = 1 --> interrupt-based comparison
  uint8_t mode = 1;

  // Choose a comparator channel, starting from index 0 and depending on the chip
  // A channel means an analog comparator, that is two pins.
  uint8_t channel = 1;

  // Since channel starts at index 0 and analog_comparator_count starts from 1,
  // a channel equal to count is already non-existing
  if (channel >= count) {
    printf("Specified channel does not exist on this board\n");
    exit(1);
  }

  switch (mode) {
    // Poll for a normal comparison every second and print the result
    case 0: analog_comparator_comparison_polling(channel);
      break;

    // Print for every interrupt received
    case 1: analog_comparator_comparison_interrupt(channel);
      break;
  }
  printf("\n");
  return 0;
}
