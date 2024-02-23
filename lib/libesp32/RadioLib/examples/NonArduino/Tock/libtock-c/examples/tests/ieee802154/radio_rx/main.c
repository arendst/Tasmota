#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "ieee802154.h"
#include "led.h"
#include "timer.h"
#include "tock.h"

// IEEE 802.15.4 sample packet reception app.
// Continually receives frames at the specified short address. Setting the
// source short address does not cause the radio to filter incoming frames, but
// does cause it to respond to the transmitter with ACKs. Toggles LED whenever
// a frame is received.
//
// Optionally uses the packet inspection functions to print out relevant
// information from the frame. (Set PRINT_PAYLOAD to 1)

char packet_rx[IEEE802154_FRAME_LEN];

static void callback(__attribute__ ((unused)) int   pans,
                     __attribute__ ((unused)) int   dst_addr,
                     __attribute__ ((unused)) int   src_addr,
                     __attribute__ ((unused)) void* ud) {
  led_toggle(0);
  // Before accessing an "allowed" buffer, we must request it back:
  ieee802154_unallow_rx_buf();

#define PRINT_PAYLOAD 1
#define PRINT_STRING 1
#if PRINT_PAYLOAD
  int payload_offset = ieee802154_frame_get_payload_offset(packet_rx);
  int payload_length = ieee802154_frame_get_payload_length(packet_rx);
  printf("Received packet with payload of %d bytes from offset %d\n", payload_length, payload_offset);
#if PRINT_STRING
  printf("%.*s\n", payload_length, packet_rx + payload_offset);
#else
  for (i = 0; i < payload_length; i++) {
    printf("%02x%c", packet_rx[payload_offset + i],
           ((i + 1) % 16 == 0 || i + 1 == payload_length) ? '\n' : ' ');
  }
#endif // PRINT_STRING

  unsigned short pan;
  unsigned short short_addr;
  unsigned char long_addr[8];
  addr_mode_t mode;

  if (ieee802154_frame_get_dst_pan(packet_rx, &pan)) {
    printf("Packet destination PAN ID: 0x%04x\n", pan);
  } else {
    printf("Packet destination PAN ID: not present\n");
  }

  mode = ieee802154_frame_get_dst_addr(packet_rx, &short_addr, long_addr);
  if (mode == ADDR_NONE) {
    printf("Packet destination address: not present\n");
  } else if (mode == ADDR_SHORT) {
    printf("Packet destination address: 0x%04x\n", short_addr);
  } else if (mode == ADDR_LONG) {
    printf("Packet destination address:");
    for (int i = 0; i < 8; i++) {
      printf(" %02x", long_addr[i]);
    }
    printf("\n");
  }

  if (ieee802154_frame_get_src_pan(packet_rx, &pan)) {
    printf("Packet source PAN ID: 0x%04x\n", pan);
  } else {
    printf("Packet source PAN ID: not present\n");
  }

  mode = ieee802154_frame_get_src_addr(packet_rx, &short_addr, long_addr);
  if (mode == ADDR_NONE) {
    printf("Packet source address: not present\n");
  } else if (mode == ADDR_SHORT) {
    printf("Packet source address: 0x%04x\n", short_addr);
  } else if (mode == ADDR_LONG) {
    printf("Packet source address:");
    for (int i = 0; i < 8; i++) {
      printf(" %02x", long_addr[i]);
    }
    printf("\n");
  }
#endif

  ieee802154_receive(callback, packet_rx, IEEE802154_FRAME_LEN);
}

int main(void) {
  memset(packet_rx, 0, IEEE802154_FRAME_LEN);
  ieee802154_set_address(0x802);
  ieee802154_set_pan(0xABCD);
  ieee802154_config_commit();
  ieee802154_up();
  ieee802154_receive(callback, packet_rx, IEEE802154_FRAME_LEN);
  while (1) {
    delay_ms(4000);
  }
}
