#include <ble.h>
#include <stdbool.h>
#include <stdio.h>
#include <tock.h>

/*******************************************************************************
 * MAIN
 ******************************************************************************/

int main(void) {
  int err;
  printf("BLE Advertising\n");

  // declarations of variables to be used in this BLE example application
  uint16_t advertising_interval_ms = 1000;
  static unsigned char data[]      = {
    0x2, // Length of this data
    0x1, // Flags
    0x6, // LE General discoverable + BD/EDR not supported
    0x7, // Length of this data
    0x9, // Complete Local Name
    'T', 'o', 'c', 'k', 'O', 'S',
    0xe, // Length of this data
    0x24, // URI
    0x17, // 'https:'
    '/', '/', 't', 'o', 'c', 'k', 'o', 's', '.', 'o', 'r', 'g'
  };

  // start advertising
  printf(" - Begin advertising!\n");
  err = ble_start_advertising(ADV_NONCONN_IND, data, sizeof(data), advertising_interval_ms);
  if (err < RETURNCODE_SUCCESS)
    printf("ble_start_advertising, error: %s\r\n", tock_strrcode(err));
  return 0;
}
