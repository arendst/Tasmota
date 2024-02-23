#include <ipc.h>
#include <stdio.h>
#include <string.h>
#include <timer.h>

// Every 500 ms use the RNG service to randomly select an LED to turn on or
// off and then use the LED service to control that LED.

size_t _led_service = -1;
size_t _rng_service = -1;
char _led_buf[64] __attribute__((aligned(64)));
char _rng_buf[64] __attribute__((aligned(64)));

uint8_t _number_of_leds = 0;

bool _done = false;

// For this simple example, the callback only need set the yield variable.
static void ipc_callback(__attribute__ ((unused)) int   pid,
                         __attribute__ ((unused)) int   len,
                         __attribute__ ((unused)) int   arg2,
                         __attribute__ ((unused)) void* ud) {
  _done = true;
}

// Uses the LED service to get how many LEDs are on the board.
static uint8_t get_number_of_leds(void) {
  _done       = false;
  _led_buf[0] = 0;
  ipc_notify_service(_led_service);
  yield_for(&_done);

  return _led_buf[0];
}

// Set an LED with the LED service.
static void set_led(uint8_t led_index, uint8_t led_state) {
  // Turn the last LED on.
  _led_buf[0] = 1;         // Set LED state.
  _led_buf[1] = led_index; // Choose the LED.
  _led_buf[2] = led_state; // Set the LED.
  _done       = false;
  ipc_notify_service(_led_service);
  yield_for(&_done);
}

// Use the RNG service to get two random bytes.
static uint16_t get_two_random_bytes(void) {
  _done       = false;
  _rng_buf[0] = 2;
  ipc_notify_service(_rng_service);
  yield_for(&_done);

  return (_rng_buf[0] << 8) | _rng_buf[1];
}

int main(void) {
  int ret;

  // Retrieve a handle to the LED service.
  ret = ipc_discover("org.tockos.tutorials.ipc.led", &_led_service);
  if (ret != RETURNCODE_SUCCESS) {
    printf("No led service\n");
    return -1;
  }

  // Setup IPC for LED service
  ipc_register_client_callback(_led_service, ipc_callback, NULL);
  ipc_share(_led_service, _led_buf, 64);

  // Retrieve a handle to the RNG service.
  ret = ipc_discover("org.tockos.tutorials.ipc.rng", &_rng_service);
  if (ret != RETURNCODE_SUCCESS) {
    printf("No rng service\n");
    return -1;
  }

  // Setup IPC for RNG service
  ipc_register_client_callback(_rng_service, ipc_callback, NULL);
  ipc_share(_rng_service, _rng_buf, 64);

  // First need to get the number of LEDs.
  _number_of_leds = get_number_of_leds();
  printf("Number of LEDs: %d\n", _number_of_leds);

  // Then randomly turn on and off LEDs using the two services.
  while (1) {
    uint16_t random = get_two_random_bytes();

    uint8_t led_index = (random & 0xFF) % _number_of_leds;
    uint8_t led_state = (random >> 8) > 0x7F;
    set_led(led_index, led_state);

    delay_ms(500);
  }

  return 0;
}
