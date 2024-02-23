#include <stdint.h>
#include <string.h>

#include <app_error.h>
#include <ble.h>

#include "env_sense_service.h"

static uint16_t service_handle;
static ble_gatts_char_handles_t temp_char_handle;
static ble_gatts_char_handles_t irradiance_char_handle;
static ble_gatts_char_handles_t humidity_char_handle;

static int16_t temperature = 0;
static uint16_t irradiance = 0;
static uint16_t humidity   = 0;

static void add_char(uint16_t uuid, uint8_t* value, ble_gatts_char_handles_t* handle) {
  uint32_t err_code;
  ble_gatts_char_md_t char_md;
  ble_gatts_attr_t attr_char_value;
  ble_uuid_t char_uuid;
  ble_gatts_attr_md_t attr_md;

  // set characteristic metadata
  memset(&char_md, 0, sizeof(char_md));
  char_md.char_props.read   = true;
  char_md.char_props.notify = true;

  // set characteristic uuid
  char_uuid.type = BLE_UUID_TYPE_BLE;
  char_uuid.uuid = uuid;

  // set attribute metadata
  memset(&attr_md, 0, sizeof(attr_md));
  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
  attr_md.vloc = BLE_GATTS_VLOC_STACK;

  // set attribute data
  memset(&attr_char_value, 0, sizeof(attr_char_value));
  attr_char_value.p_uuid    = &char_uuid;
  attr_char_value.p_attr_md = &attr_md;
  attr_char_value.init_len  = 2;
  attr_char_value.init_offs = 0;
  attr_char_value.max_len   = 2;
  attr_char_value.p_value   = value;

  err_code = sd_ble_gatts_characteristic_add(service_handle,
                                             &char_md,
                                             &attr_char_value,
                                             handle);
  APP_ERROR_CHECK(err_code);
}

void env_sense_service_init(void) {
  ble_uuid_t uuid = {
    .uuid = ENVIRONMENTAL_SENSING_SERVICE_UUID,
    .type = BLE_UUID_TYPE_BLE
  };

  uint32_t err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                               &uuid,
                                               &service_handle);
  APP_ERROR_CHECK(err_code);

  add_char(TEMPERATURE_CHAR_UUID, (uint8_t*) &temperature, &temp_char_handle);
  add_char(IRRADIANCE_CHAR_UUID, (uint8_t*)&irradiance, &irradiance_char_handle);
  add_char(HUMIDITY_CHAR_UUID, (uint8_t*) &humidity, &humidity_char_handle);
}

static uint32_t notify(uint16_t conn, uint16_t handle) {
  uint32_t err_code;
  ble_gatts_hvx_params_t hvx_params;
  hvx_params.handle = handle;
  hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
  hvx_params.offset = 0;
  hvx_params.p_len  = NULL; // notify full length. No response wanted
  hvx_params.p_data = NULL; // use existing value

  err_code = sd_ble_gatts_hvx(conn, &hvx_params);
  if (err_code == NRF_ERROR_INVALID_STATE) {
    // error means notify is not enabled by the client. IGNORE
    return NRF_SUCCESS;
  }
  return err_code;
}

static uint32_t env_sense_update(uint16_t conn, uint8_t* new_value, uint16_t handle) {
  uint32_t err_code;

  ble_gatts_value_t value = {
    .len     = 2,
    .offset  = 0,
    .p_value = new_value,
  };
  err_code = sd_ble_gatts_value_set(BLE_CONN_HANDLE_INVALID, handle, &value);
  if (err_code != NRF_SUCCESS) return err_code;

  return notify(conn, handle);
}

uint32_t env_sense_update_irradiance(uint16_t conn, uint16_t new_irradiance) {
  return env_sense_update(conn, (uint8_t*) &new_irradiance, irradiance_char_handle.value_handle);
}

uint32_t env_sense_update_temperature(uint16_t conn, int16_t new_temperature) {
  return env_sense_update(conn, (uint8_t*) &new_temperature, temp_char_handle.value_handle);
}

uint32_t env_sense_update_humidity(uint16_t conn, uint16_t new_humidity) {
  return env_sense_update(conn, (uint8_t*) &new_humidity, humidity_char_handle.value_handle);
}
