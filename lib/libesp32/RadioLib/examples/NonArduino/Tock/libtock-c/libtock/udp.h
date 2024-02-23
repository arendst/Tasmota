#pragma once

#include <stddef.h>
#include <sys/types.h>

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint16_t udp_port_t;

typedef struct ipv6_addr {
  uint8_t addr[16];
} ipv6_addr_t;

typedef struct sock_addr {
  ipv6_addr_t addr;
  udp_port_t port;
} sock_addr_t;

typedef struct sock_handle {
  sock_addr_t addr;
} sock_handle_t;

// Creates a new datagram socket bound to an address.
// Returns 0 on success, negative on failure.
int udp_socket(sock_handle_t *handle, sock_addr_t *addr);

// Takes in an addess and a handle, and copies the address into
// the handle.
// Next, Binds to the address in handle if the address is not already
// bound by another port. Binding enables an app to receive on
// the bound port, and ensures that all sent packets will have
// the bound port as their src address.
// Returns 0 on successful bind, negative on failure.
int udp_bind(sock_handle_t *handle, sock_addr_t *addr, unsigned char *buf_bind_cfg);

// Closes a socket.
// Currently only one socket can exist per app, so this fn
// simply closes any connected socket
// Returns 0 on success, negative on failure.
int udp_close(sock_handle_t *handle);

// Sends data on a socket. Requires binding to a port first.
// Returns 0 on success, negative on failure.
ssize_t udp_send_to(void *buf, size_t len, sock_addr_t *dst_addr);

// Receives messages from that socket asynchronously.
// The callback is passed the return code for the reception.
// Returns 0 on successful bind, negative on failure.
ssize_t udp_recv(subscribe_upcall callback, void *buf, size_t len);

// Receives messages from the bound socket synchronously.
// Returns 0 on successful reception, negative on failure.
// Hangs until reception occurs unless there is a failure during a bind or reception.
ssize_t udp_recv_sync(void *buf, size_t len);

// Lists `len` interfaces at the array pointed to by `ifaces`.
// Returns the _total_ number of interfaces, negative on failure.
int udp_list_ifaces(ipv6_addr_t *ifaces, size_t len);

// Returns the maximum length udp payload that the app can transmit
int udp_get_max_tx_len(int* length);

#ifdef __cplusplus
}
#endif
