#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_NRF_SERIALIZATION 0x80004

// Toggle the reset line to the nRF51 chip to reset the BLE MCU.
__attribute__ ((warn_unused_result))
int nrf51_serialization_reset (void);

// Give the BLE Serialization / UART layer a callback to call when
// a packet is received and when a TX is finished.
__attribute__ ((warn_unused_result))
int nrf51_serialization_subscribe (subscribe_upcall cb);

// Pass a buffer for the driver to write received UART bytes to.
__attribute__ ((warn_unused_result))
int nrf51_serialization_setup_receive_buffer (char* rx, int rx_len);

// Write a packet to the BLE Serialization connectivity processor.
__attribute__ ((warn_unused_result))
int nrf51_serialization_write(char* tx, int tx_len);

// Receive into the buffer passed in to `setup_receive_buffer` previously
__attribute__ ((warn_unused_result))
int nrf51_serialization_read(int rx_len);

  
#ifdef __cplusplus
}
#endif
