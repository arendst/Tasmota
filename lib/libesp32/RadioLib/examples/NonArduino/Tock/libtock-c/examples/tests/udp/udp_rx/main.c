#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "led.h"
#include "timer.h"
#include "tock.h"

#include <ieee802154.h>
#include <udp.h>

/*
 * UDP sample packet reception app.
 * Receives packets at the specified address and port for 30 seconds,
 * then closes the socket.
 * Each time a packet is received, the user LED blinks
 */

#define MAX_RX_PACKET_LEN 200

char packet_rx[MAX_RX_PACKET_LEN];
static unsigned char BUF_BIND_CFG[2 * sizeof(sock_addr_t)];
sock_handle_t* handle;

void print_ipv6(ipv6_addr_t *);

void print_ipv6(ipv6_addr_t *ipv6_addr) {
  for (int j = 0; j < 14; j += 2) {
    printf("%02x%02x:", ipv6_addr->addr[j], ipv6_addr->addr[j + 1]);
  }
  printf("%02x%02x", ipv6_addr->addr[14], ipv6_addr->addr[15]);
}

static void callback(int                            payload_len,
                     __attribute__ ((unused)) int   arg2,
                     __attribute__ ((unused)) int   arg3,
                     __attribute__ ((unused)) void* ud) {
  led_toggle(0);

#define PRINT_STRING 1
#if PRINT_STRING
  printf("[UDP_RCV]: Rcvd UDP Packet from: ");
  print_ipv6((ipv6_addr_t*)&BUF_BIND_CFG);
  printf(" : %d\n", (uint16_t)(BUF_BIND_CFG[16]) + ((uint16_t)(BUF_BIND_CFG[17]) << 8));
  printf("Packet Payload: %.*s\n", payload_len, packet_rx);
#else
  for (i = 0; i < payload_len; i++) {
    printf("%02x%c", packet_rx[i],
           ((i + 1) % 16 == 0 || i + 1 == payload_len) ? '\n' : ' ');
  }
#endif // PRINT_STRING
}

int main(void) {

  ipv6_addr_t ifaces[10];
  udp_list_ifaces(ifaces, 10);

  sock_addr_t addr = {
    ifaces[1],
    16123
  };

  printf("Opening socket on ");
  print_ipv6(&ifaces[1]);
  printf(" : %d, and binding to that socket.\n", addr.port);
  sock_handle_t h;
  handle = &h;
  int ret = udp_bind(handle, &addr, BUF_BIND_CFG);
  if (ret < 0) {
    printf("Error in bind: %d\n", ret);
  }
  ;

  if (ieee802154_driver_is_present()) {
    ieee802154_set_address(49138); // Corresponds to the dst mac addr set in kernel
    ieee802154_set_pan(0xABCD);
    ieee802154_config_commit();
    ieee802154_up();
  } else {
    printf("No 15.4 driver present, set mac address manually in kernel.\n");
  }

  memset(packet_rx, 0, MAX_RX_PACKET_LEN);
  ssize_t result = udp_recv(callback, packet_rx, MAX_RX_PACKET_LEN);

  switch (result) {
    case RETURNCODE_SUCCESS:
      printf("Succesfully bound to socket, listening for UDP packets\n\n");
      break;
    case RETURNCODE_EINVAL:
      printf("The address requested is not a local interface\n");
      break;
    case RETURNCODE_EBUSY:
      printf("Another userland app has already bound to this addr/port\n");
      break;
    case RETURNCODE_ERESERVE:
      printf("Receive Failure. Must bind to a port before calling receive\n");
      break;
    default:
      printf("Failed to bind to socket %d\n", result);
      break;
  }
  /* Tock keeps the app alive waiting for callbacks after
   * returning from main, so no need to busy wait
   * However, this app tests receiving for 10 seconds
   * then closing the connection, so we include a busy wait for that
   * reason. */
  delay_ms(30000);
  ssize_t err = udp_close(handle);
  if (err < 0) {
    printf("Error closing socket\n");
  } else {
    printf("Socket closed.\n");
  }

}
