#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <adc.h>
#include <console.h>
#include <timer.h>
#include <tock.h>


int main(void) {
  printf("[Tock] ADC Sample All Channels Test\n");

  // check if ADC driver exists
  if (!adc_is_present()) {
    printf("No ADC driver!\n");
    return -1;
  }

  int channel_count;
  adc_channel_count(&channel_count);
  printf("ADC driver exists with %d channels\n\n", channel_count);

  while (1) {
    printf("Single Samples\n");

    // iterate through the channels
    for (uint8_t channel = 0; channel < channel_count; channel++) {
      uint16_t sample;

      int err = adc_sample_sync(channel, &sample);
      if (err != 0) {
        printf("ERROR READING ADC VALUE: %i\n", err);
      }
      int millivolts = (sample * 3300) / 4095;
      printf("Channel %u: %d mV (raw: 0x%04x)\n", channel, millivolts, sample);
    }

    printf("\n");
    delay_ms(1000);
  }

  return 0;
}
