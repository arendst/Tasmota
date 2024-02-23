#include "nrf51_serialization.h"

// #define NRF51_SERIALIZATION_COMMAND_CHECK 0
#define NRF51_SERIALIZATION_COMMAND_WRITE 1
#define NRF51_SERIALIZATION_COMMAND_READ  2
#define NRF51_SERIALIZATION_COMMAND_RESET 3


int nrf51_serialization_reset (void) {
  // Reset the nRF51 chip
  syscall_return_t cval = command(DRIVER_NUM_NRF_SERIALIZATION,
                                  NRF51_SERIALIZATION_COMMAND_RESET,
                                  0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int nrf51_serialization_subscribe (subscribe_upcall cb) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_NRF_SERIALIZATION, 0, cb, NULL);
  return tock_subscribe_return_to_returncode(sval);
}

int nrf51_serialization_setup_receive_buffer (char* rx, int rx_len) {
  // Pass the RX buffer for the UART module to use.
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_NRF_SERIALIZATION, 0, rx, rx_len);
  return tock_allow_rw_return_to_returncode(aval);
}



int nrf51_serialization_write(char* tx, int tx_len) {
  // Pass in the TX buffer.
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_NRF_SERIALIZATION, 0, tx, tx_len);
  if (!aval.success) return tock_status_to_returncode(aval.status);

  // Write the data.
  syscall_return_t cval = command(DRIVER_NUM_NRF_SERIALIZATION,
                                  NRF51_SERIALIZATION_COMMAND_WRITE,
                                  0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int nrf51_serialization_read(int rx_len) {
  int read_len;
  syscall_return_t cval = command(DRIVER_NUM_NRF_SERIALIZATION,
                                  NRF51_SERIALIZATION_COMMAND_READ, rx_len, 0);
  int ret = tock_command_return_u32_to_returncode(cval, (uint32_t*) &read_len);
  if (ret < 0) return ret;

  // This shouldn't return a length and an error, but it is the signature
  // libnrfserialization expects.
  return read_len; // Actual read length
}
