#include <ble.h>
#include <gap.h>
#include <stdbool.h>
#include <stdio.h>
#include <tock.h>

// Sizes in bytes
#define DEVICE_NAME_SIZE 6
#define UUIDS_SIZE 4
#define MANUFACTURER_DATA_SIZE 2
#define FAKE_TEMPERATURE_DATA_SIZE 2

/*******************************************************************************
 * MAIN
 ******************************************************************************/

int main(void) {
  int err;
  printf("[Tutorial] BLE Advertising\n");

  // declarations of variables to be used in this BLE example application
  uint16_t advertising_interval_ms = 300;
  uint8_t device_name[]           = "TockOS";
  uint16_t uuids[]                = {0x1800, 0x1809};
  uint8_t manufacturer_data[]     = {0x13, 0x37};
  uint8_t fake_temperature_data[] = {0x00, 0x00};

  static uint8_t adv_data_buf[ADV_DATA_MAX_SIZE];

  // configure advertisement interval to 300ms
  // configure LE only and discoverable
  printf(" - Initializing BLE... %s\n", device_name);
  AdvData_t adv_data = gap_adv_data_new(adv_data_buf, sizeof(adv_data_buf));

  gap_add_flags(&adv_data, LE_GENERAL_DISCOVERABLE | BREDR_NOT_SUPPORTED);

  // configure device name as TockOS
  printf(" - Setting the device name... %s\n", device_name);
  err = gap_add_device_name(&adv_data, device_name, DEVICE_NAME_SIZE);
  if (err < RETURNCODE_SUCCESS)
    printf("ble_advertise_name, error: %s\r\n", tock_strrcode(err));

  // configure list of UUIDs */
  printf(" - Setting the device UUID...\n");
  err = gap_add_service_uuid16(&adv_data, uuids, UUIDS_SIZE);
  if (err < RETURNCODE_SUCCESS)
    printf("ble_advertise_uuid16, error: %s\r\n", tock_strrcode(err));

  // configure manufacturer data
  printf(" - Setting manufacturer data...\n");
  err = gap_add_manufacturer_specific_data(&adv_data, manufacturer_data,
                                           MANUFACTURER_DATA_SIZE);
  if (err < RETURNCODE_SUCCESS)
    printf("ble_advertise_manufacturer_specific_data, error: %s\r\n",
           tock_strrcode(err));

  // configure service data
  printf(" - Setting service data...\n");
  err = gap_add_service_data(&adv_data, uuids[1], fake_temperature_data,
                             FAKE_TEMPERATURE_DATA_SIZE);
  if (err < RETURNCODE_SUCCESS)
    printf("ble_advertise_service_data, error: %s\r\n", tock_strrcode(err));

  // start advertising
  printf(" - Begin advertising! %s\n", device_name);
  err = ble_start_advertising(ADV_NONCONN_IND, adv_data.buf, adv_data.offset, advertising_interval_ms);
  if (err < RETURNCODE_SUCCESS)
    printf("ble_start_advertising, error: %s\r\n", tock_strrcode(err));

  // configuration complete
  printf("Now advertising every %d ms as '%s'\n", advertising_interval_ms,
         device_name);
  return 0;
}
