#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <ble_advdata.h>
#include <nordic_common.h>
#include <nrf_error.h>

#include <eddystone.h>
#include <simple_adv.h>
#include <simple_ble.h>

#include <nrf51_serialization.h>

#include <console.h>
#include <tock.h>

#include "ble_nus.h"
#include "nrf.h"


/*******************************************************************************
 * BLE
 ******************************************************************************/

uint16_t conn_handle = BLE_CONN_HANDLE_INVALID;

// Intervals for advertising and connections
simple_ble_config_t ble_config = {
  .platform_id       = 0x00,                // used as 4th octect in device BLE address
  .device_id         = DEVICE_ID_DEFAULT,
  .adv_name          = "tock-uart",
  .adv_interval      = MSEC_TO_UNITS(500, UNIT_0_625_MS),
  .min_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS),
  .max_conn_interval = MSEC_TO_UNITS(1250, UNIT_1_25_MS)
};

// State for UART library.
static ble_nus_t m_nus;

void ble_address_set (void) {
  // nop
}

void ble_evt_user_handler (ble_evt_t* p_ble_evt) {
  ble_gap_conn_params_t conn_params;
  memset(&conn_params, 0, sizeof(conn_params));
  conn_params.min_conn_interval = ble_config.min_conn_interval;
  conn_params.max_conn_interval = ble_config.max_conn_interval;
  conn_params.slave_latency     = SLAVE_LATENCY;
  conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

  switch (p_ble_evt->header.evt_id) {
    case BLE_GAP_EVT_CONN_PARAM_UPDATE:
      // just update them right now
      sd_ble_gap_conn_param_update(0, &conn_params);
      break;
  }
}

// This gets called with the serial data from the BLE central.
static void nus_data_handler(ble_nus_t* p_nus, uint8_t* p_data, uint16_t length) {
  UNUSED_PARAMETER(p_nus);

  // In this app, just print it to the console.
  putnstr((char*) p_data, length);
}

void ble_evt_connected(ble_evt_t* p_ble_evt) {
  ble_common_evt_t *common = (ble_common_evt_t*) &p_ble_evt->evt;
  conn_handle = common->conn_handle;

  ble_nus_on_ble_evt(&m_nus, p_ble_evt);
}

void ble_evt_disconnected(ble_evt_t* p_ble_evt) {
  conn_handle = BLE_CONN_HANDLE_INVALID;

  ble_nus_on_ble_evt(&m_nus, p_ble_evt);
}

// On a write, need to forward that to NUS library.
void ble_evt_write(ble_evt_t* p_ble_evt) {
  ble_nus_on_ble_evt(&m_nus, p_ble_evt);
}

void ble_error (uint32_t error_code) {
  printf("BLE ERROR: Code = %d\n", (int)error_code);
}

void services_init (void) {
  uint32_t err_code;
  ble_nus_init_t nus_init;
  memset(&nus_init, 0, sizeof(nus_init));
  nus_init.data_handler = nus_data_handler;
  err_code = ble_nus_init(&m_nus, &nus_init);
  APP_ERROR_CHECK(err_code);
}


/*******************************************************************************
 * MAIN
 ******************************************************************************/

int main (void) {
  printf("[BLE] UART over BLE\n");

  // Setup BLE
  conn_handle = simple_ble_init(&ble_config)->conn_handle;

  // Advertise the UART service
  ble_uuid_t adv_uuid = {0x0001, BLE_UUID_TYPE_VENDOR_BEGIN};
  simple_adv_service(&adv_uuid);
}
