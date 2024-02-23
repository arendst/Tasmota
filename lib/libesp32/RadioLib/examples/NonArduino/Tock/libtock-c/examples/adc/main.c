#include <adc.h>
#include <stdio.h>
#include <timer.h>

int main(void) {
  // Ask the kernel how many ADC channels are on this board.
  int num_adc;
  int err = adc_channel_count(&num_adc);
  if (err < 0) {
    printf("No ADC on this board.\n");
    return err;
  }

  printf("ADC Channels: %d\n", num_adc);

  while (true) {
    for (int channel = 0; channel < num_adc; channel++) {
      uint16_t value;
      err = adc_sample_sync(channel, &value);
      if (err == RETURNCODE_SUCCESS) {
        printf("Channel %d: %d\n", channel, value);
      } else {
        printf("Channel %d: error(%i) %s \n", channel, err, tock_strrcode(err));
      }
    }
    printf("\n");
    delay_ms(1000);
  }
}
