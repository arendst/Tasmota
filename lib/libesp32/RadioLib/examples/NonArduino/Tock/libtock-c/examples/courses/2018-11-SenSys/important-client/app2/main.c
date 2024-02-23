#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <ambient_light.h>
#include <button.h>
#include <humidity.h>
#include <rng.h>
#include <temperature.h>
#include <timer.h>

#include <ieee802154.h>
#include <udp.h>

#define DEBUG 0

static unsigned char BUF_BIND_CFG[2 * sizeof(sock_addr_t)];

void print_ipv6(ipv6_addr_t *);
int serialize_to_json(char* packet, int len, uint32_t rand, int temp, int humi, int lux);

int main(void) {

  printf("[UDP] Starting UDP App.\n");

  static unsigned int humi = 0;
  static int temp = 0;
  static int lux  = 0;
  static char packet[70];

  ieee802154_set_pan(0xABCD);
  ieee802154_config_commit();
  ieee802154_up();

  ipv6_addr_t ifaces[10];
  udp_list_ifaces(ifaces, 10);

  sock_handle_t handle;
  sock_addr_t addr = {
    ifaces[2],
    11111
  };

  printf("Opening socket on ");
  print_ipv6(&ifaces[2]);
  printf(" : %d\n", addr.port);
  int bind_return = udp_bind(&handle, &addr, BUF_BIND_CFG);

  if (bind_return < 0) {
    printf("Bind failed. Error code: %d\n", bind_return);
    return -1;
  }

  // Set the below address to be the IP address of your receiver
  ipv6_addr_t dest_addr = {
    {0xfe, 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xfe, 0, 0xdf, 0xf0}
  };
  sock_addr_t destination = {
    dest_addr,
    16123
  };

  while (1) {
    temperature_read_sync(&temp);
    humidity_read_sync(&humi);
    ambient_light_read_intensity_sync(&lux);

    // get randomness
    uint32_t rand = 0;
    int num_received = 0;
    int err = rng_sync((uint8_t*)&rand, 4, 4, &num_received);
    if (err < 0) {
      printf("Error obtaining random number: %d\n", err);
    } else if (num_received < 4) {
      printf("Only obtained %d bytes of randomness\n", num_received);
    }

    int len = serialize_to_json(packet, sizeof(packet), rand, temp, humi, lux);
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

    delay_ms(5000);
  }
}

void print_ipv6(ipv6_addr_t *ipv6_addr) {
  for (int j = 0; j < 14; j += 2) {
    printf("%02x%02x:", ipv6_addr->addr[j], ipv6_addr->addr[j + 1]);
  }
  printf("%02x%02x", ipv6_addr->addr[14], ipv6_addr->addr[15]);
}

int serialize_to_json(char* buf, int buflen, uint32_t rand, int temp, int humi, int lux) {
  return snprintf(buf, buflen, "{\"rand\": %lu, \"temp\": %d, \"humi\": %d, \"lux\": %d}",
                       rand, temp, humi, lux);
}

