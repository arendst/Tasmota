#include <ble.h>
#include <rng.h>
#include <stdbool.h>
#include <stdio.h>
#include <timer.h>
#include <tock.h>

#define NRF5X_POS_4_DBM   0x04
#define NRF5X_POS_3_DBM   0x03
#define NRF5X_0_DBM       0x00
#define NRF5X_NEG_4_DBM   0xfc
#define NRF5X_NEG_8_DBM   0xf8
#define NRF5X_NEG_12_DBM  0xf4
#define NRF5X_NEG_16_DBM  0xf0
#define NRF5X_NEG_20_DBM  0xec

bool is_valid_power(uint8_t power);

int main(void) {
  int err;
  printf("[NRF5X Test] BLE TxPower\n");

  for (uint16_t i = 0; i <= 0xff; ++i) {
    bool is_valid = is_valid_power(i);

    if (is_valid) {
      err = ble_set_tx_power(i);
      if (err < RETURNCODE_SUCCESS) {
        printf("Test failed \t power_level %d could not be configured\r\n", i);
        return 0;
      }
    } else {
      err = ble_set_tx_power(i);
      if (err == RETURNCODE_SUCCESS) {
        printf("Test failed \t power_level %d is configured faulty\r\n", i);
        return 0;
      }
    }
  }

  printf("Test passed!!\r\n");
  return 0;
}

bool is_valid_power(uint8_t power) {
  switch (power) {
    case NRF5X_POS_4_DBM:
      return true;
    case NRF5X_POS_3_DBM:
      return true;
    case NRF5X_0_DBM:
      return true;
    case NRF5X_NEG_4_DBM:
      return true;
    case NRF5X_NEG_8_DBM:
      return true;
    case NRF5X_NEG_12_DBM:
      return true;
    case NRF5X_NEG_16_DBM:
      return true;
    case NRF5X_NEG_20_DBM:
      return true;
    default:
      return false;
  }
}
