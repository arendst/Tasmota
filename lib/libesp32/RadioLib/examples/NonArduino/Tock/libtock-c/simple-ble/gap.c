#include "include/gap.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

AdvData_t gap_adv_data_new(uint8_t* buf, int len) {
  AdvData_t result;
  result.buf      = buf;
  result.offset   = 0;
  result.capacity = len;
  return result;
}

int gap_add_flags(AdvData_t *adv_data, uint8_t flags) {
  int new_len = adv_data->offset + 3;
  if (new_len <= adv_data->capacity) {
    adv_data->buf[adv_data->offset]     = 2;
    adv_data->buf[adv_data->offset + 1] = GAP_FLAGS;
    adv_data->buf[adv_data->offset + 2] = flags;
    adv_data->offset = new_len;
    // always writes 3 bytes
    return 3;
  } else {
    return -1;
  }
}

// internal helper to configure gap data in the advertisement
//
// header - gap data header
// data   - buffer of data configure in the advertisement
// len    - length of data buffer
int gap_add_adv_data_field(AdvData_t *adv_data, GapAdvertisementData_t type,
                           const uint8_t *data, uint8_t data_len) {
  // make room for gap data header: length and gap_type
  uint8_t new_length = 2 + data_len + adv_data->offset;
  if (new_length > adv_data->capacity) {
    return -1;
  }
  adv_data->buf[adv_data->offset]     = data_len + 1;
  adv_data->buf[adv_data->offset + 1] = type;
  memcpy(&adv_data->buf[adv_data->offset + 2], data, data_len);
  adv_data->offset = new_length;
  return 2 + data_len;
}

int gap_add_device_name(AdvData_t *adv_data, const uint8_t *device_name, uint8_t len) {
  if (device_name == NULL) {
    return -1;
  } else {
    return gap_add_adv_data_field(adv_data, GAP_COMPLETE_LOCAL_NAME, device_name,
                                  len);
  }
}

int gap_add_service_uuid16(AdvData_t *adv_data, const uint16_t *uuid16, uint8_t len) {
  if (uuid16 == NULL) {
    return -1;
  } else {
    return gap_add_adv_data_field(adv_data, GAP_COMPLETE_LIST_16BIT_SERVICE_IDS,
                                  (uint8_t *)uuid16, len);
  }
}

int gap_add_service_data(AdvData_t *adv_data, uint16_t uuid16, uint8_t *data, uint8_t data_len) {
  int err = gap_add_service_uuid16(adv_data, &uuid16, 2);
  if (err < 0) return err;
  return gap_add_adv_data_field(adv_data, GAP_SERVICE_DATA, data, data_len);
}

int gap_add_manufacturer_specific_data(AdvData_t *adv_data, uint8_t *data, uint8_t size_b) {
  if (data == NULL) {
    return -1;
  } else {
    return gap_add_adv_data_field(adv_data, GAP_MANUFACTURER_SPECIFIC_DATA, data,
                                  size_b);
  }
}
