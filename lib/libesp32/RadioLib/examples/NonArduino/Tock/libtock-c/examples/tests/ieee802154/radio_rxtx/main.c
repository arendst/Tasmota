#include <stdbool.h>
#include <stdio.h>

#include "ieee802154.h"
#include "led.h"
#include "timer.h"

// IEEE 802.15.4 sample packet echo app.
// Continually receives packets at the specified short address, then retransmits them.

char packet_rx[IEEE802154_FRAME_LEN];
bool toggle = true;

int main(void) {
  unsigned short ADDR = 0x0802;
  unsigned short PAN  = 0xABCD;
  ieee802154_set_address(ADDR);
  ieee802154_set_pan(PAN);
  ieee802154_config_commit();
  ieee802154_up();
  while (1) {
    if (ieee802154_receive_sync(packet_rx, IEEE802154_FRAME_LEN) >= 0) {
      unsigned short pan;
      unsigned short short_addr;

      if (!ieee802154_frame_get_dst_pan(packet_rx, &pan) || pan != PAN) {
        continue;
      }

      addr_mode_t mode = ieee802154_frame_get_dst_addr(packet_rx, &short_addr, NULL);
      if (mode != ADDR_SHORT || short_addr != ADDR) {
        continue;
      }

      // Only retransmit the payload if this packet was meant for us.
      int payload_offset = ieee802154_frame_get_payload_offset(packet_rx);
      int payload_length = ieee802154_frame_get_payload_length(packet_rx);
      delay_ms(250);
      ieee802154_send(0xFFFF,
                      SEC_LEVEL_NONE,
                      0,
                      NULL,
                      packet_rx + payload_offset,
                      payload_length);
    }
    led_toggle(0);
  }
}
