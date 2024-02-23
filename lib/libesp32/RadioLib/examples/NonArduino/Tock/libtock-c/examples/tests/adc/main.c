#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <adc.h>
#include <console.h>
#include <timer.h>
#include <tock.h>

int reference_voltage;

// List of frequencies to sample at
const uint32_t FREQS[10] = {25, 100, 500, 1000, 5000, 10000, 44100, 100000, 150000, 175000};

static void test_single_samples(uint8_t channel) {
  uint16_t sample;

  int err = adc_sample_sync(channel, &sample);
  if (err < 0) {
    printf("Error sampling ADC: %d\n", err);

  } else {
    // All ADC results are left-aligned, resulting in a 16-bit resolution
    // millivolts = sample * reference_voltage_mv / resolution
    int millivolts = (sample * reference_voltage) / ((1 << 16) - 1);
    printf("ADC Reading: %d mV (raw: 0x%04x)\n", millivolts, sample);
  }
}

static void test_sampling_buffer(uint8_t channel, int index) {
  uint32_t length = 16;
  uint16_t buf[length];
  memset(buf, 0, length);

  printf("%lu ADC samples at %lu Hz\n", length, FREQS[index]);
  int err = adc_sample_buffer_sync(channel, FREQS[index], buf, length);
  if (err < 0) {
    printf("Error sampling ADC: %d\n", err);

  } else {
    printf("\t[ ");
    for (uint32_t i = 0; i < length; i++) {
      // convert to millivolts
      printf("%u ", (buf[i] * reference_voltage) / ((1 << 16) - 1));
    }
    printf("]\n");
  }
}

int main(void) {
  printf("[Tock] ADC Test\n");

  // check if ADC driver exists
  if (!adc_is_present()) {
    printf("No ADC driver!\n");
    return -1;
  }
  int count;
  adc_channel_count(&count);
  printf("ADC driver exists with %d channels\n", count);

  reference_voltage = adc_get_reference_voltage();
  if (reference_voltage > 0) {
    printf("ADC reference voltage %d.%dV\n", reference_voltage / 1000, reference_voltage % 1000);
  } else {
    reference_voltage = 3300;
    printf("ADC no reference voltage, assuming 3.3V\n");
  }

  int resolution = adc_get_resolution_bits();
  printf("ADC resolution %d bits\n", resolution);

  while (1) {
    // iterate through the channels
    for (uint8_t channel = 0; channel < count; channel++) {

      printf("\nSingle Samples - Channel %u\n", channel);
      for (uint32_t i = 0; i < 10; i++) {
        test_single_samples(channel);
        delay_ms(100);
      }

      printf("\nBuffered Samples - Channel %u\n", channel);
      for (uint32_t i = 0; i < 10; i++) {
        test_sampling_buffer(channel, i);
        delay_ms(100);
      }
    }
  }

  return 0;
}
