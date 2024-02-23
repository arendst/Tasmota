#include <stdbool.h>
#include <stdio.h>

#include "ieee802154.h"
#include "led.h"
#include "timer.h"

// IEEE 802.15.4 sample packet transmission/ack app.
// Continually transmits frames at the specified short address to the specified
// destination address. Blinks the LED only if the transmitted frame is also acked.

#define BUF_SIZE 60
char packet_tx[BUF_SIZE];
bool toggle = true;

int main(void) {
  int i;
  char counter = 0;
  for (i = 0; i < BUF_SIZE; i++) {
    packet_tx[i] = i;
  }
  ieee802154_set_address(0x1540);
  ieee802154_set_pan(0xABCD);
  ieee802154_config_commit();
  ieee802154_up();
  while (1) {
    int err = ieee802154_send(0x0802,
                              SEC_LEVEL_NONE,
                              0,
                              NULL,
                              packet_tx,
                              BUF_SIZE);
    if (err == RETURNCODE_SUCCESS) {
      led_toggle(0);
      printf("Packet sent and acked.\n");
    } else if (err == RETURNCODE_ENOACK) {
      printf("Packet sent, but not acked.\n");
    } else {
      printf("Failed to send packet.\n");
    }
    counter++;
    packet_tx[0] = counter;
    delay_ms(4000);
  }
}
