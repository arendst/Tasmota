#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <led.h>
#include <proximity.h>
#include <timer.h>
#include <tock.h>

int main(void){

  printf("Proximity Sensor Test\n");

  // Check if driver/sensor is on the board
  if ( driver_exists(DRIVER_NUM_PROXIMITY) ) {
    printf("Driver exists\n");
  } else {
    printf("Driver does not exist\n");
    return -1;
  }

  // Check led count
  int num_leds;
  led_count(&num_leds);
  printf("Number of LEDs on the board: %d\n", num_leds);

  // Blink LED lights faster as proximity reading increases.
  // Main Loop starts once proximity reading is above a certain threshold (175)

  uint8_t frequency = 255;
  int period        = 1000;

  proximity_set_interrupt_thresholds(0, 175);
  proximity_read_on_interrupt_sync(&frequency);

  while (true) {

    for (int led = 0; led < num_leds; led++) {
      led_on(led);
      delay_ms(period / (frequency + 1));
      led_off(led);
    }

    if (proximity_read_sync(&frequency) < 0) {
      printf("Could not read proximity");
      return -1;
    }

  }
}