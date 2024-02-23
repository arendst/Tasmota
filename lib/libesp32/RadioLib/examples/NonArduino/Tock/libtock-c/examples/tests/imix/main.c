#include "math.h"
#include <stdbool.h>
#include <stdio.h>

#include <ble_advdata.h>
#include <nordic_common.h>
#include <nrf_error.h>

#include <simple_adv.h>
#include <simple_ble.h>

#include <adc.h>
#include <ambient_light.h>
#include <button.h>
#include <gpio.h>
#include <humidity.h>
#include <ieee802154.h>
#include <led.h>
#include <ninedof.h>
#include <nrf51_serialization.h>
#include <temperature.h>
#include <timer.h>

// Intervals for BLE advertising and connections
simple_ble_config_t ble_config = {
  .platform_id       = 0x13,                // used as 4th octect in device BLE address
  .device_id         = DEVICE_ID_DEFAULT,
  .adv_name          = (char*)"imix",
  .adv_interval      = MSEC_TO_UNITS(1000, UNIT_0_625_MS),
  .min_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS),
  .max_conn_interval = MSEC_TO_UNITS(1250, UNIT_1_25_MS),
};

// Empty handler for setting BLE addresses
void ble_address_set (void) {
  // nop
}

// Callback for button presses.
//   btn_num: The index of the button associated with the callback
//   val: 1 if pressed, 0 if depressed
static void button_callback(__attribute__ ((unused)) int   btn_num,
                            int                            val,
                            __attribute__ ((unused)) int   arg2,
                            __attribute__ ((unused)) void *ud) {
  if (val == 1) {
    led_on(0);
  } else {
    led_off(0);
  }
}

static void sample_sensors (void) {
  printf("[imix Sensor Reading]\n");
  // Sensors: temperature/humidity, acceleration, light
  int temp;
  temperature_read_sync(&temp);
  printf("  Temperature:  %d.%02d degrees C\n", temp / 100, temp % 100);

  unsigned humi;
  humidity_read_sync(&humi);
  printf("  Humidity:     %u.%02u%%\n", humi / 100, humi % 100);

  uint32_t accel_mag = ninedof_read_accel_mag();
  printf("  Acceleration: %lu\n", accel_mag);

  int light;
  ambient_light_read_intensity_sync(&light);
  printf("  Light:        %d\n", light);

  // Analog inputs: A0-A5
  for (int a = 0; a < 6; a++) {
    uint16_t val;
    adc_sample_sync(a, &val);
    int ival = (val * 3300) / (4095 << 4);
    printf("  A%i:           %d mV\n", a, ival);
  }

  // Digital inputs: D0, D1, D6, D7
  for (int d = 0; d < 4; d++) {
    int val;
    gpio_read(0, &val);
    printf("  D%i:           %d\n", d, val);
  }
  printf("\n");

  // toggle the user LED
  led_toggle(1);
}

int main(void) {
  printf("[imix] Test App!\n");
  printf("[imix] Samples all sensors.\n");
  printf("[imix] Transmits name over BLE.\n");
  printf("[imix] Button controls LED.\n");

  // Setup BLE
  simple_ble_init(&ble_config);
  printf("[imix] BLE initialized.\n");
  simple_adv_only_name();
  printf("[imix] BLE advertising.\n");

  // Enable button callbacks
  button_subscribe(button_callback, NULL);
  button_enable_interrupt(0);
  printf("[imix] Button initialized.\n");

  // Setup D0, D1, D6, D7
  gpio_enable_input(0, PullDown); // D0
  gpio_enable_input(1, PullDown); // D1
  gpio_enable_input(2, PullDown); // D6
  gpio_enable_input(3, PullDown); // D7
  printf("[imix] GPIO D0, D1, D6, D7 configured to be pulldown.\n");

  // Should add UDP here

  // sample sensors every second
  while (1) {
    printf("[imix] Sampling sensors.\n");
    sample_sensors();
    delay_ms(1000);
  }
}
