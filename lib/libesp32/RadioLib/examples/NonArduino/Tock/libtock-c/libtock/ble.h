#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 *   DRIVER DEFINITIONS   -- corresponds to different system calls
 *
 *    *_CMD   - command call
 *    *_ALLOW - allow call
 *    *_SUB   - subscribe call
 *
 ******************************************************************************/

#define BLE_DRIVER_NUMBER 0x30000

#define BLE_ADV_START_CMD 0
#define BLE_ADV_STOP_CMD 1
#define BLE_CFG_TX_POWER_CMD 2
#define BLE_SCAN_CMD 5

#define BLE_SCAN_SUB 0

#define BLE_CFG_ADV_BUF_ALLOWRO 0
#define BLE_CFG_SCAN_BUF_ALLOWRW 0

#define ADV_IND  0x00
#define ADV_DIRECT_IND  0x01
#define ADV_NONCONN_IND  0x02
#define ADV_SCAN_IND  0x06 

typedef enum {
  POSITIVE_10_DBM = 10,
  POSITIVE_9_DBM = 9,
  POSITIVE_8_DBM = 8,
  POSITIVE_7_DBM = 7,
  POSITIVE_6_DBM = 6,
  POSITIVE_5_DBM = 5,
  POSITIVE_4_DBM = 4,
  POSITIVE_3_DBM = 3,
  POSITIVE_2_DBM = 2,
  POSITIVE_1_DBM = 1,
  ZERO_DBM = 0,
  NEGATIVE_1_DBM = 0xff,
  NEGATIVE_2_DBM = 0xfe,
  NEGATIVE_3_DBM = 0xfd,
  NEGATIVE_4_DBM = 0xfc,
  NEGATIVE_5_DBM = 0xfb,
  NEGATIVE_6_DBM = 0xfa,
  NEGATIVE_7_DBM = 0xf9,
  NEGATIVE_8_DBM = 0xf8,
  NEGATIVE_9_DBM = 0xf7,
  NEGATIVE_10_DBM = 0xf6,
  NEGATIVE_11_DBM = 0xf5,
  NEGATIVE_12_DBM = 0xf4,
  NEGATIVE_13_DBM = 0xf3,
  NEGATIVE_14_DBM = 0xf2,
  NEGATIVE_15_DBM = 0xf1,
  NEGATIVE_16_DBM = 0xf0,
  NEGATIVE_17_DBM = 0xef,
  NEGATIVE_18_DBM = 0xee,
  NEGATIVE_19_DBM = 0xed,
  NEGATIVE_20_DBM = 0xec,
} TxPower_t;

// start advertising
//
// pdu_type           - Type of advertising PDU. One of ADV_IND, ADV_NONCONN_IND or ADV_SCAN_IND
// advd               - The advertising data
// len                - Length of the advertising data (will be truncated to 31 bytes)
// interval           - The advertising interval in milliseconds
int ble_start_advertising(int pdu_type, uint8_t* advd, int len, uint16_t interval);

// stop advertising but don't change anything in the packet configuration
int ble_stop_advertising(void);

// passive scanning of advertisements
//
// data                 - array of bytes to write the received advertisment to
// len                  - max_size (39 bytes)
// callback             - callback handler to call when an advertisement is
//                        received
//
// type signature of the callback handler:
// static void callback(int result,
//                      int len,
//                      __attribute__((unused)) int unused2,
//                      __attribute__((unused)) void* ud);
//
// The kernel will fill the array of bytes with the received advertisement
// it's then up to user-space application to determine what to do in the
// callback handler.
//
// result               - kernel indicates whether the radio rx was successful
//                        or not
// len                  - the number of bytes received via the radio
//
int ble_start_passive_scan(uint8_t *data, uint8_t len, subscribe_upcall callback);

// stop passive scanning
int ble_stop_passive_scan(void);

// configure tx_power
//
// power_level          - transmitting power in dBM of the radio
//                        according to Bluetooth 4.2 (-20 dBm to 10 dBm)
//
int ble_set_tx_power(TxPower_t power_level);

#ifdef __cplusplus
}
#endif
