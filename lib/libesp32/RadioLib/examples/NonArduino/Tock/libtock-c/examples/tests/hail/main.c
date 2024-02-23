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
#include <crc.h>
#include <gpio.h>
#include <humidity.h>
#include <led.h>
#include <ninedof.h>
#include <nrf51_serialization.h>
#include <rng.h>
#include <temperature.h>
#include <timer.h>


/////////////////////////////////////////////////////////////////////
// Software CRC implementation for validating CRC driver
//
// From http://home.thep.lu.se/~bjorn/crc/ (April 22, 2020)
static uint32_t crc32_for_byte(uint32_t r) {
  for (int j = 0; j < 8; ++j)
    r = (r & 1 ? 0 : (uint32_t)0xEDB88320L) ^ r >> 1;
  return r ^ (uint32_t)0xFF000000L;
}

static void reference_crc32(const void *data, size_t n_bytes, uint32_t* crc) {
  static uint32_t table[0x100];
  if (!*table)
    for (size_t i = 0; i < 0x100; ++i)
      table[i] = crc32_for_byte(i);
  for (size_t i = 0; i < n_bytes; ++i)
    *crc = table[(uint8_t)*crc ^ ((uint8_t*)data)[i]] ^ *crc >> 8;
}
/////////////////////////////////////////////////////////////////////



// Intervals for BLE advertising and connections
simple_ble_config_t ble_config = {
  .platform_id       = 0x13,                // used as 4th octect in device BLE address
  .device_id         = DEVICE_ID_DEFAULT,
  .adv_name          = (char*)"Hail",
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
    led_on(1); // green
  } else {
    led_off(1);
  }
}

// Callback for gpio interrupts.
//   - pin_num: The index of the pin associated with the callback.
//   - pin_state: 1 if high, 0 if low.
static void gpio_callback(  int                            pin_num,
                            int                            pin_state,
                            __attribute__ ((unused)) int   arg2,
                            __attribute__ ((unused)) void *ud) {
  printf("GPIO Interrupt: pin: %i, state: %i\n", pin_num, pin_state);
}

static void sample_sensors (void) {

  // Sensors: temperature/humidity, acceleration, light
  int temp;
  temperature_read_sync(&temp);
  unsigned humi;
  humidity_read_sync(&humi);
  uint32_t accel_mag = ninedof_read_accel_mag();
  int light;
  ambient_light_read_intensity_sync(&light);

  // Analog inputs: A0-A5
  uint16_t val;
  adc_sample_sync(0, &val);
  int a0 = (val * 3300) / (4095 << 4);
  adc_sample_sync(1, &val);
  int a1 = (val * 3300) / (4095 << 4);
  adc_sample_sync(2, &val);
  int a2 = (val * 3300) / (4095 << 4);
  adc_sample_sync(3, &val);
  int a3 = (val * 3300) / (4095 << 4);
  adc_sample_sync(4, &val);
  int a4 = (val * 3300) / (4095 << 4);
  adc_sample_sync(5, &val);
  int a5 = (val * 3300) / (4095 << 4);

  // Digital inputs: D0, D1, D6, D7
  int d0;
  gpio_read(0, &d0);
  int d1;
  gpio_read(1, &d1);
  int d6;
  gpio_read(2, &d6);
  int d7;
  gpio_read(3, &d7);

  // Random bytes
  uint8_t rand[5];
  int count;
  rng_sync(rand, 5, 5, &count);

  // CRC of the random bytes
  uint32_t crc;
  crc_compute(rand, 5, CRC_32, &crc);
  uint32_t reference_crc = 0;
  reference_crc32(rand, 5, &reference_crc);

  // print results
  printf("[Hail Sensor Reading]\n");
  printf("  Temperature:  %d 1/100 degrees C\n", temp);
  printf("  Humidity:     %u 0.01%%\n", humi);
  printf("  Light:        %d\n", light);
  printf("  Acceleration: %lu\n", accel_mag);
  printf("  A0:           %d mV\n", a0);
  printf("  A1:           %d mV\n", a1);
  printf("  A2:           %d mV\n", a2);
  printf("  A3:           %d mV\n", a3);
  printf("  A4:           %d mV\n", a4);
  printf("  A5:           %d mV\n", a5);
  printf("  D0:           %d\n", d0);
  printf("  D1:           %d\n", d1);
  printf("  D6:           %d\n", d6);
  printf("  D7:           %d\n", d7);
  printf("  Random:       %#04x %#04x %#04x %#04x %#04x\n", rand[0], rand[1], rand[2], rand[3], rand[4]);
  printf("  CRC:          %#010lx (%s reference implementation)\n", crc,
         (crc == reference_crc) ? "Matches" : "!! Does not match");
  printf("\n");

  // toggle the blue LED
  led_toggle(2);
}

int main(void) {
  printf("[Hail] Test App!\n");
  printf("[Hail] Samples all sensors.\n");
  printf("[Hail] Transmits name over BLE.\n");
  printf("[Hail] Button controls LED.\n");

  // Setup BLE
  simple_ble_init(&ble_config);
  simple_adv_only_name();

  // Enable button callbacks
  button_subscribe(button_callback, NULL);
  button_enable_interrupt(0);

  // Setup D0, D1, D6, D7
  gpio_enable_input(0, PullDown); // D0
  gpio_enable_input(1, PullDown); // D1
  gpio_enable_input(2, PullDown); // D6
  gpio_enable_input(3, PullDown); // D7

  // Enable interrupts on D7
  gpio_interrupt_callback(gpio_callback, NULL);
  gpio_enable_interrupt(3, Change);

  // sample sensors every second
  while (1) {
    sample_sensors();
    delay_ms(1000);
  }
}
