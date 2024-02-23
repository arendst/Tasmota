#include <stdbool.h>
#include <stdio.h>

#include <button.h>

#include <ieee802154.h>
#include <udp.h>

#define DEBUG 0

static unsigned char BUF_BIND_CFG[2 * sizeof(sock_addr_t)];
static int button_press;

void print_ipv6(ipv6_addr_t *);

void print_ipv6(ipv6_addr_t *ipv6_addr) {
  for (int j = 0; j < 14; j += 2) {
    printf("%02x%02x:", ipv6_addr->addr[j], ipv6_addr->addr[j + 1]);
  }
  printf("%02x%02x", ipv6_addr->addr[14], ipv6_addr->addr[15]);
}

int main(void) {

  char packet[64];
  button_press = 0;

  ieee802154_set_pan(0xABCD);
  ieee802154_config_commit();
  ieee802154_up();

  ipv6_addr_t ifaces[10];
  udp_list_ifaces(ifaces, 10);

  sock_handle_t handle;
  sock_addr_t addr = {
    ifaces[2],
    15123
  };

  printf("Opening socket on ");
  print_ipv6(&ifaces[2]);
  printf(" : %d\n", addr.port);
  int bind_return = udp_bind(&handle, &addr, BUF_BIND_CFG);
  if (bind_return < 0) {
    printf("Bind failed. Error code: %d\n", bind_return);
    return -1;
  }

  // Set the below address to IP address of receiver
  ipv6_addr_t dest_addr = {
    {0xfe, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xfe, 0, 0xdf, 0xf0}
  };
  sock_addr_t destination = {
    dest_addr,
    16123
  };

  int count = 0;
  while (1) {
    // wait for gpio pin to be pressed
    while (button_press == 0) {
      button_read(0, &button_press);
    }
    count++;

    int len = snprintf(packet, sizeof(packet), "{\"buttons\": %d}", count);
    if (DEBUG) {
      printf("Button press detected\n");

      printf("Sending packet (length %d) --> ", len);
      print_ipv6(&(destination.addr));
      printf(" : %d\n", destination.port);
    }
    ssize_t result = udp_send_to(packet, len, &destination);

    switch (result) {
      case RETURNCODE_SUCCESS:
        if (DEBUG) {
          printf("Packet sent.\n");
        }
        break;
      default:
        printf("Error sending packet %d\n", result);
    }

    // Debounce
    while (button_press != 0) {
      button_read(0, &button_press);
    }
  }
}
