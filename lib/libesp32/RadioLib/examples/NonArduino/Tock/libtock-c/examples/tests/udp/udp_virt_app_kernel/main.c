#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <button.h>
#include <timer.h>

#include <udp.h>

#define DEBUG 0

static unsigned char BUF_BIND_CFG[2 * sizeof(sock_addr_t)];

void print_ipv6(ipv6_addr_t *);

int main(void) {

  printf("[UDP VIRT] Starting Kernel Coop UDP Test App.\n");

  static char packet[70];
  ipv6_addr_t dest_addr = {
    {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b,
      0x1c, 0x1d, 0x1e, 0x1f}
  };
  sock_addr_t destination = {
    dest_addr,
    16123
  };

  ipv6_addr_t ifaces[10];
  udp_list_ifaces(ifaces, 10);

  sock_handle_t handle;
  sock_addr_t addr = {
    ifaces[0],
    1000 // kernel tries to bind this after me
  };

  int len        = snprintf(packet, sizeof(packet), "Hello World - App/Kernel virt\n");
  ssize_t result = udp_send_to(packet, len, &destination);
  assert(result < 0); // should fail because we have not bound

  if (DEBUG) {
    printf("Opening socket on ");
    print_ipv6(&ifaces[0]);
    printf(" : %d\n", addr.port);
  }
  int bind_return = udp_bind(&handle, &addr, BUF_BIND_CFG);
  assert(bind_return >= 0); // bind should succeed

  delay_ms(3000); // resync with kernel

  sock_addr_t addr2 = {
    ifaces[0],
    1001 // same as what kernel has successfuly bound to
  };
  bind_return = udp_bind(&handle, &addr2, BUF_BIND_CFG);
  assert(bind_return < 0); // bind should fail bc kernel has bound this
  sock_addr_t addr3 = {
    ifaces[0],
    1002 // new port
  };
  bind_return = udp_bind(&handle, &addr3, BUF_BIND_CFG);
  assert(bind_return >= 0);

  delay_ms(1000); // should be synced with kernel test 2 starting now.

  if (DEBUG) {
    printf("Sending packet (length %d) --> ", len);
    print_ipv6(&(destination.addr));
    printf(" : %d\n", destination.port);
  }
  result = udp_send_to(packet, len, &destination);
  assert(result == RETURNCODE_SUCCESS); // send should succeed

  printf("App part of app/kernel test successful!\n");
}

void print_ipv6(ipv6_addr_t *ipv6_addr) {
  for (int j = 0; j < 14; j += 2) {
    printf("%02x%02x:", ipv6_addr->addr[j], ipv6_addr->addr[j + 1]);
  }
  printf("%02x%02x", ipv6_addr->addr[14], ipv6_addr->addr[15]);
}
