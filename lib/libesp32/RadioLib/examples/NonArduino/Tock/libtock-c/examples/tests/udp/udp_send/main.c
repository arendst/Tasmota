#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <ambient_light.h>
#include <button.h>
#include <humidity.h>
#include <rng.h>
#include <temperature.h>
#include <timer.h>

#include <udp.h>

#define DEBUG 1

static unsigned char BUF_BIND_CFG[2 * sizeof(sock_addr_t)];

void print_ipv6(ipv6_addr_t *);
int serialize_to_json(char* packet, int len, uint32_t rand, int temp, int humi, int lux);

int main(void) {

  printf("[UDP] Starting UDP_Send Test App.\n");

  static char packet[70];

  ipv6_addr_t ifaces[10];
  udp_list_ifaces(ifaces, 10);

  sock_handle_t handle;
  sock_addr_t addr = {
    ifaces[0],
    11111
  };

  printf("Opening socket on ");
  print_ipv6(&ifaces[0]);
  printf(" : %d\n", addr.port);
  int bind_return = udp_bind(&handle, &addr, BUF_BIND_CFG);

  if (bind_return < 0) {
    printf("Bind failed. Error code: %d\n", bind_return);
    return -1;
  }

  // Set the below address to be the IP address of your receiver
  // The current address corresponds to the default src address
  // set in the corresponding udp_rx app
  ipv6_addr_t dest_addr = {
    {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b,
      0x1c, 0x1d, 0x1e, 0x1f}
  };
  sock_addr_t destination = {
    dest_addr,
    16123
  };

  int count = 0;

  while (1) {
    int len = snprintf(packet, sizeof(packet), "Hello World (Packet #: %d)\n",
                       count);
    if (DEBUG) {
      printf("Sending packet (length %d) --> ", len);
      print_ipv6(&(destination.addr));
      printf(" : %d\n", destination.port);
    }
    ssize_t result = udp_send_to(packet, len, &destination);

    switch (result) {
      case RETURNCODE_SUCCESS:
        if (DEBUG) {
          printf("Packet sent.\n\n");
        }
        break;
      default:
        printf("Error sending packet %d\n\n", result);
    }
    count++;
    delay_ms(5000);
  }
}

void print_ipv6(ipv6_addr_t *ipv6_addr) {
  for (int j = 0; j < 14; j += 2) {
    printf("%02x%02x:", ipv6_addr->addr[j], ipv6_addr->addr[j + 1]);
  }
  printf("%02x%02x", ipv6_addr->addr[14], ipv6_addr->addr[15]);
}
