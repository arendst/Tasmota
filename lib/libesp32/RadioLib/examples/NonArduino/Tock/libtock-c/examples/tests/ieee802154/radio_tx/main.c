#include <stdbool.h>
#include <stdio.h>

#include "gpio.h"
#include "ieee802154.h"
#include "led.h"
#include "timer.h"
#include "tock.h"

// IEEE 802.15.4 sample packet transmission app.
// Continually transmits frames at the specified short address to the specified
// destination address.

#define BUF_SIZE 60
char packet[BUF_SIZE];
bool toggle = true;

int main(void) {
  int i;
  for (i = 0; i < BUF_SIZE; i++) {
    packet[i] = i;
  }
  gpio_enable_output(0);
  ieee802154_set_address(0x1540);
  ieee802154_set_pan(0xABCD);
  ieee802154_config_commit();
  ieee802154_up();
  while (1) {
    led_toggle(0);
    int err = ieee802154_send(0x0802,
                              SEC_LEVEL_NONE,
                              0,
                              NULL,
                              packet,
                              BUF_SIZE);
    if (err == RETURNCODE_SUCCESS) {
      printf("Transmitted successfully.\n");
    } else if (err == RETURNCODE_ENOACK) {
      printf("Transmitted but packet not acknowledged.\n");
      gpio_toggle(0);
    } else {
      printf("Transmit failed with error %i.\n", err);
      gpio_toggle(0);
    }
    delay_ms(250);
  }
}
