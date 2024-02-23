/*
 * BLE setup functions
 */

#include "ble.h"
#include "tock.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int ble_start_advertising(int pdu_type, uint8_t* advd, int len, uint16_t interval) {
  allow_ro_return_t err = allow_readonly(BLE_DRIVER_NUMBER, BLE_CFG_ADV_BUF_ALLOWRO, advd, len);
  if (!err.success) return tock_status_to_returncode(err.status);

  syscall_return_t res = command(BLE_DRIVER_NUMBER, BLE_ADV_START_CMD, pdu_type, interval);
  return tock_command_return_novalue_to_returncode(res);
}

int ble_stop_advertising(void) {
  syscall_return_t res = command(BLE_DRIVER_NUMBER, BLE_ADV_STOP_CMD, 1, 0);
  return tock_command_return_novalue_to_returncode(res);
}

int ble_start_passive_scan(uint8_t *data, uint8_t max_len, subscribe_upcall callback) {
  if (data == NULL || callback == NULL) {
    return RETURNCODE_FAIL;
  } else {

    subscribe_return_t sub_err = subscribe(BLE_DRIVER_NUMBER, BLE_SCAN_SUB, callback, NULL);
    if (!sub_err.success) return tock_status_to_returncode(sub_err.status);

    allow_rw_return_t allow_err =
      allow_readwrite(BLE_DRIVER_NUMBER, BLE_CFG_SCAN_BUF_ALLOWRW, (void *)data, max_len);
    if (!allow_err.success) return tock_status_to_returncode(allow_err.status);

    syscall_return_t res = command(BLE_DRIVER_NUMBER, BLE_SCAN_CMD, 1, 0);
    return tock_command_return_novalue_to_returncode(res);
  }
}

int ble_stop_passive_scan(void) {
  syscall_return_t res = command(BLE_DRIVER_NUMBER, BLE_ADV_STOP_CMD, 1, 0);
  return tock_command_return_novalue_to_returncode(res);
}

int ble_set_tx_power(TxPower_t power_level) {
  syscall_return_t res = command(BLE_DRIVER_NUMBER, BLE_CFG_TX_POWER_CMD, power_level, 0);
  return tock_command_return_novalue_to_returncode(res);
}
