#include <ipc.h>
#include <led.h>
#include <tock.h>

// This service can control the LEDs on a board.
//
// Interface
// =========
//
// The first byte of the shared buffer is the command byte.
// Command 0x00: Return the number of LEDs on the board. The first byte of the
//               buffer will be set with the number of LEDs.
// Command 0x01: Set an LED on or off. The second byte is the index of the LED
//               to set. The third byte sets it on or off. 0 means off and any
//               other value means on.

uint8_t _number_of_leds = 0;

static void ipc_callback(int pid, int len, int buf, __attribute__ ((unused)) void* ud) {
  uint8_t* buffer = (uint8_t*) buf;

  if (len < 1) {
    // Need at least one byte for the command.
    return;
  }

  uint8_t command = buffer[0];

  switch (command) {
    case 0:
      // Return the number of LEDs on the board
      buffer[0] = _number_of_leds;
      ipc_notify_client(pid);
      break;

    case 1:
      // Turn on/off a certain LED
      if (len < 3) {
        // Not enough here!
        return;
      }
      uint8_t led_id    = buffer[1];
      uint8_t led_state = buffer[2] > 0;

      if (led_state == 0) {
        led_off(led_id);
      } else {
        led_on(led_id);
      }
      ipc_notify_client(pid);
      break;
  }
}

int main(void) {
  // First get the number of LEDs setup on this board.
  led_count((int*) &_number_of_leds);

  ipc_register_service_callback("org.tockos.tutorials.ipc.led", ipc_callback,
                                NULL);
  return 0;
}
