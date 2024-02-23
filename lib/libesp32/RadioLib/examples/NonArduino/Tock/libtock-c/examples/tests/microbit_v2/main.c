// Micro:bit v2 testing application.
// This app is modeled after the Hail test app by bradjc.

#include <stdio.h>

#include <adc.h>
#include <alarm.h>
#include <ble.h>
#include <button.h>
#include <buzzer.h>
#include <gap.h>
#include <gpio.h>
#include <led.h>
#include <ninedof.h>
#include <sound_pressure.h>
#include <temperature.h>
#include <timer.h>
#include <tock.h>


static int check_err(int retval, const char* func_name) {
  if (retval < RETURNCODE_SUCCESS) {
    printf("Function: %s returned error: %s\n", func_name, tock_strrcode(retval));
    tock_exit(1);
  }
  return retval;
}


static bool tone_complete = true;
static void tone_callback(void) {
  // allow additional notes to be played
  tone_complete = true;
}

static void light_leds(int leds[], int len) {
  for (int i = 0; i < len; i++) {
    check_err(led_on(leds[i]), "led_on");
  }
}

static void button_callback(int                            btn_num,
                            int                            button_pressed,
                            __attribute__ ((unused)) int   arg2,
                            __attribute__ ((unused)) void *ud) {

  // Do not run if this was a button release or if tone is not finished
  if (!button_pressed || !tone_complete) {
    if (!tone_complete) {
      printf("Please wait for the tone to stop\n");
    }
    return;
  }

  // Play a note and make an LED pattern appear
  tone_complete = false;
  if (btn_num == 0) {
    // When playing notes, note that the minimum frequency the Microbit can play is 489 Hz.
    check_err(tone(NOTE_C5, 500, tone_callback), "tone");
    int leds[10] = {2, 6, 8, 11, 12, 13, 16, 18, 21, 23};
    light_leds(leds, 10);
  } else if (btn_num == 1) {
    check_err(tone(NOTE_E5, 500, tone_callback), "tone");
    int leds[11] = {1, 2, 6, 8, 11, 12, 13, 16, 18, 21, 22};
    light_leds(leds, 11);
  }
}

static void sample_sensors(void) {
  check_err(led_toggle(0), "led_toggle");  // Blink top left LED
  for (int i = 1; i < 25; i++) {
    check_err(led_off(i), "led_off");
  }

  // Sensors: temperature, acceleration, sound pressure
  int temp = 0;
  check_err(temperature_read_sync(&temp), "temperature_read_sync");
  uint32_t accel_mag = check_err(ninedof_read_accel_mag(), "ninedof_read_accel_mag");
  int x = 0;
  int y = 0;
  int z = 0;
  check_err(ninedof_read_magnetometer_sync(&x, &y, &z), "ninedof_read_magnetometer_sync");
  unsigned char sound_pres;
  check_err(sound_pressure_read_sync(&sound_pres), "sound_pressure_read_sync");

  // Analog inputs: P0-P2
  uint16_t val = 0;
  check_err(adc_sample_sync(0, &val), "adc_sample_sync");
  int p0 = (val * 3300) / (4095 << 4);
  check_err(adc_sample_sync(1, &val), "adc_sample_sync");
  int p1 = (val * 3300) / (4095 << 4);
  check_err(adc_sample_sync(2, &val), "adc_sample_sync");
  int p2 = (val * 3300) / (4095 << 4);
  if (p0 < 10 || p1 < 10 || p2 < 10) { // pin is connected to gnd
    check_err(led_on(20), "led_on");
  }

  // Digital inputs: P8, P9, P16
  int p8 = 0;
  check_err(gpio_read(8, &p8), "gpio_read");
  int p9 = 0;
  check_err(gpio_read(9, &p9), "gpio_read");
  int p16 = 0;
  check_err(gpio_read(16, &p16), "gpio_read");

  // print results
  printf("[Micro:bit Sensor Reading]\n");
  // Temp is given in hundredths of a degree C, in format XX00
  printf("  Temperature: reading: %d.0 degrees C\n", temp / 100);
  printf("  Acceleration: %lu\n", accel_mag);
  printf("  Magnetometer: X: %d, Y: %d, Z: %d\n", x, y, z);
  printf("  Ambient Sound: %i\n", sound_pres);
  printf("ADC:\n");
  printf("  P0:           %d mV\n", p0);
  printf("  P1:           %d mV\n", p1);
  printf("  P2:           %d mV\n", p2);
  printf("Digital:\n");
  printf("  P8:           %d\n", p8);
  printf("  P9:           %d\n", p9);
  printf("  P16:          %d\n", p16);
  printf("\n");
}

int main(void) {
  printf("[Micro:bit] Test App!\n");
  printf("[Micro:bit] Samples all sensors.\n");
  printf("[Micro:bit] Transmits name over BLE.\n");
  printf("[Micro:bit] Button controls LED.\n");

  // Setup BLE
  static uint8_t adv_data_buf[ADV_DATA_MAX_SIZE];
  AdvData_t adv_data = gap_adv_data_new(adv_data_buf, sizeof(adv_data_buf));
  uint16_t advertising_interval_ms = 1000;
  uint8_t device_name[] = "Micro_bit:v2";
  check_err(gap_add_device_name(&adv_data, device_name, sizeof(device_name) - 1), "gap_add_device_name");
  check_err(ble_start_advertising(ADV_NON_CONN_IND, adv_data.buf, adv_data.offset, advertising_interval_ms),
   "ble_start_advertising");
  printf("Now advertising every %d ms as '%s'\n", advertising_interval_ms, device_name);

  // Enable button callbacks
  check_err(button_subscribe(button_callback, NULL), "button_subscribe");
  check_err(button_enable_interrupt(0), "button_enable_interrupt");
  check_err(button_enable_interrupt(1), "button_enable_interrupt");
  printf("Set up button callbacks!\n");

  // Enable sound pressure sensor
  check_err(sound_pressure_enable(), "sound_pressure_enable");
  printf("Enabled sound pressure!\n");

  // Setup P8, P9, P16
  check_err(gpio_enable_input(8, PullDown), "gpio_enable_input"); // P8
  check_err(gpio_enable_input(9, PullDown), "gpio_enable_input"); // P9
  check_err(gpio_enable_input(16, PullDown), "gpio_enable_input"); // P16
  printf("Set up gpio pins!\n");

  printf("Setup complete!\n");
  // sample sensors every second
  while (1) {
    sample_sensors();
    delay_ms(1000);
  }
}
