#include <gap.h>
#include <rng.h>
#include <stdbool.h>
#include <stdio.h>
#include <timer.h>
#include <tock.h>

int test_off_by_one_name(void);
int test_off_by_one_service_data(void);
int test_exactly_full_buffer(void);
int test_exactly_full_buffer_service_data(void);

static uint8_t buf[ADV_DATA_MAX_SIZE];

/*******************************************************************************
 * MAIN
 ******************************************************************************/
int main(void) {
  int err;
  printf("[Test] Bluetooth Low Energy Gap Library\r\n");

  err = test_off_by_one_name();
  if (err == RETURNCODE_SUCCESS) {
    printf("test_off_by_one_name failed: %s\r\n", tock_strrcode(err));
    return err;
  }

  err = test_off_by_one_service_data();
  if (err == RETURNCODE_SUCCESS) {
    printf("test_off_by_one_service_data failed: %s\r\n", tock_strrcode(err));
    return err;
  }

  err = test_exactly_full_buffer_service_data();
  if (err != RETURNCODE_SUCCESS) {
    printf("test_exactly_full_buffer_service_data failed: %s\r\n", tock_strrcode(err));
    return err;
  }

  err = test_exactly_full_buffer();
  if (err != RETURNCODE_SUCCESS) {
    printf("test_exactly_full_buffer failed: %s\r\n", tock_strrcode(err));
    return err;
  }

  printf("TEST PASSED\r\n");
  return 0;
}

/*******************************************************************************
 * TESTS
 ******************************************************************************/

// GAP_COMPLETE_LOCAL_NAME || Len || Name
//
// Total 32 bytes
int test_off_by_one_name(void) {
  unsigned char device_name[] = "TockTockTockTockTockTockTockTo";

  AdvData_t adv_data = gap_adv_data_new(buf, sizeof(buf));
  return gap_add_device_name(&adv_data, device_name, sizeof(device_name) - 1);
}

// Len || GAP_COMPLETE_LIST_16BIT_SERVICE_IDS || UUID16 (4 bytes)
// Len || GAP_SERVICE_DATA || Service Data (28 bytes)
//
// Total 32 bytes
int test_off_by_one_service_data(void) {
  unsigned char data[] = "TockTockTockTockTockTockTo";

  AdvData_t adv_data = gap_adv_data_new(buf, sizeof(buf));
  return gap_add_service_data(&adv_data, 0x1801, data, sizeof(data) - 1);
}

// Len || GAP_COMPLETE_LIST_16BIT_SERVICE_IDS || UUID16 (4 bytes)
// Len || GAP_SERVICE_DATA || Service Data (27 bytes)
//
// Total 31 bytes
int test_exactly_full_buffer_service_data(void) {
  unsigned char data[] = "TockTockTockTockTockTockT";

  AdvData_t adv_data = gap_adv_data_new(buf, sizeof(buf));
  return gap_add_service_data(&adv_data, 0x1801, data, sizeof(data) - 1);
}

// GAP_COMPLETE_LOCAL_NAME || Len || Name
//
// Total 31 bytes
int test_exactly_full_buffer(void) {
  unsigned char device_name[] = "TockTockTockTockTockTockTockT";

  AdvData_t adv_data = gap_adv_data_new(buf, sizeof(buf));
  return gap_add_device_name(&adv_data, device_name, sizeof(device_name) - 1);
}
